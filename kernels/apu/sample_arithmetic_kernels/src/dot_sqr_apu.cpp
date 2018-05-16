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
* OF MERCHANTABILITY AND FITNESS FOR a_high PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY out_high OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/
/*!
* \file apu_dot_sqr_apu.cpp
* \brief Element-wise square.
* \author Anca Dima
* \version
* \date
****************************************************************************/

// In order to compile it as CPP do following: 
// In Visual Studio->Solution Explorer->Project "convolution_Kernel", right click -> Properties -> C/C++ ->Advanced->CompileAs->Compile As C++
#ifdef APEX2_EMULATE
#include "apu_cond.hpp"
#endif

#include "dot_sqr_apu.h"
#include "dot_multiplic_apu.h"

/*Square root of a*/
vec32u vsqrt_32(vec32u a)
{
	vec16u ah = (vec16u)(a >> 16);
	vec16u u = 0x4000, r = u + u;
	vec16u tmp;
	vec32u prod;
	vbool  c;
	vec32u res;
	vif(a > 1) {
		for (int i = 6; i >= 0; i--, u >>= 1){
			r = vasb(r, u, vmul_uhuh(r, r) <= ah);
		}

		for (int i = 7; i >= 0; i--, u >>= 1){
			prod = vec32u(vmul_uhuh(r, r), vmul_ulul(r, r));
			tmp = vmul_uhul(r, r);
			vacm(prod, tmp);
			vacm(prod, tmp);
			vsub(a, prod, c);
			r = vasb(r, u, ~c);/* prod <= a    ==     a >= prod, ergo a-prod does not carry. */
			/* r = vasb(r, u, prod <= a); */
		}

		prod = vec32u(vmul_uhuh(r, r), vmul_ulul(r, r));
		tmp = vmul_uhul(r, r);
		vacm(prod, tmp);
		vacm(prod, tmp);

		vsub(a, prod, c);
		res = (vec32u)vsubx(r, (vec16u)0, c);
	} velse{
		res = a;
	} vendif

		return res;
}


void dot_sqr_in16s_out32u_filter(vec32u* dst, vec16s* srcA
			, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec16s* fsrcA = srcA;
	vec32u* out_high = dst;
	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			vec32u tmp = (vec32u)vabs(fsrcA[bx]);
			out_high[bx] = tmp*tmp;
		}
		out_high += outStrideWidth;
		fsrcA += inStrideWidth;
	}
}



void dot_sqr_in32s_out32u_filter(vec32u* dst, vec32s* srcA
			, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32s* fsrcA = srcA;
	vec32u* out_high = dst;
	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			vec32u tmp = (vec32u)vabs(fsrcA[bx]);
			out_high[bx] = tmp*tmp;
		}
		out_high += outStrideWidth;
		fsrcA += inStrideWidth;
	}
}

void dot_sqr_in32s_out64u_filter(vec32u* out_high, vec32u* out_low, vec32s* srcA
							, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
   vec32s* fsrcA = srcA;   
   int32u scalingFact = 16;
   vec32u vScalingFact = scalingFact; // number of shifts to the right, i.e. scaling fact is 1/(2^8)
   vec32u thresh = (1 << scalingFact);

   vec32u lowerWordMask = 0x0000FFFF;

   for (int16s by = 0; by < bh; ++by)  // y data row
   {
	   for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
	   {		   
		   vec32u Aabs = (vec32u)( vabs(fsrcA[bx]) );
		   out_high[bx] = 0;
		   out_low[bx] = 0;
		   vec32u a_high;
		   vec32u a_low;
		   
		   vec32u tmpOutHigh, tmpOutLow;

		   a_low = Aabs & lowerWordMask;
		   a_high = (vec32u) (Aabs >> vScalingFact);

		   vbool getCarry = 0, hasCarry1, hasCarry2;

		   vif(Aabs > thresh) {		   
			   vec32u term3, term3_high, term3_low;
			   vec32u term1_2;

			   term1_2 = (a_high * a_low);

 			   term3 = a_low * a_low;
			   term3_high = term3 >> vScalingFact;
			   term3_low = term3 & lowerWordMask;

			   vec32u sum;
			   sum = vaddx(term1_2, term1_2, getCarry, hasCarry1);
			   sum = vaddx(sum, term3_high, hasCarry1, hasCarry2);

			   tmpOutLow = ((sum & lowerWordMask) << vScalingFact) + term3_low;
	
			   vbool totalHasCarry;
			   tmpOutHigh = (vec32u)vaddx(a_high*a_high, (sum >> vScalingFact), hasCarry2, totalHasCarry);

		   } velse{	
			   tmpOutLow = Aabs * Aabs;
			   tmpOutHigh = 0;
		   } vendif
		
		   out_high[bx] = tmpOutHigh;
		   out_low[bx] = tmpOutLow;			   		   
			   		 
	   }

	   out_high += outStrideWidth;
	   out_low += outStrideWidth;
	   fsrcA += inStrideWidth;
	   
   }	
} 




