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
* \file comparison_acf.cpp
* \addtogroup comparison
* \addtogroup el_comparison Comparison
* \ingroup comparison
* @{
* \brief Element-wise comparison
*/
#ifdef ACF_KERNEL_METADATA
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#include "comparison_acf.h"



/****************************************************************************/
/*!
Lower kernel metadata. Compares pixelwise two unsigned 8bit images.
Outputs unsigned 8bit comparison result. Is true if  INPUTA < INPUTB

\param  LOWER_KN			Define for Kernel name
\param 3					Number of ports
\param "Port LOWER_KN_INA"	Define for name of first input image (unsigned 8bit)
\param "Port LOWER_KN_INB"	Define for name of second input image (unsigned 8bit)
\param "Port LOWER_KN_OUT"	Define for name of comparison result of the two images (unsigned 8bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(LOWER_K)//_kernel_info_apu_add
(
   LOWER_KN,
   3,
   __port(__index(0),
          __identifier(LOWER_KN_INA),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier(LOWER_KN_INB),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(2),
          __identifier(LOWER_KN_OUT),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
		  __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);




/****************************************************************************/
/*!
Lower kernel metadata. Compares pixelwise an unsigned 8bit image with a scalar value.
Outputs unsigned 8bit comparison result. Is true if  INPUTA < INPUTB

\param  LOWER_SCALAR_KN			Define for Kernel name
\param 3					Number of ports
\param "Port LOWER_KN_INA"	Define for name of first input image (unsigned 8bit)
\param "Port LOWER_KN_SCALAR"	Define for name of scalar value(unsigned 8bit)
\param "Port LOWER_KN_OUT"	Define for name of comparison result of the two images (unsigned 8bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(LOWER_SCALAR_K)//_kernel_info_apu_add
(
LOWER_SCALAR_KN,
3,
__port(__index(0),
__identifier(LOWER_KN_INA),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),
__port(__index(1),
__identifier(LOWER_KN_SCALAR),
__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),

__port(__index(2),
__identifier(LOWER_KN_OUT),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1))
);



/****************************************************************************/
/*!
Lower kernel metadata. Compares pixelwise two signed 16bit images.
Outputs unsigned 8bit comparison result. Is true if  INPUTA < INPUTB

\param  LOWER_In16s_KN		Define for Kernel name
\param 3					Number of ports
\param "Port LOWER_KN_INA"	Define for name of first input image (signed 16bit)
\param "Port LOWER_KN_INB"	Define for name of second input image (signed 16bit)
\param "Port LOWER_KN_OUT"	Define for name of comparison result of the two images (unsigned 8bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(LOWER_In16s_K)
(
LOWER_In16s_KN,
3,
__port(__index(0),
__identifier(LOWER_KN_INA),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d16s),
__e0_size(1, 1),
__ek_size(1, 1)),
__port(__index(1),
__identifier(LOWER_KN_INB),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d16s),
__e0_size(1, 1),
__ek_size(1, 1)),
__port(__index(2),
__identifier(LOWER_KN_OUT),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1))
);



/****************************************************************************/
/*!
Lower kernel metadata. Compares pixelwise two signed 32bit images.
Outputs unsigned 8bit comparison result. Is true if  INPUTA < INPUTB

\param  LOWER_In32s_KN		Define for Kernel name
\param 3					Number of ports
\param "Port LOWER_KN_INA"	Define for name of first input image (signed 32bit)
\param "Port LOWER_KN_INB"	Define for name of second input image (signed 32bit)
\param "Port LOWER_KN_OUT"	Define for name of comparison result of the two images. (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(LOWER_In32s_K)
(
LOWER_In32s_KN,
3,
__port(__index(0),
	__identifier(LOWER_KN_INA),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(1),
	__identifier(LOWER_KN_INB),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(2),
	__identifier(LOWER_KN_OUT),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(1, 1))
);



/****************************************************************************/
/*!
Absolute Lower kernel metadata. Compares pixelwise the absolute values of two signed 32bit images.
Outputs unsigned 8bit comparison result. Is true if  abs(INPUTA) <= abs(INPUTB)

\param  ABS_LOWER_In32s_KN	Define for Kernel name
\param 3					Number of ports
\param "Port LOWER_KN_INA"	Define for name of first input image (signed 32bit)
\param "Port LOWER_KN_INB"	Define for name of second input image (signed 32bit)
\param "Port LOWER_KN_OUT"	Define for name ofcomparison result of the two images (unsigned  8bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(ABS_LOWER_In32s_K)
(
ABS_LOWER_In32s_KN,
3,
__port(__index(0),
	__identifier(LOWER_KN_INA),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(1),
	__identifier(LOWER_KN_INB),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
	__port(__index(2),

__identifier(LOWER_KN_OUT),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(1, 1))
);




/****************************************************************************/
/*!
Absolute Lower kernel metadata. Compares pixelwise the absolute values of signed 32bit image with a single scalar unsigned 16bit value.
Outputs unsigned 8bit comparison result. Is true if  abs(INPUTA) <= scalar

\param  ABS_LOWER_In32s_scalar16u_KN Define for Kernel name
\param 3					Number of ports
\param "Port LOWER_KN_INA"	Define for name of first input image (signed 32bit)
\param "Port LOWER_KN_INB"	Define for name of scalar value (unsigned 16bit)
\param "Port LOWER_KN_OUT"	Define for name of comparison result of the two images (unsigned 8bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(ABS_LOWER_In32s_scalar16u_K)
(
ABS_LOWER_In32s_scalar16u_KN,
3,
__port(__index(0),
__identifier(LOWER_KN_INA),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d32s),
__e0_size(1, 1),
__ek_size(1, 1)),

__port(__index(1),
__identifier(LOWER_KN_INB),
__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d16u),
__e0_size(1, 1),
__ek_size(1, 1)),
__port(__index(2),

__identifier(LOWER_KN_OUT),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1))
);

/****************************************************************************/
/*!
Lower or equal kernel metadata. Compares pixelwise the values of two signed 32bit images.
Outputs unsigned 8bit comparison result. Is true if INPUTA <= INPUTB

\param  LOWER_EQUAL_In32s_KN Define for Kernel name
\param 3					Number of ports
\param "Port LOWER_KN_INA"	Define for name of first input image (signed 32bit)
\param "Port LOWER_KN_INB"	Define for name of second input image (signed 32bit)
\param "Port LOWER_KN_OUT"	Define for name of comparison result of the two images (unsigned 8bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(LOWER_EQUAL_In32s_K)
(
LOWER_EQUAL_In32s_KN,
3,
__port(__index(0),
	__identifier(LOWER_KN_INA),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(1),
	__identifier(LOWER_KN_INB),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(2),
	__identifier(LOWER_KN_OUT),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(1, 1))

);



/****************************************************************************/
/*!
MASK operator kernel metadata. Returns the values of the pixels where mask is not zero.
Outputs unsigned 8bit mask result. Is IN_IMG if  (MASK != 0) otherwise 0

\param  MASK_K				Define for Kernel name
\param 3					Number of ports
\param "Port MASK_KN_IN_IMG"	Define for name of first input image (unsigned 16bit)
\param "Port MASK_KN_IN_MASK"	Define for name of second input image (unsigned 16bit)
\param "Port MASK_KN_OUT"	Define for name of comparison result of the two images (unsigned 16bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(MASK8b_K)
(
MASK8b_KN,
3,
__port(__index(0),
__identifier(MASK_KN_IN_IMG),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),
__port(__index(1),
__identifier(MASK_KN_IN_MASK),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),
__port(__index(2),
__identifier(MASK_KN_OUT),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1))

);



/****************************************************************************/
/*!
AND operator kernel metadata. Pixelwise "AND" operator between two unsigned 8bit images.
Outputs unsigned 8bit comparison result. Is true if  (INPUTA != 0) && (INPUTB != 0)

\param  AND_K				Define for Kernel name
\param 3					Number of ports
\param "Port AND_KN_INA"	Define for name of first input image (unsigned 16bit)
\param "Port AND_KN_INB"	Define for name of second input image (unsigned 16bit)
\param "Port AND_KN_OUT"	Define for name of comparison result of the two images (unsigned 16bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(AND_K)
(
AND_KN,
3,
__port(__index(0),
__identifier(AND_KN_INA),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),
__port(__index(1),
__identifier(AND_KN_INB),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),
__port(__index(2),
__identifier(AND_KN_OUT),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1))

);



/****************************************************************************/
/*!
AND operator kernel metadata. Pixelwise "AND" operator between two unsigned 16bit images.
Outputs unsigned 16bit comparison result. Is true if  (INPUTA != 0) && (INPUTB != 0)

\param  AND_In16u_Out16u_K	Define for Kernel name
\param 3					Number of ports
\param "Port AND_KN_INA"	Define for name of first input image (unsigned 16bit)
\param "Port AND_KN_INB"	Define for name of second input image (unsigned 16bit)
\param "Port AND_KN_OUT"	Define for name of comparison result of the two images (unsigned 16bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(AND_In16u_Out16u_K)
(
AND_In16u_Out16u_KN,
3,
__port(__index(0),
	__identifier(AND_KN_INA),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16u),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(1),
	__identifier(AND_KN_INB),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16u),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(2),
	__identifier(AND_KN_OUT),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16u),
	__e0_size(1, 1),
	__ek_size(1, 1))

);




/****************************************************************************/
/*!
AND operator kernel metadata. Pixelwise "AND" operator between three unsigned 16bit images.
Outputs unsigned 16bit comparison result. Is true if  (INPUTA != 0) && (INPUTB != 0) && (INPUTC != 0)

\param  AND_3Pt_In16u_Out16u_K	Define for Kernel name
\param 3						Number of ports
\param "Port AND_KN_INA"		Define for name of first input image (unsigned 16bit)
\param "Port AND_KN_INB"		Define for name of second input image (unsigned 16bit)
\param "Port AND_KN_INC"		Define for name of third input image (unsigned 16bit)
\param "Port AND_KN_OUT"		Define for name of comparison result of the two images (unsigned 16bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(AND_3Pt_In16u_Out16u_K)
(
AND_3Pt_In16u_Out16u_KN,
4,
__port(__index(0),
	__identifier(AND_KN_INA),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16u),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(1),
	__identifier(AND_KN_INB),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16u),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(2),
	__identifier(AND_KN_INC),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16u),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(3),
	__identifier(AND_KN_OUT),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16u),
	__e0_size(1, 1),
	__ek_size(1, 1))

);



/****************************************************************************/
/*!
AND operator kernel metadata. Pixelwise "AND" operator between two unsigned 8bit images.
Outputs unsigned 16bit and operator result. Is true if  (INPUTA != 0) && (INPUTB != 0)

\param  AND_In08u_Out16u_KN	Define for Kernel name
\param 3					Number of ports
\param "Port AND_KN_INA"	Define for name of first input image (unsigned 8bit)
\param "Port AND_KN_INB"	Define for name of second input image (unsigned 8bit)
\param "Port AND_KN_OUT"	Define for name of comparison result of the two images (unsigned 16bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(AND_In08u_Out16u_K)
(
AND_In08u_Out16u_KN,
3,
__port(__index(0),
__identifier(AND_KN_INA),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),
__port(__index(1),
__identifier(AND_KN_INB),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),
__port(__index(2),
__identifier(AND_KN_OUT),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d16u),
__e0_size(1, 1),
__ek_size(1, 1))

);


/****************************************************************************/
/*!
AND operator kernel metadata. Pixelwise "AND" operator between two unsigned 16bit images.
Outputs unsigned 16bit and operator result. Is true if  (INPUTA != 0) && (INPUTB != 0)

\param AND_In08u_In16u_Out16u_KN	Define for Kernel name
\param 3							Number of ports
\param "Port AND_KN_INA"			Define for name of first input image (unsigned 8bit)
\param "Port AND_KN_INB"			Define for name of second input image (unsigned 8bit)
\param "Port AND_KN_OUT"			Define for name of comparison result of the two images (unsigned 16bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(AND_In08u_In16u_Out16u_K)
(
AND_In08u_In16u_Out16u_KN,
3,
__port(__index(0),
__identifier(AND_KN_INA),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),
__port(__index(1),
__identifier(AND_KN_INB),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d16u),
__e0_size(1, 1),
__ek_size(1, 1)),
__port(__index(2),
__identifier(AND_KN_OUT),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d16u),
__e0_size(1, 1),
__ek_size(1, 1))

);


/****************************************************************************/
/*!
Lower kernel metadata. Compares pixelwise the values of two signed 64bit images.
Outputs unsigned 8bit comparison result. Is true if INPUTA <= INPUTB

\param  LOWER_In64s_KN			Define for Kernel name
\param 3						Number of ports
\param "Port LOWER_KN_INA_HIGH"	Define for name of signed 32bit high word of first signed 64bit input image
\param "Port LOWER_KN_INA_LOW"	Define for name of unsigned 32bit low word of first signed 64bit input image
\param "Port LOWER_KN_INB_HIGH"	Define for name of signed 32bit high word of second signed 64bit input image
\param "Port LOWER_KN_INB_LOW"	Define for name of unsigned 32bit low word of second signed 64bit input image
\param "Port LOWER_KN_OUT"		Define for name of unsigned 8bit comparison result of the two images.
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(LOWER_In64s_K)
(
LOWER_In64s_KN,
5,
	__port(__index(0),
		__identifier(LOWER_KN_INA_HIGH),
		__attributes(ACF_ATTR_VEC_IN),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d32s),
		__e0_size(1, 1),
		__ek_size(1, 1)),

	__port(__index(1),
		__identifier(LOWER_KN_INA_LOW),
		__attributes(ACF_ATTR_VEC_IN),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d32u),
		__e0_size(1, 1),
		__ek_size(1, 1)),

	__port(__index(2),
		__identifier(LOWER_KN_INB_HIGH),
		__attributes(ACF_ATTR_VEC_IN),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d32s),
		__e0_size(1, 1),
		__ek_size(1, 1)),

	__port(__index(3),
		__identifier(LOWER_KN_INB_LOW),
		__attributes(ACF_ATTR_VEC_IN),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d32u),
		__e0_size(1, 1),
		__ek_size(1, 1)),
		
	__port(__index(4),
		__identifier(LOWER_KN_OUT),
		__attributes(ACF_ATTR_VEC_OUT),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d08u),
		__e0_size(1, 1),
		__ek_size(1, 1))
);



/****************************************************************************/
/*!
Lower kernel metadata. Compares pixelwise the values of two unsigned 64bit images.
Outputs unsigned 8bit comparison result. Is true if INPUTA <= INPUTB

\param  LOWER_In64u_KN			Define for Kernel name
\param 3						Number of ports
\param "Port LOWER_KN_INA_HIGH"	Define for name of unsigned 32bit high word of first unsigned signed 64bit input image
\param "Port LOWER_KN_INA_LOW"	Define for name of unsigned 32bit low word of first unsigned signed 64bit input image
\param "Port LOWER_KN_INB_HIGH"	Define for name of unsigned 32bit high word of second unsigned 64bit input image
\param "Port LOWER_KN_INB_LOW"	Define for name of unsigned 32bit low word of second unsigned 64bit input image
\param "Port LOWER_KN_OUT"		Define for name of unsigned 8bit comparison result of the two images.
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(LOWER_In64u_K)
(
LOWER_In64u_KN,
5,
__port(__index(0),
	__identifier(LOWER_KN_INA_HIGH),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32u),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(1),
	__identifier(LOWER_KN_INA_LOW),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32u),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(2),
	__identifier(LOWER_KN_INB_HIGH),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32u),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(3),
	__identifier(LOWER_KN_INB_LOW),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32u),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(4),
	__identifier(LOWER_KN_OUT),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(1, 1))
);
#endif //#ifdef ACF_KERNEL_METADATA



#ifdef ACF_KERNEL_IMPLEMENTATION

#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#include "comparison_apu.h"

void apu_lower(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out0)
{
	vec08u*        lIn0 = (vec08u*)in0.pMem;
	vec08u*        lIn1 = (vec08u*)in1.pMem;
	vbool*        lOut0 = (vbool*)out0.pMem;

	lower(lOut0, lIn0, lIn1, in0.chunkWidth, in0.chunkHeight, in0.chunkSpan, out0.chunkSpan);
}

void apu_lower_scalar(kernel_io_desc in0, kernel_io_desc in_scalar, kernel_io_desc out0)
{
	vec08u*        lIn0 = (vec08u*)in0.pMem;
	unsigned char  lIn_Scalar = *(unsigned char*)in_scalar.pMem;
	vec08u*        lOut0 = (vec08u*)out0.pMem;

	lower_scalar(lOut0, lIn0, lIn_Scalar, in0.chunkWidth, in0.chunkHeight, in0.chunkSpan, out0.chunkSpan);
}

void apu_lower_in16s(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out0)
{
	vec16s*        lIn0 = (vec16s*)in0.pMem;
	vec16s*        lIn1 = (vec16s*)in1.pMem;
	vbool*        lOut0 = (vbool*)out0.pMem;
	int32s inStrideW = in0.chunkSpan / sizeof(int16s);
	int32s outStrideW = out0.chunkSpan / sizeof(int08u);

	lower_in16s(lOut0, lIn0, lIn1, in0.chunkWidth, in0.chunkHeight, inStrideW, outStrideW);
}

void apu_lower_in32s(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out0)
{
	vec32s*        lIn0 = (vec32s*)in0.pMem;
	vec32s*        lIn1 = (vec32s*)in1.pMem;
	vbool*        lOut0 = (vbool*)out0.pMem;
	int32s inStrideW = in0.chunkSpan / sizeof(int32s);
	int32s outStrideW = out0.chunkSpan / sizeof(int08u);

	lower_in32s(lOut0, lIn0, lIn1, in0.chunkWidth, in0.chunkHeight, inStrideW, outStrideW);
}



void apu_abs_lower_in32s(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out0)
{
	vec32s*        lIn0 = (vec32s*)in0.pMem;
	vec32s*        lIn1 = (vec32s*)in1.pMem;
	vbool*        lOut0 = (vbool*)out0.pMem;
	int32s inStrideW = in0.chunkSpan / sizeof(int32s);
	int32s outStrideW = out0.chunkSpan / sizeof(int08u);

	absLower_in32s(lOut0, lIn0, lIn1, in0.chunkWidth, in0.chunkHeight, inStrideW, outStrideW);
}



void apu_abs_lower_in32s_scalar16u(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out0)
{
	vec32s*        lIn0 = (vec32s*)in0.pMem;
	int16u        lIn1 = *(int16u*)in1.pMem;
	vbool*        lOut0 = (vbool*)out0.pMem;
	int32s inStrideW = in0.chunkSpan / sizeof(int32s);
	int32s outStrideW = out0.chunkSpan / sizeof(int08u);

	absLower_in32s_scalar16u(lOut0, lIn0, lIn1, in0.chunkWidth, in0.chunkHeight, inStrideW, outStrideW);
}



void apu_lower_in64s(kernel_io_desc in0_high, kernel_io_desc in0_low, kernel_io_desc in1_high, kernel_io_desc in1_low, kernel_io_desc out0)
{
	vec32s*        lIn0_high = (vec32s*)in0_high.pMem;
	vec32s*        lIn1_high = (vec32s*)in1_high.pMem;
	vbool*        lOut0 = (vbool*)out0.pMem;

	vec32u*        lIn0_low = (vec32u*)in0_low.pMem;
	vec32u*        lIn1_low = (vec32u*)in1_low.pMem;
	int32s inStrideW = in0_high.chunkSpan / sizeof(int32s);
	int32s outStrideW = out0.chunkSpan / sizeof(int08u);

	lower_in64s(lOut0, lIn0_high, lIn0_low, lIn1_high, lIn1_low, in0_high.chunkWidth, in0_high.chunkHeight, inStrideW, outStrideW);
}



void apu_lower_in64u(kernel_io_desc in0_high, kernel_io_desc in0_low, kernel_io_desc in1_high, kernel_io_desc in1_low, kernel_io_desc out0)
{
	vec32u* lIn0_high = (vec32u*)in0_high.pMem;
	vec32u* lIn1_high = (vec32u*)in1_high.pMem;
	vbool*  lOut0 = (vbool*)out0.pMem;

	vec32u* lIn0_low = (vec32u*)in0_low.pMem;
	vec32u* lIn1_low = (vec32u*)in1_low.pMem;
	int32s inStrideW = in0_high.chunkSpan / sizeof(int32u);
	int32s outStrideW = out0.chunkSpan / sizeof(int08u);

	lower_in64u(lOut0, lIn0_high, lIn0_low, lIn1_high, lIn1_low, in0_high.chunkWidth, in0_high.chunkHeight, inStrideW, outStrideW);
}



void apu_lowerEqual_in32s(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out)
{
	vec32s*        lIn0 = (vec32s*)in0.pMem;
	vec32s*        lIn1 = (vec32s*)in1.pMem;
	vbool*        lOut = (vbool*)out.pMem;

	int32s inStrideW = in0.chunkSpan / sizeof(int32s);
	int32s outStrideW = out.chunkSpan / sizeof(int08u);
	lowerEqual_in32s(lOut, lIn0, lIn1, in0.chunkWidth, in0.chunkHeight, inStrideW, outStrideW);
}


void apu_and_in08u_out16u(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out)
{
	vec08u* lIn0 = (vec08u*)in0.pMem;
	vec08u* lIn1 = (vec08u*)in1.pMem;
	vec16u* lOut = (vec16u*)out.pMem;

	int32s inStrideW = in0.chunkSpan / sizeof(int08u);
	int32s outStrideW = out.chunkSpan / sizeof(int16u);
	and_in08u_out16u(lOut, lIn0, lIn1, in0.chunkWidth, in0.chunkHeight, inStrideW, outStrideW);
}

void apu_and_in08u_in16u_out16u(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out)
{
	vec08u* lIn0 = (vec08u*)in0.pMem;
	vec16u* lIn1 = (vec16u*)in1.pMem;
	vec16u* lOut = (vec16u*)out.pMem;

	int32s inStrideW = in0.chunkSpan / sizeof(int08u);
	int32s outStrideW = out.chunkSpan / sizeof(int16u);
	and_in08u_in16u_out16u(lOut, lIn0, lIn1, in0.chunkWidth, in0.chunkHeight, inStrideW, outStrideW);
}



void apu_and(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out)
{
	vec08u* lIn0 = (vec08u*)in0.pMem;
	vec08u* lIn1 = (vec08u*)in1.pMem;
	vec08u* lOut = (vec08u*)out.pMem;

	int32s inStrideW = in0.chunkSpan;
	int32s outStrideW = out.chunkSpan;
	and_kn(lOut, lIn0, lIn1, in0.chunkWidth, in0.chunkHeight, inStrideW, outStrideW);
}



void apu_mask8b(kernel_io_desc in0, kernel_io_desc inMask, kernel_io_desc out)
{
	vec08u* lIn0 = (vec08u*)in0.pMem;
	vec08u* lInMask = (vec08u*)inMask.pMem;
	vec08u* lOut = (vec08u*)out.pMem;

	int32s inStrideW = in0.chunkSpan;
	int32s outStrideW = out.chunkSpan;
	mask_kn(lOut, lIn0, lInMask , in0.chunkWidth, in0.chunkHeight, inStrideW, outStrideW);
}


void apu_and_in16u_out16u(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out)
{
	vec16u* lIn0 = (vec16u*)in0.pMem;
	vec16u* lIn1 = (vec16u*)in1.pMem;
	vec16u* lOut = (vec16u*)out.pMem;

	int32s inStrideW = in0.chunkSpan / sizeof(int16u);
	int32s outStrideW = out.chunkSpan / sizeof(int16u);
	and_in16u_out16u(lOut, lIn0, lIn1, in0.chunkWidth, in0.chunkHeight, inStrideW, outStrideW);
}

void apu_and_3Pt_in16u_out16u(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc in2, kernel_io_desc out)
{
	vec16u* lIn0 = (vec16u*)in0.pMem;
	vec16u* lIn1 = (vec16u*)in1.pMem;
	vec16u*	lIn2 = (vec16u*)in2.pMem;
	vec16u* lOut = (vec16u*)out.pMem;

	int32s inStrideW = in0.chunkSpan / sizeof(int16u);
	int32s outStrideW = out.chunkSpan / sizeof(int16u);
	and_3Pt_in16u_out16u(lOut, lIn0, lIn1, lIn2,  in0.chunkWidth, in0.chunkHeight, inStrideW, outStrideW);
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
