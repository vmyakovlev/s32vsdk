/*
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
 * @file     fdma_func.c
 * @brief    functionality of fDMA driver
 ****************************************************************************/

/*==============================================================================
Revision History:
                        Modification      Tracking
Author (core ID)        Date D/M/Y         Number     Description of Changes
Tomas Babinec           27-May-2014                    Inital version.
Tomas Babinec           02-May-2017       vsdk-575     Traceability.
Tomas Babinec           25-May-2017       vsdk-575     Misra.
==============================================================================*/

#define VDB_LOG_NOTE_OFF

#if !defined(__STANDALONE__)
  #include <linux/kernel.h>     // We're doing kernel work
  #include <linux/module.h>     // Specifically, a module 
  #include <linux/interrupt.h>  // For interrupts
  #include <asm/siginfo.h>      // siginfo
  #include <linux/sched.h>      // send_sig_info
  #include <linux/types.h>
  #include <linux/delay.h>      // for msleep
  #include <asm/io.h>           // for ioremap
#else
  #include <unistd.h>
  #include <stdint.h>
  #include <string.h>
  #include "oal_irq.h"
  #define printk
#endif // else from if !defined(__STANDALONE__)

#include "fdma_func.h"
#include "oal_chunk_ids.h"
#include "seq_types.h"
#include "vdb_log.h"

//****************************************************************************
// consts
//****************************************************************************
#define TDT_SIZE (((FDMA_TD_CNT * 3u) + 1u) * (uint32_t)sizeof(FDMA_Td_t)) 
#define TDT_ISP0 0u ///< index of TDT for IPS transfer (array 0)
#define TDT_ISP1 1u ///< index of TDT for IPS transfer (array 1)
#define TDT_USER 2u ///< index of TDT for user transfers

//****************************************************************************
// macros
//****************************************************************************

//****************************************************************************
// types
//****************************************************************************

//****************************************************************************
// global data
//****************************************************************************

//****************************************************************************
// local data
//****************************************************************************

// pointer to driver info structre
static FDMA_DrvInfo_t *spFdmaDrvInfo = NULL;

//****************************************************************************
// local data
//****************************************************************************

static SEQ_FdmaComm_t *spSeqComm = NULL;

//****************************************************************************
// external function declarations
//****************************************************************************
  
// declaration of functions imported from OAL module
// used for TDT allocation in SRAM
void *OAL_KernelMemoryAlloc(uint64_t size, uint64_t align, uint8_t chunk_id);
uint64_t OAL_KernelMemoryFree(void *physical_pointer);

//*****************************************************************************
// local function declarations
//*****************************************************************************

// Initialize TDMs
static int32_t FDMA_TdmsInit(void);

// SeqUp FDMA registers to be ready for first TX scheduling
static void FdmaHwInit(void);

#if defined(__STANDALONE__)
  unsigned long get_uptime_microS(void);

  #define msleep(_val) \
        {\
          unsigned long lStart = get_uptime_microS();\
          while((get_uptime_microS() - lStart) < _val*1000);\
        }  
#endif // #if defined(__STANDALONE__)

// Register IRQs for FDMA module
static int32_t FdmaIrqRegister(void);

// Releses FDMA IRQs
static void FdmaIrqFree(void);

// Initializes the Tdt structure.
static int32_t FDMA_TdtInit(FDMA_Tdt_t *apTdt);

// Releases TDT memory in SRAM
static void FDMA_TdtFree(FDMA_Tdt_t *apTdt);

// set IO remapped memory content
static void* IoMemSet(uint8_t* apAddr, uint8_t aValue, size_t aSize);

// send signal to user application
static void Sig2UserSend(uint32_t aTdIdx, uint32_t aEvent);

#ifdef __STANDALONE__
  static void FdmaDoneIrqHandler(void);
  static void FdmaErrIrqHandler(void);
#else // #ifdef __STANDALONE__
  static irqreturn_t FdmaDoneIrqHandler(int32_t aIrq, void *apDev_id);
  static irqreturn_t FdmaErrIrqHandler(int32_t aIrq, void *apDev_id);
#endif // else from #ifdef __STANDALONE__
 
//*****************************************************************************
// function definitions
//*****************************************************************************

