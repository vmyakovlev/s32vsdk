
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
* \file gradient_apu.cpp
* \brief gradient implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/
#ifdef ACF_KERNEL_IMPLEMENTATION

#include "gradient_apu.h"

/*****************************************************************************
* functions
*****************************************************************************/

void apuGradient(vec16s* apcSobelX, vec16s* apcSobelY, const vec08u* apInput,
  int aBlockWidth, int aBlockHeight, int aStride)
{
  for (int y = 0; y < aBlockHeight; ++y)
  {
    for (int x = 0; x < aBlockWidth; ++x)
    {

      vec16s sx = vec16s((vec08s)apInput[x - aStride - 1]) - vec16s((vec08s)apInput[x - aStride + 1]) +
                  ((vec16s)((vec08s)apInput[x - 1]) << 1) - ((vec16s)((vec08s)apInput[x + 1]) << 1) +
                  vec16s((vec08s)apInput[x + aStride - 1]) - vec16s((vec08s)apInput[x + aStride + 1]);

      vec16s sy = vec16s((vec08s)apInput[x - aStride - 1]) + ((vec16s)((vec08s)apInput[x - aStride]) << 1)  + vec16s((vec08s)apInput[x - aStride + 1]) +
                  vec16s((vec08s)-apInput[x + aStride - 1]) - ((vec16s)((vec08s)apInput[x + aStride]) << 1)  - vec16s((vec08s)apInput[x + aStride + 1]);

      apcSobelX[x] = sx;
      apcSobelY[x] = sy;
    }

    apInput += aStride;
    apcSobelX += aBlockWidth;
    apcSobelY += aBlockWidth;
  }
}



void apuGradient_out08s(vec08s* apcSobelX, vec08s* apcSobelY, const vec08u* apInput,
	int aBlockWidth, int aBlockHeight, int aStride)
{
	for (int y = 0; y < aBlockHeight; ++y)
	{
		for (int x = 0; x < aBlockWidth; ++x)
		{

			vec16s sx = vec16s(apInput[x - aStride - 1]) - vec16s(apInput[x - aStride + 1]) +
					((vec16s)(apInput[x - 1]) << 1) - ((vec16s)(apInput[x + 1]) << 1) +
					vec16s(apInput[x + aStride - 1]) - vec16s(apInput[x + aStride + 1]);

			vec16s sy = vec16s(apInput[x - aStride - 1]) + ((vec16s)(apInput[x - aStride]) << 1) + vec16s(apInput[x - aStride + 1]) +
				vec16s(-apInput[x + aStride - 1]) - ((vec16s)(apInput[x + aStride]) << 1) - vec16s(apInput[x + aStride + 1]);

			apcSobelX[x] = vec08s(sx>>3);
			apcSobelY[x] = vec08s(sy>>3);
		}

		apInput += aStride;
		apcSobelX += aBlockWidth;
		apcSobelY += aBlockWidth;
	}
}


void apuGradientAbs(vec08s* apcSobelX, vec08s* apcSobelY, vec08u* apcAbsSum, const vec08u* apInput,
	int aBlockWidth, int aBlockHeight, int aStride)
{
	for (int y = 0; y < aBlockHeight; ++y)
	{
		for (int x = 0; x < aBlockWidth; ++x)
		{
			/*
			vec16s sx = vec16s(apInput[x - aStride - 1]) - vec16s(apInput[x - aStride + 1]) +
				((vec16s)(apInput[x - 1]) << 1) - ((vec16s)(apInput[x + 1]) << 1) +
				vec16s(apInput[x + aStride - 1]) - vec16s(apInput[x + aStride + 1]);
			sx >>= 3;

			vec16s sy = vec16s(apInput[x - aStride - 1]) + ((vec16s)(apInput[x - aStride]) << 1) + vec16s(apInput[x - aStride + 1]) +
				vec16s(-apInput[x + aStride - 1]) - ((vec16s)(apInput[x + aStride]) << 1) - vec16s(apInput[x + aStride + 1]) ;
			sy >>= 3;
		*/
			vec16s sx = ((vec16s)apInput[x - 1] - (vec16s)apInput[x + 1]) >> 1;

			vec16s sy = ((vec16s)apInput[x - aStride] - (vec16s)apInput[x + aStride]) >> 1;

			apcSobelX[x] = vec08s(sx);
			apcSobelY[x] = vec08s(sy);
			apcAbsSum[x] = vec08u(vabs(sx) + vabs(sy));
		}

		apInput += aStride;
		apcSobelX += aBlockWidth;
		apcSobelY += aBlockWidth;
		apcAbsSum += aBlockWidth;
	}
}



void apuGradAbs(vec08u* apcAbsSum, const vec08u* apInput,
	int aBlockWidth, int aBlockHeight, int aStride)
{
	for (int y = 0; y < aBlockHeight; ++y)
	{
		for (int x = 0; x < aBlockWidth; ++x)
		{
			vec16s sx = vec16s(apInput[x - aStride - 1]) - vec16s(apInput[x - aStride + 1]) +
				((vec16s)(apInput[x - 1]) << 1) - ((vec16s)(apInput[x + 1]) << 1) +
				vec16s(apInput[x + aStride - 1]) - vec16s(apInput[x + aStride + 1]);
			sx >>= 3;

			vec16s sy = vec16s(apInput[x - aStride - 1]) + ((vec16s)(apInput[x - aStride]) << 1) + vec16s(apInput[x - aStride + 1]) +
				vec16s(-apInput[x + aStride - 1]) - ((vec16s)(apInput[x + aStride]) << 1) - vec16s(apInput[x + aStride + 1]);
			sy >>= 3;

			apcAbsSum[x] = vec08u(vabs(sx) + vabs(sy));
		}

		apInput += aStride;
		apcAbsSum += aBlockWidth;
	}
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION 
