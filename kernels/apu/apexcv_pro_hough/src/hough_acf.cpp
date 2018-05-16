/*********************************************************************************
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
 ******************************************************************************/

#include "cg_kernel.h"
#include "hough_config_acf.h"

/*******************************************************************************
 *  \file hough_acf.cpp
 *  \brief ACF metadata and wrapper function for Hough Transform
 ******************************************************************************/
#ifdef ACF_KERNEL_METADATA

/*******************************************************************************
 *  \brief ACF metadata for the Extract Edge Pixels (EEP) kernel
 *  \see UG-10267-03 ACF User Guide, Section 3.2.2
 ******************************************************************************/

KERNEL_INFO _kernel_info_hough_init_pixel_offset(
  "acf_hough_init_pixel_offset",
  3,
  __port(__index(0),
         __identifier("OFFSET_X"),
         __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(1),
         __identifier("OFFSET_Y"),
         __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(2),
         __identifier("PARAMS"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(4, 1)));

KERNEL_INFO _kernel_info_hough_init_cu_index(
  "acf_hough_init_cu_index",
  2,
  __port(__index(0),
         __identifier("CU_INDEX"),
         __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(1),
         __identifier("PARAMS"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(2, 1)));

KERNEL_INFO _kernel_info_hough_pixel_threshold(
  "acf_hough_pixel_threshold",
  7,
  __port(__index(0),
         __identifier("PIXELS_X"),
         __attributes(ACF_ATTR_VEC_OUT),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(1),
         __identifier("PIXELS_Y"),
         __attributes(ACF_ATTR_VEC_OUT),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(2),
         __identifier("PIXEL_COUNT"),
         __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(3),
         __identifier("OFFSET_X"),
         __attributes(ACF_ATTR_VEC_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(4),
         __identifier("OFFSET_Y"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(5),
         __identifier("IMAGE"),
         __attributes(ACF_ATTR_VEC_IN),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(6),
         __identifier("THRESHOLD"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(1, 1)));

KERNEL_INFO _kernel_info_hough_scalarize_pixels(
  "acf_hough_scalarize_pixels",
  7,
  __port(__index(0),
         __identifier("SCL_PIXELS_X"),
         __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(HOUGH_PIXEL_BUFFER_SIZE, 1)),
  __port(__index(1),
         __identifier("SCL_PIXELS_Y"),
         __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(HOUGH_PIXEL_BUFFER_SIZE, 1)),
  __port(__index(2),
         __identifier("SCL_PIXEL_COUNT"),
         __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d32s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(3),
         __identifier("VEC_PIXELS_X"),
         __attributes(ACF_ATTR_VEC_IN),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(4),
         __identifier("VEC_PIXELS_Y"),
         __attributes(ACF_ATTR_VEC_IN),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(5),
         __identifier("VEC_PIXEL_COUNT"),
         __attributes(ACF_ATTR_VEC_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(6),
         __identifier("CU_COUNT"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(1, 1)));

KERNEL_INFO _kernel_info_hough_accumulate(
  "acf_hough_accumulate",
  7,
  __port(__index(0),
         __identifier("ACCUMULATOR"),
         __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16u),
         __e0_size(1, 1),
         __ek_size(HOUGH_ACCUM_BUFFER_SIZE, 1)),
  __port(__index(1),
         __identifier("PIXELS_X"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(HOUGH_PIXEL_BUFFER_SIZE, 1)),
  __port(__index(2),
         __identifier("PIXELS_Y"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(HOUGH_PIXEL_BUFFER_SIZE, 1)),
  __port(__index(3),
         __identifier("PIXEL_COUNT"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d32s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(4),
         __identifier("COS_TABLE"),
         __attributes(ACF_ATTR_VEC_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(5),
         __identifier("SIN_TABLE"),
         __attributes(ACF_ATTR_VEC_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(6),
         __identifier("RHO_COUNT"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(1, 1)));

KERNEL_INFO _kernel_info_hough_get_lines(
  "acf_hough_get_lines",
  5,
  __port(__index(0),
         __identifier("LINES"),
         __attributes(ACF_ATTR_VEC_OUT_FIFO_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d32u),
         __e0_size(1, 1),
         __ek_size(HOUGH_MAX_LINES_PER_CU, 1)),
  __port(__index(1),
         __identifier("LINE_COUNT"),
         __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d32s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(2),
         __identifier("ACCUMULATOR"),
         __attributes(ACF_ATTR_VEC_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16u),
         __e0_size(1, 1),
         __ek_size(HOUGH_ACCUM_BUFFER_SIZE, 1)),
  __port(__index(3),
         __identifier("CU_INDEX"),
         __attributes(ACF_ATTR_VEC_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(4),
         __identifier("PARAMS"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(6, 1)));
#endif /* ACF_KERNEL_METADATA */

#ifdef ACF_KERNEL_IMPLEMENTATION
void acf_hough_init_pixel_offset(kernel_io_desc aOffsetX,
                                 kernel_io_desc aOffsetY,
                                 kernel_io_desc aConfig)
{
  if(ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
  {
    vec16s *pOffsetX      = (vec16s *)aOffsetX.pMem;
    int16s *pOffsetY      = (int16s *)aOffsetY.pMem;
    const int16s *pConfig = (const int16s *)aConfig.pMem;

    // aImage center y
    *pOffsetY = -pConfig[3];

    // pConfig[2] = aImage center x
    // pConfig[1] = block columns x
    // pConfig[0] = Nr of computing units
    *pOffsetX = apu_hough_get_offset_x(pConfig[2],
                                       pConfig[1],
                                       pConfig[0]);
  }
}

void acf_hough_init_cu_index(kernel_io_desc aCuIndex,
                             kernel_io_desc aConfig)
{
  if(ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
  {
    vec16s *pCuIndex      = (vec16s *)aCuIndex.pMem;
    const int16s *pConfig = (const int16s *)aConfig.pMem;

    // pConfig[0] = Start index
    // pConfig[1] = Nr of computing units
    *pCuIndex = apu_hough_get_cu_index(pConfig[0],
                                       pConfig[1]);
  }
}

/*******************************************************************************
 *  \brief ACF wrapper function for the Hough Transform (Extract Edge Pixels)
 *  \see UG-10267-03 ACF User Guide, Section 3.2.3
 ******************************************************************************/
void acf_hough_pixel_threshold(kernel_io_desc aPixelsX,
                               kernel_io_desc aPixelsY,
                               kernel_io_desc aPixelCount,
                               kernel_io_desc aOffsetX,
                               kernel_io_desc aOffsetY,
                               kernel_io_desc aImage,
                               kernel_io_desc aThreshold)
{
  vec16s *pPixelsX       = (vec16s *)aPixelsX.pMem;
  vec16s *pPixelsY       = (vec16s *)aPixelsY.pMem;
  vec16s *pPixelCount    = (vec16s *)aPixelCount.pMem;
  int16s *pOffsetY       = (int16s *)aOffsetY.pMem;
  const vec16s *pOffsetX = (const vec16s *)aOffsetX.pMem;
  const vec08u *pImage   = (const vec08u *)aImage.pMem;
  const int *pThreshold  = (const int *)aThreshold.pMem;
  *pPixelCount = apu_hough_pixel_threshold(pPixelsX,
                                           pPixelsY,
                                           *pOffsetX,
                                           *pOffsetY,
                                           pImage,
                                           aImage.chunkSpan,
                                           aImage.chunkWidth,
                                           aImage.chunkHeight,
                                           *pThreshold);

  (*pOffsetY) += aImage.chunkHeight;
}

void acf_hough_scalarize_pixels(kernel_io_desc aScaPixelsX,
                                kernel_io_desc aScaPixelsY,
                                kernel_io_desc aScaPixelCount,
                                kernel_io_desc aVecPixelsX,
                                kernel_io_desc aVecPixelsY,
                                kernel_io_desc aVecPixelCount,
                                kernel_io_desc aCuCount)
{
  int16s *pScaPixelsX          = (int16s *)aScaPixelsX.pMem;
  int16s *pScaPixelsY          = (int16s *)aScaPixelsY.pMem;
  int32s *pScaPixelCount       = (int32s *)aScaPixelCount.pMem;
  const vec16s *pVecPixelsX    = (const vec16s *)aVecPixelsX.pMem;
  const vec16s *pVecPixelsY    = (const vec16s *)aVecPixelsY.pMem;
  const vec16s *pVecPixelCount = (const vec16s *)aVecPixelCount.pMem;
  const int16s *pCuCount       = (const int16s *)aCuCount.pMem;
  *pScaPixelCount = apu_hough_scalarize_pixels(pScaPixelsX,
                                               pScaPixelsY,
                                               pVecPixelsX,
                                               pVecPixelsY,
                                               *pVecPixelCount,
                                               *pCuCount);
}

/*******************************************************************************
 *  \brief ACF wrapper function for the Hough Transform (voting and get_lines)
 *  \see UG-10267-03 ACF User Guide, Section 3.2.3
 ******************************************************************************/
void acf_hough_accumulate(kernel_io_desc aAccumulator,
                          kernel_io_desc aPixelsX,
                          kernel_io_desc aPixelsY,
                          kernel_io_desc aPixelCount,
                          kernel_io_desc aTabCos,
                          kernel_io_desc aTabSin,
                          kernel_io_desc aRhoCount)
{
#if (__llvm__)
  //workaround for vmemset issue
  vec16u *volatile pAccumulator = (vec16u *)aAccumulator.pMem;
#else (__chess__)
  vec16u *pAccumulator = (vec16u *)aAccumulator.pMem;
#endif
  const int16s *pPixelsX    = (const int16s *)aPixelsX.pMem;
  const int16s *pPixelsY    = (const int16s *)aPixelsY.pMem;
  const int32s *pPixelCount = (int32s *)aPixelCount.pMem;
  const vec16s *pTabCos     = (const vec16s *)aTabCos.pMem;
  const vec16s *pTabSin     = (const vec16s *)aTabSin.pMem;
  const int rhoCount        = *((const int16s *)aRhoCount.pMem);

  if(ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
  {
    // Zero the aAccumulator
    // Adding 2 for padding
    for(int r = 0, rEnd = rhoCount + 2; r < rEnd; r++)
    {
      pAccumulator[r] = 0;
    }
  }

  // Offset due to padding
  apu_hough_accumulate(pAccumulator + 1,
                       pPixelsX,
                       pPixelsY,
                       *pPixelCount,
                       *pTabCos,
                       *pTabSin,
                       (rhoCount >> 1));
}

/*******************************************************************************
 *  \brief ACF wrapper function for the Hough Transform (voting and get_lines)
 *  \see UG-10267-03 ACF User Guide, Section 3.2.3
 ******************************************************************************/
void acf_hough_get_lines(kernel_io_desc aLines,
                         kernel_io_desc aLineCount,
                         kernel_io_desc aAccumulator,
                         kernel_io_desc aCuIndex,
                         kernel_io_desc aConfig)
{
  if(ACF_RET_VAR(ACF_VAR_LAST_TILE_FLAG))
  {
    vec32u *pLines             = (vec32u *)aLines.pMem;
    const vec16u *pAccumulator = (const vec16u *)aAccumulator.pMem + 1;
    const vec16s *pCuIndex     = (const vec16s *)aCuIndex.pMem;
    const int16s *pConfig      = (const int16s *)aConfig.pMem;
    const vec16s vLineCount    = apu_hough_get_lines(pLines,
                                                     pAccumulator,
                                                     *pCuIndex,
                                                     pConfig[0] /* rho */,
                                                     pConfig[1] /* theta */,
                                                     pConfig[2] /* acc thr */,
                                                     pConfig[3] /* maxLinesCU */,
                                                     pConfig[4] /* NMS flag */,
                                                     pConfig[5] /* CU count */);

    ACF_PUSH_VEC_BUFFER_TO_FIFO(aLines, (vec16u)vLineCount * 4, aLineCount);
  }
}

#endif /* ACF_KERNEL_IMPLEMENTATION */
