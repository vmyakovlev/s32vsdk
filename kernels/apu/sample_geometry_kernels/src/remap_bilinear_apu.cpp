#ifdef ACF_KERNEL_IMPLEMENTATION

/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
* Copyright (c) 2017 NXP
*  All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/
/**
* \file remap_bilinear_apu.cpp
* \brief remap_bilinear implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/

#include "remap_bilinear_apu.h"

#ifdef APEX2_EMULATE
#include "apu_vec.hpp"
#endif


void
remap_bilinear_rgb_apu (vec32u* dst, vec32u* src, vec16u* offset, vec08u* delta,
						int sstride, int dstride, int bw, int bh) 
{
   vec16u idx;             // offset value, used for src lookup
   
   vec32u sA, sB, sC, sD;  // 32-bit source values for ABCD
   vec08u eA, eB, eC, eD;  //  8-bit element of source value
   vec08u deltaX, deltaY;  //  8-bit delta values

   // 1-d offset for offset/delta lookup
   int offset_idx = 0;
   // index of first pixel in dest tile row
   int dest_row = 0;

   
   // loop on destination pixels
   for (int y=0; y<bh; y++) {
      for (int x=0; x<bw; x++) {
        idx = (vec16u)offset[offset_idx];

        sA = vload(src, (vec16s)idx);
        sB = vload(src, (vec16s)(idx+1));
        sC = vload(src, (vec16s)(idx+sstride));
        sD = vload(src, (vec16s)(idx+sstride+1));
        
        // deltaX index is 2i+0.  deltaY index is 2i+1
        deltaX = delta[(offset_idx<<1)+0];
        deltaY = delta[(offset_idx<<1)+1];
        
        vec32u destPixel = 0;

        // Process each 8-bit element of the 32-bit pixel
        for (int k=0; k<32; k+=8) {
            eA = (vec08u)(sA >> k);              
            eB = (vec08u)(sB >> k);              
            eC = (vec08u)(sC >> k);              
            eD = (vec08u)(sD >> k);  

/*            
            // Original implementation - correct, but is not a bit-wise match with reference code
            // see CIMG_RefDewarp_c.cpp, fixedPointBilinRGB()
            vec16u AB = (vec16u)((eB - eA) * deltaX) + ((vec16u)eA << 8);
            vec16u CD = (vec16u)((eD - eC) * deltaX) + ((vec16u)eC << 8);
            
            vec32u ABCD = (((vec32u)CD - (vec32u)AB) * (vec32u)deltaY) + ((vec32u)AB << 8);
            ABCD += 0x00008000;   // for rounding
            
            vec08u destElement = (vec08u)(ABCD >> 16);
*/

            //vec08u AB = ((((eB - eA) * deltaX) + 0) >> 8) + eA;
            //vec08u CD = ((((eD - eC) * deltaX) + 0) >> 8) + eC;
            vec16u AB = ((((((vec16u)eB - (vec16u)eA) * (vec16u)deltaX) + 0) >> 8) + (vec16u)eA);
            vec16u CD = ((((((vec16u)eD - (vec16u)eC) * (vec16u)deltaX) + 0) >> 8) + (vec16u)eC);
            
            // see CLOVER-159
            AB &= 0x00ff;
            CD &= 0x00ff;
            
            //vec08u ABCD = ((((CD - AB) * deltaY) + 128) >> 8) + AB;
            vec16u ABCD = ((((CD - AB) * (vec16u)deltaY) + 128) >> 8) + AB;

            // see CLOVER-159
            ABCD &= 0x00ff;

            vec08u destElement = (vec08u)ABCD;
            destPixel |= (vec32u)destElement << k;
            
        }
         
        //dst[idx] = destPixel;
        vstore(dst, (vec16s)(dest_row + x), destPixel);
        
        // Assumes no stride in offset/delta.
        offset_idx++;
      }  // next pixel
      dest_row += dstride;
   } // next row
}



void
remap_bilinear_grayscale_impl(vec08u* dst, vec08u* src, vec16u* offset, vec08u* delta, 
							  int sstride, int dstride, int bw, int bh) 
{
 
   vec16u idx;             // offset value, used for src lookup
   
   vec08u sA, sB, sC, sD;  // 8-bit source values for ABCD
//   vec08u eA, eB, eC, eD;  //  8-bit element of source value
   vec08u deltaX, deltaY;  //  8-bit delta values
   
   // 1-d offset for offset/delta lookup
   int offset_idx = 0;
   // index of first pixel in dest tile row
   int dest_row = 0;

   
   // loop on destination pixels
   for (int y=0; y<bh; y++) {
      for (int x=0; x<bw; x++) {
        idx = (vec16u)offset[offset_idx];

      sA = vload(src, (vec16s)idx);
      sB = vload(src, (vec16s)(idx+1));
      sC = vload(src, (vec16s)(idx+sstride));
      sD = vload(src, (vec16s)(idx+sstride+1)); 
        
      // see CLOVER-159
      sA &= 0x00ff;
      sB &= 0x00ff;
      sC &= 0x00ff;
      sD &= 0x00ff;

      // deltaX index is 2i+0.  deltaY index is 2i+1
        deltaX = delta[(offset_idx<<1)+0];
        deltaY = delta[(offset_idx<<1)+1];
        
      vec08u destPixel;

      //vec08u AB = ((((sB - sA) * deltaX) + 0) >> 8) + sA;
      //vec08u CD = ((((sD - sC) * deltaX) + 0) >> 8) + sC;
      vec16u AB = ((((((vec16u)sB - (vec16u)sA) * (vec16u)deltaX) + 0) >> 8) + (vec16u)sA);
      vec16u CD = ((((((vec16u)sD - (vec16u)sC) * (vec16u)deltaX) + 0) >> 8) + (vec16u)sC);
            
      // see CLOVER-159
      AB &= 0x00ff;
      CD &= 0x00ff;
            
      //vec08u ABCD = ((((CD - AB) * deltaY) + 128) >> 8) + AB;
      vec16u ABCD = ((((CD - AB) * (vec16u)deltaY) + 128) >> 8) + AB;

      // see CLOVER-159
      ABCD &= 0x00ff;

    destPixel = (vec08u)ABCD;
      
      //dst[idx] = destPixel;
        vstore(dst, (vec16s)(dest_row + x), destPixel);
      
        // Assumes no stride in offset/delta.
        offset_idx++;
      }  // next pixel
      dest_row += dstride;
   } // next row
}
#endif
