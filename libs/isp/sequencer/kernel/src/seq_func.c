/*
 * Copyright (c) 2016 Freescale Semiconductor
 * Copyright 2016-2017 NXP
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
 * @file     seq_func.c
 * @brief    functionality of Sequencer driver
 ****************************************************************************/

/*==============================================================================
Revision History:
                        Modification      Tracking
Author (core ID)        Date D/M/Y         Number     Description of Changes
Tomas Babinec           12-Jun-2014                    Inital version.
Tomas Babinec           05-Jun-2017       vsdk-572     Traceability.
Tomas Babinec           11-Jul-2017       vsdk-1265    RAB default value to 0.
==============================================================================*/

#ifndef __STANDALONE__
  #include <linux/kernel.h>     // We're doing kernel work
  #include <linux/module.h>     // Specifically, a module 
  #include <asm/uaccess.h>      // for get_user and put_user 
  #include <linux/sched.h>
  #include <linux/spinlock_types.h>
  #include <linux/spinlock.h>
  #include <linux/wait.h>
  #include <linux/slab.h>       // for malloc and free
  #include <linux/types.h>
  #include <linux/delay.h>      // for msleep
  #include <linux/jiffies.h>
  #include <asm/io.h>           // for ioremap
  
#else // #ifndef __STANDALONE__
  #include <stdio.h>
  #include <unistd.h>
  #include <stdlib.h>
  #include <stdint.h>
  #include <string.h>
  #include <oal_irq.h>
  
  #include "seq_sa_kernel_api.h"

  extern void SeqSigHandler(int aSigNum, int aValue); 
#endif // else from #ifndef __STANDALONE__

#include "seq.h"
#include "seq_func.h"
#include "vdb_log.h"
#include "vdlist.h"

//*****************************************************************************
// constants
//*****************************************************************************
//#define IRQ_DEBUG

#define SEQ_ISP0_TDT_IDX    0     ///< index of ISP 0 transfer descriptor table
#define SEQ_ISP1_TDT_IDX    1     ///< index of ISP 1 transfer descriptor table
#define SEQ_USER_TDT_IDX    2     ///< index of USER  transfer descriptor table

//*****************************************************************************
// local types
//*****************************************************************************

typedef void(*MsgInstantService_t)(void);

//*****************************************************************************
// local functions
//*****************************************************************************

/****************************************************************************/
/** Initializes Sequencer driver internal data.
* 
* \param apSeqDrvInfo pointer to prefilled sequencer driver infor structure
* 
* \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
*
*****************************************************************************/
static int32_t SeqDrvDataInit(SEQ_DrvInfo_t *apSeqDrvInfo);

/****************************************************************************/
/** Registers Sequencer IRQs.
* 
* \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
*
*****************************************************************************/
static int32_t SeqIrqsRequest(void);

/****************************************************************************/
/** Releases Sequencer IRQs.
* 
* \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
*
*****************************************************************************/
static void SeqIrqsFree(void);

/****************************************************************************/
/** Puts Sequencer HW into reset.
* 
*****************************************************************************/
static void SeqHwReset(void);

/****************************************************************************/
/** Instant FrameDone message response.
* 
* \note Does only the time critical part. The rest left to the tasklet.
* 
*
*****************************************************************************/
static inline void MsgFrameDoneInstant(void);

/****************************************************************************/
/** FrameDone message handler.
* 
*****************************************************************************/
static inline void MsgFrameDone_Handler(void);

/****************************************************************************/
/** Sequencer Done message handler.
* 
*****************************************************************************/
static inline void MsgSeqDone_Handler(void);

#ifndef __STANDALONE__
  /****************************************************************************/
  /** Handler for Sequencer message IRQ.
  * 
  * \param aIrq     irq number
  * \param apDev_id identification of the device
  * 
  * \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
  *
  *****************************************************************************/
  static irqreturn_t SEQ_MsgIrqHandler(int32_t aIrq, void *apDev_id);

  /****************************************************************************/
  /** Handler for HOST message acknowledge IRQ in Linux.
  * 
  * \param aIrq     irq number
  * \param apDev_id identification of the device
  * 
  * \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
  *
  *****************************************************************************/
  static irqreturn_t SEQ_MsgAckIrqHandler(int32_t aIrq, void *apDev_id);
#else
  /****************************************************************************/
  /** Handler for HOST message IRQ on STANDALONE.
  *
  *****************************************************************************/
  static void SEQ_MsgIrqHandler(void);
  
  /****************************************************************************/
  /** Handler for HOST message acknowledge IRQ on STANDALONE.
  *
  *****************************************************************************/
  static void SEQ_MsgAckIrqHandler(void);
#endif // #ifndef __STANDALONE__

/****************************************************************************/
/** Does Message related deffered work.
* 
* \param aArg not used
* 
* \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
*
*****************************************************************************/
static void SEQ_S2hMsgTasklet(unsigned long aArg);

/****************************************************************************/
/** Sets up and schedules a timer.
* \note Fixed behavior as watch dog for Sequencer's in-time-response check.
* \TODO: make it configurable.
*****************************************************************************/
#ifdef SEQ_WDT_ENABLED
static void SEQ_WDTSchedule(void);
#endif //#ifdef SEQ_WDT_ENABLED

/****************************************************************************/
/** Removes a previously scheduled timer.
* \note Fixed behavior as watch dog for Sequencer's in-time-response check.
* \TODO: make it configurable.
*****************************************************************************/
static void SEQ_WDTRemove(void);

/****************************************************************************/
/** Initializes specified messaging pool message pool.
*
* Computes wrapparound if needed.
* 
* \param apMsgPool pointer to messaging pool
* \param apMsgBase pointer to first message in the pool
* \param aMsgCnt   number of messages in the pool
* 
* \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
*
*****************************************************************************/
static void SEQ_MsgPoolInit(SEQ_MessagePool_t *apMsgPool, 
                            uint8_t* apMsgBase, 
                            uint32_t aMsgCnt);

/****************************************************************************/
/** Shifts write pointer in message pool.
*
* Computes wrapparound if needed. In case of an owerflow the write pointer is
* set to NULL and SEQ_DRV_FAILURE is returned.
* 
* \param apMsgPool pointer to messaging pool
* 
* \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
*
*****************************************************************************/
static int32_t SEQ_MsgPoolWriteShift(SEQ_MessagePool_t *apMsgPool);

/****************************************************************************/
/** Shifts read pointer in message pool.
* 
* Computes wrapparound if needed. If the pool is empty already, SEQ_DRV_FAILURE  
* is returned.
* 
* \param apMsgPool pointer to messaging pool
* 
* \return SEQ_DRV_SUCCESS if all ok, SEQ_DRV_FAILURE if pool empty already
*
*****************************************************************************/
static int32_t SEQ_MsgPoolReadShift(SEQ_MessagePool_t *apMsgPool);

#if (ISP_EVT_LOG_LEN > 0)
static void SEQ_DRV_IrqIpusdoneProcess(void);
static void SEQ_DRV_IrqIpuvdoneProcess(void);
static void SEQ_DRV_IrqOtherdoneProcess(void);
static void SEQ_DRV_IrqSwEventProcess(void);
#endif // #if (ISP_EVT_LOG_LEN > 0)

/****************************************************************************/
/** Reads histogram values from specified IPU engine.
* 
* \param apHistl pointer to histogram structure
*
*****************************************************************************/
static void SEQ_DRV_IpuHistRead(SEQ_IpuHist_t *apHist);

/****************************************************************************/
/** Reads histogram values from specified IPU engine.
* 
* \param apStat pointer to statistics structure
*
*****************************************************************************/
static void SEQ_DRV_IpuStatRead(SEQ_IpuStat_t *apStat);

/****************************************************************************/
/** Sends a signal to registered user process.
* 
* \param aPid   process PID
* \param aValue value to be passed with the signal
*
*****************************************************************************/
inline static void Sig2UsrSend(pid_t aPid, int32_t aValue);

#ifdef __ARMV8

///< not optimized byte-wise memset is required
#define memset MemSet32

static void MemSet32(void* apDst, uint8_t aVal, size_t aSize)
{
  size_t i;
  uint8_t *lpDst = (uint8_t*)apDst;
  for(i = 0; i < aSize; i++)
  {
    lpDst[i] = aVal;
  } // for all data
} // MemSet32()

#endif

//*****************************************************************************
// global data
//*****************************************************************************

// delta measurement for SEQ FW
static jiffy_t sTimeDeltaStart = 0;
static jiffy_t sTimeDeltaEnd   = 0;

///< fdma communication structure
SEQ_FdmaComm_t gSEQ_DRV_FdmaComm = 
{
  .mppTdt           = {NULL},
  .mpTdtPhysAddr    = {0},
  .mpEvent2FdmaSend = NULL,
  .mpFdmaTcSchedule = NULL,
  .mpSeqTcSchedule  = NULL
};    

// tasklet for message defered processing
DECLARE_TASKLET(gSeqMsg_tasklet, SEQ_S2hMsgTasklet, 0);

// ISP frame temporal statistics
static SEQ_FrmTemporalStat_t sSeqTimeStat = {(uint64_t)0};

//*****************************************************************************
// local data
//*****************************************************************************

// tasklet info
static uint32_t sTaskletWork  = 0;
static uint32_t sH2SmsgCnt    = 0;

///< local Sequencer driver info pointer
static SEQ_DrvInfo_t *spSeqDrvInfo = NULL;

// sequencer messaging pools
static SEQ_MessagePool_t sS2hMsg, sH2sMsg; 

////////
//start: Linux process safety

// wait queue for Sequencer Ready after boot
static wait_queue_head_t sSeqRabQueue;

// wait queue for messaging to Sequencer
static wait_queue_head_t sSeqMsgQueue;

// wait queue for Sequencer message acknowledge signal
static wait_queue_head_t sSeqAckQueue;

// pointer to sender process message structure - valid only if waiting for reply
static SEQ_Message_t *spSeqInbMsg = NULL;

// simulates the flag in messaging interface HOST->Seq
volatile uint32_t sSeqMsgReady  = 1;

// equales 1 if someone waits for Sequencer to ack message
volatile uint8_t sSeqAckWait    = 0;

// Ready after boot message received (0 = NO, YES > 0)
volatile static char sSeqRab = 0;

// sequncer status 
static uint32_t gSeqStatus = SEQ_STATUS_OFF;

// spinlock for access to outbound message
// used to synchronize between different host threads trying to send a message
static spinlock_t sSeqMsgLock;

// guards global variable access if needed
static spinlock_t sSeqGlobalLock;

// Buffer stream access lock
static spinlock_t sStreamLock;

#ifndef __STANDALONE__
  //watch dog timer
  static struct timer_list sWDTimer;
#endif

static pid_t sSeqUserPid = 0;

//end: Linux process safety
//////

// buffer list management
static SEQ_FrmStream_t *spFrmStreams[FDMA_CHNL_CNT];

// index of current ISP TDT
static uint32_t sTdtIdx = 0;

// array of instant message handling routines
static MsgInstantService_t sMsgInstant[SEQ_MSG_TYPE_S2H_MAX - SEQ_MSG_TYPE_RAB];

// set to 1 if IPU profiling has finished
static uint32_t sIpuProfileDone = 0;

// how many CSI lines expected per frame
static uint32_t      sCSINumLines = 0; // # of lines from csi per frame

// ISP event log - only implemented for STANDALONE
#ifdef __STANDALONE__
static SEQ_EventRecord_t  sEventRecords[ISP_EVT_LOG_LEN];
static SEQ_EventLog_t     sEventLog = {sEventRecords, 0};
#endif // #ifdef __STANDALONE__
static uint8_t            sEventLogOn = 0; // by default event loggin OFF

static uint8_t            sGraphOn = 0; // by default graph is OFF

// Histograms
static SEQ_IpuHist_t *spIpuHists[IPU_HIST_CNT];

// statistic data
static SEQ_IpuStat_t *spIpuStat    = NULL;

/****************************************************************************/
/** 
* @brief Sets up the internal functionality of the Sequencer kernel module. 
* @details Initializes Sequencer related resources.
* 
* @param apSeqDrvInfo pointer to prefilled sequencer driver infor structure
* 
* @return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
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
* @implements SEQ_DRV_Setup_Activity
*
*****************************************************************************/
int32_t SEQ_DRV_Setup(SEQ_DrvInfo_t* apSeqDrvInfo)
{  
  int32_t lRet = SEQ_DRV_SUCCESS;
  
  // *** initialize internal data ***
  if(SeqDrvDataInit(apSeqDrvInfo) != SEQ_DRV_SUCCESS)
  {
    lRet = SEQ_DRV_FAILURE;
  } // if driver data init failed                 
  else
  {
    // *** register IRQs ***
    if(SeqIrqsRequest() != SEQ_DRV_SUCCESS)
    {
      lRet = SEQ_DRV_FAILURE;
      spSeqDrvInfo = NULL;
    }
  }// else from if driver data init failed
  
  return lRet;
} // SeqSetup(void)

/****************************************************************************/
/** 
* @brief Releases all Sequencer related resources.
* @details Releases all Sequencer related resources.
* 
* @return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
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
* @implements SEQ_DRV_Close_Activity
*****************************************************************************/
int32_t SEQ_DRV_Close(void)
{  
  uint32_t i = 0;

  // reset HW
  SeqHwReset();
  
  // cleanup driver
  SeqIrqsFree();
  SEQ_DRV_FrmStreamsReset();
  
  for(i = 0; i < IPU_HIST_CNT; i++)
  {
    if(spIpuHists[i] != NULL)
    {
      SEQ_DRV_IpuHistogramDisable((SEQ_IpuEngine_t)i);
    } // if enabled
  } // for all IPU hist engines
  
  // set all Instant message handlers to null
  for(i = 0; i < (SEQ_MSG_TYPE_S2H_MAX - SEQ_MSG_TYPE_RAB); i++)
  {
    sMsgInstant[i] = NULL;
  } // for all message types
  
  spSeqDrvInfo = NULL; 
  
  return SEQ_DRV_SUCCESS;
} // SeqClose(void)

