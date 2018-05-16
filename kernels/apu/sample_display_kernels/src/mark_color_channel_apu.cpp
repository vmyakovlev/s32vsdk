
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
* \file mark_color_channel_apu.cpp
* \brief mark_color_channel implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/
#ifdef ACF_KERNEL_IMPLEMENTATION

#include "mark_color_channel_apu.h"

/*****************************************************************************
* functions
*****************************************************************************/

void mark_color_channel(vec08u* dst, vec08u* srcImage, vec08u* srcMarker, int bw, int bh, int08u channel, int inStride, int inMarkerStride, int outStride)
{
  for (int by = 0; by < bh; ++by)
  {
    int x3 = 0;
    for (int bx = 0; bx < bw; ++bx)
    {
      vec08u res[3];
      res[0] = srcImage[x3];
      res[1] = srcImage[x3 + 1];
      res[2] = srcImage[x3 + 2];
      vec08u marker = srcMarker[bx];
      vif(marker != 0)
      {
        res[channel] = marker;
      }
      vendif;
      dst[x3] = res[0];
      dst[x3 + 1] = res[1];
      dst[x3 + 2] = res[2];

      x3 += 3;
    }
    dst += outStride;
    srcImage += inStride;
    srcMarker += inMarkerStride;
  }
}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION 
