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
 * \file     ov10640_user.cpp
 * \brief    User-space library calls to access Ov10640 functionality.
 * \author   Tomas Babinec
 * \version  0.1
 * \date     12-December-2015
 * \note
 ****************************************************************************/
#include "s32vs234.h"
#include <stdint.h>
#include <cstring>

#ifndef __STANDALONE__
  #include <fcntl.h>        // open 
  #include <unistd.h>       // exit 
  #include <sys/ioctl.h>    // ioctl 
  #include <sys/mman.h>     // mmap
#else
  #include "ov10640_func.h"
#endif // else from #ifndef __STANDALONE__

#define VDB_LOG_NOTE_OFF

#include "ov10640_config_mipi.h"
#include "ov10640_config_viu.h"

#include "isp_cam_generic.h"
#include "cam_ioctl.h"
#include "isp_cam_ov10640.h"
#include "vdb_log.h"

//****************************************************************************
// consts
//****************************************************************************

#define OV10640_COMPATIBLE_STRING "s32v-ov10640"

#define OV10640_CHIP_ID_H_ADDR 0x300A  ///< chip id High byte
#define OV10640_CHIP_ID_L_ADDR 0x300B  ///< chip id Low byte

#define OV10640_SW_CTRL1_ADDR 0x3012  
#define OV10640_SW_CTRL1_STDB 0x0     ///< software standby
#define OV10640_SW_CTRL1_STRM 0x1     ///< streaming

#define OV10640_SW_CTRL2_ADDR 0x3013  ///< bit 0 = software reset

#define OV10640_DVP_H_SIZE_H_ADDR 0x307C  ///< line length high byte
#define OV10640_DVP_H_SIZE_L_ADDR 0x307D
#define OV10640_DVP_V_SIZE_H_ADDR 0x307E  ///< frame length high byte
#define OV10640_DVP_V_SIZE_L_ADDR 0x307F

#define OV10640_HTS_H_ADDR 0x3080  ///< line length high byte
#define OV10640_HTS_L_ADDR 0x3081  ///< line length low byte
#define OV10640_VTS_H_ADDR 0x3082  ///< frame length high byte
#define OV10640_VTS_L_ADDR 0x3083  ///< frame length low byte

#define OV10640_READ_MODE_ADDR 0x3090
#define OV10640_READ_MODE_VR   0x4
#define OV10640_READ_MODE_HR   0x2

#define OV10640_READ_CTRL_ADDR 0x3091
#define OV10640_READ_CTRL_SHOW_STAT 0x8  ///< shows statistics rows
#define OV10640_READ_CTRL_SHOW_REGS 0x4  ///< shows register rows

#define OV10640_REAL_GAIN_L_H_ADDR  0x3105  ///< Long real gain
#define OV10640_REAL_GAIN_L_L_ADDR  0x3106  ///< Long real gain
#define OV10640_REAL_GAIN_S_H_ADDR  0x3107  ///< short real gain
#define OV10640_REAL_GAIN_S_L_ADDR  0x3108  ///< short real gain
#define OV10640_REAL_GAIN_VS_H_ADDR 0x3109  ///< veryshort real gain
#define OV10640_REAL_GAIN_VS_L_ADDR 0x310a  ///< veryshort real gain

#define OV10640_RATIO_LS_H_ADDR  0x310b  ///< long/short ratio
#define OV10640_RATIO_LS_L_ADDR  0x310c  ///< long/short ratio
#define OV10640_RATIO_LVS_H_ADDR 0x310d  ///< long/veryshort ratio
#define OV10640_RATIO_LVS_L_ADDR 0x310e  ///< long/veryshort ratio
#define OV10640_RATIO_SVS_H_ADDR 0x310f  ///< short/veryshort ratio
#define OV10640_RATIO_SVS_L_ADDR 0x3110  ///< short/veryshort ratio

#define OV10640_D_EV_L_3_ADDR 0x3111  ///< Longest EV byte 3
#define OV10640_D_EV_L_2_ADDR 0x3112  ///< Longest EV byte 2
#define OV10640_D_EV_L_1_ADDR 0x3113  ///< Longest EV byte 1
#define OV10640_D_EV_L_0_ADDR 0x3114  ///< Longest EV byte 0

#define OV10640_D_EV_S_3_ADDR 0x3115  ///< Shortest EV byte 3
#define OV10640_D_EV_S_2_ADDR 0x3116  ///< Shortest EV byte 2
#define OV10640_D_EV_S_1_ADDR 0x3117  ///< Shortest EV byte 1
#define OV10640_D_EV_S_0_ADDR 0x3118  ///< Shortest EV byte 0

// long channel gain
#define OV10640_R_R_GAIN_L_I_H_ADDR  0x31c3
#define OV10640_R_R_GAIN_L_I_L_ADDR  0x31c4
#define OV10640_R_GR_GAIN_L_I_H_ADDR 0x31c5
#define OV10640_R_GR_GAIN_L_I_L_ADDR 0x31c6
#define OV10640_R_GB_GAIN_L_I_H_ADDR 0x31c7
#define OV10640_R_GB_GAIN_L_I_L_ADDR 0x31c8
#define OV10640_R_B_GAIN_L_I_H_ADDR  0x31c9
#define OV10640_R_B_GAIN_L_I_L_ADDR  0x31ca
// short channel gain
#define OV10640_R_R_GAIN_S_I_H_ADDR  0x31cb
#define OV10640_R_R_GAIN_S_I_L_ADDR  0x31cc
#define OV10640_R_GR_GAIN_S_I_H_ADDR 0x31cd
#define OV10640_R_GR_GAIN_S_I_L_ADDR 0x31ce
#define OV10640_R_GB_GAIN_S_I_H_ADDR 0x31cf
#define OV10640_R_GB_GAIN_S_I_L_ADDR 0x31d0
#define OV10640_R_B_GAIN_S_I_H_ADDR  0x31d1
#define OV10640_R_B_GAIN_S_I_L_ADDR  0x31d2
// very short channel gain
#define OV10640_R_R_GAIN_VS_I_H_ADDR  0x31d3
#define OV10640_R_R_GAIN_VS_I_L_ADDR  0x31d4
#define OV10640_R_GR_GAIN_VS_I_H_ADDR 0x31d5
#define OV10640_R_GR_GAIN_VS_I_L_ADDR 0x31d6
#define OV10640_R_GB_GAIN_VS_I_H_ADDR 0x31d7
#define OV10640_R_GB_GAIN_VS_I_L_ADDR 0x31d8
#define OV10640_R_B_GAIN_VS_I_H_ADDR  0x31d9
#define OV10640_R_B_GAIN_VS_I_L_ADDR  0x31da

// black level pedestal
#define OV10640_BLC_TARGET_L_H_ADDR   0x30C3
#define OV10640_BLC_TARGET_L_L_ADDR   0x30C4
#define OV10640_BLC_TARGET_S_H_ADDR   0x30C5
#define OV10640_BLC_TARGET_S_L_ADDR   0x30C6
#define OV10640_BLC_TARGET_VS_H_ADDR  0x30C7
#define OV10640_BLC_TARGET_VS_L_ADDR  0x30C8

// Exposure Time
#define OV10640_EXPO_L_H_ADDR   0x30E6
#define OV10640_EXPO_L_L_ADDR   0x30E7
#define OV10640_EXPO_S_H_ADDR   0x30E8
#define OV10640_EXPO_S_L_ADDR   0x30E9
#define OV10640_EXPO_VS_ADDR    0x30EA        // last 5bits means fraction

#define OV10640_CG_AGAIN_ADDR   0x30EB
#define OV10640_DGAIN_L_H_ADDR  0x30EC
#define OV10640_DGAIN_L_L_ADDR  0x30ED
#define OV10640_DGAIN_S_H_ADDR  0x30EE
#define OV10640_DGAIN_S_L_ADDR  0x30EF
#define OV10640_DGAIN_VS_H_ADDR 0x30F0
#define OV10640_DGAIN_VS_L_ADDR 0x30F1

