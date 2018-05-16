/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2013-2017 NXP
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
****************************************************************************/

 #ifdef ACF_KERNEL_IMPLEMENTATION 
#include "histogram_equalization_apu.h"
#include <string.h>


#ifdef APU_HISTOGRAM_EQUALIZATION_DEBUG
#include <stdio.h>
#include "apu_io.h"
#endif

 void apu_generate_lut_impl
(
   uint8_t* lut,           
   uint32_t* hist,
   uint32_t numPixels 
)
{
   int i = 0; 
   int tgtBin = 0;
   uint32_t sum = 0;
   uint32_t divisor;
   uint32_t compareValue;
   uint32_t firstBin = hist[0];
   while (!firstBin)
   {
      firstBin = hist[++i];
   }
   lut[i] = 0;
   i++;
   /* Code below implements without using division:
   lut[i] = (uint8_t)((float)(sum * 255)/(numPixels - firstBin) + .5);
   */
   divisor = numPixels - firstBin;
   compareValue = (divisor + 1) >> 1;
   for(; i < 256; i++)
   {
      sum += hist[i] * 255;
      while(sum >= compareValue)
      {
          compareValue += divisor;
          tgtBin++;
      }
      lut[i] = tgtBin;     
   }

}

void apu_histogram_equalization_impl
(
   vec08u*  dst,
   int dstr,
   vec08u*  src,     
   int sstr,         
   int bw,
   int bh, 
   vec08u* lut
)
{
#if (defined(APEX2_EMULATE) || (!defined(__chess__)))
   for(int i = 0; i < (bw * bh); i++)
   {
     dst[i] = vload(lut, src[i]);
   }
#else
   for (int y = 0; y < bh; ++y) chess_loop_range(1,)
   {
      vec08u srcData = src[0];
      vec08u dstData = (vec08u)lut[srcData];
      for(int x = 1; x < bw; ++x) chess_loop_range(1,)
      {
         srcData = src[x];
         dst[x - 1] = dstData;
         dstData = (vec08u)lut[srcData];
      }
      dst[bw - 1] = dstData;
      src += sstr;
      dst += dstr;
   }
#endif
}


#endif   // #ifdef ACF_KERNEL_IMPLEMENTATION 
