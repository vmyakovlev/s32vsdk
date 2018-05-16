/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2017 NXP
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
* \file histogram_acf.cpp
* \addtogroup apexcv-histogram
* \addtogroup histogram
* \ingroup histogram
* @{
* \brief histogram operations mimicking opencv
*/

/*!*********************************************************************************
*  @file histogram.c
*  @brief ACF metadata and wrapper function for histogram kernels
***********************************************************************************/

#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA
#include "histogram_acf.h"

/*!*********************************************************************************
*  \brief ACF metadata for histogram
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/

KERNEL_INFO  kernelInfoConcat(OPTIM_HISTOGRAM_K)
(
   OPTIM_HISTOGRAM_KN,
   3,
   __port(__index(0),
          __identifier(INPUT_0),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),     
   __port(__index(1),
          __identifier(L_HIST),
          __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED), 
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(256, 1)),          
   __port(__index(2),
          __identifier(OUTPUT_0),
          __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED), 
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32u),
          __e0_size(1, 1),
          __ek_size(256, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "histogram_apu.h"

/*!*********************************************************************************
*  \brief ACF wrapper function for histogram
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/                                 
void histogram(kernel_io_desc lIn0, kernel_io_desc llhist, kernel_io_desc lOut0)
{
   vec08u*   lpvIn0  = (vec08u*)lIn0.pMem;
   vec16u*   lplhist = (vec16u*)llhist.pMem;
   uint32_t* lpvOut0 = (uint32_t*)lOut0.pMem;
  
   int16_t lBeginCU = ACF_RET_VAR(ACF_VAR_FIRST_CUID);
   int16_t lEndCU = lBeginCU + ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS);
  
   if(ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG)) {
      apu_hist_init(lplhist,lpvOut0);
   }

   apu_hist1_l(lplhist, lpvIn0, lIn0.chunkSpan, lIn0.chunkWidth, lIn0.chunkHeight);
  
   if(ACF_RET_VAR(ACF_VAR_LAST_TILE_FLAG)) {
      apu_hist1_gn(lpvOut0, lplhist, lBeginCU, lEndCU);
   }
}
   
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
/*! @} */
