/*
 * Copyright (c) 2016 Freescale Semiconductor
 * Copyright 2016-2017 NXP
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
 * \file     seq_core.c
 * \brief    init/exit functionality of sequencer driver
 ****************************************************************************/

#include <linux/kernel.h>	// We're doing kernel work
#include <linux/module.h>	// Specifically, a module 
#include <linux/device.h>	// for class_create
#include <linux/moduleparam.h>  // for parameter use
#include <linux/kmod.h>		// for inter_module_xxx
#include <linux/init.h>
#include <linux/mm.h>     // mmap, VM_RESERVED

#include <linux/of_platform.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/cdev.h>		// Char device structure 
#include <asm/uaccess.h>	// for get_user and put_user 
#include <linux/mutex.h>	// for semaphore/mutex
#include <linux/sched.h>	// for current->pid
  
#include "seq.h"
#include "seq_func.h"
#include "seq_ioctl.h"
#include "vdb_log.h"

//*****************************************************************************
// constants
//*****************************************************************************

#define CLASS_NAME "seqdrv" ///< name of the device class name
#define MSG_RDY_IRQ_IDX 0   ///< Message ready IRQ index in DTB node
#define MSG_ACK_IRQ_IDX 1   ///< Message acknowledge IRQ index in DTB node

#define CTRL_BLCK_REG_IDX 0 ///< Control block register index in DTB node
#define EVNT_BLCK_REG_IDX 1 ///< Event control block register index in DTB node

#define FDMA_MOD_NAME "fdma"

//*****************************************************************************
// Local function declarations
//*****************************************************************************

static int SeqMmap(struct file *filp, struct vm_area_struct *vma);
static int SeqDrvProbe(struct platform_device *apDev);
static int SeqDrvRemove(struct platform_device *apDev);
static int SeqOpen(struct inode *apInode, struct file *apFile);
static int SeqRelease(struct inode *apInode, struct file *apFile);
static void SigSend(int32_t aProcId, int32_t aValue);
static int __init SeqInit(void);
static void __exit SeqExit(void);
static int32_t FdmaInsmoded(void);

/****************************************************************************/
/** Maps Sequencer register space, ISP rams and IPUs.
*  
* \return 0 if all ok, otherwise negative error code
*****************************************************************************/
static int32_t SeqMap(struct platform_device *apDev);

/****************************************************************************/
/** Unmaps Sequencer register space, rams and IPU regs.
*  
*****************************************************************************/
static void SeqUnmap(void);

/****************************************************************************/
/** Maps DbgPram, Pram, Cram, Kram.
*  
* \return 0 if all ok, otherwise negative error code
*****************************************************************************/
static int32_t RamMap(void);

/****************************************************************************/
/** Unmaps ISP rams.
*  
*****************************************************************************/
static void RamUnmap(void);

/****************************************************************************/
/** Maps IPU registers.
*  
* \return 0 if all ok, otherwise negative error code
*****************************************************************************/
static int32_t IpuMap(void);

/****************************************************************************/
/** Unmaps IPU regs.
*  
*****************************************************************************/
static void IpuUnmap(void);

//*****************************************************************************
// Global variables
//*****************************************************************************

//*****************************************************************************
// Local variables
//*****************************************************************************

// char device structure for Sequencer module
static struct cdev sSeqCdev;

// Sequencer device number
static int sSeqDevno;
// major device number
static int sMajor;

// Sequencer driver info
static SEQ_DrvInfo_t sSeqDrvInfo;

// Sequencer driver class
static struct class *gSeqDriverClass = NULL;

// internal device open counter
static int sDeviceOpenCnt = 0;

// platform device structure
static struct platform_device *sSeqPdev;

