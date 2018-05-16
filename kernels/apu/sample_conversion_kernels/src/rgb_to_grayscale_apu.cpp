
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
* \file rgb_to_grayscale_apu.cpp
* \brief rgb_to_grayscale implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/
#ifdef ACF_KERNEL_IMPLEMENTATION

#include "rgb_to_grayscale_apu.h"

/*****************************************************************************
* functions
*****************************************************************************/

void rgb_to_grayscale(vec08u* apDest, const vec08u* apcSrc,
  int aBlockWidth, int aBlockHeight, int aOutputSpan, int aInputSpan)
{
  for (int16s y = 0; y < aBlockHeight; ++y)
  {
    int16s x3 = 0;
    for (int16s x = 0; x < aBlockWidth; ++x)
    {
      vec16u accum = 27 * vec16u(apcSrc[x3]) + 92 * vec16u(apcSrc[x3 + 1]) + 9 * vec16u(apcSrc[x3 + 2]);
      apDest[x] = vec08u(accum >> 7);
      x3 += 3;
    }

    apDest += aOutputSpan;
    apcSrc += aInputSpan;
  }
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION 
