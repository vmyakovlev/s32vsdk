#ifdef ACF_KERNEL_IMPLEMENTATION

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
* \file apu_max_apu.cpp
* \brief max implementation for APEX
* \author Rostislav Hulik
* \version
* \date
****************************************************************************/

#include "max_apu.h"

/*****************************************************************************
* functions
*****************************************************************************/

void max(vec08u* dst, vec08u* srcImage0, vec08u* srcImage1, int bw, int bh, int inStrideW, int outStrideW)
{
	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			vec08u a = srcImage0[bx];
			vec08u b = srcImage1[bx];
			vif(a > b) 
			{
			    dst[bx] = a;
			}
			velse
			{
			    dst[bx] = b;
			}
			vendif;
		}
		dst += outStrideW;
		srcImage0 += inStrideW;
		srcImage1 += inStrideW;
	}
}

#endif
