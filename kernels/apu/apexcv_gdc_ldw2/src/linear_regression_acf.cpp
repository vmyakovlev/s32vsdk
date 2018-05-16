/**************************************************************************************************
 * 
 * NXP Confidential Proprietary
 *
 * (C) Copyright CogniVue Corporation. 2015 All right reserved.
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

#include "Kernels_Configuration.h"

#ifdef ACF_KERNEL_METADATA

KERNEL_INFO _linear_regression
(
   "linear_regression",
   4,
   __port(__index(0),
          __identifier("POINTS_DATA"),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32u),
          __e0_size(1, 1),
          __ek_size(6, 1)),

   __port(__index(1),
          __identifier("MAX_VOTES_LEFT"),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),

   __port(__index(2),
          __identifier("MAX_VOTES_RIGHT"),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),

   __port(__index(3),
		    __identifier("DATA"),
		    __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
		    __spatial_dep(0,0,0,0),
		    __e0_data_type(d32u),
		    __e0_size(1, 1),
		    __ek_size(12, 1))
   /* Output data is in the format:
    * [0] 0 no valid points; 1 valid point
    * [1] line angle
    * [2] zero // todo: get rid of this. Make sure no conflict with the rest of the code.
    * [3] arbitrary x-value (currently TOPVIEW_HEIGHT - 1)
    * [4] y-value at [3]
    * [5] intercept
    */
);

#endif // end: #ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

const uint32_t gcSectionHeight  = TOPVIEW_HEIGHT/gcNumOfAccumalators;
const uint32_t halfTopviewWidth = TOPVIEW_WIDTH/2;
const uint8_t  cMinPointsFound  = 2; // minimum number of valid points required to make a line
const float  M_PI             = 3.14159265358f;
const float  M_PI_4           = M_PI / 4;


void linear_regression(
  kernel_io_desc points_data_param,
  kernel_io_desc max_votes_left_param,
  kernel_io_desc mac_votes_right_param,
  kernel_io_desc dataOut_param)
{
  uint32_t* pointsData = (uint32_t*)points_data_param.pMem;
  uint32_t* laneData   = (uint32_t*)dataOut_param.pMem;

  const uint32_t cMaxVotesLeft  = (uint32_t) max_votes_left_param.pMem;
  const uint32_t cMaxVotesRight = (uint32_t) mac_votes_right_param.pMem;
  const uint8_t  lStrideOut       = dataOut_param.chunkSpan>>2;

  RegressionLine(
    cMaxVotesLeft,
    &pointsData[0],
    gcDataOutStride,
    &laneData[0]
  );

  RegressionLine(
    cMaxVotesRight,
    &pointsData[3],
    gcDataOutStride,
    &laneData[lStrideOut]
  );

} // end: linear_regression


int16_t FastArcTan(float x)
{
  return (int16_t)fabsf((M_PI_4 * x - x * (fabsf(x) - 1) * (0.2447f + 0.0663f * fabsf(x))) * 180 / M_PI);
}

void RegressionLine(
      const uint32_t 	cMaxVotes,
      const uint32_t* 	cPointsData,
      const uint8_t 	cDataStride,
            uint32_t* 	rLane
){
   const uint8_t cMinValidVotes = cMaxVotes / 2;
   uint32_t sumY = 0, sumX = 0, sumXY = 0, sumX2 = 0, x, y, validPointsCounter = 0;

  // Find sums for linear regression
  for(uint8_t i = 0; i < gcNumOfAccumalators; i++){
    x = TOPVIEW_HEIGHT - (TOPVIEW_HEIGHT/gcNumOfAccumalators / 2) - (TOPVIEW_HEIGHT/gcNumOfAccumalators * i);
    y = cPointsData[i * cDataStride + 1];

    // Check for valid points
    if (cPointsData[i * cDataStride] > cMinValidVotes   &&
        cPointsData[i * cDataStride + 2]
    ) {
      sumY  += y;
      sumXY += y * x;
      sumX  += x;
      sumX2 += x * x;
      ++validPointsCounter;
    } // end if: check for valid points 
  } // end for: find sums

  // CASE 1: No valid lane information
  if(validPointsCounter < cMinPointsFound){ 
    rLane[0] = 0;
  } 
  // CASE 2: Lanes were found
  else {
    rLane[0] = 1;

    float xMean = (float)sumX / validPointsCounter;
    float yMean = (float)sumY / validPointsCounter;
    int32s denominator = sumX2 - sumX * xMean;
    float slope = (sumXY - sumX * yMean) / denominator;
    float yInt = yMean - slope * xMean;

    rLane[1] = 180 + FastArcTan(slope);


    rLane[2] = 0; // get rid of this?
    rLane[3] = TOPVIEW_HEIGHT-1;
    rLane[4] = (uint32_t)(rLane[3] * slope + yInt);
    rLane[5] = (uint32_t)yInt;
  }
} // end: regressionLine() 

#endif //end: #ifdef ACF_KERNEL_IMPLEMENTATION