/****************************************************************************/
/** 
* @brief Sets up the internal functionality of the fdma kernel module. Initializes
*   fdma related resources.
* @details Sets up the internal functionality of the fdma kernel module. Initializes
*   fdma related resources.
* 
* @param[in]  apFdmaDrvInfo pointer to driver data structure
* 
* @return FDMA_DRV_SUCCESS if all ok, otherwise FDMA_DRV_FAILURE
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
* 
* @implements FDMA_Setup_Activity
*
*****************************************************************************/
int32_t FDMA_Setup(FDMA_DrvInfo_t *apFdmaDrvInfo)
{
  int32_t  lRet      = FDMA_DRV_SUCCESS;
  uint32_t lErrStage = 0u;

  if(apFdmaDrvInfo != NULL)
  {  
    spFdmaDrvInfo = apFdmaDrvInfo;
    spSeqComm     = apFdmaDrvInfo->mpSeqComm;
    if(spSeqComm != NULL)
    {
      if(spSeqComm->mpSeqTcSchedule != NULL)
      {
        VDB_LOG_NOTE("Sequencer-based mode.\n");        
        
        spSeqComm->mpEvent2FdmaSend = &FDMA_SeqEventHandler;
        spSeqComm->mpFdmaTcSchedule = &FDMA_TcSchedule;
        
        apFdmaDrvInfo->mSequencerOn = (uint8_t)SEQ_BASED_MODE;
      } // if apFdmaDrvInfo symbol found
    }
    else
    {
      VDB_LOG_NOTE("Sequencer-less mode.\n");
      spSeqComm = NULL;
      apFdmaDrvInfo->mSequencerOn = (uint8_t)SEQ_LESS_MODE;
    } // else from if apFdmaDrvInfo symbol found
  
    // *** initialize TDT area in SRAM ***
    if(FDMA_TdtInit(apFdmaDrvInfo->mpTdt) != FDMA_DRV_SUCCESS)
    {
      VDB_LOG_ERROR("TDT init failed\n");
      lErrStage = 1u;
    } // if tdt init failed
    else
    {    
      // *** init list of unused TDMs and TC envents ***
      if(FDMA_TdmsInit() != FDMA_DRV_SUCCESS)
      {
        VDB_LOG_ERROR("Failed to initialize TDMs.\n");
        lErrStage = 2u;
      } // if TDM init failed  
      else
      {
        // *** register IRQs ***
        if(FdmaIrqRegister() != FDMA_DRV_SUCCESS)
        {
          VDB_LOG_ERROR("Failed to register IRQs.\n");
          lErrStage = 3u;
          FDMA_TdmListInit(&apFdmaDrvInfo->mTdmFreeList);
        } // if IRQ registering failed
        else
        {    
          // *** initialize FastDMA ***  
          FdmaHwInit();
        } // else from if IRQ registering failed
      } // else from if TDM init failed
    } // else from if tdt init failed
    
    // *** error processing ***
    if(lErrStage > 2u)
    {
      FDMA_TdmListInit(&apFdmaDrvInfo->mTdmFreeList);
    }
    
    if(lErrStage > 1u)
    {
      FDMA_TdtFree(apFdmaDrvInfo->mpTdt);
    }
    
    if(lErrStage > 0u)
    {
      apFdmaDrvInfo->mSequencerOn = (uint8_t)SEQ_LESS_MODE;
      spFdmaDrvInfo = NULL;
      spSeqComm = NULL;
      lRet = FDMA_DRV_FAILURE;
      VDB_LOG_ERROR("Canceling FDMA insmod.\n");
    }
  } // if driver info exists
  else
  {
    lRet = FDMA_DRV_FAILURE;
  } // else if driver info exists
  
  return lRet;
} // FDMA_Setup()

/****************************************************************************/
/** 
* @brief Releases all fdma related resources.
* @details Releases all fdma related resources.
* 
* @return FDMA_DRV_SUCCESS if all ok, otherwise FDMA_DRV_FAILURE
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*  
* @implements FDMA_Close_Activity
* 
*****************************************************************************/
void FDMA_Close(void)
{   
  // reset
  spFdmaDrvInfo->mpRegs->CTRL = spFdmaDrvInfo->mpRegs->CTRL | FDMA_SOFT_RST_BIT;
  msleep((int32_t)50);

  // release IRQ channels
  FdmaIrqFree();
  
  // reset TDM list
  FDMA_TdmListInit(&spFdmaDrvInfo->mTdmFreeList);
  
  // *** free the SRAM TDT memory in SRAM ***
  FDMA_TdtFree(spFdmaDrvInfo->mpTdt);
  
  spFdmaDrvInfo = NULL;
} // FDMA_Close()

//****************************************************************************

int32_t FDMA_TdmsInit(void)
{  
  int32_t  lRet = FDMA_DRV_SUCCESS;
  uint32_t iter = 0u;
  
  // init the free list
  FDMA_TdmListInit(&spFdmaDrvInfo->mTdmFreeList);
  
  // push all available TDMs as free
  for( iter = 0u; iter < FDMA_TD_CNT; ++iter)
  {
    spFdmaDrvInfo->mpTdmArr[iter].mTdIdx = (uint8_t)iter; // TDM init
    if(FDMA_TdmListPushBack(&spFdmaDrvInfo->mTdmFreeList, 
                            &spFdmaDrvInfo->mpTdmArr[iter]) != FDMA_DRV_SUCCESS)
    {
      VDB_LOG_ERROR("Tdm number %d already in the list.\n",(int32_t) iter);
      lRet = FDMA_DRV_FAILURE;
      break;
    } // if tdm list push back failed
    
    // reset TC event
    spFdmaDrvInfo->mpTcEventArr[iter].mXfrStat        = (uint32_t)0;
    spFdmaDrvInfo->mpTcEventArr[iter].mXfrCalcCrcVal  = (uint32_t)0;
    spFdmaDrvInfo->mpTcEventArr[iter].mXfrCurrDdrPtr  = (uint32_t)0;
    spFdmaDrvInfo->mpTcEventArr[iter].mXfrCurrSramPtr = (uint32_t)0;  
    spFdmaDrvInfo->mpTcEventArr[iter].mXfrRecNum      = (uint32_t)0;
    spFdmaDrvInfo->mpTcEventArr[iter].mSramNextLine   = (uint32_t)0;  
  } // for all list nodes
  
  return lRet;
} // FDMA_TdmsInit()

//****************************************************************************

