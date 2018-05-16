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
 * \file     cam_core.c
 * \brief    init/exit functionality of camera driver
 * \author   Tomas Babinec
 * \version  0.1
 * \date     13.6.2016
 * \note
 ****************************************************************************/

#include <linux/kernel.h>	// We're doing kernel work
#include <linux/module.h>	// Specifically, a module 
#include <linux/moduleparam.h>  // for cmd line arguments
#include <linux/device.h> // for class_create
#include <linux/slab.h>		// for kmalloc
#include <linux/interrupt.h>  // For interrupts

#include <linux/of_platform.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>		// Char device structure 
#include <linux/mutex.h>	// for semaphore/mutex
#include <linux/string.h>

#include "config.h"
#include "cam.h"
#include "cam_ioctl.h"
#include "cam_func.h"
#include "vdb_log.h"

//*****************************************************************************
// constants
//*****************************************************************************

//*****************************************************************************
// Local function declarations
//*****************************************************************************

static int CamOpen(struct inode *const apInode, struct file *const apFile);
static int CamRelease(struct inode *const apInode, struct file *const apFile);
static int __init CamInit(void);
static void __exit CamExit(void);

//*****************************************************************************
// Global variables
//*****************************************************************************

//*****************************************************************************
// Static variables
//*****************************************************************************

// internal device open counter
static int sDeviceOpenCnt = 0;

// char device structure for fDMA module
static struct cdev sCamCdev;

// device number
static unsigned int SCamDevno;
// major device number
//static int sMajor;

// Cam driver class
static struct class *sCamDriverClass = NULL;

// file operations
static struct file_operations sFops = {
  .unlocked_ioctl = CamIoctl,
  .open = CamOpen,
  .release = CamRelease,  /* a.k.a. close */
}; // struct file_operations

//****************************************************************************
// 
// Essential kernel module behaviour definition 
//
static int CamOpen(struct inode *const apInode, struct file *const apFile)
{
  VDB_LOG_NOTE("Cam generic driver open call\n");

  sDeviceOpenCnt++;
  
  try_module_get(THIS_MODULE);
  return CAM_DRV_SUCCESS;
} // CamOpen()

//****************************************************************************

//
// called whenever a process attempts to close the device file 
//
static int CamRelease(struct inode *const apInode, struct file *const apFile)
{
  sDeviceOpenCnt--;

  module_put(THIS_MODULE);

  return CAM_DRV_SUCCESS;
} // CamRelease()

//****************************************************************************

// 
// Initialize the module - Register the character device 
//
static int __init CamInit(void)
{
  int32_t lRetVal = 0;
  
  // enable logging
  lRetVal = vdb_LogInit();

  // *** Setup the driver ***  
  if(CAM_DRV_Setup() != CAM_DRV_SUCCESS)
  {
    lRetVal = printk("CGD: setup failed.\n");
    lRetVal = CAM_DRV_FAILURE;
  } // if driver setup failed
  else
  {
    // *** initialize device file ***
  
    // get device number
    if (alloc_chrdev_region(&SCamDevno, 0, 1, CAM_DEVICE_NAME) < 0)
    {
      lRetVal = printk("CGD: Device number allocation failed.\n");
      lRetVal = CAM_DRV_Close();
      lRetVal = CAM_DRV_FAILURE;
    } // if alloc char dev number failed
    else
    {
      sCamDriverClass = class_create(THIS_MODULE, "cam_generic_drv");
      if(NULL == sCamDriverClass)
      {
        lRetVal = printk("CGD: Driver class creation failed.\n");
        unregister_chrdev_region(SCamDevno, 1);	  
        lRetVal = CAM_DRV_Close();
        lRetVal = CAM_DRV_FAILURE;
      } // if cam driver class creation failed
      else
      {
        if (device_create(sCamDriverClass, 
                    NULL, 
                    SCamDevno, 
                    NULL, 
                    CAM_DEVICE_NAME) == NULL)
        {
          lRetVal = printk("CGD: Driver device creation failed.\n");
          class_destroy(sCamDriverClass);
          unregister_chrdev_region(SCamDevno, 1);	  
          lRetVal = CAM_DRV_Close();
          lRetVal = CAM_DRV_FAILURE;
        } // if device not created
        else
        {
          cdev_init(&sCamCdev, &sFops);
          sCamCdev.owner = THIS_MODULE;
          sCamCdev.ops = &sFops;

          if(0 != cdev_add(&sCamCdev, SCamDevno, 1))
          {
            lRetVal = printk("CGD: Device registration failed.\n");
            device_destroy(sCamDriverClass, SCamDevno);
            class_destroy(sCamDriverClass);
            unregister_chrdev_region(SCamDevno, 1);	  
            lRetVal = CAM_DRV_Close();
            lRetVal = CAM_DRV_FAILURE;
          } // if device registration failed
          else
          {
            // print success message
            lRetVal = printk("CGD: ready.\n");
            lRetVal = CAM_DRV_SUCCESS;
          }
        }
      }
    }
  }  
  
  // init error processing
  if( lRetVal != CAM_DRV_SUCCESS )
  {
      lRetVal = printk("CGD: Error during insmod. Terminating ...\n");
      lRetVal = CAM_DRV_FAILURE;
      vdb_LogStop();
  } // if some failure
  return lRetVal;
} // CamInit()

//****************************************************************************

// 
// Cleanup - unregister the appropriate file from /proc 
//
static void __exit CamExit(void)
{
  int32_t lRetVal = 0;
  // 
  // Unregister the device 
  //
  VDB_LOG_NOTE("Removing cam character device driver module.\n");
  cdev_del(&sCamCdev);
  device_destroy(sCamDriverClass, SCamDevno);
  class_destroy(sCamDriverClass);
  unregister_chrdev_region(SCamDevno, 1);

  // relase resources
  lRetVal = CAM_DRV_Close();
  
  vdb_LogStop();
  
  lRetVal = printk("CGD: exited.\n");
} // CamExit()

//****************************************************************************

module_init(CamInit);
module_exit(CamExit);

//****************************************************************************

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tomas Babinec <tomas.babinec <at> nxp.com>");
MODULE_DESCRIPTION("Camera generic NXP driver.");
MODULE_ALIAS("cam-generic");
