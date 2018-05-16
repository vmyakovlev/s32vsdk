/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2015-2017 NXP
* All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/

#include "cg_kernel.h"

#ifdef ACF_KERNEL_METADATA

KERNEL_INFO _remap_bilinear_rgb
(
   "wrap_remap_bilinear_rgb",
   4,
   __port(__index(0),
          __identifier("DST"),
          __attributes(ACF_ATTR_OUT),
          __spatial_dep(0,0,0,0),      // bot and right dependencies are contained in data, not ACF
          __e0_data_type(d32u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier("SRC"),
          __attributes(ACF_ATTR_IN | ATTR_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32u),
          __e0_size(1, 1),
          __ek_size(8, 9)),
          
   // todo currently does not match 32 bit size in code, but I think 16 bit is correct
   __port(__index(2),
          __identifier("OFFSET"),
          __attributes(ACF_ATTR_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(3),
          __identifier("DELTA"),
          __attributes(ACF_ATTR_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(2, 1),
          __ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION



void
wrap_remap_bilinear_rgb (kernel_io_desc dst_param,
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

   remap_bilinear_rgb(
      dst,
      src,
      offset,
      delta,
      sstride,
      dstride,
      bw,
      bh);
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef ACF_KERNEL_METADATA

KERNEL_INFO _remap_bilinear_greyscale
(
   "wrap_remap_bilinear_gray",
   4,
   __port(__index(0),
          __identifier("DST"),
          __attributes(ACF_ATTR_OUT),
          __spatial_dep(0,0,0,0),      // bot and right dependencies are contained in data, not ACF
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier("SRC"),
          __attributes(ACF_ATTR_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),            // source block size - simple test case
          
   // todo currently does not match 32 bit size in code, but I think 16 bit is correct
   __port(__index(2),
          __identifier("OFFSET"),
          __attributes(ACF_ATTR_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(3),
          __identifier("DELTA"),
          __attributes(ACF_ATTR_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(2, 1),
          __ek_size(1, 1))
);

KERNEL_INFO _remap_block_size
(
   "wrap_remap_block_size",
   5,
   __port(__index(0),
          __identifier("MAP"),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier("TWO_HEIGHT"),
          __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(2, 1),
          __ek_size(1, 1)),
   __port(__index(2),
          __identifier("FOUR_HEIGHT"),
          __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(2, 1),
          __ek_size(1, 1)),
   __port(__index(3),
          __identifier("EIGHT_HEIGHT"),
          __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(2, 1),
          __ek_size(1, 1)),
   __port(__index(4),
          __identifier("SIXTEEN_HEIGHT"),
          __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(2, 1),
          __ek_size(1, 1))          
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

void
wrap_remap_bilinear_gray(
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

   remap_bilinear_grayscale(
      dst,
      src,
      offset,
      delta,
      sstride,
      dstride,
      bw,
      bh);
}

void wrap_remap_block_size(kernel_io_desc map, kernel_io_desc two, kernel_io_desc four, kernel_io_desc eight, kernel_io_desc sixteen)
{
   vec32u *p_map = (vec32u*)map.pMem;
   vec16u *p_two = (vec16u*)two.pMem;
   vec16u *p_four = (vec16u*)four.pMem;
   vec16u *p_eight = (vec16u*)eight.pMem;
   vec16u *p_sixteen = (vec16u*)sixteen.pMem;
   
   remap_block_size(p_map, map.chunkWidth, map.chunkHeight, ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS), &p_two[0], &p_two[1], &p_four[0], &p_four[1], &p_eight[0], &p_eight[1], &p_sixteen[0], &p_sixteen[1]);
}

#endif