// IPU HW address array
static volatile void* sppIpuPtrs[IPUS_CNT + IPUV_CNT] = { 
  (volatile void*)&IPUS_0, (volatile void*)&IPUS_1, (volatile void*)&IPUS_2, 
  (volatile void*)&IPUS_3, (volatile void*)&IPUS_4, (volatile void*)&IPUS_5,
  (volatile void*)&IPUS_6, (volatile void*)&IPUS_7, (volatile void*)&IPUV_0, 
  (volatile void*)&IPUV_1, (volatile void*)&IPUV_2, (volatile void*)&IPUV_3
};

//*****************************************************************************

// Module Declarations 
// file operations
static struct file_operations sFops = 
{
  .owner          = THIS_MODULE,
  .unlocked_ioctl = SeqIoctl,
  .mmap           = SeqMmap,
  .open           = SeqOpen,
  .release        = SeqRelease,
}; // struct file_operations

//*****************************************************************************

// module device table
static struct of_device_id sSeqOfMatch[] = 
{
  { 
    .compatible = "fsl,s32v234-vseq",
  },
  {}
};

//*****************************************************************************

MODULE_DEVICE_TABLE(of, sSeqOfMatch);

//*****************************************************************************

// platform driver structure
static struct platform_driver sSeqPlatformDriver = 
{
  .probe  = SeqDrvProbe,
  .remove = SeqDrvRemove,
  .driver = 
  {
    .name           = "sequencer",
    .owner          = THIS_MODULE,
    .of_match_table = sSeqOfMatch,
  },
};

//*****************************************************************************

//
// external kernel modules functionality interface
//

static int SeqMmap(struct file *filp, struct vm_area_struct *vma)
{
  unsigned long lSize = (unsigned long)(vma->vm_end - vma->vm_start);
  unsigned long lPhysAddr = (vma->vm_pgoff << PAGE_SHIFT);
  int lRet = 0;

  // *** check region ***
  switch(lPhysAddr)
  {
    case KRAM_BASE:
    {
      if(lSize > KRAM_SIZE)
      {
        lRet = -EINVAL;
      } // if too big return error
    } // if KRAM
    break;
    case PRAM_BASE:
    {
      if(lSize > PRAM_SIZE)
      {
        lRet = -EINVAL;
      } // if too big return error
    } // if PRAM
    break;
    case CRAM_BASE:
    {
      if(lSize > CRAM_SIZE)
      {
        lRet = -EINVAL;
      } // if too big return error
    } // if CRAM
    break;
    default:
    {
      lRet = -EINVAL;
    } // if other region
    break;
  } // switch regions

  
  if(lRet == 0)
  {
    vma->vm_flags |= VM_IO;
    vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
    
    lRet = remap_pfn_range(vma, vma->vm_start, __phys_to_pfn(lPhysAddr), lSize, vma->vm_page_prot);   
  } // if all ok so far
  
  return lRet; 
} // SeqMmap()

//*****************************************************************************

// 
// called whenever a process attempts to open the device file 
//
static int SeqOpen(struct inode *apInode, struct file *apFile)
{
  VDB_LOG_NOTE("Openning Sequencer device.\n", apFile);

  // if only one open allowed
  if (sDeviceOpenCnt > 0)
  {
    return -EBUSY;
  } // if device opened already

  sDeviceOpenCnt++;

  // remember the user PID
  SEQ_UserPidSet(current->pid);

  try_module_get(THIS_MODULE);
  return SEQ_DRV_SUCCESS;
} // SeqOpen()

//*****************************************************************************

//
// called whenever a process attempts to close the device file 
//
static int SeqRelease(struct inode *apInode, struct file *apFile)
{
  sDeviceOpenCnt--;

  module_put(THIS_MODULE);

  SEQ_UserPidSet(0);

  return SEQ_DRV_SUCCESS;
} // SeqRelease()

//*****************************************************************************
// 
// Probe the driver
//

