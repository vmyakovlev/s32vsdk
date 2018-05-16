/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor;
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
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "linux/videodev2.h"
#include "linux/fb.h"
#include "video/fsl_dcu_ioctl.h"
#include <frame_output_v234fb.h>
#include "oal.h"
#include "vdb_log.h"
#include <sys/ioctl.h>  
#include <errno.h>
#include <string.h>

namespace io
{

FrameOutputV234Fb::FrameOutputV234Fb(uint32_t aWidth, 
                                     uint32_t aHeight, 
                                     IO_DATA_DEPTH aDepth, 
                                     IO_DATA_CHANNELS aChannels, 
                                     uint32_t aFormat):
  mpFrameBuffer(NULL), mScreenIdx(0), mWidth(aWidth), mHeight(aHeight), 
  mDepth(aDepth), mChannels(aChannels), mBytesPerPix(0), mFormat(aFormat), 
  mSize(0), mStatus(IO_OK), mFbFd(-1), mDcuFd(-1)
{  
  FrameOutputV234Fb::Init(aWidth,
                          aHeight,
                          aDepth,
                          aChannels,
                          aFormat);
}

//***************************************************************************

FrameOutputV234Fb::FrameOutputV234Fb()
  : 
    mpFrameBuffer(0),
    mScreenIdx(0),
    mWidth(0),
    mHeight(0),
    mDepth(IO_DATA_DEPTH_NOT_INITIALIZED),
    mChannels(IO_DATA_CH_NOT_INITIALIZED),
    mBytesPerPix(0),
    mFormat(0),
    mSize(0),
    mStatus(IO_ERROR),
    mFbFd(-1),
    mDcuFd(-1)
{
  memset(mppScreens, sizeof(mppScreens), 0);
}

//***************************************************************************

void FrameOutputV234Fb::Init( uint32_t aWidth, 
                              uint32_t aHeight, 
                              IO_DATA_DEPTH aDepth, 
                              IO_DATA_CHANNELS aChannels, 
                              uint32_t aFormat)
{  
  
  mWidth    = aWidth;
  mHeight   = aHeight;
  mDepth    = aDepth;
  mChannels = aChannels;
  mFormat   = aFormat;
  mStatus   = IO_OK;
  
  if(mpFrameBuffer)
  {
    munmap(mpFrameBuffer, mSize);
    close(mDcuFd);
    close(mFbFd);
    mpFrameBuffer = NULL;
  } // reset before reopening  
  
  mFbFd = open("/dev/fb0", O_RDWR);
  mDcuFd = open("/dev/dcu", O_RDWR);
 
  if (mFbFd < 0)
  {
    mStatus = IO_ERROR;
  } // if file open failed
  else
  {
    mStatus = IO_OK;
  } // else from if file open failed
  
  struct fb_fix_screeninfo mFixedInfo;  ///< fixed frame buffer info from ioctl
  
  ///< Linux display:
  ///< Get current Linux FB info and geometry setup.
  if (!ioctl( mFbFd,
              FBIOGET_VSCREENINFO,
              &mScreenInfo) && 
      !ioctl( mFbFd,
              FBIOGET_FSCREENINFO,
              &mFixedInfo))
  {          
    ///< Linux display:
    ///< Modify the pixel format if needed.
    
    // *** change number of channels ***
    if(mChannels == 2)
    {
      mScreenInfo.bits_per_pixel = 16;
      mScreenInfo.transp.offset = 0;
      mScreenInfo.transp.length = 0;
      mScreenInfo.transp.msb_right = 0;
    } // if 2 channels
    else
    if(mChannels == 3)
    {
      if(aFormat == DCU_BPP_YCbCr422)
      {
        mScreenInfo.bits_per_pixel = 16;
        // RGBA componets setup has to be 0
        memset(&mScreenInfo.red, 0, sizeof(mScreenInfo.red));
        memset(&mScreenInfo.green, 0, sizeof(mScreenInfo.green));
        memset(&mScreenInfo.blue, 0, sizeof(mScreenInfo.blue));
        memset(&mScreenInfo.transp, 0, sizeof(mScreenInfo.transp));
        mScreenInfo.grayscale = V4L2_PIX_FMT_UYVY; 
      } // if DCU_BPP_YCbCr422
      else
      {
        // provide proper RGBA components setup
        mScreenInfo.bits_per_pixel    = 24;
        mScreenInfo.red.offset        = 16;
        mScreenInfo.red.length        = 8;
        mScreenInfo.red.msb_right     = 0;
        mScreenInfo.green.offset      = 8;
        mScreenInfo.green.length      = 8;
        mScreenInfo.green.msb_right   = 0;
        mScreenInfo.blue.offset       = 0;
        mScreenInfo.blue.length       = 0;
        mScreenInfo.blue.msb_right    = 0;
        mScreenInfo.transp.offset     = 0;
        mScreenInfo.transp.length     = 0;
        mScreenInfo.transp.msb_right  = 0;
        mScreenInfo.grayscale         = 0;
      } // else from if DCU_BPP_YCbCr422
    } // if 3 color channels
    else if(mChannels == 4)
    {
      // provide proper RGBA components setup
      mScreenInfo.bits_per_pixel = 32;
      mScreenInfo.red.offset        = 16;
      mScreenInfo.red.length        = 8;
      mScreenInfo.red.msb_right     = 0;
      mScreenInfo.green.offset      = 8;
      mScreenInfo.green.length      = 8;
      mScreenInfo.green.msb_right   = 0;
      mScreenInfo.blue.offset       = 0;
      mScreenInfo.blue.length       = 0;
      mScreenInfo.blue.msb_right    = 0;
      mScreenInfo.transp.offset     = 24;
      mScreenInfo.transp.length     = 8;
      mScreenInfo.transp.msb_right  = 0;
      mScreenInfo.grayscale         = 0;
    } // if 4 color channels
    else
    {
      printf("Unknown config required. Keeping default settings:\n");
      printf("v_heigth == %u, v_width == %u\n",
                  mScreenInfo.yres_virtual, mScreenInfo.xres_virtual);
      printf("height == %u, width == %u\n",
                    mScreenInfo.yres, mScreenInfo.xres);
      printf("bpp == %u\n",
                    mScreenInfo.bits_per_pixel);
    } // else from if 4 color channels
    
    ///< Linux display:
    ///< Modify geometry if needed.
    
    mScreenInfo.yres_virtual  = mHeight * DCU_FRM_CNT;
    mScreenInfo.xres_virtual  = mWidth;
    mScreenInfo.yres          = mHeight;
    mScreenInfo.xres          = mWidth;
    
    // *** reset PAN if any ***
    mScreenInfo.yoffset       = 0;  
    mScreenInfo.xoffset       = 0;  
    
    ///< Linux display:
    ///< Apply the updates to LinuxFB
    // set screeninfo
    ioctl(mFbFd,
          FBIOPUT_VSCREENINFO,
          &mScreenInfo);
  }
  
  mSize = mWidth * mHeight * mScreenInfo.bits_per_pixel/8;
  
  // map the whole frambuffer
  mpFrameBuffer = (uint8_t *)mmap(0, 
                                  mSize * DCU_FRM_CNT, // double buffered
                                  PROT_READ | PROT_WRITE | PROT_EXEC, 
                                  MAP_SHARED, 
                                  mFbFd, 
                                  0x0);
  
  if(mpFrameBuffer == (void*)-1)
  {
    printf("Framebuffer mapping failed.\n");
    printf("%s\n", strerror(errno));
  }
  else
  {
    printf("Framebuffer mapped at %p.\n", mpFrameBuffer);
  }
  // setup screen pointers
  for(uint32_t i = 0; i < DCU_FRM_CNT; i++)
  {
    mppScreens[i] = mpFrameBuffer + i * mSize;
    //printf("%p\n", mppScreens[i]);
  } // for all screens
} // FrameOutputV234Fb::FrameOutputV234Fb()

//***************************************************************************

void FrameOutputV234Fb::Disable()
{
  if(mpFrameBuffer)
  {
    munmap(mpFrameBuffer, mSize);
  }
  
  if(mDcuFd != -1)
  {
    close(mDcuFd);
  } // if dcu file opened
  
  if(mFbFd != -1)
  {
    close(mFbFd);
  } // if fb file opened
} // FrameOutputV234Fb::Disable()

//***************************************************************************

FrameOutputV234Fb::~FrameOutputV234Fb()
{
  Disable();
} // FrameOutputV234Fb::~FrameOutputV234Fb()

//***************************************************************************

void FrameOutputV234Fb::PutFrame(vsdk::UMat frame)
{
  ///< Linux display:
  ///< Keep track of the back/front buffer index.
  uint32_t lScreenIdx = (mScreenIdx + 1) % DCU_FRM_CNT;
  
  ///< Linux display:
  ///< Copy user data to backbuffer.

  vsdk::Mat matrix = frame.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED);
  if (!matrix.empty())
    memcpy(mppScreens[lScreenIdx], matrix.data, mSize);    
  else
    printf("DCU: Cannot map a buffer.\n");
  
