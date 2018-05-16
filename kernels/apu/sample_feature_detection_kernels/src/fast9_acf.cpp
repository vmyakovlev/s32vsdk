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

/*!
* \file fast9_acf.cpp
* \addtogroup feature_detection Feature detection
* \addtogroup FAST9 Fast9 feature detection
* \ingroup feature_detection
* @{
* \brief FAST9 feature point detection
*/
#ifdef APEX2_EMULATE
#include <acf_kernel.hpp> // if using the ACF emulation library
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA

#include "fast9_acf.h"

/****************************************************************************/
/*!
   FAST9 feature point detection kernel metadata.
   
   \param FAST9_KN			   Define for Kernel name
   \param 3					   Number of ports
   \param "Port FAST9_KN_IN0"  Define for name of input image (unsigned 8bit)
   \param "Port 'FAST9_KN_OUT" Define for name of output image (unsigned 8bit)
   \param "Port FAST9_KN_IN1"  Define for name of threshold used for classifying ring pixels (unsigned 8bit)
							  (brighter/darker/similar)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(FAST9_K)
(
	FAST9_KN,
	3,
	__port(__index(0),
		__identifier(FAST9_KN_IN_IMG),
		__attributes(ACF_ATTR_VEC_IN),
		__spatial_dep(3, 3, 3, 3),
		__e0_data_type(d08u),
		__e0_size(1, 1),
		__ek_size(1, 1)),
	__port(__index(1),
		__identifier(FAST9_KN_OUT),
		__attributes(ACF_ATTR_VEC_OUT),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d08u),
		__e0_size(1, 1),
		__ek_size(1, 1)),
	__port(__index(2),
		__identifier(FAST9_KN_IN_THR),
		__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d08u),
		__e0_size(1, 1),
		__ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "fast9_apu.h"

void apu_fast9(kernel_io_desc lInImg, kernel_io_desc lOutImg, kernel_io_desc lInThr)
{
  vec08u* lpvInImg = (vec08u*)lInImg.pMem;
  vec08u* lpvOutImg = (vec08u*)lOutImg.pMem;

  int08u threshold = ((int08u*)lInThr.pMem)[0];
  apu_fast9_unsuppressed_score(
    lpvInImg, lpvOutImg,
    lInImg.chunkSpan, lOutImg.chunkSpan,
    lInImg.chunkWidth, lInImg.chunkHeight,
    threshold);
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
