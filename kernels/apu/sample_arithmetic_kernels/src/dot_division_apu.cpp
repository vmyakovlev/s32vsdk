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
* \file apu_dot_division_kernel_apu.c
* \brief Elementwise division of two vectors/matrices. If divisor is zero, a value of zero is returned (in order not to influence following arithmetics).
* \author Anca Dima
* \version
* \date
****************************************************************************/

// In order to compile it as CPP do following: 
// In Visual Studio->Solution Explorer->Project "convolution_Kernel", right click -> Properties -> C/C++ ->Advanced->CompileAs->Compile As C++

#ifdef APEX2_EMULATE
#include "apu_cond.hpp"
#include "apu_vec.hpp"
#endif

#include "dot_division_apu.h"
#include "dot_multiplic_apu.h"

void dot_division_filter(vec32s* dst, vec32s* srcA, vec32s* srcB, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
   vec32s* fsrcA = srcA;
   vec32s* fsrcB = srcB;
   vec32s* out = dst;
   for (int16s by = 0; by < bh; ++by)  // y data row
   {
	   for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
	   {
		   vif(fsrcB[bx] != 0) {
			   out[bx] = fsrcA[bx] / fsrcB[bx];
		   } velse{
			   out[bx] = 0; // or should it rather be MAX_INT???
		   }
		   vendif
	   }

	   out += outStrideWidth;
	   fsrcA += inStrideWidth;
	   fsrcB += inStrideWidth;
   }	
} 


// Computes the log_2 of all the elements of the input
void computeLog2(vec08u* log2Fact, vec32s* input, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32s *crtinput = input;
	vec08u  *crtLog2Fact = log2Fact;
	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			crtLog2Fact[bx] = 0; 
			vec32u crtDown = vabs(crtinput[bx]);
			vbool largerThanOne = crtDown >= 1;
			// Find out the binary logarithm of each inputisor in the vector
			while (vany(largerThanOne)) {
				vif(largerThanOne){
					crtDown >>= 1;
					crtLog2Fact[bx] ++;
					largerThanOne = crtDown >= 1;
				} vendif
			}
		}
		crtinput += inStrideWidth;
		crtLog2Fact += outStrideWidth;
	}
}

void computeLog2u(vec08u* log2Fact, vec32u* input, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32u *crtinput = input;
	vec08u  *crtLog2Fact = log2Fact;
	vec32u upperBitZero = 0x7FFFFFFF;

	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			crtLog2Fact[bx] = 0;
			vec32u crtDown = crtinput[bx];
			vbool largerThanOne = crtDown >= 1;
			
			// Find out the binary logarithm of each inputisor in the vector
			while (vany(largerThanOne)) {
				vif(largerThanOne){
					crtDown = (crtDown >> 1) & upperBitZero; // delete the sign bit after shifting, if there was one, since it is an unsigned value					
					crtLog2Fact[bx]++;
					largerThanOne = crtDown >= 1;
				} vendif
			}
		}
		crtinput += inStrideWidth;
		crtLog2Fact += outStrideWidth;
	}
}


// Computes the log_2 of all the elements of the input
void compute64bitLog2(vec08u* log2Fact, vec32s* input_high, vec32u* input_low, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32s *crtInput_high = input_high;
	vec32u *crtInput_low = input_low;
	vec08u  *crtLog2Fact = log2Fact;
	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			crtLog2Fact[bx] = 0;

			vec32s th = crtInput_high[bx];
			vec32u tl = crtInput_low[bx];
			vif(th < 0) {
				change64bitSign(th, tl);
			} vendif

			vec32u thu = vec32u(th);
			vif(thu > 0){
				computeLog2u(&(crtLog2Fact[bx]), &thu, 1, 1, 1, 1);
			} vendif

			vif(crtLog2Fact[bx] > 0)
			{
				crtLog2Fact[bx] += 32;
			} velse {
				computeLog2u(&(crtLog2Fact[bx]), &(crtInput_low[bx]), 1, 1, 1, 1);
			} vendif
		}
		crtInput_high += inStrideWidth;
		crtInput_low += inStrideWidth;
		crtLog2Fact += outStrideWidth;
	}
}



// Computes the log_2 of all the elements of the input
void compute64bitLog2u(vec08u* log2Fact, vec32u* input_high, vec32u* input_low, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32u *crtInput_high = input_high;
	vec32u *crtInput_low = input_low;
	vec08u  *crtLog2Fact = log2Fact;
	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			crtLog2Fact[bx] = 0;

			vif(crtInput_high[bx] > 0){
				computeLog2u(&(crtLog2Fact[bx]), &crtInput_high[bx], 1, 1, 1, 1);
			} vendif

			vif(crtLog2Fact[bx] > 0)
			{
					crtLog2Fact[bx] += 32;
			} velse{
				computeLog2u(&(crtLog2Fact[bx]), &(crtInput_low[bx]), 1, 1, 1, 1);
			} vendif
		}
		crtInput_high += inStrideWidth;
		crtInput_low += inStrideWidth;
		crtLog2Fact += outStrideWidth;
	}
}


