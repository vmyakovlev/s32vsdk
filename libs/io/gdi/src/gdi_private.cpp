/*****************************************************************************
 *
 * Freescale Confidential Proprietary
 *
 * Copyright (c) 2015 Freescale Semiconductor;
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
 * \file     GdiPrivate.cpp
 * \brief    GDI private infrastructure and functionality implementation.
 * \author   Tomas Babinec
 * \version  0.1
 * \date     17-September-2015
 ****************************************************************************/
#include <string.h>
#include <iostream>

#ifdef INTEGRITY
#include <INTEGRITY.h>
#endif

#include "gdi_types_public.hpp"
#include "gdi_public.hpp"
#include "gdi_private.hpp"
#include "gdi_image.hpp"

#ifndef VLAB
#include "framebuffer.h"
#endif //VLAB

#include "vdb_log.h"
#include "gdi_utils.hpp"
#include "oal_cont_alloc.h"

//*****************************************************************************

GDI_Display::GDI_Display() :
    mBufferImage(), mBufferImage2()
{
	mpFBDesignation = NULL;
  mpVpdRegs = NULL;
#ifndef INTEGRITY
	mpConPath = NULL;
  mFbdevFd.fd = -1;
#endif // ifndef INTEGRITY
  mBufLen = 0;
  mEnabledCnt = 0;
  // default update value
  mUpdatesOn = true;

  // set default background color to black
  mBckgColor = GDI_ColorRgbaPack(black);
} // GDI_Display()

//*****************************************************************************

GDI_Display::~GDI_Display()
{
#ifndef INTEGRITY
  if (mFbdevFd.fd > -1)
  {
    OAL_FileClose(&mFbdevFd);
  } // if file opened
#endif // ifndef INTEGRITY
} // ~GDI_Display()

//*****************************************************************************

LIB_RESULT GDI_Display::BufferEnableVlab()
{
  LIB_RESULT lres = LIB_SUCCESS;
  GDI_ImageDescriptor tmpImgDsc(640u,
                                480u,
                                RGB888);

  VDB_LOG_NOTE("enabling VLAB display\n");
  mBufferImage2.DescriptorSet(tmpImgDsc);
  GDI_VpdEnable(&mBufferImage2,
                &mpVpdRegs);
  mBufLen = mBufferImage2.mDesc.mHeight * mBufferImage2.mDesc.mWidthStep;
  mBufferImage.DescriptorSet(tmpImgDsc);
  if (mBufferImage.Create() != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Frame-buffer double image creation failed.\n");
    lres = LIB_FAILURE;
  } // if Create()

  return lres;
} // GDI_Display::BufferEnableVlab()

//*****************************************************************************

LIB_RESULT GDI_Display::BufferDisableVlab()
{
  LIB_RESULT lres = LIB_SUCCESS;
  if (mBufferImage2.mData != MAP_FAILED )
  {
    GDI_VpdDisable(&mBufferImage2,
                   &mpVpdRegs);
    mBufferImage2.mData = NULL;
    mBufferImage.mData = NULL;
    mBufLen = 0;
  } // if BufferImage.Data

  return lres;
} // GDI_Display::BufferDisableVlab()

//*****************************************************************************

LIB_RESULT GDI_Display::FBEnable()
{
  LIB_RESULT lres = LIB_SUCCESS;
//  check current state of frame buffer
  if (Enabled())
  {
    // already enabled > just increment number of enable calls
    mEnabledCnt++;
  } // already enable
  else
  {
#ifdef INTEGRITY
#ifndef VLAB
    // do the Integrity OS steps
    lres = GhsFBEnable();
#endif // ifndef VLAB
#else // ifdef INTEGRITY
    // do the Linux OS steps
    lres = LinuxFBEnable();
#endif // ifdef INTEGRITY
    if (lres == LIB_SUCCESS)
    {
      mEnabledCnt++;
    } // if enabled
  } // not enabled yet
  return lres;
} // FBEnable()

//*****************************************************************************

LIB_RESULT GDI_Display::FBDisable()
{
  LIB_RESULT lres = LIB_SUCCESS;
  if (mEnabledCnt > 1)
  {
    // other clients still out there > just decrement number of enable calls
    mEnabledCnt--;
  } // if not last client
  else
  {
#ifdef INTEGRITY
#ifndef VLAB
    // do the Integrity OS steps
    lres = GhsFBDisable();
#endif // ifndef VLAB
#else
    // do the Linux OS steps
    lres = LinuxFBDisable();
#endif
    if (lres == LIB_SUCCESS)
    {
      mEnabledCnt--;
    }
  } // else: last client
  return lres;
} // FBDisable()

//*****************************************************************************

bool GDI_Display::Enabled()
{
  return ((mBufferImage2.mData != MAP_FAILED )&& (mBufferImage.mData != NULL));
} // Enabled()

//*****************************************************************************

#ifdef INTEGRITY
#ifndef VLAB
LIB_RESULT GDI_Display::GhsFBEnable()
{
  LIB_RESULT lres = LIB_SUCCESS;
  if(mpFBDesignation == 0)
  {
    lres = LIB_FAILURE;
  } // if bad driver designation
  else
  {
    Error E;
    uintptr_t context = 0;
    const char *name;
    do {
      // Open and initialize hardware
      E = gh_FB_get_next_driver(&context, &mpFbdrv);
      if (E != Success) {
        VDB_LOG_ERROR("Could not find working driver.\n");
        lres = LIB_FAILURE;
        break;
      } // if get next drive failed
      else
      {
        if ((gh_FB_get_driver_name(mpFbdrv, &name) != Success) ||
            strcmp(name, mpFBDesignation) != 0)
        {
          continue;
        } // This is not the driver we are looking for. Move along.

        // get default configuration.
        // Note the map address and length may be 0, meaning the FB is not yet
        // mapped
        memset(&mFbinfo_screen, 0, sizeof(FBInfo));
        mFbinfo_screen.Kind = FB_LAYER_PHYSICAL;
        CheckSuccess(gh_FB_check_info(mpFbdrv, &mFbinfo_screen));

        // try to open with default config
        E = gh_FB_open(mpFbdrv, &mFbinfo_screen, &mFbhandle_screen);
        if (E != Success) {
          const char *description;
          gh_FB_get_driver_description(mpFbdrv, &description);
          VDB_LOG_ERROR("Could not initialize device %s. Skipping.\n", description);
          lres = LIB_FAILURE;
          break;
        } // Device initialization failed.
      } // else: get next driver succeeded
    } while (mFbhandle_screen == NULL);
    if(lres == LIB_SUCCESS)
    {
      E = gh_FB_get_info(mFbhandle_screen, &mFbinfo_screen);
      if (E != Success)
      {
        VDB_LOG_ERROR("Could not get physical screen info.\n");
        lres = LIB_FAILURE;
      } // if get screen info failed
      else
      {
        if ( gh_FB_api_version() >= 2 )
        {
          memset(&mFbinfo_layer1, 0, sizeof(FBInfo));
          /* Use the same resolution as the physical screen (full plane) */
          mFbinfo_layer1.Format = FB_FORMAT_RGB;
          mFbinfo_layer1.Width = mFbinfo_screen.Width;
          mFbinfo_layer1.Height = mFbinfo_screen.Height;
          mFbinfo_layer1.Parent = mFbhandle_screen;
          mFbinfo_layer1.Kind = FB_LAYER_LOGICAL;
          /* Select the background layer */
          mFbinfo_layer1.UniqueId = (Address)0;
          /* Use RGB565 for the framebuffer, no Alpha */
          mFbinfo_layer1.BitsPerPixel = 16;
          mFbinfo_layer1.Red.Offset = 11;
          mFbinfo_layer1.Red.Bits = 5;
          mFbinfo_layer1.Green.Offset = 5;
          mFbinfo_layer1.Green.Bits = 6;
          mFbinfo_layer1.Blue.Offset = 0;
          mFbinfo_layer1.Blue.Bits = 5;
          mFbinfo_layer1.Alpha.Offset = 16;
          mFbinfo_layer1.Alpha.Bits = 0;

          CheckSuccess(gh_FB_check_info(mpFbdrv, &mFbinfo_layer1));
        
          /* try to open with default config */
          E = gh_FB_open(mpFbdrv, &mFbinfo_layer1, &mFbhandle_layer1);
          if (E != Success) {
            VDB_LOG_ERROR("Could not open framebuffer layer1.\n");
            lres = LIB_FAILURE;
          } // if FB open failed
          else
          {
            CheckSuccess(gh_FB_get_info(mFbhandle_layer1, &mFbinfo_layer1));
            // construct FB image
            mBufferImage2.DescriptorSet(GDI_ImageDescriptor(mFbinfo_screen.Width, mFbinfo_screen.Height, RGB565));
            mBufferImage2.mData = (uint8_t*) mFbinfo_layer1.Start;

            mBufferImage.mDesc = mBufferImage2.mDesc;
            mBufferImage.Create();
          } // else: FB open succeeded
        } // if multilayer FB
        else
        {
          VDB_LOG_ERROR("Framebuffer api version < 2 => not supported.\n");
          lres = LIB_FAILURE;
        }  // else: not a multilayer FB
      } // else: get screen info succeeded
    } // if all OK so far		
  } // else: driver designation specified
  return lres;
} // GhsFBEnable()

//*****************************************************************************

LIB_RESULT GDI_Display::GhsFBDisable()
{
  LIB_RESULT lres = LIB_SUCCESS;
  // turn off the display 
  CheckSuccess(gh_FB_set_property(mFbhandle_screen, FB_PROPERTY_VISIBLE, 0));

  // Close FrameBuffer handles
  if (mFbhandle_layer1 )
  {
    CheckSuccess(gh_FB_close(mFbhandle_layer1));
  }	

  CheckSuccess(gh_FB_close(mFbhandle_screen));


  // destroy images
  mBufferImage2.Destroy();
  mBufferImage.Destroy();
  return lres;
} // GhsFBDisable()
#endif //ifndef VLAB
#else // else from ifdef INTEGRITY

//*****************************************************************************

LIB_RESULT GDI_Display::LinuxConsoleModeSet(char const *apCon, uint32_t aMode)
{
  // open console dev file
  LIB_RESULT lres = LIB_SUCCESS;
  OAL_FILE_DESCRIPTOR fdc;
  fdc.fd = -1;

  if (OAL_FileOpen(&fdc,
                   apCon,
                   OAL_FILE_RDWR) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Console %s open failed.\n", apCon);
    if(apCon == NULL)
    {
      VDB_LOG_ERROR("Console path was NULL.\n");
    } // if apCon == NULL
    lres = LIB_FAILURE;
  } else
  {
    // set console to graphics mode
    if (ioctl(fdc.fd,
              KDSETMODE,
              aMode) < 0)
    {
      VDB_LOG_ERROR("Error setting graphics mode to console\n");
      OAL_FileClose(&fdc);
      return LIB_FAILURE;
    } // if ioctl() failed

    // close console dev file
    if (OAL_FileClose(&fdc) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Console close failed.\n");
      lres = LIB_SUCCESS;
    } // if console close failed
  } // else from if console open failed
  return lres;
} // LinuxConsoleModeSet()

