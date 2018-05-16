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
* \file gauss_5x5_apu.cpp
* \brief 5x5 Gauss filter implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/
#ifdef ACF_KERNEL_IMPLEMENTATION

#include "gauss_5x5_apu.h"

void Gauss_5x5__filter(vec08u* dst, vec08u* src, int sstr, int bw, int bh)
{
   vec08u* fsrc = (src - (sstr << 1));
   for (int by=0; by<bh; ++by)  // y data row
   {
     for (int bx=0; bx<bw; ++bx) // x in the blck_tile
     {
       vec16s r = 0;
       r += (vec16s)fsrc[bx-2] + (vec16s)fsrc[bx-1] + (vec16s)fsrc[bx] + (vec16s)fsrc[bx+1] + (vec16s)fsrc[bx+2];
       fsrc += sstr;
       r += (vec16s)fsrc[bx-2] + 5 * (vec16s)fsrc[bx-1] + 5 * (vec16s)fsrc[bx] + 5 * (vec16s)fsrc[bx+1] + (vec16s)fsrc[bx+2];
       fsrc += sstr;
       r += (vec16s)fsrc[bx-2] + 5 * (vec16s)fsrc[bx-1] + (vec16s)(fsrc[bx] << 3) + 5 * (vec16s)fsrc[bx+1] + (vec16s)fsrc[bx+2];
       fsrc += sstr;
       r += (vec16s)fsrc[bx-2] + 5 * (vec16s)fsrc[bx-1] + 5 * (vec16s)fsrc[bx] + 5 * (vec16s)fsrc[bx+1] + (vec16s)fsrc[bx+2];
       fsrc += sstr;
       r += (vec16s)fsrc[bx-2] + (vec16s)fsrc[bx-1] + (vec16s)fsrc[bx] + (vec16s)fsrc[bx+1] + (vec16s)fsrc[bx+2];
       fsrc -= (sstr << 2);
       // Scaling
       dst[bx] = (vec08u)(r >> 6);
     }
     dst  += bw;
     fsrc += sstr;
   }
} 

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
