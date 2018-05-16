/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
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
****************************************************************************/

#ifndef FRAMEINPUTPATTERNGEN_H
#define FRAMEINPUTPATTERNGEN_H

#include <frame_input_base.h>

namespace io
{
  class FrameInputPatternGen;
  
  typedef enum 
  {
    IO_PATTERN_CHESS = 0,
    IO_PATTERN_STRIPES_RGB,
    IO_PATTERN_WLK_STRIPES_RGB,
    IO_PATTERN_NOT_INITIALIZED
  } IO_PATTERN;
  
//****************************************************************************
  
  struct PatternBase
  {
    IO_PATTERN mPatternType;
    
    PatternBase(void);
    virtual int32_t Generate(FrameInputPatternGen *apGenerator, void *apFrame) = 0;
    virtual ~PatternBase() = 0;
  }; // struct PatternBase
  
//****************************************************************************

  struct PatternChessBW : public PatternBase
  {
    uint32_t mSize;   /// size of one square in pixs
    
    PatternChessBW(void);
    PatternChessBW(uint32_t mSize);
    int32_t Generate(FrameInputPatternGen *apGenerator, void *apFrame);
    ~PatternChessBW();
  }; // PatternChessBW
  
//****************************************************************************

  struct PatternStripesRGB : public PatternBase
  {
    uint32_t mStripeWidth;   /// size of one square in pixs
    
    PatternStripesRGB(void);
    PatternStripesRGB(uint32_t mStripeWidth);
    int32_t Generate(FrameInputPatternGen *apGenerator, void *apFrame);
    ~PatternStripesRGB();
  }; // PatternStripesRGB
  
//****************************************************************************

  struct PatternWlkStripesRGB : public PatternBase
  {
    uint32_t mStripeWidth;   /// size of one square in pixs
    uint32_t mWlkStep;
    uint32_t mWlkState;
    
    PatternWlkStripesRGB(void);
    PatternWlkStripesRGB(uint32_t mStripeWidth, uint32_t mWlkStep);
    int32_t Generate(FrameInputPatternGen *apGenerator, void *apFrame);
    ~PatternWlkStripesRGB();
  }; // PatternStripesRGB

//****************************************************************************
  
  class FrameInputPatternGen : public FrameInputBase
  {
    public:
      FrameInputPatternGen(PatternBase *apPattern, uint32_t aWidth, 
                           uint32_t aHeight, IO_DATA_DEPTH aDepth, 
                           IO_DATA_CHANNELS aChannels);
      
      ~FrameInputPatternGen();
    
      void *           GetFrame();
      IO_STATUS        GetStatus(); 
      
      uint32_t         GetWidth();
      uint32_t         GetHeight();
      IO_DATA_DEPTH    GetDepth();
      IO_DATA_CHANNELS GetChannels();
      
    private:
      void*            mpFrame;
      void*            mpFrame_handle;
      PatternBase*     mpPattern;
      uint32_t         mWidth;
      uint32_t         mHeight;
      uint32_t         mFrameSize;
      IO_DATA_DEPTH    mDepth;
      IO_DATA_CHANNELS mChannels;
      
      IO_STATUS        mStatus;
  };
}


#endif /* FRAMEINPUTPATTERNGEN_H */