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
*  \file apu_color_conversion.c
*  \brief APU color conversion kernels.
***********************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include <stdio.h>
#include "color_conversion_apu.h"

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
                           int bw, int bh)
{
   vec08u r, g, b;
   for (int by = 0; by < bh; by++) chess_loop_range(1,)  {
      for (int bx = 0; bx < bw; bx++) chess_loop_range(1,)  {  
         r = (vec08u)((src[bx] >> 8) & 0xF8);
         g = (vec08u)((src[bx] >> 3) & 0xFC);
         b = (vec08u)((src[bx] & 0x1F) << 3);
         r |= (r >> 5);
         g |= (g >> 6);
         b |= (b >> 5);

         dst[bx] = (((vec32u)r << 16) | ((vec32u)g << 8) | (vec32u)b);
      }
      src += sstr;
      dst += dstr;
   }
}

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
                           int bw, int bh)
{
   vec08u r, g, b;
   for (int by = 0; by < bh; by++) chess_loop_range(1,)  {
      for (int bx = 0; bx < bw; bx++) chess_loop_range(1,)  {
         r = (vec08u)(src[bx] >> 19);
         g = (vec08u)((src[bx] >> 10) & 0x3F);
         b = (vec08u)((src[bx] >> 3) & 0x1F);

         dst[bx] = (((vec16u)r << 11) | ((vec16u)g << 5) | (vec16u)b);
      }
      src += sstr;
      dst += dstr;
   }
}

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
apu_csc_rgb2y (      vec08u* dst, int ds,
               const vec08u* src, int ss,
               int bw, int bh,
               int r2y, int g2y, int b2y)
{
   for (int y=0; y<bh; ++y)   chess_loop_range(1,)

   {
      const vec08u* s = (const vec08u*)src;
            vec08u* d = (      vec08u*)dst;

      const int rnd = 128;
      const int scl =   8;

      for (int x=0; x<bw; x+=4) chess_loop_range(1,)
      {
         // absolutly required by chunck of 3 (otherwise it is less efficient)
         const vec08u r0 = s[ 0];
         const vec08u g0 = s[ 1];
         const vec08u b0 = s[ 2]; s += 4;

         //
         const vec16u y0 =
               (   rnd
                + ((vec16u)r0 * r2y)
                + ((vec16u)g0 * g2y)
                + ((vec16u)b0 * b2y) ) >> scl;

         const vec08u r1 = s[ 0];
         const vec08u g1 = s[ 1];
         const vec08u b1 = s[ 2]; s += 4;

         const vec08u r2 = s[ 0];
         const vec08u g2 = s[ 1];
         const vec08u b2 = s[ 2]; s += 4;

         const vec08u r3 = s[ 0];
         const vec08u g3 = s[ 1];
         const vec08u b3 = s[ 2]; s += 4;

         const vec16u y1 =
               (   rnd
                + ((vec16u)r1 * r2y)
                + ((vec16u)g1 * g2y)
                + ((vec16u)b1 * b2y) ) >> scl;
         const vec16u y2 =
               (   rnd
                + ((vec16u)r2 * r2y)
                + ((vec16u)g2 * g2y)
                + ((vec16u)b2 * b2y) ) >> scl;
         const vec16u y3 =
               (   rnd
                + ((vec16u)r3 * r2y)
                + ((vec16u)g3 * g2y)
                + ((vec16u)b3 * b2y) ) >> scl;

         vec16u da = vsat(y0, 0, 255);
         vec16u db = vsat(y1, 0, 255);
         vec16u dc = vsat(y2, 0, 255);
         vec16u dd = vsat(y3, 0, 255);

         d[0] = (vec08u)da;
         d[1] = (vec08u)db;
         d[2] = (vec08u)dc;
         d[3] = (vec08u)dd; d+=4;
      }

      src += ss;
      dst += ds;
   }
}

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
                int bw, int bh)
{
   for (int y=0; y<bh; ++y)   chess_loop_range(1,)
   {
      const vec08u* s = (const vec08u*)src;
            vec08u* d = (      vec08u*)dst;

      for (int x=0; x<4*bw; x+=4) chess_loop_range(1,)
      {
         // must be vec16s
         const vec16u r = (vec16u)s[x+0];
         const vec16u g = (vec16u)s[x+1];
         const vec16u b = (vec16u)s[x+2];


         const vec16u y  = (
                           + (b *  29)
                           + (r *  77)
                           + (g * 150)
                           +     128
                     ) >> 8;

         const vec16u u  = (
                           + (r * -43)
                           + (g * -85)
                           + (b * 128)
                           +    128
                     ) >> 8;

         const vec16u v  = (
                           + (b * -21)
                           + (g *-107)
                           + (r * 128)
                           +    128
                     ) >> 8;

         d[x+0] = (vec08u)(y      );
         d[x+1] = (vec08u)(u + 128);
         d[x+2] = (vec08u)(v + 128);
         d[x+3] = 0;

      }

      src += ss;
      dst += ds;
   }
}

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
       int bw, int bh, int num_ch)
{
   for (int y = 0; y < bh; ++y) chess_loop_range(1,)
   {
      for(int x = 0; x < bw; ++x) chess_loop_range(1,)
      {
         dst[x] = src[num_ch * x];
      }
      src += sstr;
      dst += dstr;
   }
}


