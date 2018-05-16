/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
* Copyright (c) 2017 NXP
*  All Rights Reserved
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
* \file haar_cascade_acf.cpp
* \addtogroup object_detection Object detection
* \addtogroup haar Haar cascade object detection
* \ingroup object_detection
* @{
* \brief Haar cascade object detection
*/
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#include "cascade_definitions.h"

#ifdef ACF_KERNEL_METADATA

#include "haar_cascade_acf.h"


/****************************************************************************/
/*!
   Haar cascade object detection kernel metadata.
   
   \param apu_haar_cascade                  Kernel name
   \param 11                                Number of ports
   \param "Port HAAR_CASC_IN_INTEGR_IMG"	Define for name of input integral image (unsigned 32bit)
   \param "Port HAAR_CASC_IN_INTEGR_IMG_SQR" Define for name of input integral image of squared values (unsigned 32bit)
   \param "Port HAAR_CASC_LINE_IDX"         Define for name of line index buffer (unsigned 16bit)
   \param "Port HAAR_CASC_WND_BUF"          Define for name of integral image window buffer (unsigned 32bit)
   \param "Port HAAR_CASC_WND_BUF_SQR"      Define for name of integral image of squared values window buffer (unsigned 32bit)
   \param "Port HAAR_CASC_IN_CASC_SZ"       Define for name of number of features and stages (2 16-bit values) (unsigned 16bit)
   \param "Port HAAR_CASC_IN_CASC_FEAT"     Define for name of array of Haar-like features (unsigned 8bit)
   \param "Port HAAR_CASC_IN_CASC_STAGES"   Define for name of array of cascade stages (unsigned 8bit)
   \param "Port HAAR_CASC_IN_PIX_SHFT"      Define for name of LUT containing tile shifts needed for horizontal pixel offsets (unsigned 8bit)
   \param "Port HAAR_CASC_IN_PIX_OFFSETS"   Define for name of LUT containing in-tile offsets needed for horizontal pixel offsets (unsigned 8bit)
   \param "Port HAAR_CASC_OUT"              Define for name of output image (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(HAAR_CASC_K)
(
HAAR_CASC_KN,
11,
__port(__index(0),
  __identifier(HAAR_CASC_IN_INTEGR_IMG),
  __attributes(ACF_ATTR_VEC_IN),
  __spatial_dep(1, 0, 1, 0),
  __e0_data_type(d32u),
  __e0_size(1, 1),
  __ek_size(1, 1)),
__port(__index(1),
  __identifier(HAAR_CASC_IN_INTEGR_IMG_SQR),
  __attributes(ACF_ATTR_VEC_IN),
  __spatial_dep(1, 0, 1, 0),
  __e0_data_type(d32u),
  __e0_size(1, 1),
  __ek_size(1, 1)),
__port(__index(2),
  __identifier(HAAR_CASC_LINE_IDX),
  __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
  __spatial_dep(0, 0, 0, 0),
  __e0_data_type(d16u),
  __e0_size(1, 1),
  __ek_size(1, 1)),
__port(__index(3),
  __identifier(HAAR_CASC_WND_BUF),
  __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
  __spatial_dep(0, 0, 0, 0),
  __e0_data_type(d32u),
  __e0_size(1, 1),
  __ek_size(MAX_WINDOW_WIDTH_HAAR + 1, 32)),
__port(__index(4),
  __identifier(HAAR_CASC_WND_BUF_SQR),
  __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
  __spatial_dep(0, 0, 0, 0),
  __e0_data_type(d32u),
  __e0_size(1, 1),
  __ek_size(MAX_WINDOW_WIDTH_HAAR + 1, 32)),
__port(__index(5),
  __identifier(HAAR_CASC_IN_CASC_SZ),
  __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
  __spatial_dep(0, 0, 0, 0),
  __e0_data_type(d16u),
  __e0_size(1, 1),
  __ek_size(2, 1)),
__port(__index(6),
  __identifier(HAAR_CASC_IN_CASC_FEAT),
  __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
  __spatial_dep(0, 0, 0, 0),
  __e0_data_type(d08u),
  __e0_size(1, 1),
  __ek_size(19800, 1)),
__port(__index(7),
  __identifier(HAAR_CASC_IN_CASC_STAGES),
  __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
  __spatial_dep(0, 0, 0, 0),
  __e0_data_type(d08u),
  __e0_size(1, 1),
  __ek_size(70, 1)),
__port(__index(8),
  __identifier(HAAR_CASC_IN_PIX_SHFT),
  __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
  __spatial_dep(0, 0, 0, 0),
  __e0_data_type(d08u),
  __e0_size(1, 1),
  __ek_size(32, 1)),
__port(__index(9),
  __identifier(HAAR_CASC_IN_PIX_OFFS),
  __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
  __spatial_dep(0, 0, 0, 0),
  __e0_data_type(d08u),
  __e0_size(1, 1),
  __ek_size(32, 1)),
__port(__index(10),
  __identifier(HAAR_CASC_OUT),
  __attributes(ACF_ATTR_VEC_OUT),
  __spatial_dep(0, 0, 0, 0),
  __e0_data_type(d08u),
  __e0_size(1, 1),
  __ek_size(1, 1))
);
#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "haar_cascade_apu.h"

void apu_haar_cascade(
  kernel_io_desc lInII,
  kernel_io_desc lInII2,
  kernel_io_desc lLineIndex,
  kernel_io_desc lWindowBuffer,
  kernel_io_desc lWindowBuffer2,
  kernel_io_desc lInCSizes,
  kernel_io_desc lInCFeatures,
  kernel_io_desc lInCstages,
  kernel_io_desc lInPixelShifts,
  kernel_io_desc lInPixelOffsets,
  kernel_io_desc lOut)
{
  vec32u* lpvInII = (vec32u*)lInII.pMem;
  vec32u* lpvInII2 = (vec32u*)lInII2.pMem;
  vec08u* lpvOut = (vec08u*)lOut.pMem;

  int16u* sizes = (int16u*)lInCSizes.pMem;
  int stageCount = (int)sizes[1];

  int16u* pIndex = (int16u*)lLineIndex.pMem;
  int16u lineIndex = *pIndex;
  if (ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
  {
    lineIndex = 0;
  }
  *pIndex = lineIndex + 1;
  vec32u* lineBuffer = (vec32u*)lWindowBuffer.pMem;
  vec32u* lineBuffer2 = (vec32u*)lWindowBuffer2.pMem;

  const int08u* xShifts = (int08u*)lInPixelShifts.pMem;
  const int08u* xOffsets = (int08u*)lInPixelOffsets.pMem;

  haar_cascade(lpvOut, lpvInII, lpvInII2, lOut.chunkSpan, lInII.chunkSpan, lOut.chunkWidth, lOut.chunkHeight,
    lineIndex, lineBuffer, lineBuffer2,
    (APEX_HaarCascadeFeature*)lInCFeatures.pMem,
    stageCount, (APEX_HaarCascadeStage*)lInCstages.pMem,
    xShifts, xOffsets);
}

#endif

/*! @} */
