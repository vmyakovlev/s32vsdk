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
* \brief    types declarations for Ov10640 camera driver
* \author   Tomas Babinec
* \version  0.1
* \date     26.05.2015
* \note
****************************************************************************/
#ifndef OV10640TYPES_H
#define OV10640TYPES_H

#ifdef __cplusplus 
extern "C" { 
#endif

#if !defined(__KERNEL__) && !defined(__STANDALONE__)
#include <stdint.h>

#endif 




#define OV10640_EXP_12COM 0  
#define OV10640_EXP_LONG  1  
#define OV10640_EXP_SHRT  2  
#define OV10640_EXP_VSHRT 4  
#define OV10640_EXP_3x12  8  
#define OV10640_EXP_CNLG  16 
#define OV10640_EXP_AUTO  32 
  
#define OV10640_EMBD_OFF  0  
#define OV10640_EMBD_REGS 1  
#define OV10640_EMBD_STAT 2  
  
#define OV10640_EXP_FL_RG    1       
#define OV10640_EXP_FL_RATIO 2       
#define OV10640_EXP_FL_EV    4       
#define OV10640_EXP_FL_CNLG  8       
   



  
typedef struct OV10640_Geometry
{
  enum CSI_IDX   mCsiIdx; 
  CamI2cClient_t mClient; 
  
  
  uint32_t mDvpVsize;
  uint32_t mDvpHsize;
  
  
  uint8_t mVerFlip;
  uint8_t mHorFlip;
  
  
  uint8_t mFps;     
  
  
  uint8_t mEmbd;    
  
  
  uint8_t mExpMode; 
} OV10640_Geometry_t;



typedef struct OV10640_Exposure
{
  enum CSI_IDX   mCsiIdx; 
  CamI2cClient_t mClient; 
  
  uint16_t mRealGainLong;
  uint16_t mRealGainShort;
  uint16_t mRealGainVeryShort;
  
  uint16_t mLongShortRatio;
  uint16_t mLongVeryShortRatio;
  uint16_t mShortVeryShortRatio;
  
  uint8_t  mLongestEvBytes[4];    
  uint8_t  mShortestEvBytes[4];  
  
  
  uint16_t mExposureTime[3];
  uint8_t  mDeflickeringEnable;	
  float	   mFlickerPeriod; 
  uint16_t mExpCoef[3];		
  uint8_t  mConversionGain[3];
  uint8_t  mConversionGainEnable[3];  
  uint8_t  mAnalogGain[3];
  uint16_t mDigitalGain[3];
  
  float mExposure[3];
  
  float mAECFactor;
  
  
  uint16_t mMinExposureTime[3];
  uint8_t  mMinAnalogGain[3];
  uint16_t mMinDigitalGain[3];  
  
  uint16_t mMaxExposureTime[3];
  uint8_t  mMaxAnalogGain[3];
  uint16_t mMaxDigitalGain[3];
  
  float mMinExposure[3];
  float mMaxExposure[3]; 
  
  float mRatioLSExp;	
  float mRatioLVSExp;	
  float mRatioPositionS;	
  float mRatioMargin;
  
  
  
  uint16_t mpLongChnlGain[4];
  uint16_t mpShortChnlGain[4];
  uint16_t mpVeryShortChnlGain[4];
    
  uint8_t  mConfigFlags;     
} OV10640_Exposure_t;



typedef struct OV10640_ChGain
{
  enum CSI_IDX   mCsiIdx;   
  CamI2cClient_t mClient;   
  
  uint16_t mLevel[4];       
  uint16_t mMax;            
  uint16_t mMin;            
  
} OV10640_ChGain_t;



typedef enum
{
  BLUE_CH =0,
  GREEN_B_CH = 1,
  GREEN_R_CH = 2,
  RED_CH = 3
} OV10640_ColorCh_t;



typedef enum
{
  ExpTime_VShort = 0,
  ExpTime_Short = 1,
  ExpTime_Long = 2
} OV10640_ExpTime_t;



#ifdef __cplusplus 
} 
#endif

#endif /* OV10640TYPES_H */