//*****************************************************************************

LIB_RESULT GDI_Display::LinuxFBFileOpen(const char * apPath)
{
  LIB_RESULT lres = LIB_SUCCESS;
  if (mFbdevFd.fd < 0)
  {
    if (OAL_FileOpen(&mFbdevFd,
                     apPath,
                     OAL_FILE_RDWR) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("FB dev file (%s) open failed.\n", apPath);
      lres = LIB_FAILURE;
    } // if FileOpen() failed
  } else
  {
    VDB_LOG_WARNING("File opened already.\n");
    lres = LIB_FAILURE;
  } // else from if file opened already
  
  return lres;
} // LinuxFBFileOpen()

//*****************************************************************************

LIB_RESULT GDI_Display::LinuxFBFileClose()
{
  if (mFbdevFd.fd > -1)
  {
    if (OAL_FileClose(&mFbdevFd) == LIB_FAILURE)
    {
      VDB_LOG_ERROR("FB dev file close failed.\n");
      return LIB_FAILURE;
    } // if FileClose() failed
  } // if descriptor opened
  return LIB_SUCCESS;
} // LinuxFBFileClose()

//*****************************************************************************

LIB_RESULT GDI_Display::LinuxFBConnect()
{
  LIB_RESULT ret = LIB_SUCCESS;
  
  if (mFbdevFd.fd > -1)
  {
    if (!ioctl(mFbdevFd.fd,
               FBIOGET_VSCREENINFO,
               &mScreenInfo) && !ioctl(mFbdevFd.fd,
                                       FBIOGET_FSCREENINFO,
                                       &mFixedInfo))
    {
      VDB_LOG_NOTE("v_heigth == %u, v_width == %u\n",
                   mScreenInfo.yres_virtual, mScreenInfo.xres_virtual);
      VDB_LOG_NOTE("height == %u, width == %u\n",
                   mScreenInfo.yres, mScreenInfo.xres);
      VDB_LOG_NOTE("bpp == %u\n",
                   mScreenInfo.bits_per_pixel);
      
      // remeber if we're using the early HDMA driver FB
      mHdmiFb = (strcmp(mFixedInfo.id, "apex_hdmi") == 0) ? 1 : 0;

      GDI_IMAGE_FORMAT pixelFormat = GDI_ImageFormatDeduce(mScreenInfo);
      
      // create framebuffer image descriptor
      // first use xres_virtual for correct widthStep calculation
      GDI_ImageDescriptor tmpImgDsc(mScreenInfo.xres_virtual,
                                    mScreenInfo.yres,
                                    pixelFormat);
      // now change to only visible xres
      tmpImgDsc.mWidth = mScreenInfo.xres;

      // check bits per pixel match
      if (tmpImgDsc.mBpp != mScreenInfo.bits_per_pixel)
      {
        // bites per pixel mismatch mScreenInfo
        VDB_LOG_ERROR(" Image descriptor bpp mismatch mScreenInfo bpp.\n");
        ret = LIB_FAILURE;
      } // if bpp don't match
      else
      {
        // set image descriptor to BufferImage
        mBufferImage2.DescriptorSet(tmpImgDsc);

        // compute size of buffer-image data
        mBufLen = mScreenInfo.yres_virtual * mFixedInfo.line_length;

        // map frame-buffer device image data to BufferImage
        /*mBufferImage2.mData = (uint8_t*) mmap(NULL,
                      mBufLen * (mHdmiFb + 1),
                      PROT_READ | PROT_WRITE,
                      MAP_SHARED,
                      mFbdevFd.fd,
                      0);*/

        mpFbMapBase = (uint8_t*) mmap(NULL,
                    mBufLen * (mHdmiFb + 1),
                    PROT_READ | PROT_WRITE,
                    MAP_SHARED,
                    mFbdevFd.fd,
                    0);
        //  check result of mapping
        if (mpFbMapBase == MAP_FAILED )
        {
          VDB_LOG_ERROR(" Frame-buffer image data mapping failed.\n");
          ret = LIB_FAILURE;
        } // if FB map failed
        else
        {
          // setup GDI visible frame-buffer = back buffer of the FB
          if(mHdmiFb)
          {
            VDB_LOG_NOTE(" This is HDMI FB impelmentation.\n");
            uint8_t lBack = 0;
            // Get back buffer index
            ioctl(mFbdevFd.fd, CMD_GET_BACK, &lBack);
            // Set back buffer address
            mBufferImage2.mData = mpFbMapBase + lBack * mBufLen;
          } // if the HDMI driver FB is used
          else
          {
            mBufferImage2.mData = mpFbMapBase;
          } // else from if the HDMI driver FB is used
          
          // create frame-buffer double image - for GDI to draw to
          mBufferImage.DescriptorSet(tmpImgDsc);	  
          if (mBufferImage.Create() != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Frame-buffer double image creation failed.\n");
            ret = LIB_FAILURE;
          } // if Create() failed
        } // else from FB map failed
      } // else from if bpp don't match
    } // if ioctl() succeeded 
    else
    {
      VDB_LOG_ERROR("Framebuffer ioctls for ScreenInfo and FixedInfo failed.\n");
      ret = LIB_FAILURE;
    } // else from if ioctl
  } 
  else
  {
    VDB_LOG_ERROR("FrameBuffer device file not opened.\n");
    ret = LIB_FAILURE;
  } // else from if fb opened
  return ret;
} // LinuxFBConnect()

//*****************************************************************************

LIB_RESULT GDI_Display::LinuxFBDisconnect()
{
  if (mpFbMapBase != MAP_FAILED )
  {
    if (munmap(mpFbMapBase,
               mBufLen * (mHdmiFb + 1)) != 0)
    {
      VDB_LOG_ERROR(" Unmapping frame-buffer failed.\n");
      return LIB_FAILURE;
    } // if unmap

    mBufferImage2.mData = NULL;
    //mBufferImage.Destroy();
    //mBufferImage.mData = NULL;
    mBufLen = 0;
  } // if BufferImage.Data
  return LIB_SUCCESS; // success even if mBuffer not mapped
} // LinuxFBDisconnect()

//*****************************************************************************

LIB_RESULT GDI_Display::LinuxFBEnable()
{
  LIB_RESULT lres = LIB_SUCCESS;
#ifndef ARM
  // problem with console mode setting on zc702 > only for UBUNTU testing
  // todo: find out cause of the problem (maybe remote access over putty)

  //  set console to GRAPHICS mode
  if (LinuxConsoleModeSet(mpConPath,
      KD_GRAPHICS) == LIB_FAILURE)
  {
    VDB_LOG_ERROR("Server: Setting console to graphics mode failed.\n");
    lres = LIB_FAILURE;
  } // if setting console graphics mode failed
  else
  {
#endif  // ifndef ARM
    //  open framebuffer device file
    if (LinuxFBFileOpen(mpFBDesignation)
        != LIB_SUCCESS)
    {
#ifndef ARM
      // try to switch back to text mode
      LinuxConsoleModeSet(mpConPath,
          KD_TEXT);
#endif  // ifndef ARM
      VDB_LOG_ERROR("Opening Linux frame-buffer file failed.\n");
      lres = LIB_FAILURE;
    } // if FB file open failed
    else
    {
      //  get screeninfo and map framebuffer
      if (LinuxFBConnect() != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Connecting frame-buffer failed.\n");
        // no further checking -> return LIB_FAILURE anyway
#ifndef ARM
        // try to switch back to text mode
        LinuxConsoleModeSet(mpConPath,
            KD_TEXT);
#endif  // ifndef ARM
        // try to close framebuffer file
        LinuxFBFileClose();
        lres = LIB_FAILURE;
      } // if connecting frame buffer failed
    } // else: fb file opened successfully
#ifndef ARM
  } // else: console mode set OK
#endif // ifdef ARM
  //  clear screen
  ColorFill(GDI_ColorRgbaPack(black));
  return lres;
} // LinuxFBEnable()

//*****************************************************************************

LIB_RESULT GDI_Display::LinuxFBDisable()
{
  LIB_RESULT lres = LIB_SUCCESS;

  // disable frame-buffer access
  if (LinuxFBDisconnect() != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Disabling frame-buffer failed.\n");
    lres = LIB_FAILURE;
  } // if fb disconnect failed

  // even if disconnect failed close fb file
  if (LinuxFBFileClose() != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Closing frame-buffer file failed.\n");
    lres = LIB_FAILURE;
  } // if FB file close failed

#ifndef ARM
  // switch back to console text mode
  if (LinuxConsoleModeSet(mpConPath,
      KD_TEXT) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Seting console mode to text failed.\n");
    lres = LIB_FAILURE;
  } // if console mode set failed
#endif  // ifndef ARM

  // destroy images
  mBufferImage2.Destroy();
  mBufferImage.Destroy();

  return lres;
} // LinuxFBDisable()
#endif // else from ifdef INTEGRITY

//*****************************************************************************

bool GDI_Display::FbUpdate(GDI_Rectangle &arRct)
{
  // copy display region to frame-buffer
  bool ret = true;
  GDI_Image lSrcIm, lDstIm;
  uint32_t lDataOffset = arRct.mY * lDstIm.mDesc.mWidthStep
      + (arRct.mX * (lDstIm.mDesc.mBpp >> 3));
  lSrcIm.DescriptorSet(mBufferImage.mDesc);
  lDstIm.DescriptorSet(mBufferImage2.mDesc);

  lSrcIm.mData = mBufferImage.mData + lDataOffset;
  lDstIm.mData = mBufferImage2.mData + lDataOffset;
  
  if (lSrcIm.CopyTo(lDstIm) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Image copy from draw-buffer to frame-buffer failed.\n");
    ret = LIB_FAILURE;
  } // if CopyTo() != LIB_SUCCESS

  lSrcIm.mData = NULL;
  lDstIm.mData = NULL;

  if (mpVpdRegs != NULL)
  {
    // inform video probe
    GDI_VpdUpdateSignal(mpVpdRegs);
  } // if VLAB

#ifndef VLAB
  if(mHdmiFb)
  {
    uint8_t lBack = 0;
    VDB_LOG_NOTE("base: %x, mData: %x, %x mBuflen\n", mpFbMapBase, mBufferImage2.mData, mBufLen);
    // swap buffers
    ioctl(mFbdevFd.fd, CMD_PAINT, &lBack);
    mBufferImage2.mData = mpFbMapBase + lBack * mBufLen;
    VDB_LOG_NOTE("mData: %x, lBack: %x \n", mpFbMapBase, lBack);
  } // if HDMI FB
#endif // #ifndef VLAB  
  return ret;
} // GDI_Display::UpdateFb()

