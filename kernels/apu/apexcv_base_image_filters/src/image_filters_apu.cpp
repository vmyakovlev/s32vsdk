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
*  \file apu_filter.c
*  \brief APU filter kernels.
***********************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "image_filters_apu.h"
#include <math.h>

void
apu_flt_bilateral_5x5_init(vec08u __cmem *r_lut, uint8_t *d_lut, int sigmaR, int sigmaD)
{
   float inv_sigmaR = -0.5f / (sigmaR * sigmaR);
   float inv_sigmaD = -0.5f / (sigmaD * sigmaD);
   float y;
   
   for (int i = 0; i < 256; ++i)
   {
      y = expf((i * i) * inv_sigmaR);
      r_lut[i] = (uint8_t)(y * 255 + 0.5f);
   }
   
   for (int j = 0; j < 9; ++j)
   {
      y = expf(j * inv_sigmaD);
      d_lut[j] = (uint8_t)(y * 255 + 0.5f);
   }
}
             
void             
apu_flt_bilateral_5x5(  vec08u* dst, int dstr,
                  const vec08u* src, int sstr,
                             int bw, int bh,
               vec08u __cmem *r_lut, uint8_t *d_lut )
{  
   vec32u weight;          //FP  0_16
   vec32u sum_wi;          //FP 13_16
   vec32u sum_weight;      //FP  5_16
   
   for (int y = 0; y < bh; ++y) chess_loop_range(1,)
   {
      for (int x = 0; x < bw; ++x) chess_loop_range(1,) 
      {
         vec08u center_i = src[y * sstr + x];
         sum_weight = 0;
         sum_wi = 0;
         
         for (int fy = -2; fy < 3; ++fy) chess_loop_range(1,)
         {
            for (int fx = -2; fx < 3; ++fx) chess_loop_range(1,)
            { 
               vec08u filter_i = src[(y + fy) * sstr + (x + fx)];
               vec16s vidx = (vec16s)vabs_diffu((vec16u)center_i, (vec16u)filter_i);
               
         weight = ((vec32u)vload(r_lut, vidx)) * d_lut[fx*fx+fy*fy];
               sum_weight += weight;
               sum_wi += ((vec32u)filter_i )* weight;
            }
         }
         //TODO: temporary divide
         // dst[y*dstr+x] = (vec08u)((vec16u)((sum_wi / ((sum_weight+1) >> 1)) + 1) >> 1);
     dst[y*dstr+x] = (vec08u)(vec16u)((sum_wi+((sum_weight+1)>>1)) / sum_weight);
         //dst[y*dstr+x] = (vec08u)(vec16u)tmp;
      }
   }
}
#endif /* ACF_KERNEL_IMPLEMENTATION */
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
*  \file apu_filter.c
*  \brief APU filter kernels.
***********************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "image_filters_apu.h"
#include <math.h>

void apu_flt_box_1x3(
               vec16s* dst, int dstr,
         const vec16s* src, int sstr,
               int bw, int bh
)
{
   /** process 2 pixels at a time
    * load 4 pixels, store 2 pixels
    * use the partial addition from the middle two rows for the rows above and below 
    */
   for (int h = 0; h < bh; h+=2)  chess_loop_range(1,) {
      /* Row Pointers */
      const vec16s* restrict src1 = src-sstr;
      const vec16s* restrict src2 = src;
      const vec16s* restrict src3 = src+sstr;
      const vec16s* restrict src4 = src3+sstr;
      vec16s* restrict dst1 = dst;
      vec16s* restrict dst2 = dst + dstr;

      vec16s sum1, sum2, sum_partial, src1_val, src2_val, src3_val, src4_val;

      /* Loop Preamble */
      src1_val = *src1++;
      src2_val = *src2++;
      src3_val = *src3++;
      src4_val = *src4++;

      sum_partial = src2_val + src3_val;
      sum1 = src1_val + sum_partial;
      sum2 = src4_val + sum_partial;

      /* Core loop */
      for (int w = 0; w < bw; w++) chess_loop_range(1,) {
         src1_val = *src1++;
         src2_val = *src2++;
         src3_val = *src3++;
         src4_val = *src4++;

         *dst1++ = sum1;
         *dst2++ = sum2;

         sum_partial = src2_val + src3_val;
         sum1 = src1_val + sum_partial;
         sum2 = src4_val + sum_partial;
      }
      src += 2*sstr;
      dst += 2*dstr;
   }
}

void apu_flt_box_3x1(
               vec16s* dst, int dstr,
         const vec16s* src, int sstr,
               int bw, int bh
)
{
   for (int h = 0; h < bh; h++)  chess_loop_range(1,) {
      const vec16s* restrict src_temp = src-1;
      vec16s* restrict dst_temp = dst;

      vec16s sum, l, m, r;
      sum  = l = *src_temp++;
      sum += m = *src_temp++;
      sum += r = *src_temp++;

      for (int w = 0; w < bw; w++) chess_loop_range(1,) chess_unroll_loop(2) {
         l = m >> 0, m = r >> 0, r = *src_temp++; // compiler doesn't produce optimal code without shifting by 0
         *dst_temp++ = sum;
         sum = l+m+r;
      }
      src += sstr;
      dst += dstr;
   }
}

// http://www.hackersdelight.org/divcMore.pdf Page 6
vec16u vdivu_by9(vec16u n)
{
   vec16u q, r;
   
   q = n - (n >> 3);
   q = q + (q >> 6);
   q = q >> 3;
   r = n - q * 9;
   
   return q + ((r + 7) >> 4);
}

void             
apu_flt_box_3x3( 
               vec08u* dst, int dstr,
         const vec08u* src, int sstr,
               int bw, int bh
)
{  
   for (int y = 0; y < bh; ++y) chess_loop_range(1,) {
      for (int x = 0; x < bw; ++x) chess_loop_range(1,) {
         vec16u sum = 0;
         
         for (int fy = -1; fy < 2; ++fy) chess_loop_range(1,) {
            for (int fx = -1; fx < 2; ++fx) chess_loop_range(1,) { 
               sum += (vec16u)src[(y + fy) * sstr + (x + fx)];
            }
         }
         //divide by 9
         dst[y*dstr+x] = (vec08u)((vdivu_by9(sum << 1) + 1) >> 1);
      }
   }
}

// Local helper functions
static inline vec16s _box_3x3(const vec16s *src, int stride);
static inline vec16s _box_5x5(const vec16s *src, int stride);
static inline vec16s _box_3x3_copy(const vec16s *src, int stride, vec16s prev);
static inline vec16s _box_5x5_copy(const vec16s *src, int stride, vec16s prev);

void apu_flt_box_3x3(
               vec16s* dst, int dstr,
         const vec16s* src, int sstr,
               int bw, int bh
)
{
//! Apply box filter
    vec16s prev;
    for (int h = 0; h < bh; h++) chess_loop_range(1,) {
        prev = _box_3x3(&src[0], sstr);  // first column in each row adds up 3x3 pixels
        dst[0] = prev;
        for (int w = 1; w < bw; w++) chess_loop_range(1,) {
            prev = _box_3x3_copy(&src[w], sstr, prev); // use previous sum to calculate current pixel sum
            dst[w] = prev;
        }
        src += sstr;
        dst += dstr;
    }
}

//! [apply 3x3 box]
// summation of 3x3 pixels
static inline vec16s _box_3x3(const vec16s *src, int stride)
{
    vec16s t0 = src[-stride-1] + src[-stride] + src[-stride+1]
                + src[-1] + src[0] + src[1] + src[stride-1] + src[stride] + src[stride+1];

    return t0;
}

// to decrease # of addition, subtract colomn[-2] from previous sum and add column[1]
static inline vec16s _box_3x3_copy(const vec16s *src, int stride, vec16s prev)
{
    vec16s tp = -src[-stride-2] + src[-stride+1]
                -src[-2]        + src[1]
                -src[stride-2]  + src[stride+1];

    return prev+tp;
}
//! [apply 3x3 box]

void apu_flt_box_5x5( 
               vec16s* dst, int dstr,
         const vec16s* src, int sstr,
               int bw, int bh
)
{
//! Apply box filter
    vec16s prev;
    for (int h = 0; h < bh; h++) chess_loop_range(1,) {
        prev = _box_5x5(&src[0], sstr);  // first column in each row adds up 5x5 pixels
        dst[0] = prev;
        for (int w = 1; w < bw; w++) chess_loop_range(1,) {
            prev = _box_5x5_copy(&src[w], sstr, prev); // use previous sum to calculate current pixel sum
            dst[w] = prev;
        }
        src += sstr;
        dst += dstr;
    }
}

//! [apply 5x5 box]
// summation of 5x5 pixels
static inline vec16s _box_5x5(const vec16s *src, int stride)
{
    vec16s t0 = src[-2*stride-2] + src[-2*stride-1] + src[-2*stride] + src[-2*stride+1] + src[-2*stride+2];
    vec16s t1 = src[-stride-2]   + src[-stride-1]   + src[-stride]   + src[-stride+1]   + src[-stride+2];
    vec16s t2 = src[-2]          + src[-1]          + src[0]         + src[1]           + src[2];
    vec16s t3 = src[stride-2]    + src[stride-1]    + src[stride]    + src[stride+1]    + src[stride+2];
    vec16s t4 = src[2*stride-2]  + src[2*stride-1]  + src[2*stride]  + src[2*stride+1]  + src[2*stride+2];

    return t0 + t1 + t2 + t3 + t4;
}

// to decrease # of addition, subtract colomn[-3] from previous sum and add column[2]
static inline vec16s _box_5x5_copy(const vec16s *src, int stride, vec16s prev)
{
    vec16s tp = -src[-2*stride-3]   + src[-2*stride+2]
                -src[-stride-3]     + src[-stride+2]
                -src[-3]            + src[2]
                -src[stride-3]      + src[stride+2]
                -src[2*stride-3]    + src[2*stride+2];

    return prev+tp;
}
//! [apply 5x5 box]

#endif /* ACF_KERNEL_IMPLEMENTATION */
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
*  \file apu_filter.c
*  \brief APU filter kernels.
***********************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "image_filters_apu.h"

namespace ht{/* Namespace ht */

inline static vec08u div9(vec16u in)
{   
    // 1/9 = 0.1111; 0.1111 * 2^8 = 28.4444 => a / 9 = a * 28
  vec16u acm = vec16u(28);
  vec16u mult = in * acm;
    acm        += mult + (mult>>6);    
    acm       >>= 8;  
    return (vec08u)acm;
}

void apu_flt_box_3x3(vec08u* dst, int dstr, const vec08u* src, int sstr, int width, int heigth)
{
    int x, y;
        
    vec16u   centerLeftSum;
    vec16u   centerRightSum;
    vec16u   westSum;
    vec16u   eastSum;
    vec16u   acc0;
    vec16u   acc1;
    vec16u   centerSum;
    
  /*
  Input = 
  |a|b|c|d|
  |e|f|g|h|
  |i|j|k|l|
        cmn = b + c + f + g + j + k;
  *out++ = (a + e + i + cmn)/9;
  *out++ = (d + h + i + cmn)/9;
  */
    
    for(y=0;y<heigth;y++) chess_loop_range(1,)
    {        
        vec08u* restrict dest   = (vec08u* restrict)(dst);
        vec08u* restrict line0  = (vec08u* restrict)(src - sstr - 1);
        vec08u* restrict line1  = (vec08u* restrict)(src - 1);
        vec08u* restrict line2  = (vec08u* restrict)(src + sstr - 1);
        
        // westSum = a + e + i;
        westSum        = (vec16u)*line0++ + 
                         (vec16u)*line1++ + 
                         (vec16u)*line2++;
        
        //centerLeftSum = b + f + j;        
        centerLeftSum  = (vec16u)*line0++ + 
                         (vec16u)*line1++ + 
                         (vec16u)*line2++;
        
        //centerRightSum = c + g + k;
        centerRightSum = (vec16u)*line0++ + 
                         (vec16u)*line1++ + 
                         (vec16u)*line2++;
                         
        vec16u cmn =  centerLeftSum;
        for(x=0;x<width;x+=2) chess_loop_range(1,)
        {   
            // Load the right side data
            eastSum = (vec16u)*line0++ +  
                      (vec16u)*line1++ +  
                      (vec16u)*line2++;                
            
            // Update the center sum which is common for both samples
            cmn += centerRightSum;
            
            // Accumulate for both output samples
            acc0 = (vec16u)(westSum + cmn); 
            acc1 = (vec16u)(eastSum + cmn);  
            
            // Division by 9 
            *dest++ = (vec08u)(div9(acc0));
            *dest++ = (vec08u)(div9(acc1));
          
            // Change the state of the common data region
            westSum        = centerRightSum;
            cmn            = eastSum;      
            centerRightSum = ((vec16u)*line0++) + ((vec16u)*line1++) + ((vec16u)*line2++);
            
        }
        src += sstr;
        dst += dstr;
    }
}
}/* Namespace ht */
#endif /* ACF_KERNEL_IMPLEMENTATION */
/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
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

#ifdef ACF_KERNEL_IMPLEMENTATION
#include "image_filters_apu.h"

void apu_flt_census_3x3(vec08u * dst, int dstr, const vec08u * src, int sstr, int bw, int bh)
{
    int col = 0;
    int row = 0;
    
    vec16s acc;
    vec08u a, b, c, d, e, f, g, h, i;

    /*
        |a|b|c|
        |d|e|f|
        |g|h|i|        
        out = (a>e)<<7 | (b>e)<<6 | (c>e)<<5 | (d>e)<<4 | (f>e)<<3 | (g>e)<<2 | (h>e)<<1 | (i>e);
    */    
    for (row=0; row<bh; row++) chess_loop_range(1,)
    {    
    vec08u* restrict line0 = (vec08u* restrict)(src - sstr - 1);
    vec08u* restrict line1 = (vec08u* restrict)(src - 1);
    vec08u* restrict line2 = (vec08u* restrict)(src + sstr - 1);
      vec08u* restrict out   = (vec08u* restrict)(dst);

        a = *line0++; 
    b = *line0++;
    d = *line1++;
    e = *line1++;
    g = *line2++;
    h = *line2++;
        for (col=0; col<bw; col++) chess_loop_range(1,)
        {               
      c = *line0++;
      f = *line1++;
      i = *line2++;

            acc =   (vec16s)(a > e);   
      acc = vslc(acc, (b > e));
      acc = vslc(acc, (c > e));
      acc = vslc(acc, (d > e));
      acc = vslc(acc, (f > e));
      acc = vslc(acc, (g > e));
      acc = vslc(acc, (h > e));
      acc = vslc(acc, (i > e));
            
            // Save the intermediate values for the next iteration of the loop
      *out++ = (vec08u)(acc);
            a = b;
            b = c;
            d = e;
            e = f;
            g = h;
            h = i;    
        }
        src += sstr;
        dst += dstr;
    }    
}
#endif /* ACF_KERNEL_IMPLEMENTATION */

/*!*********************************************************************************
*  \file apu_filter.c
*  \brief APU filter kernels.
***********************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "image_filters_apu.h"
#include <math.h>

namespace ht{/* Namespace ht */

