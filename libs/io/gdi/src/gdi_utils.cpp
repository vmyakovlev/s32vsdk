/*****************************************************************************
 *
 * Freescale Confidential Proprietary
 *
 * Copyright (c) 2013 Freescale Semiconductor;
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
 * \file 		gdi_utils.cpp
 * \brief 		GDI api utilities definition
 * \author 		Tomas Babinec
 * \version 	0.1
 * \date 		23-07-2013
 ****************************************************************************/

#include <string.h>
#include "gdi_image.hpp"
#include "gdi_utils.hpp"
#include "vdb_log.h"

// GDI_COLOR_PALLETTE RGB pixels data definition
const uint8_t gdi_color_pallette_rgb[][3] = {
    { (uint8_t) 0,   (uint8_t) 0,   (uint8_t) 0 }, // black    0
    { (uint8_t) 128, (uint8_t) 128, (uint8_t) 128 }, // gray     1
    { (uint8_t) 192, (uint8_t) 192, (uint8_t) 192 }, // silver   2
    { (uint8_t) 255, (uint8_t) 255, (uint8_t) 255 }, // white    3
    { (uint8_t) 128, (uint8_t) 0,   (uint8_t) 0 }, // maroon   4
    { (uint8_t) 255, (uint8_t) 0,   (uint8_t) 0 }, // red      5
    { (uint8_t) 128, (uint8_t) 128, (uint8_t) 0 }, // olive    6
    { (uint8_t) 255, (uint8_t) 255, (uint8_t) 0 }, // yellow   7
    { (uint8_t) 0,   (uint8_t) 128, (uint8_t) 0 }, // green    8
    { (uint8_t) 0,   (uint8_t) 255, (uint8_t) 0 }, // lime     9
    { (uint8_t) 0,   (uint8_t) 128, (uint8_t) 128 }, // teal     10
    { (uint8_t) 0,   (uint8_t) 255, (uint8_t) 255 }, // aqua     11
    { (uint8_t) 0,   (uint8_t) 0,   (uint8_t) 128 }, // navy     12
    { (uint8_t) 0,   (uint8_t) 0,   (uint8_t) 255 }, // blue     13
    { (uint8_t) 128, (uint8_t) 0,   (uint8_t) 128 }, // purple   14
    { (uint8_t) 255, (uint8_t) 0,   (uint8_t) 255 }, // fuchsia  15
    };

GDI_ColorRGBA GDI_ColorRgbaPack(GDI_COLOR_PALLETTE aColor, uint8_t aAlpha)
{
  GDI_ColorRGBA ret;
  uint8_t *aux = (uint8_t*)&ret;

  aux[0] = gdi_color_pallette_rgb[aColor][0];
  aux[1] = gdi_color_pallette_rgb[aColor][1];
  aux[2] = gdi_color_pallette_rgb[aColor][2];
  aux[3] = aAlpha;

  return ret;
} // GDI_ColorRgbaPack()

GDI_ColorRGBA GDI_ColorRgbaPack(uint8_t aR, uint8_t aG, uint8_t aB, uint8_t aAlpha)
{
  GDI_ColorRGBA ret;
  uint8_t *aux = (uint8_t*)&ret;

  aux[0] = aR;
  aux[1] = aG;
  aux[2] = aB;
  aux[3] = aAlpha;

  return ret;
} // GDI_ColorRgbaPack()

// GDI_Rectangle constructors

GDI_Rectangle::GDI_Rectangle() :
    mX(0), mY(0), mWidth(0), mHeight(0)
{
}

GDI_Rectangle::GDI_Rectangle(uint32_t aX,
                             uint32_t aY,
                             uint32_t aWidth,
                             uint32_t aHeight) :
    mX(aX), mY(aY), mWidth(aWidth), mHeight(aHeight)
{
}

// GDI_W2dMapInfo constructor

GDI_W2dMapInfo::GDI_W2dMapInfo() :
    mWindowHandle(0), mDisplayId(GDI_DISPLAY_ID_INVALID), mLayerId(0), mWindowRectangle(), mDisplayRectangle()
{
}

// GDI utility functions definitions
void GDI_RectFill_BGR565(uint32_t aX,
                     uint32_t aY,
                     uint32_t aW,
                     uint32_t aH,
                     GDI_ColorRGBA aRgb888,
                     GDI_Image *apImg)
{
  // check bytes per pixel

  uint32_t bytesPerPixel = apImg->mDesc.mBpp >> 3;
  if (bytesPerPixel != 2)
  {
    VDB_LOG_ERROR("Image has to have 2bytes per pixel.\n");
    return;
  }

  // prepare pixel content bitwise
  uint8_t *pRgb = (uint8_t*)&aRgb888;
  uint16_t pix = ((short) pRgb[0]) >> 3;       // red
  pix |= (((short) pRgb[1]) << 3) & 0x7e0;  // green
  pix |= (((short) (pRgb[2] & 0xf8)) << 8); // blue

  uint32_t xs = aX + aW, ys = aY + aH;
  if ((xs - 1) > apImg->mDesc.mWidth)
  {
    xs = apImg->mDesc.mWidth;
  }
  if ((ys - 1) > apImg->mDesc.mHeight)
  {
    ys = apImg->mDesc.mHeight;
  }

  // set first row
  uint32_t row_length = aW * bytesPerPixel;
  uint8_t* dst8 = apImg->mData + aY * apImg->mDesc.mWidthStep + aX * bytesPerPixel;
  uint8_t* first_row = dst8;
  uint16_t* dst16 = (uint16_t*)dst8;

  for (uint32_t i = aX; i < xs; ++i)
  {
    *dst16 = pix;
    dst16++;
  } // for pixels in first row

  dst8 += apImg->mDesc.mWidthStep;
  for (uint32_t i = aY + 1; i < ys; ++i)
  {
    memcpy(dst8, first_row, row_length);
    dst8 += apImg->mDesc.mWidthStep;
  } // for remaining rows

  /*for (uint32_t j = aY; j < ys; ++j)
  {
    for (uint32_t i = aX; i < xs; ++i)
    {
      uint32_t addr = j * apImg->mDesc.mWidthStep + i * bytesPerPixel;
      *((uint16_t*) (&apImg->mData[addr])) = pix;
    }
  }*/
} // RectFill_BGR565()

