#ifdef ACF_KERNEL_IMPLEMENTATION

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
* \file apu_histogram_apu.cpp
* \brief histogram implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/

#include "histogram_apu.h"

#ifdef APEX2_EMULATE
#include "apu_vec.hpp"
#endif

/*****************************************************************************
* functions
*****************************************************************************/

void hist(vec08u* lpvIn0, vec32u* lpvOut0, bool isFirstTile, int lStrideIn0, int chunkWidth, int chunkHeight, int outChunkWidth)
{
  
  //initialize the static vector output buffer to zero
  if (isFirstTile)
  {
      for (int x=0; x < outChunkWidth; x++)
        lpvOut0[x] = 0;
  }
  for (int y=0; y < chunkHeight; y++)
  {
    for (int x=0; x < chunkWidth; x++)
    {
      vec08u lvBinIndex = lpvIn0[y*lStrideIn0+x];
      vec32u lvTmp = vload(lpvOut0, lvBinIndex);
      lvTmp += 1;
      vstore(lpvOut0, lvBinIndex, lvTmp);
    }
  }
}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
