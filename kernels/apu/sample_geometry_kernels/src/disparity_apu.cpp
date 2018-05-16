#ifdef ACF_KERNEL_IMPLEMENTATION

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
* \file disparity_apu.cpp
* \brief add implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/

#ifdef APEX2_EMULATE
#include "apu_vec.hpp"
#endif

#include "disparity_apu.h"

/*****************************************************************************
* functions
*****************************************************************************/

void disparity(vec08u* dst, vec08u* srcImage0, vec08u* srcImage1, int bw, int bh, int cw, int ch, int inStrideW0, int inStrideW1, int outStrideW)
{
	vec08u a[9];
	vec08u b[9];
	vec08u c[9];
	vec08u current_sad = 255;
	vec08u new_sad = 0;
	vec08u disp = 0;
	vec08u diff = 0;

	int cx;
	vec08u ave_a, ave_b, sum_a, sum_b;
	int pc=0;
	int blocksize = 9;

	for (int by = 0; by < bh; by+=1)  // y data row
	{
		for (int bx = 0; bx < bw; bx+=1) // x in the blck_tile
		{

			for (cx = 0; cx < 60;){
				for (int px = -1; px < 2; ++px){
					for (int py = -1; py < 2; ++py){

						a[pc] = srcImage0[bx + (inStrideW0 * py) + px];
						b[pc] = srcImage1[bx + (inStrideW1 * py) + px + cx];
						pc++;

					}
				}

				/* for Average calculation */
				/*
				for (int ai = 0; ai < blocksize; ai++){
					sum_a = sum_a + a[ai];
					sum_b = sum_b + b[ai];
				}

				ave_a = sum_a / blocksize;
				ave_b = sum_b / blocksize;
				*/

				for (int i = 0; i < blocksize; i++){

					vif(b[i] >= a[i]){
						//	c[i] = (b[i] - ave_b) - (a[i] - ave_a);
						c[i] = b[i] - a[i];
					} velse{
						//	c[i] = (a[i] - ave_a) - (b[i] - ave_b);
						c[i] = a[i] - b[i];
					} vendif

						new_sad = new_sad + c[i];
				}

				vif(new_sad < current_sad){
					current_sad = new_sad;
					dst[bx] = cx;
				} velse{
					current_sad = current_sad;
				} vendif

				new_sad = 0;
				pc = 0;
				ave_a = 0;
				ave_b = 0;
				
				if (cx < 3) cx++;
				else if (cx < 10) cx += 2;
				else if (cx < 30) cx += 4;
				else cx += 6;
			}

			// Weak Pattern Removal
			vif(srcImage0[bx] >= srcImage0[bx - 1]){
				diff = srcImage0[bx] - srcImage0[bx - 1];
			} velse{
				diff = srcImage0[bx - 1] - srcImage0[bx];
			} vendif

			vif(diff < 2){
				dst[bx] = 0;
			} velse{
				dst[bx] = dst[bx];
			} vendif

			//dst[bx] = 255;
			current_sad = 255;
			new_sad = 0;
			
		}

		dst += outStrideW;
		srcImage0 += inStrideW0;
		srcImage1 += inStrideW1;
	}

}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION 
