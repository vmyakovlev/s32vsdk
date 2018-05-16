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

#ifndef FRAMEOUTPUTDCU_H
#define FRAMEOUTPUTDCU_H

#include <frame_output_base.h>
#include <stddef.h>
#include "dcu.h"
#include <umat.hpp>

#define DCU_FRAME_BUFFER_CNT 2

// comment in case of arbittrary geometry support
#define DCU_FIXED
#define DCU_FRM_WIDTH 1920
#define DCU_FRM_HEIGHT 1080

namespace io
{
  class FrameOutputDCU : public FrameOutputBase
  {
    public:
      /************************************************************************/
      /** Initializes the use of the DCU object.
       *
       * No interaction with DCU driver yet
       * 
       ************************************************************************/
      FrameOutputDCU(void);
      
      /************************************************************************/
      /** Initializes the use of the DCU object and HW.
       *
       * Internaly calls Init() method.
       * 
       * \param aWidth    desired width in pixel (has to be supported)
       * \param aHeight   desired height in pixel
       * \param aDepth    bit depth per channel 
       * \param aChannels number of channels per pixel
       * \param aFormat   DCU pixel format to be used
       * 
       ************************************************************************/
      FrameOutputDCU(uint32_t aWidth, 
                     uint32_t aHeight, 
                     IO_DATA_DEPTH aDepth, 
                     IO_DATA_CHANNELS aChannels, 
                     Dcu_BPP_t aFormat = DCU_BPP_24);
      
      /************************************************************************/
      /** Cleans up the DCU object.
       *
       ************************************************************************/
      ~FrameOutputDCU(void);
    
      /************************************************************************/
      /** Initializes the use of the DCU HW.
       *
       * \param aWidth    desired width in pixel (has to be supported)
       * \param aHeight   desired height in pixel
       * \param aDepth    bit depth per channel 
       * \param aChannels number of channels per pixel
       * \param aFormat   DCU pixel format to be used
       * 
       ************************************************************************/
      void         Init(uint32_t aWidth, 
                        uint32_t aHeight, 
                        IO_DATA_DEPTH aDepth, 
                        IO_DATA_CHANNELS aChannels, 
                        Dcu_BPP_t aFormat = DCU_BPP_24);
      
      /************************************************************************/
      /** DCU SW reset.
       *
       ************************************************************************/
      void         Reset(void);
      
      /************************************************************************/
      /** Disableds DCU.
       *
       ************************************************************************/
      void         Disable(void);
      
      /************************************************************************/
      /** Displays the specified image data.
       *
       * No cache fulsh.
       * 
       * \param frame  pointer to data to be displayed
       * 
       ************************************************************************/
      void         PutFrame(void *frame);
      
      /************************************************************************/
      /** Displays the specified image data.
       *
       * \param frame  pointer to data to be displayed
       * \param aFlushCahe if true cache is flushed before
       * 
       ************************************************************************/
      void         PutFrame(void *frame, bool aFlushCache);
      
      /************************************************************************/
      /** Displays the specified image data.
       *
       * \param umat  pointer to data to be displayed
       * 
       ************************************************************************/
      void         PutFrame(vsdk::UMat umat);
      
      /************************************************************************/
      /** Returns status of the object.
       *
       * \return IO_OK if all OK so far
       *         IO_ERROR if error detected
       * 
       ************************************************************************/
      IO_STATUS    GetStatus(); 
      
    private:
      /************************************************************************/
      /** Initialization of the object internal values.
       *
       * \return IO_OK if all OK so far
       *         IO_ERROR if error detected
       * 
       ************************************************************************/
      void         BaseInit();
      
      // *** private members ***
      uint8_t *             mppFrameBuffers[DCU_FRAME_BUFFER_CNT]; ///< buffer pointers
      void *                mppFbsVirtual[DCU_FRAME_BUFFER_CNT]; ///< virtual buffer pointers
      void *                mpFbsPhysical[DCU_FRAME_BUFFER_CNT]; ///< physical buffer adresses
      
      uint32_t              mWidth;
      uint32_t              mHeight;
      IO_DATA_DEPTH         mDepth;
      IO_DATA_CHANNELS      mChannels;
      size_t                mSize;
      
      IO_STATUS             mStatus;        ///< status of the frame_o interface
      Dcu_LCD_Para_t        mLcdPara;       ///< LCD parameters
      uint32_t              mDcuClk;        ///< DCU clock
      Dcu_LCD_Connection_t  mLcdConnection; ///< LCD connection type
      Dcu_LayerParams_t     mDcuLayerParams;///< DCU layer setup
      uint32_t              mNextBuff;      ///< next DCU buffer
  }; // class FrameOutputDCU
} // namespace io


#endif /* FRAMEOUTPUTDCU_H */