static int SeqDrvProbe(struct platform_device *apDev)
{
  
  int lRet = 0;

  sSeqPdev = apDev;
  
  // *** Setup the driver ***
  sSeqDrvInfo.mpSeqCBregs      = NULL,
  sSeqDrvInfo.mpSeqECregs      = NULL,
  sSeqDrvInfo.mpCram           = NULL,
  sSeqDrvInfo.mpDbgPram        = NULL, 
  sSeqDrvInfo.mpPram           = NULL,
  sSeqDrvInfo.mpKram           = NULL,
  sSeqDrvInfo.mIrqNums.mMsgRdy = (uint32_t)0;
  sSeqDrvInfo.mIrqNums.mMsgAck = (uint32_t)0;
  sSeqDrvInfo.mpSigInvoke      = &SigSend;
  sSeqDrvInfo.mpCommInfo       = NULL;

  if((lRet = SeqMap(apDev)) != 0)
  {
    return lRet;
  } // if Seqmap failed
  
  sSeqDrvInfo.mIrqNums.mMsgRdy = irq_of_parse_and_map(apDev->dev.of_node, 0); 
  sSeqDrvInfo.mIrqNums.mMsgAck = irq_of_parse_and_map(apDev->dev.of_node, 1); 

  return lRet;
} // SeqDrvProbe()

//*****************************************************************************

static int32_t SeqMap(struct platform_device *apDev)
{
  struct resource *lpRes     = NULL;
  void            *lpSeqRegs = NULL;
  
  // *** map control block register space ***
  lpRes = platform_get_resource(apDev, IORESOURCE_MEM, CTRL_BLCK_REG_IDX);
  if (!lpRes) 
  {
    dev_err(&apDev->dev, "could not get control block registers IO resource\n");
    return -ENODEV;
  } // if failed to get CTRL_BLCK resource
  
  lpSeqRegs = devm_ioremap_resource(&apDev->dev, lpRes);
  if (IS_ERR(lpSeqRegs)) 
  {
    dev_err(&apDev->dev, "could not ioremap control block registers\n");
    return PTR_ERR(lpSeqRegs);
  } // if failed to remap CTRL_BLCK regs
  
  sSeqDrvInfo.mpSeqCBregs = (SEQ_CtrlBlkRegs_t*) lpSeqRegs;
  
  // *** map event-control block register space ***
  lpRes = platform_get_resource(apDev, IORESOURCE_MEM, EVNT_BLCK_REG_IDX);
  if (!lpRes) 
  {
    dev_err(&apDev->dev, "could not get event block registers IO resource\n");
    return -ENODEV;
  } // if failed to get EVNT_BLCK resource
  
  lpSeqRegs = devm_ioremap_resource(&apDev->dev, lpRes);
  if (IS_ERR(lpSeqRegs)) 
  {
    dev_err(&apDev->dev, "could not ioremap event block registers\n");
    return PTR_ERR(lpSeqRegs);
  } // if failed to remap EVNT_BLCK regs
  
  sSeqDrvInfo.mpSeqECregs = (SEQ_EvtCtrlRegs_t*) lpSeqRegs;
  
  // *** map ISP memories ***
  if(RamMap() != 0)
  {
    goto ram_map_failed;
  } // if ram_map_failed
  
  // *** map IPU regs ***  
  if(IpuMap() != SEQ_DRV_SUCCESS)
  {
    goto ipu_map_failed;
  } // if failed to map IPU registers
  
  return 0;
  
ipu_map_failed:
  RamUnmap();
ram_map_failed:
  sSeqDrvInfo.mpSeqCBregs = NULL;
  sSeqDrvInfo.mpSeqECregs = NULL;
  
  return -ENODEV;
} // SeqMap(struct platform_device *apDev)

//*****************************************************************************

static void SeqUnmap()
{  
  IpuUnmap();
  RamUnmap();
    
  sSeqDrvInfo.mpSeqCBregs = NULL;
  sSeqDrvInfo.mpSeqECregs = NULL;
} // SeqUnmap()

//*****************************************************************************

