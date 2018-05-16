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
/**
* \file     cam.h
* \brief    declarations for Generic camera driver
* \author   Tomas Babinec
* \version  0.1
* \date     16-June-2016
* \note
****************************************************************************/

#ifndef CAM_H
#define CAM_H

#ifdef __cplusplus 
extern "C" { 
#endif

#if !defined(__STANDALONE__)
#include <linux/ioctl.h>
# endif // !defined(__STANDALONE__)

#include "isp_hw.h"
#include "cam_types.h"
  
#ifndef __STANDALONE__  
  #include "cam_ioctl.h"
#endif // #ifndef __STANDALONE__  

/*****************************************************************************
* MACRO definitions
*****************************************************************************/

#define CAM_DRV_SUCCESS  0
#define CAM_DRV_FAILURE -1


#define I2C_CLI_CNT_MAX 50        ///< number of supported I2C clients
#define MAX_COMPATIBLE_BYTES 16

// name of the device file
#define CAM_DEVICE_NAME "cam_generic"

// magic number for fDMA driver
#define CAM_IOC_MAGIC 'c'

#ifdef __cplusplus 
} //extern "C" 
#endif

#endif /* CAM_H */