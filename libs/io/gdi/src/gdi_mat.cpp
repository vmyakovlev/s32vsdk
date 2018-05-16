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
 * \file     gdi_mat.cpp
 * \brief    GDI_Mat functionality implementation.
 * \author   Tomas Babinec
 * \version  0.1
 * \date     1.8.2013
 * \note
 ****************************************************************************/

#include <string.h> // because of memcpy
#include "gdi_image.hpp"
#include "vdb_log.h"

//
// GDI_Mat
//

#ifdef GDI_OCV
LIB_RESULT GDI_ImageFormat2CvType(GDI_IMAGE_FORMAT aGdiImageFormat,
                                  int &arCvType)
{
  LIB_RESULT lres = LIB_SUCCESS;
  switch (aGdiImageFormat)
  {
    case ABGR8888:
    case RGBA8888:
    {
      arCvType = CV_8UC4;
    } // case RGBA8888
      break;
    case BGR888:
    case RGB888:
    {
      arCvType = CV_8UC3;
    } // case RGB888
      break;
    case BGR565:
    {
      arCvType = CV_8UC2;
    } // case BGR565
      break;
    default:
    {
      lres = LIB_FAILURE;
    } // default
      break;
  } // switch(gdiImageFormat)
  return lres;
}

// gdi_ImageFormat2CV_TYPE()

//
// GDI_Mat()
//

GDI_Mat::GDI_Mat() :
    mHwAddress(0), mPixelFormat(GDI_IMAGE_FORMAT_INVALID)
{
} // GDI_Mat()

GDI_Mat::GDI_Mat(uint32_t aWidth,
                 uint32_t aHeight,
                 GDI_IMAGE_FORMAT aPixelFormat)
{
  initEmpty();

  int cv_type;
  if (GDI_ImageFormat2CvType(aPixelFormat,
                             cv_type) == LIB_SUCCESS)
  {
    Mat::create(aHeight,
                aWidth,
                cv_type);
  } // if ImageFormat compatible
  mHwAddress = 0;
  mPixelFormat = aPixelFormat;
} // GDI_Mat(int,int,int)

GDI_Mat::GDI_Mat(const GDI_Mat &arcMat) :
    Mat(arcMat), mHwAddress(0), mPixelFormat(arcMat.mPixelFormat)
{
} // GDI_Mat(const GDI_Mat&)

GDI_Mat::GDI_Mat(const GDI_Mat& arcM, const cv::Rect& arcRoi) :
    cv::Mat(arcM,
            arcRoi),
    mHwAddress(0), mPixelFormat(arcM.mPixelFormat)
{
} // GDI_Mat(const GDI_Mat&, const cv::Rect&)

GDI_Mat::GDI_Mat(uint32_t aWidth,
                 uint32_t aHeight,
                 void* apData,
                 GDI_IMAGE_FORMAT aImageFormat,
                 size_t) :
    mHwAddress(0)
{
  initEmpty();
  int32_t cv_type;
  if (GDI_ImageFormat2CvType(aImageFormat,
                             cv_type) == LIB_SUCCESS)
  {
    *((cv::Mat*) this) = Mat(aHeight,
                             aWidth,
                             cv_type,
                             apData);
  } // if compatible GDI format
  mPixelFormat = aImageFormat;
} // GDI_Mat(user data)

GDI_Mat::GDI_Mat(const GDI_Image &arcGdiImage)
{
  int32_t cv_type;
  if (GDI_ImageFormat2CvType(arcGdiImage.mDesc.mPixelFormat,
                             cv_type) == LIB_SUCCESS)
  {
    *((cv::Mat*) this) = Mat(arcGdiImage.mDesc.mHeight,
                             arcGdiImage.mDesc.mWidth,
                             cv_type,
                             arcGdiImage.mData,
                             arcGdiImage.mDesc.mWidthStep);
  } else
  {
    initEmpty();
  } // else from if compatible GDI format
  mHwAddress = arcGdiImage.mHwAddress;
  mPixelFormat = arcGdiImage.mDesc.mPixelFormat;
} // GDI_Mat(GDI_Image)

cv::Mat& GDI_Mat::operator =(const cv::Mat& arcM)
{
  cv::Mat::operator =(arcM);
  return *this;
} // GDI_Mat::operator = (const cv::Mat&)

GDI_Mat& GDI_Mat::operator =(const GDI_Mat& arcGDI_Mat)
{
  cv::Mat::operator =(static_cast<const cv::Mat&>(arcGDI_Mat));
  mHwAddress = arcGDI_Mat.mHwAddress;
  mPixelFormat = arcGDI_Mat.mPixelFormat;
  return *this;
} // GDI_Mat::operator = (const GDI_Mat&)

GDI_Mat GDI_Mat::clone()
{
  GDI_Mat tmp;
  Mat *tmp_p = (Mat*) &tmp;
  *tmp_p = ((Mat*) this)->clone();
  tmp.mPixelFormat = mPixelFormat;
  return tmp;
} // GDI_Mat::clone()

GDI_Mat::~GDI_Mat()
{
  mHwAddress = 0;
  mPixelFormat = GDI_IMAGE_FORMAT_INVALID;
} // ~GDI_Mat()
#endif
