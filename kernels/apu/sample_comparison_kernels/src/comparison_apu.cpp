
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
* \file comparison_apu.cpp
* \brief compare functions implementation for APEX
* \author Anca Dima
* \version
* \date
****************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "comparison_apu.h"

#ifdef APEX2_EMULATE
#include "apu_vec.hpp"
#endif

/*****************************************************************************
* functions
*****************************************************************************/

void lower(vbool* dst, vec08u* srcImage0, vec08u* srcImage1, int bw, int bh, int inStrideW, int outStrideW)
{
	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			dst[bx] = srcImage0[bx] < srcImage1[bx];
		}
		dst += outStrideW;
		srcImage0 += inStrideW;
		srcImage1 += inStrideW;
	}
}


void lower_scalar(vec08u* dst, vec08u* srcImage, unsigned char scalar, int bw, int bh, int inStrideW, int outStrideW)
{
	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			dst[bx] = vec08u(srcImage[bx] < scalar);
		}
		dst += outStrideW;
		srcImage += inStrideW;		
	}
}


void lower_in16s(vbool* dst, vec16s* srcImage0, vec16s* srcImage1, int bw, int bh, int inStrideW, int outStrideW)
{
	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			dst[bx] = srcImage0[bx] < srcImage1[bx];
		}
		dst += outStrideW;
		srcImage0 += inStrideW;
		srcImage1 += inStrideW;
	}
}


void lower_in32s(vbool* dst, vec32s* srcImage0, vec32s* srcImage1, int bw, int bh, int inStrideW, int outStrideW)
{
	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			dst[bx] = srcImage0[bx] < srcImage1[bx];
		}
		dst += outStrideW;
		srcImage0 += inStrideW;
		srcImage1 += inStrideW;
	}
}




void absLower_in32s(vbool* dst, vec32s* srcImage0, vec32s* srcImage1, int bw, int bh, int inStrideW, int outStrideW)
{
	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			dst[bx] = vabs(srcImage0[bx]) < vabs(srcImage1[bx]);
		}
		dst += outStrideW;
		srcImage0 += inStrideW;
		srcImage1 += inStrideW;
	}
}



void absLower_in32s_scalar16u(vbool* dst, vec32s* srcImage0, int16u compVal, int bw, int bh, int inStrideW, int outStrideW)
{
	vec32s compVect = (int32s)compVal;
	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			dst[bx] = vabs(srcImage0[bx]) < compVect;
		}
		dst += outStrideW;
		srcImage0 += inStrideW;
	}
}


void lower_in64s(vbool* dst, vec32s* srcImage0_high, vec32u* srcImage0_low, vec32s* srcImage1_high, vec32u* srcImage1_low, int bw, int bh, int inStrideW, int outStrideW)
{

	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			dst[bx] = (srcImage0_high[bx] < srcImage1_high[bx]) || ((srcImage0_high[bx] == srcImage1_high[bx]) && (srcImage0_low[bx] < srcImage1_low[bx]));
		}

		dst += outStrideW;

		srcImage0_high += inStrideW;
		srcImage1_high += inStrideW;
				
		srcImage0_low += inStrideW;
		srcImage1_low += inStrideW;
	}
}


void lower_in64u(vbool* dst, vec32u* srcImage0_high, vec32u* srcImage0_low, vec32u* srcImage1_high, vec32u* srcImage1_low, int bw, int bh, int inStrideW, int outStrideW)
{

	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			dst[bx] = (srcImage0_high[bx] < srcImage1_high[bx]) || ((srcImage0_high[bx] == srcImage1_high[bx]) && (srcImage0_low[bx] < srcImage1_low[bx]));
		}

		dst += outStrideW;

		srcImage0_high += inStrideW;
		srcImage1_high += inStrideW;

		srcImage0_low += inStrideW;
		srcImage1_low += inStrideW;

	}
}


void lowerEqual_in32s(vbool* dst, vec32s* srcImage0, vec32s* srcImage1, int bw, int bh, int inStrideW, int outStrideW)
{
	
	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			dst[bx] = srcImage0[bx] <= srcImage1[bx];
		}

		dst += outStrideW;
		srcImage0 += inStrideW;
		srcImage1 += inStrideW;
	}
}

void and_in08u_out16u(vec16u* dst, vec08u* srcImage0, vec08u* srcImage1, int bw, int bh, int inStrideW, int outStrideW) {


	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			dst[bx] = vec16u((srcImage0[bx] != 0) && (srcImage1[bx] != 0));
		}

		dst += outStrideW;
		srcImage0 += inStrideW;
		srcImage1 += inStrideW;
	}
}


void and_in08u_in16u_out16u(vec16u* dst, vec08u* srcImage0, vec16u* srcImage1, int bw, int bh, int inStrideW, int outStrideW) {


	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			dst[bx] = vec16u( (srcImage0[bx] != 0) && (srcImage1[bx] != 0));
		}

		dst += outStrideW;
		srcImage0 += inStrideW;
		srcImage1 += inStrideW;
	}
}


void mask_kn(vec08u* dst, vec08u* srcImage, vec08u* srcMask, int bw, int bh, int inStrideW, int outStrideW) {


	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			dst[bx] = (vec08u) vselect(srcImage[bx], (vec08u)0, srcMask[bx] != 0);
		}

		dst += outStrideW;
		srcImage += inStrideW;
		srcMask += inStrideW;
	}
}

void and_kn(vec08u* dst, vec08u* srcImage0, vec08u* srcImage1, int bw, int bh, int inStrideW, int outStrideW) {


	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			dst[bx] = vec08u((srcImage0[bx] != 0) && (srcImage1[bx] != 0));
		}

		dst += outStrideW;
		srcImage0 += inStrideW;
		srcImage1 += inStrideW;
	}
}

void and_in16u_out16u(vec16u* dst, vec16u* srcImage0, vec16u* srcImage1, int bw, int bh, int inStrideW, int outStrideW) {


	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			dst[bx] = vec16u((srcImage0[bx] != 0) && (srcImage1[bx] != 0) );
		}

		dst += outStrideW;
		srcImage0 += inStrideW;
		srcImage1 += inStrideW;
	}
}


void and_3Pt_in16u_out16u(vec16u* dst, vec16u* srcImage0, vec16u* srcImage1, vec16u* srcImage2, int bw, int bh, int inStrideW, int outStrideW) {


	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			dst[bx] = vec16u((srcImage0[bx] != 0) && (srcImage1[bx] != 0) && (srcImage2[bx] != 0));
		}

		dst += outStrideW;
		srcImage0 += inStrideW;
		srcImage1 += inStrideW;
		srcImage2 += inStrideW;
	}
}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION 