/*!
Insert channel from interleaved color formats.

\param dst    - [Output] Destination pointer
\param dstr   - [Input]  Destination stride
\param src    - [Input]  Source pointer
\param sstr   - [Input]  Source stride
\param bw     - [Input]  Block width
\param bh     - [Input]  Block height
\param aChannelIndex - [Input]  index of the channel to update
\param aChannelCount - [Input]  Number of U8 channels in each block

*/
void
apu_insert_channel_2ch(vec08u* dst, int dstr,
const vec08u* src0, int sstr0,
const vec08u* src1, int sstr1,
int bw, int bh, int aChannelIndex)
{
  if (0 == aChannelIndex)
  {
    for (int y = 0; y < bh; ++y) chess_loop_range(1, )
    {
      for (int x = 0; x < bw; ++x) chess_loop_range(1, )
      {
        dst[2 * x + 0] = src0[x];
        dst[2 * x + 1] = src1[2 * x + 1];
      }
      src0 += sstr0;
      src1 += sstr1;
      dst += dstr;
    }
  }
  else
  {
    for (int y = 0; y < bh; ++y) chess_loop_range(1, )
    {
      for (int x = 0; x < bw; ++x) chess_loop_range(1, )
      {
        dst[2 * x + 0] = src1[2 * x + 0];
        dst[2 * x + 1] = src0[x];
      }
      src0 += sstr0;
      src1 += sstr1;
      dst += dstr;
    }
  }
}

void
apu_insert_channel_3ch(vec08u* dst, int dstr,
const vec08u* src0, int sstr0,
const vec08u* src1, int sstr1,
int bw, int bh, int aChannelIndex)
{
  if (0 == aChannelIndex)
  {
    for (int y = 0; y < bh; ++y) chess_loop_range(1, )
    {
      for (int x = 0; x < bw; ++x) chess_loop_range(1, )
      {
        dst[3 * x + 0] = src0[x];
        dst[3 * x + 1] = src1[3 * x + 1];
        dst[3 * x + 2] = src1[3 * x + 2];
      }
      src0 += sstr0;
      src1 += sstr1;
      dst += dstr;
    }
  }
  else if (1 == aChannelIndex)
  {
    for (int y = 0; y < bh; ++y) chess_loop_range(1, )
    {
      for (int x = 0; x < bw; ++x) chess_loop_range(1, )
      {
        dst[3 * x + 0] = src1[3 * x + 0];
        dst[3 * x + 1] = src0[x];
        dst[3 * x + 2] = src1[3 * x + 2];
      }
      src0 += sstr0;
      src1 += sstr1;
      dst += dstr;
    }
  }
  else
  {
    for (int y = 0; y < bh; ++y) chess_loop_range(1, )
    {
      for (int x = 0; x < bw; ++x) chess_loop_range(1, )
      {
        dst[3 * x + 0] = src1[3 * x + 0];
        dst[3 * x + 1] = src1[3 * x + 1];
        dst[3 * x + 2] = src0[x];
      }
      src0 += sstr0;
      src1 += sstr1;
      dst += dstr;
    }
  }
}

