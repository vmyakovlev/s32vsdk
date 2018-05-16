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
* \file arithmetic.cpp
* \addtogroup apexcv-interpolation
* \addtogroup interpolation
* \ingroup interpolation
* @{
* \brief linear and bilinear interpolation operations mimicking opencv
*/

/*!*********************************************************************************
*  @file interpolation.c
*  @brief ACF metadata and wrapper function for interpolation kernels
***********************************************************************************/
 
#ifdef ACF_KERNEL_METADATA
#include "interpolation_acf.h"

/*!*********************************************************************************
*  \brief ACF metadata for linear interpolation grayscale (unsigned 8 bit)
*  \see U
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(INTERP_LIN_GRAYSC_K)
(
   INTERP_LIN_GRAYSC_KN,//interp_linear_grayscale
   3,
   __port(__index(0),
          __identifier(INPUT_0),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,1,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier(OUTPUT_0),
          __attributes(ACF_ATTR_VEC_OUT), 
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(2),
          __identifier(DELTA_X),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for bilinear interpolation grayscale (unsigned 8 bit)
*  \see U
***********************************************************************************/

KERNEL_INFO kernelInfoConcat(INTERP_BILIN_GRAYSC_K)
(
   INTERP_BILIN_GRAYSC_KN,
   3,
   __port(__index(0),
          __identifier(INPUT_0),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,1,0,1),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier(OUTPUT_0),
          __attributes(ACF_ATTR_VEC_OUT), 
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(2),
          __identifier(DELTA_XY),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(2, 1),
          __ek_size(1, 1))
);


KERNEL_INFO kernelInfoConcat(INTERP_BICUBIC_GRAYSC_K)
(
   INTERP_BICUBIC_GRAYSC_KN,
   4,
   __port(__index(0),
          __identifier(INPUT_0),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(2,2,2,2),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier(OUTPUT_0),
          __attributes(ACF_ATTR_VEC_OUT), 
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(2),
          __identifier(X_OFFSET),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(3),
          __identifier(Y_OFFSET),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "interpolation_apu.h"


/*!*********************************************************************************
*  \brief ACF wrapper function for linear grayscale interpolation (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, 
***********************************************************************************/                                 
void interp_linear_grayscale(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lDelta0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;
   vec08u* lpvDelta0 = (vec08u*)lDelta0.pMem;
   
   apu_interp_linear_grayscale(lpvOut0, lOut0.chunkSpan,
                                lpvIn0, lIn0.chunkSpan,
                       lIn0.chunkWidth, lIn0.chunkHeight,
                             lpvDelta0);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for bilinear grayscale interpolation (unsigned 8 bit)
*  \see UG-10267-03 ACF User Guide, 
***********************************************************************************/                                 
void interp_bilinear_grayscale(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lDelta0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;
   vec08u* lpvDelta0 = (vec08u*)lDelta0.pMem;
   
   apu_interp_bilinear_grayscale(lpvOut0, lOut0.chunkSpan,
                                 lpvIn0, lIn0.chunkSpan,
                        lIn0.chunkWidth, lIn0.chunkHeight,
                              lpvDelta0);
}

void interp_bicubic_grayscale(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc xin, kernel_io_desc yin)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;
   uint8_t* x_offset = (uint8_t*)xin.pMem; 
   uint8_t* y_offset = (uint8_t*)yin.pMem;

   apu_interp_bicubic_grayscale(lpvOut0, lOut0.chunkSpan,
                                lpvIn0, lIn0.chunkSpan,
                                lIn0.chunkWidth, lIn0.chunkHeight,
                                x_offset[0], y_offset[0]);
}


#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
/*! @} */
