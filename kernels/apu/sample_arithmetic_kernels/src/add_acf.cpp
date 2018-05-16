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
* \file apu_acf.cpp
* \addtogroup arithmetic
* \addtogroup addition
* \ingroup arithmetic
* @{
* \brief Element-wise addition
*/
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA

#include "add_acf.h"

/****************************************************************************/
/*!
Addition kernel metadata. Adds pixelwise two unsigned 8bit images. 
Outputs 16bit unsigned addition result.

\param  ADD_KN				Define for Kernel name
\param 3					Number of ports
\param "Port ADD_KN_INA"	Define for name of first input image (unsigned 8bit)
\param "Port ADD_KN_INB"	Define for name of second input image (unsigned 8bit)
\param "Port ADD_KN_OUT"	Define for name of addition result of the two images (unsigned 16bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(ADD_K)//_kernel_info_apu_add
(
   ADD_KN,
   3,
   __port(__index(0),
          __identifier(ADD_KN_INA),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier(ADD_KN_INB),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(2),
          __identifier(ADD_KN_OUT),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);



/****************************************************************************/
/*!
Addition kernel metadata. Adds pixelwise two signed 16bit images. 
Outputs signed 32bit addition result

\param  ADD_In16s_Out32s_KN		Define for Kernel name
\param 3						Number of ports
\param "Port ADD_KN_INA"		Define for name of first input image (signed 16bit)
\param "Port ADD_KN_INB"		Define for name of second input image (signed 16bit)
\param "Port ADD_KN_OUT"		Define for name of addition result (signed 32bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(ADD_In16s_Out32s_K)
(
	ADD_In16s_Out32s_KN,
	3,
	__port(__index(0),
		__identifier(ADD_KN_INA),
		__attributes(ACF_ATTR_VEC_IN),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d16s),
		__e0_size(1, 1),
		__ek_size(1, 1)),
	__port(__index(1),
		__identifier(ADD_KN_INB),
		__attributes(ACF_ATTR_VEC_IN),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d16s),
		__e0_size(1, 1),
		__ek_size(1, 1)),
	__port(__index(2),
		__identifier(ADD_KN_OUT),
		__attributes(ACF_ATTR_VEC_OUT),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d32s),
		__e0_size(1, 1),
		__ek_size(1, 1))
);

/****************************************************************************/
/*!
Addition kernel metadata. Adds pixelwise two signed 32bit images. 
Outputs signed 32bit addition result
\warning Does not check out of range values above/below +/-2^(31-1)

\param  ADD_In32s_Out32s_KN		Define for Kernel name
\param 3						Number of ports
\param "Port ADD_KN_INA"		Define for name of first input image (signed 32bit)
\param "Port ADD_KN_INB"		Define for name of second input image (signed 32bit)
\param "Port ADD_KN_OUT"		Define for name  of addition result (signed 32bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(ADD_In32s_Out32s_K)
(
ADD_In32s_Out32s_KN,
3,
__port(__index(0),
	__identifier(ADD_KN_INA),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(1),
	__identifier(ADD_KN_INB),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(2),
	__identifier(ADD_KN_OUT),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1))
);


/****************************************************************************/
/*!
Addition kernel metadata. Adds pixelwise two images in fixed point format of the type Q3.28. 
Outputs signed 32bit the integer addition result (in normal integer format) and the fractional addition result in Q3.28 format

\warning Does not check out of range values above/below +/-2^(31-1)

\param  ADD_32S_Q3_28_KN		Define for Kernel name
\param 4						Number of ports
\param "Port ADD_KN_INA"		Define for name  of first input image (signed 32bit) in fixed point Q3.28 format
\param "Port ADD_KN_INB"		Define for name  of second input image (signed 32bit) in fixed point Q3.28 format 
\param "Port ADD_KN_OUT"		Define for name  of 32bit signed integer part of addition result of the two images.
\param "Port ADD_KN_OUT_FRAC"	Define for name  of 32bit fractional part of addition result of the two images in fixed point Q3.28 format.
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(ADD_32S_Q3_28_K)
(
ADD_32S_Q3_28_KN,
4,
__port(__index(0),
	__identifier(ADD_KN_INA),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(1),
	__identifier(ADD_KN_INB),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(2),
	__identifier(ADD_KN_OUT),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(3),
	__identifier(ADD_KN_OUT_FRAC),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1))

);


/****************************************************************************/
/*!
Addition kernel metadata. Adds pixelwise two signed 64bit images.
Outputs signed 64bit addition result
\warning Does not check out of range values above 2^(63-1)

\param  ADD_In64s_Out64s_KN		Define for Kernel name
\param 6						Number of ports
\param "Port ADD_KN_INA_HIGH"	Define for name of signed 32bit high word of first input image 
\param "Port ADD_KN_INA_LOW"	Define for name of unsigned 32bit low word of firstinput image 
\param "Port ADD_KN_INB_HIGH"	Define for name of signed 32bit high word of second input image
\param "Port ADD_KN_INB_LOW"	Define for name of unsigned 32bit low word of second input image
\param "Port ADD_KN_OUT_HIGH"	Define for name of signed 32bit high word of addition result
\param "Port ADD_KN_OUT_LOW"	Define for name of unsigned 32bit low word of addition result
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(ADD_In64s_Out64s_K)
(
ADD_In64s_Out64s_KN,
6,
	__port(__index(0),
		__identifier(ADD_KN_INA_HIGH),
		__attributes(ACF_ATTR_VEC_IN),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d32s),
		__e0_size(1, 1),
		__ek_size(1, 1)),

	__port(__index(1),
		__identifier(ADD_KN_INA_LOW),
		__attributes(ACF_ATTR_VEC_IN),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d32u),
		__e0_size(1, 1),
		__ek_size(1, 1)),

	__port(__index(2),
		__identifier(ADD_KN_INB_HIGH),
		__attributes(ACF_ATTR_VEC_IN),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d32s),
		__e0_size(1, 1),
		__ek_size(1, 1)),

	__port(__index(3),
		__identifier(ADD_KN_INB_LOW),
		__attributes(ACF_ATTR_VEC_IN),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d32u),
		__e0_size(1, 1),
		__ek_size(1, 1)),
		
	__port(__index(4),
		__identifier(ADD_KN_OUT_HIGH),
		__attributes(ACF_ATTR_VEC_OUT),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d32s),
		__e0_size(1, 1),
		__ek_size(1, 1)),
		
	__port(__index(5),
		__identifier(ADD_KN_OUT_LOW),
		__attributes(ACF_ATTR_VEC_OUT),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d32u),
		__e0_size(1, 1),
		__ek_size(1, 1))
);
#endif //#ifdef ACF_KERNEL_METADATA



#ifdef ACF_KERNEL_IMPLEMENTATION

#include "add_apu.h"

void apu_add(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out0)
{
	vec08u*        lIn0 = (vec08u*)in0.pMem;
	vec08u*        lIn1 = (vec08u*)in1.pMem;
	vec16u*        lOut0 = (vec16u*)out0.pMem;

	int32s inStrideW = in0.chunkSpan / sizeof(int08u);
	int32s outStrideW = out0.chunkSpan / sizeof(int16s);


	add(lOut0, lIn0, lIn1, in0.chunkWidth, in0.chunkHeight, inStrideW, outStrideW);
}

void apu_add_in16s_out32s(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out0)
{
	vec16s*        lIn0 = (vec16s*)in0.pMem;
	vec16s*        lIn1 = (vec16s*)in1.pMem;
	vec32s*        lOut0 = (vec32s*)out0.pMem;
	int32s inStrideW = in0.chunkSpan / sizeof(int16s);
	int32s outStrideW = out0.chunkSpan / sizeof(int32s);

	add_in16s_out32s(lOut0, lIn0, lIn1, in0.chunkWidth, in0.chunkHeight, inStrideW, outStrideW);
}

void apu_add_in32s_out32s(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out0)
{
	vec32s*        lIn0 = (vec32s*)in0.pMem;
	vec32s*        lIn1 = (vec32s*)in1.pMem;
	vec32s*        lOut0 = (vec32s*)out0.pMem;

	int32s inStrideW = in0.chunkSpan / sizeof(int32s);
	int32s outStrideW = out0.chunkSpan / sizeof(int32s);
	add_in32s_out32s(lOut0, lIn0, lIn1, in0.chunkWidth, in0.chunkHeight, inStrideW, outStrideW);
}


void apu_add_in32Q3_28_out32Q3_28(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc outInt, kernel_io_desc outFrac)
{
	vec32s*        lIn0 = (vec32s*)in0.pMem;
	vec32s*        lIn1 = (vec32s*)in1.pMem;
	vec32s*        lOutInt = (vec32s*)outInt.pMem;
	vec32s*        lOutFrac = (vec32s*)outFrac.pMem;

	int32s inStrideW = in0.chunkSpan / sizeof(int32s);
	int32s outStrideW = outInt.chunkSpan / sizeof(int32s);
	add_in32Q3_28_out32Q3_28(lOutInt, lOutFrac, lIn0, lIn1, in0.chunkWidth, in0.chunkHeight, inStrideW, outStrideW);
}



void apu_add_in64s_out64s(kernel_io_desc in0_high, kernel_io_desc in0_low, kernel_io_desc in1_high, kernel_io_desc in1_low, kernel_io_desc out0_high, kernel_io_desc out0_low)
{
	vec32s*        lIn0_high = (vec32s*)in0_high.pMem;
	vec32s*        lIn1_high = (vec32s*)in1_high.pMem;
	vec32s*        lOut0_high = (vec32s*)out0_high.pMem;

	vec32u*        lIn0_low = (vec32u*)in0_low.pMem;
	vec32u*        lIn1_low = (vec32u*)in1_low.pMem;
	vec32u*        lOut0_low = (vec32u*)out0_low.pMem;
	int32s inStrideW = in0_high.chunkSpan / sizeof(int32s);
	int32s outStrideW = out0_high.chunkSpan / sizeof(int32s);

	add_in64s_out64s(lOut0_high, lOut0_low, lIn0_high, lIn0_low, lIn1_high, lIn1_low, in0_high.chunkWidth, in0_high.chunkHeight, inStrideW, outStrideW);
}


#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
