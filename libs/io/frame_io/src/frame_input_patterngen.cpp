/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2014 Freescale Semiconductor;
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

#include <frame_input_patterngen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <oal.h>

namespace io
{

PatternBase::PatternBase()
{
}

//****************************************************************************  

PatternBase::~PatternBase()
{
}

//****************************************************************************
  
PatternChessBW::PatternChessBW()
{
  mPatternType = IO_PATTERN_CHESS;
  mSize = 100;
}

//****************************************************************************

PatternChessBW::PatternChessBW(uint32_t aSize): mSize(aSize)
{
  mPatternType = IO_PATTERN_CHESS;
}

//****************************************************************************

PatternChessBW::~PatternChessBW()
{
}

//****************************************************************************

int32_t PatternChessBW::Generate(FrameInputPatternGen *apGenerator, void *apFrame)
{
  uint32_t lWidth   = apGenerator->GetWidth();  
  uint32_t lHeight  = apGenerator->GetHeight();
  uint32_t lChanCnt = apGenerator->GetChannels();
  
  uint32_t lFullW   = lWidth/mSize;
  uint32_t lFullH   = lHeight/mSize;
  
  uint32_t lRestW   = lWidth - lFullW * mSize;
  uint32_t lRestH   = lHeight - lFullH * mSize;
  
  uint32_t lColShift  = mSize * lChanCnt;
  uint32_t lLineShift = lWidth * lChanCnt;
  //uint32_t lRowShift  = lLineShift * mSize;
  
  uint8_t *lpDataDst  = (uint8_t*) apFrame;
  uint8_t *lpDataSrc  = (uint8_t*) apFrame;
  uint8_t lPixVal     = 1;
    
  
  // *** generate all full rows ***
  for(uint32_t i = 0; i < lFullH; i++)
  {
    // *** generate first line ***
    for(uint32_t j = 0; j < lFullW; j++)
    { 
      memset(lpDataDst, lPixVal * 255, lColShift);
      lPixVal = !lPixVal;
      lpDataDst += lColShift;
    } // for all ful collumns 

    // do the rest of the line
    if(lRestW != 0)
    {
      memset(lpDataDst, lPixVal * 255, lRestW * lChanCnt);
      lPixVal = !lPixVal;
      lpDataDst += lRestW * lChanCnt;    
    }
    
    // set src pointer to the first line beginning
    lpDataSrc = lpDataDst - lLineShift;
    
    // *** do the rest of the row ***
    for(uint32_t j = 0; j < (mSize -1); j++)
    {
      memcpy(lpDataDst, lpDataSrc, lLineShift);
      lpDataDst += lLineShift;
    } // for all lines in a rows
  }
  
  // *** do the reset of the lines
  if(lRestH != 0)
  {
     // *** generate first line ***
    for(uint32_t j = 0; j < lFullW; j++)
    { 
      memset(lpDataDst, lPixVal * 255, lColShift);
      lPixVal = !lPixVal;
      lpDataDst += lColShift;
    } // for all ful collumns 

    // do the rest of the line
    if(lRestW != 0)
    {
      memset(lpDataDst, lPixVal * 255, lRestW * lChanCnt);
      lPixVal = !lPixVal;
      lpDataDst += lRestW * lChanCnt;    
    }
    
    // set src pointer to the first line beginning
    lpDataSrc = lpDataDst - lLineShift;
    
    // *** copy the lines ***
    for(uint32_t j = 0; j < (lRestH -1); j++)
    {
      memcpy(lpDataDst, lpDataSrc, lLineShift);
      lpDataDst += lLineShift;
    } // for all lines in a rows
  } // if there are some lines left
  
  return 0;
} // PatternChessBW::Generate()

//****************************************************************************

PatternStripesRGB::PatternStripesRGB()
{
  mPatternType = IO_PATTERN_STRIPES_RGB;
  mStripeWidth = 10;
}

//****************************************************************************

PatternStripesRGB::PatternStripesRGB(uint32_t aStripeWidth)
  :mStripeWidth(aStripeWidth)
{
  mPatternType = IO_PATTERN_STRIPES_RGB;
}

//****************************************************************************

PatternStripesRGB::~PatternStripesRGB()
{
}

//****************************************************************************

int32_t PatternStripesRGB::Generate(FrameInputPatternGen *apGenerator, void *apFrame)
{
  if((apGenerator->GetChannels() != IO_DATA_CH3) 
     && (apGenerator->GetChannels() != IO_DATA_CH4))
  {
    return -1;
  }
  
  uint32_t lWidth     = apGenerator->GetWidth();  
  uint32_t lHeight    = apGenerator->GetHeight();
  uint32_t lChanCnt   = apGenerator->GetChannels();
    
  uint32_t lFullW     = lWidth/mStripeWidth;
  
  uint32_t lRestW     = lWidth - lFullW * mStripeWidth;
  
  uint32_t lLineShift = lWidth * lChanCnt;
  
  uint8_t *lpDataDst    = (uint8_t*) apFrame;
  uint8_t lPixVal       = 0;
  uint8_t lPixRGB[3][3] = {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}};
    
  
  // *** generate first line ***
  for(uint32_t i = 0; i < lFullW; i++)
  { 
    for(uint32_t j = 0; j < mStripeWidth; j++)
    {
      lpDataDst[0] = lPixRGB[lPixVal][0]; 
      lpDataDst[1] = lPixRGB[lPixVal][1];
      lpDataDst[2] = lPixRGB[lPixVal][2];
      if(lChanCnt == 4)
      {
        lpDataDst[3] = 255;
      }
      lpDataDst += lChanCnt;
    }
    // shift color
    lPixVal = (lPixVal + 1) % 3;
  } // for all ful collumns 

