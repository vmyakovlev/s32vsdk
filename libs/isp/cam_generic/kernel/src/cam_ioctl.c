/*
 * Copyright (c) 2015-2016 Freescale Semiconductor
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
 * \file     fdma_ioctl.c
 * \brief    IOCTL functionality of fDMA driver
 * \author   Tomas Babinec
 * \version  0.1
 * \date     21. September 2015
 * \note
 ****************************************************************************/

#include <asm/uaccess.h>	// for get_user and put_user 
#include <linux/mutex.h>	// for semaphore/mutex
#include <linux/string.h>	// for memset
#include <linux/sched.h>	// for current->pid

#include "cam.h"
#include "cam_func.h"
#include "sony_func.h"
#include "ov10640_func.h"
#include "vdb_log.h"

#define I2C_CACHE_SIZE (uint32_t)256 // number of bytes for caching I2C addrs and vals

//*****************************************************************************

// CAM locking mutex
#if 0
extern mutex_t gCamMutex;
#endif // 0
///< Register read function pointers
static int32_t (*sppRegRead[2])(const CamGeneric_t *const apCam, 
                                const uint16_t      aRegAddr, 
                                uint8_t      *const apVal) = {
  CAM_DRV_RegRead8,
  CAM_DRV_RegRead16                                  
};

///< Register write function pointers
static int32_t (*sppRegWrite[2])(const CamGeneric_t *const apCam, 
                                 const uint16_t      aRegAddr,
                                 const uint8_t       aVal) = {
  CAM_DRV_RegWrite8,
  CAM_DRV_RegWrite16
};

///< Command write function pointers
static int32_t (*sppCmdWrite[2])(const CamGeneric_t  *const apCam, 
                                 I2cWriteCmd_t *const apCmd) = {
                                    
  CAM_DRV_CmdWrite8,                                        
  CAM_DRV_CmdWrite16
};

//*****************************************************************************

static long ReadRegBatch(unsigned long aIoctlParam);
static long WriteRegBatch(unsigned long aIoctlParam);
static long ReadRegSingle(unsigned long aIoctlParam);
static long WriteRegSingle(unsigned long aIoctlParam);
static long WriteRegTest(unsigned long aIoctlParam);

//*****************************************************************************

