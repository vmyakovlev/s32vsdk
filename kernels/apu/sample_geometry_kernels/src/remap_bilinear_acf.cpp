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
* \file remap_bilinear_acf.cpp
* \addtogroup geometry
* \addtogroup remap_bilinear Remap bilinear
* \ingroup geometry
* @{
* \brief Element-wise bilinear interpolation
*/


#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif


#ifdef ACF_KERNEL_METADATA

#include "remap_bilinear_acf.h"


/****************************************************************************/
/*!
RGB interpolation kernel metadata. Interpolates between neighboring pixels of a RGB image. 
Outputs 8bit unsigned interpolation result.

\param  REMAP_BILIN_GRAY_KN			Define for Kernel name
\param 4							Number of ports
\param "Port REMAP_BILIN_KN_DST"	Define for name of interpolation result (unsigned 32bit).
\param "Port REMAP_BILIN_KN_SRC"	Define for name of first input image (unsigned 32bit)
\param "Port REMAP_BILIN_KN_OFFS"	Define for name of input offsets (unsigned 16bit)
\param "Port REMAP_BILIN_KN_DELTA"	Define for name of second input deltas (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(REMAP_BILIN_RGB_K)
(
   REMAP_BILIN_RGB_KN,
   4,
   __port(__index(0),
          __identifier(REMAP_BILIN_KN_DST),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),      // bot and right dependencies are contained in data, not ACF
          __e0_data_type(d32u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier(REMAP_BILIN_KN_SRC),
          __attributes(ACF_ATTR_VEC_IN_FIXED/*ACF_ATTR_IN | ATTR_FIXED*/),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32u),
          __e0_size(1, 1),
          __ek_size(8, 9)),
          
   // todo currently does not match 32 bit size in code, but I think 16 bit is correct
   __port(__index(2),
          __identifier(REMAP_BILIN_KN_OFFS),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(3),
          __identifier(REMAP_BILIN_KN_DELTA),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(2, 1),
          __ek_size(1, 1))
);


/****************************************************************************/
/*!
GrayScale interpolation kernel metadata. Interpolates between neighboring pixels of a grayscale image. 
Outputs 8bit unsigned interpolation result.

\param  REMAP_BILIN_GRAY_KN			Define for Kernel name
\param 4							Number of ports
\param "Port REMAP_BILIN_KN_DST"	Define for name of interpolation result (unsigned 8bit).
\param "Port REMAP_BILIN_KN_SRC"	Define for name of first input image (unsigned 8bit)
\param "Port REMAP_BILIN_KN_OFFS"	Define for name of input offsets (unsigned 16bit)
\param "Port REMAP_BILIN_KN_DELTA"	Define for name of second input deltas (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(REMAP_BILIN_GRAY_K)
(
   REMAP_BILIN_GRAY_KN,
   4,
   __port(__index(0),
          __identifier(REMAP_BILIN_KN_DST),
		  __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),      // bot and right dependencies are contained in data, not ACF
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier(REMAP_BILIN_KN_SRC),
          __attributes(ACF_ATTR_VEC_IN_FIXED/*ACF_ATTR_IN | ATTR_FIXED*/),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(36, 12)),            // source block size
          
   // todo currently does not match 32 bit size in code, but I think 16 bit is correct
   __port(__index(2),
          __identifier(REMAP_BILIN_KN_OFFS),
		  __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(3),
          __identifier(REMAP_BILIN_KN_DELTA),
		  __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(2, 1),
          __ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

#include "remap_bilinear_apu.h"

//void
//remap_bilinear (vec32u __cmem* dst,
//                vec32u __cmem* src,
//                vec16u __cmem* offset, 
//                vec08u __cmem* delta, 
//                int stride, // stride of the src array in cmem
//                int bs) // block size.  extraced from dst

void
remap_bilinear_rgb (kernel_io_desc dst_param,
                    kernel_io_desc src_param,
                    kernel_io_desc offset_param, 
                    kernel_io_desc delta_param) 
{
   vec32u* dst    = (vec32u*)dst_param.pMem;
   vec32u* src    = (vec32u*)src_param.pMem;
   vec16u* offset = (vec16u*)offset_param.pMem;
   vec08u* delta  = (vec08u*)delta_param.pMem;
   int sstride     = src_param.chunkSpan / sizeof(vec32u);  // stride of the src array in cmem
   int dstride     = dst_param.chunkSpan / sizeof(vec32u);  // stride of the src array in cmem
   int bw         = dst_param.chunkWidth;
   int bh         = dst_param.chunkHeight;
   remap_bilinear_rgb_impl(dst, src, offset, delta, sstride, dstride, bw, bh);   
}



void
remap_bilinear_grayscale(
   kernel_io_desc dst_param,
   kernel_io_desc src_param,
   kernel_io_desc offset_param, 
   kernel_io_desc delta_param) 
{
   vec08u* dst    = (vec08u*)dst_param.pMem;
   vec08u* src    = (vec08u*)src_param.pMem;
   vec16u* offset = (vec16u*)offset_param.pMem;
   vec08u* delta  = (vec08u*)delta_param.pMem;
   int sstride     = src_param.chunkSpan / sizeof(vec08u);  // stride of the src array in cmem
   int dstride     = dst_param.chunkSpan / sizeof(vec08u);  // stride of the src array in cmem
   
   int bw         = dst_param.chunkWidth;
   int bh         = dst_param.chunkHeight;

   remap_bilinear_grayscale_impl(dst, src, offset, delta, sstride, dstride, bw, bh);
   
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
