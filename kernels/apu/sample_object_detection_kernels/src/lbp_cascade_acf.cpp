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
* \file lbp_cascade_acf.cpp
* \addtogroup object_detection Object detection
* \addtogroup lbp LBP cascade object detection
* \ingroup object_detection
* @{
* \brief LBP cascade object detection
*/
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#include "cascade_definitions.h"

#ifdef ACF_KERNEL_METADATA

#include "lbp_cascade_acf.h"

/****************************************************************************/
/*!
   LBP cascade object detection kernel metadata.
   
   \param LBP_CASC_KN		                  Define for Kernel name
   \param 9                                   Number of ports
   \param "Port LBP_CASC_IN_INTEGR_IMG"       Define for name of input integral image (unsigned 32bit)
   \param "Port LBP_CASC_LINE_IDX"            Define for name of line index buffer (unsigned 16bit)
   \param "Port LBP_CASC_WND_BUF"             Define for name of integral image window buffer (unsigned 32bit)
   \param "Port LBP_CASC_IN_CASC_SZ_AND_SKIP" Define for name of number of features and stages + whetherthe odd rows and columns should be skipped (3 16-bit values) (unsigned 16bit)
   \param "Port LBP_CASC_IN_CASC_FEAT"        Define for name of array of Haar-like features (unsigned 8bit)
   \param "Port LBP_CASC_IN_CASC_STAGES"      Define for name of array of cascade stages (unsigned 8bit)
   \param "Port LBP_CASC_IN_PIX_SHFT"         Define for name of LUT containing tile shifts needed for horizontal pixel offsets (unsigned 8bit)
   \param "Port LBP_CASC_IN_PIX_OFFS"         Define for name of LUT containing in-tile offsets needed for horizontal pixel offsets (unsigned 8bit)
   \param "Port LBP_CASC_OUT"                 Define for name of output image (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(LBP_CASC_K)
(
	LBP_CASC_KN,
	9,
	__port(__index(0),
		__identifier(LBP_CASC_IN_INTEGR_IMG),
		__attributes(ACF_ATTR_VEC_IN),
		__spatial_dep(1, 0, 1, 0),
		__e0_data_type(d32u),
		__e0_size(1, 1),
		__ek_size(1, 1)),
	__port(__index(1),
		__identifier(LBP_CASC_LINE_IDX),
		__attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d16u),
		__e0_size(1, 1),
		__ek_size(1, 1)),
	__port(__index(2),
		__identifier(LBP_CASC_WND_BUF),
		__attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d32u),
		__e0_size(1, 1),
		__ek_size(MAX_WINDOW_WIDTH_LBP + 1, 32)),
	__port(__index(3),
		__identifier(LBP_CASC_IN_CASC_SZ_AND_SKIP),
		__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d16u),
		__e0_size(1, 1),
		__ek_size(3, 1)),
	__port(__index(4),
		__identifier(LBP_CASC_IN_CASC_FEAT),
		__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d08u),
		__e0_size(1, 1),
		__ek_size(10000, 1)),
		//__ek_size(4096, 1)),
	__port(__index(5),
		__identifier(LBP_CASC_IN_CASC_STAGES),
		__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d08u),
		__e0_size(1, 1),
		__ek_size(200, 1)),
		//__ek_size(64, 1)),
	__port(__index(6),
		__identifier(LBP_CASC_IN_PIX_SHFT),
		__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d08u),
		__e0_size(1, 1),
		__ek_size(64, 1)),
	__port(__index(7),
		__identifier(LBP_CASC_IN_PIX_OFFS),
		__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d08u),
		__e0_size(1, 1),
		__ek_size(64, 1)),
	__port(__index(8),
		__identifier(LBP_CASC_OUT),
		__attributes(ACF_ATTR_VEC_OUT),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d08u),
		__e0_size(1, 1),
		__ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "lbp_cascade_apu.h"

void apu_lbp_cascade(
  kernel_io_desc lInII,
  kernel_io_desc lLineIndex,
  kernel_io_desc lWindowBuffer,
  kernel_io_desc lInCSizesAndSkip,
  kernel_io_desc lInCFeatures,
  kernel_io_desc lInCstages,
  kernel_io_desc lInPixelShifts,
  kernel_io_desc lInPixelOffsets,
  kernel_io_desc lOut)
{

  vec32u* lpvInII = (vec32u*)lInII.pMem;
  vec08u* lpvOut = (vec08u*)lOut.pMem;

  int16u* sizesAndSkip = (int16u*)lInCSizesAndSkip.pMem;
  int stageCount = (int)sizesAndSkip[1];

  int16u* pIndex = (int16u*)lLineIndex.pMem;
  int16u lineIndex = *pIndex;
  if (ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
  {
    lineIndex = 0;
  }
  *pIndex = lineIndex + 1;
  vec32u* lineBuffer = (vec32u*)lWindowBuffer.pMem;

  const int08u* xShifts = (int08u*)lInPixelShifts.pMem;
  const int08u* xOffsets = (int08u*)lInPixelOffsets.pMem;

  int skip = sizesAndSkip[2] ? 1 : 0;

  lbp_cascade(lpvOut, lpvInII, lOut.chunkSpan, lInII.chunkSpan, lOut.chunkWidth, lOut.chunkHeight,
    lineIndex, lineBuffer,
    (APEX_lbpFeature*)lInCFeatures.pMem,
    stageCount, (APEX_lbpStage*)lInCstages.pMem,
    xShifts, xOffsets, skip);
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