long CamIoctl(struct file* const apFile,	/* ditto */
              const unsigned int aIoctlNum,	/* number and param for ioctl */
              const unsigned long aIoctlParam)
{
  long lRet = CAM_DRV_SUCCESS;
#if 0  
  // only one IOCTL can be processed at the time
  if(mutex_lock_interruptible(&gCamMutex) > 0)
  {
    VDB_LOG_WARNING("CAM mutex down interrupted.\n");
    lRet = -ERESTARTSYS;
  } // if CAM mutex interrupted
  else
  {
    VDB_LOG_NOTE("CAM mutex locked.\n");
#endif //0
    
    CamI2cCfg_t lCfg;
    CamI2cClient_t lCamHandle;

    // switch the ioctl command
    switch (aIoctlNum) 
    {
      //***********************************************************************
      // *** Generic camera ***      

      case IOCTL_CAM_CLI_ADD:
        if (copy_from_user(&lCfg, (CamI2cCfg_t *)aIoctlParam, 
            sizeof(CamI2cCfg_t)) != 0)
        {
          VDB_LOG_ERROR("Copy from user failed.\n");
          lRet = -EIO;
        } // if copy from user failed
        else
        {  
          CamI2cClient_t lClientHandle;
          lClientHandle = CAM_DRV_ClientAdd(&lCfg);
          if(lClientHandle == 0)
          {
            VDB_LOG_ERROR("Failed to add a client.\n");
            lRet = CAM_DRV_FAILURE;
          } // if failed to add a client
          else
          {
            // *** write back the client handle to user space ***
            if (copy_to_user((CamI2cClient_t*)aIoctlParam, 
                              &lClientHandle, 
                              sizeof(CamI2cClient_t)) != 0)
            {
              VDB_LOG_ERROR("Copy to user failed.\n");
              lRet = -EIO;
            } // if copy from user failed
          } // else from if failed to add a client
        } // else from if copy from user failed
        break;

      case IOCTL_CAM_CLI_REM:
        lCamHandle = (CamI2cClient_t)aIoctlParam;
        lRet = CAM_DRV_ClientRem(lCamHandle);
        break;

      case IOCTL_CAM_REG_RDB:
        lRet = ReadRegBatch(aIoctlParam);       
        break;

      case IOCTL_CAM_REG_WRB:
        lRet = WriteRegBatch(aIoctlParam);                
        break;

      case IOCTL_CAM_REG_RDS:
        lRet = ReadRegSingle(aIoctlParam);       
        break;
      
      case IOCTL_CAM_REG_WRS:
        lRet = WriteRegSingle(aIoctlParam);                
        break;      
      
      case IOCTL_CAM_REG_WRT:
        lRet = WriteRegTest(aIoctlParam);                
        break;   
      //***********************************************************************
      // *** Sony imx224 MipiCam ***
      case IOCTL_SONY_CONNECT:
        if(SONY_DRV_Setup((CamI2cClient_t)aIoctlParam))              
        {
          VDB_LOG_ERROR("Failed to connect Sony camera.\n");
          lRet = CAM_DRV_FAILURE;
        } // if failed to read the data
        break;
      
      case IOCTL_SONY_DISCONNECT:
        if(SONY_DRV_Close((CamI2cClient_t)aIoctlParam))              
        {
          VDB_LOG_ERROR("Failed to disconnect Sony camera.\n");
          lRet = CAM_DRV_FAILURE;
        } // if failed to read the data        
        break;
      
      case IOCTL_SONY_EXP_STATUS_GET:
        if (0U == aIoctlParam)
        {
          lRet = (long)SONY_DRV_ExpStateGet(CSI_IDX_0);
        }
        else if(1U == aIoctlParam)
        {
          lRet = (long)SONY_DRV_ExpStateGet(CSI_IDX_1);
        }
        else
        {
          VDB_LOG_ERROR("Failed to get the Sony exposure status.\n");
          lRet = CAM_DRV_FAILURE;
        }
        break;
      case IOCTL_SONY_EXP_SENT:
      {       
        SONY_DRV_ExpSent((enum I2C_BUS_IDX)aIoctlParam);
      } // IOCTL_SONY_EXP_STATUS_SET
      break;
     
      //***********************************************************************
      // *** Maxim MipiCam Start/Stop ***
      
      //***********************************************************************
      // *** OV10635 Viu ***
      
      //***********************************************************************
      // *** Maxim OV10640 ***
      
      //***********************************************************************
      // *** OV10640 MipiCam ***
      case IOCTL_OV10640_CONNECT:
        if(OV10640_DRV_Setup((CamI2cClient_t)aIoctlParam))              
        {
          VDB_LOG_ERROR("Failed to connect Sony camera.\n");
          lRet = CAM_DRV_FAILURE;
        } // if failed to read the data
        break;
      
      case IOCTL_OV10640_DISCONNECT:
        if(OV10640_DRV_Close((CamI2cClient_t)aIoctlParam))              
        {
          VDB_LOG_ERROR("Failed to disconnect Sony camera.\n");
          lRet = CAM_DRV_FAILURE;
        } // if failed to read the data        
        break;
      //***********************************************************************
      
      default:
        VDB_LOG_WARNING("Unknown ioctl command.\n");
        lRet = printk("Unknown ioctl command.\n");
        
        lRet = -ENOTTY;
        break;            
      
    } // switch ioctl command
#if 0  
    // relase the CAM mutex
    mutex_unlock(&gCamMutex);
    VDB_LOG_NOTE("Cam mutex released.\n");
  }// else from if mutex down interrupted
#endif // 0

  return lRet;
} // CamIoctl()

//*****************************************************************************

