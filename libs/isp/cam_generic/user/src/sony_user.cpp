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
 * \file     sony_user.cpp
 * \brief    User-space library calls to access Sony functionality.
 * \author   Tomas Babinec
 * \version  0.1
 * \date     24-november-2015
 * \note
 ****************************************************************************/
#include "s32vs234.h"
#include <stdint.h>
#include <cstring>
#include <cstdlib>

#ifndef __STANDALONE__
  #include <fcntl.h>     // open 
  #include <unistd.h>    // exit 
  #include <sys/ioctl.h> // ioctl 
  #include <sys/mman.h>  // mmap
#else
  #include "sony_func.h"
#endif // else from #ifndef __STANDALONE__

#include "sony_config.h"
#include "sony_config_DOL2.h"

#include "isp_cam_generic.h"
#include "cam_ioctl.h"
#include "isp_cam_sony.h"
#include "vdb_log.h"

//****************************************************************************
// consts
//****************************************************************************
#define VDB_LOG_NOTE_OFF

#define HIST_GREEN_START 128
#define HIST_GREEN_STOP  189
#define HIST_START_IDX (127)
#define HIST_STOP_IDX  (64)

#define SONY_COMPATIBLE_STRING "s32v-sony-imx224"

// *** important camera registers #define SONY_REG_BEG 0x3000
///< first register address
#define SONY_REG_END 0x33EF  ///< last register address

#define SONY_STANDBY_ADDR 0x3000
#define SONY_STANDBY_OPER 0x0
#define SONY_STANDBY_STDB 0x1

//****************************************************************************
// *** sony exposure and geometry config ***

//< winmode registers
#define SONY_WINMODE_ADDR 0x3007
#define SONY_WINMODE_VR   0x1
#define SONY_WINMODE_HR   0x2
#define SONY_WINMODE_QVGA 0x0
#define SONY_WINMODE_720P 0x10
#define SONY_WINMODE_CROP 0x40

//< max frame height registers
#define SONY_VMAXL_ADDR 0x3018
#define SONY_VMAXH_ADDR 0x3019
#define SONY_VMAX_MASK  0x1fff

//< max frame widht registers
#define SONY_HMAXL_ADDR 0x301b
#define SONY_HMAXH_ADDR 0x301c
#define SONY_HMAX_MASK  0x3fff

//< cropping window optical blanking height registers
#define SONY_WINWV_OB_ADDR 0x3036 
#define SONY_WINWV_OB_MASK  0x1f

//< sync mode (master/slave)
#define SONY_XMSTA_ADDR 0x3002 
#define SONY_XMSTA_MASK 0x1

//< cropping window height registers
#define SONY_WINWVL_ADDR 0x303a 
#define SONY_WINWVH_ADDR 0x303b
#define SONY_WINWV_MASK  0x3ff

//< cropping window width registers
#define SONY_WINWHL_ADDR 0x303e
#define SONY_WINWHH_ADDR 0x303f
#define SONY_WINWH_MASK  0x7ff

//< cropping window offset y registers
#define SONY_WINPVL_ADDR 0x3038 
#define SONY_WINPVH_ADDR 0x3039
#define SONY_WINPV_MASK  0x3ff

//< cropping window offset x registers
#define SONY_WINPHL_ADDR 0x303c
#define SONY_WINPHH_ADDR 0x303d
#define SONY_WINPH_MASK  0x7ff

//< conversion registers
#define SONY_CONV_HIGH  0x1
#define SONY_CONV_LOW   0x0
#define SONY_CONV_MASK  0x10
#define SONY_CONV_ADDR  0x3009  //< address of the conversion gain register

//< number of exposure lines registers
#define SONY_SHS1L_ADDR 0x3020  //< address of number of exposure lines
#define SONY_SHS1H_ADDR 0x3021  //< address of number of exposure lines
#define SONY_SHS1_MASK  0x1fff  //< SHS1[12:0] bitmask //sth 17 bit ??

#define SONY_SHS2L_ADDR 0x3023  //< address of number of exposure lines
#define SONY_SHS2H_ADDR 0x3024  //< address of number of exposure lines
#define SONY_SHS2_MASK  0xffff  //< SHS2[12:0] bitmask

#define SONY_LSHSL_ADDR 0x3020  //< address of number of exposure lines
#define SONY_LSHSH_ADDR 0x3021  //< address of number of exposure lines
#define SONY_LSHS_MASK  0x1fff  //< SHS1[12:0] bitmask //sth 17 bit ??

#define SONY_LSHSL_ADDR_DOL 0x3023  //< address of number of exposure lines
#define SONY_LSHSH_ADDR_DOL 0x3024  //< address of number of exposure lines
#define SONY_LSHS_MASK_DOL  0xffff  //< SHS1[12:0] bitmask
#define SONY_SSHSL_ADDR SONY_LSHSL_ADDR  //< address of number of exposure lines
#define SONY_SSHSH_ADDR SONY_LSHSH_ADDR  //< address of number of exposure lines
#define SONY_SSHS_MASK  0xffff  //< SHS1[12:0] bitmask

//< analog gain registers
#define SONY_GAINL_ADDR 0x3014
#define SONY_GAINH_ADDR 0x3015
#define SONY_GAIN_MASK  0x3ff
#define SONY_GAIN_MAX  720

//< max allowed values
#define SONY_WIDTH_MAX   1304
#define SONY_HEIGHT_MAX  976
#define SONY_WWIDTH_MIN  368
#define SONY_WHEIGHT_MIN 304
#define SONY_OB_MAX      16

//****************************************************************************
// types
//****************************************************************************


//****************************************************************************
// global variables
//****************************************************************************
#ifndef __STANDALONE__
  extern int32_t gCamFD;   ///< csi driver file descriptor
#endif //ifndef __STANDALONE__

//*****************************************************************************
// local data
//*****************************************************************************

//static uint8_t sInitialized = 0; ///< >0 if initialized

static CamI2cClient_t spClients[2] = {0}; // initialize to invalid

// Exposure lines range (max value)
static uint32_t sExpRange = 990;  // default value

// sony single/dual exposure
static uint32_t sLshslAddr = SONY_LSHSL_ADDR;
static uint32_t sLshshAddr = SONY_LSHSH_ADDR;
static uint32_t sLshsMask  = SONY_LSHS_MASK;  
static uint32_t sExpMode   = SONY_SINGLE_EXP; // single exposure by default  

// chip geometry settings
static SONY_Geometry_t sGeometry;
static SONY_Exposure_t sExposure;

//*****************************************************************************
// static functions declarations
//*****************************************************************************