void GDI_RectFill_BGRA(uint32_t aX,
                   uint32_t aY,
                   uint32_t aW,
                   uint32_t aH,
                   GDI_ColorRGBA aRgba,
                   GDI_Image *apImg)
{
  // check number of channels is 4
  uint32_t bytesPerPixel = apImg->mDesc.mBpp >> 3;
  if (bytesPerPixel < 4)
  {
    VDB_LOG_NOTE("wrong number of bytes\n");
    return;
  }

  uint32_t xs = aX + aW, ys = aY + aH;
  if ((xs - 1) > apImg->mDesc.mWidth)
  {
    xs = apImg->mDesc.mWidth;
  }
  if ((ys - 1) > apImg->mDesc.mHeight)
  {
    ys = apImg->mDesc.mHeight;
  }

  uint8_t *pRgba = (uint8_t*)&aRgba;
  for (uint32_t j = aY; j < ys; ++j)
  {
    for (uint32_t i = aX; i < xs; ++i)
    {
     // VDB_LOG_NOTE("o");
      uint32_t addr = j * apImg->mDesc.mWidthStep + i * bytesPerPixel;
      //VDB_LOG_NOTE("x");
      apImg->mData[addr] = pRgba[2];
      apImg->mData[addr + 1] = pRgba[1];
      apImg->mData[addr + 2] = pRgba[0];
      apImg->mData[addr + 3] = pRgba[3];
    }
  }
} // RectFill_BGRA()

void GDI_RectFill_RGB(uint32_t aX,
                  uint32_t aY,
                  uint32_t aW,
                  uint32_t aH,
                  GDI_ColorRGBA aRgb,
                  GDI_Image *apImg)
{
  // check number of channels is 3
  uint32_t bytesPerPixel = apImg->mDesc.mBpp >> 3;
  if (bytesPerPixel < 3)
  {
    VDB_LOG_NOTE("Bad pixel format (bpp).\n");
    return;
  }

  uint32_t xs = aX + aW, ys = aY + aH;
  if ((xs - 1) > apImg->mDesc.mWidth)
  {
    xs = apImg->mDesc.mWidth;
  }
  if ((ys - 1) > apImg->mDesc.mHeight)
  {
    ys = apImg->mDesc.mHeight;
  }

  uint8_t *pRgb = (uint8_t*)&aRgb;
  for (uint32_t j = aY; j < ys; ++j)
  {
    for (uint32_t i = aX; i < xs; ++i)
    {
      uint32_t addr = j * apImg->mDesc.mWidthStep + i * bytesPerPixel;
      apImg->mData[addr] = pRgb[0];
      apImg->mData[addr + 1] = pRgb[1];
      apImg->mData[addr + 2] = pRgb[2];
    }
  }
} // RectFill_RGB()

void GDI_RectFill(uint32_t aX,
                  uint32_t aY,
                  uint32_t aW,
                  uint32_t aH,
                  GDI_ColorRGBA aColor,
                  GDI_Image *apImg)
{
  void (*FillFcn)(uint32_t,
                  uint32_t,
                  uint32_t,
                  uint32_t,
                  GDI_ColorRGBA,
                  GDI_Image*);

  // choose what drawing function to call
  switch (apImg->mDesc.mPixelFormat)
  {
    case RGBA8888:
    {
      FillFcn = &GDI_RectFill_BGRA;
    } // case ARGB8888
      break;
    case RGB888:
    {
      FillFcn = &GDI_RectFill_RGB;
    } // case RGB888
      break;
    case BGR565:
    {
      FillFcn = &GDI_RectFill_BGR565;
    } // case BGR565
      break;
    default:
    {
      // if unknown pixel format try to find matching bpp
      if (apImg->mDesc.mBpp == 32)
      {
        FillFcn = &GDI_RectFill_BGRA;
      } else if (apImg->mDesc.mBpp == 24)
      {
        FillFcn = &GDI_RectFill_RGB;
      } else if (apImg->mDesc.mBpp == 16)
      {
        FillFcn = &GDI_RectFill_BGR565;
      } else
      {
        VDB_LOG_ERROR("Rectangle drawing doesn't support pixel format of the image.\n");
        return;
      }
    } // default
      break;
  } // switch pixel format
  FillFcn(aX,aY,aW,aH,aColor,apImg);
} // RectFill()

