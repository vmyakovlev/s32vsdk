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

#ifdef __cplusplus
extern "C"{
#endif

/*==============================================================================
*                                        INCLUDE FILES
==============================================================================*/

/**
 * @file     h264enc_core.c
 * @brief    init/exit functionality of H264 encoder driver
*/
  
#include <linux/kernel.h> // We're doing kernel work
#include <linux/module.h> // Specifically, a module 
#include <linux/device.h> // for class_create
#include <linux/moduleparam.h> // for parameter use
#include <linux/init.h>
#include <linux/slab.h>   // for kmalloc
#include <linux/sched.h>  // for current->pid
#include <linux/clk.h>
#include <linux/pm_runtime.h>

#include <linux/of_platform.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/cdev.h>   // Char device structure 
#include <asm/uaccess.h>  // for get_user and put_user 
#include <linux/mutex.h>  // for semaphore/mutex

#include "h264enc.h"
#include "h264enc_ioctl.h"
#include "h264enc_func.h"
#include "vdb_log.h"

/*==============================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/

static struct platform_device *sH264EncoderPdev;

/*==============================================================================
*                             LOCAL MACROS
==============================================================================*/

#define CTRL_BLCK_REG_IDX 0 /* Control block register index in DTB node */

#define H264_MODULE_CLK_NAME "fsl_h264enc"  ///< main module clock struct name
#define H264_PER_CLK_NAME    "per"   ///< peripheral interface clock struct name
#define H264_BCLOCK_CLK_NAME "bclock"   ///< SRAM interface clock struct name

/*==============================================================================
*                                      LOCAL CONSTANTS
==============================================================================*/


/*==============================================================================
*                             LOCAL VARIABLES
==============================================================================*/

/** 
* @brief          internal device open counter   
*/
static int gDeviceOpenCnt = 0;  

/** 
* @brief          char device structure for H264 Encoder module 
*/
static struct cdev gH264EncCdev;

/** 
* @brief          H264 Encoder device number
*/
static int gH264EncDevno;

/** 
* @brief          major device number
*/
static int gMajor;

/** 
* @brief          H264 Encoder driver class
*/
static struct class *gH264EncDriverClass = NULL;

/** 
* @brief          H264 The device-driver device struct pointer
*/

static struct device* gH264Device = NULL;  

/** 
* @brief          PID of the user
*/
static pid_t gH264EncUserPid;

/*==============================================================================
*                                      GLOBAL CONSTANTS
==============================================================================*/


/*==============================================================================
*                                      GLOBAL VARIABLES
==============================================================================*/

/** 
* @brief          H264 Encoder Driver Info
*/

H264Enc_DrvInfo_t gH264EncDrvInfo =
{
  .mpH264encCBregs = NULL, ///< control block registers
  .mIrqNums =         ///< irqnumbers
  {
    0, 0, 0, 0, 0, 0, 0, 0
  }
};

void __iomem    *h264enc_reg_base;

H264ENC_REGS *reg_base;

/*=============================================================================
*                                   LOCAL FUNCTION PROTOTYPES
=============================================================================*/

static int H264EncOpen(struct inode *apInode, struct file *apFile);
static int H264EncDrvProbe(struct platform_device *apDev);
static int H264EncRemove(struct platform_device *apDev);
static int H264EncRelease(struct inode *apInode, struct file *apFile);
static int H264EncClockSetup(struct platform_device *apDev, 
                             H264Enc_DrvInfo_t apDrvInfo);
static int __init H264EncInit(void);
static void __exit H264EncExit(void);

/*==============================================================================
*                                LOCAL FUNCTIONS
==============================================================================*/

/**
* @brief         Setup H264 Encoder
* @details       Setup H264 Encoder, calls function for data initializing,
*                requestst irqs
*
* @return        int32_t
* @api
* @isr
*
*
* @pre
* @post
*
* @note
*/
int32_t H264ENC_Setup(void)
{
  uint32_t lRet = H264ENC_DRV_SUCCESS;

  if(H264EncDrvDataInit() != H264ENC_DRV_SUCCESS) /* initialize internal data */
  {
    lRet = H264ENC_DRV_FAILURE;
    goto data_init_failed;
  } /* if driver data init failed     */
  
  if(H264EncIrqsRequest() != H264ENC_DRV_SUCCESS) /* register IRQs */
  {
    lRet = H264ENC_DRV_FAILURE;
    goto irq_request_failed;
  }
 
  if(lRet != H264ENC_DRV_SUCCESS)  /* Error handling */
  {
    VDB_LOG_NOTE("In error handling.\n");
      H264EncIrqsFree();
    irq_request_failed:
      H264EncUnmap();
    data_init_failed:
      ;
    lRet = H264ENC_DRV_FAILURE;
    VDB_LOG_ERROR("Canceling H264Enc driver insmod.\n");
  } /* if error detected;  */

  VDB_LOG_NOTE("Leaving Setup\n");

  return lRet;
} /* H264ENC_Setup() */


/** 
* @brief          Essential kernel module behaviour definition  
* @details        
*
* @param[in]      pdev      struct platform device
* @return         int
* @api
* @isr
*
* @pre
*
* @post
* 
* @implements H264EncOpen_Activity
*
* @note
*/

static int H264EncOpen(struct inode *apInode, struct file *apFile)
{
  VDB_LOG_NOTE("device_open(%p)\n", apFile);

  if (gDeviceOpenCnt > 0)   /* only one open allowed */
  {
    VDB_LOG_ERROR("Openned already.\n");
    return -EBUSY;
  } // if device opened already

  gH264EncUserPid = current->pid; /* remember the user PID */
      
  gDeviceOpenCnt++;
  
  try_module_get(THIS_MODULE);
  return H264ENC_DRV_SUCCESS;
} // H264EncOpen()


/**
* @brief          
* @details        called whenever a process attempts to close the device file 
*
* @param[in]      pdev      struct platform device
* @return         int
* @api
* @isr
*
* @pre
*
* @post
* 
* @implements H264EncRelease_Activity
*
* @note
*/
static int H264EncRelease(struct inode *apInode, struct file *apFile)
{
  gH264EncUserPid = 0;
        
  gDeviceOpenCnt--;

  module_put(THIS_MODULE);

  return H264ENC_DRV_SUCCESS;
} // H264EncRelease()

/**
* @brief          
* @details        clock_gating 
*
* @param[in]      apDev      struct platform device
*                 apDrvInfo
* @return         int
* @api
* @isr
*
* @pre
*
* @post
* 
*
* @note
*/

static int H264EncClockSetup(struct platform_device *apDev, 
                         H264Enc_DrvInfo_t   apDrvInfo)
{
  struct clk *lpH264EncClk = NULL;
  int lRet = 0;  
  
  lpH264EncClk = devm_clk_get(&apDev->dev, H264_MODULE_CLK_NAME);
  
  if (IS_ERR(lpH264EncClk)) 
  {
    lRet = PTR_ERR(lpH264EncClk);
    dev_err(&apDev->dev, "failed to get main H264Encoder module clk: %d\n", 
            lRet);
    goto module_clk;
  } // if failed to get clock

  lRet = clk_prepare_enable(lpH264EncClk);
  
  if (lRet)
  {
    dev_err(&apDev->dev, "failed to enable main H264Encoder module clk: %d\n", 
            lRet);
    goto module_clk;
  } // if failed to enable clock
  
  apDrvInfo.mpModuleClk = lpH264EncClk;  
  lpH264EncClk = devm_clk_get(&apDev->dev, H264_PER_CLK_NAME);
  
  if (IS_ERR(lpH264EncClk)) 
  {
    lRet = PTR_ERR(lpH264EncClk);
    dev_err(&apDev->dev, "failed to get H264Encoder PER clk: %d\n", lRet);
    goto per_clk;
  } // if failed to get clock

  lRet = clk_prepare_enable(lpH264EncClk);
  
  if (lRet)
  {
    dev_err(&apDev->dev, "failed to enable H264Encoder PER clk: %d\n", lRet);
    goto per_clk;
  } // if failed to enable clock
  
  apDrvInfo.mpPerClk = lpH264EncClk;  
  lpH264EncClk = devm_clk_get(&apDev->dev, H264_BCLOCK_CLK_NAME);
  
  if (IS_ERR(lpH264EncClk)) 
  {
    lRet = PTR_ERR(lpH264EncClk);
    dev_err(&apDev->dev, "failed to get H264Encoder BCLOCK clk: %d\n", lRet);
    goto bclock_clk;
  } // if failed to get clock

  lRet = clk_prepare_enable(lpH264EncClk);
  
  if (lRet)
  {
    dev_err(&apDev->dev, "failed to enable H264Encoder BLOCK clk: %d\n", lRet);
    goto bclock_clk;
  } // if failed to enable clock
  
  apDrvInfo.mpBclockClk = lpH264EncClk;
  
  if(lRet)
  {
    bclock_clk:
      clk_disable_unprepare(apDrvInfo.mpPerClk);
      apDrvInfo.mpBclockClk = NULL;
    per_clk:  
      clk_disable_unprepare(apDrvInfo.mpModuleClk);
      apDrvInfo.mpModuleClk = NULL;
    module_clk:
    
    lRet = -EFAULT;
  }
  
  return lRet;
} // H264enc ClockSetup()


/*============================================================================*/
// STRUCT operations
/*============================================================================*/
 

/**
* @brief          Struct platform_driver
* @details        Module Declarations.  This structure will hold the functions 
*                 to be called when a process does something to the device we 
*                 created. Since a pointer to this structure is kept in the 
*                 devices table, it can't be local to init_module. NULL is for
*                 unimplemented functions.
* @pre
* @post
*
*/

static struct file_operations fops =
{
  .owner          = THIS_MODULE,
  .unlocked_ioctl = H264EncIoctl,
  .open = H264EncOpen,
  .release = H264EncRelease,	/* a.k.a. close */
}; /* struct file_operations */

//****************************************************************************

/**
* @brief          Module device table
* @details        Module device table
* @pre
* @post
*
*/
static struct of_device_id H264enc_of_match[] = 
{
  { 
    .compatible = "fsl,s32v234-h264_encoder",
  },
  {}
};

MODULE_DEVICE_TABLE(of, H264enc_of_match);

//****************************************************************************

static int H264Enc_runtime_suspend(struct device *apDev)
{
  H264Enc_DrvInfo_t *lpDrvInfo = dev_get_drvdata(apDev);

  clk_disable_unprepare(lpDrvInfo->mpModuleClk);
  clk_disable_unprepare(lpDrvInfo->mpPerClk);
  clk_disable_unprepare(lpDrvInfo->mpBclockClk);  
  
  return 0;
} // H264Enc_runtime_suspend()

//****************************************************************************

static int H264Enc_runtime_resume(struct device *apDev)
{
  int lRet = 0;
  H264Enc_DrvInfo_t *lpDrvInfo = dev_get_drvdata(apDev);

  lRet = clk_prepare_enable(lpDrvInfo->mpModuleClk);
  
  if(lRet)
  {
    printk("failed to enable H264 Encoder module clock.\n");
    return lRet;
  }
  
  lRet = clk_prepare_enable(lpDrvInfo->mpPerClk);
  
  if(lRet)
  {
    clk_disable_unprepare(lpDrvInfo->mpModuleClk);
    printk("failed to enable H264 Per clock.\n");
    return lRet;
  }
  
  lRet = clk_prepare_enable(lpDrvInfo->mpBclockClk);
  
  if(lRet)
  {
    clk_disable_unprepare(lpDrvInfo->mpBclockClk);
    clk_disable_unprepare(lpDrvInfo->mpModuleClk);
    printk("failed to enable H264 Bclock clock.\n");
    return lRet;
  } 
  
  return lRet;
} // H264Enc_runtime_resume()

//*****************************************************************************

static struct dev_pm_ops H264Enc_pm_ops = 
{
    SET_RUNTIME_PM_OPS( H264Enc_runtime_suspend,
                        H264Enc_runtime_resume, 
                        NULL)
};

//*****************************************************************************

/**
* @brief          Struct platform_driver
* @details        Struct platform_driver
* @pre
* @post
*
*/

static struct platform_driver H264encoder_platform_driver = 
{
  .probe = H264EncDrvProbe,
  .remove = H264EncRemove,
  .driver = 
  {
    .name = "H264_encoder",
    .owner = THIS_MODULE,
    .of_match_table = H264enc_of_match,
    .pm = &H264Enc_pm_ops,
  },
};


/*============================================================================*/
/**
* @brief          The function H264 Encoder probe
* @details        Probe the driver
*
* @param[in]      apDev      struct platform device
* @return         int
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/

static int H264EncDrvProbe(struct platform_device *apDev)
{
  struct resource *lpRes;
  int ret;

  sH264EncoderPdev = apDev;
  ret = 0;
  
  lpRes = platform_get_resource(apDev, IORESOURCE_MEM, CTRL_BLCK_REG_IDX);
  
  if (!lpRes)  /* map control block register space */
  {
    dev_err(&apDev->dev, "could not get control block registers IO resource\n");
    return -ENODEV;
  }

  h264enc_reg_base = devm_ioremap_resource(&apDev->dev, lpRes);
  
  if (IS_ERR(h264enc_reg_base)) 
  {
    dev_err(&apDev->dev, "could not ioremap control block registers\n");
    return PTR_ERR(h264enc_reg_base);
  }
  
    reg_base = (H264ENC_REGS *)h264enc_reg_base;
  
  gH264EncDrvInfo.mpH264encCBregs = (H264ENC_Regs_t*) h264enc_reg_base;
  
  gH264EncDrvInfo.mIrqNums.mH264ENC_VI_FRAME_DONE_INT = 
    irq_of_parse_and_map(apDev->dev.of_node, 0); 
  gH264EncDrvInfo.mIrqNums.mH264ENC_NEW_ROW_INT = 
    irq_of_parse_and_map(apDev->dev.of_node, 1); 
  gH264EncDrvInfo.mIrqNums.mH264ENC_CB_ALARM_INT = 
    irq_of_parse_and_map(apDev->dev.of_node, 2); 
  gH264EncDrvInfo.mIrqNums.mH264ENC_VI_LINE_INT = 
    irq_of_parse_and_map(apDev->dev.of_node, 3); 
  gH264EncDrvInfo.mIrqNums.mH264ENC_VI_LCOUNT_ALARM_INT = 
    irq_of_parse_and_map(apDev->dev.of_node, 4); 
  gH264EncDrvInfo.mIrqNums.mH264ENC_VI_OOS_INT = 
    irq_of_parse_and_map(apDev->dev.of_node, 5); 
  gH264EncDrvInfo.mIrqNums.mH264ENC_MBRW_DONE_INT = 
    irq_of_parse_and_map(apDev->dev.of_node, 6); 
  gH264EncDrvInfo.mIrqNums.mH264ENC_MBRW_FF_OVF_INT = 
    irq_of_parse_and_map(apDev->dev.of_node, 7); 
  
#if 0
  /* enable clocks for DCU */
  dcu_clk = devm_clk_get(&pdev->dev, "dcu");
  if (IS_ERR(dcu_clk)) {
    dcu_init_status = DCU_INIT_ERR_CFG;
    ret = PTR_ERR(dcu_clk);
    dev_err(&pdev->dev, "could not get clock\n");
    goto failed_getclock;
  }
  clk_prepare_enable(dcu_clk);

  /* get DCU clock in Hz*/
  dcu_clk_val = clk_get_rate(dcu_clk);
  dcu_fb_data->clk = dcu_clk;
  pm_runtime_enable(&pdev->dev);
  pm_runtime_get_sync(&pdev->dev);

#endif 

 dev_set_drvdata(&apDev->dev, &gH264EncDrvInfo);

 if(H264EncClockSetup(apDev,gH264EncDrvInfo))
  {
    dev_err(&apDev->dev, "clock setup failed.\n");
    return PTR_ERR(h264enc_reg_base);    
  }
  
  pm_runtime_enable(&apDev->dev);
  ret = pm_runtime_get_sync(&apDev->dev);


  return ret;
} // H264encDrvProbe()