//****************************************************************************

static int32_t SeqDrvDataInit(SEQ_DrvInfo_t *apSeqDrvInfo)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  uint32_t i = 0;
  
  if(apSeqDrvInfo != NULL)
  {
    // remember driver info
    spSeqDrvInfo = apSeqDrvInfo;
    
    // set func for FDMA Tc cheduling
    gSEQ_DRV_FdmaComm.mpSeqTcSchedule  = &SEQ_DRV_FdmaTcScheduleMsgSend;
    
    // *** init isp rams ***
    memset(spSeqDrvInfo->mpDbgPram, 0, DBG_PRAM_SIZE);
    memset(spSeqDrvInfo->mpPram,    0, PRAM_SIZE);
    memset(spSeqDrvInfo->mpCram,    0, CRAM_SIZE);           
    memset(spSeqDrvInfo->mpKram,    0, KRAM_SIZE);
      
    // ***  reset Sequencer ***
    SeqHwReset();
    
    // initialize utilities
    spin_lock_init(&sSeqMsgLock);
    spin_lock_init(&sSeqGlobalLock);
    spin_lock_init(&sStreamLock);
    init_waitqueue_head(&sSeqRabQueue);
    init_waitqueue_head(&sSeqMsgQueue);
    init_waitqueue_head(&sSeqAckQueue);
    
#ifndef __STANDALONE__
    init_timer(&sWDTimer);
#else
    sSeqUserPid = 1;
#endif //__STANDALONE__
    
    // set all Instant message handlers to null
    for(i = 0; i < (SEQ_MSG_TYPE_S2H_MAX - SEQ_MSG_TYPE_RAB); i++)
    {
      sMsgInstant[i] = NULL;
    } // for all message types
    
    // set available Instant message handlers
    // both are sharing the same handler
    sMsgInstant[SEQ_MSG_TYPE_FRAMEDONE - SEQ_MSG_TYPE_RAB] = 
                                                  MsgFrameDoneInstant;
    sMsgInstant[SEQ_MSG_TYPE_IPU_PROFILE_DONE - SEQ_MSG_TYPE_RAB] = 
                                                  MsgFrameDoneInstant;
                                                  
    // *** prepare messaging ***
      
    // set up message pools
    SEQ_MsgPoolInit(&sS2hMsg, 
                    &spSeqDrvInfo->mpPram[PRAM_OUT_MSG_OFF], 
                    SEQ2HOST_MESSAGE_CNT);
    SEQ_MsgPoolInit(&sH2sMsg, 
                    &spSeqDrvInfo->mpPram[PRAM_IN_MSG_OFF], 
                    HOST2SEQ_MESSAGE_CNT );
    
    // set initial message addresses
    spSeqDrvInfo->mpSeqCBregs->INMSG_ADDR.R   = 
          (uint32_t)((uintptr_t)sH2sMsg.mpWrite - (uintptr_t)sS2hMsg.mpBase + 
          SEQ_PRAM_OFFSET);
    spSeqDrvInfo->mpSeqCBregs->OUTMSG_ADDR.R  = 
          (uint32_t)((uintptr_t)sS2hMsg.mpWrite - (uintptr_t)sS2hMsg.mpBase) + 
          SEQ_PRAM_BASE;
    
    // fill in communication info
    spSeqDrvInfo->mpCommInfo = \
      (SEQ_pram_map_t*) &(spSeqDrvInfo->mpPram[SEQ_PRAM_MAP_OFF]);
    spSeqDrvInfo->mpCommInfo->mOutMsgAddrOff = \
      (uint32_t) (SEQ_PRAM_BASE + PRAM_OUT_MSG_OFF);
    spSeqDrvInfo->mpCommInfo->mAuxDataOff    = \
      (uint32_t) (SEQ_PRAM_BASE + PRAM_AUX_DATA_OFF);   
    spSeqDrvInfo->mpCommInfo->mPatchListOff  = \
      (uint32_t) (SEQ_PRAM_BASE + PRAM_PATCH_LIST_OFF);  
    spSeqDrvInfo->mpCommInfo->mPatchListNum  = \
      (uint32_t) (0);  
    spSeqDrvInfo->mpCommInfo->mWriteRegOff  = \
      (uint32_t) (SEQ_PRAM_BASE + PRAM_REGLISTW_OFF);  
    spSeqDrvInfo->mpCommInfo->mWriteRegNum  = \
      (uint32_t) (0);    
    spSeqDrvInfo->mpCommInfo->mReadRegOff  = \
      (uint32_t) (SEQ_PRAM_BASE + PRAM_REGLISTR_OFF);  
    spSeqDrvInfo->mpCommInfo->mReadRegNum  = \
      (uint32_t) (0);  
    spSeqDrvInfo->mpCommInfo->mGraphOff      = \
      (uint32_t) (CRAM_BASE_M0  + SEQ_GRAPH_CRAM_OFFSET);    
        
    VDB_LOG_NOTE("PRAM map content:\nmOutMsgAddrOff: 0x%x\nmAuxDataOff: 0x%x"
                 "\nmGraphOff 0x%x\n", 
                 (unsigned) spSeqDrvInfo->mpCommInfo->mOutMsgAddrOff, 
                 (unsigned) spSeqDrvInfo->mpCommInfo->mAuxDataOff, 
                 (unsigned) spSeqDrvInfo->mpCommInfo->mGraphOff);    

    VDB_LOG_NOTE("Message pools initialized. S2h base: %p, H2s base: %p, "
                  "pram base: %p\n", 
                  (void*) sS2hMsg.mpBase, 
                  (void*) sH2sMsg.mpBase, 
                  (void*) &spSeqDrvInfo->mpPram[0]
                );
    
    // *** init Frame buffer stream pointers ***
    for(i = 0; i < FDMA_CHNL_CNT; i++)
    {
      spFrmStreams[i] = NULL;
    } // for all FDMA channels
    
    // *** reset IPUS histograms ***
    for(i = 0; i < 3/*IPUS_CNT*/; i++)
    {
      spSeqDrvInfo->mppIpusRegs[i]->HOST_HISTCLEAR.B.HISTCLEAR = 1;
    } // for all IPUS
  } // if Sequcner driver info not NULL
  else
  {
    lRet = SEQ_DRV_FAILURE;
  } // else from if Sequcner driver info not NULL
  return lRet;
} // SeqDrvDataInit()

//*****************************************************************************
 
static int32_t SeqIrqsRequest()
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  
  if (request_irq(spSeqDrvInfo->mIrqNums.mMsgRdy, 
                  &SEQ_MsgIrqHandler, 
                  0, 
                  SEQ_DEVICE_NAME, 
                  NULL
                 ) != (int32_t)0
     )
  {
    lRet = SEQ_DRV_FAILURE;
    VDB_LOG_ERROR("Failed to request IRQ for incomming message signal.\n");
  } // if request for MSG IRQ failed
  else
  {
    VDB_LOG_NOTE("MSG_READY_IRQ requsted ok\n");
    if (request_irq(spSeqDrvInfo->mIrqNums.mMsgAck, 
                    &SEQ_MsgAckIrqHandler, 
                    0, 
                    SEQ_DEVICE_NAME, 
                    NULL
                   ) != (int32_t)0
       )
    {
      lRet = SEQ_DRV_FAILURE;
      free_irq(SEQ_IRQ_MSG, NULL);
      VDB_LOG_ERROR("Failed to request IRQ for outbound message ack signal.\n");
    } // if request for MSG_ACK IRQ failed
    else
    {
      VDB_LOG_NOTE("MSG_ACK_IRQ requsted ok\n");
    }
  } // else from if request DONE IRQ failed

#ifdef __STANDALONE__  

#if (ISP_EVT_LOG_LEN > 0)
  OAL_IRQInit(SEQ_DRV_IrqIpusdoneProcess, 
              SEQ_IRQ_IPUS_DONE, 
              0xA0, 
              OAL_IRQ_EDGE);
  OAL_IRQEnable(SEQ_IRQ_IPUS_DONE);
  OAL_IRQInit(SEQ_DRV_IrqIpuvdoneProcess, 
              SEQ_IRQ_IPUV_DONE, 
              0xA0, 
              OAL_IRQ_EDGE);
  OAL_IRQEnable(SEQ_IRQ_IPUV_DONE);
  OAL_IRQInit(SEQ_DRV_IrqOtherdoneProcess, 
              SEQ_IRQ_OTHER_DONE, 
              0xA0, 
              OAL_IRQ_EDGE);
  OAL_IRQEnable(SEQ_IRQ_OTHER_DONE);
  
  for(i = SEQ_IRQ_SW_EVT_0; i <= SEQ_IRQ_SW_EVT_7; i++)
  {
    OAL_IRQInit(SEQ_DRV_IrqSwEventProcess, i, 0xA0, OAL_IRQ_EDGE);
    OAL_IRQEnable(i);
  } // for all SW_EVT IRQs
#endif // #if (ISP_EVT_LOG_LEN > 0)
  
#endif // #ifndef __STANDALONE__ 
  return lRet;
} // SeqIrqsRequest()

//*****************************************************************************

static void SeqIrqsFree()
{   
  free_irq(spSeqDrvInfo->mIrqNums.mMsgRdy, NULL);
  free_irq(spSeqDrvInfo->mIrqNums.mMsgAck, NULL); 
} // SeqIrqsFree()

//*****************************************************************************

static void SeqHwReset()
{
  // *** clear message flag bits ***
  // w1c 2x
  spSeqDrvInfo->mpSeqCBregs->MSGCTRL.R = spSeqDrvInfo->mpSeqCBregs->MSGCTRL.R;
  spSeqDrvInfo->mpSeqCBregs->MSGCTRL.R = spSeqDrvInfo->mpSeqCBregs->MSGCTRL.R;
  
  // *** make sure Sequencer HW is in reset ***
  // cm0 clock off, cm0 reset, seq int logig reset
  spSeqDrvInfo->mpSeqCBregs->CLKRST.R = 0x3; 
    
  // disable IRQs to host
  spSeqDrvInfo->mpSeqCBregs->HOST_IRQ_EN.R = 0;
  
  // reset ERROR status
  spSeqDrvInfo->mpSeqCBregs->ERRSTAT.R = spSeqDrvInfo->mpSeqCBregs->ERRSTAT.R;
  
  // reset ready after boot message status
  sSeqRab = 0;
} // SeqHwReset()

/****************************************************************************/
/** 
* @brief Resets the Sequencer HW.
* @details Resets the Sequencer HW.
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
* @implements SEQ_DRV_Reset_Activity 
*****************************************************************************/
void SEQ_DRV_Reset()
{
  spin_lock(&sSeqGlobalLock);
  if((gSeqStatus == SEQ_STATUS_RESET) || (gSeqStatus == SEQ_STATUS_OFF))
  {
    spin_unlock(&sSeqGlobalLock);
    SeqHwReset();
    VDB_LOG_WARNING("SEQ NOT ON.\n");
  } // if in reset already
  else
  {
    gSeqStatus = SEQ_STATUS_RESET;
    spin_unlock(&sSeqGlobalLock);
  
    // *** make sure Sequencer HW is reseted ***
    SeqHwReset();
    
    // clean up streams
    SEQ_DRV_FrmStreamsReset();
    
    // wake up RAB queue - can time-out on its own
    sSeqRab = 0;

    wake_up(&sSeqRabQueue);
    
    // wake up MsgQueue
    spin_lock(&sSeqMsgLock);

    sSeqMsgReady = 1;
    spin_unlock(&sSeqMsgLock);
    wake_up(&sSeqMsgQueue);
    
    // wake up possible ACK queue
    spin_lock(&sSeqMsgLock);    
    SEQ_WDTRemove();

    if(spSeqInbMsg)
    {
      // assert condition
      sSeqAckWait = 1;
      spin_unlock(&sSeqMsgLock);
    
      VDB_LOG_NOTE("Wake up the sender.\n");
      
      // wake up sender
      wake_up(&sSeqAckQueue);
    } // if the sender is waiting
    else
    {
      spin_unlock(&sSeqMsgLock);
    }
    
    spin_lock(&sSeqGlobalLock);
    gSeqStatus = SEQ_STATUS_OFF;
    spin_unlock(&sSeqGlobalLock);  
  } // else from if in reset already  
  
  // enable Sequencer clock
  spSeqDrvInfo->mpSeqCBregs->CLKRST.B.CM0p_CLK_EN = 1;
  // deassert Sequencer module reset (CM0+ stays in RESET) 
  spSeqDrvInfo->mpSeqCBregs->CLKRST.B.SEQ_INT_RESET = 0; 
  
} // SEQ_DRV_Reset()