/****************************************************************************/
/** Fills in command members based on I2C config table.
 * 
 * \param apCmd      pointer to destination command array
 * \param apSrcTable pointer to source table
 * \param aCmdCnt    number of commands in the arrays
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
static void WriteCmdArrGenerate(
              I2cWriteCmd_t *apCmd, 
              const uint8_t *apSrcTable,
              uint32_t       aCmdCnt
            );

static void SONY_CropParamCheck(SONY_Geometry_t &arGeom);

//*****************************************************************************
// functions definitions
//*****************************************************************************

CAM_LIB_RESULT SONY_Open(const CSI_IDX aCsiIdx)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;
  CamI2cCfg_t    lCamCfg;
  
  if(CAM_Open() == CAM_LIB_SUCCESS)
  {
    if(spClients[aCsiIdx] == 0)
    {
      lCamCfg.mI2cBusIdx    = (I2C_BUS_IDX) aCsiIdx;
      lCamCfg.mI2cAddr      = (uint8_t)I2C_SONY_CAM_ADDR;
      lCamCfg.mRegAddrWidth = I2C_REG_ADDR_WIDTH_16;

      memcpy(lCamCfg.mpCompatible,
              SONY_COMPATIBLE_STRING, 
              strlen(SONY_COMPATIBLE_STRING));


      spClients[aCsiIdx] = CAM_ClientAdd(lCamCfg);
    } // if not created yet
    else
    {
      VDB_LOG_ERROR("Sony IMX224 is already registered on CSI idx %u.\n",
                    (uint32_t)aCsiIdx);
      lRet = CAM_LIB_FAILURE;
    } // else if not created yet

    if(spClients[aCsiIdx] != 0)
    {
      int32_t lRetVal = 0;
#ifndef __STANDALONE__   
      lRetVal = ioctl(gCamFD, IOCTL_SONY_CONNECT, spClients[aCsiIdx]);
#else // #ifndef __STANDALONE__
      lRetVal = SONY_DRV_Setup(spClients[aCsiIdx]);
#endif // else #ifndef __STANDALONE__
      if (lRetVal < 0) 
      {
        VDB_LOG_ERROR("Failed to connect the camera driver.\n");
        lRet = CAM_LIB_FAILURE;
        spClients[aCsiIdx] = 0;
      } // if ioctl returned failure
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
} // SONY_Open()

//*****************************************************************************

CAM_LIB_RESULT SONY_Close(const CSI_IDX aCsiIdx)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;

  if(spClients[aCsiIdx] != 0)
  {
    int32_t lRetVal = 0;
#ifndef __STANDALONE__   
    lRetVal = ioctl(gCamFD, IOCTL_SONY_DISCONNECT, spClients[aCsiIdx]);    
#else // #ifndef __STANDALONE__
    lRetVal = SONY_DRV_Close(spClients[aCsiIdx]);    
#endif // else #ifndef __STANDALONE__
    if (lRetVal < 0) 
    {
      VDB_LOG_ERROR("Failed to disconnect the camera driver.\n");
      lRet = CAM_LIB_FAILURE;
    } // if ioctl returned failure  
    
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
  } // if camera client attached
  else
  {
    VDB_LOG_ERROR("No such client connected.\n");
    lRet = CAM_LIB_FAILURE;
  } // else from if camera client attached
  return lRet;
} // SONY_Close()

//*****************************************************************************

CAM_LIB_RESULT SONY_RegWrite(const CSI_IDX  aCsiIdx, 
                             const uint16_t aRegAddr,
                             const uint8_t  aVal
                            )
{
  return CAM_WriteSingle(spClients[aCsiIdx], aRegAddr, aVal);
} // SONY_RegWrite

//*****************************************************************************

CAM_LIB_RESULT SONY_RegRead(const CSI_IDX  aCsiIdx, 
                            const uint16_t aRegAddr,
                            uint8_t &arVal
                           )
{
  return CAM_ReadSingle(spClients[aCsiIdx], aRegAddr, arVal);
} // SONY_RegWrite

//*****************************************************************************

CAM_LIB_RESULT SONY_RegConfig(const CSI_IDX aCsiIdx, const uint8_t aExpMode)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;
  
  if(spClients[aCsiIdx] != NULL)
  {
    uint8_t *lpSonyTable = NULL;
    uint32_t lCmdCnt;
    //put camera to standby mode
    CAM_WriteSingle(spClients[aCsiIdx], (uint16_t)0x3, (uint8_t)1);
    CAM_WriteSingle(spClients[aCsiIdx], (uint16_t)0x0, (uint8_t)1);
    
    if(aExpMode == SONY_SINGLE_EXP)
    {
      lpSonyTable = Sony_Table;
      lCmdCnt     = (uint32_t)(sizeof(Sony_Table)/3);
      sLshslAddr  = (uint32_t)SONY_LSHSL_ADDR;
      sLshshAddr  = (uint32_t)SONY_LSHSH_ADDR;
      sLshsMask   = (uint32_t)SONY_LSHS_MASK;  
      sExpMode    = (uint32_t)SONY_SINGLE_EXP;
    }// if SE
    else if(aExpMode == SONY_DUAL_EXP)
    {
      lpSonyTable = Sony_Table_DOL;
      lCmdCnt     = (uint32_t)(sizeof(Sony_Table_DOL)/3);
      sLshslAddr  = (uint32_t)SONY_LSHSL_ADDR_DOL;
      sLshshAddr  = (uint32_t)SONY_LSHSH_ADDR_DOL;
      sLshsMask   = (uint32_t)SONY_LSHS_MASK_DOL;  
      sExpMode    = (uint32_t)SONY_DUAL_EXP;
    } // if DOL
    else  
    {
      sExpMode = (uint32_t)SONY_DUAL_EXP + (uint32_t)1;
      VDB_LOG_ERROR("Unknown exposure mode.\n");
      lRet = CAM_LIB_FAILURE;
    } // if unknown exp mode
    
    if(NULL != lpSonyTable)
    {      
      I2cWriteCmd_t *lpCmds = (I2cWriteCmd_t *)malloc(
        sizeof(I2cWriteCmd_t) * lCmdCnt);
      
      if(lpCmds != NULL)
      {
        WriteCmdArrGenerate(lpCmds, lpSonyTable, lCmdCnt);
      
        I2cWriteBatch_t lWriteBatch;
        lWriteBatch.mDestDev = spClients[aCsiIdx]; 
        lWriteBatch.mCmdCnt  = lCmdCnt;  
        lWriteBatch.mpCmdArr = lpCmds; 
        
        lRet = (CAM_LIB_RESULT)CAM_WriteBatch(lWriteBatch);
              
        SONY_VmaxGet(aCsiIdx, sExpRange);

        if(sExpMode == (uint32_t)SONY_DUAL_EXP)
        {
          sExpRange <<= 1;
        } // if dual exposure mode 

        SONY_ExpLinesGet(aCsiIdx, sExposure.mExposureLines);
        SONY_ConvGet(aCsiIdx, sExposure.mConversionGain);
        SONY_GainGet(aCsiIdx, sExposure.mAnalogGain);
        sExposure.mExposureLines = sExpRange - sExposure.mExposureLines
                                                                  - (uint32_t)1;
        
        free(lpCmds);
      } // if command list allocated ok
      else
      {
        VDB_LOG_ERROR("Failed to allocate command array.\n");
        lRet = CAM_LIB_FAILURE;
      } // else from if command list allocated ok      
    } // if sony table exists
  } // if camera client attached
  else
  {
    VDB_LOG_ERROR("No such client connected.\n");
    lRet = CAM_LIB_FAILURE;
  } // else from if camera client attached
  
  return lRet;
} // SONY_RegConfig(CSI_BufferList_t *apBuffsList)

//*****************************************************************************

CAM_LIB_RESULT SONY_CsiEnable(const CSI_IDX aCsiIdx)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;

  lRet = CAM_WriteSingle(spClients[aCsiIdx],
                         (uint16_t)SONY_STANDBY_ADDR,
                         (uint8_t)SONY_STANDBY_OPER);
    
  return lRet;
} // SONY_CsiEnable(CSI_IDX aCsiIdx)

//*****************************************************************************

CAM_LIB_RESULT SONY_CsiDisable(const CSI_IDX aCsiIdx)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;

  // bring camera out of standby
  lRet = CAM_WriteSingle(spClients[aCsiIdx],
                         (uint16_t)SONY_STANDBY_ADDR,
                         (uint8_t)SONY_STANDBY_STDB);
  
  return lRet;
} // SONY_CsiDisable(CSI_IDX aCsiIdx)

//*****************************************************************************
//*** Exposure related functionality 
//*****************************************************************************

int32_t SONY_ConvGet(const CSI_IDX aCsiIdx, uint8_t &arConv)
{
  int32_t lRet = 0;
 
  lRet = CAM_ReadSingle(spClients[aCsiIdx], (uint16_t)SONY_CONV_ADDR, arConv);
  
  arConv = (((uint8_t)0x10 == (arConv & (uint8_t)SONY_CONV_MASK))?
                                (uint8_t)SONY_CONV_HIGH:(uint8_t)SONY_CONV_LOW);
  
  return lRet;
} // SONY_ConvGet()

//*****************************************************************************

int32_t SONY_ConvHighSet(const CSI_IDX aCsiIdx)
{
  int32_t lRet = CAM_LIB_SUCCESS;
  uint8_t lTmp = (uint8_t)0;
  
  if(CAM_ReadSingle(
      spClients[aCsiIdx], 
      (uint16_t)SONY_CONV_ADDR, 
      lTmp) == CAM_LIB_SUCCESS)
  {
    lRet = CAM_WriteSingle(
            spClients[aCsiIdx], 
            (uint16_t)SONY_CONV_ADDR, 
            (lTmp | SONY_CONV_MASK));  
  }
  else
  {
    lRet = CAM_LIB_FAILURE;
  }
  
  return lRet;
} // SONY_ConvHighSet()

//*****************************************************************************

int32_t SONY_ConvLowSet(const CSI_IDX aCsiIdx)
{
  int32_t lRet = CAM_LIB_SUCCESS;
  uint8_t lTmp = (uint8_t)0;
  
  if(CAM_ReadSingle(
      spClients[aCsiIdx], 
      (uint16_t)SONY_CONV_ADDR, 
      lTmp) == CAM_LIB_SUCCESS)
  {
    lRet = CAM_WriteSingle(
            spClients[aCsiIdx], 
            (uint16_t)SONY_CONV_ADDR, 
            (lTmp & (~SONY_CONV_MASK)));
  }
  else
  {
    lRet = CAM_LIB_FAILURE;
  }
  
  return lRet;  
} // SONY_ConvLowSet()

//*****************************************************************************

int32_t SONY_ExpLinesGet(const CSI_IDX aCsiIdx, uint32_t &arExpLines)
{
  int32_t lRet   = CAM_LIB_SUCCESS;
  uint8_t *lpTmp = (uint8_t*)&arExpLines;
  
  if(CAM_ReadSingle(
      spClients[aCsiIdx], 
      (uint16_t)sLshslAddr, 
      lpTmp[0]) == CAM_LIB_SUCCESS)
  {
    if(CAM_ReadSingle(
        spClients[aCsiIdx], 
        (uint16_t)sLshshAddr, 
        lpTmp[1]) != CAM_LIB_SUCCESS)
    {
      lRet = CAM_LIB_FAILURE;
    } // if not read successfully
  } // if read successfully
  else
  {
    lRet = CAM_LIB_FAILURE;
  } // else from if read successfully
  
  arExpLines &= sLshsMask;
  
  return lRet;
} // SONY_ExpLineGet

//*****************************************************************************

int32_t SONY_ExpLinesSGet(const CSI_IDX aCsiIdx, uint32_t &arExpLines)
{
  int32_t lRet   = CAM_LIB_SUCCESS;
  uint8_t *lpTmp = (uint8_t*)&arExpLines;
  
  if(CAM_ReadSingle(
      spClients[aCsiIdx], 
      (uint16_t)SONY_SSHSL_ADDR, 
      lpTmp[0]) == CAM_LIB_SUCCESS)
  {
    if(CAM_ReadSingle(
        spClients[aCsiIdx], 
        (uint16_t)SONY_SSHSH_ADDR, 
        lpTmp[1]) != CAM_LIB_SUCCESS)
    {
      lRet = CAM_LIB_FAILURE;
    } // if not read successfully
  } // if read successfully
  else
  {
    lRet = CAM_LIB_FAILURE;
  } // else from if read successfully
  
  arExpLines &= (uint32_t)SONY_SSHS_MASK;
  
  return lRet;
} // SONY_ExpLinesSGet

//*****************************************************************************

int32_t SONY_ExpLinesSet(const CSI_IDX aCsiIdx, uint32_t aLineCnt)
{  
  int32_t lRet = CAM_LIB_SUCCESS;
  uint8_t lTmp;
  int32_t lShortLineCnt;
  
  if(aLineCnt > (sExpRange - (uint32_t)3))
  {
    aLineCnt = sExpRange - (uint32_t)3;
  } // correct line count if needed
  
  lTmp = (uint8_t)(aLineCnt & (uint32_t)0xff);
  if(CAM_WriteSingle(
      spClients[aCsiIdx], 
      (uint16_t)sLshslAddr, 
      lTmp) == CAM_LIB_SUCCESS)
  {
    lTmp = (uint8_t)((aLineCnt >> 8) & (sLshsMask >> 8));  
    if(CAM_WriteSingle(
        spClients[aCsiIdx], 
        (uint16_t)sLshshAddr, 
        lTmp) != CAM_LIB_SUCCESS)
    {
      lRet = CAM_LIB_FAILURE;
    }

    if(sExpMode == (uint32_t)SONY_DUAL_EXP)
    {
      // short exposure
      lShortLineCnt = (int32_t)sExpRange - (int32_t)aLineCnt + (int32_t)1;//to time
      lShortLineCnt >>= 4;  // 1:16 exposure ratio
      lShortLineCnt = 0x31 /*RSH1*/ - lShortLineCnt -1;
      if (lShortLineCnt < 3) 
      {
        lShortLineCnt=3;
      }
      if (lShortLineCnt > 0x2F /*RSH-2*/) 
      {
        lShortLineCnt=0x2F;
      }
      lTmp = (uint8_t)lShortLineCnt & (uint8_t)0xff;
      if(CAM_WriteSingle(
          spClients[aCsiIdx], 
          (uint16_t)SONY_SSHSL_ADDR, 
          lTmp) == CAM_LIB_SUCCESS)
      {
        lTmp = (uint8_t)(((uint32_t)lShortLineCnt >> 8) 
                                             & ((uint32_t)SONY_SSHS_MASK >> 8));  
        if(CAM_WriteSingle(
            spClients[aCsiIdx], 
            (uint16_t)SONY_SSHSH_ADDR, 
            lTmp) != CAM_LIB_SUCCESS)
        {
          lRet = CAM_LIB_FAILURE;
        }
      } // if write successfully
      else
      {
        lRet = CAM_LIB_FAILURE;
      } // else from if write successfully
    }
  } // if write successfully
  else
  {
    lRet = CAM_LIB_FAILURE;
  } // else from if write successfully
    
  return lRet;
} // SONY_ExpLineSet()

