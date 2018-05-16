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
* \file upsample_acf.cpp
* \addtogroup resizing
* \addtogroup upsample Image upsampling
* \ingroup resizing
* @{
* \brief Image upsampling
*/
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif



#ifdef ACF_KERNEL_METADATA
#include "upsample_acf.h"

/****************************************************************************/
/*!
   Image 2x upsampling kernel metadata.
   
   \param UPSAMPLE_KN				Define for Kernel name
   \param 2							Number of ports
   \param "Port UPSAMPLE_KN_IN"		Define for name of input image (unsigned 8bit)
   \param "Port UPSAMPLE_KN_OUT"	Define for name of output image (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(UPSAMPLE_K) //_kernel_info_upsample
(
   UPSAMPLE_KN,
   2,
   __port(__index(0),
          __identifier(UPSAMPLE_KN_IN),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(1,1,1,1),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
   __identifier(UPSAMPLE_KN_OUT),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(2, 2))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "upsample_apu.h"

void apu_upsample(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

   upsample(lpvOut0, lpvIn0, lIn0.chunkWidth, lIn0.chunkHeight, lIn0.chunkSpan, lOut0.chunkSpan);
}


#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
