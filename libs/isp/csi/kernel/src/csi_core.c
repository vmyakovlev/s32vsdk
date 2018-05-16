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

/*==============================================================================
*                                 INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
/**
* @file            csi_core.c
*/

#include <linux/kernel.h>       // We're doing kernel work
#include <linux/module.h>       // Specifically, a module 
#include <linux/moduleparam.h>  // for cmd line arguments
#include <linux/device.h>       // for class_create
#include <linux/slab.h>         // for kmalloc
#include <linux/interrupt.h>    // For interrupts
#include <linux/clk.h>
#include <linux/pm_runtime.h>

#include <linux/of_platform.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>       // Char device structure 
#include <linux/mutex.h>      // for semaphore/mutex
#include <linux/string.h>

#include "config.h"
#include "csi.h"
#include "csi_ioctl.h"
#include "csi_func.h"
#include "vdb_log.h"

/*==============================================================================
*                        LOCAL CONSTANTS
==============================================================================*/

#define CLASS_NAME "csidrv" // name of the device class name
#define DEV_OPEN_CNT_MAX 1  // maximum number of allowed opened descriptors

#define ERR_IRQ_IDX 0   // CSI error related IRQ index in DTB node
#define FRM_IRQ_IDX 1   // CSI frame related IRQ index in DTB node

#define CSI_MODULE_CLK_NAME "csi"  // CSI main module clock name
#define CSI_LI_CLK_NAME     "cli"  // CSI PHY interface clock name
#define CSI_UI_CLK_NAME     "cui"  // CSI PHY interface clock name
#define CSI_DPHY_CLK_NAME   "dphy" // CSI PHY low power clock name

/*==============================================================================
*                        LOCAL FUNCTION PROTOTYPES
==============================================================================*/

static int CsiDrvProbe(struct platform_device *apDev);
static int CsiDrvRemove(struct platform_device *apDev);
static int CsiOpen(struct inode *apInode, struct file *apFile);
static int CsiRelease(struct inode *apInode, struct file *apFile);
static int CsiClockSetup(struct platform_device *apDev, 
                         CSI_DrvInfo_t   *apDrvInfo);
static int __init CsiInit(void);
static void __exit CsiExit(void);

static int CsiRuntimeSuspend(struct device *apDev);
static int CsiRuntimeResume(struct device *apDev);

/*==============================================================================
*                       GLOBAL VARIABLES
==============================================================================*/

/*==============================================================================
*                       LOCAL VARIABLES
==============================================================================*/

// internal device open counter
static int sDeviceOpenCnt = 0;

// char device structure for fDMA module
static struct cdev sCsiCdev;

// CSI device number
static int sCsiDevno;
// major device number
static int sMajor;

// Csi driver class
static struct class *sCsiDriverClass = NULL;

// Csi platform device array
static struct platform_device *sppCsiDev[CSI_IDX_CNT] = {NULL};

// CSI driver info array for available receivers
static CSI_DrvInfo_t *sppCsiDrvInfo[CSI_IDX_CNT] = {NULL};

// file operations
static struct file_operations sFops = {
  .unlocked_ioctl = CsiIoctl,
  .open           = CsiOpen,
  .release        = CsiRelease,  // a.k.a. close
}; // struct file_operations

// power management operations
static struct dev_pm_ops csi_pm_ops = 
{
    SET_RUNTIME_PM_OPS( CsiRuntimeSuspend,
                        CsiRuntimeResume, 
                        NULL)
};

/**
* @brief          Module device table
* @details        Module device table
* @pre
* @post
*
*/
static struct of_device_id csi_of_match[] = 
{
  {                
    .compatible = "fsl,s32v234-mipicsi2-0",
  },
  {                
    .compatible = "fsl,s32v234-mipicsi2-1",
  },
  {}
};

MODULE_DEVICE_TABLE(of, csi_of_match);

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
static struct platform_driver csi_platform_driver = {
  .probe  = CsiDrvProbe,
  .remove = CsiDrvRemove,
  .driver = 
  {
    .name           = "csi",
    .owner          = THIS_MODULE,
    .of_match_table = csi_of_match,
    .pm             = &csi_pm_ops,
  },
};


