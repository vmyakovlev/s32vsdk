/**************************************************************************************************
 * 
 * NXP Confidential Proprietary
 *
 * Copyright (c) 2015, Digital Media Professionals. All rights reserved.
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
#ifdef ACF_KERNEL_METADATA

KERNEL_INFO kernel_info_lbp_extract_descriptor
(
   "lbp_extract_descriptor",
   3,
   __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(32, 32)),
   
   __port(  __index(1),
      __identifier("PROCESS_SIZE"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(2, 1),
      __ek_size(1, 1)),
   
   __port(  __index(2),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      __e0_size(128, 2),
      __ek_size(1, 1))
);

KERNEL_INFO kernel_info_compare_descriptor
(
   "lbp_compare_descriptor",
   4,
   __port(  __index(0),
      __identifier("INPUT_MODEL"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      // __e0_size(128, 1),
      __e0_size(128, 2),
      __ek_size(1, 1)),
   
   __port(  __index(1),
      __identifier("INPUT_TEST"),
      __attributes(ACF_ATTR_VEC_IN_STATIC),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      __e0_size(128, 2),
      __ek_size(1, 1)),
      
   __port(  __index(2),
      __identifier("CLOSEST_ID"),
      __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
      
   __port(  __index(3),
      __identifier("DISTANCE"),
      __attributes(ACF_ATTR_SCL_OUT_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      // worst case number of distance models is 1024
      __ek_size(1, 1))
);
#endif // ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION
#include <stdint.h>
#include "lbp_apu.h"

void lbp_extract_descriptor(
   kernel_io_desc lIn                  /*!< Input image data. */,
   kernel_io_desc lProcessSize         /*!< Processing size (width, height). */,
   kernel_io_desc lOut                 /*!< Output descriptor buffer. */
)
{
   vec08u *lpvIn = (vec08u *)lIn.pMem;
   int16u *lpvProcessSize = (int16u *)lProcessSize.pMem;
   vec08u *lpvOut = (vec08u *)lOut.pMem;
   
   apu_lbp_extract_descriptor(lpvIn, lIn.chunkSpan, lpvOut, lpvProcessSize[0], lpvProcessSize[1], ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS));
}

void lbp_compare_descriptor(
   kernel_io_desc lInModel             /*!< Input model descriptor (128x1x4xn). */,
   kernel_io_desc lInTest              /*!< Input test descriptor (128x4). */,
   kernel_io_desc lOutClosestId        /*!< Output closest model id. */,
   kernel_io_desc lOutDistance         /*!< Output histogram distance. */
)
{
   vec08u *lpvInModel = (vec08u *)lInModel.pMem;
   vec08u *lpvInTest = (vec08u *)lInTest.pMem;
   int16u *lpvOutClosestId = (int16u *)lOutClosestId.pMem;
   int32s *lpvOutDistance = (int32s *)lOutDistance.pMem;
   static int32s lMinDist;       // minimum histogram distance
   vec32s localDist;
   int32s totalDist;                  // accumulated histogram distance
   static int16u modelId;        // current model descriptor id
   
   if (ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
   {
      lpvOutClosestId[0] = 0;
      lMinDist = 0x7fffffff;
      modelId = 0;
   }
      totalDist = 0;

      localDist = 0;
   
   apu_lbp_compare_histogram(lpvInModel, &lpvInTest[0], &localDist, lInTest.chunkHeight*lInTest.chunkSpan*2, ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS));

   apu_lbp_accumulate_distance(localDist, &totalDist, ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS));
   *lpvOutDistance = totalDist;
   if (totalDist<lMinDist) // if the current descriptor distance is the minimum, update closest ID
   {
      lMinDist = totalDist;
      lpvOutClosestId[0] = modelId;
   }
   modelId++;
}

#endif // ACF_KERNEL_IMPLEMENTATION
