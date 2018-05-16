
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
* \file downsample_apu.cpp
* \brief downsample implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/
#ifdef ACF_KERNEL_IMPLEMENTATION

#include "downsample_apu.h"

/*****************************************************************************
* functions
*****************************************************************************/

void downsample(vec08u* apDest, const vec08u* apcSrc,
	int aOutBlockWidth, int aOutBlockHeight,
	int aInBlockStride, int aOutBlockStride)
{
	int aInBlockStep = aInBlockStride << 1;

	for (int16s y = 0; y < aOutBlockHeight; ++y)
	{
		for (int16s x = 0; x < aOutBlockWidth; ++x)
		{
			vec08u lt = apcSrc[(x << 1)];
			vec08u rt = apcSrc[(x << 1) + 1];
			vec08u lb = apcSrc[(x << 1) + aInBlockStride];
			vec08u rb = apcSrc[(x << 1) + 1 + aInBlockStride];
			vec16u acc = (vec16u)lt + (vec16u)rt + (vec16u)lb + (vec16u)rb;
			acc >>= 2;
			apDest[x] = vec08u(acc);
		}

		apDest += aOutBlockStride;
		apcSrc += aInBlockStep;
	}
}


void downsample_16u(vec16u* apDest, const vec16u* apcSrc,
	int /*aInBlockWidth*/, int /*aInBlockHeight*/,
	int aOutBlockWidth, int aOutBlockHeight,
	int aInBlockStride, int aOutBlockStride)
{
	int aInBlockStep = aInBlockStride << 1;
	for (int16s y = 0; y < aOutBlockHeight; ++y)
	{
		for (int16s x = 0; x < aOutBlockWidth; ++x)
		{
			vec16u lt = apcSrc[(x << 1)];
			vec16u rt = apcSrc[(x << 1) + 1];
			vec16u lb = apcSrc[(x << 1) + aInBlockStride];
			vec16u rb = apcSrc[(x << 1) + 1 + aInBlockStride];
			vec32u acc = (vec32u)lt + (vec32u)rt + (vec32u)lb + (vec32u)rb;
			acc >>= 2;
			apDest[x] = (vec16u) acc;
		}

		apDest += aOutBlockStride;
		apcSrc += aInBlockStep;
	}
}

void downsample_gauss(vec08u* apDest, const vec08u* apcIn,
	int32s aOutBlockWidth, int32s aOutBlockHeight,
	int32s aInBlockStride, int32s aOutBlockStride) {


	for (int16s y = 0; y < aOutBlockHeight; ++y)
	{
		for (int16s x = 0; x < aOutBlockWidth; ++x)
		{
			int16s srcX = x << 1;
      vec16u v0 = (vec16u)apcIn[srcX - aInBlockStride - 1];
      vec16u v1 = (vec16u)apcIn[srcX - aInBlockStride];
      vec16u v2 = (vec16u)apcIn[srcX - aInBlockStride + 1];

			vec16u v3 = (vec16u)apcIn[srcX - 1];
			vec16u v4 = (vec16u)apcIn[srcX];
			vec16u v5 = (vec16u)apcIn[srcX + 1];

      vec16u v6 = (vec16u)apcIn[srcX + aInBlockStride - 1];
      vec16u v7 = (vec16u)apcIn[srcX + aInBlockStride];
      vec16u v8 = (vec16u)apcIn[srcX + aInBlockStride + 1];

			vec16u v = v0 + (v1 << 1) + v2 
					+ (v3 << 1) + (v4 << 2) + (v5 << 1)
					+ v6 + (v7 << 1) + v8;

			//div 16
			apDest[x] = vec08u(v >> 4);
		}

		apDest += aOutBlockStride;
		apcIn += aInBlockStride << 1;
	}

}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION 
