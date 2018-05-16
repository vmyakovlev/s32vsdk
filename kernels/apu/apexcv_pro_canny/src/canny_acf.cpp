/**************************************************************************************************
 * 
 * NXP Confidential Proprietary
 *
 * Copyright 2014-2017 NXP 
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
*  \file canny_acf.cpp
*  \brief ACF Metadata and wrapper function for Canny edge detector
***********************************************************************************/

#ifdef ACF_KERNEL_METADATA

/*!*********************************************************************************
*  \brief ACF metadata for the non-maxima suppression kernel
***********************************************************************************/
KERNEL_INFO kernel_info_canny_non_maxima_suppress
(
   "canny_non_maxima_suppress",
   4,
   __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(2, 2, 2, 2),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(2, 1)),

   __port(  __index(1),
      __identifier("LOW_THRESH"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

   __port(  __index(2),
      __identifier("HIGH_THRESH"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

   __port(  __index(3),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(2, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the non-maxima suppression & edge promotion kernel
***********************************************************************************/
KERNEL_INFO kernel_info_canny_nms_promote
(
   "canny_nms_promote",
   4,
   __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(2, 2, 2, 2),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(2, 1)),

   __port(  __index(1),
      __identifier("LOW_THRESH"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

   __port(  __index(2),
      __identifier("HIGH_THRESH"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

   __port(  __index(3),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(2, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the edge promotion kernel
***********************************************************************************/
KERNEL_INFO kernel_info_canny_promote_edges
(
   "canny_promote_edges",
   2,
   __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(1, 1, 1, 1),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(4, 1)),

   __port(  __index(1),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(4, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the internal edge promotion kernel
***********************************************************************************/
KERNEL_INFO kernel_info_canny_promote_edges_full
(
   "canny_promote_edges_full",
   2,
   __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(1, 1, 1, 1),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(4, 1)),

   __port(  __index(1),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(4, 1))
);

/*!*********************************************************************************
*  \brief ACF metadata for the create image kernel
***********************************************************************************/
KERNEL_INFO kernel_info_canny_create_image
(
   "canny_create_image",
   2,
   __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(2, 2)),

   __port(  __index(1),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(2, 2))
);

#endif /* ACF_KERNEL_METADATA */

#ifdef ACF_KERNEL_IMPLEMENTATION
#include "canny_apu.h"

/*!*********************************************************************************
*  \brief ACF wrapper function for the non-maxima suppression kernel
***********************************************************************************/
void canny_non_maxima_suppress(
               kernel_io_desc lIn,
               kernel_io_desc lInLowThresh,
               kernel_io_desc lInHighThresh,
               kernel_io_desc lOut
)
{
   vec08u* lpvIn = (vec08u*)lIn.pMem;
   int16u* lpvInLowThresh = (int16u*)lInLowThresh.pMem;
   int16u* lpvInHighThresh = (int16u*)lInHighThresh.pMem;
   vec08u* lpvOut = (vec08u*)lOut.pMem;
   
   apu_canny_suppress(lpvOut, lOut.chunkSpan,
                      lpvIn,  lIn.chunkSpan,
                      lIn.chunkWidth, lIn.chunkHeight,
                      lpvInLowThresh[0], lpvInHighThresh[0]);
}
/*!*********************************************************************************
*  \brief ACF wrapper function for the non-maxima suppression & edge promotion kernel
***********************************************************************************/
void canny_nms_promote(
               kernel_io_desc lIn,
               kernel_io_desc lInLowThresh,
               kernel_io_desc lInHighThresh,
               kernel_io_desc lOut
)
{
   vec08u* lpvIn = (vec08u*)lIn.pMem;
   int16u* lpvInLowThresh = (int16u*)lInLowThresh.pMem;
   int16u* lpvInHighThresh = (int16u*)lInHighThresh.pMem;
   vec08u* lpvOut = (vec08u*)lOut.pMem;
   
   apu_canny_suppress_promote(lpvOut, lOut.chunkSpan,
                              lpvIn,  lIn.chunkSpan,
                              lIn.chunkWidth, lIn.chunkHeight,
                              lpvInLowThresh[0], lpvInHighThresh[0]);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for the edge promotion kernel
***********************************************************************************/
void canny_promote_edges(
               kernel_io_desc lIn,
               kernel_io_desc lOut
)
{
   vec08u* lpvIn = (vec08u*)lIn.pMem;
   vec08u* lpvOut = (vec08u*)lOut.pMem;
   
   apu_canny_connect_edges(lpvOut, lOut.chunkSpan, lpvIn, lIn.chunkSpan, lIn.chunkWidth, lIn.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for the internal edge promotion kernel
***********************************************************************************/
void canny_promote_edges_full(
               kernel_io_desc lIn,
               kernel_io_desc lOut
)
{
   vec08u* lpvIn = (vec08u*)lIn.pMem;
   vec08u* lpvOut = (vec08u*)lOut.pMem;
   
   apu_canny_connect_edges_full(lpvOut, lOut.chunkSpan, lpvIn, lIn.chunkSpan, lIn.chunkWidth, lIn.chunkHeight);
}

/*!*********************************************************************************
*  \brief ACF wrapper function for the create image kernel
***********************************************************************************/
void canny_create_image(
               kernel_io_desc lIn,
               kernel_io_desc lOut
)
{
   vec08u* lpvIn = (vec08u*)lIn.pMem;
   vec08u* lpvOut = (vec08u*)lOut.pMem;

   apu_canny_create_image(lpvOut, lOut.chunkSpan, lpvIn, lIn.chunkSpan, lIn.chunkWidth, lIn.chunkHeight);
}

#endif /* ACF_KERNEL_IMPLEMENTATION */
