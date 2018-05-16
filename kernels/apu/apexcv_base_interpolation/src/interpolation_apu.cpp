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
*  \file apu_interpolation.c
*  \brief APU interpolation kernels.
***********************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION
#include "interpolation_apu.h"
#include "bicubicLUT.h"

// bicubic helper methods
static void
accessLUT(
            uint8_t x_offset, uint8_t y_offset,
            int16_t xlut[4], int16_t ylut[4])
{
   xlut[0] = BicubicLUT[0][x_offset];
   xlut[1] = BicubicLUT[1][x_offset];
   xlut[2] = BicubicLUT[2][x_offset];
   xlut[3] = BicubicLUT[3][x_offset];

   ylut[0] = BicubicLUT[0][y_offset];
   ylut[1] = BicubicLUT[1][y_offset];
   ylut[2] = BicubicLUT[2][y_offset];
   ylut[3] = BicubicLUT[3][y_offset];
}

vec08u
cubicInterpolate(
               uint8_t t,
               vec08u a0, vec08u a1, vec08u a2, vec08u a3,
               int16_t lut[4])
{
   vec16s tmp = ((vec16s)a0*lut[0] + (vec16s)a1*lut[1] + (vec16s)a2*lut[2] + (vec16s)a3*lut[3] + 128)>>8;
   return (vec08u)tmp;
}

vec08u
XYInterpolation(
            vec08u p[4][4],
            uint8_t x_offset, uint8_t y_offset,
            int16_t xlut[4], int16_t ylut[4])
{
   vec08u arr[4];
   arr[0] = cubicInterpolate(x_offset, p[0][0], p[1][0], p[2][0], p[3][0], xlut);
   arr[1] = cubicInterpolate(x_offset, p[0][1], p[1][1], p[2][1], p[3][1], xlut);
   arr[2] = cubicInterpolate(x_offset, p[0][2], p[1][2], p[2][2], p[3][2], xlut);
   arr[3] = cubicInterpolate(x_offset, p[0][3], p[1][3], p[2][3], p[3][3], xlut);
   return cubicInterpolate(y_offset, arr[0], arr[1], arr[2], arr[3], ylut);
}
//bicubic helper methods end

void
apu_interp_linear_grayscale (
               vec08u* dst, int dstr,
         const vec08u* src, int sstr,
               int bw, int bh,
         const vec08u* delta)
{
   vec16u s11, s12, s13;   // 8-bit source values
   vec16u delta1, delta2;  // 8-bit x delta value

   int offset_idx = 0;     // 1-d offset for offset/delta lookup
   int dest_row = 0;       // offset to start of destination row
   int src_row = 0;        // offset to start of source row
   int idx_s = 0;          // offset to current pixel being processed
   int bx;
   
   // loop on destination pixels
   for (int by = 0; by < bh; ++by, dest_row+=dstr) chess_loop_range(1,) {
      for (bx = 0; bx < bw-1; bx+=2, offset_idx+=2, idx_s+=2) chess_loop_range(1,) {
         s11 = (vec16u)src[idx_s];
         s12 = (vec16u)src[idx_s+1];
         s13 = (vec16u)src[idx_s+2];
         
         delta1 = (vec16u)delta[offset_idx];
         delta2 = (vec16u)delta[offset_idx+1];

         // Assumes only X delta available
         dst[dest_row + bx] = (vec08u)(((((s12 - s11) * delta1) + 0x80) >> 8) + s11);
         dst[dest_row + bx + 1] = (vec08u)(((((s13 - s12) * delta2) + 0x80) >> 8) + s12);
      }
      // Process the last pixel in the row for odd width blocks
      if (bx == bw-1) {
         s11 = (vec16u)src[idx_s];
         s12 = (vec16u)src[idx_s+1];
         
         delta1 = (vec16u)delta[offset_idx];

         // Assumes only X delta available
         dst[dest_row + bx] = (vec08u)(((((s12 - s11) * delta1) + 0x80) >> 8) + s11);
      }
      src_row += sstr;
      idx_s = src_row;
   } // next row
}

void
apu_interp_bilinear_grayscale (
               vec08u* dst, int dstr,
         const vec08u* src, int sstr,
               int bw, int bh,
         const vec08u* delta)
{
   vec16u s11, s12, s21, s22;  // 8-bit source values
   vec16u deltaX, deltaY;      // 8-bit delta value
   vec16u diff, r1, r2;        // Intermediate results

   // loop on destination pixels
   for (int bx = 0; bx < bw; bx++) chess_loop_range(1,) {
      for (int by = 0, idx_s = bx, idx_d = bx, idx_o = bx<<1; 
           by < bh; by++, idx_s+=sstr,idx_d+=dstr, idx_o+=bw<<1) chess_loop_range(1,)
      {
         s11 = (vec16u)src[idx_s];
         s12 = (vec16u)src[idx_s+1];
         s21 = (vec16u)src[idx_s+sstr];
         s22 = (vec16u)src[idx_s+sstr+1];
         
         deltaX = (vec16u)delta[idx_o];   // Delta X
         deltaY = (vec16u)delta[idx_o+1]; // Delta Y
         
         r1 = ((((s12 - s11) * deltaX) + 0x80) >> 8) + s11;
         r2 = ((((s22 - s21) * deltaX) + 0x80) >> 8) + s21;
         r1 &= 0x00ff;
         r2 &= 0x00ff;
         
         dst[idx_d] = (vec08u)(((((r2 - r1) * deltaY) + 0x80) >> 8) + r1);
      }
   } // next row
}

void
apu_interp_bicubic_grayscale(
            vec08u* dst, int dstr,
      const vec08u* src, int sstr,
            int bw, int bh,
            uint8_t x_offset, uint8_t y_offset)
{
   int16_t xlut[4];
  int16_t ylut[4];
   accessLUT(x_offset, y_offset, xlut, ylut);
  vec08u arr[4][4];

  for(int i = 0; i<bh; i++){
    for(int j = 0; j <bw; j++){
      for(int a = -1; a < 3; a++){
         arr[0][a+1] = src[j-1 + (i+a)*sstr];
         arr[1][a+1] = src[j+0 + (i+a)*sstr];
         arr[2][a+1] = src[j+1 + (i+a)*sstr];
         arr[3][a+1] = src[j+2 + (i+a)*sstr];
      }
        dst[j + (i)*dstr] = XYInterpolation(arr, x_offset, y_offset, xlut, ylut);
    }
  }
}

#endif /* ACF_KERNEL_IMPLEMENTATION */