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
* \file sat_apu.cpp
* \brief sat_box_filter implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#ifdef APEX2_EMULATE
#include <apu_lib.hpp>
#include <apu_vec.hpp>
//#include <acf_kernel.hpp>
using namespace APEX2;
#endif

#include "sat_apu.h"
#include "add_apu.h"

static inline vec32u vectorMoveShiftRight32(vec32u aV, int aNElements)
{
  vec16u vL = vec16u(aV);
  vec16u vH = vec16u(aV >> 16);
  for (int i = 0; i < aNElements; ++i)
  {
    vL = vmsr(vL);
    vH = vmsr(vH);
  }
  aV = ((vec32u)vH << 16) | vec32u(vL);
  return aV;
}


static inline vec32s vectorMoveShiftRight32s(vec32s aV, int aNElements)
{
	vec16u vL = vec16u(aV);
	vec16s vH = vec16s(aV >> 16);
	for (int i = 0; i < aNElements; ++i)
	{
		vL = vmsr(vL);
		vH = vmsr(vH);
	}
	aV = ((vec32s)vH << 16) | vec32s(vL);
	return aV;
}


static inline vec32u interBlockAccum32(vec32u aV)
{
  aV = vectorMoveShiftRight32(aV, 1);
  int stillShift = APU_VSIZE;
  for (int i = 1; i <= stillShift; i <<= 1){
	  aV += vectorMoveShiftRight32(aV, i);
  }
  /*
  This is what is done in the loop above with 32 CUs. The loop is valid also for 64 CUs
  aV += vectorMoveShiftRight32(aV, 1);
  aV += vectorMoveShiftRight32(aV, 2);
  aV += vectorMoveShiftRight32(aV, 4);
  aV += vectorMoveShiftRight32(aV, 8);
  aV += vectorMoveShiftRight32(aV, 16);
  aV += vectorMoveShiftRight32(aV, 32);*/
  return aV;
}



void sat32(vec32u* apDest, vec32u* apPrevRow,
  const vec08u* apcSrc,
  int inStrideW, int outStrideW,
  int bw, int bh, int08u aFirstTile)
{
  int destinationStride32 = outStrideW >> 2;
  //null the previous row in the first tile
  //TODO: is it possible outside the kernel implementation function?
  if (aFirstTile)
  {
    for (int16s x = 0; x < bw; ++x)
    {
      apPrevRow[x] = 0;
    }
  }

  //first step uses the previous row
  vec32u blockAccum = 0;
  for (int16s x = 0; x < bw; ++x) chess_prepare_for_pipelining
  {
    blockAccum += (vec32u)(apcSrc[x]);
    apDest[x] = blockAccum + apPrevRow[x];
  }

  blockAccum = interBlockAccum32(blockAccum);

  for (int16s x = 0; x < bw; ++x)
  {
    apDest[x] += blockAccum;
  }

  for (int16s y = 1; y < bh; ++y)
  {
    apDest += destinationStride32;
    apcSrc += inStrideW;

    blockAccum = 0;
    for (int16s x = 0; x < bw; ++x)
    {
      blockAccum += (vec32u)(apcSrc[x]);
      apDest[x] = blockAccum + apDest[x - destinationStride32];
    }

    blockAccum = interBlockAccum32(blockAccum);

    for (int16s x = 0; x < bw; ++x)
    {
      apDest[x] += blockAccum;
    }
  }

  //save the last row as the previous row
  for (int16s x = 0; x < bw; ++x)
  {
    apPrevRow[x] = apDest[x];
  }
}



#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
