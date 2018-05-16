
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
* \file dilate_diamond_apu.cpp
* \brief dilate_diamond implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/
#ifdef ACF_KERNEL_IMPLEMENTATION

#include "dilate_diamond_apu.h"

/*****************************************************************************
* functions
*****************************************************************************/

vec08u vmax(vec08u a, vec08u b)
{
	vec08u ret = a;
	vif(b > a)
	{
		ret = b;
	}
	vendif;
	return ret;
}

void dilate_diamond(vec08u* dst, vec08u* src, int dstr, int sstr, int bw, int bh)
{
  vec08u* fsrc = (src - (sstr << 1));
  for (int by = 0; by<bh; ++by)  // y data row
  {
    for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
    {
      vec08u r = 0;
      r = vmax(r, fsrc[bx]);
      fsrc += sstr;
      r = vmax(r, fsrc[bx - 1]);
      r = vmax(r, fsrc[bx]);
      r = vmax(r, fsrc[bx + 1]);
      fsrc += sstr;
      r = vmax(r, fsrc[bx - 2]);
      r = vmax(r, fsrc[bx - 1]);
      r = vmax(r, fsrc[bx]);
      r = vmax(r, fsrc[bx + 1]);
      r = vmax(r, fsrc[bx + 2]);
      fsrc += sstr;
      r = vmax(r, fsrc[bx - 1]);
      r = vmax(r, fsrc[bx]);
      r = vmax(r, fsrc[bx + 1]);
      fsrc += sstr;
      r = vmax(r, fsrc[bx]);

      fsrc -= (sstr << 2);

      dst[bx] = r;
    }
    dst += dstr;
    fsrc += sstr;
  }
}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION 
