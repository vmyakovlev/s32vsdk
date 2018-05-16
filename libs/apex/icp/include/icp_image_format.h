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

#ifndef ICPIMAGEFORMAT_H
#define ICPIMAGEFORMAT_H

#include <global_errors.h>  // for LIB_RESULT

/**
 *  \file icp_image_format.h
 *  \brief enum declaring image types
 */

enum
{
   //RGB
   ICP_COLOR_RGBA8888_RGBA = 0,///< Plane[0] : Byte Order : R.G.B.A. / 32bpp
   ICP_COLOR_RGBA8888_BGRA,    ///< Plane[0] : Byte Order : B.G.R.A. / 32bpp

   ICP_COLOR_RGB888_RGB,       ///< Plane[0] : Byte Order : R.G.B. / 32bpp
   ICP_COLOR_RGB888_BGR,       ///< Plane[0] : Byte Order : B.G.R. / 32bpp

   ICP_COLOR_RGB565_RGB,       ///< Plane[0] : Byte Order : R.G.B. / 16bpp
   ICP_COLOR_RGB565_BGR,       ///< Plane[0] : Byte Order : B.G.R. / 16bpp

   ICP_COLOR_RGB666_RGB,       ///< Plane[0] : Byte Order : R.G.B. / 32bpp
   ICP_COLOR_RGB666_BGR,       ///< Plane[0] : Byte Order : B.G.R. / 32bpp

   ICP_COLOR_RGBA4444_RGBA,    ///< Plane[0] : Byte Order : R.G.B.A. / 16bpp
   ICP_COLOR_RGBA4444_BGRA,    ///< Plane[0] : Byte Order : B.G.R.A. / 16bpp

   //YCbCr444
   ICP_COLOR_YCbCr444_YCbCr,   ///< Plane[0] : Byte Order : Y.Cb.Cr / 32bpp
   ICP_COLOR_YCbCr444_CrCbY,   ///< Plane[0] : Byte Order : Cr.Cb.Y / 32bpp

   ICP_COLOR_YCbCr444_Y_Cb_Cr, ///< Plane[0] : Y / 8bpp  \n
   ///< Plane[1] : Cb / 8bpp \n
   ///< Plane[2] : Cr / 8bpp

   ICP_COLOR_YCbCr444_Y_CbCr,  ///< Plane[0] : Byte Order : Y / 8bpp  \n
   ///< Plane[1] : Byte Order : Cb.Cr / 16bpp
   //YCbCr422
   ICP_COLOR_YCbCr422_YCbYCr,  ///< Plane[0] : Y0.U0.Y1.V0. / 32bpp
   ICP_COLOR_YCbCr422_YCrYCb,  ///< Plane[0] : Y0.V0.Y1.U0. / 32bpp
   ICP_COLOR_YCbCr422_CrYCbY,  ///< Plane[0] : V0.Y0.U0.Y1. / 32bpp
   ICP_COLOR_YCbCr422_CbYCrY,  ///< Plane[0] : U0.Y0.V0.Y1. / 32bpp

   ICP_COLOR_YCbCr422_Y_Cb_Cr, ///< Plane[0] : Y / 8bpp  \n
   ///< Plane[1] : Cb / 8bpp \n
   ///< Plane[2] : Cr / 8bpp

   ICP_COLOR_YCbCr422_Y_CbCr,  ///< Plane[0] : Byte Order : Y / 8bpp  \n
   ///< Plane[1] : Byte Order : Cb.Cr / 16bpp


   //YCbCr420
   ICP_COLOR_YCbCr420_Y_CbCr,  ///< Plane[0] : Byte Order : Y / 8bpp  \n
   ///< Plane[1] : Byte Order : Cb.Cr / 16bpp

   ICP_COLOR_YCbCr420_Y_CrCb,  ///< Plane[0] : Byte Order : Y / 8bpp  \n
   ///< Plane[1] : Byte Order : Cr.Cb / 16bpp


   ICP_COLOR_YCbCr420_Y_Cb_Cr, ///< Plane[0] : Y / 8bpp  \n
   ///< Plane[1] : Cb / 8bpp \n
   ///< Plane[2] : Cr / 8bpp

   //Misc
   ICP_COLOR_XYZ444_XYZ,       ///< Plane[0] : X.Y.Z. / 32bpp
   ICP_COLOR_HSV_HSV,          ///< Plane[0] : H.S.V. / 32bpp

   //Single Channel Formats
   ICP_COLOR_GRAYSCALE,        ///< Plane[0] : Y / 8bpp

   //Invalid/uninitialized
   ICP_COLOR_INVALID,          ///<ICP_COLOR_INVALID
};

#endif /* ICPIMAGEFORMAT_H */