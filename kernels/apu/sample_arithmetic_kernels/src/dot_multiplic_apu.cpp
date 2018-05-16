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
* OF MERCHANTABILITY AND FITNESS FOR fsrcA[bx] PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY dst_high OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/
/**
* \file apu_dot_multiplic_apu.c
* \brief Elementwise multiplication of two vectors/matrices. 
* \author Anca Dima
* \version
* \date
****************************************************************************/

// In order to compile it as CPP do following: 
// In Visual Studio->Solution Explorer->Project "convolution_Kernel", right click -> Properties -> C/C++ ->Advanced->CompileAs->Compile As C++

#ifdef APEX2_EMULATE
#include "apu_cond.hpp"
#include "apu_vec.hpp"
using namespace APEX2;
#endif

#include "dot_multiplic_apu.h"


vbool hasSign(vec32s& a, vec32s& b){
	return (a < 0 && b >= 0) || (a >= 0 && b < 0);
}

// helper function for changing the sign. TBD: find a proper place for it, since it is a common purpose function 
void change64bitSign(vec32s& highWord, vec32u& lowWord) {
	lowWord = ~lowWord;
	vbool getC = false, hasC;
	vec32u ones = 1;
	lowWord = vaddx(lowWord, ones, getC, hasC);
	highWord = (~highWord) + vec32s(hasC);
}


void dot_mult_in16s_out32s_filter(vec32s* dst, vec16s* srcA, vec16s* srcB, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec16s* fsrcA = srcA;
	vec16s* fsrcB = srcB;
	vec32s* out = dst;
	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			vec32s tmpA = vec32s(fsrcA[bx]);
			vec32s tmpB = vec32s(fsrcB[bx]);
			out[bx] =  tmpA * tmpB;
		}
		out += outStrideWidth;
		fsrcA += inStrideWidth;
		fsrcB += inStrideWidth;

	}
}



void dot_mult_in32s_in16s_out32s_filter(vec32s* dst, vec32s* srcA, vec16s* srcB, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32s* fsrcA = srcA;
	vec16s* fsrcB = srcB;
	vec32s* out = dst;
	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{			
			vec32s tmpB = vec32s(fsrcB[bx]);
			out[bx] = fsrcA[bx] * tmpB;
		}
		out += outStrideWidth;
		fsrcA += inStrideWidth;
		fsrcB += inStrideWidth;

	}
}



void dot_mult_in32s_out32s_filter(vec32s* dst, vec32s* srcA, vec32s* srcB, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32s* fsrcA = srcA;
	vec32s* fsrcB = srcB;
	vec32s* out = dst;
	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			out[bx] = fsrcA[bx] * fsrcB[bx];
		}
		out += outStrideWidth;
		fsrcA += inStrideWidth;
		fsrcB += inStrideWidth;

	}
}



