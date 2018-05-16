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
* \file     cam_public.h
* \brief    public declarations for Generic CAM user space library
* \author   Tomas Babinec
* \version  0.1
* \date     13-June-2016
* \note
****************************************************************************/

#ifndef CAMPUBLIC_H
#define CAMPUBLIC_H

#include "../libs/isp/cam_generic/kernel/include/cam.h"

//*****************************************************************************
// constants
//*****************************************************************************

#define EVTLOG_STRING_SIZE   (10*10*750) // 10 col * 10 char/col * 750 line/frame 

//*****************************************************************************
// types
//*****************************************************************************

/****************************************************************************/
/** Sequencer user space library return values.
 * 
 ****************************************************************************/
enum CAM_LIB_RESULT
{
  CAM_LIB_SUCCESS = 0,
  CAM_LIB_FAILURE = 1
}; // CAM_LIB_RESULT

//*****************************************************************************
// public functionality declarations
//*****************************************************************************

/****************************************************************************/
/** Prepares the CAM driver to be used.
 *
 * In Linux the cam special device file is opened.
 * In standalone environment the CAM_SetUp() function is called directly to
 * initialize the internal driver structures and CAM HW.
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
CAM_LIB_RESULT CAM_Open(void);

/****************************************************************************/
/** Deinitializes the CAM driver use.
 * 
 * In Linux the cam special device file is closed.
 * In standalone environment the CAM_Close() function is called directly to
 * release internal driver resources and disconnect from the HW.
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE if the close fails
 ****************************************************************************/
CAM_LIB_RESULT CAM_Close(void);

/****************************************************************************/
/** Add Generic camera client.
 * 
 * \param arCamCfg[in] Specifies camera properties (i2c bus index, address)
 * 
 * \return camera handle if all ok
 *         0 otherwise
 ****************************************************************************/
CamI2cClient_t CAM_ClientAdd(CamI2cCfg_t &arCamCfg);

/****************************************************************************/
/** Remove Generic camera client.
 * 
 * \param aCamHandle[in] Identifies Camera client instance.
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
CAM_LIB_RESULT CAM_ClientRem(CamI2cClient_t aCamHandle);

/****************************************************************************/
/** Executes batch of I2C read commands.
 * 
 * \param arBatch[in/out] Describes the read commands.
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
CAM_LIB_RESULT CAM_ReadBatch(I2cReadBatch_t &arBatch);

/****************************************************************************/
/** Executes batch of I2C write commands.
 * 
 * \param arBatch[in] Describes the write commands.
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
CAM_LIB_RESULT CAM_WriteBatch(I2cWriteBatch_t &arBatch);

/****************************************************************************/
/** Read a single register value over I2C.
 * 
 * \param aHandle[in]  handle to device to read from.
 * \param aRegAddr[in] register address to read from.
 *
 * \return read value if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
int32_t CAM_ReadSingle(const CamI2cClient_t aHandle, const uint16_t aRegAddr);

/****************************************************************************/
/** Read a single register value over I2C.
 * 
 * \param aHandle[in]  handle to device to read from.
 * \param aRegAddr[in] register address to read from.
 * \param arVal[out]   read value
 *
 * \return CAM_LIB_SUCCESS,
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
CAM_LIB_RESULT CAM_ReadSingle(const CamI2cClient_t aHandle, 
                              const uint16_t aRegAddr, 
                              uint8_t  &arVal);

/****************************************************************************/
/** Write a single register value over I2C
 * 
 * \param aHandle[in]  handle to device to write to.
 * \param aRegAddr[in] register address to write to
 * \param aVal[in]     value to be writen to register
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
CAM_LIB_RESULT CAM_WriteSingle(const CamI2cClient_t aHandle, 
                               const uint16_t       aRegAddr,
                               const uint8_t        aVal
                              );

#endif /* CAMPUBLIC_H */

//****************************************************************************
/* EOF */