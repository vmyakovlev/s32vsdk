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
* \file     sdi_types.hpp
* \brief    Types types definition.
* \author   Tomas Babinec
* \version  0.1
* \date     17-September-2013
****************************************************************************/

#ifndef SDITYPES_HPP
#define SDITYPES_HPP

#include "global_errors.h"
#include "../../../isp/csi/kernel/include/csi_types.h"
#include "../../../isp/h264dec/kernel/common/include/h264dcd_types.h"
#include "../../../isp/jpegdec/kernel/common/include/jpegdcd_types.h"
#include "isp_viu.h"
#include "seq_graph_meta.h"

/*****************************************************************************
* defines
*****************************************************************************/

//#define SDI_MAX_OUTPUT_CNT 5  ///< maximum allowed number of simultaneous outputs
#define SDI_SENSOR_CNT 4      ///< number off supported sensors
#define SDI_MAX_CLIENT_NUM 10 ///< maxim number of clients for SDI

/*****************************************************************************
* enums
*****************************************************************************/

/***************************************************************************/
/** Enumeration of supported pixel formats.
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
   GS8,                     ///< Grayscale 8 bit; be wary when using this type since for CV220X, SDI will allocate an extra memory buffer per window.
   SDI_IMAGE_FORMAT_NONE,   ///< No format
   SDI_IMAGE_FORMAT_INVALID ///< Invalid format
} SDI_IMAGE_FORMAT;

/***************************************************************************/
/** Enumeration of supported sensors.
 *
 ***************************************************************************/

enum SDI_SENSOR
{
  SDI_SENSOR_INVALID = 0, ///!< SDI_SENSOR_INVALID
  SDI_SENSOR_CAM1,        ///!< SDI_SENSOR_CAM1
  SDI_SENSOR_CAM2,        ///!< SDI_SENSOR_CAM2
  SDI_SENSOR_ETH1,        ///!< SDI_SENSOR_ETH1
  SDI_SENSOR_ETH2         ///!< SDI_SENSOR_ETH2
}; // SDI_SENSOR

/***************************************************************************/
/** Enumeration of possible SDI parameters.
 *
 ***************************************************************************/
enum SDI_IO_PARAM
{
  SDI_IO_PARAM_INVALID,       //!< SDI_IO_PARAM_INVALID
  SDI_IO_PARAM_DDR_BUF_LIST,  //!< list of DDR buffers
  SDI_IO_PARAM_SRAM_BUF,
  SDI_IO_PARAM_AE_LOCK,       //!< SDI_IO_PARAM_AE_LOCK
  SDI_IO_PARAM_AUTOFOCUS,     //!< SDI_IO_PARAM_AUTOFOCUS
  SDI_IO_PARAM_AWB_LOCK,      //!< SDI_IO_PARAM_AWB_LOCK
  SDI_IO_PARAM_EFFECT,        //!< SDI_IO_PARAM_EFFECT
  SDI_IO_PARAM_ESD_STATUS,    //!< SDI_IO_PARAM_ESD_STATUS
  SDI_IO_PARAM_EXPOSURE,      //!< SDI_IO_PARAM_EXPOSURE
  SDI_IO_PARAM_FACE_DETECTION,//!< SDI_IO_PARAM_FACE_DETECTION
  SDI_IO_PARAM_FLICKER_MODE,  //!< SDI_IO_PARAM_FLICKER_MODE
  SDI_IO_PARAM_ILLUMINANCE,   //!< SDI_IO_PARAM_ILLUMINANCE
  SDI_IO_PARAM_MACRO,         //!< SDI_IO_PARAM_MACRO
  SDI_IO_PARAM_SATURATION,    //!< SDI_IO_PARAM_SATURATION
  SDI_IO_PARAM_SCENE_MODE,    //!< SDI_IO_PARAM_SCENE_MODE
  SDI_IO_PARAM_SHARPNESS,     //!< SDI_IO_PARAM_SHARPNESS
  SDI_IO_PARAM_SPOT,          //!< SDI_IO_PARAM_SPOT
  SDI_IO_PARAM_STABILIZATION, //!< SDI_IO_PARAM_STABILIZATION
  SDI_IO_PARAM_STROBE,        //!< SDI_IO_PARAM_STROBE
  SDI_IO_PARAM_STROBE_MODE,   //!< SDI_IO_PARAM_STROBE_MODE
  SDI_IO_PARAM_WDR,           //!< SDI_IO_PARAM_WDR
  SDI_IO_PARAM_WHITE_BALANCE, //!< SDI_IO_PARAM_WHITE_BALANCE
  SDI_IO_PARAM_INPUT_SOURCE,  //!< SDI_IO_PARAM_INPUT_SOURCE
  SDI_IO_PARAM_FRAME_RATE,    //!< SDI_IO_PARAM_FRAME_RATE
  SDI_IO_PARAM_MIRROR,        //!< SDI_IO_PARAM_MIRROR
  SDI_IO_PARAM_LAST           ///< last parameter in the list
}; // SDI_IO_PARAM

