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
* \file correlation_acf.cpp
* \addtogroup filtering
* \addtogroup correlation
* \ingroup filtering
* @{
* \brief Correlation, respectively convolution with a reversed filter
*/
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif
#include "correlation_acf.h"

#ifdef ACF_KERNEL_METADATA


/****************************************************************************/
/*!
Gradient in X direction kernel metadata. Convolution of input unsigned 8bit image with a [-1 0 1] row-filter.
Outputs correlation result (signed 16bit) (i.e. the gradients in X direction of the input image)

\param  CORR_GRAD_X_KN			 Define for Kernel name
\param 2						 Number of ports
\param "Port CORR_Kernel_Input"	 Define for name of input image (unsigned 8bit)
\param "Port CORR_Kernel_Output" Define for name of correlation result (signed 16bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(CORR_GRAD_X_K)(
CORR_GRAD_X_KN,
2,
__port(__index(0),
__identifier(CORR_Kernel_Input),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(1, 1, 0, 0), //TBD: problem not yet solved. This allows filters of maximal width/height 11 
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(1),
__identifier(CORR_Kernel_Output),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1))

);



/****************************************************************************/
/*!
Gradient in Y direction kernel metadata. Convolution of input unsigned 8bit image with a [-1 0 1]^T column-filter.
Outputs correlation result (signed 16bit) (i.e. the gradients in Y direction of the input image)

\param  CORR_GRAD_Y_KN			  Define for Kernel name
\param 2						  Number of ports
\param "Port CORR_Kernel_Input"	  Define for name of input image (unsigned 8bit)
\param "Port CORR_Kernel_Output"  Define for name of correlation result (signed 16bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(CORR_GRAD_Y_K)
(
CORR_GRAD_Y_KN,
2,
__port(__index(0),
__identifier(CORR_Kernel_Input),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 1, 1), //TBD: problem not yet solved. This allows filters of maximal width/height 11 
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(1),
__identifier(CORR_Kernel_Output),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1))
);



/****************************************************************************/
/*!
Gradient in X direction kernel metadata for Scharr_X filter. Convolution of input unsigned 8bit image with a scharr_x filter.
Outputs correlation result (signed 16bit) (i.e. the gradients in X direction of the input image)

\param  CORR_SCHARR_X_KN		 Define for Kernel name
\param 2						 Number of ports
\param "Port CORR_Kernel_Input"	 Define for name of input image (unsigned 8bit)
\param "Port CORR_Kernel_Output" Define for name of correlation result (signed 16bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(CORR_SCHARR_X_K)
(
CORR_SCHARR_X_KN,
2,
__port(__index(0),
__identifier(CORR_Kernel_Input),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(1, 1, 1, 1),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(1),
__identifier(CORR_Kernel_Output),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1))
);



/****************************************************************************/
/*!
Gradient in Y direction kernel metadata for Scharr_Y filter. Convolution of input unsigned 8bit image with a scharr_y filter.
Outputs correlation result (signed 16bit) (i.e. the gradients in Y direction of the input image)

\param  CORR_SCHARR_Y_KN		 Define for Kernel name
\param 2						 Number of ports
\param "Port CORR_Kernel_Input"	 Define for name of input image (unsigned 8bit)
\param "Port CORR_Kernel_Output" Define for name of correlation result (signed 16bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(CORR_SCHARR_Y_K)
(
CORR_SCHARR_Y_KN,
2,
__port(__index(0),
__identifier(CORR_Kernel_Input),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(1, 1, 1, 1),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(1),
__identifier(CORR_Kernel_Output),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1))
);

/****************************************************************************/
/*!
General correlation kernel metadata. General correlation of input unsigned 8bit image with a  1D or 2D filter.
Outputs correlation result (signed 16bit).
\warning The filter width and height (max 11x11) have to be defined in the macro definitions FW and FH for the currently used kernel!
\param  CORR_KN							Define for Kernel name
\param 5								Number of ports
\param "Port CORR_Kernel_Input"			Define for name of input image (unsigned 8bit). The spatial dependencies of this port - HFW and HFH - are half the filter width/height. They are defined in the header of this file as HFW FW>>1 and HFH FH >> 1
\param "Port CORR_Kernel_Output"		Define for name of correlation result (signed 16bit)
\param "Port CORR_Kernel_Filter"		Define for name of filter coefficients the input has to be convolved with (signed 16bit)
\param "Port CORR_Kernel_FilterScale"	Define for name of the scalar value of the normalization factor used for the filter (signed 16bit)
\param "Port CORR_Kernel_FiltSymmFl"	Define for name of the scalar value of the symmetry flag defined for this filter as it is specified in "symmetry_flags.h" file  (unsigned 16bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(CORR_K)
(
CORR_KN,
5, 
__port(__index(0),
__identifier(CORR_Kernel_Input),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(HFW, HFW, HFH, HFH), 
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(1),
__identifier(CORR_Kernel_Output),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0,0,0,0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(2),
__identifier(CORR_Kernel_Filter),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0,0,0,0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(FW, FH)),

__port(__index(3),
__identifier(CORR_Kernel_FilterScale),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(4),
__identifier(CORR_Kernel_FiltSymmFl),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16u),
	__e0_size(1, 1),
	__ek_size(1, 1))
);



/****************************************************************************/
/*!
General correlation kernel metadata. General correlation of input unsigned 8bit image with a filter of size 1x3  (convenience kernel for CORR_K).
Outputs correlation result (signed 16bit).
\warning The filter width and height have to be defined in the macro definitions FW and FH for the currently used kernel!
\param  CORR_KN_1x3_KN					Define for Kernel name
\param 5								Number of ports
\param "Port CORR_Kernel_Input"			Define for name of input image (unsigned 8bit). The spatial dependencies of this port - HFW and HFH - are half the filter width/height. They are defined in the header of this file as HFW FW>>1 and HFH FH >> 1
\param "Port CORR_Kernel_Output"		Define for name of correlation result (signed 16bit)
\param "Port CORR_Kernel_Filter"		Define for name of filter coefficients the input has to be convolved with (signed 16bit)
\param "Port CORR_Kernel_FilterScale"	Define for name of the scalar value of the normalization factor used for the filter (signed 16bit)
\param "Port CORR_Kernel_FiltSymmFl"	Define for name of the scalar value of the symmetry flag defined for this filter as it is specified in "symmetry_flags.h" file (unsigned 16bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(CORR_KN_1x3_K)
(
CORR_KN_1x3_KN,
5,
__port(__index(0),
__identifier(CORR_Kernel_Input),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 1, 1), 
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(1),
__identifier(CORR_Kernel_Output),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(2),
__identifier(CORR_Kernel_Filter),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 3)),
__port(__index(3),
__identifier(CORR_Kernel_FilterScale),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(4),
__identifier(CORR_Kernel_FiltSymmFl),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16u),
	__e0_size(1, 1),
	__ek_size(1, 1))
);



/****************************************************************************/
/*!
General correlation kernel metadata. General correlation of input unsigned 8bit image with a filter of size 3x1 (convenience kernel for CORR_K).
Outputs correlation result (signed 16bit).
\warning The filter width and height have to be defined in the macro definitions FW and FH for the currently used kernel!
\param  CORR_KN_3x1_KN					Define for Kernel name
\param 5								Number of ports
\param "Port CORR_Kernel_Input"			Define for name of input image (unsigned 8bit). The spatial dependencies of this port - HFW and HFH - are half the filter width/height. They are defined in the header of this file as HFW FW>>1 and HFH FH >> 1
\param "Port CORR_Kernel_Output"		Define for name of correlation result (signed 16bit)
\param "Port CORR_Kernel_Filter"		Define for name of filter coefficients the input has to be convolved with (signed 16bit)
\param "Port CORR_Kernel_FilterScale"	Define for name of the scalar value of the normalization factor used for the filter (signed 16bit)
\param "Port CORR_Kernel_FiltSymmFl"	Define for name of the scalar value of the symmetry flag defined for this filter as it is specified in "symmetry_flags.h" file (unsigned 16bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(CORR_KN_3x1_K)
(
CORR_KN_3x1_KN,
5,
__port(__index(0),
__identifier(CORR_Kernel_Input),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(1, 1, 0, 0), 
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(1),
__identifier(CORR_Kernel_Output),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(2),
__identifier(CORR_Kernel_Filter),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(3, 1)),
__port(__index(3),
__identifier(CORR_Kernel_FilterScale),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(4),
__identifier(CORR_Kernel_FiltSymmFl),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16u),
	__e0_size(1, 1),
	__ek_size(1, 1))
);




/****************************************************************************/
/*!
General correlation kernel metadata. General correlation of input unsigned 8bit image with a filter of size 3x3 (convenience kernel for CORR_K).
Outputs correlation result (signed 16bit).
\warning The filter width and height have to be defined in the macro definitions FW and FH for the currently used kernel!
\param  CORR_KN_3x3_KN					Define for Kernel name
\param 5								Number of ports
\param "Port CORR_Kernel_Input"			Define for name of input image (unsigned 8bit). The spatial dependencies of this port - HFW and HFH - are half the filter width/height. They are defined in the header of this file as HFW FW>>1 and HFH FH >> 1
\param "Port CORR_Kernel_Output"		Define for name of correlation result (signed 16bit)
\param "Port CORR_Kernel_Filter"		Define for name of filter coefficients the input has to be convolved with (signed 16bit)
\param "Port CORR_Kernel_FilterScale"	Define for name of the scalar value of the normalization factor used for the filter (signed 16bit)
\param "Port CORR_Kernel_FiltSymmFl"	Define for name of the scalar value of the symmetry flag defined for this filter as it is specified in "symmetry_flags.h" file (unsigned 16bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(CORR_KN_3x3_K)
(
CORR_KN_3x3_KN,
5,
__port(__index(0),
__identifier(CORR_Kernel_Input),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(1, 1, 1, 1), 
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(1),
__identifier(CORR_Kernel_Output),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(2),
__identifier(CORR_Kernel_Filter),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(3, 3)),
__port(__index(3),
__identifier(CORR_Kernel_FilterScale),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(4),
__identifier(CORR_Kernel_FiltSymmFl),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16u),
	__e0_size(1, 1),
	__ek_size(1, 1))
);


/****************************************************************************/
/*!
General correlation kernel metadata. General correlation of input unsigned 8bit image with a filter of size 5x5 (convenience kernel for CORR_K).
Outputs correlation result (signed 16bit).
\warning The filter width and height have to be defined in the macro definitions FW and FH for the currently used kernel!
\param  CORR_KN_5x5_KN					Define for Kernel name
\param 5								Number of ports
\param "Port CORR_Kernel_Input"			Define for name of input image (unsigned 8bit). The spatial dependencies of this port - HFW and HFH - are half the filter width/height. They are defined in the header of this file as HFW FW>>1 and HFH FH >> 1
\param "Port CORR_Kernel_Output"		Define for name of correlation result (signed 16bit)
\param "Port CORR_Kernel_Filter"		Define for name of filter coefficients the input has to be convolved with (signed 16bit)
\param "Port CORR_Kernel_FilterScale"	Define for name of the scalar value of the normalization factor used for the filter (signed 16bit)
\param "Port CORR_Kernel_FiltSymmFl"	Define for name of the scalar value of the symmetry flag defined for this filter as it is specified in "symmetry_flags.h" file (unsigned 16bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(CORR_KN_5x5_K)(
CORR_KN_5x5_KN,
5,
__port(__index(0),
__identifier(CORR_Kernel_Input),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(2, 2, 2, 2), 
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(1, 1)),
	__port(__index(1),
__identifier(CORR_Kernel_Output),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
	__port(__index(2),
__identifier(CORR_Kernel_Filter),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(5, 5)),
	__port(__index(3),
__identifier(CORR_Kernel_FilterScale),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
	__port(__index(4),
__identifier(CORR_Kernel_FiltSymmFl),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16u),
	__e0_size(1, 1),
	__ek_size(1, 1))
);


/****************************************************************************/
/*!
General correlation kernel metadata. General correlation of input unsigned 8bit image with a filter of size 7x7 (convenience kernel for CORR_K).
Outputs correlation result (signed 16bit).
\warning The filter width and height have to be defined in the macro definitions FW and FH for the currently used kernel!
\param  CORR_KN_7x7_KN					Define for Kernel name
\param 5								Number of ports
\param "Port CORR_Kernel_Input"			Define for name of input image (unsigned 8bit). The spatial dependencies of this port - HFW and HFH - are half the filter width/height. They are defined in the header of this file as HFW FW>>1 and HFH FH >> 1
\param "Port CORR_Kernel_Output"		Define for name of correlation result (signed 16bit)
\param "Port CORR_Kernel_Filter"		Define for name of filter coefficients the input has to be convolved with (signed 16bit)
\param "Port CORR_Kernel_FilterScale"	Define for name of the scalar value of the normalization factor used for the filter (signed 16bit)
\param "Port CORR_Kernel_FiltSymmFl"	Define for name of the scalar value of the symmetry flag defined for this filter as it is specified in "symmetry_flags.h" file (unsigned 16bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(CORR_KN_7x7_K)(
CORR_KN_7x7_KN,
5,
__port(__index(0),
__identifier(CORR_Kernel_Input),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(3, 3, 3, 3),
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(1),
__identifier(CORR_Kernel_Output),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(2),
__identifier(CORR_Kernel_Filter),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(7, 7)),
__port(__index(3),
__identifier(CORR_Kernel_FilterScale),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(4),
__identifier(CORR_Kernel_FiltSymmFl),
	__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16u),
	__e0_size(1, 1),
	__ek_size(1, 1))
);


#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "symmetry_flags.h"
#include "correlation_apu.h"


void apu_correlation(kernel_io_desc lImgIn, kernel_io_desc lImgOut, kernel_io_desc lFilterIn,/* kernel_io_desc lFilterWidth, kernel_io_desc lFilterHeight, */kernel_io_desc lFilterScale, kernel_io_desc lFilterSymFlags)
{
   vec08u*  lpvImgIn		= (vec08u*)  lImgIn.pMem;
   int16s dataSzIn = sizeof(int08u);

   vec16s*  lpvImgOut		= (vec16s*)  lImgOut.pMem;
   int16s dataSzOut = sizeof(int16s);

   const int16s* lpvFilterCoefs = (const int16s*)  lFilterIn.pMem;
   int16s   filtWidth = FW;//((int16s*) lFilterWidth.pMem)[0];
   int16u   filtHeight = FH;//((int16u*)lFilterHeight.pMem)[0];
   int16s   filtScale = ((int16s*) lFilterScale.pMem)[0];
   int16u   filtSymFlags = ((int16u*) lFilterSymFlags.pMem)[0];
   int16s xSkip = 1, ySkip = 1; // do not perform downsampling
   
   performCorrelation(filtSymFlags, lpvImgOut, lpvImgIn, int16s(lImgIn.chunkSpan / dataSzIn), int16s(lImgIn.chunkWidth), int16s(lImgIn.chunkHeight), int16s(lImgOut.chunkSpan / dataSzOut), xSkip, ySkip, filtWidth, filtHeight, filtScale, lpvFilterCoefs);

}

