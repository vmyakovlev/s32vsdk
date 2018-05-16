/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
* Copyright (c) 2017 NXP
*  All Rights Reserved
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

/**
* \file haar_cascade_apu.cpp
* \brief Haar cascade implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/


#ifdef ACF_KERNEL_IMPLEMENTATION

#include "haar_cascade_apu.h"

inline vec32u isqrt32(vec32u x)
{
  vec32u root, remainder, place;

  root = 0;
  remainder = x;
  place = 0x40000000; // OR place = 0x4000; OR place = 0x40; - respectively  

  vbool pgr = place > remainder;
  while (vany(pgr))
  {
    vif(pgr)
    {
      place = place >> 2;
      pgr = place > remainder;
    }
    vendif;
  }
  vbool pnz = place != 0;
  while (vany(pnz))
  {
    vif(pnz)
    {
      vif(remainder >= root + place)
      {
        remainder = remainder - root - place;
        root = root + (place << 1);
      }
      vendif;

      root = root >> 1;
      place = place >> 2;

      pnz = place != 0;
    }
    vendif;
  }
  return root;
}

inline vec32u getFromII(const vec32u* in, int shift, int index, int yOffset)
{
  vec32u ret = in[yOffset + index];
  for (int i = 0; i < shift; ++i)
  {
    ret = vmsl(ret);
  }
  return ret;
}

struct haarRect
{
  int x, y;
  int width, height;
  int32s weight;
};

//returns rectangle count
int inline decodeHaarRects(haarRect *haarRects, const APEX_HaarCascadeFeature& feature, int32u invWindowAreaScalar)
{
  int fX = (feature.x5y5w5_1 & 0xF800) >> (5 + 5 + 1);
  int fY = (feature.x5y5w5_1 & 0x07C0) >> (5 + 1);
  int fWidth = (feature.x5y5w5_1 & 0x003E) >> (1);
  int fHeight = (feature.h5type3) >> (3);
  unsigned int type = feature.h5type3 & 0x07;

  haarRects[0].x = fX;
  haarRects[0].y = fY;
  haarRects[0].width = fWidth;
  haarRects[0].height = fHeight;
  haarRects[0].weight = -int32s(invWindowAreaScalar);

  int nRects = 2;

  const int thirds[] =
  { 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7 };

  //lines / 4 rectangles
  if (type & 0x4)
  {
    haarRects[1].weight = int32s(invWindowAreaScalar << 1);
    //4 rectangles
    if (type & 0x2)
    {
      haarRects[2].weight = haarRects[1].weight;
      nRects = 3;
      int halfWidth = fWidth >> 1;
      int halfHeight = fHeight >> 1;
      haarRects[1].width = halfWidth;
      haarRects[1].height = halfHeight;
      haarRects[1].y = fY;
      haarRects[2].width = halfWidth;
      haarRects[2].height = halfHeight;
      haarRects[2].y = fY + halfHeight;
      if (type & 0x1)
      {
        // /
        //
        haarRects[1].x = fX + halfWidth;
        haarRects[2].x = fX;
      }
      else
      {
        // \
                //
        haarRects[1].x = fX;
        haarRects[2].x = fX + halfWidth;
      }
    }
    //lines
    else
    {
      haarRects[1].weight = int32s(invWindowAreaScalar * 3);
      //vertical
      if (type & 0x1)
      {
        //int widthThird = fWidth / 3;
        int widthThird = thirds[fWidth];
        haarRects[1].x = fX + widthThird;
        haarRects[1].y = fY;
        haarRects[1].width = widthThird;
        haarRects[1].height = fHeight;
      }
      //horizontal
      else
      {
        //int heightThird = fHeight / 3;
        int heightThird = thirds[fHeight];
        haarRects[1].x = fX;
        haarRects[1].y = fY + heightThird;
        haarRects[1].width = fWidth;
        haarRects[1].height = heightThird;
      }
    }
  }
  //edges
  else
  {
    haarRects[1].weight = int32s(invWindowAreaScalar << 1);
    //vertical
    if (type & 0x2)
    {
      int halfWidth = fWidth >> 1;
      haarRects[1].y = fY;
      haarRects[1].width = halfWidth;
      haarRects[1].height = fHeight;
      if (type & 0x1)
      {
        //right positive
        haarRects[1].x = fX + halfWidth;
      }
      else
      {
        //left positive
        haarRects[1].x = fX;
      }
    }
    //horizontal
    else
    {
      int halfHeight = fHeight >> 1;
      haarRects[1].x = fX;
      haarRects[1].width = fWidth;
      haarRects[1].height = halfHeight;
      if (type & 0x1)
      {
        //bottom positive
        haarRects[1].y = fY + halfHeight;
      }
      else
      {
        //top positive
        haarRects[1].y = fY;
      }
    }
  }

  return nRects;
}

//TODO: separate strides for II and II2?
void haar_cascade(vec08u* apOut, vec32u* apInII, vec32u* apInII2, int /*aOutStride*/, int /*aInStride*/, int aTileWidth, int /*aTileHeight*/,
  int16u aLineIndex, vec32u* apWindowBuffer, vec32u* apWindowBuffer2,
  const APEX_HaarCascadeFeature* apcFeatures,
  int aStageCount, const APEX_HaarCascadeStage* apcStages,
  const int08u* apcXshifts, const int08u* apcXoffsets)
{
  int index = (int)aLineIndex;
  int bufferLineIndex = index & 0x1F;
  int realLineWidth = aTileWidth + 1;
  vec32u* line = &apWindowBuffer[bufferLineIndex * realLineWidth];
  vec32u* line2 = &apWindowBuffer2[bufferLineIndex * realLineWidth];
  //include the padding
  vec32u* inputLine = apInII - 1;
  vec32u* inputLine2 = apInII2 - 1;
  for (int i = 0; i < realLineWidth; ++i)
  {
    line[i] = inputLine[i];
    line2[i] = inputLine2[i];
  }
  //null the leftmost element
  line[0] = vput(line[0], (int32u)0, 0);
  line[1] = vput(line[1], (int32u)0, 0);
  if (index < HAAR_WINDOW_SIZE)
  {
    for (int i = 0; i < aTileWidth; ++i)
    {
      apOut[i] = 0;
    }
    return;
  }

  int windowStartLine = (index + 32 - HAAR_WINDOW_SIZE) & 0x1F;
  int windowStartBase = windowStartLine * realLineWidth;

  const int windowAreaScalar = (HAAR_WINDOW_SIZE - 2) * (HAAR_WINDOW_SIZE - 2);
  
  int yWindowOffset = (windowStartLine + (HAAR_WINDOW_SIZE - 2) & 0x1F) * realLineWidth;

  haarRect haarRects[3];

  for (int x = 0; x < aTileWidth; ++x)
  {
    //compute the standard deviation

    //the line is already shifted, no need to use another -1
    int08u xR = int08u(x) + HAAR_WINDOW_SIZE - 1;
    //int08u xShift1 = int08u(xR) / int08u(aTileWidth);
    //int08u xIndex1 = int08u(xR) % int08u(aTileWidth);
    int08u xShift1 = apcXshifts[xR];
    int08u xIndex1 = apcXoffsets[xR];

    vec32u ii00 = apWindowBuffer[windowStartBase + x];
    vec32u ii11 = getFromII(apWindowBuffer, xShift1, xIndex1, yWindowOffset);
    vec32u ii01 = getFromII(apWindowBuffer, xShift1, xIndex1, windowStartBase);
    vec32u ii10 = apWindowBuffer[x + yWindowOffset];

    vec32u ii200 = apWindowBuffer2[windowStartBase + x];
    vec32u ii211 = getFromII(apWindowBuffer2, xShift1, xIndex1, yWindowOffset);
    vec32u ii201 = getFromII(apWindowBuffer2, xShift1, xIndex1, windowStartBase);
    vec32u ii210 = apWindowBuffer2[x + yWindowOffset];

    vec32u sum =
      +ii00
      + ii11
      - ii01
      - ii10;

    vec32u sumSqr =
      +ii200
      + ii211
      - ii201
      - ii210;

    /*vec32u mean = (sum * FEATURE_FIXED_POINT_MULTIPLIER) / windowArea;

    vec32u mos = (sumSqr / windowArea) * FEATURE_FIXED_POINT_MULTIPLIER;
    vec32u som = (mean >> FEATURE_FRACTIONAL_BITS) * mean;
    //TODO: possible variance underflow?
    vec32u variance = mos - som;*/

    vec32u mean = (sum * HAAR_WINDOW_AREA_COEFFICIENT);

    vec32u mos = (sumSqr)* HAAR_WINDOW_AREA_COEFFICIENT;
    vec32u som = (mean >> FEATURE_FRACTIONAL_BITS) * mean;
    //TODO: possible variance underflow?
    vec32u variance = mos - som;

    vec32u standardDeviation = isqrt32(variance) * featureFixedCoefficientSqrt;

    /////////////////standard deviation ready
    vbool detected = true;
    //stages
    int featureIndex = 0;
    for (int stageIndex = 0; stageIndex < aStageCount; ++stageIndex)
    {
      APEX_HaarCascadeStage hcs = apcStages[stageIndex];

      STAGE_FIXED_POINT_TYPE stageSum = 0;

      //features
      for (int fi = 0; fi < hcs.featureCount; ++fi)
      {
        APEX_HaarCascadeFeature hcf = apcFeatures[featureIndex];

        vec32s featureSum = 0;

        //rectangles
        int rectCount = decodeHaarRects(haarRects, hcf, invWindowAreaScalar);
        for (int fri = 0; fri < rectCount; ++fri)
        {
          int rx = haarRects[fri].x;
          int ry = haarRects[fri].y;
          int rwidth = haarRects[fri].width;
          int rheight = haarRects[fri].height;

          //weightType2... original weights -1, 2 or 3
          vec32s weight = vec32s(haarRects[fri].weight);

          int xi0 = x + rx;
          int xi1 = xi0 + rwidth;
          //don't add y, we're already on the starting line
          int baseYPlus32 = windowStartLine + ry - 1 + 32;
          int yi0 = (baseYPlus32 & 0x1F) * realLineWidth;
          int yi1 = ((baseYPlus32 + rheight) & 0x1F) * realLineWidth;

          //int xShift0 = xi0 / aTileWidth;
          //int xIndex0 = xi0 % aTileWidth;
          //int xShift1 = xi1 / aTileWidth;
          //int xIndex1 = xi1 % aTileWidth;
          int xShift0 = apcXshifts[xi0];
          int xIndex0 = apcXoffsets[xi0];
          int xShift1 = apcXshifts[xi1];
          int xIndex1 = apcXoffsets[xi1];

          vec32u ii00 = getFromII(apWindowBuffer, xShift0, xIndex0, yi0);
          vec32u ii11 = getFromII(apWindowBuffer, xShift1, xIndex1, yi1);
          vec32u ii01 = getFromII(apWindowBuffer, xShift1, xIndex1, yi0);
          vec32u ii10 = getFromII(apWindowBuffer, xShift0, xIndex0, yi1);

          vec32u sum =
            +ii00
            + ii11
            - ii01
            - ii10;

          featureSum += weight * vec32s(sum);
        }

        //read the threshold as signed!
        vec32s threshold = vec32s(vec16s(hcf.threshold));
        vec16u leftVal = vec16u(hcf.leftVal);
        vec16u rightVal = vec16u(hcf.rightVal);

        vec32s adjustedThreshold = vec32s(threshold * standardDeviation) >> FEATURE_FRACTIONAL_BITS;

        vif(featureSum < adjustedThreshold)
        {
          stageSum += leftVal;
        }
        velse
        {
          stageSum += rightVal;
        }
        vendif;

        ++featureIndex;
      }

      vif(stageSum < STAGE_FIXED_POINT_TYPE(hcs.thresholdFixed))
      {
        detected = false;
      }
      vendif;

      if (vall(!detected))
      {
        break;
      }
    }

    apOut[x] = 0;
    vif(detected)
    {
      apOut[x] = 255;
    }
    vendif;
  }
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
