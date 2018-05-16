/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2015-2016 Freescale Semiconductor
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
* \file     sony_public.h
* \brief    public declarations for Sony user space library
* \author   Tomas Babinec
* \version  0.1
* \date     03-November-2014
* \note
****************************************************************************/

#ifndef SONYPUBLIC_H
#define SONYPUBLIC_H

#include "config.h"
#include "isp_cam_generic.h"
#include "../libs/isp/cam_generic/kernel/include/sony_types.h"
#include "../libs/isp/sequencer/kernel/include/seq_types.h"

//*****************************************************************************
// constants
//*****************************************************************************

// *** SONY exposure control ***
#define SONY_EXP_MAX_CHNG          8192
#define SONY_EXP_100_MULTI         12
#define SONY_EXP_100_CHNG          (1 << SONY_EXP_100_MULTI)
#define SONY_EXP_AGAIN_DOUBLE      60
#define SONY_EXP_AGAIN_MAX         720
#ifdef CAM_SONY_DOL
#define SONY_EXP_CGAIN_UP_THRESH   (64 * 12) ///< threshold to switch to high ConvGain
#define SONY_EXP_CGAIN_DOWN_THRESH (64 * 2)  ///< threshold to switch to low ConvGain
#define SONY_EXP_AGAIN_UP_THRESH   ((sGeometry.mVmax - (uint32_t)1)*2)
#define SONY_EXP_AGAIN_DOWN_THRESH ((((uint32_t)3 * sGeometry.mVmax) >> 3)*(uint32_t)2)
#else
#define SONY_EXP_AGAIN_UP_THRESH   (sGeometry.mVmax - (uint32_t)1)
#define SONY_EXP_AGAIN_DOWN_THRESH (((uint32_t)3 * sGeometry.mVmax) >> 3)
#define SONY_EXP_CGAIN_UP_THRESH   (32 * 12) ///< threshold to switch to high ConvGain
#define SONY_EXP_CGAIN_DOWN_THRESH (32 * 2)  ///< threshold to switch to low ConvGain
#endif

//*****************************************************************************
// types
//*****************************************************************************

//*****************************************************************************
// public functionality declarations
//*****************************************************************************

/****************************************************************************/
/** Connect to the Sony camera HW.
* 
* \param aCsiIdx[in] Csi interface the camera is connected to
* 
* \return CSI_LIB_SUCCESS if all ok
*         CSI_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT SONY_Open(const CSI_IDX aCsiIdx);

/****************************************************************************/
/** Disconnect from the Sony camera HW.
* 
* \param aCsiIdx[in] Csi interface the camera is connected to
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT SONY_Close(const CSI_IDX aCsiIdx);

/****************************************************************************/
/** Writes a value over I2C to Sony camera register.
* 
* \param aCsiIdx[in]  Csi interface the camera is connected to
* \param aRegAddr[in] address of the register
* \param aVal[in]     value to be written
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT SONY_RegWrite(const CSI_IDX  aCsiIdx, 
                             uint16_t aRegAddr,
                             uint8_t  aVal
                            );

/****************************************************************************/
/** Reads a value over I2C from Sony camera register.
* 
* \param aCsiIdx[in]  Csi interface the camera is connected to
* \param aRegAddr[in] address of the register
* \param aVal[out]    value read
* 
* \return CSI_LIB_SUCCESS if all ok
*         CSI_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT SONY_RegRead(const CSI_IDX  aCsiIdx, 
                            uint16_t aRegAddr,
                            uint8_t &arVal
                           );

/****************************************************************************/
/** Configures Sony camera registers.
* 
* \param aCsiIdx[in] Csi interface the camera is connected to
* \param aExpMode[in]   exposure mode (SINGLE/DUAL)
* 
* \return CSI_LIB_SUCCESS if all ok
*         CSI_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT SONY_RegConfig(const CSI_IDX aCsiIdx,
                              const uint8_t aExpMode = SONY_SINGLE_EXP
                             );

/****************************************************************************/
/** Checks current status of exposure control state machine.
 * 
 * \param aCsiIdx[in] Csi interface the camera is connected to
 * \param arExpState[out] variable to store the state to
 * 
 * \return CSI_LIB_SUCCESS if all ok
 *         CSI_LIB_FAILURE otherwise
 ****************************************************************************/