void apu_flt_centraldx_3x3(vec16s* restrict dst,
                           int dstr,
                           const vec08u* restrict src,
                           int sstr,
                           int bw,
                           int bh,
                           signed char k0,
                           signed char k1,
                           signed char k2)
{
    
    /*    
     The algo works on 16 bit inputs
    Input:
    ---------
    |a|b|c|d|
    ---------
    |e|f|g|h|
    ---------
    |i|j|k|l|
    ---------
    k0, k1, k2
    out0 = (c-a)*k0 + (g-e)*k1 + (k-i)*k2
    out1 = (d-b)*k0 + (h-f)*k1 + (l-j)*k2
    The algo can have a very big reusability factor:
    x = line0, y = line1 and z = line2
    posSum0 = x[2]*k0 + y[2]*k1 + z[2]*k2;
    negSum0 = x[0]*k0 + y[0]*k1 + z[0]*k2;
    out[0] = posSum0 - negSum0;
    posSum1 = x[3]*k0 + y[3]*k1 + z[3]*k2;
    negSum1 = x[1]*k0 + y[1]*k1 + z[1]*k2;
    out[1] = posSum1 - negSum1;
    -------------------------------------
    posSum0 = x[4]*k0 + y[4]*k1 + z[4]*k2;
    negSum0 = x[2]*k0 + y[2]*k1 + z[2]*k2;
    out[2] = posSum0 - negSum0;
    posSum1 = x[5]*k0 + y[5]*k1 + z[5]*k2;
    negSum1 = x[3]*k0 + y[3]*k1 + z[3]*k2;
    out[3] = posSum1 - negSum1;
    
    One can see that negSum0 step 3 is equal to posSum0 step 1 ! */

    int x, y;
    vec16s posSum0;
    vec16s posSum1;
    vec16s negSum0;
    vec16s negSum1;
    
    vec16u newDataLine0;
    vec16u newDataLine1;
    vec16u newDataLine2;
    
    vec16s mpyK0;
    vec16s mpyK1; 
    vec16s mpyK2;
        
    vec16u chess_storage(chess_register) prevDataLine0;
    vec16u chess_storage(chess_register) prevDataLine1;
    vec16u chess_storage(chess_register) prevDataLine2;
    
    for (y = 0; y<bh; y++) chess_loop_range(1,)
    {
        vec16u* restrict line0 = (vec16u* restrict)(src - sstr - 2);
        vec16u* restrict line1 = (vec16u* restrict)(src - 2);
        vec16u* restrict line2 = (vec16u* restrict)(src + sstr - 2);
        vec16s* restrict dest = dst;
        
        /* Calculate the loop preamble */
        newDataLine0 = *line0++;
        newDataLine1 = *line1++;
        newDataLine2 = *line2++;    
      negSum0      = (vec16s)vmul_uhsl(newDataLine0, k0) + vmul_uhsl(newDataLine1, k1) + vmul_uhsl(newDataLine2, k2);
        
        newDataLine0 = *line0++;
        newDataLine1 = *line1++;
        newDataLine2 = *line2++;                   
        posSum0      = (vec16s)vmul_uhsl(newDataLine0, k0) + vmul_uhsl(newDataLine1, k1) + vmul_uhsl(newDataLine2, k2);
        negSum1      = (vec16s)vmul_ulsl(newDataLine0, k0) + vmul_ulsl(newDataLine1, k1) + vmul_ulsl(newDataLine2, k2);        
        
        newDataLine0 = *line0++;
        newDataLine1 = *line1++;
        newDataLine2 = *line2++;
        
        prevDataLine0 = newDataLine0;        
        prevDataLine1 = newDataLine1;
        prevDataLine2 = newDataLine2;
        
        mpyK0 = (vec16s)vmul_ulsl(prevDataLine0, k0);
        mpyK1 = (vec16s)vmul_ulsl(prevDataLine1, k1);
        mpyK1 = mpyK0 + mpyK1;        
        mpyK2 = (vec16s)vmul_ulsl(prevDataLine2, k2);        
        
        vec16s d0;
        vec16s d1;
            
        for (x = 0; x<bw; x += 2) chess_loop_range(1,)
        {
             /*        |      ALU              |    Shift                    |          Mul                                   |      Ld/St      */             
            posSum1 = mpyK1 + mpyK2;                            mpyK0 = (vec16s)vmul_uhsl(prevDataLine0, k0); newDataLine0 = *line0++;            
            d0      = posSum0 - negSum0;                        mpyK1 = (vec16s)vmul_uhsl(prevDataLine1, k1); newDataLine1 = *line1++;            
            mpyK1   = mpyK0 + mpyK1;     negSum0 = posSum0 <<0; mpyK2 = (vec16s)vmul_uhsl(prevDataLine2, k2); newDataLine2 = *line2++; prevDataLine0 = newDataLine0;          
            posSum0 = mpyK1 + mpyK2;                            mpyK0 = (vec16s)vmul_ulsl(newDataLine0, k0);                           prevDataLine1 = newDataLine1;           
            d1      = posSum1 - negSum1;                        mpyK1 = (vec16s)vmul_ulsl(newDataLine1, k1);       *dest++ = d0;       prevDataLine2 = newDataLine2;           
            mpyK1   = mpyK0 + mpyK1;     negSum1 = posSum1 <<0; mpyK2 = (vec16s)vmul_ulsl(newDataLine2, k2);       *dest++ = d1;                       
        }               
        src += sstr;
        dst += dstr;
    }
}


void apu_flt_centraldy_3x3(vec16s* restrict dst, int dstr, const vec08u* restrict src,
  int sstr, int bw, int bh,
  signed char k0, signed char k1, signed char k2)
{
  int x, y;
  vec16s newDataLine0;
  vec16s newDataLine2;

  vec16s diff0, diff1, diff2, diff3;
  for (y = 0; y < bh; y++) chess_loop_range(1, )
  {
    /* Pointers */
    vec08u * restrict line0 = (vec08u * restrict)(src - sstr - 1);
    vec08u * restrict line2 = (vec08u * restrict)(src + sstr - 1);
    vec16s * restrict dest = dst;

    newDataLine0 = (vec16s)(*line0++);
    newDataLine2 = (vec16s)(*line2++);
    diff0 = (newDataLine2 - newDataLine0);

    newDataLine0 = (vec16s)(*line0++);
    newDataLine2 = (vec16s)(*line2++);
    diff1 = (newDataLine2 - newDataLine0);
    
    newDataLine0 = (vec16s)(*line0++);
    newDataLine2 = (vec16s)(*line2++);
    diff2 = (newDataLine2 - newDataLine0);
    
    // out = (line2[x] - line0[x])*k0 + (line2[x+1] - line0[x+1])*k1 + (line2[x+2] - line0[x+2])*k2;
    for (x = 0; x < bw; x += 2) chess_loop_range(1, )
    {
      *dest++ = diff0 * k0 + diff1 * k1 + diff2 * k2;
      newDataLine0 = (vec16s)(*line0++);
      newDataLine2 = (vec16s)(*line2++);
      diff3 = (newDataLine2 - newDataLine0);      
      *dest++ = diff1 * k0 + diff2 * k1 + diff3 * k2;
      diff0 = diff2;
      diff1 = diff3;
      diff2 = ((vec16s)(*line2++) - (vec16s)(*line0++));
    }

    /* Increment pointers */
    src += sstr;
    dst += dstr;
  }
}
}/* Namespace ht */
#endif /* ACF_KERNEL_IMPLEMENTATION */
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
*  \file apu_filter.c
*  \brief APU filter kernels.
***********************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "image_filters_apu.h"
#include <math.h>

/*!
   Calculate a gaussian 3x3 filter.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void apu_flt_gaussian_3x3(vec08u* dst, int dstr, const vec08u* src, int sstr, int bw, int bh)
{
   //  1,  2,  1,
   //  2,  4,  2,  * 1/16
   //  1,  2,  1
   
    // Column accumulators
    vec16u sumCol0;
    vec16u sumCol1;
    vec16u sumCol2;
    vec16u sumCol3;
    
    // Locals that will hold the current memory data
    vec16u dataLine0;
    vec16u dataLine1;
    vec16u dataLine2;
    
    // Multi-sample temporary storage
    vec16u acc0;
    vec16u acc1;
    
    // Partial filter coefficients
    unsigned int c0 = 1;
    unsigned int c1 = 2;
    unsigned int c2 = 1;
    
    for (int y = 0; y < bh; y++) chess_loop_range(1,) 
    {
        // Prepare pointers
        vec08u* restrict line0 = (vec08u* restrict)(src - sstr - 1);
        vec08u* restrict line1 = (vec08u* restrict)(src        - 1);
        vec08u* restrict line2 = (vec08u* restrict)(src + sstr - 1);
        
        // Load data from main memory
        dataLine0 = (vec16u)(*line0++);
        dataLine1 = (vec16u)(*line1++);
        dataLine2 = (vec16u)(*line2++);
        
        // Precalculate the first column
        sumCol0 = dataLine0 + (dataLine1 << 1) + dataLine2;
        
        // Load data from main memory
        dataLine0 = (vec16u)(*line0++);
        dataLine1 = (vec16u)(*line1++);
        dataLine2 = (vec16u)(*line2++);
        
        // Precalculate the second column
        sumCol1 = dataLine0 + (dataLine1 << 1) + dataLine2;
        
        for (int x = 0; x < bw; x += 2) chess_loop_range(1,) 
        {
            // Load data from main memory
            dataLine0  = (vec16u)(*line0++);
            dataLine1  = (vec16u)(*line1++);
            dataLine2  = (vec16u)(*line2++);
            
            // Precalculate column
            sumCol2    = dataLine0 + (dataLine1 << 1) + dataLine2;
            
            // Load data from main memory
            dataLine0  = (vec16u)(*line0++);
            dataLine1  = (vec16u)(*line1++);
            dataLine2  = (vec16u)(*line2++);
            
            // Precalculate column
            sumCol3    = dataLine0 + (dataLine1 << 1) + dataLine2;
            
            // Accumulate samples for each output
            acc0       = sumCol0 + 2 * sumCol1 + sumCol2;
            acc1       = sumCol1 + 2 * sumCol2 + sumCol3;
            
            // Multi-sample store
            dst[x + 0] = (vec08u)(acc0 >> 4);
            dst[x + 1] = (vec08u)(acc1 >> 4);
            
            // Save column accumulators for next iteration
            sumCol0   = sumCol2;
            sumCol1   = sumCol3;
        }      
        src += sstr;
        dst += dstr;
    }
}

/*!
   Calculate a gaussian 5x5 filter.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void apu_flt_gaussian_5x5(vec08u* dst, int dstr, const vec08u* src, int sstr, int bw, int bh)
{
    //  1,   4,   6,  4,  1,
    //  4,  16,  24, 16,  4, 
    //  6,  24,  36, 24,  6, * 1/256
    //  4,  16,  24, 16,  4,
    //  1,   4,   6,  4,  1,
    int x, y;
    vec16u sumCol0;
    vec16u sumCol1;
    vec16u sumCol2;
    vec16u sumCol3;    
    vec16u sumCol4;    
    vec16u sumCol5;    
    
    vec16u acc0;
    vec16u acc1;
    
    // Partial filter coefficients
    unsigned int c0 = 1;
    unsigned int c1 = 4;
    unsigned int c2 = 6;
    unsigned int c3 = 4;
    unsigned int c4 = 1;
    
    for(y = 0; y < bh; y++) chess_loop_range(1,)
    {        
        vec16u* restrict line0 = (vec16u* restrict)(src - 2 * sstr - 2);
        vec16u* restrict line1 = (vec16u* restrict)(src     - sstr - 2);
        vec16u* restrict line2 = (vec16u* restrict)(src            - 2);  
        vec16u* restrict line3 = (vec16u* restrict)(src     + sstr - 2);
        vec16u* restrict line4 = (vec16u* restrict)(src + 2 * sstr - 2);
        
        // Read data from main memory 2 Bytes at a time
        vec16u dataLine0 = (*line0++);
        vec16u dataLine1 = (*line1++);
        vec16u dataLine2 = (*line2++);
        vec16u dataLine3 = (*line3++);
        vec16u dataLine4 = (*line4++);        
        
        // Column accumulators
        sumCol0 = vmul_ulul(dataLine0, c0) + vmul_ulul(dataLine1, c1) + vmul_ulul(dataLine2, c2) + vmul_ulul(dataLine3, c3) + vmul_ulul(dataLine4, c4);
        sumCol1 = vmul_uhul(dataLine0, c0) + vmul_uhul(dataLine1, c1) + vmul_uhul(dataLine2, c2) + vmul_uhul(dataLine3, c3) + vmul_uhul(dataLine4, c4);
        
        // Read data from main memory 2 Bytes at a time
        dataLine0 = (*line0++);
        dataLine1 = (*line1++);
        dataLine2 = (*line2++);
        dataLine3 = (*line3++);
        dataLine4 = (*line4++);        
        
        // Column accumulators
        sumCol2 = vmul_ulul(dataLine0, c0) + vmul_ulul(dataLine1, c1) + vmul_ulul(dataLine2, c2) + vmul_ulul(dataLine3, c3) + vmul_ulul(dataLine4, c4);
        sumCol3 = vmul_uhul(dataLine0, c0) + vmul_uhul(dataLine1, c1) + vmul_uhul(dataLine2, c2) + vmul_uhul(dataLine3, c3) + vmul_uhul(dataLine4, c4);
        
        for(x = 0;x < bw; x += 2) chess_loop_range(1,)
        {
            // Precalculating the filter output
            acc0 = sumCol0 + (sumCol1 << 2) + 6 * sumCol2 + (sumCol3 << 2);
            acc1 = sumCol1 + (sumCol2 << 2) + 6 * sumCol3;
            
            // Read data from main memory
            dataLine0 = (*line0++);
            dataLine1 = (*line1++);
            dataLine2 = (*line2++);
            dataLine3 = (*line3++);
            dataLine4 = (*line4++);            

            // Accumulating the last two columns in the 5x5 matrix
            sumCol4 = vmul_ulul(dataLine0, c0) + vmul_ulul(dataLine1, c1) + vmul_ulul(dataLine2, c2) + vmul_ulul(dataLine3, c3) + vmul_ulul(dataLine4, c4);
            sumCol5 = vmul_uhul(dataLine0, c0) + vmul_uhul(dataLine1, c1) + vmul_uhul(dataLine2, c2) + vmul_uhul(dataLine3, c3) + vmul_uhul(dataLine4, c4);
            
            // Adding the rest of the data to the multi-sample output
            acc0 += sumCol4;
            acc1 += (sumCol4 << 2) + sumCol5;
            
            // Dividing by 256
            dst[x + 0] = (vec08u)(acc0 >> 8);
            dst[x + 1] = (vec08u)(acc1 >> 8);
            
            // Saving the partial sums for the next iteration
            sumCol0 = sumCol2; sumCol1 = sumCol3; sumCol2 = sumCol4; sumCol3 = sumCol5;        
        }
        src += sstr;
        dst += dstr;
    }
}
#endif /* ACF_KERNEL_IMPLEMENTATION */

/*!*********************************************************************************
*  \file apu_filter.c
*  \brief APU filter kernels.
***********************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "image_filters_apu.h"
#include <math.h>



/**
 * Computes the positive/negative gains of the filter by summing up all
 * positive and negative coefficients together separately and returning both.
 */

static inline void filterGain8bit(const int8_t* flt,
                                  unsigned      numTaps,
                                  int32_t&      pos,
                                  int32_t&      neg){
    
    pos = neg = 0;
    
    for(unsigned i=0;i<numTaps;i++){
        flt[i] >= 0 ? pos += flt[i] : neg += flt[i];
    }
}

/**
 * Computes from the filter's gain the right shift required for
 * normalization of intermediates to yield an 8-bit unsigned value.
 * 
 * If the gain is not an exact power-of-2, a compromise is adopted. The
 * shift is computed to yield a value greater than 8 but less than 9 bits
 * in size. A saturation will be then required to clip off excesses
 * beyond the range of 8-bit unsigned.
 * 
 * Assumes the filter gain is non-zero.
 */

