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

#ifndef CSIFUNC_H
#define CSIFUNC_H

#ifdef __cplusplus 
extern "C" { 
#endif

/**
* @file           csi_func.h
* @brief          Declaration of H264 Encoder driver internal functionality
*/ 
     
#include "csi_types.h"

/*==============================================================================
*                                   CONSTANTS
==============================================================================*/

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

/*==============================================================================
*                           FUNCTION PROTOTYPES
==============================================================================*/

/**
* @brief        Initializes CSI driver to use the HW.                
* @details      Sets up the internal functionality of the CSI
*      
* @param[in]    appCsiDrvInfo array of pointers to driver data structures
* 
* @return       CSI_DRV_SUCCESS if all ok,
*               CSI_DRV_FAILURE otherwise 
* 
*/ 
int32_t CSI_DRV_Setup(CSI_DrvInfo_t **appCsiDrvInfo);


/**
* @brief        Deinitialize the driver.               
* @details      Deinitialize the driver
*   
*/ 
void CSI_DRV_Close(void);  
  

/**
* @brief        Configures MIPI-CSI2 receiver registers.               
* @details      Configures MIPI-CSI2 receiver registers.
*  
* @return       apcCsiCfg Mipi Csi-2 config structure
* 
*/ 
void CSI_DRV_Config(const CSI_Cfg_t *apcCsiCfg);


/**
* @brief        Configures MIPI-CSI2 embeded data reception.             
* @details      Configures MIPI-CSI2 embeded data reception.
*  
* @param        apcCfg embedded data config structure
* 
*/ 
void CSI_DRV_EmbdConfig(const CSI_EmbdCfg_t *apcCfg);


/**
* @brief        Requests embedded data to be recaptured.         
* @details      Requests embedded data to be recaptured.
*  
* @param        apcCfg index of the CSI interface to capture the data from
* 
*/ 
void CSI_DRV_EmbdRecap(const enum CSI_IDX acCsiIdx);


/**
* @brief        Gets status of embedded data capturing.        
* @details      Gets status of embedded data capturing.
*  
* @param        acCsiIdx index of the CSI interface to capture the data from
* 
* @returns      return CSI_EMBD_STATUS_NA if nothing available
*               combination of flags CSI_EMBD_STATUS_1 and/or CSI_EMBD_STATUS_2
*               if some data captured
*/ 
uint32_t CSI_DRV_EmbdStatusGet(const enum CSI_IDX acCsiIdx);


/**
* @brief        Gets latest error status.
* @details      Gets latest error status.
*  
* @param        acCsiIdx index of the CSI interface to capture the data from
* 
* @returns      pointer to error status structure
* 
*/ 
CSI_ErrStatus_t* CSI_DRV_ErrStatusGet(const enum CSI_IDX acCsiIdx);


/**
* @brief        Clear all so far captured errors.
* @details      Clear all so far captured errors.
*  
* @param        acCsiIdx index of the CSI interface to capture the data from
* 
* @returns      pointer to error status structure
* 
*/ 
void CSI_DRV_ErrClear(const enum CSI_IDX acCsiIdx);


/**
* @brief        Disables MIPI-CSI2 receiver operation.
* @details      Disables MIPI-CSI2 receiver operation.
*  
* @param        aCsiIdKey Identifies CSI interface channels to Configures
* 
* 
*/ 
void CSI_DRV_Stop(CSI_IdKey_t aCsiIdKey);


/**
* @brief        Turns on the MIPI receiver functionality.
* @details      Turns on the MIPI receiver functionality.
*  
* @param        aCsiIdKey Identifies CSI interface channels to Configures
* 
* MIPI CSI receiver should be enabled before the cam starts to generate data.
*/ 
void CSI_DRV_Start(CSI_IdKey_t aCsiIdKey);


/**
* @brief        Prints content of CSI status registers.
* @details      Prints content of CSI status registers.
*  
* @param        aCsiIdx   index of CSI interface (valid 0 & 1)
* 
* 
*/ 
void CSI_DRV_StatusPrint(enum CSI_IDX aCsiIdx);


/**
* @brief        Registers new IRQ handler for particular CSI interface.
* @details      Registers new IRQ handler for particular CSI interface.
*  
* @param        aCsiIdx   index of CSI interface (valid 0 & 1)
* 
* 
*/ 
void CSI_IrqHandlerRegister(enum CSI_IDX aCsiIdx, 
                            void(*apIrqHandler)(enum CSI_IDX aCsiIdx, 
                                                uint32_t aIrqType)
                           );


static inline uint32_t HSSETL_COMPUTE(uint32_t freq);

static inline uint32_t CSI_HSSETL_MIN(uint32_t freq);

static inline uint32_t CSI_HSSETL_MAX(uint32_t freq);

static inline uint32_t CSI_UI(uint32_t freq);

/*==============================================================================
*                         INLINE FUNCTIONS
==============================================================================*/

static inline uint32_t HSSETL_COMPUTE(uint32_t freq)
{
  uint32_t tmp_freq = freq;
  uint32_t ret_freq = 0U;
  uint32_t tmp_min = CSI_HSSETL_MIN(tmp_freq);
  uint32_t tmp_max = CSI_HSSETL_MAX(tmp_freq);
  
  ret_freq = ((tmp_min + tmp_max)/30U) -1U;
  
  return ret_freq;
}

static inline uint32_t CSI_HSSETL_MIN(uint32_t freq)
{
  uint32_t ui_freq = CSI_UI(freq);
  uint32_t min = 85U + 6U * ui_freq;
  return min;
}

static inline uint32_t CSI_HSSETL_MAX(uint32_t freq)
{
  uint32_t ui_freq = CSI_UI(freq);
  uint32_t max = 145U + (10U * ui_freq);
  return max;
}

static inline uint32_t CSI_UI(uint32_t freq)
{
  return 1000U/freq;
}

#ifdef __cplusplus 
} //extern "C" 
#endif

#endif /* CSIFUNC_H */
