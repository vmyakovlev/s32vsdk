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
*  \file apu_color_conversion.h
*  \brief APU color conversion kernels.
***********************************************************************************/
#ifdef ACF_KERNEL_IMPLEMENTATION

#ifndef APUCOLORCONVERSIONIMPL_H
#define APUCOLORCONVERSIONIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif



/*!
   Converts 16 bit RGB565 to 24 bit RGB888

   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src   - [Input]  Source values block pointer
   \param sstr  - [Input]  Source values block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void 
apu_csc_rgb565_to_rgb888( vec32u* dst, int dstr,
                const vec16u* src, int sstr,
                           int bw, int bh);

/*!
   Converts 24 bit RGB888 to 16 bit RGB565

   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src   - [Input]  Source values block pointer
   \param sstr  - [Input]  Source values block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void
apu_csc_rgb888_to_rgb565( vec16u* dst, int dstr,
                    const vec32u* src, int sstr,
                               int bw, int bh);

/*!
   Calculate grayscale pixel from RGBX pixel.

   \param dst  - [Output] Destination block pointer   (Grayscale)
   \param ds   - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer        (RGBX)
   \param ss   - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
   \param r2y  - [Input]  Red to gray conversion factor
   \param g2y  - [Input]  Green to gray conversion factor
   \param b2y  - [Input]  Blue to gray conversion factor
 */
void
apu_csc_rgb2y( vec08u* dst, int ds,
               const vec08u* src, int ss,
               int bw, int bh,
               int r2y, int g2y, int b2y);

/*!
   Calculate YUVX pixel from RGBX pixel.

   References on formula and values used for conversion:
   http://www.itu.int/dms_pubrec/itu-r/rec/bt/R-REC-BT.601-7-201103-I!!PDF-E.pdf
   http://www.poynton.com/PDFs/ColorFAQ.pdf

   \param dst  - [Output] Destination block pointer  (YUVX)
   \param ds   - [Input]  Destination block span
   \param src  - [Input]  Source block pointer       (RGBX)
   \param ss   - [Input]  Source block span
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height

 */
void
apu_csc_rgb2yuv(      vec08u* dst, int ds,
                const vec08u* src, int ss,
                int bw, int bh);

/*!
   Extract channel from interleaved color formats.

   \param dst    - [Output] Destination pointer
   \param dstr   - [Input]  Destination stride
   \param src    - [Input]  Source pointer
   \param sstr   - [Input]  Source stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
   \param num_ch - [Input]  Number of U8 channels in each block

 */
void
apu_extract_channel(vec08u* dst, int dstr,
       const vec08u* src, int sstr,
       int bw, int bh, int num_ch);

/*!
Insert channel from interleaved color formats.

\param dst    - [Output] Destination pointer
\param dstr   - [Input]  Destination stride
\param src    - [Input]  Source pointer
\param sstr   - [Input]  Source stride
\param bw     - [Input]  Block width
\param bh     - [Input]  Block height
\param num_ch - [Input]  Number of U8 channels in each block

*/
void
apu_insert_channel_2ch(vec08u* dst, int dstr,
const vec08u* src0, int sstr0,
const vec08u* src1, int sstr1,
int bw, int bh, int num_ch);

void
apu_insert_channel_3ch(vec08u* dst, int dstr,
const vec08u* src0, int sstr0,
const vec08u* src1, int sstr1,
int bw, int bh, int num_ch);

void
apu_insert_channel_4ch(vec08u* dst, int dstr,
const vec08u* src0, int sstr0,
const vec08u* src1, int sstr1,
int bw, int bh, int num_ch);



/*!
Split channel

\param dst    - [Output] Destination pointer
\param dstr   - [Input]  Destination stride
\param src    - [Input]  Source pointer
\param sstr   - [Input]  Source stride
\param bw     - [Input]  Block width
\param bh     - [Input]  Block height
*/
void
apu_split_channel_2ch(vec08u* dst0, int dstr0,
vec08u* dst1, int dstr1,
const vec08u* src, int sstr,
int bw, int bh);

void
apu_split_channel_3ch(vec08u* dst0, int dstr0,
vec08u* dst1, int dstr1,
vec08u* dst2, int dstr2,
const vec08u* src, int sstr,
int bw, int bh);

void
apu_split_channel_4ch(vec08u* dst0, int dstr0,
vec08u* dst1, int dstr1,
vec08u* dst2, int dstr2,
vec08u* dst3, int dstr3,
const vec08u* src, int sstr,
int bw, int bh);




/*!
Merge channel

\param dst    - [Output] Destination pointer
\param dstr   - [Input]  Destination stride
\param src    - [Input]  Source pointer
\param sstr   - [Input]  Source stride
\param bw     - [Input]  Block width
\param bh     - [Input]  Block height
*/

void
apu_merge_channel_2ch(vec08u* dst0, int dstr0,
const vec08u* src0, int sstr0,
const vec08u* src1, int sstr1,
int bw, int bh);

void
apu_merge_channel_3ch(vec08u* dst0, int dstr0,
const vec08u* src0, int sstr0,
const vec08u* src1, int sstr1,
const vec08u* src2, int sstr2,
int bw, int bh);

void
apu_merge_channel_4ch(vec08u* dst0, int dstr0,
const vec08u* src0, int sstr0,
const vec08u* src1, int sstr1,
const vec08u* src2, int sstr2,
const vec08u* src3, int sstr3,
int bw, int bh);





void apu_rgb888_to_grey(vec08u* apDest, const vec08u* apcSrc,
  int aBlockWidth, int aBlockHeight, int aOutputSpan, int aInputSpan);

void apu_bgr888_to_grey(vec08u* apDest, const vec08u* apcSrc,
  int aBlockWidth, int aBlockHeight, int aOutputSpan, int aInputSpan);

void apu_grey_to_rgb888(vec08u* apDest, const vec08u* apcSrc,
  int aBlockWidth, int aBlockHeight, int aOutputSpan, int aInputSpan);

                
/* Namespace for Hand-Tuned functions */
namespace ht{
/*!
   3-channel to grayscale, 8-bit color conversion.
 
   The source image is laid out in the format
  
              | A0 B0 C0 X0 | A1 B1 C1 X1 | A2 B2 C2 X2 | ...
  
   with each pixel consisting of three 8-bit channels and an 8-bit padding.
   The output image's pixels are a linear function of the corresponding input pixel's
   channels:
 
              Y = kA*A + kB*B + kC*C + kR
  
   with kA, kB, kC any 8-bit unsigned value and kR any 16-bit unsigned value.
  
   For input
            - RGBX: kA=76, kB=150, kC=29, kR=127
            - BGRX: kA=29, kB=150, kC=76, kR=127

   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src   - [Input]  Source values block pointer
   \param sstr  - [Input]  Source values block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
   \param kA    - [Input]  Conversion Factor A
   \param kB    - [Input]  Conversion Factor B
   \param kC    - [Input]  Conversion Factor C
   \param kR    - [Input]  Conversion Factor R
 */
void apu_csc_8UC3_Y(vec16s* restrict dst, int dstr, const vec08u* restrict src,
                    int sstr, int bw, int bh,
                    uint8_t kA, uint8_t kB, uint8_t kC, uint16_t kR);
}

#endif /* APUCOLORCONVERSIONIMPL_H */
#endif /* ACF_KERNEL_IMPLEMENTATION */
