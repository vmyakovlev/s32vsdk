/*******************************************************************************
 *
 * NXP Confidential Proprietary
 *
 * Copyright (c) 2016-2017 NXP
 * All Rights Reserved
 *
 *******************************************************************************
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
 ******************************************************************************/

/*******************************************************************************
 *  \file orb_apu.cpp
 *  \brief ACP kernel implementation
 ******************************************************************************/

#include "orb_apu.h"

#ifdef ACF_KERNEL_IMPLEMENTATION
// Lut used to index into a patch in a circular manner
// Max patch size is 25 !
static int8_t tapsForCircle[25][18] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 4, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 5, 5, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 6, 6, 5, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 7, 7, 6, 6, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 8, 8, 7, 7, 6, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 9, 9, 8, 8, 7, 7, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 10, 10, 10, 9, 9, 8, 7, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 11, 11, 11, 10, 10, 9, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 12, 12, 12, 11, 11, 10, 10, 9, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 13, 13, 13, 12, 12, 12, 11, 10, 9, 8, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 14, 14, 14, 13, 13, 13, 12, 11, 11, 10, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 15, 15, 15, 14, 14, 14, 13, 13, 12, 11, 10, 0, 0, 0, 0, 0, 0, 0 },
  { 16, 16, 16, 15, 15, 15, 14, 14, 13, 12, 12, 11, 0, 0, 0, 0, 0, 0 },
  { 17, 17, 17, 17, 16, 16, 15, 15, 14, 14, 13, 12, 11, 0, 0, 0, 0, 0 },
  { 18, 18, 18, 18, 17, 17, 17, 16, 16, 15, 14, 13, 12, 0, 0, 0, 0, 0 },
  { 19, 19, 19, 19, 18, 18, 18, 17, 17, 16, 15, 15, 14, 13, 0, 0, 0, 0 },
  { 20, 20, 20, 20, 19, 19, 19, 18, 18, 17, 17, 16, 15, 14, 13, 0, 0, 0 },
  { 21, 21, 21, 21, 20, 20, 20, 19, 19, 18, 18, 17, 16, 16, 15, 0, 0, 0 },
  { 22, 22, 22, 22, 21, 21, 21, 20, 20, 20, 19, 18, 18, 17, 16, 15, 0, 0 },
  { 23, 23, 23, 23, 22, 22, 22, 22, 21, 21, 20, 20, 19, 18, 17, 17, 15, 0 },
  { 24, 24, 24, 24, 23, 23, 23, 23, 22, 22, 21, 21, 20, 19, 19, 18, 17, 0 },
  { 25, 25, 25, 25, 24, 24, 24, 24, 23, 23, 22, 22, 21, 21, 20, 19, 18, 17 }
};

static int8_t gSinCosExp = 8;

inline static void sinFixedPoint256Levels(vec16s x, vec16s *magnitude)
{
  // The input parameter is inside [0, 255] mapped to [0, 360] degrees
  // The first quadrant approximation is sin(x) = 3/pi*x - 4/(pi^3) * (x ^ 3)
  // Moving to fixed point we have sin(x) = 244 * x - 33 * x^3;
  vec32s in;
  vec32s in_3;
  vec32s acc;
  vec16s apOut;
  vec16s sign = 1;

  // Moving the data into the first quadrant
  vif(x <= 128)
  {
    vif(x > 64)
    {
      x = 128 - x;
    }
    vendif
      sign = 1;
  }
  velse
  {
    vif(x <= 192)
    {
      x = x - 128;
    }
    velse
    {
      x = 256 - x;
    }
    vendif
      sign = -1;
  }
  vendif

  // Third order sinus approximation, max error is at abapOut ~1.2 degrees
    in = (vec32s)x;
  in_3       = in * in * in;
  acc        = (6 * in - ((in_3) >> 11));
  apOut      = ((vec16s)acc) * sign;
  *magnitude = apOut;
}

inline static void cosFixedPoint256Levels(vec16s x, vec16s *magnitude)
{
  // The well know information that cos(x) = sin(pi/2 - x)
  // Here in apOut code cos(x) = sin(64 - x)
  x = 64 - x;
  vif(x < 0)
  {
    x = x + 256;
  }
  vendif
    sinFixedPoint256Levels(x, magnitude);
}