static void FdmaHwInit(void)
{          
  // do SW reset just to be sure
  spFdmaDrvInfo->mpRegs->CTRL = spFdmaDrvInfo->mpRegs->CTRL | FDMA_SOFT_RST_BIT;

  // wait a bit
  msleep((int32_t)50);  
  
  // set count of available TDs
  spFdmaDrvInfo->mpRegs->XFR_REC_CNT = (uint32_t) FDMA_TD_CNT;

  if(spFdmaDrvInfo->mSequencerOn == SEQ_BASED_MODE)
  {
    // set address of ISP transaction descriptor table
    spFdmaDrvInfo->mpRegs->XFR_REC_LIST_PTR = spFdmaDrvInfo->mTdtBasePhy;
    // disable all IRQs
    spFdmaDrvInfo->mpRegs->IRQ_EN = (uint32_t)0;
  } // if SequencerOn
  else
  {
    // set address of USER transaction descriptor table
    spFdmaDrvInfo->mpRegs->XFR_REC_LIST_PTR = spFdmaDrvInfo->mTdtBasePhy +
        (((uint32_t)2u) * ((uint32_t)FDMA_CHNL_CNT) * 
         ((uint32_t)sizeof(FDMA_Td_t)));
    // enable all IRQs
    spFdmaDrvInfo->mpRegs->IRQ_EN = (uint32_t)(FDMA_IRQ_EN_MASK_ALL);
  } // else from if SequencerOn
} // FdmaHwInit()

/****************************************************************************/
/** 
* @brief Passes FDMA events to user application.
* @details Passes FDMA events to user application. 
*          In Linux sends signal.
*          In standalone invokes handler directly.
* 
* @param aTdIdx index of event related transferdescriptor
* @param aEvent event ID
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*  
* @implements Sig2UserSend_Activity
*
*****************************************************************************/
static void Sig2UserSend(uint32_t aTdIdx, uint32_t aEvent)
{
  // invoke proper functionality
  if(aTdIdx < FDMA_TD_CNT)
  {
    uint32_t lSigVal = (aEvent << ((uint32_t)8)) | ((uint32_t)aTdIdx);
    spFdmaDrvInfo->mpSigInvoke
    (
      (int32_t)spFdmaDrvInfo->mpTdmArr[aTdIdx].mOwnerProcId,
      (int32_t) lSigVal
    );
  } // if td index in range
} // Sig2UserSend()

//****************************************************************************

static int32_t FdmaIrqRegister(void)
{
  int32_t lRet = FDMA_DRV_SUCCESS;
  
  // only in case Sequencer disabled
  if(spFdmaDrvInfo->mSequencerOn == ((uint8_t)SEQ_LESS_MODE))
  {
#ifndef __STANDALONE__  
    if (request_irq(spFdmaDrvInfo->mIrqDone, 
                    &FdmaDoneIrqHandler, 
                    IRQF_TRIGGER_RISING, 
                    FDMA_DEVICE_NAME, 
                    NULL) != (int32_t)0)
    {
      lRet = FDMA_DRV_FAILURE;
      VDB_LOG_ERROR("Failed to request IRQ for DONE signal.\n");
    } // if request for DONE IRQ failed
    else 
    {
      if (request_irq(spFdmaDrvInfo->mIrqErr, 
                      &FdmaErrIrqHandler, 
                      IRQF_TRIGGER_RISING, 
                      FDMA_DEVICE_NAME, 
                      NULL) != (int32_t)0)
      {
        lRet = FDMA_DRV_FAILURE;
        VDB_LOG_ERROR("Failed to request IRQ for ERROR signal.\n");
        free_irq(spFdmaDrvInfo->mIrqDone, NULL);
      } // if request for DONE IRQ failed
    } // else from if IRQ_DONE registration failed  
#else
    OAL_IRQInit(&FdmaDoneIrqHandler, 
                spFdmaDrvInfo->mIrqDone, 
                0xA0u, 
                OAL_IRQ_EDGE
                );
    OAL_IRQInit(&FdmaErrIrqHandler, 
                spFdmaDrvInfo->mIrqErr, 
                0xA0u, 
                OAL_IRQ_EDGE
                );
      
    OAL_IRQEnable(spFdmaDrvInfo->mIrqDone);
    OAL_IRQEnable(spFdmaDrvInfo->mIrqErr);
#endif // from #ifndef __STANDALONE__
  } // if Sequencer is not enabled
  
  return lRet;
} // FdmaIrqRegister()

//****************************************************************************

static void FdmaIrqFree(void)
{    
#if !defined(__STANDALONE__)  
  if(spFdmaDrvInfo->mSequencerOn == ((uint8_t)SEQ_LESS_MODE))
  {
    free_irq(spFdmaDrvInfo->mIrqDone, NULL);
    free_irq(spFdmaDrvInfo->mIrqErr, NULL);
  }
#endif // #if !defined(__STANDALONE__)
} // FdmaIrqRegister()