static inline int filterShiftU8bit(uint32_t posGain){
    return 31-clz(posGain);
}

/*!
   Calculate the output of generic (fw x fh) filter.
   
   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
   \param flt  - [Input]  Filter Coeffiencients (pointer)
   \param fw   - [Input]  Filter width
   \param fh   - [Input]  Filter height
 */
void  
apu_flt_fir2(
         vec08u* restrict dst_, int dstr,        
   const vec08u* restrict src, int sstr,        
   int bw, int bh,                     
   const int8_t* restrict flt, int fw, int fh)
{
   // Adjust src
   src -= ((fh >> 1)*sstr + (fw >> 1));
   
   for (int by=0; by<bh; ++by) chess_loop_range(1,) 
   {
      for (int bx=0; bx<bw; ++bx) chess_loop_range(1,) 
      {
         vec16s r = 0;
         for (int fy=0, fk=0; fy<fh; ++fy) chess_loop_range(1,) 
         {
            for (int fx=0; fx<fw; ++fx, ++fk) chess_loop_range(1,) 
            {
               r += ((vec16s)flt[fk] * (vec16s)src[(by+fy)*sstr + (bx+fx)]);
            }
         }
         
         // Saturation
         r = vsat(r, 0, 255);
         
         dst_[by*dstr + bx] = (vec08u)r;
      }
   }
}   


/*!
   Calculate the output of generic (fw x fh) filter with scaling.
   
   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
   \param flt  - [Input]  Filter Coeffiencients (pointer)
   \param fw   - [Input]  Filter width
   \param fh   - [Input]  Filter height
   \param fscl - [Input]  Filter Scale (2^fscl)
 */

void  
apu_flt_fir2(
         vec08u* restrict dst, int dstr,        
   const vec08u* restrict src, int sstr,        
   int bw, int bh,                     
   const int8_t* restrict flt, int fw, int fh,  
   int fscl)                     
{
   // Adjust src
   src -= ((fh >> 1)*sstr + (fw >> 1));
   
   const int fone = (1 << fscl);
   
   for (int by=0; by<bh; ++by) chess_loop_range(1,) 
   {
      for (int bx=0; bx<bw; ++bx) chess_loop_range(1,) 
      {
         vec16s r = 0;
         for (int fy=0, fk=0; fy<fh; ++fy) chess_loop_range(1,)  
         {
            for (int fx=0; fx<fw; ++fx, ++fk) chess_loop_range(1,) 
            {
               r += ((vec16s)flt[fk] * (vec16s)src[(by+fy)*sstr + (bx+fx)]);
            }
         }
         // Rounding & Scaling
         r  += (fone >> 1);
         r >>= fscl;
         
         // Saturation
         r = vsat(r, 0, 255);
         
         dst[by*dstr + bx] = (vec08u)r;
      }
   }
}   

/*!
   Convolve: Calculate the output of generic (fw x fh) filter.
   
   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
   \param flt  - [Input]  Filter Coeffiencients (pointer)
   \param fw   - [Input]  Filter width
   \param fh   - [Input]  Filter height
 */
void  
apu_flt_fir2(
         vec16s* restrict dst, int dstr,        
   const vec08u* restrict src, int sstr,        
   int bw, int bh,                     
   const int8_t* restrict flt, int fw, int fh)
{
   // Adjust src
   src -= ((fh >> 1)*sstr + (fw >> 1));
   
   for (int by=0; by<bh; ++by) chess_loop_range(1,) 
   {
      for (int bx=0; bx<bw; ++bx) chess_loop_range(1,) 
      {
         vec32s r = 0;
         for (int fy=0, fk=0; fy<fh; ++fy) chess_loop_range(1,) 
         {
            for (int fx=0; fx<fw; ++fx, ++fk) chess_loop_range(1,) 
            {
               r += (vec32s)(flt[fk] * (vec16s)src[(by+fy)*sstr + (bx+fx)]);
            }
         }
         
         // Saturation TODO: use vsat for 32 bit vector once supported
         r = vselect(r, vec32s(32767), r <= 32767);
     r = vselect(r, vec32s(-32768), r >= -32768);
         
         dst[by*dstr + bx] = (vec16s)r;
      }
   }
}


/*!
   Convolve: Calculate the output of generic (fw x fh) filter with scaling.
   
   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
   \param flt  - [Input]  Filter Coeffiencients (pointer)
   \param fw   - [Input]  Filter width
   \param fh   - [Input]  Filter height
   \param fscl - [Input]  Filter Scale (2^fscl)
 */
void  
apu_flt_fir2(
         vec16s* dst, int dstr,        
   const vec08u* src, int sstr,        
   int bw, int bh,                     
   const int8_t* flt, int fw, int fh,  
   int fscl)                     
{
   // Adjust src
   src -= ((fh >> 1)*sstr + (fw >> 1));
   
   const int fone = (1 << fscl);
   
   for (int by=0; by<bh; ++by) chess_loop_range(1,) 
   {
      for (int bx=0; bx<bw; ++bx) chess_loop_range(1,) 
      {
         vec32s r = 0;
         for (int fy=0, fk=0; fy<fh; ++fy) chess_loop_range(1,) 
         {
            for (int fx=0; fx<fw; ++fx, ++fk) chess_loop_range(1,) 
            {
               r += (vec32s)(flt[fk] * (vec16s)src[(by+fy)*sstr + (bx+fx)]);
            }
         }
         // Rounding & Scaling
         r  += (fone >> 1);
         r >>= fscl;
         
         // Saturation TODO: use vsat for 32 bit vector once supported
         r = vselect(r, vec32s(32767), r <= 32767);
     r = vselect(r, vec32s(-32768), r >= -32768);
         
         dst[by*dstr + bx] = (vec16s)r;
      }
   }
}


#endif /* ACF_KERNEL_IMPLEMENTATION */

/*!*********************************************************************************
*  \file apu_filter.c
*  \brief APU filter kernels.
***********************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "image_filters_apu.h"
#include <math.h>

namespace ht{/* Namespace ht */

#define VMAC_HUHS(accH, accL, a, b) {vec16s mult = vmul_uhsh(a, b); vacl(accH, accL, mult);};
#define VMAC_LULS(accH, accL, a, b) {vec16s mult = vmul_ulsl(a, b); vacl(accH, accL, mult);};
#define VMAC_HULS(accH, accL, a, b) {vec16s mult = vmul_uhsl(a, b); vacl(accH, accL, mult);};
#define VMAC_LUHS(accH, accL, a, b) {vec16s mult = vmul_ulsh(a, b); vacl(accH, accL, mult);};

/**
     * Computes the positive/negative gains of the filter by summing up all
     * positive and negative coefficients together separately and returning both.
 */

static inline void filterGain8bit(const int8_t* flt,
                                  unsigned      numTaps,
                                  int32_t&      pos,
                                  int32_t&      neg)
{
    pos = neg = 0;
    for(unsigned i=0;i<numTaps;i++){
        flt[i] >= 0 ? pos += flt[i] : neg += flt[i];
    }
}

/**
     * Computes from the filter's gain the right shift required for
     * normalization of intermediates to yield an 8-bit unsigned value.
     *
     * If the gain is not an exact power-of-2, a compromise is adopted. The
     * shift is computed to yield a value greater than 8 but less than 9 bits
     * in size. A saturation will be then required to clip off excesses
     * beyond the range of 8-bit unsigned.
     *
     * Assumes the filter gain is non-zero.
*/

static inline int filterShiftU8bit(uint32_t posGain)
{
    return 31 - clz(posGain);
}

/*!
    Calculate the output of generic (3 x 3) filter.
    \param dst  - [Output] Destination block pointer
    \param dstr - [Input]  Destination block stride
    \param src  - [Input]  Source block pointer
    \param sstr - [Input]  Source block stride
    \param bw   - [Input]  Block width
    \param bh   - [Input]  Block height
    \param flt  - [Input]  Filter Coeffiencients (pointer)
*/

#if 1    
   
void apu_flt_generic_3x3(vec08u* restrict dst, 
                         int dstr, 
                         const vec08u* restrict src,
                         int sstr, 
                         int bw, 
                         int bh, 
                         const int8_t* restrict flt, 
                         int fscale)
{
   /* Adjust pointers for our needs */
   src -= sstr;

   /* Compute gain of filter */
   int normSh = fscale;
   vec32s vNormSh = normSh;

   /* Load filter coefficients in registers */
   const int32_t* f = (const int32_t*)flt;
   int32_t lf0  = *f++;
   int32_t lf1  = *f++;
   int32_t lf2  = *f++;

   /* For each row... */
   for (int by=0; by<bh; ++by) chess_loop_range(1,){
      /* Compute row pointers */
      vec08u* restrict chess_storage(R5) dst_ = chess_copy(dst);
      const vec16u* va = (const vec16u*)(src);
      const vec16u* vb = (const vec16u*)((const vec08u*)va + sstr);
      const vec16u* vc = (const vec16u*)((const vec08u*)vb + sstr);
      vec16s chess_storage(V0) lh = 0;
      vec16s chess_storage(V2) rh = 0;
      vec16u chess_storage(V1) ll = 0;
      vec16u chess_storage(V3) rl = 0;

      /* Load initial column */
      vec16u v0 = va++[-1], v1 = vb++[-1], v2 = vc++[-1],
             v3,            v4,            v5;

      /* For each column... */
      for (int bx=0; bx<bw; bx+=2) chess_loop_range(1,){
         /* Temporaries */
         vec16s chess_storage(V7) lm;
         vec16s chess_storage(V7) rm;

         /**
          * Pessimization: Force coefficients to be unpacked every iteration,
          * avoiding the compiler's preference for spilling to memory
          * (and consuming power).
          */

         lf0         = chess_copy(lf0);/* Defeat LICM */
         lf1         = chess_copy(lf1);/* Defeat LICM */
         lf2         = chess_copy(lf2);/* Defeat LICM */
         int32_t f0  = lf0 >>  0;
         int32_t f1  = lf0 >> 16;
         int32_t f2  = lf1 >>  0;
         int32_t f3  = lf1 >> 16;
         int32_t f4  = lf2 >>  0;

         /**
          * f0l f0h f1l
          * f1h f2l f2h
          * f3l f3h f4l
          */

         /* HOT ZONE BEGIN */
         /*      VACC      |            VMUL            |    S/VLD     | */
                             lm = (vec16s)vmul_uhsl(v0,  f0)    , v0   = *va-- ;
         vacl(lh, ll, lm)  , lm = (vec16s)vmul_uhsh(v1,  f1)    , v1   = *vb-- ;
         vacl(lh, ll, lm)  , lm = (vec16s)vmul_uhsl(v2,  f3)    , v2   = *vc-- ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_ulsl(v0,  f1)    , v0   = *va, va+=2;
         vacl(rh, rl, rm)  , rm = (vec16s)vmul_ulsh(v1,  f2)    , v1   = *vb, vb+=2;
         vacl(rh, rl, rm)  , rm = (vec16s)vmul_ulsl(v2,  f4)    , v2   = *vc, vc+=2;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_ulsh(v0,  f0)                   ;
         vacl(lh, ll, lm)  , lm = (vec16s)vmul_uhsl(v0,  f1)                   ;
         vacl(lh, ll, lm)  , lm = (vec16s)vmul_ulsl(v1,  f2)                   ;
         vacl(lh, ll, lm)  , lm = (vec16s)vmul_uhsh(v1,  f2)                   ;
         vacl(lh, ll, lm)  , lm = (vec16s)vmul_ulsh(v2,  f3)                   ;
         vacl(lh, ll, lm)  , lm = (vec16s)vmul_uhsl(v2,  f4)                   ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_ulsl(v0,  f0)                   ;
         vacl(rh, rl, rm)  , rm = (vec16s)vmul_uhsh(v0,  f0)                   ;
         vacl(rh, rl, rm)  , rm = (vec16s)vmul_ulsh(v1,  f1)                   ;
         vacl(rh, rl, rm)  , rm = (vec16s)vmul_uhsl(v1,  f2)                   ;
         vacl(rh, rl, rm)  , rm = (vec16s)vmul_ulsl(v2,  f3)                   ;
         vacl(rh, rl, rm)  , rm = (vec16s)vmul_uhsh(v2,  f3)                   ;
         vacl(rh, rl, rm)                                              ;
         /*    VACC        |            VMUL            |    S/VLD     | */
         /* HOT ZONE  END  */
         // Saturation TODO: use vsat for 32 bit vector once supported
         vec32s lacc = vsra(vec32s(lh, ll), vNormSh);
         vec32s racc = vsra(vec32s(rh, rl), vNormSh);
#if defined(__chess__) || defined (APEX2_EMULATE)
          ll = lacc.low();
          rl = racc.low();
#endif
#if defined(__llvm__)
         ll = (vec16u)__builtin_apex_vec32_get_lo(lacc);
         rl = (vec16u)__builtin_apex_vec32_get_lo(racc);
#endif

         const int usatByte = chess_copy(0xFF);
         ll = (vec16u)vsat((vec16s)ll, 0, usatByte);
         rl = (vec16u)vsat((vec16s)rl, 0, usatByte);

         *dst_++ = (vec08u)ll;
         *dst_++ = (vec08u)rl;

         rh = (lh  &= 0);
         rl = (ll >>= 15);
      }

      src  += sstr;
      dst += dstr;
   }
}

#else   
    
