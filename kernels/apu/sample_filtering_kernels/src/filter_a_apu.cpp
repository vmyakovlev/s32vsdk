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
* \file filter_a_apu.cpp
* \brief Filtering with general filter 1D/2D implementation for APEX
* \author 
* \version
* \date
****************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION


#include "filter_a_apu.h"
#include "filter_a_cfg.h"

void apu_filter_a_impl(vec08u* lpvIn0, int16_t lStrideIn0,
				       uint8_t* lpInCoef,
				      vec08u* lpvOut0, int16_t lStrideOut0,
				      int16_t lChunkWidth, int16_t lChunkHeight)
{
   int16_t lSumWeights = 0;
   for (int16_t i=0; i<FILTER_W*FILTER_H; i++)
      lSumWeights += lpInCoef[i];

   int16_t lShift = 0;
   if      (lSumWeights == 1)  lShift = 0;
   else if (lSumWeights == 2)  lShift = 1;
   else if (lSumWeights == 4)  lShift = 2;
   else if (lSumWeights == 8)  lShift = 3;
   else if (lSumWeights == 16) lShift = 4;
   else if (lSumWeights == 32) lShift = 5;
   else if (lSumWeights == 64) lShift = 6;

   for (int16_t y=0; y<lChunkHeight; y++)
   {
      for (int16_t x=0; x<lChunkWidth; x++)
      {
         vec08u* lpvTmp = &lpvIn0[(y-(FILTER_H>>1))*lStrideIn0 + x - (FILTER_W>>1)];
         vec16u lvTmpRes = 0;
         for (int16_t y_flt=0; y_flt<FILTER_H; y_flt++)
         {
            for (int16_t x_flt=0; x_flt<FILTER_W; x_flt++)
            {
               lvTmpRes += (vec16u)(lpvTmp[y_flt*lStrideIn0+x_flt]*
                            lpInCoef[FILTER_W*y_flt+x_flt]);
            }
         }
         lpvOut0[y*lStrideOut0+x] = (vec08u)(lvTmpRes>>lShift);
      }
   }
}

#endif //ACF_KERNEL_IMPLEMENTATION