static long ReadRegBatch(unsigned long aIoctlParam)
{
  long lRet = CAM_DRV_SUCCESS;
  I2cReadBatch_t lBatch;
  // *** fetch batch info from user space ***
  if (copy_from_user( &lBatch, (I2cReadBatch_t *)aIoctlParam, 
      sizeof(I2cReadBatch_t)) != 0)
  {
    VDB_LOG_ERROR("Copy from user failed.\n");
    lRet = -EIO;
  } // if copy from user failed
  else
  { 
    if( (lBatch.mDestDev  != 0) &&
        (lBatch.mpRegAddr != NULL) && 
        (lBatch.mpVal     != NULL))
    {
      const CamGeneric_t * const lpCam = lBatch.mDestDev;
      uint32_t      lPartSize = (uint32_t)(I2C_CACHE_SIZE/((uint32_t)3)); // 3 bytes per read command
      uint32_t      i;
      int32_t      (*lpRegRead)(const CamGeneric_t *const apCam, 
                                const uint16_t      aRegAddr, 
                                uint8_t      *const apVal);
      
      // *** decide which function to use 8/16 bit ***
      lpRegRead = sppRegRead[lpCam->mCfg.mRegAddrWidth];

      // *** divide batch into smaller parts ***
      for(i = 0; i < lBatch.mCmdCnt; i += lPartSize)
      {
        uint32_t lCmdCnt = min((lBatch.mCmdCnt - i), lPartSize);
        uint16_t lpAddr[I2C_CACHE_SIZE/3];
        uint8_t  lpVal[I2C_CACHE_SIZE/3];
        
        // *** fetch register addresses from user space ***
        if (copy_from_user( lpAddr, 
                            (I2cReadBatch_t *)&(lBatch.mpRegAddr[i]), 
                            sizeof(uint16_t) * lCmdCnt) != 0)
        {
          VDB_LOG_ERROR("Copy from user failed.\n");
          lRet = -EIO;
        } // if copy from user failed
        else
        { 
          // *** read from the addresses ***
          uint32_t j;
          for( j = 0; j < lCmdCnt; j++)
          {
            if(lpRegRead(lpCam, 
                         lpAddr[j], 
                         &(lpVal[j])) != CAM_DRV_SUCCESS)              
            {
              VDB_LOG_ERROR("Failed to read from I2C address 0x%x.\n",
                            lpAddr[j]);
              lRet = CAM_DRV_FAILURE;
            } // if failed to read the data
          } // for all commands in this part of the batch
          
          // *** write back the read data to user space ***
          if (copy_to_user((uint8_t*)&(lBatch.mpVal[i]), 
                            lpVal, 
                            sizeof(uint8_t) * lCmdCnt) != 0)
          {
            VDB_LOG_ERROR("Copy to user failed.\n");
            lRet = -EIO;
          } // if copy from user failed
        } // else from if copy from user failed
      } // until all commands fetched and done
    } // if pointers not NULL
    else
    {
      VDB_LOG_ERROR("Batch contains NULL pointers.\n");
      lRet = CAM_DRV_FAILURE;
    } // else from if pointers not NULL
  } // else from if copy from user failed   
  
  return lRet;
} // ReadRegBatch()

//*****************************************************************************

