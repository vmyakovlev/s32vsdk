/*******************************************************************************
 *
 * NXP Confidential Proprietary
 *
 * Copyright (c) 2014-2017 NXP
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
 *  @file hough_apu.cpp
 *  @brief Implementation of Hough Transform kernels
 ******************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION
/*******************************************************************************
 *  \brief Returns x offset per CU
 ******************************************************************************/
vec16s apu_hough_get_offset_x(int aImageCenterX, int aBlockCols, int aCuCount)
{
  vec16s offsetX = 0;

  for(int i = 0; i < aCuCount; ++i)
  {
    offsetX = vput(offsetX, i * aBlockCols - aImageCenterX, i);
  }

  return offsetX;
}

/*******************************************************************************
 *  \brief Returns the y offset
 ******************************************************************************/
int apu_hough_get_offset_y(int aImageCenterY)
{
  return -aImageCenterY;
}

/*******************************************************************************
 *  \brief Returns the CuId in CMEM
 ******************************************************************************/
vec16s apu_hough_get_cu_index(int aIndexStart, int aCuCount)
{
  vec16s cuIndex = 0;

  for(int i = 0; i < aCuCount; ++i)
  {
    cuIndex = vput(cuIndex, aIndexStart + i, i);
  }

  return cuIndex;
}

/*******************************************************************************
 *  \brief Extract edge pixels from chunk and store in DMEM
 ******************************************************************************/
vec16s apu_hough_pixel_threshold(vec16s *apPixelsX,
                                 vec16s *apPixelsY,
                                 vec16s aOffsetX,
                                 int16s aOffsetY,
                                 const vec08u *apImage,
                                 int aStride,
                                 int aCols,
                                 int aRows,
                                 int aThreshold)
{
  // Threshold the apImage
  vec16s vPixelCount = 0;

  for(int y = aOffsetY, yEnd = y + aRows; y < yEnd; y++, apImage += aStride)
  {
    for(int c = 0; c < aCols; ++c)
    {
      vif(apImage[c] > aThreshold)
      {
        vec16s x = aOffsetX + c;
        vstore(apPixelsX, vPixelCount, x);
        vstore(apPixelsY, vPixelCount, y);
        ++vPixelCount;
      }
      vendif
    }
  }

  return vPixelCount;
}

/*******************************************************************************
 *  \brief Serializes a set of pixel coordinates
 *  \returns Pixel count in DMEM.
 ******************************************************************************/
int apu_hough_scalarize_pixels(int16s *apSclPixelsX,
                               int16s *apSclPixelsY,
                               const vec16s *apPixelsX,
                               const vec16s *apPixelsY,
                               vec16s aPixelCount,
                               int aCuCount)
{
  // Scalarize the thresholded pixels
  int sPixelCount = 0;

  for(int c = 0; c < aCuCount; ++c)
  {
    int cuPixelCount = vget(aPixelCount, c);

    for(int i = 0; i < cuPixelCount; ++i)
    {
      apSclPixelsX[i] = vget(apPixelsX[i], c);
      apSclPixelsY[i] = vget(apPixelsY[i], c);
    }

    apSclPixelsX += cuPixelCount;
    apSclPixelsY += cuPixelCount;
    sPixelCount  += cuPixelCount;
  }

  return sPixelCount;
}

/*******************************************************************************
 *  \brief Accumulate the Hough Transform (rho x theta space)
 ******************************************************************************/
void apu_hough_accumulate(vec16u *apAccumulator,
                          const int16s *apPixelsX,
                          const int16s *apPixelsY,
                          int aPixelCount,
                          vec16s aTabCos,
                          vec16s aTabSin,
                          int aRhoOffset)
{
  const int tabQ        = 14;
  const int tabOne      = 1 << tabQ;
  const int tabHalf     = tabOne >> 1;
  const vec32s tabCos32 = (vec32s)aTabCos;
  const vec32s tabSin32 = (vec32s)aTabSin;

  for(int i = 0; i < aPixelCount; i++)
  {
    vec32s r32 = apPixelsX[i] * tabCos32 + apPixelsY[i] * tabSin32;
    vec16s r;

    // Rounding
    vif(r32 < 0)
    {
      r = -(vec16s)((tabHalf - r32) >> tabQ);
    }
    velse
    {
      r = (vec16s)((r32 + tabHalf) >> tabQ);
    }
    vendif

      r += aRhoOffset;

    // Fetch the bin and increase its value
    const vec16u val = vload(apAccumulator, r) + 1;
    vstore(apAccumulator, r, val);
  }
}

