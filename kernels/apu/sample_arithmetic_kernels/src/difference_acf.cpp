/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
* Copyright (c) 2017 NXP
* 
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

/*!
* \file difference_acf.cpp
* \addtogroup arithmetic
* \addtogroup difference
* \ingroup arithmetic
* @{
* \brief Element-wise difference
*/
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA

#include "difference_acf.h"

/****************************************************************************/
/*!
Difference kernel metadata. Computes pixelwise the difference between two unsigned 8bit images.
Outputs 16bit signed difference result.

\param  DIFF_In08u_Out16s_KN	Macro Definition for Kernel name
\param 3						Number of ports
\param "Port DIFF_KN_INA"		Define for name of first input image (unsigned 8bit)
\param "Port DIFF_KN_INB"		Define for name of second input image (unsigned 8bit)
\param "Port DIFF_KN_OUT"		Define for name of difference result of the two images (signed 16bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(DIFF_In08u_Out16s_K) //_kernel_info_apu_diff_in08u_out16s 
(
   DIFF_In08u_Out16s_KN,
   3,
   __port(__index(0),
   __identifier(DIFF_KN_INA),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0), 
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),

  __port(__index(1),
		  __identifier(DIFF_KN_INB),
		  __attributes(ACF_ATTR_VEC_IN),
		  __spatial_dep(0,0,0,0), 
		  __e0_data_type(d08u),
		  __e0_size(1, 1),
		  __ek_size(1, 1)),

   __port(__index(2),
          __identifier(DIFF_KN_OUT),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16s),
          __e0_size(1, 1),
          __ek_size(1, 1))
);


/****************************************************************************/
/*!
Difference kernel metadata. Computes pixelwise the difference between two signed 16bit images.
Outputs 16bit signed difference result.
\warning No sanity checks for out of bounds values above/below +/-2^(15-1) are performed.
\param  DIFF_In16s_Out16s_KN	Macro Definition for Kernel name
\param 3						Number of ports
\param "Port DIFF_KN_INA"		Define for name of first input image (signed 16bit)
\param "Port DIFF_KN_INB"		Define for name of second input image (signed 16bit)
\param "Port DIFF_KN_OUT"		Define for name of result of the two images (signed 16bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(DIFF_In16s_Out16s_K) //_kernel_info_apu_diff_in16s_out16s
(
DIFF_In16s_Out16s_KN,
3,
__port(__index(0),
__identifier(DIFF_KN_INA),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d16s),
__e0_size(1, 1),
__ek_size(1, 1)),

__port(__index(1),
__identifier(DIFF_KN_INB),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d16s),
__e0_size(1, 1),
__ek_size(1, 1)),

__port(__index(2),
__identifier(DIFF_KN_OUT),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d16s),
__e0_size(1, 1),
__ek_size(1, 1))
);


/****************************************************************************/
/*!
Difference kernel metadata. Computes pixelwise the difference between two signed 16bit images.
Outputs 32bit signed difference result.

\param  DIFF_In16s_Out32s_KN	Macro Definition for Kernel name
\param 3						Number of ports
\param "Port DIFF_KN_INA"		Define for name of first input image (signed 16bit)
\param "Port DIFF_KN_INB"		Define for name of second input image (signed 16bit)
\param "Port DIFF_KN_OUT"		Define for name of result of the two images (signed 32bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(DIFF_In16s_Out32s_K) //_kernel_info_apu_diff_in16s_out32s
(
DIFF_In16s_Out32s_KN,
3,
	__port(__index(0),
		__identifier(DIFF_KN_INA),
		__attributes(ACF_ATTR_VEC_IN),
		__spatial_dep(0, 0, 0, 0), 
		__e0_data_type(d16s),
		__e0_size(1, 1),
		__ek_size(1, 1)),

	__port(__index(1),
		__identifier(DIFF_KN_INB),
		__attributes(ACF_ATTR_VEC_IN),
		__spatial_dep(0, 0, 0, 0), 
		__e0_data_type(d16s),
		__e0_size(1, 1),
		__ek_size(1, 1)),

	__port(__index(2),
		__identifier(DIFF_KN_OUT),
		__attributes(ACF_ATTR_VEC_OUT),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d32s),
		__e0_size(1, 1),
		__ek_size(1, 1))
);

/****************************************************************************/
/*!
Difference kernel metadata. Computes pixelwise the difference between two signed 32bit images.
Outputs 32bit signed difference result.
\warning No sanity checks for out of bounds values above/below +/-2^(31-1) are performed.
\param  DIFF_In32s_Out32s_KN	Macro Definition for Kernel name
\param 3						Number of ports
\param "Port DIFF_KN_INA"		Define for name of first input image (signed 32bit)
\param "Port DIFF_KN_INB"		Define for name of second input image (signed 32bit)
\param "Port DIFF_KN_OUT"		Define for name of result of the two images (signed 32bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(DIFF_In32s_Out32s_K) //_kernel_info_apu_diff_in32s_out32s
(
	DIFF_In32s_Out32s_KN,
	3,
	__port(__index(0),
		__identifier(DIFF_KN_INA),
		__attributes(ACF_ATTR_VEC_IN),
		__spatial_dep(0, 0, 0, 0), 
		__e0_data_type(d32s),
		__e0_size(1, 1),
		__ek_size(1, 1)),

	__port(__index(1),
		__identifier(DIFF_KN_INB),
		__attributes(ACF_ATTR_VEC_IN),
		__spatial_dep(0, 0, 0, 0), 
		__e0_data_type(d32s),
		__e0_size(1, 1),
		__ek_size(1, 1)),

	__port(__index(2),
		__identifier(DIFF_KN_OUT),
		__attributes(ACF_ATTR_VEC_OUT),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d32s),
		__e0_size(1, 1),
		__ek_size(1, 1))
);

/****************************************************************************/
/*!
Difference kernel metadata. Computes pixelwise the difference between two signed 64bit images.
Outputs 64bit signed difference result.
\warning No sanity checks for out of bounds values above/below +/- 2^(63-1) are performed.
\param  DIFF_In64s_Out64s_KN	Macro Definition for Kernel name
\param 6						Number of ports
\param "Port DIFF_KN_INA_HIGH"	Define for name of first signed 32bit high word of input image 
\param "Port DIFF_KN_INA_LOW"	Define for name of first unsigned 32bit low word of input image
\param "Port DIFF_KN_INB_HIGH"	Define for name of first signed 32bit high word of input image
\param "Port DIFF_KN_INB_LOW"	Define for name of first unsigned 32bit low word of input image
\param "Port DIFF_KN_OUT_HIGH"	Define for name of signed 32bit high word of difference result
\param "Port DIFF_KN_OUT_LOW"	Define for name of unsigned 32bit low word of difference result
*****************************************************************************/