void apu_flt_generic_3x3(vec08u* restrict dst, 
                         int dstr, 
                         const vec08u* restrict src,
                         int sstr, 
                         int bw, 
                         int bh, 
                         const int8_t* restrict flt, 
                         int fscale)
{
    int x, y;
    const int8_t* restrict fltCoef = (const int8_t* restrict)(flt);

    const int8_t chess_storage(chess_register) c00 = *fltCoef++;
    const int8_t chess_storage(chess_register) c01 = *fltCoef++;
    const int8_t chess_storage(chess_register) c02 = *fltCoef++;

    const int8_t chess_storage(chess_register) c10 = *fltCoef++;
    const int8_t chess_storage(chess_register) c11 = *fltCoef++;
    const int8_t chess_storage(chess_register) c12 = *fltCoef++;

    const int8_t chess_storage(chess_register) c20 = *fltCoef++;
    const int8_t chess_storage(chess_register) c21 = *fltCoef++;
    const int8_t chess_storage(chess_register) c22 = *fltCoef++;    
    vec16s acc0H = 0;
    vec16u acc0L = 0;
    vec16s acc1H = 0;
    vec16u acc1L = 0;   
    vec32s acc0, acc1;
    vec08u out0, out1;  
    const int32_t scale = chess_copy(fscale);
    const uint32_t maxByteU = chess_copy(0xFF);
    for (y = 0; y < bh; y++) chess_loop_range(1, )
    {
        vec08u* restrict dest = (vec08u* restrict)(dst);
        vec16u* restrict line0 = (vec16u* restrict)(src - sstr - 2);
        vec16u* restrict line1 = (vec16u* restrict)(src - 2);
        vec16u* restrict line2 = (vec16u* restrict)(src + sstr - 2);    
        vec16u  newDataL0;
        vec16u  newDataL1;
        vec16u  newDataL2;  
    
        for (x = 0; x < bw / 2; x++) chess_loop_range(1, )
        {
            // Clear the accumulators
            acc0H = acc0H * 0x00;
            acc0L = acc0L & 0x00;
            acc1H = acc1H * 0x00;
            acc1L = acc1L & 0x00;   
            
            // Read data
            newDataL0 = *line0++;
            newDataL1 = *line1++;
            newDataL2 = *line2++;   
            VMAC_HULS(acc0H, acc0L, newDataL0, c00);
            VMAC_HULS(acc0H, acc0L, newDataL1, c10);
            VMAC_HULS(acc0H, acc0L, newDataL2, c20);   
            
            newDataL0 = *line0++;
            newDataL1 = *line1++;
            newDataL2 = *line2++;
            VMAC_LULS(acc0H, acc0L, newDataL0, c01);
            VMAC_HULS(acc0H, acc0L, newDataL0, c02);
            VMAC_HULS(acc1H, acc1L, newDataL0, c00);
            VMAC_LULS(acc1H, acc1L, newDataL0, c01);    
            VMAC_LULS(acc0H, acc0L, newDataL1, c11);
            VMAC_HULS(acc0H, acc0L, newDataL1, c12);
            VMAC_LULS(acc1H, acc1L, newDataL1, c10);
            VMAC_HULS(acc1H, acc1L, newDataL1, c11);    
            VMAC_LULS(acc0H, acc0L, newDataL2, c21);
            VMAC_HULS(acc0H, acc0L, newDataL2, c22);
            VMAC_LULS(acc1H, acc1L, newDataL2, c20);
            VMAC_HULS(acc1H, acc1L, newDataL2, c21);  
            
            newDataL0 = *line0++;
            newDataL1 = *line1++;
            newDataL2 = *line2++;
            VMAC_LULS(acc1H, acc1L, newDataL0, c02);
            VMAC_LULS(acc1H, acc1L, newDataL1, c12);
            VMAC_LULS(acc1H, acc1L, newDataL2, c22);  
            
            acc0 = vec32s(acc0H, acc0L) >> scale;
            out0 = (vec08u)vsat(acc0.low(), 0, maxByteU);
            *dest++ = out0; 
            acc1 = vec32s(acc1H, acc1L) >> scale;
            out1 = (vec08u)vsat(acc1.low(), 0, maxByteU);
            *dest++ = out1;
            
            // Go back two steps
            line0 -= 2;
            line1 -= 2;
            line2 -= 2;
        }
        src += sstr;
        dst += dstr;
    }
}

#endif

#if 1

/*!
   Calculate the output of generic (5 x 5) filter.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
   \param flt  - [Input]  Filter Coeffiencients (pointer)
 */
void apu_flt_generic_5x5(
         vec08u* restrict dst, int dstr,
   const vec08u* restrict src, int sstr,
   int bw, int bh,
   const int8_t* restrict flt, int fscale){
   /* Adjust pointers for our needs */
   src -= 2*sstr;
   src -= 2;

   /* Compute gain of filter */
   int normSh = fscale;
   vec32s vNormSh = normSh;

   /* Load filter coefficients in registers */
   const int32_t* f = (const int32_t*)flt;
   int32_t lf0  = *f++;
   int32_t lf1  = *f++;
   int32_t lf2  = *f++;
   int32_t lf3  = *f++;
   int32_t lf4  = *f++;
   int32_t lf5  = *f++;
   int32_t lf6  = *f++;

   /* For each row... */
   for (int by=0; by<bh; ++by) chess_loop_range(1,){
      /* Compute row pointers */
      vec08u* restrict chess_storage(R5) dst_ = chess_copy(dst);
      const vec16u* va = (const vec16u*)(src);
      const vec16u* vb = (const vec16u*)((const vec08u*)va + sstr);
      const vec16u* vc = (const vec16u*)((const vec08u*)vb + sstr);
      const vec16u* vd = (const vec16u*)((const vec08u*)vc + sstr);
      const vec16u* ve = (const vec16u*)((const vec08u*)vd + sstr);
      vec16s chess_storage(V0) lh = 0;
      vec16s chess_storage(V2) rh = 0;
      vec16u chess_storage(V1) ll = 0;
      vec16u chess_storage(V3) rl = 0;

      /* For each column... */
      for (int bx=0; bx<bw; bx+=2) chess_loop_range(1,){
         /* Temporaries */
         vec16s chess_storage(V7) lm;
         vec16s chess_storage(V7) rm;
         vec16u  v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14;

         /**
          * Pessimization: Force coefficients to be unpacked every iteration,
          * avoiding the compiler's preference for spilling to memory
          * (and consuming power).
          */

         lf0         = chess_copy(lf0);/* Defeat LICM */
         lf1         = chess_copy(lf1);/* Defeat LICM */
         lf2         = chess_copy(lf2);/* Defeat LICM */
         lf3         = chess_copy(lf3);/* Defeat LICM */
         lf4         = chess_copy(lf4);/* Defeat LICM */
         lf5         = chess_copy(lf5);/* Defeat LICM */
         lf6         = chess_copy(lf6);/* Defeat LICM */
         int32_t f0  = lf0 >>  0;
         int32_t f1  = lf0 >> 16;
         int32_t f2  = lf1 >>  0;
         int32_t f3  = lf1 >> 16;
         int32_t f4  = lf2 >>  0;
         int32_t f5  = lf2 >> 16;
         int32_t f6  = lf3 >>  0;
         int32_t f7  = lf3 >> 16;
         int32_t f8  = lf4 >>  0;
         int32_t f9  = lf4 >> 16;
         int32_t f10 = lf5 >>  0;
         int32_t f11 = lf5 >> 16;
         int32_t f12 = lf6 >>  0;
         /* HOT ZONE BEGIN */
         /*      VACC      |            VMUL            |    S/VLD     | */
                                                          v0   = *va++ ; /* Start Row 0 */
                                                                       ;
                                                                       ;
                             lm = (vec16s)vmul_ulsl(v0,  f0)    , v1   = *va++ ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_uhsl(v0,  f0)                   ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_uhsh(v0,  f0)                   ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_ulsh(v1,  f0)                   ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_ulsl(v1,  f1)    , v2   = *va-- ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_uhsl(v1,  f1)                   ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_uhsh(v1,  f1)                   ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_ulsh(v2,  f1)    , v3   = *vb++ ; /* Start Row 1 */
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_ulsl(v2,  f2)                   ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_uhsl(v2,  f2)                   ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_ulsh(v3,  f2)    , v4   = *vb++ ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_uhsh(v3,  f2)                   ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_uhsl(v3,  f3)                   ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_ulsl(v4,  f3)                   ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_ulsh(v4,  f3)    , v5   = *vb-- ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_uhsh(v4,  f3)                   ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_uhsl(v4,  f4)                   ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_ulsl(v5,  f4)    , v6   = *vc++ ; /* Start Row 2 */
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_ulsh(v5,  f4)                   ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_uhsh(v5,  f4)                   ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_ulsl(v6,  f5)    , v7   = *vc++ ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_uhsl(v6,  f5)                   ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_uhsh(v6,  f5)                   ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_ulsh(v7,  f5)                   ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_ulsl(v7,  f6)    , v8   = *vc-- ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_uhsl(v7,  f6)                   ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_uhsh(v7,  f6)                   ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_ulsh(v8,  f6)    , v9   = *vd++ ; /* Start Row 3 */
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_ulsl(v8,  f7)                   ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_uhsl(v8,  f7)                   ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_ulsh(v9,  f7)    , v10  = *vd++ ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_uhsh(v9,  f7)                   ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_uhsl(v9,  f8)                   ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_ulsl(v10, f8)                   ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_ulsh(v10, f8)    , v11  = *vd-- ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_uhsh(v10, f8)                   ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_uhsl(v10, f9)                   ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_ulsl(v11, f9)    , v12  = *ve++ ; /* Start Row 4 */
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_ulsh(v11, f9)                   ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_uhsh(v11, f9)                   ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_ulsl(v12, f10)   , v13  = *ve++ ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_uhsl(v12, f10)                  ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_uhsh(v12, f10)                  ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_ulsh(v13, f10)                  ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_ulsl(v13, f11)   , v14  = *ve-- ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_uhsl(v13, f11)                  ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_uhsh(v13, f11)                  ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_ulsh(v14, f11)                  ;
         vacl(rh, rl, rm)  , lm = (vec16s)vmul_ulsl(v14, f12)                  ;
         vacl(lh, ll, lm)  , rm = (vec16s)vmul_uhsl(v14, f12)                  ;
         vacl(rh, rl, rm)                                              ;
         /*    VACC        |            VMUL            |    S/VLD     | */
         /* HOT ZONE  END  */
         // Saturation TODO: use vsat for 32 bit vector once supported
         vec32s lacc = vsra(vec32s(lh, ll), vNormSh);
         vec32s racc = vsra(vec32s(rh, rl), vNormSh);
#if defined(__chess__) || defined (APEX2_EMULATE)      
         ll = lacc.low();
         rl = racc.low();
#endif
#if defined(__llvm__)
         ll = (vec16u)__builtin_apex_vec32_get_lo(lacc);
         rl = (vec16u)__builtin_apex_vec32_get_lo(racc);
#endif
         

         /*lacc = vselect(lacc, 0, lacc >= 0);
         lacc = vselect(lacc, 255, lacc <= 255);
         racc = vselect(racc, 0, racc >= 0);
         racc = vselect(racc, 255, racc <= 255);
         */

         const int usatByte = chess_copy(0xFF);
         ll = (vec16u)vsat((vec16s)ll, 0, usatByte);
         rl = (vec16u)vsat((vec16s)rl, 0, usatByte);

         *dst_++ = (vec08u)ll;
         *dst_++ = (vec08u)rl;

         rh = (lh  &= 0);
         rl = (ll >>= 15);
      }

      src  += sstr;
      dst += dstr;
   }
}

#else
    
void apu_flt_generic_5x5(vec08u* restrict dst,
                         int dstr,
                         const vec08u* restrict src,
                         int sstr, 
                         int width,
                         int heigth,
                         const int8_t* restrict flt,
                         int fscale)

{
    int x, y;   
    // Cast the flt pointer into an unsigned one
    const uint8_t* restrict fltCoef = (const uint8_t* restrict)(flt);
    // The fitler coefficients are read
    const uint16_t chess_storage(chess_register) c00_01 = (uint16_t)fltCoef[0] << 8 | (uint16_t)fltCoef[1];
    const uint16_t chess_storage(chess_register) c02_03 = (uint16_t)fltCoef[2] << 8 | (uint16_t)fltCoef[3];
    const uint16_t chess_storage(chess_register) c04 = (uint16_t)fltCoef[4];
    const uint16_t chess_storage(chess_register) c10_11 = (uint16_t)fltCoef[5] << 8 | (uint16_t)fltCoef[6];
    const uint16_t chess_storage(chess_register) c12_13 = (uint16_t)fltCoef[7] << 8 | (uint16_t)fltCoef[8];
    const uint16_t chess_storage(chess_register) c14 = (uint16_t)fltCoef[9];
    const uint16_t chess_storage(chess_register) c20_21 = (uint16_t)fltCoef[10] << 8 | (uint16_t)fltCoef[11];
    const uint16_t chess_storage(chess_register) c22_23 = (uint16_t)fltCoef[12] << 8 | (uint16_t)fltCoef[13];
    const uint16_t chess_storage(chess_register) c24 = (uint16_t)fltCoef[14];
    const uint16_t chess_storage(chess_register) c30_31 = (uint16_t)fltCoef[15] << 8 | (uint16_t)fltCoef[16];
    const uint16_t chess_storage(chess_register) c32_33 = (uint16_t)fltCoef[17] << 8 | (uint16_t)fltCoef[18];
    const uint16_t chess_storage(chess_register) c34 = (uint16_t)fltCoef[19];
    const uint16_t chess_storage(chess_register) c40_41 = (uint16_t)fltCoef[20] << 8 | (uint16_t)fltCoef[21];
    const uint16_t chess_storage(chess_register) c42_43 = (uint16_t)fltCoef[22] << 8 | (uint16_t)fltCoef[23];
    const uint16_t chess_storage(chess_register) c44 = (uint16_t)fltCoef[24];
    const int32_t scale                              = chess_copy(fscale);
    const uint32_t maxByteU                          = chess_copy(0xFF);
    
    vec16s accOut0H = 0;
    vec16u accOut0L = 0;
    vec16s accOut1H = 0;
    vec16u accOut1L = 0;
    vec32s acc;
    vec08u out;

    for (y = 0; y < heigth; y++) chess_loop_range(1, )
    {
        vec16u  newDataL0;
        vec16u  newDataL1;
        vec16u  newDataL2;
        vec16u  newDataL3;
        vec16u  newDataL4;        
        
        // The calculus is made for bytes
        vec16u* restrict line0 = (vec16u* restrict)(src - 2 * sstr - 2);
        vec16u* restrict line1 = (vec16u* restrict)(src - sstr - 2);
        vec16u* restrict line2 = (vec16u* restrict)(src - 2);
        vec16u* restrict line3 = (vec16u* restrict)(src + sstr - 2);
        vec16u* restrict line4 = (vec16u* restrict)(src + 2 * sstr - 2);
        vec08u* restrict dest  = (vec08u* restrict)(dst);
        
        for (x = 0; x < width; x += 2) chess_loop_range(1, )
        {
            // Read the first line
            newDataL0 = *line0++;
            
            // Clear the accumulators 
            accOut0H = accOut0H & 0x00;
            accOut0L = accOut0L * 0x00;
            accOut1H = accOut1H & 0x00;
            accOut1L = accOut1L * 0x00;

            newDataL1 = *line1++;
            VMAC_LUHS(accOut0H, accOut0L, newDataL0, c00_01);
            VMAC_HULS(accOut0H, accOut0L, newDataL0, c00_01);
            VMAC_HUHS(accOut1H, accOut1L, newDataL0, c00_01);

            newDataL2 = *line2++;
            VMAC_LUHS(accOut0H, accOut0L, newDataL1, c10_11);
            VMAC_HULS(accOut0H, accOut0L, newDataL1, c10_11);
            VMAC_HUHS(accOut1H, accOut1L, newDataL1, c10_11);

            newDataL3 = *line3++;
            VMAC_LUHS(accOut0H, accOut0L, newDataL2, c20_21);
            VMAC_HULS(accOut0H, accOut0L, newDataL2, c20_21);
            VMAC_HUHS(accOut1H, accOut1L, newDataL2, c20_21);

            newDataL4 = *line4++;
            VMAC_LUHS(accOut0H, accOut0L, newDataL3, c30_31);
            VMAC_HULS(accOut0H, accOut0L, newDataL3, c30_31);
            VMAC_HUHS(accOut1H, accOut1L, newDataL3, c30_31);

            newDataL0 = *line0++;
            VMAC_LUHS(accOut0H, accOut0L, newDataL4, c40_41);
            VMAC_HULS(accOut0H, accOut0L, newDataL4, c40_41);
            VMAC_HUHS(accOut1H, accOut1L, newDataL4, c40_41);
            
            newDataL1 = *line1++;
            VMAC_LUHS(accOut0H, accOut0L, newDataL0, c02_03);
            VMAC_LULS(accOut1H, accOut1L, newDataL0, c00_01);
            VMAC_HULS(accOut0H, accOut0L, newDataL0, c02_03);
            VMAC_HUHS(accOut1H, accOut1L, newDataL0, c02_03);

            newDataL2 = *line2++;
            VMAC_LUHS(accOut0H, accOut0L, newDataL1, c12_13);
            VMAC_LULS(accOut1H, accOut1L, newDataL1, c10_11);
            VMAC_HULS(accOut0H, accOut0L, newDataL1, c12_13);
            VMAC_HUHS(accOut1H, accOut1L, newDataL1, c12_13);

            newDataL3 = *line3++;
            VMAC_LUHS(accOut0H, accOut0L, newDataL2, c22_23);
            VMAC_LULS(accOut1H, accOut1L, newDataL2, c20_21);
            VMAC_HULS(accOut0H, accOut0L, newDataL2, c22_23);
            VMAC_HUHS(accOut1H, accOut1L, newDataL2, c22_23);

            newDataL4 = *line4++;
            VMAC_LUHS(accOut0H, accOut0L, newDataL3, c32_33);
            VMAC_LULS(accOut1H, accOut1L, newDataL3, c30_31);
            VMAC_HULS(accOut0H, accOut0L, newDataL3, c32_33);
            VMAC_HUHS(accOut1H, accOut1L, newDataL3, c32_33);

            newDataL0 = *line0++;
            VMAC_LUHS(accOut0H, accOut0L, newDataL4, c42_43);
            VMAC_LULS(accOut1H, accOut1L, newDataL4, c40_41);
            VMAC_HULS(accOut0H, accOut0L, newDataL4, c42_43);
            VMAC_HUHS(accOut1H, accOut1L, newDataL4, c42_43);
           
            newDataL1 = *line1++;
            VMAC_LULS(accOut0H, accOut0L, newDataL0, c04);
            VMAC_LULS(accOut1H, accOut1L, newDataL0, c02_03);
            VMAC_HULS(accOut1H, accOut1L, newDataL0, c04);

            newDataL2 = *line2++;
            VMAC_LULS(accOut0H, accOut0L, newDataL1, c14);
            VMAC_LULS(accOut1H, accOut1L, newDataL1, c12_13);
            VMAC_HULS(accOut1H, accOut1L, newDataL1, c14);

            newDataL3 = *line3++;
            VMAC_LULS(accOut0H, accOut0L, newDataL2, c24);
            VMAC_LULS(accOut1H, accOut1L, newDataL2, c22_23);
            VMAC_HULS(accOut1H, accOut1L, newDataL2, c24);

            newDataL4 = *line4++;
            VMAC_LULS(accOut0H, accOut0L, newDataL3, c34);
            VMAC_LULS(accOut1H, accOut1L, newDataL3, c32_33);
            VMAC_HULS(accOut1H, accOut1L, newDataL3, c34);

            VMAC_LULS(accOut0H, accOut0L, newDataL4, c44);
            VMAC_LULS(accOut1H, accOut1L, newDataL4, c42_43);
            VMAC_HULS(accOut1H, accOut1L, newDataL4, c44);

            acc = vec32s(accOut0H, accOut0L) >> scale;
            out = (vec08u)vsat(acc.low(), 0, maxByteU);
            *dest++ = out;

            acc = vec32s(accOut1H, accOut1L) >> scale;
            out = (vec08u)vsat(acc.low(), 0, maxByteU);
            *dest++ = out;
            
            line0 -= 2;
            line1 -= 2;
            line2 -= 2;
            line3 -= 2;
            line4 -= 2;         
        }
        src += sstr;
        dst += dstr;
      }
}

