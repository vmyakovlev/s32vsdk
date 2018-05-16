/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
* All Rights Reserved
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
* \file filter.cpp
* \brief Implementation of different filters, by specifying filter coefficients
* \author Anca Dima
* \version
* \date 11-February-2014
****************************************************************************/

#include "utils_filter.hpp"


namespace Convolution {

	const short gaussFilter3x3Data[3][3] =
	{
		{ 1, 2, 1 },
		{ 2, 4, 2 },
		{ 1, 2, 1 }
	};

	const short gaussFilter5x5Data[5][5] =
	{
		{ 1, 1, 1, 1, 1 },
		{ 1, 5, 5, 5, 1 },
		{ 1, 5, 8, 5, 1 },
		{ 1, 5, 5, 5, 1 },
		{ 1, 1, 1, 1, 1 }
	};

	const short gradientX[1][3] =
	{
		{ -1, 0, 1 }
	};

	const short gradientY[3][1] =
	{
		{ -1 },
		{ 0 },
		{ 1 }
	};

	const short gradientXY[3][3] =
	{
		{ -1, 0, 1 },
		{ 0, 0, 0 },
		{ 1, 0, -1 }
	};

	const short scharrX[3][3] =
	{
		{ 3, 0,  -3},
		{10, 0, -10},
		{ 3, 0,  -3}
	};

	const short scharrY[3][3] =
	{
		{  3,  10,  3 },
		{  0,   0,  0 },
		{ -3, -10, -3 }
	};


	const CFilter<3, 3, short> gaussFilter3x3(16, gaussFilter3x3Data);
	const CFilter<5, 5, short> gaussFilter5x5(64, gaussFilter5x5Data);
	const CFilter<3, 1, short> gradientXFilter(1, gradientX);
	const CFilter<1, 3, short> gradientYFilter(1, gradientY);
	const CFilter<3, 3, short> gradientXYFilter(1, gradientXY);
	const CFilter<3, 3, short> scharrXFilter(scharrX);
	const CFilter<3, 3, short> scharrYFilter(scharrY);
} //namespace convolution