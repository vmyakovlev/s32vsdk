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
* \file     fdma_ioctl.h
* \brief    ioctl related declarations for Sequencer driver
* \author   Tomas Babinec
* \version  0.1
* \date     30.5.2014
* \note
****************************************************************************/

#ifndef SEQIOCTL_H
#define SEQIOCTL_H

#ifdef __cplusplus 
extern "C" { 
#endif

#include <linux/fs.h>		// required for the struct file declaration

///////////////////////////////////////////
//
// IOCTL command codes numbers allocation
//
  
// IOCTL: graph download
#define IOCTL_SEQ_GRAPH_DOWNLOAD _IOW(SEQ_IOC_MAGIC, 6, SEQ_GraphPackage_t*)
  
// IOCTL: DDR buffer lists setup
#define IOCTL_SEQ_DDRBUF_SET _IOW(SEQ_IOC_MAGIC, 7, void*)

// IOCTL: graph start
#define IOCTL_SEQ_START _IOW(SEQ_IOC_MAGIC, 8, SEQ_StartCfg_t*)
  
// IOCTL: graph start
#define IOCTL_SEQ_AUX_DATA_GET _IOR(SEQ_IOC_MAGIC, 9, void*)
  
// IOCTL: graph stop
#define IOCTL_SEQ_STOP _IO(SEQ_IOC_MAGIC, 10)
  
// IOCTL: graph stop wait
#define IOCTL_SEQ_STOP_WAIT _IO(SEQ_IOC_MAGIC, 11)
  
// IOCTL: SEQ HW & driver reset
#define IOCTL_SEQ_RESET _IO(SEQ_IOC_MAGIC, 12)
  
// IOCTL: SEQ boot
#define IOCTL_SEQ_BOOT _IO(SEQ_IOC_MAGIC, 13)

// IOCTL: Irq serve
#define IOCTL_SEQ_IRQ_SRV _IO(SEQ_IOC_MAGIC, 14)
  
// IOCTL: send message
#define IOCTL_SEQ_MSG_SEND _IOW(SEQ_IOC_MAGIC, 15, void*) 

// IOCTL: send message wait
#define IOCTL_SEQ_MSG_SEND_WAIT _IOWR(SEQ_IOC_MAGIC, 16, void*) 
  
// IOCTL: request buffers
#define IOCTL_SEQ_BUF_REQ _IOW(SEQ_IOC_MAGIC, 17, void*) 
  
// IOCTL: queries buffer
#define IOCTL_SEQ_BUF_QRY _IOWR(SEQ_IOC_MAGIC, 18, void*) 

// IOCTL: queries buffer
#define IOCTL_SEQ_TIME_STAT _IOR(SEQ_IOC_MAGIC, 19, void*) 
  
// IOCTL: removes buffers in all streams
#define IOCTL_SEQ_BUF_PSH _IOW(SEQ_IOC_MAGIC, 20, void*) 
  
// IOCTL: removes buffers in all streams
#define IOCTL_SEQ_BUF_POP _IOWR(SEQ_IOC_MAGIC, 21, void*) 
  
// IOCTL: requests performance data for IPUs
#define IOCTL_SEQ_IPU_PROFILE_REQ _IOR(SEQ_IOC_MAGIC, 22, uint32_t)
  
// IOCTL: gets performance data for IPUs
#define IOCTL_SEQ_IPU_PROFILE_GET _IOR(SEQ_IOC_MAGIC, 23, void*)

// IOCTL: gets Event log data
#define IOCTL_SEQ_IPU_EVENT_LOG _IOR(SEQ_IOC_MAGIC, 24, void*)  
  
// IOCTL: enables Event logging
#define IOCTL_SEQ_IPU_EVENT_LOG_ENABLE _IO(SEQ_IOC_MAGIC, 25) 

// IOCTL: disables Event logging
#define IOCTL_SEQ_IPU_EVENT_LOG_DISABLE _IO(SEQ_IOC_MAGIC, 26) 
  
// IOCTL: gets graph structure from PRAM
#define IOCTL_SEQ_GRAPH_FETCH _IOR(SEQ_IOC_MAGIC, 27, void*) 
  
// IOCTL: enables histogram readout for IPU engine
#define IOCTL_SEQ_HIST_ENABLE _IOW(SEQ_IOC_MAGIC, 28, SEQ_IpuEngine_t) 
  
// IOCTL: disables histogram readout for IPU engine
#define IOCTL_SEQ_HIST_DISABLE _IOW(SEQ_IOC_MAGIC, 29, SEQ_IpuEngine_t) 
  
// IOCTL: gets latest histogram data for particular IPU engine
#define IOCTL_SEQ_HIST_GET _IOWR(SEQ_IOC_MAGIC, 30, SEQ_IpuHist_t*) 

// IOCTL: enables statistics readout 
#define IOCTL_SEQ_STAT_ENABLE _IO(SEQ_IOC_MAGIC, 28) 
  
// IOCTL: disables statistics readout 
#define IOCTL_SEQ_STAT_DISABLE _IO(SEQ_IOC_MAGIC, 29)
  
// IOCTL: gets latest statistic data for particular IPU engine
#define IOCTL_SEQ_STAT_GET _IOWR(SEQ_IOC_MAGIC, 31, SEQ_IpuStat_t*) 

// IOCTL: configures frame based graph patching
#define IOCTL_SEQ_PATCH _IOW(SEQ_IOC_MAGIC, 32, void*)  

// IOCTL: sets up register list in PRAM
#define IOCTL_SEQ_REGLIST_SET _IOWR(SEQ_IOC_MAGIC, 33, void*)  

// IOCTL: reads back register list from PRAM
#define IOCTL_SEQ_REGLIST_GET _IOW(SEQ_IOC_MAGIC, 34, void*)     

///////////////////////////////////////////
//
// IOCTL cmds processing code
//

// only for kernel space
#if defined(__KERNEL__) || defined(ISP_SIM)

long SeqIoctl(struct file *apFile,	/* ditto */
		 unsigned int aIoctlNum,	/* number and param for ioctl */
		 unsigned long aIoctlParam);

#endif // if defined(__KERNEL__) || defined(ISP_SIM)

#ifdef __cplusplus 
} 
#endif 

#endif /* SEQIOCTL_H */