/****************************************************************************/
/** 
* @brief Attempt to boot Sequencer.
* @details Waits for the ready-after-boot message to arrive. Fails if wait times out or
* reset is done.
* 
* @return SEQ_DRV_SUCCESS if all OK, otherwise SEQ_DRV_FAILURE.
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
* @implements SEQ_DRV_Boot_Activity
*****************************************************************************/
uint32_t SEQ_DRV_Boot()
{  
  uint32_t lRet = SEQ_DRV_SUCCESS;
  uint32_t lRetTmp = 0;
  
  spin_lock(&sSeqGlobalLock);
  if((gSeqStatus == SEQ_STATUS_OFF) && 
     (spSeqDrvInfo->mpSeqCBregs->CLKRST.B.CM0p_RESET == 1))
  {    
    gSeqStatus = SEQ_STATUS_BOOT;
    spin_unlock(&sSeqGlobalLock);
    
    //TDT addresses have to be assigned already by fmda module
    if(gSEQ_DRV_FdmaComm.mppTdt[SEQ_ISP0_TDT_IDX] != NULL)
    {  
      // reset the Error flags just to be sure
      spSeqDrvInfo->mpSeqCBregs->ERRSTAT.R = 
                                    spSeqDrvInfo->mpSeqCBregs->ERRSTAT.R;

      // enable IRQs
      if( sEventLogOn > 0)
      {
        spSeqDrvInfo->mpSeqCBregs->HOST_IRQ_EN.R = 0xff007c;
      }
      else
      {
        spSeqDrvInfo->mpSeqCBregs->HOST_IRQ_EN.R = 0xc;
      }
  
      //write 0b to CM0p_RESET bit to BOOT Sequencer
      spSeqDrvInfo->mpSeqCBregs->CLKRST.B.CM0p_RESET = 0;
      
      // wait for Sequencer "Ready after boot" message
      VDB_LOG_NOTE("Waiting for Sequencer to Boot.\n");
      lRetTmp = wait_event_timeout(sSeqRabQueue, sSeqRab, SEQ_RAB_TIMEOUT);
      VDB_LOG_NOTE("Wait ended with %u\n", lRetTmp);
      
      if( lRetTmp < 2)
      {
        VDB_LOG_NOTE("Wait for Sequencer to boot timed out.\n");
        lRet = SEQ_DRV_FAILURE;
        SEQ_DRV_Reset();
      } // RAB wait timedout
      else
      {
        spin_lock(&sSeqGlobalLock);
        
        if(gSeqStatus == SEQ_STATUS_BOOT)
        {
          gSeqStatus = SEQ_STATUS_ON;
          spin_unlock(&sSeqGlobalLock);
          VDB_LOG_NOTE("Sequencer \"Ready after boot\" message received.\n");
        } // if message received
        else
        {
          spin_unlock(&sSeqGlobalLock);
          lRet = SEQ_DRV_FAILURE;
          SEQ_DRV_Reset();
          VDB_LOG_NOTE("RAB message not received. Sequencer boot failed.\n");
        } // else from if message received
      } // else from if RAB timedout
      
      sSeqRab = 0;  
    } // if TDT found
    else
    {
      gSEQ_DRV_FdmaComm.mppTdt[SEQ_USER_TDT_IDX] = NULL;
      gSEQ_DRV_FdmaComm.mppTdt[SEQ_ISP0_TDT_IDX] = NULL;
      gSEQ_DRV_FdmaComm.mppTdt[SEQ_ISP1_TDT_IDX] = NULL;
      VDB_LOG_ERROR("FDMA TDT not found.\n Aborting boot.\n");
      gSeqStatus = SEQ_STATUS_OFF;
      lRet = SEQ_DRV_FAILURE;
    } // else from if TDT found
  } // if Sequencer in reset
  else
  {
    spin_unlock(&sSeqGlobalLock);
    VDB_LOG_ERROR("Sequencer is not OFF. Aborting boot.\n");
    lRet = SEQ_DRV_FAILURE;
  } // else form if Sequencer in reset
  
  return lRet;
} // SEQ_DRV_Boot()
  
