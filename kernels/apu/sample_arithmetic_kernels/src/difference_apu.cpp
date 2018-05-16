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
* \file apu_difference_kernel_apu.c
* \brief Computes differences between images
* \author Anca Dima
* \version
* \date
****************************************************************************/

// In order to compile it as CPP do following: 
// In Visual Studio->Solution Explorer->Project "convolution_Kernel", right click -> Properties -> C/C++ ->Advanced->CompileAs->Compile As C++

#include "difference_apu.h"

#ifdef APEX2_EMULATE
#include "apu_cond.hpp"
#endif

void difference_filter_in08u_out16s(vec16s* dst, vec08u* srcA, vec08u* srcB, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
   vec08u* fsrcA = srcA;
   vec08u* fsrcB = srcB;
   vec16s* out = dst;
   for (int16s by = 0; by < bh; ++by)  // y data row
   {
	   for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
	   {
		   vec16s tmpA = (vec16s)fsrcA[bx];
		   vec16s tmpB = (vec16s)fsrcB[bx];
		   out[bx] = tmpA - tmpB;
	   }

		out += outStrideWidth;
		fsrcA += inStrideWidth;
		fsrcB += inStrideWidth;

   }	
} 





void difference_filter_in16s_out16s(vec16s* dst, vec16s* srcA, vec16s* srcB, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec16s* fsrcA = srcA;
	vec16s* fsrcB = srcB;
	vec16s* out = dst;
	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			out[bx] = fsrcA[bx] - fsrcB[bx];
		}
		out += outStrideWidth;
		fsrcA += inStrideWidth;
		fsrcB += inStrideWidth;

	}
}


void difference_filter_in16s_out32s(vec32s* dst, vec16s* srcA, vec16s* srcB, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec16s* fsrcA = srcA;
	vec16s* fsrcB = srcB;
	vec32s* out = dst;
	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			out[bx] = ((vec32s) fsrcA[bx]) - ((vec32s)fsrcB[bx]);
		}
		out += outStrideWidth;
		fsrcA += inStrideWidth;
		fsrcB += inStrideWidth;

	}
}



void difference_filter_in32s_out32s(vec32s* dst, vec32s* srcA, vec32s* srcB, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32s* fsrcA = srcA;
	vec32s* fsrcB = srcB;
	vec32s* out = dst;
	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			out[bx] = fsrcA[bx] - fsrcB[bx];
		}
		out += outStrideWidth;
		fsrcA += inStrideWidth;
		fsrcB += inStrideWidth;

	}
}


void difference_filter_in32u_out32s(vec32s* dst, vec32u* srcA, vec32u* srcB, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32u* fsrcA = srcA;
	vec32u* fsrcB = srcB;
	vec32s* out = dst;
	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			out[bx] = vec32s(fsrcA[bx]) - vec32s(fsrcB[bx]);
		}
		out += outStrideWidth;
		fsrcA += inStrideWidth;
		fsrcB += inStrideWidth;

	}
}


void difference_filter_in32s_out64s(vec32s* dst_high, vec32u* dst_low, vec32s* srcA, vec32s* srcB, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32s* fsrcA = srcA;
	vec32s* fsrcB = srcB;
	vec32s* out_high = dst_high;
	vec32u* out_low = dst_low;
	vbool getCarry;
	vbool hasCarry;


	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			getCarry = false;
			hasCarry;
			vec32s tmpLow = vsubx(fsrcA[bx], fsrcB[bx], getCarry, hasCarry);
			out_low[bx] = vec32u(tmpLow);
			
			vif (vabs(fsrcA[bx]) > vabs(fsrcB[bx]) ){
				out_high[bx] = vec32s(hasCarry);
			} velse{
				out_high[bx] = -vec32s(hasCarry);
			}vendif
		}

		out_high += outStrideWidth;
		out_low += outStrideWidth;
		fsrcA += inStrideWidth;
		fsrcB += inStrideWidth;

	}
}

void difference_filter_in64u_out64s(vec32s* dst_high, vec32u* dst_low, vec32u* srcA_high, vec32u* srcA_low, vec32u* srcB_high, vec32u* srcB_low, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32u* fsrcA_high = srcA_high;
	vec32u* fsrcB_high = srcB_high;
	vec32s* out_high = dst_high;

	vec32u* fsrcA_low = srcA_low;
	vec32u* fsrcB_low = srcB_low;
	vec32u* out_low = dst_low;

	vbool getCarry;
	vbool hasCarry;

	
	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			getCarry = false;
			hasCarry;
			out_low[bx] = vsubx(fsrcA_low[bx], fsrcB_low[bx], getCarry, hasCarry);
			out_high[bx] = (vec32s) vsubx(fsrcA_high[bx], fsrcB_high[bx], hasCarry, getCarry);
		}
		out_low += outStrideWidth;
		fsrcA_low += inStrideWidth;
		fsrcB_low += inStrideWidth;

		out_high += outStrideWidth;
		fsrcA_high += inStrideWidth;
		fsrcB_high += inStrideWidth;

	}
}


void difference_filter_in64s_out64s(vec32s* dst_high, vec32u* dst_low, vec32s* srcA_high, vec32u* srcA_low, vec32s* srcB_high, vec32u* srcB_low, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32s* fsrcA_high = srcA_high;
	vec32s* fsrcB_high = srcB_high;
	vec32s* out_high = dst_high;

	vec32u* fsrcA_low = srcA_low;
	vec32u* fsrcB_low = srcB_low;
	vec32u* out_low = dst_low;

	vbool getCarry;
	vbool hasCarry;

	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			getCarry = false;
			hasCarry;

			out_low[bx] = vsubx(fsrcA_low[bx], fsrcB_low[bx], getCarry, hasCarry);
			out_high[bx] = vsubx(fsrcA_high[bx], fsrcB_high[bx], hasCarry);
		}
		out_low += outStrideWidth;
		fsrcA_low += inStrideWidth;
		fsrcB_low += inStrideWidth;

		out_high += outStrideWidth;
		fsrcA_high += inStrideWidth;
		fsrcB_high += inStrideWidth;

	}
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