void
apu_insert_channel_4ch(vec08u* dst, int dstr,
const vec08u* src0, int sstr0,
const vec08u* src1, int sstr1,
int bw, int bh, int aChannelIndex)
{
  if (0 == aChannelIndex)
  {
    for (int y = 0; y < bh; ++y) chess_loop_range(1, )
    {
      for (int x = 0; x < bw; ++x) chess_loop_range(1, )
      {
        dst[4 * x + 0] = src0[x];
        dst[4 * x + 1] = src1[4 * x + 1];
        dst[4 * x + 2] = src1[4 * x + 2];
        dst[4 * x + 3] = src1[4 * x + 3];
      }
      src0 += sstr0;
      src1 += sstr1;
      dst += dstr;
    }
  }
  else if (1 == aChannelIndex)
  {
    for (int y = 0; y < bh; ++y) chess_loop_range(1, )
    {
      for (int x = 0; x < bw; ++x) chess_loop_range(1, )
      {
        dst[4 * x + 0] = src1[4 * x + 0];
        dst[4 * x + 1] = src0[x];
        dst[4 * x + 2] = src1[4 * x + 2];
        dst[4 * x + 3] = src1[4 * x + 3];
      }
      src0 += sstr0;
      src1 += sstr1;
      dst += dstr;
    }
  }
  else if (2 == aChannelIndex)
  {
    for (int y = 0; y < bh; ++y) chess_loop_range(1, )
    {
      for (int x = 0; x < bw; ++x) chess_loop_range(1, )
      {
        dst[4 * x + 0] = src1[4 * x + 0];
        dst[4 * x + 1] = src1[4 * x + 1];
        dst[4 * x + 2] = src0[x];
        dst[4 * x + 3] = src1[4 * x + 3];
      }
      src0 += sstr0;
      src1 += sstr1;
      dst += dstr;
    }
  }
  else
  {
    for (int y = 0; y < bh; ++y) chess_loop_range(1, )
    {
      for (int x = 0; x < bw; ++x) chess_loop_range(1, )
      {
        dst[4 * x + 0] = src1[4 * x + 0];
        dst[4 * x + 1] = src1[4 * x + 1];
        dst[4 * x + 2] = src1[4 * x + 2];
        dst[4 * x + 3] = src0[x];
      }
      src0 += sstr0;
      src1 += sstr1;
      dst += dstr;
    }
  }
}



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
apu_split_channel_4ch(vec08u* dst0, int dstr0,
vec08u* dst1, int dstr1,
vec08u* dst2, int dstr2,
vec08u* dst3, int dstr3,
const vec08u* src, int sstr,
int bw, int bh)
{
  for (int y = 0; y < bh; ++y) chess_loop_range(1, )
  {
    for (int x = 0; x < bw; ++x) chess_loop_range(1, )
    {
      dst0[x] = src[x * 4 + 0];
      dst1[x] = src[x * 4 + 1];
      dst2[x] = src[x * 4 + 2];
      dst3[x] = src[x * 4 + 3];
    }
    src += sstr;
    dst0 += dstr0;
    dst1 += dstr1;
    dst2 += dstr2;
    dst3 += dstr3;
  }
}

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
apu_split_channel_3ch(vec08u* dst0, int dstr0,
vec08u* dst1, int dstr1,
vec08u* dst2, int dstr2,
const vec08u* src, int sstr,
int bw, int bh)
{
  for (int y = 0; y < bh; ++y) chess_loop_range(1, )
  {
    for (int x = 0; x < bw; ++x) chess_loop_range(1, )
    {
      dst0[x] = src[x * 3 + 0];
      dst1[x] = src[x * 3 + 1];
      dst2[x] = src[x * 3 + 2];
    }
    src += sstr;
    dst0 += dstr0;
    dst1 += dstr1;
    dst2 += dstr2;
  }
}

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
int bw, int bh)
{
  for (int y = 0; y < bh; ++y) chess_loop_range(1, )
  {
    for (int x = 0; x < bw; ++x) chess_loop_range(1, )
    {
      dst0[x] = src[x * 2 + 0];
      dst1[x] = src[x * 2 + 1];
    }
    src += sstr;
    dst0 += dstr0;
    dst1 += dstr1;
  }
}



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
apu_merge_channel_4ch(vec08u* dst0, int dstr0,
const vec08u* src0, int sstr0,
const vec08u* src1, int sstr1,
const vec08u* src2, int sstr2,
const vec08u* src3, int sstr3,
int bw, int bh)
{
  for (int y = 0; y < bh; ++y) chess_loop_range(1, )
  {
    for (int x = 0; x < bw; ++x) chess_loop_range(1, )
    {
      dst0[x * 4 + 0] = src0[x];
      dst0[x * 4 + 1] = src1[x];
      dst0[x * 4 + 2] = src2[x];
      dst0[x * 4 + 3] = src3[x];
    }
    src0 += sstr0;
    src1 += sstr1;
    src2 += sstr2;
    src3 += sstr3;
    dst0 += dstr0;
  }
}

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
apu_merge_channel_3ch(vec08u* dst0, int dstr0,
const vec08u* src0, int sstr0,
const vec08u* src1, int sstr1,
const vec08u* src2, int sstr2,
int bw, int bh)
{
  for (int y = 0; y < bh; ++y) chess_loop_range(1, )
  {
    for (int x = 0; x < bw; ++x) chess_loop_range(1, )
    {
      dst0[x * 3 + 0] = src0[x];
      dst0[x * 3 + 1] = src1[x];
      dst0[x * 3 + 2] = src2[x];
    }
    src0 += sstr0;
    src1 += sstr1;
    src2 += sstr2;
    dst0 += dstr0;
  }
}

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
int bw, int bh)
{
  for (int y = 0; y < bh; ++y) chess_loop_range(1, )
  {
    for (int x = 0; x < bw; ++x) chess_loop_range(1, )
    {
      dst0[x * 2 + 0] = src0[x];
      dst0[x * 2 + 1] = src1[x];
    }
    src0 += sstr0;
    src1 += sstr1;
    dst0 += dstr0;
  }
}



