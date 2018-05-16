/*
 * Copyright (c) 2014-2016 Freescale Semiconductor
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/*==============================================================================
Revision History:
                        Modification       Tracking
Author (core ID)        Date D/M/Y         Number     Description of Changes
Tomas Babinec           20-Oct-2015                   Init version
Xuyen Dao               05-Aug-2016        VSDK-251   Update template
==============================================================================*/
#ifndef OV10635TYPES_H
#define OV10635TYPES_H

#ifdef __cplusplus
extern "C"{
#endif

/**
* @file           ov10635_viu_types.h
* @brief          types declarations for OV10635 camera driver
*/

/*==============================================================================
*                               INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/

#if !defined(__KERNEL__) && !defined(__STANDALONE__)
#include <stdint.h>

#endif // if !defined(__KERNEL__) && !defined(__STANDALONE__)

/*==============================================================================
*                               CONSTANTS
==============================================================================*/


/*==============================================================================
*                          DEFINES AND MACROS
==============================================================================*/


/*==============================================================================
*                              ENUMS
==============================================================================*/
/**
* @brief          Enum OV10635_ExpState
* @details        Enum OV10635_ExpState
* @pre
* @post
*
*/
typedef enum OV10635_ExpState
{
  OV10635_EXP_SENT = 1,    // exposure update sent through i2c
  OV10635_EXP_APPLIED,     // exposure update applied to registers
  OV10635_EXP_USED,        // exposure update used for latest frame
  OV10635_EXP_READY,       // exposure ready to be updated again
} OV10635_ExpState_t; // OV10635_ExpState

/*==============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/
/**
* @brief          Struct OV10635_Geometry
* @details        Struct OV10635_Geometry
* @pre
* @post
*
*/
typedef struct OV10635_Geometry
{
  enum CSI_IDX mCsiIdx;   // CSI interface IDX

  // window mode
  uint32_t mWinMode;      // OV10635_QVGA, OV10635_720P, OV10635_CROP

  // frame max geometry
  uint32_t mVmax;         // maximum number of lines
  uint32_t mHmax;         // maximum number of pulses per line, only for readout

  // ROI
  uint32_t mXoff;
  uint32_t mYoff;
  uint32_t mWidth;
  uint32_t mHeight;
  uint8_t  mOb;           // optical black lines <6;16>

  // image flip
  uint8_t mVerFlip;
  uint8_t mHorFlip;

  // FPS
  uint8_t mFps;           // frames per second

  // single/dual exposure
  uint8_t mExpMode;       // OV10635_SINGLE_EXP or OV10635_DUAL_EXP

  // master/slave
  uint8_t mSyncMode;      // OV10635_MASTER or OV10635_SLAVE

} OV10635_Geometry_t;// OV10635_Geometry_t;

/**
* @brief          Struct OV10635_Exposure
* @details        Struct OV10635_Exposure
* @pre
* @post
*
*/
typedef struct OV10635_Exposure
{
  enum CSI_IDX mCsiIdx;          // CSI interface IDX

  uint32_t mAnalogGain;          // 0 <-> 72dB; <0; 720>
  uint32_t mExposureLines;       // <0; VMAX>
  uint32_t mExposureLinesShort;  // <0; VMAX>
  uint8_t  mConversionGain;      // Low, High; <0; 1>

  uint8_t mConfigFlags;          // what should be set; all if 0
} OV10635_Exposure_t; // OV10635_Exposure_t

/**
* @brief          Struct OV10635_ChGain
* @details        Struct OV10635_ChGain
* @pre
* @post
*
*/
typedef struct OV10635_ChGain
{
  enum CSI_IDX mCsiIdx;     // CSI interface IDX

  uint16_t mLevel[4];       // one per channel
  uint16_t mMax;            // maximum Level
  uint16_t mMin;            // minium Level

} OV10635_ChGain_t; // OV10635_ChGain_t;

/*==============================================================================
*                        GLOBAL VARIABLE DECLARATIONS
==============================================================================*/

/*==============================================================================
*                           FUNCTION PROTOTYPES
==============================================================================*/


#ifdef __cplusplus
} //extern "C" {
#endif

#endif /* OV10635TYPES_H */
