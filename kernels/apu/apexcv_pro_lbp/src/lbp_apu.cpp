/**************************************************************************************************
 * 
 * NXP Confidential Proprietary
 *
 * Copyright (c) 2015, Digital Media Professionals. All rights reserved.
 * Copyright 2017 NXP 
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
 **************************************************************************************************/
 
#ifdef ACF_KERNEL_IMPLEMENTATION
#ifdef DEBUG
#include <stdio.h>
#endif

#include "lbp_apu.h"

// #define APU_VSIZE 32

static const int16u LBP_RADIUS = 2;
static const int16u LBP_NEIGHBORS = 8;
static const int16u LBP_GRID_X = 8;
static const int16u LBP_GRID_Y = 8;
static const int16u LBP_HIST_LENGTH = (1<<LBP_NEIGHBORS);
static const int16u LOCAL_ACUM_NUM = 8;

// HELPER FUNCTIONS
static inline vec08u _calculate_local_binary_pattern(vec08u *src, int16u sstr);
static inline vec32s _calculate_distance(vec08u src0, vec08u src1, int32_t numCU);


/*!
   Calculate LBP descriptor.
   (unsigned 8-bit -> unsigned 8-bit)
   
   \param lpvIn         - [Input]   Pointer to the input image data.
   \param lStrideIn     - [Input]   Stride of the input image data.
   \param lpvOut        - [Output]  Pointer to the output descriptor data.
   \param lChunkWidth   - [Input]   Processing chunk width.
   \parma lChunkHeight  - [Input]   Processing chunk height.
 */
void apu_lbp_extract_descriptor(
   vec08u *lpvIn,
   int16u lStrideIn,
   vec08u *lpvOut,
   int16u lChunkWidth,
   int16u lChunkHeight,
   int32_t numCU
)
{
   // zero clear
   for (int16u i = 0; i < LBP_HIST_LENGTH; i++)
      lpvOut[i] = 0;
   
   // skip margin (= radius pixel)
   int16u lOffset = LBP_RADIUS*lStrideIn+LBP_RADIUS;
   lpvIn += lOffset;
   
   for (int16u h = 0; h < lChunkHeight; h++) chess_loop_range(1,)
   {
      for (int16u w = 0; w < lChunkWidth; w++)  chess_loop_range(1,)
      {
         // compute LBP of the pixel
         vec08u lbp = _calculate_local_binary_pattern(&lpvIn[w], lStrideIn);
         
         // create histogram
         vec08u d = vload(lpvOut, lbp);
         vstore(lpvOut, lbp, d+1);
#ifdef DEBUG
         for (int16u cu = 0; cu < numCU; cu++)
         {
            int08u val = vget(d, cu);
            if (val==0xff)
               printf("apu_lbp_create_histogram() overflow!\n");
         }
#endif         
      }
      lpvIn += lStrideIn;
   }
}


/*!
   Compare two descriptors.
   (unsigned 8-bit -> signed 32-bit)
   
   \param lpvIn0        - [Input]   Pointer to the input image0 data.
   \param lpvIn1        - [Input]   Pointer to the input image1 data.
   \param lpvOut        - [Output]  Pointer to the histogram distance buffer.
   \param lCompareSize  - [Input]   Comparing array size.
 */
void apu_lbp_compare_histogram(
   vec08u *lpvIn0,
   vec08u *lpvIn1,
   vec32s *lpvOut,
   int16u lCompareSize,
   int32_t numCU
)
{
   for (int16u i = 0; i < lCompareSize; i++) chess_loop_range(1,)
   {
      lpvOut[0] += _calculate_distance(lpvIn0[i], lpvIn1[i], numCU);
   }
}

// misa's implementation
void apu_lbp_compare_histogram_misa(
   vec08u *lpvIn0,
   vec08u *lpvIn1,
   vec32s *lpvOut,
   int16u lCompareSize,
   int32_t numCU
)
{
   for (int16u i = 0; i < lCompareSize; i++) chess_loop_range(1,)
   {
      lpvOut[0] += _calculate_distance(lpvIn0[i], lpvIn1[i], numCU);
   }
}


/*!
   Accumulate local value in each CU to obtain one scalar value.
   (signed 32-bit -> signed 32-bit)
   
   \param lpvIn         - [Input]   Pointer to the local value.
   \param lpvOut        - [Output]  Pointer to the accumulated value.
 */
void apu_lbp_accumulate_distance(
   vec32s lpvIn,
   int32s *lpvOut,
   int32_t numCU
)
{
   vec32s localAcum = lpvIn;
   lpvOut[0] = 0;

   // move block sum value to left CU and add local distance LOCAL_ACUM_NUM-1 times
   for (int16u i = 0; i < (LOCAL_ACUM_NUM-1); i++)
   {
      localAcum = vmrl(localAcum, 0);
      localAcum += lpvIn;
   }
   // collect APU_VSIZE>>3 block sum values to merge into one global sum value
   for (int16u i = 0; i < (numCU>>3); i++)
   {
      lpvOut[0] += vget(localAcum, (i<<3));
   }
}

// HELPER FUNCTIONS
static inline vec08u _calculate_local_binary_pattern(vec08u *src, int16u sstr)
{
   vec08u center = src[0];
   vec16u shifted = (vec16u)center<<7;

   vec16u t1 = 31*(vec16u)src[-2*sstr+1]+22*(vec16u)src[-2*sstr+2]+44*(vec16u)src[-sstr+1]+31*(vec16u)src[-sstr+2];
   vec16u t3 = 22*(vec16u)src[-2*sstr-2]+31*(vec16u)src[-2*sstr-1]+31*(vec16u)src[-sstr-2]+44*(vec16u)src[-sstr-1];
   vec16u t5 = 31*(vec16u)src[sstr-2]+44*(vec16u)src[sstr-1]+22*(vec16u)src[2*sstr-2]+31*(vec16u)src[2*sstr-1];
   vec16u t7 = 44*(vec16u)src[sstr+1]+31*(vec16u)src[sstr+2]+31*(vec16u)src[2*sstr+1]+22*(vec16u)src[2*sstr+2];
   
   vec08u dst = (vec08u)(src[2]>=center) | ((vec08u)(t1>=shifted)<<1) | ((vec08u)(src[-2*sstr]>=center)<<2) |
      ((vec08u)(t3>=shifted)<<3) | ((vec08u)(src[-2]>=center)<<4) | ((vec08u)(t5>=shifted)<<5) |
      ((vec08u)(src[2*sstr]>=center)<<6) | ((vec08u)(t7>=shifted)<<7);

   return dst;
}

static inline vec32s _calculate_distance(vec08u src0, vec08u src1, int32_t numCU)
{
   vec16u diff = vabs((vec16s)src0 - (vec16s)src1);
   vec16u deno = (vec16u)src0 + (vec16u)src1;
   vec16u dist = vselect(0, (diff*diff)/deno, deno==0);
   
#ifdef DEBUG
   vec32s d = (vec32s)src0 - (vec32s)src1;
   vec32s n = (vec32s)src0 + (vec32s)src1;
   vec16u err = vselect((vec16u)1, (vec16u)0, d!=(vec32s)diff);
   err += vselect((vec16u)1, (vec16u)0, n!=(vec32s)deno);
   int16u total_err = 0;
   for (int16u cu = 0; cu < numCU; cu++)
      total_err += vget(err, cu);
   if (total_err!=0)
      printf("_calculate_distance() overflow!\n");
#endif
   
   return (vec32s)dist;
}

#endif // ACF_KERNEL_IMPLEMENTATION