//*****************************************************************************

int32_t SONY_GainGet(const CSI_IDX aCsiIdx, uint32_t &arGain)
{  
  int32_t lRet = CAM_LIB_SUCCESS;
  uint8_t *lpTmp = (uint8_t*)&arGain;
  
  if(CAM_ReadSingle(
      spClients[aCsiIdx], 
      (uint16_t)SONY_GAINL_ADDR, 
      lpTmp[0]) == CAM_LIB_SUCCESS)
  {
    if(CAM_ReadSingle(
        spClients[aCsiIdx], 
        (uint16_t)SONY_GAINH_ADDR, 
        lpTmp[1]) != CAM_LIB_SUCCESS)
    {
      lRet = CAM_LIB_FAILURE;
    } // if not read successfully
  } // if read successfully
  else
  {
    lRet = CAM_LIB_FAILURE;
  } // else from if read successfully
  
  arGain &= (uint32_t)SONY_GAIN_MASK;
  
  return lRet;
} // SONY_GainGet()

//*****************************************************************************

int32_t SONY_GainSet(const CSI_IDX aCsiIdx, uint32_t aGain)
{  
  int32_t lRet = CAM_LIB_SUCCESS;
  uint8_t lTmp;
  
  if(aGain > (uint32_t)SONY_GAIN_MAX)
  {
    aGain = (uint32_t)SONY_GAIN_MAX;
  }

  lTmp = (uint8_t)(aGain & (uint32_t)0xff);
  if(CAM_WriteSingle(
      spClients[aCsiIdx], 
      (uint16_t)SONY_GAINL_ADDR, 
      lTmp) == CAM_LIB_SUCCESS)
  {
    lTmp = (uint8_t)((aGain >> 8) & ((uint32_t)SONY_GAIN_MASK >> 8));
    if(CAM_WriteSingle(
        spClients[aCsiIdx], 
        (uint16_t)SONY_GAINH_ADDR, 
        lTmp) != CAM_LIB_SUCCESS)
    {
      lRet = CAM_LIB_FAILURE;
    }
  } // if read successfully
  else
  {
    lRet = CAM_LIB_FAILURE;
  } // else from if read successfully
    
  return lRet;
} // SONY_GainSet()

//*****************************************************************************

CAM_LIB_RESULT SONY_ExposureStatusGet(
                 const CSI_IDX aCsiIdx, 
                 SONY_ExpState_t &arExpState)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;

#ifndef __STANDALONE__   
  int32_t lRetVal = 0;
  lRetVal = ioctl(gCamFD, IOCTL_SONY_EXP_STATUS_GET,
                  (I2C_BUS_IDX)aCsiIdx);
  if ( lRetVal < (int32_t)0) 
  {
    VDB_LOG_ERROR("Failed to get camera exposure setup.\n");
    lRet = CAM_LIB_FAILURE;
  } // if ioctl returned failure
  else
  {
    arExpState = (SONY_ExpState_t)lRetVal;
  } // else from if ioctl returned failure
#else // #ifndef __STANDALONE__
  arExpState = SONY_DRV_ExpStateGet(aCsiIdx);
#endif // else #ifndef __STANDALONE__
  
  return lRet;
} // SONY_ExposureStatusGet()

//*****************************************************************************

CAM_LIB_RESULT SONY_ExposureGet(SONY_Exposure_t &arExpo)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;

  CSI_IDX &lrCsiIdx = arExpo.mCsiIdx;
  
  if(arExpo.mConfigFlags == (uint8_t)0)
  {
    arExpo.mConfigFlags = (uint8_t)0xff;
  }
  
  // analog gain
  if((uint8_t)0 != (arExpo.mConfigFlags & (uint8_t)SONY_EXP_AG))
  {
      if(SONY_GainGet(
          lrCsiIdx, 
          arExpo.mAnalogGain) != CAM_LIB_SUCCESS)
      {
        lRet = CAM_LIB_FAILURE;
      } // if not success
  } // if AG
  
  // exposure lines
  if((uint8_t)0 != (arExpo.mConfigFlags & (uint8_t)SONY_EXP_EL))
  {
    if(SONY_ExpLinesGet(
          lrCsiIdx, 
          arExpo.mExposureLines) != CAM_LIB_SUCCESS)
    {
      lRet = CAM_LIB_FAILURE;
    } //if not success
    else
    {
      arExpo.mExposureLines = sExpRange - arExpo.mExposureLines - (uint32_t)1;
    }
    if(sExpMode == (uint32_t)SONY_DUAL_EXP)
    {
      if(SONY_ExpLinesSGet(
            lrCsiIdx, 
            arExpo.mExposureLinesShort) != CAM_LIB_SUCCESS)
      {
        lRet = CAM_LIB_FAILURE;
      } //if not success
      else
      {
        arExpo.mExposureLinesShort = (uint32_t)0x31 
                                     - arExpo.mExposureLinesShort - (uint32_t)1;
      }
    } // if DOL used
  } // if EL
  
  // conversion gain
  if((uint8_t)0 != (arExpo.mConfigFlags & (uint8_t)SONY_EXP_CG))
  {
    if(SONY_ConvGet(
          lrCsiIdx, 
          arExpo.mConversionGain) != CAM_LIB_SUCCESS)
    {
      lRet = CAM_LIB_FAILURE;
    } // if not success
  }  // if CG

  sExposure = arExpo;
  
  return lRet;
} // SONY_ExposureGet()