void GDI_StripesDraw(int aStripeHeight, GDI_Image *apImg)
{
  uint32_t stripesNum = apImg->mDesc.mHeight / aStripeHeight;
  void (*RectFill)(uint32_t, uint32_t, uint32_t, uint32_t, GDI_ColorRGBA, GDI_Image*);

  // choose what drawing function to call
  switch (apImg->mDesc.mPixelFormat)
  {
    case RGBA8888:
    {
      RectFill = &GDI_RectFill_BGRA;
    }
      break;
    case BGR565:
    {
      RectFill = &GDI_RectFill_BGR565;
    }
      break;
    case RGB888:
    {
      RectFill = &GDI_RectFill_RGB;
    }
      break;
    default:
    {
      if (apImg->mDesc.mBpp == 32)
      {
        RectFill = &GDI_RectFill_BGRA;
      } else if (apImg->mDesc.mBpp == 24)
      {
        RectFill = &GDI_RectFill_RGB;
      } else if (apImg->mDesc.mBpp == 16)
      {
        RectFill = &GDI_RectFill_BGR565;
      } else
      {
        VDB_LOG_ERROR("Stripes drawing doesn't support this pixel format.\n");
        return;
      }
    }
      break;
  }

  for (uint32_t i = 0; i < stripesNum; ++i)
  {
    RectFill(0,
             i * aStripeHeight,
             apImg->mDesc.mWidth,
             aStripeHeight,
             GDI_ColorRgbaPack(GDI_COLOR_PALLETTE((i % 15) + 1)),
             apImg);
  }
  // draw what's left
  RectFill(0,
           stripesNum * aStripeHeight,
           apImg->mDesc.mWidth,
           apImg->mDesc.mHeight - (aStripeHeight * stripesNum),
           GDI_ColorRgbaPack(GDI_COLOR_PALLETTE((stripesNum % 7) + 1)),
           apImg);
} // StipesDraw()

// rectangles intersection
size_t GDI_RectIntersect(const GDI_Rectangle &aRct1,
                     const GDI_Rectangle &aRct2,
                     GDI_Rectangle &aRctOut)
{

  uint32_t xs1 = aRct1.mWidth + aRct1.mX;
  uint32_t xs2 = aRct2.mWidth + aRct2.mX;
  uint32_t ys1 = aRct1.mHeight + aRct1.mY;
  uint32_t ys2 = aRct2.mHeight + aRct2.mY;

  // check x coords
  if (aRct1.mX > aRct2.mX)
  {
    aRctOut.mX = aRct1.mX;
    // check widths
    if (xs1 > xs2)
    {
      // check if any overlap at all
      if (xs2 < aRct1.mX)
      {
        // no overlap
        aRctOut.mWidth = 0;
      } else
      {
        aRctOut.mWidth = xs2 - aRct1.mX;
      }
    } else
    {
      // width the same as Rct1
      aRctOut.mWidth = aRct1.mWidth;
    }
  } else
  {
    aRctOut.mX = aRct2.mX;
    // check widths
    if (xs2 > xs1)
    {
      // check if any overlap at all
      if (xs1 < aRct2.mX)
      {
        // no overlap
        aRctOut.mWidth = 0;

      } else
      {
        // yes there is
        aRctOut.mWidth = xs1 - aRct2.mX;
      }
    } else
    {
      // width the same as Rct1
      aRctOut.mWidth = aRct2.mWidth;
    }
  }

  // check y coords
  if (aRct1.mY > aRct2.mY)
  {
    aRctOut.mY = aRct1.mY;
    // check widths
    if (ys1 > ys2)
    {
      // check if any overlap at all
      if (ys2 < aRct1.mY)
      {
        // no overlap
        aRctOut.mHeight = 0;
      } else
      {
        // yes there is
        aRctOut.mHeight = ys2 - aRct1.mY;
      }
    } else
    {
      // height the same as Rct1
      aRctOut.mHeight = aRct1.mHeight;
    }
  } else
  {
    aRctOut.mY = aRct2.mY;
    // check heights
    if (ys2 > ys1)
    {
      // check if any overlap at all
      if (ys1 < aRct2.mY)
      {
        // no overlap
        aRctOut.mHeight = 0;

      } else
      {
        // yes there is
        aRctOut.mHeight = ys1 - aRct2.mY;
      }
    } else
    {
      // height the same as Rct1
      aRctOut.mHeight = aRct2.mHeight;
    }
  }

  // return overlap area
  return (aRctOut.mWidth * aRctOut.mHeight);
} // RectIntersect()

