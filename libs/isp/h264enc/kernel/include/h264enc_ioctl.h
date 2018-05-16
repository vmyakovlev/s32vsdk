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
                              Modification     Tracking
Author (core ID)              Date D/M/Y       Number     Description of Changes
---------------------------   ----------    ------------  ----------------------
Roman Kubica                  14/10/2016                   Initial Version
---------------------------   ----------    ------------  ----------------------
==============================================================================*/

#ifndef H264ENCIOCTL_H
#define H264ENCIOCTL_H

#ifdef __cplusplus
extern "C"{
#endif

/**
* @file           h264enc_ioctl.h 
*/



/*=============================================================================
*                                        INCLUDE FILES
==============================================================================*/

#include <linux/fs.h>		// required for the struct file declaration

/*==============================================================================
*                            DEFINES AND MACROS
==============================================================================*/

/* IOCTL: encodig configuration setup */
#define IOCTL_H264_ENC_CONFIG_SET _IOW(H264ENC_IOC_MAGIC, 0, char *)

/* IOCTL: set memory buffers */
#define IOCTL_H264_ENC_BUFFERS_SET _IOW(H264ENC_IOC_MAGIC, 1, char *)

/* IOCTL: how many more lines are available in input buffer */
#define IOCTL_H264_ENC_FETCH _IOW(H264ENC_IOC_MAGIC, 2, uint32_t)

/* IOCTL: invokes bistream alarm address shift */
#define IOCTL_H264_ENC_BS_ALARM_SHIFT _IOW(H264ENC_IOC_MAGIC, 3, uint32_t)

/* IOCTL: get current HW status */
#define IOCTL_H264_ENC_STATUS_GET _IOR(H264ENC_IOC_MAGIC, 4, char*)

/* IOCTL: stops the encoding sequence */
#define IOCTL_H264_ENC_STOP _IO(H264ENC_IOC_MAGIC, 5)

/* IOCTL: provides macroblock encoding status */
#define IOCTL_H264_ENC_BS_ROW_DONE_GET _IOR(H264ENC_IOC_MAGIC, 6, uint8_t *)

/* IOCTL */
#define IOCTL_H264_ENC_RESET_VARS _IO(H264ENC_IOC_MAGIC, 7)

/*==============================================================================
*                                  ENUMS
==============================================================================*/

/*=============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
* ============================================================================*/

/*==============================================================================
*                        GLOBAL VARIABLE DECLARATIONS
==============================================================================*/

/*==============================================================================
*                                   CONSTANTS
==============================================================================*/

#ifdef __KERNEL__

/*==============================================================================
*                           FUNCTION PROTOTYPES
==============================================================================*/

long H264EncIoctl(struct file *apFile, 
           unsigned int aIoctlNum,	/* number for ioctl */
           unsigned long aIoctlParam); /* parameter for ioctl */

#endif // ifdef __KERNEL__


#ifdef __cplusplus
}
#endif

#endif /* H264ENCIOCTL_H */