  ///< Linux display:
  ///< Adjust the panning in Linux FB.
  mScreenInfo.yoffset = lScreenIdx * mHeight;  
  
  ///< Linux display:
  ///< Display the backbuffer.
  // set screeninfo
  ioctl(mFbFd,
        FBIOPAN_DISPLAY,
        &mScreenInfo);
  mScreenIdx = lScreenIdx;
} // FrameOutputV234Fb::PutFrame()

//***************************************************************************
  
void FrameOutputV234Fb::PutFrame(void *frame)
{
  PutFrame(frame, true);
} // FrameOutputV234Fb::PutFrame(void *frame)

//***************************************************************************

void FrameOutputV234Fb::PutFrame(void *frame, bool aInvalidateCache)
{
  ///< Linux display:
  ///< Keep track of the back/front buffer index.
  uint32_t lScreenIdx = (mScreenIdx + 1) % DCU_FRM_CNT;
  
  ///< Linux display:
  ///< Copy user data to backbuffer.
   
  if(aInvalidateCache)
  {
    OAL_MemoryInvalidate(frame,mSize);
  } // if cache to be invalidated
  
  memcpy(mppScreens[lScreenIdx], frame, mSize);    
  
  ///< Linux display:
  ///< Adjust the panning in Linux FB.
  mScreenInfo.yoffset = lScreenIdx * mHeight;  
  
  ///< Linux display:
  ///< Display the backbuffer.
  // set screeninfo
  ioctl(mFbFd,
        FBIOPAN_DISPLAY,
        &mScreenInfo);
  mScreenIdx = lScreenIdx;
} // FrameOutputV234Fb::PutFrame()