KERNEL_INFO kernelInfoConcat(DIFF_In64s_Out64s_K)
(
DIFF_In64s_Out64s_KN,
6,
__port(__index(0),
__identifier(DIFF_KN_INA_HIGH),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0), 
__e0_data_type(d32s),
__e0_size(1, 1),
__ek_size(1, 1)),

__port(__index(1),
__identifier(DIFF_KN_INA_LOW),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0), 
__e0_data_type(d32u),
__e0_size(1, 1),
__ek_size(1, 1)),

__port(__index(2),
__identifier(DIFF_KN_INB_HIGH),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0), 
__e0_data_type(d32s),
__e0_size(1, 1),
__ek_size(1, 1)),

__port(__index(3),
__identifier(DIFF_KN_INB_LOW),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0), 
__e0_data_type(d32u),
__e0_size(1, 1),
__ek_size(1, 1)),

__port(__index(4),
__identifier(DIFF_KN_OUT_HIGH),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d32s),
__e0_size(1, 1),
__ek_size(1, 1)),

__port(__index(5),
__identifier(DIFF_KN_OUT_LOW),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d32u),
__e0_size(1, 1),
__ek_size(1, 1))

);


#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "difference_apu.h"
#include <stdio.h>

void apu_diff_in08u_out16s(kernel_io_desc lImgInA, kernel_io_desc lImgInB, kernel_io_desc lImgOut)
{
   vec08u*  lpvImgInA	= (vec08u*) lImgInA.pMem;
   vec08u*  lpvImgInB	= (vec08u*) lImgInB.pMem;
   vec16s*  lpvImgOut	= (vec16s*) lImgOut.pMem;
   int16s outDataSz = sizeof(int16s);
   int16s outStrideWidth = lImgOut.chunkSpan / outDataSz;

   
   difference_filter_in08u_out16s(lpvImgOut, lpvImgInA, lpvImgInB, lImgInA.chunkWidth, lImgInA.chunkHeight, lImgInA.chunkSpan, outStrideWidth);
}



