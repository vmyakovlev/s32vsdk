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
* \file sad_acf.cpp
* \addtogroup feature_detection Feature detection
* \addtogroup SAD Sum of Absolute Differences
* \ingroup feature_detection
* @{
* \brief SAD (Sum of Absolute Differences ~= image similarity)
*/
#ifdef APEX2_EMULATE
#include <acf_kernel.hpp> // if using the ACF emulation library
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA

#include "sad_acf.h"

/****************************************************************************/
/*!
SAD Sum of Absolute Differences kernel metadata.

\param SAD_KN			  Definition for kernel name
\param 3				  Number of ports
\param "Port SAD_KN_IN0"  Define for name of first input image (unsigned 8bit)
\param "Port SAD_KN_IN1"  Define for name of second input image (unsigned 8bit)
\param "Port SAD_KN_OUT"  Define for name of output image containing the SAD values for each pixel (unsigned 8bit)
*****************************************************************************/

KERNEL_INFO kernelInfoConcat(SAD_K)
(
   SAD_KN,
   3,
   __port(__index(0),
          __identifier(SAD_KN_IN0),
		 __attributes(ACF_ATTR_VEC_IN_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(4, 4)),
   __port(__index(1),
          __identifier(SAD_KN_IN1),
		  __attributes(ACF_ATTR_VEC_IN_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(8, 8)),
   __port(__index(2),
          __identifier(SAD_KN_OUT),
		  __attributes(ACF_ATTR_VEC_OUT_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(4, 1),
          __ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "sad_apu.h"

void apu_sad (kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvIn1  = (vec08u*)lIn1.pMem;
   vec32u* lpvOut0 = (vec32u*)lOut0.pMem;

   apu_sad_impl(lpvIn0, lIn0.chunkSpan, 4, 4,
           lpvIn1, lIn1.chunkSpan, 8, 8,
           lpvOut0, lOut0.chunkSpan, 1, 1);
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