CAM_LIB_RESULT SONY_ExposureStatusGet(const CSI_IDX aCsiIdx, 
                                      SONY_ExpState_t &arExpState);

/****************************************************************************/
/** Enables CSI transmission from Sony camera.
 * 
 * \param apGraphPackage packed graph structure
 * 
 * \return CSI_LIB_SUCCESS if all ok
 *         CSI_LIB_FAILURE otherwise
 ****************************************************************************/
CAM_LIB_RESULT SONY_CsiEnable(const CSI_IDX aCsiIdx);

/****************************************************************************/
/** Disables CSI transmission from Sony camera.
* 
* \return CSI_LIB_SUCCESS if all ok, 
*         CSI_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT SONY_CsiDisable(const CSI_IDX aCsiIdx);

//****************************************************************************
//*** EXPOSURE CONTROL ***
//****************************************************************************

/****************************************************************************/
/** Reads current setup of the conversion gain.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param arConv  reference to Converersion variable
 * 
 * \return SONY_LIB_SUCCESS if all ok
 *         SONY_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_ConvGet(const CSI_IDX aCsiIdx, uint8_t &arConv);

/****************************************************************************/
/** Sets the conversion gain to low.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * 
 * \return SONY_LIB_SUCCESS if all ok
 *         SONY_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_ConvLowSet(const CSI_IDX aCsiIdx);

/****************************************************************************/
/** Sets the confersion gain to high.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * 
 * \return SONY_LIB_SUCCESS if all ok
 *         SONY_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_ConvHighSet(const CSI_IDX aCsiIdx);

/****************************************************************************/
/** Reads current setup of the exposure line count.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param arExpLines pointer to expline variable
 * 
 * \return SONY_LIB_SUCCESS if all ok
 *         SONY_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_ExpLinesGet(const CSI_IDX aCsiIdx, uint32_t &arExplines);

/****************************************************************************/
/** Reads current setup of the exposure line count of short exposure.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param arExpLines pointer to expline variable
 * 
 * \return SONY_LIB_SUCCESS if all ok
 *         SONY_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_ExpLinesSGet(const CSI_IDX aCsiIdx, uint32_t &arExplines);

/****************************************************************************/
/** Sets up the exposure line count.
 * 
 * The provided line count can be modified to fit the possible range.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param aLineCnt exposure time in number of lines 
 * 
 * \return SONY_LIB_SUCCESS if all ok
 *         SONY_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_ExpLinesSet(const CSI_IDX aCsiIdx, uint32_t aLineCnt);

/****************************************************************************/
/** Reads current setup of the analog gain.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param arGain  pointer to gain variable in dB*10
 *
 * \return SONY_LIB_SUCCESS if all ok
 *         SONY_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_GainGet(const CSI_IDX aCsiIdx, uint32_t &arGain);

/****************************************************************************/
/** Sets up the analog gain.
 * 
 * The provided analog gained can be adjusted to fit the allowed range.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param aGain   analog gain in dB*10 to be set
 * 
 * \return SONY_LIB_SUCCESS if all ok
 *         SONY_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_GainSet(const CSI_IDX aCsiIdx, uint32_t aGain);

/****************************************************************************/
/** Gets the camera exposure setup.
 *
 * Reads values over i2c from camera module.
 * 
 * \param arExpo  pointer to structure to be filled with exposure settings
 * 
 * \return CSI_LIB_SUCCESS if all ok
 *         CSI_LIB_FAILURE otherwise
 ****************************************************************************/
CAM_LIB_RESULT SONY_ExposureGet(SONY_Exposure_t &arExpo);

/****************************************************************************/
/** Sets the camera exposure values.
 *
 * The single exposure value is split into exposure lines / conversion gain /
 * analog gain configuration and set over i2c to the sensor.
 * 
 * \param aCsiIdx    CSI interface index the camera is connected to.
 * \param aExposure  number defining the exposure level <0; SONY_EXP_MAX>
 *                   values higher than SONY_EXP_TIME_MAX use analog gain
 *                   config, which will increase level of noise in the image
 * 
 * \return CSI_LIB_SUCCESS if all ok
 *         CSI_LIB_FAILURE otherwise
 ****************************************************************************/
