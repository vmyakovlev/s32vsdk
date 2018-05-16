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

/* Information for doxygen */
/**
 *  \file   gdi_types_public.hpp
 *  \brief  Type definitions for GDI component
 *
 */

#ifndef GDITYPESPUBLIC_HPP
#define GDITYPESPUBLIC_HPP


/***************************************************************************/

#include "oal.h"
#include "global_errors.h"
#include <stdint.h>

// Log notes disable switch
//#define VDB_LOG_NOTE_OFF

// OCV enable switch
#ifndef __STANDALONE__  // disable OCV use under standalone
  #define GDI_OCV
#endif // ifndef __STANDALONE__


// shared window buffers enabled if defined
#define GDI_SWB

// Contiguos memory usage enable switch
//#define GDI_CONTIG

// define in case VLAB video probe display HW is available
// otherwise implementation with brake points on APEX MCDMA registers is used
#define TREERUNNER

/*****************************************************************************
* types
*****************************************************************************/

typedef uint32_t GDI_ColorRGBA;

/***************************************************************************/
/**
 * \brief GDI Image Format
 *
 ***************************************************************************/
typedef enum
{
   RGB888 = 0,              ///< Red in 2nd most significant byte, then Green, then Blue
   BGR888,                  ///< Blue in 2nd most significant byte, then Green, then Red
   RGB565,                  ///< Red in most significant 5 bits, then 6 Green bits , then 5 Blue bits
   RGB565_8BitAlphaPlane,   ///< RGB565 followed with a per pixel 8bit alpha plane
   BGR565,                  ///< Blue in most significant 5 bits, then 6 Green bits , then 5 Red bits
   BGR565_8BitAlphaPlane,   ///< BGR565 followed with a per pixel 8bit alpha plane
   RGB666,                  ///< RGB666
   BGR666,                  ///< BGR666
   YUV422Stream_UYVY,       ///< U Y0 V Y1 where U is the least significant byte
   YUV422Stream_YUYV,       ///< Y0 U Y1 V where Y0 is the least significant byte
   YUV420Planar,            ///< Deprecated
   YUV420Planar_Y1_UV2,     ///< Y plane followed by a UV plane
   YUV420Planar_Y1_VU2,     ///< Y plane followed by a VU plane
   YUV420Planar_Y1_U2_V3,   ///< Y plane followed by a U plane and a V plane
   YUV422Planar,            ///< Deprecated
   YUV444Stream_VUY,        ///< Y U V where V is the least significant byte
   YUV444Stream_YUV,        ///< V U Y where Y is the least significant byte
   RGBA4444,                ///< RGBA format where Red channel is in most significant byte, and Blue is in second most most significant byte
   RGBA8888,                ///< RGBA format where Red channel is in most significant byte, and Blue is in second most most significant byte
   ABGR4444,                ///< ARGB format where Alpha channel is in most significant byte, and Blue is in second most most significant byte
   ABGR8888,                ///< ARGB format where Alpha channel is in most significant byte, and Blue is in second most most significant byte
   GS8,                     ///< Grayscale 8 bit; be wary when using this type since for CV220X, GDI will allocate an extra memory buffer per window.
   GDI_IMAGE_FORMAT_NONE,   ///< No format
   GDI_IMAGE_FORMAT_INVALID ///< Invalid format
} GDI_IMAGE_FORMAT;


/***************************************************************************/
/**
 * \brief GDI Display ID
 *
 ***************************************************************************/
typedef enum {
   GDI_DISPLAY_LCD1        = 0x00,  ///< LCD1
   GDI_DISPLAY_LCD2        = 0x01,  ///< LCD2
   GDI_DISPLAY_AXI_VDMA	   = 0x02,  ///< AXI VDMA output on ZYNQ
   GDI_DISPLAY_TVOUT_NTSC  = 0x03,  ///< TV OUT NTSC
   GDI_DISPLAY_TVOUT_PAL   = 0x04,  ///< TV OUT PAL
   GDI_DISPLAY_XGA         = 0x05,  ///< XGA
   GDI_DISPLAY_VLAB        = 0x06,  ///< VLAB specific video probe device
   GDI_DISPLAY_ALL         = 0xFF,  ///< Refers to ALL displays
   GDI_DISPLAY_ID_INVALID           ///< Invalid display
} GDI_DISPLAY_ID;