// Short exposure parameters limits and threshold
#define OV10640_DIGITAL_GAIN_S_MIN      576   //0x300
#define OV10640_DIGITAL_GAIN_S_MAX      0x800 //??
#define OV10640_EXPOSURE_TIME_S_MIN     4  //Has to be equal or below L Minimum 
// #define OV10640_EXPOSURE_TIME_S_MAX  771
// S exposure time is limited by the configured exposure time of L
#define OV10640_ANA_GAIN_S_MIN          0
#define OV10640_ANA_GAIN_S_MAX          0x3

// Long exposure parameters limits and threshold
#define OV10640_DIGITAL_GAIN_L_MIN      0x150 //minimum from camera AEC 0x15A
               // 0x15A = b1.01011010 = 1.35   Absolute minimum from the camera
#define OV10640_DIGITAL_GAIN_L_MAX      0x800 // (6.16b) Absolute maximum from 
       //the camera is 63.99 (0x2FFF) (Probably too high =>satyuration artifact)
#define OV10640_EXPOSURE_TIME_L_MIN     4
#define OV10640_EXPOSURE_TIME_L_MAX     727   //VTS = frame lines + blank lines
#define OV10640_ANA_GAIN_L_MIN          0
#define OV10640_ANA_GAIN_L_MAX          0x3

// Very Short exposure parameters limits and threshold
#define OV10640_DIGITAL_GAIN_VS_MIN     0x18F //from the camera AEC 0x15A
                 // 0x15A = b1.01011010 = 1.35 Absolute minimum from the camera
#define OV10640_DIGITAL_GAIN_VS_MAX     0x800 // (6.16b) Absolute maximum from
       // the camera is 63.99 (0x2FFF)(Probably too high =>saturation artifact)
#define OV10640_EXPOSURE_TIME_VS_MIN    0x10  // Absolute minimum from the camera is 0x10 (0.5 lines)	- 16
#define OV10640_EXPOSURE_TIME_VS_MAX    0x7F  // Absolute maximum from the camera is 0x7f (3.969 lines) - 127
#define OV10640_ANA_GAIN_VS_MIN         0x1   // Asolute maximum is 0x3 (*4)	
#define OV10640_ANA_GAIN_VS_MAX         0x1   // Asolute maximum is 0x3 (*4)	

#define OV10640_CONVERSION_GAIN_FACTOR  (2.57)
#define OV10640_UPDATE_FACTOR                   (0.5)

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
// local variables
//*****************************************************************************

static CamI2cClient_t spClients[2] = {0}; // initialize to invalid

// 12bit combined exp by default 
static uint32_t       sExpMode     = OV10640_EXP_12COM;  
static I2C_BUS_IDX    sViuI2cBus[] = {I2C_BUS_IDX_1, I2C_BUS_IDX_0};
const  float          gCGThreshold = 1.5; 	// Has to be >1.0 to work
const  float          gAGThreshold = 1.3; 	// Has to be >1.0 to work

//*****************************************************************************
// Local functions definitions
//*****************************************************************************
//*****************************************************************************
// functions definitions
//*****************************************************************************

// *** MipiCsi-2 ***

CAM_LIB_RESULT OV10640_Open(CSI_IDX aCsiIdx)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;
  CamI2cCfg_t    lCamCfg;
  
  if(CAM_Open() == CAM_LIB_SUCCESS)
  {
    if(spClients[aCsiIdx] == 0)
    {
      lCamCfg.mI2cBusIdx    = (I2C_BUS_IDX) aCsiIdx;
      lCamCfg.mI2cAddr      = I2C_OV10640_CAM_ADDR;
      lCamCfg.mRegAddrWidth = I2C_REG_ADDR_WIDTH_16;

      memcpy(lCamCfg.mpCompatible,
              OV10640_COMPATIBLE_STRING, 
              strlen(OV10640_COMPATIBLE_STRING));


      spClients[aCsiIdx] = CAM_ClientAdd(lCamCfg);
    } // if not created yet
    else
    {
      VDB_LOG_ERROR("Ov10640 is already registered on CSI idx %u.\n",
                    (uint32_t)aCsiIdx);
      lRet = CAM_LIB_FAILURE;
    } // else if not created yet

    if(spClients[aCsiIdx] != 0)
    {
      int32_t lRetVal = 0;
#ifndef __STANDALONE__   
      lRetVal = ioctl(gCamFD, IOCTL_OV10640_CONNECT, spClients[aCsiIdx]);
#else // #ifndef __STANDALONE__
      lRetVal = OV10640_DRV_Setup(spClients[aCsiIdx]);
#endif // else #ifndef __STANDALONE__
      if (lRetVal < 0) 
      {
        VDB_LOG_ERROR("Failed to connect the camera driver.\n");
        lRet = CAM_LIB_FAILURE;
      } // if ioctl returned failure
      else
      {
        uint32_t  lChipId   = 0;
        uint8_t  *lpChipIdH = ((uint8_t*)&lChipId + 1);
        uint8_t  *lpChipIdL = (uint8_t*)&lChipId;
        
        CAM_ReadSingle(spClients[aCsiIdx], OV10640_CHIP_ID_H_ADDR, *lpChipIdH);
        CAM_ReadSingle(spClients[aCsiIdx], OV10640_CHIP_ID_L_ADDR, *lpChipIdL);
                
        printf("Ov10640 chip ID 0x%x at csi_idx: %u\n", lChipId, aCsiIdx);  
      } // else from if ioctl returned failure
    } // if client added successfully
    else
    {
      VDB_LOG_ERROR(
        "Failed to add Sony camera client: CSI idx %u, address 0x%x.\n",
        aCsiIdx,
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
} // OV10640_Open()

//*****************************************************************************

CAM_LIB_RESULT OV10640_Close(CSI_IDX aCsiIdx)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;

  if(spClients[aCsiIdx] != 0)
  {
    int32_t lRetVal = 0;

#ifndef __STANDALONE__   
    lRetVal = ioctl(gCamFD, 
              IOCTL_OV10640_DISCONNECT, 
              spClients[aCsiIdx]);
#else // #ifndef __STANDALONE__
    lRetVal = OV10640_DRV_Close(spClients[aCsiIdx]); 
#endif // else #ifndef __STANDALONE__    
    if (lRetVal < 0) 
    {
      VDB_LOG_ERROR("Failed to disconnect the camera driver.\n");
      lRet = CAM_LIB_FAILURE;
    } // if returned failure  
    else
    {
      if(CAM_ClientRem(spClients[aCsiIdx]) != CAM_LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Failed to remove the camera client.\n");
        lRet = CAM_LIB_FAILURE;
      } // if ClientRem() failed
      else
      {
        spClients[aCsiIdx] = 0;
        lRet = CAM_Close();
      } // else from if CLientRem() failed
    } // else from if lRet
  } // if camera client attached
  else
  {
    VDB_LOG_ERROR("No such client connected.\n");
    lRet = CAM_LIB_FAILURE;
  } // else from if camera client attached
  return lRet;
  
} // OV10640_Close()

//*****************************************************************************

CAM_LIB_RESULT OV10640_RegConfig(CSI_IDX aCsiIdx)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;
  
  if(spClients[aCsiIdx] != 0)
  { 
    uint32_t i         = 0;
    uint8_t *lpOvTable = 
        Ov10640_MIPI_basic_configuration_2MIPI_lane_12bcombined;
    uint32_t lOvTabSz  = 
        sizeof(Ov10640_MIPI_basic_configuration_2MIPI_lane_12bcombined);

    //put camera to standby mode  
    OV10640_CsiDisable(aCsiIdx);

    // *** basic configuration ***
    for(i = 0; i < lOvTabSz; i = i + 3)
    {
      CAM_WriteSingle(spClients[aCsiIdx],
                      (lpOvTable[i] << 8) | lpOvTable[i + 1], 
                      lpOvTable[i + 2]);
    }// for all table entries 
    
    sExpMode = OV10640_EXP_12COM;
  } // if camera client attached
  else
  {
    VDB_LOG_ERROR("No such client connected.\n");
    lRet = CAM_LIB_FAILURE;
  } // else from if camera client attached
  
  return lRet;
} // CSI_RegConfig(CSI_IDX aCsiIdx)