/****************************************************************************/
#ifndef __STANDALONE__
/** 
* @brief   Handler for Sequencer message IRQ.
* @details Handler for Sequencer message IRQ.
* 
* @param aIrq     irq number
* @param apDev_id identification of the device
* 
* @return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
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
* @implements SEQ_MsgIrqHandler_Activity
*
*****************************************************************************/
static irqreturn_t SEQ_MsgIrqHandler(int32_t aIrq, void *apDev_id)
{
  int32_t lRet = IRQ_HANDLED;
  
#else
/** 
* @brief   Handler for Sequencer message IRQ.
* @details Handler for Sequencer message IRQ.
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
* @implements SEQ_MsgIrqHandler_Activity
*****************************************************************************/
void SEQ_MsgIrqHandler()
{
#endif //#ifndef __STANDALONE__  
  SEQ_pram_map_t *lpCommInfo = NULL; 
  uint8_t lChannelRelease = 1; 
 
  // remember there's work
  sTaskletWork++;
    
  if(SEQ_MsgPoolWriteShift(&sS2hMsg) != SEQ_DRV_SUCCESS)
  { 
    lChannelRelease = 0; // remember the channel cannot be released
    
    VDB_LOG_ERROR("Seq2Host message queue full.\n");
  } // if write message shift failed == pool is full
  
  if((sS2hMsg.mpRead->mType < SEQ_MSG_TYPE_S2H_MAX) && 
     (sS2hMsg.mpRead->mType > SEQ_MSG_TYPE_H2S_MAX))
  {
    //invoke instant handler
    if( sMsgInstant[sS2hMsg.mpRead->mType - SEQ_MSG_TYPE_RAB] != NULL)
    {
      sMsgInstant[sS2hMsg.mpRead->mType - SEQ_MSG_TYPE_RAB]();
    } // if instant service set
  } // if a valid message number
  else
  {
    VDB_LOG_ERROR("Invalid Seq -> Host message type %u.\n", 
                  (unsigned) sS2hMsg.mpRead->mType);
  } // else from if a valid message number

  if(lChannelRelease == 1)
  {
    // set next message address to comm info
    lpCommInfo = (SEQ_pram_map_t*) &(spSeqDrvInfo->mpPram[SEQ_PRAM_MAP_OFF]);
    lpCommInfo->mOutMsgAddrOff = (uint32_t)((uintptr_t)sS2hMsg.mpWrite - 
                                            (uintptr_t)sS2hMsg.mpBase + 
                                            PRAM_OUT_MSG_OFF + SEQ_PRAM_BASE);
      
    // reset the message bit in CTRL_BLK_MSGCTRL register
    spSeqDrvInfo->mpSeqCBregs->MSGCTRL.R = 0x1;
  }// if msg channel should be released    
   
  // schedule deffered work
  tasklet_schedule(&gSeqMsg_tasklet);

#ifndef __STANDALONE__  
  return lRet;
#endif // #ifndef __STANDALONE__ 
} // SEQ_MsgIrqHandler()    

/****************************************************************************/
#ifndef __STANDALONE__
/** 
* @brief   Handler for HOST message acknowledge IRQ in Linux.
* @details Handler for HOST message acknowledge IRQ in Linux.
* 
* @param aIrq     irq number
* @param apDev_id identification of the device
* 
* @return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
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
* @implements SEQ_MsgAckIrqHandler_Activity
*
*****************************************************************************/
static irqreturn_t SEQ_MsgAckIrqHandler(int32_t aIrq, void *apDev_id)
{
  int32_t ret = IRQ_HANDLED;
  
  // remove watch dog timer
  SEQ_WDTRemove();  
#else
/** 
* @brief   Handler for HOST message acknowledge IRQ in SA.
* @details Handler for HOST message acknowledge IRQ in SA.
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
* @implements SEQ_MsgAckIrqHandler_Activity
*****************************************************************************/
void SEQ_MsgAckIrqHandler()
{
#endif //#ifndef __STANDALONE__  

  // reset the message bit in CTRL_BLK_MSGCTRL register
  spSeqDrvInfo->mpSeqCBregs->MSGCTRL.R = 0x20000;
  
  // *** if required wake up the sender ****
  if(spSeqInbMsg)
  {
    // copy message content
    *spSeqInbMsg = *sH2sMsg.mpBase;
        
    // assert condition
    sSeqAckWait = 1;
    
    // wake up sender
    wake_up(&sSeqAckQueue);
  } // if the sender is waiting
  
  //TODO: move WAKEING UP to tasklet !!!
  // reenable channel
  sSeqMsgReady = 1;     

  // wake up other possible senders
  wake_up_interruptible(&sSeqMsgQueue);

#ifndef __STANDALONE__
  return ret;
#endif // ifndef __STANDALONE__
} // SeqMsgAckIrqHandler()

//*****************************************************************************

#if (ISP_EVT_LOG_LEN > 0)
void SEQ_DRV_IrqIpusdoneProcess()
{
  SEQ_EventRecord_t *lpEvent = &(sEventLog.mpRecords[sEventLog.mNextRecIdx++]);
  if(sEventLog.mNextRecIdx > (ISP_EVT_LOG_LEN - 1)) 
  {
    sEventLog.mNextRecIdx = ISP_EVT_LOG_LEN - 1;
  }
  lpEvent->mEvent.R = (ISP_LOGEVT_IPUS << 26) |
                      (spSeqDrvInfo->mpSeqECregs->IPUS_CNTR_ALLEVT.R & 
                       0x000000ff);
  lpEvent->mTime  = jiffies();
} // SEQ_DRV_IrqIpusdoneProcess

//*****************************************************************************

void SEQ_DRV_IrqIpuvdoneProcess()
{
  SEQ_EventRecord_t *lpEvent = &(sEventLog.mpRecords[sEventLog.mNextRecIdx++]);
  if(sEventLog.mNextRecIdx > (ISP_EVT_LOG_LEN - 1)) 
  {
    sEventLog.mNextRecIdx = ISP_EVT_LOG_LEN - 1;
  }
  lpEvent->mEvent.R = (ISP_LOGEVT_IPUV << 26) | 
                       (spSeqDrvInfo->mpSeqECregs->IPUV_CNTR_ALLEVT.R &
                        0x0000000f);
  lpEvent->mTime  = jiffies();
} // SEQ_DRV_IrqIpuvdoneProcess

//*****************************************************************************

void SEQ_DRV_IrqOtherdoneProcess()
{
  SEQ_EventRecord_t *lpEvent = &(sEventLog.mpRecords[sEventLog.mNextRecIdx++]);
  if(sEventLog.mNextRecIdx > (ISP_EVT_LOG_LEN - 1)) 
  {
    sEventLog.mNextRecIdx = ISP_EVT_LOG_LEN - 1;
  }
  lpEvent->mEvent.R = (ISP_LOGEVT_OTHER << 26) | 
                      (spSeqDrvInfo->mpSeqECregs->OTHR_CNTR_ALLEVT.R &
                       0x03ffffff);
  
  lpEvent->mTime  = jiffies();
} // SEQ_DRV_IrqOthedoneProcess

//*****************************************************************************

void SEQ_DRV_IrqSwEventProcess()
{
  SEQ_EventRecord_t *lpEvent = &(sEventLog.mpRecords[sEventLog.mNextRecIdx++]);
  if(sEventLog.mNextRecIdx > (ISP_EVT_LOG_LEN - 1)) 
  {
    sEventLog.mNextRecIdx = ISP_EVT_LOG_LEN - 1;
  }
  lpEvent->mEvent.R = (ISP_LOGEVT_SW_EVT << 26) | 
                      (spSeqDrvInfo->mpSeqECregs->SW_EVT_TRIG.R &
                       0x000000ff);
  
  lpEvent->mTime  = jiffies();  
  spSeqDrvInfo->mpSeqECregs->SW_EVT_CLR_DONE.R = 
                      (spSeqDrvInfo->mpSeqECregs->SW_EVT_TRIG.R & 0x000000ff);
} // SEQ_DRV_IrqSwEventProcess()
#endif // #if (ISP_EVT_LOG_LEN > 0)

/****************************************************************************/
/** 
* @brief Passes Sequencer events to user application.
* @details Passes Sequencer events to user application. 
*          In Linux sends signal.
*          In standalone invokes handler directly.
* 
* @param aPid   pid of the destination process
* @param aValue number to be attached to the signal
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
* @implements Sig2UsrSend_Activity
*
*****************************************************************************/

inline static void Sig2UsrSend(int32_t aPid, int32_t aValue)
{
  // invoke proper functionality
  spSeqDrvInfo->mpSigInvoke(aPid, aValue);
} // Sig2UsrSend()
 
//*****************************************************************************
volatile uint64_t sTime = 0;
static void SEQ_S2hMsgTasklet(unsigned long aArg)
{     
  // if there is any work left?
  while(sS2hMsg.mpWrite != sS2hMsg.mpRead)
  {    
    // remember oldest message pointer
    SEQ_Message_t *pMsg = sS2hMsg.mpRead;
        
    VDB_LOG_NOTE("S2h MSG pointer: %p \n", (void*) sS2hMsg.mpRead);
    // process the oldest message    
    switch(pMsg->mType)
    {
      //case 0x1234:
      case SEQ_MSG_TYPE_RAB: 
      {
        // remember Ready after boot message was received and wake up
        sSeqRab = 1;
        wake_up(&sSeqRabQueue);
        VDB_LOG_NOTE("Sequencer \"Ready after boot\" message received.\n");
        //printk("  RAB\n");
      } // case SEQ_MSG_TYPE_RAB
      break;
      
      case SEQ_MSG_TYPE_SEQDONE:
      {
        VDB_LOG_NOTE("SEQDONE message received.\n");
      } // default
      break;
      case SEQ_MSG_TYPE_IPU_PROFILE_DONE:
      {
        sIpuProfileDone = 1; // remember profiling has finished
      } // case SEQ_MSG_TYPE_IPU_PROFILE_DONE
      case SEQ_MSG_TYPE_FRAMEDONE:
      {
        VDB_LOG_NOTE("FRAMEDONE message received.\n");
        sSeqTimeStat.mFrmDoneCnt++;
        sSeqTimeStat.mLastDoneTime = jiffies;
        
        MsgFrameDone_Handler();
      } // default
      break;
      case SEQ_MSG_TYPE_FDMADONE:
      {
        VDB_LOG_NOTE("FDMADONE message received.\n");
      } // SEQ_MSG_TYPE_FDMADONE /// fallthrough
      case SEQ_MSG_TYPE_FDMAERROR:
      {
        VDB_LOG_NOTE("FDMAERROR message received.\n");
      } // default /// fallthrough
      case SEQ_MSG_TYPE_FDMACRCERROR:
      {
        if(gSEQ_DRV_FdmaComm.mpEvent2FdmaSend)
        {
          SEQM_fdma_event_t *lpEvent = (SEQM_fdma_event_t*) pMsg->mData;
          FDMA_TcEvent_t     lEvent;
          lEvent.mXfrStat        = lpEvent->mXfrStat;
          lEvent.mXfrCalcCrcVal  = lpEvent->mXfrCalcCrcVal;
          lEvent.mXfrCurrDdrPtr  = lpEvent->mXfrCurrDdrPtr;
          lEvent.mXfrCurrSramPtr = lpEvent->mXfrCurrSramPtr;
          lEvent.mXfrRecNum      = lpEvent->mTdIdx;
          lEvent.mSramNextLine   = lpEvent->mSramNextLine; 
          
          gSEQ_DRV_FdmaComm.mpEvent2FdmaSend(&lEvent);
        } // if send defined
        VDB_LOG_NOTE("FDMACRCERROR message received.\n");
      } // default
      break;
      case SEQ_MSG_TYPE_BUFFOVF:
      {        
        VDB_LOG_ERROR("BUFFOVF message received.\n");
      } // default
      break;
      case SEQ_MSG_TYPE_OTHERERROR:
      {
        VDB_LOG_NOTE("OTHERERROR message received.\n");
        VDB_LOG_NOTE("Message: %s\n",&spSeqDrvInfo->mpPram[PRAM_AUX_DATA_OFF]);
      } // default
      break;
      case SEQ_MSG_TYPE_GET_IDLE_CNT:
      {
      } // case SEQ_MSG_TYPE_GET_IDLE_CNT
      break;
      case SEQ_MSG_TYPE_DELTA_START:
      {
        sTimeDeltaStart = jiffies;
      } // case SEQ_MSG_TYPE_DELTA_START
      break;
      case SEQ_MSG_TYPE_DELTA_END:
      {
#ifdef __STANDALONE__  
        SEQM_delta_t*   pDelta    = (SEQM_delta_t*)pMsg->mData;
         // *** delta in uSec ***
        volatile jiffy_t  delta   = sTimeDeltaEnd - sTimeDeltaStart; 
        volatile float  tDeltaSec = ((float)delta)*1.0e-6;
        volatile float  tSeqCyc   = 1.0f/175.0e6; //250.0e6;
        volatile float  nCycle    = tDeltaSec/tSeqCyc;
        volatile float  tSingle   = nCycle/pDelta->mDelta;
        volatile float  tNetto    = tSingle - 5.0f;
        volatile float  fCore     = (8.0f * 
                                     (float)pDelta->mDelta*1.0e6)/(float)delta;
        
        for(;;);  // *** you can break here ***
#endif // ifdef __STANDALONE__
        sTimeDeltaEnd = jiffies;

      } // case SEQ_MSG_TYPE_DELTA_END
      break;
      case SEQ_MSG_TYPE_ECHO:
      {
        VDB_LOG_NOTE("ECHO message received.\n");
      } // default
      break; 
      
      default:
      {
        VDB_LOG_WARNING("Unknow Sequencer message type (%u) received.\n", 
                        (unsigned) pMsg->mType);
      } // default
      break;
    }; // switch message type
    
    // *** send message ***
    if(sSeqUserPid != 0)
    {
      VDB_LOG_NOTE("Sending signal to %u.\n", sSeqUserPid);
      
      Sig2UsrSend(sSeqUserPid, pMsg->mType);
    } // if there is a user
    
    // *** shift read pointer ***
    if(SEQ_MsgPoolReadShift(&sS2hMsg) != SEQ_DRV_SUCCESS)
    {
      VDB_LOG_ERROR("Message pool read shift failed.\n");
      break;
    } // if read shift failed
    else
    {
      // reenable S2H messaging if pool was full  
      if( sS2hMsg.mpWrite == NULL)
      {
        SEQ_pram_map_t *lpCommInfo = NULL;
        // set next message address to comm info
        lpCommInfo = (SEQ_pram_map_t*)&(spSeqDrvInfo->mpPram[SEQ_PRAM_MAP_OFF]);
        lpCommInfo->mOutMsgAddrOff = (uint32_t)((uintptr_t)sS2hMsg.mpWrite -
                                                (uintptr_t)sS2hMsg.mpBase + 
                                                PRAM_OUT_MSG_OFF + 
                                                SEQ_PRAM_BASE);

        // reset the message bit in CTRL_BLK_MSGCTRL register
        spSeqDrvInfo->mpSeqCBregs->MSGCTRL.R = 0x1;
        VDB_LOG_NOTE("Released messaging channel after overflow.");
      } // if pool was full
    } // else from if read shift failed
    
    sTaskletWork--;
  } // while there is a message pending 
}// SEQ_S2hMsgTasklet

//*****************************************************************************

#ifdef SEQ_WDT_ENABLED
static void SEQ_WDTRun(unsigned long aData)
{
  VDB_LOG_ERROR("Sequencer watch dog timer timed out.\n");
  // *** if the sender is waiting ****
  if(spSeqInbMsg)
  { 
    // set message type to ACK timed out
    spSeqInbMsg->mType = SEQ_MSG_TYPE_ACKTO;
    // assert acknowledged condition
    sSeqAckWait = 1;
    
    VDB_LOG_NOTE("Wakeing up sender.\n");
    // wake up sender
    wake_up(&sSeqAckQueue);
  } // if the sender is waiting
  
  // wake up other possible senders
  VDB_LOG_NOTE("Releasing outbound message channel.\n");
  sSeqMsgReady = 1;
  wake_up_interruptible(&sSeqMsgQueue);  
  
  VDB_LOG_NOTE("======== leaving WDTRun() =============\n");
} // SEQ_WDTRun()
#endif // #ifdef SEQ_WDT_ENABLED

//*****************************************************************************

#ifdef SEQ_WDT_ENABLED
static void SEQ_WDTSchedule()
{
  // TODO: in case of Host->Seq MSG queue add synchronization
  // register the timer
#ifndef __STANDALONE__    
  sWDTimer.data = (unsigned long)jiffies;
  sWDTimer.function = SEQ_WDTRun;
  sWDTimer.expires = jiffies + SEQ_WDT_TIMEOUT; // parameter

  add_timer(&sWDTimer);    
  VDB_LOG_NOTE("\n**************** Timer scheduled (jiffies %lu)\n", 
               (unsigned long) sWDTimer.data);
#endif // #ifndef __STANDALONE__  
} // SEQ_WDTSchedule()
#endif // #ifdef SEQ_WDT_ENABLED

//*****************************************************************************

static void SEQ_WDTRemove()
{
  // TODO: in case of Host->Seq MSG queue add synchronization
  // remove the timer
#ifndef __STANDALONE__  
  del_timer(&sWDTimer);
#endif // #ifndef __STANDALONE__  
  VDB_LOG_NOTE("\n********************** Timer removed (jiffies %lu)\n", 
               (unsigned long)jiffies);
} // SEQ_WDTRemove()

/****************************************************************************/
/** 
* @brief Invokes fDMA TD scheduling through Sequencer HW.
* @details Invokes fDMA TD scheduling through Sequencer HW.
* 
* @param apcFdmaTc pointer to Fdma transfer channel
* 
* @return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
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
* @implements SEQ_DRV_FdmaTcScheduleMsgSend_Activity
*
*****************************************************************************/
int32_t SEQ_DRV_FdmaTcScheduleMsgSend(const FDMA_Tc_t* apcFdmaTc)
{
  int32_t ret = SEQ_DRV_SUCCESS;
  
  FDMA_TcEvent_t     lEvent;
  lEvent.mXfrStat        = 0;
  lEvent.mXfrCalcCrcVal  = 0;
  lEvent.mXfrCurrDdrPtr  = 0;
  lEvent.mXfrCurrSramPtr = 0;
  lEvent.mXfrRecNum      = apcFdmaTc->mTdm.mTdIdx;
  lEvent.mSramNextLine   = 0; 
  
  VDB_LOG_NOTE("Invoking fDMA TD scheduling.\n");
  VDB_LOG_FCN_NOT_IMPLEMENTED();
  
  gSEQ_DRV_FdmaComm.mpEvent2FdmaSend(&lEvent);
  
  return ret;
} // SEQ_FdmaTdSchedule(const FDMA_Tc_t* apcFdmaTc);

/****************************************************************************/
/** 
* @brief Sends message to Sequencer.
* @details Sends message to Sequencer.
* 
* @param apMsg pointer to the message structure
* @param aWait if non zero the call blocks until SeqFW acknowledges the MSG.
* 
* @return SEQ_DRV_SUCCESS if all OK, SEQ_DRV_FAILURE otherwise
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
* @implements SEQ_DRV_MsgSend_Activity
*****************************************************************************/
int32_t SEQ_DRV_MsgSend(SEQ_Message_t *apMsg, uint8_t aWait)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  
  // get Sequencer message spinlock
  spin_lock(&sSeqMsgLock);
 
  // remember who's sending the message
#ifndef __STANDALONE__
  sSeqUserPid = current->pid;
#else
  sSeqUserPid = 1;
#endif 
  
  // while channel in use
  while(sSeqMsgReady == 0)
  {
    int32_t wait_ret = 0;
    // release spinlock
    spin_unlock( &sSeqMsgLock);
    wait_ret = wait_event_interruptible(sSeqMsgQueue, sSeqMsgReady);
    if(wait_ret == -ERESTARTSYS)
    {
      lRet = -ERESTARTSYS;
      break;
    } // if interrupted
    // get spinlock to synchronize bettween possible more threads
    spin_lock( &sSeqMsgLock);
  } // while message in use
  
  if(lRet == SEQ_DRV_SUCCESS)
  {
    spin_lock(&sSeqGlobalLock);
    if(gSeqStatus == SEQ_STATUS_ON)
    {
      uint32_t lMsgOffset = 0;
      spin_unlock(&sSeqGlobalLock);
     
      // set message content
      *sH2sMsg.mpBase = *apMsg;
	
      // remember wait state
      spSeqInbMsg = (aWait > 0) ? apMsg : NULL;
      sSeqAckWait = 0;
      
      if(aWait)
      {
        VDB_LOG_NOTE("Wait required.");
      }
      
      // prepare message
      lMsgOffset = (uint32_t)((uintptr_t)sH2sMsg.mpBase - 
                              (uintptr_t)spSeqDrvInfo->mpPram + 
                              SEQ_PRAM_OFFSET);
      spSeqDrvInfo->mpSeqCBregs->INMSG_ADDR.R = lMsgOffset;
	
      // set channel to "in use"
      sSeqMsgReady = 0;
      sH2SmsgCnt++;
      VDB_LOG_NOTE("Message to Sequencer #%u was sent (msg offset 0x%x).\n", 
                   (unsigned) sH2SmsgCnt, 
                   (unsigned) lMsgOffset);
      
      // schedule the watch dog timer
#ifdef SEQ_WDT_ENABLED      
      SEQ_WDTSchedule();
#endif // #ifdef SEQ_WDT_ENABLED
      
      // release spinlock
      spin_unlock(&sSeqMsgLock);
      
      // send the message
      spSeqDrvInfo->mpSeqCBregs->INMSG_LEN.B.INBOUND_MSG_LEN = 
                                                          sizeof(SEQ_Message_t);
      
      // if required, wait for Sequencer to acknowledge
      if(aWait)
      {
        VDB_LOG_NOTE("Waiting for Sequencer to replay.\n");
        // wait for acknowledge event - cannot be interruptible 
        wait_event(sSeqAckQueue, sSeqAckWait);
        
        *apMsg = *sH2sMsg.mpBase;  // read back the message
        
        VDB_LOG_NOTE("ACK wait finished.\n");
      } // if we should wait for the acknowledge
    } // if SEQ is on
    else
    {
      spin_unlock(&sSeqGlobalLock);
      spin_unlock(&sSeqMsgLock);
      VDB_LOG_WARNING("SEQ is not ON. Aborting MSG.\n");
      lRet = SEQ_DRV_FAILURE;
    } // else from if SEQ on   
  } // if all ok so far
  else
  {
    VDB_LOG_ERROR("FAILED to send the message.");
    // release spinlock
    spin_unlock(&sSeqMsgLock);
  } // else from if all ok so far

  return lRet;
} // SEQ_DRV_MsgSend()

//*****************************************************************************

static void SEQ_MsgPoolInit(SEQ_MessagePool_t *apMsgPool, 
                            uint8_t* apMsgBase, 
                            uint32_t mMsgCnt)
{
  apMsgPool->mpWrite =  apMsgPool->mpRead = apMsgPool->mpBase = 
                        (SEQ_Message_t*) apMsgBase;
  apMsgPool->mpLast = (SEQ_Message_t*)((uintptr_t)(apMsgPool->mpBase) + \
                      sizeof(SEQ_Message_t) * (mMsgCnt - 1));
} // SEQ_MsgPoolInit(SEQ_MessagePool_t *apMsgPool, char* apMsgBase)

//*****************************************************************************

int32_t SEQ_MsgPoolWriteShift(SEQ_MessagePool_t *apMsgPool)
{
  int32_t ret = SEQ_DRV_SUCCESS;
    
  if( apMsgPool->mpWrite )
  {  
    SEQ_Message_t *pNext = (apMsgPool->mpWrite + 1);
    if( pNext > apMsgPool->mpLast)
    {
      pNext = apMsgPool->mpBase;
    } // if wrapparound
    if(pNext == apMsgPool->mpRead)
    {
      // remember overflow
      ret = SEQ_DRV_FAILURE;
      pNext = NULL;
    } // if message pool overflow 
    apMsgPool->mpWrite = pNext;
  } // if no unresolved overflow
  else
  {
    ret = SEQ_DRV_SUCCESS;
  } // else from if no unresolved overflow
    
  return ret;
} // SEQ_MsgPoolWriteShift(SEQ_MessagePool_t *apMsgPool)

//*****************************************************************************

static int32_t SEQ_MsgPoolReadShift(SEQ_MessagePool_t *apMsgPool)
{
  int32_t ret = SEQ_DRV_SUCCESS;

  if(apMsgPool->mpWrite == apMsgPool->mpRead)
  {
    ret = SEQ_DRV_FAILURE;
  } // if no message to shift to
  else
  {
    SEQ_Message_t *pNext = (apMsgPool->mpRead + 1);
    if( pNext > apMsgPool->mpLast)
    {
      pNext = apMsgPool->mpBase;
    } // if wrapparound    

    apMsgPool->mpRead = pNext;
  } // else from if no message to shift to  
  
  return ret;
} // SEQ_MsgPoolReadShift(SEQ_MessagePool_t *apMsgPool)

//*****************************************************************************

static inline void MsgFrameDoneInstant()
{
#ifdef ISP_FRAME_DONE_LOG
  SEQ_EventRecord_t *lpEvent = NULL;
  lpEvent = &(sEventLog.mpRecords[sEventLog.mNextRecIdx++]);
  if(sEventLog.mNextRecIdx > (ISP_EVT_LOG_LEN - 1)) 
  {
    sEventLog.mNextRecIdx = ISP_EVT_LOG_LEN - 1;
  }
  lpEvent->mEvent.R = (ISP_LOGEVT_SEQ_FD << 26);
  lpEvent->mTime  = jiffies;
#endif // ifdef ISP_FRAME_DONE_LOG
} // MsgFrameDoneInstant()

//*****************************************************************************

static inline void MsgFrameDone_Handler()
{
  uint32_t i = 0;
  //VDB_LOG_NOTE("FrameDone instant message handler invoked.\n");
  
  SEQM_frame_end_t*   pFrmEndMsg = (SEQM_frame_end_t*)sS2hMsg.mpRead->mData;
  
  // *** update transfer descriptors ***
  
  for(i = 0; i < FDMA_CHNL_CNT; i++)
  {
    SEQ_FrmStream_t *lpStream = spFrmStreams[i];
    // find ddr buffer lists
    if(lpStream != NULL)
    {
      unsigned long lFlags;
      FDMA_Td_t *lpTd = &(gSEQ_DRV_FdmaComm.mppTdt[sTdtIdx][i]);
      // lock and save the irq state, if locked already it is saved in flags
      spin_lock_irqsave(&sStreamLock, lFlags); 
     
      //*** shift buffers and queues ***
      if((SEQ_FrmBuffer_t*)lpStream->mWorkQueue.mpHead->mpData != \
         lpStream->mpNextBuffer)
      {   
        // update queues
        struct VDList_Node *lpFrmNode = VDList_HeadPop(&(lpStream->mWorkQueue));       
        SEQ_FrmBuffer_t *lpFrmBuffer  = (SEQ_FrmBuffer_t*)(lpFrmNode->mpData);
        // update frame sequence number
        lpFrmBuffer->mFrmSeqNum = pFrmEndMsg->mFrameSeq; 
        VDList_TailPush(&(lpStream->mDoneQueue), 
                        lpFrmNode);
      } // if next is different from head
      
      if(lpStream->mWorkQueue.mLength > 1)
      {   
        lpStream->mpNextBuffer = 
          (SEQ_FrmBuffer_t*)lpStream->mWorkQueue.mpHead->mpNext->mpData;
      } // if more then one left in work queue
      
      //*** update TDT ***
      
      lpTd->mDdrImgStartAddr = lpStream->mpNextBuffer->mPhyAddr;
      lpTd->mDdrCurrLine     = 0;
      if(lpStream->mSramLpF != 0)
      {
        // recompute initial SRAM line index
        lpTd->mSramCurrLine  = (2*lpStream->mSramLpF + // for next transfer
                                lpTd->mSramCurrLine - lpTd->mDdrLineCnt) %
                                lpTd->mSramLineCnt;  
      } // if lines per frame set => free running SRAM buffer      
      else
      {
        lpTd->mSramCurrLine = 0;
      } // else if lines per frame set => free running SRAM buffer      
      // unlock and return to the previous state specified in flags
      spin_unlock_irqrestore(&sStreamLock, lFlags);    
    } // if stream setup
  } // for all possible channels
  
  sTdtIdx = (sTdtIdx + 1)%2; 
  
  // *** fetch IPU histograms ***
  for(i = 0; i < IPU_HIST_CNT; i++)
  {
    SEQ_DRV_IpuHistRead(spIpuHists[i]);
    if(spIpuHists[i])
    {
      spIpuHists[i]->mFrmSeqNum = pFrmEndMsg->mFrameSeq;
    }
  } // for all histogram capable IPUs
  
  if(spIpuStat != NULL)
  {
    SEQ_DRV_IpuStatRead(spIpuStat);
    spIpuStat->mFrmSeqNum = pFrmEndMsg->mFrameSeq;
  }
} // MsgFrameDone_Handler()

//*****************************************************************************

static inline void MsgSeqDone_Handler()
{
  uint32_t i = 0;
  //VDB_LOG_NOTE("FrameDone instant message handler invoked.\n");
  
  for(i = 0; i < FDMA_CHNL_CNT; i++)
  {
    SEQ_FrmStream_t *lpStream = spFrmStreams[i];
    // find ddr buffer lists
    if(lpStream != NULL)
    {
      //*** pop clean work queue ***
      while(lpStream->mWorkQueue.mLength > 0)
      {
        ((SEQ_FrmBuffer_t*)VDList_HeadPop(&(lpStream->mWorkQueue))->mpData)\
          ->mState = SEQ_FRM_BUFFER_STATE_USER;
      } // while work queue not empty
      
      lpStream->mpNextBuffer = NULL;
    } // if stream setup
  } // for all possible channels
  
  sTdtIdx  = 0;  
  sGraphOn = 0;
} // MsgSeqDone_Handler()

/****************************************************************************/
/** 
* @brief Downloads Graph to PRAM location.
* @details Downloads Graph to PRAM location.
* 
* @param apGraph pointer to graph structure in user-space
* 
* @return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
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
* @implements SEQ_DRV_GraphDownload_Activity
*
*****************************************************************************/
int32_t SEQ_DRV_GraphDownload(SEQ_GraphPackage_t *apGraph)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  SEQ_GraphPackage_t lGraph;
  if (copy_from_user( &lGraph, 
                      apGraph, 
                      sizeof(SEQ_GraphPackage_t)) != 0)
  {
    VDB_LOG_ERROR("Copy from user failed.\n");
    lRet = -EIO;
  } // if copy from user failed
  else
  {   
    // remember graph limits
    sCSINumLines = lGraph.mCSINumLines;
    if((lGraph.mGraphSize + lGraph.mBufferListSize) > SEQ_GRAPH_SIZE_MAX)
    {
      VDB_LOG_ERROR("Graph + Buffer list too big "
                    "(0x%x bytes; max allowed 0x%x bytes).\n", 
                    (unsigned) (lGraph.mGraphSize + lGraph.mBufferListSize), 
                    (unsigned) SEQ_GRAPH_SIZE_MAX);
      lRet = -EIO;
    } // if graph too big
    else
    {
      SEQ_pram_map_t *lpCommInfo = 
           (SEQ_pram_map_t*) &(spSeqDrvInfo->mpPram[SEQ_PRAM_MAP_OFF]); 
      void *lpM0Graph = (void*)&spSeqDrvInfo->mpCram[lpCommInfo->mGraphOff];
      
      // copy the graph to apropriate slot
      if (copy_from_user(lpM0Graph, (uint8_t *)lGraph.mpGraphData, 
          lGraph.mGraphSize) != 0)
      {
        VDB_LOG_ERROR("Graph data download failed.\n");
        lRet = SEQ_DRV_FAILURE;
      } // if graph data download failed
      else 
      {
        uint16_t lBuffListOff = (uint16_t)ALIGN128(lpCommInfo->mGraphOff + \
                                lGraph.mGraphSize);
        VDB_LOG_NOTE("Graph downloaded successfully at %p.\n", 
                     (void*)  lpM0Graph);
        lpCommInfo->mBufferListOff = lBuffListOff;
        //lpCommWords[1] = (lpCommWords[1] & 0xff00) || lBuffListOff;
        if(lGraph.mpBufferList != NULL)
        {
          void *lpBufferList = (void*)((uintptr_t)spSeqDrvInfo->mpCram + \
                               lpCommInfo->mBufferListOff);
          if (copy_from_user( lpBufferList, 
                              (uint8_t *)lGraph.mpBufferList, 
                              lGraph.mBufferListSize) != 0)
          {
            uint16_t lBuffListOff = (uint16_t)0x1000;
            VDB_LOG_ERROR("Graph BufferList download failed.\n");
            lpCommInfo->mBufferListOff = lBuffListOff;
            lRet = SEQ_DRV_FAILURE;
          } // if graph data download failed
          else 
          {
            VDB_LOG_NOTE("Graph BufferList downloaded successfully at %p.\n", 
                         (void*) lpBufferList);
          } // else from if graph download failed
        } // if there is a buffer list
        else
        {
          uint16_t lBuffListOff = (uint16_t)0x1000;
          lpCommInfo->mBufferListOff = lBuffListOff;
          VDB_LOG_NOTE("No graph BufferList in the package.\n");
        } // else from if there is a buffer list
        
      } // else form if graph data download failed
    } // else from if graph too big
    
  } // else from if copy from user failed
  return lRet;
} // SEQ_DRV_GraphDownload(SEQ_GraphPackage_t *apGraph)