/***************************************************************************/
/**
 * \brief GDI Display Type
 *
 ***************************************************************************/
typedef enum
{
   GDI_DISPLAY_TYPE_LCD,            ///< LCD
   GDI_DISPLAY_TYPE_TV,             ///< TV
   GDI_DISPLAY_TYPE_XGA,            ///< XGA
   GDI_DISPLAY_TYPE_STREAMING,      ///< Streaming
   GDI_DISPLAY_TYPE_OTHER,          ///< Other 
   GDI_DISPLAY_TYPE_INVALID         ///< Invalid type
} GDI_DISPLAY_TYPE;


/****************************************************************************/
/** Simple color palate enum with 16 basic named colors
 *
 ****************************************************************************/
typedef
enum{
  black = 0,    //!< black
  gray = 1,     //!< gray
  silver = 2,   //!< silver
  white = 3,    //!< white
  maroon = 4,   //!< maroon
  red = 5,      //!< red
  olive = 6,    //!< olive
  yellow = 7,   //!< yellow
  green = 8,    //!< green
  lime = 9,     //!< lime
  teal = 10,    //!< teal
  aqua = 11,    //!< aqua
  navy = 12,    //!< navy
  blue = 13,    //!< blue
  purple = 14,  //!< purple
  fuchsia = 15  //!< fuchsia

} GDI_COLOR_PALLETTE;

/*****************************************************************************
* types
*****************************************************************************/

/***************************************************************************/
/**
 * \brief GDI Display Info
 *
 * \b mAvailable:          Display device is available
 *
 * \b mActive:             Display device is enabled
 *
 * \b mRotationInDegreesX: The degrees of rotation around the X axis
 *
 * \b mRotationInDegreesY: The degrees of rotation around the Y axis
 *
 * \b mRotationInDegreesZ: The degrees of rotation around the Z axis
 *
 * \b mDisplayType:        Display type
 *
 * \b mWidthInPixels:      Width in pixel of the display frame
 *
 * \b mHeightInPixels:     Height in pixel of the display frame
 *
 ***************************************************************************/
typedef struct
{
   bool             mAvailable;          ///< Display device is available
   bool             mActive;             ///< Display device is enabled
   uint16_t         mRotationInDegreesX; ///< The degrees of rotation around the X axis
   uint16_t         mRotationInDegreesY; ///< The degrees of rotation around the Y axis
   uint16_t         mRotationInDegreesZ; ///< The degrees of rotation around the Z axis
   GDI_DISPLAY_TYPE mDisplayType;        ///< Display type
   uint32_t         mWidthInPixels;      ///< Width in pixel of the display frame
   uint32_t         mHeightInPixels;     ///< Height in pixel of the display frame
   GDI_IMAGE_FORMAT mImageFormat;	 ///< Image pixel format
} GDI_DisplayInfo;

/***************************************************************************/
/**
 * \brief GDI Display Alpha
 *
 * \b Keyx: Alpha key pixel value using RGB888 format. Lower color depth
 * displays have their bits mapped to the lower bits of their respective
 * colors. For example a RGB565 buffer will use only the lower 5 bits of
 * the R and B and the lower 6 bits of the G.
 *
 * \b Maskx:   Allow any color bit to be ignored. A 0 in any mask bit
 * means the corresponding Keyx value can be either 0 or 1 (i.e.
 * ignore).
 *
 * \b Alphax:  Level of transparency of the image. The value represents
 * a range from 0 for total transparency to 255 for total opacity.
 * Values greater than 255 are NOT supported.
 *
 * \b DefaultAlpha:  Alpha blend value to use for the image if no matches
 * are found.
 ***************************************************************************/