void dot_sqr_in32u_out64u_filter(vec32u* out_high, vec32u* out_low, vec32u* srcA
								, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{
	vec32u* fsrcA = srcA;
	int32u scalingFact = 16;
	vec32u vScalingFact = scalingFact; // number of shifts to the right, i.e. scaling fact is 1/(2^8)
	vec32u thresh = (1 << scalingFact);

	vec32u lowerWordMask = 0x0000FFFF;

	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			vec32u *Aabs = &(fsrcA[bx]);
			out_high[bx] = 0;
			out_low[bx] = 0;
			vec32u a_high;
			vec32u a_low;

			vec32u tmpOutHigh, tmpOutLow;

			a_low = *Aabs & lowerWordMask;
			a_high = (vec32u)(*Aabs >> vScalingFact);

			vbool getCarry = 0, hasCarry1, hasCarry2;

			vif(*Aabs > thresh) {
				vec32u term3, term3_high, term3_low;
				vec32u term1_2;

				term1_2 = (a_high * a_low);

				term3 = a_low * a_low;
				term3_high = term3 >> vScalingFact;
				term3_low = term3 & lowerWordMask;

				vec32u sum;
				sum = vaddx(term1_2, term1_2, getCarry, hasCarry1);
				sum = vaddx(sum, term3_high, hasCarry1, hasCarry2);

				tmpOutLow = ((sum & lowerWordMask) << vScalingFact) + term3_low;

				vbool totalHasCarry;
				tmpOutHigh = (vec32u)vaddx(a_high*a_high, (sum >> vScalingFact), hasCarry2, totalHasCarry);

			} velse{
				tmpOutLow = (*Aabs) * (*Aabs);
				tmpOutHigh = 0;
			} vendif

			out_high[bx] = tmpOutHigh;
			out_low[bx] = tmpOutLow;

		}

		out_high += outStrideWidth;
		out_low += outStrideWidth;
		fsrcA += inStrideWidth;

	}
}



