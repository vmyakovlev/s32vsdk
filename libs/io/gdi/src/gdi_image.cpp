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
/**
 * \file     gdi_image.cpp
 * \brief    GDI_Image functionality implementation.
 * \author   Tomas Babinec
 * \version  0.1
 * \date     1.8.2013
 * \note
 ****************************************************************************/

#include <string.h> // because of memcpy
#include "gdi_image.hpp"
#include "vdb_log.h"

GDI_PackedPixel::GDI_PackedPixel() :
    mFormat(GDI_IMAGE_FORMAT_NONE), mData(0)
{
} // GDI_PackedPixel()

GDI_ImageDescriptor::GDI_ImageDescriptor()
{
  mWidth = 0;
  mHeight = 0;
  mWidthStep = 0;
  mBpp = 0;
  mPixelFormat = GDI_IMAGE_FORMAT_NONE;
} // GDI_ImageDescriptor()

// copy constructor
GDI_ImageDescriptor::GDI_ImageDescriptor(const GDI_ImageDescriptor &src) :
    mWidth(src.mWidth),
    mHeight(src.mHeight),
    mWidthStep(src.mWidthStep),
    mBpp(src.mBpp),
    mPixelFormat(src.mPixelFormat)
{
} // GDI_ImageDescriptor(const GDI_ImageDescriptor &)

// parametric constructor
GDI_ImageDescriptor::GDI_ImageDescriptor(uint32_t aWidth,
                                         uint32_t aHeight,
                                         GDI_IMAGE_FORMAT aPixelFormat)
{
  mWidth = aWidth;
  mHeight = aHeight;
  mPixelFormat = aPixelFormat;

  // check pixel format to determine bits per pixel
  switch (mPixelFormat)
  {
    case RGB888:
    case BGR888:
    case YUV444Stream_VUY:
    case YUV444Stream_YUV:
    {
      mBpp = 24;
    }
      break;
    case RGB565:
    case RGB565_8BitAlphaPlane:
    case BGR565:
    case BGR565_8BitAlphaPlane:
    case RGBA4444:
    case ABGR4444:
    case YUV422Stream_UYVY:
    case YUV422Stream_YUYV:
    case YUV422Planar:
    {
      mBpp = 16;
    }
      break;
      // todo: correct bpp representation, don't know right now
    case RGB666:
    case BGR666:
    
    case YUV420Planar:
    case YUV420Planar_Y1_UV2:
    case YUV420Planar_Y1_VU2:
    case YUV420Planar_Y1_U2_V3:
    {
      mBpp = 0;
    }
      break;
    case GS8:
    {
      mBpp = 8;
    }
      break;
    case RGBA8888:
    case ABGR8888:
    {
      mBpp = 32;
    }
      break;
    case GDI_IMAGE_FORMAT_NONE:
    case GDI_IMAGE_FORMAT_INVALID:
    {
      mBpp = 0;
    }
      break;
    default:
    {
      mPixelFormat = GDI_IMAGE_FORMAT_INVALID;
      mBpp = 0;
    }
      break;
  }
  mWidthStep = mWidth * (mBpp >> 3);
} // GDI_ImageDescriptor(uint, uint, GDI_IMAGE_PIXEL_FORMAT)

GDI_ImageDescriptor& GDI_ImageDescriptor::operator = (const GDI_ImageDescriptor& arcSrc)
{
  mWidth = arcSrc.mWidth;
  mHeight = arcSrc.mHeight;
  mWidthStep = arcSrc.mWidthStep;
  mBpp = arcSrc.mBpp;
  mPixelFormat = arcSrc.mPixelFormat;

  return *this;
} // GDI_ImageDescriptor::operator =()

GDI_ImageDescriptor::~GDI_ImageDescriptor()
{
  mWidth = 0;
  mHeight = 0;
  mWidthStep = 0;
  mBpp = 0;
  mPixelFormat = GDI_IMAGE_FORMAT_INVALID;
} // ~GDI_ImageDescriptor()