inline static vec08u atan2FixedPoint256Levels(vec32s y, vec32s x)
{
  // Calculates fixed point atan2(y, x) and maps it from [0, 2*pi] to [0, 255]
  // Atan2(y, x) = atan(y/x) plus sign information !
  // The code forces the calculus in the first quadrant then adds the sign info
  // First quadrant aproximation of atan(x) = 0.1963 * x^3 - 0.9817 * x + 0.7854
  // Fixed point: atan(x) = (50 * x^3 - 251 * x + 201)
  // rad2Taps = round(128/pi)
  unsigned char a0       = 201;
  unsigned char a1       = 251;
  unsigned char a2       = 50;
  unsigned char rad2Taps = 41;

  vec16s apOut;
  vec16s offset;
  vec32s absY = (vec32s)vabs(y);
  vec32s acc;
  vec32s r;
  vec32s r3;

  // Helper local variables
  vec32s numerator, denominator;

  // Parameter self normalization
  vif(x >= 0)
  {
    // Quadrant offset
    offset      = 0;
    numerator   = (x - absY) * 256;
    denominator = (x + absY);
  }
  velse
  {
    // Quadrant offset
    offset      = 64;
    numerator   = (x + absY) * 256;
    denominator = (absY - x);
  }
  vendif

  // atan2(y, x) ~= atan(y/x) + quadrant information
    r = numerator / denominator;

  // atan(x) approximation
  r3  = ((r * r * r) >> 16);
  acc = (a0 - ((a1 * r) >> 8) + ((a2 * r3) >> 8));

  // Here we can work on 16 bits
  // Converting from radins to taps
  apOut = (vec16s)((((vec16s)acc) * rad2Taps) >> 8);

  // Add quadrant offset
  apOut = apOut + offset;

  // If we are in quadrant III or IV we need to negate the apOutput
  vif(y < 0)
  apOut = -apOut;
  vendif

  // Atan2 always apOutputs a value between 0-255
    vif(apOut < 0)
  apOut = apOut + 256;
  vendif

  // Checking special input values
  vif(x == 0)
  {
    vif(y > 0)
    // pi/2
    apOut = 64;
    velse
      vif(y < 0)
    // 3/2 * pi
    apOut = 192;
    velse
      apOut = 0;
    vendif
      vendif
  }

  vendif return (vec08u)apOut;
}

inline static vec08u ico_kernel(vec08u *restrict apCenter,
                                int aImgStr,
                                int8_t aRadiusMax)
{
  int u, v;
  int aRadius;

  // Clear the accumulators
  vec32s m01 = 0;
  vec32s m10 = 0;
  const vec08u *restrict north;
  const vec08u *restrict sapOuth;

  // Centroid moments: m01, m10
  // m01 = sum(sum(y*I(x, y)));
  // m10 = sum(x * sum(I(x, y)));
  for(u = -aRadiusMax; u <= aRadiusMax; u++)
  {
    vec16s data = (vec16s)((vec16u)apCenter[u]);
    vec16s mult = (vec16s)(u * data);
    m10 = m10 + (vec32s)mult;
  }

  for(v = 1; v <= aRadiusMax; v++)
  {
    vec32s acc = 0;
    aRadius = tapsForCircle[aRadiusMax - 1][v - 1];
    north   = (const vec08u *restrict)(apCenter + v * aImgStr);
    sapOuth = (const vec08u *restrict)(apCenter - v * aImgStr);

    for(u = -aRadius; u <= aRadius; u++)
    {
      vec16s n   = (vec16s)((vec16u)north[u]);
      vec16s s   = (vec16s)((vec16u)sapOuth[u]);
      vec16s dif = n - s;
      vec16s sum = n + s;
      acc = acc + (vec32s)dif;
      m10 = m10 + (vec32s)(u * sum);
    }

    m01 = m01 + v * acc;
  }

  // Angle of the centroid
  return atan2FixedPoint256Levels(m01, m10);
}

inline static vec08u rotateAndGetData(vec08u *apData,
                                      int aImgStr,
                                      int16_t aXPtrn,
                                      int16_t aYPtrn,
                                      vec16s aSinM,
                                      vec16s aCosM)
{
  // Rotate the indexes for the descriptor
  // R(fi) = [cos(fi), -sin(fi); sin(fi), cos(fi)]
  vec16s x     = (aXPtrn * aCosM - aYPtrn * aSinM) >> gSinCosExp;
  vec16s y     = (aXPtrn * aSinM + aYPtrn * aCosM) >> gSinCosExp;
  vec16s index = x + aImgStr * y;
  vec08u data  = (vec08u)vload(apData, index);

  return data;
}