static int32_t RamMap()
{
  int32_t lRet = 0;
  
  // map PRAM
  sSeqDrvInfo.mpDbgPram = (uint8_t*) ioremap(DBG_PRAM_BASE, DBG_PRAM_SIZE);
  if(sSeqDrvInfo.mpDbgPram == NULL)
  {
    printk("SEQ: could not ioremap DbgPRAM\n");
    return -ENODEV;
  } // if failed to map PRAM
  
  sSeqDrvInfo.mpPram    = (uint8_t*) ioremap(PRAM_BASE, PRAM_SIZE);
  if(sSeqDrvInfo.mpPram == NULL)
  {
    printk("SEQ: could not ioremap PRAM\n");
    goto pram_map_fail;
  } // if PRAM not mapped
  
  // map CRAM
  sSeqDrvInfo.mpCram  = (uint8_t*) ioremap(CRAM_BASE, CRAM_SIZE);
  if(sSeqDrvInfo.mpCram == NULL)
  {
    printk("SEQ: could not ioremap CRAM\n");  
    goto cram_map_fail;
  } // if CRAM registers not mapped
  
  // map KRAM
  sSeqDrvInfo.mpKram = (uint8_t*) ioremap(KRAM_BASE, KRAM_SIZE);
  if(sSeqDrvInfo.mpKram  == NULL)
  {   
    printk("SEQ: could not ioremap KRAM\n");  
    goto kram_map_fail;
  } // if KRAM registers not mapped

  return lRet;
  
kram_map_fail:  
  iounmap(sSeqDrvInfo.mpCram);
  sSeqDrvInfo.mpCram    = NULL;
cram_map_fail:  
  iounmap(sSeqDrvInfo.mpPram);
  sSeqDrvInfo.mpPram    = NULL;
pram_map_fail:  
  iounmap(sSeqDrvInfo.mpDbgPram);
  sSeqDrvInfo.mpDbgPram = NULL;    
  lRet = -ENODEV;
  
  return lRet;
} // RamMap()

//*****************************************************************************

static void RamUnmap()
{ 
  if(sSeqDrvInfo.mpCram != NULL)
  {
    iounmap(sSeqDrvInfo.mpCram);
    sSeqDrvInfo.mpCram    = NULL;
  }
  if(sSeqDrvInfo.mpPram)
  {
    iounmap(sSeqDrvInfo.mpPram);
    sSeqDrvInfo.mpPram    = NULL;
  }
  if(sSeqDrvInfo.mpDbgPram)
  {
    iounmap(sSeqDrvInfo.mpDbgPram);
    sSeqDrvInfo.mpDbgPram = NULL;
  }    
} // RamUnmap()

//*****************************************************************************

static int32_t IpuMap()
{
  int32_t lRet  = SEQ_DRV_SUCCESS;
  uint32_t i    = 0;
  
  // map IPUSs registers  
  for(i = 0; i < IPUS_CNT; i ++)
  {   
    sSeqDrvInfo.mppIpusRegs[i] = 
        (SEQ_IpusRegs_t*) ioremap((uintptr_t)sppIpuPtrs[i], 
                                  sizeof(SEQ_IpusRegs_t));
    if(sSeqDrvInfo.mppIpusRegs[i] == NULL)
    {
      VDB_LOG_ERROR("Failed to map IPUs_%u registers.\n", i);
      lRet = SEQ_DRV_FAILURE;
      break;
    } // if Sequencer registers not mapped
  } // for all IPUSs
  
  if(lRet == SEQ_DRV_SUCCESS)
  {
    // map IPUVs registers  
    for(i = 0; i < IPUV_CNT; i ++)
    {   
      sSeqDrvInfo.mppIpuvRegs[i] = 
        (SEQ_IpuvRegs_t*) ioremap((uintptr_t)sppIpuPtrs[i + IPUS_CNT], 
                                  sizeof(SEQ_IpuvRegs_t));
      if(sSeqDrvInfo.mppIpuvRegs[i] == NULL)
      {
        VDB_LOG_ERROR("Failed to map IPUv_%u registers.\n", i);
        lRet = SEQ_DRV_FAILURE;
        break;
      } // if Sequencer registers not mapped
    } // for all IPUVs
  } // if still all ok
  
  if(lRet != SEQ_DRV_SUCCESS)
  {
    IpuUnmap();
  } // if sth failed
  
  return lRet;
} // IpuMap()

