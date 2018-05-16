
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
* \file saturate_nonzero_apu.cpp
* \brief saturate_nonzero implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/
#ifdef ACF_KERNEL_IMPLEMENTATION

#include "saturate_nonzero_apu.h"

/*****************************************************************************
* functions
*****************************************************************************/

void saturate_nonzero(vec08u* dst, vec08u* src, int /*dstr*/, int /*sstr*/, int bw, int bh)
{
  for (int by = 0; by<bh; ++by)  // y data row
  {
    for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
    {
      vec08u res = 255;
      vif(src[bx] == 0)
      {
        res = 0;
      }
      vendif;

      dst[bx] = res;
    }
    dst += bw;
    src += bw;
  }
}



void binarize(vec08u* dst, vec32u* src, int /*dstr*/, int /*sstr*/, int bw, int bh)
{
	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			dst[bx] = (vec08u) vselect(vec08u(0), vec08u(255), src[bx] == 0);
		}
		dst += bw;
		src += bw;
	}
}



void mask(vec16s* dstX, vec16s* dstY, vec32u* srcFlags, vec16s* inX, vec16s* inY, int /*dstr*/, int /*sstr*/, int bw, int bh)
{
	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			vif(srcFlags[bx] == 0) {
				dstX[bx] = 0;
				dstY[bx] = 0;
			} velse{
				dstX[bx] = inX[bx];
				dstY[bx] = inY[bx];
			}vendif
		}
		dstX	+= bw;
		dstY	+= bw;
		inX		+= bw;
		inY		+= bw;
		srcFlags+= bw;
	}
}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION 