/***************************************************************************/
/** Enumeration of possible SDI preprocessing steps.
 *
 ***************************************************************************/
enum SDI_IO_PROCESS
{
  SDI_PROCESS_NONE          , ///< None
  SDI_PROCESS_GRAYSCALE     , ///< Grayscale
  SDI_PROCESS_NEGATIVE      , ///< Negative
  SDI_PROCESS_SKETCH        , ///< Sketch
  SDI_PROCESS_SEPIA         , ///< Sepia
  SDI_PROCESS_EMBOSS        , ///< Emboss
  SDI_PROCESS_BLUE          , ///< Blue
  SDI_PROCESS_BW            , ///< Black/White
  SDI_PROCESS_ORANGE        , ///< Orange
  SDI_PROCESS_GREEN         , ///< Green
  SDI_PROCESS_AQUA          , ///< Aqua
  SDI_PROCESS_ANTIQUE       , ///< Antique
  SDI_PROCESS_MOONLIGHT     , ///< Moon light
  SDI_PROCESS_CONTRAST      , ///< Contrast
  SDI_PROCESS_COOL          , ///< Cool
  SDI_PROCESS_VIOLET        , ///< Violet
  SDI_PROCESS_EDGE_DETECTION, ///< Edge Detection
  SDI_PROCESS_SOLARIZE      , ///< Solarize
  SDI_PROCESS_WATERCOLOR    , ///< Water color
  SDI_PROCESS_NOISEREDUCTION, ///< Noise reduction
  SDI_PROCESS_YELLOW        , ///< Yellow
  SDI_PROCESS_NIGHT         , ///< Night
  SDI_PROCESS_RED           , ///< Red
  SDI_PROCESS_PINK          , ///< Pink
  SDI_PROCESS_PURPLE        , ///< Purple
  SDI_PROCESS_OUTLINE,        ///< Outline
  SDI_PROCESS_ALL,            ///< all PROCESSs passed as structure in the mpValue of ParameterSet();
}; // SDI_IO_PROCESS

/***************************************************************************/
/** Enumeration of possible SDI functionality return values.
 *
 ***************************************************************************/
enum SDI_RESULT
{
  SDI_SUCCESS = LIB_SUCCESS,  ///< everything went ok
  SDI_FAILURE = LIB_FAILURE,  ///< something failed (general information)
  SDI_NOT_ENOUGH_RESOURCES,   ///< SDI has not enough resources to support this requirement
  SDI_GRABBER_NOT_READY,      ///< grabber is not ready for requested operation
}; // SDI_RESULT

/***************************************************************************/
/** Describes configuration of one virtual channel on MIPI-CSI2 interface.
 *
 ***************************************************************************/

struct SDI_MipiVcDescriptor
{
  uint32_t mBufAddr;          ///< SRAM buffer address
  uint32_t mLineCnt;          ///< number of lines fitting into buffer
  uint32_t mLineBytes;        ///< number of bytes per line
  uint32_t mLineStride;       ///< number of bytes to next line begin
  uint32_t mFrameHeight;      ///< expected number of lines per frame
}; // SDI_MipiVcDescriptor_t; 

/***************************************************************************/
/** Describes configuration of one MIPI-CSI2 interface.
 *
 ***************************************************************************/

