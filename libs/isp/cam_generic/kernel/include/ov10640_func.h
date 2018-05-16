/*
 * Copyright (c) 2015-2016 Freescale Semiconductor
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
 * \file    ov10640_func.hpp
 * \brief   Omnivision 10640 MIPI/CSI camera functionality declaration.
 * \author  Tomas Babinec
 * \version 0.1
 * \date    14-December-2015
 ****************************************************************************/

#ifndef OV10640FUNC_H
#define OV10640FUNC_H

#include "cam_types.h"
#include "ov10640_types.h"

#ifdef __cplusplus 
extern "C" { 
#endif




  
#define OV10640_DRV_SUCCESS CAM_DRV_SUCCESS
#define OV10640_DRV_FAILURE CAM_DRV_FAILURE  









/****************************************************************************/
/** Configures Ov10640 camera driver.
 * 
 * \param aClient client handle.
 *
 * \return OV10640_DRV_SUCCESS if all ok
 *         OV10640_DRV_FAILURE otherwise
 ****************************************************************************/
int32_t OV10640_DRV_Setup(const CamI2cClient_t aClient);

/****************************************************************************/
/** Terminate Ov10640 camera driver existance.
 * 
 * \param aClient client handle.
 * 
 * \return OV10640_DRV_SUCCESS if all ok
 *         OV10640_DRV_FAILURE otherwise
 ****************************************************************************/
int32_t OV10640_DRV_Close(const CamI2cClient_t aClient);

#ifdef __cplusplus 
} 
#endif

#endif /* OV10640FUNC_H */