// Computes the inverse of the divisor in Q3.28 with the Newton-Raphson method
void computeInv_NewtonRaphson(vec32s* invDiv, vec32s* div, vec08u* log2Fact, const int08s shiftFact, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32s *crtDiv = div;
	vec32s *crtInvDiv = invDiv;
	vec08u  *crtlog2Fact = log2Fact;
	//	const int08s shiftFact = 3; // Q3.28 (integer part of the number < 8)

	int initialShift = 28;
	vec32s _48To17 = 757935405;//48/17 = 2.8 * (2^28)
	vec32u _32To17 = 505290270;//32/17 = 1.88 * (2^28)

	int32s _48To17_at30 = 3031741621;
	int32s _32To17_at30 = 2021161080;
	const int08s shiftFactPl1 = shiftFact+1; // Q3.28 (integer part of the number < 8)
	const int08s QFact = 32 - shiftFactPl1; // decimal part of the number (1 << 28)
	vec08u vecQFactu = vec08u(QFact);
	const int32s low28Bits = 0x0fffffff;
	const vec32s oneShifted = 1 << QFact;
	int32s diffShift = (QFact - initialShift);
	int32s diffShiftTo30 = (30 - QFact);

	// Adapt constants to other resolutions
	if(diffShift != 0){
		if (diffShiftTo30 < 0) {
			_48To17 = _48To17_at30 << -diffShiftTo30;
			_32To17 = _32To17_at30 << -diffShiftTo30;
		}
		else {
			_48To17 = _48To17_at30 >> diffShiftTo30;
			_32To17 = _32To17_at30 >> diffShiftTo30;
		}
	} 

	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			vif(crtDiv[bx] != 1) {
				vbool hasSign = crtDiv[bx] < 0;
				vec32u cDiv_u = (vec32u)vabs(crtDiv[bx]);

				computeLog2u(&crtlog2Fact[bx], &cDiv_u, 1, 1, 1, 1);
				vec08s reprShift = vec08s(crtlog2Fact[bx]) - QFact;
				vec32u divShifted;
				vif (reprShift >= 0) {
					divShifted = cDiv_u >> (vec32u)reprShift;
				} velse {
					divShifted = cDiv_u << (vec32u)(-reprShift);
				} vendif

				vec32s divShifted_s = vec32s(divShifted);

				//Compute x0 = _48To17 - _32To17 * divShifted;				
				vec32s x0;
				vec32u x0h, x0l;
				vec32u x0_mul_high, x0_mul_low;
				//32To17 * divShifted;
				dot_mult_in32u_out64u_filter(&x0_mul_high, &x0_mul_low, &_32To17, &divShifted, 1, 1, 1, 1);

				vif(x0_mul_high > low28Bits) {
					// TBD: don't know what to do?
					rsh_in64u_out64u_filter(&x0h, &x0l, &x0_mul_high, &x0_mul_low, &vecQFactu, 1, 1, 1, 1);
				} velse {
					rsh_in64u_out64u_filter(&x0h, &x0l, &x0_mul_high, &x0_mul_low, &vecQFactu, 1, 1, 1, 1);
				} vendif

				x0 = (vec32s)x0l;

				//_48To17 - _32To17 * divShifted;
				x0 = _48To17 - x0;

				vec32s x1 = 0;
				int32s nrIt = 3; //log_2((QFact+1)/log_2(17))
				for (int32s i = 0; i < nrIt; ++i) {
					vec32s x0_mul_high, x0h_s;
					vec32u x0_mul_low;

					//Compute in the loop: x1 = x0 + x0 * (1 - x0*divShifted);				
					//x0*divShifted part
					dot_mult_in32s_out64s_filter(&x0_mul_high, &x0_mul_low, &x0, &divShifted_s, 1, 1, 1, 1);

					vif (x0_mul_high  > low28Bits) {
						// TBD: don't know what to do?
						rsh_in64s_out64s_filter(&x0h_s, &x0l, &x0_mul_high, &x0_mul_low, &vecQFactu, 1, 1, 1, 1);
					} velse {
						rsh_in64s_out64s_filter(&x0h_s, &x0l, &x0_mul_high, &x0_mul_low, &vecQFactu, 1, 1, 1, 1);
					} vendif

					x0_mul_low = x0l;

					// (1 - x0*divShifted) part
					vec32s diffWithOne = oneShifted - vec32s(x0_mul_low);
					vec32s x0MulDiff_high;
					vec32u x0MulDiff_low;

					//  x0 * (1 - x0*divShifted)
					dot_mult_in32s_out64s_filter(&x0MulDiff_high, &x0MulDiff_low, &x0, &diffWithOne, 1, 1, 1, 1);
					{
						vec32s x0h_s;
						vif(x0_mul_high > low28Bits) {
							// TBD: don't know what to do?
							rsh_in64s_out64s_filter(&x0h_s, &x0l, &x0MulDiff_high, &x0MulDiff_low, &vecQFactu, 1, 1, 1, 1);
						} velse {
							rsh_in64s_out64s_filter(&x0h_s, &x0l, &x0MulDiff_high, &x0MulDiff_low, &vecQFactu, 1, 1, 1, 1);
						} vendif
					}

					x1 = (vec32s)x0l;

					//x1 = x0 + x0 * (1 - x0*divShifted); 
					x1 += x0;

					x0 = x1;
				}
				crtInvDiv[bx] = (x1 >> (vec32s)crtlog2Fact[bx]); // is in Q3.28 format!

				vif(hasSign) {
					crtInvDiv[bx] = -crtInvDiv[bx];
				} vendif

			} velse {
				crtInvDiv[bx] = (1 << QFact); // 1 in Q3.28 format
			} vendif

		}
		crtDiv += inStrideWidth;
		crtlog2Fact += outStrideWidth;
		crtInvDiv += outStrideWidth;
	}
}