//*****************************************************************************

static void IpuUnmap()
{
  uint32_t i = 0;
  
  // unmap IPUSs registers  
  for(i = 0; i < IPUS_CNT; i ++)
  {
    if(sSeqDrvInfo.mppIpusRegs[i] != NULL)
    {
      iounmap(sSeqDrvInfo.mppIpusRegs[i]);
      sSeqDrvInfo.mppIpusRegs[i] = NULL;
    } // if mapped
  } // for all IPUSs
  
  // unmap IPUVs registers  
  for(i = 0; i < IPUV_CNT; i ++)
  {
    if(sSeqDrvInfo.mppIpuvRegs[i] != NULL)
    {
      iounmap(sSeqDrvInfo.mppIpuvRegs[i]);
      sSeqDrvInfo.mppIpuvRegs[i] = NULL;
    } // if mapped
  } // for all IPUVs
} // IpuUnmap()

//*****************************************************************************

static int SeqDrvRemove(struct platform_device *apDev)
{
  SeqUnmap();
  return 0;
} // SeqDrvRemove()

//*****************************************************************************

int32_t FdmaInsmoded()
{
  int32_t lRetVal = 0;
  uint32_t i      = 0;
  struct module *lpMod = NULL;  
  
  // lock module_mutex
  while(mutex_lock_interruptible(&module_mutex) != 0) 
  {
    // retry 10 times
    if(++i >= 10)
    {
      VDB_LOG_ERROR("Failed to lock module_mutex.\n");
      lRetVal = -EINTR;
      goto fdma_on_already;
    } // if i < 10
  } // while mutex locking interrupted

  // find module
  lpMod = find_module(FDMA_MOD_NAME);
  mutex_unlock(&module_mutex);
  if (lpMod != NULL) 
  {
    mutex_unlock(&module_mutex);
    VDB_LOG_ERROR("FDMA module already insmoded. Must not be!\n");
    lRetVal = -1;
  } // if found
  
fdma_on_already:  
  return lRetVal;
} // FdmaExistanceCheck()

//*****************************************************************************

static void SigSend(int32_t aPid, int32_t aValue)
{
  struct siginfo lInfo;
  struct pid *lpPidStruct;
  struct task_struct *lpTask;
    
  // send the signal to user space
  memset(&lInfo, 0, sizeof(struct siginfo));
  lInfo.si_signo = SEQ_EVENT_SIGNAL;
  lInfo.si_code  = SI_QUEUE;
  lInfo.si_int   = (aValue);
  rcu_read_lock();
  lpPidStruct    = find_get_pid(aPid);
  lpTask         = pid_task(lpPidStruct,PIDTYPE_PID);
  if(lpTask == NULL)
  {
    VDB_LOG_ERROR("No such pid %u. Signal not sent.\n", (uint32_t) aPid);
    rcu_read_unlock();
  } // if no such pidRegisterred
  else
  {
    rcu_read_unlock();
    //send the signal
    if (send_sig_info(SEQ_EVENT_SIGNAL, &lInfo, lpTask) < (int32_t)0) {
      VDB_LOG_ERROR("Error sending signal to user space.\n");
    } // if sending signal failed
  } // else if no such pid
} // Sig2UsrSend()

//*****************************************************************************
  
