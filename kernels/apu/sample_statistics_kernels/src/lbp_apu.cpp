#ifdef ACF_KERNEL_IMPLEMENTATION

/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
* Copyright (c) 2017 NXP
* 
* All Rights Reserved
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
/**
* \file lbp_apu.cpp
* \brief histogram implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/

#include "lbp_apu.h"
#include "lbp_defines.h"

#ifdef APEX2_EMULATE
#include "apu_vec.hpp"
#endif

/* */

static vec16s computeLBP(vec08u* origInImg, int16s inStride) {
	vec16u around[8];
	vec08u *inImg = origInImg + inStride + 1; // go to the second element of the second row.
	vec16u upLeft = (vec16u)inImg[-1] + (vec16u)inImg[0], 
		upCenter =(vec16u)inImg[0] + (vec16u)inImg[1],
		upRight = (vec16u)inImg[1] + (vec16u)inImg[2], 
		botLeft = (vec16u)inImg[inStride - 1] + (vec16u)inImg[inStride], 
		botCenter = (vec16u)inImg[inStride] + (vec16u)inImg[inStride+1],
		botRight = (vec16u)inImg[inStride + 1] + (vec16u)inImg[inStride+2];
	int16s secStride = inStride << 1;

	vec16u ctr = upCenter + botCenter;

	around[0] = (vec16u)inImg[-inStride - 1] + (vec16u)inImg[-inStride] + upLeft;
	around[1] = (vec16u)inImg[-inStride] + (vec16u)inImg[-inStride+1] + upCenter;
	around[2] = (vec16u)inImg[-inStride + 1] + (vec16u)inImg[-inStride+2] + upRight;
	around[3] = upRight + botRight;
	around[4] = botRight + (vec16u)inImg[secStride + 1] + (vec16u)inImg[secStride + 2];
	around[5] = botCenter + (vec16u)inImg[secStride] + (vec16u)inImg[secStride + 1];
	around[6] = botLeft + (vec16u)inImg[secStride-1] + (vec16u)inImg[secStride];
	around[7] = upLeft + botLeft;

	vec16s res = 0;
	for (int08u i = 0; i < 8; ++i) {
		res |= ((vec16s)(around[i] > ctr)) << 7 - i;
	}
	return res;
}

/*****************************************************************************
* functions
*****************************************************************************/
/* Compute for each tile a LBP (256 bins, with 4 overlaps). DO NOT COUNT THE ZEROs 
*/
void lbp(vec08u* lpvInImg, vec16u* lpvOut, vec16u* lpvOutBlNorm, int lStrideIn, int chunkWidth, int chunkHeight, int lStrideOut, bool isStart)
{
  vec16u *out = lpvOut;
  vec08u *inImg = lpvInImg;// -LBP_OVERLAP * lStrideIn;

  // int16u overlapStep = LBP_OVERLAP;
  
  //initialize the vector output buffer to zero
   
   for (int i = 0; i < lStrideOut; ++i) {
	   out[i] = 0;
   }
   

  *lpvOutBlNorm = 1;
  vec32u secScaleNorm = 1;
  out = lpvOut;
  int16s overlap = 1; // every p
  
  for (int16s y = 0; y < chunkHeight; y += overlap)
  {

	for (int16s x = 0; x < chunkWidth; x += overlap)
	{
		vec16s lbpV = computeLBP(&inImg[x], lStrideIn);
		vec16u cOut;
		if (vany(lbpV != 0)) {
			vif(lbpV > 0) {
				*lpvOutBlNorm += 1;
				cOut = vload(out, lbpV);
				cOut++;
				vstore(out, lbpV, cOut);
			} vendif
		}
    }
	inImg += lStrideIn;
	//out += lStrideOut; //one per Stride, accumulates all histograms
  }


  vif(*lpvOutBlNorm > 1)
  {
	  *lpvOutBlNorm -= 1;
  }vendif
  // NOTE: here the out vector should be divided by the norm. However, since outside a division has to be made anyway, the division is left to be performed outside
  // i.e there should be still following code:
  /* for (int i = 0; i < NR_FEATURES_PER_BOX; ++i) {
		out[i] <<= 4; //(on a 4x4 patch the values are < 2^12, thus for the division, those can be scaled up)
		out[i] /= *lpvOutBlNorm;
  }
  */
}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