/*==============================================================================
*                                LOCAL FUNCTIONS
==============================================================================*/
/**
* @brief          The function csi open
* @details        The function csi open
*
* @param[in]      inod      struct inode
* @param[in]      pfile     file open
* @return         int
* @api
* @isr
*
* @pre
*
* @post
*
* @implements     CsiOpen_Activity
* 
* @note
*/
static int CsiOpen(struct inode *apInode, struct file *apFile)
{
  VDB_LOG_NOTE("CSI open call\n");

  // allow to be opened only once
  if (sDeviceOpenCnt >= DEV_OPEN_CNT_MAX)
  {
    return -EBUSY;
  } // if device opened already

  sDeviceOpenCnt++;
	  
  try_module_get(THIS_MODULE);
  return CSI_DRV_SUCCESS;
} // CsiOpen()

/*============================================================================*/
/**
* @brief          The function viulite close
* @details        Called whenever a process attempts to close the device file 
*
* @param[in]      inod      struct inode
* @param[in]      pfile     file close
* @return         int
* @api
* @isr
*
* @pre
*
* @post
*
* @implements     CsiRelease_Activity
* 
* @note
*/
static int CsiRelease(struct inode *apInode, struct file *apFile)
{
  sDeviceOpenCnt--;

  module_put(THIS_MODULE);

  return CSI_DRV_SUCCESS;
} // CsiRelease()