//*****************************************************************************

CAM_LIB_RESULT OV10640_CsiEnable(CSI_IDX aCsiIdx)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;

  uint8_t lVal = OV10640_SW_CTRL1_STDB;
  
  // bring camera out of standby
  CAM_WriteSingle(spClients[aCsiIdx], 
                  OV10640_SW_CTRL1_ADDR, 
                  OV10640_SW_CTRL1_STRM);
  
  while(lVal != OV10640_SW_CTRL1_STRM)
  {
    CAM_ReadSingle(spClients[aCsiIdx], OV10640_SW_CTRL1_ADDR, lVal);
  } // wait for streaming
  
  return lRet;
} // OV10640_CsiEnable(CSI_IDX aCsiIdx)

//*****************************************************************************

CAM_LIB_RESULT OV10640_CsiDisable(enum CSI_IDX aCsiIdx)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;

  uint8_t lVal = OV10640_SW_CTRL1_STRM;
  // bring camera to standby
  CAM_WriteSingle(spClients[aCsiIdx], 
                  OV10640_SW_CTRL1_ADDR, 
                  OV10640_SW_CTRL1_STDB); 
  
  while(lVal != OV10640_SW_CTRL1_STDB)
  {
    CAM_ReadSingle(spClients[aCsiIdx], OV10640_SW_CTRL1_ADDR, lVal);
  } // wait for standby
  
  return lRet;
} // OV10640_CsiDisable(enum CSI_IDX aCsiIdx)

//***************************************************************************

// *** VIU ***

CAM_LIB_RESULT OV10640_Open(VIU_IDX aViuIdx)
{
  CAM_LIB_RESULT lRet       = CAM_LIB_SUCCESS;
  CamI2cCfg_t    lCamCfg;
  uint32_t       lClientIdx = sViuI2cBus[aViuIdx];
  
  if(CAM_Open() == CAM_LIB_SUCCESS)
  {
    if(spClients[lClientIdx] == 0)
    {
      lCamCfg.mI2cBusIdx    = (I2C_BUS_IDX) lClientIdx;
      lCamCfg.mI2cAddr      = I2C_OV10640_CAM_ADDR;
      lCamCfg.mRegAddrWidth = I2C_REG_ADDR_WIDTH_16;

      memcpy(lCamCfg.mpCompatible,
              OV10640_COMPATIBLE_STRING, 
              strlen(OV10640_COMPATIBLE_STRING));


      spClients[lClientIdx] = CAM_ClientAdd(lCamCfg);
    } // if not created yet
    else
    {
      VDB_LOG_ERROR("Ov10640 is already registered on VIU idx %u.\n",
                    (uint32_t)aViuIdx);
      lRet = CAM_LIB_FAILURE;
    } // else if not created yet

    if(spClients[lClientIdx] != 0)
    {
      uint32_t  lChipId   = 0;
      uint8_t  *lpChipIdH = ((uint8_t*)&lChipId + 1);
      uint8_t  *lpChipIdL = (uint8_t*)&lChipId;
      
      CAM_ReadSingle(spClients[lClientIdx], 
                      OV10640_CHIP_ID_H_ADDR, 
                      *lpChipIdH);
      CAM_ReadSingle(spClients[lClientIdx], 
                      OV10640_CHIP_ID_L_ADDR, 
                      *lpChipIdL);
              
      printf("Ov10640 chip ID 0x%x at VIU idx: %u\n", lChipId, aViuIdx);  
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
} // OV10640_Open()

//*****************************************************************************

CAM_LIB_RESULT OV10640_Close(VIU_IDX aViuIdx)
{
  CAM_LIB_RESULT lRet       = CAM_LIB_SUCCESS;
  uint32_t       lClientIdx = sViuI2cBus[aViuIdx];

  if(spClients[lClientIdx] != 0)
  {
    if(CAM_ClientRem(spClients[lClientIdx]) != CAM_LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to remove the camera client.\n");
      lRet = CAM_LIB_FAILURE;
    } // if ClientRem() failed
    else
    {
      spClients[lClientIdx] = 0;
    } // else from if CLientRem() failed
  } // if camera client attached
  else
  {
    VDB_LOG_ERROR("No such client connected.\n");
    lRet = CAM_LIB_FAILURE;
  } // else from if camera client attached
  return lRet;
  
} // OV10640_Close()

//*****************************************************************************

CAM_LIB_RESULT OV10640_RegConfig(VIU_IDX aViuIdx)
{
  CAM_LIB_RESULT lRet       = CAM_LIB_SUCCESS;
  uint32_t       lClientIdx = sViuI2cBus[aViuIdx];
  
  if(spClients[lClientIdx] != 0)
  { 
    uint32_t i         = 0;
    uint8_t *lpOvTable = Ov10640Viu_Table;
    uint32_t lOvTabSz  = sizeof(Ov10640Viu_Table);

    // *** basic configuration ***
    for(i = 0; i < lOvTabSz; i = i + 3)
    {
      CAM_WriteSingle(spClients[lClientIdx],
                      (lpOvTable[i] << 8) | lpOvTable[i + 1], 
                      lpOvTable[i + 2]);
    }// for all table entries 
  } // if camera client attached
  else
  {
    VDB_LOG_ERROR("No such client connected.\n");
    lRet = CAM_LIB_FAILURE;
  } // else from if camera client attached
  
  return lRet;
} // OV10640_RegConfig()

//*****************************************************************************

CAM_LIB_RESULT OV10640_DataEnable(VIU_IDX aViuIdx)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;

  uint8_t lVal = OV10640_SW_CTRL1_STDB;
  
  // bring camera out of standby
  CAM_WriteSingle(spClients[sViuI2cBus[aViuIdx]], 
                  OV10640_SW_CTRL1_ADDR, 
                  OV10640_SW_CTRL1_STRM);
  
  while(lVal != OV10640_SW_CTRL1_STRM)
  {
    CAM_ReadSingle(spClients[sViuI2cBus[aViuIdx]], OV10640_SW_CTRL1_ADDR, lVal);
  } // wait for streaming
  
  return lRet;
} // OV10640_DataEnable()

//*****************************************************************************

CAM_LIB_RESULT OV10640_DataDisable(VIU_IDX aViuIdx)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;

  uint8_t lVal = OV10640_SW_CTRL1_STRM;
  // bring camera to standby
  CAM_WriteSingle(spClients[sViuI2cBus[aViuIdx]], 
                  OV10640_SW_CTRL1_ADDR, 
                  OV10640_SW_CTRL1_STDB); 
  
  while(lVal != OV10640_SW_CTRL1_STDB)
  {
    CAM_ReadSingle(spClients[sViuI2cBus[aViuIdx]], OV10640_SW_CTRL1_ADDR, lVal);
  } // wait for standby
  
  return lRet;
} // OV10640_DataDisable()

//***************************************************************************

// *** GENERIC ***