//***************************************************************************

void FrameOutputV234Fb::PutFrameAlpha(void *frame, bool aInvalidateCache)
{
  uint8_t *lpLineSrc = (uint8_t*)frame;
  uint8_t *lpLineDst = (uint8_t*)mpFrameBuffer;
  
  if(aInvalidateCache)
  {
    OAL_MemoryInvalidate(frame,mSize);
  } // if cache to be invalidated
  
  ioctl(mFbFd, FBIO_WAITFORVSYNC, 0);
  
  for(uint32_t y = 0; y < mHeight; y++)
  {
    uint8_t *lpSrc = lpLineSrc;
    uint8_t *lpDst = lpLineDst;
    
    for(uint32_t x = 0; x < mWidth; x++)
    {
      lpDst[0] = lpSrc[0];
      lpDst[1] = lpSrc[1];
      lpDst[2] = lpSrc[2];
      lpDst[3] = 255;
      lpDst += 4;
      lpSrc += 3;
    }
    lpLineSrc += mWidth * 4;
    lpLineDst += mWidth * 4;
  } 
} // FrameOutputV234Fb::PutFrameAlpha(

//***************************************************************************

IO_STATUS FrameOutputV234Fb::GetStatus()
{
  return mStatus;
} // FrameOutputV234Fb::GetStatus()

} // namespace io

//***************************************************************************