/*****************************************************************************
 *
 * Freescale Confidential Proprietary
 *
 * Copyright (c) 2015 Freescale Semiconductor;
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
 * \file     ov10635_user.cpp
 * \brief    User-space library calls to access Ov10635 functionality.
 * \author   Tomas Babinec
 * \version  0.1
 * \date     15-March-2016
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
#endif // else from #ifndef __STANDALONE__

#define VDB_LOG_NOTE_OFF

#include "ov10635_config_viu.h"
#include "isp_cam_generic.h"
#include "cam_ioctl.h"
#include "isp_cam_ov10635.h"
#include "vdb_log.h"

//****************************************************************************
// consts
//****************************************************************************

#define OV10635_COMPATIBLE_STRING "s32v-ov10635"

#define OV10635_PID_ADDR       ((uint16_t)0x300A)
#define OV10635_VER_ADDR       ((uint16_t)0x300B)
#define OV10635_GATE_CTRL_ADDR ((uint16_t)0x3012)
#define OV10635_SUBVER_ID_ADDR ((uint16_t)0x302A)

//****************************************************************************
// types
//****************************************************************************

//****************************************************************************
// global variables
//****************************************************************************
#ifndef __STANDALONE__
  extern int32_t gCamFD;   ///< cam driver file descriptor
#endif //ifndef __STANDALONE__

//*****************************************************************************
// local data
//*****************************************************************************

static CamI2cClient_t spClients[2] = {0}; // initialize to invalid
static I2C_BUS_IDX    sI2cBus[]    = {I2C_BUS_IDX_1, I2C_BUS_IDX_0};

//*****************************************************************************
// functions definitions
//*****************************************************************************

CAM_LIB_RESULT OV10635_Open(const VIU_IDX aViuIdx)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;
  CamI2cCfg_t    lCamCfg;
  
  if(CAM_Open() == CAM_LIB_SUCCESS)
  {
    if(spClients[aViuIdx] == 0)
    {
      lCamCfg.mI2cBusIdx    = sI2cBus[aViuIdx];
      lCamCfg.mI2cAddr      = (uint8_t)I2C_OV10635_CAM_ADDR; 
      lCamCfg.mRegAddrWidth = I2C_REG_ADDR_WIDTH_16;

      memcpy(lCamCfg.mpCompatible,
              OV10635_COMPATIBLE_STRING, 
              strlen(OV10635_COMPATIBLE_STRING));


      spClients[aViuIdx] = CAM_ClientAdd(lCamCfg);
    } // if not created yet
    else
    {
      VDB_LOG_ERROR("Ov10635 is already registered on VIU idx %u.\n",
                    (uint32_t)aViuIdx);
      lRet = CAM_LIB_FAILURE;
    } // else if not created yet

    if(spClients[aViuIdx] != 0)
    {
      uint8_t   lSubVer;
      uint8_t   lModelId;
      uint8_t   lVer;
      uint8_t   lGate;
      
      CAM_ReadSingle( spClients[aViuIdx], 
                      OV10635_SUBVER_ID_ADDR, 
                      lSubVer);
      
      CAM_ReadSingle( spClients[aViuIdx], 
                      OV10635_PID_ADDR, 
                      lModelId);
      
      CAM_ReadSingle( spClients[aViuIdx], 
                      OV10635_VER_ADDR, 
                      lVer);
      
      CAM_ReadSingle( spClients[aViuIdx], 
                      OV10635_GATE_CTRL_ADDR, 
                      lGate);
              
      printf("Ov10635 on VIU %u model PID 0x%x, model VER 0x%x, model SUBVER 0x%x, gate ctrl 0x%x\n",
              aViuIdx,
              lModelId, 
              lVer, 
              lSubVer,
              lGate
            );
    } // if client added successfully
    else
    {
      VDB_LOG_ERROR(
        "Failed to add Sony camera client: VIU idx %u, address 0x%x.\n",
        aViuIdx,
        lCamCfg.mI2cAddr
      );
      lRet = CAM_LIB_FAILURE;
    } // else if client added successfully
  } // if generic camera driver ready
  else
  {
    VDB_LOG_ERROR("Failed to open generic camera driver.\n");
    lRet = CAM_LIB_FAILURE;
  } // else from if generic camera driver ready
  
  return lRet;
} // OV10635_Open()

//*****************************************************************************

CAM_LIB_RESULT OV10635_Close(const VIU_IDX aViuIdx)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;
  
  if(spClients[aViuIdx] != 0)
  {    
    if(CAM_ClientRem(spClients[aViuIdx]) != CAM_LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to remove the camera client.\n");
      lRet = CAM_LIB_FAILURE;
    } // if ClientRem() failed
    else
    {
      spClients[aViuIdx] = 0;
    } // else from if CLientRem() failed
  } // if camera client attached
  else
  {
    VDB_LOG_ERROR("No such client connected.\n");
    lRet = CAM_LIB_FAILURE;
  } // else from if camera client attached
  return lRet;
} // OV10635_Close()

//*****************************************************************************

CAM_LIB_RESULT OV10635_RegConfig(const VIU_IDX aViuIdx)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;

  if(spClients[aViuIdx] != 0)
  { 
    uint32_t i         = (uint32_t)0;
    uint8_t *lpOvTable = Ov10635Viu_Table;
    uint32_t lOvTabSz  = (uint32_t)sizeof(Ov10635Viu_Table);

    // *** basic configuration ***
    for(i = (uint32_t)0; i < lOvTabSz; i = i + (uint32_t)3)
    {
      CAM_WriteSingle(spClients[aViuIdx],
                     (uint16_t)((uint16_t)lpOvTable[i] << 8)
                                        | (uint16_t)lpOvTable[i + (uint32_t)1], 
                     lpOvTable[i + (uint32_t)2]);
    }// for all table entries 
  } // if camera client attached
  else
  {
    VDB_LOG_ERROR("No such client connected.\n");
    lRet = CAM_LIB_FAILURE;
  } // else from if camera client attached
  
  return lRet;
} // OV10635_RegConfig()

//*****************************************************************************

CAM_LIB_RESULT OV10635_DataEnable(const VIU_IDX aViuIdx)
{
  CAM_LIB_RESULT lRet = (CAM_LIB_RESULT)aViuIdx;

  ///NOTE: nothing to be done
  lRet = CAM_LIB_SUCCESS;
  
  return lRet;
} // OV10635_CsiEnable(enum VIU_IDX aViuIdx)

//*****************************************************************************

CAM_LIB_RESULT OV10635_DataDisable(const VIU_IDX aViuIdx)
{
  CAM_LIB_RESULT lRet = (CAM_LIB_RESULT)aViuIdx;

  ///NOTE: nothing to be done
  lRet = CAM_LIB_SUCCESS;
  
  return lRet;
} // OV10635_CsiDisable(enum VIU_IDX aViuIdx)

//***************************************************************************

/*EOF*/
