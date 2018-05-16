/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2013-2017 NXP
* All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION
#include "hog_apu.h"
#include "hog_config.h"
#include <string.h>

//#define APU_HOG_DEBUG

#ifdef APU_HOG_DEBUG
#include <stdio.h>
#include "apu_io.h"
#endif


typedef unsigned int uint;

/******** Fixed Point Constants ***********************************/
const uint DESC_Q          = 8;
const uint HIST_Q          = 10;
const uint SVM_Q           = 4;
const uint TWO_HIST_Q      = HIST_Q << 1;
const uint HIST_TO_DESC_Q  = TWO_HIST_Q - DESC_Q;
const int32u EPSILON       = 209715;

inline int wrapID(int id, int N)
{
   if(id>=N)
   {
      id -= N;
   }
   return id;
}

inline bool powerOfTwo(int x)
{
  return (x & (x - 1)) == 0;
}

inline int modPowerOfTwo(int a, int b)
{
  return a & (b - 1);
}

inline int mod(int a, int b)
{
  return powerOfTwo(b) ? modPowerOfTwo(a,b) : a%b;
}

inline int divShiftPowerOfTwo(unsigned int x)
{
   int n;

   if(x == 0) return(0);
   n = 31;
   if(x <= 0x0000FFFF) {n = n -16; x = x <<16;}
   if(x <= 0x00FFFFFF) {n = n - 8; x = x << 8;}
   if(x <= 0x0FFFFFFF) {n = n - 4; x = x << 4;}
   if(x <= 0x3FFFFFFF) {n = n - 2; x = x << 2;}
   if(x <= 0x7FFFFFFF) {n = n - 1;}
   return n;
}

inline void fxInvSqrt(vec16u& y, vec16u& R, vec32u x, vec16u Q)
{
   const vec16u P = 15;

   vec16s n;
   vec32u z = x;

   vif(z > 65535)
   {
      z >>= 16;
      n = 16;
   }
   velse
   {
      n = 0;
   }
   vendif

   vec16u z_ = (vec16u)z;
   n += 16 - vclz(z_);

   vec16s TWO_K = (vec16s)Q-n;
   TWO_K += (TWO_K & 1);
   const vec16u M = (vec16u)((vec16s)Q - TWO_K);

   vif(P >= M)
   {
      x <<= (vec32u)(P-M);
   }
   velse
   {
      x >>= (vec32u)(M-P);
   }
   vendif

   vec16u x_        = (vec16u)x;
   vec16u half_x    = x_ >> (vec16u)1;
   vec16u quarter_x = x_ >> (vec16u)2;

   vif(x_ < 25494)
   {
      y = (vec16u)61928 - x_;            // 0.5   <= x < 0.778
   }
   velse
   {
      vif(x_ < 40632)
      {
         y = (vec16u)49152 - half_x;     // 0.778 <= x < 1.24
      }
      velse
      {
         y = (vec16u)39012- quarter_x;   // 1.24  <= x < 2.0
      }
      vendif
   }
   vendif

   vif(TWO_K < 0)
   {
      y >>= - (TWO_K >> 1);
      R = P;
   }
   velse
   {
      R = P - (TWO_K >> 1);
   }
   vendif
}

inline vec08u getAngleBin(vec16s x, vec16s y)
{
   const vec16s s_num = 15;
   const vec16s s_den = 35;

   // rotate vector forward by PI/16
   const vec16s c_den = 5;

   vec16s tmp = c_den*x - y;
   y *= c_den;
   y += x;
   x = tmp;

   y >>= 1;
   x >>= 1;

   vec16s u, v;

   vif(x >= 0)
   {
      u = x;
   }
   velse
   {
      u = -x;
      y = -y;
   }
   vendif

   vec08u id;

   vif(y >= 0)
   {
      v = y;
      id = 0;
   }
   velse
   {
      v = u;
      u = -y;
      id = 4;
   }
   vendif

   vif(v >= u)
   {
      id += 2;
      tmp = -v;
      v = -u;
      u = tmp;
   }
   vendif

   vif((s_num * u) <= (s_den * v))
   {
      id += 1;
   }
   vendif

   // Zero cases
   vif(x == 0)
   {
      id = 4;
   }
   vendif

   vif(y == 0)
   {
      id = 0;
   }
   vendif

   return id;
}

