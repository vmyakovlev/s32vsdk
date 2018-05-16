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
#include <stdlib.h>

#ifdef ACF_KERNEL_METADATA

KERNEL_INFO _thresh_val_histogram
(
   "thresh_val_histogram",
   3,
   __port(__index(0),
          __identifier("SRC"),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
  __port(__index(1),
		  __identifier("THRESH_IN"),
		  __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
		  __spatial_dep(0,0,0,0),
		  __e0_data_type(d08u),
		  __e0_size(1, 1),
		  __ek_size(1, 1)),
  __port(__index(2),
		  __identifier("THRESH_OUT"),
		  __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
		  __spatial_dep(0,0,0,0),
		  __e0_data_type(d08u),
		  __e0_size(1, 1),
		  __ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION
const int08u startBlock = (TOPVIEW_HEIGHT / gcRemapDestBlockHeight) * (1 - HISTOGRAM_AREA);

void thresh_val_histogram(kernel_io_desc lIn0,
		kernel_io_desc thresholdin_param,
		kernel_io_desc thresholdout_param)
{
   vec08u* lpvIn0   = (vec08u*)lIn0.pMem;
   int08u* threshold_prev = (int08u*)thresholdin_param.pMem;
   int08u* threshold = (int08u*)thresholdout_param.pMem;
   int lStrideIn0   = lIn0.chunkSpan;
   static int08u counter;
   static vec32u vhistogram[HISTOGRAM_BIN_COUNT];
   static int32u histogram[HISTOGRAM_BIN_COUNT];

   //initialize the static vector output buffer to zero
   if (ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
   {
	   for (int i=0; i<HISTOGRAM_BIN_COUNT; i++){
		   vhistogram[i] = 0;
		   histogram[i] = 0;
	   }
	   counter = 0;
   }
   if(counter++ >= startBlock){
	   for (int y=0; y<lIn0.chunkHeight; y++)
	   {
		   for (int x=0; x<lIn0.chunkWidth; x++)
		   {
			   vec08u lvBinIndex = lpvIn0[y*lStrideIn0+x] >> 2;//shift for histogram size [256/4]or[64]
			   vec32u lvTmp = vload(vhistogram, lvBinIndex);
			   lvTmp += 1;
			   vstore(vhistogram, lvBinIndex, lvTmp);
		   }
	   }
   }

   if (ACF_RET_VAR(ACF_VAR_LAST_TILE_FLAG))
   {
	   int16_t lFirstCuId         = ACF_RET_VAR(ACF_VAR_FIRST_CUID);
	   int16_t lTileWidthInChunks = ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS);

	   for (int i=lFirstCuId; i<lFirstCuId+lTileWidthInChunks; i++)
	   {
		   for (int x=0; x<HISTOGRAM_BIN_COUNT; x++)
		   {
			   histogram[x] += vget(vhistogram[x], i);
		   }
	   }

	   int32u maxPixCount = 0;
	   int08u currentPixIndex = 0;
	   int08u maxLum[10];//ten peaks is a magical number

	   int08u majorityPixelLum = 0;//Just incase peaks were not found
	   int32u majorityPixelCount = 0;//Just incase peaks were not found
	   int08u histogramUpperLimit = HISTOGRAM_BIN_COUNT - 2;// assuming no peaks on the very ends
	   for(uint32_t i = 2; i < histogramUpperLimit; i++){
		   if(currentPixIndex >= 10){
			   currentPixIndex = 0;
		   }
		   if(histogram[i-1] < histogram[i] && histogram[i-2] < histogram[i] &&
				   histogram[i+1] < histogram[i] && histogram[i+2] < histogram[i]){
			   maxLum[currentPixIndex++] = i << 2;
			   maxPixCount = currentPixIndex > maxPixCount ? currentPixIndex : maxPixCount;
		   }
		   if(majorityPixelCount<histogram[i]){
			   majorityPixelCount = histogram[i];
			   majorityPixelLum = i << 2;
		   }

	   }
           int08u th = majorityPixelLum;
           int32u verticalThreshold = majorityPixelCount*3/4;
           int08u currentDiff = abs(th - threshold_prev[0]);
           for(int08u i = 0; i < maxPixCount; i++){
                   if(abs(maxLum[i] - threshold_prev[0]) < currentDiff && histogram[maxLum[i]/4] > verticalThreshold){
                           th = maxLum[i];
                           currentDiff = abs(maxLum[i] - threshold_prev[0]);
                   }
           }
           threshold[0] = th;
   }
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