struct SDI_MipiDescriptor
{
  enum CSI_IDX            mIdx;         ///< Mipi CSI index;
  SDI_MipiVcDescriptor    mpVc[4];      ///< VC config
  CSI_CameraType_t        mCamType;     ///< type of the MIPI camera connected
  uint32_t                mNumCameras;  ///< number of attached cameras (MAXIM)
}; // SDI_MipiDescriptor_t;

/***************************************************************************/
/** Describers configuration of one VIU interface
 *
 ***************************************************************************/

struct SDI_ViuDescriptor
{
  enum VIU_IDX      mIdx;          ///< VIU index;
  uint32_t          mBufAddr;      ///< SRAM buffer address
  uint32_t          mLineCnt;      ///< number of lines fitting into buffer
  uint32_t          mLineBytes;    ///< number of bytes per line
  uint32_t          mLinePadding;  ///< number of bytes padded (end->next line)
  VIU_CameraType_t  mCamType;      ///< type of the VIU camera connected
}; // SDI_ViuDescriptor_t;

/***************************************************************************/
/** Image component buffer descriptor for codecs.
 *
 ***************************************************************************/
struct SDI_ComponentDescriptor
{
  uint32_t mBufAddr;          ///< SRAM buffer address
  uint32_t mLineCnt;          ///< number of lines fitting into buffer
  uint32_t mLineBytes;        ///< number of bytes per line
  uint32_t mLineStride;       ///< number of bytes to next line begin
}; // SDI_ComponentDescriptor; 

/***************************************************************************/
/** Describer one Jpeg output component buffer geometry.
 *
 ***************************************************************************/
struct SDI_JpegDecStreamDescriptor
{
  uint16_t        mHeight;              ///< coded image y size in lines
  SAMPLE_PREC     mPrecision;           ///< PREC_8BITS/PREC_12BITS
  uint8_t         mComponentCnt;        ///< number of color components
  JPEGDCD_BOOL    mWrapperConfig;       ///< JPEGDCD_ON/JPEGDCD_OFF == 
                                        ///< configured/decoded params
  uint8_t         mC1_samplingfactor_h; ///< sampling factors
  uint8_t         mC1_samplingfactor_v;
  uint8_t         mC2_samplingfactor_h;
  uint8_t         mC2_samplingfactor_v;
  uint8_t         mC3_samplingfactor_h;
  uint8_t         mC3_samplingfactor_v;
  uint8_t         mC4_samplingfactor_h;
  uint8_t         mC4_samplingfactor_v;
}; // SDI_JpegDecStreamDescriptor; 

/***************************************************************************/
/** Enumeration of possible JPEG image components.
 *
 ***************************************************************************/

enum SDI_JPEG_COMPONENT
{
  SDI_JPEG_COMPONENT_Y = 0,
  SDI_JPEG_COMPONENT_U = 1,
  SDI_JPEG_COMPONENT_V = 2,
  SDI_JPEG_COMPONENT_A = 3,
}; // enum SDI_JPEG_COMPONENT

/***************************************************************************/
/** Describer Jpeg configureation.
 *
 ***************************************************************************/

struct SDI_JpegDecDescriptor
{
  COMPAT_MODE  mMode;       ///< CONTEXT_SWITCH/COMPATIBILITY
  JPEGDCD_BOOL mOutBufSmpl; ///< JPEGDCD_ON to resample output buffer addresses
  uint8_t      mStreamCnt;  ///< number of active streams
  uint8_t      mBankStride; ///< mBankStride * 8 bytes
  uint32_t     mRstInterval;///< Reset interval in number of lines
  
  SDI_ComponentDescriptor     mpCompDesc[4];   ///< JPEG has 4 component buffers
  SDI_JpegDecStreamDescriptor mpStreamDesc[4]; ///< JPEG decodes upto 4 streams
}; // SDI_JpegDecDescriptor;

/***************************************************************************/
/** Enumeration of possible H264 stream image components.
 *
 ***************************************************************************/

enum SDI_H264_COMPONENT
{
  SDI_H264_COMPONENT_Y  = 0,
  SDI_H264_COMPONENT_CB = 1,
  SDI_H264_COMPONENT_CR = 2,
}; // enum SDI_H264_COMPONENT

