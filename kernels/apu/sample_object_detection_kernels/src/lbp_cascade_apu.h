/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2013-2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
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
* \file lbp_cascade_apu.h
* \brief Object detection based on LBP features implementation for APEX
* \addtogroup lbp
* @{
****************************************************************************/

#ifndef APULBPCASCADEIMPL_H
#define APULBPCASCADEIMPL_H

#ifdef APEX2_EMULATE 
#include <apu_lib.hpp> // if using the APU emulation library
using namespace APEX2;
#endif

#include "cascade_definitions.h"

typedef vec32s STAGE_FIXED_POINT_TYPE;
const int STAGE_FRACTIONAL_BITS = 28;
const int STAGE_FIXED_POINT_MULTIPLIER = (1 << STAGE_FRACTIONAL_BITS);
typedef int32s STAGE_FIXED_POINT_TYPE_SCALAR;

struct APEX_lbpFeature
{
  int32_t values[8];
  STAGE_FIXED_POINT_TYPE_SCALAR leafValuesFixed[2];
  uint8_t x, y, width, height;
};

struct APEX_lbpStage
{
  uint8_t featureCount;
  STAGE_FIXED_POINT_TYPE_SCALAR threshold;
};

/****************************************************************************/
/*!
   Detects objects using local binary pattern feature cascades.
   
   The algorithm searches for 24x24-pixel objects using a local binary pattern (LBP) classifier provided by the user. For each input pixel, it outputs 255 if the pixel is a lower left corner of an object and 0 otherwise.
   The classifier consists of a number of stages, each stage contains multiple LBP features. The window has to pass every stage in order to be classified as containing an object.
   
   See Shengcai Liao, Xiangxin Zhu, Zhen Lei, Lun Zhang and Stan Z. Li. Learning Multi-scale Block Local Binary Patterns for Face Recognition\n
   http://www.nlpr.labs.gov.cn/2007papers/gjhy/gh98.pdf \n
   
   The format of the classifier stage is:\n
   
   \code{.cpp}
   struct APEX_lbpStage
   {
     uint8_t featureCount;
     int32s threshold;
   };
   \endcode
   Where featureCount is a number of LBP features included in the stage.\n
   threshold is a fixed point threshold (28 fractional bits) which has to be exceeded by the sum of the feature test results in order for the stage to pass.
   
   An LBP feature is a rectangle subdivided into a uniform 3x3 grid. Pixel values in each of the 9 sub-areas are summed. The sums of the outer areas are compared to the sum of the central area resulting in a 8-bit code (starting in the upper left area and going clockwise).
   This code is then used as an index to 256-bit table and the resulting bit tells whether the feature passed or not.
   \code{.cpp}
   struct APEX_lbpFeature
   {
     int32_t values[8];
     int32s leafValuesFixed[2];
     uint8_t x, y, width, height;
   };
   \endcode
   values[8] is a 256-bit classifying table used to determine whether the LBP feature with a given code should pass\n
   Where threshold is a fixed-point number (13 fractional bits) which has to be exceeded by the weighted sum of the feature rectangles in order for the feature to pass.\n
   leafValuesFixed are fixed-point values (28 fractional bits), leafValuesFixed[0] is added to the stage sum if the feature passes, leafValuesFixed[1] is added to the stage sum if the feature doesn't pass.\n
   x, y define the position of the feature's upper left corner\n
   width, height define the size of the feature
   
   The kernel expects an integral image. The implementation retains a rolling window for the integral image, therefore sufficiently sized buffer has to be supplied.
   
   \brief Local Binary Pattern object detection.
 
   \param apOut           - [Output] pointer to the destination buffer
   \param apInII          - [Input]  pointer to the source integral image buffer
   \param aOutStride      - [Input]  line stride of the destination data
   \param aInStride       - [Input]  line stride of the source data
   \param aTileWidth      - [Input]  width of one data tile
   \param aTileHeight     - [Input]  height of one data tile
   \param aLineIndex      - [Input]  index of currently processed line
   \param apWindowBuffer  - [Buffer] pointer to the input integral image window
                                     buffer
   \param apcFeatures     - [Input]  pointer to the LBP feature array
   \param aStageCount     - [Input]  number of LBP feature cascade stages
   \param apcStages       - [Input]  pointer to the LBP feature cascade stages
   \param apcXshifts      - [Input]  pointer to LUT containing tile shifts
                                     needed for horizontal pixel offsets
   \param apcXoffsets     - [Input]  pointer to LUT containing in-tile offsets
                                     needed for horizontal pixel offsets
   \param skipOdd         - [Input]  1 if even columns and rows should be
                                     skipped, 0 otherwise
*****************************************************************************/
void lbp_cascade(
  vec08u* apOut,
  vec32u* apInII,
  int aOutStride,
  int aInStride,
  int aTileWidth,
  int aTileHeight,
  int16u aLineIndex,
  vec32u* apWindowBuffer,
  const APEX_lbpFeature* apcFeatures,
  int aStageCount,
  const APEX_lbpStage* apcStages,
  const int08u* apcXshifts,
  const int08u* apcXoffsets,
  int skipOdd
  );

#endif /* APULBPCASCADEIMPL_H */

/*! @} */
