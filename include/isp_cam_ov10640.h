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
* \file     ov10640_public.h
* \brief    public declarations for Omnivion10640 user space library
* \author   Tomas Babinec
* \version  0.1
* \date     14-December-2014
* \note
****************************************************************************/

#ifndef OV10640PUBLIC_H
#define OV10640PUBLIC_H

#include "isp_cam_generic.h"
#include "../libs/isp/cam_generic/kernel/include/ov10640_types.h"

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
/** Connect to the camera HW.
* 
* \param aCsiIdx[in] Csi interface the camera is connected to
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT OV10640_Open(CSI_IDX aCsiIdx);

/****************************************************************************/
/** Disconnect from the camera HW.
* 
* \param aCsiIdx[in] Csi interface the camera is connected to
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT OV10640_Close(CSI_IDX aCsiIdx);

/****************************************************************************/
/** Configures Ov10640 camera registers.
* 
* \param aCsiIdx[in] Csi interface the camera is connected to
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT OV10640_RegConfig(CSI_IDX aCsiIdx);

/****************************************************************************/
/** Connect to the camera HW.
* 
* \param aViuIdx[in] Viu interface the camera is connected to
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT OV10640_Open(VIU_IDX aViuIdx);

/****************************************************************************/
/** Disconnect from the camera HW.
* 
* \param aViuIdx[in] Viu interface the camera is connected to
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT OV10640_Close(VIU_IDX aViuIdx);

/****************************************************************************/
/** Configures Ov10640 camera registers.
* 
* \param aViuIdx[in] Viu interface the camera is connected to
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT OV10640_RegConfig(VIU_IDX aViuIdx);

/****************************************************************************/
/** Enables data transmission from OV10640 camera.
 * 
 * \param aViuIdx which VIU index the camera is connected to
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
CAM_LIB_RESULT OV10640_DataEnable(VIU_IDX aViuIdx);

/****************************************************************************/
/** Disables data transmission from OV10640 camera.
* 
* \param aViuIdx which VIU index the camera is connected to
* 
* \return CAM_LIB_SUCCESS if all ok, 
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT OV10640_DataDisable(VIU_IDX aViuIdx);

/****************************************************************************/
/** Configures Ov10640 camera for 3x12bit output mode (raw12)
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 ****************************************************************************/
void OV10640_Mode3x12(CSI_IDX aCsiIdx);

/****************************************************************************/
/** Configures Ov10640 to do AEC and apply AWB.
 * 
 * \param aCsiIdx CSI interface index the camera is connected to.
 ****************************************************************************/
void OV10640_AEC_WB_On(CSI_IDX aCsiIdx);

/****************************************************************************/
/** Configures Ov10640 channel gain related registers.
 * 
 * \param arExp reference to exposure parameters defining structure.
 ****************************************************************************/
void OV10640_ChGain(OV10640_Exposure_t &arExp);

/****************************************************************************/
/** Enables CSI transmission from OV10640 camera.
 * 
 * \param aCsiIdx which CSI interface the camera is connected to
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
CAM_LIB_RESULT OV10640_CsiEnable(CSI_IDX aCsiIdx);

/****************************************************************************/
/** Disables CSI transmission from OV10640 camera.
*
* \param aCsiIdx which CSI interface the camera is connected to
* 
* \return CAM_LIB_SUCCESS if all ok, 
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT OV10640_CsiDisable(CSI_IDX aCsiIdx);

/****************************************************************************/
/** Configures geometry setting to OV10640 camera.
*
* \param  arGeom reference to structure with geometry parameters
* 
* \return CAM_LIB_SUCCESS if all ok, 
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT OV10640_GeometrySet(OV10640_Geometry_t &arGeom);

/****************************************************************************/
/** Reads Exposure settings from OV10640 camera.
*
* \param  arExp reference to structure with exposure parameters
* 
* \return CAM_LIB_SUCCESS if all ok, 
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT OV10640_ExposureGet(OV10640_Exposure_t &arExp);

/****************************************************************************/
/** Configures Exposure parameters to OV10640 camera.
*
* \param  arExp reference to structure with exposure parameters
* 
* \return CAM_LIB_SUCCESS if all ok, 
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT OV10640_ExposureSet(OV10640_Exposure_t &arExp);

/****************************************************************************/
/** Configures operational mode to OV10640 camera.
*
* \param  arGeom reference to structure with geometry parameters
* 
* \return CAM_LIB_SUCCESS if all ok, 
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
void OV10640_ModeSet(OV10640_Geometry_t &arGeom);

/****************************************************************************/
/** Gets from the camera the exposure time of one exposure (VS, S or L).
*
* \param  arExp  exposure parameters structure used to save the time.
* \param  aExpT  type of exposure for which the exposure time is get.
* 
* \return void. 
*****************************************************************************/
void OV10640_ExposureTimeGet(OV10640_Exposure_t &arExp,
                                     OV10640_ExpTime_t aExp);

/****************************************************************************/
/** Sets the exposure time for one exposure(VS, S or L).
*
* \param  arExp  exposure parameters structure contain the new exposure time.
* \param  aExpT  type of exposure for which the exposure time is set.
* 
* \return void.
*****************************************************************************/
void OV10640_ExposureTimeSet(OV10640_Exposure_t &arExp,
                                    OV10640_ExpTime_t aExp);

