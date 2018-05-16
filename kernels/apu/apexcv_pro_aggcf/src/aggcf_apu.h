/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor
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

#ifdef ACF_KERNEL_IMPLEMENTATION

#ifndef AGGCFAPU_H
#define AGGCFAPU_H

#include "stdint.h"
//#include "assert.h"

/* Common types: */
/* 16-bit */
typedef int16_t   fx16p0;
typedef uint16_t ufx16p0;
typedef uint16_t ufx16p1;
typedef uint16_t ufx16p2;
/* 32-bit */
typedef uint32_t ufx32p0;
typedef int32_t   fx32p0;
typedef uint32_t ufx32p1;
typedef uint32_t ufx32p2;
/* 64-bit */
typedef uint64_t ufx64p0;
typedef int64_t   fx64p0;
typedef uint64_t ufx64p1;


/**
 * unsigned 32 bit integer
 */
typedef uint32_t fx_u32;
/**
 * signed 32 bit integer
 */
typedef int32_t  fx_s32;
typedef int64_t  fx_s64;

typedef fx_s32   fx_scirc;
#define CORDIC_CIRC_HALF         (1U<<31)
typedef fx_s32   fx_s0229;
typedef fx_u32   fx_u0329;
typedef fx_u32   fx_ucirc;


typedef struct CORDIC_STATE{
   //! CORDIC “computer” X register: x coordinate of the rotation vector. Value stored in QS2.29 format.
   fx_s0229 x;
   //! CORDIC “computer” Y register: y coordinate of the rotation vector. Value stored in QS2.29 format.
   fx_s0229 y;
   //! CORDIC “computer” T register: elementary rotation angle. Value stored in QS2.29 format.
   fx_s0229 t;
} CORDIC_STATE;


typedef struct result
{
   vec32u M0;
   vec32u M1;
   vec32u O0;
   vec32u O1;
}interpolation_Result;


vec32s count_leading_zero(vec32u x);

vec32s count_leading_digit(vec32s x);

uint32_t count_leading_zero_s(uint32_t x);

int32_t count_leading_digit_s(int32_t x);

ufx32p0 fxrecip22(ufx32p1 d);


void fxgradMag_c_row_opt(vec16u* L, vec16u* U, vec16u* V, int32_t instride, int32_t outstride,
                     vec16u* M, vec16u* O,
                     int32_t h, int32_t w,
                     vec32s *Gx, vec32s *Gy, vec32u *M2, vec32u *lut);

void fxgradMag_c_row(vec16u* L, vec16u* U, vec16u* V, int32_t instride, int32_t outstrde,
                     vec16u* M, vec16u* O,
                     int32_t h, int32_t w, int32_t full,
                     vec32s *Gx, vec32s *Gy, vec32u *M2);

void fxgradHist_c_row_nontrilinear_pixel(vec16u* M, vec16u* O,
                 vec16u *bin0, vec16u *bin1, vec16u *bin2, vec16u *bin3, vec16u *bin4, vec16u *bin5,
                 int32_t h, int32_t w, int32_t instride, int32_t outstride,
                 int32_t hb, int32_t wb,
                 int32_t nb, int32_t bin, int32_t nOrientations, int32_t softBin, int32_t full,
                 vec32u *M0, vec32u*M1, vec32u*O0, vec32u *O1, int32_t *offset);


void fxgradHist_c_row_nontrilinear(vec16u* M, vec16u* O, 
                 vec16u* bin0, vec16u *bin1, vec16u *bin2, vec16u *bin3, vec16u *bin4, vec16u *bin5,
                 int32_t h, int32_t w, int32_t instride, int32_t outstride,
		 int32_t hb, int32_t wb,
                 int32_t nb, int32_t bin, int32_t nOrientations, int32_t softBin, int32_t full,
                 vec32u *M0, vec32u*M1, vec32u*O0, vec32u *O1, int32_t *offset);


void fxgradHist_c_row_trilinear(vec16u* M, vec16u* O, 
                vec16u* bin0, vec16u *bin1, vec16u *bin2, vec16u *bin3, vec16u *bin4, vec16u *bin5,
                int32_t h, int32_t w, int32_t instride, int32_t outstride,
                int32_t wb, int32_t hb, int32_t nb, int32_t bin, int32_t nOrientations, int32_t softBin, int32_t full,
                vec32u *M0, vec32u*M1, vec32u*O0, vec32u *O1);


void triangleFilt1_c_row(vec16u* in, vec16u* out, int32_t h, int32_t w, int32_t instride, int32_t outstride, vec32u *T, uint32_t p, int32_t pad);


void triangleFilt1_c_row_norm(vec16u* in, vec16u* out, int32_t h, int32_t w, int32_t instride, int32_t outstride, vec32u *T, uint32_t p, int32_t pad, uint32_t normconst);


void triangleFilter(vec16u* in, int32_t instride, vec32s* temp_out, int32_t tempOutstride, vec16u* out, int32_t outstride, int32_t h, int32_t w, int32_t radius,
                    vec32s *T_lastline_buf,  vec32s *U_lastline_buf, vec32u firstchunk, vec32u lastchunk);

void apu_bi_pyramid_hori(vec16u* in, vec16u* out, int32_t instride, int32_t outstride, int32_t chunkWidth, int32_t chunkHeight, int scaleRatio, int shift);

void apu_bi_pyramid_verti(vec16u* in, vec16u* out, int32_t instride, int32_t outstride, int32_t chunkWidth, int32_t chunkHeight, int scaleRatio, int shift);

void apu_bi_pyramid_pass(vec16u* in, vec16u* out, int32_t instride, int32_t outstride, int32_t chunkWidth, int32_t chunkHeight);

void apu_aggcf_rgb2luv(
   vec32s *rgbImgIn,
   vec16u *lImgOut,
   vec16u *uImgOut,
   vec16u *vImgOut,
   int16u strideIn,
   int16u strideOut,
   int16u chunkWidth,
   int16u chunkHeight,
   vec16u *luv_lut
   );


#endif /* AGGCFAPU_H */

#endif 