void apu_gradient_x(kernel_io_desc lImgIn, kernel_io_desc lImgOut)
{

	vec08u*  lpvImgIn = (vec08u*)lImgIn.pMem;
	int16s dataSzIn = sizeof(int08u);

	vec16s*  lpvImgOut = (vec16s*)lImgOut.pMem;
	int16s dataSzOut = sizeof(int16s);

	const int16s lpvFilterCoefs[3] = { -1, 0, 1 };
	int16s   filtScale = 1;
	int16u   filtSymFlags = (int16u)ANTI_SYM_X;
	int16s xSkip = 1, ySkip = 1; // do not perform downsampling

	int16u   filtWidth = 3;
	int16u   filtHeight = 1;

	performCorrelation(filtSymFlags, lpvImgOut, lpvImgIn, int16s(lImgIn.chunkSpan / dataSzIn), int16s(lImgIn.chunkWidth), int16s(lImgIn.chunkHeight), int16s(lImgOut.chunkSpan / dataSzOut), xSkip, ySkip, filtWidth, filtHeight, filtScale, (const int16s*)lpvFilterCoefs);
}


void apu_gradient_y(kernel_io_desc lImgIn, kernel_io_desc lImgOut)
{
	vec08u*  lpvImgIn = (vec08u*)lImgIn.pMem;
	int16s dataSzIn = sizeof(int08u);

	vec16s*  lpvImgOut = (vec16s*)lImgOut.pMem;
	int16s dataSzOut = sizeof(int16s);

	const int16s lpvFilterCoefs[3] = { -1, 0, 1 };
	int16s   filtScale = 1;
	int16u   filtSymFlags = (int16u) ANTI_SYM_Y;
	int16s xSkip = 1, ySkip = 1; // do not perform downsampling

	int16u   filtWidth = 1;
	int16u   filtHeight = 3;

	performCorrelation(filtSymFlags, lpvImgOut, lpvImgIn, int16s(lImgIn.chunkSpan / dataSzIn), int16s(lImgIn.chunkWidth), int16s(lImgIn.chunkHeight), int16s(lImgOut.chunkSpan / dataSzOut), xSkip, ySkip, filtWidth, filtHeight, filtScale, (const int16s*)lpvFilterCoefs);
}


