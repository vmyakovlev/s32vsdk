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

/*==============================================================================
Revision History:
                        Modification       Tracking
Author (core ID)        Date D/M/Y         Number     Description of Changes
Roman Kubica            13-October-2016               Init version

==============================================================================*/

#ifndef H264ENC_H
#define H264ENC_H

/**
* \file     h264enc.h
* \brief    declarations for H264 Encoder driver
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==============================================================================
*                             INCLUDE FILES
==============================================================================*/

#include <linux/ioctl.h>

/*==============================================================================
*                             CONSTANTS
==============================================================================*/

/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/


#define H264ENC_FRAME_DONE_SIGNAL    45 /* H264Enc signals for userspace */
 
#define H264ENC_DRV_SUCCESS          0  /* H264Enc status */
#define H264ENC_DRV_FAILURE         -1

#define H264ENC_DEVICE_NAME         "h264enc" /* name of the device file */

#define H264ENC_IOC_MAGIC           'h' + 'e' /* magic number for driver */

#define H264ENC_BASE                0x400DC000 /* H264Enc registers Offset */
#define H264ENC_SIZE                0x288

/*==============================================================================
*                                  ENUMS
==============================================================================*/

/*==============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/

/*==============================================================================
*                        GLOBAL VARIABLE DECLARATIONS
==============================================================================*/

/*=============================================================================
*                              FUNCTION PROTOTYPES
==============================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* H264ENC_H */

/** @} */
