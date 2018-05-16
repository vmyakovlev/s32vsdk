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
#include "fast_apu.h"

void apu_nms(
      const vec08u* apcIn, vec08u* apOut,
      int aInStride, int aOutStride,
      int aTileWidth, int aTileHeight)
{
  for (int y = 0; y < aTileHeight; ++y)
  {
    for (int x = 0; x < aTileWidth; ++x)
    {
      vec08u v = apcIn[x];

      vbool fail = v < apcIn[x - 1 - aInStride];
      fail = fail || (v < apcIn[x - aInStride]);
      fail = fail || (v < apcIn[x + 1 - aInStride]);
      fail = fail || (v < apcIn[x - 1]);
      fail = fail || (v <= apcIn[x + 1]);
      fail = fail || (v <= apcIn[x - 1 + aInStride]);
      fail = fail || (v <= apcIn[x + aInStride]);
      fail = fail || (v <= apcIn[x + 1 + aInStride]);
      vif (fail)
      {
        v = 0;
      }
      vendif;
      apOut[x] = v;
    }

    apcIn += aInStride;
    apOut += aOutStride;
  }
}

void apu_fastx_offset
(
   uint32_t* offsets, //
   int      sstr,
   int      circumference
)
{
   int rval = 0;
   static const int16_t pos[][2] =
   {
   //  9/16: 0 - 16
      {0,  3}, { 1,  3}, { 2,  2}, { 3,  1}, { 3, 0}, { 3, -1}, { 2, -2}, { 1, -3},
      {0, -3}, {-1, -3}, {-2, -2}, {-3, -1}, {-3, 0}, {-3,  1}, {-2,  2}, {-1,  3},
   //  7/12: 16 - 12
      {0,  2}, { 1,  2}, { 2,  1}, { 2, 0}, { 2, -1}, { 1, -2},
      {0, -2}, {-1, -2}, {-2, -1}, {-2, 0}, {-2,  1}, {-1,  2},
   //  5/ 8: 28 - 36
      {0,  1}, { 1,  1}, { 1, 0}, { 1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1,  1}
   };

   const int16_t (*p)[2] = 0;
   int16_t s = 0;
   
   switch (circumference)
   {
   case 16:  p = &pos[ 0]; s = 16; break;
   case 12:  p = &pos[16]; s = 12; break;
   case  8:  p = &pos[28]; s =  8; break;
   default: rval = -1;   
   }
   
   if (0 == rval)
   {
      for (int k=0; k<s; ++k)
         offsets[k] = p[k][0] + p[k][1]*sstr;
   }
}

/* fast without NMS */
void apu_fast
(
   vec08u*  dst, int dstr,    //!< Image of corners (binary/corner score/reponse/strenght)
   vec08u*  src, int sstr,    //!< Source image (grayscale)
   int  bw,  int bh,          //!< block width/height
   int  threshold,            //!< Threshold
   int* offset                //!< pre-computed offset positions
)
{
   vec08u d[16]; // difference

   const vec08u* sptr = src;
         vec08u* dptr = dst;

   for (int y=0; y<bh; ++y) chess_loop_range(1,) 
   {
      for (int x=0; x<bw; ++x) chess_loop_range(1,) 
      {
         // initialize dst
         vec16s ic  = (vec16s)sptr[x];
         vec16s tl  = ic - threshold; //lower threshold
         vec16s th  = ic + threshold; //upper threshold

         vec16u fl  = 0;
         vec16u fh  = 0;
         
         vec16s ik = (vec16s)sptr[x + offset[0]];
         for (int k=1; k<16; ++k)
         {
            fl = (vec16u)vsrc((vec16s)fl, (ik < tl));  // pushbit
            fh = (vec16u)vsrc((vec16s)fh, (ik > th));  // pushbit
            d[k - 1] =  (vec08u)vabs_diff(ic, ik); // difference information
            ik = (vec16s)sptr[x + offset[k]];
         }
         fl = (vec16u)vsrc((vec16s)fl, (ik < tl));  // pushbit
         fh = (vec16u)vsrc((vec16s)fh, (ik > th));  // pushbit
         d[15] =  (vec08u)vabs_diff(ic, ik);  // difference information
            
         // select the highest population count
         vec16u fs  = vselect(fl, fh, vpcnt(fl) > vpcnt(fh));

         // compute fast9 response
         vec16u rs;
         {
            vec16u tl, th;
            vsr(tl, th, fs, fs, 1); rs = fs & tl;  
            vsr(tl, th, rs, rs, 2); rs = rs & tl;  
            vsr(tl, th, rs, rs, 4); rs = rs & tl;  
            vsr(tl, th, fs, fs, 8); rs = rs & tl;
         }

         // compute corner score
         // 1: corners
         // 0: no corners
         vec16s   score = (vec16s)vselect((vec16s)1, 0, (rs != 0));
         dptr[x] = (vec08u)score;
      }
      sptr += sstr;
      dptr += dstr;
   }
}

