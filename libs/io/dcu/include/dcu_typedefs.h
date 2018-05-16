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
*   @file    typedefs.h
*
*   @brief   This files defines data types used in the header files.
*   @details Types definitions used in the driver.
*/
/*==================================================================================================
*   (c) Copyright 2014 Freescale Semiconductor, Inc
*   All Rights Reserved.
==================================================================================================*/
/*==================================================================================================
Revision History:
                             Modification     Tracking
Author (core ID)              Date D/M/Y       Number     Description of Changes
---------------------------   ----------    ------------  ------------------------------------------
Cristian Tomescu (B13031)     21/03/2014    ENGR00298226  First version of the updated 2D-ACE driver.
---------------------------   ----------    ------------  ------------------------------------------
Cristian Tomescu (B13031)     20/05/2014    ENGR00307868  Review and update the 2D-ACE driver.
---------------------------   ----------    ------------  ------------------------------------------
==================================================================================================*/
#ifndef DCUTYPEDEFS_H
#define DCUTYPEDEFS_H

#include "dcu_cfg.h"

#if ( DRV_BARE_METAL == DCU_DRV_VARIANT )
  #include <stdint.h>
#endif /* DCU_DRV_VARIANT == DRV_BARE_METAL */

#if ( DRV_LINUX_OS == DCU_DRV_VARIANT )
  #include <linux/module.h>
  #include <linux/kernel.h>
#endif /* DCU_DRV_VARIANT == DRV_LINUX_OS */
    
    /* Standard typedefs used by header files, based on ISO C standard */
    typedef volatile int8_t vint8_t;
    typedef volatile uint8_t vuint8_t;

    typedef volatile int16_t vint16_t;
    typedef volatile uint16_t vuint16_t;

    typedef volatile int32_t vint32_t;
    typedef volatile uint32_t vuint32_t;
    
#endif /* DCUTYPEDEFS_H */
