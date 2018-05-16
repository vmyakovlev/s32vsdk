/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
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
* \file     maxim_public.h
* \brief    public declarations for maxim user space library
* \author   Tomas Babinec
* \version  0.1
* \date     14-December-2014
* \note
****************************************************************************/

#ifndef MAXIMPUBLIC_H
#define MAXIMPUBLIC_H

#include "../libs/isp/csi/kernel/include/csi.h"
#include "isp_cam_generic.h"
#include "../libs/isp/cam_generic/kernel/include/maxim_types.h"
#include "isp_cam_ov10640.h"

//*****************************************************************************
// constants
//*****************************************************************************

//*****************************************************************************
// types
//*****************************************************************************

//*****************************************************************************
// public functionality declarations
//*****************************************************************************

/****************************************************************************/
/** Connect to the Maxim HW setup.
* 
* \param arConfig[in] configuration of the MAXIM hw setup
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT MAXIM_Open(const MAXIM_Cfg_t &arConfig);

/****************************************************************************/
/** Disconnect from the Maxim HW setup.
* 
* \param aCsiIdx[in] index of the CSI interface the camera is attached to
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT MAXIM_Close(const enum CSI_IDX aCsiIdx);

/****************************************************************************/
/** Enables CSI transmission from Maxim.
 * 
 * \param aCsiIdx index of the csi interface
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
CAM_LIB_RESULT MAXIM_CsiEnable(const enum CSI_IDX aCsiIdx);

/****************************************************************************/
/** Disables CSI transmission from Maxim.
* 
* \param aCsiIdx index of the csi interface
* 
* \return CAM_LIB_SUCCESS if all ok, 
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT MAXIM_CsiDisable(const enum CSI_IDX aCsiIdx);

/****************************************************************************/
/** Configures Maxim HW & Ov10640 camera registers.
* 
* Every camera/serializer will have unique addresses
* 
* \param aCsiIdx[in] index of the CSI interface the camera is attached to
* \param acCamCnt[in] number of cameras connected
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT MAXIM_OV10640_InitComplex(const enum CSI_IDX aCsiIdx, 
                                         uint32_t acCamCnt);

/****************************************************************************/
/** Configures Maxim HW & Ov10640 camera registers.
* 
* Only broadcast address are used no camera specific config.
* 
* \param aCsiIdx[in] index of the CSI interface the camera is attached to
* \param acCamCnt[in] number of cameras connected
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT MAXIM_OV10640_InitSimple(const enum CSI_IDX aCsiIdx, 
                                        uint32_t acCamCnt);

/****************************************************************************/
/** Writes a value over I2C to Maxim connected camera register.
* 
* By default uses broadcast to update all attached cameras at once.
* 
* \param aCsiIdx[in]  Csi interface the camera is connected to
* \param aRegAddr[in] address of the register
* \param aVal[in]     value to be written
* \param aChnlIdx[in] Maxim channel to be used
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT MAXIM_CAM_RegWrite(
  const enum CSI_IDX  aCsiIdx, 
  const uint16_t aRegAddr,
  const uint8_t  aVal,
  const enum MAXIM_CHNL aChnlIdx = MAXIM_CHNL_ORIG_BC
);

/****************************************************************************/
/** Reads a value over I2C from Maxim connected camera register.
* 
* By default reads only from the camera attached to first deserializer slot.
* 
* \param aCsiIdx[in]  Csi interface the camera is connected to
* \param aRegAddr[in] address of the register
* \param arVal[out]    value read
* \param aChnlIdx[in] Maxim channel to be used
* 
* \return CSI_LIB_SUCCESS if all ok
*         CSI_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT MAXIM_CAM_RegRead(  
  const CSI_IDX  aCsiIdx, 
  const uint16_t aRegAddr,
  uint8_t  &arVal,
  const enum MAXIM_CHNL aChnlIdx = MAXIM_CHNL_0
);

/****************************************************************************/
/** Writes a value over I2C to Maxim connected serializer register.
* 
* By default uses broadcast to update all attached serializers at once.
* 
* \param aCsiIdx[in]  Csi interface the camera is connected to
* \param aRegAddr[in] address of the register
* \param aVal[in]     value to be written
* \param aChnlIdx[in] Maxim channel to be used
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT MAXIM_SER_RegWrite(
  const enum CSI_IDX  aCsiIdx, 
  const uint16_t aRegAddr,
  const uint8_t  aVal,
  const enum MAXIM_CHNL aChnlIdx = MAXIM_CHNL_ORIG_BC
);

/****************************************************************************/
/** Reads a value over I2C from Maxim connected serializer register.
* 
* By default reads only from the seralizer attached to first deserializer slot.
* 
* \param aCsiIdx[in]  Csi interface the camera is connected to
* \param aRegAddr[in] address of the register
* \param arVal[out]    value read
* \param aChnlIdx[in] Maxim channel to be used
* 
* \return CSI_LIB_SUCCESS if all ok
*         CSI_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT MAXIM_SER_RegRead(  
  const enum CSI_IDX  aCsiIdx, 
  const uint16_t aRegAddr,
  uint8_t  &arVal,
  const enum MAXIM_CHNL aChnlIdx = MAXIM_CHNL_0
);

/****************************************************************************/
/** Writes a value over I2C to Maxim connected camera register.
* 
* By default uses broadcast to update all attached cameras at once.
* 
* \param aCsiIdx[in]  Csi interface the camera is connected to
* \param aRegAddr[in] address of the register
* \param aVal[in]     value to be written
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT MAXIM_DES_RegWrite(
  const enum CSI_IDX  aCsiIdx, 
  const uint16_t aRegAddr,
  const uint8_t  aVal
);

/****************************************************************************/
/** Reads a value over I2C from Maxim connected camera register.
* 
* By default reads only from the camera attached to first deserializer slot.
* 
* \param aCsiIdx[in]  Csi interface the camera is connected to
* \param aRegAddr[in] address of the register
* \param arVal[out]   value read
* 
* \return CSI_LIB_SUCCESS if all ok
*         CSI_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT MAXIM_DES_RegRead(  
  const enum CSI_IDX  aCsiIdx, 
  const uint16_t aRegAddr,
  uint8_t  &arVal
);

/****************************************************************************/
/** Configures Exposure parameters to the OV10640 cameras.
* 
* Uses broadcast to update all attached cameras at once.
*
* \param  arExp reference to structure with exposure parameters
* 
* \return CAM_LIB_SUCCESS if all ok, 
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT MAXIM_OV10640_ExposureSet(OV10640_Exposure_t &arExp);

/****************************************************************************/
/** Configures Maxim HW & Ov10635 camera registers.
* 
* Default serializer/camera I2C address used as broadcast.
* 
* \param aCsiIdx[in] index of the CSI interface the camera is attached to
* \param acCamCnt[in] number of cameras connected
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT MAXIM_OV10635_InitComplex(const enum CSI_IDX aCsiIdx, 
                                         uint32_t acCamCnt);

/****************************************************************************/
/** Configures Maxim HW & Sony imx224 camera registers.
* 
* Every camera/serializer will have unique addresses
* 
* \param aCsiIdx[in] index of the CSI interface the camera is attached to
* \param acCamCnt[in] number of cameras connected
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT MAXIM_SONY_InitComplex(const enum CSI_IDX aCsiIdx,
                                      uint32_t acCamCnt);

CAM_LIB_RESULT MAXIM9286_96705_Init(const enum CSI_IDX aCsiIdx, 
                                         uint32_t acCamCnt);
void MAXIM_DeserDump(uint8_t* const apDump, uint32_t aSize);

#endif /* MAXIMPUBLIC_H */

/* EOF */