void apu_scharr_x(kernel_io_desc lImgIn, kernel_io_desc lImgOut)
{
	vec16s*  lpvImgIn = (vec16s*)lImgIn.pMem;
	vec16s*  lpvImgOut = (vec16s*)lImgOut.pMem;
	int32s dataSz = sizeof(int16s);

	const int16s lpvFilterCoefs[9] = { -3, 0, 3,
		-10, 0, 10,
		-3, 0, 3 };
	int16s   filtScale = 32 / scharrFiltUpscale;
	int16u   filtSymFlags = (int16u)ANTI_SYM_X_SYM_Y;
	int16s xSkip = 1, ySkip = 1; // do not perform downsampling


	int16u   filtWidth = 3;
	int16u   filtHeight = 3;

	/*
#ifdef _DEBUG
	static int it = 0;
	int cuIdx = 8;
	if (it == 0) {
		fprintf(stderr, "scharr_x %d\n", it);


		fprintf(stderr, "\nOrigImg \n");

		vec16s* orig = lpvImgIn;
		for (int i = 0; i < lImgIn.chunkHeight; ++i, orig += lImgIn.chunkSpan / dataSz) {
			for (int j = 0; j < lImgIn.chunkWidth; ++j) {
				int32s crt = vget(orig[j], cuIdx);
				fprintf(stderr, "%d\t", crt / 16);
			}
			fprintf(stderr, "\n");
		}
	}
#endif
	*/
	
	//performCorrelation(filtSymFlags, lpvImgOut, lpvImgIn, lImgIn.chunkSpan/dataSz, lImgIn.chunkWidth, lImgIn.chunkHeight, lImgOut.chunkSpan/dataSz, xSkip, ySkip, filtWidth, filtHeight, filtScale, (const int16s*)lpvFilterCoefs);
	correlation__antisymXsymYfilter_16s(lpvImgOut, lpvImgIn, int16s(lImgIn.chunkSpan / dataSz), int16s(lImgIn.chunkWidth), int16s(lImgIn.chunkHeight), int16s(lImgOut.chunkSpan / dataSz), xSkip, ySkip, filtWidth, filtHeight, filtScale, (const int16s*)lpvFilterCoefs);
	
	/*
#ifdef _DEBUG
	if (it++ == 0) {

		fprintf(stderr, "\nGrad X\n");

		vec16s* gX = lpvImgOut;
		for (int i = 0; i < lImgOut.chunkHeight; ++i, gX += lImgOut.chunkSpan / (dataSz << 1)) {
			for (int j = 0; j < lImgOut.chunkWidth; ++j) {
				int32s crt = vget(gX[j], cuIdx);
				fprintf(stderr, "%d\t", crt/16);
			}
			fprintf(stderr, "\n");
		}
	}
#endif
*/
}



