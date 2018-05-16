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
* \file dot_multiplic_acf.cpp
* \addtogroup arithmetic
* \addtogroup multiplication
* \ingroup arithmetic
* @{
* \brief Element-wise multiplication
*/
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA

#include "common_stringify_macros.h"
#include "dot_multiplic_acf.h"

/****************************************************************************/
/*!
Multiplication kernel metadata. Multiplies pixelwise two signed 16bit images.
Outputs signed 32bit multiplication result

\param  MULT_In16s_Out32s_KN	Define for Kernel name
\param 3						Number of ports
\param "Port MULT_KN_INA"		Define for name of first input image name (signed 16bit)
\param "Port MULT_KN_INB"		Define for name of second input image name (signed 16bit)
\param "Port MULT_KN_OUT"		Define name of multiplication result (signed 32bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(MULT_In16s_Out32s_K) (
	MULT_In16s_Out32s_KN,
   3,
   __port(__index(0),
          __identifier(MULT_KN_INA),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0), 
          __e0_data_type(d16s),
          __e0_size(1, 1),
          __ek_size(1, 1)),

  __port(__index(1),
		  __identifier(MULT_KN_INB),
		  __attributes(ACF_ATTR_VEC_IN),
		  __spatial_dep(0,0,0,0), 
		  __e0_data_type(d16s),
		  __e0_size(1, 1),
		  __ek_size(1, 1)),

   __port(__index(2),
          __identifier(MULT_KN_OUT),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32s),
          __e0_size(1, 1),
          __ek_size(1, 1))
);


/****************************************************************************/
/*!
Multiplication kernel metadata. Multiplies pixelwise two signed 32bit images.
Outputs signed 32bit multiplication result
\warning Does not check out of range values above/below +/-2^(31-1)
\param  MULT_In32s_Out32s_KN		Define for Kernel name
\param 3									Number of ports
\param "Port MULT_KN_INA"	Define for name of first input image (signed 32bit)
\param "Port MULT_KN_INB"	Define for name of second input image (signed 32bit)
\param "Port MULT_KN_OUT"	Define for name of multiplication result (signed 32bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(MULT_In32s_Out32s_K) (
MULT_In32s_Out32s_KN,
3,
__port(__index(0),
__identifier(MULT_KN_INA),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(1),
__identifier(MULT_KN_INB),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(2),
__identifier(MULT_KN_OUT),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1))
);

/****************************************************************************/
/*!
Multiplication kernel metadata. Multiplies pixelwise two signed 32bit images.
Outputs signed 64bit multiplication result
\param  MULT_In32s_Out64s_KN	Define for Kernel name
\param 4						Number of ports
\param "Port MULT_KN_INA"		Define for name of first input image (signed 32bit)
\param "Port MULT_KN_INB"		Define for name of second input image (signed 32bit)
\param "Port MULT_KN_OUT_HIGH"	Define for name of signed 32bit high word of multiplication result
\param "Port MULT_KN_OUT_LOW"	Define for name of unsigned 32bit low word of multiplication result
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(MULT_In32s_Out64s_K) (
MULT_In32s_Out64s_KN,
4,
__port(__index(0),
__identifier(MULT_KN_INA),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0), 
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(1),
__identifier(MULT_KN_INB),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0), 
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(2),
__identifier(MULT_KN_OUT_HIGH),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(3),
__identifier(MULT_KN_OUT_LOW),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32u),
	__e0_size(1, 1),
	__ek_size(1, 1))

);


/****************************************************************************/
/*!
Multiplication kernel metadata. Multiplies pixelwise two signed 32bit images.
Outputs signed 64bit multiplication result
\param  MULT_In32s_Out64s_KN	Define for Kernel name
\param 4						Number of ports
\param "Port MULT_KN_INA"		Define for name of first input image (signed 32bit)
\param "Port MULT_KN_INB"		Define for name of second input image (signed 32bit)
\param "Port MULT_KN_OUT_HIGH"	Define for name of signed 32bit high word of multiplication result
\param "Port MULT_KN_OUT_LOW"	Define for name of unsigned 32bit low word of multiplication result
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(MULT_In32s_In16s_Out32s_K) (
MULT_In32s_In16s_Out32s_KN,
3,
__port(__index(0),
	__identifier(MULT_KN_INA),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(1),
	__identifier(MULT_KN_INB),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(2),
	__identifier(MULT_KN_OUT),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1))

);


/****************************************************************************/
/*!
Scalar multiplication kernel metadata. Multiplies pixelwise a unsigned 8bit image with a scalar value.
Outputs signed 16bit multiplication result
\warning The type of the scalar value is int32s for convenience. It should have values in the maximal range of shorts. No checks are performed for out of range values below/above +/-2^(15-1).
\param  MULT_SCALAR_In08u_Out16s_KN		Define for Kernel name
\param 3								Number of ports
\param "Port MULT_SCALAR_KN_IN"			Define for name of input image (signed 32bit)
\param "Port MULT_SCALAR_KN_OUT"		Define for name of multiplication result (signed 32bit)
\param "Port MULT_SCALAR_KN_IN_SCALAR"	Define for name of scalar used to multiply the image (signed 32bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(MULT_SCALAR_In08u_Out16s_K)
(
MULT_SCALAR_In08u_Out16s_KN,
3,
__port(__index(0),
__identifier(MULT_SCALAR_KN_IN),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(1),
__identifier(MULT_SCALAR_KN_OUT),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(2),
__identifier(MULT_SCALAR_KN_IN_SCALAR),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1))

);

/****************************************************************************/
/*!
Scalar multiplication kernel metadata. Multiplies pixelwise a signed 32bit image with a scalar value.
Outputs signed 32bit multiplication result
\warning No checks are performed for out of range values below/above +/-2^(31-1).
\param  MULT_SCALAR_In32s_Out32s_KN		Define for Kernel name
\param 3								Number of ports
\param "Port MULT_SCALAR_KN_IN"			Define for name of the input image (signed 32bit)
\param "Port MULT_SCALAR_KN_OUT"		Define for name of multiplication result (signed 32bit)
\param "Port MULT_SCALAR_KN_IN_SCALAR"	Define for name of scalar used to multiply the image (signed 32bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(MULT_SCALAR_In32s_Out32s_K)
(
MULT_SCALAR_In32s_Out32s_KN,
3,
__port(__index(0),
__identifier(MULT_SCALAR_KN_IN),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0), 
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(1),
__identifier(MULT_SCALAR_KN_OUT),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(2),
__identifier(MULT_SCALAR_KN_IN_SCALAR),
	__attributes(ACF_ATTR_SCL_IN_FIXED),
	__spatial_dep(0, 0, 0, 0), 
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1))

);


/****************************************************************************/
/*!
Left shift kernel metadata. Shifts to the left each pixel of a unsigned 16bit image by a scalar shift value.
Outputs signed 16bit shift result
\warning No checks are performed for carry-over resulting from the left shift.
\param  LEFT_SHIFT_In16u_Out16s_KN		Define for Kernel name
\param 3								Number of ports
\param "Port MULT_SCALAR_KN_IN"			Define for name of input image (unsigned 16bit)
\param "Port MULT_SCALAR_KN_OUT"		Define for name of shift result (signed 16bit)
\param "Port MULT_SCALAR_KN_IN_SCALAR"	Define for name of scalar used to left shift the image pixels (unsigned 32bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(LEFT_SHIFT_In16u_Out16s_K)
(
LEFT_SHIFT_In16u_Out16s_KN,
3,
__port(__index(0),
	__identifier(MULT_SCALAR_KN_IN),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0), 
	__e0_data_type(d16u),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(1),
	__identifier(MULT_SCALAR_KN_OUT),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(2),
	__identifier(MULT_SCALAR_KN_IN_SCALAR),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32u),
	__e0_size(1, 1),
	__ek_size(1, 1))

);



/****************************************************************************/
/*!
Right shift kernel metadata. Shifts to the right each pixel of a signed 64bit image by a scalar shift value. Outputs signed 64bit shift result

\param RIGHT_SHIFT_In64s_Out64s_KN		Define for Kernel name
\param 5								Number of ports
\param "Port RIGHT_SHIFT_In64s_HIGH"	Define for name of the signed 32bit high word of the input image 
\param "Port RIGHT_SHIFT_In64s_LOW"		Define for name of the unsigned 32bit low word of the input image
\param "Port RIGHT_SHIFT_Out64s_HIGH"	Define for name of the signed 32bit high word of the shift result
\param "Port RIGHT_SHIFT_Out64s_LOW"	Define for name of the unsigned 32bit low word of the shift result
\param "Port RIGHT_SHIFT_Fact"			Define name of the unsigned 32bit scalar used to left shift the image pixels.
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(RIGHT_SHIFT_In64s_Out64s_K)
(
RIGHT_SHIFT_In64s_Out64s_KN,
5,
__port(__index(0),
	__identifier(RIGHT_SHIFT_In64s_HIGH),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(1),
	__identifier(RIGHT_SHIFT_In64s_LOW),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32u),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(2),
	__identifier(RIGHT_SHIFT_Out64s_HIGH),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(3),
	__identifier(RIGHT_SHIFT_Out64s_LOW),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32u),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(4),
	__identifier(RIGHT_SHIFT_Fact),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1))
);




/****************************************************************************/
/*!
Right shift kernel metadata. Shifts to the right each pixel of a signed 64bit image by a scalar shift value. Outputs signed 32bit shift result

\param RIGHT_SHIFT_In64s_Out32s_KN		Define for Kernel name
\param 5									Number of ports
\param "Port RIGHT_SHIFT_In64s_HIGH"	Define for name of the signed 32bit high word of the input image
\param "Port RIGHT_SHIFT_In64s_LOW"		Define for name of the unsigned 32bit low word of the input image
\param "Port RIGHT_SHIFT_Out64s_HIGH"	Define for name of the signed 32bit high word of the shift result
\param "Port RIGHT_SHIFT_Out64s_LOW"	Define for name of the unsigned 32bit low word of the shift result
\param "Port RIGHT_SHIFT_Fact"			Define for name of the unsigned 32bit scalar used to left shift the image pixels.
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(RIGHT_SHIFT_In64s_Out32s_K)
(
RIGHT_SHIFT_In64s_Out32s_KN,
4,
__port(__index(0),
	__identifier(RIGHT_SHIFT_In64s_HIGH),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(1),
	__identifier(RIGHT_SHIFT_In64s_LOW),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32u),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(2),
	__identifier(RIGHT_SHIFT_OUT32s),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(3),
	__identifier(RIGHT_SHIFT_Fact),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1))
);

/****************************************************************************/
/*!
Left shift kernel metadata by one. Shifts to the left each pixel of a signed 32bit image by one.Outputs signed 32bit shift result

\warning No checks are performed for carry-over resulting from the left shift.
\param  MULT_BY_2_In32s_Out32s_KN			Define for Kernel name
\param 2									Number of ports
\param "Port MULT_BY_2_In32s_Out32s_KN_IN"	Define for name of the input image (signed 32bit)
\param "Port MULT_BY_2_In32s_Out32s_KN_OUT"	Define for name of the shift result (signed 32bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(MULT_BY_2_In32s_Out32s_K)
(
MULT_BY_2_In32s_Out32s_KN,
2,
__port(__index(0),
	__identifier(MULT_BY_2_In32s_Out32s_KN_IN),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0), 
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(1),
	__identifier(MULT_BY_2_In32s_Out32s_KN_OUT),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1))

);



/****************************************************************************/
/*!
Left shift kernel metadata by one. Shifts to the left each pixel of a signed 32bit image by one. Outputs signed 64bit shift result

\param  MULT_BY_2_In32s_Out64s_KN				Define for Kernel name
\param 3										Number of ports
\param "Port MULT_BY_2_In32s_Out32s_KN_IN"		Define for name of the input image (signed 32bit)
\param "Port MULT_BY_2_In32s_Out32s_KN_OUT_H"	Define name of signed 32bit high word of shift result
\param "Port MULT_BY_2_In32s_Out32s_KN_OUT_LOW"	Define name of unsigned 32bit low word of shift result
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(MULT_BY_2_In32s_Out64s_K)
(
MULT_BY_2_In32s_Out64s_KN,
3,
__port(__index(0),
	__identifier(MULT_BY_2_In32s_Out32s_KN_IN),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(1),
	__identifier(MULT_BY_2_In32s_Out32s_KN_OUT_H),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(2),
	__identifier(MULT_BY_2_In32s_Out32s_KN_OUT_LOW),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32u),
	__e0_size(1, 1),
	__ek_size(1, 1))

);

/****************************************************************************/
/*!
Left shift kernel metadata by one for fixed point numbers in Q3.28 format. Shifts to the left by one each pixel of a signed 32bit image in fixed point Q3.28 format.
Outputs signed 64bit shift result in fixed point Q3.28 format

\param  MULT_BY_2_Q3_28_In32s_Out64s_KN		Define for Kernel name
\param 3									Number of ports
\param "Port MULT_BY_2_In32s_Out32s_KN_IN"	Define for name of the input image (signed 32bit)
\param "Port MULT_BY_2_Q3_28_KN_OUT_INT"	Define name of signed 32bit integer part of shift result (in normal integer format)
\param "Port MULT_BY_2_Q3_28_KN_OUT_FRAC"	Define name of unsigned 32bit fractional part of shift result (in fixed point Q3.28 format).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(MULT_BY_2_Q3_28_In32s_Out64s_K)
(
MULT_BY_2_Q3_28_In32s_Out64s_KN,
3,
__port(__index(0),
	__identifier(MULT_BY_2_In32s_Out32s_KN_IN),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(1),
	__identifier(MULT_BY_2_Q3_28_KN_OUT_INT),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
	
__port(__index(2),
	__identifier(MULT_BY_2_Q3_28_KN_OUT_FRAC),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "dot_multiplic_apu.h"

void apu_dot_mult_in16s_out32s(kernel_io_desc lImgInA, kernel_io_desc lImgInB, kernel_io_desc lImgOut)
{
	vec16s*  lpvImgInA = (vec16s*) lImgInA.pMem;
	vec16s*  lpvImgInB = (vec16s*) lImgInB.pMem;
	vec32s*  lpvImgOut = (vec32s*)lImgOut.pMem;
	int inDataSpan = lImgInA.chunkSpan / sizeof(int16s);
	int outDataSpan = lImgOut.chunkSpan / sizeof(int32s);

	dot_mult_in16s_out32s_filter(lpvImgOut, lpvImgInA, lpvImgInB, lImgInA.chunkWidth, lImgInA.chunkHeight, inDataSpan, outDataSpan);
}

void apu_dot_mult_in32s_out32s(kernel_io_desc lImgInA, kernel_io_desc lImgInB, kernel_io_desc lImgOut)
{
	vec32s*  lpvImgInA = (vec32s*)lImgInA.pMem;
	vec32s*  lpvImgInB = (vec32s*)lImgInB.pMem;
	vec32s*  lpvImgOut = (vec32s*)lImgOut.pMem;
	int inDataSpan = lImgInA.chunkSpan / sizeof(int32s);
	int outDataSpan = lImgOut.chunkSpan / sizeof(int32s);
	
	dot_mult_in32s_out32s_filter(lpvImgOut, lpvImgInA, lpvImgInB, lImgInA.chunkWidth, lImgInA.chunkHeight,  inDataSpan,  outDataSpan);
}

void apu_dot_mult_in32s_out64s(kernel_io_desc lImgInA, kernel_io_desc lImgInB, kernel_io_desc lImgOut_high, kernel_io_desc lImgOut_low)
{
   vec32s*  lpvImgInA	= (vec32s*) lImgInA.pMem;
   vec32s*  lpvImgInB	= (vec32s*) lImgInB.pMem;

   vec32s*  lpvImgOut_high = (vec32s*)lImgOut_high.pMem;
   vec32u*  lpvImgOut_low = (vec32u*)lImgOut_low.pMem;
   int inDataSpan = lImgInA.chunkSpan / sizeof(int32s);
   int outDataSpan = lImgOut_low.chunkSpan / sizeof(int32s);

   dot_mult_in32s_out64s_filter(lpvImgOut_high, lpvImgOut_low, lpvImgInA, lpvImgInB, lImgInA.chunkWidth, lImgInA.chunkHeight, inDataSpan, outDataSpan);
}



void apu_dot_mult_in32s_in16s_out32s(kernel_io_desc lImgInA, kernel_io_desc lImgInB, kernel_io_desc lImgOut)
{
	vec32s*  lpvImgInA = (vec32s*)lImgInA.pMem;
	vec16s*  lpvImgInB = (vec16s*)lImgInB.pMem;

	vec32s*  lpvImgOut = (vec32s*)lImgOut.pMem;
	
	int inDataSpan = lImgInA.chunkSpan / sizeof(int32s);
	int outDataSpan = lImgOut.chunkSpan / sizeof(int32s);

	dot_mult_in32s_in16s_out32s_filter(lpvImgOut, lpvImgInA, lpvImgInB, lImgInA.chunkWidth, lImgInA.chunkHeight, inDataSpan, outDataSpan);
}


void apu_dot_mult_scalar_in08u_out16s(kernel_io_desc lImgInA, kernel_io_desc lImgOut, kernel_io_desc scalarVal )
{
	vec08u*  lpvImgInA = (vec08u*)lImgInA.pMem;
	int32s  scVal = *((int32s*) scalarVal.pMem);
	vec16s*  lpvImgOut = (vec16s*)lImgOut.pMem;
	int inDataSpan = lImgInA.chunkSpan / sizeof(int08u);
	int outDataSpan = lImgOut.chunkSpan / sizeof(int16s);

	dot_mult_scalar_in08u_out16s_filter(lpvImgOut, lpvImgInA, scVal, lImgInA.chunkWidth, lImgInA.chunkHeight, inDataSpan, outDataSpan);
}

void apu_dot_left_shift_in16u_out16s(kernel_io_desc lImgInA, kernel_io_desc lImgOut, kernel_io_desc inputShiftFact)
{
	vec16u*  lpvImgInA = (vec16u*)lImgInA.pMem;
	vec16s*  lpvImgOut = (vec16s*)lImgOut.pMem;
	int16s sF = *((int32u*)inputShiftFact.pMem);
	vec16s  leftShiftFact = vec16s((int16s) sF);
	int inDataSpan = lImgInA.chunkSpan / sizeof(int16u);
	int outDataSpan = lImgOut.chunkSpan / sizeof(int16s);
	
	lsh_in16u_out16s_filter(lpvImgOut, lpvImgInA, &leftShiftFact, lImgInA.chunkWidth, lImgInA.chunkHeight, inDataSpan, outDataSpan);	
}

void apu_dot_mult_scalar_in32s_out32s(kernel_io_desc lImgInA, kernel_io_desc lImgOut, kernel_io_desc scalarVal)
{
	vec32s*  lpvImgInA = (vec32s*)lImgInA.pMem;
	int32s  scVal = *((int32s*)scalarVal.pMem);
	vec32s*  lpvImgOut = (vec32s*)lImgOut.pMem;
	int inDataSpan = lImgInA.chunkSpan / sizeof(int32s);
	int outDataSpan = lImgOut.chunkSpan / sizeof(int32s);
	
	dot_mult_scalar_in32s_out32s_filter(lpvImgOut, lpvImgInA, scVal, lImgInA.chunkWidth, lImgInA.chunkHeight, inDataSpan, outDataSpan);
}

void apu_dot_lsh1_in32s_out32s(kernel_io_desc lImgIn, kernel_io_desc lImgOut)
{
	vec32s*  lpvImgIn = (vec32s*)lImgIn.pMem;
	vec32s*  lpvImgOut = (vec32s*)lImgOut.pMem;
	int32s	scalar = 2;
	int inDataSpan = lImgIn.chunkSpan / sizeof(int32s);
	int outDataSpan = lImgOut.chunkSpan / sizeof(int32s);
	vec08u shftFact = 1;
	lsh_in32s_out32s_filter(lpvImgOut, lpvImgIn, &shftFact, lImgIn.chunkWidth, lImgIn.chunkHeight, inDataSpan,outDataSpan);
}

void apu_dot_lsh1_in32u_out32u(kernel_io_desc lImgIn, kernel_io_desc lImgOut)
{
	vec32u*  lpvImgIn = (vec32u*)lImgIn.pMem;
	vec32u*  lpvImgOut = (vec32u*)lImgOut.pMem;
	int32u	scalar = 2;
	int inDataSpan = lImgIn.chunkSpan / sizeof(int32u);
	int outDataSpan = lImgOut.chunkSpan / sizeof(int32u);
	vec08u shftFact = 1;
	lsh_in32u_out32u_filter(lpvImgOut, lpvImgIn, &shftFact, lImgIn.chunkWidth, lImgIn.chunkHeight, inDataSpan,  outDataSpan);
}

void apu_dot_lsh1_in32s_out64s(kernel_io_desc lImgIn, kernel_io_desc lImgOut_high, kernel_io_desc lImgOut_low)
{
	vec32s*  lpvImgIn = (vec32s*)lImgIn.pMem;
	vec32u*  lpvImgOut_low = (vec32u*)lImgOut_low.pMem;
	vec32s*  lpvImgOut_high = (vec32s*)lImgOut_high.pMem;
	int inDataSpan = lImgIn.chunkSpan / sizeof(int32s);
	int outDataSpan = lImgOut_high.chunkSpan / sizeof(int32s);
	vec08u shftFact = 1;
	lsh_in32s_out64s_filter(lpvImgOut_high, lpvImgOut_low, lpvImgIn, &shftFact, lImgIn.chunkWidth, lImgIn.chunkHeight, inDataSpan, outDataSpan);
}


void apu_dot_lsh1_in32u_out64u(kernel_io_desc lImgIn, kernel_io_desc lImgOut_high, kernel_io_desc lImgOut_low)
{
	vec32u*  lpvImgIn = (vec32u*)lImgIn.pMem;
	vec32u*  lpvImgOut_low = (vec32u*)lImgOut_low.pMem;
	vec32u*  lpvImgOut_high = (vec32u*)lImgOut_high.pMem;
	int inDataSpan = lImgIn.chunkSpan / sizeof(int32u);
	int outDataSpan = lImgOut_high.chunkSpan / sizeof(int32u);
	vec08u shftFact = 1;
	lsh_in32u_out64u_filter(lpvImgOut_high, lpvImgOut_low, lpvImgIn, &shftFact, lImgIn.chunkWidth, lImgIn.chunkHeight,inDataSpan,  outDataSpan);
}


void apu_dot_lsh1_in32s_Q3_28_out64s(kernel_io_desc lImgIn, kernel_io_desc lImgOut_int, kernel_io_desc lImgOut_frac)
{
	vec32s*  lpvImgIn = (vec32s*)lImgIn.pMem;
	vec32s*  lpvImgOut_frac = (vec32s*)lImgOut_frac.pMem;
	vec32s*  lpvImgOut_int = (vec32s*)lImgOut_int.pMem;
	int inDataSpan = lImgIn.chunkSpan / sizeof(int32u);
	int outDataSpan = lImgOut_int.chunkSpan / sizeof(int32u);
	vec08u shftFact = 1;
	lsh_in32s_Q3_28_out64s_filter(lpvImgOut_int, lpvImgOut_frac, lpvImgIn, &shftFact, lImgIn.chunkWidth, lImgIn.chunkHeight, inDataSpan,  outDataSpan);
}


void apu_dot_right_shift_in64s_out64s(kernel_io_desc lImgIn_high, kernel_io_desc lImgIn_low, kernel_io_desc lImgOut_high, kernel_io_desc lImgOut_low, kernel_io_desc shiftFact)
{
	vec32s* dst_high=	(vec32s*) lImgOut_high.pMem;
	vec32u* dst_low =	(vec32u*) lImgOut_low.pMem;
	vec32s* in_high =	(vec32s*) lImgIn_high.pMem;
	vec32u* in_low	=	(vec32u*) lImgIn_low.pMem;
	vec08u rightShiftFact  = (vec08u) (int08u)(*((int32s*) shiftFact.pMem));

	int16s bw = lImgIn_low.chunkWidth;
	int16s bh = lImgIn_low.chunkHeight;
	int16s inStrideWidth = lImgIn_low.chunkSpan / sizeof(int32u);
	int16s outStrideWidth = lImgOut_low.chunkSpan / sizeof(int32u);

	rsh_in64s_out64s_filter(dst_high, dst_low, in_high, in_low, &rightShiftFact, bw, bh, inStrideWidth, outStrideWidth);

}

void apu_dot_right_shift_in64s_out32s(kernel_io_desc lImgIn_high, kernel_io_desc lImgIn_low, kernel_io_desc lImgOut, kernel_io_desc shiftFact)
{
	vec32s* dst = (vec32s*)lImgOut.pMem;
	vec32s* in_high = (vec32s*)lImgIn_high.pMem;
	vec32u* in_low = (vec32u*)lImgIn_low.pMem;
	vec08u rightShiftFact = (vec08u)(int08u)(*((int32s*)shiftFact.pMem));

	int16s bw = lImgIn_low.chunkWidth;
	int16s bh = lImgIn_low.chunkHeight;
	int16s inStrideWidth = lImgIn_low.chunkSpan / sizeof(int32u);
	int16s outStrideWidth = lImgOut.chunkSpan / sizeof(int32u);

	rsh_in64s_out32s_filter(dst, in_high, in_low, &rightShiftFact, bw, bh, inStrideWidth, outStrideWidth);
}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