/*============================================================================*/
/**
* @brief          The function H264Encoder remove
* @details        The function H264Encoder remove
*
* @param[in]      apdev      struct platform device
* @return         int
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/
static int H264EncRemove(struct platform_device *apDev)
{
  H264Enc_DrvInfo_t *lpDrvInfo = dev_get_drvdata(&apDev->dev);
    
  clk_disable_unprepare(lpDrvInfo->mpModuleClk); /* Disable clocks */
  clk_disable_unprepare(lpDrvInfo->mpPerClk);
  clk_disable_unprepare(lpDrvInfo->mpBclockClk);
  
  pm_runtime_put_sync(&apDev->dev);
  pm_runtime_disable(&apDev->dev);
  
  printk("H264enc_drv_remove\n");
  return 0;
} /* H264encDrvRemove() */

/*============================================================================*/
/**
* @brief          Initialize the module - Register the character device 
* @details        Initialize the module - Register the character device 
*    
* @return         int
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/
static int __init H264EncInit(void)
{
  int32_t ret_val = 0;

  vdb_LogInit();   /* enable logging */

  platform_driver_register(&H264encoder_platform_driver);

  if(sH264EncoderPdev == NULL)   /* Setup the driver */
  {
    ret_val = H264ENC_DRV_FAILURE;
    VDB_LOG_ERROR("H264 encoder setup failed.\n");
    goto h264enc_setup_failed;
  }           
      
  if(H264ENC_Setup() != H264ENC_DRV_SUCCESS)
  {
    ret_val = H264ENC_DRV_FAILURE;
    VDB_LOG_ERROR("Device number allocation failed.\n");
    goto h264enc_setup_failed;
  } /* if driver setup failed */
 
  VDB_LOG_NOTE("Setup ended OK.\n");  /* initialize device file */
  /* get device number */

  if (alloc_chrdev_region(&gH264EncDevno, 0, 1, H264ENC_DEVICE_NAME) < 0)
  {
    ret_val = H264ENC_DRV_FAILURE;
    VDB_LOG_ERROR("Device number allocation failed.\n");
    goto device_number_allocation_failed;
  } /* if alloc char dev number failed */

  VDB_LOG_NOTE("chrdev region allocated OK.\n");

  gH264EncDriverClass = class_create(THIS_MODULE, "h264EncDrv");
  
  if(IS_ERR(gH264EncDriverClass))
  {
    ret_val = H264ENC_DRV_FAILURE;
    printk(KERN_ALERT "Driver class creation failed.\n");
    goto driver_class_cration_failed;
  }	
  VDB_LOG_NOTE("driver class created OK.\n");  
 
  gH264Device = (device_create(gH264EncDriverClass, 
                               NULL,
                               gH264EncDevno,
                               NULL,
                               H264ENC_DEVICE_NAME));
  
  if(IS_ERR(gH264Device))
  {
    VDB_LOG_ERROR("Driver device creation failed.\n");
    goto device_creation_failed;
  }  

  cdev_init(&gH264EncCdev, &fops);
  gH264EncCdev.owner = THIS_MODULE;
  gH264EncCdev.ops = &fops;
  VDB_LOG_NOTE("Cdev inited.\n");

  if(cdev_add(&gH264EncCdev, gH264EncDevno, 1))
  {
    ret_val = H264ENC_DRV_FAILURE;
    VDB_LOG_ERROR("Device registration failed.\n");
    goto registration_failed;
  } /* if device registration failed */
  VDB_LOG_NOTE("Cdev Added.\n");

  gMajor = MAJOR(gH264EncDevno);
      
  if(ret_val == H264ENC_DRV_SUCCESS)  
  {
  printk("H264Enc driver ready.\n"); 

  VDB_LOG_NOTE("H264ENC driver module registration succeeded.\n"
          "I was assigned major number %d. To talk to\n"
          "the driver's dev file /dev/%s has been created\n"	
          "Remove the device file (rm) and module (rmmod) when done.\n",
          gMajor, H264ENC_DEVICE_NAME);
  }

  if( ret_val )   /* init error processing */
  {  
      cdev_del(&gH264EncCdev);
    registration_failed:
      device_destroy(gH264EncDriverClass, gH264EncDevno);
    device_creation_failed:
      class_destroy(gH264EncDriverClass);
    driver_class_cration_failed:
      unregister_chrdev_region(gH264EncDevno, 1);  
    device_number_allocation_failed:
    
    h264enc_setup_failed:      
      platform_driver_unregister(&H264encoder_platform_driver);    
      H264ENC_Close();
      VDB_LOG_ERROR("Error during insmod. Terminating ...\n");
      vdb_LogStop();
  } /* if some failure */

  return ret_val;
} /* H264EncInit() */


