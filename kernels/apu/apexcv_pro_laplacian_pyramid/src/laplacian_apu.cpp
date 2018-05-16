/**************************************************************************************************
 * 
 * NXP Confidential Proprietary
 *
 * Copyright 2016 NXP 
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

/*==================================================================================================
Revision History:
                              Modification     Tracking
Author (core ID)              Date D/M/Y       Number     Description of Changes
---------------------------   ----------    ------------  ------------------------------------------
Adrian Grigore (nxa11567)     13/12/2016    VSDK-554      Created
---------------------------   ----------    ------------  ------------------------------------------
==================================================================================================*/

/*!*********************************************************************************
*  \file laplacian_apu.cpp
*  \brief APU Laplacian Pyramid Implementation.
***********************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "laplacian_apu.h"
#include <stdint.h>

void apu_pyr_horizontal_gaus_laplacian(
         const vec08u* lpvIn,
               int16u  lStrideIn,
               vec16u* lpvOut,
               int16u  lStrideOut,
               int16u  lChunkWidth,
               int16u  lChunkHeight)
{
   const int bh   = lChunkHeight, bw = lChunkWidth, ss = lStrideIn, ds = lStrideOut;
   int chess_storage(R14) k4k1 = 0x0401;
   int chess_storage(R15) k0k6 = 0x0006;
   
   if (0 == (bw & 0x1)) //Optimized path - two columns
   {
      //Iterate over rows,
      for(int y=0; y<bh; ++y) chess_loop_range(1,)
      {
         //Pointer setup
         const vec08u*  ps = lpvIn-2;
               vec16u*  pd = lpvOut;
           
         //Loop preamble
         vec16u   lcol = *(vec16u*)ps; ps += 2;
         vec16u   mcol = *(vec16u*)ps; ps += 2;
         
         //Iterate over columns
         for (int x=0; x<bw; x+=2)  chess_loop_range(2,)
         {
           //Variables
            vec16u lacc,racc;
            //Load next pair of pixels
            vec16u rcol = *(vec16u*)ps; ps += 2;
            
            lacc  = vmul_ulul(k4k1, lcol)
             + vmul_uhuh(k4k1, lcol)
             + vmul_ulul(k0k6, mcol)
             + vmul_uhuh(k4k1, mcol)
             + vmul_ulul(k4k1, rcol);
            *pd++ = lacc;
            racc  = (lcol>>8)
             + vmul_uhul(k4k1, mcol)
             + vmul_uluh(k0k6, mcol)
             + vmul_uhul(k4k1, rcol)
             + (rcol>>8);
            *pd++ = racc;
            
            //Shift state
            lcol = mcol;
            mcol = rcol;
         }

         //Pointer update
         lpvIn  += ss;
         lpvOut += ds;
      }
   }
   else //One pixel at a time
   {
      for(int y=0; y<bh; ++y) chess_loop_range(1,)
      {
         //Pointer setup
         const vec08u*  ps = lpvIn-2;
               vec16u*  pd = lpvOut;
           
         //Loop preamble
         vec16u   val0 = (vec16u)(*ps); ps++;
         vec16u   val1 = (vec16u)(*ps); ps++;
         vec16u   val2 = (vec16u)(*ps); ps++;
         vec16u   val3 = (vec16u)(*ps); ps++;
         vec16u   val4 = (vec16u)(*ps); ps++;
         
         //Iterate over columns
         for (int x=0; x<bw; ++x)  chess_loop_range(1,)
         {
           //Variables
            vec16u acc;
            
            acc  = val0 + val4
                 + ((val1 + val3)<<2)
                 + vmul(val2,k0k6);
            *pd++ = acc;
            
            //Shift state
            val0 = val1;
            val1 = val2;
            val2 = val3;
            val3 = val4;
            //Load next pixel
            val4 = (vec16u)(*ps); ps++;
         }
      //Pointer update
      lpvIn  += ss;
      lpvOut += ds;
      }
   }
}


void apu_pyr_vertical_gaus_laplacian_mid(
         const vec08u* lpvIn,
               int16u  lStrideIn,
         const vec16u* lpvIn_gauss,
               int16u  lStrideIn_gauss,
               vec16s* lpvOut_lap,
               int16u  lStrideOut_lap,
               vec08u* lpvOut_nex,
               int16u  lStrideOut_nex,
               int16u  lChunkWidth,
               int16u  lChunkHeight)
{
   const int bh = lChunkHeight, bw = lChunkWidth;
   const int ss0 = lStrideIn, ss1 = lStrideIn_gauss;
   const int ds0 = lStrideOut_lap, ds1 = lStrideOut_nex;
   int16_t chess_storage(R15) k6 = (0x0006);

   //Iterate over rows,
   for (int y=0; y<bh/2; ++y) chess_loop_range(1,)
   {
      //Variables
      const vec16u* chess_storage(R5) ps0 = lpvIn_gauss - 2 * ss1;
      const vec16u* chess_storage(R6) ps1 = lpvIn_gauss - 1 * ss1;
      const vec16u* chess_storage(R7) ps2 = lpvIn_gauss + 0 * ss1;
      const vec16u* chess_storage(R8) ps3 = lpvIn_gauss + 1 * ss1;
      const vec16u* chess_storage(R9) ps4 = lpvIn_gauss + 2 * ss1;
      const vec16u* chess_storage(R10) ps5 = lpvIn_gauss + 3 * ss1;
      
      const vec08u* pis0 = lpvIn + 0 * ss0;
      const vec08u* pis1 = lpvIn + 1 * ss0;
      
            vec08u* pd  = lpvOut_nex;
            vec16s* pd0 = lpvOut_lap;
            vec16s* pd1 = lpvOut_lap + ds0;
     
      //Iterate over columns
      for (int x=0; x<(bw/2); ++x)    chess_loop_range(2,) 
      {
         vec16u acc0,acc1,acc2,acc3;
         
         vec16u s0 = *ps0++;
         vec16u s1 = *ps1++;
         vec16u s2 = *ps2++;
         vec16u s3 = *ps3++;
         vec16u s4 = *ps4++;
         vec16u s5 = *ps5++;
         
         //Load horizontal convolution result
         acc0  = s0 +
                 s4 +
                 ((s1 + s3) << 2) + 
                 vmul(s2, k6);
         acc1  = s1 + 
                 s5 + 
                 ((s2 + s4) << 2) + 
                 vmul(s3, k6);
         acc0 >>= 8;
         acc1 >>= 8;
         
         //Store sum
         
         *pd0++ = (vec16s)(*pis0++) - (vec16s)acc0;
         *pd1++ = (vec16s)(*pis1++) - (vec16s)acc1;
                  
         //Load horizontal convolution result
         s0 = *ps0++;
         s1 = *ps1++;
         s2 = *ps2++;
         s3 = *ps3++;
         s4 = *ps4++;
         s5 = *ps5++;
         
         acc2  = s0 +
                 s4 +
                 ((s1 + s3) << 2) + 
                 vmul(s2, k6);
         acc3  = s1 + 
                 s5 + 
                 ((s2 + s4) << 2) + 
                 vmul(s3, k6);
         acc2 >>= 8;
         acc3 >>= 8;
         
         //Store sum
         *pd++ = (vec08u)acc3;
         *pd0++ = (vec16s)(*pis0++) - (vec16s)acc2;
         *pd1++ = (vec16s)(*pis1++) - (vec16s)acc3;
      }
      //Pointer update
      lpvIn       += 2 * ss0;
      lpvIn_gauss += 2 * ss1;
      lpvOut_nex  += 1 * ds1;
      lpvOut_lap  += 2 * ds0;
   }
}

void apu_pyr_vertical_gaus_laplacian_last(
         const vec08u* lpvIn,
               int16u  lStrideIn,
         const vec16u* lpvIn_gauss,
               int16u  lStrideIn_gauss,
               vec16s* lpvOut_lap,
               int16u  lStrideOut_lap,
               vec16s* lpvOut_out,
               int16u  lStrideOut_out,
               int16u  lChunkWidth,
               int16u  lChunkHeight)
{
   const int bh = lChunkHeight, bw = lChunkWidth;
   const int ss0 = lStrideIn, ss1 = lStrideIn_gauss;
   const int ds0 = lStrideOut_lap, ds1 = lStrideOut_out;
   int16_t chess_storage(R15) k6 = (0x0006);
   
   //Iterate over rows,
   for (int y=0; y<bh; ++y) chess_loop_range(1,)
   {
      //Variables
      const vec16u* chess_storage(R5) ps0 = lpvIn_gauss - 2 * ss1;
      const vec16u* chess_storage(R6) ps1 = lpvIn_gauss - 1 * ss1;
      const vec16u* chess_storage(R7) ps2 = lpvIn_gauss + 0 * ss1;
      const vec16u* chess_storage(R8) ps3 = lpvIn_gauss + 1 * ss1;
      const vec16u* chess_storage(R9) ps4 = lpvIn_gauss + 2 * ss1;
      
      const vec08u* pis = lpvIn;
      
            vec16s* pdo = lpvOut_out;
            vec16s* pdl = lpvOut_lap;
            
      //Loop preamble
      vec16u s0 = *ps0++;
      vec16u s1 = *ps1++;
      vec16u s2 = *ps2++;
      vec16u s3 = *ps3++;
      vec16u s4 = *ps4++;

      //Iterate over columns
      for (int x=0; x<bw; ++x)    chess_loop_range(1,) 
      {
         vec16u acc;

         acc = s0 +
               s4 +
               ((s1 + s3) << 2) + 
               vmul(s2, k6);
         acc >>= 8;
         
         //Load next values
         s0 = *ps0++;
         s1 = *ps1++;
         s2 = *ps2++;
         s3 = *ps3++;
         s4 = *ps4++;
         
         //Store sum
         *pdo++ = (vec16s)acc;
         
         *pdl++ = (vec16s)(*pis++) - (vec16s)acc;
      }
      //Pointer update
      lpvIn       += ss0;
      lpvIn_gauss += ss1;
      lpvOut_out  += ds1;
      lpvOut_lap  += ds0;
   }
}

#endif // ACF_KERNEL_IMPLEMENTATION
