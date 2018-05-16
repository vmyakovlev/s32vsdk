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
* \file     ov10635_types.h
* \brief    types declarations for OV10635 camera driver
* \author   Tomas Babinec
* \version  0.1
* \date     20.10.2015
* \note
****************************************************************************/
#ifndef OV10635TYPES_H
#define OV10635TYPES_H

#ifdef __cplusplus 
extern "C" { 
#endif

#if !defined(__KERNEL__) && !defined(__STANDALONE__)
#include <stdint.h>

#endif 







  
typedef struct OV10635_Geometry
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
  
} OV10635_Geometry_t;



typedef struct OV10635_Exposure
{
  enum CSI_IDX mCsiIdx; 
  
  uint32_t mAnalogGain;          
  uint32_t mExposureLines;       
  uint32_t mExposureLinesShort;  
  uint8_t  mConversionGain;      
  
  uint8_t mConfigFlags;     
} OV10635_Exposure_t;



typedef enum OV10635_ExpState
{
  OV10635_EXP_SENT = 1,  
  OV10635_EXP_APPLIED,   
  OV10635_EXP_USED,      
  OV10635_EXP_READY,     
}  OV10635_ExpState_t;



typedef struct OV10635_ChGain
{
  enum CSI_IDX mCsiIdx;     
  
  uint16_t mLevel[4];       
  uint16_t mMax;            
  uint16_t mMin;            
  
}  OV10635_ChGain_t;



#ifdef __cplusplus 
} 
#endif

#endif /* OV10635TYPES_H */