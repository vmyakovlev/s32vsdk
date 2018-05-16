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
 * \file    seq_comm.h
 * \brief   ISP sequencer communication definition file
 * \author
 * \author
 * \version 0.1
 * \date    14-01-2013
 * \note    initial version
 ****************************************************************************/

#ifndef SEQCOMM_H
#define SEQCOMM_H

#ifndef __ASSEMBLER__

#ifdef  __cplusplus
extern "C" {
#endif

/*****************************************************************************
* publics consts
*****************************************************************************/
#define SEQ_PRAM_MAP_OFF        0x0 
#define SEQ_PRAM_AUX_DATA_SIZE  0x100
#define SEQ_PATCH_CNT           8
#define SEQ_PATCH_LIST_NUM      4
#define SEQ_REG_WRITE_NUM       16
#define SEQ_REG_READ_NUM        16

/****************************************************************************/
/** Enumerates possible message status values (SEQ_Message_t::mStatus).
*****************************************************************************/
typedef enum
{
  SEQ_MSG_STAT_EMPTY = 1,
  SEQ_MSG_STAT_READY,
  SEQ_MSG_STAT_EXECUTING,
  SEQ_MSG_STAT_RESULT
  
} SEQ_message_status_t;

/****************************************************************************/
/** Enumerates possible message type values (SEQ_Message_t::mType)
*
* Defines message types for both directions (SEQ <-> HOST).
*****************************************************************************/
typedef enum
{
  // Host -> Sequencer
  SEQ_MSG_TYPE_NONE = 0,      //< no/invalid message
  SEQ_MSG_TYPE_SWRESET = 1,   //< software reset
  SEQ_MSG_TYPE_START,         //< start the graph
  SEQ_MSG_TYPE_STOP,          //< stop gracefully the graph
  SEQ_MSG_TYPE_START_NODE,    //< start a certain node
  SEQ_MSG_TYPE_CHECK_NODE,    //< check a certain node to be started
  SEQ_MSG_TYPE_SET_REG,       //< set a device register
  SEQ_MSG_TYPE_GET_REG,       //< get the value of a device register
  SEQ_MSG_TYPE_IPU_PROFILE,   //< request IPU profiling to be initiated
  SEQ_MSG_TYPE_STATUS,        //< return current status
  SEQ_MSG_TYPE_FDMASCHED,     //< schedule FDMA transaction
  SEQ_MSG_TYPE_ACKTO,         //< acknowledgment timed out
  SEQ_MSG_TYPE_ECHO,          //< acknowledgment timed out
  SEQ_MSG_TYPE_GET_IDLE_CNT,  //< get the current sequencers idle count 
  SEQ_MSG_TYPE_RES_IDLE_CNT,  //< reset the sequencers idle count 
  SEQ_MSG_TYPE_CSI_VSYNC,     //< CSI interface frame start 
  SEQ_MSG_TYPE_H2S_MAX,       //< max possible H2S message number 
  // Sequencer -> Host
  SEQ_MSG_TYPE_RAB=0x1000,        //< ready after boot
  SEQ_MSG_TYPE_SEQDONE,           //< Sequencer has finished
  SEQ_MSG_TYPE_FRAMEDONE,         //< 1 frame was finished 
  SEQ_MSG_TYPE_FDMADONE,          //< 1 FDMA transaction done
  SEQ_MSG_TYPE_IPU_PROFILE_DONE,  //< IPU profiling finished
  SEQ_MSG_TYPE_DELTA_START,       //< measure time delta start
  SEQ_MSG_TYPE_DELTA_END,         //< measure time delta end
  SEQ_MSG_TYPE_FDMAERROR,         //< FDMA error occurred 
  SEQ_MSG_TYPE_FDMACRCERROR,      //< FDMA CRC error occurred
  SEQ_MSG_TYPE_BUFFOVF,           //< buffer overflow detected
  SEQ_MSG_TYPE_OTHERERROR,        //< other error - might be specified in the future
  SEQ_MSG_TYPE_S2H_MAX            //< max possible message number 

} SEQ_MessageType_t;

typedef enum
{
  RESET_COLD=0,                   //< cold reset
  RESET_WARM,                     //< warm reset
  RESET_STOP                      //< just stop

} SEQ_reset_t;

/*****************************************************************************
* publics types
*****************************************************************************/

/****************************************************************************/
/** Defines structure of a Sequencer message.
*
* The same structure used for both directions (SEQ <-> HOST).
*****************************************************************************/

#pragma pack(push, 2)

typedef struct SEQ_Message
{
  uint16_t	mStatus;	///< message status
  uint16_t	mType;		///< message type - always required
  uint8_t	mData[60];	///< message data

} SEQ_Message_t
#ifdef __GNUC__
__attribute__ ((aligned (2)))
#endif
;

/****************************************************************************/
/** Defines structure of a Sequencer communication info.
*
*****************************************************************************/

typedef struct SEQ_pram_map
{
  // PRAM content
  uint32_t  mOutMsgAddrOff; ///< 00 outbound message address offset
  uint32_t  mAuxDataOff;    ///< 04 outbound message auxiliary data
  uint32_t  mPatchListOff;  ///< 08 offset to the buffer list
  uint32_t  mPatchListNum;  ///< 0c number of patch list used
  uint32_t  mWriteRegOff;   ///< 10 offset to the write register list
  uint32_t  mWriteRegNum;   ///< 14 number of register in write register list
  uint32_t  mReadRegOff;    ///< 18 offset to the read register list
  uint32_t  mReadRegNum;    ///< 1c number of register in read register list
  // CRAM content
  uint32_t  mGraphOff;      ///< 20 offset of the graph
  uint32_t  mBufferListOff; ///< 24 offset to the buffer list
  // !!! has to dividable by 64bit !!!
  
} SEQ_pram_map_t
#ifdef __GNUC__
__attribute__ ((aligned (2)))
#endif
;

/****************************************************************************/
/** Defines structure of a Sequencer patch list.
*
*****************************************************************************/

typedef struct SEQ_patch_list
{
  uint16_t  mOffset;				///< offset from graph base of location to be patched
  uint16_t  mNumber;				///< number of values in value array
  uint16_t  mVal[SEQ_PATCH_CNT];	///< value array

} SEQ_patch_list_t
#ifdef __GNUC__
__attribute__ ((aligned (2)))
#endif
;

/****************************************************************************/
/** Structure defining the Sequencer messages payload(content).
*****************************************************************************/

typedef struct SEQM_start
{
  uint16_t  mNumCamLines;	///< number of camera lines per frame
  uint32_t  mFDMAlistBase0;	///< base address#0 of the FDMA list table to use for this frame
  uint32_t  mFDMAlistBase1;	///< base address#1 of the FDMA list table to use for this frame
  uint32_t  mFrames2Grab;   ///< number of frames to be grabbed (0 => unlimited)

} SEQM_start_t
#ifdef __GNUC__
__attribute__ ((aligned (2)))
#endif
;

typedef struct SEQM_swreset
{
  SEQ_reset_t	mReset;

} SEQM_swreset_t
#ifdef __GNUC__
__attribute__ ((aligned (2)))
#endif
;

typedef struct SEQM_start_node_para
{
  uint16_t	mNode;	///< index of nodes to start
  uint16_t	mChan;	///< channel of nodes to start

} SEQM_start_node_para_t
#ifdef __GNUC__
__attribute__ ((aligned (2)))
#endif
;

typedef struct SEQM_start_node
{
  uint16_t			mNum;	///< number of nodes to start
  SEQM_start_node_para_t	mNodes;	///< index/channel of nodes to start

} SEQM_start_node_t
#ifdef __GNUC__
__attribute__ ((aligned (2)))
#endif
;

typedef struct SEQM_check_node
{
  uint16_t	mNode;	///< index of nodes to start

} SEQM_check_node_t
#ifdef __GNUC__
__attribute__ ((aligned (2)))
#endif
;

typedef struct SEQM_ipu_reg
{
  uint32_t	mEngBase;	///< base address of engine
  uint16_t	mIndex;		///< index of register
  uint32_t	mData;		///< value of register
  uint16_t	mDirect;	///< direct address access

} SEQM_ipu_reg_t
#ifdef __GNUC__
__attribute__ ((aligned (2)))
#endif
;

typedef struct SEQM_echo
{
  uint32_t	mReplayDly;	///< replay delay

} SEQM_echo_t
#ifdef __GNUC__
__attribute__ ((aligned (2)))
#endif
;

typedef struct SEQM_profile
{
  uint32_t  mFrmCnt; ///< for how many frames to profile

} SEQM_profile_t
#ifdef __GNUC__
__attribute__ ((aligned (2)))
#endif
;

typedef struct SEQM_idle_cnt
{
  uint32_t  mIdleL; ///< idle count low
  uint32_t  mIdleH; ///< idle count high

} SEQM_idle_cnt_t
#ifdef __GNUC__
__attribute__ ((aligned (2)))
#endif
;

typedef struct SEQM_delta
{
  uint32_t  mDelta; ///< number of actions

} SEQM_delta_t
#ifdef __GNUC__
__attribute__ ((aligned (2)))
#endif
;

typedef struct SEQM_fdma_sched
{
  uint32_t  mLineNums; ///< SRAM/DDR line numbers to be set
  uint32_t  mTdIdx;    ///< index of TD to be scheduled
} SEQM_fdma_sched_t
#ifdef __GNUC__
__attribute__ ((aligned (2)))
#endif
;

typedef struct SEQM_fdma_event
{
  uint32_t mTdIdx;    ///< index of TD corresponding to the event
  uint32_t mXfrStat;	///< status register copy
  uint32_t mXfrCalcCrcVal; ///< calculated crc value
  uint32_t mXfrCurrDdrPtr;	 ///< current ddr pointer
  uint32_t mXfrCurrSramPtr;  ///< current sram pointer
  uint32_t mSramNextLine;    ///< sram next line
} SEQM_fdma_event_t
#ifdef __GNUC__
__attribute__ ((aligned (2)))
#endif
;

typedef struct SEQM_frame_end
{
  uint32_t mFrameSeq;    ///< frame sequence number
} SEQM_frame_end_t
#ifdef __GNUC__
__attribute__ ((aligned (2)))
#endif
;

#pragma pack(pop)

/*****************************************************************************
* publics functions (prototypes)
*****************************************************************************/

#ifdef  __cplusplus
}
#endif

#endif // __ASSEMBLER__

#endif /* SEQCOMM_H */

/*EOF*/
