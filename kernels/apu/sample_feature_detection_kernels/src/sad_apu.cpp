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
* \file sad_apu.cpp
* \brief sad implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/
#include "sad_apu.h"


#ifdef APEX2_EMULATE
#include "apu_vec.hpp"
#endif
/*!
   Calculate mimimum SAD & location given a 8x8 template in a 32x32 window

   \param aMinSAD   - [Output] Destination minimum block SAD
   \param aLocation - [Output] Destination min SAD location: x, y
   \param aTemplate - [Input]  Source template block pointer
   \param aWindow   - [Input]  Source window block pointer
   \param aStart    - [Input]  Start position pointer
 */

inline
vec16u
apu_sad_full(
         const vec08u* aTemplate,   const int32_t ts,
         const vec08u* aWindow,     const int32_t ws,
         const int32_t tw, const int32_t th
         )
{
   vec16u s = 0;

   for (int32_t y=0; y<th; ++y)
   {
      for (int32_t x=0; x<tw; ++x)
      {
         vec16s d = ((vec16s) aTemplate[x]) - ((vec16s) aWindow[x]);
         #if 1
         s += vabs(d);
         #else
         vif (d < 0)
             s -= d;
         velse
             s += d;
         vendif
         #endif

      }
      aTemplate += ts;
      aWindow   += ws;
   }

   return s;
}


void apu_sad_impl (vec08u* lpvIn0, int16_t lStrideIn0, int16_t lChunkWidthIn0, int16_t lChunkHeightIn0,
              vec08u* lpvIn1, int16_t lStrideIn1, int16_t lChunkWidthIn1, int16_t lChunkHeightIn1,
              vec32u* lpvOut0, int16_t /*lStrideOut0*/, int16_t /*lChunkWidthOut0*/, int16_t /*lChunkHeightOut1*/)
{

   //implementation
   vec08u xmin = 0xFF, ymin = 0xFF;
   vec16u smin = 0xFFFF;

   //vec16u* aMinSAD = (vec16u*)lpvOut0;

   //vec08u* aLocation0 = (((vec08u*)lpvOut0) + 2);
   //vec08u* aLocation1 = (((vec08u*)lpvOut0) + 3);

   vec08u* aMinSAD_lo = (((vec08u*)lpvOut0) + 0);
   vec08u* aMinSAD_hi = (((vec08u*)lpvOut0) + 1);
   vec08u* aLocation2 = (((vec08u*)lpvOut0) + 2);
   vec08u* aLocation3 = (((vec08u*)lpvOut0) + 3);
   
   vec08u* aTemplate = lpvIn0;
   vec08u* aWindow   = lpvIn1;

   const int32_t tw = lChunkWidthIn0;
   const int32_t th = lChunkHeightIn0;
   const int32_t ts = lStrideIn0;

   const int32_t ww = lChunkWidthIn1;
   const int32_t wh = lChunkHeightIn1;
   const int32_t ws = lStrideIn1;

#if 1
   for (int y=0; y<=(wh-th); ++y) chess_loop_range(1,)
   {
      for (int x=0; x<=(ww-tw); ++x) chess_loop_range(1,)
      {
		  vec16u s = apu_sad_full(aTemplate, ts, aWindow + x + y*ws, ws, tw, th); 

         vif (s < smin)
         {
            smin = s;
            xmin = x;
            ymin = y;
         }
         vendif
      }
   }

   aMinSAD_lo[0] = (vec08u)(smin & 0x00FF);
   aMinSAD_hi[0] = (vec08u)((smin >> 8) & 0x00FF);
   aLocation2[0] = xmin;
   aLocation3[0] = ymin;

  // aMinSAD[0]    = smin;
  // aLocation0[0] = xmin;
  // aLocation1[0] = ymin;
#endif
}

#endif //ACF_KERNEL_IMPLEMENTATION