#ifndef INTEGRITY
GDI_IMAGE_FORMAT GDI_ImageFormatDeduce(const struct fb_var_screeninfo &aScrInfo)
{
  GDI_IMAGE_FORMAT pixelFormat = GDI_IMAGE_FORMAT_INVALID;

  VDB_LOG_NOTE("ImageFormatDeduce result: ");
  switch (aScrInfo.bits_per_pixel)
  {
    case 8:
    {
      // check if grayscale
      if (aScrInfo.grayscale == 1)
      {
        pixelFormat = GS8;
        VDB_LOG_NOTE("GS8\n");
      } else
      {
        // probably colormap
        // not supported now
        pixelFormat = GDI_IMAGE_FORMAT_INVALID;
      }
    }
      break;
    case 16:
    {
      // check transparency > if alpha plane present
      if (aScrInfo.transp.length)
      {
        // there is alpha plane
        // check color plane order
        if (aScrInfo.red.offset == 0)
        {
          // red is first
          pixelFormat = RGB565_8BitAlphaPlane;
          VDB_LOG_NOTE("RGB565_8BitAlphaPlane\n");
          break;
        }
        if (aScrInfo.blue.offset == 0)
        {
          // blue is first
          pixelFormat = BGR565_8BitAlphaPlane;
          VDB_LOG_NOTE("BGR565_8BitAlphaPlane\n");
          break;
        }
        if ((aScrInfo.transp.offset == 0) && (aScrInfo.transp.length == 4))
        {
          pixelFormat = RGBA4444;
          VDB_LOG_NOTE("RGBA4444\n");
          break;
        } else
        {
          // unknown > invalid format
          pixelFormat = GDI_IMAGE_FORMAT_INVALID;
        }
      } else
      {
        // there is no alpha plane
        // check color plane order
        if (aScrInfo.red.offset == 0)
        {
          // red is first
          pixelFormat = RGB565;
          VDB_LOG_NOTE("RGB565\n");
          break;
        }
        if (aScrInfo.blue.offset == 0)
        {
          // blue is first
          pixelFormat = BGR565;
          VDB_LOG_NOTE("BGR565\n");
          break;
        } else
        {
          // unknown > invalid format
          pixelFormat = GDI_IMAGE_FORMAT_INVALID;
        }
      }
    }
      break;
    case 24:
    {
      // check color plane order
      if (aScrInfo.red.offset == 0)
      {
        // red is first
        pixelFormat = RGB888;
        VDB_LOG_NOTE("RGB\n");
        break;
      }
      if (aScrInfo.blue.offset == 0)
      {
        // blue is first
        pixelFormat = BGR888;
        VDB_LOG_NOTE("BGR\n");
        break;
      } else
      {
        // unknown > invalid format
        pixelFormat = GDI_IMAGE_FORMAT_INVALID;
      }
    }
      break;
    case 32:
    {
      // check color plane order
      if (aScrInfo.red.offset == 0)
      {
        // red is first
        pixelFormat = ABGR8888;
        VDB_LOG_NOTE("BGRA8888\n");
        break;
      }
      if (aScrInfo.blue.offset == 0)
      {
        // blue is first
        // todo: actually BGRA8888 format
        pixelFormat = RGBA8888;
        VDB_LOG_NOTE("ARGB\n");
        break;
      } else
      {
        VDB_LOG_NOTE("unknown image format\n");
        // unknown > invalid format
        pixelFormat = GDI_IMAGE_FORMAT_INVALID;
      }
    }
      break;
    default:
    {
      VDB_LOG_NOTE("unknown image format\n");
      pixelFormat = GDI_IMAGE_FORMAT_INVALID;
    }
      break;
  }

  return pixelFormat;
} // ImageFormatDeduce()
#endif // ifndef INTEGRITY

/*LIB_RESULT Icp2GdiImageFormat(ICP_COLOR_MODEL aIcpColorModel,
                              GDI_IMAGE_FORMAT &arGdiImageFormat)
{
  switch (aIcpColorModel)
  {
    case ICP_COLOR_RGBA8888_RGBA:
    {
      arGdiImageFormat = ABGR8888;
      break;
    } // case ICP_COLOR_RGBA8888_RGBA
    case ICP_COLOR_RGBA8888_BGRA:
    {
      arGdiImageFormat = ARGB8888;
      break;
    } // case ICP_COLOR_RGBA8888_ARGB
    case ICP_COLOR_RGB888_RGB:
    {
      arGdiImageFormat = RGB888;
      break;
    } // case ICP_COLOR_RGB888_RGB
    case ICP_COLOR_RGB888_BGR:
    {
      arGdiImageFormat = BGR888;
      break;
    } // case ICP_COLOR_RGB888_BGR
    case ICP_COLOR_RGB565_RGB:
    {
      arGdiImageFormat = RGB565;
      break;
    } // case ICP_COLOR_RG565_RGB
    case ICP_COLOR_RGB565_BGR:
    {
      arGdiImageFormat = BGR565;
      break;
    } // case ICP_COLOR_RGB565_BGR
    case ICP_COLOR_RGB666_RGB:
    {
      arGdiImageFormat = RGB666;
      break;
    } // case ICP_COLOR_RGB666_RGB
    case ICP_COLOR_RGB666_BGR:
    {
      arGdiImageFormat = BGR666;
      break;
    } // case ICP_COLOR_RGB666_BGR
    case ICP_COLOR_RGBA4444_RGBA:
    {
      arGdiImageFormat = ARGB4444;
      break;
    } // case ICP_COLOR_RGBA4444_RGBA
    case ICP_COLOR_RGBA4444_BGRA:
    {
      arGdiImageFormat = ABGR4444;
      break;
    } // case ICP_COLOR_RGBA4444_BGRA
    case ICP_COLOR_YCbCr444_YCbCr:
    {
      arGdiImageFormat = YUV444Stream_YUV;
      break;
    }
    case ICP_COLOR_YCbCr444_CrCbY:
    {
      arGdiImageFormat = YUV444Stream_VUY;
      break;
    }

    case ICP_COLOR_YCbCr422_YCbYCr:
    {
      arGdiImageFormat = YUV422Stream_YUYV;
      break;
    }

    case ICP_COLOR_YCbCr422_CrYCbY:
    {
      arGdiImageFormat = YUV422Stream_UYVY;
      break;
    }
    case ICP_COLOR_YCbCr422_CbYCrY:
    {
      arGdiImageFormat = YUV420Planar_Y1_U2_V3;
      break;
    }

    case ICP_COLOR_YCbCr420_Y_CbCr:
    {
      arGdiImageFormat = YUV420Planar_Y1_UV2;
      break;
    }
    case ICP_COLOR_YCbCr420_Y_CrCb:
    {
      arGdiImageFormat = YUV420Planar_Y1_VU2;
      break;
    }
    case ICP_COLOR_YCbCr420_Y_Cb_Cr:
    {
      arGdiImageFormat = YUV420Planar_Y1_U2_V3;
      break;
    }
    case ICP_COLOR_GRAYSCALE:
    {
      arGdiImageFormat = GS8;
      break;
    }
    case ICP_COLOR_YCbCr444_Y_Cb_Cr:
    case ICP_COLOR_YCbCr444_Y_CbCr:
    case ICP_COLOR_YCbCr422_YCrYCb:
    case ICP_COLOR_YCbCr422_Y_Cb_Cr:
    case ICP_COLOR_YCbCr422_Y_CbCr:
    case ICP_COLOR_XYZ444_XYZ:
    case ICP_COLOR_HSV_HSV:
    case ICP_COLOR_INVALID:
    default:
    {
      arGdiImageFormat = GDI_IMAGE_FORMAT_INVALID;
      break;
    } // default
  } // switch(aIcpColorModel)
  return LIB_SUCCESS;
}*/