CAM_LIB_RESULT OV10640_GeometrySet(OV10640_Geometry_t &arGeom)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;
  
  uint8_t lFlipMode = 0;
  uint8_t  lReadCtrl  = 0;
  uint16_t lDvpVsize  = 0;
  uint8_t *lpDvpVsize = (uint8_t*)&lDvpVsize;
  //setup correct mode
  if(arGeom.mExpMode != sExpMode)
  {
    OV10640_ModeSet(arGeom);
    sExpMode = arGeom.mExpMode;
  } // if mode differs
  
  CAM_ReadSingle(spClients[arGeom.mCsiIdx], OV10640_READ_MODE_ADDR, lFlipMode);
  // *** set flip mode ***
  if(arGeom.mVerFlip)
  {
    lFlipMode |= OV10640_READ_MODE_VR;
  } // vertical flip
  else
  {
    lFlipMode &= (~OV10640_READ_MODE_VR);
  } // else if vertical flip
  
  if(arGeom.mHorFlip)
  {
    lFlipMode |= OV10640_READ_MODE_HR;
  } // horizontal flip
  else
  {
    lFlipMode &= (~OV10640_READ_MODE_HR);
  } // else if horizontal flip
  
  CAM_WriteSingle(spClients[arGeom.mCsiIdx], OV10640_READ_MODE_ADDR, lFlipMode);
  
  // *** set read control (embedded data) ***
  CAM_ReadSingle(spClients[arGeom.mCsiIdx], 
                 OV10640_DVP_V_SIZE_L_ADDR, 
                 lpDvpVsize[0]);
  CAM_ReadSingle(spClients[arGeom.mCsiIdx], 
                 OV10640_DVP_V_SIZE_H_ADDR, 
                 lpDvpVsize[1]);
  CAM_ReadSingle(spClients[arGeom.mCsiIdx], 
                 OV10640_READ_CTRL_ADDR, 
                 lReadCtrl);
  if(arGeom.mEmbd & OV10640_EMBD_REGS)
  {
    lDvpVsize += (lReadCtrl & OV10640_READ_CTRL_SHOW_REGS)?0:2;
    lReadCtrl |= OV10640_READ_CTRL_SHOW_REGS;
  } // register output
  else
  {
    lDvpVsize -= (lReadCtrl & OV10640_READ_CTRL_SHOW_REGS)?2:0;
    lReadCtrl &= (~OV10640_READ_CTRL_SHOW_REGS);
  } // else if register output
  if(arGeom.mEmbd & OV10640_EMBD_STAT)
  {
    lDvpVsize += (lReadCtrl & OV10640_READ_CTRL_SHOW_STAT)?0:2;
    lReadCtrl |= OV10640_READ_CTRL_SHOW_STAT;
  } // statistics output
  else
  {
    lDvpVsize -= (lReadCtrl & OV10640_READ_CTRL_SHOW_STAT)?2:0;
    lReadCtrl &= (~OV10640_READ_CTRL_SHOW_STAT);
  } // else if statistics output     
  
  CAM_WriteSingle(spClients[arGeom.mCsiIdx], 
                  OV10640_DVP_V_SIZE_L_ADDR, 
                  lpDvpVsize[0]);
  CAM_WriteSingle(spClients[arGeom.mCsiIdx], 
                  OV10640_DVP_V_SIZE_H_ADDR, 
                  lpDvpVsize[1]);
  CAM_WriteSingle(spClients[arGeom.mCsiIdx], 
                  OV10640_READ_CTRL_ADDR, 
                  lReadCtrl);
  
  return lRet;
} // OV10640_GeometrySet()

//***************************************************************************

CAM_LIB_RESULT OV10640_ExposureGet(OV10640_Exposure_t &arExp)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;

  if(arExp.mConfigFlags == 0)
  {
    arExp.mConfigFlags = 0xff;
  } // if all should be read
  
  // real gain
  if(arExp.mConfigFlags | OV10640_EXP_FL_RG)
  {
    uint8_t *lpTmpH = ((uint8_t*)&(arExp.mRealGainLong) + 1);
    uint8_t *lpTmpL = (uint8_t*)&(arExp.mRealGainLong);
    
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_REAL_GAIN_L_H_ADDR,
                    *lpTmpH);
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_REAL_GAIN_L_L_ADDR,
                    *lpTmpL);
    
    lpTmpH = ((uint8_t*)&(arExp.mRealGainShort) + 1);
    lpTmpL = (uint8_t*)&(arExp.mRealGainShort);
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_REAL_GAIN_S_H_ADDR,
                    *lpTmpH);
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_REAL_GAIN_S_L_ADDR,
                    *lpTmpL);
    
    lpTmpH = ((uint8_t*)&(arExp.mRealGainVeryShort) + 1);
    lpTmpL = (uint8_t*)&(arExp.mRealGainVeryShort);
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_REAL_GAIN_VS_H_ADDR,
                    *lpTmpH);
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_REAL_GAIN_VS_L_ADDR,
                    *lpTmpL);
  } // if real gain should be read
  
  if(arExp.mConfigFlags | OV10640_EXP_FL_RATIO)
  {
    uint8_t *lpTmpH = ((uint8_t*)&(arExp.mLongShortRatio) + 1);
    uint8_t *lpTmpL = (uint8_t*)&(arExp.mLongShortRatio);
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_RATIO_LS_H_ADDR,
                    *lpTmpH);
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_RATIO_LS_L_ADDR,
                    *lpTmpL);
    
    lpTmpH = ((uint8_t*)&(arExp.mLongVeryShortRatio) + 1);
    lpTmpL = (uint8_t*)&(arExp.mLongVeryShortRatio);
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_RATIO_LVS_H_ADDR,
                    *lpTmpH);
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_RATIO_LVS_L_ADDR,
                    *lpTmpL);
    
    lpTmpH = ((uint8_t*)&(arExp.mShortVeryShortRatio) + 1);
    lpTmpL = (uint8_t*)&(arExp.mShortVeryShortRatio);
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_RATIO_SVS_H_ADDR,
                    *lpTmpH);
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_RATIO_SVS_L_ADDR,
                    *lpTmpL);
    
  } // if ratios should be read
  
  if(arExp.mConfigFlags | OV10640_EXP_FL_EV)
  {
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_D_EV_L_3_ADDR,
                    arExp.mLongestEvBytes[3]);
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_D_EV_L_2_ADDR,
                    arExp.mLongestEvBytes[2]);
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_D_EV_L_1_ADDR,
                    arExp.mLongestEvBytes[1]);
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_D_EV_L_0_ADDR,
                    arExp.mLongestEvBytes[0]);
    
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_D_EV_S_3_ADDR,
                    arExp.mShortestEvBytes[3]);
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_D_EV_S_2_ADDR,
                    arExp.mShortestEvBytes[2]);
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_D_EV_S_1_ADDR,
                    arExp.mShortestEvBytes[1]);
    CAM_ReadSingle(spClients[arExp.mCsiIdx],
                    OV10640_D_EV_S_0_ADDR,
                    arExp.mShortestEvBytes[0]);
  } // if EV to be set
  
  return lRet;
} // OV10640_ExposureGet()

//***************************************************************************

CAM_LIB_RESULT OV10640_ExposureSet(OV10640_Exposure_t &arExp)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;
 
  if(arExp.mConfigFlags == 0)
  {
    arExp.mConfigFlags = 0xff;
  } // if all should be read
  
  // real gain
  if(arExp.mConfigFlags | OV10640_EXP_FL_RG)
  {
    
  } // if real gain should be read
  
  if(arExp.mConfigFlags | OV10640_EXP_FL_RATIO)
  {
    
  } // if ratios should be read
  
  if(arExp.mConfigFlags | OV10640_EXP_FL_EV)
  {
    
  } // if EV to be set
  
  if(arExp.mConfigFlags | OV10640_EXP_FL_CNLG)
  {
    OV10640_ChGain(arExp);
  } // if EV to be set
  
  return lRet;
} // OV10640_ExposureSet()

//****************************************************************************

