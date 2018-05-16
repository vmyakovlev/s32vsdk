/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
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
*  \file apu_filter.h
*  \brief APU filter kernels.
***********************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION
#ifndef IMAGEFILTERSAPU_H
#define IMAGEFILTERSAPU_H
#include <stdint.h>

#ifdef APEX2_EMULATE
#include "apu_lib.hpp"
#include "apu_extras.hpp"
using namespace APEX2;
#else
  #ifndef __llvm__
    #include <APU2_vector.h>
  #endif
#endif
#include <stdint.h>

/*!
   Creates the LUT for bilateral filter from specified sigmas

   \param r_lut  - [Output] Intensity (range) lut
   \param d_lut  - [Output] Distance (domain) lut
   \param simgaR - [Input]  Intensity (range) sigma value
   \param sigmaD - [Input]  Distance (domain) sigma value
 */
void
apu_flt_bilateral_5x5_init(vec08u __cmem *r_lut, uint8_t *d_lut, int sigmaR, int sigmaD);

/*!
   Calculates 5x5 bilateral filter of image

   \param dst    - [Output] Destination block pointer
   \param dstr   - [Input]  Destination block stride
   \param src    - [Input]  Source value block pointer
   \param sstr   - [Input]  Source value block stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
   \param r_lut  - [Input]  Intensity (range) lut
   \param d_lut  - [Input]  Distance (domain) lut
 */
void
apu_flt_bilateral_5x5( vec08u* dst, int dstr,
             const vec08u* src, int sstr,
                        int bw, int bh,
          vec08u __cmem *r_lut, uint8_t *d_lut );

// Box Filters
/*!
   Apply a 1x3 box filter to an image.
   (signed 16-bit -> signed 16-bit)

   \param dst     - [Output]  Pointer to the destination buffer.
   \param dstr    - [Input]   Stride of the destination buffer.
   \param src     - [Input]   Pointer to the source image. The source image is assumed to be padded as (n,s,w,e)=(1,1,0,0).
   \param sstr    - [Input]   Stride of the source image.
   \param bw      - [Input]   Chunk width.
   \param bh      - [Input]   Chunk height.
 */
void apu_flt_box_1x3( vec16s* dst, int dstr, const vec16s* src, int sstr, int bw, int bh );

/*!
   Apply a 3x1 box filter to an image.
   (signed 16-bit -> signed 16-bit)

   \param dst     - [Output]  Pointer to the destination buffer.
   \param dstr    - [Input]   Stride of the destination buffer.
   \param src     - [Input]   Pointer to the source image. The source image is assumed to be padded as (n,s,w,e)=(0,0,1,1).
   \param sstr    - [Input]   Stride of the source image.
   \param bw      - [Input]   Chunk width.
   \param bh      - [Input]   Chunk height.
 */
void apu_flt_box_3x1( vec16s* dst, int dstr, const vec16s* src, int sstr, int bw, int bh );

/*!
   Calculates a box 3x3 filter.

   \param dst    - [Output] Destination block pointer
   \param dstr   - [Input]  Destination block stride
   \param src    - [Input]  Source value block pointer
   \param sstr   - [Input]  Source value block stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
 */
void
apu_flt_box_3x3( vec08u* dst, int dstr,
       const vec08u* src, int sstr,
                  int bw, int bh );

/*!
   Calculates a box 3x3 filter.

   \param dst    - [Output] Destination block pointer
   \param dstr   - [Input]  Destination block stride
   \param src    - [Input]  Source value block pointer
   \param sstr   - [Input]  Source value block stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
 */
void
apu_flt_box_3x3( vec16s* dst, int dstr,
       const vec16s* src, int sstr,
                  int bw, int bh );

/*!
   Calculates a box 5x5 filter.

   \param dst    - [Output] Destination block pointer
   \param dstr   - [Input]  Destination block stride
   \param src    - [Input]  Source value block pointer
   \param sstr   - [Input]  Source value block stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
 */
void
apu_flt_box_5x5( vec16s* dst, int dstr,
       const vec16s* src, int sstr,
                  int bw, int bh );
/*!
   Calculates a census 3x3 filter.

   \param dst    - [Output] Destination block pointer
   \param dstr   - [Input]  Destination block stride
   \param src    - [Input]  Source value block pointer
   \param sstr   - [Input]  Source value block stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
 */