void normalizeL2(vec08u* normalizedHist, const vec16u* hist, int16s histSize)
{
   // L2-Normalization of the block histogram using a quasi-floating point method

   vec32u sum = EPSILON;

   for(int16s i=0; i<histSize; i++) chess_loop_range(1,)
   {
      vec32u tmp = (vec32u)hist[i];
      sum += tmp * tmp;
   }

   vec16u scale;
   vec16u R;
   fxInvSqrt(scale, R, sum, TWO_HIST_Q);

   vif(HIST_Q > R)
   {
      scale <<= (HIST_Q-R);
   }
   velse
   {
      scale >>= (R-HIST_Q);
   }

   vec32u scale32 = (vec32u)scale;

   for(int i=0; i<histSize; i++) chess_loop_range(1,)
   {
      normalizedHist[i] = (vec08u)(((vec32u)hist[i] * scale32) >> HIST_TO_DESC_Q);
   }
}

void HOG_convertFunction(int08s* dst, double* src, int size)
{
   for(int i = 0; i < size-2; ++i) {
      dst[i] = (int08s)(src[i] * 128 - 0.5);
   }

   int16s offset =  (int16s)(src[size - 2] * 2048 - 0.5);
   memcpy(&dst[size - 2], &offset, 2);
}

/*!*********************************************************************************
*  \brief Create the lookup tables for block histogram management and score calculation.
*         See documentation for more information.
***********************************************************************************/
void HOG_createLUTs(int08u* xOffsetLUT          /*!< The x-axis LUT for the detection window */,
                    int08u* rotateLUT           /*!< The rotate LUT for score transfer across CU boundaries */,
                    int08u* yOffsetLUT          /*!< The y-axis LUT for the detection window */,
                    int stridesPerChunkX        /*!< Number of detection window strides per CU on the X axis */,
                    int stridesPerBlockX        /*!< Number of detection window strides per block width */,
                    int stridesPerBlockY        /*!< Number of detection window strides per block height */,
                    int numBlocksX              /*!< Number of blocks that make up the width of a detection window */,
                    int numBlocksY              /*!< Number of blocks that make up the height of a detection window */)
{
   /* We start with the last column of block histograms for the respective
   *  detection window. Its location in the CU array depends on how many
   *  block histograms are stored per CU. With stridesPerBlockX = 2 and numBlocksX = 8,
   *  our detection window of interest spans 15 block histograms. We modulo the index
   *  of this last block histogram column (index = 14) with the number of block histograms per CU
   *  (stridesPerChunkX) to obtain the index of our block histogram relative to the CU it
   *  is in. Note that this index does not tell us which CU the histogram is in. */
   if(powerOfTwo(stridesPerChunkX))
   {
       for(int x = 0, j = 0; x < stridesPerChunkX; ++x)
       {
          for(int k = numBlocksX - 1; k >= 0; --k, ++j)
          {
             xOffsetLUT[j] = (int08u)modPowerOfTwo(x + k * stridesPerBlockX, stridesPerChunkX);
          }
       }
   }
   else
   {
       for(int x = 0, j = 0; x < stridesPerChunkX; ++x)
       {
          for(int k = numBlocksX - 1; k >= 0; --k, ++j)
          {
             xOffsetLUT[j] = (int08u)((x + k * stridesPerBlockX) % stridesPerChunkX);
          }
       }
   }

   /* The rotate LUT is related to the xOffset LUT. The xOffset LUT is used to multiply
   *  a block histogram column with the SVM function by starting from the right most histogram column
   *  in the detection window of interest. Since the block histograms of the detection window
   *  will span multiple CUs and the xOffset LUT is only used as an index relative to the current CU,
   *  as the column multiplications cross over CUs the current score must also be moved to the new CU.
   *  The rotateLUT is a table of flags to indicate if this move needs to be done or not. It is checked
   *  before every column multiplication. */
   for(int h = 0; h < stridesPerChunkX; ++h)
   {
      rotateLUT[h] = (h >= (stridesPerChunkX-stridesPerBlockX));
   }

   /* The Y offset LUT manages access to a circular buffer containing the block histograms of a
   *  at least one detection window. Histograms for seperate windows are interleaved in memory
   *  based on the block height and detection window stride in the Y axis. For example, when
   *  the detection window stride Y is half the height of a block, every other block histogram will
   *  belong to a detection window. */
   for(int y = 0; y < stridesPerBlockY * numBlocksY; ++y)
   {
      if(powerOfTwo(stridesPerChunkX))
      {
         yOffsetLUT[y] = modPowerOfTwo(y + stridesPerBlockY, stridesPerBlockY * numBlocksY);
      }
      else
      {
         yOffsetLUT[y] = (y + stridesPerBlockY) % (stridesPerBlockY * numBlocksY);
      }
   }
}