GDI_Image::GDI_Image() :
    mDelData(false), mHwAddress(0), mDesc(), mData(NULL)
{
} //  GDI_Image()

GDI_Image::GDI_Image(const GDI_ImageDescriptor &arcDesc) :
    mDelData(false), mHwAddress(0), mDesc(arcDesc), mData(NULL)
{
} // GDI_IMage(ImageDescriptor&)

GDI_Image::GDI_Image(uint32_t aWidth,
                     uint32_t aHeight,
                     GDI_IMAGE_FORMAT aPixelFormat) :
    mDelData(false), mHwAddress(0), mDesc(aWidth,
                                          aHeight,
                                          aPixelFormat),
    mData(NULL)
{
} // GDI_Image(width, height, format)

GDI_Image::GDI_Image(const GDI_Image& arcGdiImage)
{
  mDelData = false;
  mHwAddress = arcGdiImage.mHwAddress;
  mDesc = arcGdiImage.mDesc;
  mData = arcGdiImage.mData;
} // GDI_Image(GDI_Image&)

#ifdef GDI_OCV
GDI_Image::GDI_Image(const GDI_Mat& arcGdiMat) :
    mDelData(false), mHwAddress(0), mDesc(arcGdiMat.cols,
                                          arcGdiMat.rows,
                                          arcGdiMat.mPixelFormat),
    mData((uint8_t*) arcGdiMat.data)
{
  // todo: check for contiguous allocation
  // in case its a ROI or step does not match
  mDesc.mWidthStep = (uint32_t) arcGdiMat.step[0];
} // GDI_Image(GDI_Mat&);
#endif

GDI_Image::~GDI_Image()
{
  Destroy();
} //  ~GDI_Image()

LIB_RESULT GDI_Image::DescriptorSet(const GDI_ImageDescriptor &aDesc)
{
  if (mData != NULL)
  {
    //  image data already exist has to be destroyed first
    VDB_LOG_ERROR("Image data exist already. Have to be destroyed first.\n");
    return LIB_FAILURE;
  }
  mDesc = aDesc;
  return LIB_SUCCESS;
} // GDI_Image::DescriptorSet()

LIB_RESULT GDI_Image::Create()
{
  LIB_RESULT lres = LIB_SUCCESS;
  if (mData != NULL)
  {
    // image data exist already
    VDB_LOG_ERROR("image data exist already, need to be destroyed first\n.");
    lres = LIB_FAILURE;
  } else
  {
    // compute size
    uint32_t sz = mDesc.mHeight * mDesc.mWidthStep;
    //todo: do not know, what kind of alignment to choose
    mData =
        (uint8_t*) OAL_MemoryAllocFlag(sz,
                                       OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE));

    if (mData == NULL)
    {
      VDB_LOG_ERROR("memory not allocated > image creation failed\n");
      lres = LIB_FAILURE;
    } // if allocation failed
    else
    {
      mDelData = true;
    } // else from if allocation failed
  } // else from if data exist already
  return lres;
} //  GDI_Image::Create()

GDI_Image GDI_Image::Clone() const
{
  GDI_Image ret(mDesc);
  ret.Create();
  CopyTo(ret);
  return ret;
} // GDI_Image::Clone()

void GDI_Image::Destroy()
{
  if (mDelData)
  {
    mDelData = false;
    VDB_LOG_NOTE("del data \n");
    if (mData != NULL)
    {
      OAL_MemoryFree(mData);
    } // if data exist
  } // if should delete

  mHwAddress = 0;
  mData = NULL;
  mDesc.mHeight = 0;
  mDesc.mWidth = 0;
  mDesc.mWidthStep = 0;
  mDesc.mBpp = 0;
  mDesc.mPixelFormat = GDI_IMAGE_FORMAT_NONE;
} //  GDI_Image::Destroy()

