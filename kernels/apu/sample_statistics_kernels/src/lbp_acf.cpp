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
* \file lbp_acf.cpp
* \addtogroup statistics
* \addtogroup lpb
* \ingroup statistics
* @{
* \brief Linear Binary Pattern
*/

#ifdef ACF_KERNEL_METADATA
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#include "lbp_acf.h"
#include "lbp_defines.h"


/****************************************************************************/
/*!
   Histogram of Linear Binary Pattern kernel metadata.
   
   \param LBP_KN			  Define for Kernel name
   \param 3				  Number of ports
   \param "Port LBP_KN_INImg  		Define for name of input image (signed 32bit)
   \param "Port LBP_KN_OUT"		 Define for name of lpb vector output, CU count X 64 bins of 4bits each (i.e. 2 x unsigned 32bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(LBP_K)
(
  LBP_KN,
  3,
  __port(__index(0),
	__identifier(LBP_KN_InImg),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(LBP_LAT_DEPENDENCY, LBP_LAT_DEPENDENCY, LBP_LAT_DEPENDENCY, LBP_LAT_DEPENDENCY),
	__e0_data_type(d08u),
	__e0_size(LBP_LAT_DEPENDENCY, LBP_LAT_DEPENDENCY),
	__ek_size(1, 1)),

__port(__index(1),
    __identifier(LBP_KN_OUT),
	__attributes(ACF_ATTR_VEC_OUT_FIXED),
    __spatial_dep(0,0,0,0),
    __e0_data_type(d16u),
	__e0_size(LBP_NR_FEATURES_PER_BOX, 1),
	__ek_size(1, 1)),

__port(__index(2),
	__identifier(LBP_KN_OUT_BL_NORM),
	__attributes(ACF_ATTR_VEC_OUT_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16u),
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


#include "lbp_apu.h"

void apu_lbp(kernel_io_desc lInImg, kernel_io_desc lOut, kernel_io_desc lOutBlNorm)
{
	vec08u* lpvInImg = (vec08u*)lInImg.pMem;

  vec16u* lpvOut	 = (vec16u*)lOut.pMem;
  vec16u* lpvOutBlNorm = (vec16u*)lOutBlNorm.pMem;

  int lStrideIn = lInImg.chunkSpan / sizeof(int08u);

  int lStrideOut = lOut.chunkSpan / sizeof(int16u);

  bool isFirstTile = ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG);
  
  lbp(lpvInImg, lpvOut, lpvOutBlNorm, lStrideIn, lInImg.chunkWidth, lInImg.chunkHeight, lStrideOut, isFirstTile);

}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
