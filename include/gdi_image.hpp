/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor
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
 * \file     gdi_imgae.hpp
 * \brief    GDI_Image and GDI_Mat type definitions.
 * \author   Tomas Babinec
 * \version  0.1
 * \date     10-01-2014
 * \note
 ****************************************************************************/

#ifndef GDIIMAGE_HPP
#define GDIIMAGE_HPP

#include <stdio.h>

// *** to avoid conflicting typedefs ***
#define int64 global_int64
#define uint64 global_uint64
  #include "../libs/io/gdi/include/gdi_types_public.hpp"
#undef  int64
#undef  uint64

#ifdef GDI_OCV
  #define int64 ocv_int64
  #define uint64 ocv_uint64
    #include "opencv2/core/core.hpp"
  #undef  int64
  #undef  uint64
  #define int64 ambiguous use global_int64 or ocv_int64
  #define uint64 ambiguous use global_uint64 or ocv_uint64
#else 
  #define int64 global_int64
  #define uint64 global_uint64
#endif

#include "umat.hpp"

/*****************************************************************************
 * preliminary types declaration
 *****************************************************************************/
class GDI_Mat;

/***************************************************************************/
/** Describes one pixel data (packed format).
 *
 ***************************************************************************/
struct GDI_PackedPixel
{
  GDI_IMAGE_FORMAT mFormat;
  GDI_ColorRGBA mData;

  GDI_PackedPixel();
};

/****************************************************************************/
/** Structure for simple image description.
 *
 * Describes image dimensions and format
 ****************************************************************************/
struct GDI_ImageDescriptor
{
  uint32_t mWidth; ///< image width  [pixels]
  uint32_t mHeight; ///< image height [pixels]
  uint32_t mWidthStep; ///< image line length in [bytes]
  uint32_t mBpp; ///< bits per pixel [bits]

  GDI_IMAGE_FORMAT mPixelFormat; ///< pixel format

  /****************************************************************************/
  /** Nonparametric class constructor.
   *
   * Sets members to default (0, GDI_IMAGE_FORMAT_NONE).
   *
   ****************************************************************************/
  GDI_ImageDescriptor();

  /****************************************************************************/
  /** Copy constructor
   *
   ****************************************************************************/
  GDI_ImageDescriptor(const GDI_ImageDescriptor &arcSrc);

  /****************************************************************************/
  /** Sets member as specified by attributes
   *
   * Determines bits per pixel and image line length based on image format.
   *
   * \param aWidth        image width [pixels]
   * \param aHeight       image height [pixels]
   * \param aPixelFormat  pixel format
   ****************************************************************************/
  GDI_ImageDescriptor(uint32_t aWidth,
                      uint32_t aHeight,
                      GDI_IMAGE_FORMAT aPixelFormat);

  /****************************************************************************/
  /** Does member by member assignment.
   *
   * \param arcSrc instance to be copied.
   * \return reference to this object after assignment.
   ****************************************************************************/
  GDI_ImageDescriptor& operator = (const GDI_ImageDescriptor& arcSrc);

  /****************************************************************************/
  /** Sets members to default values.
   *
   ****************************************************************************/
  ~GDI_ImageDescriptor();
};
//  GDI_ImageDescriptor

//  holds whole image representation compiled from image descriptor and image data
/****************************************************************************/
/** Structure for simple image data handling.
 *
 * Includes data description and image data pointer.
 *
 ****************************************************************************/
struct GDI_Image
{
private:
  bool mDelData;    ///< if the data should be deleted when destructor called
public:
  uint64_t mHwAddress; ///< HW address of allocated memory (only if contiguous, otherwise 0)
  GDI_ImageDescriptor mDesc; ///< image descriptor member
  uint8_t *mData; ///< pointer to image data

  /****************************************************************************/
  /** Nonparametric image class constructor
   *
   * Sets members to default (0, NULL, GDI_IMAGE_FORMAT_NONE).
   *
   ****************************************************************************/
  GDI_Image();

  /****************************************************************************/
  /** Initializes image parameters according to specified image descriptor.
   *
   * No data memory allocation. Create method has to be called.
   *
   * \param arcDesc  image descriptor with required parameters.
   ****************************************************************************/
  GDI_Image(const GDI_ImageDescriptor &arcDesc);

  /****************************************************************************/
  /** Initializes image instance but no memory allocated for data.
   *
   * To allocate appropriate data memory, call to Create method is required.
   *
   * \param aWidth        image width [pixels]
   * \param aHeight       image height [pixels]
   * \param aPixelFormat  pixel format
   */
  GDI_Image(uint32_t aWidth, uint32_t aHeight, GDI_IMAGE_FORMAT aPixelFormat);

  /****************************************************************************/
  /** Copy-constructor.
   *
   * \param arcGdiImage source image data.
   ****************************************************************************/
  GDI_Image(const GDI_Image& arcGdiImage);

#ifdef GDI_OCV
  /****************************************************************************/
  /** Conversion constructor from GDI_Mat.
   *
   * \param arcGdiMat source image data.
   ****************************************************************************/
  GDI_Image(const GDI_Mat& arcGdiMat);
#endif

  /****************************************************************************/
  /** Calls Destroy() method.
   *
   ****************************************************************************/
  ~GDI_Image();

  /****************************************************************************/
  /** Sets new image descriptor parameters.
   *
   * If data already allocated to the old descriptor, call to Destroy() required
   * first.
   *
   * \param aDesc new image descrtiption parameters
   *
   * \return LIB_SUCCESS if all ok else LIB_FAILURE
   ****************************************************************************/
  LIB_RESULT DescriptorSet(const GDI_ImageDescriptor &aDesc);