//*****************************************************************************

bool GDI_Display::FbUpdate()
{
  // copy display region to frame-buffer
  (void) memcpy(mBufferImage2.mData,
                mBufferImage.mData,
                mBufferImage.mDesc.mHeight * mBufferImage.mDesc.mWidthStep);
  if (mpVpdRegs != NULL)
  {
    VDB_LOG_NOTE("informing probe.\n");
    // inform video probe
    GDI_VpdUpdateSignal(mpVpdRegs);
  } // if VLAB

#ifndef VLAB  
  if(mHdmiFb)
  {
    uint8_t lBack = 0;
    // swap buffers
    ioctl(mFbdevFd.fd, CMD_PAINT, &lBack);
    mBufferImage2.mData = mpFbMapBase + lBack * mBufLen;
  } // if HDMI FB
#endif // #ifndef VLAB  


  VDB_LOG_NOTE("============ FB Update ============\n");

  return true;
} // GDI_Display::UpdateFb()

//*****************************************************************************

bool GDI_Display::UpdatesOn()
{
  return (mUpdatesOn);
} // Update()

//*****************************************************************************

void GDI_Display::RectDraw(uint32_t aX,
                           uint32_t aY,
                           uint32_t aW,
                           uint32_t aH,
                           GDI_ColorRGBA aColor)
{
  void (*RectFill)(uint32_t,
                   uint32_t,
                   uint32_t,
                   uint32_t,
                   GDI_ColorRGBA,
                   GDI_Image*);

  // choose what drawing function to call
  switch (mBufferImage.mDesc.mPixelFormat)
  {
    case RGBA8888:
    {
      RectFill = &GDI_RectFill_BGRA;
    } // case ARGB8888
      break;
    case RGB888:
    {
      RectFill = &GDI_RectFill_RGB;
    } // case RGB888
      break;
    case BGR565:
    {
      RectFill = &GDI_RectFill_BGR565;
    } // case BGR565
      break;
    default:
    {
      // if unknown pixel format try to find matching bpp
      if (mBufferImage.mDesc.mBpp == 32)
      {
        RectFill = &GDI_RectFill_BGRA;
      } else if (mBufferImage.mDesc.mBpp == 24)
      {
        RectFill = &GDI_RectFill_RGB;
      } else if (mBufferImage.mDesc.mBpp == 16)
      {
        RectFill = &GDI_RectFill_BGR565;
      } else
      {
        VDB_LOG_ERROR("Rectangle drawing doesn't support this pixel format.\n");
        return;
      }
    } // default
      break;
  } // switch pixel format

  RectFill(aX,
           aY,
           aW,
           aH,
           aColor,
           &mBufferImage);
} // RectDraw()

//*****************************************************************************

void GDI_Display::ColorFill(GDI_ColorRGBA aColor)
{
  // remember color
  mBckgColor = aColor;
  // redraw the buffer
  RectDraw(0,
           0,
           mBufferImage.mDesc.mWidth,
           mBufferImage.mDesc.mHeight,
           aColor);
  //mBufferImage.CopyTo(mBufferImage2);
  FbUpdate();
} // GDI_Display::ColorFill()

//*****************************************************************************

//
//  class Window definitions
//

void* GDI_Window::operator new(size_t aSize)
{
  void* storage = OAL_MemoryAlloc(aSize);
  // no_throw setup if commented
  //if(storage == NULL)
  //  throw "memory allocation failed";
  return storage;
} // GDI_Window::operator new()

//*****************************************************************************

void GDI_Window::operator delete(void* apPtr)
{
  OAL_MemoryFree(apPtr);
  return;
} // GDI_Window::operator delete()

//*****************************************************************************

GDI_Window::GDI_Window() :
    mImageShared(false), mDelDiplayedBuffer(true)
{
  // set handle to 0xffffffff;
  mHandle = 0xffffffff;

  // reset display map
  for (int i = 0; i < GDI_DISPLAY_ID_NUM; ++i)
  {
    mDisplayMap[i] = NULL;
  } // for all displays
} // GDI_Window()

//*****************************************************************************

GDI_Window::~GDI_Window()
{
  // destroy all mappings
  GDI_W2dMapPrivate *tmpDisplayMapInfo;
  Gdi_Msg_MD msg;
  for (uint32_t i = 0; i < GDI_DISPLAY_ID_NUM; ++i)
  {
    if ((tmpDisplayMapInfo = mDisplayMap[i]) != NULL)
    {
      msg.mWindowHandle = mHandle;
      msg.mDisplayId = (GDI_DISPLAY_ID) i;
      msg.mLayerId = tmpDisplayMapInfo->mLayer;
      msg.mResult = LIB_FAILURE;
      GDI_Manager::MappingDestroy(&msg);
    } // if mapping exist
  } // for all displays

  // destroy image
  if (mImage.mHwAddress != 0)
  {
    VDB_LOG_NOTE("Contains HW address\n");
#ifdef GDI_CONTIG
    if (!OAL_ContAllocUnmapVirtual(mImage.mHwAddress))
    {
      VDB_LOG_ERROR("Server: unmapping virtual address failed.\n");
    } // else from if GetVirtual() succeeded
    if (!OAL_ContAllocFreePhysical(mImage.mHwAddress))
    {
      VDB_LOG_ERROR("Server: freeing contiguous memory failed.\n");
    } // if FreePhysical() failed
#endif // ifdef GDI_CONTIG
    GDI_OSWBServer::BufferDrop(mImage.mHwAddress);
    mImage.mData = NULL;
    mImage.Destroy();
  } else
  {
    VDB_LOG_NOTE("Doesn't contain HW address\n");
    mImage.Destroy();
  } // else from if contiguous allocation

} //~GDI_Window()

//*****************************************************************************

LIB_RESULT GDI_Window::ImageCreate(const GDI_ImageDescriptor &aImgDsc)
{
  LIB_RESULT lres = LIB_SUCCESS;
  if (mImage.DescriptorSet(aImgDsc) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Setting image descriptor failed.\n");
    lres = LIB_FAILURE;
  } else
  {
    if (mImage.Create() != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Creating image descriptor failed.\n");
      lres = LIB_FAILURE;
    } // if creating image failed
  } // else from if DescriptorSet() failed
  return lres;
} // ImageCreate()

//*****************************************************************************

void GDI_Window::MappingSet(GDI_W2dMapPrivate *mWndMapInfo)
{
  mDisplayMap[mWndMapInfo->mDisplayId] = mWndMapInfo;
} // MappingSet()

//*****************************************************************************

void GDI_Window::MappingReset(GDI_DISPLAY_ID aDisplayId)
{
  mDisplayMap[aDisplayId] = NULL;
  // mapping info memory released by manager
} // MappingReset()

//*****************************************************************************

//
//  class GDI_W2fbMapInfo definitions
//

void* GDI_W2dMapPrivate::operator new(size_t aSize)
{
  void* storage = OAL_MemoryAlloc(aSize);
  // no_throw setup if commented
  //if(storage == NULL)
  //  throw "memory allocation failed";
  return storage;
} // operator new()

//*****************************************************************************

void GDI_W2dMapPrivate::operator delete(void* apPtr)
{
  OAL_MemoryFree(apPtr);
  return;
} // operator delete()

//*****************************************************************************

GDI_W2dMapPrivate::GDI_W2dMapPrivate()
{
  mpWnd = NULL; //  pointer to mapped window
  mDisplayId = GDI_DISPLAY_ID_INVALID;
  mLayer = 0xffffffff; //  to which layer is the window mapped
  //  data mapping info set all to 0xffffffff
  mWindowRect.mX = 0xffffffff;
  mWindowRect.mY = 0xffffffff;
  mWindowRect.mWidth = 0xffffffff;
  mWindowRect.mHeight = 0xffffffff;
  mDisplayRect.mX = 0xffffffff;
  mDisplayRect.mY = 0xffffffff;
  mDisplayRect.mWidth = 0xffffffff;
  mDisplayRect.mHeight = 0xffffffff;
} // GDI_W2dMapPrivate()

//*****************************************************************************

GDI_W2dMapPrivate::GDI_W2dMapPrivate(GDI_Window* apWindow, Gdi_Msg_MC* apMsg)
{
  mpWnd = apWindow; //  pointer to mapped window
  mDisplayId = apMsg->mW2dMapInfo.mDisplayId;
  mLayer = apMsg->mW2dMapInfo.mLayerId; //  to which layer is the window mapped

  //  data mapping info
  mWindowRect.mX = apMsg->mW2dMapInfo.mWindowRectangle.mX;
  mWindowRect.mY = apMsg->mW2dMapInfo.mWindowRectangle.mY;
  mWindowRect.mWidth = apMsg->mW2dMapInfo.mWindowRectangle.mWidth;
  mWindowRect.mHeight = apMsg->mW2dMapInfo.mWindowRectangle.mHeight;
  mDisplayRect.mX = apMsg->mW2dMapInfo.mDisplayRectangle.mX;
  mDisplayRect.mY = apMsg->mW2dMapInfo.mDisplayRectangle.mY;
  mDisplayRect.mWidth = apMsg->mW2dMapInfo.mDisplayRectangle.mWidth;
  mDisplayRect.mHeight = apMsg->mW2dMapInfo.mDisplayRectangle.mHeight;
} // GDI_W2dMapPrivate(apWindow, apMsg)

//*****************************************************************************

GDI_W2dMapPrivate::GDI_W2dMapPrivate(GDI_Window* apWindow,
                                 GDI_DISPLAY_ID const aDisplayId,
                                 uint32_t const aLayerId,
                                 uint32_t const aWindowRegionX,
                                 uint32_t const aWindowRegionY,
                                 uint32_t const aWindowRegionWidth,
                                 uint32_t const aWindowRegionHeight,
                                 uint32_t const aDisplayRegionX,
                                 uint32_t const aDisplayRegionY,
                                 uint32_t const aDisplayRegionWidth,
                                 uint32_t const aDisplayRegionHeight)
{
  mpWnd = apWindow; //  pointer to mapped window
  mDisplayId = aDisplayId;
  mLayer = aLayerId; //  to which layer is the window mapped

  //  data mapping info set all to 0xffffffff
  mWindowRect.mX = aWindowRegionX;
  mWindowRect.mY = aWindowRegionY;
  mWindowRect.mWidth = aWindowRegionWidth;
  mWindowRect.mHeight = aWindowRegionHeight;
  mDisplayRect.mX = aDisplayRegionX;
  mDisplayRect.mY = aDisplayRegionY;
  mDisplayRect.mWidth = aDisplayRegionWidth;
  mDisplayRect.mHeight = aDisplayRegionHeight;
} // GDI_W2dMapPrivate(many)

//*****************************************************************************

GDI_W2dMapPrivate::~GDI_W2dMapPrivate()
{
} // ~GDI_W2dMapPrivate()

