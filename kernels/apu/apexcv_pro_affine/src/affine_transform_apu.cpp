
/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2017 NXP
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

/*!*********************************************************************************
*  \file affine_transform_apu.cpp
*  \brief APU affine transform kernel.
***********************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION
#include <stdio.h>
#include "affine_transform_apu.h"
#include "affine_definitions.h"

// HELPER FUNCTION
static inline vec16u _interpolate(vec08u src0, vec08u src1, vec08u src2, vec08u src3, vec16u dx, vec16u dy);

void acf_affine_init_cu_offsets(int32s *mat, int32s chunk_width, int32s chunk_height, int32s tile_width_in_chunks)    
{     
   int32s ox = mat[2];
   int32s oy = mat[5];
   int32s bx = mat[2];
   int32s by = mat[5];
   
   if (mat[0] < 0) {
      bx += mat[0] * chunk_width;
   }

   if (mat[1] < 0) {
      bx += mat[1] * chunk_height;
   }
    
   if (mat[3] < 0) {
      by += mat[3] * chunk_width;
   }

   if (mat[4] < 0) {
      by += mat[4] * chunk_height;
   }
   
   for (int i = 0; i < tile_width_in_chunks; ++i) {
      g_x_offs = vput(g_x_offs, ox, i);
      g_y_offs = vput(g_y_offs, oy, i);
      
      ox += mat[0] * chunk_width;
      oy += mat[3] * chunk_width;
      
      chunk_base_x = vput(chunk_base_x, bx, i);
      chunk_base_y = vput(chunk_base_y, by, i);
      
      bx += mat[0] * chunk_width;
      by += mat[3] * chunk_width;
   }
}

// 2x2 bilinear interpolation
// src pixels:  +-------+-------+
//              |       |       |
//              |  p00  |  p01  |
//              |       |       |
//              +-------+-------+
//              |       |       |
//              |  p10  |  p11  |
//              |       |       |
//              +-------+-------+
//  dst = p00*(1-a)*(1-b) + p01*a*(1-b) + p10*(1-a)*b + p11*a*b
void acf_affine_bilinear_interpolation(const vec08u *src, int32s src_stride, int32s src_width, int32s src_height, int32s *mat, vec08u *dst, int32s dst_stride, int32s chunk_width, int32s chunk_height, int32s image_width, int32s image_height)
{
   int32s mask = ((1 << INTER_BITS) - 1);
   int32s width_sub_pad = image_width - 1;
   int32s height_sub_pad = image_height - 1;
   
   vec32s start_x = chunk_base_x & ~mask;
   start_x = vselect(start_x, 0, start_x > 0);
   start_x = vselect(start_x, (image_width - src_width) << INTER_BITS, start_x < ((image_width - src_width) << INTER_BITS));
   
   vec32s start_y = chunk_base_y & ~mask;
   start_y = vselect(start_y, 0, start_y > 0);
   start_y = vselect(start_y, (image_height - src_height) << INTER_BITS, start_y < ((image_height - src_height) << INTER_BITS));
         
   for (int y = 0; y < chunk_height; ++y) {
      
      vec32s line_x_offs = g_x_offs;
      vec32s line_y_offs = g_y_offs;
      for (int x = 0; x < chunk_width; ++x) {
         vec32s t_x = vselect(line_x_offs, 0, line_x_offs > 0);
         t_x = vselect(t_x, width_sub_pad << INTER_BITS, t_x < (width_sub_pad << INTER_BITS));
         vec16u chunk_x = (vec16u)((t_x - start_x) >> INTER_BITS);
         
         vec32s t_y = vselect(line_y_offs, 0, line_y_offs > 0);
         t_y = vselect(t_y, height_sub_pad << INTER_BITS, t_y < (height_sub_pad << INTER_BITS));
         vec16u chunk_y = (vec16u)((t_y - start_y) >> INTER_BITS);
   
         vec16s chunk_offset = (vec16s)(chunk_y * src_stride + chunk_x);
         
         vec08u input0 = vload(src, chunk_offset);                   // p00
         vec08u input1 = vload(src, chunk_offset + 1);               // p01
         vec08u input2 = vload(src, chunk_offset + src_width);       // p10
         vec08u input3 = vload(src, chunk_offset + src_width + 1);   // p11

         vec16u ta = _interpolate(input0, input1, input2, input3, (vec16u)(t_x & mask) >> 1, (vec16u)(t_y & mask) >> 1);
         dst[y * dst_stride + x] = (vec08u)vsat(ta, 0, 255);
         
         line_x_offs += mat[0];
         line_y_offs += mat[3];
      }
      
      g_x_offs += mat[1];
      g_y_offs += mat[4];
   }
   
   chunk_base_x += mat[1] * chunk_height;
   chunk_base_y += mat[4] * chunk_height;
}

static inline vec16u _interpolate(vec08u src0, vec08u src1, vec08u src2, vec08u src3, vec16u dx, vec16u dy)
{
    vec16u sx = 16 - dx;
    vec16u sy = 16 - dy;
    vec16u ti = (vec16u)src0*sx*sy + (vec16u)src1*dx*sy + (vec16u)src2*sx*dy + (vec16u)src3*dx*dy;

    return (((ti>>7)+1)>>1);
}

#endif // ACF_KERNEL_IMPLEMENTATION