/****************************************************************************/
#ifndef __STANDALONE__
/** 
* @brief FDMA DONE IRQ handler.
* @details FDMA DONE IRQ handler.
* 
* @param aIrq     irq number
* @param apDev_id identification of the device
* 
* @return IRQ_HANDLED when done
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*  
* @implements FdmaDoneIrqHandler_Activity
*
*****************************************************************************/
static irqreturn_t FdmaDoneIrqHandler(int32_t aIrq, void *apDev_id)
{
  irqreturn_t lRet = IRQ_HANDLED;
#else // #ifndef __STANDALONE__
/** 
* @brief FDMA DONE IRQ handler.
* @details FDMA DONE IRQ handler.
* 
* @return IRQ_HANDLED when done
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*  
* @implements FdmaDoneIrqHandler_Activity
*
*****************************************************************************/
static void FdmaDoneIrqHandler(void)
{      
#endif // else from #ifndef __STANDALONE__    
  // read registers - remeber event
  uint32_t        lTdIdx  = spFdmaDrvInfo->mpRegs->XFR_REC_NUM_DONE;
  FDMA_TcEvent_t *pEvent  = &(spFdmaDrvInfo->mpTcEventArr[lTdIdx]);

  pEvent->mXfrStat        = spFdmaDrvInfo->mpRegs->XFR_STAT;
  pEvent->mXfrCalcCrcVal  = spFdmaDrvInfo->mpRegs->CALC_CRC_VAL;
  pEvent->mXfrCurrDdrPtr  = spFdmaDrvInfo->mpRegs->CURR_DDR_PTR;
  pEvent->mXfrCurrSramPtr = spFdmaDrvInfo->mpRegs->CURR_SRAM_PTR;
  pEvent->mSramNextLine   = 
              (spFdmaDrvInfo->mpRegs->NEXT_LINE & FDMA_SRAM_NEXT_LINE_MASK)
              >> FDMA_SRAM_NEXT_LINE_SHIFT;
  
  Sig2UserSend(lTdIdx, FDMA_DONE_EVENT);  
  
  // reset the done bit in STAT register
  spFdmaDrvInfo->mpRegs->XFR_STAT = (uint32_t)FDMA_IRQ_TYPE_XFR_DONE;
  
  VDB_LOG_NOTE("FDMA DONE IRQ handled.\n");
#ifndef __STANDALONE__
  return lRet;
#endif // #ifndef __STANDALONE__
} // FdmaDoneIrqHandler()

/****************************************************************************/
#ifndef __STANDALONE__
/** 
* @brief FDMA Error IRQ handler.
* @details FDMA Error IRQ handler.
*
* @param aIrq     irq number
* @param apDev_id identification of the device
* 
* @return IRQ_HANDLED when done
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
* 
* @implements FdmaErrIrqHandler_Activity
*
*****************************************************************************/
static irqreturn_t FdmaErrIrqHandler(int32_t aIrq, void *apDev_id)
{
  irqreturn_t  lRet = IRQ_HANDLED;
#else // #ifndef __STANDALONE__
/** 
* @brief FDMA Error IRQ handler.
* @details FDMA Error IRQ handler.
* 
* @return IRQ_HANDLED when done
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
* 
* @implements FdmaErrIrqHandler_Activity
*
*****************************************************************************/
static void FdmaErrIrqHandler(void)
{      
#endif // else from #ifndef __STANDALONE__
  
  // read registers
  uint32_t        lTdIdx = spFdmaDrvInfo->mpRegs->ERR_XFR_REC_NUM;
  FDMA_TcEvent_t *pEvent = &(spFdmaDrvInfo->mpTcEventArr[lTdIdx]);
  
  pEvent->mXfrStat        = spFdmaDrvInfo->mpRegs->XFR_STAT;
  pEvent->mXfrCalcCrcVal  = spFdmaDrvInfo->mpRegs->CALC_CRC_VAL;
  pEvent->mXfrCurrDdrPtr  = spFdmaDrvInfo->mpRegs->CURR_DDR_PTR;
  pEvent->mXfrCurrSramPtr = spFdmaDrvInfo->mpRegs->CURR_SRAM_PTR;
  pEvent->mSramNextLine   = 
                (spFdmaDrvInfo->mpRegs->NEXT_LINE & FDMA_SRAM_NEXT_LINE_MASK)
                >> FDMA_SRAM_NEXT_LINE_SHIFT;

  /* send the signal */
  Sig2UserSend(lTdIdx, FDMA_ERR_EVENT); 
  
  // reset signaled err flags
  spFdmaDrvInfo->mpRegs->XFR_STAT = (pEvent->mXfrStat & 
                                       (uint32_t)FDMA_IRQ_ERR_MASK_ALL);
  
  VDB_LOG_NOTE("FDMA ERR IRQ handled.\n");
#ifndef __STANDALONE__
  return lRet;
#endif // #ifndef __STANDALONE__
} // FdmaErrIrqHandler()

/****************************************************************************/
/** 
* @brief Handles events signaled from Sequencer driver. Called by Sequencer deriver.
* @details Handles events signaled from Sequencer driver. Called by Sequencer deriver.
* 
* @param apcEvent  event describing structure
* 
* @return FDMA_DRV_SUCCESS if all ok
*         FDMA_DRV_FAILURE otherwise
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*  
* @implements FDMA_SEqEventHandler_Activity
*
*****************************************************************************/
int32_t FDMA_SeqEventHandler(const FDMA_TcEvent_t *apcEvent)
{
  int32_t         lRet   = FDMA_DRV_SUCCESS;
  
  /* send the signal */
  Sig2UserSend(spFdmaDrvInfo->mpTcEventArr[apcEvent->mXfrRecNum].mXfrRecNum, 
               0u); 
  
  VDB_LOG_NOTE("SEQ -> FDMA Event handler called.\n");
  return lRet;
} // FDMA_SeqEventHandler(FDMA_TcEvent_t *apEvent)

//****************************************************************************

static int32_t FDMA_TdtInit(FDMA_Tdt_t *apTdt)
{
  int32_t lRet = FDMA_DRV_SUCCESS;
  
  // set number of TDs
  apTdt[TDT_USER].mTdCnt = FDMA_TD_CNT;
  apTdt[TDT_ISP0].mTdCnt = FDMA_TD_CNT;
  apTdt[TDT_ISP1].mTdCnt = FDMA_TD_CNT;
  
  // allocate SRAM for TDT
  spFdmaDrvInfo->mpTdtBasePhyHndl = OAL_KernelMemoryAlloc(
    TDT_SIZE, 
    0, 
    OAL_CHUNK_ID_SRAM_SINGLE);
  if(spFdmaDrvInfo->mpTdtBasePhyHndl != NULL)
  {
    uint32_t lTdtBase = (uint32_t)(uintptr_t)spFdmaDrvInfo->mpTdtBasePhyHndl;
    // make sure it is alligned to 32bits
    lTdtBase = (lTdtBase + 0x1Fu) & 0xFFFFFFE0u; 
    spFdmaDrvInfo->mTdtBasePhy = lTdtBase;    
    
    // setup the TDT structures TDT_USER
#if !defined(__STANDALONE__)      
    apTdt[TDT_ISP0].mpBase = ioremap(lTdtBase, TDT_SIZE);
#else // #if !defined(__STANDALONE__)  
    apTdt[TDT_ISP0].mpBase = (FDMA_Td_t*)spFdmaDrvInfo->mpTdtBasePhyHndl;
#endif // #else from #if !defined(__STANDALONE__)  
    if(apTdt[TDT_ISP0].mpBase == NULL)
    {
      VDB_LOG_ERROR("Failed to map TDT.\n");
      lRet = FDMA_DRV_FAILURE;
    } // if failed to map TDT
    else
    {
      apTdt[TDT_ISP1].mpBase = &(apTdt[TDT_ISP0].mpBase[FDMA_TD_CNT]);
      apTdt[TDT_USER].mpBase = &(apTdt[TDT_ISP0].mpBase[FDMA_TD_CNT * 2u]);
      
      // clear the TDs
      (void)IoMemSet((uint8_t*)apTdt[TDT_ISP0].mpBase, 
              0u, 
              sizeof(FDMA_Td_t) * ((((uint32_t)FDMA_TD_CNT)*3u) + 1u));

      // set TDTs to  Sequencer
      if(spSeqComm != NULL)
      {
        spSeqComm->mppTdt[TDT_ISP0]        = apTdt[TDT_ISP0].mpBase;
        spSeqComm->mpTdtPhysAddr[TDT_ISP0] = lTdtBase;
        spSeqComm->mppTdt[TDT_ISP1]        = apTdt[TDT_ISP1].mpBase;
        spSeqComm->mpTdtPhysAddr[TDT_ISP1] = 
                    lTdtBase + ((uint32_t)FDMA_CHNL_CNT) * 
                    (uint32_t)sizeof(FDMA_Td_t);
        spSeqComm->mppTdt[TDT_USER]        = apTdt[TDT_USER].mpBase;
        spSeqComm->mpTdtPhysAddr[TDT_USER] = 
                    lTdtBase + 2u * ((uint32_t)FDMA_CHNL_CNT) * 
                    (uint32_t)sizeof(FDMA_Td_t);
      } // if sequencer is there
       
      VDB_LOG_NOTE("addr allocated: 0x%x\n", spFdmaDrvInfo->mTdtBasePhy);
      VDB_LOG_NOTE("TDT at virtual address 0x%p\n", (void*) apTdt->mpBase);
    }// else from if failed to map TDT
  } // if SRAM alloc succeeded
  else
  {
    VDB_LOG_ERROR("Failed to allocate TDTs in SRAM.\n");
    lRet = FDMA_DRV_FAILURE;
  } // if SRAM allocation succeeded
  
  return lRet;
} // FDMA_TdtInit()

//****************************************************************************

static void FDMA_TdtFree(FDMA_Tdt_t *apTdt)
{
  if(spFdmaDrvInfo->mpTdtBasePhyHndl != NULL)
  {
    VDB_LOG_NOTE("SRAM addr to free: %p\n", (void*) apTdt);
#if !defined(__STANDALONE__)     
    if(apTdt[TDT_ISP0].mpBase != NULL)
    {
      iounmap(apTdt[TDT_ISP0].mpBase);
    } // if TDT mapping exists
#endif // #if !defined(__STANDALONE__) 
    // free tdt in SRAM
    if(OAL_KernelMemoryFree(spFdmaDrvInfo->mpTdtBasePhyHndl) != (uint64_t)0)
    {
      VDB_LOG_ERROR("Failed to free TDT region in SRAM.\n");
    } // if failed to free the TDT
  } // if tdt allocated
  
  // reset number of TDs
  apTdt[TDT_USER].mTdCnt = (uint32_t)0;
  apTdt[TDT_ISP0].mTdCnt = (uint32_t)0;
  apTdt[TDT_ISP1].mTdCnt = (uint32_t)0;
  // reset tdt address
  apTdt[TDT_USER].mpBase = NULL;
  apTdt[TDT_ISP0].mpBase = NULL;
  apTdt[TDT_ISP1].mpBase = NULL;
} // FDMA_TdtFree()

//****************************************************************************

// initializes given tdm list
void FDMA_TdmListInit(FDMA_TdmList_t *apTdmList)
{
  uint32_t iter;
  apTdmList->mpHead = NULL;
  apTdmList->mpTail = NULL;
  
  apTdmList->mLength = 0u;  
  
  // reset all nodes in the pool
  for(iter = 0u; iter < FDMA_TD_CNT; ++iter)
  {
    apTdmList->mNodePool[iter].mpPrev = NULL;
    apTdmList->mNodePool[iter].mpNext = NULL;
    apTdmList->mNodePool[iter].mpTdm  = NULL;
  } // for all nodesl
} // FDMA_TdmListInit()

//****************************************************************************

// pushes TDM to the front of the list
int32_t FDMA_TdmListPushFront(FDMA_TdmList_t *apTdmList, FDMA_Tdm_t *apTdm)
{
  FDMA_TdmListNode_t *pCurrentNode = &(apTdmList->mNodePool[apTdm->mTdIdx]);
  int32_t             lRetVal       = FDMA_DRV_FAILURE;
  
  if(pCurrentNode->mpTdm == NULL)
  {
    if(apTdmList->mpHead != NULL)
    {
      apTdmList->mpHead->mpPrev = pCurrentNode;
    } // if not empty
    else
    {
      apTdmList->mpTail = pCurrentNode;
    } // else from if not empty
    
    pCurrentNode->mpNext = apTdmList->mpHead;  
    ++apTdmList->mLength;
    apTdmList->mpHead    = pCurrentNode;    
    pCurrentNode->mpPrev = NULL;
    pCurrentNode->mpTdm  = apTdm;  
    
    lRetVal = FDMA_DRV_SUCCESS;
  } // if TDM not in the list yet
  
  return lRetVal;
} // FDMA_TdmListPushFront()

//****************************************************************************

// pushes TDM to the back of the list
int32_t FDMA_TdmListPushBack(FDMA_TdmList_t *apTdmList, FDMA_Tdm_t *apTdm)
{
  FDMA_TdmListNode_t *pCurrentNode = &(apTdmList->mNodePool[apTdm->mTdIdx]);
  int32_t             lRetVal      = FDMA_DRV_FAILURE;
  
  if(pCurrentNode->mpTdm == NULL)
  {
    if(apTdmList->mpTail != NULL)
    {
      apTdmList->mpTail->mpNext = pCurrentNode;
    } // if not empty
    else
    {
      apTdmList->mpHead = pCurrentNode;
    } // else from if not empty
    
    pCurrentNode->mpPrev = apTdmList->mpTail;    
    apTdmList->mpTail    = pCurrentNode;
    ++apTdmList->mLength;
    
    pCurrentNode->mpNext = NULL;
    pCurrentNode->mpTdm  = apTdm;  
    
    lRetVal = FDMA_DRV_SUCCESS;
  } // if TDM not in the list yet
  
  return lRetVal;
} // FDMA_TdmListPushBack()

//****************************************************************************

FDMA_Tdm_t* FDMA_TdmListPopFront(FDMA_TdmList_t *apTdmList)
{
  FDMA_TdmListNode_t *pCurrentNode = apTdmList->mpHead;
  FDMA_Tdm_t         *pRet         = NULL;
  
  if( pCurrentNode != NULL)
  {
     pRet = pCurrentNode->mpTdm;
     
     if(pCurrentNode->mpNext != NULL)
     {
       pCurrentNode->mpNext->mpPrev = NULL;
       apTdmList->mpHead            = pCurrentNode->mpNext;
     } // if not last node
     else 
     {
       apTdmList->mpHead = NULL;
       apTdmList->mpTail = NULL;
     } // else from if not last node
     
     --apTdmList->mLength;     
     
     // reset the node
     pCurrentNode->mpPrev = NULL;
     pCurrentNode->mpNext = NULL;
     pCurrentNode->mpTdm  = NULL;
  } // if list not empty
  
  return pRet;  
} // FDMA_TdmListPopFront()

//****************************************************************************

FDMA_Tdm_t* FDMA_TdmListPopBack(FDMA_TdmList_t *apTdmList)
{
  FDMA_TdmListNode_t *pCurrentNode = apTdmList->mpTail;
  FDMA_Tdm_t         *pRet         = NULL;
  
  if( pCurrentNode != NULL)
  {
     pRet = pCurrentNode->mpTdm;
     
     if(pCurrentNode->mpPrev != NULL)
     {
       pCurrentNode->mpPrev->mpNext = NULL;
       apTdmList->mpTail            = pCurrentNode->mpPrev;
     } // if not last node
     else 
     {
       apTdmList->mpHead = NULL;
       apTdmList->mpTail = NULL;
     } // else from if not last node
     
     --apTdmList->mLength;     
     
     // reset the node
     pCurrentNode->mpPrev = NULL;
     pCurrentNode->mpNext = NULL;
     pCurrentNode->mpTdm  = NULL;
  } // if list not empty
  
  return pRet;  
} // FDMA_TdmListPopBack()

/****************************************************************************/
/** 
* @brief Pops a TDM out of the free TDM list if available.
* @details Pops a TDM out of the free TDM list if available.
*
* @return pointer to the poped TDM structure, NULL if list was empty
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*  
* @implements FDMA_TdmFreePop_Activity
* 
*****************************************************************************/
FDMA_Tdm_t * FDMA_TdmFreePop(void)
{         
  //try to pop free TD
  return FDMA_TdmListPopFront(&spFdmaDrvInfo->mTdmFreeList);
} // FDMA_Td_t* FDMA_TdFreePop()

/****************************************************************************/
/** 
* @brief Returns a TDM back to the free TDM list.
* @details Returns a TDM back to the free TDM list.
* 
* @param apcTdm TDM to be released
*
* @return FDMA_DRV_SUCCESS if all ok
*         FDMA_DRV_FAILURE if failed
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*  
* @implements FDMA_TdRelease_Activity
*
*****************************************************************************/
int32_t FDMA_TdRelease(const FDMA_Tdm_t* apcTdm)
{
  int32_t     lRet      = FDMA_DRV_SUCCESS;
  FDMA_Tdm_t *lpFreeTdm = NULL;
    
  if(apcTdm->mTdIdx < FDMA_TD_CNT)
  {
    // remember pointer to the TDM to be freed
    lpFreeTdm = &spFdmaDrvInfo->mpTdmArr[apcTdm->mTdIdx];
    
    // reset the TD      
    (void)memset(&(spFdmaDrvInfo->mpTdt[TDT_ISP0].mpBase[apcTdm->mTdIdx]), 
           (int32_t)0, 
           sizeof(FDMA_Td_t));
      
    //set TDM as free
    lpFreeTdm->mReserved    = (uint8_t)0;
    lpFreeTdm->mOwnerProcId = (int32_t)0;
    
    //try to push free TDM
    if( FDMA_TdmListPushFront(&(spFdmaDrvInfo->mTdmFreeList), lpFreeTdm) 
        != FDMA_DRV_SUCCESS )
    {
      VDB_LOG_ERROR("Push free failed.\n");
      lRet = FDMA_DRV_FAILURE;
    } // if push failed
  }else
  {
    VDB_LOG_WARNING("No such TD index (%u). No action taken.\n", 
                    (uint32_t) apcTdm->mTdIdx);
    lRet = FDMA_DRV_FAILURE;
  }
  
  return lRet;
} // int32_t FDMA_TdRelease(const FDMA_Tdm_t* apcTdm)

/****************************************************************************/
/** 
* @brief Returns Sequencer relation mode.
* @details Returns Sequencer relation mode.
*
* @return number of available TDMs
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*  
* @implements FDMA_SeqModeGet_Activity
* 
*****************************************************************************/
uint8_t FDMA_SeqModeGet(void)
{   
  uint8_t lRet;
  if(spFdmaDrvInfo != NULL)
  {
    lRet = spFdmaDrvInfo->mSequencerOn;
  }
  else
  {
    lRet = (uint8_t)SEQ_LESS_MODE;
  }
  
  return lRet;
} // uint32_t FDMA_TdFreeCntGet()

/****************************************************************************/
/** 
* @brief Returns number of TDMs in the free TDM list.
* @details Returns number of TDMs in the free TDM list.
*
* @return number of available TDMs
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*  
* @implements FDMA_TdmFreeCntGet_Activity
* 
*****************************************************************************/
uint32_t FDMA_TdmFreeCntGet(void)
{   
  return spFdmaDrvInfo->mTdmFreeList.mLength;
} // uint32_t FDMA_TdFreeCntGet()

/****************************************************************************/
/** 
* @brief Applies the TD setup to SRAM USER TDT.
* @details Applies the TD setup to SRAM USER TDT.
* 
* @param apcTc pointer to a TC with the TD configuration. 
*
* @return FDMA_DRV_SUCCESS
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*  
* @implements FDMA_TdConfig_Activity
* 
*****************************************************************************/
int32_t FDMA_TdConfig( const FDMA_Tc_t* apcTc)
{  
  int32_t lRet = FDMA_DRV_SUCCESS;
  if(apcTc != NULL)
  {
    if(apcTc->mTdm.mTdIdx < FDMA_TD_CNT)
    {
      spFdmaDrvInfo->mpTdt[TDT_USER].mpBase[apcTc->mTdm.mTdIdx] = apcTc->mTd;
    }else
    {
      VDB_LOG_WARNING("No such TD index (%u). No action taken.\n", 
                      (unsigned) apcTc->mTdm.mTdIdx);
      lRet = FDMA_DRV_FAILURE;
    }
  }else
  {
    VDB_LOG_ERROR("Tc is NULL.\n");
    lRet = FDMA_DRV_FAILURE;
  }
  return lRet;
} // uint32_t FDMA_TdConfig( const FDMA_Tc_t* apcTc)

/****************************************************************************/
/** 
* @brief Applies the TD setup to SRAM ISP TDT.
* @details Applies the TD setup to SRAM ISP TDT.
* 
* @param apcTc pointer to a TC with the TD configuration. 
*
* @return FDMA_DRV_SUCCESS
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*  
* @implements FDMA_TdIspConfig_Activity
* 
*****************************************************************************/
int32_t FDMA_TdIspConfig( const FDMA_Tc_t* apcTc)
{  
  int32_t lRet = FDMA_DRV_SUCCESS;
  if(apcTc != NULL)
  {
    if(apcTc->mTdm.mTdIdx < FDMA_TD_CNT)
    {
      // set both ping-pong TDs
      spFdmaDrvInfo->mpTdt[TDT_ISP0].mpBase[apcTc->mTdm.mTdIdx] = apcTc->mTd;
      spFdmaDrvInfo->mpTdt[TDT_ISP1].mpBase[apcTc->mTdm.mTdIdx] = apcTc->mTd;
    }else
    {
      VDB_LOG_WARNING("No such TD index (%u). No action taken.\n", 
                      (unsigned) apcTc->mTdm.mTdIdx);
      lRet = FDMA_DRV_FAILURE;
    }
  }else
  {
    VDB_LOG_ERROR("Tc is NULL.\n");
    lRet = FDMA_DRV_FAILURE;
  }
  return lRet;
} // uint32_t FDMA_TdIspConfig( const FDMA_Tc_t* apcTc)

/****************************************************************************/
/** 
* @brief Schedules the given TC.
* @details Schedules the given TC.
* 
* @param apcTc pointer to a TC. 
*
* @return FDMA_DRV_SUCCESS
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*  
* @implements FDMA_TcSchedule_Activity
* 
*****************************************************************************/
int32_t FDMA_TcSchedule( const FDMA_Tc_t* apcTc)
{  
  int32_t lRet = FDMA_DRV_SUCCESS;
  if(spFdmaDrvInfo->mSequencerOn == SEQ_BASED_MODE)
  {
    VDB_LOG_ERROR("Scheduling not allowed in Sequencer based mode.\n");
    lRet = FDMA_DRV_FAILURE;
  } // if sequencer based mode
  else if(apcTc != NULL)
  {
    if(apcTc->mTdm.mTdIdx < FDMA_TD_CNT)
    {
      // find out queue status
      if( (spFdmaDrvInfo->mpRegs->XFR_STAT & 0x2u) > 0u )
      {
        lRet = FDMA_DRV_ERR_QUEUE_FULL;
        VDB_LOG_WARNING("FDMA transaction queue full.\n");
      } // if queue full
      else
      {
        uint32_t lLineNums = 0u;
        // Set DDR current line - 0x3FFF = use value from TD in SRAM        
        lLineNums  = apcTc->mTd.mSramCurrLine << 16u;
        lLineNums |= apcTc->mTd.mDdrCurrLine;  
          
        spFdmaDrvInfo->mpRegs->XFR_LINE_NUM = lLineNums;
        
        VDB_LOG_NOTE("LINE_NUM = 0x%x\n", (unsigned) lLineNums);
        
        // schedule the transaction
        spFdmaDrvInfo->mpRegs->XFR_REC_NUM = apcTc->mTdm.mTdIdx;
      } //else from if queue full   
    }else
    {
      VDB_LOG_WARNING("No such TD index (%u). No action taken.\n", 
                      (unsigned) apcTc->mTdm.mTdIdx);
      lRet = FDMA_DRV_FAILURE;
    }
  }else
  {
    VDB_LOG_ERROR("Tc is NULL.\n");
    lRet = FDMA_DRV_FAILURE;
  }
  return lRet;
} // int32_t FDMA_TcSchedule( const FDMA_Tc_t* apcTc)

/****************************************************************************/
/** 
 * @brief Gets latest event context for given TC.
 * @details TC specified with index in arEvent.mXfrRecNum number.
 * 
 * \param  arEvent reference to FDMA event
 * 
 * \return FDMA_LIB_SUCCESS if all ok
 *         FDMA_LIB_FAILURE if failed
 * 
 * @api
 * @isr
 *
 * @pre
 *
 * @post
 *
 * @note
 *  
 * \implements FDMA_EventGet_Activity
 * 
 ****************************************************************************/
int32_t FDMA_EventGet(FDMA_TcEvent_t *apEvent)
{  
  int32_t lRet = FDMA_DRV_SUCCESS;
  if(apEvent->mXfrRecNum < (uint32_t)FDMA_TD_CNT)
  {
    *apEvent = spFdmaDrvInfo->mpTcEventArr[apEvent->mXfrRecNum];
  } // if TD exists
  else
  {
    lRet = FDMA_DRV_FAILURE;
  } // else from if TD exists
  
  return lRet;
} // FDMA_EventGet(FDMA_TcEvent_t *apEvent)

/****************************************************************************/
/** 
* @brief Gets pointer to FDMA registers mapping.
* @details Gets pointer to FDMA registers mapping.
* 
* @return pointer to FDMA registers mapping
*         NULL if not mapped
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*  
* @implements FDMA_RegsGet_Activity
* 
*****************************************************************************/
FDMA_Regs_t * FDMA_RegsGet(void)
{  
  return spFdmaDrvInfo->mpRegs;
} // FDMA_Regs_t* FDMA_RegsGet()

/****************************************************************************/
/** 
* @brief Gets current status of a transfer channel.
* @details Gets the physical address of the USER TDT in SRAM.
* 
* @return FDMA_DRV_SUCCESS if all ok
*         FDMA_DRV_FAILURE if failed
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*  
* @implements FDMA_TcGet_Activity
* 
*****************************************************************************/
int32_t FDMA_TcGet(FDMA_Tc_t* apTc)
{  
  int32_t lRet = FDMA_DRV_SUCCESS;
  
  if(apTc != NULL)
  {
    if(apTc->mTdm.mTdIdx < FDMA_TD_CNT)
    {
      // read the TD  
      apTc->mTd = spFdmaDrvInfo->mpTdt[TDT_USER].mpBase[apTc->mTdm.mTdIdx];
      apTc->mTdm = spFdmaDrvInfo->mpTdmArr[apTc->mTdm.mTdIdx];
    }else
    {
      VDB_LOG_WARNING("No such TD index (%u). No action taken.\n", 
                      (unsigned) apTc->mTdm.mTdIdx);
      lRet = FDMA_DRV_FAILURE;
    }    
  }else
  {
    VDB_LOG_ERROR("Supplied TC is NULL. No action taken.\n");
    lRet = FDMA_DRV_FAILURE;
  }
  
  return lRet;
} // int32_t FDMA_TcGet(FDMA_Tc_t* apTc)

/****************************************************************************/
/** 
* @brief Gets the physical address of the USER TDT in SRAM.
* @details Gets the physical address of the USER TDT in SRAM.
* 
* @return physical address of the USER TDT
*         0 if not mapped
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*  
* @implements FDMA_TdtPhysAddrGet_Activity
* 
*****************************************************************************/
uint32_t FDMA_TdtPhysAddrGet(void)
{   
  return (spFdmaDrvInfo->mTdtBasePhy + ((uint32_t)(TDT_USER * FDMA_TD_CNT) * 
         (uint32_t)sizeof(FDMA_Td_t)));
} // int32_t FDMA_TdtPhysAddrGet()

//*****************************************************************************

static void* IoMemSet(uint8_t* apAddr, uint8_t aValue, size_t aSize)
{
  size_t   iter  = (size_t)0;
  uint8_t *lpDst = apAddr;
  for(iter = (size_t)0; iter < aSize; iter++)
  {
    *lpDst = aValue;
    lpDst++;
  } // for all bytes
  return lpDst;
} // IoMemSet

//*****************************************************************************
