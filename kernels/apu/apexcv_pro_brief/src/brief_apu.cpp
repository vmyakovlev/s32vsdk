/*******************************************************************************
 *
 * NXP Confidential Proprietary
 *
 * Copyright 2014-2017 NXP
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
 ******************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION
#include "brief_apu.h"

volatile vec08u gSmplStorage[MAX_SAMPLES];

vec08u GaussianFilter9x9(vec08u *aImg, int aStr)
{
  // Coeffs
  /*
     25	60	112	 163	185	  163	112	60	25
     60	144	269	 391	443	  391	269	144	60
     112	269	502	 731	828	  731	502	269	112
     163	391	731	1064	1205	1064	731	391	163
     185	443	828	1205	1366	1205	828	443	185
     163	391	731	1064	1205	1064	731	391	163
     112	269	502  731	828	  731	502	269	112
     60	144	269	 391	443	  391	269	144	60
     25	60	112	 163	185	  163	112	60	25
   */

  vec32u out;

  vec08u *line0 = &aImg[-4 * aStr - 4];
  vec08u *line1 = &aImg[-3 * aStr - 4];
  vec08u *line2 = &aImg[-2 * aStr - 4];
  vec08u *line3 = &aImg[-1 * aStr - 4];
  vec08u *line4 = &aImg[-4];
  vec08u *line5 = &aImg[-1 * aStr - 4];
  vec08u *line6 = &aImg[-2 * aStr - 4];
  vec08u *line7 = &aImg[-3 * aStr - 4];
  vec08u *line8 = &aImg[-4 * aStr - 4];

  // Accmulate lines
  out = (vec32u)(*line0++) * 25 +
        (vec32u)(*line0++) * 60 +
        (vec32u)(*line0++) * 112 +
        (vec32u)(*line0++) * 163 +
        (vec32u)(*line0++) * 185 +
        (vec32u)(*line0++) * 163 +
        (vec32u)(*line0++) * 112 +
        (vec32u)(*line0++) * 60 +
        (vec32u)(*line0++) * 25;

  out += (vec32u)(*line1++) * 60 +
         (vec32u)(*line1++) * 144 +
         (vec32u)(*line1++) * 269 +
         (vec32u)(*line1++) * 391 +
         (vec32u)(*line1++) * 443 +
         (vec32u)(*line1++) * 391 +
         (vec32u)(*line1++) * 269 +
         (vec32u)(*line1++) * 144 +
         (vec32u)(*line1++) * 60;

  out += (vec32u)(*line2++) * 112 +
         (vec32u)(*line2++) * 269 +
         (vec32u)(*line2++) * 502 +
         (vec32u)(*line2++) * 731 +
         (vec32u)(*line2++) * 828 +
         (vec32u)(*line2++) * 731 +
         (vec32u)(*line2++) * 502 +
         (vec32u)(*line2++) * 269 +
         (vec32u)(*line2++) * 112;

  out += (vec32u)(*line3++) * 163 +
         (vec32u)(*line3++) * 391 +
         (vec32u)(*line3++) * 731 +
         (vec32u)(*line3++) * 1064 +
         (vec32u)(*line3++) * 1205 +
         (vec32u)(*line3++) * 1064 +
         (vec32u)(*line3++) * 731 +
         (vec32u)(*line3++) * 391 +
         (vec32u)(*line3++) * 163;

  out += (vec32u)(*line4++) * 185 +
         (vec32u)(*line4++) * 443 +
         (vec32u)(*line4++) * 828 +
         (vec32u)(*line4++) * 1205 +
         (vec32u)(*line4++) * 1366 +
         (vec32u)(*line4++) * 1205 +
         (vec32u)(*line4++) * 828 +
         (vec32u)(*line4++) * 443 +
         (vec32u)(*line4++) * 185;

  out += (vec32u)(*line5++) * 163 +
         (vec32u)(*line5++) * 391 +
         (vec32u)(*line5++) * 731 +
         (vec32u)(*line5++) * 1064 +
         (vec32u)(*line5++) * 1205 +
         (vec32u)(*line5++) * 1064 +
         (vec32u)(*line5++) * 731 +
         (vec32u)(*line5++) * 391 +
         (vec32u)(*line5++) * 163;

  out += (vec32u)(*line6++) * 112 +
         (vec32u)(*line6++) * 269 +
         (vec32u)(*line6++) * 502 +
         (vec32u)(*line6++) * 731 +
         (vec32u)(*line6++) * 828 +
         (vec32u)(*line6++) * 731 +
         (vec32u)(*line6++) * 502 +
         (vec32u)(*line6++) * 269 +
         (vec32u)(*line6++) * 112;

  out += (vec32u)(*line7++) * 60 +
         (vec32u)(*line7++) * 144 +
         (vec32u)(*line7++) * 269 +
         (vec32u)(*line7++) * 391 +
         (vec32u)(*line7++) * 443 +
         (vec32u)(*line7++) * 391 +
         (vec32u)(*line7++) * 269 +
         (vec32u)(*line7++) * 144 +
         (vec32u)(*line7++) * 60;

  out += (vec32u)(*line8++) * 25 +
         (vec32u)(*line8++) * 60 +
         (vec32u)(*line8++) * 112 +
         (vec32u)(*line8++) * 163 +
         (vec32u)(*line8++) * 185 +
         (vec32u)(*line8++) * 163 +
         (vec32u)(*line8++) * 112 +
         (vec32u)(*line8++) * 60 +
         (vec32u)(*line8++) * 25;

  out = out >> 15;

  return (vec08u)out;
}