/*============================================================================*/
/**
* @brief          CSI clock setup
* @details        CSI clock setup
*
* @param[in]      apdev      struct platform device
* @param[in]      apDrvInfo 
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
static int CsiClockSetup(struct platform_device *apDev, 
                         CSI_DrvInfo_t   *apDrvInfo)
{
  struct clk *lpCsiClk = NULL;
  int          lRet = 0;  
  
  lpCsiClk = devm_clk_get(&apDev->dev, CSI_MODULE_CLK_NAME);
  if (IS_ERR(lpCsiClk)) 
  {
    lRet = PTR_ERR(lpCsiClk);
    dev_err(&apDev->dev, "failed to get main csi module clk: %d\n", lRet);
    goto module_clk;
  } // if failed to get clock

  lRet = clk_prepare_enable(lpCsiClk);
  if (lRet)
  {
    dev_err(&apDev->dev, "failed to enable main csi module clk: %d\n", lRet);
    goto module_clk;
  } // if failed to enable clock
  
  apDrvInfo->mpModuleClk = lpCsiClk;
  
  lpCsiClk = devm_clk_get(&apDev->dev, CSI_LI_CLK_NAME);
  if (IS_ERR(lpCsiClk)) 
  {
    lRet = PTR_ERR(lpCsiClk);
    dev_err(&apDev->dev, "failed to get csi PHY interface clk: %d\n", lRet);
    goto li_clk;
  } // if failed to get clock

  lRet = clk_prepare_enable(lpCsiClk);
  if (lRet)
  {
    dev_err(&apDev->dev, "failed to enable csi PHY interface clk: %d\n", lRet);
    goto li_clk;
  } // if failed to enable clock
  
  apDrvInfo->mpLiClk = lpCsiClk;
  
  lpCsiClk = devm_clk_get(&apDev->dev, CSI_UI_CLK_NAME);
  if (IS_ERR(lpCsiClk)) 
  {
    lRet = PTR_ERR(lpCsiClk);
    dev_err(&apDev->dev, "failed to get csi user interface clk: %d\n", lRet);
    goto ui_clk;
  } // if failed to get clock

  lRet = clk_prepare_enable(lpCsiClk);
  if (lRet)
  {
    dev_err(&apDev->dev, "failed to enable csi PHY low power clk: %d\n", lRet);
    goto ui_clk;
  } // if failed to enable clock
  
  apDrvInfo->mpUiClk = lpCsiClk;
  
  lpCsiClk = devm_clk_get(&apDev->dev, CSI_DPHY_CLK_NAME);
  if (IS_ERR(lpCsiClk)) 
  {
    lRet = PTR_ERR(lpCsiClk);
    dev_err(&apDev->dev, "failed to get csi PHY low power clk: %d\n", lRet);
    goto dphy_clk;
  } // if failed to get clock

  lRet = clk_prepare_enable(lpCsiClk);
  if (lRet)
  {
    dev_err(&apDev->dev, "failed to enable csi user interface clk: %d\n", lRet);
    goto dphy_clk;
  } // if failed to enable clock
  
  apDrvInfo->mpDphyClk = lpCsiClk;

  if(lRet)
  {
  dphy_clk:
    clk_disable_unprepare(apDrvInfo->mpUiClk);
    apDrvInfo->mpUiClk = NULL;
  ui_clk:
    clk_disable_unprepare(apDrvInfo->mpLiClk);
    apDrvInfo->mpLiClk = NULL;
  li_clk:  
    clk_disable_unprepare(apDrvInfo->mpModuleClk);
    apDrvInfo->mpModuleClk = NULL;
  module_clk:
    lRet = -EFAULT;
  }
  
  return lRet;
} // CsiClockSetup()

/*============================================================================*/
/**
* @brief          Probe the driver
* @details        Probe the driver
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
static int CsiDrvProbe(struct platform_device *apDev)
{
  struct resource *lpRes;
  uint32_t         lCsiIdx;
  void*            lpCsiRegs = NULL;
  int              lRet      = 0;
  CSI_DrvInfo_t   *lpDrvInfo = NULL; 

  // find out csi index
  if(strstr(apDev->name, "mipicsi2_0") != NULL)
  {
    lCsiIdx = 0;
  }
  else if(strstr(apDev->name, "mipicsi2_1") != NULL)
  {
    lCsiIdx = 1;
  }
  else
  {
    dev_err(&apDev->dev, "Bad platform device\n");
    return -ENODEV;
  }
  
  if(sppCsiDrvInfo[lCsiIdx] != NULL)
  {
    dev_err(&apDev->dev,"CSI_%u probed already\n", lCsiIdx);
    return -ENODEV;
  } // if already probed
  
  lpDrvInfo = devm_kzalloc(&apDev->dev, 
                           sizeof(CSI_DrvInfo_t), 
                           GFP_KERNEL);
  if(lpDrvInfo == NULL)
  {
    dev_err(&apDev->dev, 
                  "failed to allocate CSI_%u driver data struct\n", lCsiIdx);
    return -ENODEV;
  } // if failed to allocate driver data
  
  // initial driver data setup
  lpDrvInfo->mCsiIdx        = lCsiIdx;
  lpDrvInfo->mpRegs         = NULL;
  lpDrvInfo->mIrqNums.mErr  = 0;
  lpDrvInfo->mIrqNums.mFrm  = 0;
  lpDrvInfo->mpModuleClk    = NULL;  
  lpDrvInfo->mpLiClk        = NULL;      
  lpDrvInfo->mpUiClk        = NULL; 
  lpDrvInfo->mpDphyClk      = NULL;
  lpDrvInfo->mEmbedStatus   = CSI_EMBD_STATUS_NA;
  lpDrvInfo->mPPErrIrqMask  = 0;
  lpDrvInfo->mPhyErrIrqMask = 0;
  lpDrvInfo->mFrameIrqMask  = 0;
  lpDrvInfo->mUserPid       = 0;
  memset(&(lpDrvInfo->mErrStatus), 0, sizeof(CSI_ErrStatus_t));

  /* map register space */
  lpRes = platform_get_resource(apDev, IORESOURCE_MEM, 0);
  if (!lpRes) 
  {
    dev_err(&apDev->dev, "could not get CSI regs resource\n");
    return -ENODEV;
  }

  lpCsiRegs = devm_ioremap_resource(&apDev->dev, lpRes);
  if (IS_ERR(lpCsiRegs)) 
  {
    dev_err(&apDev->dev, "could not ioremap CSI regs resource\n");
    return PTR_ERR(lpCsiRegs);
  }
  lpDrvInfo->mpRegs = (CSI_Regs_t*) lpCsiRegs;

  lpDrvInfo->mIrqNums.mErr = irq_of_parse_and_map(
                                                  apDev->dev.of_node, 
                                                  ERR_IRQ_IDX
                                                 );
  
  lpDrvInfo->mIrqNums.mFrm = irq_of_parse_and_map(
                                                  apDev->dev.of_node, 
                                                  FRM_IRQ_IDX
   
                                              ); 
  
  dev_set_drvdata(&apDev->dev, lpDrvInfo);
  
  if(CsiClockSetup(apDev,lpDrvInfo))
  {
    dev_err(&apDev->dev, "clock setup failed.\n");
    return PTR_ERR(lpCsiRegs);
  } // if CsiClockSetup failed
  
  pm_runtime_enable(&apDev->dev);
  lRet = pm_runtime_get_sync(&apDev->dev);
  
  sppCsiDev[lCsiIdx]     = apDev;
  sppCsiDrvInfo[lCsiIdx] = lpDrvInfo;
  
  return lRet;
} // CsiDrvProbe()

