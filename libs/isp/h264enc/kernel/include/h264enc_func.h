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

/*==============================================================================
Revision History:
                        Modification       Tracking
Author (core ID)        Date D/M/Y         Number     Description of Changes
Roman Kubica            13-Oct-2016                   Init version

==============================================================================*/

#ifndef H264ENCFUNC_H
#define H264ENCFUNC_H

#ifdef __cplusplus
extern "C"{
#endif

/**
* @file           h264enc_func.h
* @brief          Declaration of H264 Encoder driver internal functionality
*/ 
    
/*==============================================================================
*                               INCLUDE FILES
==============================================================================*/
  
#include <linux/interrupt.h> 
#include "h264enc_types.h"
#include <linux/uwb/whci.h>

/*==============================================================================
*                                   CONSTANTS
==============================================================================*/

#ifdef __KERNEL__

/*==============================================================================
*                          DEFINES AND MACROS
==============================================================================*/

/*==============================================================================
*                                  ENUMS
==============================================================================*/

/*=============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
* ============================================================================*/

/*==============================================================================
*                        GLOBAL VARIABLE DECLARATIONS
==============================================================================*/

extern void __iomem    *h264enc_reg_base;

extern  H264ENC_REGS *reg_base;

/*==============================================================================
*                           FUNCTION PROTOTYPES
==============================================================================*/


/**
* @brief        Sets up the internal functionality of the H264 Encoder kernel 
*               module.
* @details      Sets up the internal functionality of the H264 Encoder kernel 
*               module.
*               Initializes H264 Encoder related resources. Returns 
*               H264ENC_DRV_SUCCESS
*               if all ok, otherwise H264ENC_DRV_FAILURE*
*/ 
int32_t H264ENC_Setup(void);


/**
* @brief        Releases all H264 Encoder related resources.
* @details      Releases all H264 Encoder related resources. Return 
*               H264ENC_DRV_SUCCESS 
*               if all ok, otherwise H264ENC_DRV_FAILURE. 
*/
void H264ENC_Close(void);


/**
* @brief        Sets parameters for encoding       
* @details      Sets parameters for encoding         
*/
void H264ENC_Config(const H264ENC_Config_t *apcConfig);


/**
* @brief        Sets specified buffer addresses to the register values.      
* @details      Sets up the input (Y, U, V) and output (bitstream data) buffers. 
* @param        apBuffers pointer to buffer defining structure   
*/
void H264ENC_BuffersSet(const H264ENC_Buffers_t *apcBuffers);

/**
* @brief        Provides current encoding status description.      
* @details      Provides current encoding status description.   
* @param        apStatus pointer to status describing structure  
*/
void H264ENC_StatusGet(H264ENC_Status_t *apcStatus);


/**
* @brief        Remembers more lines are ready in the input buffer.      
* @details      Remembers more lines are ready in the input buffer.
* @param        aMoreLines number of new lines in input buffer       
*/
void H264ENC_LinesFetch(uint32_t aMoreLines);


/**
* @brief        Shifts bitstream buffer alarm address.        
* @details      Should be used after the old data have been poped and can be 
*               overwritten. 
* @param        aAlarmShift number of new lines in input buffer     
*/
void H264ENC_BitstreamAlarmShift(uint32_t aAlarmShift);


/**
* @brief        Stops the encoder.     
* @details      Stops the encoder and resets the encoder core, input pixel
*               wrapper and
*               output stream wrapper.            
*/
void H264ENC_Stop(void);


/**
* @brief        Resets internal variable data.         
* @details      Resets internal variable data.     
*/
void H264ENC_ResetVars(void);

/******************************************************************************/

/**
* @brief        Provides infomation if complete macroblock has been encoded.        
* @details      Provides infomation if complete macroblock has been encoded.      
*/
uint8_t H264ENC_RowDoneStatusGet(void); 

/******************************************************************************/

/**
* @brief          Initializes H264Enc driver internal data.
* @details        Initializes H264Enc driver internal data.
*/

int32_t H264EncDrvDataInit(void);

/******************************************************************************/

/**
* @brief          Registers H264Enc IRQs.
* @details        Registers H264Enc IRQs.
*/

int32_t H264EncIrqsRequest(void);

/******************************************************************************/

/**
* @brief          Releases H264Enc IRQs.
* @details        Releases H264Enc IRQs.
*/

void H264EncIrqsFree(void);

/**
* @brief          UnMaps H264Enc register space.
* @details        UnMaps H264Enc register space.
*/

void H264EncUnmap(void);

/**
* @brief          Maps H264Enc register space
* @details        Maps H264Enc register space
*/

int32_t H264EncMap(void);

/**
* @brief          Enable H264 Encoder interrupts
* @details        Enable H264 Encoder interrupts
*/

void H264_ENC_enable_interrupts(void);

/**
* @brief         H264Enc IRQ handler
* @details       All H264Enc IRQs are orred to just one interrupt signal on 
*                s32v234. 
*/

irqreturn_t H264ENC_S32v234IrqHandler(int32_t aIrq, void *apDev_id);

/**
* @brief         MBRW_FF_OVF_INT handler
* @details       This interrupt is handled using separated interrupt signal
*/

irqreturn_t H264ENC_FF_OVF_IrqHandler(int32_t aIrq, void *apDev_id);

/**
* @brief         reg_read32
* @details       Function for register reading 
*                s32v234. 
*/

static inline uint32_t reg_read32(uint32_t *reg_addr)
{
    return ioread32(reg_addr);
}

/**
* @brief         reg_write32r
* @details       Function for register writing
*                s32v234. 
*/

static inline void reg_write32(uint32_t *reg_addr, uint32_t value)
{
    iowrite32(value, reg_addr);  
}

#ifdef __cplusplus
}
#endif

#endif /*ifdef __KERNEL__ */

#endif /* H264ENCFUNC_H */