GDI_FracDef::GDI_FracDef()
{
  mFrac = 1;             // Fractal type, 1=Julia, 2=Mandelbrot
  mConX = -0.74;         // Real constant, horizontal axis (x)
  mConY = 0.1;           // Imaginary constant, vertical axis (y)
  mMaxX = 2;             // Rightmost Real point of plane to be displayed
  mMinX = -2;            // Leftmost Real point
  mMaxY = 1;             // Uppermost Imaginary point
  mMinY = -1;            // Lowermost Imaginary point
  mIniter = 50;          // # of times to repeat function

  mScrSizeX = 320;             // Horizontal screen size in pixels
  mScrSizeY = 240;             // Vertical screen size

  mPixCorX = (mMaxX - mMinX) / mScrSizeX; // 1 pixel on screen = this many units on the
  mPixCorY = (mMaxY - mMinY) / mScrSizeY;        // plane for both x and y axis'

  mNewColor = 0;             // Used to speed up generation
  mLastColor = 0;            // See main function for more explanation
} // GDI_FracDef::GDI_FracDef()

void GDI_FracDraw(GDI_Image &aImg, GDI_FracDef & aFdef)
{
  uint32_t j = 0;
  while (j < aImg.mDesc.mHeight)
  {
    uint32_t i = 0;
    while (i < aImg.mDesc.mWidth)
    {
      if (aFdef.mFrac == 1)
      {
        // GDI_Julia set
        GDI_Julia(i,
                  j,
                  aImg,
                  aFdef);
        if (aFdef.mLastColor != aFdef.mNewColor)
        {
          GDI_Julia(i - 1,
                    j,
                    aImg,
                    aFdef);
        } else
        {
          const uint8_t *pix = gdi_color_pallette_rgb[0];
          if (aImg.mDesc.mBpp == 32)
          {
            uint32_t pix32 = 0;
            memcpy((char*) &pix32,
                   pix,
                   3);
            aImg.PixelPut(i,
                          j,
                          (uint8_t*) &pix32);
          }

          if (aImg.mDesc.mBpp == 16)
          {
            uint16_t pix16 = 0;
            pix16 = (pix[2] << 11) | (pix[1] << 5) | pix[0];
            aImg.PixelPut(i,
                          j,
                          (uint8_t*) &pix16);
          }
        }
      } else
      {
        // GDI_Mandelbrot set
        GDI_Mandel(i,
                   j,
                   aImg,
                   aFdef);
        if (aFdef.mLastColor != aFdef.mNewColor)
        {
          GDI_Mandel(i - 1,
                     j,
                     aImg,
                     aFdef);
        } else
        {
          const uint8_t *pix = gdi_color_pallette_rgb[0];
          if (aImg.mDesc.mBpp == 32)
          {
            uint32_t pix32 = 0;
            memcpy((uint8_t*) &pix32,
                   pix,
                   3);
            aImg.PixelPut(i,
                          j,
                          (uint8_t*) &pix32);
          }

          if (aImg.mDesc.mBpp == 16)
          {
            uint16_t pix16 = 0;
            pix16 = (pix[2] << 11) | (pix[1] << 5) | pix[0];
            aImg.PixelPut(i,
                          j,
                          (uint8_t*) &pix16);
          }
        }
      } // if frac

      aFdef.mNewColor = aFdef.mLastColor;
      i += 2;
    } // while width
    j++;
  } // while height
} // GDI_FracDraw()

void GDI_Julia(int aXpt, int aYpt, GDI_Image &aImg, GDI_FracDef & aFdef)
{
  long double x = aXpt * aFdef.mPixCorX + aFdef.mMinX;
  long double y = aFdef.mMaxY - aYpt * aFdef.mPixCorY; //converting from pixels to points
  long double xnew = 0;
  long double ynew = 0;

  int k;
  for (k = 0; k <= aFdef.mIniter; k++)  // Each pixel loop
  {
    //The Julia Function Z=Z*Z+c (of complex numbers) into x and y parts
    xnew = x * x - y * y + aFdef.mConX;
    ynew = 2 * x * y + aFdef.mConY;
    x = xnew;
    y = ynew;
    if ((x * x + y * y) > 4)
    {
      break;  // Break condition Meaning the loop will go
    }
    // on to a value of infinity.
  } // for End each pixel loop

  int color = k;
  if (color > 15)
  {
    color = (color % 15) + 1;
  }
  if (k >= aFdef.mIniter)
  {
    color = 0;
  }

  const uint8_t *pix = gdi_color_pallette_rgb[color];
  if (aImg.mDesc.mBpp == 32)
  {
    uint32_t pix32 = 0;
    memcpy((uint8_t*) &pix32,
           pix,
           3);
    aImg.PixelPut(aXpt,
                  aYpt,
                  (uint8_t*) &pix32);
  }

  if (aImg.mDesc.mBpp == 24)
  {
    uint32_t pix32 = 0;
    memcpy((uint8_t*) &pix32,
           pix,
           3);
    aImg.PixelPut(aXpt,
                  aYpt,
                  (uint8_t*) &pix32);
  }
  
  if (aImg.mDesc.mBpp == 16)
  {
    uint16_t pix16 = 0;
    pix16 = (pix[2] << 11) | (pix[1] << 5) | pix[0];
    aImg.PixelPut(aXpt,
                  aYpt,
                  (uint8_t*) &pix16);
  }
  aFdef.mNewColor = color;
} // GDI_Julia()