static long WriteRegBatch(unsigned long aIoctlParam)
{
  long lRet = CAM_DRV_SUCCESS;
  I2cWriteBatch_t lBatch;
  // *** fetch batch info from user space ***
  if (copy_from_user( &lBatch, (I2cReadBatch_t *)aIoctlParam, 
      sizeof(I2cReadBatch_t)) != 0)
  {
    VDB_LOG_ERROR("Copy from user failed.\n");
    lRet = -EIO;
  } // if copy from user failed
  else
  { 
    if((lBatch.mDestDev  != 0) &&
       (lBatch.mpCmdArr  != NULL))
    {
      const CamGeneric_t * const lpCam     = lBatch.mDestDev;
	  uint32_t      lNumBytes = (uint32_t)sizeof(I2cWriteCmd_t);
      uint32_t      lPartSize = (uint32_t)(I2C_CACHE_SIZE/lNumBytes);
      uint32_t      i;
      int32_t      (*lpCmdWrite)(const CamGeneric_t  *const apCam, 
                                 I2cWriteCmd_t *const apCmd);      
      
      lpCmdWrite = sppCmdWrite[lpCam->mCfg.mRegAddrWidth];
      
      // *** divide batch into smaller parts ***
      for(i = 0; i < lBatch.mCmdCnt; i += lPartSize)
      {
        uint32_t      lCmdCnt = min((lBatch.mCmdCnt - i), lPartSize);
        I2cWriteCmd_t lpCmd[lPartSize];
        
        // *** fetch commands from user space ***
        if (copy_from_user( lpCmd, 
                            (I2cReadBatch_t *)&(lBatch.mpCmdArr[i]), 
                            sizeof(I2cWriteCmd_t) * lCmdCnt) != 0)
        {
          VDB_LOG_ERROR("Copy from user failed.\n");
          lRet = -EIO;
        } // if copy from user failed
        else
        { 
          // *** write the commands ***
          uint32_t j;
          for( j = 0; j < lCmdCnt; j++)
          {
            if(lpCmdWrite(lpCam, 
                          &(lpCmd[j])) != CAM_DRV_SUCCESS)              
            {
              VDB_LOG_ERROR("Failed to write to I2C address 0x%x.\n"
                            "Terminating batch command at index %u.\n",
                            lpCmd[j].mRegAddr, j
                           );
              lRet = CAM_DRV_FAILURE;
              break;
            } // if failed to read the data
          } // for all commands in this part of the batch
        } // else from if copy from user failed
      } // until all commands fetched and done
    } // if pointers not NULL
    else
    {
      VDB_LOG_ERROR("Batch contains NULL pointers.\n");
      lRet = CAM_DRV_FAILURE;
    } // else from if pointers not NULL
  } // else from if copy from user failed  
  
  return lRet;
} // WriteRegBatch()

//*****************************************************************************

static long ReadRegSingle(unsigned long aIoctlParam)
{
  long lRet = CAM_DRV_SUCCESS;
  I2cReadBatch_t lBatch;
  // *** fetch batch info from user space ***
  if (copy_from_user( &lBatch, (I2cReadBatch_t *)aIoctlParam, 
      sizeof(I2cReadBatch_t)) != 0)
  {
    VDB_LOG_ERROR("Copy from user failed.\n");
    lRet = -EIO;
  } // if copy from user failed
  else
  { 
    if(lBatch.mDestDev != 0)
    {
      const CamGeneric_t *const lpCam = lBatch.mDestDev;
      int32_t      (*lpRegRead)(const CamGeneric_t *const apCam, 
                                const uint16_t     aRegAddr, 
                                uint8_t     *const apVal);
      
      lpRegRead = sppRegRead[lpCam->mCfg.mRegAddrWidth];

      if(lpRegRead(lpCam, (uint16_t)lBatch.mCmdCnt, (uint8_t*)&(lBatch.mCmdCnt)) 
           != CAM_DRV_SUCCESS)        
      {
        VDB_LOG_ERROR("Failed to read from I2C address 0x%x.\n", \
                      lBatch.mCmdCnt);
        lRet = CAM_DRV_FAILURE;
      } // if failed to read the data      
          
      // *** write back the read data to user space ***
      if (copy_to_user((I2cReadBatch_t *)aIoctlParam, 
                        &lBatch, 
                        sizeof(I2cReadBatch_t)) != 0)
      {
        VDB_LOG_ERROR("Copy to user failed.\n");
        lRet = -EIO;
      } // if copy from user failed        
    } // if input OK
    else
    {
      VDB_LOG_ERROR("Bad I2C command config.\n");
      lRet = CAM_DRV_FAILURE;
    } // else from if pointers not NULL
  } // else from if copy from user failed   
  
  return lRet;
} // ReadRegSingle()

