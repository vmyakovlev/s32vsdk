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
* @file     fdma_types.h
* @brief    types declarations for fDMA driver
****************************************************************************/

/*==============================================================================
Revision History:
                        Modification      Tracking
Author (core ID)        Date D/M/Y         Number     Description of Changes
Tomas Babinec           09-Jun-2014                    Inital version.
Tomas Babinec           12-May-2017       vsdk-575     Traceability.
Tomas Babinec           25-May-2017       vsdk-575     Misra.
==============================================================================*/

#ifndef FDMATYPES_H
#define FDMATYPES_H

#ifdef __STANDALONE__
  #include <unistd.h>
#endif // ifdef __STANDALONE__
  
#if !defined(__KERNEL__) && !defined(__STANDALONE__)
#include <stdint.h>
#include <sys/types.h>
#endif // if !defined(__KERNEL__) && !defined(__STANDALONE__)

#ifdef __cplusplus 
extern "C" { 
#endif

//*****************************************************************************
// constants
//*****************************************************************************
  
#define FDMA_IRQ_DONE    ((uint32_t)115U)
#define FDMA_IRQ_ERR     ((uint32_t)116U)

#define FDMA_EVENT_SIGNAL   ((uint32_t)56U)
#define FDMA_DONE_EVENT     ((uint32_t)1U)
#define FDMA_ERR_EVENT      ((uint32_t)2U)

#define FDMA_DRV_SUCCESS  ((int32_t)0)
#define FDMA_DRV_FAILURE  ((int32_t)-1)

#define FDMA_DRV_ERR_QUEUE_FULL ((int32_t)-2)

#define FDMA_MEM_BUF_LEN ((uint32_t)256U)

// name of the device file
#define FDMA_DEVICE_NAME "fdma"

// magic number for fDMA driver
#define FDMA_IOC_MAGIC 'f'

///< number of transaction descriptors in SRAM table (TDT)
#define FDMA_TD_CNT    ((uint32_t)16U)

///< possible Sequencer mode states
#define SEQ_BASED_MODE ((uint32_t)1U)
#define SEQ_LESS_MODE  ((uint32_t)0U)
  
#define FDMA_TDT_CNT   ((uint32_t)3U) // number of TDTs (1x user, 2x ISP)
  
// register related
#define FDMA_SOFT_RST_BIT         ((uint32_t)0x1U)
#define FDMA_DDR_NEXT_LINE_MASK   ((uint32_t)0xffffU)
#define FDMA_SRAM_NEXT_LINE_MASK  ((uint32_t)0x3fff0000U)
#define FDMA_SRAM_NEXT_LINE_SHIFT ((uint32_t)0x10U)
  
// IRQ related  
#define FDMA_IRQ_ERR_MASK_ALL  ((uint32_t)0x7FC)    ///< all FDMA error flags

#define FDMA_IRQ_DONE_EN_MASK    ((uint32_t)0x1U)    ///< frame done IRQ
#define FDMA_IRQ_ERR_EN_MASK     ((uint32_t)0x2U)    ///< frame err IRQ
#define FDMA_IRQ_CRC_ERR_EN_MASK ((uint32_t)0x4U)    ///< frame crc err IRQ
  
#define FDMA_IRQ_EN_MASK_ALL (FDMA_IRQ_DONE_EN_MASK|FDMA_IRQ_ERR_EN_MASK|\
                              FDMA_IRQ_CRC_ERR_EN_MASK)

//*****************************************************************************
// types
//*****************************************************************************
typedef volatile int8_t vint8_t;
typedef volatile uint8_t vuint8_t;
typedef volatile int16_t vint16_t;
typedef volatile uint16_t vuint16_t;
typedef volatile int32_t vint32_t;
typedef volatile uint32_t vuint32_t;

typedef struct mutex mutex_t;

#ifdef __STANDALONE__
  typedef uint32_t irqreturn_t;
#endif // ifdef __STANDALONE__
  