inline vec32u getLine(vec16u aAcc, int aRhoIndex, vec16s aThetaIndex)
{
  vec32u packedAccum = ((vec32u)aAcc) << 20;
  int32u packedRho   = ((int32u)aRhoIndex) << 8;
  vec32u packedTheta = (vec32u)aThetaIndex;

  return packedAccum | packedRho | packedTheta;
}

/*******************************************************************************
 *  \brief Store the lines for which the acc > threshold using NMS
 *  \returns Line count in CMEM.
 ******************************************************************************/
vec16s apu_hough_get_lines(vec32u *apLines,
                           const vec16u *apAccm,
                           vec16s aThetaIndex,
                           int aRhoCount,
                           int aThetaCount,
                           int aThreshold,
                           int aMaxLinesPerCU,
                           int aNonMaxSupFlag,
                           int aCuCount)
{
  const vec16s maxLineCount = aMaxLinesPerCU;
  vec16s lineCount          = 0;
  int cuOffset;

  switch(aNonMaxSupFlag)
  {
    case 0:
    {
      for(int r = 0; r < aRhoCount; ++r)
      {
        vec16u a = apAccm[r];

        // Above threshold and enough CMEM ?
        vif((a > aThreshold) && (lineCount < maxLineCount))
        {
          vstore(apLines, lineCount, getLine(a, r, aThetaIndex));
          ++lineCount;
        }
        vendif
      }

      cuOffset = 0;
      break;
    }

    case 1:
    {
      for(int r = 0; r < aRhoCount; ++r)
      {
        vec16u a = apAccm[r];

        vif((a > aThreshold) && /* Above Threshold ? */
            (a > apAccm[r - 1]) && /* Compare with previous rho value*/
            (a >= apAccm[r + 1]) && /* Compare with next rho value */
            (lineCount < maxLineCount)) /*! Is CMEM enough for a new line ? */
        {
          vstore(apLines, lineCount, getLine(a, r, aThetaIndex));
          ++lineCount;
        }
        vendif
      }

      cuOffset = 0;
      break;
    }

    case 2:
    {
      for(int r = 0; r < aRhoCount; ++r)
      {
        vec16u a = apAccm[r];

        vif(a > aThreshold && /* Above Threshold ? */
            a > vmsr(a) && /* Compare with previous angle neighbour */
            a >= vmsl(a) && /* Compare with next angle neighbour */
            lineCount < maxLineCount) /*! Enough CMEM remaining for new line ? */
        {
          vstore(apLines, lineCount, getLine(a, r, aThetaIndex));
          ++lineCount;
        }
        vendif
      }

      // Suppress results in first/last CU because they have 1 neighbouring angle.
      lineCount = vput(lineCount, 0, 0);
      cuOffset  = 1;
      //lineCount = vput(lineCount, 0, aCuCount-1);
    }
    break;

    default:
    {
      for(int r = 0; r < aRhoCount; ++r)
      {
        vec16u a = apAccm[r];

        vif(a > aThreshold && /* Above Threshold ? */
            a > apAccm[r - 1] && /* Compare with previous rho value*/
            a >= apAccm[r + 1] && /* Compare with next rho value */
            a > vmsr(a) && /* Compare with previous angle neighbour */
            a >= vmsl(a) && /* Compare with next angle neighbour */
            lineCount < maxLineCount) /*! Enough CMEM remaining for new line ? */
        {
          vstore(apLines, lineCount, getLine(a, r, aThetaIndex));
          ++lineCount;
        }
        vendif
      }

      // Suppress results in the first and last CU since
      // they only have one neighbouring angle.
      lineCount = vput(lineCount, 0, 0);
      cuOffset  = 1;

      //lineCount = vput(lineCount, 0, aCuCount-1);
      // Suppress results in the last CU that is used.
      // This will be different from aCuCount,
      // when the angles to detect do not fill all the CUs
      //lineCount = vput(lineCount, 0, cuEnd-1);
    }
    break;
  }

  // Suppress results from unused CUs
  for(int i = aThetaCount + cuOffset; i < aCuCount; ++i)
  {
    lineCount = vput(lineCount, 0, i);
  }

  return lineCount;
}

#endif /* ACF_KERNEL_IMPLEMENTATION */