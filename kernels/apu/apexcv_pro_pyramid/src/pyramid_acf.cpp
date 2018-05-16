/**************************************************************************************************
 * 
 * NXP Confidential Proprietary
 *
 * Copyright (c) 2014, Digital Media Professionals. All rights reserved.
 * Copyright 2017 NXP 
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
 **************************************************************************************************/
/*!*********************************************************************************
*  @file
*  @brief ACF metadata and wrapper function for the \ref secImagePyramid "image pyramid creation".
***********************************************************************************/
#ifdef ACF_KERNEL_METADATA

/*!*********************************************************************************
*  \brief ACF metadata for the 5x1 horizontal Gaussian operation.
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernel_info_horizontal_gaus
(
   "horizontal_gaus",
   2,
   __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(2, 2, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   
   __port(  __index(1),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 5x1 horizontal Gaussian and expand operation.
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernel_info_horizontal_gaus_and_expand
(
   "horizontal_gaus_and_expand",
   2,
   __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(2, 2, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   
   __port(  __index(1),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(2, 2))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 1x5 vertical Gaussian operation.
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernel_info_vertical_gaus
(
   "vertical_gaus",
   2,
   __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 2, 2),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   
   __port(  __index(1),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 1x5 vertical Gaussian and reduce operation.
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernel_info_vertical_gaus_and_reduce
(
   "vertical_gaus_and_reduce",
   2,
   __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 2, 2),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(2, 2)),
   
   __port(  __index(1),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1))
);

#endif /* ACF_KERNEL_METADATA */

#ifdef ACF_KERNEL_IMPLEMENTATION
#include "pyramid_apu.h"

/*!*********************************************************************************
*  \brief ACF wrapper function for the 5x1 horizontal Gaussian operation.
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void horizontal_gaus(kernel_io_desc lIn      /*!< The padded source image.*/,
   kernel_io_desc lOut                 /*!< The destination image.*/
)
{
   vec08u* lpvIn = (vec08u*)lIn.pMem;
   vec16s* lpvOut = (vec16s*)lOut.pMem;
   
   apu_pyr_horizontal_gaus(lpvIn, lIn.chunkSpan, lpvOut, lOut.chunkSpan/2, lIn.chunkWidth, lIn.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for the 5x1 horizontal Gaussian and expand operation.
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void horizontal_gaus_and_expand(kernel_io_desc lIn    /*!< The padded source image.*/,
   kernel_io_desc lOut                          /*!< The destination image.*/
)
{
   vec08u* lpvIn = (vec08u*)lIn.pMem;
   vec16s* lpvOut = (vec16s*)lOut.pMem;
   
   apu_pyr_horizontal_gaus_and_expand(lpvIn, lIn.chunkSpan, lpvOut, lOut.chunkSpan/2, lIn.chunkWidth, lIn.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for the 1x5 vertical Gaussian operation.
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void vertical_gaus(kernel_io_desc lIn     /*!< The padded source image.*/,
   kernel_io_desc lOut                 /*!< The destination image.*/
)
{
   vec16s* lpvIn = (vec16s*)lIn.pMem;
   vec08u* lpvOut = (vec08u*)lOut.pMem;
   
   apu_pyr_vertical_gaus(lpvIn, lIn.chunkSpan/2, lpvOut, lOut.chunkSpan, lIn.chunkWidth, lIn.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for the 1x5 vertical Gaussian and reduce operation.
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void vertical_gaus_and_reduce(kernel_io_desc lIn      /*!< The padded source image.*/,
   kernel_io_desc lOut                          /*!< The destination image.*/
)
{
   vec16s* lpvIn = (vec16s*)lIn.pMem;
   vec08u* lpvOut = (vec08u*)lOut.pMem;
   
   apu_pyr_vertical_gaus_and_reduce(lpvIn, lIn.chunkSpan/2, lpvOut, lOut.chunkSpan, lIn.chunkWidth, lIn.chunkHeight);
}

#endif /* ACF_KERNEL_IMPLEMENTATION */