void apu_scharr_y(kernel_io_desc lImgIn, kernel_io_desc lImgOut)
{
	vec16s*  lpvImgIn = (vec16s*)lImgIn.pMem;
	vec16s*  lpvImgOut = (vec16s*)lImgOut.pMem;
	int16s dataSz = sizeof(int16s);

	const int16s lpvFilterCoefs[9] = {
		 -3, -10, -3 ,
		 0, 0, 0 ,
		 3, 10, 3 
	};

	int16s   filtScale = 32/scharrFiltUpscale; // actually it is 32, but trying to preserve more detail  
	int16u   filtSymFlags = (int16u)SYM_X_ANTI_SYM_Y;
	int16s xSkip = 1, ySkip = 1; // do not perform downsampling

	int16u   filtWidth = 3;
	int16u   filtHeight = 3;
	
	//performCorrelation_16s(filtSymFlags, lpvImgOut, lpvImgIn, lImgIn.chunkSpan, lImgIn.chunkWidth, lImgIn.chunkHeight, lImgOut.chunkSpan >> 1, xSkip, ySkip, filtWidth, filtHeight, filtScale, (const int16s*)lpvFilterCoefs);
	correlation__symXantisymYfilter_16s(lpvImgOut, lpvImgIn, int16s(lImgIn.chunkSpan / dataSz), int16s(lImgIn.chunkWidth), int16s(lImgIn.chunkHeight), int16s(lImgOut.chunkSpan / dataSz), xSkip, ySkip, filtWidth, filtHeight, filtScale, (const int16s*)lpvFilterCoefs);
	
	/*
#ifdef _DEBUG
	static int it = 0;
	if (it++ == 0) {
		fprintf(stderr, "scharr_y %d\n", it);
		int cuIdx = 8;
		fprintf(stderr, "\nGrad Y\n");

		vec16s* gY = lpvImgOut;
		for (int i = 0; i < lImgOut.chunkHeight; ++i, gY += lImgOut.chunkSpan / (dataSz << 1)) {
			for (int j = 0; j < lImgOut.chunkWidth; ++j) {
				int32s crt = vget(gY[j], cuIdx);
				fprintf(stderr, "%d\t", crt/16);
			}
			fprintf(stderr, "\n");
		}
	}
#endif
*/
}