#endif

}
#endif /* ACF_KERNEL_IMPLEMENTATION */

/*!*********************************************************************************
*  \file apu_filter.c
*  \brief APU filter kernels.
***********************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "image_filters_apu.h"

#define sort2El(a, b) vswap(a, b, a > b);
#define min3(a, b, c) sort2El(a, b); sort2El(a, c);
#define max3(a, b, c) sort2El(a, c); sort2El(b, c);
#define min4(a, b, c, d) sort2El(a, b); sort2El(c, d); sort2El(a, c);
#define max4(a, b, c, d) sort2El(a, b); sort2El(c, d); sort2El(b, d);
#define min5(a, b, c, d, e) sort2El(a, b); sort2El(c, d); min3(a, c, e);
#define max5(a, b, c, d, e) sort2El(a, b); sort2El(c, d); max3(b, d, e);
#define min6(a, b, c, d, e, f) sort2El(a, b); sort2El(c, d); sort2El(e, f); min3(a, c, e);
#define max6(a, b, c, d, e, f) sort2El(a, b); sort2El(c, d); sort2El(e, f); max3(b, d, f);
#define minmax3(a, b, c) min3(a, b, c); sort2El(b, c);
#define minmax4(a, b, c, d) sort2El(a, b); sort2El(c, d); sort2El(a, c); sort2El(b, d);
#define minmax5(a, b, c, d, e) sort2El(a, b); sort2El(c, d); min3(a, c, e); max3(b, d, e);

#define minmax6(a, b, c, d, e, f) \
sort2El(a, b); sort2El(c, d); sort2El(e, f); min3(a, c, e); max3(b, d, f);

#define minmax7(a, b, c, d, e, f, g) \
sort2El(a, b); sort2El(c, d); sort2El(e, f); min4(a, c, e, g); max4(b, d, f, g);

#define minmax8(a, b, c, d, e, f, g, h) \
sort2El(a, b); sort2El(c, d); sort2El(e, f); sort2El(g, h); min4(a, c, e, g); max4(b, d, f, h);

#define minmax9(a, b, c, d, e, f, g, h, i) \
sort2El(a, b); sort2El(c, d); sort2El(e, f); sort2El(g, h); min5(a, c, e, g, i); max5(b, d, f, h, i);

#define minmax10(a, b, c, d, e, f, g, h, i, j) \
sort2El(a, b); sort2El(c, d); sort2El(e, f); sort2El(g, h); sort2El(i, j); min5(a, c, e, g, i); \
max5(b, d, f, h, j);

#define minmax11(a, b, c, d, e, f, g, h, i, j, k) \
sort2El(a, b); sort2El(c, d); sort2El(e, f); sort2El(g, h); sort2El(i, j); min6(a, c, e, g, i, k); \
max6(b, d, f, h, j, k);

#define minmax12(a, b, c, d, e, f, g, h, i, j, k, l) \
sort2El(a, b); sort2El(c, d); sort2El(e, f); sort2El(g, h); sort2El(i, j); sort2El(k, l); \
min6(a, c, e, g, i, k); max6(b, d, f, h, j, l);

#define minmax13(a, b, c, d, e, f, g, h, i, j, k, l, m) \
sort2El(a, b); sort2El(c, d); sort2El(e, f); sort2El(g, h); sort2El(i, j); sort2El(k, l); min3(a, c, e); \
min3(g, i, k); min3(a, g, m); max3(b, d, f); max3(h, j, l); max3(f, l, m);

#define minmax14(a, b, c, d, e, f, g, h, i, j, k, l, m, n) \
sort2El(a, b); sort2El(c, d); sort2El(e, f); sort2El(g, h); sort2El(i, j); sort2El(k, l); sort2El(m, n); \
min3(a, c, e); min3(g, i, k); min3(a, g, m); max3(b, d, f); max3(h, j, l); max3(f, l, n);

#define minmax16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) \
sort2El(a, b); sort2El(c, d); sort2El(e, f); sort2El(g, h); sort2El(i, j); sort2El(k, l); sort2El(m, n); sort2El(o, p); \
minmax8(a, c, e, g, i, k, m, o); minmax8(b, d, f, h, j, l, n, p); sort2El(a, b); sort2El(o, p); \

#define preProcCommonEl(a, b, c, d, e, f) \
sort2El(a, d); \
sort2El(b, e); \
sort2El(a, b); sort2El(c, f); \
sort2El(a, c); sort2El(e, f); \
sort2El(b, c); sort2El(d, f); \
sort2El(d, e); \
sort2El(b, d); \

/*!
Calculate a 3x3 median filter.

\param dst  - [Output] Destination block pointer
\param dstr - [Input]  Destination block stride
\param src  - [Input]  Source block pointer
\param sstr - [Input]  Source block stride
\param bw   - [Input]  Block width
\param bh   - [Input]  Block height
*/

void apu_flt_median_3x3(vec08u* dst, int dstr, const vec08u* src, int sstr, int bw, int bh)
{
    vec08u* restrict dstRst = (vec08u* restrict)dst;
    vec08u* restrict line0 = (vec08u* restrict)(src - sstr);
    vec08u* restrict line1 = (vec08u* restrict)(src);
    vec08u* restrict line2 = (vec08u* restrict)(src + sstr);

    vec16s a1, a2, a3, a4, a5, a6, newElement;
    vec16s tmp1, tmp2, tmp3, tmp4, tmp5;
    
    /*
    Each connection inside the graph si actually a 2 elements sort
    sort2El(a, b) if(a>b) {aux=b; b=a; a=b;};    
    
    Sorting network common part:
    a1 -*-**----
    a2 -|**|*-*-
    a3 -||***-|-
    a4 -*||****-
    a5 --*|*|*--
    a6 ---*-*---    
    a7 ---------
    a8 ---------
    a9 ---------
    
    Full sorting network:
    a1 -*-**---   ---------     
    a2 -|**|*-*   *--------
    a3 -||***-|   |-***----
    a4 -*||****   |-|*|*-**
    a5 --*|*|*-   |*|**|*|*
    a6 ---*-*--   ||||-|||-
    a7 --------   ***|-|||-
    a8 --------   ---*-*||-
    a9 --------   ------**-
    ^           ^ ^
  |Common part| | Per sample part

    This is the box image with multisample of 2
    ---------
    |a|b|c|d|
    ---------
    |e|f|g|h|
    ---------
    |i|j|k|l|
    ---------    
    The b, c, f, g, j, k are common elements between two consecutive samples. 
    Before multi sample:
    output0 = median(a, b, c, e, f, g, i, j, k);
    output1 = median(b, c, d, f, g, h, j, k, l);
    After data reuse:
    commondata = preprocessCommonData(b, c, f, g, j, k);
    load:a, e, i
    output0 = processRestOfSortingNetwork(commondata, a, e, i);
    load: d, h, l
    output1 = processRestOfSortingNetwork(commondata, d, h, l);    
    */
    
    for (int y=0; y<bh; y++)
    {
        for (int x=0; x<bw; x+=2)
        {
            a1 = (vec16s)line0[x];
            a2 = (vec16s)line0[x + 1];
            a3 = (vec16s)line1[x];
            a4 = (vec16s)line1[x + 1];
            a5 = (vec16s)line2[x];
            a6 = (vec16s)line2[x + 1];
            
            // Common processing part between two consecutive sampels
            preProcCommonEl(a1, a2, a3, a4, a5, a6);
            tmp1 = a2; tmp2 = a3; tmp3 = a4; tmp4 = a5; tmp5 = a6;            
            
            // Finishing the sorting network for the first output
            newElement = (vec16s)line0[x - 1];
            sort2El(a2, newElement);
            sort2El(a5, newElement);
            sort2El(a3, newElement);

            newElement = (vec16s)line1[x - 1];
            sort2El(a5, newElement);
            sort2El(a3, a4);
            sort2El(a3, a5);
            sort2El(a4, newElement);

            newElement = (vec16s)line2[x - 1];
            sort2El(a5, newElement);
            sort2El(a4, newElement);
            sort2El(a4, a5);
            dstRst[x] = (vec08u)a5;

            // Finishing the sorting network for the second output
            newElement = (vec16s)line0[x + 2];
            sort2El(tmp1, newElement);
            sort2El(tmp4, newElement);
            sort2El(tmp2, newElement);

            newElement = (vec16s)line1[x + 2];
            sort2El(tmp4, newElement);
            sort2El(tmp2, tmp3);
            sort2El(tmp2, tmp4);
            sort2El(tmp3, newElement);

            newElement = (vec16s)line2[x + 2];
            sort2El(tmp4, newElement);
            sort2El(tmp3, newElement);
            sort2El(tmp3, tmp4);
            dstRst[x + 1] = (vec08u)tmp4;
        }

        line0 = line1;
        line1 = line2;
        line2 += sstr;
        dstRst += dstr;
    }
}