/****************************************************************************/
/** Gets from the camera the analogue and conversion gain of the 3 exposures.
*
* \param  arExp  exposure parameters structure used to save the analogue 
*                                      and conversion gain configuration.
* 
* \return void. 
*****************************************************************************/
void OV10640_ExposureAnalogConversionGainGet(OV10640_Exposure_t &arExp);

/****************************************************************************/
/** Sets the analogue and conversion gain of the 3 exposures(S,VS and L).
*
* \param  arExp  exposure parameters structure containing the new 
*                             analogue and conversion gain configuration.
* 
* \return void.
*****************************************************************************/
void OV10640_ExposureAnalogConversionGainSet(OV10640_Exposure_t &arExp);

/****************************************************************************/
/** Gets from the camera the digital gain for one exposure(VS, S or L).
*
* \param  arExp  exposure parameters structure used to save the digital gain.
* \param  aExpT  type of exposure for which the digital gain value is get.
* 
* \return void 
*****************************************************************************/
void OV10640_ExposureDigitalGainGet(OV10640_Exposure_t &arExp,
                                              OV10640_ExpTime_t aExp);
/****************************************************************************/
/** Sets the digital gain for one exposure(VS, S or L).
*
* \param  arExp   exposure parameter structure contain new digital gain value.
* \param  aExpT   type of exposure for which the digital gain is set.
* 
* \return void.
*****************************************************************************/
void OV10640_ExposureDigitalGainSet(OV10640_Exposure_t &arExp,
                                               OV10640_ExpTime_t aExp);

/****************************************************************************/
/** Gets the exposure parameters(time and gains) for one exposure(VS, S or L).
*
* \param  arExp  exposure parameters structure used to save the parameters.
* \param  aExpT type of exposure for which to get the parameters.
* 
* \return void.
*****************************************************************************/
void OV10640_ExposureAllExpGet(OV10640_Exposure_t &arExp,
                                         OV10640_ExpTime_t aExp);

/****************************************************************************/
/** Sets the exposures parameters(time and gains) for one exposure(VS, S or L).
*
* \param  arExp   exposure parameter structure contain the new parameter value
* \param  aExpT   type of exposure for which the parameters are set
* 
* \return void 
*****************************************************************************/
void OV10640_ExposureAllExpSet(OV10640_Exposure_t &arExp,
                                         OV10640_ExpTime_t aExp);

/****************************************************************************/
/** Computes the total exposure(exposure time, analogue gain, conversion gain,
*                  digital gain) for one of the exposure types(VS, S or L).
* \param  arExp  exposure parameter structure contain the parameters.
* \param  aExp   type of exposure for which the parameters are computed.
* 
* \return void.
*****************************************************************************/
void OV10640_ExposureTotal(OV10640_Exposure_t &arExp, 
                              OV10640_ExpTime_t aExp);

/****************************************************************************/
/** Compute the exposures parameters for the 3 exposure types: 
*        - based on total exposure value, the exposure time and the various gain
*          are computed for each exposure.
*        - exposure time first is maximised, then conversion gain, then 
*          analogue gain and in last the digital gain.
*        - S exposure parameters are computed to maintain the ratio with L.
*
* \param  arExp   exposure parameter structure contain the exposure parameters.
* \param  arNewLExpTotal    total exposure for L.
* \param  arNewVSExpTotal	total exposure for S
* 
* \return void.
*****************************************************************************/
void OV10640_ExposureComputationInd(OV10640_Exposure_t &arExp,
                                        float arNewLExpTotal,
                                        float arNewVSExpTotal);

/****************************************************************************/
/** Compute the correction factor to be applied to the total exposure 
* based on histogram data 
*
* \param  apHist		    pointer to the histogram
* \param  aPixCntOverExP	over exposed pixel number target
* \param  aBinOverExpLevel	over exposed level (bin number in histogram)
* 
* \return                   the correction to apply to the exposure
*****************************************************************************/
float OV10640_ExposureCorrection( uint32_t *apHist, 
                                  uint32_t aPixCntOverExp, 
                                  uint8_t  aBinOverExpLevel);

/****************************************************************************/
/** Writes a value over I2C to Ov10640 camera register.
* 
* \param aCsiIdx[in]  Csi interface the camera is connected to
* \param aRegAddr[in] address of the register
* \param aVal[in]     value to be written
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT OV10640_RegWrite( CSI_IDX  aCsiIdx, 
                                 uint16_t aRegAddr,
                                 uint8_t  aVal );

/****************************************************************************/
/** Reads a value over I2C from Ov10640 camera register.
* 
* \param aCsiIdx[in]  Csi interface the camera is connected to
* \param aRegAddr[in] address of the register
* \param aVal[out]    value read
* 
* \return CSI_LIB_SUCCESS if all ok
*         CSI_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT OV10640_RegRead(CSI_IDX  aCsiIdx, 
                            uint16_t aRegAddr,
                            uint8_t &arVal
                           );


#endif /* OV10640_PUBLIC_H */


/* EOF */
