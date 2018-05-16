/*****************************************************************************
 *
 * Freescale Confidential Proprietary
 *
 * Copyright (c) 2015-16 Freescale Semiconductor;
 * Copyright (c) 2016-17 NXP;
 * All Rights Reserved
 *
 *****************************************************************************
 *
 * THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/**
 * \file     cam_user.cpp
 * \brief    User-space library calls to access Generic camera driver
 *           functionality.
 * \author   Tomas Babinec
 * \version  0.1
 * \date     13-June-2016
 * \note
 ****************************************************************************/
#include "s32vs234.h"
#include <stdint.h>
#include <cstring>

#ifndef __STANDALONE__
  #include <fcntl.h>		// open 
  #include <unistd.h>		// exit 
  #include <sys/ioctl.h>	// ioctl 
  #include <sys/mman.h>	// mmap
#else
  #include "cam_func.h"
#endif // else from #ifndef __STANDALONE__

#define VDB_LOG_NOTE_OFF

#include "isp_cam_generic.h"
#include "vdb_log.h"
#include "isp_hw.h"

//****************************************************************************
// consts
//****************************************************************************


//****************************************************************************
// types
//****************************************************************************


//****************************************************************************
// global variables
//****************************************************************************


//*****************************************************************************
// local data
//*****************************************************************************

int32_t gCamFD = -1;    ///< cam driver file descriptor

#ifdef __STANDALONE__
///< Register read function pointers
static int32_t (*sppRegRead[2])(const CamGeneric_t *const apCam, 
                                const uint16_t      aRegAddr, 
                                uint8_t *const      apVal) = {
  CAM_DRV_RegRead8,
  CAM_DRV_RegRead16                                  
};

///< Register write function pointers
static int32_t (*sppRegWrite[2])(const CamGeneric_t * const apCam, 
                                 const uint16_t       aRegAddr,
                                 const uint8_t        aVal) = {
  CAM_DRV_RegWrite8,
  CAM_DRV_RegWrite16
};

///< Command write function pointers
static int32_t (*sppCmdWrite[2])(const CamGeneric_t  *const apCam, 
                                       I2cWriteCmd_t *apCmd) = {
                                    
  CAM_DRV_CmdWrite8,                                        
  CAM_DRV_CmdWrite16
};
#endif // __STANDALONE__

//****************************************************************************
// local function declarations
//****************************************************************************


//*****************************************************************************
// functions definitions
//*****************************************************************************

CAM_LIB_RESULT CAM_Open()
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;

#ifndef __STANDALONE__  
  if(gCamFD == -1)
  {  
    gCamFD = open("/dev/cam_generic", 0);
    if (gCamFD == -1) 
    {
      VDB_LOG_ERROR("Can't open device file: %s\n", CAM_DEVICE_NAME);
      lRet = CAM_LIB_FAILURE;
    } // if open failed
  } // if not opened yet
  else 
  {
    VDB_LOG_WARNING("Device file: %s opened already.\n", CAM_DEVICE_NAME);
  } // else from if not opened yet
#else // from ifndef __STANDALONE__
  if(CAM_DRV_Setup() != CAM_DRV_SUCCESS)
  {
    lRet = CAM_LIB_FAILURE;
  } // if Setup failed
  else
  {
    gCamFD = 0;
    lRet   = CAM_LIB_SUCCESS;
  } // else from if setup failed
#endif // #ifndef __STANDALONE__
  
  return lRet;
} // CAM_Reserve()

//*****************************************************************************

CAM_LIB_RESULT CAM_Close()
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;
#ifndef __STANDALONE__  
  if(gCamFD != -1)
  {
    if (close(gCamFD) != 0) 
    {
      VDB_LOG_ERROR("Failed to close device file %s.\n", CAM_DEVICE_NAME);
      lRet = CAM_LIB_FAILURE;
    } // if open failed
    else
    {
      gCamFD = -1;
    } // else if open failed
  } // not openned yet
  else 
  {
    VDB_LOG_WARNING(" already closed.\n");
  } // else from if not openned yet
#else
  CAM_DRV_Close();
  gCamFD = -1;
#endif // else from #ifndef __STANDALONE__ 
  return lRet;
} // CAM_Release()

//*****************************************************************************
// Camera client management
//*****************************************************************************

CamI2cClient_t CAM_ClientAdd(CamI2cCfg_t &arCfg)
{
  CamI2cClient_t lRet = NULL;

#ifndef __STANDALONE__  
  if(gCamFD == -1)
  {
    VDB_LOG_ERROR("CAM device file not opened.\n");
  } // if CAM not opened
  else
  {
    CamI2cCfg_t lCfgCpy = arCfg;
    if (ioctl(gCamFD, IOCTL_CAM_CLI_ADD, &lCfgCpy) != CAM_DRV_SUCCESS) 
    {
      VDB_LOG_ERROR("Adding new camera client failed\n");
    } // if ioctl returned failure
    else
    {
      lRet = *((CamI2cClient_t*)&lCfgCpy);
    } // else from if ioctl returned failure
  } // else from if CAM not opened
#else // #ifndef __STANDALONE__
  lRet = CAM_DRV_ClientAdd(&arCfg);
#endif // else #ifndef __STANDALONE__
  
  return lRet;
} // CAM_ClientAdd()

//*****************************************************************************

CAM_LIB_RESULT CAM_ClientRem(CamI2cClient_t aHandle)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;

  //*** check if valid handle ***
  if(aHandle != NULL)
  {
#ifndef __STANDALONE__  
    if(gCamFD == -1)
    {
      VDB_LOG_ERROR("CAM device file not opened.\n");
      lRet = CAM_LIB_FAILURE;
    } // if CAM not opened
    else
    {
      if (ioctl(gCamFD, IOCTL_CAM_CLI_REM, aHandle) < 0) 
      {
        VDB_LOG_ERROR("Removing camera client failed\n");
        lRet = CAM_LIB_FAILURE;
      } // if ioctl returned failure
    } // else from if CAM not opened
#else // #ifndef __STANDALONE__
    if(CAM_DRV_ClientRem(aHandle) != CAM_DRV_SUCCESS)
    {
      VDB_LOG_ERROR("Removing camera client failed\n");
      lRet = CAM_LIB_FAILURE;
    } // if client removal failure
#endif // else #ifndef __STANDALONE__
  }
  else
  {
    VDB_LOG_ERROR("No such client connected.\n");
    lRet = CAM_LIB_FAILURE;
  } // else from if camera client attached
  
  return lRet;
} // CAM_ClientRem()

//*****************************************************************************
// I2c Read/Write
//*****************************************************************************

CAM_LIB_RESULT CAM_ReadBatch(I2cReadBatch_t &arBatch)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;
  
  //*** check if valid handle ***
  if(arBatch.mDestDev != NULL)
  {
#ifndef __STANDALONE__  
    if(gCamFD == -1)
    {
      VDB_LOG_ERROR("CAM device file not opened.\n");
      lRet = CAM_LIB_FAILURE;
    } // if CAM not opened
    else
    {
      if (ioctl(gCamFD, IOCTL_CAM_REG_RDB, &arBatch) < 0) 
      {
        VDB_LOG_ERROR("I2c read batch failed\n");
        lRet = CAM_LIB_FAILURE;
      } // if ioctl returned failure
    } // else from if CAM not opened
#else // #ifndef __STANDALONE__
    CamGeneric_t *lpCam = (CamGeneric_t*)arBatch.mDestDev;
    int32_t      (*lpI2cRead)(const CamGeneric_t *const apCam, 
                              const uint16_t      aRegAddr, 
                              uint8_t *const      apVal);
      
    lpI2cRead = sppRegRead[lpCam->mCfg.mRegAddrWidth];
    
    for(uint32_t i = 0; i < arBatch.mCmdCnt; i++)
    {
      if(lpI2cRead(lpCam, 
                   arBatch.mpRegAddr[i], 
                   &(arBatch.mpVal[i])) != CAM_DRV_SUCCESS)
        
      {
        VDB_LOG_ERROR("Failed to read from I2C address 0x%x.\n", 
                      arBatch.mpRegAddr[i]);

        lRet = CAM_LIB_FAILURE;
      } // if failed to read the data
    } // for all commands in this part of the batch
#endif // else #ifndef __STANDALONE__
  }
  else
  {
    VDB_LOG_ERROR("No such client connected.\n");
    lRet = CAM_LIB_FAILURE;
  } // else from if camera client attached
  
  return lRet;
} // CAM_ReadBatch()

//*****************************************************************************

CAM_LIB_RESULT CAM_WriteBatch(I2cWriteBatch_t &arBatch)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;

  //*** check if valid handle ***
  if(arBatch.mDestDev != NULL)
  {
#ifndef __STANDALONE__  
    if(gCamFD == -1)
    {
      VDB_LOG_ERROR("CAM device file not opened.\n");
      lRet = CAM_LIB_FAILURE;
    } // if CAM not opened
    else
    {
      if (ioctl(gCamFD, IOCTL_CAM_REG_WRB, &arBatch) < 0) 
      {
        VDB_LOG_ERROR("I2c write batch failed\n");
        lRet = CAM_LIB_FAILURE;
      } // if ioctl returned failure
    } // else from if CAM not opened
#else // #ifndef __STANDALONE__
    CamGeneric_t *lpCam     = (CamGeneric_t*)arBatch.mDestDev;

    int32_t     (*lpI2cWrite)(const CamGeneric_t  *const apCam, 
                              I2cWriteCmd_t *apCmd);
    
    lpI2cWrite = sppCmdWrite[lpCam->mCfg.mRegAddrWidth];
        
    for(uint32_t i = 0; i < arBatch.mCmdCnt; i++)
    {
      if(lpI2cWrite(lpCam, 
                    &(arBatch.mpCmdArr[i])) != CAM_DRV_SUCCESS)              
      {
        VDB_LOG_ERROR("Failed to write to I2C address 0x%x.\n", 
                      arBatch.mpCmdArr[i].mRegAddr
                    );
        lRet = CAM_LIB_FAILURE;
      } // if failed to read the data
    } // for all commands in this part of the batch
#endif // else #ifndef __STANDALONE__
  }
  else
  {
    VDB_LOG_ERROR("No such client connected.\n");
    lRet = CAM_LIB_FAILURE;
  } // else from if camera client attached
  
  return lRet;
} // CAM_WriteBatch()

