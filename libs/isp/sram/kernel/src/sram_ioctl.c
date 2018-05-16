/*
 * Copyright (c) 2014 Freescale Semiconductor
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
 * \file     sram_ioctl.c
 * \brief    IOCTL functionality of SRAM driver
 * \author   Tomas Babinec
 * \version  0.1
 * \date     4.4.2014
 * \note
 ****************************************************************************/

#ifdef ARM
#undef ARM
#endif // ifdef ARM

#include <asm/uaccess.h>	// for get_user and put_user 
#include <linux/mutex.h>	// for semaphore/mutex

#include "sram.h"
#include "sram_func.h"
#include "sram_dl_mspace.h"
#include "vdb_log.h"

//*****************************************************************************
// global variables
//*****************************************************************************

// start addresses of different SRAM segments:
// 0: multi-banked static region
// 1: multi-banked dynamic mspace
// 2: single-banked static region
// 3: single-banked dynamic mspace
extern char *gSramSegments[];

// single-banked mspace for DL allocator
extern char *gSramSingleMspace;
// multi-banked mspace for DL allocator
extern char *gSramMultiMspace;

// SRAM locking mutex
extern mutex_t gSramMutex;


static unsigned long sAllSize = 0;

//*****************************************************************************
// functions
//*****************************************************************************

long SramIoctl(struct file *apFile,		// ditto 
unsigned int aIoctlNum,	// CMD number 
unsigned long aIoctlParam)	// param for ioctl
{
  long ret_val = SRAM_DRV_SUCCESS;
  // switch the ioctl command
  switch (aIoctlNum) {
    
      // Allocate a new contiguous region (single-banked)
    case IOCTL_SRAM_S_MALLOC:
    {
      unsigned long size;
      unsigned long addr;
    
      if (copy_from_user(&size, (unsigned long *)aIoctlParam, sizeof(unsigned long)) != 0)
      {
        VDB_LOG_ERROR("Copy from user failed.\n");
        ret_val = -EIO;
        break;
      } // if copy from user failed
      addr = SRAM_SingleMalloc(size);
      if((addr < SRAM_BASE) || (addr >= (SRAM_BASE + SRAM_SINGLE_SIZE)))
      {
        VDB_LOG_ERROR("Allocation of %lu bytes (single-banked) failed %lu. %lu\n", size, addr, size);
        addr = 0;
        ret_val = SRAM_DRV_FAILURE;
      }
      else
      {
        VDB_LOG_NOTE("Allocated %lu bytes at address: %x (single-banked). %lu\n", size, addr, size);
        sAllSize += size;
      }
    
      if (copy_to_user((unsigned long *)aIoctlParam, &addr, sizeof(unsigned long)) != 0)
      {
        VDB_LOG_ERROR("Copy to user failed.\n");
        ret_val = -EIO;
      } // if copy to user failed
    } // case IOCTL_SRAM_MALLOC
      break;
      
    // Allocate a new contiguous region (multi-banked)
    case IOCTL_SRAM_M_MALLOC:
    {
      unsigned long size;
      unsigned long addr;
    
      if (copy_from_user(&size, (unsigned long *)aIoctlParam, sizeof(unsigned long)) != 0)
      {
        VDB_LOG_ERROR("Copy from user failed.\n");
        ret_val = -EIO;
        break;
      } // if copy from user failed
      addr = SRAM_MultiMalloc(size);
      VDB_LOG_NOTE("Allocated %lu bytes at address: %lu (multi-banked)\n", size, addr);
  
      if (copy_to_user((unsigned long *)aIoctlParam, &addr, sizeof(unsigned long)) != 0)
      {
        VDB_LOG_ERROR("Copy to user failed.\n");
        ret_val = -EIO;
      } // if copy to user failed
    } // case IOCTL_SRAM_MALLOC
      break;
    
    // Free unused contiguous region
    case IOCTL_SRAM_FREE:
    {
      unsigned long addr;
    
      if (copy_from_user(&addr, (unsigned long *)aIoctlParam, sizeof(unsigned long)) != 0)
      {
        VDB_LOG_ERROR("Copy from user failed.\n");
        ret_val = -EIO;
      } // if copy from user failed
    
      SRAM_Free(addr);
    } // case IOCTL_SRAM_FREE
      break;

    
    // Free unused contiguous region
    case IOCTL_SRAM_LOCK:
    {
      if(mutex_lock_interruptible(&gSramMutex) > 0)
      {
        VDB_LOG_WARNING("SRAM mutex down interrupted.\n");
        ret_val = -ERESTARTSYS;
      } // if interrupted
      else
      {
        VDB_LOG_NOTE("SRAM mutex locked.\n");
      }
    } // case IOCTL_SRAM_LOCK
      break;
    
    // Free unused contiguous region
    case IOCTL_SRAM_UNLOCK:
    {
      mutex_unlock(&gSramMutex);
      VDB_LOG_NOTE("SRAM mutex released.\n");
    } // case IOCTL_SRAM_UNLOCK
      break;
    
    // default
    default:
    {
      VDB_LOG_ERROR("Unknown ioctl command.\n");
      ret_val = SRAM_DRV_FAILURE;
    } // default
      break;
  } // switch ioctl command

  return ret_val;
} // SramIoctl()