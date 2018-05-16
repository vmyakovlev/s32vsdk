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
* \file apu_add_apu.cpp
* \brief add implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/

#ifdef APEX2_EMULATE
#include "apu_vec.hpp"
#endif

#include "add_apu.h"

/*****************************************************************************
* functions
*****************************************************************************/

void add(vec16u* dst, vec08u* srcImage0, vec08u* srcImage1, int bw, int bh, int inStrideW, int outStrideW)
{
	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			vec16u a = (vec16u) srcImage0[bx];
			vec16u b = (vec16u)srcImage1[bx];
			dst[bx] = a + b;
		}
		dst += outStrideW;
		srcImage0 += inStrideW;
		srcImage1 += inStrideW;
	}
}

void add_in16s_out32s(vec32s* dst, vec16s* srcImage0, vec16s* srcImage1, int bw, int bh, int inStrideW, int outStrideW)
{
	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			vec32s a = (vec32s) srcImage0[bx];
			vec32s b = (vec32s) srcImage1[bx];
			dst[bx] = a + b;
		}
		dst += outStrideW;
		srcImage0 += inStrideW;
		srcImage1 += inStrideW;
	}
}


void add_in32s_out32s(vec32s* dst, vec32s* srcImage0, vec32s* srcImage1, int bw, int bh, int inStrideW, int outStrideW)
{
	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			dst[bx] = srcImage0[bx] + srcImage1[bx];
		}
		dst += outStrideW;
		srcImage0 += inStrideW;
		srcImage1 += inStrideW;
	}
}


void add_in32u_out32u(vec32u* dst, vec32u* srcImage0, vec32u* srcImage1, int bw, int bh, int inStrideW, int outStrideW)
{
	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			dst[bx] = srcImage0[bx] + srcImage1[bx];
		}
		dst += outStrideW;
		srcImage0 += inStrideW;
		srcImage1 += inStrideW;
	}
}

void add_in32Q3_28_out32Q3_28(vec32s* dstInt, vec32s* dstFrac, vec32s* srcImage0, vec32s* srcImage1, int bw, int bh, int inStrideW, int outStrideW)
{
	const int32s shiftVal = 3;
	const vec32s shiftValPl1 = shiftVal +1;
	const vec32s intShiftVal = 31 - shiftVal;
	const vec32s upperBitMask = 0x7fffffff;
	const vec32s fracThreshold = upperBitMask >> shiftVal; // for Q3.28 format


	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			vbool isCarry = false;
			vbool hasCarry;
			
			vec32s tmpRes;
			tmpRes = vaddx(srcImage0[bx], srcImage1[bx], isCarry, hasCarry);
			vec32s absTmpRes = (vec32s) vabs(tmpRes);
			dstInt[bx] = 0;
			dstFrac[bx] = tmpRes & fracThreshold;

			vif(hasCarry == true || absTmpRes > fracThreshold) {
				vec32s tmpHasCarry = vec32s(hasCarry) << shiftValPl1;
				dstFrac[bx] = absTmpRes & fracThreshold;

				vif(srcImage0[bx] >= 0 && srcImage1[bx] >= 0 || srcImage0[bx] < 0 && srcImage1[bx] < 0){
						vif(srcImage0[bx] < 0){
							tmpHasCarry = -tmpHasCarry;
							dstFrac[bx] = -dstFrac[bx];
						}vendif
				} velse {
					vec32s absV = (vec32s) vabs(srcImage1[bx]);
					vif(srcImage0[bx] >= 0 && srcImage1[bx] < 0 && absV > srcImage0[bx]) {
						tmpHasCarry = -tmpHasCarry;
						dstFrac[bx] = -dstFrac[bx];
					} vendif
					
					absV = (vec32s)vabs(srcImage0[bx]);
					vif(srcImage0[bx] < 0 && srcImage1[bx] >= 0 && absV > srcImage1[bx]){
						tmpHasCarry = -tmpHasCarry;
						dstFrac[bx] = -dstFrac[bx];
					} vendif
				} vendif
				
				dstInt[bx] = (tmpRes >> intShiftVal) + tmpHasCarry;
			} vendif
		}
		dstInt += outStrideW;
		dstFrac += outStrideW;
		srcImage0 += inStrideW;
		srcImage1 += inStrideW;
	}
}

void add_in64s_out64s(vec32s* dst_high, vec32u* dst_low, vec32s* srcImage0_high, vec32u* srcImage0_low, vec32s* srcImage1_high, vec32u* srcImage1_low, int bw, int bh, int inStrideW, int outStrideW)
{

	vbool getCarry;
	vbool hasCarry;

	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) chess_prepare_for_pipelining// x in the blck_tile
		{
			getCarry = false;
			hasCarry;
			
			dst_low[bx] = vaddx(srcImage0_low[bx], srcImage1_low[bx], getCarry, hasCarry);
			dst_high[bx] = vaddx(srcImage0_high[bx], srcImage1_high[bx], hasCarry);
			
		}

		dst_high += outStrideW;
		dst_low += outStrideW;

		srcImage0_high += inStrideW;
		srcImage1_high += inStrideW;

		srcImage0_low += inStrideW;
		srcImage1_low += inStrideW;

	}
	}


void add_in64u_out64u(vec32u* dst_high, vec32u* dst_low, vec32u* srcImage0_high, vec32u* srcImage0_low, vec32u* srcImage1_high, vec32u* srcImage1_low, int bw, int bh, int inStrideW, int outStrideW)
{

	vbool getCarry;
	vbool hasCarry;

	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) chess_prepare_for_pipelining// x in the blck_tile
		{
			getCarry = false;
			hasCarry;

			dst_low[bx] = vaddx(srcImage0_low[bx], srcImage1_low[bx], getCarry, hasCarry);
			dst_high[bx] = vaddx(srcImage0_high[bx], srcImage1_high[bx], hasCarry);
		}

		dst_high += outStrideW;
		dst_low += outStrideW;

		srcImage0_high += inStrideW;
		srcImage1_high += inStrideW;

		srcImage0_low += inStrideW;
		srcImage1_low += inStrideW;

	}
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION 