void GDI_Mandel(int aXpt, int aYpt, GDI_Image &aImg, GDI_FracDef & aFdef)
{
  long double x = 0;
  long double y = 0;    //converting from pixels to points
  long double xnew = 0;
  long double ynew = 0;

  int k;
  for (k = 0; k <= aFdef.mIniter; k++)  // Each pixel loop
  {
    //The Mandelbrot Function Z=Z*Z+c into x and y parts
    xnew = x * x - y * y + aXpt * aFdef.mPixCorX + aFdef.mMinX;
    ynew = 2 * x * y + aFdef.mMaxY - aYpt * aFdef.mPixCorY;
    x = xnew;
    y = ynew;
    if ((x * x + y * y) > 4)
    {
      break;  // Break condition
    }
  } // for End each pixel loop

  int color = k;
  if (color > 15)
  {
    color = (color % 15) + 1;
  }

  if (k >= aFdef.mIniter)
  {
    color = 0;
  }

  const uint8_t *pix = gdi_color_pallette_rgb[color];
  if (aImg.mDesc.mBpp == 32)
  {
    uint32_t pix32 = 0;
    memcpy((uint8_t*) &pix32,
           pix,
           3);
    aImg.PixelPut(aXpt,
                  aYpt,
                  (uint8_t*) &pix32);
  }

  if (aImg.mDesc.mBpp == 16)
  {
    uint16_t pix16 = 0;
    pix16 = (pix[2] << 11) | (pix[1] << 5) | pix[0];
    aImg.PixelPut(aXpt,
                  aYpt,
                  (uint8_t*) &pix16);
  }

  aFdef.mNewColor = color;
} // GDI_Mandel()

#ifndef __STANDALONE__
#ifdef __VPD__
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>


#include <map>

#ifdef TREERUNNER
#define GDI_VPD_MEM 0x14000000 //0x11000000
#define GDI_VPD_REG 0x16000000 //0x10fff000

LIB_RESULT GDI_VpdEnable(GDI_Image *apImg, vpd_regs_t **apVpdRegsMem)
{
  LIB_RESULT lres = LIB_SUCCESS;

  uint32_t img_size = apImg->mDesc.mHeight * apImg->mDesc.mWidthStep;

  VDB_LOG_NOTE("%u %u\n",
               apImg->mDesc.mHeight, apImg->mDesc.mWidthStep);
#ifdef INTEGRITY
#include "vpd.h"
  // map VPD registers
  *apVpdRegsMem = (vpd_regs_t*) VPD_RegsMap();
  if(apVpdRegsMem != NULL)
  {
    // map VPD data
    apImg->mData = (uint8_t*) VPD_DataMap();
    if(apImg->mData == NULL)
    {
      VDB_LOG_ERROR("Mapping video probe device data memory failed.\n");
      lres = LIB_FAILURE;
    } // if mapping VPD data failed
  } // if mapping VPD registers succeeded
  else
  {
    VDB_LOG_ERROR("Mapping video probe device registers failed.\n");
    lres = LIB_FAILURE;
  } // else from if mapping VPD registers succeeded
  
#else // ifdef INTEGRITY
  OAL_FILE_DESCRIPTOR vpdRegsFd;
  if (OAL_FileOpen(&vpdRegsFd,
                   "/dev/mem",
                   OAL_FILE_RDWR) == LIB_FAILURE)
  {
    VDB_LOG_ERROR("/dev/mem open failed.\n");
    lres = LIB_FAILURE;
  } else
  {
    // map VPD registers
    *apVpdRegsMem = (vpd_regs_t*) mmap(NULL,
                                       sizeof(vpd_regs_t),
                                       PROT_READ | PROT_WRITE,
                                       MAP_SHARED,
                                       vpdRegsFd.fd,
                                       GDI_VPD_REG);
    if (*apVpdRegsMem == NULL)
    {
      VDB_LOG_ERROR("Mapping video probe device registers failed.\n");
      lres = LIB_FAILURE;
    } else
    {
      // map VPD data
      apImg->mData = (uint8_t*) mmap(NULL,
                                  img_size,
                                  PROT_READ | PROT_WRITE,
                                  MAP_SHARED,
                                  vpdRegsFd.fd,
                                  GDI_VPD_MEM);
      if (apImg->mData == NULL)
      {
        VDB_LOG_ERROR("Mapping video probe device data memory failed.\n");
        lres = LIB_FAILURE;
      } // if data mapping failed
    } // else from if vpd map failed
  } // if FileOpen()

  if (OAL_FileClose(&vpdRegsFd) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Closing /dev/mem failed.\n");
  } // if FileClose() failed
#endif // else from ifdef INTEGRITY
  if(lres == LIB_SUCCESS)
      {
        // set RGB format
        (*apVpdRegsMem)->FORMAT = 0x3;
        // set resolution
        (*apVpdRegsMem)->WIDTH = apImg->mDesc.mWidth;
        (*apVpdRegsMem)->HEIGHT = apImg->mDesc.mHeight;

        printf("Please connect video probe to port 3000.\n");

        // enable video probe
        (*apVpdRegsMem)->ENABLE = 0x1;

      } // else from image data map failed

      if (lres == LIB_FAILURE)
      {
        if (munmap(*apVpdRegsMem,
                   sizeof(vpd_regs_t)) != 0)
        {
          VDB_LOG_ERROR(" Unmapping video probe device memory failed.\n");
        } // unmap() failed

        apImg->mData = NULL;
  } // if LIB_SUCCESS

  return lres;
} // GDI_VpdEnable()