//*****************************************************************************

CAM_LIB_RESULT SONY_ExposureSet(SONY_Exposure_t &arExpo)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;
  CAM_LIB_RESULT lRet1 = CAM_LIB_SUCCESS;

  CSI_IDX &lrCsiIdx = arExpo.mCsiIdx;
  // hold register values and make synchronously valid
  lRet = CAM_WriteSingle(spClients[lrCsiIdx], (uint16_t)0x3001, (uint8_t)1); // set to 1 to hold

  if (CAM_LIB_SUCCESS == lRet)
  {
    // flags for all?
    if(arExpo.mConfigFlags == 0)
    {
      arExpo.mConfigFlags = (uint8_t)0xff;
    } // if flags == 0

    // analog gain
    if((uint8_t)0 != (arExpo.mConfigFlags & (uint8_t)SONY_EXP_AG))
    {
      if(SONY_GainSet(lrCsiIdx, arExpo.mAnalogGain) != CAM_LIB_SUCCESS)
      {
        lRet = CAM_LIB_FAILURE;
      } // if not success
      else
      {
      }
    } // if AG

    // exposure lines
    if(((uint8_t)0 != (arExpo.mConfigFlags & (uint8_t)SONY_EXP_EL))
                                                   && (CAM_LIB_SUCCESS == lRet))
    {
      if(SONY_ExpLinesSet(
          lrCsiIdx, 
          (sExpRange - arExpo.mExposureLines - (uint32_t)1)) != CAM_LIB_SUCCESS)
      {
        lRet = CAM_LIB_FAILURE;
      } // if not success
      else
      {
      }
    } // if EL
    
    // conversion gain
    if(((uint8_t)0 != (arExpo.mConfigFlags & (uint8_t)SONY_EXP_CG))
                                                   && (CAM_LIB_SUCCESS == lRet))
    {
      if(arExpo.mConversionGain == SONY_CONV_LOW)
      {
        if(SONY_ConvLowSet(lrCsiIdx) != CAM_LIB_SUCCESS)
        {
          lRet = CAM_LIB_FAILURE;
        } // if not success
        else
        {
        }
      } // if low
      else
      {
        if(SONY_ConvHighSet(lrCsiIdx) != CAM_LIB_SUCCESS)
        {
          lRet = CAM_LIB_FAILURE;
        } // if not success
        else
        {
        }
      } // else from if low
    } // if CG      

    if (CAM_LIB_SUCCESS == lRet)
    {
      sExposure = arExpo;
#ifndef __STANDALONE__ 
      int32_t lRetVal = ioctl(
                      gCamFD, 
                      IOCTL_SONY_EXP_SENT,
                      (I2C_BUS_IDX)arExpo.mCsiIdx);

      if (lRetVal < 0) 
      {
        VDB_LOG_ERROR("Failed to get camera exposure setup.\n");
        lRet = CAM_LIB_FAILURE;
      } // if ioctl returned failure
#else // #ifndef __STANDALONE__
      SONY_DRV_ExpSent(arExpo.mCsiIdx);
#endif // else #ifndef __STANDALONE__
    }

    lRet1 = CAM_WriteSingle(spClients[lrCsiIdx], (uint16_t)0x3001, (uint8_t)0);
  }

  if ((CAM_LIB_SUCCESS == lRet) || (CAM_LIB_FAILURE == lRet1))
  {
    lRet = lRet1;
  }
  else
  {
  }
  return (lRet);
} // SONY_ExposureSet()

//*****************************************************************************

#define EXP_DEBUG 0//200  // number of frames to debug the exposure values
#if (EXP_DEBUG > 0)
  static uint32_t sppExpRec[EXP_DEBUG][4]; // aExposure, exp lines, conv gain, a gain
  static uint32_t sExpRecIdx = 0;
  static float    spHistEnergy[EXP_DEBUG] = {0.0};
#endif // #if (EXP_DEBUG > 0)

CAM_LIB_RESULT SONY_ExposureSet(const CSI_IDX aCsiIdx, const uint32_t aExposure)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;
  uint32_t lExpLines   = (uint32_t)0;
  uint32_t lConv       = (uint32_t)sExposure.mConversionGain;
  uint32_t lAnalogGain = sExposure.mAnalogGain;

  // *** if increase required ***
  if(aExposure > (uint32_t)SONY_EXP_100_CHNG)
  {
    lExpLines = (sExposure.mExposureLines * aExposure) >> SONY_EXP_100_MULTI;
    if(lExpLines == sExposure.mExposureLines)
    {
      // do at least one line increase
      lExpLines++;
    } // if no change
                 
    if(lExpLines > (uint32_t)SONY_EXP_AGAIN_UP_THRESH) // too much
    {
      // increase conversion gain if possible
      if(lConv == (uint32_t)0)
      {
        lExpLines >>= 1;
        lConv       = (uint32_t)1;
      } // if conversion LOW
      
      // use analog gain if needed
      if(lExpLines > (uint32_t)SONY_EXP_AGAIN_UP_THRESH) // still too much
      {
        // use 2x analog gain (+6dB) only once
        if(lAnalogGain < (uint32_t)SONY_EXP_AGAIN_MAX)
        {
          lExpLines   >>= 1;
          lAnalogGain += (uint32_t)SONY_EXP_AGAIN_DOUBLE;
        } // if some again margin left
       
        if(lExpLines > (uint32_t)SONY_EXP_AGAIN_UP_THRESH)//still too much - cut
        {
          lExpLines = (uint32_t)SONY_EXP_AGAIN_UP_THRESH;
        } // if still too much
      } // if still too much
    } // if exp lines HIGH
    else
    {
      // increase conversion gain if possible
      if((lConv == (uint32_t)0) 
                            && (lExpLines > (uint32_t)SONY_EXP_CGAIN_UP_THRESH))
      {
        lExpLines >>= 1;
        lConv       = (uint32_t)1;
      } // exp lines high and conversion LOW
      
      // decrease analog gain if possible
      if(lAnalogGain > (uint32_t)0)
      {
        if(lExpLines < (uint32_t)SONY_EXP_AGAIN_DOWN_THRESH)
        {
          // try to reduce analog gain
          lAnalogGain -= (uint32_t)SONY_EXP_AGAIN_DOUBLE;
          lExpLines <<= 1; // 2x explines
        } // if exp lines low
      } // if analog gain used
    } // else from if exp lines HIGH
  } // if increase required
  
  //*** if decrease required ***
  if(aExposure <= (uint32_t)SONY_EXP_100_CHNG)
  {
    lExpLines = (sExposure.mExposureLines * aExposure) >> SONY_EXP_100_MULTI;
    
    // if analog gain set at the moment                 
    if(lAnalogGain > (uint32_t)0)
    {
      if(lExpLines < (uint32_t)SONY_EXP_AGAIN_DOWN_THRESH)
      {
        // try to reduce analog gain first
        lAnalogGain -= (uint32_t)SONY_EXP_AGAIN_DOUBLE;
        lExpLines <<= 1; // 2x explines
      } // if exp lines low   
    } // if some analog gain set

    // if too low exp lines, try to decrease conversion gain if possible
    if(lExpLines < (uint32_t)SONY_EXP_CGAIN_DOWN_THRESH)
    {
      // decrease conversion gain if possible
      if(sExposure.mConversionGain == (uint8_t)1)
      {
        lExpLines <<= 1;
        lConv       = (uint32_t)0;
      } // if conversion gain High
    } // if exp lines too low   
  } // if decrease required

  sExposure.mAnalogGain     = lAnalogGain;
  sExposure.mExposureLines  = /*sGeometry.mVmax -*/ lExpLines /*- 1*/;
  sExposure.mConversionGain = (uint8_t)lConv;
  
  sExposure.mCsiIdx = aCsiIdx;

#if (EXP_DEBUG > 0)
  if(sExpRecIdx < EXP_DEBUG)
  {
    sppExpRec[sExpRecIdx][0] = aExposure; // aExposure, exp lines, conv gain, a gain
    sppExpRec[sExpRecIdx][1] = lExpLines;
    sppExpRec[sExpRecIdx][2] = lConv;
    sppExpRec[sExpRecIdx][3] = lAnalogGain;
    sExpRecIdx++;
  } // if not all recorded
  else
  {
    while(1); // wait here
  }// else from if not all recorded
