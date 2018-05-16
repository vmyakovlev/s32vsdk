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
* \file upsample_apu.cpp
* \brief upsample implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/
#ifdef ACF_KERNEL_IMPLEMENTATION


#include "upsample_apu.h"

/*****************************************************************************
* functions
*****************************************************************************/

static inline vec08u avg2(vec08u v0, vec08u v1)
{
  vec16u sum = (vec16u)v0 + (vec16u)v1;
  return vec08u(sum >> 1);
}

static inline vec08u avg4(vec08u v0, vec08u v1, vec08u v2, vec08u v3)
{
	vec16u sum = (vec16u)v0 + (vec16u)v1 + (vec16u)v2 + (vec16u)v3;
	return vec08u(sum >> 2);
}

void upsample(vec08u* apDest, const vec08u* apcSrc,
	int aInBlockWidth, int aInBlockHeight,
	int aInBlockStride, int aOutBlockStride)
{
  for (int16s y = 0; y < aInBlockHeight; ++y)
  {
    for (int16s x = 0; x < aInBlockWidth; ++x)
    {
      vec08u x1y1 = apcSrc[x];
      vec08u x2y1 = apcSrc[x + 1];
      vec08u x1y2 = apcSrc[x + aInBlockStride];
      vec08u x2y2 = apcSrc[x + 1 + aInBlockStride];
      apDest[(x << 1)] = x1y1;
      apDest[(x << 1) + 1] = avg2(x1y1, x2y1);
      apDest[(x << 1) + aOutBlockStride] = avg2(x1y1, x1y2);
      apDest[(x << 1) + aOutBlockStride + 1] = avg4(x1y1, x2y1, x1y2, x2y2);
    }
    apDest += (aOutBlockStride << 1);
    apcSrc += aInBlockStride;
  }
}


#endif //#ifdef ACF_KERNEL_IMPLEMENTATION 
