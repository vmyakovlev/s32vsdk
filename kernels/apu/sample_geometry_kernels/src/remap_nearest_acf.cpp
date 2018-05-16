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
* \file remap_nearest_acf.cpp
* \addtogroup geometry
* \addtogroup remap_nearest Remap nearest
* \ingroup geometry
* @{
* \brief Element-wise nearest neighbor interpolation
*/


#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif


#ifdef ACF_KERNEL_METADATA

#include "remap_nearest_acf.h"


/****************************************************************************/
/*!
GrayScale interpolation kernel metadata. Interpolates between neighboring pixels of a grayscale image. 
Outputs 8bit unsigned interpolation result.

\param  REMAP_NEAREST_GRAY_KN			Define for Kernel name
\param 4							Number of ports
\param "Port REMAP_NEAREST_KN_DST"	Define for name of interpolation result (unsigned 8bit).
\param "Port REMAP_NEAREST_KN_SRC"	Define for name of first input image (unsigned 8bit)
\param "Port REMAP_NEAREST_KN_OFFS"	Define for name of input offsets (unsigned 16bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(REMAP_NEAREST_GRAY_K)
(
   REMAP_NEAREST_GRAY_KN,
   3,
   __port(__index(0),
          __identifier(REMAP_NEAREST_KN_DST),
		  __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),      // bot and right dependencies are contained in data, not ACF
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier(REMAP_NEAREST_KN_SRC),
          __attributes(ACF_ATTR_VEC_IN/*ACF_ATTR_IN | ATTR_FIXED*/),
          __spatial_dep(10,10,10,10),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),            // source block size
          
   // todo currently does not match 32 bit size in code, but I think 16 bit is correct
   __port(__index(2),
          __identifier(REMAP_NEAREST_KN_OFFS),
		  __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

#include "remap_nearest_apu.h"


void
remap_nearest_grayscale(
   kernel_io_desc dst_param,
   kernel_io_desc src_param,
   kernel_io_desc offset_param) 
{
   vec08u* dst    = (vec08u*)dst_param.pMem;
   vec08u* src    = (vec08u*)src_param.pMem;
   vec16u* offset = (vec16u*)offset_param.pMem;
   int sstride     = src_param.chunkSpan / sizeof(vec08u);  // stride of the src array in cmem
   int dstride     = dst_param.chunkSpan / sizeof(vec08u);  // stride of the src array in cmem
   
   int bw         = dst_param.chunkWidth;
   int bh         = dst_param.chunkHeight;

   remap_nearest_grayscale_impl(dst, src, offset, sstride, dstride, bw, bh);
   
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
