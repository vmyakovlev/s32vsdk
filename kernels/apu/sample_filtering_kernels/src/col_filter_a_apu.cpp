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
* \file col_filter_a_apu.cpp
* \brief Filtering with general filter image columns - implementation for APEX
* \author CGV
* \version
* \date
****************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION  

#ifdef APEX2_EMULATE
#include "apu_lib.hpp"
using namespace APEX2;
#endif

void col_filter(      vec08u* dst, int dstStride,
                const vec08u* src, int srcStride, int rows, int cols,
                const unsigned char* filter, int filterSize, int filterQ)
{
//! A 1 dimensional column filter.

//! [rounding]
/*! We are rounding, so we will shift by (filterQ-1), add 1, then shift one more. */
   --filterQ;     
//! [rounding]

//! [source offset]
/*! The source data is padded by ACF.  Offset the data pointer to the padded edge. */
   src -= (filterSize >> 1);
//! [source offset]

   for(int y=0; y<rows; ++y, dst += dstStride, src += srcStride)
   {  
//! [weighted sum]   
      const vec08u* s = src;
      for(int x=0; x<cols; ++x, ++s)
      {
         vec16u sum = 0;
         for (int c=0; c<filterSize; ++c)
         {
            sum += (vec16u)s[c] * filter[c];
         }
         dst[x] = (vec08u)(((sum >> filterQ)+1)>>1);
      }
//! [weighted sum]
   }
}

#endif   // #ifdef ACF_KERNEL_IMPLEMENTATION