/****************************************************************************/
/** 
* @brief Copies the Patch list to PRAM.
* @details Copies the Patch list to PRAM.
* 
* @param  apPatchArr patch array pointer
* @param  aPatchNum  number of patch lists in the array         
* 
* @return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
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
* @implements SEQ_DRV_PatchListSet_Activity
 ****************************************************************************/
int32_t SEQ_DRV_PatchListSet(SEQ_patch_list_t *apPatchArr, 
                             uint32_t          aPatchNum)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  if(aPatchNum > 0)
  {
    if(aPatchNum <= SEQ_PATCH_LIST_NUM)
    {
      if(apPatchArr != NULL)
      { 
        uint8_t *lpPatchList; 
        lpPatchList = \
          &(spSeqDrvInfo->mpPram[PRAM_PATCH_LIST_OFF]);
        
        memcpy(lpPatchList, 
               apPatchArr, 
               sizeof(SEQ_patch_list_t)*aPatchNum);    
        spSeqDrvInfo->mpCommInfo->mPatchListNum = aPatchNum;        
      } // if not NULL patch list
      else
      {
        VDB_LOG_WARNING("Patch list is NULL.\n");
        lRet = SEQ_DRV_FAILURE;
      } // else from if not NULL patch list
    } // if patch list size in range
    else
    {
      VDB_LOG_ERROR("Patch list too long. (%u/%u given/allowed)\n", 
                    aPatchNum,
                    SEQ_PATCH_LIST_NUM
                   );
      lRet = SEQ_DRV_FAILURE;
    } // else from if patch list size in range
  } // if PatchNum > 0
  else
  {
    spSeqDrvInfo->mpCommInfo->mPatchListNum = 0;
  } // else from if PatchNum > 0  

  return lRet;
} // SEQ_DRV_PatchListSet()

/****************************************************************************/
/** 
* @brief Loads current state of the Graph from PRAM location.
* @details Loads current state of the Graph from PRAM location.
* 
* @param apGraph pointer to graph structure in user-space
* 
* @return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
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
* @implements SEQ_DRV_GraphFetch_Activity
*
*****************************************************************************/
int32_t SEQ_DRV_GraphFetch(SEQ_GraphPackage_t *apGraph)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  SEQ_GraphPackage_t lGraph;
  if (copy_from_user( &lGraph, 
                      apGraph, 
                      sizeof(SEQ_GraphPackage_t)) != 0)
  {
    VDB_LOG_ERROR("Copy from user failed.\n");
    lRet = -EIO;
  } // if copy from user failed
  else
  {   
    if((lGraph.mGraphSize + lGraph.mBufferListSize) > SEQ_GRAPH_SIZE_MAX)
    {
      VDB_LOG_ERROR("Graph + Buffer list too big "
                    "(0x%x bytes; max allowed 0x%x bytes).\n", 
                    (unsigned) (lGraph.mGraphSize + lGraph.mBufferListSize), 
                    (unsigned) SEQ_GRAPH_SIZE_MAX);
      lRet = -EIO;
    } // if graph too big
    else
    {
      SEQ_pram_map_t *lpCommInfo = (SEQ_pram_map_t*) &
                                   (spSeqDrvInfo->mpPram[SEQ_PRAM_MAP_OFF]); 
      void *lpM0Graph = (void*)&spSeqDrvInfo->mpCram[lpCommInfo->mGraphOff];
      // copy the graph to user
      if (copy_to_user((uint8_t *)lGraph.mpGraphData, lpM0Graph, 
          lGraph.mGraphSize) != 0)
      {
        VDB_LOG_ERROR("Graph data fetch failed.\n");
        lRet = SEQ_DRV_FAILURE;
      } // if graph data fetch failed
    } // else from if graph too big
  } // else from if copy from user failed
  return lRet;
} // SEQ_DRV_GraphFetch(SEQ_GraphPackage_t *apGraph)