void dot_mult_in32u_out64u_filter(vec32u* dst_high, vec32u* dst_low, vec32u* srcA, vec32u* srcB, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{

	vec32u* fsrcA = srcA;
	vec32u* fsrcB = srcB;

	vec32u lowerWordMask = 0x0000FFFF;
	vec32u vScalingFact = 16; // number of shifts to the right, i.e. scaling fact is 1/(2^16)


	// temporary variables needed inside the loop
	vec32u highA;
	vec32u highB;
	vec32u lowA;
	vec32u lowB;
	vec32u term1;
	vec32u term2;
	vec32u term3, term3_high, term3_low;


	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			vec32s altHigh;
			vec32u altLow;

			vec32u Aabs = fsrcA[bx];
			vec32u Babs = fsrcB[bx];

			dst_high[bx] = 0;
			dst_low[bx] = 0;
			highA = Aabs >> vScalingFact;
			lowA = Aabs & lowerWordMask;

			highB = Babs >> vScalingFact;
			lowB = Babs & lowerWordMask;

			vif(highA > 0 || highB > 0){

				//vif(highA > 0) {
				term1 = lowA * highB;
				//} vendif

				//vif(highB > 0) {
				term2 = lowB * highA;
				//} vendif

				//vif(lowA > 0 && lowB > 0) {
				term3 = lowB * lowA;
				term3_high = term3 >> vScalingFact;
				term3_low = term3 & lowerWordMask;
				//} vendif

				// alternatively:
				vbool sum_hasCarry1, sum_hasCarry2;
				vbool sum_getCarry = 0, totalHasCarry;
				vec32u sum;

				sum = vaddx(term1, term2, sum_getCarry, sum_hasCarry1);
				sum = vaddx(sum, term3_high, sum_hasCarry1, sum_hasCarry2);
				altLow = ((sum & lowerWordMask) << vScalingFact) + term3_low;

				altHigh = (vec32s)vaddx(highA*highB, (sum >> vScalingFact), sum_hasCarry2, totalHasCarry);

	
			} velse{
				vec32u mulHigh = fsrcA[bx] * fsrcB[bx];
				altLow = mulHigh;// put into the lower 32bit-word the result of the multiplication
				altHigh = 0;
			} vendif

			dst_high[bx] = (vec32u)altHigh;
			dst_low[bx] = altLow;
		}

		dst_high += outStrideWidth;
		dst_low += outStrideWidth;
		fsrcA += inStrideWidth;
		fsrcB += inStrideWidth;

	}
}



void dot_mult_in32s_out64s_filter(vec32s* dst_high, vec32u* dst_low, vec32s* srcA, vec32s* srcB, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{

	vec32s* fsrcA = srcA;
	vec32s* fsrcB = srcB;

	vec32u lowerWordMask = 0x0000FFFF;
	vec32u vScalingFact = 16; // number of shifts to the right, i.e. scaling fact is 1/(2^16)


	// temporary variables needed inside the loop
	vec32u highA;
	vec32u highB;
	vec32u lowA;
	vec32u lowB;
	vec32u term1;
	vec32u term2;
	vec32u term3, term3_high, term3_low;
		

	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			vec32s altHigh;
			vec32u altLow;
	
			vec32u Aabs = (vec32u)(vabs(fsrcA[bx]));
			vec32u Babs = (vec32u)(vabs(fsrcB[bx]));

			vbool signHigh = hasSign(fsrcA[bx], fsrcB[bx]);
							
			dst_high[bx] = 0;
			dst_low[bx] = 0;
			highA = Aabs >> vScalingFact;
			lowA = Aabs & lowerWordMask;
			
			highB = Babs >> vScalingFact;
			lowB = Babs & lowerWordMask;

			vif(highA > 0 || highB > 0){

				//vif(highA > 0) {
					term1 = lowA * highB;
				//} vendif

				//vif(highB > 0) {
					term2 = lowB * highA;
				//} vendif

				//vif(lowA > 0 && lowB > 0) {
					term3 = lowB * lowA;
					term3_high = term3 >> vScalingFact;
					term3_low = term3 & lowerWordMask;
				//} vendif

				// alternatively:
				vbool sum_hasCarry1, sum_hasCarry2;
				vbool sum_getCarry = 0, totalHasCarry;
				vec32u sum;

				sum = vaddx(term1, term2, sum_getCarry, sum_hasCarry1);
				sum = vaddx(sum, term3_high, sum_hasCarry1, sum_hasCarry2);
				altLow  = ((sum & lowerWordMask) << vScalingFact) + term3_low;

				altHigh = (vec32s)vaddx(highA*highB, (sum >> vScalingFact), sum_hasCarry2, totalHasCarry);

			} velse{
				altLow = Aabs * Babs;
				altHigh = 0;
			} vendif

			vif(signHigh) {
				change64bitSign(altHigh, altLow);
			} vendif

			dst_high[bx] = altHigh;
			dst_low[bx] = altLow;
		}

		dst_high += outStrideWidth;
		dst_low += outStrideWidth;
		fsrcA += inStrideWidth;
		fsrcB += inStrideWidth;

	}
}