// 
// Initialize the module - Register the character device 
//
static int __init SeqInit()
{
  int32_t lRetVal = 0;
  
  // enable logging
  vdb_LogInit();

  if((lRetVal = FdmaInsmoded()) != 0)
  {
    goto fdma_check_failed;
  } // if fdma check failed

  platform_driver_register(&sSeqPlatformDriver);
  if(sSeqPdev == NULL)
  {
    lRetVal = SEQ_DRV_FAILURE;
    printk("SEQ: platform driver registration failed.\n");
    goto seq_setup_failed;
  }

  // *** Setup the driver ***
  if(SEQ_DRV_Setup(&sSeqDrvInfo) != SEQ_DRV_SUCCESS)
  {
    lRetVal = SEQ_DRV_FAILURE;
    printk("SEQ: setup failed.\n");
    goto seq_setup_failed;
  } // if driver setup failed

  // *** initialize device file ***
  
  // get device number
  if (alloc_chrdev_region(&sSeqDevno, 0, 1, SEQ_DEVICE_NAME) < 0)
  {
    lRetVal = SEQ_DRV_FAILURE;
    printk("SEQ: device number allocation failed.\n");
    goto device_number_allocation_failed;
  } // if alloc char dev number failedl

  if(IS_ERR(gSeqDriverClass = class_create(THIS_MODULE, CLASS_NAME)))
  {
    lRetVal = SEQ_DRV_FAILURE;
    printk("SEQ: driver class creation failed.\n");
    goto driver_class_cration_failed;
  } // if fdma driver class creation failed

  if (IS_ERR(device_create(gSeqDriverClass, NULL, sSeqDevno, NULL, SEQ_DEVICE_NAME)))
  {
    printk("SEQ: driver device creation failed.\n");
    goto device_creation_failed;
  } // if device cration failed

  cdev_init(&sSeqCdev, &sFops);
  sSeqCdev.owner = THIS_MODULE;
  sSeqCdev.ops = &sFops;

  if(cdev_add(&sSeqCdev, sSeqDevno, 1))
  {
    lRetVal = SEQ_DRV_FAILURE;
    printk("SEQ: device registration failed.\n");
    goto registration_failed;
  } // if device registration failed

  sMajor = MAJOR(sSeqDevno);

  // print usage message
  printk("SEQ: driver ready.\n");
  
  // init error processing
  if( lRetVal )
  {
    //fdma_tdt_init_failed:	
      cdev_del(&sSeqCdev);
    registration_failed:
      device_destroy(gSeqDriverClass, sSeqDevno);
    device_creation_failed:
      class_destroy(gSeqDriverClass);
    driver_class_cration_failed:
      unregister_chrdev_region(sSeqDevno, 1);	  
    device_number_allocation_failed:
      SEQ_DRV_Close();
    seq_setup_failed:
      platform_driver_unregister(&sSeqPlatformDriver);
    fdma_check_failed:
      printk("SEQ: Error during seq.ko insmod. Terminating ...\n");
      vdb_LogStop();
  } // if some failure

  return lRetVal;
} // SeqInit()

//*****************************************************************************

// 
// Cleanup - unregister the appropriate file from /proc 
//
static void __exit SeqExit()
{
  // 
  // Unregister the device 
  //
  VDB_LOG_NOTE("Removing Sequencer character device driver module.\n");
  cdev_del(&sSeqCdev);
  device_destroy(gSeqDriverClass, sSeqDevno);
  class_destroy(gSeqDriverClass);
  unregister_chrdev_region(sSeqDevno, 1);
  
  SEQ_DRV_Close();
  
  platform_driver_unregister(&sSeqPlatformDriver);
  
  vdb_LogStop();
  
  printk("SEQ: driver exited.\n");
} // SeqExit()

//*****************************************************************************

#if 1
  module_init(SeqInit);
  module_exit(SeqExit);
#else
  module_platform_driver(sSeqPlatformDriver);  
#endif
  
//*****************************************************************************

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tomas Babinec <tomas.babinec <at> freescale.com>");
MODULE_DESCRIPTION("Sequencer Freescale driver.");
MODULE_ALIAS("seq");