#endif // #if (EXP_DEBUG > 0)  
  
  if(SONY_ExposureSet(sExposure) != CAM_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to set camera exposure setup.\n");
    lRet = CAM_LIB_FAILURE;
  } // if ioctl returned failure
  
  return lRet;
} // SONY_ExposureSet()

//*****************************************************************************

float SONY_HistogramEnergyRelativ(const SEQ_IpuHist_t *const apHist)
{
  uint64_t lEnergy = (uint64_t)0;
  uint32_t lSum    = (uint32_t)0; 
  
  for(uint32_t i=(uint32_t)HIST_GREEN_START; i < (uint32_t)HIST_GREEN_STOP; i++)
  {
    
    lEnergy += ((uint64_t)(apHist->mpData[i]) * ((uint64_t)i
                                                - (uint64_t)HIST_GREEN_START));
    lSum    += apHist->mpData[i];
  }
  
  return ((float)lEnergy/((float)lSum
                                * (float)(HIST_GREEN_STOP - HIST_GREEN_START)));
} // SONY_HistogramEnergyRelativ

//***************************************************************************

void SONY_ExposureControlOver(SEQ_IpuHist_t *apHist,
                              const uint32_t aOverExpPixCnt)
{
  SONY_ExposureControlOver(apHist, aOverExpPixCnt, (uint32_t)32, NULL);
} // SONY_ExposureControlOver

//***************************************************************************

void SONY_ExposureControlOver(
       const SEQ_IpuHist_t *const apHist,
       const uint32_t aOverExpPixCnt, uint32_t aOverExpLevel,
       uint32_t *const apExp)
{
  uint16_t lIndex        = (uint16_t)HIST_START_IDX;
  int16_t  lIndexStop    = (int16_t)0;
  uint32_t lSumLev       = (uint32_t)0;
  uint32_t lSum          = (uint32_t)0;
  uint32_t lExposure     = (uint32_t)SONY_EXP_100_CHNG;

  aOverExpLevel += (uint32_t)HIST_STOP_IDX;
  if (aOverExpLevel > (uint32_t)HIST_START_IDX)
  {
           aOverExpLevel = (uint32_t)HIST_START_IDX;
  }
  else
  {
  }

  if (aOverExpLevel < (uint32_t)HIST_STOP_IDX)
  {
           aOverExpLevel = (uint32_t)HIST_STOP_IDX;
  }
  else
  {
  }
  
  // lSumLev is computed
  while ((uint32_t)lIndex >= aOverExpLevel)
  {        
    if (lSum <  aOverExpPixCnt) {
      lSum += apHist->mpData[lIndex];
      lIndexStop = (int16_t)lIndex;
    }
    lSumLev += apHist->mpData[lIndex--];
  }

  // overexposed level is reached
  while ((lIndex >= HIST_STOP_IDX) && 
          (lSum < aOverExpPixCnt)) 
  {

    lSum += apHist->mpData[lIndex--];
    lIndexStop = (int16_t)lIndex;
  }

  lIndexStop -= (int16_t)HIST_STOP_IDX;
  if (lIndexStop < 1) 
  {
    lIndexStop = 1;
  }


  if (lIndexStop >= (int16_t)((int32_t)aOverExpLevel - (int32_t)HIST_STOP_IDX)) 
  {
    if (lSumLev > (uint32_t)((uint32_t)2*aOverExpPixCnt))            
    {     
      // (x 3/4)
      lExposure = (uint32_t)((SONY_EXP_100_CHNG*96) >> 7);   
    } // if 2x too muany
    else if (lSumLev > ((uint32_t)((uint32_t)3*aOverExpPixCnt)>>1))     
    {  
      // (x 7/8)
      lExposure = (uint32_t)((SONY_EXP_100_CHNG*112)>>7);   
    } // if 1,5x too many
    else if (lSumLev > ((uint32_t)((uint32_t)5*aOverExpPixCnt)>>2))     
    {  
      // (x 15/16)
      lExposure = (uint32_t)((SONY_EXP_100_CHNG*120)>>7);   
    } // if 1,25 too many
    else if (lSumLev > (uint32_t)(((uint32_t)9*aOverExpPixCnt)>>3))    
    {  
      // (x 31/32)
      lExposure = (uint32_t)(SONY_EXP_100_CHNG*124)>>7;   
    } // if 1,125 too many
    else
    {
    }
  } // too many overexposed pixels
  else 
  {
    aOverExpLevel -= (uint32_t)HIST_STOP_IDX;
    lExposure += (uint32_t)((((uint32_t)SONY_EXP_100_CHNG << 10)*
      (aOverExpLevel - (uint32_t)lIndexStop)/(aOverExpLevel)) >> 13);    
  } // else form if too many overexposed pixels
  
  if (apExp)
  {
      *apExp=lExposure;
  }
  else if(lExposure != (uint32_t)SONY_EXP_100_CHNG)
  {
    // apply exposure change
    SONY_ExposureSet(CSI_IDX_0, lExposure);
  } // if exposure change required
  else
  {
  }
} // SONY_ExposureControlOver

//***************************************************************************

void SONY_ExposureControlEqual(const SEQ_IpuHist_t *const apHist, float aOffset)
{ 
  SONY_ExposureControlEqual(apHist, aOffset, 0);
} // SONY_ExposureControl()

//***************************************************************************

void SONY_ExposureControlEqual(const SEQ_IpuHist_t *const apHist, 
                               float aOffset,
                               uint32_t *const apExp)
{ 
  if(apHist != NULL)
  {
    if(aOffset > (float)0.5)
    {
      aOffset = (float)0.5;
    }
    if(aOffset < (float)(-0.5))
    {
      aOffset = (float)(-0.5);
    }
    
    float lRelEnergy = SONY_HistogramEnergyRelativ(apHist);
#if (EXP_DEBUG > 0)    
    if(sExpRecIdx < EXP_DEBUG)
    {
      spHistEnergy[sExpRecIdx] = lRelEnergy;
    }
#endif // #if (EXP_DEBUG > 0)
 
    lRelEnergy = ((float)(((float)0.5 + aOffset) - lRelEnergy)) * (float)0.5;

    if (NULL != apExp)
    {
      *apExp = (uint32_t)((float)SONY_EXP_100_CHNG
                                   + (lRelEnergy * ((float)SONY_EXP_100_CHNG)));
    }
    else
    {  
      SONY_ExposureSet(CSI_IDX_0, (uint32_t)((float)SONY_EXP_100_CHNG
                                    + (lRelEnergy * (float)SONY_EXP_100_CHNG)));
    }
  } // if histogram not NULL
} // SONY_ExposureControlEqual()

//*****************************************************************************

//****************************************************************************
//*** GEOMETRY CONFIG ***
//****************************************************************************

//*****************************************************************************

int32_t SONY_VmaxGet(const CSI_IDX aCsiIdx, uint32_t &arVmax)
{
  int32_t lRet = CAM_LIB_SUCCESS;
  uint8_t *lpTmp = (uint8_t*)&arVmax;
    
  if(CAM_ReadSingle(spClients[aCsiIdx],
                    (uint16_t)SONY_VMAXL_ADDR,
                    lpTmp[0]) == CAM_LIB_SUCCESS)
  {
    if(CAM_ReadSingle(spClients[aCsiIdx], (uint16_t)SONY_VMAXH_ADDR, lpTmp[1])
                   != CAM_LIB_SUCCESS)
    {
      lRet = CAM_LIB_FAILURE;
    } // if not read successfully
  } // if read successfully
  else
  {
    lRet = CAM_LIB_FAILURE;
  } // else from if read successfully
  
  arVmax &= (uint32_t)SONY_VMAX_MASK;
  
  return lRet;
} // SONY_VmaxGet

//*****************************************************************************

int32_t SONY_VmaxSet(const CSI_IDX aCsiIdx, uint32_t aLineCnt)
{  
  int32_t lRet = CAM_LIB_SUCCESS;
  uint8_t lTmp;
  
  aLineCnt &= (uint32_t)SONY_VMAX_MASK;
  
  lTmp = (uint8_t)(aLineCnt & (uint32_t)0xff);
  if(CAM_WriteSingle(spClients[aCsiIdx], (uint16_t)SONY_VMAXL_ADDR, lTmp)
               == CAM_LIB_SUCCESS)
  {
    lTmp = (uint8_t)((aLineCnt >> 8)&((uint32_t)SONY_VMAX_MASK >> 8));  
    if(CAM_WriteSingle(spClients[aCsiIdx], (uint16_t)SONY_VMAXH_ADDR, lTmp)
                != CAM_LIB_SUCCESS)
    {
      lRet = CAM_LIB_FAILURE;
    }
  } // if read successfully
  else
  {
    lRet = CAM_LIB_FAILURE;
  } // else from if read successfully
    
  return lRet;
} // SONY_VmaxSet()

