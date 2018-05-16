/**************************************************************************************************
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
 **************************************************************************************************/

#include "cg_kernel.h"

#ifdef ACF_KERNEL_METADATA

KERNEL_INFO _kernel_info_hrsz16
(
   "apu_resize16_horizontal",
   5,
   __port(__index(0),
          __identifier("SRC"),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier("DST"),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(2),
          __identifier("SRC_WIDTH"),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16s),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(3),
          __identifier("DST_WIDTH"),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16s),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(4),
          __identifier("HRSZ_DESC"),
          __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32s),
          __e0_size(1, 1),
          __ek_size(11, 1))
);

KERNEL_INFO _kernel_info_vrsz16
(
   "apu_resize16_vertical",
   5,
   __port(__index(0),
          __identifier("SRC"),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier("DST"),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(2),
          __identifier("SRC_HEIGHT"),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16s),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(3),
          __identifier("DST_HEIGHT"),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16s),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(4),
          __identifier("VRSZ_DESC"),
          __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32s),
          __e0_size(1, 1),
          __ek_size(11, 1))
);


KERNEL_INFO _kernel_info_hrsz
(
   "apu_resize_horizontal",
   5,
   __port(__index(0),
          __identifier("SRC"),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier("DST"),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(2),
          __identifier("SRC_WIDTH"),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16s),
          __e0_size(1, 1),
          __ek_size(1, 1)),          
   __port(__index(3),
          __identifier("DST_WIDTH"),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16s),
          __e0_size(1, 1),
          __ek_size(1, 1)),          
   __port(__index(4),
          __identifier("HRSZ_DESC"),
          __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32s),
          __e0_size(1, 1),
          __ek_size(11, 1))
);

KERNEL_INFO _kernel_info_vrsz
(
   "apu_resize_vertical",
   5,
   __port(__index(0),
          __identifier("SRC"),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier("DST"),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(2),
          __identifier("SRC_HEIGHT"),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16s),
          __e0_size(1, 1),
          __ek_size(1, 1)),          
   __port(__index(3),
          __identifier("DST_HEIGHT"),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16s),
          __e0_size(1, 1),
          __ek_size(1, 1)),          
   __port(__index(4),
          __identifier("VRSZ_DESC"),
          __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32s),
          __e0_size(1, 1),
          __ek_size(11, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

//TODO: change when makefiles support include directories or repo structure is changed...
#include "resize_apu.h"

void
apu_resize_horizontal(kernel_io_desc src,
                      kernel_io_desc dst,
                      kernel_io_desc src_w,
                      kernel_io_desc dst_w,
                      kernel_io_desc hrsz_desc) 
{
   vec08u *s = (vec08u*)src.pMem;
   vec08u *d = (vec08u*)dst.pMem;
   int16s sw = *(int16s*)src_w.pMem;
   int16s dw = *(int16s*)dst_w.pMem;
   resize_descriptor *r = (resize_descriptor*)hrsz_desc.pMem;

   if (ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG)) {
      get_polyphase_params(sw, dw, HRSZ_DECIMAL_SCL, r);
      init_cu_offsets(r, dst.chunkWidth, ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS));
   }
   
   apu_hrsz_polyphase(s, src.chunkSpan, src.chunkWidth, d, dst.chunkSpan, dst.chunkWidth, dst.chunkHeight, r);
}

void
apu_resize_vertical(kernel_io_desc src,
                    kernel_io_desc dst,
                    kernel_io_desc src_h,
                    kernel_io_desc dst_h,
                    kernel_io_desc vrsz_desc) 
{
   vec08u *s = (vec08u*)src.pMem;
   vec08u *d = (vec08u*)dst.pMem;
   int16s sh = *(int16s*)src_h.pMem;
   int16s dh = *(int16s*)dst_h.pMem;
   resize_descriptor *r = (resize_descriptor*)vrsz_desc.pMem;

   if (ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG)) {
      get_polyphase_params(sh, dh, VRSZ_DECIMAL_SCL, r);
   }
   
   apu_vrsz_polyphase(s, src.chunkSpan, src.chunkWidth, d, dst.chunkSpan, dst.chunkWidth, dst.chunkHeight, r);
}


void
apu_resize16_horizontal(kernel_io_desc src,
                      kernel_io_desc dst,
                      kernel_io_desc src_w,
                      kernel_io_desc dst_w,
                      kernel_io_desc hrsz_desc)
{
   vec16u *s = (vec16u*)src.pMem;
   vec16u *d = (vec16u*)dst.pMem;
   int16s sw = *(int16s*)src_w.pMem;
   int16s dw = *(int16s*)dst_w.pMem;
   resize_descriptor *r = (resize_descriptor*)hrsz_desc.pMem;

   if (ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG)) {
      get_polyphase_params(sw, dw, HRSZ_DECIMAL_SCL, r);
      init_cu_offsets(r, dst.chunkWidth, ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS));
   }

   apu_hrsz_polyphase16(s, src.chunkSpan>>1, src.chunkWidth, d, dst.chunkSpan>>1, dst.chunkWidth, dst.chunkHeight, r);
}

void
apu_resize16_vertical(kernel_io_desc src,
                    kernel_io_desc dst,
                    kernel_io_desc src_h,
                    kernel_io_desc dst_h,
                    kernel_io_desc vrsz_desc)
{
   vec16u *s = (vec16u*)src.pMem;
   vec16u *d = (vec16u*)dst.pMem;
   int16s sh = *(int16s*)src_h.pMem;
   int16s dh = *(int16s*)dst_h.pMem;
   resize_descriptor *r = (resize_descriptor*)vrsz_desc.pMem;

   if (ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG)) {
      get_polyphase_params(sh, dh, VRSZ_DECIMAL_SCL, r);
   }

   apu_vrsz_polyphase16(s, src.chunkSpan>>1, src.chunkWidth, d, dst.chunkSpan>>1, dst.chunkWidth, dst.chunkHeight, r);
}



#endif