void dot_sqr_in64s_out64u_filter(vec32u* out_high, vec32u* out_low, vec32s* srcA_high, vec32u* srcA_low
							, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{

	vec32s* fsrcA_high = srcA_high;
	vec32u* fsrcA_low = srcA_low;

	vec32u lowerWordMask = 0x0000FFFF;
	vec32u vScalingFact = 16; // number of shifts to the right, i.e. scaling fact is 1/(2^16)

	// temporary variables needed inside the loop
	vec32u highA;	
	vec32u lowA;	
	vec32u term1;
	vec32u term3, term3_high, term3_low;

	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			vec32u altHigh;
			vec32u altLow;
			
			vec32u Aabs_high, Aabs_low;
			Aabs_low = fsrcA_low[bx];
			
			vif(fsrcA_high[bx] < 0){
				vec32s tmpHigh = fsrcA_high[bx];
				change64bitSign(tmpHigh, Aabs_low);
				Aabs_high = (vec32u)tmpHigh;
			} velse {
				Aabs_high = (vec32u)fsrcA_high[bx];
			} vendif
			
			out_high[bx] = 0;
			out_low[bx] = 0;
			highA = Aabs_low >> vScalingFact;
			lowA = Aabs_low & lowerWordMask;


			// \warning overflow might arise here!!!
			altHigh = (Aabs_high*Aabs_low)<<1; //ignore any other carries and Aabs_high*Babs_high, since it would be out of 64bit bounds.

			vif(highA > 0){

				term1 = lowA * highA;

				term3 = lowA * lowA;
				term3_high = term3 >> vScalingFact;
				term3_low = term3 & lowerWordMask;

				// alternatively:
				vbool sum_hasCarry1, sum_hasCarry2;
				vbool sum_getCarry = 0, totalHasCarry;
				vec32u sum;

				sum = vaddx(term1, term1, sum_getCarry, sum_hasCarry1);
				sum = vaddx(sum, term3_high, sum_hasCarry1, sum_hasCarry2);
				altLow = ((sum & lowerWordMask) << vScalingFact) + term3_low;

				altHigh += (vec32s)vaddx(highA*highA, (sum >> vScalingFact), sum_hasCarry2, totalHasCarry);
			} velse{
				altLow = Aabs_low * Aabs_low;
			} vendif

				out_high[bx] = altHigh;
				out_low[bx] = altLow;
		}

		out_high += outStrideWidth;
		out_low += outStrideWidth;
		fsrcA_high += inStrideWidth;
		fsrcA_low += inStrideWidth;
	}
}


void dot_sqr_in64u_out64u_filter(vec32u* out_high, vec32u* out_low, vec32u* srcA_high, vec32u* srcA_low
						, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth)
{

	vec32u* fsrcA_high = srcA_high;
	vec32u* fsrcA_low = srcA_low;

	vec32u lowerWordMask = 0x0000FFFF;
	vec32u vScalingFact = 16; // number of shifts to the right, i.e. scaling fact is 1/(2^16)

	// temporary variables needed inside the loop
	vec32u highA;
	vec32u lowA;
	vec32u term1;
	vec32u term3, term3_high, term3_low;

	for (int16s by = 0; by < bh; ++by)  // y data row
	{
		for (int16s bx = 0; bx < bw; ++bx) // x in the blck_tile
		{
			vec32u altHigh;
			vec32u altLow;

			vec32u Aabs_high, Aabs_low;
			Aabs_low = fsrcA_low[bx];
			Aabs_high = (vec32u)fsrcA_high[bx];

			out_high[bx] = 0;
			out_low[bx] = 0;
			highA = Aabs_low >> vScalingFact;
			lowA = Aabs_low & lowerWordMask;


			// \warning overflow might arise here!!!
			altHigh = (Aabs_high*Aabs_low) << 1; //ignore any other carries and Aabs_high*Babs_high, since it would be out of 64bit bounds.

			vif(highA > 0){

				term1 = lowA * highA;

				term3 = lowA * lowA;
				term3_high = term3 >> vScalingFact;
				term3_low = term3 & lowerWordMask;

				// alternatively:
				vbool sum_hasCarry1, sum_hasCarry2;
				vbool sum_getCarry = 0, totalHasCarry;
				vec32u sum;

				sum = vaddx(term1, term1, sum_getCarry, sum_hasCarry1);
				sum = vaddx(sum, term3_high, sum_hasCarry1, sum_hasCarry2);
				altLow = ((sum & lowerWordMask) << vScalingFact) + term3_low;

				altHigh += (vec32s)vaddx(highA*highA, (sum >> vScalingFact), sum_hasCarry2, totalHasCarry);
			} velse {
				altLow = Aabs_low * Aabs_low;
			} vendif

			out_high[bx] = altHigh;
			out_low[bx] = altLow;
		}

		out_high += outStrideWidth;
		out_low += outStrideWidth;
		fsrcA_high += inStrideWidth;
		fsrcA_low += inStrideWidth;
	}
}


#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