//*****************************************************************************

int32_t SONY_HmaxGet(const CSI_IDX aCsiIdx, uint32_t &arHmax)
{
  int32_t lRet = CAM_LIB_SUCCESS;
  uint8_t *lpTmp = (uint8_t*)&arHmax;
  
  if(CAM_ReadSingle(spClients[aCsiIdx], (uint16_t)SONY_HMAXL_ADDR, lpTmp[0])
         == CAM_LIB_SUCCESS)
  {
    if(CAM_ReadSingle(spClients[aCsiIdx], (uint16_t)SONY_HMAXH_ADDR, lpTmp[1])
            != CAM_LIB_SUCCESS)
    {
      lRet = CAM_LIB_FAILURE;
    } // if not read successfully
  } // if read successfully
  else
  {
    lRet = CAM_LIB_FAILURE;
  } // else from if read successfully
  
  arHmax &= (uint32_t)SONY_HMAX_MASK;
  
  return lRet;
} // SONY_HmaxGet

//*****************************************************************************

int32_t SONY_HmaxSet(const CSI_IDX aCsiIdx, uint32_t aColCnt)
{  
  int32_t lRet = CAM_LIB_SUCCESS;
  uint8_t lTmp;
  
  aColCnt &= (uint32_t)SONY_HMAX_MASK;
  
  lTmp = (uint8_t)(aColCnt & (uint32_t)0x000000ff);
  if(CAM_WriteSingle(spClients[aCsiIdx], (uint16_t)SONY_HMAXL_ADDR, lTmp)
        == CAM_LIB_SUCCESS)
  {
    lTmp = (uint8_t)((aColCnt >> 8) & ((uint32_t)SONY_HMAX_MASK >> 8));  
    if(CAM_WriteSingle(spClients[aCsiIdx], (uint16_t)SONY_HMAXH_ADDR, lTmp)
          != CAM_LIB_SUCCESS)
    {
      lRet = CAM_LIB_FAILURE;
    }
  } // if read successfully
  else
  {
    lRet = CAM_LIB_FAILURE;
  } // else from if read successfully
    
  return lRet;
} // SONY_HmaxSet()

//*****************************************************************************

int32_t SONY_ObWvGet(const CSI_IDX aCsiIdx, uint8_t &arLineCnt)
{
  int32_t lRet = 0;
  
  lRet = CAM_ReadSingle(spClients[aCsiIdx],
                        (uint16_t)SONY_WINWV_OB_ADDR,
                        arLineCnt);
  arLineCnt &= SONY_WINWV_OB_MASK;
  
  return lRet;
} // SONY_CropObWvGet()

//*****************************************************************************

int32_t SONY_ObWvSet(const CSI_IDX aCsiIdx, uint8_t aLineCnt)
{    
  aLineCnt &= SONY_WINWV_OB_MASK;
  
  return CAM_WriteSingle(spClients[aCsiIdx],
                         (uint16_t) SONY_WINWV_OB_ADDR,
                         aLineCnt);
} // SONY_ObWvSet()

//*****************************************************************************

int32_t SONY_SyncGet(const CSI_IDX aCsiIdx, uint8_t &arSyncMode)
{
  int32_t lRet = 0;
  
  lRet = CAM_ReadSingle(spClients[aCsiIdx],
                        (uint16_t)SONY_XMSTA_ADDR,
                         arSyncMode);
  arSyncMode &= SONY_XMSTA_MASK;
  
  return lRet;
} // SONY_SyncGet()

//*****************************************************************************

int32_t SONY_SyncSet(const CSI_IDX aCsiIdx, uint8_t aSyncMode)
{    
  aSyncMode &= (uint8_t)SONY_XMSTA_MASK;
  
  return (CAM_WriteSingle(spClients[aCsiIdx],
                          (uint16_t)SONY_XMSTA_ADDR,
                           aSyncMode));
} // SONY_ObWvSet()

//*****************************************************************************

int32_t SONY_CropWvSet(const CSI_IDX aCsiIdx, uint32_t aHeight)
{  
  int32_t lRet = CAM_LIB_SUCCESS;
  uint8_t lTmp;
  
  aHeight &= (uint32_t)SONY_WINWV_MASK;
  
  lTmp = (uint8_t)(aHeight & (uint32_t)0xff);
  if(CAM_WriteSingle(spClients[aCsiIdx], (uint16_t)SONY_WINWVL_ADDR, lTmp)
          == CAM_LIB_SUCCESS)
  {
    lTmp = (uint8_t)((aHeight >> 8) & ((uint32_t)SONY_WINWV_MASK >> 8));
    if(CAM_WriteSingle(spClients[aCsiIdx], (uint16_t)SONY_WINWVH_ADDR, lTmp) != CAM_LIB_SUCCESS)
    {
      lRet = CAM_LIB_FAILURE;
    }
  } // if read successfully
  else
  {
    lRet = CAM_LIB_FAILURE;
  } // else from if read successfully
    
  return lRet;
} // SONY_CropWvSet()

//*****************************************************************************

int32_t SONY_CropWvGet(const CSI_IDX aCsiIdx, uint32_t &arHeight)
{
  int32_t lRet = CAM_LIB_SUCCESS;
  uint8_t *lpTmp = (uint8_t*)&arHeight;
  
  if(CAM_ReadSingle(spClients[aCsiIdx], (uint16_t)SONY_WINWVL_ADDR, lpTmp[0]) == CAM_LIB_SUCCESS)
  {
    if(CAM_ReadSingle(spClients[aCsiIdx], (uint16_t)SONY_WINWVH_ADDR, lpTmp[1]) != CAM_LIB_SUCCESS)
    {
      lRet = CAM_LIB_FAILURE;
    } // if not read successfully
  } // if read successfully
  else
  {
    lRet = CAM_LIB_FAILURE;
  } // else from if read successfully
  
  arHeight &= (uint32_t)SONY_WINWV_MASK;
  
  return lRet;
} // SONY_CropWvGet()

//*****************************************************************************

int32_t SONY_CropWhSet(const CSI_IDX aCsiIdx, uint32_t aWidth)
{  
  int32_t lRet = CAM_LIB_SUCCESS;
  uint8_t lTmp;
  
  aWidth &= (uint32_t)SONY_WINWH_MASK;
  
  lTmp = (uint8_t)(aWidth & (uint32_t)0xff);
  if(CAM_WriteSingle(spClients[aCsiIdx], (uint16_t)SONY_WINWHL_ADDR, lTmp) == CAM_LIB_SUCCESS)
  {
    lTmp = (uint8_t)((aWidth >> 8) & ((uint32_t)SONY_WINWH_MASK >> 8));
    if(CAM_WriteSingle(spClients[aCsiIdx], (uint16_t)SONY_WINWHH_ADDR, lTmp) != CAM_LIB_SUCCESS)
    {
      lRet = CAM_LIB_FAILURE;
    }
  } // if read successfully
  else
  {
    lRet = CAM_LIB_FAILURE;
  } // else from if read successfully
    
  return lRet;
} // SONY_CropWhSet()

//*****************************************************************************

int32_t SONY_CropWhGet(const CSI_IDX aCsiIdx, uint32_t &arWidth)
{
  int32_t lRet = CAM_LIB_SUCCESS;
  uint8_t *lpTmp = (uint8_t*)&arWidth;
  
  if(CAM_ReadSingle(spClients[aCsiIdx], (uint16_t)SONY_WINWHL_ADDR, lpTmp[0]) == CAM_LIB_SUCCESS)
  {
    if(CAM_ReadSingle(spClients[aCsiIdx], (uint16_t)SONY_WINWHH_ADDR, lpTmp[1]) != CAM_LIB_SUCCESS)
    {
      lRet = CAM_LIB_FAILURE;
    } // if not read successfully
  } // if read successfully
  else
  {
    lRet = CAM_LIB_FAILURE;
  } // else from if read successfully
  
  arWidth &= (uint32_t)SONY_WINWH_MASK;
  
  return (lRet & SONY_WINWH_MASK);
} // SONY_CropWhGet()


//*****************************************************************************

int32_t SONY_CropPvGet(const CSI_IDX aCsiIdx, uint32_t &arYoff)
{  
  int32_t lRet = CAM_LIB_SUCCESS;
  uint8_t *lpTmp = (uint8_t*)&arYoff;
  
  if(CAM_ReadSingle(spClients[aCsiIdx], (uint16_t)SONY_WINPVL_ADDR, lpTmp[0]) == CAM_LIB_SUCCESS)
  {
    if(CAM_ReadSingle(spClients[aCsiIdx], (uint16_t)SONY_WINPVH_ADDR, lpTmp[1]) != CAM_LIB_SUCCESS)
    {
      lRet = CAM_LIB_FAILURE;
    } // if not read successfully
  } // if read successfully
  else
  {
    lRet = CAM_LIB_FAILURE;
  } // else from if read successfully
  
  arYoff &= (uint32_t)SONY_WINPV_MASK;
  
  return (lRet);
} // SONY_CropPvGet()

