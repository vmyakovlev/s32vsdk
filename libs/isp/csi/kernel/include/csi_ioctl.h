/*
 * Copyright (c) 2015-2016 Freescale Semiconductor
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

#ifndef CSIIOCTL_H
#define CSIIOCTL_H

/**
* @file           csi_ioctl.h
* @brief          Declaration of H264 Encoder driver internal functionality
*/ 


/*=============================================================================
*                                        INCLUDE FILES
==============================================================================*/
#include <linux/fs.h>		// required for the struct file declaration

/*==============================================================================
*                            DEFINES AND MACROS
==============================================================================*/
#define IOCTL_CSI_CONFIG          _IOW(CSI_IOC_MAGIC, 0, char *)
#define IOCTL_CSI_EMBD_CONFIG     _IOW(CSI_IOC_MAGIC, 1, CSI_EmbdCfg_t*)
#define IOCTL_SONY_REG_CONFIG     _IOR(CSI_IOC_MAGIC, 3, uint32_t)
#define IOCTL_CSI_START           _IOW(CSI_IOC_MAGIC, 4, uint32_t)
#define IOCTL_CSI_STOP            _IOW(CSI_IOC_MAGIC, 5, uint32_t)

#define IOCTL_OV10635_REG_CONFIG  _IOR(CSI_IOC_MAGIC, 23, uint32_t)
#define IOCTL_OV10635_START       _IOW(CSI_IOC_MAGIC, 24, uint32_t)
#define IOCTL_OV10635_STOP        _IOW(CSI_IOC_MAGIC, 25, uint32_t)

#define IOCTL_OV10635_CONNECT     _IOW(CSI_IOC_MAGIC, 30, uint32_t)
#define IOCTL_OV10635_DISCONNECT  _IOW(CSI_IOC_MAGIC, 31, uint32_t)

#define IOCTL_CSI_EMBD_RECAP      _IOR(CSI_IOC_MAGIC, 34, uint32_t)
#define IOCTL_CSI_EMBD_STATUS_GET _IOR(CSI_IOC_MAGIC, 35, uint32_t)

#define IOCTL_CSI_ERR_STATUS_GET  _IOWR(CSI_IOC_MAGIC,36, uint32_t)
#define IOCTL_CSI_ERR_CLEAR       _IOR(CSI_IOC_MAGIC, 37, uint32_t)

// IOCTL command codes numbers allocation
// IOCTL: reserve CSI interface

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
// only for kernel space

/*==============================================================================
*                           FUNCTION PROTOTYPES
==============================================================================*/

long CsiIoctl(struct file *apFile,	/* ditto */
    unsigned int aIoctlNum,	/* number and param for ioctl */
    unsigned long aIoctlParam);

#endif

#endif /* CSIIOCTL_H */

/* EOF */