//*****************************************************************************

//
//  Definition of GDI_OSWBServer static members
//

#define GDI_OSWB_IDX_LEN 2
#define GDI_OSWB_PREFIX_LEN 6	
#define GDI_OSWB_NAME_LEN GDI_OSWB_IDX_LEN + GDI_OSWB_PREFIX_LEN	///< length of buffers' names
#define GDI_OSWB_NAME_PREFIX "gdiWB"

const uint8_t gcOSWBMaxBufferCnt = GDI_MAX_OSWB_CNT ;		///< maximum allowed number of allocated buffers
const uint32_t GDI_OSWBServer::mscPoolSize = GDI_MAX_OSWB_POOL_SIZE;		///< size (in bytes) of the memory pool used for buffer allocation
uint32_t GDI_OSWBServer::msPoolSizeAvailable = 0;	///< number of bytes available for allocation

const uint8_t* gcpOSWBNamePrefix = (uint8_t*) GDI_OSWB_NAME_PREFIX;	///< prefix of the buffers' names (gdiSWB**)
std::list<uint8_t> GDI_OSWBServer::msFreeBufferList(0);	///< list of allocated indexes
OAL_SHARED_MEM GDI_OSWBServer::mspIndex2ShmMap[GDI_MAX_OSWB_CNT];		///< table for mapping buffer index to OAL shm object

//*****************************************************************************

//
//  GDI_OSWBServer methods
//

void OSWB_Index2Name(uint8_t aIdx, int8_t* const acpName)
{
  // copy name prefix
  int i = 0;
  while(gcpOSWBNamePrefix[i])
  {
    acpName[i] = gcpOSWBNamePrefix[i];
    ++i;
  } // while prefix character not 0

  //set idx
  acpName[i] = '0' + aIdx / 10;
  ++i;
  acpName[i] = '0' + aIdx % 10;
  ++i;
  acpName[i] = 0;

} //OSWB_Index2Name(uint8_t &aIdx, int8_t* const acpName)

//*****************************************************************************

LIB_RESULT GDI_OSWBServer::Init()
{
  LIB_RESULT lres = LIB_SUCCESS;

  msPoolSizeAvailable = mscPoolSize;
  for(int i = 0; i < gcOSWBMaxBufferCnt; ++i)
  {
    msFreeBufferList.push_back(i);
    mspIndex2ShmMap[i] = 0;
  } // for all possible buffers

return lres;
} // LIB_RESULT GDI_OSWBServer::Init()

//*****************************************************************************

// TODO
LIB_RESULT GDI_OSWBServer::Close()
{
  LIB_RESULT lres = LIB_SUCCESS;

  msFreeBufferList.clear();
  msPoolSizeAvailable = 0;
  for (int i = 0; i < gcOSWBMaxBufferCnt; ++i)
  {
    // check if all unmapped
    if ( mspIndex2ShmMap[i] != 0)
    {
      // destroy the OAL shm memory
      if( OAL_SharedMemoryDestroy(mspIndex2ShmMap[i]) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("OAL shm destruction failed.");
        lres = LIB_FAILURE;
      } // if shm destroy failed
      mspIndex2ShmMap[i] = 0;
    } //  if index still mapped
  } // for all possible buffers
  return lres;
} // GDI_OSWBServer::Close()

//*****************************************************************************

LIB_RESULT GDI_OSWBServer::BufferGet(uint32_t aSize, uint32_t &arIdx)
{
  LIB_RESULT lres = LIB_SUCCESS;

  // is there enough memory left?
  if( msPoolSizeAvailable < aSize)
  {
    VDB_LOG_ERROR("Not enough buffer pool memory available.");
    lres = LIB_FAILURE;
  } // if not enough pool left
  else
  {
    // is there a free buffer slot left?
    if( msFreeBufferList.size() < 1)
    {
      VDB_LOG_ERROR("No free buffer slot left.");
      lres = LIB_FAILURE;
    } // if no buffer slot left
    else
    {		
      // get first free buffer slot
      uint32_t idx = msFreeBufferList.front();
      // construct name
      int8_t shmName[GDI_OSWB_NAME_LEN + 1];
      OSWB_Index2Name(idx, shmName);

      // create the shm region
      if(OAL_SharedMemoryCreate(&mspIndex2ShmMap[idx],(const char*)shmName,aSize) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("OAL shm creation failed.");
        lres = LIB_FAILURE;				
      } // if OAL shm creation faied
      else
      {
        // remove it from the free buffer list
        msFreeBufferList.pop_front();
        // set the return index
        arIdx = idx + 1;
      } // else from if OAL shm creation failed
    } // else from no more buffer slots left
  } // else from not enough pool left
  return lres;
} // GDI_OSWBServer::BufferGet(uint32_t aSize, uint32_t &arIdx)

//*****************************************************************************

LIB_RESULT GDI_OSWBServer::BufferGet(uint32_t aIdx, void** appBuffer)
{
  LIB_RESULT lres = LIB_SUCCESS;

  --aIdx;

  if( mspIndex2ShmMap[aIdx] == 0)
  {
    VDB_LOG_ERROR("Requested index not mapped to OAL shm.");
    lres = LIB_FAILURE;
  } // if index not mapped
  else
  {
    (void)OAL_SharedMemoryGetPointer(appBuffer, mspIndex2ShmMap[aIdx]);
  } // else from if index not mapped

  return lres;
} // GDI_OSWBServer::BufferGet(uint32_t aIdx, void** appBuffer)

//*****************************************************************************

LIB_RESULT GDI_OSWBServer::BufferDrop(uint32_t aIdx)
{
  LIB_RESULT lres = LIB_SUCCESS;

  --aIdx;
  VDB_LOG_NOTE("Drop idx: %u\n", aIdx);

  OAL_SHARED_MEM shm = mspIndex2ShmMap[aIdx];
  // check if mapping exists
  if( shm == 0)
  {
    VDB_LOG_ERROR("Given index not mapped to OAL shm.\n");
    lres = LIB_FAILURE;
  } // if index not mapped
  else
  {
    mspIndex2ShmMap[aIdx] = 0;
    uint32_t shm_size = OAL_SharedMemorySizeGet(shm);
    if( OAL_SharedMemoryDestroy(shm) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("OAL shm destruction failed.\n");
      lres = LIB_FAILURE;
    } // if shm destroy failed
    else
    {
      // put the index among free buffer slots again
      msFreeBufferList.push_back(aIdx);
      // mark buffers size as available
      msPoolSizeAvailable += shm_size;
    } // else from if shm destroy failed
  } // else from if index not mapped

  return lres;
} // GDI_OSWBServer::BufferDrop(uint32_t aIdx)

//*****************************************************************************

//
//  Definition of GDI_OSWBServer static members of
//
              
OAL_SHARED_MEM GDI_OSWBClient::mspIndex2ShmMap[GDI_MAX_OSWB_CNT];		///< table for mapping buffer index to OAL shm object

//
//  GDI_OSWBServer methods
//

void GDI_OSWBClient::Init()
{
  for(int i = 0; i < gcOSWBMaxBufferCnt; ++i)
  {
    mspIndex2ShmMap[i] = 0;
  } // for all possible buffers
} // LIB_RESULT GDI_OSWBSClient::Init()

//*****************************************************************************

LIB_RESULT GDI_OSWBClient::Close()
{
  LIB_RESULT lres = LIB_SUCCESS;

  for (int i = 0; i < gcOSWBMaxBufferCnt; ++i)
  {
    // check if all unmapped
    if ( mspIndex2ShmMap[i] != 0)
    {
      // destroy the OAL shm memory
      if( OAL_SharedMemoryDestroy(mspIndex2ShmMap[i]) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("OAL shm destruction failed.");
        lres = LIB_FAILURE;
      } // if shm destroy failed
      mspIndex2ShmMap[i] = 0;
    } //  if index still mapped
  } // for all possible buffers
  return lres;
} // GDI_OSWBClient::Close()

//*****************************************************************************

LIB_RESULT GDI_OSWBClient::BufferGet(uint32_t aSize, uint32_t aIdx)
{
  LIB_RESULT lres = LIB_SUCCESS;

  --aIdx;

  if( aIdx > gcOSWBMaxBufferCnt)
  {
    VDB_LOG_ERROR("Index too high.");
    lres = LIB_FAILURE;
  } // if index too high
  else
  {		
    // check if the slot is free
    if( mspIndex2ShmMap[aIdx] != 0)
    {
      VDB_LOG_ERROR("Index mapped already.");
      lres = LIB_FAILURE;
    } // if mapped already
    else
    {
      // construct name
      int8_t shmName[GDI_OSWB_NAME_LEN + 1];
      OSWB_Index2Name(aIdx, shmName);

      // create the shm region
      if(OAL_SharedMemoryGetByName(&mspIndex2ShmMap[aIdx],(const char*)shmName,aSize) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("OAL shm get by name failed.");
        lres = LIB_FAILURE;				
      } // if OAL shm creation faied
    } // else from if idx mapped already
  } // else from no more buffer slots left

  return lres;
} // GDI_OSWBClient::BufferGet(uint32_t aSize, uint32_t &arIdx)

//*****************************************************************************

LIB_RESULT GDI_OSWBClient::BufferGet(uint32_t aIdx, void** appBuffer)
{
  LIB_RESULT lres = LIB_SUCCESS;

  --aIdx;
  if( mspIndex2ShmMap[aIdx] == 0)
  {
    VDB_LOG_ERROR("Requested index not mapped to OAL shm.");
    lres = LIB_FAILURE;
  } // if index not mapped
  else
  {
    (void)OAL_SharedMemoryGetPointer(appBuffer, mspIndex2ShmMap[aIdx]);
  } // else from if index not mapped

  return lres;
} // GDI_OSWBClient::BufferGet(uint32_t aIdx, void** appBuffer)

//*****************************************************************************

LIB_RESULT GDI_OSWBClient::BufferDrop(uint32_t aIdx)
{
  LIB_RESULT lres = LIB_SUCCESS;

  --aIdx;
  OAL_SHARED_MEM shm = mspIndex2ShmMap[aIdx];
  // check if mapping exists
  if( shm == 0)
  {
    VDB_LOG_ERROR("Given index not mapped to OAL shm.");
    lres = LIB_FAILURE;
  } // if index not mapped
  else
  {
    mspIndex2ShmMap[aIdx] = 0;
    if( OAL_SharedMemoryDestroy(shm) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("OAL shm destruction failed.");
      lres = LIB_FAILURE;
    } // if shm destroy failed
  } // else from if index not mapped

  return lres;
} // GDI_OSWBClient::BufferDrop(uint32_t aIdx)

//*****************************************************************************

//
//  Define static members of GDI_Manager
//

