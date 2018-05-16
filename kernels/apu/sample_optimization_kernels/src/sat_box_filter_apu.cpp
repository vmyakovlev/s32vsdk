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
* \file sat_box_filter_apu.cpp
* \brief sat_box_filter implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/
#ifdef ACF_KERNEL_IMPLEMENTATION


#include "sat_box_filter_apu.h"
#include "add_apu.h"
#include "difference_apu.h"

/*****************************************************************************
* functions
*****************************************************************************/
#ifdef APEX2_EMULATE
extern const int BOX_SIZE;// = 5;
extern const int BOX_AREA;// = (2 * BOX_SIZE + 1) * (2 * BOX_SIZE + 1);
#else
const int BOX_SIZE = 5;
const int BOX_AREA = (2 * BOX_SIZE + 1) * (2 * BOX_SIZE + 1);
#endif

void sat_box_filter_impl(vec08u* apDest,
	const vec32u* apcSrc,
	int aBlockWidth, int aBlockHeight,
	int aSourceStride, int aDestStride)
{
	int int32SourceStride = aSourceStride >> 2;
	for (int y = 0; y < aBlockHeight; ++y)
	{
		for (int x = 0; x < aBlockWidth; ++x)
		{
			vec32u vLeftTop = apcSrc[x - (BOX_SIZE + 1) - (BOX_SIZE + 1) * int32SourceStride];
			vec32u vRgtTop = apcSrc[x + BOX_SIZE - (BOX_SIZE + 1) * int32SourceStride];
			vec32u vLeftBot = apcSrc[x - (BOX_SIZE + 1) + BOX_SIZE * int32SourceStride];
			vec32u vRgtBot = apcSrc[x + BOX_SIZE + BOX_SIZE * int32SourceStride];
			vec32u v = (vRgtBot + vLeftTop - vRgtTop - vLeftBot) / BOX_AREA;
			vif(v > 255)
			{
				v = 255;
			}
			vendif;
			apDest[x] = vec08u(v);
		}

		apcSrc += int32SourceStride;
		apDest += aDestStride;
	}
}



#endif //#ifdef ACF_KERNEL_IMPLEMENTATION 