CAM_LIB_RESULT SONY_ExposureSet(const CSI_IDX aCsiIdx, 
                                const uint32_t aExposure);

/****************************************************************************/
/** Sets the camera exposure values.
 *
 * Writes values over i2c to camera module. Provided values can be updated to 
 * fit camera requirements. Updated values available on return.
 * 
 * \param arExpo  pointer to exoposure structure 
 * 
 * \return CSI_LIB_SUCCESS if all ok
 *         CSI_LIB_FAILURE otherwise
 ****************************************************************************/
CAM_LIB_RESULT SONY_ExposureSet(SONY_Exposure_t &arExpo);

//****************************************************************************
//*** GEOMETRY CONFIG ***
//****************************************************************************

/****************************************************************************/
/** Gets the number of optical black lines per frame.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param arLineCnt pointer to line count variable
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_ObWvGet(const CSI_IDX aCsiIdx, uint8_t &arLineCnt);

/****************************************************************************/
/** Sets the number of optical blacklines per frame.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param sLineCnt number of lines to be set
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_ObWvSet(const CSI_IDX aCsiIdx, uint8_t aLineCnt);

/****************************************************************************/
/** Gets the sync mode.
 * 
 * \param aCsiIdx    CSI interface index the camera is connected to.
 * \param arSyncMode SONY_MASTER / SONY_SLAVE
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_SyncGet(const CSI_IDX aCsiIdx, uint8_t &arSyncMode);

/****************************************************************************/
/** Sets the sync mode.
 * 
 * \param aCsiIdx   CSI interface index the camera is connected to.
 * \param aSyncMode SONY_MASTER / SONY_SLAVE
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_SyncSet(const CSI_IDX aCsiIdx, uint8_t aSyncMode);

/****************************************************************************/
/** Gets the maximum number of lines per frame.
 * 
 * The actual number of lines depends on cropping setup.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param arVmax  pointer to VMAX variable
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_VmaxGet(const CSI_IDX aCsiIdx, uint32_t &arVmax);

/****************************************************************************/
/** Sets the maximum number of lines per frame.
 * 
 * The actual number of lines depends on cropping setup.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param sLineCnt number of lines to be set
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_VmaxSet(const CSI_IDX aCsiIdx, uint32_t aLineCnt);

/****************************************************************************/
/** Gets the maximum number of columns per frame.
 * 
 * The actual number of columns depends on cropping setup.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param arHmax  pointer to HMAX variable
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_HmaxGet(const CSI_IDX aCsiIdx, uint32_t &arHmax);

/****************************************************************************/
/** Sets the maximum number of columns per frame.
 * 
 * The actual number of lines depends on cropping setup.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param aColCnt number of columns to be set
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_HmaxSet(const CSI_IDX aCsiIdx, uint32_t aColCnt);

/****************************************************************************/
/** Gets the crop window number of lines per frame.
 * 
 * The actual number of lines depends on cropping setup.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param arHeight pointer to height variable
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_CropWvGet(const CSI_IDX aCsiIdx, uint32_t &arHeight);

/****************************************************************************/
/** Sets the crop window number of lines per frame.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param sLineCnt number of lines to be set
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_CropWvSet(const CSI_IDX aCsiIdx, uint32_t aHeight);

/****************************************************************************/
/** Gets the crop window number of columns per frame.
 *
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param arWidth pointer to width variable
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_CropWhGet(const CSI_IDX aCsiIdx, uint32_t &arWidth);

/****************************************************************************/
/** Sets the crop window number of lines per frame.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param aWidth number of columns to be set
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_CropWhSet(const CSI_IDX aCsiIdx, uint32_t aWidth);

/****************************************************************************/
/** Gets the crop window y offset.
 *
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param arXoff pointer to y offset variable
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_CropPvGet(const CSI_IDX aCsiIdx, uint32_t &arYoff);

/****************************************************************************/
/** Sets the crop window y offset.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param aYoff y offset to be set
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_CropPvSet(const CSI_IDX aCsiIdx, uint32_t aYoff);

/****************************************************************************/
/** Gets the crop window x offset.
 *
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param arXoff pointer to X offset variable
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_CropPhGet(const CSI_IDX aCsiIdx, uint32_t &arXoff);

/****************************************************************************/
/** Sets the crop window x offset.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param aXoff X offset variable to be set
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_CropPhSet(const CSI_IDX aCsiIdx, const uint32_t aXoff);

/****************************************************************************/
/** Gets the window mode register value.
 *
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param arWinmode pointer to winmode variable
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_WinmodeGet(const CSI_IDX aCsiIdx, uint8_t &arWinmode);

/****************************************************************************/
/** Sets the window mode register value.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 * \param aWinmode winmode register value to be set
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_WinmodeSet(const CSI_IDX aCsiIdx, const uint8_t aWinmode);

/****************************************************************************/
/** Gets the camera geometry.
 *
 * Reads values over i2c from camera module.
 * 
 * \param arGeom  reference to structure to be filled with geometry
 * 
 * \return CSI_LIB_SUCCESS if all ok
 *         CSI_LIB_FAILURE otherwise
 ****************************************************************************/