void dot_mult_in64s_out64s_filter(vec32s* dst_high, vec32u* dst_low, vec32s* srcA_high, vec32u* srcA_low, vec32s* srcB_high, vec32u* srcB_low, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{

	vec32s* fsrcA_high = srcA_high;
	vec32s* fsrcB_high = srcB_high;
	vec32u* fsrcA_low = srcA_low;
	vec32u* fsrcB_low = srcB_low;

	vec32u lowerWordMask = 0x0000FFFF;
	vec32u vScalingFact = 16; // number of shifts to the right, i.e. scaling fact is 1/(2^16)
	
	// temporary variables needed inside the loop
	vec32u highA;
	vec32u highB;
	vec32u lowA;
	vec32u lowB;
	vec32u term1;
	vec32u term2;
	vec32u term3, term3_high, term3_low;
	
	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			vec32s altHigh;
			vec32u altLow;

			vbool signHigh = hasSign(fsrcA_high[bx], fsrcB_high[bx]);

			vec32u Aabs_high, Aabs_low, Babs_high, Babs_low;
			Aabs_low = fsrcA_low[bx];
			Babs_low = fsrcB_low[bx];
			vif (fsrcA_high[bx] < 0){
				vec32s tmpHigh = fsrcA_high[bx];
				change64bitSign(tmpHigh, Aabs_low);
				Aabs_high = (vec32u)tmpHigh;
			} velse{
				Aabs_high = (vec32u) fsrcA_high[bx];
			} vendif

			vif (fsrcB_high[bx] < 0){
				vec32s tmpHigh = fsrcB_high[bx];
				change64bitSign(tmpHigh, Babs_low);
				Babs_high = (vec32u)tmpHigh;
			} velse{
				Babs_high = (vec32u)fsrcB_high[bx];
			} vendif

			dst_high[bx] = 0;
			dst_low[bx] = 0;
			highA = Aabs_low >> vScalingFact;
			lowA = Aabs_low & lowerWordMask;

			highB = Babs_low >> vScalingFact;
			lowB = Babs_low & lowerWordMask;

			// \warning overflow might arise here!!!
			altHigh = vec32s(Aabs_high*Babs_low + Babs_high*Aabs_low); //ignore any other carries and Aabs_high*Babs_high, since it would be out of 64bit bounds.

			vif(highA > 0 || highB > 0 ){

				//vif(highA > 0) {
				term1 = lowA * highB;
				//} vendif

				//vif(highB > 0) {
				term2 = lowB * highA;
				//} vendif

				//vif(lowA > 0 && lowB > 0) {
				term3 = lowB * lowA;
				term3_high = term3 >> vScalingFact;
				term3_low = term3 & lowerWordMask;
				//} vendif

				// alternatively:
				vbool sum_hasCarry1, sum_hasCarry2;
				vbool sum_getCarry = 0, totalHasCarry;
				vec32u sum;

				sum = vaddx(term1, term2, sum_getCarry, sum_hasCarry1);
				sum = vaddx(sum, term3_high, sum_hasCarry1, sum_hasCarry2);
				altLow = ((sum & lowerWordMask) << vScalingFact) + term3_low;

				altHigh += (vec32s)vaddx(highA*highB, (sum >> vScalingFact), sum_hasCarry2, totalHasCarry);
			} velse{
				altLow = fsrcA_low[bx] * fsrcB_low[bx];
			} vendif

			vif(signHigh) {
				change64bitSign(altHigh, altLow);
			} vendif

			dst_high[bx] = altHigh;
			dst_low[bx] = altLow;
		}

		dst_high += outStrideWidth;
		dst_low += outStrideWidth;
		fsrcA_high += inStrideWidth;
		fsrcB_high += inStrideWidth;

		fsrcA_low += inStrideWidth;
		fsrcB_low += inStrideWidth;

	}
}