void GDI_VpdUpdateSignal(vpd_regs_t *apVpdRegsMem)
{
  apVpdRegsMem->CONTROL = 0x1;
} // GDI_VpdEnable()

LIB_RESULT GDI_VpdDisable(GDI_Image *apImg, vpd_regs_t **apVpdRegsMem)
{
  LIB_RESULT lres = LIB_SUCCESS;

  uint32_t img_size = apImg->mDesc.mHeight * apImg->mDesc.mWidthStep;

  (*apVpdRegsMem)->ENABLE = 0x0;
#ifdef INTEGRITY
#else // ifdef INTEGRITY  
  if (munmap(apImg->mData,
             img_size) != 0)
  {
    VDB_LOG_ERROR("Unmapping video probe device data memory failed.\n");
    lres = LIB_FAILURE;
  } // unmap() failed

  if (munmap(*apVpdRegsMem,
             sizeof(vpd_regs_t)) != 0)
  {
    VDB_LOG_ERROR("Unmapping video probe device memory failed.\n");
    lres = LIB_FAILURE;
  } // unmap() failed
#endif // ifdef INTEGRITY

  apImg->mData = NULL;
  *apVpdRegsMem = NULL;

  return lres;
} // GDI_VpdDisable()

LIB_RESULT GDI_VpdTest()
{
  LIB_RESULT lres = LIB_SUCCESS;
  vpd_regs_t *vpdRegsMem = NULL;
  GDI_Image img(640,
                480,
                RGB888);

  //uint32_t img_size = img.mDesc.mHeight * img.mDesc.mWidthStep;

  if (GDI_VpdEnable(&img, &vpdRegsMem) == LIB_FAILURE)
  {
    VDB_LOG_ERROR("Failed to enable VPD.\n");
        lres = LIB_FAILURE;
      } else
      {
        // set RGB format
        vpdRegsMem->FORMAT = 0x3;
        // set resolution
        vpdRegsMem->WIDTH = 640;
        vpdRegsMem->HEIGHT = 480;

        VDB_LOG_NOTE("Please connect video probe to port 3000.\n");

        // enable video probe display and wait for video probe connection
        vpdRegsMem->ENABLE = 0x1;

        // fill image data
        GDI_RectFill_RGB(20,
                     20,
                     600,
                     440,
                     GDI_ColorRgbaPack(maroon),
                     &img);
        vpdRegsMem->CONTROL = 0x1;
        sleep(0.1);
        GDI_RectFill_RGB(20,
                     20,
                     600,
                     440,
                     GDI_ColorRgbaPack(red),
                     &img);
        vpdRegsMem->CONTROL = 0x1;
        sleep(0.1);
        GDI_RectFill_RGB(20,
                     20,
                     600,
                     440,
                     GDI_ColorRgbaPack(green),
                     &img);
        vpdRegsMem->CONTROL = 0x1;
        sleep(0.1);
        GDI_RectFill_RGB(20,
                     20,
                     600,
                     440,
                     GDI_ColorRgbaPack(blue),
                     &img);

        vpdRegsMem->CONTROL = 0x1;

      printf("waiting for 0.5 seconds.\n");
      sleep(0.5);
	
        vpdRegsMem->ENABLE = 0x0;

      GDI_VpdDisable(&img, &vpdRegsMem);
  } // if FileOpen()

  return lres;
} // GDI_VpdTest()

#else // ifdef TREERUNNER
//
// Video probe device functions for S32V234 toolbox
//

// define VLAB base address for APEX_MCDMA_CH07 registers
#define APEX_MCDMA_CH07_BASE 0x73100000
// define where the display data are expected to be
#define DISPLAY_DATA_ADDR 0xD0000000

LIB_RESULT GDI_VpdEnable(GDI_Image *apImg, vpd_regs_t **apVpdRegsMem)
{
  LIB_RESULT lres = LIB_SUCCESS;

  uint32_t img_size = apImg->mDesc.mHeight * apImg->mDesc.mWidthStep;

  VDB_LOG_NOTE("%u %u\n",
               apImg->mDesc.mHeight, apImg->mDesc.mWidthStep);
  OAL_FILE_DESCRIPTOR vpdRegsFd;
  if (OAL_FileOpen(&vpdRegsFd,
                   "/dev/mem",
                   OAL_FILE_RDWR) == LIB_FAILURE)
  {
    VDB_LOG_ERROR("/dev/mem open failed.\n");
    lres = LIB_FAILURE;
  } else
  {
    *apVpdRegsMem = (vpd_regs_t*) mmap(NULL,
                                       sizeof(vpd_regs_t),
                                       PROT_READ | PROT_WRITE,
                                       MAP_SHARED,
                                       vpdRegsFd.fd,
                                       APEX_MCDMA_CH07_BASE);
    if (*apVpdRegsMem == NULL)
    {
      VDB_LOG_ERROR("Mapping video probe device memory failed.\n");
      lres = LIB_FAILURE;
    } else
    {
      apImg->mData = (uint8_t*) mmap(NULL,
                                  img_size,
                                  PROT_READ | PROT_WRITE,
                                  MAP_SHARED,
                                  vpdRegsFd.fd,
                                  DISPLAY_DATA_ADDR);
      if (apImg->mData == NULL)
      {
        VDB_LOG_ERROR("Mapping video probe device data memory failed.\n");
        lres = LIB_FAILURE;
      } else
      {
        // set RGB format
        (*apVpdRegsMem)->mDst = DISPLAY_DATA_ADDR;

        printf("Please connect video probe to port 3001.\n");

      } // else from image data map failed

      if (lres == LIB_FAILURE)
      {
        if (munmap(*apVpdRegsMem,
                   sizeof(vpd_regs_t)) != 0)
        {
          VDB_LOG_ERROR(" Unmapping video probe device memory failed.\n");
        } // unmap() failed

        apImg->mData = NULL;
      } // if LIB_FAILURE
    } // else from if vpd map failed
  } // if FileOpen()

  if (OAL_FileClose(&vpdRegsFd) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Closing /dev/mem failed.\n");
  } // if FileClose() failed

  return lres;
} // GDI_VpdEnable()

