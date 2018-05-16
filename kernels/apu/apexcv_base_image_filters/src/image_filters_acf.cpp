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
* \file filter.cpp
* \addtogroup apexcv-filter
* \addtogroup filter
* \ingroup filter
* @{
* \brief filter operations mimicking opencv
*/

/*!*********************************************************************************
*  @file filter.c
*  @brief ACF metadata and wrapper function for filter kernels
***********************************************************************************/

#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA

#include "image_filters_acf.h"

/*!*********************************************************************************
*  \brief ACF metadata for bilateral 5x5 filter
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(BILATERAL_5X5_K)
(
  BILATERAL_5X5_KN,
   6,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2,2,2,2),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(R_LUT),
            __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(256, 1)),
   __port(  __index(2),
            __identifier(D_LUT),
            __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(9, 1)),
   __port(  __index(3),
            __identifier(SIGMA_R),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d32s),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(4),
            __identifier(SIGMA_D),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d32s),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(5),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for box 1x3 filter
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(BOX_1X3_16S_K)
(
  BOX_1X3_16S_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(0,0,1,2),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(1, 2)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(1, 2))
);

/*!*********************************************************************************
*  \brief ACF metadata for box 3x1 filter
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(BOX_3X1_16S_K)
(
  BOX_3X1_16S_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1,1,0,0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for box 3x3 filter
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(BOX_3X3_08U_K)
(
   BOX_3X3_08U_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1,1,1,1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 3x3 Box blur.
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(BOX_3X3_16S_K)
(
   BOX_3X3_16S_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1, 1, 1, 1),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(1, 1)),

   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 5x5 Box blur.
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(BOX_5X5_16S_K)
(
   BOX_5X5_16S_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2, 2, 2, 2),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(1, 1)),

   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for census (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernelInfoConcat(CENSUS_3X3_08U_K)
(
   CENSUS_3X3_08U_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1,1,1,1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for convolve (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(CONVOLVE_3X3_08U_K)
(
   CONVOLVE_3X3_08U_KN,
   3,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1,1,1,1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
    __port(  __index(2),
            __identifier(F_COEFF),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(9, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for convolve with scale (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(CONVOLVE_SCALE_3X3_08U_K)
(
   CONVOLVE_SCALE_3X3_08U_KN,
   4,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1,1,1,1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
    __port(  __index(2),
            __identifier(F_COEFF),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(9, 1)),
    __port(  __index(3),
            __identifier(F_SCALE),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d32s),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for convolve (signed 16 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(CONVOLVE_3X3_16S_K)
(
   CONVOLVE_3X3_16S_KN,
   3,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1,1,1,1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(1, 1)),
    __port(  __index(2),
            __identifier(F_COEFF),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(9, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for convolve with scale (signed 16 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(CONVOLVE_SCALE_3X3_16S_K)
(
   CONVOLVE_SCALE_3X3_16S_KN,
   4,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1,1,1,1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(1, 1)),
    __port(  __index(2),
            __identifier(F_COEFF),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(9, 1)),
    __port(  __index(3),
            __identifier(F_SCALE),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d32s),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for convolve (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(CONVOLVE_5X5_08U_K)
(
   CONVOLVE_5X5_08U_KN,
   3,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2,2,2,2),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
    __port(  __index(2),
            __identifier(F_COEFF),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(25, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for convolve with scale (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(CONVOLVE_SCALE_5X5_08U_K)
(
  CONVOLVE_SCALE_5X5_08U_KN,
    4,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2,2,2,2),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
    __port(  __index(2),
            __identifier(F_COEFF),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(25, 1)),
    __port(  __index(3),
            __identifier(F_SCALE),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d32s),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for convolve (signed 16 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(CONVOLVE_5X5_16S_K)
(
   CONVOLVE_5X5_16S_KN,
   3,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2,2,2,2),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(1, 1)),
    __port(  __index(2),
            __identifier(F_COEFF),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(25, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for convolve with scale (signed 16 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(CONVOLVE_SCALE_5X5_16S_K)
(
  CONVOLVE_SCALE_5X5_16S_KN,
   4,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2,2,2,2),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(1, 1)),
    __port(  __index(2),
            __identifier(F_COEFF),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(25, 1)),
    __port(  __index(3),
            __identifier(F_SCALE),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d32s),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for dilate 3x3 (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(DILATE_3X3_08U_K)
(
   DILATE_3X3_08U_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1,1,1,1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for dilate 3x3 (signed 16 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(DILATE_3X3_16S_K)
(
   DILATE_3X3_16S_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1,1,1,1),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(2, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(2, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for erode 3x3 (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(ERODE_3X3_08U_K)
(
   ERODE_3X3_08U_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1,1,1,1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for gaussian 3x3 (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(GAUSSIAN_3X3_08U_K)
(
   GAUSSIAN_3X3_08U_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1,1,1,1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for gaussian 5x5 (unsigned 8 bit)
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(GAUSSIAN_5X5_08U_K)
(
   GAUSSIAN_5X5_08U_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2,2,2,2),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for median 3x3 (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(MEDIAN_3X3_08U_K)
(
   MEDIAN_3X3_08U_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1,1,1,1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 2)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 2))
);

/*!*********************************************************************************
*  \brief ACF metadata for median 5x5 (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(MEDIAN_5X5_08U_K)
(
   MEDIAN_5X5_08U_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2,2,2,2),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(2, 2)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(2, 2))
);

/*!*********************************************************************************
*  \brief ACF metadata for prewitt 3x3 x
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(PREWITT_X_3X3_K)
(
   PREWITT_X_3X3_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1,1,1,1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for prewitt 3x3 y
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(PREWITT_Y_3X3_K)
(
   PREWITT_Y_3X3_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1,1,1,1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 3x3 Sobel operation (unsigned 8 bit).
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(SOBEL_3X3_08U_K)
(
   SOBEL_3X3_08U_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1,1,1,1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 3x3 Sobel X&Y operation (unsigned 8 bit).
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(SOBEL_XY_3X3_08U_K)
(
   SOBEL_XY_3X3_08U_KN,
   3,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1,1,1,1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(2),
            __identifier("OUTPUT_1"),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 3x3 Sobel Y operation (unsigned 8 bit).
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(SOBEL_X_3X3_08U_K)
(
   SOBEL_X_3X3_08U_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1,1,1,1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 3x3 Sobel X operation (signed 8 bit).
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(SOBEL_X_3X3_08S_K)
(
   SOBEL_X_3X3_08S_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1, 1, 1, 1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),

   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 3x3 Sobel Y operation (unsigned 8 bit).
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(SOBEL_Y_3X3_08U_K)
(
   SOBEL_Y_3X3_08U_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1,1,1,1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 3x3 Sobel Y operation (signed 8 bit).
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(SOBEL_Y_3X3_08S_K)
(
   SOBEL_Y_3X3_08S_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1, 1, 1, 1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),

   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(1, 1))
);


/*!*********************************************************************************
*  \brief ACF metadata for Sobel 5x5 (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(SOBEL_5X5_08U_K)
(
   SOBEL_5X5_08U_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2,2,2,2),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 5x5 Sobel X operation (unsigned 8 bit).
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(SOBEL_X_5X5_08U_K)
(
   SOBEL_X_5X5_08U_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2,2,2,2),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 5x5 Sobel X operation (signed 8 bit).
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(SOBEL_X_5X5_08S_K)
(
   SOBEL_X_5X5_08S_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2, 2, 2, 2),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),

   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 5x5 Sobel Y operation (unsigned 8 bit).
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(SOBEL_Y_5X5_08U_K)
(
   SOBEL_Y_5X5_08U_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2,2,2,2),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0,0,0,0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 5x5 Sobel Y operation (signed 8 bit).
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(SOBEL_Y_5X5_08S_K)
(
   SOBEL_Y_5X5_08S_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2, 2, 2, 2),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),

   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/* Hand-Tuned kernel function ACF metadata */
/*!*********************************************************************************
*  \brief ACF metadata for the Hand-Tuned Box 3x3 Operation. (unsigned 8-bit).
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(HT_BOX_3X3_08U_K)
(
   HT_BOX_3X3_08U_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1, 1, 1, 1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(2, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(2, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the Hand-Tuned 3x3 Central DX Operation (signed 16-bit).
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(HT_CENTRALDX_3X3_16S_K)
(
   HT_CENTRALDX_3X3_16S_KN,
   5,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2, 2, 1, 1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(4, 1)),
   __port(  __index(1),
            __identifier("K_0"),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(2),
            __identifier("K_1"),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(3),
            __identifier("K_2"),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(4),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(4, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the Hand-Tuned 3x3 Central DY Operation (signed 16-bit).
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(HT_CENTRALDY_3X3_16S_K)
(
  HT_CENTRALDY_3X3_16S_KN,
   5,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1, 1, 1, 1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(4, 1)),
   __port(  __index(1),
            __identifier("K_0"),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(2),
            __identifier("K_1"),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(3),
            __identifier("K_2"),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(4),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(4, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the Hand-Tuned 3x3 Generic Filter Operation (unsigned 8-bit).
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(HT_GENERIC_3X3_08U_K)
(
   HT_GENERIC_3X3_08U_KN,
   4,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1, 1, 1, 1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(2, 1)),
   __port(  __index(1),
            __identifier(F_COEFF),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(12, 1)),
   __port(  __index(2),
            __identifier(F_SCALE),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(3),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(2, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the Hand-Tuned 5x5 Generic Filter Operation (unsigned 8-bit).
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(HT_GENERIC_5X5_08U_K)
(
   HT_GENERIC_5X5_08U_KN,
   4,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2, 2, 2, 2),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(2, 1)),
   __port(  __index(1),
            __identifier(F_COEFF),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(28, 1)),
   __port(  __index(2),
            __identifier(F_SCALE),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(1, 1)),
   __port(  __index(3),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(2, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the Hand-Tuned Saturate to signed 8-bit Operation.
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(HT_SATURATE_08S_K)
(
   HT_SATURATE_08S_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(2, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(2, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the Hand-Tuned 3x3 Separable Filter Operation (signed 16-bit).
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(HT_SEPARABLE_3X3_16S_K)
(
   HT_SEPARABLE_3X3_16S_KN,
   4,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1, 1, 1, 1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(4, 1)),
   __port(  __index(1),
            __identifier(F_ROW),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(3, 1)),
   __port(  __index(2),
            __identifier(F_COL),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(3, 1)),
   __port(  __index(3),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(4, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the Hand-Tuned 5x5 Separable Filter Operation (signed 16-bit).
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(HT_SEPARABLE_5X5_16S_K)
(
   HT_SEPARABLE_5X5_16S_KN,
   4,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2, 2, 2, 2),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(4, 1)),
   __port(  __index(1),
            __identifier(F_ROW),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(5, 1)),
   __port(  __index(2),
            __identifier(F_COL),
            __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(5, 1)),
   __port(  __index(3),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(4, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the Hand-Tuned 3x3 Sobel Operation (signed 16-bit).
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(HT_SOBEL_3X3_16S_K)
(
   HT_SOBEL_3X3_16S_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2, 2, 2, 2),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(4, 1)),
   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(4, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the Hand-Tuned 3x3 Sobel X Operation (signed 8-bit).
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(HT_SOBEL_X_3X3_08S_K)
(
   HT_SOBEL_X_3X3_08S_KN,
   3,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2, 2, 1, 1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(4, 1)),
   __port(  __index(1),
            __identifier(BUFFER_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(4, 1)),
   __port(  __index(2),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(4, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the Hand-Tuned 3x3 Sobel Y Operation (signed 8-bit).
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(HT_SOBEL_Y_3X3_08S_K)
(
   HT_SOBEL_Y_3X3_08S_KN,
   3,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2, 2, 1, 1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(4, 1)),
   __port(  __index(1),
            __identifier(BUFFER_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(4, 1)),
   __port(  __index(2),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d08s),
            __e0_size(1, 1),
            __ek_size(4, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 3x3 Sobel X operation (signed 16 bit).
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(HT_SOBEL_X_3X3_16S_K)
(
   HT_SOBEL_X_3X3_16S_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1, 1, 1, 1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(4, 1)),

   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(4, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 3x3 Sobel Y operation (signed 16 bit).
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(HT_SOBEL_Y_3X3_16S_K)
(
   HT_SOBEL_Y_3X3_16S_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(1, 1, 1, 1),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(4, 1)),

   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(4, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 5x5 Sobel X operation (signed 16 bit).
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(HT_SOBEL_X_5X5_16S_K)
(
   HT_SOBEL_X_5X5_16S_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2, 2, 2, 2),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),

   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 5x5 Sobel Y operation (signed 16 bit).
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(HT_SOBEL_Y_5X5_16S_K)
(
   HT_SOBEL_Y_5X5_16S_KN,
   2,
   __port(  __index(0),
            __identifier(INPUT_0),
            __attributes(ACF_ATTR_VEC_IN),
            __spatial_dep(2, 2, 2, 2),
            __e0_data_type(d08u),
            __e0_size(1, 1),
            __ek_size(1, 1)),

   __port(  __index(1),
            __identifier(OUTPUT_0),
            __attributes(ACF_ATTR_VEC_OUT),
            __spatial_dep(0, 0, 0, 0),
            __e0_data_type(d16s),
            __e0_size(1, 1),
            __ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "image_filters_apu.h"

/*!*********************************************************************************
*  \brief ACF wrapper function for bilateral 5x5 filter
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void bilateral_5x5(kernel_io_desc lIn0, kernel_io_desc lRLut, kernel_io_desc lDLut, kernel_io_desc lSigmaR, kernel_io_desc lSigmaD, kernel_io_desc lOut0)
{
   vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
   vec08u* lpRLut = (vec08u*)lRLut.pMem;
   unsigned char* lpDLut = (unsigned char*)lDLut.pMem;
   int* lpSigmaR = (int*)lSigmaR.pMem;
   int* lpSigmaD = (int*)lSigmaD.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

   if(ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG)) {
      apu_flt_bilateral_5x5_init( lpRLut, lpDLut, lpSigmaR[0], lpSigmaD[0] );
   }

   apu_flt_bilateral_5x5(lpvOut0, lOut0.chunkSpan,
                          lpvIn0, lIn0.chunkSpan,
                 lIn0.chunkWidth, lIn0.chunkHeight,
                 (vec08u*)lpRLut, lpDLut );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for box 1x3 filter
***********************************************************************************/
void box_1x3_16s(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec16s* lpvIn0  = (vec16s*)lIn0.pMem;
   vec16s* lpvOut0 = (vec16s*)lOut0.pMem;

   apu_flt_box_1x3(lpvOut0, lOut0.chunkSpan>>1,
                    lpvIn0, lIn0.chunkSpan>>1,
           lIn0.chunkWidth, lIn0.chunkHeight );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for box 3x1 filter
***********************************************************************************/
void box_3x1_16s(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec16s* lpvIn0  = (vec16s*)lIn0.pMem;
   vec16s* lpvOut0 = (vec16s*)lOut0.pMem;

   apu_flt_box_3x1(lpvOut0, lOut0.chunkSpan>>1,
                   lpvIn0, lIn0.chunkSpan>>1,
          lIn0.chunkWidth, lIn0.chunkHeight );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for box 3x3 filter
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void box_3x3_08u(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

   apu_flt_box_3x3(lpvOut0, lOut0.chunkSpan,
                    lpvIn0, lIn0.chunkSpan,
           lIn0.chunkWidth, lIn0.chunkHeight );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for the 3x3 Box blur.
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void box_3x3_16s(kernel_io_desc lIn          /*!< The padded source image.*/,
                 kernel_io_desc lOut         /*!< The destination image.*/
)
{
   vec16s* lpvIn = (vec16s*)lIn.pMem;
   vec16s* lpvOut = (vec16s*)lOut.pMem;

   apu_flt_box_3x3(lpvOut, lOut.chunkSpan>>1,
                    lpvIn, lIn.chunkSpan>>1,
           lIn.chunkWidth, lIn.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for the 5x5 Box blur.
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void box_5x5_16s(kernel_io_desc lIn          /*!< The padded source image.*/,
                 kernel_io_desc lOut         /*!< The destination image.*/
)
{
   vec16s* lpvIn = (vec16s*)lIn.pMem;
   vec16s* lpvOut = (vec16s*)lOut.pMem;

   apu_flt_box_5x5(lpvOut, lOut.chunkSpan>>1,
                    lpvIn, lIn.chunkSpan>>1,
           lIn.chunkWidth, lIn.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for census (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void census_3x3_08u(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0          = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0         = (vec08u*)lOut0.pMem;

   apu_flt_census_3x3(lpvOut0,
                      lOut0.chunkSpan,  
            lpvIn0,
                      lIn0.chunkSpan,
                      lIn0.chunkWidth, 
                      lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for convolve (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void convolve_3x3_08u(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lFilterCoeff)
{
   vec08u* lpvIn0          = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0         = (vec08u*)lOut0.pMem;
   int8_t* lpFilterCoeff   = (int8_t*)lFilterCoeff.pMem;

   apu_flt_fir2(lpvOut0, lOut0.chunkSpan,
                 lpvIn0, lIn0.chunkSpan,
        lIn0.chunkWidth, lIn0.chunkHeight,
          lpFilterCoeff, 3, 3 );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for convolve with scale (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void convolve_scale_3x3_08u(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lFilterCoeff, kernel_io_desc lFilterScale)
{
   vec08u* lpvIn0          = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0         = (vec08u*)lOut0.pMem;
   int8_t* lpFilterCoeff   = (int8_t*)lFilterCoeff.pMem;
   int* lpFilterScale      = (int*)lFilterScale.pMem;

   apu_flt_fir2(lpvOut0, lOut0.chunkSpan,
                 lpvIn0, lIn0.chunkSpan,
        lIn0.chunkWidth, lIn0.chunkHeight,
          lpFilterCoeff, 3, 3, lpFilterScale[0] );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for convolve (signed 16 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void convolve_3x3_16s(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lFilterCoeff)
{
   vec08u* lpvIn0          = (vec08u*)lIn0.pMem;
   vec16s* lpvOut0         = (vec16s*)lOut0.pMem;
   int8_t* lpFilterCoeff   = (int8_t*)lFilterCoeff.pMem;

   apu_flt_fir2(lpvOut0, lOut0.chunkSpan>>1,
                 lpvIn0, lIn0.chunkSpan,
        lIn0.chunkWidth, lIn0.chunkHeight,
          lpFilterCoeff, 3, 3 );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for convolve with scale (signed 16 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void convolve_scale_3x3_16s(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lFilterCoeff, kernel_io_desc lFilterScale)
{
   vec08u* lpvIn0          = (vec08u*)lIn0.pMem;
   vec16s* lpvOut0         = (vec16s*)lOut0.pMem;
   int8_t* lpFilterCoeff   = (int8_t*)lFilterCoeff.pMem;
   int* lpFilterScale     = (int*)lFilterScale.pMem;

   apu_flt_fir2(lpvOut0, lOut0.chunkSpan>>1,
                 lpvIn0, lIn0.chunkSpan,
        lIn0.chunkWidth, lIn0.chunkHeight,
          lpFilterCoeff, 3, 3, lpFilterScale[0] );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for convolve (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void convolve_5x5_08u(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lFilterCoeff)
{
   vec08u* lpvIn0          = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0         = (vec08u*)lOut0.pMem;
   int8_t* lpFilterCoeff   = (int8_t*)lFilterCoeff.pMem;

   apu_flt_fir2(lpvOut0, lOut0.chunkSpan,
                 lpvIn0, lIn0.chunkSpan,
        lIn0.chunkWidth, lIn0.chunkHeight,
          lpFilterCoeff, 5, 5 );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for convolve with scale (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void convolve_scale_5x5_08u(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lFilterCoeff, kernel_io_desc lFilterScale)
{
   vec08u* lpvIn0          = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0         = (vec08u*)lOut0.pMem;
   int8_t* lpFilterCoeff   = (int8_t*)lFilterCoeff.pMem;
   int* lpFilterScale     = (int*)lFilterScale.pMem;

   apu_flt_fir2(lpvOut0, lOut0.chunkSpan,
                 lpvIn0, lIn0.chunkSpan,
        lIn0.chunkWidth, lIn0.chunkHeight,
          lpFilterCoeff, 5, 5, lpFilterScale[0] );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for convolve (signed 16 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void convolve_5x5_16s(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lFilterCoeff)
{
   vec08u* lpvIn0          = (vec08u*)lIn0.pMem;
   vec16s* lpvOut0         = (vec16s*)lOut0.pMem;
   int8_t* lpFilterCoeff   = (int8_t*)lFilterCoeff.pMem;

   apu_flt_fir2(lpvOut0, lOut0.chunkSpan / 2,
                 lpvIn0, lIn0.chunkSpan,
        lIn0.chunkWidth, lIn0.chunkHeight,
          lpFilterCoeff, 5, 5 );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for convolve with scale (signed 16 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void convolve_scale_5x5_16s(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lFilterCoeff, kernel_io_desc lFilterScale)
{
   vec08u* lpvIn0          = (vec08u*)lIn0.pMem;
   vec16s* lpvOut0         = (vec16s*)lOut0.pMem;
   int8_t* lpFilterCoeff   = (int8_t*)lFilterCoeff.pMem;
    int* lpFilterScale     = (int*)lFilterScale.pMem;

   apu_flt_fir2(lpvOut0, lOut0.chunkSpan / 2,
                 lpvIn0, lIn0.chunkSpan,
        lIn0.chunkWidth, lIn0.chunkHeight,
          lpFilterCoeff, 5, 5, lpFilterScale[0] );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for dilate 3x3 (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void dilate_3x3_08u(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

   apu_flt_dilate_3x3(lpvOut0, lOut0.chunkSpan,
                       lpvIn0, lIn0.chunkSpan,
              lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for dilate 3x3 (signed 16 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void dilate_3x3_16s(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec16s* lpvIn0  = (vec16s*)lIn0.pMem;
   vec16s* lpvOut0 = (vec16s*)lOut0.pMem;

   apu_flt_dilate_3x3(lpvOut0, lOut0.chunkSpan>>1,
                       lpvIn0, lIn0.chunkSpan>>1,
              lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for erode 3x3 (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void erode_3x3_08u(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

   apu_flt_erode_3x3(lpvOut0, lOut0.chunkSpan,
                      lpvIn0, lIn0.chunkSpan,
             lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for gaussian 3x3 (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void gaussian_3x3_08u(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

   apu_flt_gaussian_3x3(lpvOut0, lOut0.chunkSpan,
                         lpvIn0, lIn0.chunkSpan,
                lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for gaussian 5x5 (unsigned 8 bit)
***********************************************************************************/
void gaussian_5x5_08u(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

   apu_flt_gaussian_5x5(lpvOut0, lOut0.chunkSpan,
                         lpvIn0, lIn0.chunkSpan,
                lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for median 3x3 (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void median_3x3_08u(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

   apu_flt_median_3x3(lpvOut0, lOut0.chunkSpan,
                       lpvIn0, lIn0.chunkSpan,
              lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for median 3x3 (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void median_5x5_08u(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

   apu_flt_median_5x5(lpvOut0, lOut0.chunkSpan,
                       lpvIn0, lIn0.chunkSpan,
              lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for prewitt x 3x3
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void prewitt_x_3x3(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec16s* lpvOut0 = (vec16s*)lOut0.pMem;

   apu_flt_prewitt_3x3_x(lpvOut0, lOut0.chunkSpan>>1,
                          lpvIn0, lIn0.chunkSpan,
                 lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for prewitt y 3x3
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void prewitt_y_3x3(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec16s* lpvOut0 = (vec16s*)lOut0.pMem;

   apu_flt_prewitt_3x3_y(lpvOut0, lOut0.chunkSpan>>1,
                          lpvIn0, lIn0.chunkSpan,
                 lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for sobel 3x3 (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void sobel_3x3_08u(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

   apu_flt_sobel_3x3(lpvOut0, lOut0.chunkSpan,
                      lpvIn0, lIn0.chunkSpan,
             lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for sobel 3x3 x&y (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void sobel_xy_3x3_08u(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lOut1)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;
   vec08u* lpvOut1 = (vec08u*)lOut1.pMem;

   apu_flt_sobel_3x3_xy(lpvOut0, lOut0.chunkSpan,
                        lpvOut1, lOut1.chunkSpan,
                         lpvIn0, lIn0.chunkSpan,
                lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for sobel x 3x3 (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void sobel_x_3x3_08u(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

   apu_flt_sobel_3x3_x(lpvOut0, lOut0.chunkSpan,
                        lpvIn0, lIn0.chunkSpan,
               lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for sobel x 3x3 (unsigned 8 bit to signed 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void sobel_x_3x3_08s(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08s* lpvOut0 = (vec08s*)lOut0.pMem;

   apu_flt_sobel_3x3_x(lpvOut0, lOut0.chunkSpan,
                        lpvIn0, lIn0.chunkSpan,
               lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for sobel y 3x3 (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void sobel_y_3x3_08u(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

   apu_flt_sobel_3x3_y(lpvOut0, lOut0.chunkSpan,
                        lpvIn0, lIn0.chunkSpan,
               lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for sobel y 3x3 (unsigned 8 bit to signed 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void sobel_y_3x3_08s(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08s* lpvOut0 = (vec08s*)lOut0.pMem;

   apu_flt_sobel_3x3_y(lpvOut0, lOut0.chunkSpan,
                        lpvIn0, lIn0.chunkSpan,
               lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for sobel 5x5 (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void sobel_5x5_08u(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

   apu_flt_sobel_5x5(lpvOut0, lOut0.chunkSpan,
                      lpvIn0, lIn0.chunkSpan,
             lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for sobel x 5x5 (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void sobel_x_5x5_08u(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

   apu_flt_sobel_5x5_x(lpvOut0, lOut0.chunkSpan,
                        lpvIn0, lIn0.chunkSpan,
               lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for sobel x 5x5 (unsigned 8 bit to signed 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void sobel_x_5x5_08s(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08s* lpvOut0 = (vec08s*)lOut0.pMem;

   apu_flt_sobel_5x5_x(lpvOut0, lOut0.chunkSpan,
                        lpvIn0, lIn0.chunkSpan,
               lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for sobel y 5x5 (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void sobel_y_5x5_08u(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

   apu_flt_sobel_5x5_y(lpvOut0, lOut0.chunkSpan,
                        lpvIn0, lIn0.chunkSpan,
               lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for sobel y 5x5 (unsigned 8 bit).
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void sobel_y_5x5_08s(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08s* lpvOut0 = (vec08s*)lOut0.pMem;

   apu_flt_sobel_5x5_y(lpvOut0, lOut0.chunkSpan,
                        lpvIn0, lIn0.chunkSpan,
               lIn0.chunkWidth, lIn0.chunkHeight);
}

/* Hand-Tuned kernel function ACF functions */
/*!*********************************************************************************
*  \brief ACF wrapper function for Hand-Tuned Box_3x3 Operation (unsigned 8 bit).
***********************************************************************************/
void ht_box_3x3_08u(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

   ht::apu_flt_box_3x3(lpvOut0, lOut0.chunkSpan,
                        lpvIn0, lIn0.chunkSpan,
               lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for Hand-Tuned 3x3 Central DX Operation (signed 16-bit).
***********************************************************************************/
void ht_centraldx_3x3_16s(kernel_io_desc lIn0, kernel_io_desc lK0, kernel_io_desc lK1, kernel_io_desc lK2, kernel_io_desc lOut0)
{
   vec08u* lpvIn0    = (vec08u*)lIn0.pMem;
   vec16s* lpvOut0   = (vec16s*)lOut0.pMem;
   int8_t* lpK0      = (int8_t*)lK0.pMem;
   int8_t* lpK1      = (int8_t*)lK1.pMem;
   int8_t* lpK2      = (int8_t*)lK2.pMem;

   ht::apu_flt_centraldx_3x3(lpvOut0, lOut0.chunkSpan>>1,
                              lpvIn0, lIn0.chunkSpan,
                     lIn0.chunkWidth, lIn0.chunkHeight,
                             lpK0[0], lpK1[0], lpK2[0] );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for Hand-Tuned 3x3 Central DY Operation (signed 16-bit).
***********************************************************************************/
void ht_centraldy_3x3_16s(kernel_io_desc lIn0, kernel_io_desc lK0, kernel_io_desc lK1, kernel_io_desc lK2, kernel_io_desc lOut0)
{
   vec08u* lpvIn0    = (vec08u*)lIn0.pMem;
   vec16s* lpvOut0   = (vec16s*)lOut0.pMem;
   int8_t* lpK0      = (int8_t*)lK0.pMem;
   int8_t* lpK1      = (int8_t*)lK1.pMem;
   int8_t* lpK2      = (int8_t*)lK2.pMem;

   ht::apu_flt_centraldy_3x3(lpvOut0, lOut0.chunkSpan>>1,
                              lpvIn0, lIn0.chunkSpan,
                     lIn0.chunkWidth, lIn0.chunkHeight,
                             lpK0[0], lpK1[0], lpK2[0] );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for Hand-Tuned 3x3 Generic Operation (unsigned 8-bit).
***********************************************************************************/
void ht_generic_3x3_08u(kernel_io_desc lIn0, kernel_io_desc lFilterCoeff, kernel_io_desc lFilterScale, kernel_io_desc lOut0)
{
   vec08u* lpvIn0          = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0         = (vec08u*)lOut0.pMem;
   int8_t* lpFilterCoeff   = (int8_t*)lFilterCoeff.pMem;
   int8_t* lpFilterScale   = (int8_t*)lFilterScale.pMem;

   ht::apu_flt_generic_3x3(lpvOut0, lOut0.chunkSpan,
                            lpvIn0, lIn0.chunkSpan,
                   lIn0.chunkWidth, lIn0.chunkHeight,
                     lpFilterCoeff, lpFilterScale[0] );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for Hand-Tuned 5x5 Generic Operation (unsigned 8-bit).
***********************************************************************************/
void ht_generic_5x5_08u(kernel_io_desc lIn0, kernel_io_desc lFilterCoeff, kernel_io_desc lFilterScale, kernel_io_desc lOut0)
{
   vec08u* lpvIn0          = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0         = (vec08u*)lOut0.pMem;
   int8_t* lpFilterCoeff   = (int8_t*)lFilterCoeff.pMem;
   int8_t* lpFilterScale   = (int8_t*)lFilterScale.pMem;

   ht::apu_flt_generic_5x5(lpvOut0, lOut0.chunkSpan,
                            lpvIn0, lIn0.chunkSpan,
                   lIn0.chunkWidth, lIn0.chunkHeight,
                     lpFilterCoeff, lpFilterScale[0] );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for Hand-Tuned Saturate Operation (signed 8-bit).
***********************************************************************************/
void ht_saturate_08s(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec16s* lpvIn0  = (vec16s*)lIn0.pMem;
   vec08s* lpvOut0 = (vec08s*)lOut0.pMem;

   ht::apu_flt_saturate(lpvOut0, lOut0.chunkSpan,
                         lpvIn0, lIn0.chunkSpan>>1,
                lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for Hand-Tuned 3x3 Separable Filter Operation (signed 16-bit).
***********************************************************************************/
void ht_separable_3x3_16s(kernel_io_desc lIn0, kernel_io_desc lFilterRow, kernel_io_desc lFilterCol, kernel_io_desc lOut0)
{
   vec08u* lpvIn0       = (vec08u*)lIn0.pMem;
   vec16s* lpvOut0      = (vec16s*)lOut0.pMem;
   int8_t* lpFilterCol   = (int8_t*)lFilterCol.pMem;
   int8_t* lpFilterRow   = (int8_t*)lFilterRow.pMem;

   ht::apu_flt_separable_3x3(lpvOut0, lOut0.chunkSpan>>1,
                              lpvIn0, lIn0.chunkSpan,
                     lIn0.chunkWidth, lIn0.chunkHeight,
                          lpFilterRow, lpFilterCol );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for Hand-Tuned 5x5 Separable Filter Operation (signed 16-bit).
***********************************************************************************/
void ht_separable_5x5_16s(kernel_io_desc lIn0, kernel_io_desc lFilterRow, kernel_io_desc lFilterCol, kernel_io_desc lOut0)
{
   vec08u* lpvIn0       = (vec08u*)lIn0.pMem;
   vec16s* lpvOut0      = (vec16s*)lOut0.pMem;
   int8_t* lpFilterCol   = (int8_t*)lFilterCol.pMem;
   int8_t* lpFilterRow   = (int8_t*)lFilterRow.pMem;

   ht::apu_flt_separable_5x5(lpvOut0, lOut0.chunkSpan>>1,
                              lpvIn0, lIn0.chunkSpan,
                     lIn0.chunkWidth, lIn0.chunkHeight,
                          lpFilterRow, lpFilterCol );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for Hand-Tuned Sobel 3x3 Operation (signed 16-bit).
***********************************************************************************/
void ht_sobel_3x3_16s(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec16s* lpvOut0 = (vec16s*)lOut0.pMem;

   ht::apu_flt_sobel_3x3(lpvOut0, lOut0.chunkSpan>>1,
                          lpvIn0, lIn0.chunkSpan,
                 lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for Hand-Tuned Sobel 3x3 X Operation (signed 8-bit).
***********************************************************************************/
void ht_sobel_x_3x3_08s(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lOut1)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec16s* lpvOut0 = (vec16s*)lOut0.pMem;
   vec08s* lpvOut1 = (vec08s*)lOut1.pMem;

   ht::apu_flt_sobel_3x3_x(lpvOut1, lOut1.chunkSpan,
                           lpvOut0,
                            lpvIn0, lIn0.chunkSpan,
                   lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for Hand-Tuned Sobel 3x3 Y Operation (signed 8-bit).
***********************************************************************************/
void ht_sobel_y_3x3_08s(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lOut1)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec16s* lpvOut0 = (vec16s*)lOut0.pMem;
   vec08s* lpvOut1 = (vec08s*)lOut1.pMem;

   ht::apu_flt_sobel_3x3_y(lpvOut1, lOut1.chunkSpan,
                           lpvOut0,
                            lpvIn0, lIn0.chunkSpan,
                   lIn0.chunkWidth, lIn0.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for Hand-Tuned Sobel 3x3 X Operation (signed 16-bit).
***********************************************************************************/
void ht_sobel_x_3x3_16s(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec16s* lpvOut0 = (vec16s*)lOut0.pMem;

   ht::apu_flt_centraldx_3x3(lpvOut0, lOut0.chunkSpan>>1,
                              lpvIn0, lIn0.chunkSpan,
                     lIn0.chunkWidth, lIn0.chunkHeight,
                             1, 2, 1 );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for Hand-Tuned Sobel 3x3 Y Operation (signed 16-bit).
***********************************************************************************/
void ht_sobel_y_3x3_16s(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec16s* lpvOut0 = (vec16s*)lOut0.pMem;

   ht::apu_flt_centraldy_3x3(lpvOut0, lOut0.chunkSpan>>1,
                              lpvIn0, lIn0.chunkSpan,
                     lIn0.chunkWidth, lIn0.chunkHeight,
                             1, 2, 1 );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for Hand-Tuned Sobel 5x5 X Operation (signed 16-bit).
***********************************************************************************/
void ht_sobel_x_5x5_16s(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0        = (vec08u*)lIn0.pMem;
   vec16s* lpvOut0       = (vec16s*)lOut0.pMem;
   int8_t lpFilterRow[5] = {-1,-2, 0, 2, 1};
   int8_t lpFilterCol[5] = { 1, 4, 6, 4, 1};

   ht::apu_flt_separable_5x5(lpvOut0, lOut0.chunkSpan>>1,
                              lpvIn0, lIn0.chunkSpan,
                     lIn0.chunkWidth, lIn0.chunkHeight,
                          lpFilterRow, lpFilterCol );
}

/*!*********************************************************************************
*  \brief ACF wrapper function for Hand-Tuned Sobel 5x5 X Operation (signed 16-bit).
***********************************************************************************/
void ht_sobel_y_5x5_16s(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0        = (vec08u*)lIn0.pMem;
   vec16s* lpvOut0       = (vec16s*)lOut0.pMem;
   int8_t lpFilterRow[5] = { 1, 4, 6, 4, 1};
   int8_t lpFilterCol[5] = {-1,-2, 0, 2, 1};

   ht::apu_flt_separable_5x5(lpvOut0, lOut0.chunkSpan>>1,
                              lpvIn0, lIn0.chunkSpan,
                     lIn0.chunkWidth, lIn0.chunkHeight,
                          lpFilterRow, lpFilterCol );
}
   
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
