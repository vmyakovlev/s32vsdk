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
* \file gauss_3x3_apu.cpp
* \brief 3x3 Gauss filter
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "gauss_3x3_apu.h"

//TODO: separable?
void apu_gauss_3x3(vec08u* apOut, const vec08u* apcIn,
 int aOutStride, int aInStride,
 int aTileWidth, int aTileHeight)
{
  for (int y = 0; y < aTileHeight; ++y)
  {
    for (int x = 0; x < aTileWidth; ++x)
    {
//use commented code in Target > 12R2
/*      vec16u v =      (vec16u)apcIn[x - aInStride - 1] + 2 * (vec16u)apcIn[x - aInStride] +     (vec16u)apcIn[x - aInStride + 1] +
                  2 * (vec16u)apcIn[x             - 1] + 4 * (vec16u)apcIn[x            ] + 2 * (vec16u)apcIn[x             + 1] +
                      (vec16u)apcIn[x + aInStride - 1] + 2 * (vec16u)apcIn[x + aInStride] +     (vec16u)apcIn[x + aInStride + 1];*/
                 
      vec16u v0 = (vec16u)apcIn[x - aInStride - 1];
      vec16u v1 = (vec16u)apcIn[x - aInStride    ];
      vec16u v2 = (vec16u)apcIn[x - aInStride + 1];
      
      vec16u v3 = (vec16u)apcIn[x             - 1];
      vec16u v4 = (vec16u)apcIn[x                ];
      vec16u v5 = (vec16u)apcIn[x             + 1];
      
      vec16u v6 = (vec16u)apcIn[x + aInStride - 1];
      vec16u v7 = (vec16u)apcIn[x + aInStride    ];
      vec16u v8 = (vec16u)apcIn[x + aInStride + 1];
      
      vec16u v =  v0     + (v1 << 1) + v2
                + (v3 << 1) + (v4 << 2) + (v5 << 1)
                + v6     + (v7 << 1) + v8;

      //div 16
      apOut[x] = vec08u(v >> 4);
    }

    apcIn += aInStride;
    apOut += aOutStride;
  }
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