void dot_mult_in64u_out64u_filter(vec32u* dst_high, vec32u* dst_low, vec32u* srcA_high, vec32u* srcA_low, vec32u* /*srcB_high*/, vec32u* srcB_low, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{

	vec32u* fsrcA_high = srcA_high;
	//vec32u* fsrcB_high = srcB_high;
	vec32u* fsrcA_low = srcA_low;
	vec32u* fsrcB_low = srcB_low;

	vec32u lowerWordMask = 0x0000FFFF;
	vec32u vScalingFact = 16; // number of shifts to the right, i.e. scaling fact is 1/(2^16)

	// temporary variables needed inside the loop
	vec32u highA;
	vec32u highB;
	vec32u lowA;
	vec32u lowB;
	vec32u term1;
	vec32u term2;
	vec32u term3, term3_high, term3_low;

	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			vec32u altHigh;
			vec32u altLow;

			vec32u Aabs_high, Aabs_low, Babs_high, Babs_low;
			Aabs_low = fsrcA_low[bx];
			Babs_low = fsrcB_low[bx];
			Aabs_high = (vec32u)fsrcA_high[bx];
			Babs_high = (vec32u)fsrcA_high[bx];

			dst_high[bx] = 0;
			dst_low[bx] = 0;
			highA = Aabs_low >> vScalingFact;
			lowA = Aabs_low & lowerWordMask;

			highB = Babs_low >> vScalingFact;
			lowB = Babs_low & lowerWordMask;

			altHigh = Aabs_high*Babs_low + Babs_high*Aabs_low; //ignore any other carries and Aabs_high*Babs_high, since it would be out of 64bit bounds.

			vif(highA > 0 || highB > 0){

				//vif(highA > 0) {
				term1 = lowA * highB;
				//} vendif

				//vif(highB > 0) {
				term2 = lowB * highA;
				//} vendif

				//vif(lowA > 0 && lowB > 0) {
				term3 = lowB * lowA;
				term3_high = term3 >> vScalingFact;
				term3_low = term3 & lowerWordMask;
				//} vendif

				// alternatively:
				vbool sum_hasCarry1, sum_hasCarry2;
				vbool sum_getCarry = 0, totalHasCarry;
				vec32u sum;

				sum = vaddx(term1, term2, sum_getCarry, sum_hasCarry1);
				sum = vaddx(sum, term3_high, sum_hasCarry1, sum_hasCarry2);
				altLow = ((sum & lowerWordMask) << vScalingFact) + term3_low;

				altHigh += (vec32s)vaddx(highA*highB, (sum >> vScalingFact), sum_hasCarry2, totalHasCarry);
			} velse{
				altLow = fsrcA_low[bx] * fsrcB_low[bx];
			} vendif

			dst_high[bx] = altHigh;
			dst_low[bx] = altLow;
		}

		dst_high += outStrideWidth;
		dst_low += outStrideWidth;
		fsrcA_high += inStrideWidth;
		//fsrcB_high += inStrideWidth;

		fsrcA_low += inStrideWidth;
		fsrcB_low += inStrideWidth;

	}
}






void dot_mult_scalar_in08u_out16s_filter(vec16s* dst, vec08u* srcA, int32s scalar, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec08u* fsrcA = srcA;
	vec16s* dst_high = dst;
	if (scalar != 1)
	{
		vec16s fsrcB = (int16s) scalar;
		for (int16s by = 0; by < bh; ++by)  // y data row
		{
			for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
			{
				dst_high[bx] = ((vec16s) fsrcA[bx]) * fsrcB;
			}
			dst_high += outStrideWidth;
			fsrcA += inStrideWidth;

		}
	}
	else
	{
		for (int16s by = 0; by < bh; ++by)  // y data row
		{
			for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
			{
				dst_high[bx] = (vec16s) fsrcA[bx];
			}
			dst_high += outStrideWidth;
			fsrcA += inStrideWidth;
		}
	}
}

