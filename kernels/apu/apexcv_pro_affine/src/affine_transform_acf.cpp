
/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2014, Digital Media Professionals
* Copyright (c) 2016-2017 NXP
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

/*!*********************************************************************************
*  \file affine_transform_apu.cpp
*  \brief ACF Affine Transform Wrapper.
***********************************************************************************/
/*!*********************************************************************************
*  @file
*  @brief ACF metadata and wrapper function for the \ref secAffineTransform "affine transformation".
***********************************************************************************/
#ifdef ACF_KERNEL_METADATA

/*!*********************************************************************************
*  \brief ACF metadata for the bilinear interpolation.
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernel_info_affine_bilinear_interpolate
(
   "affine_bilinear_interpolate",
   5,
   __port(__index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)), 
   __port(__index(1),
      __identifier("MATRIX"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(6, 1),
      __ek_size(1, 1)),
   __port(__index(2),
      __identifier("IMAGE_WIDTH"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   __port(__index(3),
      __identifier("IMAGE_HEIGHT"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   __port(__index(4),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1))
);

#endif // ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "affine_transform_apu.h"

/*!*********************************************************************************
*  \brief ACF wrapper function for the bilinear interpolation.
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void affine_bilinear_interpolate(
               kernel_io_desc lIn            /*!< Input source image.*/,
               kernel_io_desc lMat           /*!< Input affine inverse matrix.*/,
               kernel_io_desc lImageWidth    /*!< Input image width.*/,
               kernel_io_desc lImageHeight   /*!< Input image height.*/,
               kernel_io_desc lOut           /*!< Output buffer.*/
)
{
   vec08u *lpvIn           = (vec08u*)lIn.pMem;
   int32s *lpMat           = (int32s*)lMat.pMem;
   int16s lpImageWidth     = *(int16s*)lImageWidth.pMem;
   int16s lpImageHeight    = *(int16s*)lImageHeight.pMem;
   vec08u *lpvOut          = (vec08u*)lOut.pMem;
   
   if (ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG)) {
      acf_affine_init_cu_offsets(lpMat, lOut.chunkWidth, lOut.chunkHeight, ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS));
   }
   
   acf_affine_bilinear_interpolation(lpvIn, lIn.chunkSpan, lIn.chunkWidth, lIn.chunkHeight, lpMat, lpvOut, lOut.chunkSpan, lOut.chunkWidth, lOut.chunkHeight, lpImageWidth, lpImageHeight);
}

#endif // ACF_KERNEL_IMPLEMENTATION