typedef struct
{
   uint32_t mKey0;           ///< Alpha key pixel value
   uint32_t mMask0;          ///< Allow any color bit to be ignored
   uint32_t mAlpha0;         ///< Alpha blend value to use
   uint32_t mEnableAlpha0;   ///< Enable or disable alpha blending
   uint32_t mKey1;           ///< Alpha key pixel value
   uint32_t mMask1;          ///< Allow any color bit to be ignored
   uint32_t mAlpha1;         ///< Alpha blend value to use
   uint32_t mEnableAlpha1;   ///< Enable or disable alpha blending
   uint32_t mKey2;           ///< Alpha key pixel value
   uint32_t mMask2;          ///< Allow any color bit to be ignored
   uint32_t mAlpha2;         ///< Alpha blend value to use
   uint32_t mEnableAlpha2;   ///< Enable or disable alpha blending
   uint32_t mDefaultAlpha;   ///< Alpha blend value to use for the image if no matches are found
} GDI_IMAGE_ALPHA;

/****************************************************************************/
/** GDI rectangle data encapsulation
 *
 ****************************************************************************/
struct GDI_Rectangle
{
  uint32_t mX;      ///< x origin
  uint32_t mY;      ///< y origin
  uint32_t mWidth;  ///< width
  uint32_t mHeight; ///< height

  GDI_Rectangle();
  GDI_Rectangle(uint32_t aX, uint32_t aY, uint32_t aWidth, uint32_t aHeight);
};

/****************************************************************************/
/** GDI window to display info encapsulation
 *
 ****************************************************************************/
struct GDI_W2dMapInfo
{
  uint32_t mWindowHandle;             ///< window handle
  GDI_DISPLAY_ID mDisplayId;          ///< GDI display id
  uint32_t mLayerId;                  ///< display layer to which to map
  GDI_Rectangle mWindowRectangle;     ///< window rectangle to be mapped
  GDI_Rectangle mDisplayRectangle;    ///< display rectangle to be mapped to

  GDI_W2dMapInfo();
};

/****************************************************************************/
/** GDI fractal defining data encapsulation
 *
 ****************************************************************************/
struct GDI_FracDef{
  int mFrac;            ///< Fractal type, 1=Julia, 2=Mandelbrot
  float mConX;            ///< Real constant, horizontal axis (x)
  float mConY;            ///< Imaginary constant, vertical axis (y)
  float mMaxX;            ///< Rightmost Real point of plane to be displayed
  float mMinX;            ///< Leftmost Real point
  float mMaxY;            ///< Uppermost Imaginary point
  float mMinY;            ///< Lowermost Imaginary point
  float mIniter;          ///< # of times to repeat function

  float mPixCorX;         ///< 1 pixel on screen = this many units on the
  float mPixCorY;         ///< plane for both x and y axis'

  int mScrSizeX;          ///< Horizontal screen size in pixels
  int mScrSizeY;          ///< Vertical screen size

  int mNewColor;          ///< Used to speed up generation
  int mLastColor;         ///< See main function for more explanation

  /****************************************************************************/
  /** Non-parametric constructor.
   *
   * Sets default fractal defining values.
   *
   ****************************************************************************/
  GDI_FracDef();
}; //GDI_FracDef

#ifdef TREERUNNER
/****************************************************************************/
/** Virtual probe device registers for Treerunner toolbox
 *
 ****************************************************************************/
typedef struct _vpd_regs_t
{
  uint32_t FORMAT;
  uint32_t WIDTH;
  uint32_t HEIGHT;
  uint32_t ENABLE;
  uint32_t CONTROL;
} vpd_regs_t;

#else 

/****************************************************************************/
/** Virtual probe device registers for S32V234 toolbox 
 * 
 * \note actually using the APEX2_0 MCDMA_CH07 SRC and DST registers
 *
 ****************************************************************************/
typedef struct _vpd_regs_t{
  uint8_t mStuff[0x800];
  uint32_t mSignal;
  uint32_t mDst;
} vpd_regs_t;

#endif // ifdef TREERUNNER

#endif /* GDITYPESPUBLIC_HPP */

 