void dot_mult_scalar_in32s_out32s_filter(vec32s* dst, vec32s* srcA, int32s scalar, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32s* fsrcA = srcA;
	vec32s* dst_high = dst;
	if (scalar != 1) 
	{
		vec32s fsrcB = scalar;
		for (int16s by = 0; by < bh; ++by)  // y data row
		{
			for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
			{
				dst_high[bx] = fsrcA[bx] * fsrcB;
			}
			dst_high += outStrideWidth;
			fsrcA += inStrideWidth;

		}
	} else 
	{
		for (int16s by = 0; by < bh; ++by)  // y data row
		{
			for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
			{
				dst_high[bx] = fsrcA[bx];
			}
			dst_high += outStrideWidth;
			fsrcA += inStrideWidth;
		}
	}
}




void lsh_in32u_out32u_filter(vec32u* upShifted, vec32u* src, vec08u* leftShiftFact, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32u* crtRow = src;
	vec32u* crtupShifted = upShifted;
	for (int16s y = 0; y < bh; ++y)
	{
		for (int16s x = 0; x < bw; x++) // start accumulation only at xOffs
		{
			crtupShifted[x] = crtRow[x] << (vec32u)(*leftShiftFact);
		}
		crtRow += inStrideWidth; //number of pixels on a row, including padding

		crtupShifted += outStrideWidth;
	}
}



void lsh_in16u_out16s_filter(vec16s* upShifted, vec16u* src, vec16s* leftShiftFact, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec16u* crtRow = src;
	vec16s* crtupShifted = upShifted;
	for (int16s y = 0; y < bh; ++y)
	{
		for (int16s x = 0; x < bw; x++) // start accumulation only at xOffs
		{
			crtupShifted[x] = vec16s(crtRow[x]) << *leftShiftFact; 
		}
		crtRow += inStrideWidth; //number of pixels on a row, including padding

		crtupShifted += outStrideWidth;
	}
}

void lsh_in32s_out32s_filter(vec32s* upShifted, vec32s* src, vec08u* leftShiftFact, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32s* crtRow = src;
	vec32s* crtupShifted = upShifted;
	vec32s lSF = (vec32s)*leftShiftFact; // one single factor value for the whole chunk

	for (int16s y = 0; y < bh; ++y)
	{
		for (int16s x = 0; x < bw; x++) // start accumulation only at xOffs
		{
			crtupShifted[x] = crtRow[x] << lSF;
		}
		crtRow += inStrideWidth; //number of pixels on a row, including padding

		crtupShifted += outStrideWidth;
	}
}


void lsh_in32s_out64s_filter(vec32s* upShifted_high, vec32u* upShifted_low, vec32s* src, vec08u* leftShiftFact
	, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth){

	vec32s* crtRow = src;
	vec32u* crtupShifted_low = upShifted_low;
	vec32s* crtupShifted_high = upShifted_high;
	vec32u lSF = (vec32u)*leftShiftFact; // one single factor value for the whole chunk
	vec32s lSFs = vec32s(lSF);

	const vec32s highShift = vec32s(32) - lSFs;

	vec32u vZeros = 0;
	vec32s diffShift = lSFs - vec32s(32);

	for (int16s y = 0; y < bh; ++y)
	{
		for (int16s x = 0; x < bw; x++) // start accumulation only at xOffs
		{
			vif(*leftShiftFact < 32) {
				crtupShifted_low[x] = vec32u(crtRow[x]) << lSF;
				crtupShifted_high[x] = (crtRow[x] >> highShift);
			} velse{
				crtupShifted_low[x] = vZeros;
				crtupShifted_high[x] = vec32s(crtupShifted_low[x]) << diffShift;
			} vendif

		}
		crtRow += inStrideWidth; //number of pixels on a row, including padding

		crtupShifted_low += outStrideWidth;
		crtupShifted_high += outStrideWidth;
	}
}