//*****************************************************************************

int32_t SONY_CropPvSet(const CSI_IDX aCsiIdx, uint32_t aLineCnt)
{  
  int32_t lRet = CAM_LIB_SUCCESS;
  uint8_t lTmp;
  
  aLineCnt &= (uint32_t)SONY_WINPV_MASK;
  
  lTmp = (uint8_t)(aLineCnt & (uint32_t)0xff);
  if(CAM_WriteSingle(spClients[aCsiIdx], (uint16_t)SONY_WINPVL_ADDR, lTmp) == CAM_LIB_SUCCESS)
  {
    lTmp = (uint8_t)(aLineCnt >> 8);  
    if(CAM_WriteSingle(spClients[aCsiIdx], (uint16_t)SONY_WINPVH_ADDR, lTmp) != CAM_LIB_SUCCESS)
    {
      lRet = CAM_LIB_FAILURE;
    }
  } // if read successfully
  else
  {
    lRet = CAM_LIB_FAILURE;
  } // else from if read successfully
    
  return lRet;
} // SONY_CropPvSet()

//*****************************************************************************

int32_t SONY_CropPhGet(const CSI_IDX aCsiIdx, uint32_t &arXoff)
{ 
  int32_t lRet = CAM_LIB_SUCCESS;
  uint8_t *lpTmp = (uint8_t*)&arXoff;
  
  if(CAM_ReadSingle(spClients[aCsiIdx], (uint16_t)SONY_WINPHL_ADDR, lpTmp[0]) == CAM_LIB_SUCCESS)
  {
    if(CAM_ReadSingle(spClients[aCsiIdx], (uint16_t)SONY_WINPHH_ADDR, lpTmp[1]) != CAM_LIB_SUCCESS)
    {
      lRet = CAM_LIB_FAILURE;
    } // if not read successfully
  } // if read successfully
  else
  {
    lRet = CAM_LIB_FAILURE;
  } // else from if read successfully
  
  arXoff &= (uint32_t)SONY_WINPH_MASK;
  
  return lRet;
} // SONY_CropPhGet()

//*****************************************************************************

int32_t SONY_CropPhSet(const CSI_IDX aCsiIdx, const uint32_t aXoff)
{  
  int32_t lRet = CAM_LIB_SUCCESS;
  uint8_t lTmp;
  
  lTmp = (uint8_t)(aXoff & (uint32_t)0xff);
  if(CAM_WriteSingle(spClients[aCsiIdx], (uint16_t)SONY_WINPHL_ADDR, lTmp) == CAM_LIB_SUCCESS)
  {
    lTmp = (uint8_t)((aXoff >> 8) & ((uint32_t)SONY_WINPH_MASK >> 8));  
    if(CAM_WriteSingle(spClients[aCsiIdx], (uint16_t)SONY_WINPHH_ADDR, lTmp) != CAM_LIB_SUCCESS)
    {
      lRet = CAM_LIB_FAILURE;
    }
  } // if read successfully
  else
  {
    lRet = CAM_LIB_FAILURE;
  } // else from if read successfully
    
  return lRet;
} // SONY_CropPhSet()

//*****************************************************************************

int32_t SONY_WinmodeGet(const CSI_IDX aCsiIdx, uint8_t &arWinmode)
{  
  return CAM_ReadSingle(spClients[aCsiIdx], (uint16_t)SONY_WINMODE_ADDR, arWinmode); 
} // SONY_CropPhSet()

//*****************************************************************************

int32_t SONY_WinmodeSet(const CSI_IDX aCsiIdx, const uint8_t aWinmode)
{  
  return CAM_WriteSingle(spClients[aCsiIdx], (uint16_t)SONY_WINMODE_ADDR, aWinmode); 
} // SONY_CropPhSet()

//*****************************************************************************

CAM_LIB_RESULT SONY_GeometryGet(SONY_Geometry_t &arGeom)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;

  uint8_t     lWinmode   = (uint8_t)0;
  CSI_IDX &lrCsiIdx = arGeom.mCsiIdx;

  // *** Get values ***
  if(SONY_VmaxGet(lrCsiIdx,   arGeom.mVmax) != CAM_LIB_SUCCESS)
  {
    lRet = CAM_LIB_FAILURE;
  } // if failed
  if(SONY_HmaxGet(lrCsiIdx,   arGeom.mHmax) != CAM_LIB_SUCCESS)
  {
    lRet = CAM_LIB_FAILURE;
  } // if failed
  if(SONY_CropPhGet(lrCsiIdx, arGeom.mXoff) != CAM_LIB_SUCCESS)
  {
    lRet = CAM_LIB_FAILURE;
  } // if failed
  if(SONY_CropPvGet(lrCsiIdx, arGeom.mYoff) != CAM_LIB_SUCCESS)
  {
    lRet = CAM_LIB_FAILURE;
  } // if failed
  if(SONY_CropWhGet(lrCsiIdx, arGeom.mWidth) != CAM_LIB_SUCCESS)
  {
    lRet = CAM_LIB_FAILURE;
  } // if failed
  if(SONY_CropWvGet(lrCsiIdx, arGeom.mHeight) != CAM_LIB_SUCCESS)
  {
    lRet = CAM_LIB_FAILURE;
  } // if failed
  if(SONY_ObWvGet(lrCsiIdx,   arGeom.mOb) != CAM_LIB_SUCCESS)
  {
    lRet = CAM_LIB_FAILURE;
  } // if failed
  
  arGeom.mExpMode = (uint8_t)SONY_SINGLE_EXP; // TODO: check for
  
  if(SONY_WinmodeGet(lrCsiIdx, lWinmode) != CAM_LIB_SUCCESS)
  {
    lRet = CAM_LIB_FAILURE;
  } // if failed
  else
  {
  }
  
  // *** Eval win mode ***
  if(((lWinmode & (uint8_t)SONY_WINMODE_CROP) == (uint8_t)SONY_WINMODE_CROP))
  {
    arGeom.mWinMode = (uint32_t)SONY_CROP;
  }
  else if(((lWinmode & (uint8_t)SONY_WINMODE_720P) == (uint8_t)SONY_WINMODE_720P))
  {
    arGeom.mWinMode = (uint32_t)SONY_720P;
  }
  else if(((lWinmode & (uint8_t)SONY_WINMODE_QVGA) == (uint8_t)SONY_WINMODE_QVGA))
  {
    arGeom.mWinMode = (uint32_t)SONY_QVGA;
  }
  else
  {
  }
  
  // *** Eval flop mode ***
  arGeom.mVerFlip = ((uint8_t)0 != (lWinmode & (uint8_t)SONY_WINMODE_VR))? (uint8_t)1: (uint8_t)0;
  arGeom.mHorFlip = ((uint8_t)0 != (lWinmode & (uint8_t)SONY_WINMODE_HR))? (uint8_t)1: (uint8_t)0;
  
  // *** Compute aprox FPS ***
  arGeom.mFps     = (uint8_t)(((uint32_t)(30*4500*1100))/(arGeom.mVmax * arGeom.mHmax));
  
  // *** get sync mode (master/slave) ***
  if(SONY_SyncGet(lrCsiIdx, arGeom.mSyncMode) == CAM_LIB_FAILURE)
  {
    lRet = CAM_LIB_FAILURE;
  }    

  sGeometry = arGeom;
  
  return lRet;
} // SONY_GeometryGet()