/*!*********************************************************************************
*  \brief Compute the gradient angle and respective histogram bin as well as magnitude
***********************************************************************************/
void HOG_getGradient(vec08u* gradMag            /*!< The output gradient magnitude */,
                     int mStride                /*!< The stride of the magnitude buffer */,
                     vec08u* gradBin            /*!< The output gradient bin */,
                     int bStride                /*!< The stride of the bin buffer */,
                     const vec08u* image        /*!< The input image */,
                     int iStride                /*!< The stride of the input buffer */,
                     int rows                   /*!< The height of the input buffer */,
                     int cols                   /*!< The width of the input buffer */)
{
   for(int y=0; y<rows; ++y, image += iStride, gradMag += mStride, gradBin += bStride) chess_loop_range(1,)
   {
      const vec08u* iPtr = image;
      for(int x=0; x<cols; ++x, ++iPtr) chess_loop_range(1,)
      {
         // Get the derivatives
         const vec16s dx = (vec16s)iPtr[1]       - (vec16s)iPtr[-1];
         const vec16s dy = (vec16s)iPtr[iStride] - (vec16s)iPtr[-iStride];

         // Get the angle bin
         gradBin[x] = getAngleBin(dx, dy);

         // Compute the L1 norm, with clipping at 255
         vec16s t = (vec16s)vabs(dx) + vabs(dy);
         gradMag[x] = (vec08u)vsat(t, 0, 255);
      }
   }
}

/*!*********************************************************************************
*  \brief Compute the cell histogram of oriented gradients.
***********************************************************************************/
void HOG_getCellHistograms(vec16u* cellHistograms       /*!< The sub-block histogram buffer. */,
                           int cStride                  /*!< The sub-block histogram stride.*/,
                           const vec08u* gradMag        /*!< The gradient magnitude image. */,
                           int mStride                  /*!< The gradient magnitude stride. */,
                           const vec08u* gradBin        /*!< The gradient angle bin image. */,
                           int bStride                  /*!< The gradient angle bin stride. */,
                           int chunkWidth               /*!< The number of gradient image columns. */,
                           int chunkHeight              /*!< The number of gradient image rows. */,
                           int windowStrideX            /*!< The horizontal detection window stride. */,
                           int windowStrideY            /*!< The vertical detection window stride. */)
{

   for(int y = 0; y < chunkHeight; y += windowStrideY, cellHistograms += cStride) chess_loop_range(1,)
   {
      vec16u* c = cellHistograms;

      for(int x = 0; x < chunkWidth; x += windowStrideX, c += N_BINS) chess_loop_range(1,)
      {
         //Initialize the cell histogram to zero
         for(int i = 0; i < N_BINS; ++i) chess_loop_range(1,)
         {
            c[i] = 0;
         }

         //Compute the cell histogram
         vec08u* m = (vec08u*)gradMag + y * mStride + x;
         vec08u* b = (vec08u*)gradBin + y * bStride + x;

         for(int cy = 0; cy < windowStrideY; ++cy, m += mStride, b += bStride) chess_loop_range(1,) 
         {
            for(int cx = 0; cx < windowStrideX; ++cx) chess_loop_range(1,) 
            {
               vec16u h = vload(c, b[cx]);
               h += (vec16u)m[cx];
               vstore(c, b[cx], h);
            }
         }
      }
   }
}

/*!*********************************************************************************
*  \brief Compute and return the normalized block histogram of cell histograms.
***********************************************************************************/
void HOG_getBlockHistograms(vec08u* blockHistograms      /*!< The block histograms. */,
                            int blockStride              /*!< The stride of the block histogram image. */,
                            vec16u* cellHistograms       /*!< The sub-block histogram buffer. */,
                            int cellStride               /*!< The stride of the sub-block histogram image. */,
                            int chunkWidth               /*!< The number of sub-block histogram image columns. */,
                            int chunkHeight              /*!< The number of sub-block histogram image rows. */,
                            int stridesPerBlockX         /*!< The number of sub-blocks per block. */,
                            int stridesPerBlockY         /*!< The number of sub-blocks per block. */)
{

   vec16u blockHist[N_BINS];

   for(int y = 0; y < chunkHeight; ++y, blockHistograms += blockStride, cellHistograms += cellStride) chess_loop_range(1,)
   {
      vec08u* b = blockHistograms;
      vec16u* c = cellHistograms;

      for(int x = 0; x < chunkWidth; ++x, c+= N_BINS, b += N_BINS) chess_loop_range(1,)
      {

         //Initialize the non-normalized block histogram to zero
         for(int i = 0; i < N_BINS; ++i) chess_loop_range(1,) {
            blockHist[i] = 0;
         }

         //Accumulate sub-histograms into block histogram
         for(int cy = 0; cy < stridesPerBlockY; ++cy) chess_loop_range(1,)
         {
            for(int cx = 0; cx < stridesPerBlockX; ++cx) chess_loop_range(1,)
            {
               vec16u* sub_c = c + cy * cellStride + cx * N_BINS;

               for(int i = 0; i < N_BINS; ++i) chess_loop_range(1,)
               {
                  blockHist[i] += sub_c[i];
               }
            }
         }

         //Normalize the block histogram
         normalizeL2(b, blockHist, N_BINS);
      }
   }
}

