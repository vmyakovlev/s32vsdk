
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
/**
* \file harris_apu.cpp
* \brief harris corner detector implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/
#ifdef ACF_KERNEL_IMPLEMENTATION

#include "harris_apu.h"

/*****************************************************************************
* functions
*****************************************************************************/

void apuHarrisResponse(vec16u* apResponse,
  vec16s* apGradX, vec16s* apGradY, vec16s* apTemp,
  int aBlockWidth, int aBlockHeight, int aStride, int k, int responseBitShift, int aWindowSize, int thresh, bool isFirstSlice)
{
	int startRow = isFirstSlice ? -aWindowSize : aWindowSize;

	vec16s* gradX2 = apGradX + startRow * aStride;
	vec16s* gradY2 = apGradY + startRow * aStride;
	vec16s* gradXY = apTemp + startRow * aStride;
	vec32s vthresh = thresh;

  for (int y = startRow; y < aBlockHeight + aWindowSize ; ++y)
  {
	for (int x = -aWindowSize; x < aBlockWidth + aWindowSize; ++x)
    {
		vec16s gx = gradX2[x];
		vec16s gy = gradY2[x];
		gradXY[x] = gx*gy;
		gradX2[x] = gx*gx;
		gradY2[x] = gy*gy;
    }
    gradX2 += aStride;
    gradY2 += aStride;
    gradXY += aStride; // aBlockWidth
  }

  //rewind
  gradX2 = apGradX;
  gradY2 = apGradY;
  gradXY = apTemp;
  
  for (int y = 0; y < aBlockHeight; ++y)
  {
	for (int x = 0; x < aBlockWidth; ++x)
    {
      vec32u sumGx2 = 0;
      vec32u sumGy2 = 0;
      vec32s sumGxy = 0;

      for (int wy = -aWindowSize; wy <= aWindowSize; ++wy)
      {
        for (int wx = x - aWindowSize; wx <= x + aWindowSize; ++wx)
        {
          int index = wy * aStride + wx;
		  vec32u tx = (vec32u)((vec16u)gradX2[index]);
		  vec32u ty = (vec32u)((vec16u)gradY2[index]);
		  sumGx2 += tx;
		  sumGy2 += ty;
		  int indexXY = index;//wy * aBlockWidth + wx;
          sumGxy += (vec32s)gradXY[indexXY];
        }
      }

      vec32u trace = sumGx2 + sumGy2;
      vec32s det = (vec32s)(sumGx2 * sumGy2) - (vec32s)(sumGxy * sumGxy);

      vec32s response = det - (vec32s)(k * trace * trace >> 8); // if not shifted to the right, all responses < 0
		
      response >>= responseBitShift;
	  response = vselect(response, 0, response > vthresh);
	  response = vselect(response, 65535, response < 65535);
      apResponse[x] = (vec16u)response;
    }

    apResponse += aBlockWidth;
    gradX2 += aStride;
    gradY2 += aStride;
    gradXY += aStride; //aBlockWidth
  }
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION 