inline void rbrief_kernel(vec08u *apCenter,
                          int aImgStr,
                          int8_t *apSmplPtrn,
                          vec08u *apOut,
                          vec08u apOrntn,
                          int aDscrSzB)
{
  int16_t x0;
  int16_t x1;
  int16_t y0;
  int16_t y1;
  vec16s aSinM;
  vec16s aCosM;
  vec08u data0;
  vec08u data1;

  // Clear output accumulator
  vec16s out = 0;

  // Calculate the rotation parameters
  sinFixedPoint256Levels((vec16s)((vec16u)apOrntn), &aSinM);
  cosFixedPoint256Levels((vec16s)((vec16u)apOrntn), &aCosM);

  // Generating the descriptor
  for(int byte = 0; byte < aDscrSzB; byte++)
  {
    // For each byte I need 8 comparisons
    // One comparison implies two set of points
    // Each point has <x,y>
    // This is why I multiply byte with 32....
    // vslc() is done directly on conditional registers

    // Bit 7
    y1    = apSmplPtrn[32 * byte + 31];
    x1    = apSmplPtrn[32 * byte + 30];
    y0    = apSmplPtrn[32 * byte + 29];
    x0    = apSmplPtrn[32 * byte + 28];
    data1 = rotateAndGetData(apCenter, aImgStr, x1, y1, aSinM, aCosM);
    data0 = rotateAndGetData(apCenter, aImgStr, x0, y0, aSinM, aCosM);
    out   = (vec16s)(data0 < data1);

    // Bit 6
    y1    = apSmplPtrn[32 * byte + 27];
    x1    = apSmplPtrn[32 * byte + 26];
    y0    = apSmplPtrn[32 * byte + 25];
    x0    = apSmplPtrn[32 * byte + 24];
    data1 = rotateAndGetData(apCenter, aImgStr, x1, y1, aSinM, aCosM);
    data0 = rotateAndGetData(apCenter, aImgStr, x0, y0, aSinM, aCosM);
    out   = vslc(out, (data0 < data1));

    // Bit 5
    y1    = apSmplPtrn[32 * byte + 23];
    x1    = apSmplPtrn[32 * byte + 22];
    y0    = apSmplPtrn[32 * byte + 21];
    x0    = apSmplPtrn[32 * byte + 20];
    data1 = rotateAndGetData(apCenter, aImgStr, x1, y1, aSinM, aCosM);
    data0 = rotateAndGetData(apCenter, aImgStr, x0, y0, aSinM, aCosM);
    out   = vslc(out, (data0 < data1));

    // Bit 4
    y1    = apSmplPtrn[32 * byte + 19];
    x1    = apSmplPtrn[32 * byte + 18];
    y0    = apSmplPtrn[32 * byte + 17];
    x0    = apSmplPtrn[32 * byte + 16];
    data1 = rotateAndGetData(apCenter, aImgStr, x1, y1, aSinM, aCosM);
    data0 = rotateAndGetData(apCenter, aImgStr, x0, y0, aSinM, aCosM);
    out   = vslc(out, (data0 < data1));

    // Bit 3
    y1    = apSmplPtrn[32 * byte + 15];
    x1    = apSmplPtrn[32 * byte + 14];
    y0    = apSmplPtrn[32 * byte + 13];
    x0    = apSmplPtrn[32 * byte + 12];
    data1 = rotateAndGetData(apCenter, aImgStr, x1, y1, aSinM, aCosM);
    data0 = rotateAndGetData(apCenter, aImgStr, x0, y0, aSinM, aCosM);
    out   = vslc(out, (data0 < data1));

    // Bit 2
    y1    = apSmplPtrn[32 * byte + 11];
    x1    = apSmplPtrn[32 * byte + 10];
    y0    = apSmplPtrn[32 * byte + 9];
    x0    = apSmplPtrn[32 * byte + 8];
    data1 = rotateAndGetData(apCenter, aImgStr, x1, y1, aSinM, aCosM);
    data0 = rotateAndGetData(apCenter, aImgStr, x0, y0, aSinM, aCosM);
    out   = vslc(out, (data0 < data1));

    // Bit 1
    y1    = apSmplPtrn[32 * byte + 7];
    x1    = apSmplPtrn[32 * byte + 6];
    y0    = apSmplPtrn[32 * byte + 5];
    x0    = apSmplPtrn[32 * byte + 4];
    data1 = rotateAndGetData(apCenter, aImgStr, x1, y1, aSinM, aCosM);
    data0 = rotateAndGetData(apCenter, aImgStr, x0, y0, aSinM, aCosM);
    out   = vslc(out, (data0 < data1));

    // Bit 0
    y1    = apSmplPtrn[32 * byte + 3];
    x1    = apSmplPtrn[32 * byte + 2];
    y0    = apSmplPtrn[32 * byte + 1];
    x0    = apSmplPtrn[32 * byte + 0];
    data1 = rotateAndGetData(apCenter, aImgStr, x1, y1, aSinM, aCosM);
    data0 = rotateAndGetData(apCenter, aImgStr, x0, y0, aSinM, aCosM);
    out   = vslc(out, (data0 < data1));

    // Write the data to output buffer
    *apOut++ = (vec08u)out;
  }
}

/****************************************************************************/
// API FUNCTIONS
/****************************************************************************/
void apu_orb_orientation(vec16u *apOut,
                         vec08u *apIn,
                         int aInStr,
                         int aPatchSz,
                         int aRadius)
{
  int halfPatchSize = aPatchSz >> 1;
  vec08u *apCenter  = (vec08u *)(apIn + halfPatchSize + aInStr * halfPatchSize);
  *apOut++ = (vec16u)ico_kernel(apCenter, aInStr, aRadius);
}

void apu_orb_rbrief(vec08u *apIn,
                    int aInStr,
                    vec16u *apOrntn,
                    int8_t *apSmplPtrn,
                    vec08u *apOut,
                    int8_t aDscrSzB,
                    int8_t aPatchSz)
{
  int halfPatchSize = (int)(aPatchSz >> 1);
  vec08u *pMid      = (vec08u *)(apIn + halfPatchSize + aInStr * halfPatchSize);
  rbrief_kernel(pMid, aInStr, apSmplPtrn, apOut, (vec08u) * apOrntn, aDscrSzB);
}

#endif /* ACF_KERNEL_IMPLEMENTATION */