void apu_correlation_1x3(kernel_io_desc lImgIn, kernel_io_desc lImgOut, kernel_io_desc lFilterIn, kernel_io_desc lFilterScale, kernel_io_desc lFilterSymFlags)
{
	vec08u*  lpvImgIn = (vec08u*)lImgIn.pMem;
	int16s dataSzIn = sizeof(int08u);

	vec16s*  lpvImgOut = (vec16s*)lImgOut.pMem;
	int16s dataSzOut = sizeof(int16s);

	const int16s* lpvFilterCoefs = (const int16s*)lFilterIn.pMem;
	int16s   filtScale = ((int16s*)lFilterScale.pMem)[0];
	int16u   filtSymFlags = ((int16u*)lFilterSymFlags.pMem)[0];
	int16s xSkip = 1, ySkip = 1; // do not perform downsampling

	int16u   filtWidth = 1;
	int16u   filtHeight = 3;

	
	performCorrelation(filtSymFlags, lpvImgOut, lpvImgIn, int16s(lImgIn.chunkSpan / dataSzIn), int16s(lImgIn.chunkWidth), int16s(lImgIn.chunkHeight), int16s(lImgOut.chunkSpan / dataSzOut), xSkip, ySkip, filtWidth, filtHeight, filtScale, lpvFilterCoefs);
}