/// Manager class member: array of display instances
GDI_Display GDI_Manager::mDisplays[GDI_DISPLAY_ID_NUM];
/// Manager class memeber: array of fb designation strings
#ifdef INTEGRITY
const char * const GDI_Manager::mppFBDesignations[] =
{
  "imx6_hdmi",	///< LCD1
  0,						///< LCD2
  0,						///< not used
  0,						///< TV OUT NTSC
  0,						///< TV OUT PAL
  0,						///< XGA
  0,						///< VLAB VPD
}; 
#else // from ifdef INTEGRITY
const char * const GDI_Manager::mppFBDesignations[] =
{
  "/dev/fb0",  		///< LCD1
  0,            	///< LCD2
  "/dev/fb_axivdma",	///< AXI_VDMA
  0,            	///< TV OUT NTSC
  0,            	///< TV OUT PAL
  0,            	///< XGA
  0,            	///< VLAB VPD
};
#endif // ifdef INTEGRITY
/// Manager class member: Path to frame-buffer device - just one currently
//const char* GDI_Manager::mpFbPath = "/dev/fb0";
/// Manager class member: Path to console device
const char* GDI_Manager::mpConPath = "/dev/console";
/// Manager class member: number to assigned as next window handle
uint32_t GDI_Manager::mNextHandle = 0;
/// Manager class member: map of handle->window pairs
std::map<uint32_t, GDI_Window*> GDI_Manager::mWindows;
/// Manager class member: 2D array for window->display/layer mapping
GDI_W2dMapPrivate* GDI_Manager::mWndMap[GDI_DISPLAY_ID_NUM][MAX_NUM_OF_LAYERS];

//*****************************************************************************

//
//  GDI_Manager methods
//

LIB_RESULT GDI_Manager::DisplayControl(Gdi_Msg_DC *apMsg)
{
  LIB_RESULT lres = LIB_SUCCESS;

  // check display ID
  if (apMsg->mDisplayId >= GDI_DISPLAY_ID_NUM)
  {
    VDB_LOG_ERROR("Server:  Wrong display id.\n");
    lres = LIB_FAILURE;
  } // if bad display id
  else
  {
    // check if VLAB display
    if (apMsg->mDisplayId == GDI_DISPLAY_VLAB)
    {
      if (apMsg->mEnable)
      {
        if (mDisplays[apMsg->mDisplayId].Enabled())
        {
          // already enabled > just increment number of enable calls
          mDisplays[apMsg->mDisplayId].mEnabledCnt++;
        } // if enabled already
        else
        {
          lres = mDisplays[GDI_DISPLAY_VLAB].BufferEnableVlab();
          mDisplays[apMsg->mDisplayId].mEnabledCnt++;
        } // else: not yet enabled
      } // if enable
      else
      {
        if (mDisplays[apMsg->mDisplayId].mEnabledCnt > 1)
        {
          // other clients still out there > just decrement number of enable calls
          mDisplays[apMsg->mDisplayId].mEnabledCnt--;
        } // if not last client
        else
        {
          lres = mDisplays[GDI_DISPLAY_VLAB].BufferDisableVlab();
          mDisplays[apMsg->mDisplayId].mEnabledCnt--;
        } // else: last client disable
      } // else from if enable
    } // if VLAB
    else
    {
      if (apMsg->mEnable)
      {
        //  Enable display for frame-buffer access
        lres = mDisplays[apMsg->mDisplayId].FBEnable();				
      } // if enable required
      else
      {
        //  Disable display frame-buffer access
        lres = mDisplays[apMsg->mDisplayId].FBDisable();
      } // if display enable
    } // else: not VLAB
  } // if DISPLAY_ID OK
  return lres;
} // DisplayControl(apMsg)

//*****************************************************************************

LIB_RESULT GDI_Manager::DisplayControlUpdate(Gdi_Msg_DC *apMsg)
{
  // check display ID
  if (apMsg->mDisplayId >= GDI_DISPLAY_ID_NUM)
  {
    VDB_LOG_ERROR("server:  Wrong display id.\n");
    return LIB_FAILURE;
  }

  // set the update state directly to specified display
  // redraw whole display if updates enabled
  if ((!mDisplays[apMsg->mDisplayId].mUpdatesOn) && (apMsg->mEnable))
  {
    mDisplays[apMsg->mDisplayId].mUpdatesOn = true;
    GDI_Rectangle rct(0,
                      0,
                      mDisplays[apMsg->mDisplayId].mBufferImage.mDesc.mWidth,
                      mDisplays[apMsg->mDisplayId].mBufferImage.mDesc.mHeight);
    DisplayRedraw(apMsg->mDisplayId,
                  MAX_NUM_OF_LAYERS,
                  rct);
  } else
  {
    if (!apMsg->mEnable)
    {
      mDisplays[apMsg->mDisplayId].mUpdatesOn = false;
    } // if disable updates
  } // if update

  return LIB_SUCCESS;
} // DisplayControlUpdate()

//*****************************************************************************

LIB_RESULT GDI_Manager::DisplayInfoGet(Gdi_Msg_DIG *apMsg)
{
  // don't know how to find out - probably specific framebuffer has to exist

  LIB_RESULT lres = LIB_SUCCESS;

#ifdef VLAB
  VDB_LOG_NOTE("VLAB.\n");
  if(apMsg->mDid == GDI_DISPLAY_VLAB)
  {
    apMsg->mDinfo.mAvailable = true;
  } else
  {
    apMsg->mDinfo.mAvailable = false;
  } // else from if ALL displays
#else
  VDB_LOG_NOTE("LCD1.\n");
  if ((apMsg->mDid == GDI_DISPLAY_LCD1) || (apMsg->mDid == GDI_DISPLAY_AXI_VDMA))
  {
    apMsg->mDinfo.mAvailable = true;
  } else
  {
    apMsg->mDinfo.mAvailable = false;
  } // else from if LCD1
#endif

  if (apMsg->mDinfo.mAvailable)
  {
    apMsg->mDinfo.mActive = mDisplays[apMsg->mDid].Enabled();

    apMsg->mDinfo.mHeightInPixels =
        mDisplays[apMsg->mDid].mBufferImage.mDesc.mHeight;
    apMsg->mDinfo.mWidthInPixels =
        mDisplays[apMsg->mDid].mBufferImage.mDesc.mWidth;

    apMsg->mDinfo.mImageFormat =  mDisplays[apMsg->mDid].mBufferImage.mDesc.mPixelFormat;

    // not used now
    apMsg->mDinfo.mDisplayType = GDI_DISPLAY_TYPE_LCD;
    apMsg->mDinfo.mRotationInDegreesX = 0;
    apMsg->mDinfo.mRotationInDegreesY = 0;
    apMsg->mDinfo.mRotationInDegreesZ = 0;
  } else
  {
    VDB_LOG_WARNING("Display UNavailable.\n");
    lres = LIB_FAILURE;
  } // else form if display available

  return lres;
} // DisplayInfoGet(msgDIG)

//*****************************************************************************

LIB_RESULT GDI_Manager::DisplayColorFill(Gdi_Msg_DCF *apMsg)
{
  LIB_RESULT lres = LIB_SUCCESS;
 GDI_Display &rDisplay = mDisplays[apMsg->mDid];
  if (rDisplay.Enabled())
  {
    //mDisplays[apMsg->mDid].ColorFill(apMsg->mColorRGBA);
    rDisplay.mBckgColor = apMsg->mColorRGBA;
    DisplayRedraw(apMsg->mDid,MAX_NUM_OF_LAYERS, GDI_Rectangle(0,0,rDisplay.mBufferImage.mDesc.mWidth,rDisplay.mBufferImage.mDesc.mHeight));
    
  } else
  {
    VDB_LOG_WARNING("Requested display is disabled.\n");
    lres = LIB_FAILURE;
  } // else from if display enabled

  return lres;
} // DisplayColorFill()

//*****************************************************************************

LIB_RESULT GDI_Manager::DisplayBufferCopy(Gdi_Msg_DBC *apMsg, char* apData)
{
  LIB_RESULT lres = LIB_SUCCESS;

  // check display is enabled
  GDI_Display &disp = mDisplays[apMsg->mDid];
  if (!disp.Enabled())
  {
    VDB_LOG_ERROR("Server:  display disabled.\n");
    return LIB_FAILURE;
  }

  // copy image descriptor to the message
  apMsg->mDesc = disp.mBufferImage.mDesc;

  // check frame buffer fits to shared memory
  size_t isz = disp.mBufferImage.mDesc.mHeight
      * disp.mBufferImage.mDesc.mWidthStep;
  if (isz <= GDI_SHM_IMG_DATA_SIZE)
  {
    // copy data
    memcpy(apData,
           disp.mBufferImage.mData,
           isz);
    //disp.UpdateFb();
  } else
  {
    // report error
    VDB_LOG_ERROR("Server:  display image buffer bigger then image exchange memory area.\n");
    return LIB_FAILURE;
  }

  return lres;
} //  GDI_Manager::DisplayBufferCopy

//*****************************************************************************