typedef struct FDMA_Regs {
  vuint32_t XFR_REC_LIST_PTR; /* Transfer Records List Pointer register */
  vuint32_t XFR_REC_CNT;    /* Total entries in Transfer Record List */
  vuint32_t XFR_REC_NUM;    /* Transfer Record number for current Line transfer */
  vuint32_t XFR_LINE_NUM;  /* DDR and SRAM Line numbers for current transfer */
  vuint32_t LINE_INCR;        /* Line increment value for SRAM and DDR */
  vuint32_t IRQ_EN;              /* Interrupt enable register */
  vuint32_t XFR_STAT;          /* Status register */
  vuint32_t CALC_CRC_VAL;  /* Calculated CRC value */
  vuint32_t CURR_DDR_PTR;  /* Current DDR address */
  vuint32_t CURR_SRAM_PTR; /* Current SRAM address */
  vuint32_t XFR_REC_NUM_DONE; /* Last completed Transfer Record Number */
  vuint32_t ERR_XFR_REC_NUM; /* Transfer Record Number of an Erroneous Transfer */
  vuint32_t NEXT_LINE;        /* SRAM and DDR next Line number */
  vuint32_t CTRL;                  /* Control register */
} FDMA_Regs_t;

//*****************************************************************************

//typedef volatile struct FDMA_tag FDMA_Regs_t;

//*****************************************************************************

enum FDMA_IRQ_TYPE
{
  FDMA_IRQ_TYPE_CRC_ERR      = 0x00004,
  FDMA_IRQ_TYPE_CMD_CFG_ERR  = 0x00008,
  FDMA_IRQ_TYPE_TR_RD_ERR    = 0x00010,
  FDMA_IRQ_TYPE_CRC_RD_ERR   = 0x00020,
  FDMA_IRQ_TYPE_XFR_CFG_ERR  = 0x00040,
  FDMA_IRQ_TYPE_XFR_ERR_DDR  = 0x00080,
  FDMA_IRQ_TYPE_XFR_ERR_SRAM = 0x00100,
  FDMA_IRQ_TYPE_CRC_WR_ERR   = 0x00200,
  FDMA_IRQ_TYPE_TR_WBACK_ERR = 0x00400,
  FDMA_IRQ_TYPE_XFR_DONE     = 0x10000
}; // FDMA IRQ TYPE definition  

/*****************************************************************************
* struct definitions
*****************************************************************************/
#if defined(__KERNEL__) || defined(__STANDALONE__)
///< preliminary declarations
struct fdma_td;
struct fdma_tc;
struct fdma_tdm;
struct SEQ_FdmaComm;

/****************************************************************************/
/** Transfer Descriptor Table structure.
*
*****************************************************************************/
typedef struct fdma_tdt
{
  struct fdma_td *mpBase;
  uint32_t mTdCnt;
} FDMA_Tdt_t; // struct fdma_tdt (transaction descriptor table)

/****************************************************************************/
/** Node of TDM list.
*
*****************************************************************************/
typedef struct fdma_tdm_list_node
{
  struct fdma_tdm_list_node *mpPrev;
  struct fdma_tdm_list_node *mpNext;
  struct fdma_tdm *mpTdm;
} FDMA_TdmListNode_t; // struct fdma_tdm_list_node (one node from list of fdma tdms)

/****************************************************************************/
/** DL linked list of TDM structures.
*
* Used to track TD allocations.
*****************************************************************************/
typedef struct fdma_tdm_list
{
  struct fdma_tdm_list_node *mpHead;
  struct fdma_tdm_list_node *mpTail;
  uint32_t mLength;
  struct fdma_tdm_list_node mNodePool[FDMA_TD_CNT];
} FDMA_TdmList_t; // struct fdma_tdm_list (double-linked list of transaction descriptor metadata)

#endif // #if defined(__KERNEL__) || defined(__STANDALONE__)

/****************************************************************************/
/** FDMA transfer channel event.
*
* Captures info related to single FDMA TC event.
*****************************************************************************/
typedef struct fdma_tc_event
{
  uint32_t mXfrStat;
  uint32_t mXfrCalcCrcVal;
  uint32_t mXfrCurrDdrPtr;  
  uint32_t mXfrCurrSramPtr;  
  uint32_t mXfrRecNum;  
  uint32_t mSramNextLine;  
} FDMA_TcEvent_t; // struct fdma_tc_event 