/* fast without NMS and serialized cofficients */
#define X 0
#define Y 1
#define isPowerOf2(x) ((~((x) & 0x00000001)) & 0x00000001)

static int yOffset = 0;

void apu_fast_serialized
(
   unsigned short * list, 
   int * nrOfElements,
   int firstCuId, 
   int nrOfCu,
   int maxOutSample,
   vec08u * src,
   int sstr,
   int bw,  
   int bh,
   int threshold,
   int * offset)
{
    // Temporary storage for differences
    vec08u d[16];
    
    // Temporary storage
    const int32_t pair = 2;
    int32_t taps = 0;
     
    // Chunk Id for each CU
    const vec08u * sptr = src;

    // Special exit conditions
    if(isPowerOf2(maxOutSample) == 0) {*nrOfElements = -1; return;};
    
    for (int y=0; y<bh; ++y) chess_loop_range(1,) 
    {
        for (int x=0; x<bw; ++x) chess_loop_range(1,) 
        {
            // initialize dst
            vec16s ic  = (vec16s)sptr[x];
            vec16s tl  = ic - threshold; //lower threshold
            vec16s th  = ic + threshold; //upper threshold

            vec16u fl  = 0;
            vec16u fh  = 0;
         
            vec16s ik = (vec16s)sptr[x + offset[0]];
            for (int k=1; k<16; ++k)
            {
                fl = (vec16u)vsrc((vec16s)fl, (ik < tl));  // pushbit
                fh = (vec16u)vsrc((vec16s)fh, (ik > th));  // pushbit
                d[k - 1] =  (vec08u)vabs_diff(ic, ik); // difference information
                ik = (vec16s)sptr[x + offset[k]];
            }
            fl = (vec16u)vsrc((vec16s)fl, (ik < tl));  // pushbit
            fh = (vec16u)vsrc((vec16s)fh, (ik > th));  // pushbit
            d[15] =  (vec08u)vabs_diff(ic, ik);  // difference information
            
            // select the highest population count
            vec16u fs  = vselect(fl, fh, vpcnt(fl) > vpcnt(fh));

            // compute fast9 response
            vec16u rs;
            {
                vec16u tl, th;
                vsr(tl, th, fs, fs, 1); rs = fs & tl;  
                vsr(tl, th, rs, rs, 2); rs = rs & tl;  
                vsr(tl, th, rs, rs, 4); rs = rs & tl;  
                vsr(tl, th, fs, fs, 8); rs = rs & tl;
            }

            // compute corner score
            // 1: corners
            // 0: no corners            
            vec08u isCorner = (vec08u)(rs != 0);
            
            if(vany((rs != 0)))
            {
                // Load the current number of elements
                int32_t gCnt = *nrOfElements;
                
                // Scalar processing
                for(int cuId = firstCuId; cuId < (firstCuId + nrOfCu); cuId++)
                {
                    // Ok to put inside the queue
                    int32_t isOk = vget(isCorner, cuId);
                    
                    if(isOk)
                    {
                        // Packing all the data next to each other
                        list[pair * gCnt + X] = (cuId * bw + x);
                        list[pair * gCnt + Y] = (yOffset + y);
                        gCnt++;
                        
                        // gCnt = (gCnt%maxOutSample)
                        gCnt &= (maxOutSample - 1);
                        
                        // Variable that will indicate if the buffer was wrapped around
                        taps++;
                    }
                }
                
                // Update the global counter
                *nrOfElements = gCnt;               
                
                // Don't overflow !
                if(taps > maxOutSample) {*nrOfElements = -2; return;};
            }
        }
        sptr += sstr;
    }
    yOffset += bh;
}

