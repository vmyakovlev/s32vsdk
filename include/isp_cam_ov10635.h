/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor
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
* \file     ov10635_public.h
* \brief    public declarations for Omnivion10635 user space library
* \author   Tomas Babinec
* \version  0.1
* \date     15-March-2016
* \note
****************************************************************************/

#ifndef OV10635PUBLIC_H
#define OV10635PUBLIC_H

#include "isp_csi.h"
#include "isp_viu.h"
#include "../libs/isp/cam_generic/kernel/include/ov10635_types.h"

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
/** Connect to OV10635 camera.
* 
* \param aViuIdx[in] index of the VIU interface to connect to.
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT OV10635_Open(const VIU_IDX aViuIdx);

/****************************************************************************/
/** Disconnect from OV10635 camera.
* 
* \param aViuIdx[in] index of the VIU interface to connect to.
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT OV10635_Close(const VIU_IDX aViuIdx);

/****************************************************************************/
/** Configures OV10635 camera registers.
* 
* \param apMessage[in] message to be sent
* 
* \return CAM_LIB_SUCCESS if all ok
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT OV10635_RegConfig(const VIU_IDX aViuIdx);

/****************************************************************************/
/** Enables VIU transmission from Sony camera.
 * 
 * \param apGraphPackage packed graph structure
 * 
 * \return CAM_LIB_SUCCESS if all ok
 *         CAM_LIB_FAILURE otherwise
 ****************************************************************************/
CAM_LIB_RESULT OV10635_DataEnable(const VIU_IDX aViuIdx);

/****************************************************************************/
/** Disables VIU transmission from Sony camera.
* 
* \return CAM_LIB_SUCCESS if all ok, 
*         CAM_LIB_FAILURE otherwise
*****************************************************************************/
CAM_LIB_RESULT OV10635_DataDisable(const VIU_IDX aViuIdx);

#endif /* OV10635PUBLIC_H */

/* EOF */