void OV10640_ModeSet(OV10640_Geometry_t &arGeom)
{ 
  uint32_t i = 0;

  //put camera to standby mode  
  OV10640_CsiDisable(arGeom.mCsiIdx);

  // *** channel gain default ***
  if((arGeom.mExpMode & OV10640_EXP_CNLG) == OV10640_EXP_CNLG)
  {
    for(i = 0; i < sizeof(OV10640_default_WB); i = i + 3)
    {
      CAM_WriteSingle(spClients[arGeom.mCsiIdx], 
                      (OV10640_default_WB[i] << 8) | OV10640_default_WB[i + 1], 
                      OV10640_default_WB[i + 2]);
    } // for all table entries 
  } // if channel gain requested

  // *** long exposure only ***
  if((arGeom.mExpMode & OV10640_EXP_LONG) == OV10640_EXP_LONG)
  {
    
    for(i = 0; i < sizeof(OV10640_change_to_long_exposure);i = i + 3)
    {
      CAM_WriteSingle(spClients[arGeom.mCsiIdx], 
                      (OV10640_change_to_long_exposure[i] << 8)| \
                      OV10640_change_to_long_exposure[i + 1], 
                      OV10640_change_to_long_exposure[i + 2]);
    } // for all table entries 
  } // if long exposure only
  else // *** short exposure only ***
  if((arGeom.mExpMode & OV10640_EXP_SHRT) == OV10640_EXP_SHRT)
  {
    
    for(i = 0; i < sizeof(OV10640_change_to_short_exposure);i = i + 3)
    {
      CAM_WriteSingle(spClients[arGeom.mCsiIdx], 
                      (OV10640_change_to_short_exposure[i] << 8)| \
                      OV10640_change_to_short_exposure[i + 1], 
                      OV10640_change_to_short_exposure[i + 2]);
    } // for all table entries 
  } // if short exposure only
  else // *** very short exposure only ***
  if((arGeom.mExpMode & OV10640_EXP_VSHRT) == OV10640_EXP_VSHRT)
  {
    
    for(i = 0; i < sizeof(OV10640_change_to_veryshort_exposure);i = i + 3)
    {
      CAM_WriteSingle(spClients[arGeom.mCsiIdx], 
                      (OV10640_change_to_veryshort_exposure[i] << 8)| \
                      OV10640_change_to_veryshort_exposure[i + 1], 
                      OV10640_change_to_veryshort_exposure[i + 2]);
    } // for all table entries 
  } // if short exposure only
  else // *** 3x12 ***
  if((arGeom.mExpMode & OV10640_EXP_3x12) == OV10640_EXP_3x12)
  {
    OV10640_Mode3x12(arGeom.mCsiIdx);
  } // if 3x12 exposure
  
  // *** Auto exposure control on ***
  if((arGeom.mExpMode & OV10640_EXP_AUTO) == OV10640_EXP_AUTO)
  {
    OV10640_AEC_WB_On(arGeom.mCsiIdx);
  } // if Auto exposure required
  
} // OV10640_ModeSet()

//****************************************************************************
void OV10640_ExposureTimeGet(OV10640_Exposure_t &arExp, OV10640_ExpTime_t aExp)
{
  uint8_t  lTemp;
  uint16_t lExpTime; 
  
  switch(aExp){
    case ExpTime_VShort:
      OV10640_RegRead(arExp.mCsiIdx, OV10640_EXPO_VS_ADDR, lTemp);
      lExpTime = (uint16_t)lTemp;
      break;

    case ExpTime_Short:
      OV10640_RegRead(arExp.mCsiIdx, OV10640_EXPO_S_H_ADDR, lTemp);
      lExpTime = ((uint16_t)lTemp) << 8;
      OV10640_RegRead(arExp.mCsiIdx, OV10640_EXPO_S_L_ADDR, lTemp);
      lExpTime += (uint16_t)lTemp;
      break;

    case ExpTime_Long:
      OV10640_RegRead(arExp.mCsiIdx, OV10640_EXPO_L_H_ADDR, lTemp);
      lExpTime = ((uint16_t)lTemp) << 8;
      OV10640_RegRead(arExp.mCsiIdx, OV10640_EXPO_L_L_ADDR, lTemp);
      lExpTime += (uint16_t)lTemp;
      break;

    default:
      /* Wrong value */
      break;
  }
  arExp.mExposureTime[aExp] = lExpTime;
  return;
}

//****************************************************************************
void OV10640_ExposureTimeSet(OV10640_Exposure_t &arExp, OV10640_ExpTime_t aExp)
{
  uint8_t  lTemp;
  uint16_t lExpTime = arExp.mExposureTime[aExp];
  
  switch(aExp)
  {
    case ExpTime_VShort:
      lTemp = (uint8_t)lExpTime;
      OV10640_RegWrite(arExp.mCsiIdx, OV10640_EXPO_VS_ADDR, lTemp);
      break;

    case ExpTime_Short:
      lTemp = (uint8_t)(lExpTime >> 8);
      OV10640_RegWrite(arExp.mCsiIdx, OV10640_EXPO_S_H_ADDR, lTemp);
      lTemp = (uint8_t)lExpTime;
      OV10640_RegWrite(arExp.mCsiIdx, OV10640_EXPO_S_L_ADDR, lTemp);
      break;

    case ExpTime_Long:
      lTemp = (uint8_t)(lExpTime >> 8);
      OV10640_RegWrite(arExp.mCsiIdx, OV10640_EXPO_L_H_ADDR, lTemp);
      lTemp = (uint8_t)lExpTime;
      OV10640_RegWrite(arExp.mCsiIdx, OV10640_EXPO_L_L_ADDR, lTemp);
      break;

    default:
      /* Wrong value */
      break;
  }
}

//****************************************************************************
void OV10640_ExposureDigitalGainGet(OV10640_Exposure_t &arExp,
                                        OV10640_ExpTime_t aExp)
{
  uint8_t  lTemp = 0;
  uint16_t lDigGain = 0; 

  switch(aExp)
  {
    case ExpTime_VShort:
      OV10640_RegRead(arExp.mCsiIdx, OV10640_DGAIN_VS_H_ADDR, lTemp);
      lDigGain = ((uint16_t)lTemp) << 8;
      OV10640_RegRead(arExp.mCsiIdx, OV10640_DGAIN_VS_L_ADDR, lTemp);
      lDigGain += (uint16_t)lTemp;
      break;

    case ExpTime_Short:
      OV10640_RegRead(arExp.mCsiIdx, OV10640_DGAIN_S_H_ADDR, lTemp);
      lDigGain = ((uint16_t)lTemp) << 8;
      OV10640_RegRead(arExp.mCsiIdx, OV10640_DGAIN_S_L_ADDR, lTemp);
      lDigGain += (uint16_t)lTemp;
      break;

    case ExpTime_Long:
      OV10640_RegRead(arExp.mCsiIdx, OV10640_DGAIN_L_H_ADDR, lTemp);
      lDigGain = ((uint16_t)lTemp) << 8;
      OV10640_RegRead(arExp.mCsiIdx, OV10640_DGAIN_L_L_ADDR, lTemp);
      lDigGain += (uint16_t)lTemp;
      break;

    default:
      /* Wrong value */
      break;
  }
  arExp.mDigitalGain[aExp] = lDigGain;
  return;
}

//****************************************************************************
void OV10640_ExposureDigitalGainSet ( OV10640_Exposure_t &arExp,
                                      OV10640_ExpTime_t aExp )
{
  uint8_t  gain;
  uint16_t exp_gain = arExp.mDigitalGain[aExp];

  switch(aExp)
  {
    case ExpTime_VShort:
      gain = (uint8_t)(exp_gain >> 8);
      OV10640_RegWrite(arExp.mCsiIdx, OV10640_DGAIN_VS_H_ADDR, gain);
      gain = (uint8_t)exp_gain;
      OV10640_RegWrite(arExp.mCsiIdx, OV10640_DGAIN_VS_L_ADDR, gain);
      break;
    
    case ExpTime_Short:
      gain = (uint8_t)(exp_gain >> 8);
      OV10640_RegWrite(arExp.mCsiIdx, OV10640_DGAIN_S_H_ADDR, gain);
      gain = (uint8_t)exp_gain;
      OV10640_RegWrite(arExp.mCsiIdx, OV10640_DGAIN_S_L_ADDR, gain);
      break;
    
    case ExpTime_Long:
      gain = (uint8_t)(exp_gain >> 8);
      OV10640_RegWrite(arExp.mCsiIdx, OV10640_DGAIN_L_H_ADDR, gain);
      gain = (uint8_t)exp_gain;
      OV10640_RegWrite(arExp.mCsiIdx, OV10640_DGAIN_L_L_ADDR, gain);
      break;
    
    default:
      /* Wrong value */
      break;
  }
}