//*****************************************************************************

int32_t CAM_ReadSingle(const CamI2cClient_t aHandle, const uint16_t aRegAddr)
{
  int32_t lRet = 0;
  
  //*** check if valid handle ***
  if(aHandle != NULL)
  {  
#ifndef __STANDALONE__  
    I2cReadBatch_t lBatch;
    lBatch.mDestDev = aHandle;
    lBatch.mCmdCnt  = aRegAddr;
    
    if(gCamFD == -1)
    {
      VDB_LOG_ERROR("CAM device file not opened.\n");
      lRet = -1;
    } // if CAM not opened
    else
    {
      if (ioctl(gCamFD, IOCTL_CAM_REG_RDS, &lBatch) < 0) 
      {
        VDB_LOG_ERROR("I2c read single failed\n");
        lRet = -1;
      } // if ioctl returned failure
      else
      {
        lRet = (int32_t)lBatch.mCmdCnt & 0x000000ff;
      } // else from if ioctl returned failure
    } // else from if CAM not opened
#else // #ifndef __STANDALONE__
    CamGeneric_t *lpCam = (CamGeneric_t*)  aHandle;
    int32_t     (*lpI2cRead)(const CamGeneric_t *const apCam, 
                             const uint16_t      aRegAddr, 
                             uint8_t *const      apVal);
      
    lpI2cRead = sppRegRead[lpCam->mCfg.mRegAddrWidth];
        
    if(lpI2cRead(lpCam, 
                aRegAddr, 
                (uint8_t*)&lRet) != CAM_DRV_SUCCESS)      
    {
      VDB_LOG_ERROR("Failed to read from I2C address 0x%x.\n", 
                    aRegAddr);

      lRet = -1;
    } // if failed to read the data
#endif // else #ifndef __STANDALONE__
  }
  else
  {
    VDB_LOG_ERROR("No such client connected.\n");
    lRet = -1;
  } // else from if camera client attached
  
  return lRet;
} // CAM_ReadSingle()

//*****************************************************************************

CAM_LIB_RESULT CAM_ReadSingle(const CamI2cClient_t aHandle, 
                              const uint16_t aRegAddr, 
                              uint8_t  &arVal)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;
  int32_t        lRes = -1;
  
  lRes = CAM_ReadSingle(aHandle, aRegAddr);
  if(lRes < 0)
  {
    lRet = CAM_LIB_FAILURE;
  } // if read failed
  else
  {
    arVal = (uint8_t)lRes & (uint8_t)0xff; 
  } // else from if read failed
  
  return lRet;
} //CAM_ReadSingle()

//*****************************************************************************

CAM_LIB_RESULT CAM_WriteSingle(const CamI2cClient_t aHandle, 
                               const uint16_t       aRegAddr,
                               const uint8_t        aVal
                              )
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;
  
  //*** check if valid handle ***
  if(aHandle != NULL)
  {
#ifndef __STANDALONE__  
    I2cWriteBatch_t lBatch;
    lBatch.mDestDev = aHandle;
    lBatch.mCmdCnt  = ((uint32_t)aRegAddr << 8)|(uint32_t)aVal;
    
    if(gCamFD == -1)
    {
      VDB_LOG_ERROR("CAM device file not opened.\n");
      lRet = CAM_LIB_FAILURE;
    } // if CAM not opened
    else
    {
      if (ioctl(gCamFD, IOCTL_CAM_REG_WRS, &lBatch) < 0) 
      {
        VDB_LOG_ERROR("I2c write single failed\n");
        lRet = CAM_LIB_FAILURE;
      } // if ioctl returned failure
    } // else from if CAM not opened
#else // #ifndef __STANDALONE__
    CamGeneric_t *lpCam     = (CamGeneric_t*)aHandle;
    int32_t     (*lpI2cWrite)(const CamGeneric_t * const apCam, 
                              const uint16_t       aRegAddr,
                              const uint8_t        aVal
                             );
    
    lpI2cWrite = sppRegWrite[lpCam->mCfg.mRegAddrWidth];
    
    if(lpI2cWrite(lpCam, 
                  aRegAddr,
                  aVal
                 ) != CAM_DRV_SUCCESS)              
    {
      VDB_LOG_ERROR("Failed to write to I2C address 0x%x.\n", \
                    aRegAddr);
      lRet = CAM_LIB_FAILURE;
    } // if failed to read the data
#endif // else #ifndef __STANDALONE__
  }
  else
  {
    VDB_LOG_ERROR("No such client connected.\n");
    lRet = CAM_LIB_FAILURE;
  } // else from if camera client attached
  
  return lRet;
} // CAM_WriteSingle()

//*****************************************************************************

/*EOF*/
