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

#ifndef FRAMEOUTPUTV234FB_H
#define FRAMEOUTPUTV234FB_H

#include <frame_output_base.h>
#include <stddef.h>
#include <fcntl.h>            
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "linux/fb.h"
#include "video/fsl_dcu_ioctl.h"
#include "dcu.h"
#include <umat.hpp>

// comment in case of arbittrary geometry support
#define DCU_FIXED
#define DCU_FRM_WIDTH  1920
#define DCU_FRM_HEIGHT 1080
#define DCU_FRM_SIZE   (DCU_FRM_WIDTH*DCU_FRM_HEIGHT*3)
#define DCU_FRM_CNT    2

namespace io
{
  class FrameOutputV234Fb : public FrameOutputBase
  {
    public:
      /************************************************************************/
      /** Initializes the use of the Linux FrameBuffer.
       *
       * Opens /dev/fb0. Modifies the resolution and color format if needed.
       * Maps the resulting FB memory.
       *
       * \param aWidth desired width in pixel (has to be supported)
       * \param aHeight desired height in pixel
       * \param aDepth bit depth per channel 
       * \param aChannels number of channels per pixel
       * \param aFormat DCU pixel format to be used
       * 
       ************************************************************************/
      FrameOutputV234Fb(uint32_t aWidth, 
                        uint32_t aHeight, 
                        IO_DATA_DEPTH aDepth, 
                        IO_DATA_CHANNELS aChannels, 
                        uint32_t aFormat=0);

      /************************************************************************/
      /** Destructor.
       * 
       ************************************************************************/
      FrameOutputV234Fb();        
      
      /************************************************************************/
      /** Initializes the use of the Linux FrameBuffer.
       *
       * Opens /dev/fb0. Modifies the resolution and color format if needed.
       * Maps the resulting FB memory.
       *
       * \param aWidth desired width in pixel (has to be supported)
       * \param aHeight desired height in pixel
       * \param aDepth bit depth per channel 
       * \param aChannels number of channels per pixel
       * \param aFormat DCU pixel format to be used
       * 
       ************************************************************************/
      void Init( uint32_t aWidth, 
                 uint32_t aHeight, 
                 IO_DATA_DEPTH aDepth, 
                 IO_DATA_CHANNELS aChannels, 
                 uint32_t aFormat = 0);
       
      /************************************************************************/
      /** Releases resources.
       *
       * Un-maps FB buffers. Closes file descriptors.
       * 
       ************************************************************************/
      void Disable(void);
      
      /************************************************************************/
      /** Destructor.
       *
       * Internally calls Disable() method. 
       * 
       ************************************************************************/
      ~FrameOutputV234Fb();
    
      /************************************************************************/
      /** Displayes data pointed to by the first argument.
       *
       * Internally copies (memcpy) the data from frame to the Linux FB 
       * backbuffer and pans the Linux FB to show the backbuffer.
       *
       * \param frame pointer to the frame data to be displayed
       * 
       ************************************************************************/
      void PutFrame(void *frame);
      void PutFrame(vsdk::UMat frame);
      
      /************************************************************************/
      /** Displayes data pointed to by the first argument.
       *
       * Internally copies (memcpy) the data from frame to the Linux FB 
       * backbuffer and pans the Linux FB to show the backbuffer.
       * 
       * If a FlushCache == true, the buffer's cache is flushed prior to copy.
       *
       * \param frame pointer to the frame data to be displayed
       * \param aFlushCache if a cache should be flushed or not
       * 
       ************************************************************************/
      void PutFrame(void *frame, bool aFlushCache);
      
      /************************************************************************/
      /** Displayes data pointed to by the first argument.
       *
       * Internally copies the data from frame to the Linux FB 
       * backbuffer adding an 0xff alpha value to each pixel and pans the Linux 
       * FB to show the backbuffer.
       * 
       * If a FlushCache == true, the buffer's cache is flushed prior to copy.
       *
       * \param frame pointer to the frame data to be displayed
       * \param aFlushCache if a cache should be flushed or not
       * 
       ************************************************************************/
      void  PutFrameAlpha(void *frame, bool aFlushCache=true);
      
      /************************************************************************/
      /** Returns status of the object.
       * 
       ************************************************************************/
      IO_STATUS        GetStatus(); 
      
    private:
      struct fb_var_screeninfo mScreenInfo; ///< screen info from ioctl
      uint8_t *             mpFrameBuffer;  ///< whole framebuffer mapping
      uint8_t *             mppScreens[DCU_FRM_CNT]; ///< pointer to buffers 0,1
      uint32_t              mScreenIdx;     ///< index of the displayed buffer
      
      uint32_t              mWidth;         ///< width used [pix]
      uint32_t              mHeight;        ///< height used [pix]
      IO_DATA_DEPTH         mDepth;         ///< depth of color channel [bits]
      IO_DATA_CHANNELS      mChannels;      ///< number of color channels used
      uint32_t              mBytesPerPix;   ///< number of bytes per pixel
      uint32_t              mFormat;        ///< DCU pixel format
      size_t                mSize;          ///< size of one screen buffer
      
      IO_STATUS             mStatus;        ///< status of the frame_o interface
      
      int32_t               mFbFd;          ///< file descriptor for /dev/fb0
      int32_t               mDcuFd;         ///< file descriptor for /dev/dcu
  };
}


#endif /* FRAMEOUTPUTV234FB_H */