//*****************************************************************************
static void SONY_CropParamCheck(SONY_Geometry_t &arGeom)
{
  if(arGeom.mOb > (uint8_t)SONY_OB_MAX)
  {
    arGeom.mOb = (uint8_t)SONY_OB_MAX;
  }// don't enforce low OB limit to 6
  else
  {
  }
  
  if(arGeom.mVmax < ((uint32_t)SONY_WHEIGHT_MIN + (uint32_t)arGeom.mOb + (uint32_t)13)) // why 13?
  {
    arGeom.mVmax = ((uint32_t)SONY_WHEIGHT_MIN + (uint32_t)arGeom.mOb + (uint32_t)13);
  }
  else
  {
  }
  
  if(arGeom.mWidth < (uint32_t)SONY_WWIDTH_MIN)
  {
    arGeom.mWidth = (uint32_t)SONY_WWIDTH_MIN;
  }
  else if(arGeom.mWidth > (uint32_t)SONY_WIDTH_MAX)
  {
    arGeom.mWidth = (uint32_t)SONY_WIDTH_MAX;
  }
  else
  {
  }
  
  if((arGeom.mWidth + arGeom.mXoff) > (uint32_t)SONY_WIDTH_MAX)
  {
    arGeom.mXoff = (uint32_t)SONY_WIDTH_MAX - arGeom.mWidth;
  }
  else
  {
  }
  
  if(arGeom.mHeight < (uint32_t)SONY_WHEIGHT_MIN)
  {
    arGeom.mHeight = (uint32_t)SONY_WHEIGHT_MIN;
  }
  else if(arGeom.mHeight > (uint32_t)SONY_HEIGHT_MAX)
  {
    arGeom.mHeight = (uint32_t)SONY_HEIGHT_MAX;
  }
  else
  {
  }
  
  if(arGeom.mHeight > (arGeom.mVmax - (uint32_t)16))
  {
    arGeom.mHeight = (arGeom.mVmax - (uint32_t)16);
  }
  else
  {
  }
  
  if((arGeom.mHeight + arGeom.mYoff) > (uint32_t)SONY_HEIGHT_MAX)
  {
    arGeom.mYoff = (uint32_t)SONY_HEIGHT_MAX - arGeom.mHeight;
  }
  else
  {
  }
  
  if((arGeom.mHeight + arGeom.mYoff) > arGeom.mVmax)
  {
    arGeom.mYoff = arGeom.mVmax - arGeom.mHeight - (uint32_t)9 - (uint32_t)16;
  }
  else
  {
  }

}

//*****************************************************************************

CAM_LIB_RESULT SONY_GeometrySet(SONY_Geometry_t &arGeom)
{
  CAM_LIB_RESULT lRet = CAM_LIB_SUCCESS;
  const CSI_IDX &lrCsiIdx = arGeom.mCsiIdx;
  uint8_t lWinmode = (uint8_t)0;
  
  if(arGeom.mExpMode != (uint8_t)sExpMode)
  {
    SONY_RegConfig(lrCsiIdx, arGeom.mExpMode);
    VDB_LOG_NOTE("Modified exposure to %s\n",
                 (sExpMode == (uint32_t)SONY_SINGLE_EXP)?"single":"dual");
  }
  // *** geometry update allowed only for single exposure ***//
  if(sExpMode == (uint32_t)SONY_SINGLE_EXP)
  {
    if(arGeom.mWinMode == (uint32_t)SONY_CROP)
    {
      // check values
      arGeom.mVmax     &= (uint32_t)SONY_VMAX_MASK;
      arGeom.mWidth    &= (uint32_t)0xfffffffc; // force multiple of 4
      arGeom.mXoff     &= (uint32_t)0xfffffffc; // force multiple of 4
      
      SONY_CropParamCheck(arGeom);
      
      // compute HMAX to fit required fps
      arGeom.mHmax = (uint32_t)((uint32_t)30*(uint32_t)4500*(uint32_t)1100/arGeom.mVmax/(uint32_t)arGeom.mFps);
    
      // set values
      if(SONY_VmaxSet(lrCsiIdx, arGeom.mVmax) == CAM_LIB_FAILURE)
      {
        lRet = CAM_LIB_FAILURE;
      }
      if(SONY_HmaxSet(lrCsiIdx, arGeom.mHmax) == CAM_LIB_FAILURE)
      {
        lRet = CAM_LIB_FAILURE;
      }
      if(SONY_CropPhSet(lrCsiIdx, arGeom.mXoff) == CAM_LIB_FAILURE)
      {
        lRet = CAM_LIB_FAILURE;
      }
      if(SONY_CropPvSet(lrCsiIdx, arGeom.mYoff) == CAM_LIB_FAILURE)
      {
        lRet = CAM_LIB_FAILURE;
      }
      if(SONY_CropWhSet(lrCsiIdx, arGeom.mWidth) == CAM_LIB_FAILURE)
      {
        lRet = CAM_LIB_FAILURE;
      }
      if(SONY_CropWvSet(lrCsiIdx, arGeom.mHeight) == CAM_LIB_FAILURE)
      {
        lRet = CAM_LIB_FAILURE;
      }
      
      lWinmode = (uint8_t)SONY_WINMODE_CROP;
    }
    else if(arGeom.mWinMode == (uint32_t)SONY_720P)
    {
      lWinmode = (uint8_t)SONY_WINMODE_720P;
    }
    else
    {
      lWinmode = (uint8_t)SONY_WINMODE_QVGA;
    }
  }
  else
  {
    // crop mode required in DOL mode currently
    lWinmode = (uint8_t)SONY_WINMODE_CROP;
  } // else from if single exposure mode

  // set winmode and flip mode
  if(arGeom.mVerFlip)
  {
    lWinmode |= (uint8_t)SONY_WINMODE_VR;
  }
  if(arGeom.mHorFlip)
  {
    lWinmode |= (uint8_t)SONY_WINMODE_HR;
  }
  if(SONY_WinmodeSet(lrCsiIdx, lWinmode) == CAM_LIB_FAILURE)
  {
    lRet = CAM_LIB_FAILURE;
  }

  if(sExpMode == (uint32_t)SONY_SINGLE_EXP)
  {
    // set optical black lines
    if(SONY_ObWvSet(lrCsiIdx, arGeom.mOb) == CAM_LIB_FAILURE)
    {
      lRet = CAM_LIB_FAILURE;
    }
  } // if single exposure mode
    
  // set sync mode (master/slave)
  if(SONY_SyncSet(lrCsiIdx, arGeom.mSyncMode) == CAM_LIB_FAILURE)
  {
    lRet = CAM_LIB_FAILURE;
  }

  sGeometry = arGeom;
  
  return lRet;
} // SONY_GeometrySet()

//*****************************************************************************

void SONY_ChGainMeasure(const SEQ_IpuHist_t *const apHist,
                        const uint32_t PixCntLevel,
                        SONY_ChGain_t *const aChGain)
{
  if(NULL != aChGain) 
  {
    uint8_t  lChannel;

    aChGain->mMax = (uint16_t)0;
    aChGain->mMin = ((uint16_t)64 << 6) - (uint16_t)1;

    for(lChannel = (uint8_t)0; lChannel < (uint8_t)4; lChannel++) 
    {
      uint32_t lSum;
      uint32_t lNextSum;
      uint16_t *lpLevel;
      uint8_t  lIndex;
      uint8_t  lIndexStop;
      // 6 bit + 2 to reach 8.8 Multiplier in IPU kernel
      const uint8_t ChGainFractionalBits = (uint8_t)6;

      lpLevel    = &aChGain->mLevel[lChannel];
      lIndexStop = lChannel << 6;
      lNextSum   = lSum = (uint32_t)0;
      
      for(lIndex = lIndexStop + (uint8_t)63; lIndex >= lIndexStop; lIndex--) 
      {
        lNextSum += apHist->mpData[lIndex];
        if (lNextSum > PixCntLevel) 
        {
          // linear interpolation for higher precision
          *lpLevel = (((uint16_t)lIndex - (uint16_t)1 - (uint16_t)((uint16_t)lChannel << 6)) << ChGainFractionalBits);
          // compute fractional part if any
          if(lNextSum > lSum)
          {
             *lpLevel += (uint16_t)((((PixCntLevel - lSum) << ChGainFractionalBits))\
              / (lNextSum - lSum));
          } // if fractional part is not 0
         
          // for checking range
          if (aChGain->mMax < *lpLevel) 
          {
            aChGain->mMax = *lpLevel;
          } // if new Max reached
          
          if (aChGain->mMin > *lpLevel) 
          {
            aChGain->mMin = *lpLevel;
          } // if new Min reached
          break;  // next channel
        } // if (lNextSum>PixCntLevel) 
        else 
        {
          lSum = lNextSum;
        } //else from if (lNextSum>PixCntLevel) 
      } // for index range
    } // for all channels
  } // if achGain not NULL

  return;
} // SONY_ChGainMeasure

//***************************************************************************

static void WriteCmdArrGenerate(
              I2cWriteCmd_t *const apCmd, 
              const uint8_t *const apcSrcTable,
              const uint32_t       aCmdCnt
            )
{
  if((apCmd != NULL) && (apcSrcTable != NULL))
  {
    for(uint32_t i = (uint32_t)0; i < aCmdCnt; i++)
    {
      apCmd[i].mRegAddr     = (uint16_t)(apcSrcTable[(3U*i) + 1U]) + (uint16_t)(((uint16_t)(apcSrcTable[(3U*i) + 0U])) << 8);
      apCmd[i].mVal         = apcSrcTable[(3U*i) + 2U];
      apCmd[i].mMask        = (uint8_t)0x0;   // modify all bits
      apCmd[i].mDelay       = (uint16_t)0;     // no delay
      apCmd[i].mReadBackCnt = (uint8_t)0;     // no read back
    } // for all commands
  } // if pointers not NULL
} // WriteCmdArrGenerate()                                

//***************************************************************************

/*EOF*/
