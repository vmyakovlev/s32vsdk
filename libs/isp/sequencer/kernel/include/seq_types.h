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
/**
* @file     seq_types.h
* @brief    declaration of Sequencer driver internal types
****************************************************************************/

#ifndef SEQTYPES_H
#define SEQTYPES_H

#include "seq.h"
#include "s32vs234.h"
#include "seq_comm.h"
#include "seq_graph_meta.h"
#include "vdlist.h"
#include "../../../fdma/kernel/include/fdma_types.h"

#define SEQ2HOST_MESSAGE_CNT  	3	///< number of messages from Sequencer to Host 
#define HOST2SEQ_MESSAGE_CNT  	1	///< number of messages from Host to Sequencer

#ifdef __cplusplus 
extern "C" { 
#endif

  
#define IPU_HIST_SIZE 256         ///< number of bins in histogram
#define IPU_HIST_CNT  4           ///< number of IPUs capable to do histogram 

#define IPU_STAT_SIZE 1024        ///< number of statistic values (32bit)
  
#define IPU_REGLIST_WRITE 0       ///< list of IPU registers to be written
#define IPU_REGLIST_READ  1       ///< list of IPU registers to be read

//****************************************************************************
// types
//****************************************************************************

///< new name for Sequencer control blcok register structure
typedef volatile struct VSEQ_CTRL_BLK_tag SEQ_CtrlBlkRegs_t;

///< new name for Sequencer event control register structure
typedef volatile struct VSEQ_EVT_CTRL_tag SEQ_EvtCtrlRegs_t;

///< new name for IPUS block register structure
typedef volatile struct IPUS_tag SEQ_IpusRegs_t;

///< new name for IPUV block register structure
typedef volatile struct IPUV_tag SEQ_IpuvRegs_t;

/*** host communication ***/

/****************************************************************************/
/** Defines pool for Sequencer messages.
*
* The messages are distributed from a virtually contiguous memory block which 
* has to be specified during init call.
*****************************************************************************/
typedef struct SEQ_MessagePool
{
  struct SEQ_Message *mpRead;
  struct SEQ_Message *mpWrite;
  struct SEQ_Message *mpBase;
  struct SEQ_Message *mpLast;
} SEQ_MessagePool_t;

// *** for both KERNEL and USER space ***

/****************************************************************************/
/** Defines possible states of Sequencer frame buffer
*
*****************************************************************************/
typedef enum SEQ_FrmBufferState
{
  SEQ_FRM_BUFFER_STATE_WORK = 0,  ///< in the work queue
  SEQ_FRM_BUFFER_STATE_DONE,      ///< in the done queue 
  SEQ_FRM_BUFFER_STATE_USER,      ///< available for user
} SEQ_FrmBufferState_t; // enum SEQ_FrmBufferState

/****************************************************************************/
/** 
* @brief Defines frame buffer info structure.
* @details Defines frame buffer info structure.
* @pre
* @post
* 
* @implements SEQ_FrmBufferInfo_t_struct
*****************************************************************************/
typedef struct SEQ_FrmBufferInfo
{
  uint32_t                mBufferIdx; ///< index of the buffer
  enum SEQ_FrmBufferState mState;     ///< current state of the buffer
  uint32_t                mStreamIdx; ///< index of buffer stream
  uint32_t                mFrmSeqNum; ///< frame sequence number
  uint32_t                mSize;      ///< size of the buffer
} SEQ_FrmBufferInfo_t;

/****************************************************************************/
/** Defines one frame buffer.
*
*****************************************************************************/
typedef struct SEQ_FrmBuffer
{
  uint32_t                mBufferIdx; ///< index of the buffer
  uintptr_t               mOffset;    ///< offset of the buffer/or user pointer
  uint32_t                mPhyAddr;   ///< physical address of the buffer
  enum SEQ_FrmBufferState mState;     ///< current state of the buffer
  uint32_t                mStreamIdx; ///< index of buffer stream
  uint32_t                mFrmSeqNum; ///< frame sequence number
  
  struct VDList_Node      mVDListNode;///< list node
    
} SEQ_FrmBuffer_t;

/****************************************************************************/
/** Defines one frame buffer.
*
*****************************************************************************/
typedef struct SEQ_FrmStream
{
  uint32_t                mCnt;          ///< number of available buffers
  SEQ_FrmBuffer_t*        mpBufferTable; ///< table of buffers
  size_t                  mBufferSize;   ///< size of one buffer in bytes
  
  SEQ_FrmBuffer_t*        mpNextBuffer;  ///< pointer to buffer that is sheduled
  struct VDList           mWorkQueue;    ///< queue of frames to be captured
  struct VDList           mDoneQueue;    ///< queue of done frames
  
  uint16_t                mSramLpF;      ///< lines/frame for the SRAM buffer
  uint16_t                mSramLineOffs; ///< line offset in SRAM buffer
} SEQ_FrmStream_t;

/****************************************************************************/
/** Initializes a frame stream structure members to default values.
* 
* \param apStream pointer to the frame stream structure
*****************************************************************************/

static inline void SEQ_FrmStreamInit(struct SEQ_FrmStream *apStream)
{
  apStream->mCnt          = 0;
  apStream->mpBufferTable = NULL;
  apStream->mBufferSize   = 0;
  VDList_Init(&(apStream->mWorkQueue));
  VDList_Init(&(apStream->mDoneQueue));
} // SEQ_FrmStreamInit()

/****************************************************************************/
/** 
* @brief   Describes set of DDR buffers for paticular ISP stream.
* @details Used to request sequencer frame stream to be initialized with 
*          application preallocated DDR buffers.
* @pre
* @post
* 
* @implements SEQ_BufferRequest_t_struct
*****************************************************************************/
typedef struct SEQ_BufferRequest
{
  uint32_t    mStreamIdx;  ///< index of a frame stream
  uint32_t    mCnt;        ///< number of requested buffers
  uint32_t    mBaseShift;  ///< shift from base address to valid pixel data
  size_t      mBufferSize; ///< size of one buffer in bytes
  
  uint16_t    mSramLpF;      ///< lines/frame for the SRAM buffer
  uint16_t    mSramLineOffs; ///< line offset in SRAM buffer
  
  uint32_t*   mpBuffers;   ///< pointer to array of HW addresses 
} SEQ_BufferRequest_t;

/****************************************************************************/
/** Sequencer firmware type enumeration.
 * 
 * To distinguish between CM0 and IPUx kernel code
 ****************************************************************************/
enum SEQ_FwType
{
  SEQ_FW_CM0,     ///< CM0 firmware code placed in CRAM
  SEQ_FW_KERNEL,   ///< IPUx kernel code placed in KRAM
  SEQ_FW_GRAPH,   ///< IPUx kernel code placed in KRAM
}; //SEQ_FwType

typedef enum SEQ_FwType SEQ_FwType_t;

/****************************************************************************/
/** 
* @brief Structure to keep packed graph data.
* @details Structure to keep packed graph data.
* @pre
* @post
* 
* @implements     SEQ_GraphPackage_t_struct
*****************************************************************************/
typedef struct SEQ_GraphPackage
{
  uint32_t      mGraphSize;       ///< overall size of the packed graph
  void*         mpGraphData;      ///< pointer to packed graph data
  uint32_t      mBufferListSize;  ///< size of the buffer list in bytes
  void*         mpBufferList;     ///< pointer to list of buffers
  uint32_t      mCSINumLines;     ///< # of lines expected from csi per frame
} SEQ_GraphPackage_t;

/****************************************************************************/
/** Defines one preprocessed Sequencer firmware line from S-record file.
*
* Used by SEQ_Fw_t to describe the whole firmware package.
*****************************************************************************/
typedef struct SeqFwLine
{
  uint32_t mAddr;
  uint32_t mSize;
  uint8_t *mpData;
} SEQ_FwLine_t; 

/****************************************************************************/
/** Collects all lines of a Sequencer firmware.
*
*****************************************************************************/
typedef struct SeqFw
{
  uint32_t      mLineCnt;
  SEQ_FwLine_t *mpLines;
} SEQ_Fw_t; 

/****************************************************************************/
/** Structure for sequencer start command parameters.
*
*****************************************************************************/
typedef struct SEQ_StartCfg
{
  uint32_t      mFrames2Grab;    ///< # of frames to grab
  uint32_t      mInputLines;     ///< # of lines expected per input frame
} SEQ_StartCfg_t;

/****************************************************************************/
/** 
* @brief   Sequencer message ausiliary data container.
* @details Sequencer message ausiliary data container.
* @pre
* @post
* 
* @implements SEQ_AuxData_t_struct
*****************************************************************************/
typedef struct SeqAuxData
{
  void *mpData;
  uint32_t mSize;
} SEQ_AuxData_t; // struct SeqAuxData

/****************************************************************************/
/** 
* @brief   Register list data container.
* @details Register list data container.
* @pre
* @post
* 
* @implements SEQ_RegList_t_struct
*****************************************************************************/
typedef struct SeqRegList
{
  uint32_t        mCnt;
  uint8_t         mDirection; // IPU_REGLIST_READ or IPU_REGLIST_WRITE
  SEQM_ipu_reg_t *mpData;
} SEQ_RegList_t;

/****************************************************************************/
/** 
* @brief   Patch list data container.
* @details Patch list data container.
* @pre
* @post
* 
* @implements SEQ_PatchLIstArr_t_struct
*****************************************************************************/
typedef struct SeqPatchListArr
{
  SEQ_patch_list_t *mpPatchListArr;
  uint32_t          mPatchListNum;
} SEQ_PatchLIstArr_t; // struct SeqAuxData

/****************************************************************************/
/** 
* @brief   IPU performance registers container.
* @details IPU performance registers container.
* @pre
* @post
* 
* @implements SEQ_IpuPerformanceRegs_t_struct
*****************************************************************************/
typedef struct SeqIpuPerformanceRegs
{
  uint32_t mStallStreamIn;
  uint32_t mStallStreamOut;
  uint32_t mStallStreamInOut;
  uint32_t mStallDataHazard;
  uint32_t mExecCounter;
} SEQ_IpuPerformanceRegs_t;

/****************************************************************************/
/** Define possible IPU engines.
*
*****************************************************************************/
typedef enum SEQ_IpuEngine
{
  SEQ_IPUS0 = 0,
  SEQ_IPUS1 = 1,
  SEQ_IPUS2 = 2,
  SEQ_IPUS3 = 3,
  SEQ_IPUS4 = 4,
  SEQ_IPUS5 = 5,
  SEQ_IPUS6 = 6,
  SEQ_IPUS7 = 7,
  SEQ_IPUV0 = 8,
  SEQ_IPUV1 = 9,
  SEQ_IPUV2 = 10,
  SEQ_IPUV3 = 11,
} SEQ_IpuEngine_t; 

/****************************************************************************/
/** Seqencer Event description.
*
*****************************************************************************/
typedef union SEQ_EventDesc
{
  uint32_t R;
  struct 
  {
    uint32_t mRegister:26;
    uint32_t mType:6;
  } B;
} SEQ_EventDesc_t;

/****************************************************************************/
/** Seqencer Event time record.
*
*****************************************************************************/
typedef struct SEQ_EventRecord
{
  SEQ_EventDesc_t mEvent; ///< event description
  uint64_t        mTime;  ///< time the event ocured
} SEQ_EventRecord_t; 

/****************************************************************************/
/** Series of Seqencer Event records.
*
*****************************************************************************/
typedef struct SEQ_EventLog
{
  SEQ_EventRecord_t *mpRecords;   ///< event record array pointer
  uint32_t           mNextRecIdx; ///< index of the next event record
} SEQ_EventLog_t; 

/****************************************************************************/
/** Ipu histogram container for userspace app.
*
*****************************************************************************/
typedef struct SEQ_IpuHist
{
  uint32_t        mPeak;      ///< peak value of the histogram
  uint32_t        mPeakAddr;  ///< bin index of the peak value
  uint32_t        mFrmSeqNum; ///< frame sequence number
  SEQ_IpuEngine_t mIpuIdx;    ///< IPU engines 
  uint32_t        mpData[IPU_HIST_SIZE];  ///< pointer to histogram data
} SEQ_IpuHist_t; 


/****************************************************************************/
/**
* @brief   IPU histogram container.
* @details IPU histogram container.
* @pre
* @post
* 
* @implements SEQ_IpuHistHead_t_struct
*****************************************************************************/
typedef struct SEQ_IpuHistHead
{
  uint32_t        mPeak;      ///< peak value of the histogram
  uint32_t        mPeakAddr;  ///< bin index of the peak value
  uint32_t        mFrmSeqNum; ///< frame sequence number
  SEQ_IpuEngine_t mIpuIdx;    ///< IPU engines  
  uint32_t       *mpData;     ///< pointer to histogram data
} SEQ_IpuHistHead_t; 

/****************************************************************************/
/** Ipu statisic data container for userspace.
*
*****************************************************************************/
typedef struct SEQ_IpuStat
{
  uint32_t        mFrmSeqNum; ///< frame sequence number
  uint32_t        mpData[IPU_STAT_SIZE];  ///< pointer to statistic data
} SEQ_IpuStat_t; 

/****************************************************************************/
/**
* @brief   IPU statisic data container.
* @details IPU statisic data container.
* @pre
* @post
* 
* @implements SEQ_IpuStatHead_t_struct
*****************************************************************************/
typedef struct SEQ_IpuStatHead
{
  uint32_t        mFrmSeqNum; ///< frame sequence number
  uint32_t       *mpData;  ///< pointer to statistic data
} SEQ_IpuStatHead_t; 

/****************************************************************************/
/** 
* @brief Seq driver frame temporal statistics.
* @details Seq driver frame temporal statistics.
* @pre
* @post
* 
* @implements SEQ_FrmTemporalStat_t_struct
*****************************************************************************/
typedef struct SEQ_FrmTemporalStat
{
  uint64_t mFrmDoneCnt;
  uint64_t mStartTime;    ///< in Linux jiffies in SA us
  uint64_t mLastDoneTime; ///< in Linux jiffies in SA us
} SEQ_FrmTemporalStat_t;

#if defined(__KERNEL__) || defined(__STANDALONE__)
/****************************************************************************/
/** Sequencer IRQ numbers.
*
*****************************************************************************/

typedef struct SEQ_IrqNums
{
  uint32_t mMsgRdy;
  uint32_t mMsgAck;  
} SEQ_IrqNums_t; 

/****************************************************************************/
/** Sequencer driver info.
*
*****************************************************************************/

typedef struct SEQ_DrvInfo
{
  SEQ_CtrlBlkRegs_t *mpSeqCBregs;    ///< control block registers
  SEQ_EvtCtrlRegs_t *mpSeqECregs;    ///< event control block registers
  uint8_t           *mpCram;         ///< M0+ code RAM
  uint8_t           *mpDbgPram;      ///< M0+ debug parameter RAM
  uint8_t           *mpPram;         ///< M0+ parameter RAM
  uint8_t           *mpKram;         ///< M0+ kernel RAM
  SEQ_IpusRegs_t    *mppIpusRegs[8]; ///< IPUs registers
  SEQ_IpuvRegs_t    *mppIpuvRegs[4]; ///< IPUv registers
  SEQ_IrqNums_t      mIrqNums;       ///< irqnumbers
  SEQ_pram_map_t    *mpCommInfo;     ///< pram map
  
  void          (*mpSigInvoke)(int32_t, int32_t);  ///< invokes event signaling
} SEQ_DrvInfo_t; 

typedef int32_t(*FDMA_SeqEventHandler_t)(const FDMA_TcEvent_t*);
typedef int32_t(*FDMA_TcSchedule_t)(const FDMA_Tc_t*);
typedef int32_t(*SEQ_FdmaTdSchedule_t)(const FDMA_Tc_t*);

/****************************************************************************/
/**
* @brief Communication I/F to FDMA driver.
* @details Communication I/F to FDMA driver.
* @pre
* @post
* 
* @implements SEQ_FdmaComm_t_struct
*****************************************************************************/
typedef struct SEQ_FdmaComm
{
  FDMA_Td_t *mppTdt[FDMA_TDT_CNT]; ///< transfer descriptor tables pointers
  uint32_t   mpTdtPhysAddr[FDMA_TDT_CNT]; ///< TDT physical adresses
  
  FDMA_SeqEventHandler_t mpEvent2FdmaSend; ///< event send func from Seq2Fdma
  FDMA_TcSchedule_t      mpFdmaTcSchedule; ///< FDMA drv TC scheduling func ptr
  SEQ_FdmaTdSchedule_t   mpSeqTcSchedule;  ///< SEQ drv TC scheduling func ptr
  
} SEQ_FdmaComm_t;

#endif //defined(__KERNEL__) || defined(__STANDALONE__)

#ifndef __STANDALONE__  
  typedef uint64_t jiffy_t;
#endif // ifndef SEQ_SA_KERNEL_API_H

#ifdef __cplusplus 
} 
#endif 

//****************************************************************************
#endif /* SEQTYPES_H */

/*EOF*/