
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
* \file rgb_to_hsv_apu.cpp
* \brief rgb_to_grayscale implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/
#ifdef ACF_KERNEL_IMPLEMENTATION

#include "rgb_to_hsv_apu.h"

/*****************************************************************************
* functions
*****************************************************************************/

void rgb_to_hsv_sat(vec08u* apSat, const vec08u* apcSrc,
  int aBlockWidth, int aBlockHeight, int aOutputSpan, int aInputSpan)
{
  for (int16s y = 0; y < aBlockHeight; ++y)
  {
   int16s x3 = 0;
    for (int16s x = 0; x < aBlockWidth; ++x)
    {
	
		vec16u vBlue = vec16u(apcSrc[x3]);
		vec16u vGreen = vec16u(apcSrc[x3 + 1]);
		vec16u vRed = vec16u(apcSrc[x3 + 2]);
		vec16u vMin;
		vec16u vMax;
		vec16u vDelta;
		vec16u vSat;
		//Find vMax
		vif(vRed > vGreen)
		{
			vMax = vRed;
			vMin = vGreen;
		}
		velse
		{
			vMax = vGreen;
			vMin = vRed;
		}
		vendif

		vif(vBlue > vMax)
		{
			vMax = vBlue;
		}
		vendif
		//Find vMin

		vif(vBlue < vMin)
		{
			vMin = vBlue;
		}
		vendif
		
		vDelta = vMax - vMin;

		vif(vMax > 0)
		{
			vDelta = vDelta << 255; //Multiple to get the fractional part
			vSat = (vec16u)(vDelta / vMax);
		}
		velse
		{
			vSat = 0;
		}
			vendif

		apSat[x] = vec08u(vSat);

		x3 += 3;
    }

    apSat += aOutputSpan;
    apcSrc += aInputSpan;
  }
}


void rgb_to_hsv_hue_sat(vec16u* apHue, vec08u* apSat,  const vec08u* apcSrc,
	int aBlockWidth, int aBlockHeight, int aOutputSpan, int aInputSpan)
{
	for (int16s y = 0; y < aBlockHeight; ++y)
	{
		int16s x3 = 0;
		for (int16s x = 0; x < aBlockWidth; ++x)
		{

			vec16s vBlue = vec16s(apcSrc[x3]);
			vec16s vGreen = vec16s(apcSrc[x3 + 1]);
			vec16s vRed = vec16s(apcSrc[x3 + 2]);
			vec16s vMin;
			vec16s vMax;
			vec16s vDelta;
			vec16s vSat;
			vec16s vHue;
			//Find vMax
			vif(vRed > vGreen)
			{
				vMax = vRed;
				vMin = vGreen;
			}
			velse
			{
				vMax = vGreen;
				vMin = vRed;
			}
			vendif
			vif(vBlue > vMax)
			{
				vMax = vBlue;
			}vendif

			vif(vBlue < vMin) {
					vMin = vBlue;
			} vendif

			vDelta = vMax - vMin;

			vif(vDelta != 0)
			{ // rotate hue circle by +90 degrees, such that Red is at 90 deg. 
				vif(vMax == vRed && vDelta != 0) {
					vHue = (( (vec16s)vabs(vGreen - vBlue) *60 / vDelta)  + 450) % 360;//450 = 360 + 90
				} vendif
					
				vif(vMax == vGreen) {
					vHue = (((60*(vec16s)vabs(vBlue-vRed)) / vDelta)  + 570) % 360; //570 = 360 + 120 + 90
				} vendif

				vif(vMax == vBlue) {
					vHue = (((60*(vec16s)vabs(vRed - vGreen)) / vDelta)  + 330) % 360; //690 = 360 + 240 + 90
				} vendif
				

				vDelta = vDelta << 8; //Multiply to get the fractional part
				vSat = vDelta / vMax;
			}
			velse
			{
				vSat = 0;
				vHue = 0;
			}
			vendif

			apSat[x] = vec08u(vSat);
			apHue[x] = vec16u(vHue);

			x3 += 3;
		}

		apSat += aOutputSpan;
		apHue += aOutputSpan;
		apcSrc += aInputSpan;
	}
}