LIB_RESULT GDI_Manager::DisplayRedraw(GDI_DISPLAY_ID const mDisplayId,
                                      uint32_t aLayerStart,
                                      const GDI_Rectangle &aRectangle)
{
  // create local reference to processed display
  GDI_Display &display = mDisplays[mDisplayId];

  // check if frame-buffer is mapped and updates enabled
  if (!display.Enabled() || !display.UpdatesOn())
  {
    VDB_LOG_ERROR("Display not enabled.\n");
    return LIB_FAILURE;
  }

  //
  // crop rectangle dimensions if needed
  //

  GDI_Rectangle rct;
  if (aRectangle.mX > display.mBufferImage.mDesc.mWidth)
  {
    // no drawing
    VDB_LOG_WARNING("Window out of display area.\n");
    return LIB_SUCCESS;
  } else
  {
    rct.mX = aRectangle.mX;
  }

  if (aRectangle.mY > display.mBufferImage.mDesc.mHeight)
  {
    // no drawing
    VDB_LOG_WARNING("Window out of display area.\n");
    return LIB_SUCCESS;
  } else
  {
    rct.mY = aRectangle.mY;
  }

  uint32_t xs = aRectangle.mWidth + aRectangle.mX;
  if (xs > display.mBufferImage.mDesc.mWidth)
  {
    // crop
    xs = display.mBufferImage.mDesc.mWidth;
    rct.mWidth = xs - aRectangle.mX;
  } else
  {
    rct.mWidth = aRectangle.mWidth;
  }

  uint32_t ys = aRectangle.mHeight + aRectangle.mY;
  if (ys > display.mBufferImage.mDesc.mHeight)
  {
    // crop
    ys = display.mBufferImage.mDesc.mHeight;
    rct.mHeight = ys - aRectangle.mY;
  } else
  {
    rct.mHeight = aRectangle.mHeight;
  }

  // check starting layer
  if (aLayerStart >= MAX_NUM_OF_LAYERS)
  {
    aLayerStart = MAX_NUM_OF_LAYERS - 1;
  }

  //  if redraw from bottom layer > clear the rectangle
  if (aLayerStart == (MAX_NUM_OF_LAYERS - 1))
  {
    //display.ColorFill(rgb[black]);
    display.RectDraw(rct.mX,
                     rct.mY,
                     rct.mWidth,
                     rct.mHeight,
                     display.mBckgColor);
  }

  VDB_LOG_NOTE("server: display redraw beginning.\n");
  // go through layers from aLayerStart to top (0) for this display and redraw specified area
  for (uint32_t j = aLayerStart + 1; j > 0; --j)
  {
    LIB_RESULT lres = LIB_SUCCESS;
    uint32_t layerIdx = j - 1;
    GDI_W2dMapPrivate *mapInfo = mWndMap[mDisplayId][layerIdx];
    if (mapInfo != NULL)
    {
      // this layer is mapped
      VDB_LOG_NOTE("server: drawing layer %u\n",
                   layerIdx);
      // get shortcut to window image
      GDI_Image *wndImg = &(mapInfo->mpWnd->mImage);

      if (wndImg->mData == NULL)
      {
        // pointer to window image data is NULL
        VDB_LOG_ERROR("server: window image pointer is NULL\n");
        return LIB_FAILURE;
      }

      VDB_LOG_NOTE("======================= layer data %u, ptr: %X\n", layerIdx, wndImg->mData);
      // does this mapping intersect drawing rectangle?
      GDI_Rectangle dxRct;
      size_t sz = GDI_RectIntersect(rct,
                                mapInfo->mDisplayRect,
                                dxRct);

      if (sz == 0)
      {
        // no crosssection => continue
        continue;
      }

      // there is some crosssection
      // find xRct equivalent relative to window region (no resizing involved)
      GDI_Rectangle wxRct;
      wxRct.mX = dxRct.mX - mapInfo->mDisplayRect.mX;
      wxRct.mY = dxRct.mY - mapInfo->mDisplayRect.mY;
      wxRct.mWidth = dxRct.mWidth;
      wxRct.mHeight = dxRct.mHeight;

      //  prepare image describing xsection region in window
      GDI_Image wnd, disp, buff;
      wnd.DescriptorSet(wndImg->mDesc);
      wnd.mDesc.mWidth = wxRct.mWidth;
      wnd.mDesc.mHeight = wxRct.mHeight;
      wnd.mData = (uint8_t*) (wndImg->mData
          + (wxRct.mY * wndImg->mDesc.mWidthStep
              + wxRct.mX * (wndImg->mDesc.mBpp >> 3)));

      //  prepare image describing xsection region in display
      disp.DescriptorSet(display.mBufferImage.mDesc);
      disp.mDesc.mWidth = dxRct.mWidth;
      disp.mDesc.mHeight = dxRct.mHeight;

      disp.mData = display.mBufferImage.mData + dxRct.mY * disp.mDesc.mWidthStep
          + (dxRct.mX * (disp.mDesc.mBpp >> 3));

      // copy window image data to display region
      lres = wnd.CopyTo(disp);

      wnd.mData = NULL;

      if (lres != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Window to display data copy failed\n");
        return LIB_FAILURE;
      } else
      {
        VDB_LOG_NOTE("Window to display data copy succeeded\n");
      }

      buff.mData = NULL;
      disp.mData = NULL;
    } else // if mapInfo
    {
      VDB_LOG_NOTE("server: layer %u not mapped\n",
                   layerIdx);
    }
  } // for layers
  display.FbUpdate();
  return LIB_SUCCESS;
} // GDI_Manager::DisplayRedraw()

//*****************************************************************************

LIB_RESULT GDI_Manager::WindowCreate(Gdi_Msg_WC *apMsg)
{
  //  remember handle number and increment to mNextHandle value
  uint32_t handle = mNextHandle++;

  //  create new window
  GDI_Window *pWnd = new GDI_Window();

  if (!pWnd)
  {
    VDB_LOG_ERROR("Server: allocating window failed\n");
    return LIB_FAILURE;
  }

  //  create window image
  if (pWnd->ImageCreate(apMsg->mImgDesc) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Server: window image creation failed\n");
    delete pWnd;
    return LIB_FAILURE;
  }

  // draw initial window image content = horizontal stripes
  VDB_LOG_NOTE("Server: drawing default stripes %X.\n", pWnd->mImage.mData);
  GDI_StripesDraw(20,
              &(pWnd->mImage));

  //  set handle to window
  pWnd->mHandle = handle;

  //  add new window to map
  mWindows[handle] = pWnd;

  //  set output parameter
  apMsg->mWindowHandle = handle;

  return LIB_SUCCESS;
} // GDI_Manager::WindowCreate(Gdi_Msg_WC*)

//*****************************************************************************

LIB_RESULT GDI_Manager::WindowDestroy(Gdi_Msg_WD *apMsg)
{
  // find window handle
  std::map<uint32_t, GDI_Window*>::iterator it;

  it = mWindows.find(apMsg->mWindowHandle);

  if (it != mWindows.end())
  {
    // destroy & release memory of the window
    delete (it->second);
    mWindows.erase(it);
  }
  //  if aWindowHandle not found -> not in map -> also return LIB_SUCCESS
  return LIB_SUCCESS;
} //  WindowDestroy(Gdi_Msg_WD*)

LIB_RESULT GDI_Manager::MappingCreate(Gdi_Msg_MC *apMsg)
{
  LIB_RESULT lres = LIB_SUCCESS;
  if ((apMsg->mW2dMapInfo.mLayerId < MAX_NUM_OF_LAYERS)
      && (apMsg->mW2dMapInfo.mDisplayId < GDI_DISPLAY_ID_NUM))
  {
    // find window handle
    std::map<uint32_t, GDI_Window*>::iterator it;

    it = mWindows.find(apMsg->mW2dMapInfo.mWindowHandle);

    if (it == mWindows.end())
    {
      VDB_LOG_ERROR("Manager: No such window exists.\n");
      lres = LIB_FAILURE;
    } else
    {
      if (mWndMap[apMsg->mW2dMapInfo.mDisplayId][apMsg->mW2dMapInfo.mLayerId]
          == NULL)
      {
        // generate new window map info
        GDI_W2dMapPrivate *wndMapInfo = new GDI_W2dMapPrivate(it->second,
                                                          apMsg);

        if (!wndMapInfo)
        {
          VDB_LOG_ERROR("Manager: Mapping creation failed.\n");
          lres = LIB_FAILURE;
        } else
        {
          //  remember window position in mWndMap
          mWndMap[wndMapInfo->mDisplayId][wndMapInfo->mLayer] = wndMapInfo;

          //  set Display mapping in Window -> both way link
          wndMapInfo->mpWnd->MappingSet(wndMapInfo);

          // redraw display
          if (DisplayRedraw(apMsg->mW2dMapInfo.mDisplayId,
                            apMsg->mW2dMapInfo.mLayerId,
                            wndMapInfo->mDisplayRect) != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Manager: DisplayRedraw failed.\n");
            lres = LIB_FAILURE;
          } // if DisplayRedraw() failed
        } // else from if !wndMapInfo
      } else
      {
        VDB_LOG_ERROR("Manager: Layer (%u) already mapped.\n",
                      apMsg->mW2dMapInfo.mLayerId);
        lres = LIB_FAILURE;
      } // else from layer not mapped yet
    } // else from if window exists
  } else
  {
    VDB_LOG_ERROR("Manager: No such layer (%u) or display.\n",
                  apMsg->mW2dMapInfo.mLayerId);
    lres = LIB_FAILURE;
  }
  return lres;
} //  MappingCreate(Gdi_Msg_MC*)

//*****************************************************************************

LIB_RESULT GDI_Manager::MappingDestroy(Gdi_Msg_MD *apMsg)
{
  //  get the mapping info
  GDI_W2dMapPrivate *tmp = mWndMap[apMsg->mDisplayId][apMsg->mLayerId];

  if (tmp != NULL) //  no mapping -> LIB_SUCCESS
  {
    //  Reset Display mapping in Window
    tmp->mpWnd->MappingReset(tmp->mDisplayId);

    //  reset mapping
    mWndMap[apMsg->mDisplayId][apMsg->mLayerId] = NULL;

    // redraw display
    DisplayRedraw(apMsg->mDisplayId,
                  MAX_NUM_OF_LAYERS,
                  tmp->mDisplayRect);

    // release memory
    delete tmp;
  } // if mapping exists

  return LIB_SUCCESS;
} //  MappingDestroy(Gdi_Msg_MD*)

//*****************************************************************************

LIB_RESULT GDI_Manager::MappingModify(Gdi_Msg_MC *apMsg)
{
  // check layer
  if (apMsg->mW2dMapInfo.mLayerId >= MAX_NUM_OF_LAYERS)
  {
    VDB_LOG_ERROR("server: no such layer.\n");
    return LIB_FAILURE;
  } // id layer

  // find the mapping
  GDI_W2dMapPrivate *tmp_map =
      mWndMap[apMsg->mW2dMapInfo.mDisplayId][apMsg->mW2dMapInfo.mLayerId];

  if (tmp_map == NULL)
  {
    VDB_LOG_ERROR("Server:  no such mapping exists.\n");
    return LIB_FAILURE;
  }

  // remember old display area
  GDI_Rectangle rct = tmp_map->mDisplayRect;

  // copy mapping data
  tmp_map->mWindowRect.mX = apMsg->mW2dMapInfo.mWindowRectangle.mX;
  tmp_map->mWindowRect.mY = apMsg->mW2dMapInfo.mWindowRectangle.mY;
  tmp_map->mWindowRect.mWidth = apMsg->mW2dMapInfo.mWindowRectangle.mWidth;
  tmp_map->mWindowRect.mHeight = apMsg->mW2dMapInfo.mWindowRectangle.mHeight;
  tmp_map->mDisplayRect.mX = apMsg->mW2dMapInfo.mDisplayRectangle.mX;
  tmp_map->mDisplayRect.mY = apMsg->mW2dMapInfo.mDisplayRectangle.mY;
  tmp_map->mDisplayRect.mWidth = apMsg->mW2dMapInfo.mDisplayRectangle.mWidth;
  tmp_map->mDisplayRect.mHeight = apMsg->mW2dMapInfo.mDisplayRectangle.mHeight;

  // compute redraw rectangle
  uint32_t o_end_x = rct.mX + rct.mWidth;
  uint32_t o_end_y = rct.mY + rct.mHeight;
  uint32_t n_end_x = tmp_map->mDisplayRect.mX + tmp_map->mDisplayRect.mWidth;
  uint32_t n_end_y = tmp_map->mDisplayRect.mY + tmp_map->mDisplayRect.mHeight;

  if (rct.mX > tmp_map->mDisplayRect.mX)
  {
    rct.mX = tmp_map->mDisplayRect.mX;
  } // if x
  if (rct.mY > tmp_map->mDisplayRect.mY)
  {
    rct.mY = tmp_map->mDisplayRect.mY;
  } // if Y

  if (o_end_x < n_end_x)
  {
    rct.mWidth = n_end_x - rct.mX;
  } else
  {
    rct.mWidth = o_end_x - rct.mX;
  } // if width
  if (o_end_y < n_end_y)
  {
    rct.mHeight = n_end_y - rct.mY;
  } else
  {
    rct.mHeight = o_end_y - rct.mY;
  } // if height

  // redraw display
  return GDI_Manager::DisplayRedraw(apMsg->mW2dMapInfo.mDisplayId,
                                    MAX_NUM_OF_LAYERS,
                                    rct);
} //  MappingModify(Gdi_Msg_MC*)

