/**************************************************************************************************
 * 
 * NXP Confidential Proprietary
 *
 * Copyright 2016 NXP 
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

/*==================================================================================================
Revision History:
                              Modification     Tracking
Author (core ID)              Date D/M/Y       Number     Description of Changes
---------------------------   ----------    ------------  ------------------------------------------
Adrian Grigore (nxa11567)     13/12/2016    VSDK-554      Created
---------------------------   ----------    ------------  ------------------------------------------
==================================================================================================*/

/*!*********************************************************************************
*  @file laplacian_acf.cpp
*  @brief ACF metadata and wrapper function for the Laplacian Pyramid
***********************************************************************************/
#ifdef ACF_KERNEL_METADATA

/*!*********************************************************************************
*  \brief ACF metadata for the 5x1 horizontal Gaussian operation.
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernel_info_horizontal_gaus_laplacian_mid
(
   "horizontal_gaus_laplacian_mid",
   2,
   __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(2, 2, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(2, 2)),
   
   __port(  __index(1),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(2, 2))
);


KERNEL_INFO kernel_info_horizontal_gaus_laplacian_last
(
   "horizontal_gaus_laplacian_last",
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
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1))
);


/*!*********************************************************************************
*  \brief ACF metadata for the 1x5 vertical Gaussian and Laplacian output mid level.
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernel_info_vertical_gaus_laplacian_mid
(
   "vertical_gaus_laplacian_mid",
   4,
   __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(2, 2)),
      
   __port(  __index(1),
      __identifier("INPUT_GAUSS"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 2, 2),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(2, 2)),
      
   __port(  __index(2),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(2, 2)),
   
   __port(  __index(3),
      __identifier("OUTPUT_NEXT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the 1x5 vertical Gaussian and Laplacian output last level.
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernel_info_vertical_gaus_laplacian_last
(
   "vertical_gaus_laplacian_last",
   4,
   __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
      
   __port(  __index(1),
      __identifier("INPUT_GAUSS"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 2, 2),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
      
   __port(  __index(2),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   
   __port(  __index(3),
      __identifier("OUTPUT_REC"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(1, 1))
);

#endif /* ACF_KERNEL_METADATA */

#ifdef ACF_KERNEL_IMPLEMENTATION
#include "laplacian_apu.h"

/*!*********************************************************************************
*  \brief ACF wrapper function for the 5x1 horizontal Gaussian operation.
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void horizontal_gaus_laplacian_mid(kernel_io_desc lIn      /*!< The padded source image.*/,
   kernel_io_desc lOut                 /*!< The destination image.*/
)
{
   vec08u* lpvIn = (vec08u*)lIn.pMem;
   vec16u* lpvOut = (vec16u*)lOut.pMem;
   
   apu_pyr_horizontal_gaus_laplacian(lpvIn, lIn.chunkSpan,
                                     lpvOut, lOut.chunkSpan/2,
                                     lIn.chunkWidth, lIn.chunkHeight);
}
void horizontal_gaus_laplacian_last(kernel_io_desc lIn      /*!< The padded source image.*/,
   kernel_io_desc lOut                 /*!< The destination image.*/
)
{
   vec08u* lpvIn = (vec08u*)lIn.pMem;
   vec16u* lpvOut = (vec16u*)lOut.pMem;
   
   apu_pyr_horizontal_gaus_laplacian(lpvIn, lIn.chunkSpan,
                                     lpvOut, lOut.chunkSpan/2,
                                     lIn.chunkWidth, lIn.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for the 1x5 vertical Gaussian and Laplacian output mid level.
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void vertical_gaus_laplacian_mid(kernel_io_desc lIn   /*!< Ihe source image*/,
   kernel_io_desc lIn_gaus                /*!< The padded image from horizontal gauss.*/,
   kernel_io_desc lOut_lap                /*!< The destination image - laplacian data.*/,
   kernel_io_desc lOut_next               /*!< The image for next level input.*/
)
{
   vec08u* lpvIn       = (vec08u*)lIn.pMem;
   vec16u* lpvIn_gaus  = (vec16u*)lIn_gaus.pMem;
   vec16s* lpvOut_lap  = (vec16s*)lOut_lap.pMem;
   vec08u* lpvOut_next = (vec08u*)lOut_next.pMem;
   
   apu_pyr_vertical_gaus_laplacian_mid(lpvIn, lIn.chunkSpan,
                                       lpvIn_gaus, lIn_gaus.chunkSpan/2,
                                       lpvOut_lap, lOut_lap.chunkSpan/2,
                                       lpvOut_next, lOut_next.chunkSpan,
                                       lIn.chunkWidth, lIn.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for the 1x5 vertical Gaussian and Laplacian output last level.
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void vertical_gaus_laplacian_last(kernel_io_desc lIn   /*!< Ihe source image*/,
   kernel_io_desc lIn_gaus                /*!< The padded image from horizontal gauss.*/,
   kernel_io_desc lOut_lap                /*!< The destination image - laplacian data.*/,
   kernel_io_desc lOut_rec                /*!< The image for Laplacian reconstruction.*/
)
{
   vec08u* lpvIn      = (vec08u*)lIn.pMem;
   vec16u* lpvIn_gaus = (vec16u*)lIn_gaus.pMem;
   vec16s* lpvOut_lap = (vec16s*)lOut_lap.pMem;
   vec16s* lpvOut_rec = (vec16s*)lOut_rec.pMem;
   
   apu_pyr_vertical_gaus_laplacian_last(lpvIn, lIn.chunkSpan,
                                        lpvIn_gaus, lIn_gaus.chunkSpan/2,
                                        lpvOut_lap, lOut_lap.chunkSpan/2,
                                        lpvOut_rec, lOut_rec.chunkSpan/2,
                                        lIn.chunkWidth, lIn.chunkHeight);
}

#endif /* ACF_KERNEL_IMPLEMENTATION */