void rgb_to_hsv_hue_sat_grey(vec16u* apHue, vec08u* apSat, vec08u *apGrey, const vec08u* apcSrc,
	int aBlockWidth, int aBlockHeight, int aOutputSpan, int aInputSpan)
{
	for (int16s y = 0; y < aBlockHeight; ++y)
	{
		int16s x3 = 0;
		for (int16s x = 0; x < aBlockWidth; ++x)
		{

			vec16s vBlue = vec16s(apcSrc[x3]);
			vec16s vGreen = vec16s(apcSrc[x3 + 1]);
			vec16s vRed = vec16s(apcSrc[x3 + 2]);
			vec16s vMin;
			vec16s vMax;
			vec16s vDelta;
			vec16s vSat;
			vec16s vHue;
			//Find vMax
			vif(vRed > vGreen)
			{
				vMax = vRed;
				vMin = vGreen;
			}
			velse
			{
				vMax = vGreen;
				vMin = vRed;
			}
				vendif
				vif(vBlue > vMax)
			{
				vMax = vBlue;
			}vendif

				vif(vBlue < vMin) {
				vMin = vBlue;
			} vendif

				vDelta = vMax - vMin;

			vif(vDelta != 0)
			{ // rotate hue circle by +90 degrees, such that Red is at 90 deg. 
				vif(vMax == vRed && vDelta != 0) {
					vHue = (((vec16s)vabs(vGreen - vBlue) * 60 / vDelta) + 450) % 360;//450 = 360 + 90
				} vendif

					vif(vMax == vGreen) {
					vHue = (((60 * (vec16s)vabs(vBlue - vRed)) / vDelta) + 570) % 360; //570 = 360 + 120 + 90
				} vendif

					vif(vMax == vBlue) {
					vHue = (((60 * (vec16s)vabs(vRed - vGreen)) / vDelta) + 330) % 360; //690 = 360 + 240 + 90
				} vendif


					vDelta = vDelta << 8; //Multiply to get the fractional part
				vSat = vDelta / vMax;
			}
			velse
			{
				vSat = 0;
				vHue = 0;
			}
				vendif

				apSat[x] = vec08u(vSat);
			apHue[x] = vec16u(vHue);


			// transform to grey
			vec16u accum = 27 * vec16u(apcSrc[x3]) + 92 * vec16u(apcSrc[x3 + 1]) + 9 * vec16u(apcSrc[x3 + 2]);
			apGrey[x] = vec08u(accum >> 7);


			x3 += 3;
		}

		apSat += aOutputSpan;
		apHue += aOutputSpan;
		apGrey += aOutputSpan;
		apcSrc += aInputSpan;
	}
}


void rgb_to_hsv_svr(vec08u* apSat, vec08u* apVal, vec08u* apRed, const vec08u* apcSrc,
	int aBlockWidth, int aBlockHeight, int aOutputSpan, int aInputSpan)
{

	vec08u *redArr = apRed;

	for (int16s y = 0; y < aBlockHeight; ++y)
	{
		int16s x3 = 0;
		for (int16s x = 0; x < aBlockWidth; ++x)
		{
			vec16u vBlue = vec16u(apcSrc[x3]);
			vec16u vGreen = vec16u(apcSrc[x3 + 1]);
			redArr[x] = apcSrc[x3 + 2];
			vec16u vRed = vec16u(redArr[x]);

			vec16u vMin;
			vec16u vMax;
			vec16u vDelta;
			vec16u vSat;
			//Find vMax
			vif(vRed > vGreen)
			{
				vMax = vRed;
				vMin = vGreen;
				redArr[x] -= (apcSrc[x3 + 1] >> 1); // redArr -= green/2
			}
			velse
			{
				redArr[x] = 0;
				vMax = vGreen;
				vMin = vRed;
			}
			vendif
			vif(vBlue > vMax)
			{
				vMax = vBlue;
				redArr[x] = 0;
			} velse{
				vec08u partBlue = apcSrc[x3] >> 1;
				vif(redArr[x] > partBlue) {
					redArr[x] -= partBlue; //redArr -= blue/2
				} velse{
					redArr[x] = 0;
				} vendif
			}
			vendif

			//Find vMin
			vif(vBlue < vMin)
			{
				vMin = vBlue;
			} 
			vendif

			vDelta = vMax - vMin;

			vif(vMax > 0)
			{
				vDelta = vDelta << 8; //Multiple to get the fractional part
				vSat = vDelta / vMax;
			}
			velse
			{
				vSat = 0;
			}
				vendif

			apSat[x] = vec08u(vSat);
			apVal[x] = vec08u(vMax);
		
			x3 += 3;
		}

		apSat += aOutputSpan;
		apVal += aOutputSpan;
		redArr += aOutputSpan;
		apcSrc += aInputSpan;
	}

}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION 
