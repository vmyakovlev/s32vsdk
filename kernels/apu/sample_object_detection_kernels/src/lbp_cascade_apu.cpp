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
* \file lbp_cascade_apu.cpp
* \brief LBP cascade implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "lbp_cascade_apu.h"

inline vec32u getFromII(const vec32u* in, int shift, int index, int yOffset)
{
  vec32u ret = in[yOffset + index];
  for (int i = 0; i < shift; ++i)
  {
    ret = vmsl(ret);
  }
  return ret;
}

//TODO: separate strides for II and II2?
void lbp_cascade(vec08u* apOut, vec32u* apInII, int /*aOutStride*/, int /*aInStride*/, int aTileWidth, int /*aTileHeight*/,
  int16u aLineIndex, vec32u* apWindowBuffer,
  const APEX_lbpFeature* apcFeatures,
  int aStageCount, const APEX_lbpStage* apcStages,
  const int08u* apcXshifts, const int08u* apcXoffsets, int skipOdd)
{
  int index = (int)aLineIndex;
  int bufferLineIndex = index & 0x1F;
  int realLineWidth = aTileWidth + 1;
  vec32u* line = &apWindowBuffer[bufferLineIndex * realLineWidth];
  //include the padding
  vec32u* inputLine = apInII - 1;
  for (int i = 0; i < realLineWidth; ++i)
  {
    line[i] = inputLine[i];
  }
  //null the leftmost element
  line[0] = vput(line[0], (int32u)0, 0);
  int skipLine = int(skipOdd && (index & 0x1));
  if (index < LBP_WINDOW_SIZE || skipLine)
  {
    for (int i = 0; i < aTileWidth; ++i)
    {
      apOut[i] = 0;
    }
    return;
  }

  int windowStart = (index + 32 - LBP_WINDOW_SIZE) & 0x1F;

  int jump = 1 + skipOdd;
  for (int x = 0; x < aTileWidth; x += jump)
  {
    int featureIndex = 0;
    vbool detected = true;
    apOut[x] = 0;
    if (skipOdd && x != aTileWidth - 1)
    {
      apOut[x + 1] = 0;
    }

    for (int stageIndex = 0; stageIndex < aStageCount; ++stageIndex)
    {
      APEX_lbpStage lbps = apcStages[stageIndex];
      vec32s stageSum = 0;

      for (int fi = 0; fi < lbps.featureCount; ++fi)
      {
        APEX_lbpFeature lbpf = apcFeatures[featureIndex];

        //grid pattern values
        vec32s gpv[16];

        //int gridStartX = x + lbpf.x;
        //int gridStartY = y + lbpf.y;
        //the line is already shifted, no need to use another -1
        int gridStartX = x + lbpf.x;
        int gridStartY = lbpf.y - 1;
        int gridOffsetX = 0;
        int gridOffsetY = 0;

        int wpi = 0;
        for (int wpY = 0; wpY < 4; ++wpY)
        {
          gridOffsetX = 0;
          for (int wpX = 0; wpX < 4; ++wpX)
          {
            int xi = gridStartX + gridOffsetX;

            //int xShift = xi / aTileWidth;
            //int xIndex = xi % aTileWidth;

            int xShift = apcXshifts[xi];
            int xIndex = apcXoffsets[xi];

            //gpv[wpi] = apInII[gridStartX + gridOffsetX + (gridStartY + gridOffsetY) * int32SourceStride];
            int y = ((windowStart + gridStartY + gridOffsetY) & 0x1F) * realLineWidth;
            gpv[wpi] = (vec32s)getFromII(apWindowBuffer, xShift, xIndex, y);

            gridOffsetX += lbpf.width;
            ++wpi;
          }
          gridOffsetY += lbpf.height;
        }

        vec32s center = gpv[5] + gpv[10] - gpv[6] - gpv[9];

        vec32s c0 = gpv[0] + gpv[5] - gpv[1] - gpv[4];
        vec32s c1 = gpv[1] + gpv[6] - gpv[2] - gpv[5];
        vec32s c2 = gpv[2] + gpv[7] - gpv[3] - gpv[6];
        vec32s c3 = gpv[6] + gpv[11] - gpv[7] - gpv[10];
        vec32s c4 = gpv[10] + gpv[15] - gpv[11] - gpv[14];
        vec32s c5 = gpv[9] + gpv[14] - gpv[10] - gpv[13];
        vec32s c6 = gpv[8] + gpv[13] - gpv[9] - gpv[12];
        vec32s c7 = gpv[4] + gpv[9] - gpv[5] - gpv[8];

		//the code contains comparison results (grid sub-area sum to center) in its bits
        vec16s code = vec16s(0);
        vif(c0 >= center)
        {
          code |= vec16s(0x0080);
        }
        vendif;
        vif(c1 >= center)
        {
          code |= vec16s(0x0040);
        }
        vendif;
        vif(c2 >= center)
        {
          code |= vec16s(0x0020);
        }
        vendif;
        vif(c3 >= center)
        {
          code |= vec16s(0x0010);
        }
        vendif;
        vif(c4 >= center)
        {
          code |= vec16s(0x0008);
        }
        vendif;
        vif(c5 >= center)
        {
          code |= vec16s(0x0004);
        }
        vendif;
        vif(c6 >= center)
        {
          code |= vec16s(0x0002);
        }
        vendif;
        vif(c7 >= center)
        {
          code |= vec16s(0x0001);
        }
        vendif;

        vec32s values[8];
        for (int vi = 0; vi < 8; ++vi)
        {
          values[vi] = lbpf.values[vi];
        }

        //get one of the 256 bits contained in values[8] indexed by code in 2 steps
		
		//indexing values[] by 3 most significant bits of the code
        vec32s featureVal = vload(values, code >> 5);
		//check the bit indexed by the 5 least significant bits
        vif((featureVal & ((vec32s)(1) << (vec32s)(code & 0x1f))) != vec32s(0))
        {
          stageSum += vec32s(lbpf.leafValuesFixed[0]);
        }
        velse
        {
          stageSum += vec32s(lbpf.leafValuesFixed[1]);
        }
        vendif;

        ++featureIndex;
      }

      vif(stageSum < lbps.threshold)
      {
        detected = false;
      }
      vendif;

      if (vall(!detected))
      {
        break;
      }
    }

    vif(detected)
    {
      apOut[x] = 255;
    }
    vendif;
  }
}

#endif