void apu_rgb888_to_grey(vec08u* apDest, const vec08u* apcSrc,
  int aBlockWidth, int aBlockHeight, int aOutputSpan, int aInputSpan)
{
  for (int16s y = 0; y < aBlockHeight; ++y)
  {
    int16s x3 = 0;
    for (int16s x = 0; x < aBlockWidth; ++x)
    {
      /* https://en.wikipedia.org/wiki/Grayscale#Converting_color_to_grayscale */
      /* (R*0.2126 + G*0.7152 + B*0.0722) scaled to power of 2: x 128        */
      /* gives (R*27 + G*92 + B*9)/128 */
      vec16u accum = 27 * vec16u(apcSrc[x3]) + 92 * vec16u(apcSrc[x3 + 1]) + 9 * vec16u(apcSrc[x3 + 2]);
      apDest[x] = vec08u(accum >> 7);
      x3 += 3;
    }

    apDest += aOutputSpan;
    apcSrc += aInputSpan;
  }
}

void apu_bgr888_to_grey(vec08u* apDest, const vec08u* apcSrc,
  int aBlockWidth, int aBlockHeight, int aOutputSpan, int aInputSpan)
{
  for (int16s y = 0; y < aBlockHeight; ++y)
  {
    int16s x3 = 0;
    for (int16s x = 0; x < aBlockWidth; ++x)
    {
      /* https://en.wikipedia.org/wiki/Grayscale#Converting_color_to_grayscale */
      /* (R*0.2126 + G*0.7152 + B*0.0722) scaled to power of 2: x 128        */
      /* gives (R*27 + G*92 + B*9)/128 */
      vec16u accum = 27 * vec16u(apcSrc[x3 + 2]) + 92 * vec16u(apcSrc[x3 + 1]) + 9 * vec16u(apcSrc[x3 + 0]);
      apDest[x] = vec08u(accum >> 7);
      x3 += 3;
    }

    apDest += aOutputSpan;
    apcSrc += aInputSpan;
  }
}

