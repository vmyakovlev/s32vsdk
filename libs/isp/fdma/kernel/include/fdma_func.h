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
/**
* @file     fdma_func.h
* @brief    declaration of fDMA driver internal functionality
****************************************************************************/

/*==============================================================================
Revision History:
                        Modification      Tracking
Author (core ID)        Date D/M/Y         Number     Description of Changes
Tomas Babinec           27-May-2014                    Inital version.
Tomas Babinec           02-May-2017       vsdk-575     Tracebility.
Tomas Babinec           25-May-2017       vsdk-575     Misra.
==============================================================================*/

#ifndef FDMAFUNC_H
#define FDMAFUNC_H

#if !defined(__STANDALONE__)
  #include <linux/interrupt.h>
#else  // if !defined(__STANDALONE__)
  #include <stdio.h>
  #include <stdint.h>
  #include <string.h>
#endif // else from if !defined(__STANDALONE__)

#include "fdma_types.h"

#ifdef __cplusplus 
extern "C" { 
#endif

#if defined(__KERNEL__) || defined(__STANDALONE__)

/*****************************************************************************
* functions (prototypes)
*****************************************************************************/

/****************************************************************************/
/** Sets up the internal functionality of the fdma kernel module. Initializes
*   fdma related resources.
* 
* @param[in]  apFdmaDrvInfo pointer to driver data structure
* 
* @return FDMA_DRV_SUCCESS if all ok, otherwise FDMA_DRV_FAILURE
* 
* @implements FDMA_Setup_Activity
*
*****************************************************************************/
int32_t FDMA_Setup(FDMA_DrvInfo_t *apFdmaDrvInfo);

/****************************************************************************/
/** Releases all fdma related resources.
* 
* @return FDMA_DRV_SUCCESS if all ok, otherwise FDMA_DRV_FAILURE
* 
* @implements FDMA_Close_Activity
* 
*****************************************************************************/
void FDMA_Close(void);

/****************************************************************************/
/** Handles events signaled from Sequencer driver.
* 
* @param apcEvent  event describing structure
* 
* @return FDMA_DRV_SUCCESS if all ok
*         FDMA_DRV_FAILURE otherwise
* 
* @implements FDMA_SEqEventHandler_Activity
*
*****************************************************************************/
int32_t FDMA_SeqEventHandler(const FDMA_TcEvent_t *apcEvent);

/****************************************************************************/
/** Initializes the double linked list for TDMs.
*
* Has to be called before the list is used. Sets the list as empty and resets
* all nodes in the node pool. 
* 
* @param apTdmList pointer to a list structure to be initialized.
*****************************************************************************/
void FDMA_TdmListInit(FDMA_TdmList_t *apTdmList);

/****************************************************************************/
/** Pushes TDM structure to the begining of the list.
*
* @param apTdmList pointer to a list structure to push the TDM to.
* @param apTdm pointer to TDM structure to be pushed.
*
* @return FDMA_DRV_SUCCESS if all ok, otherwise FDMA_DRV_FAILURE
*****************************************************************************/
int32_t FDMA_TdmListPushFront(FDMA_TdmList_t *apTdmList, 
                              FDMA_Tdm_t *apTdm);

/****************************************************************************/
/** Pushes TDM structure to the end of the list..
*
* @param apTdmList pointer to a list structure to push the TDM to.
* @param apTdm pointer to TDM structure to be pushed.
*
* @return FDMA_DRV_SUCCESS if all ok, otherwise FDMA_DRV_FAILURE
*****************************************************************************/
int32_t FDMA_TdmListPushBack(FDMA_TdmList_t *apTdmList, 
                             FDMA_Tdm_t *apTdm);

/****************************************************************************/
/** Removes the a TDM from the beginning of the list.
* 
* @param apTdmList pointer to a list structure to be initialized.
* 
* @return pointer to the poped TDM structure, NULL if list was empty
*
*****************************************************************************/
FDMA_Tdm_t* FDMA_TdmListPopFront(FDMA_TdmList_t *apTdmList);

/****************************************************************************/
/** Removes the a TDM from the end of the list.
* 
* @param apTdmList pointer to a list structure to be initialized.
*
* @return pointer to the poped TDM structure, NULL if list was empty
*****************************************************************************/
FDMA_Tdm_t* FDMA_TdmListPopBack(FDMA_TdmList_t *apTdmList);

/****************************************************************************/
/** Pops a TDM out of the free TDM list if available.
*
* @return pointer to the poped TDM structure, NULL if list was empty
* 
* @implements FDMA_TdmFreePop_Activity
* 
*****************************************************************************/
FDMA_Tdm_t * FDMA_TdmFreePop(void);

/****************************************************************************/
/** Returns a TDM back to the free TDM list.
* 
* @param apcTdm TDM to be released
*
* @return FDMA_DRV_SUCCESS if all ok
*         FDMA_DRV_FAILURE if failed
* 
* @implements FDMA_TdRelease_Activity
*
*****************************************************************************/
int32_t FDMA_TdRelease(const FDMA_Tdm_t* apcTdm);

/****************************************************************************/
/** Returns Sequencer relation mode.
*
* @return SEQ_BASED_MODE, SEQ_LESS_MODE
* 
* @implements FDMA_SeqModeGet_Activity
* 
*****************************************************************************/
uint8_t FDMA_SeqModeGet(void);

/****************************************************************************/
/** Returns number of TDMs in the free TDM list.
*
* @return number of available TDMs
* 
* @implements FDMA_TdmFreeCntGet_Activity
* 
*****************************************************************************/
uint32_t FDMA_TdmFreeCntGet(void);

/****************************************************************************/
/** Applies the TD setup to SRAM USER TDT.
* 
* @param apcTc pointer to a TC with the TD configuration. 
*
* @return FDMA_DRV_SUCCESS
* 
* @implements FDMA_TdConfig_Activity
* 
*****************************************************************************/
int32_t FDMA_TdConfig( const FDMA_Tc_t* apcTc);

/****************************************************************************/
/** Applies the TD setup to SRAM ISP TDT.
* 
* @param apcTc pointer to a TC with the TD configuration. 
*
* @return FDMA_DRV_SUCCESS
* 
* @implements FDMA_TdIspConfig_Activity
* 
*****************************************************************************/
int32_t FDMA_TdIspConfig( const FDMA_Tc_t* apcTc);

/****************************************************************************/
/** Schedules the given TC.
* 
* @param apcTc pointer to a TC. 
*
* @return FDMA_DRV_SUCCESS
* 
* @implements FDMA_TcSchedule_Activity
* 
*****************************************************************************/
int32_t FDMA_TcSchedule( const FDMA_Tc_t* apcTc);

/****************************************************************************/
/** Gets latest event context for given TC.
 * 
 * TC specified with index in arEvent.mXfrRecNum number.
 * 
 * \param  arEvent reference to FDMA event
 * 
 * \return FDMA_LIB_SUCCESS if all ok
 *         FDMA_LIB_FAILURE if failed
 * 
 * \implements FDMA_EventGet_Activity
 * 
 ****************************************************************************/
int32_t FDMA_EventGet(FDMA_TcEvent_t *apEvent);

/****************************************************************************/
/** Gets pointer to FDMA registers mapping.
* 
* @return pointer to FDMA registers mapping
*         NULL if not mapped
* 
* @implements FDMA_RegsGet_Activity
* 
*****************************************************************************/
FDMA_Regs_t * FDMA_RegsGet(void);

/****************************************************************************/
/** Gets current status of a transfer channel.
* 
* @return FDMA_DRV_SUCCESS if all ok
*         FDMA_DRV_FAILURE if failed
* 
* @implements FDMA_TcGet_Activity
* 
*****************************************************************************/
int32_t FDMA_TcGet(FDMA_Tc_t* apTc);

/****************************************************************************/
/** Gets the physical address of the USER TDT in SRAM.
* 
* @return physical address of the USER TDT
*         0 if not mapped
* 
* @implements FDMA_TdtPhysAddrGet_Activity
* 
*****************************************************************************/
uint32_t FDMA_TdtPhysAddrGet(void);

#endif // if defined(__KERNEL__) || defined(__STANDALONE__)

#ifdef __cplusplus 
} // extern c declaration
#endif

#endif /* FDMAFUNC_H */