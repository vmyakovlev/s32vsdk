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
* \file row_filter_a_apu.cpp
* \brief Filtering with general filter image rows - implementation for APEX
* \author CGV
* \version
* \date
****************************************************************************/


#ifdef ACF_KERNEL_IMPLEMENTATION

#ifdef APEX2_EMULATE
#include "apu_lib.hpp"
using namespace APEX2;
#endif

/*! Fixed point rounding flag. */
#define ROW_FILTER_ROUND 1    

void row_filter_impl(      vec08u* dst, int dstStride,
                const vec08u* src, int srcStride, int rows, int cols,
                const unsigned char* filter, int filterSize, int filterQ)
{
   // A 1 dimensional vertical filter.
   
#if (ROW_FILTER_ROUND)
   --filterQ;
#endif
   
   // The source data is padded by ACF.  Offset the data pointer to the padded edge.
   src -= srcStride*(filterSize >> 1);

   for(int y=0; y<rows; ++y, dst += dstStride, src += srcStride)
   {      
      const vec08u* s = src;
      for(int x=0; x<cols; ++x, ++s)
      {
         // Apply the vertical filters for this column
         vec16u sum = 0;
         for (int r=0; r<filterSize; ++r)
         {
            sum += (vec16u)s[r*srcStride] * filter[r];
         }
         
#if (ROW_FILTER_ROUND)
         dst[x] = (vec08u)(((sum >> filterQ)+1) >> 1);
#else
         dst[x] = (vec08u)(sum >> filterQ);
#endif

      }
   }
}

#endif   // #ifdef ACF_KERNEL_IMPLEMENTATION
