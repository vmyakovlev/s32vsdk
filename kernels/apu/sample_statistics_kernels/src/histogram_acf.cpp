/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
* Copyright (c) 2017 NXP
* 
* All Rights Reserved
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
/*!
* \file apu_histogram.cpp
* \addtogroup statistics
* \addtogroup histogram
* \ingroup statistics
* @{
* \brief Histogram
*/

#ifdef ACF_KERNEL_METADATA
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#include "histogram_acf.h"


/****************************************************************************/
/*!
   Histogram kernel metadata.
   
   \param HISTOGRAM_KN			  Define for Kernel name
   \param 2						  Number of ports
   \param "Port HISTOGRAM_KN_IN"  Define for name of input image (unsigned 8bit)
   \param "Port HISTOGRAM_KN_OUT" Define for name of histogram vector output, CU count X 256  (unsigned 32bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(HISTOGRAM_K)
(
  HISTOGRAM_KN,
  2,
  __port(__index(0),
    __identifier(HISTOGRAM_KN_IN),
    __attributes(ACF_ATTR_VEC_IN),
    __spatial_dep(0,0,0,0),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(1, 1)),
  __port(__index(1),
    __identifier(HISTOGRAM_KN_OUT),
    __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
    __spatial_dep(0,0,0,0),
    __e0_data_type(d32u),
    __e0_size(1, 1),
    __ek_size(256, 1))
);
#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
#include "acf_kernel.hpp"
using namespace APEX2;
#endif

#include "histogram_apu.h"


void apu_histogram(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
  vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
  vec32u* lpvOut0 = (vec32u*)lOut0.pMem;
  int lStrideIn0 = lIn0.chunkSpan;
  
  bool isFirstTile = ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG);
  hist(lpvIn0, lpvOut0, isFirstTile, lStrideIn0, lIn0.chunkWidth, lIn0.chunkHeight, lOut0.chunkWidth);

}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
