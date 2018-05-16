/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2015-2017 NXP
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
/*!*********************************************************************************
*  \file pyramid_apu.cpp
*  \brief apu Image Pyramid Implementation.
***********************************************************************************/
#ifdef ACF_KERNEL_IMPLEMENTATION

#include "pyramid_apu.h"
#include <stdint.h>

// LOCAL HELPER FUNCTIONS
static inline vec16s _apply_horizontal_gaus_odd(const vec08u *src);
static inline vec16s _apply_horizontal_gaus_even(const vec08u *src);
static inline vec08u _apply_vertical_gaus(const vec16s *src, int16u stride);

//! [horizontal gaus]
void apu_pyr_horizontal_gaus(
      const vec08u* lpvIn,    int16u lStrideIn,
            vec16s* lpvOut,   int16u lStrideOut,
            int16u lChunkWidth, int16u lChunkHeight)
{
   //Optimized.
   const int bh   = lChunkHeight, bw = lChunkWidth, ss = lStrideIn, ds = lStrideOut;
   int chess_storage(R14) k4k1 = 0x0401;
   int chess_storage(R15) k0k6 = 0x0006;
   
   //Iterate over rows,
   for(int y=0; y<bh; ++y) chess_loop_range(1,)
   {
      //Variables
      vec16s        lacc, racc;
        
      //Pointer setup
      const vec08u*  ps = lpvIn-2;
            vec16s*  pd = lpvOut;
        
      //Loop preamble
      vec16u   lcol = *(vec16u*)ps; ps += 2;
      vec16u   mcol = *(vec16u*)ps; ps += 2;
      vec16u   rcol;
      
      //Iterate over columns
      for (int x=0; x<bw; x+=2)  chess_loop_range(2,)
                                 chess_unroll_loop(2)
        {
            //Load next pair of pixels
            rcol = *(vec16u*)ps; ps += 2;
            
            //Left accumulator
            vec16s   lacc  = (vec16s)vmul_slul(k4k1, lcol);
                     lacc += vmul_shuh(k4k1, lcol);
                     lacc += vmul_slul(k0k6, mcol);
                     lacc += vmul_shuh(k4k1, mcol);
                     lacc += vmul_slul(k4k1, rcol);
            
            //Store
            *pd++ = lacc;
            
            //Shift state
            lcol = mcol;mcol = rcol;
        }
        
        //Pointer update
        lpvIn  += ss;
        lpvOut += ds;
    }
}
//! [horizontal gaus]
//! [horizontal gaus expand]
void apu_pyr_horizontal_gaus_and_expand(
      const vec08u* lpvIn, int16u lStrideIn,
            vec16s* lpvOut, int16u lStrideOut,
            int16u lChunkWidth, int16u lChunkHeight)
{
   vec16s *lpvOutEvenRow = lpvOut + lStrideOut;
   for (int16u h = 0; h < lChunkHeight; h++)
   {
      for (int16u w = 0; w < lChunkWidth; w++)
      {
         lpvOut[w<<1] = _apply_horizontal_gaus_odd(&lpvIn[w]);
         lpvOut[(w<<1)+1] = _apply_horizontal_gaus_even(&lpvIn[w]);
         lpvOutEvenRow[w<<1] = 0;
         lpvOutEvenRow[(w<<1)+1] = 0;
      }
      lpvIn += lStrideIn;
      lpvOut += (lStrideOut<<1);
      lpvOutEvenRow += (lStrideOut<<1);
   }
}
//! [horizontal gaus expand]
//! [vertical gaus reduce]
void apu_pyr_vertical_gaus_and_reduce(
      const vec16s* lpvIn, int16u lStrideIn,
            vec08u* lpvOut, int16u lStrideOut,
            int16u lChunkWidth, int16u lChunkHeight)
{
   //Optimized.
   const int bh = lChunkHeight, bw = lChunkWidth, ss = lStrideIn, ds = lStrideOut;
   int16_t chess_storage(R14) k4 = (0x0004);
   int16_t chess_storage(R15) k6 = (0x0006);

   //Iterate over rows,
   for (int y=0; y<bh/2; ++y) chess_loop_range(1,)
   {
      //Variables
      const vec16s* ps0 = lpvIn-2*ss;
      const vec16s* ps1 = lpvIn-1*ss;
      const vec16s* ps2 = lpvIn+0*ss;
      const vec16s* ps3 = lpvIn+1*ss;
      const vec16s* ps4 = lpvIn+2*ss;
            vec08u* pd  = lpvOut;

      vec16s chess_storage(V0) s0 = *ps0++;
      vec16s chess_storage(V4) s4 = *ps4++;
      vec16s chess_storage(V1) s1 = *ps1++;
      vec16s chess_storage(V3) s3 = *ps3++;
      vec16s chess_storage(V2) s2 = *ps2++;
      
      //Iterate over columns
      for (int x=0; x<(bw/2); ++x)    chess_loop_range(2,) 
      {
          vec16s chess_storage(V6) acc;

         //Load horizontal convolution result
         acc  = s0;              s0 = *ps0++;
         acc += s4;              s4 = *ps4++;
         acc += vmul(s1, k4);    s1 = *ps1++;
         acc += vmul(s3, k4);    s3 = *ps3++;
         acc += vmul(s2, k6);    s2 = *ps2++;
         
         //Store sum
         *pd++ = (vec08u)((vec16u)acc >> 8);
      }
              
      //Pointer update
      lpvIn  += 2*ss;
      lpvOut += ds;
   }
}

//! [vertical gaus reduce]
//! [vertical gaus]
void apu_pyr_vertical_gaus(
      const vec16s* lpvIn, int16u lStrideIn,
            vec08u* lpvOut, int16u lStrideOut,
            int16u lChunkWidth, int16u lChunkHeight)
{
   for (int16u h = 0; h < lChunkHeight; h++)
   {
      for (int16u w = 0; w < lChunkWidth; w++)
      {
         lpvOut[w] = _apply_vertical_gaus(&lpvIn[w], lStrideIn);
      }
      lpvIn += lStrideIn;
      lpvOut += lStrideOut;
   }
}
//! [vertical gaus]

// LOCAL HELPER FUNCTIONS
//! [horizontal gaus expand helper]
// 1*src[-1]  4*0  6*src[0]  4*0  1*src[1]
static inline vec16s _apply_horizontal_gaus_odd(const vec08u *src)
{
   vec16s t0 = (vec16s)src[-1] + (vec16s)src[1];

   return (t0 + 6*(vec16s)src[0]);

}

// 1*0  4*src[0]  6*0  4*src[1]  1*0
static inline vec16s _apply_horizontal_gaus_even(const vec08u *src)
{
   vec16s t1 = (vec16s)src[0] + (vec16s)src[1];

   return (t1<<2);
}
//! [horizontal gaus expand helper]

//! [vertical gaus helper]
// 1*src[-2*stride]
// 4*src[-stride]
// 6*src[0]
// 4*src[stride]
// 1*src[2*stride]
static inline vec08u _apply_vertical_gaus(const vec16s *src, int16u stride)
{
   vec16s t0 = src[-2*stride] + src[2*stride];
   vec16s t1 = src[-stride] + src[stride];
   vec16s ta = t0 + (t1<<2) + 6*src[0];

   return (vec08u)((((ta>>5)+1)>>1)&0xFF);
}

#endif // ACF_KERNEL_IMPLEMENTATION