void lsh_in32u_out64u_filter(vec32u* upShifted_high, vec32u* upShifted_low, vec32u* src, vec08u *leftShiftFact
	, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32u* crtRow = src;
	vec32u* crtupShifted_low = upShifted_low;
	vec32u* crtupShifted_high = upShifted_high;
	vec32u lSF = (vec32u)*leftShiftFact; // one single factor value for the whole chunk

	vec32u vZeros = 0;
	vec32u diffShift = vec32u(*leftShiftFact) - vec32u(32);

	const vec32u highShift = vec32u(32) - lSF;

	for (int16s y = 0; y < bh; ++y)
	{
		for (int16s x = 0; x < bw; x++) // start accumulation only at xOffs
		{
			vif(*leftShiftFact < 32) {
				crtupShifted_low[x] = crtRow[x] << lSF;
				crtupShifted_high[x] = (crtRow[x] >> highShift);
			} velse{
				crtupShifted_low[x] = vZeros;
				crtupShifted_high[x] = crtupShifted_low[x] << diffShift;
			} vendif
		}
		crtRow += inStrideWidth; //number of pixels on a row, including padding

		crtupShifted_low += outStrideWidth;
		crtupShifted_high += outStrideWidth;
	}
}


void lsh_in32s_Q3_28_out64s_filter(vec32s* upShifted_high, vec32s* upShifted_low, vec32s* src, vec08u* leftShiftFact
	, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth){

	vec32s* crtRow = src;
	vec32s* crtupShifted_low = upShifted_low;
	vec32s* crtupShifted_high = upShifted_high;

	vec32s vZeros = 0;
	vec32s intShiftFact = vec32s(*leftShiftFact);
	vec32s diffShift = intShiftFact - vec32s(32);
	const vec32s highShift = vec32s(32) - (intShiftFact+ 3 + 1);
	const vec32s q3_28mask = vec32s(0x0FFFFFFF);
	const vec32s negFrac = 0xF0000000;
	for (int16s y = 0; y < bh; ++y)
	{
		for (int16s x = 0; x < bw; x++) // start accumulation only at xOffs
		{
			vif(*leftShiftFact < 32) {
				//vec32u absRow = vec32u(vabs(crtRow[x]));
				crtupShifted_high[x] = crtRow[x] >> highShift;
				crtupShifted_low[x] = (crtRow[x] << intShiftFact) & q3_28mask;
				vif(crtRow[x] < 0) {
					crtupShifted_low[x] += negFrac;
				} vendif
			} velse{
				crtupShifted_low[x] = vZeros;
				crtupShifted_high[x] = crtRow[x] << diffShift;
			}vendif
		}
		crtRow += inStrideWidth; //number of pixels on a row, including padding

		crtupShifted_low += outStrideWidth;
		crtupShifted_high += outStrideWidth;
	}

}
void rsh_in32u_out32u_filter(vec32u* downShift, vec32u* src, vec08u* rightShiftFact, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32u* crtRow = src;
	vec32u* crtDownShift = downShift;
	vec32u rSF = (vec32u)* rightShiftFact; // one single factor value for the whole chunk

	for (int16s y = 0; y < bh; ++y)
	{
		for (int16s x = 0; x < bw; x++) // start accumulation only at xOffs
		{
			crtDownShift[x] = crtRow[x] >> rSF;
		}
		crtRow += inStrideWidth; //number of pixels on a row, including padding

		crtDownShift += outStrideWidth;
	}
}

void rsh_in32s_out32s_filter(vec32s* downShift, vec32s* src, vec08u* rightShiftFact, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32s* crtRow = src;
	vec32s* crtDownShift = downShift;
	vec32s rSF = (vec32s)* rightShiftFact; // one single factor value for the whole chunk

	for (int16s y = 0; y < bh; ++y)
	{
		for (int16s x = 0; x < bw; x++) // start accumulation only at xOffs
		{
			crtDownShift[x] = crtRow[x] >> rSF;
		}
		crtRow += inStrideWidth; //number of pixels on a row, including padding
		
		crtDownShift += outStrideWidth;
	}
}