vec08u BoxFilter9x9(vec08u *aImg, int aStr)
{
  // Compute the BOX filter
  vec16u out;

  vec08u *line0 = &aImg[-4 * aStr - 4];
  vec08u *line1 = &aImg[-3 * aStr - 4];
  vec08u *line2 = &aImg[-2 * aStr - 4];
  vec08u *line3 = &aImg[-1 * aStr - 4];
  vec08u *line4 = &aImg[-4];
  vec08u *line5 = &aImg[-1 * aStr - 4];
  vec08u *line6 = &aImg[-2 * aStr - 4];
  vec08u *line7 = &aImg[-3 * aStr - 4];
  vec08u *line8 = &aImg[-4 * aStr - 4];

  // Accmulate lines
  out = (vec16u)(*line0++) +
        (vec16u)(*line0++) +
        (vec16u)(*line0++) +
        (vec16u)(*line0++) +
        (vec16u)(*line0++) +
        (vec16u)(*line0++) +
        (vec16u)(*line0++) +
        (vec16u)(*line0++) +
        (vec16u)(*line0++);

  out += (vec16u)(*line1++) +
         (vec16u)(*line1++) +
         (vec16u)(*line1++) +
         (vec16u)(*line1++) +
         (vec16u)(*line1++) +
         (vec16u)(*line1++) +
         (vec16u)(*line1++) +
         (vec16u)(*line1++) +
         (vec16u)(*line1++);

  out += (vec16u)(*line2++) +
         (vec16u)(*line2++) +
         (vec16u)(*line2++) +
         (vec16u)(*line2++) +
         (vec16u)(*line2++) +
         (vec16u)(*line2++) +
         (vec16u)(*line2++) +
         (vec16u)(*line2++) +
         (vec16u)(*line2++);

  out += (vec16u)(*line3++) +
         (vec16u)(*line3++) +
         (vec16u)(*line3++) +
         (vec16u)(*line3++) +
         (vec16u)(*line3++) +
         (vec16u)(*line3++) +
         (vec16u)(*line3++) +
         (vec16u)(*line3++) +
         (vec16u)(*line3++);

  out += (vec16u)(*line4++) +
         (vec16u)(*line4++) +
         (vec16u)(*line4++) +
         (vec16u)(*line4++) +
         (vec16u)(*line4++) +
         (vec16u)(*line4++) +
         (vec16u)(*line4++) +
         (vec16u)(*line4++) +
         (vec16u)(*line4++);

  out += (vec16u)(*line5++) +
         (vec16u)(*line5++) +
         (vec16u)(*line5++) +
         (vec16u)(*line5++) +
         (vec16u)(*line5++) +
         (vec16u)(*line5++) +
         (vec16u)(*line5++) +
         (vec16u)(*line5++) +
         (vec16u)(*line5++);

  out += (vec16u)(*line6++) +
         (vec16u)(*line6++) +
         (vec16u)(*line6++) +
         (vec16u)(*line6++) +
         (vec16u)(*line6++) +
         (vec16u)(*line6++) +
         (vec16u)(*line6++) +
         (vec16u)(*line6++) +
         (vec16u)(*line6++);

  out += (vec16u)(*line7++) +
         (vec16u)(*line7++) +
         (vec16u)(*line7++) +
         (vec16u)(*line7++) +
         (vec16u)(*line7++) +
         (vec16u)(*line7++) +
         (vec16u)(*line7++) +
         (vec16u)(*line7++) +
         (vec16u)(*line7++);

  out += (vec16u)(*line8++) +
         (vec16u)(*line8++) +
         (vec16u)(*line8++) +
         (vec16u)(*line8++) +
         (vec16u)(*line8++) +
         (vec16u)(*line8++) +
         (vec16u)(*line8++) +
         (vec16u)(*line8++) +
         (vec16u)(*line8++);

  // 9 x 9 = 81
  vec32u div81 = (((vec32u)out) * 405) >> 15;

  return (vec08u) div81;
}