void apu_flt_median_5x5(vec08u* dst, int dstr, const vec08u* src, int sstr, int bw, int bh)
{
    
    // Output pointer
    vec08u * restrict ptrResDst = (vec08u * restrict)(dst);

    // Input pointers
    vec08u * restrict line0 = (vec08u * restrict)(src - 2 * sstr);
    vec08u * restrict line1 = (vec08u * restrict)(src - sstr);
    vec08u * restrict line2 = (vec08u * restrict)(src);
    vec08u * restrict line3 = (vec08u * restrict)(src + sstr);
    vec08u * restrict line4 = (vec08u * restrict)(src + 2 * sstr);
    vec08u * restrict line5 = (vec08u * restrict)(src + 3 * sstr);
    /*
    -------  ----------------------------  --------
    |line00  |line01|line02|line03|line04  |line05|
    -------  ----------------------------  --------
    
    |line10  |line11|line12|line13|line14  |line15|
    -------  ----------------------------  --------
    |line20  |line21|line22|line23|line24  |line25|
    -------  ----------------------------  --------
    |line30  |line31|line32|line33|line34  |line35|
    -------  ----------------------------  --------
    |line40  |line41|line42|line43|line44  |line45|
    
    -------  ----------------------------  --------    
    |line50  |line51|line52|line53|line54  |line55|
    -------  ----------------------------  --------
    
    To simplify de explanation of the algorithm we define:
    rectangle(upperLeft, upperRight, lowerLeft, lowerRigth) 
    as a set of elements that are defined by the coordinates: upperLeft, upperRight, lowerLeft, lowerRigth
        
    rectangle(01, 03, 11, 13) -> this abstraction will contain the following elements:
    |line01|line02|line03|
    |line11|line12|line13|    
    
    The center portion of the upper grid is common to: out[x], out[x+1], out[x+stride], out[x+stride+1]    
    Common portion: 
    rectangle(11, 14, 41, 44) <=> line11->line14, line21->line24, line31->line34, line41->line44    
    
    To get the value of out[x] you need to do the following:
    out[x]          = median(rectangle(00, 04, 40, 44));
    out[x+1]        = median(rectangle(01, 05, 41, 45));
    out[x+stride]   = median(rectangle(10, 14, 50, 54));
    out[x+stride+1] = median(rectangle(11, 15, 51, 55));  
        
    Algo: 
    The median is based on a min-max forgetfull sorting algorithm. The staring base for the algo is to load 25/2+1 elements and then
    perform min-max over all the 13 elements, remove the min, replace the max with a new element and do the same until you end up with
    just 3 elements. When you do minMax3(a,b,c), b will be the median.
        
    Steps:    
    First and foremost, read the common part:    
    Common for x and y
    load: rectangle(11, 14, 41, 44), this is common to all 4 output samples:
    commonXY = preprocess rectangle(11, 14, 41, 44)
    
    Common for x:
    load: line01->line04
    commonXXY = aggregate (line01->line04) to commonXY data and preprocess
    
    out[x]:
    load: column line00->line40
    out[x] = median(commonXXY, line00->line40);
    
    out[x+1]:
    load: column line05->line45
    out[x+1] = median(commonXXY, line05->line45);
    
    Common for y:
    load: line51->line54:
    commonXYY = aggregate (line51->line54) to commonXY data and preprocess    
    
    out[x+stride]:
    load: line10->line50
    out[x+stride] = median(commonXYY, line10->line50);
    
    out[x+stride+1]:
    load: line15->line55
    out[x+stride+1] = median(commonXYY, line15->line55);
    */
    
    vec16s cmnXY0, cmnXY1, cmnXY2, cmnXY3, cmnXY4, cmnXY5, cmnXY6, cmnXY7, cmnXY8, cmnXY9, cmnXY10;
    vec16s temp0, temp1, temp2, temp3, temp4, temp5, temp6;

    vec16s line11, line12, line13, line14;
    vec16s line21, line22, line23, line24;
    vec16s line31, line32, line33, line34;
    vec16s line41, line42, line43, line44;
    vec16s newElement;

    // The technique used to speed up the kernel is a multisample loop body of 2 in x and y dimensions
    for (int y=0; y<bh; y+=2) chess_loop_range(1, )
    {
        for (int x=0; x<bw; x+=2) chess_loop_range(1, )
        {
            // Common processing
            line11 = (vec16s)line1[x - 1];
            line12 = (vec16s)line1[x];
            line13 = (vec16s)line1[x + 1];
            line14 = (vec16s)line1[x + 2];

            line21 = (vec16s)line2[x - 1];
            line22 = (vec16s)line2[x];
            line23 = (vec16s)line2[x + 1];
            line24 = (vec16s)line2[x + 2];

            line31 = (vec16s)line3[x - 1];
            line32 = (vec16s)line3[x];
            line33 = (vec16s)line3[x + 1];
            line34 = (vec16s)line3[x + 2];

            line41 = (vec16s)line4[x - 1];
            line42 = (vec16s)line4[x];
            minmax14(line11, line12, line13, line14, line21, line22, line23, line24, line31, line32, line33, line34, line41, line42);

            newElement = (vec16s)line4[x + 1];
            minmax13(line12, line13, line14, line21, line22, line23, line24, line31, line32, line33, line34, line41, newElement);

            newElement = (vec16s)line4[x + 2];
            minmax12(line13, line14, line21, line22, line23, line24, line31, line32, line33, line34, line41, newElement);

            // Buffer the common data
            cmnXY0 = line14; cmnXY1 = line21; cmnXY2 = line22; cmnXY3 = line23; cmnXY4 = line24; cmnXY5 = line31;
            cmnXY6 = line32; cmnXY7 = line33; cmnXY8 = line34; cmnXY9 = line41; cmnXY10 = newElement;

            // Multi sample on X    
            newElement = (vec16s)line0[x - 1];
            minmax11(line14, line21, line22, line23, line24, line31, line32, line33, line34, line41, newElement);

            newElement = (vec16s)line0[x];
            minmax10(line21, line22, line23, line24, line31, line32, line33, line34, line41, newElement);

            newElement = (vec16s)line0[x + 1];
            minmax9(line22, line23, line24, line31, line32, line33, line34, line41, newElement);

            newElement = (vec16s)line0[x + 2];
            minmax8(line23, line24, line31, line32, line33, line34, line41, newElement);

            // Buffer for x dimension
            temp0 = line24;
            temp1 = line31;
            temp2 = line32;
            temp3 = line33;
            temp4 = line34;
            temp5 = line41;
            
            newElement = (vec16s)line0[x - 2];
            minmax7(line24, line31, line32, line33, line34, line41, newElement);
            newElement = (vec16s)line1[x - 2];
            minmax6(line31, line32, line33, line34, line41, newElement);
            newElement = (vec16s)line2[x - 2];
            minmax5(line32, line33, line34, line41, newElement);
            newElement = (vec16s)line3[x - 2];
            minmax4(line33, line34, line41, newElement);
            newElement = (vec16s)line4[x - 2];
            minmax3(line34, line41, newElement);
            ptrResDst[x] = (vec08u)line41;

            newElement = (vec16s)line0[x + 3];
            minmax7(temp0, temp1, temp2, temp3, temp4, temp5, newElement);
            newElement = (vec16s)line1[x + 3];
            minmax6(temp1, temp2, temp3, temp4, temp5, newElement);
            newElement = (vec16s)line2[x + 3];
            minmax5(temp2, temp3, temp4, temp5, newElement);
            newElement = (vec16s)line3[x + 3];
            minmax4(temp3, temp4, temp5, newElement);
            newElement = (vec16s)line4[x + 3];
            minmax3(temp4, temp5, newElement);
            ptrResDst[x + 1] = (vec08u)temp5;

            // Multi sample on y           
            newElement = (vec16s)line5[x - 1];
            minmax11(cmnXY0, cmnXY1, cmnXY2, cmnXY3, cmnXY4, cmnXY5, cmnXY6, cmnXY7, cmnXY8, cmnXY9, newElement);
            newElement = (vec16s)line5[x];
            minmax10(cmnXY1, cmnXY2, cmnXY3, cmnXY4, cmnXY5, cmnXY6, cmnXY7, cmnXY8, cmnXY9, newElement);
            newElement = (vec16s)line5[x + 1];
            minmax9(cmnXY2, cmnXY3, cmnXY4, cmnXY5, cmnXY6, cmnXY7, cmnXY8, cmnXY9, newElement);
            newElement = (vec16s)line5[x + 2];
            minmax8(cmnXY3, cmnXY4, cmnXY5, cmnXY6, cmnXY7, cmnXY8, cmnXY9, newElement);

            // Buffer for y dimension
            temp0 = cmnXY4;
            temp1 = cmnXY5;
            temp2 = cmnXY6;
            temp3 = cmnXY7;
            temp4 = cmnXY8;
            temp5 = cmnXY9;

            newElement = (vec16s)line1[x - 2];
            minmax7(cmnXY4, cmnXY5, cmnXY6, cmnXY7, cmnXY8, cmnXY9, newElement);
            newElement = (vec16s)line2[x - 2];
            minmax6(cmnXY5, cmnXY6, cmnXY7, cmnXY8, cmnXY9, newElement);
            newElement = (vec16s)line3[x - 2];
            minmax5(cmnXY6, cmnXY7, cmnXY8, cmnXY9, newElement);
            newElement = (vec16s)line4[x - 2];
            minmax4(cmnXY7, cmnXY8, cmnXY9, newElement);
            newElement = (vec16s)line5[x - 2];
            minmax3(cmnXY8, cmnXY9, newElement);
            ptrResDst[x + dstr] = (vec08u)cmnXY9;

            newElement = (vec16s)line1[x + 3];
            minmax7(temp0, temp1, temp2, temp3, temp4, temp5, newElement);
            newElement = (vec16s)line2[x + 3];
            minmax6(temp1, temp2, temp3, temp4, temp5, newElement);
            newElement = (vec16s)line3[x + 3];
            minmax5(temp2, temp3, temp4, temp5, newElement);
            newElement = (vec16s)line4[x + 3];
            minmax4(temp3, temp4, temp5, newElement);
            newElement = (vec16s)line5[x + 3];
            minmax3(temp4, temp5, newElement);
            ptrResDst[x + 1 + dstr] = (vec08u)temp5;
        }

        line0 = line2;
        line1 = line3;
        line2 = line4;
        line3 = line5;
        line4 = line5 + sstr;
        line5 = (line5 + (sstr << 1));
        ptrResDst += (dstr << 1);
    }
}
#endif /* ACF_KERNEL_IMPLEMENTATION */

/*!*********************************************************************************
*  \file apu_filter.c
*  \brief APU filter kernels.
***********************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "image_filters_apu.h"
   
/*!
   Calculate a 3x3 dilate filter.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void
apu_flt_dilate_3x3(
            vec08u* dst, int dstr,
      const vec08u* src, int sstr,
            int bw, int bh
)
{
   // Structuring element: Rectangular - hardcoded
      //  1,  1,  1,
      //  1,  1,  1,
      //  1,  1,  1,

   
   // Loop
   // vec16s a_max, a0, a1, a2, a3, a4, a5, a6, a7, a8;
   // vec16s b_max, b6, b7, b8;
   
   for (int y = 0; y < bh; ++y) chess_loop_range(1,) 
   {
      // Neighbors:
      const vec08u* ps0 = (src-1) + (y-1)*sstr;
      const vec08u* ps1 = (src-1) + (y  )*sstr;
      const vec08u* ps2 = (src-1) + (y+1)*sstr;
      
      vec16s chess_storage(V0) s0 = (vec16s)*ps0++;
      vec16s chess_storage(V1) s1 = (vec16s)*ps1++;
      vec16s chess_storage(V2) s2 = (vec16s)*ps2++;

      vec16s chess_storage(V4) amax = s0;    s0 = (vec16s)*ps0++;
               vswap(s1, amax, s1 > amax);   s1 = (vec16s)*ps1++;
               vswap(s2, amax, s2 > amax);   s2 = (vec16s)*ps2++;
      vec16s chess_storage(V5) bmax = s0;    s0 = (vec16s)*ps0++;
               vswap(s1, bmax, s1 > bmax);   s1 = (vec16s)*ps1++;
               vswap(s2, bmax, s2 > bmax);   s2 = (vec16s)*ps2++;
      
      for (int x = 0; x < bw; ++x) chess_loop_range(1,) 
      {
         vec16s chess_storage(V6) cmax = s0;    s0 = (vec16s)*ps0++;
                  vswap(s1, cmax, s1 > cmax);   s1 = (vec16s)*ps1++;
                  vswap(s2, cmax, s2 > cmax);   s2 = (vec16s)*ps2++;
               
         // Compare and find max
         vec16s o = amax;
         vec16s b = bmax; vswap(o, b, o < b);
         vec16s c = cmax; vswap(o, c, o < c);
         
         //save max of common pixels
         amax = bmax; bmax = cmax;
         // Assign to output
         dst[x] = (vec08u)o;
      }
      
      // Proceed to next line
      dst += dstr;
   }
}



/*!
   Calculate a 3x3 dilate filter (signed 16 bit).

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void
apu_flt_dilate_3x3(
            vec16s* dst, int dstr,
      const vec16s* src, int sstr,
            int bw, int bh
)
{
   // Structuring element: Rectangular - hardcoded
      //  1,  1,  1,
      //  1,  1,  1,
      //  1,  1,  1,

   // Neighbors:
   const vec16s* s0 = src - sstr;
   const vec16s* s1 = src;
   const vec16s* s2 = src + sstr;
   
   // Loop
   vec16s a_max, a0, a1, a2, a3, a4, a5, a6, a7, a8;
   vec16s b_max, b6, b7, b8;
   
   for (int y = 0; y < bh; ++y) chess_loop_range(1,) 
   {
      for (int x = 0; x < bw; x+=2) chess_loop_range(1,) 
      {
         // Copy src cells
         a0 = s0[x];
         a1 = s0[x + 1];
         a2 = s1[x];
         a3 = s1[x + 1];
         a4 = s2[x];
         a5 = s2[x + 1];
         a6 = s0[x - 1];
         a7 = s1[x - 1];
         a8 = s2[x - 1];
         
         b6 = s0[x + 2];
         b7 = s1[x + 2];
         b8 = s2[x + 2];
            
         // Compare and find max
         a_max = a0;

         vswap(a1, a_max, a1 > a_max);
         vswap(a2, a_max, a2 > a_max);
         vswap(a3, a_max, a3 > a_max);
         vswap(a4, a_max, a4 > a_max);
         vswap(a5, a_max, a5 > a_max);
         
         //save max of common pixels
         b_max = a_max;
         
         vswap(a6, a_max, a6 > a_max);
         vswap(a7, a_max, a7 > a_max);
         vswap(a8, a_max, a8 > a_max);
         
         vswap(b6, b_max, b6 > b_max);
         vswap(b7, b_max, b7 > b_max);
         vswap(b8, b_max, b8 > b_max);
        
         // Assign to output
         dst[x]      = (vec16s)a_max;
         dst[x + 1]  = (vec16s)b_max;
      }
      
      // Proceed to next block
      s0   = s1;
      s1   = s2; 
      s2  += sstr;
      dst += dstr;
   }
}
  
/*!
   Calculate a 3x3 erode filter.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void
apu_flt_erode_3x3(
            vec08u* dst, int dstr,
      const vec08u* src, int sstr,
            int bw, int bh
)
{
   // Structuring element: Rectangular - hardcoded
      //  1,  1,  1,
      //  1,  1,  1,
      //  1,  1,  1,

   
   // Loop
   // vec16s a_max, a0, a1, a2, a3, a4, a5, a6, a7, a8;
   // vec16s b_max, b6, b7, b8;
   
   for (int y = 0; y < bh; ++y) chess_loop_range(1,) 
   {
      // Neighbors:
      const vec08u* ps0 = (src-1) + (y-1)*sstr;
      const vec08u* ps1 = (src-1) + (y  )*sstr;
      const vec08u* ps2 = (src-1) + (y+1)*sstr;
      
      vec16s chess_storage(V0) s0 = (vec16s)*ps0++;
      vec16s chess_storage(V1) s1 = (vec16s)*ps1++;
      vec16s chess_storage(V2) s2 = (vec16s)*ps2++;

      vec16s chess_storage(V4) amin = s0;    s0 = (vec16s)*ps0++;
               vswap(s1, amin, s1 < amin);   s1 = (vec16s)*ps1++;
               vswap(s2, amin, s2 < amin);   s2 = (vec16s)*ps2++;
      vec16s chess_storage(V5) bmin = s0;    s0 = (vec16s)*ps0++;
               vswap(s1, bmin, s1 < bmin);   s1 = (vec16s)*ps1++;
               vswap(s2, bmin, s2 < bmin);   s2 = (vec16s)*ps2++;
      
      for (int x = 0; x < bw; ++x) chess_loop_range(1,) 
      {
         vec16s chess_storage(V6) cmin = s0;    s0 = (vec16s)*ps0++;
                  vswap(s1, cmin, s1 < cmin);   s1 = (vec16s)*ps1++;
                  vswap(s2, cmin, s2 < cmin);   s2 = (vec16s)*ps2++;
               
         // Compare and find min
         vec16s o = amin;
         vec16s b = bmin; vswap(o, b, o > b);
         vec16s c = cmin; vswap(o, c, o > c);
         
         //save max of common pixels
         amin = bmin; bmin = cmin;
         // Assign to output
         dst[x] = (vec08u)o;
      }
      
      // Proceed to next line
      dst += dstr;
   }
}

#endif /* ACF_KERNEL_IMPLEMENTATION */

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "image_filters_apu.h"

/*!
   Applies a 3x3 prewitt filter in the x direction.
   
   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void  
apu_flt_prewitt_3x3_x(
         vec16s* dst, int dstr,        
   const vec08u* src, int sstr,        
              int bw, int bh )                  
{
   const vec08u* s0 = src - sstr;
   const vec08u* s1 = src;
   const vec08u* s2 = src + sstr;
   
   for (int by=0; by<bh; ++by) chess_loop_range(1,) 
   {
      for (int bx=0; bx<bw; ++bx) chess_loop_range(1,) 
      {
         vec16s r;
         r  = (vec16s)s0[bx + 1];
         r -= (vec16s)s0[bx - 1];
         r += (vec16s)s1[bx + 1];
         r -= (vec16s)s1[bx - 1];
         r += (vec16s)s2[bx + 1];
         r -= (vec16s)s2[bx - 1];

         dst[bx] = r;
      }
      
      s0   = s1;
      s1   = s2; 
      s2  += sstr;
      dst += dstr;  
   }
}

/*!
   Applies a 3x3 prewitt filter in the y direction.
   
   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void  
apu_flt_prewitt_3x3_y(
         vec16s* dst, int dstr,        
   const vec08u* src, int sstr,        
              int bw, int bh )                     
{
   const vec08u* s0 = src - sstr;
   const vec08u* s1 = src;
   const vec08u* s2 = src + sstr;
   
   for (int by=0; by<bh; ++by) chess_loop_range(1,) 
   {
      for (int bx=0; bx<bw; ++bx) chess_loop_range(1,) 
      {
         vec16s r;
         r  = (vec16s)s0[bx - 1];
         r += (vec16s)s0[bx];
         r += (vec16s)s0[bx + 1];
         r -= (vec16s)s2[bx - 1];
         r -= (vec16s)s2[bx];
         r -= (vec16s)s2[bx + 1];

         dst[bx] = r;
      }
      
      s0   = s1;
      s1   = s2; 
      s2  += sstr;
      dst += dstr;  
   }
}

#endif /* ACF_KERNEL_IMPLEMENTATION */