void apu_correlation_3x1(kernel_io_desc lImgIn, kernel_io_desc lImgOut, kernel_io_desc lFilterIn, kernel_io_desc lFilterScale, kernel_io_desc lFilterSymFlags)
{
	vec08u*  lpvImgIn = (vec08u*)lImgIn.pMem;
	int16s dataSzIn = sizeof(int08u);

	vec16s*  lpvImgOut = (vec16s*)lImgOut.pMem;
	int16s dataSzOut = sizeof(int16s);

	const int16s* lpvFilterCoefs = (const int16s*)lFilterIn.pMem;
	int16s   filtScale = ((int16s*)lFilterScale.pMem)[0];
	int16u   filtSymFlags = ((int16u*)lFilterSymFlags.pMem)[0];
	int16s xSkip = 1, ySkip = 1; // do not perform downsampling

	int16u   filtWidth = 3;
	int16u   filtHeight = 1;

	performCorrelation(filtSymFlags, lpvImgOut, lpvImgIn, int16s(lImgIn.chunkSpan / dataSzIn), int16s(lImgIn.chunkWidth), int16s(lImgIn.chunkHeight), int16s(lImgOut.chunkSpan / dataSzOut), xSkip, ySkip, filtWidth, filtHeight, filtScale, lpvFilterCoefs);
}