void GDI_VpdUpdateSignal(vpd_regs_t *apVpdRegsMem)
{
  apVpdRegsMem->mSignal = 0x1;
} // GDI_VpdEnable()

LIB_RESULT GDI_VpdDisable(GDI_Image *apImg, vpd_regs_t **apVpdRegsMem)
{
  LIB_RESULT lres = LIB_SUCCESS;

  uint32_t img_size = apImg->mDesc.mHeight * apImg->mDesc.mWidthStep;


  if (munmap(apImg->mData,
             img_size) != 0)
  {
    VDB_LOG_ERROR("Unmapping video probe device data memory failed.\n");
    lres = LIB_FAILURE;
  } // unmap() failed

  if (munmap(*apVpdRegsMem,
             sizeof(vpd_regs_t)) != 0)
  {
    VDB_LOG_ERROR("Unmapping video probe device memory failed.\n");
    lres = LIB_FAILURE;
  } // unmap() failed

  apImg->mData = NULL;
  *apVpdRegsMem = NULL;

  return lres;
} // GDI_VpdDisable()

LIB_RESULT GDI_VpdTest()
{
  LIB_RESULT lres = LIB_SUCCESS;
  vpd_regs_t *vpdRegsMem = NULL;
  uint32_t w = 640u, h = 480u;
  GDI_Image img(w,
                h,
                RGB888);

  uint32_t img_size = img.mDesc.mHeight * img.mDesc.mWidthStep;

  OAL_FILE_DESCRIPTOR vpdRegsFd;
  if (OAL_FileOpen(&vpdRegsFd,
                   "/dev/mem",
                   OAL_FILE_RDWR) == LIB_FAILURE)
  {
    VDB_LOG_ERROR("/dev/mem open failed.\n");
    lres = LIB_FAILURE;
  } else
  {
    vpdRegsMem = (vpd_regs_t*) mmap(NULL,
                                    sizeof(vpd_regs_t),
                                    PROT_READ | PROT_WRITE,
                                    MAP_SHARED,
                                    vpdRegsFd.fd,
                                    APEX_MCDMA_CH07_BASE);
    if (vpdRegsMem == NULL)
    {
      VDB_LOG_ERROR("Mapping video probe device memory failed.\n");
      lres = LIB_FAILURE;
    } else
    {
      img.mData = (uint8_t*) mmap(NULL,
                               img_size,
                               PROT_READ | PROT_WRITE,
                               MAP_SHARED,
                               vpdRegsFd.fd,
                               DISPLAY_DATA_ADDR);
      if (img.mData == NULL)
      {
        VDB_LOG_ERROR("Mapping video probe device data memory failed.\n");
        lres = LIB_FAILURE;
      } else
      {
        // set RGB format
        vpdRegsMem->mDst = DISPLAY_DATA_ADDR;
        
        VDB_LOG_NOTE("Please connect video probe to port 3001.\n");

        // fill image data
        GDI_RectFill_RGB(20,
                     20,
                     w - 40,
                     h - 40,
                     GDI_ColorRgbaPack(maroon),
                     &img);
        vpdRegsMem->mSignal = 0x1;
        sleep(0.1);
        GDI_RectFill_RGB(20,
                     20,
                     w - 40,
                     h - 40,
                     GDI_ColorRgbaPack(red),
                     &img);
        vpdRegsMem->mSignal = 0x1;
        sleep(0.1);
        GDI_RectFill_RGB(20,
                     20,
                     w - 40,
                     h - 40,
                     GDI_ColorRgbaPack(green),
                     &img);
        vpdRegsMem->mSignal = 0x1;
        sleep(0.1);
        GDI_RectFill_RGB(20,
                     20,
                     w - 40,
                     h - 40,
                     GDI_ColorRgbaPack(blue),
                     &img);

        vpdRegsMem->mSignal = 0x1;

	printf("waiting for 10 seconds.\n");
	sleep(10);
	
        if (munmap(img.mData,
                   img_size) != 0)
        {
          VDB_LOG_ERROR("Unmapping video probe device data memory failed.\n");
          lres = LIB_FAILURE;
        } // unmap() failed
      } // else from image data map failed

      if (munmap(vpdRegsMem,
                 sizeof(vpd_regs_t)) != 0)
      {
        VDB_LOG_ERROR(" Unmapping video probe device memory failed.\n");
        lres = LIB_FAILURE;
      } // unmap() failed

      img.mData = NULL;
    } // else from if vpd map failed
  } // if FileOpen()

  if (OAL_FileClose(&vpdRegsFd) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Closing /dev/mem failed.\n");
    lres = LIB_FAILURE;
  } // if FileClose() failed

  return lres;
} // GDI_VpdTest()
#endif // ifdef TREERUNNER
#endif // ifdef __VPD__
#endif // __STANDALONE__
