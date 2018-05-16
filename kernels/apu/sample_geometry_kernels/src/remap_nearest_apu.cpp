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
* \file remap_nearest_apu.cpp
* \brief remap_nearest implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/

#include "remap_nearest_apu.h"

#ifdef APEX2_EMULATE
#include "apu_vec.hpp"
#endif

void
remap_nearest_grayscale_impl(vec08u* dst, vec08u* src, vec16u* offset, int /*sstride*/, int dstride, int bw, int bh) {
   vec16s idx;             // offset value, used for src lookup
   vec08u src_val;
   
   // 1-d offset for offset/delta lookup
   int offset_idx = 0;
   // index of first pixel in dest tile row
   vec16s dest_row = 0;
   //const vec08u zeroVect = 0;
   
   // loop on destination pixels
   for (int y=0; y<bh; y++) {
      for (int x=0; x<bw; x++) {
        idx = (vec16s)offset[offset_idx];
        vstore(dst, dest_row + x, (vec08u) 0);

	    vif (idx != 0x8000)
 	    {
	      src_val = vload(src, idx);      
	      vstore(dst, dest_row + x, src_val);
	    }
	
	    //vstore(dst, (vec16s)(dest_row + x), vload(src, (vec16s)(dest_row + x)));      
        // Assumes no stride in offset/delta.
        offset_idx++;
      }  // next pixel
      dest_row += dstride;
   } // next row
}
#endif