/*============================================================================*/
/**
* @brief          The function H264 Encoder exit
* @details        The function H264 Encoder exit
*
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/

static void __exit H264EncExit(void)
{
  printk(KERN_INFO "Removing H264 Encoder character device driver module.\n");
  cdev_del(&gH264EncCdev);
  device_destroy(gH264EncDriverClass, gH264EncDevno);
  class_destroy(gH264EncDriverClass);
 
  unregister_chrdev_region(gH264EncDevno, 1); /* Unregister the device */

  H264ENC_Close();   /* relase resources */
  platform_driver_unregister(&H264encoder_platform_driver);

  vdb_LogStop();
} /* H264EncExit() */

module_init(H264EncInit);
module_exit(H264EncExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Kubica <roman.kubica <at> nxp.com>");
MODULE_DESCRIPTION("H264 Encoder NXP driver.");
MODULE_ALIAS(H264ENC_DEVICE_NAME);

/*============================================================================*/
/**
* @brief         Requests H264 Encoder irqs
* @details       Requests H264 Encoder irqs
*
* @return        int32_t
* @api
* @isr
*
*
* @pre
* @post
*
* @note
*/

int32_t H264EncIrqsRequest(void)
{
  int32_t lRet = H264ENC_DRV_SUCCESS;

  H264_ENC_enable_interrupts(); /* enable all IRQs */

  if( request_irq(gH264EncDrvInfo.mIrqNums.mH264ENC_VI_FRAME_DONE_INT,
    &H264ENC_S32v234IrqHandler,
    0, H264ENC_DEVICE_NAME,
    NULL))
  {
    lRet = H264ENC_DRV_FAILURE;
    VDB_LOG_ERROR("Failed to request H264 Encoder IRQ.\n");
  } /* if request for IRQ failed */

  if( request_irq(gH264EncDrvInfo.mIrqNums.mH264ENC_NEW_ROW_INT, 
    &H264ENC_S32v234IrqHandler,
    0,
    H264ENC_DEVICE_NAME,
    NULL))
  {
    lRet = H264ENC_DRV_FAILURE;
    VDB_LOG_ERROR("Failed to request H264 Encoder IRQ.\n");
  } /* if request for IRQ failed */

  if( request_irq(gH264EncDrvInfo.mIrqNums.mH264ENC_CB_ALARM_INT,
    &H264ENC_S32v234IrqHandler,
    0,
    H264ENC_DEVICE_NAME,
    NULL))
  {
    lRet = H264ENC_DRV_FAILURE;
    VDB_LOG_ERROR("Failed to request H264 Encoder IRQ.\n");
  } /* if request for IRQ failed */

  if( request_irq(gH264EncDrvInfo.mIrqNums.mH264ENC_VI_LINE_INT, 
    &H264ENC_S32v234IrqHandler,
    0,
    H264ENC_DEVICE_NAME, 
    NULL))
  {
    lRet = H264ENC_DRV_FAILURE;
    VDB_LOG_ERROR("Failed to request H264 Encoder IRQ.\n");
  } /* if request for IRQ failed */

  if( request_irq(gH264EncDrvInfo.mIrqNums.mH264ENC_VI_LCOUNT_ALARM_INT,
    &H264ENC_S32v234IrqHandler,
    0, 
    H264ENC_DEVICE_NAME,
    NULL))
  {
    lRet = H264ENC_DRV_FAILURE;
    VDB_LOG_ERROR("Failed to request H264 Encoder IRQ.\n");
  } /* if request for IRQ failed */

  if( request_irq(gH264EncDrvInfo.mIrqNums.mH264ENC_VI_OOS_INT, 
    &H264ENC_S32v234IrqHandler, 
    0, 
    H264ENC_DEVICE_NAME, 
    NULL))
  {
    lRet = H264ENC_DRV_FAILURE;
    VDB_LOG_ERROR("Failed to request H264 Encoder IRQ.\n");
  } /* if request for IRQ failed */

  if( request_irq(gH264EncDrvInfo.mIrqNums.mH264ENC_MBRW_DONE_INT,
    &H264ENC_S32v234IrqHandler,
    0, 
    H264ENC_DEVICE_NAME, 
    NULL))
  {
    lRet = H264ENC_DRV_FAILURE;
    VDB_LOG_ERROR("Failed to request H264 Encoder IRQ.\n");
  } /* if request for IRQ failed */

 if( request_irq(gH264EncDrvInfo.mIrqNums.mH264ENC_MBRW_FF_OVF_INT, 
    &H264ENC_FF_OVF_IrqHandler, 
    0,
    H264ENC_DEVICE_NAME, 
    NULL))
  {
    lRet = H264ENC_DRV_FAILURE;
    VDB_LOG_ERROR("Failed to request H264 Encoder IRQ.\n");
  } /* if request for IRQ failed */

  return lRet;
} /* H264EncIrqsRequest() */



#ifdef __cplusplus
}
#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/

/** @} */