/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2015-2017 NXP
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
//
#include "blockmatching_apu.h"

// Local helper functions

/*!
   Calculate mimimum SAD & lcoation given a 8x8 template in a 32x32 window

   \param aMinSAD   - [Output] Destination minimum block SAD
   \param aLocation - [Output] Destination min SAD location: x, y
   \param aTemplate - [Input]  Source template block pointer
   \param aWindow   - [Input]  Source window block pointer
   \param aStart    - [Input]  Start position pointer
 */


inline
vec16u
apu_sad_full(
         const vec08u* aTemplate,   const int ts,
         const vec08u* aWindow,     const int ws,
         const int tw, const int th
         )
{
   vec16u s = 0;
      
   for (int y=0; y<th; ++y) chess_loop_range(1,) 
   {
      for (int x=0; x<tw; ++x) chess_loop_range(2,) 
                                 chess_unroll_loop(2)
      {
         vec16u d = (vec16u)vabs_diff((vec16s)aTemplate[x], (vec16s)aWindow[x]);
         s = vadd_sat(s, d);
      }
      aTemplate += ts;
      aWindow   += ws;
   }
   
   return s;
}

void 
apu_sad_full(     vec16u* aMinSAD,    
                  vec16s* aLocation, 
            const vec08u* aTemplate,   const int tw, const int th, const int ts,
            const vec08u* aWindow,     const int ww, const int wh, const int ws
            )
{

   vec16s xmin = 0x7FFF, ymin = 0x7FFF;
   vec16u smin = 0xFFFF;
   
   for (int y=0; y<(wh-th+1); ++y) chess_loop_range(1,) 
   {
      for (int x=0; x<(ww-tw+1); ++x) chess_loop_range(1,) 
      {
         vec16u s = apu_sad_full(aTemplate, ts, aWindow+x+y*ws, ws, tw, th);
         
         vif (s < smin)
         {
            smin = s;
            xmin = x;
            ymin = y;
            
         }
         vendif
      }
   }

   aMinSAD[0]   = smin;
   aLocation[0] = xmin;
   aLocation[1] = ymin;
}


#endif /* ACF_KERNEL_IMPLEMENTATION */