/*!*********************************************************************************
*  \file apu_filter.c
*  \brief APU filter kernels.
***********************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "image_filters_apu.h"


namespace ht{/* Namespace ht */

/**
 * Separable 3x3 kernel.
 */

void apu_flt_separable_3x3(vec16s* restrict dst, int dstr, const vec08u* restrict src,
                           int sstr, int bw, int bh, const int8_t* fRow, const int8_t* fCol){
    /* Load coefficients. */
    typedef int16_t coeff_t;
    register coeff_t chess_storage(R15) fCol10 = fCol[1]<<8 | (uint8_t)fCol[0]; fCol += 2;
    register coeff_t chess_storage(R14) fCol2  =              (uint8_t)fCol[0];
    register coeff_t chess_storage(R13) fRow0  = *fRow++;
    register coeff_t chess_storage(R12) fRow1  = *fRow++;
    register coeff_t chess_storage(R11) fRow2  = *fRow++;
    
    /* Block width | 4, >= 4 */
    for (int y=0; y<bh; ++y) chess_loop_range(1,){
        /* Variables */
        vec16s chess_storage(V0) s0, chess_storage(V1) s1, /* Left, Middle */
               chess_storage(V2) s2;                       /* Right column */
        vec08u row0, row1, row2;
        vec08u chess_storage(V6) pix0, chess_storage(V7) pix1;
        vec16s acc;
        
        /* Row Pointers */
        const vec08u* restrict chess_storage(R2) src0 = (const vec08u*)(src-1*sstr-1);
        const vec08u* restrict chess_storage(R3) src1 = (const vec08u*)(src+0*sstr-1);
        const vec08u* restrict chess_storage(R4) src2 = (const vec08u*)(src+1*sstr-1);
        vec16s*       restrict chess_storage(R6) dst0 = dst;
        
        /* Preamble */
        /* Loop-2 */
        row0 = *src0++;
        row1 = *src1++;
        row2 = *src2++;
    s0 = vmul_slul((vec16s)fCol10, (vec16s)(pix0 = row0));
    s0 += vmul_shul((vec16s)fCol10, (vec16s)(pix1 = row1));
    s0 += vmul_slul((vec16s)fCol2,  (vec16s)(pix0 = row2));
        /* Loop-1 */
        row0 = *src0++;
        row1 = *src1++;
        row2 = *src2++;
    s1 = vmul_slul((vec16s)fCol10, (vec16s)(pix0 = row0));
    s1 += vmul_shul((vec16s)fCol10, (vec16s)(pix1 = row1));
    s1 += vmul_slul((vec16s)fCol2, (vec16s)(pix0 = row2));
        
        /* Core loop */
            row0 = *src0++;
            row1 = *src1++;
        for (int x=0; x<bw; x+=1) chess_loop_range(1,){
            /* Load next 5 pixels */
            row2 = *src2++;
            
            /* Column Filter */
      s2 = vmul_slul((vec16s)fCol10, (vec16s)(pix0 = row0));
      s2 += vmul_shul((vec16s)fCol10, (vec16s)(pix1 = row1));
      s2 += vmul_slul((vec16s)fCol2, (vec16s)(pix0 = row2));
            
            /* Row Filter */
      acc = vmul((vec16s)fRow0, s0);
      acc += vmul((vec16s)fRow1, s1);
      acc += vmul((vec16s)fRow2, s2);
            
            /* Store */
            *dst0++ = acc;
            
            /* Shift State */
            s0=s1;s1=s2;
            row0 = *src0++;
            row1 = *src1++;
        }
        
        /* Increment pointers */
        src += sstr;
        dst += dstr;
    }
}

/**
 * Separable 5x5 kernel.
 */

void apu_flt_separable_5x5(vec16s* restrict dst, int dstr, const vec08u* restrict src,
                           int sstr, int bw, int bh, const int8_t* fRow, const int8_t* fCol){
    /* Load coefficients. */
    typedef int16_t coeff_t;
    register coeff_t chess_storage(R15) fCol10 = fCol[1]<<8 | (uint8_t)fCol[0]; fCol += 2;
    register coeff_t chess_storage(R14) fCol32 = fCol[1]<<8 | (uint8_t)fCol[0]; fCol += 2;
    register coeff_t chess_storage(R13) fCol4  =              (uint8_t)fCol[0];
    register coeff_t chess_storage(R12) fRow0  = *fRow++;
    register coeff_t chess_storage(R11) fRow1  = *fRow++;
    register coeff_t chess_storage(R10) fRow2  = *fRow++;
    register coeff_t chess_storage(R9)  fRow3  = *fRow++;
    register coeff_t chess_storage(R8)  fRow4  = *fRow++;
    
    /* Block width | 4, >= 4 */
    for (int y=0; y<bh; ++y) chess_loop_range(1,){
        /* Variables */
        vec16s chess_storage(V0) s0, chess_storage(V1) s1, /* Left, Left Middle */
               chess_storage(V2) s2, chess_storage(V3) s3, /* Middle, Right Middle */
               chess_storage(V4) s4;                       /* Right column */
        vec08u row0, row1, row2, row3, row4;
        vec08u chess_storage(V7) pix;
        vec16s acc;
        
        /* Row Pointers */
        const vec08u* restrict chess_storage(R1) src0 = (const vec08u*)(src-2*sstr-2);
        const vec08u* restrict chess_storage(R2) src1 = (const vec08u*)(src-1*sstr-2);
        const vec08u* restrict chess_storage(R3) src2 = (const vec08u*)(src+0*sstr-2);
        const vec08u* restrict chess_storage(R4) src3 = (const vec08u*)(src+1*sstr-2);
        const vec08u* restrict chess_storage(R5) src4 = (const vec08u*)(src+2*sstr-2);
        vec16s*       restrict chess_storage(R6) dst0 = dst;
        
        /* Preamble */
        /* Loop-4 */
        row0 = *src0++;
        row1 = *src1++;
        row2 = *src2++;
        row3 = *src3++;
        row4 = *src4++;
    s0 = vmul_slul((vec16s)fCol10, (vec16s)(pix = row0));
    s0 += vmul_shul((vec16s)fCol10, (vec16s)(pix = row1));
    s0 += vmul_slul((vec16s)fCol32, (vec16s)(pix = row2));
    s0 += vmul_shul((vec16s)fCol32, (vec16s)(pix = row3));
    s0 += vmul_slul((vec16s)fCol4, (vec16s)(pix = row4));
        /* Loop-3 */
        row0 = *src0++;
        row1 = *src1++;
        row2 = *src2++;
        row3 = *src3++;
        row4 = *src4++;
    s1 = vmul_slul((vec16s)fCol10, (vec16s)(pix = row0));
    s1 += vmul_shul((vec16s)fCol10, (vec16s)(pix = row1));
    s1 += vmul_slul((vec16s)fCol32, (vec16s)(pix = row2));
    s1 += vmul_shul((vec16s)fCol32, (vec16s)(pix = row3));
    s1 += vmul_slul((vec16s)fCol4, (vec16s)(pix = row4));
        /* Loop-2 */
        row0 = *src0++;
        row1 = *src1++;
        row2 = *src2++;
        row3 = *src3++;
        row4 = *src4++;
    s2 = vmul_slul((vec16s)fCol10, (vec16s)(pix = row0));
    s2 += vmul_shul((vec16s)fCol10, (vec16s)(pix = row1));
    s2 += vmul_slul((vec16s)fCol32, (vec16s)(pix = row2));
    s2 += vmul_shul((vec16s)fCol32, (vec16s)(pix = row3));
    s2 += vmul_slul((vec16s)fCol4, (vec16s)(pix = row4));
        /* Loop-1 */
        row0 = *src0++;
        row1 = *src1++;
        row2 = *src2++;
        row3 = *src3++;
        row4 = *src4++;
    s3 = vmul_slul((vec16s)fCol10, (vec16s)(pix = row0));
    s3 += vmul_shul((vec16s)fCol10, (vec16s)(pix = row1));
    s3 += vmul_slul((vec16s)fCol32, (vec16s)(pix = row2));
    s3 += vmul_shul((vec16s)fCol32, (vec16s)(pix = row3));
    s3 += vmul_slul((vec16s)fCol4, (vec16s)(pix = row4));
        
        /* Core loop */
            row0 = *src0++;
        for (int x=0; x<bw; x+=1) chess_loop_range(1,){
            /* Load next 5 pixels */
            row1 = *src1++;
            row2 = *src2++;
            row3 = *src3++;
            row4 = *src4++;
            
            /* Column Filter */
      s4 = vmul_slul((vec16s)fCol10, (vec16s)(pix = row0));
      s4 += vmul_shul((vec16s)fCol10, (vec16s)(pix = row1));
      s4 += vmul_slul((vec16s)fCol32, (vec16s)(pix = row2));
      s4 += vmul_shul((vec16s)fCol32, (vec16s)(pix = row3));
      s4 += vmul_slul((vec16s)fCol4, (vec16s)(pix = row4));
            
            /* Row Filter */
      acc = vmul((vec16s)fRow0, s0);
      acc += vmul((vec16s)fRow1, s1);
      acc += vmul((vec16s)fRow2, s2);
      acc += vmul((vec16s)fRow3, s3);
      acc += vmul((vec16s)fRow4, s4);
            
            /* Store */
            *dst0++ = acc;
            
            /* Shift State */
            s0=s1;s1=s2;s2=s3;s3=s4;
            
            
            row0 = *src0++;
        }
        
        /* Increment pointers */
        src += sstr;
        dst += dstr;
    }
}

}/* Namespace ht */

#endif /* ACF_KERNEL_IMPLEMENTATION */

/*!*********************************************************************************
*  \file apu_filter.c
*  \brief APU filter kernels.
***********************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION
//
#ifndef APEX2_EMULATE
#if __chess__
chess_properties {
   pointer_index_type      DMb, DMh, DMw: int15;
   pointer_postmodify_type DMb, DMh, DMw: int5; 
   pointer_index_type      VMb, VMw: int12;
   pointer_postmodify_type VMb, VMw: int5;    
};
#endif
#endif


#include "image_filters_apu.h"

static inline vec16s apu_flt_sobel_3x3_x(const vec08u* src, int sstr);
static inline vec16s apu_flt_sobel_3x3_y(const vec08u* src, int sstr);
static inline vec16s apu_flt_sobel_5x5_x(const vec08u* src, int sstr);
static inline vec16s apu_flt_sobel_5x5_y(const vec08u* src, int sstr);

/*!
   Calculate sum of absolute values of first order derivatives x and y using sobel 3x3.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void
apu_flt_sobel_3x3(
            vec08u* dst, int dstr,
      const vec08u* src, int sstr,
            int bw, int bh
)
{
   for (int y=0; y<bh; ++y) chess_loop_range(1,) 
   {
      for (int x=0; x<bw; ++x) chess_loop_range(1,) 
      {
         vec16s tx = apu_flt_sobel_3x3_x(&src[x], sstr);
         //vif (tx<0)     { tx = -tx; } vendif
         tx = (vec16s)vabs(tx);
         vec16s ty = apu_flt_sobel_3x3_y(&src[x], sstr);
         //vif (ty<0)     { ty = -ty; } vendif
         ty = (vec16s)vabs(ty);
   
         vec16u t = (vec16u)(tx+ty);
         // vif (t > 255)  { t  = 255;   } vendif
         t  = vsat(t, 0, 255);
   
         dst[x] = (vec08u)t;
      }
      
      src += sstr;
      dst += dstr;
   }
}

/*!
   Calculate sum of absolute values of first order derivatives x and y using sobel 3x3.

   \param dst_0  - [Output] Destination block pointer
   \param dstr_0 - [Input]  Destination block stride
   \param dst_1  - [Output] Destination block pointer
   \param dstr_1 - [Input]  Destination block stride
   \param src    - [Input]  Source block pointer
   \param sstr   - [Input]  Source block stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
 */
void
apu_flt_sobel_3x3_xy(
            vec08u* dst_0, int dstr_0,
            vec08u* dst_1, int dstr_1,
      const vec08u* src, int sstr,
            int bw, int bh
)
{
   for (int y=0; y<bh; ++y) chess_loop_range(1,) 
   {
      for (int x=0; x<bw; ++x) chess_loop_range(1,) 
      {
         vec16s tx = apu_flt_sobel_3x3_x(&src[x], sstr);

         vec16s ty = apu_flt_sobel_3x3_y(&src[x], sstr);
   
         dst_0[x] = (vec08u)vsat(tx, 0, 255);
         dst_1[x] = (vec08u)vsat(ty, 0, 255);
      }
      
      src   += sstr;
      dst_0 += dstr_0;
      dst_1 += dstr_1;
   }
}

void
apu_flt_sobel_3x3_x(
            vec08u* dst, int dstr,
      const vec08u* src, int sstr,
            int bw, int bh
)
{
   for (int y=0; y<bh; ++y) chess_loop_range(1,) 
   {
      for (int x=0; x<bw; ++x) chess_loop_range(1,) 
      {
         vec16s tx = apu_flt_sobel_3x3_x(&src[x], sstr);

         // Absolute Value & Saturation
         tx = vsat(tx, 0, 255);
        
         dst[x] = (vec08u)tx;
      }
      
      src += sstr;
      dst += dstr;
   }
}

void apu_flt_sobel_3x3_x(
            vec08s* dst, int dstr,
      const vec08u* src, int sstr,
            int bw, int bh
)
{
//! 3x3 Sobel x filter
    for (int h = 0; h < bh; h++)
    {
        for (int w = 0; w < bw; w++)
        {
            vec16s tx = (apu_flt_sobel_3x3_x(&src[w], sstr)>>3);
            dst[w] = (vec08s)vsat(tx, -128, 127);    // clamp to [-128,127]
        }
        src += sstr;
        dst += dstr;
    }
}