void unpackAndFilter(vec08u *apIn, int aStr, char aFilterType, vec08u *apOut, unsigned int *apPacketRegion, int aNrOfPackets)
{
  unsigned int data;
  unsigned int x;
  unsigned int y;
  unsigned int isFirst;
  unsigned int bit;
  unsigned int offset;
  const unsigned int smplPerComp = 2;

  for(int idx = 0; idx < aNrOfPackets; idx++)
  {
    // Decode data packed
    data    = apPacketRegion[idx];
    x       = (data >> 24) & 0xFF;
    y       = (data >> 16) & 0xFF;
    isFirst = (data >> 15) & 0x01;
    bit     = data & 0xFFF;
    offset  = (isFirst ^ 0x01);

    switch(aFilterType)
    {
      case NO_FILTERING:
      {
        apOut[smplPerComp * bit + offset] = apIn[x + y * aStr];
      }
      break;

      case BOX_FILTERING:
      {
        apOut[smplPerComp * bit + offset] = BoxFilter9x9(&apIn[x + y * aStr], aStr);
      }
      break;

      case GAUSSIAN_FILTERING:
      {
        apOut[smplPerComp * bit + offset] = GaussianFilter9x9(&apIn[x + y * aStr], aStr);
      }
      break;
    }
  }

  return;
}

int apu_brief_compute_descriptor(vec08u *apIn,
                                 int aStr,
                                 char aFilterType,
                                 unsigned char *apState,
                                 unsigned int *apSmplPtrn,
                                 int aNrOfPacketsUpLeft,
                                 int aNrOfPacketsUpRight,
                                 int aNrOfPacketsLwLeft,
                                 int aNrOfPacketsLwRight,
                                 vec08u *apOut,
                                 int aDscrSzB)
{
  int idx;
  unsigned int *pPacketRegion;

  switch(*apState)
  {
    case UPPER_LEFT:
    {
      pPacketRegion = &apSmplPtrn[0];
      unpackAndFilter(apIn, aStr, aFilterType, (vec08u *)gSmplStorage, pPacketRegion, aNrOfPacketsUpLeft);
      *apState = UPPER_RIGHT;

      return NO_GO;
    }
    break;

    case UPPER_RIGHT:
    {
      pPacketRegion = &apSmplPtrn[aNrOfPacketsUpLeft];
      unpackAndFilter(apIn, aStr, aFilterType, (vec08u *)gSmplStorage, pPacketRegion, aNrOfPacketsUpRight);
      *apState = LOWER_LEFT;

      return NO_GO;
    }
    break;

    case LOWER_LEFT:
    {
      pPacketRegion = &apSmplPtrn[aNrOfPacketsUpLeft + aNrOfPacketsUpRight];
      unpackAndFilter(apIn, aStr, aFilterType, (vec08u *)gSmplStorage, pPacketRegion, aNrOfPacketsLwLeft);
      *apState = LOWER_RIGHT;

      return NO_GO;
    }
    break;

    case LOWER_RIGHT:
    {
      pPacketRegion = &apSmplPtrn[aNrOfPacketsUpLeft + aNrOfPacketsUpRight + aNrOfPacketsLwLeft];
      unpackAndFilter(apIn, aStr, aFilterType, (vec08u *)gSmplStorage, pPacketRegion, aNrOfPacketsLwRight);

      // Temporaty storage for the output
      vec16s out;
      vec08u left;
      vec08u right;
      const int jump = 16;

      for(int byte = 0; byte < aDscrSzB; byte++)
      {
        left  = gSmplStorage[jump * byte + 14];
        right = gSmplStorage[jump * byte + 15];
        out   = (vec16s)(left < right);

        left  = gSmplStorage[jump * byte + 12];
        right = gSmplStorage[jump * byte + 13];
        out   = vslc(out, (left < right));

        left  = gSmplStorage[jump * byte + 10];
        right = gSmplStorage[jump * byte + 11];
        out   = vslc(out, (left < right));

        left  = gSmplStorage[jump * byte + 8];
        right = gSmplStorage[jump * byte + 9];
        out   = vslc(out, (left < right));

        left  = gSmplStorage[jump * byte + 6];
        right = gSmplStorage[jump * byte + 7];
        out   = vslc(out, (left < right));

        left  = gSmplStorage[jump * byte + 4];
        right = gSmplStorage[jump * byte + 5];
        out   = vslc(out, (left < right));

        left  = gSmplStorage[jump * byte + 2];
        right = gSmplStorage[jump * byte + 3];
        out   = vslc(out, (left < right));

        left  = gSmplStorage[jump * byte + 0];
        right = gSmplStorage[jump * byte + 1];
        out   = vslc(out, (left < right));

        // Write the descriptor
        *apOut++ = (vec08u)out;
      }

      *apState = UPPER_LEFT;

      return GO;
    }
    break;

    default:

      return ERROR;
  }

  return ERROR;
}

#endif // ACF_KERNEL_IMPLEMENTATION