//*****************************************************************************

static long WriteRegSingle(unsigned long aIoctlParam)
{
  long lRet = CAM_DRV_SUCCESS;
  I2cWriteBatch_t lBatch;
  // *** fetch batch info from user space ***
  if (copy_from_user( &lBatch, (I2cReadBatch_t *)aIoctlParam, 
      sizeof(I2cReadBatch_t)) != 0)
  {
    VDB_LOG_ERROR("Copy from user failed.\n");
    lRet = -EIO;
  } // if copy from user failed
  else
  { 
    if(lBatch.mDestDev  != 0)
    {
      uint16_t      lRegAddr = (uint16_t)((lBatch.mCmdCnt >> 8) & 0x0000ffffU);
      uint8_t       lVal     = (uint8_t)(lBatch.mCmdCnt & 0x000000ffU);
      const CamGeneric_t * const lpCam = lBatch.mDestDev;
      int32_t      (*lpRegWrite)(const CamGeneric_t *const, const uint16_t, const uint8_t);
      
      lpRegWrite = sppRegWrite[lpCam->mCfg.mRegAddrWidth];

      if(lpRegWrite(lpCam, lRegAddr, lVal) != CAM_DRV_SUCCESS)              
      {
        VDB_LOG_ERROR("Failed to write to I2C address 0x%x.\n", \
                      lRegAddr);
        lRet = CAM_DRV_FAILURE;
      } // if failed to read the data
    } // if pointers not NULL
    else
    {
      VDB_LOG_ERROR("Bad I2c command config.\n");
      lRet = CAM_DRV_FAILURE;
    } // else from if pointers not NULL
  } // else from if copy from user failed  
  
  return lRet;
} // WriteRegSingle()

//*****************************************************************************

static long WriteRegTest(unsigned long aIoctlParam)
{
  long lRet = CAM_DRV_SUCCESS;
  I2cWriteBatch_t lBatch;
  // *** fetch batch info from user space ***
  if (copy_from_user( &lBatch, (I2cReadBatch_t *)aIoctlParam, 
      sizeof(I2cReadBatch_t)) != 0)
  {
    VDB_LOG_ERROR("Copy from user failed.\n");
    lRet = -EIO;
  } // if copy from user failed
  else
  { 
    if((lBatch.mDestDev  != 0) &&
       (lBatch.mpCmdArr  != NULL))
    {
      I2cWriteCmd_t lCmd;
      const CamGeneric_t *const lpCam = lBatch.mDestDev;
      int32_t     (*lpRegWrite)(const CamGeneric_t *const, const uint16_t, const uint8_t);
      
      lpRegWrite = sppRegWrite[lpCam->mCfg.mRegAddrWidth];

      // *** fetch commands from user space ***
      if (copy_from_user( &lCmd, 
                          (I2cWriteCmd_t *)lBatch.mpCmdArr, 
                          sizeof(I2cWriteCmd_t)) != 0)
      {
        VDB_LOG_ERROR("Copy from user failed.\n");
        lRet = -EIO;
      } // copy from user failed
      else
      { 
        // *** write the commands ***
        uint32_t j;
        for( j = 0; j < lBatch.mCmdCnt; j++)
        {
          
          if(lpRegWrite(lpCam, 
                        lCmd.mRegAddr,
                        lCmd.mVal
                       ) != CAM_DRV_SUCCESS)              
          {
            VDB_LOG_ERROR("Failed to write to I2C address 0x%x.\n", \
                          lCmd.mRegAddr);
            lRet = CAM_DRV_FAILURE;
          } // if failed to read the data
        } // all commands of the batch
      } // copy from user failed
    } // pointers not NULL
    else
    {
      VDB_LOG_ERROR("Batch contains NULL pointers.\n");
      lRet = CAM_DRV_FAILURE;
    } // else from if pointers not NULL
  } // else from if copy from user failed  
  
  return lRet;
} // WriteRegTest()

//*****************************************************************************