//*****************************************************************************

LIB_RESULT GDI_Manager::WindowColorFill(Gdi_Msg_WCF *apMsg)
{
  //  get window by handle
  std::map<uint32_t, GDI_Window*>::iterator it;

  it = mWindows.find(apMsg->mWindowHandle);

  if (it != mWindows.end())
  {
    // get window shortcut
    GDI_Window *wnd = it->second;

    // get window image
    GDI_Image &img = wnd->mImage;
    // return if image not valid
    if (img.mData == NULL)
    {
      VDB_LOG_ERROR("Window image not valid.\n");
      return LIB_FAILURE;
    }

    // draw rectangle
    GDI_RectFill(apMsg->mWindowRectangle.mX,
             apMsg->mWindowRectangle.mY,
             apMsg->mWindowRectangle.mWidth,
             apMsg->mWindowRectangle.mHeight,
             apMsg->mColorRGBA,
             &img);

    // go through all displays and redraw those the window is mapped to
    for (uint32_t i = 0; i < GDI_DISPLAY_ID_NUM; i++)
    {
      GDI_W2dMapPrivate *map_info = wnd->mDisplayMap[i];
      if (map_info != NULL)
      {
        // mapped to this display > redraw this display

        // prepare redraw rectangle structure
        GDI_Rectangle rct_redraw(apMsg->mWindowRectangle.mX
                                     + map_info->mDisplayRect.mX,
                                 apMsg->mWindowRectangle.mY
                                     + map_info->mDisplayRect.mY,
                                 apMsg->mWindowRectangle.mWidth,
                                 apMsg->mWindowRectangle.mHeight);

        // todo: check result
        DisplayRedraw((GDI_DISPLAY_ID) i,
                      map_info->mLayer,
                      rct_redraw);
      }
    }
    return LIB_SUCCESS;

  }
  return LIB_SUCCESS;
} //  GDI_Manager::WindowColorFill

//*****************************************************************************

LIB_RESULT GDI_Manager::WindowUpdate(Gdi_Msg_WU *apMsg, uint8_t* apData)
{
  LIB_RESULT lres = LIB_SUCCESS;
  //  get window by handle
  std::map<uint32_t, GDI_Window*>::iterator it;

  it = mWindows.find(apMsg->mWindowHandle);

  if (it != mWindows.end())
  {
    // get window shortcut
    GDI_Window *wnd = it->second;

    // get window image
    GDI_Image &img = wnd->mImage;
    // return if image not valid
    if (img.mData == NULL)
    {
      VDB_LOG_ERROR("Server: ERROR: Window image not valid.\n");
      return LIB_FAILURE;
    }

    // check image size
    size_t isz = img.mDesc.mHeight * img.mDesc.mWidthStep;
    if (isz <= GDI_SHM_IMG_DATA_SIZE)
    {
      // copy data
      memcpy(img.mData,
             apData,
             isz);
    } else
    {
      // report error
      VDB_LOG_ERROR("Server: window image bigger then image exchange memory area.\n");
      return LIB_FAILURE;
    }

    VDB_LOG_NOTE("Server: drawing content.\n");
    // go through all displays and redraw those the window is mapped to
    for (uint32_t i = 0; i < GDI_DISPLAY_ID_NUM; i++)
    {
      GDI_W2dMapPrivate *map_info = wnd->mDisplayMap[i];
      if (map_info != NULL)
      {
        // mapped to this display > redraw this display
        VDB_LOG_NOTE("Server: drawing content now.\n");
        // prepare redraw rectangle structure
        GDI_Rectangle rct_redraw(map_info->mDisplayRect.mX,
                                 map_info->mDisplayRect.mY,
                                 map_info->mDisplayRect.mWidth,
                                 map_info->mDisplayRect.mHeight);

        if (DisplayRedraw((GDI_DISPLAY_ID) i,
                          map_info->mLayer,
                          rct_redraw) != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Server: DisplayRedraw failed.\n");
          lres = LIB_FAILURE;
        } // if DisplayRedraw() failed
      } // if mapping exist
    } // for all displays
    return lres;

  }
  return LIB_FAILURE;
} //  GDI_Manager::WindowUpdate

//*****************************************************************************

LIB_RESULT GDI_Manager::WindowUpdateRegion(Gdi_Msg_WUR *apMsg, uint8_t* apData)
{
  LIB_RESULT lres = LIB_SUCCESS;
  //  get window by handle
  std::map<uint32_t, GDI_Window*>::iterator it;

  it = mWindows.find(apMsg->mWindowHandle);

  if (it != mWindows.end())
  {
    // get window shortcut
    GDI_Window *wnd = it->second;

    // get window image
    GDI_Image &img = wnd->mImage;
    // return if image not valid
    if (img.mData == NULL)
    {
      VDB_LOG_ERROR("Server: ERROR: Window image not valid.\n");
      return LIB_FAILURE;
    } // if

    // check rectangle lies inside window area
    if ((apMsg->mWindowRectangle.mX >= img.mDesc.mWidth)
        || (apMsg->mWindowRectangle.mY >= img.mDesc.mHeight))
    {
      VDB_LOG_ERROR("Window rectangle outside window image area.\n");
      return LIB_FAILURE;
    } // if rectangle not inside window area

    GDI_Image src(apMsg->mImageDesc);
    src.mData = apData;

    GDI_Image wndRoi;
    if (img.RoiGet(apMsg->mWindowRectangle,
                   wndRoi) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to get window image ROI.\n");
      return LIB_FAILURE;
    } // if RoiGet() failed

    // copy data
    src.CopyTo(wndRoi);
    VDB_LOG_NOTE("%u bytes of data copied.\n",
                 wndRoi.mDesc.mWidthStep * wndRoi.mDesc.mHeight);

    // go through all displays and redraw those the window is mapped to
    for (uint32_t i = 0; i < GDI_DISPLAY_ID_NUM; i++)
    {
      GDI_W2dMapPrivate *map_info = wnd->mDisplayMap[i];
      if (map_info != NULL)
      {
        // mapped to this display > redraw this display

        // prepare redraw rectangle structure
        GDI_Rectangle rct_redraw(map_info->mDisplayRect.mX,
                                 map_info->mDisplayRect.mY,
                                 map_info->mDisplayRect.mWidth,
                                 map_info->mDisplayRect.mHeight);

        if (DisplayRedraw((GDI_DISPLAY_ID) i,
                          map_info->mLayer,
                          rct_redraw) != LIB_SUCCESS)
        {
          lres = LIB_FAILURE;
        } // if
      } // if
    } // for
    return lres;

  } // if
  return LIB_FAILURE;
} //  GDI_Manager::WindowUpdateRegion

//*****************************************************************************

#ifdef GDI_SWB
#ifdef CONTIG
LIB_RESULT GDI_Manager::WindowBufferGet(Gdi_Msg_WBG *apMsg)
{
  LIB_RESULT lres = LIB_SUCCESS;
  //  get window by handle
  std::map<uint32_t, GDI_Window*>::iterator it;

  it = mWindows.find(apMsg->mWindowHandle);

  if (it != mWindows.end())
  {
    // get window shortcut
    GDI_Window *wnd = it->second;

    // get window image
    GDI_Image &img = wnd->mImage;

    unsigned long img_sz = img.mDesc.mHeight * img.mDesc.mWidthStep;
    unsigned long hw_add = OAL_ContAllocAllocPhysical(img_sz);

    if (hw_add != 0)
    {
      void *virt_add = OAL_ContAllocGetVirtual(hw_add);
      if (virt_add != NULL)
      {
        // fill in message
        apMsg->mDataShift = 0;
        apMsg->mHwAddress = hw_add;
        apMsg->mImageDesc = img.mDesc;

        // copy data
        memcpy(virt_add,
               img.mData,
               img_sz);
      } else
      {
        VDB_LOG_ERROR("Server: Getting virtual address failed.\n");
        lres = LIB_FAILURE;
        if (!OAL_ContAllocFreePhysical(hw_add))
        {
          VDB_LOG_ERROR("Server: Freeing contiguous memory failed.\n");
        } // if FreePhysical() failed
      } // else from if GetVirtual() failed
    } else
    {
      VDB_LOG_ERROR("Server: Contiguous memory allocation failed.\n");
      lres = LIB_FAILURE;
    } // else from if AllocPhysical() ok
  } else
  {
    VDB_LOG_WARNING("Server: Window does not exist.\n");
    lres = LIB_FAILURE;
  } // else from if window exists
  return lres;
} // GDI_Manager::WindowBufferGet(msgWBG)

//*****************************************************************************

LIB_RESULT GDI_Manager::WindowBufferSet(Gdi_Msg_WBS *apMsg)
{
  LIB_RESULT lres = LIB_SUCCESS;
  //  get window by handle
  std::map<uint32_t, GDI_Window*>::iterator it;

  it = mWindows.find(apMsg->mWindowHandle);

  if (it != mWindows.end())
  {
    // get window shortcut
    GDI_Window *wnd = it->second;

    // get window image
    GDI_Image &img = wnd->mImage;

    uint8_t *virt_add = (uint8_t*) OAL_ContAllocGetVirtual(apMsg->mHwAddress);
    if (virt_add != NULL)
    {
      // delete last buffer if required
      if (wnd->mDelDiplayedBuffer)
      {
        if (img.mHwAddress != 0)
        {
          if (!OAL_ContAllocUnmapVirtual(img.mHwAddress))
          {
            VDB_LOG_ERROR("Server: unmapping virtual address failed.\n");
          } // if UnmapVirtual() failed
					
					if (!OAL_ContAllocFreePhysical(img.mHwAddress))
					{
						VDB_LOG_ERROR("Server: Freeing contiguous memory failed.\n");
						lres = LIB_FAILURE;
					} // if FreePhysical() failed
					img.mHwAddress = 0;
        } else
        {
          OAL_MemoryFree(img.mData);
          img.mData = NULL;
        } // else from if contiguous allocation
      } // if delete last buffer

      wnd->mDelDiplayedBuffer = false;
      // set new window image buffer
      // todo: apply any address shift needed
      img.mData = virt_add;
      img.mHwAddress = apMsg->mHwAddress;
      // go through all displays and redraw those the window is mapped to
      for (uint32_t i = 0; i < GDI_DISPLAY_ID_NUM; i++)
      {
        GDI_W2dMapPrivate *map_info = wnd->mDisplayMap[i];
        if (map_info != NULL)
        {
          // mapped to this display > redraw this display
          VDB_LOG_NOTE("Server: drawing content now.\n");
          // prepare redraw rectangle structure
          GDI_Rectangle rct_redraw(map_info->mDisplayRect.mX,
                                   map_info->mDisplayRect.mY,
                                   map_info->mDisplayRect.mWidth,
                                   map_info->mDisplayRect.mHeight);

          if (DisplayRedraw((GDI_DISPLAY_ID) i,
                            map_info->mLayer,
                            rct_redraw) != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Server: DisplayRedraw failed.\n");
            lres = LIB_FAILURE;
          } // if DisplayRedraw() failed
        } // if mapping exist
      } // for all displays
    } else
    {
      VDB_LOG_ERROR("Server: Getting virtual address failed. Previous buffer left in place.\n");
      lres = LIB_FAILURE;
    } // else from if GetVirtual() failed
  } else
  {
    VDB_LOG_WARNING("Server: Window does not exist.\n");
    lres = LIB_FAILURE;
  } // else from if window exists
  return lres;
} // GDI_Manager::WindowBufferSet(msgWBS)