void
apu_flt_census_3x3(vec08u* dst, int dstr,
                   const vec08u* src, int sstr,
                   int bw, int bh); 
                   
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
                 int bw, int bh);

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
            vec16s* dst, int dstr,
      const vec16s* src, int sstr,
                 int bw, int bh);

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
                 int bw, int bh);

// Generic 2D Filters
/*!
   Calculate the output of generic (fw x fh) filter.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
   \param flt  - [Input]  ilter Coeffiencients (pointer)
   \param fw   - [Input]  Filter width
   \param fh   - [Input]  Filter height
 */
void apu_flt_fir2(vec08u* dst, int dstr, const vec08u* src, int sstr, int bw, int bh, const int8_t* flt, int fw, int fh);
/*!
   Calculate the output of generic (fw x fh) filter with scaling.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
   \param flt  - [Input]  ilter Coeffiencients (pointer)
   \param fw   - [Input]  Filter width
   \param fh   - [Input]  Filter height
   \param fscl - [Input]  Filter Scale (2^fscl)
 */
void apu_flt_fir2(vec08u* dst, int dstr, const vec08u* src, int sstr, int bw, int bh, const int8_t* flt, int fw, int fh, int fscl);


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
apu_flt_fir2(vec16s* dst, int dstr, const vec08u* src, int sstr, int bw, int bh, const int8_t* flt, int fw, int fh);

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
void apu_flt_fir2(vec16s* dst, int dstr, const vec08u* src, int sstr, int bw, int bh, const int8_t* flt, int fw, int fh, int fscl);

/*!
   Calculate first order derivative x using gaussian 3x3.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void apu_flt_gaussian_3x3(vec08u* dst, int dstr, const vec08u* src, int sstr, int bw, int bh);

/*!
   Calculate first order derivative x using gaussian 5x5.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void apu_flt_gaussian_5x5(vec08u* dst, int dstr, const vec08u* src, int sstr, int bw, int bh);

// Median Filters
/*!
   Calculate a 3x3 median filter.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void apu_flt_median_3x3 (vec08u* dst, int dstr, const vec08u* src, int sstr, int bw, int bh);

/*!
   Calculate a 5x5 median filter.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void
apu_flt_median_5x5(
            vec08u* dst, int dstr,
      const vec08u* src, int sstr,
                 int bw, int bh);

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
              int bw, int bh );

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
              int bw, int bh );

// Sobel Filters (3x3)
/*!
   Calculate sum of absolute values of first order derivatives x and y using sobel 3x3.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void apu_flt_sobel_3x3  (vec08u* dst, int dstr, const vec08u* src, int sstr, int bw, int bh);

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
void apu_flt_sobel_3x3_xy ( vec08u* dst_0, int dstr_0, vec08u* dst_1, int dstr_1, const vec08u* src, int sstr, int bw, int bh);

/*!
   Calculate first order derivative x using sobel 3x3.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void apu_flt_sobel_3x3_x ( vec08u* dst, int dstr, const vec08u* src, int sstr, int bw, int bh );

/*!
   Calculate first order derivative x using sobel 3x3.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void apu_flt_sobel_3x3_x ( vec08s* dst, int dstr, const vec08u* src, int sstr, int bw, int bh );

/*!
   Calculate first order derivative y using sobel 3x3.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void apu_flt_sobel_3x3_y (vec08u* dst, int dstr, const vec08u* src, int sstr, int bw, int bh);

/*!
   Calculate first order derivative y using sobel 3x3.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void apu_flt_sobel_3x3_y (vec08s* dst, int dstr, const vec08u* src, int sstr, int bw, int bh);

// Sobel Filters (5x5)
/*!
   Calculate sum of absolute values of first order derivatives x and y using sobel 5x5.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void apu_flt_sobel_5x5  (vec08u* dst, int dstr, const vec08u* src, int sstr, int bw, int bh);

/*!
   Calculate first order derivative x using sobel 5x5.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void apu_flt_sobel_5x5_x(vec08u* dst, int dstr, const vec08u* src, int sstr, int bw, int bh);

/*!
   Calculate first order derivative x using sobel 5x5.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void apu_flt_sobel_5x5_x(vec08s* dst, int dstr, const vec08u* src, int sstr, int bw, int bh);

/*!
   Calculate first order derivative y using sobel 5x5.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void apu_flt_sobel_5x5_y(vec08u* dst, int dstr, const vec08u* src, int sstr, int bw, int bh);

/*!
   Calculate first order derivative y using sobel 5x5.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
 */
