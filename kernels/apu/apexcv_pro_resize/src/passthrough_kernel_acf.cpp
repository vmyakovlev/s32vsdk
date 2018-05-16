/**************************************************************************************************
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
 **************************************************************************************************/
#ifdef ACF_KERNEL_METADATA

#include "resize_definitions_apu.h"


KERNEL_INFO _kernel_info_passthrough16_kernel
(
   "passthrough16_kernel",
   2,
   __port(__index(0),
          __identifier("INPUT_0"),
          __attributes(ACF_ATTR_IN),
          __spatial_dep(0,0,0,8), //{left, right, top, bottom}
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier("OUTPUT_0"),
          __attributes(ACF_ATTR_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);


KERNEL_INFO _kernel_info_passthrough_kernel
(
   "passthrough_kernel",
   2,
   __port(__index(0),
          __identifier("INPUT_0"),
          __attributes(ACF_ATTR_IN),
          __spatial_dep(0,0,0,8), //{left, right, top, bottom}
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier("OUTPUT_0"),
          __attributes(ACF_ATTR_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

void passthrough_kernel (kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

   int lChunkWidth  = lIn0.chunkWidth;
   int lChunkHeight = lIn0.chunkHeight;
   int lStrideIn0   = lIn0.chunkSpan;   // 8bbp
   int lStrideOut0  = lOut0.chunkSpan;  // 8bpp

   //KERNEL IMPLEMENTATION - copy
   for (int y=0; y<lChunkHeight; y++) {
      for (int x=0; x<lChunkWidth; x++) {
         lpvOut0[x] = lpvIn0[x];
      }
      
      lpvIn0 += lStrideIn0;
      lpvOut0 += lStrideOut0;
   }
}

void passthrough16_kernel (kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec16u* lpvIn0  = (vec16u*)lIn0.pMem;
   vec16u* lpvOut0 = (vec16u*)lOut0.pMem;

   int lChunkWidth  = lIn0.chunkWidth;
   int lChunkHeight = lIn0.chunkHeight;
   int lStrideIn0   = lIn0.chunkSpan/2;   // 16bbp
   int lStrideOut0  = lOut0.chunkSpan/2;  // 16bpp

   //KERNEL IMPLEMENTATION - copy
   for (int y=0; y<lChunkHeight; y++) {
      for (int x=0; x<lChunkWidth; x++) {
         lpvOut0[x] = lpvIn0[x];
      }

      lpvIn0 += lStrideIn0;
      lpvOut0 += lStrideOut0;
   }
}


#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
