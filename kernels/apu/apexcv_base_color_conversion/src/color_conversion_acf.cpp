/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2017 NXP
* All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
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
* \file color_conversion.cpp
* \addtogroup apexcv-color_conversion
* \addtogroup color_conversion
* \ingroup color_conversion
* @{
* \brief color_conversion operations mimicking opencv
*/

/*!*********************************************************************************
*  @file color_conversion.c
*  @brief ACF metadata and wrapper function for color conversion kernels
***********************************************************************************/

#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA
#include "color_conversion_acf.h"

/*!*********************************************************************************
*  \brief ACF metadata for rgb565 to rgb888x
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO  kernelInfoConcat(RGB565_TO_RGB888X_K)
(
   RGB565_TO_RGB888X_KN,
   2,
   __port(__index(0),
          __identifier(INPUT_0),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1)),    
   __port(__index(1),
          __identifier(OUTPUT_0),
          __attributes(ACF_ATTR_VEC_OUT), 
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for rgb888x to rgb565
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO  kernelInfoConcat(RGB888X_TO_RGB565_K)
(
   RGB888X_TO_RGB565_KN,
   2,
   __port(__index(0),
          __identifier(INPUT_0),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32u),
          __e0_size(1, 1),
          __ek_size(1, 1)),    
   __port(__index(1),
          __identifier(OUTPUT_0),
          __attributes(ACF_ATTR_VEC_OUT), 
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for rgb888x to y
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO  kernelInfoConcat(RGB888X_TO_Y_K)
(
   RGB888X_TO_Y_KN,
   5,
   __port(__index(0),
          __identifier(INPUT_0),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(4, 1),
          __ek_size(4, 1)), 
   __port(__index(1),
          __identifier(R2Y),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32s),
          __e0_size(1, 1),
          __ek_size(1, 1)),        
   __port(__index(2),
          __identifier(G2Y),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32s),
          __e0_size(1, 1),
          __ek_size(1, 1)), 
   __port(__index(3),
          __identifier(B2Y),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32s),
          __e0_size(1, 1),
          __ek_size(1, 1)),            
   __port(__index(4),
          __identifier(OUTPUT_0),
          __attributes(ACF_ATTR_VEC_OUT), 
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(4, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for rgb888x to y
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO  kernelInfoConcat(RGB888X_TO_YUV_K)
(
   RGB888X_TO_YUV_KN,
   2,
   __port(__index(0),
          __identifier(INPUT_0),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(4, 1),
          __ek_size(1, 1)),         
   __port(__index(1),
          __identifier(OUTPUT_0),
          __attributes(ACF_ATTR_VEC_OUT), 
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(4, 1),
          __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for channel extract
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernelInfoConcat(EXTRACT_CHANNEL_2CH_K)
(
   EXTRACT_CHANNEL_2CH_KN,
   3,
   __port(__index(0),
          __identifier(INPUT_0),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(2, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier(OUTPUT_0),
          __attributes(ACF_ATTR_VEC_OUT), 
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(2),
          __identifier(CH_ID),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED), 
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);


/*!*********************************************************************************
*  \brief ACF metadata for channel extract
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernelInfoConcat(EXTRACT_CHANNEL_3CH_K)
(
   EXTRACT_CHANNEL_3CH_KN,
   3,
   __port(__index(0),
          __identifier(INPUT_0),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(3, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier(OUTPUT_0),
          __attributes(ACF_ATTR_VEC_OUT), 
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(2),
          __identifier(CH_ID),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED), 
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);


/*!*********************************************************************************
*  \brief ACF metadata for channel extract
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernelInfoConcat(EXTRACT_CHANNEL_4CH_K)
(
   EXTRACT_CHANNEL_4CH_KN,
   3,
   __port(__index(0),
          __identifier(INPUT_0),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(4, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier(OUTPUT_0),
          __attributes(ACF_ATTR_VEC_OUT), 
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(2),
          __identifier(CH_ID),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED), 
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);


/*!*********************************************************************************
*  \brief ACF metadata for channel INSERT
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernelInfoConcat(INSERT_CHANNEL_2CH_K)
(
  INSERT_CHANNEL_2CH_KN,
  4,
  __port(__index(0),
    __identifier(INPUT_0),
    __attributes(ACF_ATTR_VEC_IN),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(1, 1)),
  __port(__index(1),
    __identifier(INPUT_1),
    __attributes(ACF_ATTR_VEC_IN),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(2, 1),
    __ek_size(1, 1)),
  __port(__index(2),
    __identifier(OUTPUT_0),
    __attributes(ACF_ATTR_VEC_OUT),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(2, 1),
    __ek_size(1, 1)),
  __port(__index(3),
    __identifier(CH_ID),
    __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(1, 1))
);


/*!*********************************************************************************
*  \brief ACF metadata for channel INSERT
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernelInfoConcat(INSERT_CHANNEL_3CH_K)
(
  INSERT_CHANNEL_3CH_KN,
  4,
  __port(__index(0),
    __identifier(INPUT_0),
    __attributes(ACF_ATTR_VEC_IN),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(1, 1)),
  __port(__index(1),
    __identifier(INPUT_1),
    __attributes(ACF_ATTR_VEC_IN),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(3, 1),
    __ek_size(1, 1)),
  __port(__index(2),
    __identifier(OUTPUT_0),
    __attributes(ACF_ATTR_VEC_OUT),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(3, 1),
    __ek_size(1, 1)),
  __port(__index(3),
    __identifier(CH_ID),
    __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(1, 1))
);


/*!*********************************************************************************
*  \brief ACF metadata for channel INSERT
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernelInfoConcat(INSERT_CHANNEL_4CH_K)
(
  INSERT_CHANNEL_4CH_KN,
  4,
  __port(__index(0),
    __identifier(INPUT_0),
    __attributes(ACF_ATTR_VEC_IN),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(1, 1)),
  __port(__index(1),
    __identifier(INPUT_1),
    __attributes(ACF_ATTR_VEC_IN),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(4, 1),
    __ek_size(1, 1)),
  __port(__index(2),
    __identifier(OUTPUT_0),
    __attributes(ACF_ATTR_VEC_OUT),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(4, 1),
    __ek_size(1, 1)),
  __port(__index(3),
    __identifier(CH_ID),
    __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(1, 1))
);


/*!*********************************************************************************
*  \brief ACF metadata for channel split
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernelInfoConcat(SPLIT_CHANNEL_2CH_K)
(
   SPLIT_CHANNEL_2CH_KN,
   3,
   __port(__index(0),
      __identifier(INPUT_0),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(2, 1),
      __ek_size(1, 1)),
   __port(__index(1),
      __identifier(OUTPUT_0),
      __attributes(ACF_ATTR_VEC_OUT), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   __port(__index(2),
      __identifier(OUTPUT_1),
      __attributes(ACF_ATTR_VEC_OUT), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for channel split
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernelInfoConcat(SPLIT_CHANNEL_3CH_K)
(
   SPLIT_CHANNEL_3CH_KN,
   4,
   __port(__index(0),
      __identifier(INPUT_0),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(3, 1),
      __ek_size(1, 1)),
   __port(__index(1),
      __identifier(OUTPUT_0),
      __attributes(ACF_ATTR_VEC_OUT), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   __port(__index(2),
      __identifier(OUTPUT_1),
      __attributes(ACF_ATTR_VEC_OUT), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   __port(__index(3),
      __identifier(OUTPUT_2),
      __attributes(ACF_ATTR_VEC_OUT), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1))
);


/*!*********************************************************************************
*  \brief ACF metadata for channel split
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernelInfoConcat(SPLIT_CHANNEL_4CH_K)
(
   SPLIT_CHANNEL_4CH_KN,
   5,
   __port(__index(0),
      __identifier(INPUT_0),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(4, 1),
      __ek_size(1, 1)),
   __port(__index(1),
      __identifier(OUTPUT_0),
      __attributes(ACF_ATTR_VEC_OUT), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   __port(__index(2),
      __identifier(OUTPUT_1),
      __attributes(ACF_ATTR_VEC_OUT), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   __port(__index(3),
      __identifier(OUTPUT_2),
      __attributes(ACF_ATTR_VEC_OUT), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   __port(__index(4),
      __identifier(OUTPUT_3),
      __attributes(ACF_ATTR_VEC_OUT), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1))
);



/*!*********************************************************************************
*  \brief ACF metadata for channel merge
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernelInfoConcat(MERGE_CHANNEL_2CH_K)
(
   MERGE_CHANNEL_2CH_KN,
   3,
   __port(__index(0),
      __identifier(INPUT_0),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   __port(__index(1),
      __identifier(INPUT_1),
      __attributes(ACF_ATTR_VEC_IN), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   __port(__index(2),
      __identifier(OUTPUT_0),
      __attributes(ACF_ATTR_VEC_OUT), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(2, 1),
      __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for channel merge
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernelInfoConcat(MERGE_CHANNEL_3CH_K)
(
   MERGE_CHANNEL_3CH_KN,
   4,
   __port(__index(0),
      __identifier(INPUT_0),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   __port(__index(1),
      __identifier(INPUT_1),
      __attributes(ACF_ATTR_VEC_IN), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   __port(__index(2),
      __identifier(INPUT_2),
      __attributes(ACF_ATTR_VEC_IN), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   __port(__index(3),
      __identifier(OUTPUT_0),
      __attributes(ACF_ATTR_VEC_OUT), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(3, 1),
      __ek_size(1, 1))
);


/*!*********************************************************************************
*  \brief ACF metadata for channel merge
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernelInfoConcat(MERGE_CHANNEL_4CH_K)
(
   MERGE_CHANNEL_4CH_KN,
   5,
   __port(__index(0),
      __identifier(INPUT_0),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   __port(__index(1),
      __identifier(INPUT_1),
      __attributes(ACF_ATTR_VEC_IN), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   __port(__index(2),
      __identifier(INPUT_2),
      __attributes(ACF_ATTR_VEC_IN), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   __port(__index(3),
      __identifier(INPUT_3),
      __attributes(ACF_ATTR_VEC_IN), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   __port(__index(4),
      __identifier(OUTPUT_0),
      __attributes(ACF_ATTR_VEC_OUT), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(4, 1),
      __ek_size(1, 1))
);




/*!*********************************************************************************
*  \brief ACF metadata for Hand-Tuned 3 Channel to Grayscale Conversion
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(HT_RGB888X_TO_Y_K)
(
   HT_RGB888X_TO_Y_KN,
   6,
   __port(__index(0),
          __identifier(INPUT_0),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(4, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier(KA),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(2),
          __identifier(KB),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(3),
          __identifier(KC),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(4),
          __identifier(KR),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(5),
          __identifier(OUTPUT_0),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16s),
          __e0_size(1, 1),
          __ek_size(1, 1))
);

/****************************************************************************/
/*!
RGB to grayscale conversion kernel metadata.

\param RGB_TO_GREY_KN_IN      Define for Kernel name
\param 2              Number of ports
\param "Port RGB_TO_GREY_KN_IN"  Define for name of input RGB image (unsigned 3x8bit)
\param "Port RGB_TO_GREY_KN_OUT" Define for name of output grayscale image (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(RGB888_TO_GREY_K)
(
  RGB888_TO_GREY_KN,
  2,
  __port(__index(0),
    __identifier(INPUT_0),
    __attributes(ACF_ATTR_VEC_IN),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(3, 1),
    __ek_size(1, 1)),
  __port(__index(1),
    __identifier(OUTPUT_0),
    __attributes(ACF_ATTR_VEC_OUT),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(1, 1))
);


/****************************************************************************/
/*!
BGR to grayscale conversion kernel metadata.

\param RGB_TO_GREY_KN_IN      Define for Kernel name
\param 2              Number of ports
\param "Port RGB_TO_GREY_KN_IN"  Define for name of input RGB image (unsigned 3x8bit)
\param "Port RGB_TO_GREY_KN_OUT" Define for name of output grayscale image (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(BGR888_TO_GREY_K)
(
  BGR888_TO_GREY_KN,
  2,
  __port(__index(0),
    __identifier(INPUT_0),
    __attributes(ACF_ATTR_VEC_IN),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(3, 1),
    __ek_size(1, 1)),
  __port(__index(1),
    __identifier(OUTPUT_0),
    __attributes(ACF_ATTR_VEC_OUT),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(1, 1))
);

/****************************************************************************/
/*!
grey value to RGB conversion kernel metadata.

\param 2              Number of ports
\param "Port RGB_TO_GREY_KN_IN"  Define for name of input RGB image (unsigned 3x8bit)
\param "Port RGB_TO_GREY_KN_OUT" Define for name of output grayscale image (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(GREY_TO_RGB888_K)
(
  GREY_TO_RGB888_KN,
  2,
  __port(__index(0),
    __identifier(INPUT_0),
    __attributes(ACF_ATTR_VEC_IN),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(1, 1)),
  __port(__index(1),
    __identifier(OUTPUT_0),
    __attributes(ACF_ATTR_VEC_OUT),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(3, 1),
    __ek_size(1, 1))
  );


#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "color_conversion_apu.h"

/*!*********************************************************************************
*  \brief ACF wrapper function for rgb565 to rgb888
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/                                 
void rgb565_to_rgb888x(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec16u* lpvIn0  = (vec16u*)lIn0.pMem;
   vec32u* lpvOut0 = (vec32u*)lOut0.pMem;
   
   apu_csc_rgb565_to_rgb888(lpvOut0, lOut0.chunkSpan / 4,
                             lpvIn0, lIn0.chunkSpan / 2,
                    lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for rgb888 to rgb565
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/                                 
void rgb888x_to_rgb565(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec32u* lpvIn0  = (vec32u*)lIn0.pMem;
   vec16u* lpvOut0 = (vec16u*)lOut0.pMem;
   
   apu_csc_rgb888_to_rgb565(lpvOut0, lOut0.chunkSpan / 2,
                             lpvIn0, lIn0.chunkSpan / 4,
                    lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for rgb888x to y
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/                                 
void rgb888x_to_y(kernel_io_desc lIn0, kernel_io_desc lR2Y, kernel_io_desc lG2Y, kernel_io_desc lB2Y, kernel_io_desc lOut0)
{
  vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
  int32_t* lpR2Y = (int32_t*)lR2Y.pMem;
  int32_t* lpG2Y = (int32_t*)lG2Y.pMem;
  int32_t* lpB2Y = (int32_t*)lB2Y.pMem;
  vec08u* lpvOut0 = (vec08u*)lOut0.pMem;
   
  apu_csc_rgb2y(lpvOut0, lOut0.chunkSpan,
                 lpvIn0, lIn0.chunkSpan,
        lIn0.chunkWidth, lIn0.chunkHeight,
               lpR2Y[0], lpG2Y[0], lpB2Y[0]);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for rgb888x to yuv
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/                                 
void rgb888x_to_yuv(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
  vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
  vec08u* lpvOut0 = (vec08u*)lOut0.pMem;
   
  apu_csc_rgb2yuv(lpvOut0, lOut0.chunkSpan,
                   lpvIn0, lIn0.chunkSpan,
          lIn0.chunkWidth, lIn0.chunkHeight );
}

/*!*********************************************************************************
*  \brief APU kernel
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void extract_channel_2ch(
  kernel_io_desc lIn0,
  kernel_io_desc lOut0,
  kernel_io_desc lChId)
{
  vec08u* lpvOut0 = (vec08u*)lOut0.pMem;
  vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
  int08u   lsChId = *((int08u*)lChId.pMem);

  apu_extract_channel(lpvOut0, lOut0.chunkSpan,
    lpvIn0 + lsChId, lIn0.chunkSpan,
    lOut0.chunkWidth, lOut0.chunkHeight, 2);
}


/*!*********************************************************************************
*  \brief APU kernel
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void extract_channel_3ch(
  kernel_io_desc lIn0,
  kernel_io_desc lOut0,
  kernel_io_desc lChId)
{
  vec08u* lpvOut0 = (vec08u*)lOut0.pMem;
  vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
  int08u   lsChId = *((int08u*)lChId.pMem);

  apu_extract_channel(lpvOut0, lOut0.chunkSpan,
    lpvIn0 + lsChId, lIn0.chunkSpan,
    lOut0.chunkWidth, lOut0.chunkHeight, 3);
}

/*!*********************************************************************************
*  \brief APU kernel
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void extract_channel_4ch(
    kernel_io_desc lIn0,
      kernel_io_desc lOut0,
      kernel_io_desc lChId)
{
  vec08u* lpvOut0    = (vec08u*)lOut0.pMem;
  vec08u* lpvIn0     = (vec08u*)lIn0.pMem;
  int08u   lsChId     = *((int08u*)lChId.pMem);
   
  apu_extract_channel(lpvOut0, lOut0.chunkSpan,
               lpvIn0 + lsChId, lIn0.chunkSpan,
               lOut0.chunkWidth, lOut0.chunkHeight, 4);
}


/*!*********************************************************************************
*  \brief APU kernel
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void insert_channel_2ch(
  kernel_io_desc lIn0,
  kernel_io_desc lIn1,
  kernel_io_desc lOut0,
  kernel_io_desc lChId)
{
  vec08u* lpvOut0 = (vec08u*)lOut0.pMem;
  vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
  vec08u* lpvIn1 = (vec08u*)lIn1.pMem;
  int08u   lsChId = *((int08u*)lChId.pMem);

  apu_insert_channel_2ch(lpvOut0, lOut0.chunkSpan,
    lpvIn0, lIn0.chunkSpan,
    lpvIn1, lIn1.chunkSpan,
    lOut0.chunkWidth, lOut0.chunkHeight, lsChId);
}


/*!*********************************************************************************
*  \brief APU kernel
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void insert_channel_3ch(
  kernel_io_desc lIn0,
  kernel_io_desc lIn1,
  kernel_io_desc lOut0,
  kernel_io_desc lChId)
{
  vec08u* lpvOut0 = (vec08u*)lOut0.pMem;
  vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
  vec08u* lpvIn1 = (vec08u*)lIn1.pMem;
  int08u   lsChId = *((int08u*)lChId.pMem);

  apu_insert_channel_3ch(lpvOut0, lOut0.chunkSpan,
    lpvIn0, lIn0.chunkSpan,
    lpvIn1, lIn1.chunkSpan,
    lOut0.chunkWidth, lOut0.chunkHeight, lsChId);
}

/*!*********************************************************************************
*  \brief APU kernel
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void insert_channel_4ch(
  kernel_io_desc lIn0,
  kernel_io_desc lIn1,
  kernel_io_desc lOut0,
  kernel_io_desc lChId)
{
  vec08u* lpvOut0 = (vec08u*)lOut0.pMem;
  vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
  vec08u* lpvIn1 = (vec08u*)lIn1.pMem;
  int08u   lsChId = *((int08u*)lChId.pMem);

  apu_insert_channel_4ch(lpvOut0, lOut0.chunkSpan,
    lpvIn0, lIn0.chunkSpan,
    lpvIn1, lIn1.chunkSpan,
    lOut0.chunkWidth, lOut0.chunkHeight, lsChId);
}



/*!*********************************************************************************
*  \brief APU kernel
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void split_channel_2ch(
  kernel_io_desc lIn0,
  kernel_io_desc lOut0,
  kernel_io_desc lOut1)
{
  vec08u* lpvOut0 = (vec08u*)lOut0.pMem;
  vec08u* lpvOut1 = (vec08u*)lOut1.pMem;
  vec08u* lpvIn0 = (vec08u*)lIn0.pMem;

  apu_split_channel_2ch(lpvOut0, lOut0.chunkSpan,
    lpvOut1, lOut1.chunkSpan,
    lpvIn0, lIn0.chunkSpan,
    lOut0.chunkWidth, lOut0.chunkHeight);
}

void split_channel_3ch(
  kernel_io_desc lIn0,
  kernel_io_desc lOut0,
  kernel_io_desc lOut1,
  kernel_io_desc lOut2)
{
  vec08u* lpvOut0 = (vec08u*)lOut0.pMem;
  vec08u* lpvOut1 = (vec08u*)lOut1.pMem;
  vec08u* lpvOut2 = (vec08u*)lOut2.pMem;
  vec08u* lpvIn0 = (vec08u*)lIn0.pMem;

  apu_split_channel_3ch(lpvOut0, lOut0.chunkSpan,
    lpvOut1, lOut1.chunkSpan,
    lpvOut2, lOut2.chunkSpan,
    lpvIn0, lIn0.chunkSpan,
    lOut0.chunkWidth, lOut0.chunkHeight);
}

void split_channel_4ch(
  kernel_io_desc lIn0,
  kernel_io_desc lOut0,
  kernel_io_desc lOut1,
  kernel_io_desc lOut2,
  kernel_io_desc lOut3)
{
  vec08u* lpvOut0 = (vec08u*)lOut0.pMem;
  vec08u* lpvOut1 = (vec08u*)lOut1.pMem;
  vec08u* lpvOut2 = (vec08u*)lOut2.pMem;
  vec08u* lpvOut3 = (vec08u*)lOut3.pMem;
  vec08u* lpvIn0 = (vec08u*)lIn0.pMem;

  apu_split_channel_4ch(lpvOut0, lOut0.chunkSpan,
    lpvOut1, lOut1.chunkSpan,
    lpvOut2, lOut2.chunkSpan,
    lpvOut3, lOut3.chunkSpan,
    lpvIn0, lIn0.chunkSpan,
    lOut0.chunkWidth, lOut0.chunkHeight);
}





void merge_channel_2ch(
  kernel_io_desc lIn0,
  kernel_io_desc lIn1,
  kernel_io_desc lOut0)
{
  vec08u* lpvOut0 = (vec08u*)lOut0.pMem;
  vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
  vec08u* lpvIn1 = (vec08u*)lIn1.pMem;

  apu_merge_channel_2ch(lpvOut0, lOut0.chunkSpan,
    lpvIn0, lIn0.chunkSpan,
    lpvIn1, lIn1.chunkSpan,
    lOut0.chunkWidth, lOut0.chunkHeight);
}


void merge_channel_3ch(
  kernel_io_desc lIn0,
  kernel_io_desc lIn1,
  kernel_io_desc lIn2,
  kernel_io_desc lOut0)
{
  vec08u* lpvOut0 = (vec08u*)lOut0.pMem;
  vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
  vec08u* lpvIn1 = (vec08u*)lIn1.pMem;
  vec08u* lpvIn2 = (vec08u*)lIn2.pMem;

  apu_merge_channel_3ch(lpvOut0, lOut0.chunkSpan,
    lpvIn0, lIn0.chunkSpan,
    lpvIn1, lIn1.chunkSpan,
    lpvIn2, lIn2.chunkSpan,
    lOut0.chunkWidth, lOut0.chunkHeight);
}


void merge_channel_4ch(
  kernel_io_desc lIn0,
  kernel_io_desc lIn1,
  kernel_io_desc lIn2,
  kernel_io_desc lIn3,
  kernel_io_desc lOut0)
{
  vec08u* lpvOut0 = (vec08u*)lOut0.pMem;
  vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
  vec08u* lpvIn1 = (vec08u*)lIn1.pMem;
  vec08u* lpvIn2 = (vec08u*)lIn2.pMem;
  vec08u* lpvIn3 = (vec08u*)lIn3.pMem;

  apu_merge_channel_4ch(lpvOut0, lOut0.chunkSpan,
    lpvIn0, lIn0.chunkSpan,
    lpvIn1, lIn1.chunkSpan,
    lpvIn2, lIn2.chunkSpan,
    lpvIn3, lIn3.chunkSpan,
    lOut0.chunkWidth, lOut0.chunkHeight);
}






/*!*********************************************************************************
*  \brief ACF wrapper function for Hand-Tuned 3 Channel to Grayscale Conversion
***********************************************************************************/
void ht_rgb888x_to_y(kernel_io_desc lIn0, kernel_io_desc lKA, kernel_io_desc lKB, kernel_io_desc lKC, kernel_io_desc lKR, kernel_io_desc lOut0)
{
  vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
  uint8_t* lpKA = (uint8_t*)lKA.pMem;
  uint8_t* lpKB = (uint8_t*)lKB.pMem;
  uint8_t* lpKC = (uint8_t*)lKC.pMem;
  uint16_t* lpKR = (uint16_t*)lKR.pMem;
  vec16s* lpvOut0 = (vec16s*)lOut0.pMem;

   ht::apu_csc_8UC3_Y(lpvOut0, lOut0.chunkSpan>>1,
                       lpvIn0, lIn0.chunkSpan,
              lIn0.chunkWidth, lIn0.chunkHeight,
                      lpKA[0], lpKB[0], lpKC[0], lpKR[0]);
}


void rgb888_to_grey(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
  vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
  vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

  apu_rgb888_to_grey(lpvOut0, lpvIn0, lIn0.chunkWidth, lIn0.chunkHeight, lOut0.chunkSpan, lIn0.chunkSpan);
}


void bgr888_to_grey(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
  vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
  vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

  apu_bgr888_to_grey(lpvOut0, lpvIn0, lIn0.chunkWidth, lIn0.chunkHeight, lOut0.chunkSpan, lIn0.chunkSpan);
}


void grey_to_rgb888(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
  vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
  vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

  apu_grey_to_rgb888(lpvOut0, lpvIn0, lIn0.chunkWidth, lIn0.chunkHeight, lOut0.chunkSpan, lIn0.chunkSpan);
}


#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
/*! @} */