void apu_grey_to_rgb888(vec08u* apDest, const vec08u* apcSrc,
  int aBlockWidth, int aBlockHeight, int aOutputSpan, int aInputSpan)
{
  for (int16s y = 0; y < aBlockHeight; ++y)
  {
    int16s x3 = 0;
    for (int16s x = 0; x < aBlockWidth; ++x)
    {
      vec08u val = apcSrc[x];
      apDest[x3 + 0] = val;
      apDest[x3 + 1] = val;
      apDest[x3 + 2] = val;
      x3 += 3;
    }

    apDest += aOutputSpan;
    apcSrc += aInputSpan;
  }
}


#endif /* ACF_KERNEL_IMPLEMENTATION */



#ifdef ACF_KERNEL_IMPLEMENTATION
//
#include "color_conversion_apu.h"


namespace ht{/* Namespace ht */

/**
 * 3-channel to grayscale, 8-bit color conversion.
 */

void apu_csc_8UC3_Y(vec16s* restrict dst, int dstr, const vec08u* restrict src,
                     int sstr, int bw, int bh,
                     uint8_t kA, uint8_t kB, uint8_t kC, uint16_t kR){
    /* Pack & Pin constants */
    int a = (kB<<8) | (kA<<0);
    int b =            kC;
    int c =            kR;
    
    for (int y=0;y<bh;y++) chess_loop_range(1,){
        /* Pointers */
        const vec16u* s = (const vec16u*)src;
        vec16s*       d = (      vec16s*)dst;
        
        /* Variables */
        vec16u chess_storage(V0) v0;
        vec16u chess_storage(V1) v1;
        vec16u chess_storage(V2) v2;
        vec16u chess_storage(V3) v3;
        vec16u chess_storage(V4) v4;
        vec16u chess_storage(V5) v5;
        vec16u chess_storage(V6) v6;
        vec16u chess_storage(V7) v7;
        vec16u l6, l7;
        
        
        /* Loop preamble */
#define SAVE_TMPS do{t0=v0;t1=v1;t2=v2;t3=v3;t4=v4;t5=v5;t6=v6;t7=v7;}while(0)
        vec16u t0, t1, t2, t3, t4, t5, t6, t7;
        
        /*        |      ALU    |    Shift    |          Mul          |      Ld/St      */
        /* Peel -2 */
        {SAVE_TMPS;             ;             ;                       ; l6 = *s++;       }
        {SAVE_TMPS;             ;             ;                       ; l7 = *s++;       }
        {SAVE_TMPS;             ;             ;                       ;           v6=l6; }
        /* Peel -1 */
        {SAVE_TMPS;             ;             ; v1 = vmul_ulul(t6, a) ; l6 = *s++;v7=l7; }
        {SAVE_TMPS; v0 = t1 + c ;             ; v1 = vmul_uhuh(t6, a) ; l7 = *s++;       }
        {SAVE_TMPS; v0 = t0 + t1;             ; v1 = vmul_ulul(t7, b) ;           v6=l6; }
        /* Core loop */
        for (int x=0; x<bw; x++) chess_loop_range(1,){
            /*        |      ALU    |    Shift    |          Mul          |      Ld/St      */
            {SAVE_TMPS; v0 = t0 + t1;             ; v1 = vmul_ulul(t6, a) ; l6 = *s++;v7=l7; }
            {SAVE_TMPS; v0 = t1 + c ; v6 = t0 >> 8; v1 = vmul_uhuh(t6, a) ; l7 = *s++;       }
            {SAVE_TMPS; v0 = t0 + t1;             ; v1 = vmul_ulul(t7, b) ; *d++ = (vec16s)t6;v6=l6; }
        }
#undef SAVE_TMPS
        
        /* Pointer increment */
        src += sstr;
        dst += dstr;
    }
}


}/* Namespace ht */
#endif /* ACF_KERNEL_IMPLEMENTATION */