  // do the rest of the line
  if(lRestW != 0)
  {
    for(uint32_t j = 0; j < lRestW; j++)
    {
      lpDataDst[0] = lPixRGB[lPixVal][0]; 
      lpDataDst[1] = lPixRGB[lPixVal][1];
      lpDataDst[2] = lPixRGB[lPixVal][2];
      if(lChanCnt == 4)
      {
        lpDataDst[3] = 255;
      }
      lpDataDst += lChanCnt;
    }
  } // if any pixels left
  
  // *** do the rest of the lines ***
  for(uint32_t j = 0; j < (lHeight -1); j++)
  {
    memcpy(lpDataDst, apFrame, lLineShift);
    lpDataDst += lLineShift;
  } // for all lines in a rows
  
  return 0;
} // PatternStripesRGB::Generate()

//****************************************************************************

PatternWlkStripesRGB::PatternWlkStripesRGB()
{
  mPatternType = IO_PATTERN_STRIPES_RGB;
  mStripeWidth = 10;
  mWlkStep     = 1;
  mWlkState    = 0;
}

//****************************************************************************

PatternWlkStripesRGB::PatternWlkStripesRGB(uint32_t aStripeWidth, uint32_t aWlkStep)
  :mStripeWidth(aStripeWidth), mWlkStep(aWlkStep), mWlkState(0)
{
  mPatternType = IO_PATTERN_STRIPES_RGB;
}

//****************************************************************************

PatternWlkStripesRGB::~PatternWlkStripesRGB()
{
}

//****************************************************************************