/****************************************************************************/
/** 
* @brief Executes the Graph.
* @details Executes the Graph.
* 
* @param aFrmCnt number of frames to capture
* @param aFrmCnt number of lines per input frame (0 => given by CSI)
* 
* @return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
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
* @implements SEQ_DRV_GraphStart_Activity
*
*****************************************************************************/
int32_t SEQ_DRV_GraphStart(uint32_t aFrmCnt, uint32_t aInputLines)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  
  uint32_t i = 0;
  
  if(gSEQ_DRV_FdmaComm.mppTdt[0] != NULL)
  {
    // reset time statistics
    sSeqTimeStat.mFrmDoneCnt   = (uint64_t)0;
    sSeqTimeStat.mStartTime    = (uint64_t)0;
    sSeqTimeStat.mLastDoneTime = (uint64_t)0;
    
    // check/prepare streams
    // clean up streams
    for(i = 0; i < FDMA_CHNL_CNT; i++)
    {
      SEQ_FrmStream_t *lpStream = spFrmStreams[i];
      if(lpStream != NULL)
      {
        FDMA_Td_t *lpTd0 = &(gSEQ_DRV_FdmaComm.mppTdt[SEQ_ISP0_TDT_IDX][i]);
        FDMA_Td_t *lpTd1 = &(gSEQ_DRV_FdmaComm.mppTdt[SEQ_ISP1_TDT_IDX][i]);
        if(lpStream->mWorkQueue.mLength == 1)
        {
          // init FDMA transfer descriptors
          SEQ_FrmBuffer_t *lpBuffer = 
            (SEQ_FrmBuffer_t*)lpStream->mWorkQueue.mpHead->mpData;
          lpTd0->mDdrImgStartAddr = lpBuffer->mPhyAddr;
          lpTd0->mDdrCurrLine     = 0;
          lpTd0->mSramCurrLine    = lpStream->mSramLineOffs % 
                                    lpTd0->mSramLineCnt;
          // next buffer is the same one
          lpStream->mpNextBuffer = lpBuffer;
          
          // copy the same setup to second TDT
          *lpTd1 = *lpTd0;
          // update only sram current line
          lpTd1->mSramCurrLine    = \
            (lpStream->mSramLineOffs + lpStream->mSramLpF) % 
            lpTd1->mSramLineCnt;
        } // if there is exactly one buffer in WORK queue  
        else if(lpStream->mWorkQueue.mLength > 1)
        {
          // init FDMA transfer descriptor in first TDT
          SEQ_FrmBuffer_t *lpBuffer = 
            (SEQ_FrmBuffer_t*)lpStream->mWorkQueue.mpHead->mpData;
          lpTd0->mDdrImgStartAddr = lpBuffer->mPhyAddr;
          lpTd0->mDdrCurrLine     = 0;
          lpTd0->mSramCurrLine    = lpStream->mSramLineOffs % 
                                    lpTd0->mSramLineCnt;
          
          // init FDMA transfer descriptor in second TDT
          lpBuffer               = 
                  (SEQ_FrmBuffer_t*)lpStream->mWorkQueue.mpHead->mpNext->mpData;
          lpTd1->mDdrImgStartAddr = lpBuffer->mPhyAddr;
          lpTd1->mDdrCurrLine     = 0;
          lpTd1->mSramCurrLine    = (lpStream->mSramLineOffs + 
                                     lpStream->mSramLpF) % lpTd1->mSramLineCnt;
          
          // next buffer is the second one in workqueue
          lpStream->mpNextBuffer = lpBuffer;
        } // if more than one buffer set in WORK queue  
        else
        {
          VDB_LOG_ERROR("Work queue for stream %u is empty. "
                        "Unable to start graph.\n");
          lRet = SEQ_DRV_FAILURE;
        } // if ther is no buffer in work queue
      } // if stream exists
    } // for all possible streams
  } // if FDMA connected properly
  else
  {
    lRet = SEQ_DRV_FAILURE;
  } // else from if FDMA connected properly  
  
  // send START message to Sequencer
  if(lRet == SEQ_DRV_SUCCESS)
  {
    SEQ_Message_t   msg;
    SEQM_start_t*   pSt = (SEQM_start_t*)&msg.mData;
    
    msg.mType   = SEQ_MSG_TYPE_START;
    msg.mStatus = SEQ_MSG_STAT_READY;
    
    pSt->mNumCamLines   = (aInputLines>0)?aInputLines:sCSINumLines; 
    pSt->mFDMAlistBase0 = gSEQ_DRV_FdmaComm.mpTdtPhysAddr[SEQ_ISP0_TDT_IDX];
    pSt->mFDMAlistBase1 = gSEQ_DRV_FdmaComm.mpTdtPhysAddr[SEQ_ISP1_TDT_IDX];
    pSt->mFrames2Grab   = aFrmCnt;

    sGraphOn = 1;
    lRet     = SEQ_DRV_MsgSend(&msg, 1);
    
    sSeqTimeStat.mStartTime = jiffies;
    
  } // if all ok so far
  
  return lRet;
} // SEQ_DRV_GraphStart()

/****************************************************************************/
/** 
* @brief Requests graceful stop of the Graph execution.
* @details Current frame will be finished.
* 
* @param aWait waits for Sequencer ACK if > 0
* 
* @return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
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
* @implements SEQ_DRV_GraphStop_Activity
 ****************************************************************************/
int32_t SEQ_DRV_GraphStop(uint8_t aWait)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
   
  // send STOP message to Sequencer
  {
    SEQ_Message_t   msg;
      
    msg.mType   = SEQ_MSG_TYPE_STOP;
    msg.mStatus = SEQ_MSG_STAT_READY;

    lRet = SEQ_DRV_MsgSend(&msg, aWait);
  } // send STOP message to Sequencer
  
  // check/reset streams
  
  return lRet;
} // SEQ_DRV_GraphStop()

#ifdef __STANDALONE__
/****************************************************************************/
/** 
* @brief Returns 0 if graph execution is off.
* @details Returns 0 if graph execution is off.
* 
* @return 0 if graph is off
*         1 if graph is on
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
* @implements SEQ_DRV_GraphStateGet_Activity
 ****************************************************************************/
int32_t SEQ_DRV_GraphStateGet()
{
  return sGraphOn;
} // SEQ_DRV_GraphStateGet()

//*****************************************************************************

uint32_t SEQ_DRV_EventLogEnable()
{  
  uint32_t lRet = SEQ_DRV_SUCCESS;
#if (ISP_EVT_LOG_LEN > 0)
  uint32_t lRetTmp = 0;
   
  spin_lock(&sSeqGlobalLock);
  
  sEventLogOn = 1;
  
  // enable all IRQs 
  spSeqDrvInfo->mpSeqCBregs->HOST_IRQ_EN.R |= 0xff007f;
  
  spin_unlock(&sSeqMsgLock);
#endif  
  return lRet;
} // SEQ_DRV_EventLogEnable()

//*****************************************************************************

uint32_t SEQ_DRV_EventLogDisable()
{  
  uint32_t lRet = SEQ_DRV_SUCCESS;
  
  spin_lock(&sSeqGlobalLock);
  
  sEventLogOn = 0;
  
  // enable MSG IRQs only
  spSeqDrvInfo->mpSeqCBregs->HOST_IRQ_EN.R = 0xc;
  
  spin_unlock(&sSeqMsgLock);
  
  return lRet;
} // SEQ_DRV_EventLogDisable()

//*****************************************************************************

int32_t SEQ_DRV_EventLogGet(SEQ_EventLog_t **appEventLog)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  if(appEventLog != NULL)
  {
    *appEventLog = &sEventLog;
  }
  else
  {
    lRet = SEQ_DRV_FAILURE;
  }
  
  return lRet;
} // SEQ_DRV_EventLogGet()

#endif // #ifdef __STANDALONE__

/****************************************************************************/
/** 
* @brief Requests frame buffers for one frame stream.
* 
* @details Based on data given in "Request" structure prepares specified stream and
* allocates framebuffer structures. Addresses for buffer data have to be 
* included in the request structure.
* Request for 0 buffer count makes the previous stream setup to be released.
* 
* @param apRequest defines request details
* 
* @return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
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
* @implements SEQ_DRV_FrmBuffersRequest_Activity
*
*****************************************************************************/
int32_t SEQ_DRV_FrmBuffersRequest(SEQ_BufferRequest_t *apRequest)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  uint32_t i = 0;
  SEQ_FrmStream_t *lpStream = spFrmStreams[apRequest->mStreamIdx];
  
  // check input structure
  if((apRequest == NULL) || (apRequest->mStreamIdx >= FDMA_CHNL_CNT))
  {
    VDB_LOG_ERROR("Bad input structure.\n");
    goto final_return;
  } // if bad input structure
  
  //TODO: lock the driver
  
  //TODO: check for STREAM is OFF
  
  // if 0 buffers requested => deallocate last
  if(apRequest->mCnt == 0)
  {
    // check if stream exists
    if( lpStream != NULL)
    {
      kfree(lpStream);
      spFrmStreams[apRequest->mStreamIdx] = NULL;
    } // if srteam exists
    
    goto final_return;
  } // if mCnt == 0
  // check if stream not set already
  if(lpStream != NULL)
  {
    lRet = SEQ_DRV_FAILURE;
    VDB_LOG_ERROR("Stream exists already. No action taken.\n");
    goto final_return;
  } // if stream exists already
  
  // check input structure
  if(apRequest->mpBuffers == NULL)
  {
    VDB_LOG_ERROR("Bad input structure.\n");
    goto final_return;
  } // if bad input structure

  // allocate new stream structure
  lpStream = (SEQ_FrmStream_t*) kmalloc(sizeof(SEQ_FrmStream_t), GFP_KERNEL);
  if(lpStream == NULL)
  {
    VDB_LOG_ERROR("Stream mem allocation failed.\n");
    lRet = SEQ_DRV_FAILURE;
    goto final_return;
  } // if mem alloc failed
  
  lpStream->mpBufferTable = \
    (SEQ_FrmBuffer_t*) kmalloc(sizeof(SEQ_FrmBuffer_t)*apRequest->mCnt, 
                               GFP_KERNEL);
    
  if(lpStream == NULL)
  {
    kfree(lpStream);
    VDB_LOG_ERROR("Frm buffer table mem allocation failed.\n");
    lRet = SEQ_DRV_FAILURE;
    goto final_return;
  } // if mem alloc failed
  
  // init stream
  lpStream->mCnt          = apRequest->mCnt;
  lpStream->mBufferSize   = apRequest->mBufferSize;
  lpStream->mpNextBuffer  = NULL;
  lpStream->mSramLpF      = apRequest->mSramLpF;
  lpStream->mSramLineOffs = apRequest->mSramLineOffs;
  VDList_Init(&(lpStream->mWorkQueue));
  VDList_Init(&(lpStream->mDoneQueue));
  
  // init buffers
  for(i = 0; i < lpStream->mCnt; i++)
  {
    lpStream->mpBufferTable[i].mBufferIdx  = i;
    lpStream->mpBufferTable[i].mPhyAddr    = apRequest->mpBuffers[i] + 
                                             apRequest->mBaseShift;
    lpStream->mpBufferTable[i].mState      = SEQ_FRM_BUFFER_STATE_USER;
    lpStream->mpBufferTable[i].mStreamIdx  = apRequest->mStreamIdx;
    
    VDList_NodeInit(&(lpStream->mpBufferTable[i].mVDListNode), 
                    (void*)&(lpStream->mpBufferTable[i]));
  } // for all buffers
  
  spFrmStreams[apRequest->mStreamIdx] = lpStream;      
  
  // TODO: unlock the driver
  
final_return:
  return lRet;
} // SEQ_DRV_FrmBuffersRequest()
  
//*****************************************************************************

void SEQ_DRV_FrmStreamsReset()
{
  uint32_t i;
  
  // clean up streams
  for(i = (uint32_t)0; i < FDMA_CHNL_CNT; i++)
  {
    SEQ_FrmStream_t *lpStream = spFrmStreams[i];
    if(lpStream != NULL)
    {
      if(lpStream->mpBufferTable != NULL)
      {
        kfree(lpStream->mpBufferTable);
      } // if buffer table exists
      kfree(lpStream);
      spFrmStreams[i] = NULL;
    } // if stream exists
  } // for all possible streams
} // FrmStreamsReset()

/****************************************************************************/
/** 
* @brief Queries buffer at given location.
* 
* @details Based on buffer location specified by the stream and buffer index in 
* BufferInfo structure the status and size of the buffer is updated in 
* the structure.
* 
* @param apRequest defines request details
* 
* @return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE if no such buffer exists
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
* @implements SEQ_DRV_FrmBufferQuery_Activity
*
*****************************************************************************/
int32_t SEQ_DRV_FrmBufferQuery(SEQ_FrmBufferInfo_t *apBufferInfo)
{
  int32_t          lRet   = SEQ_DRV_SUCCESS; 
  SEQ_FrmStream_t *lpStrm = spFrmStreams[apBufferInfo->mStreamIdx];
  
  // check if buffer exists
  if((lpStrm != NULL) && 
     (lpStrm->mCnt > apBufferInfo->mBufferIdx))
  {
    apBufferInfo->mState = 
      lpStrm->mpBufferTable[apBufferInfo->mBufferIdx].mState;
    apBufferInfo->mSize  = lpStrm->mBufferSize;
  } // if buffer exists
  else
  {
    VDB_LOG_WARNING("No such buffer\n");
    lRet = SEQ_DRV_FAILURE;
  } // if no such buffer
  
  return lRet;
} // SEQ_DRV_FrmBuffersRequest()

