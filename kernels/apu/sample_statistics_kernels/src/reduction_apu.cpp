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
* \file reduction_apu.cpp
* \brief reduction implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/

#include "reduction_apu.h"

#ifdef APEX2_EMULATE
#include "apu_vec.hpp"
#endif

/*****************************************************************************
* functions
*****************************************************************************/


void reduc(vec32u* lpvIn0, int32s* lpOut0, bool isLastTile, int16s lFirstCuId, int16s lTileWidthInChunks,
			int lChunkWidth, int lChunkHeight, int lChunkSpanIn0, int lChunkSpanOut0)
{
  if (isLastTile)
  {
    //initialize the static scalar output buffer to zero
    for (int y=0; y<lChunkHeight; y++){
		for (int x=0; x<lChunkWidth; x++){
			lpOut0[y*lChunkSpanOut0+x] = 0;
		}
	}
	
    for (int i=lFirstCuId;
    i<lFirstCuId+lTileWidthInChunks; i++)
    {
      for (int y=0; y<lChunkHeight; y++)
      {
        for (int x=0; x<lChunkWidth; x++)
        {
          lpOut0[y*lChunkSpanOut0+x] += vget(lpvIn0[y*lChunkSpanIn0+x], i);
        }
      }
    }
  }
}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION


