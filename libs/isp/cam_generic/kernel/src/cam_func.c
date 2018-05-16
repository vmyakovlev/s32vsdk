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
 * \file    csi_func.c
 * \brief   Mipi csi receiver interface functionality implementation.
 * \author  Tomas Babinec
 * \version 0.1
 * \date    18-September-2015
 ****************************************************************************/

#if !defined(__STANDALONE__)
  #include <linux/kernel.h>     // We're doing kernel work
  #include <linux/module.h>     // Specifically, a module 
  #include <linux/interrupt.h>  // For interrupts
  #include <asm/siginfo.h>      // siginfo
  #include <linux/sched.h>      // send_sig_info
  #include <linux/types.h>
  #include <linux/delay.h>      // for msleep
  #include <linux/slab.h>       // for malloc and free
  #include <linux/i2c.h>
  #include <asm/io.h>           // for ioremap
  #include <linux/of_platform.h>
  #include <linux/platform_device.h>
#else 
  #include <stdio.h>
  #include <unistd.h>
  #include <stdint.h>
  #include <stddef.h>
  #include <math.h>
  #include "i2c.h"
  
  #include "seq_sa_kernel_api.h"
  

  unsigned long get_uptime_microS(void);

  #define msleep(_val) \
        {\
          unsigned long lStart = get_uptime_microS();\
          while((get_uptime_microS() - lStart) < _val*1000);\
        }  
#endif // else from if !defined(__STANDALONE__)

#include "config.h"
#include "cam.h"
#include "cam_func.h"
#include "vdb_log.h"
#if defined(__KERNEL__)
  #include <linux/i2c.h>
#endif // #ifdef __KERNEL__

//****************************************************************************
// consts
//****************************************************************************
#define I2C_CLI_CNT_MAX 50 // number of supported I2C clients

//****************************************************************************
// macros
//****************************************************************************

//****************************************************************************
// static global variables
//***************************************************************************/

static uint32_t sCliAct = 0; ///< number of active i2c clients

static VDList_t sCamList;    ///< list of active cameras

#ifdef __KERNEL__

static int CamI2c_Probe(struct i2c_client *client,
                          const struct i2c_device_id *id);
static int CamI2c_Remove(struct i2c_client *client);

static const struct i2c_device_id sCamId[] = 
{
  {"s32v-cam-generic", 0},
  {},
};

static struct i2c_driver sCamI2cDriver = 
{
  .driver = 
  {
    .owner  = THIS_MODULE,
    .name   = "s32v-cam-generic",
  },
  .probe    = CamI2c_Probe,
  .remove   = CamI2c_Remove,
  .id_table = sCamId,
};

//****************************************************************************
// global variables
//****************************************************************************

//****************************************************************************
// functions declarations
//****************************************************************************

/****************************************************************************/
/** Remove specified camera client.
 * 
 * \param apCfg camera client config
 * 
 * \return pointer to i2c client structure if all ok,
 *         NULL otherwise.
 ****************************************************************************/
static struct i2c_client* CAM_DRV_I2cClientAdd(const CamI2cCfg_t *const apCfg);

/****************************************************************************/
/** Remove specified camera client.
 * 
 * \param apClient i2c client config
 * 
 * \return CAM_DRV_SUCCESS if all ok,
 *         CAM_DRV_FAILURE otherwise.
 ****************************************************************************/
static int32_t CAM_DRV_I2cClientRem(struct i2c_client *const apClient);

/****************************************************************************/
/** Reads 1byte from a 8bit register address in I2C device.
 *  
 * \param apI2cClient   I2c client pointer defining device
 * \param aRegAddr      8bit register address
 * \param apVal         8bit value to be read
 * 
 * \return CAM_DRV_SUCCESS if all ok
 *         CAM_DRV_FAILURE otherwise
 ****************************************************************************/
static int32_t LinuxI2cRead8(const struct i2c_client *const apI2cClient, 
                             const uint8_t aRegAddr, 
                             uint8_t *const apVal);

/****************************************************************************/
/** Reads 1byte from a 16bit register address in I2C device.
 *  
 * \param apI2cClient   I2c client pointer defining device
 * \param aRegAddr      16bit register address
 * \param apVal         8bit value to be read
 * 
 * \return CAM_DRV_SUCCESS if all ok
 *         CAM_DRV_FAILURE otherwise
 ****************************************************************************/
static int32_t LinuxI2cRead16(const struct i2c_client *const apI2cClient, 
                              const uint16_t    aRegAddr, 
                              uint8_t *const    apVal);

/****************************************************************************/
/** Writes 1byte to a 8bit register address in I2C device.
 *  
 * \param apI2cClient   I2c client pointer defining device
 * \param aRegAddr      8bit register address
 * \param aVal          8bit value to be written
 * 
 * \return CAM_DRV_SUCCESS if all ok
 *         CAM_DRV_FAILURE otherwise
 ****************************************************************************/
static int32_t LinuxI2cWrite8(const struct i2c_client *const apI2cClient, 
                              const uint16_t    aRegAddr, 
                              const uint8_t     aVal);

/****************************************************************************/
/** Writes 1byte to a 16bit register address in I2C device.
 *  
 * \param apI2cClient   I2c client pointer defining device
 * \param aRegAddr      16bit register address
 * \param aVal          8bit value to be written
 * 
 * \return CAM_DRV_SUCCESS if all ok
 *         CAM_DRV_FAILURE otherwise
 ****************************************************************************/
static int32_t LinuxI2cWrite16(const struct i2c_client *const apI2cClient, 
                               const uint16_t    aRegAddr, 
                               const uint8_t     aVal);

#endif // ifdef __KERNEL__

//****************************************************************************
// functions definitions
//****************************************************************************

int32_t CAM_DRV_Setup()
{ 
  int32_t lRet = CAM_DRV_SUCCESS;
  
  VDList_Init(&sCamList);
  
#ifdef __STANDALONE__  
  I2C_Init_Config(I2C0, I2C_SONY_CAM_ADDR << 1);  
  I2C_Init_Config(I2C1, I2C_SONY_CAM_ADDR << 1);
  I2C_Init_Config(I2C2, I2C_SONY_CAM_ADDR << 1);
#endif //#ifdef __STANDALONE__ 
  
  return lRet;
} // CAM_DRV_Setup()

//****************************************************************************

int32_t CAM_DRV_Close()
{
  int32_t lRet = CAM_DRV_SUCCESS;
    
  /// *** remove all active clients ***
  while((int32_t)1 != VDList_Empty(&sCamList))
  {
    const CamI2cClient_t lCamHandle = (CamI2cClient_t) sCamList.mpTail->mpData;
    if(lCamHandle != 0)
    {
      if(CAM_DRV_ClientRem(lCamHandle) != CAM_DRV_SUCCESS) 
      {
        VDB_LOG_ERROR("Failed to remove camera client\n");      
        lRet = CAM_DRV_FAILURE;
      } // if client remove failed
    } // if cam is NULL
    else
    {
      VDB_LOG_ERROR("Camera pointer is NULL.\n");
      lRet = CAM_DRV_FAILURE;
    } // else from if cam is NULL
  } // while not empty
  
  return lRet;
} //CAM_DRV_Close()

//****************************************************************************
// Client management
//****************************************************************************

CamI2cClient_t CAM_DRV_ClientAdd(const CamI2cCfg_t * const apCfg)
{
  CamI2cClient_t lRet  = 0;
  CamGeneric_t  *lpCam = NULL;
  
  lpCam = (CamGeneric_t *)kmalloc(sizeof(CamGeneric_t), GFP_KERNEL);
  
  if(lpCam != NULL)
  {
    lpCam->mVDListNode.mpData = (void *)lpCam;
#ifdef __KERNEL__
    lpCam->mI2cClient = CAM_DRV_I2cClientAdd(apCfg);
    if(NULL == lpCam->mI2cClient)
    {
      VDB_LOG_ERROR(
        "Failed to create I2C client (I2C addr 0x%x.)\n",
        lpCam->mCfg.mI2cAddr
      );
      
      kfree(lpCam);
    } // if failed to create I2C client
    else
    {
#endif // #ifdef __KERNEL__
      VDB_LOG_NOTE("CGM: Client added (I2C addr 0x%x)\n", lpCam->mCfg.mI2cAddr);
      lpCam->mCfg = *apCfg;    
      lRet = (CamI2cClient_t)lpCam;

      VDList_HeadPush(&sCamList, &(lpCam->mVDListNode));
#ifdef __KERNEL__
    }
#endif // #ifdef __KERNEL__
  } // if allocation succeeded
  else
  {
    VDB_LOG_ERROR("Failed to allocate memory for camera \n");
  } // else from if allocation succeeded

  return lRet;
} // CAM_DRV_ClientAdd()

//*****************************************************************************

int32_t CAM_DRV_ClientRem(const CamI2cClient_t aClient)
{
  int32_t       lRet  = CAM_DRV_SUCCESS;
  CamGeneric_t *const lpCam = aClient;
  
  if( lpCam != NULL)
  {
#ifdef __KERNEL__
    lRet = CAM_DRV_I2cClientRem(lpCam->mI2cClient);
#endif // #ifdef __KERNEL__
    VDB_LOG_NOTE("CGM: Client removed (I2C addr 0x%x)\n", lpCam->mCfg.mI2cAddr);
    VDList_Rem(&sCamList, &(lpCam->mVDListNode));
    kfree(lpCam);
          
  } // if Camera client exists
  else
  {
    lRet = CAM_DRV_FAILURE;
    VDB_LOG_ERROR("Bad Camera client handle.\n");
  } // else from if Camera client exists
  
  return lRet;
} //CAM_DRV_ClientRem()

//****************************************************************************
// I2c Read/Write
//****************************************************************************

int32_t CAM_DRV_RegWrite8(const CamGeneric_t *const apCam, 
                          const uint16_t      aRegAddr,
                          const uint8_t       aVal
                         )
{
  int32_t lRet = CAM_DRV_SUCCESS;
  
  // *** write new value ***
#ifdef __STANDALONE__     
  I2C8_DevTx(apCam->mCfg.mI2cBusIdx, 
             apCam->mCfg.mI2cAddr, 
             (uint8_t)(aRegAddr & 0x00ff), 
             aVal);
#else
  lRet = LinuxI2cWrite8(apCam->mI2cClient, 
                        (uint8_t)aRegAddr, 
                        aVal);
#endif // #ifdef __STANDALONE__ 
  
  return lRet;
} // CAM_DRV_RegWrite8()

//****************************************************************************

int32_t CAM_DRV_CmdWrite8(const CamGeneric_t  * const apCam, 
                          I2cWriteCmd_t *const        apCmd)
{
  int32_t  lRet = CAM_DRV_SUCCESS;
  uint32_t i    = 0;
  
  // *** if mask set read old value first ***
  if(((uint8_t)0) != apCmd->mMask) 
  {
    uint8_t lVal = 0;
    if(CAM_DRV_RegRead8(apCam, apCmd->mRegAddr, &lVal) != CAM_DRV_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to read value to apply mask.\n");
      lRet = CAM_DRV_FAILURE;
    } // if read failed
    else
    {
      // apply mask
      apCmd->mVal = (apCmd->mVal & (~(apCmd->mMask))) | (lVal & apCmd->mMask);
    } // else from if failed to read   
  } // if mask is used
  
  if(CAM_DRV_SUCCESS == lRet)
  {
    // *** write new value ***
#ifdef __STANDALONE__     
    I2C8_DevTx(apCam->mCfg.mI2cBusIdx, 
             apCam->mCfg.mI2cAddr, 
             (uint8_t)(apCmd->mRegAddr & 0xff), 
             apCmd->mVal);
#else
    lRet = LinuxI2cWrite8(apCam->mI2cClient, 
                        apCmd->mRegAddr, 
                        apCmd->mVal);
#endif // #ifdef __STANDALONE__ 
  
    // *** wait as requested ***
    if(((uint16_t)0) != apCmd->mDelay)
    {
      msleep(apCmd->mDelay);
    } // if delay requested

	// *** read back and check if requested ***
    while(i < (apCmd->mReadBackCnt))
    {    
      uint8_t lVal = 0;
      lRet = CAM_DRV_RegRead8(apCam, apCmd->mRegAddr, &lVal);
      if(lRet == CAM_DRV_SUCCESS)
      {
        // check
        if(apCmd->mVal == lVal)
        {
          break;
        } // if match
      } // if read successful

      lRet = CAM_DRV_FAILURE;
    
      // *** wait as requested ***
      msleep(apCmd->mDelay);
      i++;
    } // while not all attempts done
  } // if READ was successful

  return lRet;
} // CAM_DRV_CmdWrite8()

//*****************************************************************************

int32_t CAM_DRV_RegRead8(const CamGeneric_t *const apCam, 
                         const uint16_t      aRegAddr, 
                         uint8_t *const apVal)
{
  int32_t lRet = CAM_DRV_SUCCESS;
#ifdef __STANDALONE__   
  uint8_t* lpAddr = (uint8_t*) &aRegAddr;
  *apVal = I2C8_DevRx(apCam->mCfg.mI2cBusIdx, 
                     apCam->mCfg.mI2cAddr, 
                     lpAddr[0]);
#else
  lRet = LinuxI2cRead8(apCam->mI2cClient, 
                      (uint8_t)aRegAddr, 
                      apVal);
#endif // #ifdef __STANDALONE__ 
  return lRet;
} //CAM_DRV_RegRead8()

//*****************************************************************************

int32_t CAM_DRV_RegWrite16(const CamGeneric_t * const apCam, 
                           const uint16_t      aRegAddr,
                           const uint8_t       aVal
                          )
{
  int32_t lRet = CAM_DRV_SUCCESS;
  
  // *** write new value ***
#ifdef __STANDALONE__   
  uint8_t* lpAddr = (uint8_t*) &aRegAddr;
  
  I2C_DevTx(apCam->mCfg.mI2cBusIdx, 
            apCam->mCfg.mI2cAddr, 
            lpAddr[1], 
            lpAddr[0], 
            aVal);
#else
  lRet = LinuxI2cWrite16(apCam->mI2cClient, 
                         aRegAddr, 
                         aVal);
#endif // #ifdef __STANDALONE__ 
  
  return lRet;
} // CAM_DRV_CmdWrite16()

//*****************************************************************************
int32_t CAM_DRV_CmdWrite16(const CamGeneric_t  *const apCam, 
                           I2cWriteCmd_t *const apCmd)
{
  int32_t lRet = CAM_DRV_SUCCESS;
  uint32_t i = 0;
  // *** if mask set read old value first ***
  if(((uint8_t)0) != apCmd->mMask) 
  {
    uint8_t lVal = 0;
    if(CAM_DRV_RegRead16(apCam, apCmd->mRegAddr, &lVal) != CAM_DRV_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to read value to apply mask.\n");
      lRet = CAM_DRV_FAILURE;
    } // if read failed
    else
    {
      // apply mask
      apCmd->mVal = (apCmd->mVal & (~(apCmd->mMask))) | (lVal & apCmd->mMask);
    } // else from if failed to read   
  } // if mask is used
  
  if (CAM_DRV_SUCCESS == lRet)
  {
    // *** write new value ***
#ifdef __STANDALONE__   
    uint8_t* lpAddr = (uint8_t*) &(apCmd->mRegAddr);
  
    I2C_DevTx(apCam->mCfg.mI2cBusIdx, 
            apCam->mCfg.mI2cAddr, 
            lpAddr[1], 
            lpAddr[0], 
            apCmd->mVal);
#else
    lRet = LinuxI2cWrite16(apCam->mI2cClient, 
                           apCmd->mRegAddr, 
                           apCmd->mVal);
#endif // #ifdef __STANDALONE__ 
  
    // *** wait as requested ***
    if(((uint16_t)0) != apCmd->mDelay)
    {
      msleep(apCmd->mDelay);
    } // if delay required
  
    // *** read back and check if requested ***
    while(i < (apCmd->mReadBackCnt))
    {
      uint8_t lVal = 0;
      lRet = CAM_DRV_RegRead16(apCam, apCmd->mRegAddr, &lVal);
      if(CAM_DRV_SUCCESS == lRet)
      {
        // check
        if(apCmd->mVal == lVal)
        {
          break;
        } // if match
      } // if read successful
      lRet = CAM_DRV_FAILURE;
    
      // *** wait as requested ***
      msleep(apCmd->mDelay);
      i++;
    } // while not all attempts done
  } // if READ is successful  
  return lRet;
} // CAM_DRV_CmdWrite16()

//*****************************************************************************

int32_t CAM_DRV_RegRead16(const CamGeneric_t *const apCam, 
                          const uint16_t      aRegAddr, 
                          uint8_t *const      apVal)
{
  int32_t lRet = CAM_DRV_SUCCESS;
#ifdef __STANDALONE__   
  uint8_t* lpAddr = (uint8_t*) &aRegAddr;
  *apVal = I2C_DevRx(apCam->mCfg.mI2cBusIdx, 
                     apCam->mCfg.mI2cAddr, 
                     lpAddr[1], 
                     lpAddr[0]);
#else
  lRet = LinuxI2cRead16(apCam->mI2cClient, 
                       aRegAddr, 
                       apVal);
#endif // #ifdef __STANDALONE__ 
  return lRet;
} //CAM_DRV_RegRead16()

//*****************************************************************************
// Linux kernel related functionality
//*****************************************************************************

#ifdef __KERNEL__
// *** I2C client Add/Rem***

static struct i2c_client* CAM_DRV_I2cClientAdd(const CamI2cCfg_t *const apCfg)
{
  struct i2c_adapter *lpI2cAdap  = NULL;
  int    lI2cBusIdx = (int)apCfg->mI2cBusIdx;
  struct i2c_client  *lpI2cCli   = NULL;
  int lRetPrint = 0;

  lRetPrint = pr_info(
    "Probing I2c client @ 0x%x on I2C bus #%u.\n", 
    apCfg->mI2cAddr,
    lI2cBusIdx);

  lpI2cAdap = i2c_get_adapter(lI2cBusIdx);
  if(lpI2cAdap != NULL)
  {
    struct i2c_board_info lpI2cInfo = 
    {
      I2C_BOARD_INFO("s32v-cam-generic", apCfg->mI2cAddr)
    };
    
    lpI2cCli = i2c_new_device(lpI2cAdap, &lpI2cInfo);
    i2c_put_adapter(lpI2cAdap);
    if(lpI2cCli == NULL)
    {
      VDB_LOG_ERROR("I2c client is NULL.\n");
    } // if client not created
    else
    { 
      if(sCliAct == (uint32_t)0)
      {
        if(0 != i2c_add_driver(&sCamI2cDriver))
        {
          VDB_LOG_ERROR("Failed to add generic camera i2c driver.\n");
          i2c_unregister_device(lpI2cCli);
          lpI2cCli = NULL;
        } // if add Sony i2c client driver failed
        else
        {
          lRetPrint = pr_info("Generic camera I2c driver added successfully.\n");
        } // else from if add Sony i2c client driver failed
      } // if driver not registerred already
      sCliAct++;
    } // else from if no i2c client
  } // if I2C addapter not NULL
  else
  {
    VDB_LOG_ERROR("Faled to get I2C adapter %u.\n", lI2cBusIdx);
  } // else from if I2C addapter not NULL
  
  return lpI2cCli;
} // CAM_DRV_I2cClientAdd()

//*****************************************************************************

static int32_t CAM_DRV_I2cClientRem(struct i2c_client *const apClient)
{
  int32_t lRet = CAM_DRV_SUCCESS;

  if(apClient != NULL)
  {
    if(sCliAct == (uint32_t)1)
    {
      i2c_del_driver(&sCamI2cDriver);
    } // if last device active

    i2c_unregister_device(apClient);
    sCliAct--;
  } // if Client exists
  
  return lRet;
} //CAM_DRV_I2cClientRem()

//****************************************************************************

static int CamI2c_Probe(struct i2c_client *client,
                        const struct i2c_device_id *id)
{ 
  return 0;
} // SonyI2c_Probe()

//*****************************************************************************

static int CamI2c_Remove(struct i2c_client *client)
{
  return 0;
}  // SonyI2c_Remove()

//*****************************************************************************
// *** I2c linux access ***
static int32_t LinuxI2cWrite8(const struct i2c_client *const apI2cClient, 
                              const uint16_t aRegAddr, 
                              const uint8_t aVal)
{
  uint8_t  lBuf[2]   = {0};
  int32_t lNumBytes = 0;
  int32_t lRetVal   = CAM_DRV_SUCCESS;
  int lRetPrint = 0;

  lBuf[0] = (uint8_t)aRegAddr;
  lBuf[1] = aVal;
  
  if(apI2cClient != NULL)
  {
    lNumBytes = (int32_t)i2c_master_send(apI2cClient, (char *)lBuf, 2);
    
    if (lNumBytes < (int32_t)0) 
    {
      lRetPrint = pr_err("%s:write reg error:reg=%x,val=%x\n",
        __func__, aRegAddr, aVal);
      lRetVal = CAM_DRV_FAILURE;
    }
  } // if pointers not NULL
  else
  {
    lRetPrint = pr_err("%s: NULL parameter.\n", __func__);
    lRetVal = CAM_DRV_FAILURE;
  } // else from if pointers not NULL

  return lRetVal;
} // RegWrite8()

//*****************************************************************************

static int32_t LinuxI2cRead8(const struct i2c_client *const apI2cClient, 
                             const uint8_t aRegAddr, 
                             uint8_t *const apVal)
{
  int32_t lRetVal = CAM_DRV_SUCCESS;
  int lRetPrint = 0;
  
  if((apVal != NULL) && (apI2cClient != NULL))
  {
    *apVal = 0;
    if (1 != i2c_master_send(apI2cClient, (const char *)&aRegAddr, 1)) 
    {
      lRetPrint = pr_err("%s:write reg error:reg=%x\n",
          __func__, aRegAddr);
      lRetVal = CAM_DRV_FAILURE;
    }
    else if (1 != i2c_master_recv(apI2cClient, apVal, 1))
    {
      lRetPrint = pr_err("%s:read reg error:reg=%x,val=%x\n",
          __func__, aRegAddr, *apVal);
      lRetVal = CAM_DRV_FAILURE;
    }
	else
    {
      lRetVal = CAM_DRV_SUCCESS;
    }
  } // if pointers not NULL
  else
  {
    lRetPrint = pr_err("%s: NULL parameter.\n", __func__);
    lRetVal = CAM_DRV_FAILURE;
  } // else from if pointers not NULL
  
  
  return lRetVal;
} //RegRead8()

//*****************************************************************************

static int32_t LinuxI2cWrite16(const struct i2c_client *const apI2cClient, 
                               const uint16_t aRegAddr, 
                               const uint8_t aVal)
{
  uint8_t  lBuf[3]   = {0};
  int32_t lNumBytes = 0;
  int32_t lRetVal   = CAM_DRV_SUCCESS;
  int lRetPrint = 0;

  lBuf[0] = (uint8_t)(aRegAddr >> 8);
  lBuf[1] = (uint8_t)aRegAddr;
  lBuf[2] = aVal;
  
  if(apI2cClient != NULL)
  {
    lNumBytes = (int32_t)i2c_master_send(apI2cClient, (char *)lBuf, 3);
    
    if (lNumBytes < (int32_t)0) 
    {
      lRetPrint = pr_err("%s:write reg error:reg=%x,val=%x\n",
        __func__, aRegAddr, aVal);
      lRetVal = CAM_DRV_FAILURE;
    }
  } // if pointers not NULL
  else
  {
    lRetPrint = pr_err("%s: NULL parameter.\n", __func__);
    lRetVal = CAM_DRV_FAILURE;
  } // else from if pointers not NULL

  return lRetVal;
} // RegWrite16()

//*****************************************************************************

static int32_t LinuxI2cRead16(const struct i2c_client *const apI2cClient, 
                              const uint16_t aRegAddr, 
                              uint8_t *const apVal)
{
  int32_t lRetVal = CAM_DRV_SUCCESS;
  uint8_t  lBuf[2] = {0};
  const char * chBuf = (char *)lBuf;
  int lRetPrint = 0;

  lBuf[0] = (uint8_t)(aRegAddr >> 8);
  lBuf[1] = (uint8_t)aRegAddr;
  
  if((apVal != NULL) && (apI2cClient != NULL))
  {
    *apVal = 0;
    if (2 != i2c_master_send(apI2cClient, chBuf, 2)) 
    {
      lRetPrint = pr_err("%s:write reg error:reg=%x\n",
          __func__, aRegAddr);
      lRetVal = CAM_DRV_FAILURE;
    }
    else if (1 != i2c_master_recv(apI2cClient, apVal, 1))
    {
      lRetPrint = pr_err("%s:read reg error:reg=%x,val=%x\n",
          __func__, aRegAddr, *apVal);
      lRetVal = CAM_DRV_FAILURE;
    }
	else
    {
      lRetVal = CAM_DRV_SUCCESS;
    }
  } // if pointers not NULL
  else
  {
    lRetPrint = pr_err("%s: NULL parameter.\n", __func__);
    lRetVal = CAM_DRV_FAILURE;
  } // else from if pointers not NULL
  
  
  return lRetVal;
} //RegRead16()

//*****************************************************************************

#endif // ifdef __KERNEL__

/*EOF*/