void apu_diff_in16s_out16s(kernel_io_desc lImgInA, kernel_io_desc lImgInB, kernel_io_desc lImgOut)
{
	vec16s*  lpvImgInA = (vec16s*)lImgInA.pMem;
	vec16s*  lpvImgInB = (vec16s*)lImgInB.pMem;
	int dataSz = 2; //sizeof(ve32s)

	int16s inStrideWidth = lImgInA.chunkSpan / dataSz;
	vec16s*  lpvImgOut = (vec16s*)lImgOut.pMem;
	int16s outDataSz = sizeof(int16s);
	int16s outStrideWidth = lImgOut.chunkSpan / outDataSz;

	difference_filter_in16s_out16s(lpvImgOut, lpvImgInA, lpvImgInB, lImgInA.chunkWidth, lImgInA.chunkHeight, inStrideWidth, outStrideWidth);
}



void apu_diff_in16s_out32s(kernel_io_desc lImgInA, kernel_io_desc lImgInB, kernel_io_desc lImgOut)
{
	vec16s*  lpvImgInA = (vec16s*)lImgInA.pMem;
	vec16s*  lpvImgInB = (vec16s*)lImgInB.pMem;
	int16s inDataSz = sizeof(int16s);
	int16s inStrideWidth = lImgInA.chunkSpan / inDataSz;


	vec32s*  lpvImgOut = (vec32s*)lImgOut.pMem;
	int16s outDataSz = sizeof(int32s);
	int16s outStrideWidth = lImgOut.chunkSpan / outDataSz;
	
	difference_filter_in16s_out32s(lpvImgOut, lpvImgInA, lpvImgInB, lImgInA.chunkWidth, lImgInA.chunkHeight, inStrideWidth, outStrideWidth);	
}



void apu_diff_in32s_out32s(kernel_io_desc lImgInA, kernel_io_desc lImgInB, kernel_io_desc lImgOut)
{
	vec32s*  lpvImgInA = (vec32s*)lImgInA.pMem;
	vec32s*  lpvImgInB = (vec32s*)lImgInB.pMem;
	vec32s*  lpvImgOut = (vec32s*)lImgOut.pMem;
	int32s inDataSz = sizeof(int32s);
	int16s inStrideWidth = lImgInA.chunkSpan / inDataSz;

	int32s outDataSz = sizeof(int32s);
	int16s outStrideWidth = lImgOut.chunkSpan / outDataSz;

	difference_filter_in32s_out32s(lpvImgOut, lpvImgInA, lpvImgInB, lImgInA.chunkWidth, lImgInA.chunkHeight, inStrideWidth, outStrideWidth);
}



void apu_diff_in64s_out64s(kernel_io_desc lImgInA_high, kernel_io_desc lImgInA_low, kernel_io_desc lImgInB_high, kernel_io_desc lImgInB_low, kernel_io_desc lImgOut_high, kernel_io_desc lImgOut_low)
{
	vec32s*  lpvImgInA_high = (vec32s*)lImgInA_high.pMem;
	vec32s*  lpvImgInB_high = (vec32s*)lImgInB_high.pMem;
	vec32s*  lpvImgOut_high = (vec32s*)lImgOut_high.pMem;

	vec32u*  lpvImgInA_low = (vec32u*)lImgInA_low.pMem;
	vec32u*  lpvImgInB_low = (vec32u*)lImgInB_low.pMem;
	vec32u*  lpvImgOut_low = (vec32u*)lImgOut_low.pMem;

	int32s inDataSz = sizeof(int32s);
	int32s inStrideWidth = lImgInA_low.chunkSpan / inDataSz;
	int16s outDataSz = sizeof(int32s);
	int16s outStrideWidth = lImgOut_low.chunkSpan / outDataSz;

	difference_filter_in64s_out64s(lpvImgOut_high, lpvImgOut_low, lpvImgInA_high, lpvImgInA_low, lpvImgInB_high, lpvImgInB_low, lImgInA_low.chunkWidth, lImgInA_low.chunkHeight, inStrideWidth, outStrideWidth);
}


#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