void apu_correlation_3x3(kernel_io_desc lImgIn, kernel_io_desc lImgOut, kernel_io_desc lFilterIn, kernel_io_desc lFilterScale, kernel_io_desc lFilterSymFlags)
{
	vec08u*  lpvImgIn = (vec08u*)lImgIn.pMem;
	int16s dataSzIn = sizeof(int08u);

	vec16s*  lpvImgOut = (vec16s*)lImgOut.pMem;
	int16s dataSzOut = sizeof(int16s);

	const int16s* lpvFilterCoefs = (const int16s*)lFilterIn.pMem;
	int16s   filtScale = ((int16s*)lFilterScale.pMem)[0];
	int16u   filtSymFlags = ((int16u*)lFilterSymFlags.pMem)[0];
	int16s xSkip = 1, ySkip = 1; // do not perform downsampling

	int16u   filtWidth = 3;
	int16u   filtHeight = 3;

	performCorrelation(filtSymFlags, lpvImgOut, lpvImgIn, int16s(lImgIn.chunkSpan / dataSzIn), int16s(lImgIn.chunkWidth), int16s(lImgIn.chunkHeight), int16s(lImgOut.chunkSpan / dataSzOut), xSkip, ySkip, filtWidth, filtHeight, filtScale, lpvFilterCoefs);
}

void apu_correlation_5x5(kernel_io_desc lImgIn, kernel_io_desc lImgOut, kernel_io_desc lFilterIn, kernel_io_desc lFilterScale, kernel_io_desc lFilterSymFlags)
{
	vec08u*  lpvImgIn = (vec08u*)lImgIn.pMem;
	int16s dataSzIn = sizeof(int08u);

	vec16s*  lpvImgOut = (vec16s*)lImgOut.pMem;
	int16s dataSzOut = sizeof(int16s);

	const int16s* lpvFilterCoefs = (const int16s*)lFilterIn.pMem;
	int16s   filtScale = ((int16s*)lFilterScale.pMem)[0];
	int16u   filtSymFlags = ((int16u*)lFilterSymFlags.pMem)[0];
	int16s xSkip = 1, ySkip = 1; // do not perform downsampling

	int16u   filtWidth = 5;
	int16u   filtHeight = 5;

	performCorrelation(filtSymFlags, lpvImgOut, lpvImgIn, int16s(lImgIn.chunkSpan / dataSzIn), int16s(lImgIn.chunkWidth), int16s(lImgIn.chunkHeight), int16s(lImgOut.chunkSpan / dataSzOut), xSkip, ySkip, filtWidth, filtHeight, filtScale, lpvFilterCoefs);
}

void apu_correlation_7x7(kernel_io_desc lImgIn, kernel_io_desc lImgOut, kernel_io_desc lFilterIn,  kernel_io_desc lFilterScale, kernel_io_desc lFilterSymFlags)
{
	vec08u*  lpvImgIn = (vec08u*)lImgIn.pMem;
	int16s dataSzIn = sizeof(int08u);

	vec16s*  lpvImgOut = (vec16s*)lImgOut.pMem;
	int16s dataSzOut = sizeof(int16s);

	const int16s* lpvFilterCoefs = (const int16s*)lFilterIn.pMem;
	int16s   filtScale = ((int16s*)lFilterScale.pMem)[0];
	int16u   filtSymFlags = ((int16u*)lFilterSymFlags.pMem)[0];
	int16s xSkip = 1, ySkip = 1; // do not perform downsampling

	int16u   filtWidth = 7;
	int16u   filtHeight = 7;

	performCorrelation(filtSymFlags, lpvImgOut, lpvImgIn, int16s(lImgIn.chunkSpan), int16s(lImgIn.chunkWidth / dataSzIn), int16s(lImgIn.chunkHeight), int16s(lImgOut.chunkSpan / dataSzOut), xSkip, ySkip, filtWidth, filtHeight, filtScale, lpvFilterCoefs);
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
