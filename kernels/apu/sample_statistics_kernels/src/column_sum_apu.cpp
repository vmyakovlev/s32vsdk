#ifdef ACF_KERNEL_IMPLEMENTATION

/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor;
* Copyright (c) 2017 NXP
* All Rights Reserved
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
* \file column_sum_apu.cpp
* \brief cumputing sum of columns of image for APEX
* \author Daniel Lazar
* \version
* \date
****************************************************************************/

#include "column_sum_apu.h"

#ifdef APEX2_EMULATE
#include "apu_vec.hpp"
#endif

/*****************************************************************************
* functions
*****************************************************************************/

void column_sum(vec08u* lpvIn0,
                vec32u* lpvOutDown,
                vec32u* lpvOutUp,
                bool isFirstTile,
                int lStrideIn0,
                int chunkWidth,
                int chunkHeight,
                int outChunkWidth,
                int priorityDown,
                int priorityUp,
                int indexOfTileStart)
{
  static int ActualTile = 0;
  //initialize the static vector output buffer to zero
  if (isFirstTile)
  {
      for (int x=0; x < outChunkWidth; x++)
      {
        lpvOutDown[x] = 0;
        lpvOutUp[x] = 0;
      }   
  }
  
  if(ActualTile >= indexOfTileStart)
  {   
    for (int x=0; x < chunkWidth; x++)
    {
      int priorDown = priorityDown + ActualTile - indexOfTileStart;
      int priorUp = priorityUp - (ActualTile - indexOfTileStart);
      
      vec32u lvTmpDown = vload(lpvOutDown, (vec08u)x);
      vec32u lvTmpUp = vload(lpvOutUp, (vec08u)x);
      
      for (int y=0; y < chunkHeight; y++)
      {        
        vec32u lvtempValue = (vec32u)lpvIn0[y*lStrideIn0 + x];       
        lvTmpDown += lvtempValue * priorDown;
        lvTmpUp += lvtempValue * priorUp;                       
      }
      
      vstore(lpvOutDown, (vec08u)x, lvTmpDown);
      vstore(lpvOutUp, (vec08u)x, lvTmpUp);
    }
  }
  
  ActualTile ++;
  
}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
