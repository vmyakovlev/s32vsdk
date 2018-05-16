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
* \file histofgrad_apu.cpp
* \brief histogram of gradients implementation for APEX
* \author Anca Dima
* \version
* \date
****************************************************************************/

#include "histofgrad_apu.h"
#include "histofgrad_defines.h"

#ifdef APEX2_EMULATE
#include "apu_vec.hpp"
#endif

/* Divide each circle quarter into 4 sectors (i.e of ~22.5 degrees) ==> 16 sectors for the whole circle
___________
DY >= 0:
___________
dx >= 0:
		---	   tan(0)				==> dx != 0, dy == 0; 
Bin1	|	   
		|	   tan(11.25) = 0.1989
dx > dy	|	   tan(22.50) = 0.4142	==> dy/dx < 0.4142	=> 2.4143 * dy == dx
		|	-> tan(26.56) = 0.5		==> dy/dx < 0.5		=> 2 * dy < dx (for lower angles)
Bin2	|	   tan(33.75) = 0.6682
		|	
		--	-> tan(45.00) = 1		==> dx == dy		=> dy < dx
		|
Bin3	|	   tan(56.25) = 1.4966
		|	-> tan(63.43) = 2.0		==> dy/dx < 2		=> dy < 2*dx
dx < dy	|	   tan(67.50) = 2.4142
		|	   tan(78.75) = 5.0273
Bin4	|
		---	-> tan(90.00) = 0		==> dx = 0, dy != 0; => dx < dy

dx < 0:
Bin5   dy > abs(dx)   to   dy == abs(2*dx)
Bin6   dy < abs(2*dx) to   dy == dx 
Bin7   dy < abs(dx)   to 2*dy == abs(dx)
Bin8 2*dy < abs(dx)   to   dy == 0;

_________
DY <= 0:
_________
dx >= 0
		---	   tan(0)					==> dx != 0, dy == 0;
Bin9	|
		|	   tan(-11.25) = -0.1989
dx > dy	|	-> tan(-22.50) = -0.4142	==> abs(dy)/dx < 0.4142	=> 2.4143 * abs(dy) == dx
		|	   tan(-26.56) = -0.5		==> abs(dy)/dx < 0.5	=> 2 * abs(dy) == dx
Bin10	|	   tan(-33.75) = -0.6682
		|
		--	-> tan(-45.00) = -1			==> dx == abs(dy)		=> abs(dy) < dx
		|
Bin11	|	   tan(-56.25) = -1.4966
		|	   tan(-63.43) = -2.0		==> dy/dx < 2			=> abs(dy) < 2*dx (for angles below)
dx < dy	|	-> tan(-67.50) = -2.4142
		|	   tan(-78.75) = -5.0273
Bin12	|
		---	-> tan(-90.00) = 0		==> dx = 0, dy != 0; => dx < abs(dy)

dx < 0:
Bin13   abs(dy) > abs(dx)   to   abs(dy) == abs(2*dx)
Bin14   abs(dy) < abs(2*dx) to   abs(dy) == dx
Bin15   abs(dy) < abs(dx)   to 2*abs(dy) == abs(dx)
Bin16 2*abs(dy) < abs(dx)   to   abs(dy) == 0;

The input 4x4 bricks, for which the hog is computed, can lead to values up to 16, thus each histogram entry needs 4bits. We will two unsigned 32bit value for each of the dy>0 and dy < 0 sectors.
*/