//****************************************************************************
void OV10640_ExposureAnalogConversionGainGet(OV10640_Exposure_t &arExp)
{
  uint8_t lGain, lAnaGain, lConvGain;

  OV10640_RegRead(arExp.mCsiIdx, OV10640_CG_AGAIN_ADDR, lGain);

  lAnaGain  = (lGain >> 4) & 0x03;
  lConvGain = (lGain >> 7) & 0x01;
  arExp.mAnalogGain[(uint8_t)ExpTime_VShort] = lAnaGain; // x1, x2, x4 or x8
  arExp.mConversionGain[(uint8_t)ExpTime_VShort] = lConvGain;

  lAnaGain  = (lGain >> 2) & 0x03;
  lConvGain = (uint8_t)0;                   // S doesn't have a conversion gain
  arExp.mAnalogGain[(uint8_t)ExpTime_Short] = lAnaGain; // x1, x2, x4 or x8
  arExp.mConversionGain[(uint8_t)ExpTime_Short] = lConvGain;

  lAnaGain  = lGain & 0x03;
  lConvGain = (lGain >> 6) & 0x01;
  arExp.mAnalogGain[(uint8_t)ExpTime_Long] = lAnaGain; // x1, x2, x4 or x8
  arExp.mConversionGain[(uint8_t)ExpTime_Long] = lConvGain;
}

//****************************************************************************
void OV10640_ExposureAnalogConversionGainSet(OV10640_Exposure_t &arExp)
{
  uint8_t lGain = ((arExp.mConversionGain[(uint8_t)ExpTime_VShort]) << 7)
                + ((arExp.mConversionGain[(uint8_t)ExpTime_Long]) << 6) 
                + ((arExp.mAnalogGain[(uint8_t)ExpTime_VShort]) << 4)
                + ((arExp.mAnalogGain[(uint8_t)ExpTime_Short]) << 2)
                + arExp.mAnalogGain[(uint8_t)ExpTime_Long];

  OV10640_RegWrite(arExp.mCsiIdx, OV10640_CG_AGAIN_ADDR, lGain);
}

//****************************************************************************
void OV10640_ExposureAllExpGet( OV10640_Exposure_t &arExp,
                                OV10640_ExpTime_t aExp )
{
  OV10640_ExposureTimeGet(arExp, aExp);
  OV10640_ExposureAnalogConversionGainGet(arExp);
  OV10640_ExposureDigitalGainGet(arExp, aExp);
}

//****************************************************************************
void OV10640_ExposureAllExpSet( OV10640_Exposure_t &arExp,
                                OV10640_ExpTime_t aExp )
{
  OV10640_ExposureTimeSet(arExp, aExp);
//OV10640_ExposureAnalogConversionGainSet(arExp);
  OV10640_ExposureDigitalGainSet(arExp, aExp);
}

//***************************************************************************
void OV10640_ExposureTotal(OV10640_Exposure_t &arExp, OV10640_ExpTime_t aExp)
{
  float lCG;
  uint8_t lTimeDiv = 1;
  
  if(ExpTime_VShort == aExp)
  {
    lTimeDiv = 32;
  }
  
  if(0 != arExp.mConversionGain[aExp])
  {
    lCG = OV10640_CONVERSION_GAIN_FACTOR;
  }
  else
  {
    lCG = 1;
  }

  arExp.mExposure[aExp] = ((float)(arExp.mExposureTime[aExp])/lTimeDiv) * //8.5b
                           (float)(1 << arExp.mAnalogGain[aExp]) * 
                           lCG *
                           ((float)(arExp.mDigitalGain[aExp]) / 256);//6.8b value
}

