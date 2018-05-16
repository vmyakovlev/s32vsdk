/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright 2013-2017 NXP
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
/*!
* \file fast_acf.cpp
* \addtogroup apexcv_pro
* \addtogroup fast
* \ingroup aprexcv_pro
* @{
* \brief Histogram
*/
#include <stdio.h>
#ifdef ACF_KERNEL_METADATA
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#include "fast_apu.h"

#ifndef APEX2_EMULATE
#endif

KERNEL_INFO _kernel_info_fast_offset
(
   "fastx_offsets",
   3,
    __port(__index(0),
          __identifier("OUTPUT_OFFSETS"),
          __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED), 
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32u),
          __e0_size(1, 1),
          __ek_size(16, 1)),
    __port(__index(1),
          __identifier("INPUT_0"),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)), 
    __port(  __index(2),
      __identifier("CIRCUMFERENCE"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1))
);

KERNEL_INFO _kernel_info_fast
(
   "fast",
   4,
    __port(__index(0),
      __identifier("OUTPUT_0"),
      __attributes(ACF_ATTR_VEC_OUT), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
    __port(__index(1),
      __identifier("INPUT_0"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(3,3,3,3),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)), 
    __port(  __index(2),
      __identifier("THRESHOLD"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
    __port(  __index(3),
      __identifier("OFFSET_TABLE"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1))
);

KERNEL_INFO _kernel_info_fast_serialized
(
   "fast_serialized",
   6,
    __port(__index(0),
    __identifier("INPUT"),
    __attributes(ACF_ATTR_VEC_IN),
    __spatial_dep(3,3,3,3),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(1, 1)), 
    
    __port(  __index(1),
    __identifier("THRESHOLD"),
    __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(1, 1)),
    
    __port(  __index(2),
    __identifier("OFFSET_TABLE"),
    __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d32u),
    __e0_size(1, 1),
    __ek_size(16, 1)),
    
    __port(__index(3),
    __identifier("OUT_PACKED"),
    __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d32u),
    __e0_size(1, 1),
    __ek_size(8192, 1)),
    
    __port(__index(4),
    __identifier("COUNTER"),
    __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d32s),
    __e0_size(1, 1),
    __ek_size(1, 1)),
    
    __port(__index(5),
    __identifier("OUT_MAX_SIZE"),
    __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d32s),
    __e0_size(1, 1),
    __ek_size(1, 1)));

KERNEL_INFO _kernel_info_fast_nms
(
   "fast_nms",
   4,
    __port(__index(0),
      __identifier("OUTPUT_0"),
      __attributes(ACF_ATTR_VEC_OUT), 
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
    __port(__index(1),
      __identifier("INPUT_0"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(3,3,3,3),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)), 
    __port(  __index(2),
      __identifier("THRESHOLD"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
    __port(  __index(3),
      __identifier("OFFSET_TABLE"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1))
);

/****************************************************************************/
/*!
   Non-maximum suppression 3x3 kernel metadata.
   
   \param 2                   Number of ports
   \param "Port NMS16_KN_IN"  Define for name of input image (unsigned 8bit)
   \param "Port NMS16_KN_OUT" Define for name of output image (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO _kernel_info_nms3x3
(
  "nms3x3",
  2,
  __port(__index(0),
      __identifier("input"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(1,1,1,1),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
  __port(__index(1),
      __identifier("output"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
#include "acf_kernel.hpp"
using namespace APEX2;
#endif

#include "fast_apu.h"

/*!*********************************************************************************
*  \brief ACF wrapper function for FAST
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/                                 
void fast(  
   kernel_io_desc lOut0,
   kernel_io_desc lIn0, 
   kernel_io_desc threshold,
   kernel_io_desc offsets
)
{
   vec08u*  lpvIn0      = (vec08u*)lIn0.pMem;
   vec08u*  lpvOut0     = (vec08u*)lOut0.pMem;
   int*     lpvThresh   = (int*)threshold.pMem;
   int*     lpvOffsets  = (int*)offsets.pMem;

   apu_fast(
      lpvOut0, lOut0.chunkSpan,
      lpvIn0, lIn0.chunkSpan, 
      lIn0.chunkWidth, lIn0.chunkHeight,
      *lpvThresh, lpvOffsets
   ); 
}

void fast_serialized(kernel_io_desc lIn0,
                     kernel_io_desc lThreshold,
                     kernel_io_desc lOffsets,
                     kernel_io_desc lOutPacked,
                     kernel_io_desc lGlobalCnt,
                     kernel_io_desc lOutBufferSize)
{
  vec08u * pvIn0 = (vec08u *)(lIn0.pMem);
  int * pOffsets = (int *)(lOffsets.pMem);
  unsigned short * pOutPacked = (unsigned short *)(lOutPacked.pMem);
  int * pGlobalCounter = (int *)(lGlobalCnt.pMem);

  int threshold = *(int *)(lThreshold.pMem);
  int maxOutSample = *(int *)(lOutBufferSize.pMem);
  int beginCu = (int)(ACF_RET_VAR(ACF_VAR_FIRST_CUID));
  int nrOfCus = (int)(ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS));

  if (ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
  {
    *pGlobalCounter = 0;
  }

  apu_fast_serialized(pOutPacked,
                      pGlobalCounter,
                      beginCu,
                      nrOfCus,
                      maxOutSample,
                      pvIn0,
                      (int) lIn0.chunkSpan,
                      (int) lIn0.chunkWidth,
                      (int) lIn0.chunkHeight,
                      (int) threshold,
                      pOffsets);
}

void fast_nms(  
   kernel_io_desc lOut0,
   kernel_io_desc lIn0, 
   kernel_io_desc threshold,
   kernel_io_desc offsets
)
{
   vec08u*  lpvIn0     = (vec08u*)lIn0.pMem;
   vec08u*  lpvOut0    = (vec08u*)lOut0.pMem;
   int*     lpvThresh     = (int*)threshold.pMem;
   int*     lpvOffsets  = (int*)offsets.pMem;

   apu_fast_nms(
      lpvOut0, lOut0.chunkSpan,
      lpvIn0, lIn0.chunkSpan, 
      lIn0.chunkWidth, lIn0.chunkHeight,
      *lpvThresh, lpvOffsets
   ); 
}

void fastx_offsets(
   kernel_io_desc offsets, 
   kernel_io_desc lIn0,
   kernel_io_desc circumference
)
{
   uint32_t*   lpsOffset  = (uint32_t*)offsets.pMem;
   int* lpsCircumference  = (int*)circumference.pMem;

  if(ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
  {
      apu_fastx_offset(lpsOffset, lIn0.chunkSpan, *lpsCircumference);
  }
}

void nms3x3(
    kernel_io_desc input,
    kernel_io_desc output
)
{
  vec08u* lpvInput = (vec08u*)input.pMem;
  vec08u* lpvOutput = (vec08u*)output.pMem;
  apu_nms(  lpvInput, 
            lpvOutput, 
            input.chunkSpan,
            output.chunkSpan, 
            input.chunkWidth, 
            input.chunkHeight);   
}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