void apu_flt_sobel_5x5_y(vec08s* dst, int dstr, const vec08u* src, int sstr, int bw, int bh);

/* Namespace for Hand-Tuned functions */
namespace ht {
/*!
   3x3 box kernel.
 
   The image is convolved with the normalized box kernel, given below:
 
            1  1  1
            1  1  1       * (1/9)
            1  1  1

   \param dst    - [Output] Destination block pointer
   \param dstr   - [Input]  Destination block stride
   \param src    - [Input]  Source value block pointer
   \param sstr   - [Input]  Source value block stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
 */
void apu_flt_box_3x3(vec08u* restrict dst, int dstr, const vec08u* restrict src,
                        int sstr, int bw, int bh);

/*!
   Compute general central x derivative kernels of size 3x3.
 
   The image is convolved with the following kernel:
 
      -k0  0   k0
      -k1  0   k1
      -k2  0   k2
 
   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
   \param k0   - [Input]  1st Convolution Coefficient
   \param k1   - [Input]  2nd Convolution Coefficient
   \param k2   - [Input]  3rd Convolution Coefficient
 */
void apu_flt_centraldx_3x3(vec16s* restrict dst, int dstr, const vec08u* restrict src,
                           int sstr, int bw, int bh,
                           signed char k0, signed char k1, signed char k2);

/*!
   Compute general central y derivative kernels of size 3x3.
 
   The image is convolved with the following kernel:
 
      -k0 -k1 -k2
        0   0   0
       k0  k1  k2
 
   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
   \param k0   - [Input]  1st Convolution Coefficient
   \param k1   - [Input]  2nd Convolution Coefficient
   \param k2   - [Input]  3rd Convolution Coefficient
 */
void apu_flt_centraldy_3x3(vec16s* restrict dst, int dstr, const vec08u* restrict src,
                           int sstr, int bw, int bh,
                           signed char k0, signed char k1, signed char k2);

/*!
   Generic 3x3 kernel.
 
   The kernel's coefficients are
 
            f0  f1  f2
            f3  f4  f5
            f6  f7  f8
   NOTES:
 
   - The filter coefficients must be given 4-byte aligned!
   - fscale is the amount by which to shift right the accumulator prior to
    saturation.
            
   \param dst    - [Output] Destination block pointer
   \param dstr   - [Input]  Destination block stride
   \param src    - [Input]  Source block pointer
   \param sstr   - [Input]  Source block stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
   \param flt    - [Input]  3x3 Filter Coefficients
   \param fscale - [Input]  Scaling Factor (right shift)
 */
void apu_flt_generic_3x3(vec08u* restrict dst, int dstr, const vec08u* restrict src,
                            int sstr, int bw, int bh, const int8_t* restrict flt, int fscale);

/*!
   Generic 5x5 kernel.
 
   The kernel's coefficients are
 
            f0  f1  f2  f3  f4
            f5  f6  f7  f8  f9
            f10 f11 f12 f13 f14
            f15 f16 f17 f18 f19
            f20 f21 f22 f23 f24
   NOTES:
 
   - The filter coefficients must be given 4-byte aligned!
   - fscale is the amount by which to shift right the accumulator prior to
    saturation.
            
   \param dst    - [Output] Destination block pointer
   \param dstr   - [Input]  Destination block stride
   \param src    - [Input]  Source block pointer
   \param sstr   - [Input]  Source block stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
   \param flt    - [Input]  5x5 Filter Coefficients
   \param fscale - [Input]  Scaling Factor (right shift)
 */
void apu_flt_generic_5x5(vec08u* restrict dst, int dstr, const vec08u* restrict src,
                            int sstr, int bw, int bh, const int8_t* restrict flt, int fscale);

/*!
   Saturates a signed 16-bit input to a signed 8-bit output.
   Scales the output by 8, and rounds up ( (input +4) >> 3 )
            
   \param dst    - [Output] Destination block pointer
   \param dstr   - [Input]  Destination block stride
   \param src    - [Input]  Source block pointer
   \param sstr   - [Input]  Source block stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
*/
void apu_flt_saturate (
               vec08s* dst, int dstr,
         const vec16s* src, int sstr,
               int bw, int bh);

/*!
   Separable 3x3 kernel.
 
   The image is convolved with the 2D separable kernel
 
            fCol[0]
            fCol[1]  *  fRow[0] fRow[1] fRow[2]
            fCol[2]
 
   \param dst    - [Output] Destination block pointer
   \param dstr   - [Input]  Destination block stride
   \param src    - [Input]  Source block pointer
   \param sstr   - [Input]  Source block stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
   \param frow   - [Input]  3x1 Row Filter Coefficients
   \param fcol   - [Input]  3x1 Column Filter Coefficients
 */
void apu_flt_separable_3x3(vec16s* restrict dst, int dstr, const vec08u* restrict src,
                           int sstr, int bw, int bh, const int8_t* fRow, const int8_t* fCol);

/*!
   Separable 5x5 kernel.
 
   The image is convolved with the 2D separable kernel
 
            fCol[0]
            fCol[1]
            fCol[2]  *  fRow[0] fRow[1] fRow[2] fRow[3] fRow[4]
            fCol[3]
            fCol[4]
            
   \param dst    - [Output] Destination block pointer
   \param dstr   - [Input]  Destination block stride
   \param src    - [Input]  Source block pointer
   \param sstr   - [Input]  Source block stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
   \param frow   - [Input]  5x1 Row Filter Coefficients
   \param fcol   - [Input]  5x1 Column Filter Coefficients
 */
void apu_flt_separable_5x5(vec16s* restrict dst, int dstr, const vec08u* restrict src,
                           int sstr, int bw, int bh, const int8_t* fRow, const int8_t* fCol);

/*!
   Sobel 3x3 combined.
 
   Evaluates the dx kernel
 
            -1  0  1
            -2  0  2
            -1  0  1
 
   and dy kernel
 
            -1  0  1
            -2  0  2
            -1  0  1
 
   , then outputs
 
         sat(abs(dx)+abs(dy), 0, 255)
 
   \param dst    - [Output] Destination block pointer
   \param dstr   - [Input]  Destination block stride
   \param src    - [Input]  Source block pointer
   \param sstr   - [Input]  Source block stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
 */
void apu_flt_sobel_3x3(vec16s* restrict dst, int dstr, const vec08u* restrict src,
                         int sstr, int bw, int bh);

/*!
   Apply a 3x3 Sobel x filter to an image
   (unsigned 8-bit -> signed 16-bit -> scaled signed 8-bit)
   This utilizes the apu_flt_central_dx_3x3() kernel and the apu_flt_saturate() kernel.

   \param dst     - [Output]  Pointer to the destination buffer.
   \param dstr    - [Input]   Stride of the destination buffer.
   \param dst16s  - [Output]  Pointer to the 16-bit output buffer before the saturation step
   \param src     - [Input]   Pointer to the source image. The source image is assumed to be padded as (n,s,w,e) = (0,0,1,1).
   \param sstr    - [Input]   Stride of the padded source image.
   \param bw      - [Input]   Chunk width.
   \param bh      - [Input]   Chunk height.
*/
void apu_flt_sobel_3x3_x ( vec08s* dst, int dstr, vec16s* dst16s, const vec08u* src, int sstr, int bw, int bh );

/*!
   Apply a 3x3 Sobel y filter to an image
   (unsigned 8-bit -> signed 16-bit -> scaled signed 8-bit)
   This utilizes the apu_flt_central_dy_3x3() kernel and the apu_flt_saturate() kernel.

   \param dst     - [Output]  Pointer to the destination buffer.
   \param dstr    - [Input]   Stride of the destination buffer.
   \param dst16s  - [Output]  Pointer to the 16-bit output buffer before the saturation step
   \param src     - [Input]   Pointer to the source image. The source image is assumed to be padded as (n,s,w,e) = (0,0,1,1).
   \param sstr    - [Input]   Stride of the padded source image.
   \param bw      - [Input]   Chunk width.
   \param bh      - [Input]   Chunk height.
*/
void apu_flt_sobel_3x3_y ( vec08s* dst, int dstr, vec16s* dst16s, const vec08u* src, int sstr, int bw, int bh );

} /* ht namespace */

#endif /* IMAGEFILTERSAPU_H */
#endif /* ACF_KERNEL_IMPLEMENTATION */
