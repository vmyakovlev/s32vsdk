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
* \file downsample_acf.cpp
* \addtogroup resizing
* \addtogroup downsample Image downsampling
* \ingroup resizing
* @{
* \brief Image downsampling
*/
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA

#include "downsample_acf.h"


/****************************************************************************/
/*!
   Image 2x downsampling kernel metadata.
   
   \param DOWNSAMPLE_KN				Define for Kernel name
   \param 2							Number of ports
   \param "Port DOWNSAMPLE_KN_IN"	Define for name of input image (unsigned 8bit)
   \param "Port DOWNSAMPLE_KN_OUT"	Define for name of output image (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(DOWNSAMPLE_K)
(
	DOWNSAMPLE_KN,
   2,
   __port(__index(0),
   __identifier(DOWNSAMPLE_KN_IN),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(2, 2)),
   __port(__index(1),
          __identifier(DOWNSAMPLE_KN_OUT),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);


/****************************************************************************/
/*!
   Image 2x downsampling kernel (16-bit version) metadata.
   
   \param DOWNSAMPLE_16u_KN			Define for Kernel name
   \param 2							Number of ports
   \param "Port DOWNSAMPLE_KN_IN"   Define for name of input image (unsigned 16bit)
   \param "Port DOWNSAMPLE_KN_OUT"  Define for name of output image (unsigned 16bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(DOWNSAMPLE_16u_K) //_kernel_info_downsample_16u
(
DOWNSAMPLE_16u_KN,
2,
__port(__index(0),
__identifier(DOWNSAMPLE_KN_IN),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d16u),
__e0_size(1, 1),
__ek_size(2, 2)),
__port(__index(1),
__identifier(DOWNSAMPLE_KN_OUT),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d16u),
__e0_size(1, 1),
__ek_size(1, 1))
);


/****************************************************************************/
/*!
   Image 2x downsampling using Gaussian blur kernel metadata.
   
   \param DOWNSAMPLE_GAUSS_KN		Define for Kernel name
   \param 2							Number of ports
   \param "Port DOWNSAMPLE_KN_IN"	Define for name of input image (unsigned 8bit)
   \param "Port DOWNSAMPLE_KN_OUT"	Define for name of output image (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(DOWNSAMPLE_GAUSS_K) //_kernel_info_downsample_gauss
(
	DOWNSAMPLE_GAUSS_KN,
	2,
	__port(__index(0),
	__identifier(DOWNSAMPLE_KN_IN),
		__attributes(ACF_ATTR_VEC_IN),
		__spatial_dep(1, 1, 1, 1),
		__e0_data_type(d08u),
		__e0_size(1, 1),
		__ek_size(2, 2)),
		__port(__index(1),
	__identifier(DOWNSAMPLE_KN_OUT),
		__attributes(ACF_ATTR_VEC_OUT),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d08u),
		__e0_size(1, 1),
		__ek_size(1, 1))
);


#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION
#include "downsample_apu.h"

void apu_downsample(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
	vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
	vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

	downsample(lpvOut0, lpvIn0, lOut0.chunkWidth, lOut0.chunkHeight, lIn0.chunkSpan, lOut0.chunkSpan);
}


void apu_downsample_16u(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
	vec16u* lpvIn0 = (vec16u*)lIn0.pMem;
	int16s inDataSz = sizeof(int16u);
	int16s inBlckWidth = lIn0.chunkSpan / inDataSz;

	vec16u* lpvOut0 = (vec16u*)lOut0.pMem;
	int16s outDataSz = sizeof(int16u);
	int16s outBlckWidth = lOut0.chunkSpan / outDataSz;
	downsample_16u(lpvOut0, lpvIn0, inBlckWidth, lIn0.chunkHeight, lOut0.chunkWidth, lOut0.chunkHeight, inBlckWidth, outBlckWidth);
}


void apu_downsample_gauss(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
	vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
	vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

	downsample_gauss(lpvOut0, lpvIn0, lOut0.chunkWidth, lOut0.chunkHeight, lIn0.chunkSpan, lOut0.chunkSpan);
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
