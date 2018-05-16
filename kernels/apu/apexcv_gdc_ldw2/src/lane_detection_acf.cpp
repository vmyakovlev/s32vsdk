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

KERNEL_INFO _lane_detection
(
   "lane_detection",
   5,
   __port(__index(0),
          __identifier("SRC"),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier("VOTE_THRESHOLD"),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(2),
          __identifier("POINTS_DATA"),
          __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32u),
          __e0_size(1, 1),
          __ek_size(6, 80)),    // 6*gcNumOfAccumalators ( 6 * TOPVIEW_HEIGHT / gcRemapDestBlockWidth )
   /* Outputs the points in the format:
    * [i+0] LEFT vote value
    * [i+1] LEFT x-value correspeonding to vote
    * [i+2] LEFT boolean valid?
    * [i+3] RIGHT vote value
    * [i+4] RIGHT x-value corresponding to vote
    * [i+5] RIGHT boolean valid?
    * where i varies from 0... gcNumOfAccumulators
    */
   __port(__index(3),
          __identifier("MAX_LEFT"),
          __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),

   __port(__index(4),
          __identifier("MAX_RIGHT"),
          __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION
const uint32_t gcSectionHeight = TOPVIEW_HEIGHT/gcNumOfAccumalators;
const uint32_t halfTopviewWidth = TOPVIEW_WIDTH/2;

void lane_detection(
   kernel_io_desc src_param,
   kernel_io_desc voteThreshold_param,
   kernel_io_desc dataOut_param,
   kernel_io_desc maxLeft_param,
   kernel_io_desc maxRight_param)
{

  vec08u* src = (vec08u*)src_param.pMem;
  uint32_t* dataOut = (uint32_t*)dataOut_param.pMem;


  // to do: clean this up
  uint8_t* maxLeft = (uint8_t*)maxLeft_param.pMem;
  uint8_t* maxRight = (uint8_t*)maxRight_param.pMem;
  *maxLeft  = 0;
  *maxRight = 0;

  static vec32u accumulator[gcRemapDestBlockWidth];
  static int32u counter[2];

  uint8_t* cVoteThreshold = (int08u*) voteThreshold_param.pMem;
  uint8_t  lStrideIn0   = src_param.chunkSpan;

  vec32u vTempVoteSum;
  vec32u vTempMax;
  vec32u vTempMaxCUIndex;
  vbool  vTempMaxBool;

  // TODO: get number of CU from...
   uint32_t CUCount = 64;
   uint32_t voteSumLeft;
   uint32_t voteSumRight;
   uint32_t avg;
   uint32_t vote;
   uint32_t dataOutIndex;


   if (ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
   {
      for(uint8_t i = 0; i < gcRemapDestBlockWidth; ++i){
         accumulator[i] = (vec32u)0;
      }
      counter[0] = 0;// sub-sector counter (resets). counter for gcSectionHeight
      counter[1] = 0;// sub-sector counter (does not reset), counter[1] is section index

      //
      /* Outputs the points in the format for dataOut[gcDataOutStrade * gcNumOfAccumalators]
       * [i+0] LEFT vote value
       * [i+1] LEFT x-value correspeonding to vote
       * [i+2] LEFT boolean valid?
       * [i+3] RIGHT vote value
       * [i+4] RIGHT x-value corresponding to vote
       * [i+5] RIGHT boolean valid?
       * where i varies from 0... gcNumOfAccumulators
       */
       // gcNumberAccumalators = number of sections in top view height. TOPVIEW_HEIGHT/gcNumOfAccumalators
       //
	   for (uint8_t y=0; y < gcNumOfAccumalators; ++y){
		   for (uint8_t x=0; x < gcDataOutStride; ++x){
			   dataOut[y*gcDataOutStride + x] = 0;
		   }
	   }
   }
   // local accumulator: size is chunk width
   for (uint8_t y=0; y < src_param.chunkHeight; ++y){

	  // accumulate per column
      for (uint8_t x=0; x < src_param.chunkWidth; ++x){
         accumulator[x] += (vec32u)src[y*lStrideIn0+x];
      }
      // increment based on height (index to reach the end of the section): beginning of row
	   ++counter[0];
	   /// cSectionHeight = TOPVIEW_HEIGHT/gcNumOfAccumalators
	   if ( counter[0] == gcSectionHeight ){
		   vTempVoteSum = accumulator[0];
		   vTempMaxCUIndex = (vec32u) 0;
		   vTempMax = accumulator[0];
		   accumulator[0] = (vec32u)0;
		   for (int08u x=1; x<src_param.chunkWidth; x++){
			   vTempVoteSum = vadd(vTempVoteSum , accumulator[x]);
			   vTempMaxBool = vsgt( accumulator[x], vTempMax );
			   vTempMaxCUIndex = vselect((vec32u)x,vTempMaxCUIndex, vTempMaxBool);
			   vswap( vTempMax, accumulator[x], vTempMaxBool);
			   accumulator[x] = (vec32u)0;
		   }

		   dataOutIndex =  counter[1] * gcDataOutStride;

         // Processing for left side
		   voteSumLeft = 0;
		   for(uint32_t i = 0 ; i < CUCount/2; i++){
			   vote = vget(vTempMax,i);
			   voteSumLeft += vget(vTempVoteSum,i);
			   if(vote > dataOut[dataOutIndex]){
				   dataOut[dataOutIndex] = vote;
				   dataOut[dataOutIndex+1] = src_param.chunkWidth * i + vget(vTempMaxCUIndex,i);
			   }
		   }

         // Processing for right side
		   voteSumRight = 0;
		   for(uint32_t i = CUCount/2 ; i < CUCount; i++){
			   vote = vget(vTempMax,i);
			   voteSumRight += vget(vTempVoteSum,i);
			   if(vote > dataOut[dataOutIndex+3]){
				   dataOut[dataOutIndex+3] = vote;
				   dataOut[dataOutIndex+4] = src_param.chunkWidth * i + vget(vTempMaxCUIndex,i);
			   }
		   }

         // LEFT
		   avg = voteSumLeft/halfTopviewWidth;
		   dataOut[dataOutIndex+2] = ((dataOut[dataOutIndex] * cVoteThreshold[0]) / 100 > avg);
       if (dataOut[dataOutIndex] > maxLeft[0])
       {
          maxLeft[0] = dataOut[dataOutIndex];
       }


         // RIGHT
       // TODO: magic number of 100
		   avg = voteSumRight/halfTopviewWidth;
		   dataOut[dataOutIndex+5] = ((dataOut[dataOutIndex+3] * cVoteThreshold[0]) / 100 > avg);
       if (dataOut[dataOutIndex+3] > maxRight[0])
       {
          maxRight[0] = dataOut[dataOutIndex+3];
       }


		   counter[0] = 0;

		   // counter[1] is section index
		   // total dataout needs 6 (per section) * number_of_sections
		   // beginning of this kernel, it computes: gcSectionHeight = TOPVIEW_HEIGHT/gcNumOfAccumalators
		   // number of sections = top view height / gcSectionHeight
		   ++counter[1];
	   }
   }
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
