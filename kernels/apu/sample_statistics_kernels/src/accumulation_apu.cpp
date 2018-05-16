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
* \file accumulation_apu.cpp
* \brief Builds the sum of all elements of a chunk and writes out a vector of sum values.
* \author Anca Dima
* \version
* \date
****************************************************************************/

// In order to compile it as CPP do following: 
// In Visual Studio->Solution Explorer->Project "convolution_Kernel", right click -> Properties -> C/C++ ->Advanced->CompileAs->Compile As C++
#ifdef ACF_KERNEL_IMPLEMENTATION

#include "accumulation_apu.h"

#ifdef APEX2_EMULATE
#include <stdio.h>
#endif

void accumulation_in32s_filter(vec32s* lpvOut, vec32s* lpvIn, int16s strideWidth, int16s xOffs, int16s yOffs, int16s xAccWidth, int16s yAccHeight)
{
   vec32s* crtRow = lpvIn;
   vec32s* accumRow = lpvOut;

   int16u xEnd = xOffs + xAccWidth;

    (*accumRow) = 0;
   
	int16u y = 0;
	crtRow += yOffs*strideWidth;

#ifdef APEX2_EMULATE
	vbool getCarry = false, hasCarry = false;
#endif

	for (; y < yAccHeight; ++y)
	{
		for (int16u x = xOffs; x < xEnd; x++) // start accumulation only at xOffs
		{
			(*accumRow) += crtRow[x];

#ifdef APEX2_EMULATE
			{
				vbool tHasCarry = false;
				vec32s tmpAcc = vabs(*accumRow);
				tmpAcc = vaddx(tmpAcc, vabs(crtRow[x]), getCarry, tHasCarry);
				hasCarry = hasCarry || tHasCarry;

				if (vany(tHasCarry)) {
					fprintf(stderr, "\n\n-------------------- ACCUM ERROR:--------\n ACCUMULATION OVERFLOW\n-------------------- ACCUM ERROR:--------\n\n");
				}
			}
#endif

		}
		crtRow += strideWidth; //number of pixels on a row, including padding
	}

#ifdef APEX2_EMULATE
	if(vany(hasCarry)) {
		fprintf(stderr, "\n\n-------------------- ACCUM ERROR:--------\n ACCUMULATION OVERFLOW\n-------------------- ACCUM ERROR:--------\n\n");
	}
#endif

 } 

void accumulation_in32u_filter(vec32u* lpvOut, vec32u* lpvIn, int16s strideWidth, int16s xOffs, int16s yOffs, int16s xAccWidth, int16s yAccHeight)
{
	vec32u* crtRow = lpvIn;
	vec32u* accumRow = lpvOut;
	vbool getCarry = false, hasCarry;
	(*accumRow) = 0;

	int16u y = 0;
	int16u xEnd = xOffs + xAccWidth;

	crtRow += yOffs*strideWidth;
	for (; y < yAccHeight; ++y)
	{
		for (int16u x = xOffs; x < xEnd; x++) chess_prepare_for_pipelining// start accumulation only at xOffs
		{
			vbool tHasCarry;
			(*accumRow) = vaddx((*accumRow), crtRow[x], getCarry, tHasCarry);
			//hasCarry = hasCarry|| tHasCarry;
		}

		crtRow += strideWidth; //number of pixels on a row, including padding
	}

#ifdef APEX2_EMULATE
	if(vany(hasCarry)) {
		fprintf(stderr, "\n\n--------------------UNSIGNED ACCUM ERROR:--------\n ACCUMULATION OVERFLOW\n-------------------- ACCUM ERROR:--------\n\n");
	}
#endif
}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