/****************************************************************************/
/** 
* @brief Gets current state of temporal frame statistics.
* 
* @details Gets current state of temporal frame statistics.
* 
* @return pointer to temporal statistics structure
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
* @implements SEQ_DRV_TimeStatsGet_Activity
*
*****************************************************************************/
SEQ_FrmTemporalStat_t SEQ_DRV_TimeStatsGet(void)
{
#ifdef __STANDALONE__    
  return sSeqTimeStat;
#else  //#ifdef __STANDALONE__  
  SEQ_FrmTemporalStat_t lTmpStat;
  lTmpStat.mFrmDoneCnt   = sSeqTimeStat.mFrmDoneCnt;
  lTmpStat.mLastDoneTime = jiffies_to_usecs(sSeqTimeStat.mLastDoneTime);
  lTmpStat.mStartTime    = jiffies_to_usecs(sSeqTimeStat.mStartTime);
  return lTmpStat;
#endif // else from #ifdef __STANDALONE__   
} // SEQ_DRV_TimeStatsGet()

/****************************************************************************/
/** 
* @brief Adds frame buffer to work queue.
* 
* @details Performs checks if buffer is available to user.
* 
* @param apBufferInfo buffer identification (stream + buffer index).
* 
* @return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
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
* @implements SEQ_DRV_FrmBufferPush_Activity
*
*****************************************************************************/
int32_t SEQ_DRV_FrmBufferPush(SEQ_FrmBufferInfo_t *apBufferInfo)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  SEQ_FrmStream_t *lpStream = NULL;
  
  // check input structure
  if(apBufferInfo == NULL)
  {
    lRet = SEQ_DRV_FAILURE;
  } // if input structure NULL
  else if(apBufferInfo->mStreamIdx >= FDMA_CHNL_CNT)
  {
    lRet = SEQ_DRV_FAILURE;
  } // if Stream index out of range
  else
  {
    lpStream = spFrmStreams[apBufferInfo->mStreamIdx];
  } // else input ok
  
  // check if buffer exists
  if((lpStream != NULL) && 
     (lpStream->mCnt > apBufferInfo->mBufferIdx))
  { 
    SEQ_FrmBuffer_t * lpFrmBuffer = 
      &(lpStream->mpBufferTable[apBufferInfo->mBufferIdx]);
    
    if(lpFrmBuffer->mState == SEQ_FRM_BUFFER_STATE_USER)
    {
      unsigned long lFlags;
      // *** push the buffer to WORK queue *** 
      lpFrmBuffer->mState = SEQ_FRM_BUFFER_STATE_WORK;
      // TODO: lock the WORK queue
      // save the state, if locked already it is saved in flags
      spin_lock_irqsave(&sStreamLock, lFlags); 
      VDList_TailPush(&(lpStream->mWorkQueue), &(lpFrmBuffer->mVDListNode));
      // return to the formally state specified in flags   
      spin_unlock_irqrestore(&sStreamLock, lFlags); 
      // TODO: unlock the WORK queue
    }// if buffer in user state
    else
    {
      VDB_LOG_ERROR("Buffer not in user state. Push aborted.\n");
      lRet = SEQ_DRV_FAILURE;
    }// else from if buffer in user state
  } // if buffer exists
  else
  {
    VDB_LOG_ERROR("No such buffer\n");
    lRet = SEQ_DRV_FAILURE;
  } // if no such buffer
  
  return lRet;
} // SEQ_DRV_FrmBufferPush(SEQ_FrmBufferInfo_t *apBuffInfo)

/****************************************************************************/
/** 
* @brief Removes frame buffer from DONE queue.
* @details Removes frame buffer from DONE queue.
* 
* @param apBufferInfo buffer identification (stream + buffer index).
* 
* @return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
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
* @implements SEQ_DRV_FrmBufferPop_Activity
*
*****************************************************************************/
int32_t SEQ_DRV_FrmBufferPop(SEQ_FrmBufferInfo_t *apBufferInfo)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  SEQ_FrmStream_t *lpStream = NULL;
  uint32_t cnt, index;
  
  // check input structure
  if(apBufferInfo == NULL)
  {
    lRet = SEQ_DRV_FAILURE;
  } // if input structure NULL
  else if(apBufferInfo->mStreamIdx >= FDMA_CHNL_CNT)
  {
    lRet = SEQ_DRV_FAILURE;
  } // if Stream index out of range
  else
  {
    lpStream = spFrmStreams[apBufferInfo->mStreamIdx];
  } // input OK
  
  cnt = (uint8_t)(lpStream->mCnt);
  index = apBufferInfo->mStreamIdx;
  // check if stream exists
  if(lpStream != NULL)
  {    
    if(!VDList_Empty(&(lpStream->mDoneQueue)))
    {
      // *** pop a buffer from DONE queue ***
      unsigned long lFlags;
      SEQ_FrmBuffer_t * lpFrmBuffer = NULL;
      // save the state, if locked already it is saved in flags
      spin_lock_irqsave(&sStreamLock, lFlags); 
      // TODO: lock the DONE queue
      lpFrmBuffer = 
        (SEQ_FrmBuffer_t*)(VDList_HeadPop(&(lpStream->mDoneQueue))->mpData);
      // TODO: unlock the DONE queue
      // return to the formally state specified in flags      
      spin_unlock_irqrestore(&sStreamLock, lFlags); 
      
      //printk("FrmBuffer %p\n",lpFrmBuffer);
      lpFrmBuffer->mState = SEQ_FRM_BUFFER_STATE_USER;     
        
      apBufferInfo->mBufferIdx = lpFrmBuffer->mBufferIdx; 
      apBufferInfo->mFrmSeqNum = lpFrmBuffer->mFrmSeqNum;
      apBufferInfo->mState     = SEQ_FRM_BUFFER_STATE_USER;      
      apBufferInfo->mSize      = lpStream->mBufferSize;
    }// if done queue not empty
    else
    {
      VDB_LOG_NOTE("Done queue empty. Pop aborted.\n");
      lRet = SEQ_DRV_FAILURE;
    } // else from if done queue not empty
  } // if buffer exists
  else
  {
    VDB_LOG_ERROR("No such stream\n");
    lRet = SEQ_DRV_FAILURE;
  } // if no such buffer
  
  return lRet;
} // SEQ_DRV_FrmBufferPush(SEQ_FrmBufferInfo_t *apBuffInfo)

/****************************************************************************/
/** Copies PRAM auxiliary data to given structure.
* 
* \param apAuxData address of the AuxData package
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
* @implements SEQ_DRV_PramAuxDataGet_Activity
*
*****************************************************************************/
void SEQ_DRV_PramAuxDataGet(SEQ_AuxData_t* apAuxData)
{
  if(apAuxData != NULL)
  {
    apAuxData->mSize  = ((uint32_t)SEQ_PRAM_AUX_DATA_SIZE < apAuxData->mSize)?
                        (uint32_t)SEQ_PRAM_AUX_DATA_SIZE: apAuxData->mSize;
    apAuxData->mpData = &spSeqDrvInfo->mpPram[PRAM_AUX_DATA_OFF];
  }
  else
  {
    VDB_LOG_WARNING("AuxData structure pointer is NULL. No action taken.\n");
  }
} // void SEQ_DRV_PramAuxDataGet(SEQ_AuxData_t* apAuxData);

/****************************************************************************/
/** 
* @brief Copies provided register list to PRAM.
* @details Copies provided register list to PRAM.
* 
* @param  apRegList register list to copy
* @return SEQ_DRV_SUCCESS if all ok
*         SEQ_DRV_FAILURE otherwise
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
* @implements SEQ_DRV_RegListSet_Activity
*****************************************************************************/
int32_t SEQ_DRV_RegListSet(SEQ_RegList_t* apRegList)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  if(apRegList != NULL)
  {
    if(apRegList->mpData != NULL) 
    {
      SEQM_ipu_reg_t *lpDest   = NULL;
      uint32_t        lExpCnt  = (uint32_t)0;
      uint32_t       *lpRegCnt = NULL;
 
      if(apRegList->mDirection == IPU_REGLIST_WRITE)
      {
        lpDest   = (SEQM_ipu_reg_t*)&spSeqDrvInfo->mpPram[PRAM_REGLISTW_OFF];     
        lExpCnt  = SEQ_REG_WRITE_NUM;
        lpRegCnt = &(spSeqDrvInfo->mpCommInfo->mWriteRegNum);
      } // if IPU_REGLIST_WRITE
      else
      {
        lpDest   = (SEQM_ipu_reg_t*)&spSeqDrvInfo->mpPram[PRAM_REGLISTR_OFF];
        lExpCnt  = SEQ_REG_READ_NUM;
        lpRegCnt = &(spSeqDrvInfo->mpCommInfo->mReadRegNum);
      } // else from if IPU_REGLIST_WRITE
      
      if(*lpRegCnt == (uint32_t)0) // check if reglist available
      {      
        if(apRegList->mCnt <= lExpCnt)
        {
          if (copy_from_user((uint8_t *)lpDest, 
                            (uint8_t *)apRegList->mpData, 
                            sizeof(SEQM_ipu_reg_t) * apRegList->mCnt) != 
                            (int32_t)0)
          {
            VDB_LOG_ERROR("Failed to copy register list to PRAM.\n");
            lRet = SEQ_DRV_FAILURE;
          } // if graph data fetch failed

          *lpRegCnt = apRegList->mCnt; // count access confirms setup
        } // if cnt is in range
        else
        {
          lRet = SEQ_DRV_FAILURE;
          VDB_LOG_ERROR("Number of register accesses (%u) out of range (%u).\n",
                        apRegList->mCnt,
                        lExpCnt
                        );
        } // else from if cnt is in range
      }// if reglist available
      else
      {
        // reset number of entries to inform higher SW nothing was written
        apRegList->mCnt = (uint32_t)0; // this failure is not critical
      }// else from if reglist available
    } // if data NULL
    else
    {
      lRet = SEQ_DRV_FAILURE;
      VDB_LOG_ERROR("Register list data is NULL. No action taken.\n");
    } // else from if data NULL
  } // if reglist not NULL
  else
  {
    lRet = SEQ_DRV_FAILURE;
    VDB_LOG_ERROR("Register list structure pointer is NULL. No action taken.\n");
  } // else from if reglist not null
  
  return lRet;
} // void SEQ_DRV_RegListSet();

/****************************************************************************/
/** 
* @brief Copies data from PRAM to provided register list.
* @details Data copied only if there is something to be read.
* 
* @param  apRegList register list to copy
* @return SEQ_DRV_SUCCESS if all ok
*         SEQ_DRV_FAILURE otherwise
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
* @implements SEQ_DRV_RegListGet_Activity
*****************************************************************************/
int32_t SEQ_DRV_RegListGet(SEQ_RegList_t* apRegList)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  if(apRegList != NULL)
  {
    if(apRegList->mpData != NULL) 
    {
      SEQM_ipu_reg_t *lpSrc    = NULL;
      uint32_t        lExpCnt  = (uint32_t)0;
      uint32_t       *lpRegCnt = NULL;
 
      if(apRegList->mDirection == IPU_REGLIST_WRITE)
      {
        lpSrc    = (SEQM_ipu_reg_t*)&spSeqDrvInfo->mpPram[PRAM_REGLISTW_OFF];     
        lExpCnt  = SEQ_REG_WRITE_NUM;
        lpRegCnt = &(spSeqDrvInfo->mpCommInfo->mWriteRegNum);
      } // if IPU_REGLIST_WRITE
      else
      {
        lpSrc    = (SEQM_ipu_reg_t*)&spSeqDrvInfo->mpPram[PRAM_REGLISTR_OFF];
        lExpCnt  = SEQ_REG_READ_NUM;
        lpRegCnt = &(spSeqDrvInfo->mpCommInfo->mReadRegNum);
      } // else from if IPU_REGLIST_WRITE
           
      if(apRegList->mCnt <= lExpCnt)
      {
        if (copy_to_user((uint8_t *)apRegList->mpData,
                          (uint8_t *)lpSrc,  
                          sizeof(SEQM_ipu_reg_t) * apRegList->mCnt) != 
                          (int32_t)0)
        {
          VDB_LOG_ERROR("Failed to copy register list from PRAM.\n");
          lRet = SEQ_DRV_FAILURE;
        } // if graph data fetch failed
      } // if cnt is in range
      else
      {
        lRet = SEQ_DRV_FAILURE;
        VDB_LOG_ERROR("Number of register accesses (%u) out of range (%u).\n",
                      apRegList->mCnt,
                      lExpCnt
                      );
      } // else from if cnt is in range
      
      if(*lpRegCnt == (uint32_t)0)
      {
        apRegList->mCnt = (uint32_t)0;
        VDB_LOG_WARNING("Not used by M0 yet.\n");
      }// if reg list not use by M0 yet      
    } // if data NULL
    else
    {
      lRet = SEQ_DRV_FAILURE;
      VDB_LOG_ERROR("Register list data is NULL. No action taken.\n");
    } // else from if data NULL
  } // if reglist not NULL
  else
  {
    lRet = SEQ_DRV_FAILURE;
    VDB_LOG_ERROR("Register list structure pointer is NULL. No action taken.\n");
  } // else from if reglist not null
  
  return lRet;
} // void SEQ_DRV_RegListSet();

//*****************************************************************************

void SEQ_UserPidSet(pid_t aUsrPid)
{
  sSeqUserPid = aUsrPid;
} // SEQ_UserPidSet(pid_t aUsrPid)