void dot_division_filter_N64s_D32s_Q64s(vec32s* dst_high, vec32u* dst_low, vec32u* dst_rem, vec32s* nom_high, vec32u* nom_low, vec32s* divisor, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32s* cNom_high = nom_high;
	vec32u* cNom_low = nom_low;
	vec32s* out_high = dst_high;
	vec32u* out_low = dst_low;
	vec32u* out_rem = dst_rem;
	vec32s* cDiv = divisor;

	vec32u first28Bits = 0x0FFFFFFF;

	const int32s shiftFact = 3; //Q3.28 fixed point number format, i.e. decimal part is 28 bits wide (have to divide by 28 bits)
	const int32s shiftFactPl1 = shiftFact + 1;
	const int32s QFact = 32- shiftFactPl1;

	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			vbool outSgn = hasSign(*cNom_high, *cDiv);
			vec32s div = (vec32s) vabs(cDiv[bx]);

			vif(div != 0) {
				vec32s invDiv; 
				vec08u log2Fact;
				computeInv_NewtonRaphson(&invDiv, &div, &log2Fact, shiftFact, 1, 1, 1, 1);
	
				vec32s nomHighMulInv_h;
				vec32u nomLowMulInv_h, nomHighMulInv_l, nomLowMulInv_l;
				vec32u uInvDiv = vec32u(invDiv);

				// multiply the high and the low nominator part with the inverse of the divisor (64 bit multiplications)
				dot_mult_in32s_out64s_filter(&nomHighMulInv_h, &nomHighMulInv_l, &invDiv, cNom_high, 1, 1, 1, 1);
				dot_mult_in32u_out64u_filter(&nomLowMulInv_h, &nomLowMulInv_l, &uInvDiv, cNom_low, 1, 1, 1, 1);
				
				// The 96 bit result has to be rightshifted by QFact bits (do it simplified, by shifting only the necessary shiftFactPl1 bits)
				// \warning the upmost shiftFactPl1 bits will be lost! Hopefully they are not used/set
				vbool isCarry = 0, hasCarry;
				vec32u addLowHighWithHighLow = vaddx(nomLowMulInv_h, nomHighMulInv_l, isCarry, hasCarry);
				nomHighMulInv_h += vec32s(hasCarry);

				out_low[bx] = (nomLowMulInv_l >> QFact) + (addLowHighWithHighLow << shiftFactPl1);
				out_high[bx] = vec32s(addLowHighWithHighLow >> QFact) + (nomHighMulInv_h << shiftFactPl1); // \warning if nomHighMulInv_h has the 4 highest bits set, they will be lost!
				out_rem[bx] = nomLowMulInv_l & first28Bits;

				// take care of the sign!
				vif(outSgn) {
					// the divisor was turned to positive numbers, thus only if the nominator is not signed, but the divisor was signed, turn the sign
					change64bitSign(out_high[bx], out_low[bx]);
					out_rem[bx] = ~out_rem[bx]+1; // changed to (-out_rem)
				} vendif

			} velse{
				out_high[bx] = 0; // or should it rather be MAX_INT???
				out_low[bx] = 0; // or should it rather be MAX_INT???
				out_rem[bx] = 0;
			}
			vendif
		}

		out_high += outStrideWidth;
		out_low += outStrideWidth;
		out_rem += outStrideWidth;
		cNom_high += inStrideWidth;
		cNom_low += inStrideWidth;
		cDiv += inStrideWidth;
	}
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
