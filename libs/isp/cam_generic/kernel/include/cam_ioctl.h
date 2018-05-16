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
/**
* \file     cam_ioctl.h
* \brief    ioctl related declarations for Generic camera driver
* \author   Tomas Babinec
* \version  0.1
* \date     16. June 2016
* \note
****************************************************************************/

#ifndef CAMIOCTL_H
#define CAMIOCTL_H

#include <linux/fs.h>		// required for the struct file declaration

///////////////////////////////////////////
//
// IOCTL command codes numbers allocation
//

// IOCTL: reserve CAM interface
#define IOCTL_CAM_CLI_ADD         _IOWR(CAM_IOC_MAGIC, 0, CamI2cCfg_t*)
#define IOCTL_CAM_CLI_REM         _IOW(CAM_IOC_MAGIC,  1, CamI2cClient_t)
#define IOCTL_CAM_REG_RDB         _IOWR(CAM_IOC_MAGIC, 2, I2cReadBatch_t*)
#define IOCTL_CAM_REG_WRB         _IOW(CAM_IOC_MAGIC,  3, I2cWriteBatch_t*)
#define IOCTL_CAM_REG_RDS         _IOWR(CAM_IOC_MAGIC, 4, I2cReadBatch_t*)
#define IOCTL_CAM_REG_WRS         _IOW(CAM_IOC_MAGIC,  5, I2cWriteBatch_t*)
#define IOCTL_CAM_REG_WRT         _IOW(CAM_IOC_MAGIC,  6, I2cWriteBatch_t*)
#define IOCTL_SONY_CONNECT        _IOW(CAM_IOC_MAGIC,  7, uint32_t)
#define IOCTL_SONY_DISCONNECT     _IOW(CAM_IOC_MAGIC,  8, uint32_t)
#define IOCTL_SONY_EXP_STATUS_GET _IOW(CAM_IOC_MAGIC,  9, CamI2cClient_t)
#define IOCTL_SONY_EXP_SENT       _IOW(CAM_IOC_MAGIC, 10, CamI2cClient_t)
#define IOCTL_OV10640_CONNECT     _IOW(CAM_IOC_MAGIC, 11, uint32_t)
#define IOCTL_OV10640_DISCONNECT  _IOW(CAM_IOC_MAGIC, 12, uint32_t)

///////////////////////////////////////////
//
// IOCTL cmds processing code
//

// only for kernel space
#ifdef __KERNEL__

long CamIoctl(struct file *apFile,	/* ditto */
    unsigned int aIoctlNum,	/* number and param for ioctl */
    unsigned long aIoctlParam);

#endif

#endif /* CAMIOCTL_H */

/* EOF */
