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
/**
* \file     gdi_utils.hpp
* \brief    GDI utilities for Integrity
* \author   Tomas Babinec
* \version  0.1
* \date     10-01-2014
* \note
****************************************************************************/

#ifndef GDIUTILS_HPP
#define GDIUTILS_HPP

#include <stdio.h>
#ifndef INTEGRITY
#include <linux/fb.h>
#endif // ifdef INTEGRITY

/*****************************************************************************
* defines/enums (const)
*****************************************************************************/

#define N_H_STRIPES 20  // number of horizontal stripes per whole screen

/*****************************************************************************
* shared variables
*****************************************************************************/

extern const uint8_t gdi_color_pallette_rgb[][3]; // rgb array from gdi_utils

/*****************************************************************************
* functions (prototypes)
*****************************************************************************/

/****************************************************************************/
/** Set up GDI_ColorRGBA value according to specified components.
 *
 * \param aColor one of the color enumerations
 * \param aAlpha alpha value
 *
 * \return set up 32 GDI_ColorRGBA value
 ****************************************************************************/
GDI_ColorRGBA GDI_ColorRgbaPack(GDI_COLOR_PALLETTE aColor, uint8_t aAlpha = 0);

/****************************************************************************/
/** Set up GDI_ColorRGBA value based on specified pallette enum.
 *
 * \param aR red color component
 * \param aG green color component
 * \param aB blue color component
 * \param aAlpha alpha value
 *
 * \return set up 32 GDI_ColorRGBA value
 ****************************************************************************/
GDI_ColorRGBA GDI_ColorRgbaPack(uint8_t aR, uint8_t aG, uint8_t aB, uint8_t aAlpha = 0);

/****************************************************************************/
/** Fills rectangle inside an image with one color.
 *
 * Only for 32bit pixel format images.
 *
 * \param aX
 * \param aY
 * \param aW
 * \param aH
 * \param apRgba RGBA8888 color value.
 * \param apImg
 ****************************************************************************/
void GDI_RectFill_BGRA(uint32_t aX, uint32_t aY, uint32_t aW, uint32_t aH, GDI_ColorRGBA aRgba,
    GDI_Image *apImg);

/****************************************************************************/
/** Fills rectangle inside an image with one color.
 *
 * Only for 24bit pixel format images.
 *
 * \param aX
 * \param aY
 * \param aW
 * \param aH
 * \param apRgb RGBA8888 color value. Alpha value ignored.
 * \param apImg
 ****************************************************************************/
void GDI_RectFill_RGB(uint32_t aX, uint32_t aY, uint32_t aW, uint32_t aH, GDI_ColorRGBA aRgb,
    GDI_Image *apImg);

/****************************************************************************/
/** Fills rectangle inside an image with one color.
 *
 * Only for 16bit pixel format images.
 *
 * \param aX
 * \param aY
 * \param aW
 * \param aH
 * \param aColorValue RGBA8888 color value. Gets converted to BGR565.
 * \param apImg
 */
void GDI_RectFill_BGR565(uint32_t aX, uint32_t aY, uint32_t aW, uint32_t aH, GDI_ColorRGBA aRgb888,
    GDI_Image *apImg);

/****************************************************************************/
/** Fills rectangle inside an image with one color.
 *
 * Only for ARGB image data.
 *
 * \param aX
 * \param aY
 * \param aW
 * \param aH
 * \param apRgb
 * \param aBuffer
 * \param apScreen_info
 * \param apFixed_info
 */
void GDI_RectFill_ARGB(uint32_t aX, uint32_t aY, uint32_t aW, uint32_t aH, char *apRgb, char *apBuffer, struct fb_var_screeninfo *apScreen_info, struct fb_fix_screeninfo *apFixed_info);

/****************************************************************************/
/** Fills rectangle inside an image with one color.
 *
 * Chooses the right RectFill() fcn according to image format of the image.
 *
 * \param aX
 * \param aY
 * \param aW
 * \param aH
 * \param aColor RGBA8888 color value. Gets converted to appropriate format.
 * \param apImg
 ****************************************************************************/
void GDI_RectFill(uint32_t aX, uint32_t aY, uint32_t aW, uint32_t aH, GDI_ColorRGBA aColor, GDI_Image *apImg);

/****************************************************************************/
/** Evaluates intersection of two rectangles
 *
 * \param aRct1   1. rectangle to intersect
 * \param aRct2   2. rectangle to intersect
 * \param aRctOut overlapping area
 *
 * \return size of the overlapping area
 */
size_t GDI_RectIntersect(const GDI_Rectangle &aRct1, const GDI_Rectangle &aRct2, GDI_Rectangle &aRctOut);

/****************************************************************************/
/** Draws horizontal stripes to image
 *
 *  Works for 16 and 32 bit pixel format images. Aimed at BGRA8888 and BGR565.
 *
 * \param aStripeHeight height of one stripe [pixels]
 * \param apImg pointer to image
 */
void GDI_StripesDraw(int aStripeHeight, GDI_Image *apImg);

/****************************************************************************/
/** Finds out pixel format of framebuffer device.
 *
 * \param aScrInfo  framebuffer device screen info
 * \return  pixel format
 */
GDI_IMAGE_FORMAT GDI_ImageFormatDeduce(const struct fb_var_screeninfo &aScrInfo);

/****************************************************************************/
/** Draws fractal to given image.
 *
 * \param aImg  image to which the fractal will be drawn
 * \param aFdef fractal defining data structure
 *
 *****************************************************************************/
void GDI_FracDraw(GDI_Image &aImg, GDI_FracDef & aFdef);

/****************************************************************************/
/** Fills one pixel in specified image with fractal data.
 *
 * Julia set fractal.
 *
 * \param aXpt x coordinate of the pixel
 * \param aYpt y coordinate of the pixel
 * \param aImg image to draw the fractal data to
 * \param aFdef fractal defining data structure
 *
 *****************************************************************************/
void GDI_Julia(int aXpt, int aYpt, GDI_Image &aImg, GDI_FracDef & aFdef);

/****************************************************************************/
/** Fills one pixel in specified image with fractal data.
 *
 * Mandelbrot set fractal.
 *
 * \param aImg  image to which the fractal will be drawn
 * \param aFdef fractal defining data structure
 *
 *****************************************************************************/
void GDI_Mandel(int aXpt, int aYpt, GDI_Image &aImg, GDI_FracDef & aFdef);

/****************************************************************************/
/** Enables virtual probe access
 *
 * \param apImg supplies required resolution and gets pointer to vpd data
 * \param apVpdRegsMem receives pointer to vpd registers structure.
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
 ****************************************************************************/
LIB_RESULT GDI_VpdEnable(GDI_Image *apImg,vpd_regs_t **apVpdRegsMem);

void GDI_VpdUpdateSignal(vpd_regs_t *apVpdRegsMem);

/****************************************************************************/
/** Disables virtual probe access
 *
 * \param apImg pointer image to be unmapped from vpd
 * \param apVpdRegsMem pointer to vpd registers to be unmapped
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
 ****************************************************************************/
LIB_RESULT GDI_VpdDisable(GDI_Image *apImg,vpd_regs_t **apVpdRegsMem);

/****************************************************************************/
/** Video probe device testing.
 *
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
 ****************************************************************************/
LIB_RESULT GDI_VpdTest();

#endif /* GDIUTILS_HPP */