  /****************************************************************************/
  /** Allocates image data memory.
   *
   * Image descriptor has to be set up before calling this method.
   *
   * \return LIB_SUCCESS/LIB_FAILURE
   ****************************************************************************/
  LIB_RESULT Create();

  /****************************************************************************/
  /** Creates a deep copy of the image.
   *
   * \return Deep copy of the image object.
   ****************************************************************************/
  GDI_Image Clone() const;

  /****************************************************************************/
  /** Deallocates image data memory.
   *
   * Resets structure members to defaults.
   *
   ****************************************************************************/
  void Destroy();

  /****************************************************************************/
  /** Copies this data between this image and the specified image.
   *
   * Image dimensions and pixel format have to match between images.
   *
   * \param   aDstImg reference to a destination image
   * \return  LIB_SUCCESS/LIB_FAILURE
   ****************************************************************************/
  LIB_RESULT CopyTo(GDI_Image &aDstImg) const;

  /****************************************************************************/
  /** Sets specified image pixel to the defined color
   *
   * \param   aX x coordinate of the pixel
   * \param   aY y coordinate of the pixel
   * \param   apPixData new pixel data in image appropriat format
   *
   * \return LIB_SUCCESS/LIB_FAILURE
   ****************************************************************************/
  LIB_RESULT PixelPut(uint32_t aX, uint32_t aY, uint8_t *apPixData);

  /****************************************************************************/
  /** Sets up specified image object to behave as ROI for this image.
   *
   * \param arcRectangle roi defining rectangle
   * \param arRoiImage image which should be set up as roi
   *
   * \return LIB_SUCCESS/LIB_FAILURE
   ****************************************************************************/
  LIB_RESULT RoiGet(const GDI_Rectangle &arcRectangle, GDI_Image &arRoiImage);

  /****************************************************************************/
  /** Finds out data ownership.
   *
   * \return true if data will get deallocated together with this object, otherwise false
   ****************************************************************************/
  bool DataOwnershipGet();
};
//  GDI_Image

#ifdef GDI_OCV
/****************************************************************************/
/** Maps GdiImageFormats to OpenCv data types.
 *
 * \param aGdiImageFormat pixel format of gdi image
 * \param arCvType suitable representation in OpenCv.
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE (unknown GDI_IMAGE_FORMAT).
 ****************************************************************************/
LIB_RESULT GDI_ImageFormat2CvType(GDI_IMAGE_FORMAT aGdiImageFormat,
                                  int &arCvType);

/****************************************************************************/
/** OpenCv Mat class descendant.
 *
 * To be able to incorporate GDI specific info (pixel format, CMA info,...) and
 * use it with OCV at the same time.
 *
 * \note Aimed at use with images only. Only basic cv::Mat interface supported.
 ****************************************************************************/
class GDI_Mat: public cv::Mat
{
public:
  uint64_t mHwAddress; ///< HW address of allocated memory (only if contiguous, otherwise 0)
  GDI_IMAGE_FORMAT mPixelFormat;

  /****************************************************************************/
  /** Simple constructor.
   *
   * Sets default values.
   *
   ****************************************************************************/
  GDI_Mat();

  /****************************************************************************/
  /** Parametric constructor.
   *
   * \param aWidth Image width in pixels.
   * \param aHeight Image height in pixels.
   * \param aPixelFormat Pixel format for GDI use.
   ****************************************************************************/
  GDI_Mat(uint32_t aWidth, uint32_t aHeight, GDI_IMAGE_FORMAT aPixelFormat);

  /****************************************************************************/
  /** Copy constructor.
   *
   * \param arcMat The source image to be copied.
   ****************************************************************************/
  GDI_Mat(const GDI_Mat &arcMat);

  /****************************************************************************/
  /** Constructs new GDI_Mat as a Roi.
   *
   * \param arcM Source image data.
   * \param arcRoi Roi specification.
   ****************************************************************************/
  GDI_Mat(const GDI_Mat& arcM, const cv::Rect& arcRoi);

  /****************************************************************************/
  /** Constructor from user-allocated data.
   *
   * \param aWidth image width;
   * \param aHeight image height;
   * \param apData pointer to image data;
   * \param aImageFormat image pixel format;
   * \param aStep one row width step in bytes.
   ****************************************************************************/
  GDI_Mat(uint32_t aWidth,
          uint32_t aHeight,
          void* apData,
          GDI_IMAGE_FORMAT aImageFormat,
          size_t aStep = AUTO_STEP);

  /****************************************************************************/
  /** Conversion constructor from GDI_Image.
   *
   * \param arcGdiImage source image to be converted.
   ****************************************************************************/
  GDI_Mat(const GDI_Image &arcGdiImage);

  /****************************************************************************/
  /** Assignment operator from cv::Mat.
   *
   * \param arcM  Source image to be assigned to this object.
   * \return Reference to this object after assignment.
   *
   * \note GDI_Image specific fields stay untouched.
   ****************************************************************************/
  Mat& operator =(const Mat& arcM);

  /****************************************************************************/
  /** Assignment operator from GDI_Image.
   *
   * \param arcGDI_Mat Image to be assigned to this object.
   * \return Reference to this object after assignment.
   ****************************************************************************/
  GDI_Mat& operator =(const GDI_Mat& arcGDI_Mat);

  /****************************************************************************/
  /** Creates deep copy of the image object.
   *
   * \return Reference to newly created image object with data copy.
   ****************************************************************************/
  GDI_Mat clone();

  // todo: CMA

  /****************************************************************************/
  /** Destructor.
   *
   ****************************************************************************/
  ~GDI_Mat();
};
#endif

#endif /* GDIIMAGE_HPP */