void rsh_in64s_out64s_filter(vec32s* dst_high, vec32u* dst_low, vec32s* in_high, vec32u* in_low, vec08u *rightShiftFact, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32s quotRShft = vec32s(*rightShiftFact); // there is only one right shift fact for a whole chunk
	vec32u quotRShftu = vec32u(*rightShiftFact);
	vec32s shiftNR = vec32s(32) - quotRShft;
	vec32u maskRShft = vec32u(0x7FFFFFFF) >> (quotRShftu-1);

	vec32s vZeros = 0;
	vec32s diffShift = quotRShft - vec32s(32);

	vif(*rightShiftFact != 0) {
		for (int16s by = 0; by < bh; ++by)  // y data row
		{
			for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
			{
				vif(quotRShftu < 32) {
					dst_low[bx] = ((in_low[bx] >> quotRShftu) & maskRShft) + vec32u(in_high[bx] << shiftNR); //mask the right shifted low word, since the last bit is not a sign bit and has not to be propagated by the shift
					dst_high[bx] = in_high[bx] >> quotRShft;
				} velse{
					dst_high[bx] = vZeros;
					dst_low[bx] = vec32u(in_high[bx] >> diffShift);
				} vendif
			}
			dst_high += outStrideWidth;
			dst_low += outStrideWidth;
			in_high += inStrideWidth;
			in_low += inStrideWidth;
		}
	} vendif

}


void rsh_in64u_out64u_filter(vec32u* dst_high, vec32u* dst_low, vec32u* in_high, vec32u* in_low, vec08u* rightShiftFact, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vif(*rightShiftFact != 0) {
		vec32u quotRShftu = vec32u(*rightShiftFact);
		vec32u shiftNR = vec32u(32) - quotRShftu;
		vec32u maskRShft = vec32u(0x7FFFFFFF) >> (quotRShftu - 1);


		vec32u vZeros = 0;
		vec32u diffShift = quotRShftu - vec32u(32);


		for (int16s by = 0; by < bh; ++by)  // y data row
		{
			for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
			{
				vif(quotRShftu < 32) {
					dst_low[bx] = ((in_low[bx] >> quotRShftu) & maskRShft) + (in_high[bx] << shiftNR); //mask the right shifted low word, since the last bit is not a sign bit and has not to be propagated by the shift
					dst_high[bx] = (in_high[bx] >> quotRShftu) & maskRShft;
				} velse {
					dst_high[bx] = vZeros;
					dst_low[bx] = (in_high[bx] >> diffShift);
				} vendif
			}

			dst_high += outStrideWidth;
			dst_low += outStrideWidth;
			in_high += inStrideWidth;
			in_low += inStrideWidth;
		}
	} vendif

}


void rsh_in64s_out32s_filter(vec32s* dst, vec32s* in_high, vec32u* in_low, vec08u *rightShiftFact, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32s quotRShft = vec32s(*rightShiftFact); // there is only one right shift fact for a whole chunk
	vec32u quotRShftu = vec32u(*rightShiftFact);
	vec32s shiftNR = vec32s(32) - quotRShft;
	vec32u maskRShft = vec32u(0x7FFFFFFF) >> (quotRShftu - 1);

	//vec32s vZeros = 0;
	vec32s diffShift = quotRShft - vec32s(32);

	vif(*rightShiftFact != 0) {
		for (int16s by = 0; by < bh; ++by)  // y data row
		{
			for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
			{
				//vec32s dst_high = 0;

				vif(quotRShftu < 32) {
					dst[bx] = vec32s((in_low[bx] >> quotRShftu) & maskRShft) + (in_high[bx] << shiftNR); //mask the right shifted low word, since the last bit is not a sign bit and has not to be propagated by the shift
					//dst_high = in_high[bx] >> quotRShft;
				} velse{
					//dst_high = vZeros;
					dst[bx] = in_high[bx] >> diffShift;
				} vendif

				//if (vany(dst_high > 0) || vany(dst_high < -1)){
					//printf("64bit RIGHT SHIFT results is higher than 2^32");
				//}
			}
			
			dst += outStrideWidth;
			in_high += inStrideWidth;
			in_low += inStrideWidth;
		}
	} vendif

}


#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
