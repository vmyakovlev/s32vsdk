/*
 * Copyright (c) 2016 Freescale Semiconductor
 * Copyright 2017 NXP
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
 * \file    sony_func.hpp
 * \brief   Sony IMX224MQV MIPI/I2C bus camera functionality declaration.
 * \author  Tomas Babinec
 * \version 0.1
 * \date    31-August-2015
 ****************************************************************************/

#ifndef SONYFUNC_H
#define SONYFUNC_H

#include "config.h"
#include "cam_types.h"
#include "csi_types.h"
#include "sony_types.h"

#ifdef __cplusplus 
extern "C" { 
#endif




  
#define SONY_DRV_SUCCESS CAM_DRV_SUCCESS
#define SONY_DRV_FAILURE CAM_DRV_FAILURE  








  
/****************************************************************************/
/** Configures Sony camera driver.
 * 
 * \param aClient handle to the camera provided by generic driver.
 * 
 * \return SONY_DRV_SUCCESS if all ok
 *         SONY_DRV_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_DRV_Setup(const CamI2cClient_t aClient);

/****************************************************************************/
/** Terminate Sony camera driver existance.
 * 
 * \param aClient handle to the camera provided by generic driver.
 * 
 * \return SONY_DRV_SUCCESS if all ok
 *         SONY_DRV_FAILURE otherwise
 ****************************************************************************/
int32_t SONY_DRV_Close(const CamI2cClient_t aClient);

/****************************************************************************/
/** Irq handler for particular I2C bus index.
 *
 * Once registered in I2C bus driver, called each time there is a I2C bus IRQ 
 * event. For Sony used to track exposure setup state.
 * 
 * \param aCsiIdx I2C bus interface index the camera is connected to.
 * \param aIrqType  pointer to exoposure structure 
 * 
 ****************************************************************************/
void SONY_DRV_IrqHandler(enum CSI_IDX aCsiIdx, uint32_t aIrqType);

/****************************************************************************/
/* Gets the camera exposure setup state.
 *
 * Intended for check if exposure can be updated in cameras.
 * 
 * \param aCsiIdx I2C bus interface index the camera is connected to.
 * 
 * \return SONY_DRV_EXP_SENT,      < exposure update sent through i2c
 *         SONY_DRV_EXP_APPLIED,   < exposure update applied to registers
 *         SONY_DRV_EXP_USED,      < exposure update used for latest frame
 *         SONY_DRV_EXP_READY,     < exposure ready to be updated again
 ****************************************************************************/
SONY_ExpState_t SONY_DRV_ExpStateGet(enum CSI_IDX aCsiIdx);

/****************************************************************************/
/** Sets the driver internal exposure state to SENT.
 * 
 * \param aCsiIdx I2C bus interface index the camera is connected to.
 ****************************************************************************/
void SONY_DRV_ExpSent(enum CSI_IDX aCsiIdx);


#ifdef __cplusplus 
} 
#endif

#endif /* SONYFUNC_H */ 