/* fast with NMS */
void apu_fast_nms
(
   vec08u*  dst, int dstr,    //!< Image of corners (binary/corner score/reponse/strenght)
   vec08u*  src, int sstr,    //!< Source image (grayscale)
   int  bw,  int bh,          //!< block width/height
   int  threshold,            //!< Threshold
   int* offset                //!< pre-computed offset positions
)
{
   vec08u d[16]; // difference

   const vec08u* sptr = src;
         vec08u* dptr = dst;

   for (int y=0; y<bh; ++y) chess_loop_range(1,) 
   {
      for (int x=0; x<bw; ++x) chess_loop_range(1,) 
      {
         // initialize dst
         vec16s ic  = (vec16s)sptr[x];
         vec16s tl  = ic - threshold; //lower threshold
         vec16s th  = ic + threshold; //upper threshold

         vec16u fl  = 0;
         vec16u fh  = 0;
         
         vec16s ik = (vec16s)sptr[x + offset[0]];
         for (int k=1; k<16; ++k)
         {
            fl = (vec16u)vsrc((vec16s)fl, (ik < tl));  // pushbit
            fh = (vec16u)vsrc((vec16s)fh, (ik > th));  // pushbit
            d[k - 1] =  (vec08u)vabs_diff(ic, ik); // difference information
            ik = (vec16s)sptr[x + offset[k]];
         }
         fl = (vec16u)vsrc((vec16s)fl, (ik < tl));  // pushbit
         fh = (vec16u)vsrc((vec16s)fh, (ik > th));  // pushbit
         d[15] =  (vec08u)vabs_diff(ic, ik);  // difference information
            
         // select the highest population count
         vec16u fs  = vselect(fl, fh, vpcnt(fl) > vpcnt(fh));

         // compute fast9 response
         vec16u rs;
         {
            vec16u tl, th;
            vsr(tl, th, fs, fs, 1); rs = fs & tl;  
            vsr(tl, th, rs, rs, 2); rs = rs & tl;  
            vsr(tl, th, rs, rs, 4); rs = rs & tl;  
            vsr(tl, th, fs, fs, 8); rs = rs & tl;
         }

         // compute corner score
         vec16s   score = (vec16s)vselect((vec16s)threshold, 0, (rs != 0));
         /* compute score for NMS */
         vif (rs != 0)
         {
            vec16u ft = 0;
            for (int k=0; k<16; ++k) chess_unroll_loop(*)
            {
               ft = (vec16u)vsrc((vec16s)ft, (d[k] > ((vec08u)score+1))); // pushbit 
            }
            fs &= ft;
            // important step for early exit (see 6-ALL)
            {
               vec16u tl, th;
               vsr(tl, th, fs, fs, 1); rs = fs & tl;  
               vsr(tl, th, rs, rs, 2); rs = rs & tl;  
               vsr(tl, th, rs, rs, 4); rs = rs & tl;  
               vsr(tl, th, fs, fs, 8); rs = rs & tl;
            }         
#if (__chess__)
            while (rs != 0)
#elif (__llvm__)
            while (vany(rs != 0))
#endif
            {
#ifdef __llvm__
                vif(rs != 0) {
#endif
               // find the minimum value & position(s)
               vec16s tmin = 256;
               vec16u kmin =   0;
               vec16u fs_copy = fs;
               for (int k=0; k<16; ++k)
               {
                  vec16s dk = (vec16s)d[k];
                  /* move lsb -> vc */ 
                  vbool fe  = (vbool)(fs_copy);
                  fs_copy >>= 1;
                  vbool dlt = (dk <  tmin) && fe;
                  vbool dle = (dk <= tmin) && fe;
                  tmin = vselect(dk, tmin, dlt);
                  kmin = vselect( 0, kmin, dlt);
                  kmin = (vec16u)vsrc((vec16s)kmin, dle);  /* pushbit */
               }
               // update fs and rs...
               fs &= ~kmin;
               {
                  vec16u tl, th;
                  vsr(tl, th, fs, fs, 1); rs = fs & tl;  
                  vsr(tl, th, rs, rs, 2); rs = rs & tl;  
                  vsr(tl, th, rs, rs, 4); rs = rs & tl;  
                  vsr(tl, th, fs, fs, 8); rs = rs & tl;
               }
               score = tmin - 1;
#ifdef __llvm__
                } vendif
#endif
            }     // while (rs != 0)
         }        // if (rs != 0)
         dptr[x] = (vec08u)score;
      }
      sptr += sstr;
      dptr += dstr;
   }
}
#endif   // #ifdef ACF_KERNEL_IMPLEMENTATION 
