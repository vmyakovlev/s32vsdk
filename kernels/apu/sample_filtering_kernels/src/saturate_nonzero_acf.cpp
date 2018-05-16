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
* \file saturate_nonzero_acf.cpp
* \addtogroup filtering
* \addtogroup saturation
* \ingroup filtering
* @{
* \brief Saturation
*/
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA

#include "saturate_nonzero_acf.h"


/****************************************************************************/
/*!
   Non-zero saturation kernel metadata.
   
   \param SAT_NONZERO_KN		 Define for Kernel name
   \param 2						 Number of ports
   \param "Port SAT_NONZERO_IN"  Define for name of input image (unsigned 8bit)
   \param "Port SAT_NONZERO_OUT" Define for name of output image (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(SAT_NONZERO_K)
(
   SAT_NONZERO_KN,
   2,
   __port(__index(0),
		  __identifier(SAT_NONZERO_IN),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier(SAT_NONZERO_OUT),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);


/****************************************************************************/
/*!
Non-zero saturation kernel metadata.

\param SAT_NONZERO_KN		 Define for Kernel name
\param 2						 Number of ports
\param "Port SAT_NONZERO_IN"  Define for name of input image (unsigned 8bit)
\param "Port SAT_NONZERO_OUT" Define for name of output image (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(BINARIZE_K)
(
BINARIZE_KN,
2,
__port(__index(0),
	__identifier(BINARIZE_IN),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32u),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(1),
	__identifier(BINARIZE_OUT),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(1, 1))
);




/****************************************************************************/
/*!
Masking kernel metadata. Mask input to zero if flags are zero

\param MASK_KN			 Define for Kernel name
\param 5						 Number of ports
\param "Port MASK_K_INFLAGS"  Define for name of masking flags input image (unsigned 32bit)
\param "Port MASK_INX"  Define for name of input X image to be masked(signed 16bit)
\param "Port MASK_INY"  Define for name of input Y image to be masked(signed 16bit)
\param "Port MASK_OUTX" Define for name of output X image (signed 16bit)
\param "Port MASK_OUTY" Define for name of output Y image (signed 16bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(MASK_K)
(
MASK_KN,
5,
__port(__index(0),
	__identifier(MASK_INFLAGS),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32u),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(1),
	__identifier(MASK_INX),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(2),
	__identifier(MASK_INY),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(3),
	__identifier(MASK_OUTX),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

__port(__index(4),
	__identifier(MASK_OUTY),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#include "saturate_nonzero_apu.h"

void apu_saturate_nonzero (kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u*        lpvIn0   = (vec08u*)  lIn0.pMem;
   vec08u*        lpvOut0  = (vec08u*)  lOut0.pMem;

   saturate_nonzero(lpvOut0, lpvIn0, lOut0.chunkSpan, lIn0.chunkSpan, lIn0.chunkWidth, lIn0.chunkHeight);
}

void apu_binarize(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
	vec32u*        lpvIn0 = (vec32u*)lIn0.pMem;
	vec08u*        lpvOut0 = (vec08u*)lOut0.pMem;

	binarize(lpvOut0, lpvIn0, lOut0.chunkSpan, lIn0.chunkSpan, lIn0.chunkWidth, lIn0.chunkHeight);
}

void apu_mask(kernel_io_desc lInFlags, kernel_io_desc lInX, kernel_io_desc lInY, kernel_io_desc lOutX, kernel_io_desc lOutY)
{
	vec32u*        lpvInFlags = (vec32u*)lInFlags.pMem;
	vec16s*        lpvInX = (vec16s*)lInX.pMem;
	vec16s*        lpvInY = (vec16s*)lInY.pMem;
	vec16s*        lpvOutX = (vec16s*)lOutX.pMem;
	vec16s*        lpvOutY = (vec16s*)lOutY.pMem;

	mask(lpvOutX, lpvOutY, lpvInFlags, lpvInX, lpvInY, lOutX.chunkSpan, lInX.chunkSpan, lInX.chunkWidth, lInX.chunkHeight);
}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