//*****************************************************************************

LIB_RESULT GDI_Manager::WindowBufferDrop(Gdi_Msg_WBD *apMsg)
{
  LIB_RESULT lres = LIB_SUCCESS;
  //  get window by handle
  std::map<uint32_t, GDI_Window*>::iterator it;

  it = mWindows.find(apMsg->mWindowHandle);

  if (it != mWindows.end())
  {
    // get window shortcut
    GDI_Window *wnd = it->second;

    // get window image
    GDI_Image &img = wnd->mImage;

    if (apMsg->mHwAddress == img.mHwAddress)
    {
      // this buffer is in use right now
      wnd->mDelDiplayedBuffer = true; // flag for future deletion
    } else
    {
      if (!OAL_ContAllocUnmapVirtual(apMsg->mHwAddress))
      {
        VDB_LOG_ERROR("Server: CMA unmapping failed.\n");
        lres = LIB_FAILURE;
      } // if unmap failed

      if (!OAL_ContAllocFreePhysical(apMsg->mHwAddress))
      {
        VDB_LOG_ERROR("Server: freeing contiguous memory failed.\n");
      } // if FreePhysical() failed
    } // else from if buffer in use now
  } else
  {
    VDB_LOG_WARNING("Server: Window does not exist.\n");
    lres = LIB_FAILURE;
  } // else from if window exists
  return lres;
} // GDI_Manager::WindowBufferDrop(msgWBD)

//*****************************************************************************

#else // else from ifdef GDI_CONTIG
LIB_RESULT GDI_Manager::WindowBufferGet(Gdi_Msg_WBG *apMsg)
{
  LIB_RESULT lres = LIB_SUCCESS;
  //  get window by handle
  std::map<uint32_t, GDI_Window*>::iterator it;

  it = mWindows.find(apMsg->mWindowHandle);

  if (it != mWindows.end())
  {
    // get window shortcut
    GDI_Window *wnd = it->second;

    // get window image
    GDI_Image &img = wnd->mImage;

    unsigned long img_sz = img.mDesc.mHeight * img.mDesc.mWidthStep;
    uint32_t hw_add;

    if (GDI_OSWBServer::BufferGet(img_sz, hw_add) == LIB_SUCCESS)
    {
      void *virt_add;
      if (GDI_OSWBServer::BufferGet(hw_add, &virt_add) == LIB_SUCCESS)
      {
        // fill in message
        apMsg->mDataShift = 0;
        apMsg->mHwAddress = hw_add;
        apMsg->mImageDesc = img.mDesc;

        // copy data
        memcpy(virt_add,
               img.mData,
               img_sz);
      } // if get buffer address OK
			else
      {
        VDB_LOG_ERROR("Server: Getting window buffer address failed.\n");
        lres = LIB_FAILURE;
        if (GDI_OSWBServer::BufferDrop(hw_add) != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Server: Freeing window buffer failed.\n");
        } // if BufferDrop() failed
      } // else from if get buffer address ok
    } // if get buffer region OK
		else
    {
      VDB_LOG_ERROR("Server: Shared memory window buffer allocation failed.\n");
      lres = LIB_FAILURE;
    } // else from if get buffer region OK
  } else
  {
    VDB_LOG_WARNING("Server: Window does not exist.\n");
    lres = LIB_FAILURE;
  } // else from if window exists
  return lres;
} // GDI_Manager::WindowBufferGet(msgWBG)

//*****************************************************************************

LIB_RESULT GDI_Manager::WindowBufferSet(Gdi_Msg_WBS *apMsg)
{
  LIB_RESULT lres = LIB_SUCCESS;
  //  get window by handle
  std::map<uint32_t, GDI_Window*>::iterator it;

  it = mWindows.find(apMsg->mWindowHandle);

  if (it != mWindows.end())
  {
    // get window shortcut
    GDI_Window *wnd = it->second;

    // get window image
    GDI_Image &img = wnd->mImage;

    uint8_t *virt_add;
    if (GDI_OSWBServer::BufferGet((uint32_t)apMsg->mHwAddress,(void**)&virt_add) == LIB_SUCCESS)
    {
      // delete last buffer if required
      if (wnd->mDelDiplayedBuffer)
      {
        if (img.mHwAddress != 0)
        {
          if (GDI_OSWBServer::BufferDrop(img.mHwAddress) != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Server: Removing previous window buffer failed.\n");
						lres = LIB_FAILURE;
          } // if UnmapVirtual() failed
					img.mHwAddress = 0;
        } else
        {
          GDI_ImageDescriptor idsc = img.mDesc;
          img.Destroy();
          //OAL_MemoryFree(img.mData);
          //img.mData = NULL;
          img.DescriptorSet(idsc);
        } // else from if contiguous allocation
      } // if delete last buffer required

      wnd->mDelDiplayedBuffer = false;
      // set new window image buffer
      // todo: apply any address shift needed
      img.mData = virt_add;
      img.mHwAddress = apMsg->mHwAddress;
      // go through all displays and redraw those the window is mapped to
      for (uint32_t i = 0; i < GDI_DISPLAY_ID_NUM; i++)
      {
        GDI_W2dMapPrivate *map_info = wnd->mDisplayMap[i];
        if (map_info != NULL)
        {
          // mapped to this display > redraw this display
          VDB_LOG_NOTE("Server: drawing content now.\n");
          // prepare redraw rectangle structure
          GDI_Rectangle rct_redraw(map_info->mDisplayRect.mX,
                                   map_info->mDisplayRect.mY,
                                   map_info->mDisplayRect.mWidth,
                                   map_info->mDisplayRect.mHeight);

          if (DisplayRedraw((GDI_DISPLAY_ID) i,
                            map_info->mLayer,
                            rct_redraw) != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Server: DisplayRedraw failed.\n");
            lres = LIB_FAILURE;
          } // if DisplayRedraw() failed
        } // if mapping exist
      } // for all displays
    } else
    {
      VDB_LOG_ERROR("Server: Getting virtual address failed. Previous buffer left in place.\n");
      lres = LIB_FAILURE;
    } // else from if GetVirtual() failed
  } else
  {
    VDB_LOG_WARNING("Server: Window does not exist.\n");
    lres = LIB_FAILURE;
  } // else from if window exists
  return lres;
} // GDI_Manager::WindowBufferSet(msgWBS)

//*****************************************************************************

LIB_RESULT GDI_Manager::WindowBufferDrop(Gdi_Msg_WBD *apMsg)
{
  LIB_RESULT lres = LIB_SUCCESS;
  //  get window by handle
  std::map<uint32_t, GDI_Window*>::iterator it;

  it = mWindows.find(apMsg->mWindowHandle);

  if (it != mWindows.end())
  {
    // get window shortcut
    GDI_Window *wnd = it->second;

    // get window image
    GDI_Image &img = wnd->mImage;

    if (apMsg->mHwAddress == img.mHwAddress)
    {
      // this buffer is in use right now
      wnd->mDelDiplayedBuffer = true; // flag for future deletion
    } else
    {
      if (GDI_OSWBServer::BufferDrop((uint32_t)apMsg->mHwAddress))
      {
        VDB_LOG_ERROR("Server: Window buffer release failed.\n");
        lres = LIB_FAILURE;
      } // if unmap failed
    } // else from if buffer in use now
  } else
  {
    VDB_LOG_WARNING("Server: Window does not exist.\n");
    lres = LIB_FAILURE;
  } // else from if window exists
  return lres;
} // GDI_Manager::WindowBufferDrop(msgWBD)

//*****************************************************************************

#endif //ifdef GDI_CONTIG
#endif //ifdef GDI_SWB

LIB_RESULT GDI_Manager::Init()
{
  // setup display designations
  for(uint32_t i = 0; i<GDI_DISPLAY_ID_NUM; ++i)
  {
    mDisplays[i].mpFBDesignation = mppFBDesignations[i];
#ifndef INTEGRITY
    mDisplays[i].mpConPath = mpConPath;
#endif // ifdef INTEGRITY
  } // for all dispalys

  return LIB_SUCCESS;
}

//*****************************************************************************

LIB_RESULT GDI_Manager::Clean()
{
  LIB_RESULT lres = LIB_SUCCESS;
  //
  // destroy all remaining mappings
  for (uint32_t i = 0; i < GDI_DISPLAY_ID_NUM; ++i)
  {
    for (uint32_t j = 0; j < MAX_NUM_OF_LAYERS; ++j)
    {
      GDI_W2dMapPrivate *tmp = mWndMap[i][j];
      if (tmp != NULL)
      {
        // mapping exists => destroy it

        //  Reset Display mapping in Window
        tmp->mpWnd->MappingReset(tmp->mDisplayId);

        //  reset mapping
        mWndMap[i][j] = NULL;

        delete tmp;
      } // if mapping exists
    } // for all layers
  } // for all displays

  //
  // destroy all windows
  while (mWindows.size() > 0)
  {
    delete (mWindows.begin()->second);
    mWindows.erase(mWindows.begin());
  }  // while all windows

  //
  // destroy all displays
  for (uint32_t i = 0; i < GDI_DISPLAY_ID_NUM; ++i)
  {
    if (mDisplays[i].Enabled())
    {
      if (mDisplays[i].FBDisable() != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Window manager: Display %u disable failed.\n",
                      i);
        lres = LIB_FAILURE;
      } // if DisableBuffer() failed
    } // display enabled
  } // for all displays

  return lres;
} // GDI_Manager::Clean()

//*****************************************************************************

/*EOF*/