//***************************************************************************
void OV10640_ExposureComputationInd(OV10640_Exposure_t &arExp,
                                        float arNewLExpTotal,
                                        float arNewVSExpTotal)
{
  float lNewLExposure, lNewSExposure, lNewExposure;
  float lFloatDigGain, lGain, lGainCG, lCG, lExpTime, lExpVSMaxTime;
  uint8_t lAnGain;
  uint16_t lDigGain, lExpCoef = 0;
  
  lNewLExposure = arNewLExpTotal;
  /* Maintain the ratio between S and L, 
  saturation of the new exposure based on min/max */
  if( arNewLExpTotal > arExp.mMaxExposure[(uint16_t)ExpTime_Long])
  {
    // Saturate L exposure to maximum
    lNewLExposure= arExp.mMaxExposure[(uint16_t)ExpTime_Long];
  }
  else if(arNewLExpTotal < arExp.mMinExposure[(uint16_t)ExpTime_Long])
  {
    // Saturate L exposure to minimum
    lNewLExposure= arExp.mMinExposure[(uint16_t)ExpTime_Long]; 
  }
  lNewSExposure = lNewLExposure / arExp.mRatioLSExp;//Calculate S exposure based on L/S ratio

  if(lNewSExposure > arExp.mMaxExposure[(uint16_t)ExpTime_Short])
  {
    // Saturate S exposure to maximum and recompute L exposure to keep ratio
    lNewSExposure= arExp.mMaxExposure[(uint16_t)ExpTime_Short];
    lNewLExposure = lNewSExposure*arExp.mRatioLSExp;
  }
  else if(lNewSExposure < arExp.mMinExposure[(uint16_t)ExpTime_Short])
  {
    // Saturate S exposure to minimum and recompute L exposure
    lNewSExposure= arExp.mMinExposure[(uint16_t)ExpTime_Short];
    lNewLExposure = lNewSExposure*arExp.mRatioLSExp;
  }


/******************* Compute L *******************/

  lNewExposure = lNewLExposure;

  lAnGain = arExp.mMinAnalogGain[(uint16_t)ExpTime_Long];
  lDigGain = arExp.mMinDigitalGain[(uint16_t)ExpTime_Long];
  lGain = (1 << lAnGain) * (float)(lDigGain)/256;
  lGainCG = lGain * OV10640_CONVERSION_GAIN_FACTOR;
  lCG = 1;
  
  if(arExp.mConversionGainEnable[(uint16_t)ExpTime_Long])
  {
    if(arExp.mConversionGain[(uint16_t)ExpTime_Long])
    { // Was already activated
      if(lNewExposure >= (lGainCG*arExp.mMaxExposureTime[(uint16_t)ExpTime_Long]))
      {
        lCG = OV10640_CONVERSION_GAIN_FACTOR;
        lGain = lGainCG;
      }
    }
    else
    { //Conversion gain was off need to pass threshold to be increased)
      if(lNewExposure >= 
          (gCGThreshold*lGainCG*arExp.mMaxExposureTime[(uint16_t)ExpTime_Long]))
      {
        lCG = OV10640_CONVERSION_GAIN_FACTOR;
        lGain = lGainCG;
      }
    }
  }

  while((lAnGain < arExp.mMaxAnalogGain[(uint16_t)ExpTime_Long]) &&
     (lNewExposure >= (2*lGain*arExp.mMaxExposureTime[(uint16_t)ExpTime_Long]))
      && (lAnGain< arExp.mAnalogGain[(uint16_t)ExpTime_Long]))
  {// Maximising exposure time is not enough to reach new exposure 
    // so analog gain can be increased up to previous analog gain
    lAnGain++;
    lGain = lGain * 2;
  }

  if((lAnGain < arExp.mMaxAnalogGain[(uint16_t)ExpTime_Long]) &&
    (lNewExposure >=
         (gAGThreshold*2*lGain*arExp.mMaxExposureTime[(uint16_t)ExpTime_Long])))
  { //Increase analog gain more if above threshold (Hysteresis)
    lAnGain++;
    lGain = lGain*2;
  }
  lExpTime = lNewExposure/lGain;

  if(lExpTime > arExp.mMaxExposureTime[(uint16_t)ExpTime_Long]) 
  {// Maximise exposure time but also need to increase digital gain
    lExpTime = arExp.mMaxExposureTime[(uint16_t)ExpTime_Long];
    lFloatDigGain = lNewExposure / 
           (arExp.mMaxExposureTime[(uint16_t)ExpTime_Long] * (1<<lAnGain)*lCG);
    lDigGain = (uint16_t)(lFloatDigGain*256 + 0.5);
  }
  else if(arExp.mDeflickeringEnable) //Deflickering
  {
    lExpCoef = (uint16_t)(lExpTime / arExp.mFlickerPeriod);
    if(lExpCoef<1)
    {
      lExpCoef = 1;
    }
    lExpTime = lExpCoef * arExp.mFlickerPeriod;
    lFloatDigGain = lNewExposure/ (lExpTime * (1 << lAnGain) * lCG); //Compensate with digital gain
    lDigGain = (uint16_t)(lFloatDigGain*256 + 0.5);
    arExp.mExpCoef[(uint16_t)ExpTime_Long] = lExpCoef;
  }
  
  if(lCG == 1)
  {
    arExp.mConversionGain[(uint16_t)ExpTime_Long] = 0;
  }
  else{ // == Conversion gain factor
    arExp.mConversionGain[(uint16_t)ExpTime_Long] = 1;
  }
  arExp.mAnalogGain[(uint16_t)ExpTime_Long] = lAnGain;
  arExp.mDigitalGain[(uint16_t)ExpTime_Long] = lDigGain;
  arExp.mExposureTime[(uint16_t)ExpTime_Long] = (uint16_t)(lExpTime + 0.5);

  OV10640_ExposureTotal(arExp, ExpTime_Long);


/******************* Compute S *******************/

  arExp.mMaxExposureTime[(uint16_t)ExpTime_Short] = 
                                  arExp.mExposureTime[(uint16_t)ExpTime_Long];
  //exposure time of S limited by exposure time of L
  lNewExposure = lNewSExposure;
  
  lAnGain = arExp.mMinAnalogGain[(uint16_t)ExpTime_Short];
  lDigGain = arExp.mMinDigitalGain[(uint16_t)ExpTime_Short];
  lGain = (1<<lAnGain) * (float)(lDigGain)/256;
  lGainCG = lGain * OV10640_CONVERSION_GAIN_FACTOR;
  lCG = 1;
  
  while((lAnGain < arExp.mMaxAnalogGain[(uint16_t)ExpTime_Short]) &&
    (lNewExposure >= (2*lGain*arExp.mMaxExposureTime[(uint16_t)ExpTime_Short]))
    && (lAnGain< arExp.mAnalogGain[(uint16_t)ExpTime_Short]))
  {// Maximising exposure time is not enough to reach new exposure 
    // so analog gain can be increased up to previous analog gain
    lAnGain++;
    lGain = lGain * 2;
  }
  if((lAnGain < arExp.mMaxAnalogGain[(uint16_t)ExpTime_Short]) &&
     (lNewExposure >= 
       (gAGThreshold*2*lGain*arExp.mMaxExposureTime[(uint16_t)ExpTime_Short])))
  { //Increase analog gain more if above threshold (Hysteresis)
    lAnGain++;
    lGain = lGain*2;
  }
  
  lExpTime = lNewExposure/lGain;
  if(lExpTime > arExp.mMaxExposureTime[(uint16_t)ExpTime_Short]) 
  {// Maximise exposure time but also need to increase digital gain
    lExpTime = arExp.mMaxExposureTime[(uint16_t)ExpTime_Short];
    lFloatDigGain = lNewExposure / 
          (arExp.mMaxExposureTime[(uint16_t)ExpTime_Short] * (1<<lAnGain)*lCG);
    lDigGain = (uint16_t)(lFloatDigGain*256 + 0.5);
  }
  else if(arExp.mDeflickeringEnable) //Deflickering
  {
    lExpCoef = (uint16_t)(lExpTime/arExp.mFlickerPeriod);
    if(lExpCoef<1)
   {
      lExpCoef = 1;
    }
    lExpTime = lExpCoef*arExp.mFlickerPeriod;
    lFloatDigGain = lNewExposure/ (lExpTime * (1<<lAnGain)*lCG); //Compensate with digital gain
    lDigGain = (uint16_t)(lFloatDigGain*256+0.5);
    arExp.mExpCoef[(uint16_t)ExpTime_Short] = lExpCoef;
  }
  
  if(lCG == 1)
  {
    arExp.mConversionGain[(uint16_t)ExpTime_Short] = 0;
  }
  else{ // == Conversion gain factor
    arExp.mConversionGain[(uint16_t)ExpTime_Short] = 1;
  }
  arExp.mAnalogGain[(uint16_t)ExpTime_Short] = lAnGain;
  arExp.mDigitalGain[(uint16_t)ExpTime_Short] = lDigGain;
  arExp.mExposureTime[(uint16_t)ExpTime_Short] = (uint16_t)(lExpTime + 0.5);

  OV10640_ExposureTotal(arExp, ExpTime_Short);

 
/******************* Compute VS *******************/

  lNewExposure = arNewVSExpTotal;
  lExpVSMaxTime = ((float)(arExp.mMaxExposureTime[(uint16_t)ExpTime_VShort])) / 32;
  if( arNewVSExpTotal > arExp.mMaxExposure[(uint16_t)ExpTime_VShort])
  {
    // Saturate VS exposure to maximum
    lNewExposure= arExp.mMaxExposure[(uint16_t)ExpTime_VShort];
  }
  else if(arNewVSExpTotal < arExp.mMinExposure[(uint16_t)ExpTime_VShort])
  {
    // Saturate VS exposure to minimum
    lNewExposure= arExp.mMinExposure[(uint16_t)ExpTime_VShort];
  }
  
  lAnGain = arExp.mMinAnalogGain[(uint16_t)ExpTime_VShort];
  lDigGain = arExp.mMinDigitalGain[(uint16_t)ExpTime_VShort];
  lGain = (1<<lAnGain) * (float)(lDigGain)/256;
  lGainCG = lGain * OV10640_CONVERSION_GAIN_FACTOR;
  lCG = 1;
  
  if(arExp.mConversionGainEnable[(uint16_t)ExpTime_VShort])
  {
    if(arExp.mConversionGain[(uint16_t)ExpTime_VShort])
    { // Was already activated
      if(lNewExposure >= (lGainCG*lExpVSMaxTime))
      {
        lCG = OV10640_CONVERSION_GAIN_FACTOR;
        lGain = lGainCG;
      }
    }
    else
    { //Conversion gain was off need to pass threshold to be increased)
      if(lNewExposure >= (gCGThreshold * lGainCG * lExpVSMaxTime))
      {
        lCG = OV10640_CONVERSION_GAIN_FACTOR;
        lGain = lGainCG;
      }
    }
  }

  while((lAnGain < arExp.mMaxAnalogGain[(uint16_t)ExpTime_VShort]) 
    && (lNewExposure >= (2 * lGain * lExpVSMaxTime))
    && (lAnGain< arExp.mAnalogGain[(uint16_t)ExpTime_VShort]))
  { // Maximising exposure time is not enough to reach new exposure 
    // so analog gain can be increased up to previous analog gain
    lAnGain++;
    lGain = lGain * 2;
  }
  if((lAnGain < arExp.mMaxAnalogGain[(uint16_t)ExpTime_VShort]) 
      && (lNewExposure >= (gAGThreshold*2*lGain*lExpVSMaxTime)))
  { //Increase analog gain more if above threshold (Hysteresis)
    lAnGain++;
    lGain = lGain*2;
  }
  
  lExpTime = lNewExposure/lGain;
  if(lExpTime > lExpVSMaxTime) 
  { // Maximise exposure time but also need to increase digital gain
    lExpTime = arExp.mMaxExposureTime[(uint16_t)ExpTime_VShort];
    lFloatDigGain = lNewExposure / (lExpVSMaxTime * (1 << lAnGain) * lCG);
    lDigGain = (uint16_t)(lFloatDigGain*256 + 0.5);
  }
  else
  {
    lExpTime = 32 * lExpTime;	//VS format
  }
  
  if(lCG == 1)
  {
    arExp.mConversionGain[(uint16_t)ExpTime_VShort] = 0;
  }
  else{ // == Conversion gain factor
    arExp.mConversionGain[(uint16_t)ExpTime_VShort] = 1;
  }
  arExp.mAnalogGain[(uint16_t)ExpTime_VShort] = lAnGain;
  arExp.mDigitalGain[(uint16_t)ExpTime_VShort] = lDigGain;
  arExp.mExposureTime[(uint16_t)ExpTime_VShort] = (uint16_t)(lExpTime + 0.5);

  OV10640_ExposureTotal(arExp, ExpTime_VShort);
}