CAM_LIB_RESULT SONY_GeometryGet(SONY_Geometry_t &arGeom);

/****************************************************************************/
/** Sets the camera geometry.
 *
 * Writes values over i2c to camera module. Provided values can be updated to 
 * fit camera requirements. Updated values available on return.
 * 
 * \param arGeom  reference to structure to be filled with geometry
 * 
 * \return CSI_LIB_SUCCESS if all ok
 *         CSI_LIB_FAILURE otherwise
 ****************************************************************************/
CAM_LIB_RESULT SONY_GeometrySet(SONY_Geometry_t &arGeom);

/************************************************************************/
/** Contols exposure to achieve certain level of overexposed pixels.
  * 
  * \param  apHist   histogram data
  * \param  aOverExp number of pixel that should be overexposed
  ************************************************************************/
void SONY_ExposureControlOver(SEQ_IpuHist_t *apHist, const uint32_t aOverExp);

/************************************************************************/
/** Contols exposure to achieve certain level of overexposed pixels.
  * 
  * \param  apHist         histogram data
  * \param  aOverExpPixCnt number of pixel that should be overexposed
  * \param  aOverExpLevel  level that should be treated as overexposed
  * \param  aExp           pointer to the new relative exposure update value
  *                        if 0 then the new value is updated automatically
  ************************************************************************/
void SONY_ExposureControlOver(const SEQ_IpuHist_t *const apHist,
			      const uint32_t aOverExpPixCnt, uint32_t aOverExpLevel,
			      uint32_t *const aExp);

/************************************************************************/
/** Contols exposure to achieve balanced brightnes.
  * 
  * \param  apHist   histogram data
  * \param  aOffset  offset from balanced brightnes <-0.5, 0.5>
  ************************************************************************/
void SONY_ExposureControlEqual(const SEQ_IpuHist_t *const apHist,
                                              float aOffset);

/************************************************************************/
/** Contols exposure to achieve balanced brightnes.
  * 
  * \param  apHist   histogram data
  * \param  aOffset  offset from balanced brightnes <-0.5, 0.5>
  * \param  aExp           pointer to the new relative exposure update value
  *                        if 0 then the new value is updated automatically
   ************************************************************************/
void SONY_ExposureControlEqual(const SEQ_IpuHist_t *const apHist,
                               float aOffset, uint32_t *const aExp);

/************************************************************************/
/** Computes histogram energy divided by number of pixels.
  * 
  * \param  apHist   histogram data
  * 
  * \return  relative histogram energy <0.0, 1.0>
  ************************************************************************/
float SONY_HistogramEnergyRelativ(const SEQ_IpuHist_t *const apHist);

/************************************************************************/
/** Computes Channel Gain values from histograms.
  * 
  * \param  apHist         histogram data
  * \param  aPixCntLevel   pixel count that is measured in histogram
  *                        to measure sensitivity
  * \param  aChGain        resulting levels to reach pixel count in
  *                        histogram
  * 
  ************************************************************************/
void SONY_ChGainMeasure(const SEQ_IpuHist_t *const apHist,
                        const uint32_t PixCntLevel,
                        SONY_ChGain_t *const aChGain);

#endif /* SONYPUBLIC_H */

/* EOF */