/*****************************************************************************
* functions
*****************************************************************************/
/* Compute for each tile a HOG (16 bins for 360, with 4 overlaps). DO NOT COUNT THE ZERO Gradients (i.e. GX == GY == 0)
*/
void hog(vec08s* lpvInGradX, vec08s* lpvInGradY, vec16u* lpvOut, vec32u* lpvOutBlNorm, int lStrideIn, int chunkWidth, int chunkHeight, int lStrideOut)
{
  vec16u *out = lpvOut;
  vec08s *gradX = lpvInGradX;
  vec08s *gradY = lpvInGradY;

  //vec32u mask = chunkWidth*chunkHeight - 1; // one bin can have maximally value: bw*bh  (in our case 16, but 15 is representable on 4 bits)
  //const int nrScales = 2;
  //int16u smoothScale[nrScales] = { 1, 2 }; // consider mean of gradients over 1x1 and 2x2 bricks
  int16u overlapStep = HOG_OVERLAP;
  
  //initialize the vector output buffer to zero
  for (int i = 0; i < HOG_NR_FEATURES_PER_BOX; ++i) {
	  out[i] = 0;
  }

  //int right = (chunkWidth << 1) - 1;
  //int bottom = ((chunkHeight << 1) - 1) * lStrideIn, botRight = bottom + right;
  //vec32u globalNorm = lpvInAbsGradX[0] - lpvInAbsGradX[right] + lpvInAbsGradX[botRight] - lpvInAbsGradX[bottom];
  //vec32u globalYNorm = lpvInAbsGradY[0] - lpvInAbsGradY[right] + lpvInAbsGradY[botRight] - lpvInAbsGradY[bottom];
  *lpvOutBlNorm = 1;// globalXNorm + globalYNorm + 1;
  vec32u secScaleNorm = 1;
  out = lpvOut;
  for (int16s y=0; y < chunkHeight + HOG_OVERLAP; ++y)
  {
	for (int16s x = 0; x < chunkWidth + HOG_OVERLAP; ++x)
	{
		int xMod = (x / overlapStep > 0) ? 1 : 0;
		int yMod = (y / overlapStep > 0) ? 1 : 0;
		int xMin = x / chunkWidth;
		int yMin = y / chunkHeight;
		vec16s gX = vec16s(gradX[x]);
		vec16s gY = vec16s(gradY[x]);

		if (!vany(gX != 0 || gY != 0)) {
			continue;
		}

		vec16u crtGradNorm = (vec16u) vabs(gX) + (vec16u) vabs(gY);//vsqrt_32(vec32u(gX*gX + gY*gY));
		*lpvOutBlNorm += (vec32u) crtGradNorm;

		//for (int16s k = 0; k < nrScales; ++k)
		{
			vec16s shift = 0;
			vec16s increm = 1; // 4;
			/*int16s vSmoothIdx = lStrideIn * smoothScale[k], hSmoothIdx = x +smoothScale[k];
			
			if (k != 0) {
				gX += vec32s(gradX[x + 1]) + vec32s(gradX[lStrideIn + x]) + vec32s(gradX[lStrideIn + x + 1]);
				gX >>= 2;//- lpvInGradX [hSmoothIdx]+ lpvInGradX[vSmoothIdx + hSmoothIdx] - lpvInGradX[vSmoothIdx];
				gY += vec32s(gradY[x + 1]) + vec32s(gradY[lStrideIn + x]) + vec32s(gradY[lStrideIn + x + 1]);
				gY >>= 2; //- lpvInGradY[hSmoothIdx] + lpvInGradY[vSmoothIdx + hSmoothIdx] - lpvInGradY[vSmoothIdx];

				if (x == chunkWidth + OVERLAP - 1) { // We are at the right edge and look over the edge
					*lpvOutBlNorm += vabs(vec32s(gradX[x + 1])) + vabs(vec32s(gradY[x + 1]));
				}
				if (y == chunkWidth + OVERLAP - 1) {
					*lpvOutBlNorm += vabs(vec32s(gradX[lStrideIn + x])) + vabs(vec32s(gradY[lStrideIn + x]));
					if (x == chunkWidth + OVERLAP - 1) {
						*lpvOutBlNorm += vabs(vec32s(gradX[lStrideIn + x + 1])) + vabs(vec32s(gradY[lStrideIn + x + 1]));
					}
				}
			}
			*/
			vec32s initGY = (vec32s)gY; 
			gY = (vec16s) vabs(gY);

			//vec32u crtOut;
			vif(gX < 0) {
				//shift += 28; // write the the high 16bit word for bins 4-7 (i.e. 90-180)
				//increm = -4;
				shift = 7;
				increm = -1;
				gX = (vec16s) vabs(gX);
			} vendif


			vif(gX < gY) { // between 45-90 or 90-135 degrees
				shift += increm << 1;
				vif((gX << 1) <= gY) { // between 60-90
					shift += increm;
				}vendif
			} velse{
				vif((gY << 1) >= gX && gY != 0) { // between 30-45
					shift += increm;
				}vendif
			}vendif

			vec16u cOut;
			for (int l = yMin; l <= yMod; ++l) {
				for (int m = xMin; m <= xMod; ++m) {
					int16s ox = int16s(HOG_NR_FEATURES_PER_HISTO) * (m + (l << 1)); //((l << 2) + (m << 1)) +(k << 1);
					
					vif(initGY >= 0) {
						vif(!(initGY == 0 && gX == 0)) {
							//out[ox + shift] += crtGradNorm;
							cOut = vload(out, ox + shift);
							cOut += crtGradNorm;
							vstore(out, ox + shift, cOut);
						}vendif
					}velse{
						//out[ox + NR_FEATURES_PER_HISTO - shift - 1] += crtGradNorm; 	
						cOut = vload(out, vec16s(ox + int16s(HOG_NR_FEATURES_PER_HISTO) - shift - int16s(1)));
						cOut += crtGradNorm;
						vstore(out, vec16s(ox + int16s(HOG_NR_FEATURES_PER_HISTO) - shift - int16s(1)), cOut);
					}vendif
					
					/*
					vif(initGY >= 0) {
						crtOut = out[ox ];
					}velse{
						crtOut = out[ox +1];
					}vendif

					vec32u cO = 0xF & vsrl(crtOut, shift); //vsr(crtOut, shift) + 1;
								
					vif(cO < mask) {
						cO = (cO + 1) << shift;
					}vendif
					
					vec32u shiftedMask = vnot(vec32u(0xF) << shift);

					crtOut = (crtOut & shiftedMask) | cO;
					
					vif(!(gX == 0 && gY == 0)) {
						vif(initGY >= 0) {
							out[ox] = crtOut;
						}velse{
							out[ox + 1] = crtOut; 
						}vendif
					}vendif*/
				}
			}

		}
    }
	gradX += lStrideIn;
	gradY += lStrideIn;
	//out += lStrideOut; one per Stride, accumulates all histograms
  }
  // NOTE: here the out vector should be divided by the norm. However, since outside a division has to be made anyway, the division is left to be performed outside
  // i.e there should be still following code:
  /* for (int i = 0; i < NR_FEATURES_PER_BOX; ++i) {
		out[i] <<= 4; //(on a 4x4 patch the values are < 2^12, thus for the division, those can be scaled up)
		out[i] /= *lpvOutBlNorm;
  }
  */
}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