/***************************************************************************/
/** Describes configuration of H264 encoder.
 *
 ***************************************************************************/

struct SDI_H264EncDescriptor
{
  SDI_ComponentDescriptor   mpCompDesc[3];   ///< H264 has 3 component buffers
  // *** stream data description ***
  uint32_t mWidth;          ///< expected number of pixels per line
  uint32_t mHeight;         ///< expected number of lines per frame
  
  uint32_t mOutBuffAddr;    ///< address of the output buffer in SRAM
  uint32_t mOutBuffSize;    ///< size (bytes) of the output buffer in SRAM 
}; // SDI_H264EncDescriptor;

/***************************************************************************/
/** Describes configuration of H264 decoder output stream.
 *
 ***************************************************************************/
struct SDI_H264DecStreamDescriptor
{
  uint32_t mHeightLines;    ///< expected number of lines per frame
  uint32_t mActive;         ///< 0 when disabled, != 0 if enabled
  
  H264DCD_COL_FORMAT       mColFmt;            ///< COL_YUV420/COL_GREYSCALE
  H264DCD_OUTPUT_BIT_WIDTH mBitWidth;          ///< MODE_8BITS/MODE_10_12BITS
  H264DCD_BOOL             mOutBufRestartMode; ///< H264DCD_OFF/H264DCD_ON
  
  SDI_ComponentDescriptor  mpCompDesc[3];      ///< 3 components in H264 stream
}; // SDI_H264DecStreamDescriptor; 

/***************************************************************************/
/** Describes configuration of H264 decoder.
 *
 ***************************************************************************/
struct SDI_H264DecDescriptor
{
  H264DCD_DATA_MODE     mDataModeChroma; ///< Offset binary/twos complement
  H264DCD_DATAFLOW_MODE mDataFlowMode;   ///< MODE_0/MODE_1/MODE_2
  H264DCD_DECOD_CONFIG  mDecConfig;      ///< deblocking & SRAM/DDR reff. addr.
 
  SDI_H264DecStreamDescriptor mpStreamDesc[4]; ///< upto 4 streams to decode
}; // SDI_H264DecDescriptor;

//*****************************************************************************

/****************************************************************************/
/** Structure for simple image description.
 *
 * Describes image dimensions and format
 ****************************************************************************/
struct SDI_ImageDescriptor
{
  uint32_t mWidth; ///< image width  [pixels]
  uint32_t mHeight; ///< image height [pixels]
  uint32_t mWidthStep; ///< image line length in [bytes]
  uint32_t mBpp; ///< bits per pixel [bits]

  SDI_IMAGE_FORMAT mPixelFormat; ///< pixel format
  uint32_t mCvImageFormat; ///< OpneCV/UMat pixel format

  /****************************************************************************/
  /** Nonparametric class constructor.
   *
   * Sets members to default (0, SDI_IMAGE_FORMAT_NONE).
   *
   ****************************************************************************/
  SDI_ImageDescriptor();

  /****************************************************************************/
  /** Copy constructor
   *
   ****************************************************************************/
  SDI_ImageDescriptor(const SDI_ImageDescriptor &arcSrc);

  /****************************************************************************/
  /** Sets member as specified by attributes
   *
   * Determines bits per pixel and image line length based on image format.
   *
   * \param aWidth        image width [pixels]
   * \param aHeight       image height [pixels]
   * \param aPixelFormat  pixel format
   ****************************************************************************/
  SDI_ImageDescriptor(uint32_t aWidth,
                      uint32_t aHeight,
                      SDI_IMAGE_FORMAT aPixelFormat);

  /****************************************************************************/
  /** Does member by member assignment.
   *
   * \param arcSrc instance to be copied.
   * \return reference to this object after assignment.
   ****************************************************************************/
  SDI_ImageDescriptor& operator = (const SDI_ImageDescriptor& arcSrc);

  /****************************************************************************/
  /** Sets members to default values.
   *
   ****************************************************************************/
  ~SDI_ImageDescriptor();
};

#endif /* SDITYPES_HPP */
