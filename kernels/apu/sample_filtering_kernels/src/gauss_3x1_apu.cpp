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
* \file gauss_3x1_apu.cpp
* \brief add implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/

#ifdef APEX2_EMULATE
#include "apu_vec.hpp"
#endif

#include "gauss_3x1_apu.h"

/*****************************************************************************
* functions
*****************************************************************************/

void gauss_3x1(vec08u* dst, vec08u* srcImage0, int bw, int bh, int inStrideW, int outStrideW)
{
	vec08u a[3];

	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			a[0] = srcImage0[bx - 1] >> 2;
			a[1] = srcImage0[bx] >> 1;
			a[2] = srcImage0[bx + 1] >> 2;
			//a[2] = srcImage0[bx + 1] >> 2;
			//a[4] = srcImage0[bx + 1] >> 3;
			dst[bx] = a[0] + a[1] + a[2];// + a[3] + a[4];

		}

		dst += outStrideW;
		srcImage0 += inStrideW;
	}
}


#endif //#ifdef ACF_KERNEL_IMPLEMENTATION 
