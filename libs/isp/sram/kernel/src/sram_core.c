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
 * \file     sram_core.c
 * \brief    init/exit functionality of SRAM manager
 * \author   Tomas Babinec
 * \version  0.1
 * \date     22.4.2014
 * \note
 ****************************************************************************/

#include <linux/kernel.h>	// We're doing kernel work
#include <linux/module.h>	// Specifically, a module 
#include <linux/moduleparam.h>  // for parameter use
#include <linux/device.h>	// for class_create
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/cdev.h>		// Char device structure 
#include <asm/uaccess.h>	// for get_user and put_user 
#include <linux/slab.h>		// for kmalloc
#include <linux/string.h>	// for memset
#include <linux/mutex.h>	// for semaphore/mutex

#include "sram.h"
#include "sram_func.h"
#include "sram_ioctl.h"
#include "sram_dl_mspace.h"
#include "vdb_log.h"

// internal device open counter
static int gDeviceOpenCnt = 0;

// start addresses of different SRAM segments:
// 0: multi-banked static region
// 1: multi-banked dynamic mspace
// 2: single-banked static region
// 3: single-banked dynamic mspace
extern char *gSramSegments[];

extern uint32_t gSramSegmentSizes[];

// single- and multi-banked mspaces for DL allocator
extern mspace	gSramSingleMspace;
extern mspace	gSramMultiMspace;

// char device structure for SRAM module
struct cdev gSramCdev;

// SRAM device number
int32_t gSramDevno;
// major device number
int32_t gMajor;
// SRAM driver class
static struct class *gSramDriverClass = NULL;

// 
// called whenever a process attempts to open the device file 
//
static int32_t SramOpen(struct inode *apInode, struct file *apFile)
{
  // if only one open allowed
  if (gDeviceOpenCnt > 3)
  {
    return -EBUSY;
  } // if device opened already

  gDeviceOpenCnt++;
    
  try_module_get(THIS_MODULE);
  return SRAM_DRV_SUCCESS;
} // SramOpen()

//
// called whenever a process attempts to close the device file 
//
static int32_t SramRelease(struct inode *apInode, struct file *apFile)
{
  gDeviceOpenCnt--;

  module_put(THIS_MODULE);

  return SRAM_DRV_SUCCESS;
} // SramRelease()

// Module Declarations 

// 
// This structure will hold the functions to be called
// when a process does something to the device we
// created. Since a pointer to this structure is kept in
// the devices table, it can't be local to
// init_module. NULL is for unimplemented functions. 
//
struct file_operations fops = {
  .unlocked_ioctl = SramIoctl,
  .open = SramOpen,
  .release = SramRelease,	/* a.k.a. close */
}; // struct file_operations


// 
// Initialize the module - Register the character device 
//
static int __init SramInit(void)
{
  int32_t ret_val = 0;
  
  // enable logging
  vdb_LogInit();
  
  //
  // initialize device file
  //

  // get device number
  if (alloc_chrdev_region(&gSramDevno, 0, 1, SRAM_DEVICE_NAME) < 0)
  {
    ret_val = SRAM_DRV_FAILURE;
    VDB_LOG_ERROR("Device number allocation failed.\n");
    goto device_number_allocation_failed;
  } // if alloc char dev number failed

  if((gSramDriverClass = class_create(THIS_MODULE, "sramdrv")) == NULL)
  {
    ret_val = SRAM_DRV_FAILURE;
    printk(KERN_ALERT "Driver class creation failed.\n");
    goto driver_class_cration_failed;
  } // if sram driver class creation failed

  if (device_create(gSramDriverClass, NULL, gSramDevno, NULL, "sram") == NULL)
  {
    VDB_LOG_ERROR("Driver device creation failed.\n");
    goto device_creation_failed;
  }

  cdev_init(&gSramCdev, &fops);
  gSramCdev.owner = THIS_MODULE;
  gSramCdev.ops = &fops;

  if(cdev_add(&gSramCdev, gSramDevno, 1))
  {
    ret_val = SRAM_DRV_FAILURE;
    VDB_LOG_ERROR("Device registration failed.\n");
    goto registration_failed;
  } // if device registration failed
  
  gMajor = MAJOR(gSramDevno);
  
  // TODO: move it to the beginning
  ret_val = SRAM_SetUp();
  
  if(ret_val == SRAM_DRV_SUCCESS)
  {
    printk("SRAM allocator ready.\n");
    // print usage message
    VDB_LOG_NOTE("SRAM driver module registration succeeded.\n"
      "I was assigned major number %d. To talk to\n"
      "the driver a dev file /dev/%s has been created\n"	
      "Remove the device file (rm) and module (rmmod) when done.\n",
      gMajor, SRAM_DEVICE_NAME);
  } // if setup succeeded
  
  // init error processing
  if( ret_val )
  {
    //sram_tdt_init_failed:	
      cdev_del(&gSramCdev);
    registration_failed:
      device_destroy(gSramDriverClass, gSramDevno);
    device_creation_failed:
      class_destroy(gSramDriverClass);
    driver_class_cration_failed:
      unregister_chrdev_region(gSramDevno, 1);	  
    device_number_allocation_failed:
    
      VDB_LOG_ERROR("Error during insmod. Terminating ...\n");
      vdb_LogStop();
  } // if some failure
  
  return ret_val;
} // SramInit()

// 
// Cleanup - unregister the appropriate file from /proc 
//
static void __exit SramExit(void)
{	
  // 
  // Unregister the device 
  //
  printk(KERN_INFO "Removing SRAM character device driver module.\n");
  cdev_del(&gSramCdev);
  device_destroy(gSramDriverClass, gSramDevno);
  class_destroy(gSramDriverClass);
  unregister_chrdev_region(gSramDevno, 1);

  // relase resources
  //SRAM_Close();

  vdb_LogStop();
  gDeviceOpenCnt--;
} // SramExit()

module_init(SramInit);
module_exit(SramExit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tomas Babinec <tomas.babinec <at> freescale.com>");
MODULE_DESCRIPTION("SRAM Freescale manager.");
MODULE_ALIAS("sram");