/*!
   Calculate first order derivative y using sobel 3x3.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void
apu_flt_sobel_3x3_y(
            vec08u* dst, int dstr,
      const vec08u* src, int sstr,
            int bw, int bh
)
{
   for (int y=0; y<bh; ++y) chess_loop_range(1,) 
   {
      for (int x=0; x<bw; ++x) chess_loop_range(1,) 
      {
         vec16s ty = apu_flt_sobel_3x3_y(&src[x], sstr);
         
         // Absolute Value & Saturation
         ty = vsat(ty, 0, 255);
         
         dst[x] = (vec08u)ty;
      }
      
      src += sstr;
      dst += dstr;
   }
}

/*!
   Calculate first order derivative y using sobel 3x3.
   (unsigned 8 bit to signed 8 bit)

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void apu_flt_sobel_3x3_y(
            vec08s* dst, int dstr,
      const vec08u* src, int sstr,
            int bw, int bh
)
{
//! 3x3 Sobel y filter
    for (int h = 0; h < bh; h++)
    {
        for (int w = 0; w < bw; w++)
        {
            vec16s ty = (apu_flt_sobel_3x3_y(&src[w], sstr)>>3);
            dst[w] = (vec08s)vsat(ty, -128, 127);    // clamp to [-128, 127]
        }
        src += sstr;
        dst += dstr;
    }
}

/*! 
   Saturation step for 16 bits sobel results to 8 bits. 
   Scales the output by 8, and rounds up ( (input >> 2) + 1 ) >> 1 )
*/
void apu_flt_saturate_sobel_3x3 (
               vec08s* dst, int dstr,
         const vec16s* src, int sstr,
               int bw, int bh
)
{
   for (int16u h = 0; h < bh; h++) chess_loop_range(1,) // skip the check for bh being 0
   {
      const vec16s* restrict ps = src; // need temp ptrs because dst and src incremented by stride lenghts
      vec08s* restrict pd = dst;
      
      vec16s a = *ps++;
      vec16s b = *ps++;
      
      for (int16u w = 0; w < bw; w+=2) chess_loop_range(1,)
      {
         *pd++ = (vec08s)vsat(((a + 4) >> 3),-128,127);
         a = *ps++;
         *pd++ = (vec08s)vsat(((b + 4) >> 3),-128,127);
         b = *ps++;
      }
      dst += dstr;
      src += sstr;
   }
}
/*!
   Apply a 3x3 Sobel x filter to an image
   (unsigned 8-bit -> signed 16-bit -> scaled signed 8-bit)

   \param dst     - [Output]  Pointer to the destination buffer.
   \param dstr    - [Input]   Stride of the destination buffer.
   \param dst16s  - [Output]  Pointer to the 16-bit output buffer before the saturation step
   \param src     - [Input]   Pointer to the source image. The source image is assumed to be padded as (n,s,w,e) = (1,1,2,2).
   \param sstr    - [Input]   Stride of the padded source image.
   \param bw      - [Input]   Chunk width.
   \param bh      - [Input]   Chunk height.
*/
void apu_flt_sobel_3x3_x(
               vec08s* dst, int dstr,
               vec16s* dst16s,
         const vec08u* src, int sstr,
               int bw, int bh
)
{
   ht::apu_flt_centraldx_3x3( dst16s, dstr, src, sstr, bw, bh, 1, 2, 1 );
   apu_flt_saturate_sobel_3x3( dst, dstr, dst16s, dstr, bw, bh ); 
}
/*!
   Apply a 3x3 Sobel y filter to an image
   (unsigned 8-bit -> signed 16-bit -> scaled signed 8-bit)

   \param dst     - [Output]  Pointer to the destination buffer.
   \param dstr    - [Input]   Stride of the destination buffer.
   \param dst16s  - [Output]  Pointer to the 16-bit output buffer before the saturation step
   \param src     - [Input]   Pointer to the source image. The source image is assumed to be padded as (n,s,w,e) = (1,1,2,2).
   \param sstr    - [Input]   Stride of the padded source image.
   \param bw      - [Input]   Chunk width.
   \param bh      - [Input]   Chunk height.
*/
void apu_flt_sobel_3x3_y (
               vec08s* dst, int dstr,
               vec16s* dst16s,
         const vec08u* src, int sstr,
               int bw, int bh
)
{
   ht::apu_flt_centraldy_3x3( dst16s, dstr, src, sstr, bw, bh, 1, 2, 1 );
   apu_flt_saturate_sobel_3x3( dst, dstr, dst16s, dstr, bw, bh ); 
}

/*!
   Calculate sum of absolute values of first order derivatives x and y using sobel 5x5.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void
apu_flt_sobel_5x5(            
            vec08u* dst, int dstr,
      const vec08u* src, int sstr,
            int bw, int bh
)
{
   for (int y=0; y<bh; ++y) chess_loop_range(1,) 
   {
      for (int x=0; x<bw; ++x) chess_loop_range(1,) 
      {
         vec16s tx = apu_flt_sobel_5x5_x(&src[x], sstr);
         //vif (tx<0)     { tx = -tx; } vendif
         tx = (vec16s)vabs(tx);
         
         vec16s ty = apu_flt_sobel_5x5_y(&src[x], sstr);
         //vif (ty<0)     { ty = -ty; } vendif
         ty = (vec16s)vabs(ty);
         
         vec16u t = (vec16u)(tx+ty);
         vif (t > 255)  { t  = 255; } vendif
   
         dst[x] = (vec08u)t;
      }
      
      src += sstr;
      dst += dstr;
   }
}

/*!
   Calculate first order derivative x using sobel 5x5.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void
apu_flt_sobel_5x5_x(
            vec08u* dst, int dstr,
      const vec08u* src, int sstr,
            int bw, int bh
)
{
   for (int y=0; y<bh; ++y) chess_loop_range(1,) 
   {
      for (int x=0; x<bw; ++x) chess_loop_range(1,) 
      {
         vec16s tx = apu_flt_sobel_5x5_x(&src[x], sstr);
         
         // Absolute Value & Saturation
         tx = vsat(tx, 0, 255);
         
         dst[x] = (vec08u)tx;
      }
      
      src += sstr;
      dst += dstr;
   }
}

/*!
   Calculate first order derivative x using sobel 5x5.
   (unsigned 8 bit to signed 8 bit)

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void 
apu_flt_sobel_5x5_x(
            vec08s* dst, int dstr,
      const vec08u* src, int sstr,
            int bw, int bh
)
{
//! 5x5 Sobel x filter
    for (int h = 0; h < bh; h++)
    {
        for (int w = 0; w < bw; w++)
        {
            vec16s tx = (apu_flt_sobel_5x5_x(&src[w], sstr)>>5);
            dst[w] = (vec08s)vsat(tx, -128, 127);    // clamp to [-128, 127]
        }
        src += sstr;
        dst += dstr;
    }
}

/*!
   Calculate first order derivative y using sobel 5x5.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void
apu_flt_sobel_5x5_y(            
            vec08u* dst, int dstr,
      const vec08u* src, int sstr,
            int bw, int bh
)
{
   for (int y=0; y<bh; ++y) chess_loop_range(1,)  
   {
      for (int x=0; x<bw; ++x) chess_loop_range(1,) 
      {
         vec16s ty = apu_flt_sobel_5x5_y(&src[x], sstr);

         // Absolute Value & Saturation
         ty = vsat(ty, 0, 255);
      
         dst[x] = (vec08u)ty;
      }
      
      src += sstr;
      dst += dstr;
   }
}

void 
apu_flt_sobel_5x5_y(            
            vec08s* dst, int dstr,
      const vec08u* src, int sstr,
            int bw, int bh
)
{
//! 5x5 Sobel y filter
    for (int h = 0; h < bh; h++)
    {
        for (int w = 0; w < bw; w++)
        {
            vec16s ty = (apu_flt_sobel_5x5_y(&src[w], sstr)>>5);
            dst[w] = (vec08s)vsat(ty, -128, 127);    // clamp tp [-128, 127]
        }
        src += sstr;
        dst += dstr;
    }
}

// LOCAL HELPER FUNCTIONS

//  -1,  0,  1,
//  -2,  0,  2,
//  -1,  0,  1,
static
inline
vec16s
apu_flt_sobel_3x3_x(const vec08u* src, int sstr)
{
  vec16s vd = +(vec16s)src[-sstr + 1] - (vec16s)src[-sstr - 1]
    + (( (vec16s) src[+1] - (vec16s)src[-1]) << 1 )
    + (vec16s)src[+sstr + 1] - (vec16s)src[+sstr - 1];
               
               
   return vd;
}

// -1, -2, -1,
//  0,  0,  0,
// +1, +2, +1,
static
inline
vec16s
apu_flt_sobel_3x3_y(const vec08u* src, int sstr)
{
  vec16s t7 = +(vec16s)src[+sstr + 1] - (vec16s)src[-sstr - 1];
  vec16s t9 = +(vec16s)src[+sstr - 1] - (vec16s) src[-sstr + 1];
  vec16s t8 = +(vec16s)src[+sstr] - (vec16s)src[-sstr];
   vec16s vd = t7 + (t8 << 1) + t9;
   
   return vd;
}


/*
    -1,  -2,  0,  +2,  +1,
    -4,  -8,  0,  +8,  +4,
    -6, -12,  0, +12,  +6,
    -4,  -8,  0,  +8,  +4,
    -1,  -2,  0,  +2,  +1, 
 */    
static
inline
vec16s
apu_flt_sobel_5x5_x(const vec08u* src, int sstr)
{
  const vec16s t22 = -(vec16s)src[-2 * sstr - 2] + (vec16s)src[-2 * sstr + 2] - (vec16s)src[+2 * sstr - 2] + (vec16s)src[+2 * sstr + 2];
  const vec16s t12 = -(vec16s)src[-2 * sstr - 1] + (vec16s)src[-2 * sstr + 1] - (vec16s)src[+2 * sstr - 1] + (vec16s)src[+2 * sstr + 1];
  const vec16s t21 = -(vec16s)src[-1 * sstr - 2] + (vec16s)src[-1 * sstr + 2] - (vec16s)src[+1 * sstr - 2] + (vec16s)src[+1 * sstr + 2];
  const vec16s t11 = -(vec16s)src[-1 * sstr - 1] + (vec16s)src[-1 * sstr + 1] - (vec16s)src[+1 * sstr - 1] + (vec16s)src[+1 * sstr + 1];
  const vec16s t20 = -(vec16s)src[-2] + (vec16s)src[+2];
  const vec16s t10 = -(vec16s)src[-1] + (vec16s)src[+1];

   const vec16s tx   = t22 + 2*t12 + 4*t21 + 8*t11 + 6*t20 + 12*t10;
   
   return tx;
}

/*
    -1,  -4,  -6,  -4,  -1,
    -2,  -8, -12,  -8,  -2,
     0,   0,   0,   0,   0,
    +2,  +8, +12,  +8,  +2,
    +1,  +4,  +6,  +4,  +1,
*/
static
inline
vec16s
apu_flt_sobel_5x5_y(const vec08u* src, int sstr)
{
  const vec16s t22 = -(vec16s)src[-2 * sstr - 2] - (vec16s)src[-2 * sstr + 2] + (vec16s)src[+2 * sstr - 2] + (vec16s)src[+2 * sstr + 2];
  const vec16s t12 = -(vec16s)src[-2 * sstr - 1] - (vec16s)src[-2 * sstr + 1] + (vec16s)src[+2 * sstr - 1] + (vec16s)src[+2 * sstr + 1];
  const vec16s t21 = -(vec16s)src[-1 * sstr - 2] - (vec16s)src[-1 * sstr + 2] + (vec16s)src[+1 * sstr - 2] + (vec16s)src[+1 * sstr + 2];
  const vec16s t11 = -(vec16s)src[-1 * sstr - 1] - (vec16s)src[-1 * sstr + 1] + (vec16s)src[+1 * sstr - 1] + (vec16s)src[+1 * sstr + 1];
  const vec16s t02 = -(vec16s)src[-2 * sstr] + (vec16s)src[+2 * sstr];
  const vec16s t01 = -(vec16s)src[-1 * sstr] + (vec16s)src[+1 * sstr];

   const vec16s tx   = t22 + 4*t12 + 2*t21 + 8*t11 + 6*t02 + 12*t01;
   
   return tx;
}

#endif /* ACF_KERNEL_IMPLEMENTATION */

/*!*********************************************************************************
*  \file apu_filter.c
*  \brief APU filter kernels.
***********************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "image_filters_apu.h"

namespace ht{/* Namespace ht */

void apu_flt_saturate (
               vec08s* dst, int dstr,
         const vec16s* src, int sstr,
               int bw, int bh
)
{
   for (int16u h = 0; h < bh; h++) chess_loop_range(1,) // skip the check for bh being 0
   {
      const vec16s* restrict ps = src; // need temp ptrs because dst and src incremented by stride lenghts
      vec08s* restrict pd = dst;
      
      vec16s a = *ps++;
      vec16s b = *ps++;
      
      for (int16u w = 0; w < bw; w+=2) chess_loop_range(1,)
      {
         *pd++ = (vec08s)vsat(((a + 4) >> 3),-128,127);
         a = *ps++;
         *pd++ = (vec08s)vsat(((b + 4) >> 3),-128,127);
         b = *ps++;
      }
      dst += dstr;
      src += sstr;
   }
}

/* Sobel 3x3 */
void apu_flt_sobel_3x3(vec16s* restrict dst, int dstr, const vec08u* restrict src,
                         int sstr, int bw, int bh){
    const int chess_storage(R18) k2k1 = chess_copy(0x0201);
    const int chess_storage(R19) k0k2 = chess_copy(0x0002);
    const int chess_storage(R20) k255 = chess_copy(0x00FF);
    
    /* Block width | 4, >= 4 */
    for (int y=0; y<bh; ++y) chess_loop_range(1,){
        /* Variables */
        vec16s chess_storage(V0) lxcol, chess_storage(V1) mxcol, rxcol;
        vec16s chess_storage(V2) lycol, chess_storage(V3) mycol, rycol;
        vec16s lpix,  mpix,  upix;
        
        
        /* Row Pointers */
        const vec08u* src0 = (const vec08u*)(src-sstr-1);
        const vec08u* src1 = (const vec08u*)(src     -1);
        const vec08u* src2 = (const vec08u*)(src+sstr-1);
        vec16s*       dst0 = dst;
        
        upix = (vec16s)*src0++;
        lpix = (vec16s)*src2++;
        mpix = (vec16s)*src1++;
        lycol = lpix-upix;
        lxcol = upix+vmul_ulsh(mpix, k2k1)+lpix;
        upix = (vec16s)*src0++;
        lpix = (vec16s)*src2++;
        mpix = (vec16s)*src1++;
        mycol = lpix-upix;
        mxcol = upix+vmul_ulsh(mpix, k2k1)+lpix;
        upix = (vec16s)*src0++;
        lpix = (vec16s)*src2++;
        mpix = (vec16s)*src1++;
        rycol = lpix-upix;
        
        
        /* Core loop */
        for (int x=0; x<bw; x+=1) chess_loop_range(1,){
            
            /* Compute separable, column direction */
            rxcol = upix+vmul_ulsh(mpix, k2k1)+lpix;
            
            /* Compute separable, row direction */
            lxcol = rxcol-lxcol;
      lycol = lycol + vmul(mycol, (vec16s) k0k2) + rycol;
            
            /* Abs+Abs+Add+Sat+Store */
            *dst0++ = vsat(vabs_diff(lxcol, (vec16s)0)+vabs_diff(lycol, (vec16s)0), 0, k255);
            
            /* Save State */
            lxcol = mxcol;mxcol = rxcol;
            lycol = mycol;mycol = rycol;
            
            /* Load next 3 pixels */
            upix = (vec16s)*src0++;
            lpix = (vec16s)*src2++;
            mpix = (vec16s)*src1++;
            rycol = lpix-upix;
        }
        
        /* Increment pointers */
        src += sstr;
        dst += dstr;
    }
}

void apu_flt_sobel_3x3_x(
               vec08s* dst, int dstr,
               vec16s* dst16s,
         const vec08u* src, int sstr,
               int bw, int bh
)
{
   ht::apu_flt_centraldx_3x3( dst16s, dstr, src, sstr, bw, bh, 1, 2, 1 );
   ht::apu_flt_saturate( dst, dstr, dst16s, dstr, bw, bh ); 
}

void apu_flt_sobel_3x3_y (
               vec08s* dst, int dstr,
               vec16s* dst16s,
         const vec08u* src, int sstr,
               int bw, int bh
)
{
   ht::apu_flt_centraldy_3x3( dst16s, dstr, src, sstr, bw, bh, 1, 2, 1 );
   ht::apu_flt_saturate( dst, dstr, dst16s, dstr, bw, bh ); 
}

}/* Namespace ht */

#endif /* ACF_KERNEL_IMPLEMENTATION */