int32_t PatternWlkStripesRGB::Generate(FrameInputPatternGen *apGenerator, void *apFrame)
{
  if((apGenerator->GetChannels() != IO_DATA_CH3) 
     && (apGenerator->GetChannels() != IO_DATA_CH4))
  {
    return -1;
  }
  
  uint32_t lWidth   = apGenerator->GetWidth();  
  uint32_t lHeight  = apGenerator->GetHeight();
  uint32_t lChanCnt = apGenerator->GetChannels();
  
  uint32_t lFullW   = lWidth/mStripeWidth;
  
  uint32_t lRestW   = lWidth - lFullW * mStripeWidth;
  
  uint32_t lLineShift = lWidth * lChanCnt;
  
  uint8_t *lpDataDst  = (uint8_t*) apFrame;
  uint8_t lPixVal     = 0;
  uint8_t lPixRGB[3][3] = {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}};
    
  
  // *** generate first line ***
  for(uint32_t i = 0; i < lFullW; i++)
  { 
    for(uint32_t j = 0; j < mStripeWidth; j++)
    {
      lpDataDst[0] = lPixRGB[lPixVal][0]; 
      lpDataDst[1] = lPixRGB[lPixVal][1];
      lpDataDst[2] = lPixRGB[lPixVal][2];
      if(lChanCnt == 4)
      {
        lpDataDst[3] = 255;
      }
      lpDataDst += lChanCnt;
    }
    // shift color
    lPixVal = (lPixVal + 1) % 3;
  } // for all ful collumns 

  // do the rest of the line
  if(lRestW != 0)
  {
    for(uint32_t j = 0; j < lRestW; j++)
    {
      lpDataDst[0] = lPixRGB[lPixVal][0]; 
      lpDataDst[1] = lPixRGB[lPixVal][1];
      lpDataDst[2] = lPixRGB[lPixVal][2];
      if(lChanCnt == 4)
      {
        lpDataDst[3] = 255;
      }
      lpDataDst += lChanCnt;
    }
  } // if any pixels left
  
  // do the walking to 2nd line and copy back to 1st line
  {
    // copy beginnig
    size_t lCpyCnt = lLineShift - mWlkState;
    memcpy(lpDataDst, (uint8_t*)apFrame + mWlkState, lCpyCnt);
    lpDataDst += lCpyCnt;
    // copy the rest
    memcpy(lpDataDst, (uint8_t*)apFrame, lLineShift - lCpyCnt);
    lpDataDst += lLineShift - lCpyCnt;
    // copy back to 1st line
    memcpy(apFrame, lpDataDst - lLineShift, lLineShift);
  }
  
  // *** do the rest of the lines ***
  for(uint32_t j = 0; j < (lHeight -2); j++)
  {
    memcpy(lpDataDst, apFrame, lLineShift);
    lpDataDst += lLineShift;
  } // for all lines in a rows
  
  mWlkState = (mWlkState + mWlkStep * lChanCnt)%lLineShift;
  
  return 0;
} // PatternWlkStripesRGB::Generate()

//****************************************************************************

FrameInputPatternGen::FrameInputPatternGen(PatternBase *apPattern, 
                                       uint32_t aWidth, 
                                       uint32_t aHeight, 
                                       IO_DATA_DEPTH aDepth,
                                       IO_DATA_CHANNELS aChannels)
{
      mpPattern     = apPattern;
      mWidth        = aWidth;
      mHeight       = aHeight;
      mFrameSize    = aWidth * aHeight * (uint32_t)aDepth * (uint32_t)aChannels;
      mDepth        = aDepth;
      mChannels     = aChannels;
      
      // *** Allocate DDR buffers ***
#ifdef __STANDALONE__      
      mpFrame_handle = OAL_MemoryAllocFlag(mFrameSize, 
           OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE)|OAL_MEMORY_FLAG_CONTIGUOUS);
      mpFrame = OAL_MemoryReturnAddress(mpFrame_handle, ACCESS_CH_WB);
#else
      mpFrame = malloc(mFrameSize);
#endif    
      
      if(mpFrame == NULL)
      {
        mStatus = IO_ERROR;
      }      
}

//****************************************************************************

FrameInputPatternGen::~FrameInputPatternGen()
{
  if(mpFrame != NULL)
  {
#ifdef __STANDALONE__
     OAL_MemoryFree(mpFrame_handle);
#else
    free(mpFrame);
#endif
  }
}

//****************************************************************************

void * FrameInputPatternGen::GetFrame()
{
  mpPattern->Generate(this, mpFrame);
  return mpFrame;
}

//****************************************************************************

IO_STATUS FrameInputPatternGen::GetStatus()
{
  return mStatus;
}

//****************************************************************************

uint32_t FrameInputPatternGen::GetWidth()
{
  return mWidth;
}

//****************************************************************************

uint32_t FrameInputPatternGen::GetHeight()
{
  return mHeight;
}

//****************************************************************************

IO_DATA_DEPTH FrameInputPatternGen::GetDepth()
{
  return mDepth;
}

//****************************************************************************

IO_DATA_CHANNELS FrameInputPatternGen::GetChannels()
{
  return mChannels;
}

} // namespace io