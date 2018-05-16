/*
 * Copyright (c) 2014-2016 Freescale Semiconductor
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

/*==============================================================================
Revision History:
                        Modification       Tracking
Author (core ID)        Date D/M/Y         Number     Description of Changes
Cristian Tomescu        03-Jan-2016                   Init version
Xuyen Dao               05-Aug-2016        VSDK-251   Update template
Tomas Babinec           15-Sep-2016        VSDK-325   Added register defines
Cristian Tomescu        04-Nov-2016        VSDK-488   Splitting the driver
Cristian Tomescu        08-May-2017        VSDK-560   Fix MISRA violations
==============================================================================*/

#ifndef VIULITELINUX_H
#define VIULITELINUX_H

#ifdef __cplusplus
extern "C"{
#endif

/**
* @file           viulite_linux.h
* brief           linux declarations for viu driver and user applications
*/

/*==============================================================================
*                               INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
#include "viulite_types.h"

/*==============================================================================
*                                   CONSTANTS
==============================================================================*/
#define IOCTL_APP_TYPE  'c'

/*==============================================================================
*                          DEFINES AND MACROS
==============================================================================*/
/* DMA Configuring */
#define VIULITE_IOCTL_DMA_CONFIG              _IOW(IOCTL_APP_TYPE, 1, uint32_t)

/* DMA Start */
#define VIULITE_IOCTL_DMA_START               _IO(IOCTL_APP_TYPE, 3)

/* DMA Stop */
#define VIULITE_IOCTL_DMA_STOP                _IO(IOCTL_APP_TYPE, 4)

/* DMA Get Status */
#define VIULITE_IOCTL_DMA_GET_STATUS          _IOR(IOCTL_APP_TYPE, 5, uint32_t)

/* Set Data Input Format */
#define VIULITE_IOCTL_SET_VIDEOIN_FORMAT      _IOW(IOCTL_APP_TYPE, 6, uint32_t)

/* Get Data Input Format */
#define VIULITE_IOCTL_GET_VIDEOIN_FORMAT      _IOR(IOCTL_APP_TYPE, 7, uint32_t)

/* Set Data Interface */
#define VIULITE_IOCTL_SET_DATA_INTERFACE      _IOW(IOCTL_APP_TYPE, 8, uint32_t)

/* Get Data Interface */
#define VIULITE_IOCTL_GET_DATA_INTERFACE      _IOR(IOCTL_APP_TYPE, 9, uint32_t)

/* Config Interrupt Status Flags */
#define VIULITE_IOCTL_CONFIG_IRQS             _IOW(IOCTL_APP_TYPE, 10, uint32_t)

/* Reset Interrupt Status Flags */
#define VIULITE_IOCTL_RESET_IRQSTATUS         _IOW(IOCTL_APP_TYPE, 11, uint32_t)

/* Get Interrupt Status Flags */
#define VIULITE_IOCTL_GET_IRQSTATUS           _IOR(IOCTL_APP_TYPE, 12, uint32_t)

/* Sw Reset */
#define VIULITE_IOCTL_SW_RESET                _IO(IOCTL_APP_TYPE, 13)

/* Set ITU Error Code */
#define VIULITE_IOCTL_EN_ITU_ERRCODE          _IOW(IOCTL_APP_TYPE, 14, uint32_t)

/* Get ITU Error Code */
#define VIULITE_IOCTL_GET_ITU_ERRCODE         _IOR(IOCTL_APP_TYPE, 15, uint32_t)

/* Get Field Number */
#define VIULITE_IOCTL_GET_FIELDNUM            _IOR(IOCTL_APP_TYPE, 16, uint32_t)

/* Get Sync Signals Status */
#define VIULITE_IOCTL_GET_SYNC                _IOR(IOCTL_APP_TYPE, 17, uint32_t)

/*Get Frame Size: lines and pixels */
#define VIULITE_IOCTL_GET_FRAME_SIZE          _IOR(IOCTL_APP_TYPE, 18, uint32_t)

/* Set Clipping Data */
#define VIULITE_IOCTL_SET_CLIPPING            _IOW(IOCTL_APP_TYPE, 19, uint32_t)

/* Get Clipping Data */
#define VIULITE_IOCTL_GET_CLIPPING            _IOR(IOCTL_APP_TYPE, 20, uint32_t)


/*==============================================================================
*                                     ENUMS
==============================================================================*/


/*==============================================================================
*                     STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/


/*==============================================================================
*                        GLOBAL VARIABLE DECLARATIONS
==============================================================================*/


/*==============================================================================
*                           FUNCTION PROTOTYPES
==============================================================================*/


#ifdef __cplusplus
}
#endif

#endif /* VIULITELINUX_H */

/** @} */
