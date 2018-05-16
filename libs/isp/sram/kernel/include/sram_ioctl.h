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
* \file     sram_ioctl.h
* \brief    ioctl related declarations for SRAM manager driver
* \author   Tomas Babinec
* \version  0.1
* \date     22.4.2014
* \note
****************************************************************************/

#ifndef SRAMIOCTL_H
#define SRAMIOCTL_H

#include <linux/fs.h>		// required for the struct file declaration

///////////////////////////////////////////
//
// IOCTL command codes numbers allocation
//

// IOCTL: SRAM allocation request (single-banked segment)
#define IOCTL_SRAM_S_MALLOC _IOWR(SRAM_IOC_MAGIC, 1, unsigned long *)

// IOCTL: SRAM allocation request (multi-banked segment)
#define IOCTL_SRAM_M_MALLOC _IOWR(SRAM_IOC_MAGIC, 2, unsigned long *)

// IOCTL: SRAM free request
#define IOCTL_SRAM_FREE _IOW(SRAM_IOC_MAGIC, 3, unsigned long *)

// IOCTL: SRAM mutex lock cmd - waits for a mutex in kernel space
#define IOCTL_SRAM_LOCK _IO(SRAM_IOC_MAGIC, 4)

// IOCTL: SRAM mutex release - releases the mutex in kerenel space
#define IOCTL_SRAM_UNLOCK _IO(SRAM_IOC_MAGIC, 5)

///////////////////////////////////////////
//
// IOCTL cmds processing code
//

// only for kernel space
#ifdef __KERNEL__

long SramIoctl(struct file *apFile,	/* ditto */
		 unsigned int aIoctlNum,	/* number and param for ioctl */
		 unsigned long aIoctlParam);

#endif

#endif /* SRAMIOCTL_H */
