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
* \file histofgrad_acf.cpp
* \addtogroup statistics
* \addtogroup histofgrad
* \ingroup statistics
* @{
* \brief Histogram of Gradient
*/

#ifdef ACF_KERNEL_METADATA
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#include "histofgrad_acf.h"
#include "histofgrad_defines.h"


/****************************************************************************/
/*!
   Histogram of Gradient kernel metadata.
   
   \param HISTOFGRAD_KN			  Define for Kernel name
   \param 3						  Number of ports
   \param "Port HOG_KN_INGradX"  Define for name of input gradientX image (signed 32bit)
   \param "Port HOG_KN_INGradX"  Define for name of input gradientY image (signed 32bit)
   \param "Port HOG_KN_OUT"		 Define for name of histofgrad vector output, CU count X 64 bins of 4bits each (i.e. 2 x unsigned 32bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(HISTOFGRAD_K)
(
  HISTOFGRAD_KN,
  4,
  __port(__index(0),
  __identifier(HOG_KN_InGradX),
    __attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, HOG_LAT_DEPENDENCY, 0, HOG_LAT_DEPENDENCY),
    __e0_data_type(d08s),
    __e0_size(1, 1),
	__ek_size(HOG_WND_SZ, HOG_WND_SZ)),

__port(__index(1),
	__identifier(HOG_KN_InGradY),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, HOG_LAT_DEPENDENCY, 0, HOG_LAT_DEPENDENCY),
	__e0_data_type(d08s),
	__e0_size(1, 1),
	__ek_size(HOG_WND_SZ, HOG_WND_SZ)),

__port(__index(2),
    __identifier(HOG_KN_OUT),
    __attributes(ACF_ATTR_VEC_OUT),
    __spatial_dep(0,0,0,0),
    __e0_data_type(d16u),
	__e0_size(HOG_NR_FEATURES_PER_BOX, 1),
	__ek_size(1, 1)),

__port(__index(3),
	__identifier(HOG_KN_OUT_BL_NORM),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32u),
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

#include "histofgrad_apu.h"


void apu_histofgrad(kernel_io_desc lInGradX, kernel_io_desc lInGradY, kernel_io_desc lOut, kernel_io_desc lOutBlNorm)
{
  vec08s* lpvInGradX = (vec08s*)lInGradX.pMem;
  vec08s* lpvInGradY = (vec08s*)lInGradY.pMem;

  vec16u* lpvOut	 = (vec16u*)lOut.pMem;
  vec32u* lpvOutBlNorm = (vec32u*)lOutBlNorm.pMem;

  int lStrideIn	= lInGradX.chunkSpan/sizeof(int08s);
  int lStrideOut = lOut.chunkSpan / sizeof(int16u);

  
  hog(lpvInGradX,lpvInGradY, lpvOut, lpvOutBlNorm, lStrideIn, lInGradX.chunkWidth, lInGradX.chunkHeight, lStrideOut);

}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
