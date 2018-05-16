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
* \file haar_cascade_apu.h
* \brief Object detection based on Haar-like features implementation for APEX
* \addtogroup haar
* @{
****************************************************************************/

#ifndef APUHAARCASCADEIMPL_H
#define APUHAARCASCADEIMPL_H

#ifdef APEX2_EMULATE 
#include <apu_lib.hpp> // if using the APU emulation library
using namespace APEX2;
#endif

#include "cascade_definitions.h"

typedef vec16u FEATURE_FIXED_POINT_TYPE;
const int FEATURE_FRACTIONAL_BITS = 13;
const int FEATURE_FIXED_POINT_MULTIPLIER = (1 << FEATURE_FRACTIONAL_BITS);

typedef vec16u STAGE_FIXED_POINT_TYPE;
const int STAGE_FRACTIONAL_BITS = 10;
const int STAGE_FIXED_POINT_MULTIPLIER = (1 << STAGE_FRACTIONAL_BITS);

const int32u featureFixedCoefficientSqrt = 91;
const int32s invWindowAreaScalar = 25;

struct APEX_HaarCascadeFeature
{
  int16u threshold;
  int16u leftVal, rightVal;

  int16u x5y5w5_1;
  //type 0 ... horizontal edge, upper half positive
  //type 1 ... horizontal edge, lower half positive
  //type 2 ... vertical edge, left half positive
  //type 3 ... vertical edge, right half positive
  //type 4 ... line, horizontal
  //type 5 ... line, vertical
  //type 6 ... 4 rectangles, \
  //type 7 ... 4 rectangles, /
  int08u h5type3;
};

struct APEX_HaarCascadeStage
{
  //TODO: feature count could be inserted into feature/rect free bits
  int08u featureCount;
  int16u thresholdFixed;
};

/****************************************************************************/
/*!
   Detects objects using Haar-like feature cascades.
   The algorithm searches for 20x20-pixel objects using a Haar-like classifier provided by the user. For each input pixel, it outputs 255 if the pixel is a lower left corner of an object and 0 otherwise.
   The classifier consists of a number of stages, each stage contains multiple Haar-like features. The window has to pass every stage in order to be classified as containing an object.
   
   See http://docs.opencv.org/trunk/doc/py_tutorials/py_objdetect/py_face_detection/py_face_detection.html
   
   The format of the classifier stage is:\n
   
   \code{.cpp}
   struct APEX_HaarCascadeStage
   {
     int08u featureCount;
     int16u thresholdFixed;
   };
   \endcode
   Where featureCount is a number of Haar-like features included in the stage.\n
   ThresholdFixed is a fixed point threshold (10 fractional bits) which has to be exceeded by the sum of the feature test results in order for the stage to pass.
   
   A Haar-like feature is a rectangle subdivided into weighted sub-rectangles. The pixels inside the feature region are weighted according to the weight of the sub-rectangle they belong to and then they are summed together. The resulting sum is then compared with the feature's threshold and the resulting boolean tells whether the feature passed or not.
   \code{.cpp}
   struct APEX_HaarCascadeStage
   {
     int16u threshold;
     int16u leftVal, rightVal;
     int16u x5y5w5_1;
     int08u h5type3;
   };
   \endcode
   Where threshold is a fixed-point number (13 fractional bits) which has to be exceeded by the weighted sum of the feature rectangles in order for the feature to pass.\n
   leftVal, rightVal are fixed-point values (10 fractional bits), leftVal is added to the stage sum if the feature doesn't pass, righVal is added to the stage sum if the feature passes.\n
   x5y5w5_1 contains 3 values in its bits, starting from the most significant bit: upper left corner feature position x component in the first 5 bits, the y component in the next bits and the width of the feature in the next 5 bits. The least significant bit is not used.\n 
   h5type3 contains 2 values in its bits, starting from the most significant bit: feature height in the first bit, feature type in the remaining 3 bits.
   
   There are 8 possible types of feature:\n
   type 0 ... horizontal edge, upper half positive\n
   type 1 ... horizontal edge, lower half positive\n
   type 2 ... vertical edge, left half positive\n
   type 3 ... vertical edge, right half positive\n
   type 4 ... line, horizontal\n
   type 5 ... line, vertical\n
   type 6 ... 4 rectangles, upper left and lower right rectangles positive (\)\n
   type 7 ... 4 rectangles, lower left and upper right rectangles positive (/)\n
   
   The kernel expects an integral image and an integral image of squared values. The implementation retains a rolling window for the integral images, therefore sufficiently sized buffers have to be supplied.
   
   \brief Haar object detection.
 
   \param apOut           - [Output] pointer to the destination buffer
   \param apInII          - [Input]  pointer to the source integral image buffer
   \param apInII2         - [Input]  pointer to the source integral image
                                     of squared values buffer
   \param aOutStride      - [Input]  line stride of the destination data
   \param aInStride       - [Input]  line stride of the source data
   \param aTileWidth      - [Input]  width of one data tile
   \param aTileHeight     - [Input]  height of one data tile
   \param aLineIndex      - [Input]  index of currently processed line
   \param apWindowBuffer  - [Buffer] pointer to the input integral image window
                                     buffer
   \param apWindowBuffer2 - [Buffer] pointer to the input integral image
                                     of squared values window buffer
   \param apcFeatures     - [Input]  pointer to the Haar-like feature array
   \param aStageCount     - [Input]  number of Haar-like feature cascade stages
   \param apcStages       - [Input]  pointer to the Haar-like feature cascade
                                     stages
   \param apcXshifts      - [Input]  pointer to LUT containing tile shifts
                                     needed for horizontal pixel offsets
   \param apcXoffsets     - [Input]  pointer to LUT containing in-tile offsets
                                     needed for horizontal pixel offsets
*****************************************************************************/
void haar_cascade(
  vec08u* apOut,
  vec32u* apInII,
  vec32u* apInII2,
  int aOutStride,
  int aInStride,
  int aTileWidth,
  int aTileHeight,
  int16u aLineIndex,
  vec32u* apWindowBuffer,
  vec32u* apWindowBuffer2,
  const APEX_HaarCascadeFeature* apcFeatures,
  int aStageCount,
  const APEX_HaarCascadeStage* apcStages,
  const int08u* apcXshifts,
  const int08u* apcXoffsets
  );

#endif /* APUHAARCASCADEIMPL_H */

/*! @} */
