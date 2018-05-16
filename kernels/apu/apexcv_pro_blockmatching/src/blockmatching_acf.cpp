/**************************************************************************************************
 * 
 * NXP Confidential Proprietary
 *
 * Copyright 2015-2017 NXP 
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
// Do not modify without modifying blockmatching header
#define DEFAULT_TEMPLATE_SIZE 16
#define DEFAULT_WINDOW_SIZE 28
KERNEL_INFO _kernel_info_apu_bm_sad
(
   "bm_sad",
   4,
   __port(__index(0),
          __identifier("INPUT_TEMPLATE"),
          __attributes(ACF_ATTR_VEC_IN_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(DEFAULT_TEMPLATE_SIZE, DEFAULT_TEMPLATE_SIZE)),
   __port(__index(1),
          __identifier("INPUT_WINDOW"),
          __attributes(ACF_ATTR_VEC_IN_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(DEFAULT_WINDOW_SIZE, DEFAULT_WINDOW_SIZE)),
   __port(__index(2),
          __identifier("OUTPUT_SCORE"),
          __attributes(ACF_ATTR_VEC_OUT_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(3),
          __identifier("OUTPUT_POINTS"),
          __attributes(ACF_ATTR_VEC_OUT_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16s),
          __e0_size(2, 1),
          __ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "blockmatching_apu.h"

void bm_sad (kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0, kernel_io_desc lOut1)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvIn1  = (vec08u*)lIn1.pMem;
   vec16u* lpvOut0 = (vec16u*)lOut0.pMem;
   vec16s* lpvOut1 = (vec16s*)lOut1.pMem;

   apu_sad_full(lpvOut0, lpvOut1,
                lpvIn0, lIn0.chunkWidth, lIn0.chunkHeight, lIn0.chunkSpan,
                lpvIn1, lIn1.chunkWidth, lIn1.chunkHeight, lIn1.chunkSpan);
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
