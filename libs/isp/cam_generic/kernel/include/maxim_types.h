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
* \file     csi_types.h
* \brief    types declarations for MAXIM HW setup drivers
* \author   Tomas Babinec
* \version  0.1
* \date     29.4.2016
* \note
****************************************************************************/
#ifndef MAXIMTYPES_H
#define MAXIMTYPES_H

#ifdef __cplusplus 
extern "C" { 
#endif

#if !defined(__KERNEL__) && !defined(__STANDALONE__)
#include <stdint.h>

#endif // if !defined(__KERNEL__) && !defined(__STANDALONE__)

#include "../../../csi/kernel/include/csi_types.h"
//*****************************************************************************
// const
//***************************************************************************** 
#define MAXIM_CAM_SLOTS    (uint8_t)4  //< number of slots in one maxim deserializer
#define MAXIM_CAM_ENABLED  (uint8_t)1  //< maxim camera slot used  
#define MAXIM_CAM_DISABLED (uint8_t)0  //< maxim camera stot not used
  
//*****************************************************************************
// macros
//*****************************************************************************  

//*****************************************************************************
// types
//*****************************************************************************  

enum MAXIM_CHNL
{
  MAXIM_CHNL_ORIG_BC = 0,    
  MAXIM_CHNL_0       = 1,
  MAXIM_CHNL_1       = 2,
  MAXIM_CHNL_2       = 3,
  MAXIM_CHNL_3       = 4,
  MAXIM_CHNL_INVALID = 5
}; // MAXIM channel index definition  

//*****************************************************************************

typedef struct MAXIM_Cfg
{
  enum CSI_IDX     mCsiIdx;    ///< Mipi CSI index the maxim is connected to
  CSI_CameraType_t mSensorType;///< sensor/camera type;
  uint32_t         mSensors[MAXIM_CAM_SLOTS]; ///< MAXIM_CAM_ENABLED/DISABLED
} MAXIM_Cfg_t; // MAXIM configuration

//*****************************************************************************

#ifdef __cplusplus 
} //extern "C" { 
#endif

#endif /* MAXIMTYPES_H */
