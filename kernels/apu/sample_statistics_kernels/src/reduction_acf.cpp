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
* \file reduction_acf.cpp
* \addtogroup statistics
* \addtogroup reduction Reduction
* \ingroup statistics
* @{
* \brief Vector to scalar reduction
*/
#ifdef ACF_KERNEL_METADATA
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#include "reduction_acf.h"


/****************************************************************************/
/*!
   256 vectors to 256 scalars accumulation reduction.
   
   \param REDUCTION_KN				Define for Kernel name
   \param 2							Number of ports
   \param "Port REDUCTION_KN_IN"	Define for name of vector input (unsigned 32bit)
   \param "Port REDUCTION_KN_OUT"	Define for name of scalar output (unsigned 32bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(REDUCTION_K)
(
  REDUCTION_KN,
  2,
  __port(__index(0),
    __identifier(REDUCTION_KN_IN),
    __attributes(ACF_ATTR_VEC_IN_STATIC_FIXED),
    __spatial_dep(0,0,0,0),
    __e0_data_type(d32u),
    __e0_size(1, 1),
    __ek_size(256, 1)),
  __port(__index(1),
    __identifier(REDUCTION_KN_OUT),
    __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
    __spatial_dep(0,0,0,0),
    __e0_data_type(d32u),
    __e0_size(1, 1),
    __ek_size(256, 1))
);
#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#ifdef APEX2_EMULATE 
#include "acf_kernel.hpp"
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

#include "reduction_apu.h"

void apu_reduction(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
    bool isLastTile = ACF_RET_VAR(ACF_VAR_LAST_TILE_FLAG);
    vec32u* lpvIn0 = (vec32u*)lIn0.pMem;
    int32s* lpOut0 = (int32s*)lOut0.pMem;
    int lChunkWidth = lIn0.chunkWidth;
    int lChunkHeight = lIn0.chunkHeight;
    int lChunkSpanIn0 = lIn0.chunkSpan>>2;
    int lChunkSpanOut0 = lOut0.chunkSpan>>2;
	
    int16s lFirstCuId = ACF_RET_VAR(ACF_VAR_FIRST_CUID);
    int16s lTileWidthInChunks = ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS);

	reduc(lpvIn0, lpOut0, isLastTile, lFirstCuId, lTileWidthInChunks, lChunkWidth, lChunkHeight, lChunkSpanIn0, lChunkSpanOut0);
 
}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
