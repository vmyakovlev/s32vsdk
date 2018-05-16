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
* \file nms_apu.cpp
* \brief Non-maximum supression filter implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "nms_apu.h"

void apu_nms_impl(const vec08u* apcIn, vec08u* apOut,
  int aInStride, int aOutStride,
 int aTileWidth, int aTileHeight)
{
  for (int y = 0; y < aTileHeight; ++y)
  {
    for (int x = 0; x < aTileWidth; ++x)
    {
      vec08u v = apcIn[x];

      vbool fail = v <= apcIn[x - 1 - aInStride];
      fail = fail || (v <= apcIn[x - aInStride]);
      fail = fail || (v <= apcIn[x + 1 - aInStride]);
      fail = fail || (v <= apcIn[x - 1]);
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


void apu_nms16(const vec16u* apcIn, vec16u* apOut,
  int aInStride, int aOutStride,
 int aTileWidth, int aTileHeight)
{
  for (int y = 0; y < aTileHeight; ++y)
  {
    for (int x = 0; x < aTileWidth; ++x)
    {
      vec16u v = apcIn[x];

      vbool fail = v <= apcIn[x - 1 - aInStride];
      fail = fail || (v <= apcIn[x - aInStride]);
      fail = fail || (v <= apcIn[x + 1 - aInStride]);
      fail = fail || (v <= apcIn[x - 1]);
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

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