// copy this image data to aDstImg
LIB_RESULT GDI_Image::CopyTo(GDI_Image &aDstImg) const
{
  // check that image bpp match
  if (mDesc.mBpp != aDstImg.mDesc.mBpp)
  {
    VDB_LOG_ERROR("Bpp do not match (%u/%u).\n",
                  mDesc.mBpp, aDstImg.mDesc.mBpp);
    return LIB_FAILURE;
  } // if bpp don't match

  // check that image formats match
  if (mDesc.mPixelFormat != aDstImg.mDesc.mPixelFormat)
  {
    VDB_LOG_WARNING("Pixel formats don't match. Image might be distorted.\n");
  } // if pixel formats don't match

  // check image resolutions match
  if ((mDesc.mHeight != aDstImg.mDesc.mHeight)
      || (mDesc.mWidth != aDstImg.mDesc.mWidth))
  {
    VDB_LOG_ERROR("Resolution does not match.\n");
    return LIB_FAILURE;
  } // if dimensions don't match

  // prepare to copy
  uint8_t *src = mData;
  uint8_t *dst = aDstImg.mData;
  size_t sz = mDesc.mWidth * (mDesc.mBpp >> 3);

  // decide if copy at once possible
  if (mDesc.mWidthStep == aDstImg.mDesc.mWidthStep)
  {
    //printf("copy at once\n");
    // copy data at once
    (void) memcpy(dst,
                  src,
                  mDesc.mHeight * mDesc.mWidthStep);
  }else
  {
    //printf("copy by rows\n");
    // copy each row
    // has to be done by rows in case image descriptor is set to only a sub image
    for (uint32_t i = 0; i < mDesc.mHeight; ++i)
    {
      // copy data
      (void) memcpy(dst,
                    src,
                    sz);
      // move to next row
      src += mDesc.mWidthStep;
      dst += aDstImg.mDesc.mWidthStep;

    } // for all rows
  } // else from if copy at once possible
  return LIB_SUCCESS;
} // GDI_Image::CopyTo()

LIB_RESULT GDI_Image::PixelPut(uint32_t aX, uint32_t aY, uint8_t *apPixData)
{
  if (!mData || !apPixData)
  {
    VDB_LOG_ERROR("Bad pointers.\n");
    return LIB_FAILURE;
  } // if bad pointers

  if ((aX >= mDesc.mWidth) && (aY >= mDesc.mHeight))
  {
    VDB_LOG_ERROR("Coordinates overflow.\n");
    return LIB_FAILURE;
  } // if coordinates overflow

  // get image pixel pointer
  uint8_t* pix_ptr = mData + aY * mDesc.mWidthStep + aX * (mDesc.mBpp >> 3);
  (void) memcpy(pix_ptr,
                apPixData,
                mDesc.mBpp >> 3);

  return LIB_SUCCESS;
} // GDI_Image::PixelPut()

LIB_RESULT GDI_Image::RoiGet(const GDI_Rectangle &arcRectangle, GDI_Image &arRoiImage)
{
  LIB_RESULT lres = LIB_SUCCESS;
  if ((mDesc.mWidth >= (arcRectangle.mX + arcRectangle.mWidth))
      && (mDesc.mHeight >= (arcRectangle.mY + arcRectangle.mHeight)))
  {
    arRoiImage.mDesc.mWidth = arcRectangle.mWidth;
    arRoiImage.mDesc.mHeight = arcRectangle.mHeight;
    arRoiImage.mDesc.mBpp = mDesc.mBpp;
    arRoiImage.mDesc.mPixelFormat = mDesc.mPixelFormat;
    arRoiImage.mDesc.mWidthStep = mDesc.mWidthStep;

    arRoiImage.mData = mData + arcRectangle.mX * (mDesc.mBpp >> 3)
        + arcRectangle.mY * mDesc.mWidthStep;

    arRoiImage.mDelData = false;
  } else
  {
    VDB_LOG_ERROR("Bad roi requested. Rectangle overlaps source image.\n");
    lres = LIB_FAILURE;
  } // else from if bad roi requested

  return lres;
} // RoiGet()

bool GDI_Image::DataOwnershipGet()
{
  return mDelData;
} // GDI_Image::DataOwnershipGet()