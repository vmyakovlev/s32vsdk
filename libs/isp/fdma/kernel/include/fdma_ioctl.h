/*
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
* @file     fdma_ioctl.h
* @brief    ioctl related declarations for fDMA driver
****************************************************************************/

/*==============================================================================
Revision History:
                        Modification      Tracking
Author (core ID)        Date D/M/Y         Number     Description of Changes
Tomas Babinec           27-May-2014                    Inital version.
Tomas Babinec           23-May-2017       vsdk-575     Tracebility.
==============================================================================*/

#ifndef FDMAIOCTL_H
#define FDMAIOCTL_H

#include <linux/fs.h>		// required for the struct file declaration

///////////////////////////////////////////
//
// IOCTL command codes numbers allocation
//

// IOCTL: transaction descriptor reservation
#define IOCTL_FDMA_TD_RESERVE _IOR(FDMA_IOC_MAGIC, 0, char *)

// IOCTL: transaction configuration
#define IOCTL_FDMA_TD_CONFIG _IOW(FDMA_IOC_MAGIC, 1, char *)

// IOCTL: transaction configuration
#define IOCTL_FDMA_TD_ISP_CONFIG _IOW(FDMA_IOC_MAGIC, 10, char *)

// IOCTL: transaction scheduling
#define IOCTL_FDMA_TD_SCHEDULE _IOW(FDMA_IOC_MAGIC, 2, char *)

// IOCTL: transaction configuration and scheduling
#define IOCTL_FDMA_TD_CONFIG_SCHEDULE _IOW(FDMA_IOC_MAGIC, 3, char *)

// IOCTL: release transaction descriptor
#define IOCTL_FDMA_TD_RELEASE _IOW(FDMA_IOC_MAGIC, 4, char*)

// IOCTL: get copy of current fDMA registers
#define IOCTL_FDMA_SEQ_MODE_GET _IO(FDMA_IOC_MAGIC, 5)

// IOCTL: get copy of current fDMA registers
#define IOCTL_FDMA_STATUS_GET _IOWR(FDMA_IOC_MAGIC, 6, char *)

// IOCTL: update the user space copy of TD in TC
#define IOCTL_FDMA_TC_GET _IOWR(FDMA_IOC_MAGIC, 7, char *)

// IOCTL: get number of TD that are currently in use
#define IOCTL_FDMA_TD_USED _IOR(FDMA_IOC_MAGIC, 8, int32_t*)

// IOCTL: get the address of the TDT in SRAM
#define IOCTL_FDMA_TDT_GET _IOR(FDMA_IOC_MAGIC, 9, uint32_t*)

// IOCTL: get last event for specified TD
#define IOCTL_FDMA_EVENT_GET _IOWR(FDMA_IOC_MAGIC, 10, uint32_t*)

#endif /* FDMAIOCTL_H */