/*============================================================================*/
/**
* @brief          The function viulite remove
* @details        The function viulite remove
*
* @param[in]      pdev   struct platform device
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
static int CsiDrvRemove(struct platform_device *apDev)
{
  CSI_DrvInfo_t *lpDrvInfo = dev_get_drvdata(&apDev->dev);
  
  sppCsiDev[lpDrvInfo->mCsiIdx]     = NULL;
  sppCsiDrvInfo[lpDrvInfo->mCsiIdx] = NULL;
  
  // Disable clocks
  clk_disable_unprepare(lpDrvInfo->mpModuleClk);
  clk_disable_unprepare(lpDrvInfo->mpLiClk);
  clk_disable_unprepare(lpDrvInfo->mpUiClk);
  clk_disable_unprepare(lpDrvInfo->mpDphyClk);
  
  pm_runtime_put_sync(&apDev->dev);
  pm_runtime_disable(&apDev->dev);
 
  return 0;
} // CsiDrvRemove()

/*============================================================================*/
/**
* @brief          The function viulite runtime suspend
* @details        The function viulite runtime suspend
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
static int CsiRuntimeSuspend(struct device *apDev)
{
  CSI_DrvInfo_t *lpDrvInfo = dev_get_drvdata(apDev);

  clk_disable_unprepare(lpDrvInfo->mpModuleClk);
  clk_disable_unprepare(lpDrvInfo->mpLiClk);
  clk_disable_unprepare(lpDrvInfo->mpUiClk);
  clk_disable_unprepare(lpDrvInfo->mpDphyClk);
  
  return 0;
} // CsiRuntimeSuspend()

/*============================================================================*/
/**
* @brief          The function viulite runtime resume
* @details        The function viulite runtime resume
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
static int CsiRuntimeResume(struct device *apDev)
{
  int lRet = 0;
  CSI_DrvInfo_t *lpDrvInfo = dev_get_drvdata(apDev);

  lRet = clk_prepare_enable(lpDrvInfo->mpModuleClk);
  if(lRet)
  {
    VDB_LOG_ERROR("failed to enable csi module clock.\n");
    return lRet;
  }
  
  lRet = clk_prepare_enable(lpDrvInfo->mpLiClk);
  if(lRet)
  {
    clk_disable_unprepare(lpDrvInfo->mpModuleClk);
    VDB_LOG_ERROR("failed to enable csi li clock.\n");
    return lRet;
  }
  
  lRet = clk_prepare_enable(lpDrvInfo->mpUiClk);
  if(lRet)
  {
    clk_disable_unprepare(lpDrvInfo->mpLiClk);
    clk_disable_unprepare(lpDrvInfo->mpModuleClk);
    VDB_LOG_ERROR("failed to enable csi ui clock.\n");
    return lRet;
  }
  
  lRet = clk_prepare_enable(lpDrvInfo->mpDphyClk);
  if(lRet)
  {
    clk_disable_unprepare(lpDrvInfo->mpDphyClk);
    clk_disable_unprepare(lpDrvInfo->mpLiClk);
    clk_disable_unprepare(lpDrvInfo->mpModuleClk);
    VDB_LOG_ERROR("failed to enable csi dphy clock.\n");
    return lRet;
  }
  
  return lRet;
} // CsiRuntimeResume()

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
static int __init CsiInit(void)
{
  int32_t lRetVal = 0;
  
  // enable logging
  vdb_LogInit();
  
  if(platform_driver_register(&csi_platform_driver) != 0)
  {
    lRetVal = CSI_DRV_FAILURE;
    printk("CSI: platform driver registration failed.\n");
    goto csi_pd_failed;
  } // if platform driver registration failed
  
  //
  // Setup the driver  
  //
  
  if(CSI_DRV_Setup(sppCsiDrvInfo) != CSI_DRV_SUCCESS)
  {
    lRetVal = CSI_DRV_FAILURE;
    printk("CSI: driver setup failed.\n");
    goto csi_setup_failed;
  } // if driver setup failed
  
  //
  // Initialize device file
  //
  
  // get device number
  if (alloc_chrdev_region(&sCsiDevno, 0, 1, CSI_DEVICE_NAME) < 0)
  {
    lRetVal = CSI_DRV_FAILURE;
    printk("CSI: device number allocation failed.\n");
    goto device_number_allocation_failed;
  } // if alloc char dev number failed
  
  sCsiDriverClass = class_create(THIS_MODULE, CLASS_NAME);
  
  if(IS_ERR(sCsiDriverClass))
  {
    lRetVal = CSI_DRV_FAILURE;
    printk("CSI: driver class creation failed.\n");
    goto driver_class_cration_failed;
  } // if csi driver class creation failed
    
  if (IS_ERR(device_create(sCsiDriverClass, NULL, sCsiDevno, NULL, 
                    CSI_DEVICE_NAME)))
  {
    lRetVal = CSI_DRV_FAILURE;
    printk("CSI: driver device creation failed.\n");
    goto device_creation_failed;
  } // if device creation failed

  cdev_init(&sCsiCdev, &sFops);
  sCsiCdev.owner = THIS_MODULE;
  sCsiCdev.ops   = &sFops;
  
  if(cdev_add(&sCsiCdev, sCsiDevno, 1))
  {
    lRetVal = CSI_DRV_FAILURE;
    printk("CSI: device registration failed.\n");
    goto registration_failed;
  } // if device registration failed

  sMajor = MAJOR(sCsiDevno);
  
  // print success message
  printk("CSI: driver ready -> CSI0 = %s | CSI1 = %s.\n",
         (sppCsiDrvInfo[0] == NULL)?"disabled":"enabled",
         (sppCsiDrvInfo[1] == NULL)?"disabled":"enabled"
  );
  
  // init error processing
  if( lRetVal )
  {
    registration_failed:
      device_destroy(sCsiDriverClass, sCsiDevno);
    device_creation_failed:
      class_destroy(sCsiDriverClass);
    driver_class_cration_failed:
      unregister_chrdev_region(sCsiDevno, 1);	  
    device_number_allocation_failed:
    csi_setup_failed:
      platform_driver_unregister(&csi_platform_driver);
    csi_pd_failed:
      printk("CSI: Error during csi.ko insmod. Terminating ...\n");
      vdb_LogStop();
  } // if some failure
  
  return lRetVal;
} // CsiInit()

/*============================================================================*/
/**
* @brief          The function CSI exit
* @details        The function CSI exit, Unregister the device 
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
static void __exit CsiExit(void)
{ 
  VDB_LOG_NOTE("Removing csi character device driver module.\n");
  cdev_del(&sCsiCdev);
  device_destroy(sCsiDriverClass, sCsiDevno);
  class_destroy(sCsiDriverClass);
  unregister_chrdev_region(sCsiDevno, 1);
  
  CSI_DRV_Close();
  
  platform_driver_unregister(&csi_platform_driver);
  
  vdb_LogStop();
  
  printk("CSI: driver exited.\n");
} // CsiExit()

//****************************************************************************

#if 1
	module_init(CsiInit);
	module_exit(CsiExit);
#else
	module_platform_driver(csi_platform_driver);	
#endif

//****************************************************************************

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tomas Babinec <tomas.babinec <at> nxp.com>");
MODULE_DESCRIPTION("CSI NXP driver.");
MODULE_ALIAS("mipi-csi");
