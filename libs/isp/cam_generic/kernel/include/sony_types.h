/*
 * Copyright (c) 2016 Freescale Semiconductor
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
/**
* \file     sony_types.h
* \brief    types declarations for Sony_IMX224 camera driver
* \author   Tomas Babinec
* \version  0.1
* \date     20.10.2015
* \note
****************************************************************************/
#ifndef SONYTYPES_H
#define SONYTYPES_H

#ifdef __cplusplus 
extern "C" { 
#endif

#if !defined(__KERNEL__) && !defined(__STANDALONE__)
#include <stdint.h>

#endif 
  
#include "cam_types.h"  




#define SONY_SINGLE_EXP ((uint8_t)1)
#define SONY_DUAL_EXP   ((uint8_t)2)
  
#define SONY_MASTER     0  
#define SONY_SLAVE      1
  
#define SONY_CROP       0 
#define SONY_720P       1
#define SONY_QVGA       2
  
#define SONY_EXP_AG 1       
#define SONY_EXP_EL 2       
#define SONY_EXP_CG 4       
  



  
typedef struct SONY_Geometry
{
  enum CSI_IDX mCsiIdx; 
  
  
  uint32_t mWinMode; 
  
  
  uint32_t mVmax;   
  uint32_t mHmax;   
  
  
  uint32_t mXoff;
  uint32_t mYoff;
  uint32_t mWidth;
  uint32_t mHeight;
  uint8_t  mOb;     
  
  
  uint8_t mVerFlip;
  uint8_t mHorFlip;
  
  
  uint8_t mFps;     
  
  
  uint8_t mExpMode; 
  
  
  uint8_t mSyncMode;  
  
} SONY_Geometry_t;



typedef struct SONY_Exposure
{
  enum CSI_IDX mCsiIdx; 
  
  uint32_t mAnalogGain;          
  uint32_t mExposureLines;       
  uint32_t mExposureLinesShort;  
  uint8_t  mConversionGain;      
  
  uint8_t mConfigFlags;     
} SONY_Exposure_t;



typedef enum SONY_ExpState
{
  SONY_EXP_SENT = 1,  
  SONY_EXP_APPLIED,   
  SONY_EXP_USED,      
  SONY_EXP_READY,     
}  SONY_ExpState_t;



typedef struct SONY_ChGain
{
  enum CSI_IDX mCsiIdx;     
  
  uint16_t mLevel[4];       
  uint16_t mMax;            
  uint16_t mMin;            
  
}  SONY_ChGain_t;



#ifdef __cplusplus 
} 
#endif

#endif /* SONYTYPES_H */