/****************************************************************************/
/** 
* @brief Request IPU profiling to be executed for next N frames.
* @details Request IPU profiling to be executed for next N frames.
* 
* @param aFrmCnt number of frames to let the profiling to be on
* 
* @return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE otherwise
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
* @implements SEQ_IpuPerformanceInfoRequest_Activity
*****************************************************************************/
int32_t SEQ_DRV_IpuPerformanceInfoRequest(uint32_t aFrmCnt)
{
  SEQ_Message_t   lMsg;
  SEQM_profile_t  *lpProfileMsg = (SEQM_profile_t*)lMsg.mData;
  
  lpProfileMsg->mFrmCnt = aFrmCnt;
  
  lMsg.mType   = SEQ_MSG_TYPE_IPU_PROFILE;
  lMsg.mStatus = SEQ_MSG_STAT_READY;
  
  // TODO: lock
  sIpuProfileDone = 0; // forget last profiling if any
  // TODO: unlock  
  
  return (SEQ_DRV_MsgSend(&lMsg, 0));
} // SEQ_DRV_IpuPerformanceInfoRequest(uint32_t aFrmCnt)

/****************************************************************************/
/** 
* @brief Fetches IPU performance info from registers.
* @details Fetches IPU performance info from registers.
* 
* 
* @param apIpuRegs pointer to array of IPU performance register structures
* 
* @return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE if no data ready yet or apIpuRegs was NULL
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
* @implements SEQ_DRV_IpuPerformanceInfoGet_Activity
*****************************************************************************/
int32_t SEQ_DRV_IpuPerformanceInfoGet(SEQ_IpuPerformanceRegs_t* apIpuRegs)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  if((apIpuRegs == NULL) || (sIpuProfileDone == 0))
  {
    lRet = SEQ_DRV_FAILURE;
  } // if output pointer not valid or no data ready yet
  else
  {
    uint32_t i = 0;
    SEQ_IpusRegs_t **lppIpusRegs = spSeqDrvInfo->mppIpusRegs; 
    SEQ_IpuvRegs_t **lppIpuvRegs = spSeqDrvInfo->mppIpuvRegs;
  
    // get IPUS regs
    for(i = 0; i < IPUS_CNT; i++)
    {
      apIpuRegs[i].mStallStreamIn    = lppIpusRegs[i]->STALL_STREAMIN.R;
      apIpuRegs[i].mStallStreamOut   = lppIpusRegs[i]->STALL_STREAMOUT.R;
      apIpuRegs[i].mStallStreamInOut = lppIpusRegs[i]->STALL_STREAMINOUT.R;
      apIpuRegs[i].mStallDataHazard  = lppIpusRegs[i]->STALL_DATAHAZARD.R;
      apIpuRegs[i].mExecCounter      = lppIpusRegs[i]->EXEC_COUNTER.R;
    } // for all IPUS
    
    // get IPUV regs
    for(i = IPUS_CNT; i < (IPUS_CNT + IPUV_CNT); i++)
    {
      apIpuRegs[i].mStallStreamIn     = 
        lppIpuvRegs[i - IPUS_CNT]->STALL_STREAMIN.R;
      apIpuRegs[i].mStallStreamOut    = 
        lppIpuvRegs[i - IPUS_CNT]->STALL_STREAMOUT.R;
      apIpuRegs[i].mStallStreamInOut  = 
        lppIpuvRegs[i - IPUS_CNT]->STALL_STREAMINOUT.R;
      apIpuRegs[i].mStallDataHazard   = 
        lppIpuvRegs[i - IPUS_CNT]->STALL_DATAHAZARD.R;
      apIpuRegs[i].mExecCounter       = 
        lppIpuvRegs[i - IPUS_CNT]->EXEC_COUNTER.R;
    } // for all IPUv
  } // else from if output pointer not valid
  
  return lRet;
} // SEQ_UserPidSet(pid_t aUsrPid)

//*****************************************************************************

void SEQ_DRV_IpuHistRead(SEQ_IpuHist_t *apHist)
{
  uint32_t i = 0;
  if(apHist != NULL)
  {
    if(apHist->mIpuIdx < IPU_HIST_CNT)
    {
      uint32_t *lpHist = apHist->mpData;
      uint32_t lIpuIdx = apHist->mIpuIdx;
      SEQ_IpusRegs_t **lppIpusRegs = spSeqDrvInfo->mppIpusRegs; 
      
      lppIpusRegs[lIpuIdx]->HOST_HISTA.B.HISTA = 0;         
      for(i = 0; i < IPU_HIST_SIZE; i++)
      {
        lpHist[i] = lppIpusRegs[lIpuIdx]->HOST_HISTD.B.HISTD;
      } // for all bins
      
      apHist->mPeak     = lppIpusRegs[lIpuIdx]->HOST_HISTPEAK.B.HISTPEAK;
      apHist->mPeakAddr = lppIpusRegs[lIpuIdx]->HOST_HISTAMAX.B.HISTAMAX;
      
      lppIpusRegs[lIpuIdx]->HOST_HISTCLEAR.B.HISTCLEAR = 1;
    } // if valid index
  } // if histogram not null
} // void SeqHistGet()

/****************************************************************************/
/** 
* @brief Enables the use of histogram on particular IPU.
* @details Only IPU 0-3 are capable of generating histogram.
* 
* @param aIpuIdx index of the IPU
* 
* @return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE if no data ready yet or apIpuRegs was NULL
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
* @implements SEQ_DRV_IpuHistogramEnable_Activity
*****************************************************************************/
int32_t SEQ_DRV_IpuHistogramEnable(SEQ_IpuEngine_t aIpuIdx)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  if(aIpuIdx < IPU_HIST_CNT)
  {
    if(spIpuHists[aIpuIdx] == NULL)
    {
      SEQ_IpuHist_t *lpHist = kmalloc(sizeof(SEQ_IpuHist_t), GFP_KERNEL);
      if(lpHist == NULL)
      {
        VDB_LOG_ERROR("Failed to allocate histogram memory.\n");
        lRet = SEQ_DRV_FAILURE;
      } // if alloc failed
      else
      {
        memset(lpHist, 0, sizeof(SEQ_IpuHist_t));
        lpHist->mIpuIdx     = aIpuIdx;
        spIpuHists[aIpuIdx] = lpHist;
      } // else from if alloc failed
    } // if hist not enabled yet
  } // if IPU index in range
  else
  {
    VDB_LOG_ERROR("Wrong Ipu index.\n");
    lRet = SEQ_DRV_FAILURE;
  } // else from if IPU index in range
  return lRet;
} // void SEQ_DRV_IpuHistogramEnable(uint8_t aIpuIdx)

/****************************************************************************/
/** 
* @brief Disables the use of histogram on particular IPU.
* @details Only IPU 0-3 are capable of generating histogram.
* 
* @param aIpuIdx index of the IPU
* 
* @return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE if no data ready yet or apIpuRegs was NULL
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
* @implements SEQ_DRV_IpuHistogramDisable_Activity
*****************************************************************************/
void SEQ_DRV_IpuHistogramDisable(SEQ_IpuEngine_t aIpuIdx)
{
  if(aIpuIdx < IPU_HIST_CNT)
  {
    SEQ_IpuHist_t *lpHist = spIpuHists[aIpuIdx];
    spIpuHists[aIpuIdx] = NULL;
    if(lpHist != NULL)
    {
      kfree(lpHist);
    } // if hist not enabled yet
  } // if IPU index in range
} // void SEQ_DRV_IpuHistogramDisable(uint8_t aIpuIdx)

/****************************************************************************/
/** 
* @brief Gets the histogram from specified IPU engine.
* @details Only IPU 0-3 are capable of generating histogram.
* 
* @param apHist pointer to histogram head structure
* 
* @return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE if no data ready yet or apIpuRegs was NULL
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*  
* @implements SEQ_DRV_IpuHistogramGet_Activity
*****************************************************************************/
int32_t SEQ_DRV_IpuHistogramGet(SEQ_IpuHistHead_t *apHist)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  if((apHist != NULL) && (apHist->mpData != NULL))
  {
    if(apHist->mIpuIdx < IPU_HIST_CNT)
    {
      if(spIpuHists[apHist->mIpuIdx] != NULL)
      {
        do
        {
          apHist->mFrmSeqNum = spIpuHists[apHist->mIpuIdx]->mFrmSeqNum;
          if (copy_to_user((uint8_t *)apHist->mpData, 
                            spIpuHists[apHist->mIpuIdx]->mpData, 
                            sizeof(uint32_t) * IPU_HIST_SIZE) != (int32_t)0)
          {
            VDB_LOG_ERROR("Histogram data copy failed.\n");
            lRet = SEQ_DRV_FAILURE;
          } // if graph data fetch failed
          apHist->mPeak      = spIpuHists[apHist->mIpuIdx]->mPeak;
          apHist->mPeakAddr  = spIpuHists[apHist->mIpuIdx]->mPeakAddr;
          
          // make sure no update in between
        }while(apHist->mFrmSeqNum != spIpuHists[apHist->mIpuIdx]->mFrmSeqNum);
      }
      else
      {
        lRet = SEQ_DRV_FAILURE;
      }
    } // if valid index
    else
    {
      VDB_LOG_ERROR("Wrong Ipu index.\n");
      lRet = SEQ_DRV_FAILURE;
    }
  }
  else
  {
    VDB_LOG_ERROR("Histogram pointer is NULL.\n");
    lRet = SEQ_DRV_FAILURE;
  }
  
  return lRet;
} // void SEQ_DRV_IpuHistogramGet(uint8_t aIpuIdx)

//*****************************************************************************

void SEQ_DRV_IpuStatRead(SEQ_IpuStat_t *apStat)
{
  uint32_t i = 0;
  if(apStat != NULL)
  {
    uint32_t *lpStat = apStat->mpData;
    SEQ_IpusRegs_t **lppIpusRegs = spSeqDrvInfo->mppIpusRegs; 
    
    lppIpusRegs[7]->HOST_STATA.B.STATA = 0;         
    
    for(i = 0; i < IPU_STAT_SIZE; i++)
    {
      lpStat[i] = lppIpusRegs[7]->HOST_STATD.B.STATD; 
    } // for all bins
    
    lppIpusRegs[7]->HOST_STATCLEAR.B.STATCLEAR = 1;
  } // if input structure not NULL
} // void SEQ_DRV_IpuStatRead()

/****************************************************************************/
/** 
* @brief Enables the use of statistic data from IPUS_7.
* @details Enables the use of statistic data from IPUS_7.
* 
* @return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE if no data ready yet
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
* @implements SEQ_DRV_IpuStatEnable_Activity
*****************************************************************************/
int32_t SEQ_DRV_IpuStatEnable()
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  
  if(spIpuStat == NULL)
  {
    SEQ_IpuStat_t *lpStat = kmalloc(sizeof(SEQ_IpuStat_t), GFP_KERNEL);
    if(lpStat == NULL)
    {
      VDB_LOG_ERROR("Failed to allocate statistics memory.\n");
      lRet = SEQ_DRV_FAILURE;
    } // if alloc failed
    else
    {
      memset(lpStat, 0, sizeof(SEQ_IpuStat_t));
      spIpuStat = lpStat;
    } // else from if alloc failed
  } // if not enabled yet
 
  return lRet;
} // void SEQ_DRV_IpuStatEnable(uint8_t aIpuIdx)

/****************************************************************************/
/** 
* @brief Disables the use of statistic data from IPUS_7.
* @details Disables the use of statistic data from IPUS_7.
* 
* @return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE if no data ready yet
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
* @implements SEQ_DRV_IpuStatDisable_Activity
*****************************************************************************/
void SEQ_DRV_IpuStatDisable()
{
  SEQ_IpuStat_t *lpStat = spIpuStat;
  spIpuStat             = NULL;
  if(lpStat != NULL)
  {
    kfree(lpStat);
  } // if hist not enabled yet
} // void SEQ_DRV_IpuStatDisable(uint8_t aIpuIdx)

/****************************************************************************/
/** 
* @brief Gets the statistic data from IPUS_7.
* @details Gets the statistic data from IPUS_7.
* 
* @param apStat pointer to statistic data array
* 
* @return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE if no data ready yet or apStat was NULL
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
* @implements SEQ_DRV_IpuStatGet_Activity
*****************************************************************************/
int32_t SEQ_DRV_IpuStatGet(SEQ_IpuStatHead_t *apStat)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  if((apStat != NULL) && (apStat->mpData != NULL))
  {
    if(spIpuStat != NULL)
    {
      do
      {
        apStat->mFrmSeqNum = spIpuStat->mFrmSeqNum;
        if (copy_to_user((uint8_t *)apStat->mpData, 
                          spIpuStat->mpData, 
                          sizeof(uint32_t) * IPU_STAT_SIZE) != (int32_t)0)
        {
          VDB_LOG_ERROR("Statistics data copy failed.\n");
          lRet = SEQ_DRV_FAILURE;
        } // if graph data fetch failed
        // make sure no update in between
      }while(apStat->mFrmSeqNum != spIpuStat->mFrmSeqNum);
    } // if histogram enabled
    else
    {
      lRet = SEQ_DRV_FAILURE;
    } // else from if histogram enabled
  } // if Stat exists
  else
  {
    VDB_LOG_ERROR("Statistic pointer is NULL.\n");
    lRet = SEQ_DRV_FAILURE;
  }
  
  return lRet;
} // void SeqStatGet()

//*****************************************************************************

#ifndef __STANDALONE__
EXPORT_SYMBOL(gSEQ_DRV_FdmaComm);
#endif // #ifndef __STANDALONE__