// Compute partial SVM score for a single column of blocks
inline vec16s svmSum(const vec08u* blockHistogram, const int08s* svmNormal)
{
   vec32s a = 0;
   vec16s chess_storage(V7) t;

   const vec08u*     b = blockHistogram;
   const int08s*     s = svmNormal;

   vacl(a, t = vmul_ulsl((vec16s)*b++, (vec16s)*s++));
   vacl(a, t = vmul_ulsl((vec16s)*b++, (vec16s)*s++));
   vacl(a, t = vmul_ulsl((vec16s)*b++, (vec16s)*s++));
   vacl(a, t = vmul_ulsl((vec16s)*b++, (vec16s)*s++));
   vacl(a, t = vmul_ulsl((vec16s)*b++, (vec16s)*s++));
   vacl(a, t = vmul_ulsl((vec16s)*b++, (vec16s)*s++));
   vacl(a, t = vmul_ulsl((vec16s)*b++, (vec16s)*s++));
   vacl(a, t = vmul_ulsl((vec16s)*b++, (vec16s)*s++));

   (a >>= SVM_Q);

#if(__chess__)
   return (vec16s)(a.low());
#elif(__llvm__)
   return (vec16s)__builtin_apex_vec32_get_lo(a);
#endif


}

/*!*********************************************************************************
*  \brief Compute the scores of each detection window
***********************************************************************************/
void HOG_getScores(vec16s* scores                  /*!< The SVM scores. */,
                   const vec08u* histograms        /*!< The block histograms. */,
                   int chunkWidth                  /*!< The block histogram width. */,
                   int chunkHeight                 /*!< The block histogram height. */,
                   int chunkSpan                   /*!< The block histogram stride. */,
                   const int08s* svmFunction       /*!< The SVM decision function. */,
                   const int08u* xOffsetLUT        /*!< The x offset lookup table used to compute the SVM score. */,
                   const int08u* yOffsetLUT        /*!< The x offset lookup table used to compute the SVM score. */,
                   const int08u* rotateLUT         /*!< The vector rotate lookup table used to compute the SVM score. */,
                   int histogramRow                /*!< The current block histogram row. */,
                   int stridesPerBlockY            /*!< The number of horizontal detection window strides per chunk. */,
                   int numBlocksX                  /*!< Number of blocks that make up the width of a detection window */,
                   int numBlocksY                  /*!< Number of blocks that make up the width of a detection window */,
                   int svmSize                     /*!< The size in bytes of the svm decision function */)
{
   const int08s* svmNormal_end = svmFunction + svmSize;
   const int16s svmOffset = *(const int16s*)(svmNormal_end);

   for(int x = 0; x < chunkWidth; ++x, xOffsetLUT += numBlocksX) chess_loop_range(1,) 
   {
      const int08s* svmNormal = svmNormal_end;
      // Initialize SVM score accumulator to bias/offset
      scores[x] = svmOffset;
      
      // Iterate over the numBlocksX columns of block histograms in each
      // detection window
      for(int k = 0; k < numBlocksX; ++k) chess_loop_range(1,) 
      {
         const int x_ = xOffsetLUT[k];
         const vec08u* histogram = histograms + (N_BINS * x_);
         svmNormal -= numBlocksY * N_BINS;

         vec16s sum;
         if(rotateLUT[x_]) 
         {
           sum = vmrl(scores[x]);
         }
         else
         {
           sum = scores[x];
         }

         // Compute the SVM column score
         // Iterate over the chunkHeight/stridesPerBlockY = numBlocksY rows of
         // block histograms in the detection window
         int j = 0;
         int y = yOffsetLUT[histogramRow];
         for(int n = 0; n < chunkHeight; n += stridesPerBlockY, j += N_BINS) chess_loop_range(1,) 
         {
            const vec08u* hist = histogram + (chunkSpan * y);
            sum += svmSum(hist, svmNormal + j);
            y = yOffsetLUT[y];
         }

         scores[x] = sum;
      }
   }
}

#endif   // #ifdef ACF_KERNEL_IMPLEMENTATION
