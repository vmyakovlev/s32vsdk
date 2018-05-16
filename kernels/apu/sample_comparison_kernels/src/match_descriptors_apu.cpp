
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
* \file match_descriptors_apu.cpp
* \brief Binary descriptor matching implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "match_descriptors_apu.h"

#ifdef APEX2_EMULATE
#include "apu_vec.hpp"
#endif

/*****************************************************************************
* functions
*****************************************************************************/

static vec16u VectorMoveShiftRight(vec16u aV, int aNElements)
{
  for (int i = 0; i < aNElements; ++i)
  {
    aV = vmsr(aV);
  }
  return aV;
}

static unsigned int VectorSum(vec16u aV)
{
  vec16u accum = vec16u(aV);
  accum += VectorMoveShiftRight(accum, 1);
  accum += VectorMoveShiftRight(accum, 2);
  accum += VectorMoveShiftRight(accum, 4);
  accum += VectorMoveShiftRight(accum, 8);
  accum += VectorMoveShiftRight(accum, 16);
  const int SHIFT_SZ = 32;
  return vget(accum, SHIFT_SZ - 1);
}

static unsigned int PopCount(vec08u bits)
{
  //TODO: LUT?
  vec08u pc = bits;
  pc -= (pc >> 1) & vec08u(0x55);             //put count of each 2 bits into those 2 bits
  pc = (pc & vec08u(0x33)) + ((pc >> 2) & vec08u(0x33)); //put count of each 4 bits into those 4 bits 
  pc = (pc + (pc >> 4)) & vec08u(0x0f);        //put count of each 8 bits into those 8 bits 

  return VectorSum(vec16u(pc));
}

void Match(
  const vec08u* apcDescriptors0,
  unsigned int aDescriptor0Count,
  const vec08u* apcDescriptors1,
  unsigned int aDescriptor1Count,
  int16s* apMatches0,
  int16s* apMatches1,
  int08u aThreshold,
  int08u aRangeCheck)
{
  for (unsigned int i = 0; i < aDescriptor0Count; ++i)
  {
    apMatches0[i] = -1;
  }
  for (unsigned int i = 0; i < aDescriptor1Count; ++i)
  {
    apMatches1[i] = -1;
  }

  for (unsigned int i = 0; i < aDescriptor0Count; ++i)
  {
    vec08u descriptor0 = apcDescriptors0[i];
    int08u minDistA = 255;
    int08u minDistB = 255;
    int16s bestMatchIndex = -1;
    for (unsigned int j = 0; j < aDescriptor1Count; ++j)
    {
      if (apMatches1[j] >= 0)
      {
        continue;
      }

      vec08u descriptorDiff = (descriptor0 ^ apcDescriptors1[j]);
      unsigned int dist = PopCount(descriptorDiff);

      if (dist <= aThreshold)
      {
        if (dist < minDistA)
        {
          minDistB = minDistA;
          minDistA = dist;
          bestMatchIndex = j;
        }
        else if (dist < minDistB)
        {
          minDistB = dist;
        }
      }
      else if (dist < minDistB)
      {
        minDistB = dist;
      }
    }

    if (bestMatchIndex >= 0 && (minDistB - minDistA > aRangeCheck))
    {
      apMatches0[i] = bestMatchIndex;
      apMatches1[bestMatchIndex] = i;
    }
  }
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION 