//****************************************************************************
float OV10640_ExposureCorrection( uint32_t *apHist, 
                               uint32_t aPixCntOverExp, 
                               uint8_t  aBinOverExpLevel)
{
  uint16_t lHistIndex = 0;
  uint16_t lHistBin = 0;
  uint32_t lSumL = 0;
  uint32_t lSumH = 0;
  float    lLevel = 0;
  float    lUpdateExp;

  while (lHistIndex<16)
  {  // Long exposure
    lSumL = lSumH;
    lSumH += apHist[lHistBin];
    lHistIndex++;
    lHistBin++;

    // found limit now get the level
    if (lSumH > aPixCntOverExp)
    {
      uint32_t lDiffHist; 
      if(1 == lHistBin)
      { //Pixel target reached in the first bin
        lDiffHist = lSumH;
      }
      else
	  {
        lDiffHist = (lSumH - lSumL);
      }
        
      uint32_t lDiffTarget = lSumH - aPixCntOverExp;
      lLevel = lHistIndex;
      if (0 != lDiffHist)
      {
        lLevel = lLevel - (float)lDiffTarget/(float)lDiffHist;
      }
      lUpdateExp = (((float)aBinOverExpLevel)/lLevel -1.0) 
	                      * OV10640_UPDATE_FACTOR + 1.0;
      break;
    }
  }
  return lUpdateExp;
} // OV10640_ExposureCorrection

//****************************************************************************

void OV10640_Mode3x12(CSI_IDX aCsiIdx)
{ 
  uint32_t i = 0;

  // *** 3x12 enable ***
  for(i = 0; i < sizeof(OV10640_3x12); i = i + 3)
  {
   CAM_WriteSingle(spClients[aCsiIdx], 
                    (OV10640_3x12[i] << 8) | OV10640_3x12[i + 1], 
                    OV10640_3x12[i + 2]);
  } // for all table entries 
} // OV10640_Mode3x12()

//****************************************************************************

void OV10640_AEC_WB_On(CSI_IDX aCsiIdx)
{ 
  uint32_t i = 0;
 
  // *** Auto exposure control on ***
  for(i = 0; i < sizeof(Ov10640_AEC_ON); i = i + 3)
  {
   CAM_WriteSingle(spClients[aCsiIdx], 
                    (Ov10640_AEC_ON[i] << 8) | Ov10640_AEC_ON[i + 1], 
                    Ov10640_AEC_ON[i + 2]);
  } // for all table entries 
} // OV10640_AEC_WB_On()

/****************************************************************************

void OV10640_Fps(enum CSI_IDX aCsiIdx, uint32_t aFps)
{ 
  uint32_t HTS = 0;

  HTS = ((30 *2800) / aFps + 1);
  if (HTS > 8191) 
  {
    HTS = 8191;
  }
 CAM_WriteSingle(spClients[aCsiIdx], OV10640_HTS_H_ADDR,(HTS >>8) & 0xff);
 CAM_WriteSingle(spClients[aCsiIdx], OV10640_HTS_L_ADDR, HTS & 0xff);
} // OV10640_Fps()
*/

//****************************************************************************

void OV10640_ChGain(OV10640_Exposure_t &arExp)
{ 
  CamI2cClient_t lClient = arExp.mClient;
  if(lClient == 0)
  {
   lClient = spClients[arExp.mCsiIdx];
  } // if the client not specified directly
    
  // long
  CAM_WriteSingle(lClient, 
                  OV10640_R_R_GAIN_L_I_H_ADDR,
                  (arExp.mpLongChnlGain[0] >>8) & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_R_GAIN_L_I_L_ADDR, 
                  arExp.mpLongChnlGain[0]      & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_GR_GAIN_L_I_H_ADDR,
                  (arExp.mpLongChnlGain[1] >>8) & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_GR_GAIN_L_I_L_ADDR, 
                  arExp.mpLongChnlGain[1]      & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_GB_GAIN_L_I_H_ADDR,
                  (arExp.mpLongChnlGain[2] >>8) & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_GB_GAIN_L_I_L_ADDR, 
                  arExp.mpLongChnlGain[2]       & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_B_GAIN_L_I_H_ADDR,
                  (arExp.mpLongChnlGain[3] >>8) & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_B_GAIN_L_I_L_ADDR, 
                  arExp.mpLongChnlGain[3]      & 0xff);

  // short
  CAM_WriteSingle(lClient, 
                  OV10640_R_R_GAIN_S_I_H_ADDR, 
                  (arExp.mpShortChnlGain[0] >>8) & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_R_GAIN_S_I_L_ADDR, 
                  arExp.mpShortChnlGain[0]       & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_GR_GAIN_S_I_H_ADDR,
                  (arExp.mpShortChnlGain[1] >>8) & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_GR_GAIN_S_I_L_ADDR, 
                  arExp.mpShortChnlGain[1]      & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_GB_GAIN_S_I_H_ADDR,
                  (arExp.mpShortChnlGain[2] >>8) & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_GB_GAIN_S_I_L_ADDR, 
                  arExp.mpShortChnlGain[2]      & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_B_GAIN_S_I_H_ADDR,
                  (arExp.mpShortChnlGain[3] >>8) & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_B_GAIN_S_I_L_ADDR, 
                  arExp.mpShortChnlGain[3]      & 0xff);

  //very short
  CAM_WriteSingle(lClient, 
                  OV10640_R_R_GAIN_VS_I_H_ADDR,
                  (arExp.mpVeryShortChnlGain[0] >>8) & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_R_GAIN_VS_I_L_ADDR, 
                  arExp.mpVeryShortChnlGain[0]      & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_GR_GAIN_VS_I_H_ADDR,
                  (arExp.mpVeryShortChnlGain[1] >>8) & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_GR_GAIN_VS_I_L_ADDR, 
                  arExp.mpVeryShortChnlGain[1]      & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_GB_GAIN_VS_I_H_ADDR,
                  (arExp.mpVeryShortChnlGain[2] >>8) & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_GB_GAIN_VS_I_L_ADDR, 
                  arExp.mpVeryShortChnlGain[2]      & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_B_GAIN_VS_I_H_ADDR,
                  (arExp.mpVeryShortChnlGain[3] >>8) & 0xff);
  CAM_WriteSingle(lClient, 
                  OV10640_R_B_GAIN_VS_I_L_ADDR, 
                  arExp.mpVeryShortChnlGain[3]      & 0xff);
} // OV10640_ChGain()

//*****************************************************************************

CAM_LIB_RESULT OV10640_RegWrite(CSI_IDX  aCsiIdx, 
                                uint16_t aRegAddr,
                                uint8_t  aVal
                                )
{
  return CAM_WriteSingle(spClients[aCsiIdx], aRegAddr, aVal);
} // SONY_RegWrite

//*****************************************************************************

CAM_LIB_RESULT OV10640_RegRead(CSI_IDX  aCsiIdx, 
                               uint16_t aRegAddr,
                               uint8_t &arVal
                               )
{
  return CAM_ReadSingle(spClients[aCsiIdx], aRegAddr, arVal);
} // SONY_RegWrite


//*****************************************************************************

/*EOF*/