/****************************************************************************/
/** 
* @brief FDMA transfer descriptor structure.*
* @details Represents one TD in SRAM TDT.
* @pre
* @post
* 
* @implements     FDMA_Td_t_struct
*****************************************************************************/
typedef struct fdma_td
{
  uint32_t mDdrImgStartAddr : 32;	// 31:0
  uint32_t mDdrLineStride : 16;		// 47:32
  uint32_t mDdrLineCnt : 16;  		// 63:48
  uint32_t mCrcStartAddr :32;  		// 95:64
  uint32_t mCrcEna : 1;			// 96
  uint32_t mCrcMode : 2;		// 98:97
  uint32_t mCrcPoly : 1;		// 99
  uint32_t mDir : 1;			// 100
  uint32_t mWrEna : 1;			// 101
  uint32_t mTransRecWrBack : 1;		// 102
  uint32_t mThreadId : 3;		// 105:103
  uint32_t mImgDoneSel : 1;  		// 106
  uint32_t mLineSize : 21;		// 127:107
  uint32_t mSramImgStartAddr : 32;	// 159:128
  uint32_t mSramLineStride : 18;	// 177:160
  uint32_t mSramLineCnt : 14;		// 191:178
  uint32_t mDdrBurstLength : 2;		// 193:192
  uint32_t mStuffBits : 30;		// 223:194
  uint32_t mDdrCurrLine : 16;		// 239:224
  uint32_t mSramCurrLine : 16;		// 255:240
} FDMA_Td_t; // struct fdma_td (transaction descriptor)

/****************************************************************************/
/** 
* @brief FDMA transfer descriptor metadata structure.* 
* @details Caries aditional data for FDMA TDs.
* @pre
* @post
* 
* @implements     FDMA_Tdm_t_struct
*****************************************************************************/

typedef struct fdma_tdm
{
  // application related
  uint8_t  mTdIdx;
  int32_t  mOwnerProcId;
  uint8_t  mReserved;
  
  // graph/fdma related
  uint16_t mSramLpF;      ///< lines/frame for the SRAM buffer
  uint16_t mSramLineOffs; ///< line offset in SRAM buffer  
} FDMA_Tdm_t; // struct fdma_tdm (transaction descriptor metatdata)

/****************************************************************************/
/** 
* @brief FDMA transfer channel structure.
* @details Combines TD and its metadata into one structure. Represents one FDMA 
* channel that can be allocated and used by higher level SW.
* @pre
* @post
* 
* @implements     FDMA_Tc_t_struct
*****************************************************************************/
typedef struct fdma_tc
{
  struct fdma_tdm mTdm;
  struct fdma_td mTd;
} FDMA_Tc_t; // struct fdma_tc (transfer channel)

#if defined(__KERNEL__) || defined(__STANDALONE__)
/****************************************************************************/
/** FDMA driver info.
*
*****************************************************************************/

typedef struct FDMA_DrvInfo
{
  FDMA_Regs_t    *mpRegs;       ///< control block registers
  uint8_t         mSequencerOn; ///< Sequencer driver enable => 1, disabled => 2  
  int32_t       (*mpSEQ_FdmaTdSchedule)(uint32_t); ///< SEQ fdma schedule func
  void          (*mpSigInvoke)(int32_t, int32_t);  ///< invokes event signaling
  uint32_t        mIrqDone;
  uint32_t        mIrqErr;
  
  void*           mpTdtBasePhyHndl;
  uint32_t        mTdtBasePhy;
  FDMA_TcEvent_t  mpTcEventArr[FDMA_TD_CNT];
  FDMA_Tdm_t      mpTdmArr[FDMA_TD_CNT];
  FDMA_TdmList_t  mTdmFreeList;
  FDMA_Tdt_t      mpTdt[FDMA_TDT_CNT];
  struct SEQ_FdmaComm *mpSeqComm;
} FDMA_DrvInfo_t; 
#endif // #if defined(__KERNEL__) || defined(__STANDALONE__)

#ifdef __cplusplus 
} //extern c declaration
#endif

//****************************************************************************
#endif /* FDMATYPES_H */
