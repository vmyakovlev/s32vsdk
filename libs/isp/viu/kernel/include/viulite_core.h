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
Cristian Tomescu        04-Nov-2016       VSDK-488      Initial version
Cristian Tomescu        22-Dec-2016       VSDK-317     MISRA 2012 compliant.
Cristian Tomescu        25-Jan-2017       VSDK-709     Adding compiling switch.
Cristian Tomescu        08-May-2017       VSDK-560     Fix MISRA violations
==============================================================================*/
#ifndef VIULITECORE_H
#define VIULITECORE_H

#ifdef __cplusplus
extern "C"{
#endif

/**
* @file           viulite_core.h
* brief           containes the hw defines: register addresses and masks
*                 and the core driver functions declarations
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


/*==============================================================================
*                          DEFINES AND MACROS
==============================================================================*/
#define VIU0_BASEADDR           ((VIU_Registers_t *const)(0x40064000))
#define VIU1_BASEADDR           ((VIU_Registers_t *const)(0x400D0000))

/*  VIULITE Registers Masks */
#define SCR_VSYNC_OFFSET        ((uint32_t)30)
#define SCR_VSYNC_MASK          ((uint32_t)1 << SCR_VSYNC_OFFSET)
#define SCR_HSYNC_OFFSET        ((uint32_t)29)
#define SCR_HSYNC_MASK          ((uint32_t)1 << SCR_HSYNC_OFFSET)
#define SCR_FIELDNO_OFFSET      ((uint32_t)28)
#define SCR_FIELDNO_MASK        ((uint32_t)1 << SCR_FIELDNO_OFFSET)
#define SCR_ACT_OFFSET          ((uint32_t)27)
#define SCR_DMA_ACT_MASK        ((uint32_t)1 << SCR_ACT_OFFSET)
#define SCR_ITUMODE_OFFSET      ((uint32_t)13)
#define SCR_ITUMODE_MASK        ((uint32_t)1 << SCR_ITUMODE_OFFSET)
#define SCR_CPP_OFFSET          ((uint32_t)11)
#define SCR_CPP_MASK            ((uint32_t)3 << SCR_CPP_OFFSET)
#define SCR_INWIDTH_OFFSET      ((uint32_t)8)
#define SCR_INWIDTH_MASK        ((uint32_t)7 << SCR_INWIDTH_OFFSET)
#define SCR_PCLK_POL_OFFSET     ((uint32_t)6)
#define SCR_PCLK_POL_MASK       ((uint32_t)1 << SCR_PCLK_POL_OFFSET)
#define SCR_VSYNC_POL_OFFSET    ((uint32_t)5)
#define SCR_VSYNC_POL_MASK      ((uint32_t)1 << SCR_VSYNC_POL_OFFSET)
#define SCR_HSYNC_POL_OFFSET    ((uint32_t)4)
#define SCR_HSYNC_POL_MASK      ((uint32_t)1 << SCR_HSYNC_POL_OFFSET)
#define SCR_ECCEN_OFFSET        ((uint32_t)2)
#define SCR_ECCEN_MASK          ((uint32_t)1 << SCR_ECCEN_OFFSET)
#define SCR_ENDIANNESS_OFFSET   ((uint32_t)1)
#define SCR_ENDIANNESS_MASK     ((uint32_t)1 << SCR_ENDIANNESS_OFFSET)
#define SCR_SWRESET_MASK        ((uint32_t)1)
#define SCR_VIDEOIN_MASK        (SCR_INWIDTH_MASK | SCR_ITUMODE_MASK | \
                                SCR_CPP_MASK)

#define INTR_ITUERR_OFFSET      ((uint32_t)28)
#define INTR_ITUERR_MASK        ((uint32_t)0x0F << INTR_ITUERR_OFFSET)
#define INTR_STATUS_OFFSET      ((uint32_t)16)
#define INTR_STATUS_MASK        ((uint32_t)0xFF << INTR_STATUS_OFFSET)
#define INTR_FIELD_MASK         (uint32_t)(0x00010000)
#define INTR_VSYNC_MASK         (uint32_t)(0x00020000)
#define INTR_HSYNC_MASK         (uint32_t)(0x00040000)
#define INTR_VSTART_MASK        (uint32_t)(0x00080000)
#define INTR_DMA_END_MASK       (uint32_t)(0x00100000)
#define INTR_ERR_MASK           (uint32_t)(0x00200000)
#define INTR_LINE_END_MASK      (uint32_t)(0x00400000)
#define INTR_FRAME_END_MASK     (uint32_t)(0x00800000)

#define INTR_ENABLE_MASK        (uint32_t)(0x000000FF)
#define INTR_FIELD_EN           (uint32_t)(0x00000001)
#define INTR_VSYNC_EN           (uint32_t)(0x00000002)
#define INTR_HSYNC_EN           (uint32_t)(0x00000004)
#define INTR_VSTART_EN          (uint32_t)(0x00000008)
#define INTR_DMA_END_EN         (uint32_t)(0x00000010)
#define INTR_ERR_EN             (uint32_t)(0x00000020)
#define INTR_LINE_END_EN        (uint32_t)(0x00000040)
#define INTR_FRAME_END_EN       (uint32_t)(0x00000080)

#define VIU_HIGH16_MASK         (0xFFFF0000U)
#define VIU_LOW16_MASK          (0x0000FFFFU)

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
void viulite_set_videoinputformat(VIU_Registers_t *const reg_base,
                                  const VIU_INPUT_FORMAT *const in_format);
void viulite_get_videoinputformat(const VIU_Registers_t *const reg_base,
                                  VIU_INPUT_FORMAT *const in_format);
void viulite_set_datainterface(VIU_Registers_t *const reg_base,
                               const VIU_DATA_INTERFACE *const data_interface);
void viulite_get_datainterface(const VIU_Registers_t *const reg_base,
                               VIU_DATA_INTERFACE *const data_interface);
void viulite_dma_config(VIU_Registers_t *const reg_base,
                        const DMA_CONFIG *const dmaconfig);
void viulite_dma_start(VIU_Registers_t *const reg_base);
void viulite_dma_stop(VIU_Registers_t *const reg_base);
void viulite_dma_getstatus(const VIU_Registers_t *const reg_base,
                           VIU_BOOL *const dmastatus);
void viulite_sw_reset(VIU_Registers_t *const reg_base);
void viulite_enable_ituerror(VIU_Registers_t *const reg_base,
                             const VIU_BOOL itu_errset);
void viulite_get_ituerror(const VIU_Registers_t *const reg_base,
                          VIU_ITU_ERR *const itu_errcode);
void viulite_enable_irqs(VIU_Registers_t *const reg_base,
                         const uint8_t irq_mask);
void viulite_get_irqstatus(const VIU_Registers_t *reg_base,
                           uint8_t *const intr_status);
void viulite_reset_irqstatus(VIU_Registers_t *const reg_base,
                             const uint8_t intr_status);
void viulite_get_syncsignals(const VIU_Registers_t *const reg_base,
                             VIU_SYNC_STATUS *const sync_status);
void viulite_get_fieldnum(const VIU_Registers_t *const reg_base,
                          uint8_t *const field_n);
void viulite_get_framesize(const VIU_Registers_t *const reg_base,
                           VIU_FRAME_SIZE *const frame_params);
void viulite_set_clippingdata(VIU_Registers_t *const reg_base,
                              const VIU_IMAGE_PARAMS *const image_params);
void viulite_get_clippingdata(const VIU_Registers_t *const reg_base,
                              VIU_IMAGE_PARAMS *const image_params);
#ifndef __LINUX__
void VIU0_Isr(void);
void VIU1_Isr(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* VIULITECORE_H */

/** @} */
