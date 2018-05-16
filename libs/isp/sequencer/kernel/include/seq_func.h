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
* \file     seq_func.h
* \brief    declaration of Sequencer driver internal functionality
****************************************************************************/
#ifndef SEQFUNC_H
#define SEQFUNC_H

#ifndef __STANDALONE__
#include <linux/interrupt.h>    // For interrupts
#endif //#ifdef __STANDALONE__

#include "seq.h"
#include "seq_types.h"

#ifdef __cplusplus 
extern "C" { 
#endif

typedef volatile uint32_t vuint32_t;

//*****************************************************************************
// constants
//*****************************************************************************

#define SEQ_WDT_TIMEOUT 300	  ///< Sent message watch dog timeout
#define SEQ_RAB_TIMEOUT SEQ_WDT_TIMEOUT	  ///< Ready after boot wait timeout

#ifdef __STANDALONE__
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef uint32_t irqreturn_t;
#endif // #ifndef __STANDALONE__

/*****************************************************************************
* macros
*****************************************************************************/

#include "fdma_types.h"
//*****************************************************************************
// struct definitions
//*****************************************************************************

//*****************************************************************************
// functions (prototypes)
//*****************************************************************************

/****************************************************************************/
/** Sets up the internal functionality of the Sequencer kernel module. 
* Initializes Sequencer related resources.
* 
* \param apSeqDrvInfo pointer to prefilled sequencer driver infor structure
* 
* \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
*
*****************************************************************************/
int32_t SEQ_DRV_Setup(SEQ_DrvInfo_t *apSeqDrvInfo);

/****************************************************************************/
/** Releases all Sequencer related resources.
* 
* \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
*
*****************************************************************************/
int32_t SEQ_DRV_Close(void);

/****************************************************************************/
/** Resets the Sequencer HW.
*  
*****************************************************************************/
void SEQ_DRV_Reset(void);

/****************************************************************************/
/** Attempt to boot Sequencer.
* Waits for the ready-after-boot message to arrive. Fails if wait times out or
* reset is done.
* 
*  \return SEQ_DRV_SUCCESS if all OK, otherwise SEQ_DRV_FAILURE.
*****************************************************************************/
uint32_t SEQ_DRV_Boot(void);

/****************************************************************************/
/** Sends message to Sequencer.
* 
* \param apMsg pointer to the message structure
* \param aWait if non zero the call blocks until SeqFW acknowledges the MSG.
* 
* \return SEQ_DRV_SUCCESS if all OK, SEQ_DRV_FAILURE otherwise
*****************************************************************************/
int32_t SEQ_DRV_MsgSend(SEQ_Message_t *apMsg, uint8_t aWait);

/****************************************************************************/
/** Invokes fDMA TD scheduling through Sequencer HW.
* 
* \param apcFdmaTc pointer to Fdma transfer channel
* 
* \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
*
*****************************************************************************/
int32_t SEQ_DRV_FdmaTcScheduleMsgSend(const FDMA_Tc_t* apcFdmaTc);

/****************************************************************************/
/** Downloads Graph to PRAM location.
* 
* \param apGraph pointer to graph structure in user-space
* 
* \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
*
*****************************************************************************/
int32_t SEQ_DRV_GraphDownload(SEQ_GraphPackage_t *apGraph);

/****************************************************************************/
/** Copies the Patch list to PRAM.
 * 
 * \param  apPatchArr patch array pointer
 * \param  aPatchNum  number of patch lists in the array         
 * 
 * \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
 ****************************************************************************/
int32_t SEQ_DRV_PatchListSet(SEQ_patch_list_t *apPatchArr, 
                             uint32_t          aPatchNum);

/****************************************************************************/
/** Loads current state of the Graph from PRAM location.
* 
* \param apGraph pointer to graph structure in user-space
* 
* \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
*
*****************************************************************************/
int32_t SEQ_DRV_GraphFetch(SEQ_GraphPackage_t *apGraph);

/****************************************************************************/
/** Executes the Graph.
* 
* \param aFrmCnt number of frames to capture
* \param aFrmCnt number of lines per input frame (0 => given by CSI)
* 
* \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
*
*****************************************************************************/
int32_t SEQ_DRV_GraphStart(uint32_t aFrmCnt, uint32_t aInputLines);

/****************************************************************************/
/** Requests graceful stop of the Graph execution.
 * 
 * Current frame will be finished.
 * 
 * \param aWait waits for Sequencer ACK if > 0
 * 
 * \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
 ****************************************************************************/
int32_t SEQ_DRV_GraphStop(uint8_t aWait);

#ifdef __STANDALONE__
/****************************************************************************/
/** Returns 0 if graph execution is off.
 * 
 * \return 0 if graph is off
 *         1 if graph is on
 ****************************************************************************/
int32_t SEQ_DRV_GraphStateGet(void);

/****************************************************************************/
/** Enables logging of specified events.
* 
* \return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE otherwise
*****************************************************************************/
uint32_t SEQ_DRV_EventLogEnable(void);

/****************************************************************************/
/** Enables logging of specified events.
* 
* \return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE otherwise
*****************************************************************************/
uint32_t SEQ_DRV_EventLogDisable(void);

/****************************************************************************/
/** Gets the Event series.
* The event series contains so far recieved events. Max 500 0000 events can be
* collected.
* 
* \param apEventSeries pointer to event series structure
* 
* \return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE if no data ready yet or apIpuRegs was NULL
*****************************************************************************/
int32_t SEQ_DRV_EventLogGet(SEQ_EventLog_t **appEventLog);
#endif // #ifdef __STANDALONE__

/****************************************************************************/
/** Requests frame buffers for one frame stream.
* 
* Based on data given in "Request" structure prepares specified stream and
* allocates framebuffer structures. Addresses for buffer data have to be 
* included in the request structure.
* 
* Request for 0 buffer count makes the previous stream setup to be released.
* 
* \param apRequest defines request details
* 
* \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
*
*****************************************************************************/
int32_t SEQ_DRV_FrmBuffersRequest(SEQ_BufferRequest_t *apRequest);

/****************************************************************************/
/** Resets all allocated streams.
*
*****************************************************************************/
void SEQ_DRV_FrmStreamsReset(void);

/****************************************************************************/
/** Queries buffer at given location.
* 
* Based on buffer location specified by the stream and buffer index in 
* BufferInfo structure the status and size of the buffer is updated in 
* the structure.
* 
* \param apRequest defines request details
* 
* \return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE if no such buffer exists
*
*****************************************************************************/
int32_t SEQ_DRV_FrmBufferQuery(SEQ_FrmBufferInfo_t *apBufferInfo);

/****************************************************************************/
/** 
* Gets current state of temporal frame statistics.
* 
* \return temporal statistics structure
*
*****************************************************************************/
SEQ_FrmTemporalStat_t SEQ_DRV_TimeStatsGet(void);

/****************************************************************************/
/** Adds frame buffer to work queue.
* 
* Performs checks if buffer is available to user.
* 
* \param apBufferInfo buffer identification (stream + buffer index).
* 
* \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
*
*****************************************************************************/
int32_t SEQ_DRV_FrmBufferPush(SEQ_FrmBufferInfo_t *apBufferInfo);

/****************************************************************************/
/** Removes frame buffer from DONE queue.
* 
* \param apBufferInfo buffer identification (stream + buffer index).
* 
* \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
*
*****************************************************************************/
int32_t SEQ_DRV_FrmBufferPop(SEQ_FrmBufferInfo_t *apBufferInfo);

/****************************************************************************/
/** Copies PRAM auxiliary data to given structure.
* 
* \param apAuxData address of the AuxData package
*
*****************************************************************************/
void SEQ_DRV_PramAuxDataGet(SEQ_AuxData_t* apAuxData);

/****************************************************************************/
/** Copies provided register list to PRAM.
* 
* \param  apRegList register list to copy
* \return SEQ_DRV_SUCCESS if all ok
*         SEQ_DRV_FAILURE otherwise
*****************************************************************************/
int32_t SEQ_DRV_RegListSet(SEQ_RegList_t* apRegList);

/****************************************************************************/
/** Copies data from PRAM to provided register list.
* Data copied only if there is something to be read.
* 
* \param  apRegList register list to copy
* \return SEQ_DRV_SUCCESS if all ok
*         SEQ_DRV_FAILURE otherwise
*****************************************************************************/
int32_t SEQ_DRV_RegListGet(SEQ_RegList_t* apRegList);

/****************************************************************************/
/** sets user proccess pid
* 
* \param aUsrPid user pid to be set
*
*****************************************************************************/
void SEQ_UserPidSet(pid_t aUsrPid);

/****************************************************************************/
/** Request IPU profiling to be executed for next N frames.
* 
* \param aFrmCnt number of frames to let the profiling to be on
* 
* \return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE otherwise
*****************************************************************************/
int32_t SEQ_DRV_IpuPerformanceInfoRequest(uint32_t aFrmCnt);

/****************************************************************************/
/** Fetches IPU performance info from registers.
* 
* \param apIpuRegs pointer to array of IPU performance register structures
* 
* \return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE if no data ready yet or apIpuRegs was NULL
*****************************************************************************/
int32_t SEQ_DRV_IpuPerformanceInfoGet(SEQ_IpuPerformanceRegs_t *apIpuRegs);

/****************************************************************************/
/** Enables the use of histogram on particular IPU.
*   Only IPU 0-3 are capable of generating histogram.
* 
* \param aIpuIdx index of the IPU
* 
* \return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE if no data ready yet or apIpuRegs was NULL
*****************************************************************************/
int32_t SEQ_DRV_IpuHistogramEnable(SEQ_IpuEngine_t aIpuIdx);

/****************************************************************************/
/** Disables the use of histogram on particular IPU.
*   Only IPU 0-3 are capable of generating histogram.
* 
* \param aIpuIdx index of the IPU
* 
* \return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE if no data ready yet or apIpuRegs was NULL
*****************************************************************************/
void SEQ_DRV_IpuHistogramDisable(SEQ_IpuEngine_t aIpuIdx);

/****************************************************************************/
/** Gets the histogram from specified IPU engine.
*   Only IPU 0-3 are capable of generating histogram.
* 
* \param apHist pointer to histogram head structure
* 
* \return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE if no data ready yet or apIpuRegs was NULL
*****************************************************************************/
int32_t SEQ_DRV_IpuHistogramGet(SEQ_IpuHistHead_t *apHist);


/****************************************************************************/
/** Enables the use of statistic data from IPUS_7.
* 
* \return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE if no data ready yet
*****************************************************************************/
int32_t SEQ_DRV_IpuStatEnable(void);

/****************************************************************************/
/** Disables the use of statistic data from IPUS_7.
* 
* \return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE if no data ready yet
*****************************************************************************/
void SEQ_DRV_IpuStatDisable(void);

/****************************************************************************/
/** Gets the statistic data from IPUS_7.
* 
* \param apStat pointer to statistic data array
* 
* \return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE if no data ready yet or apStat was NULL
*****************************************************************************/
int32_t SEQ_DRV_IpuStatGet(SEQ_IpuStatHead_t *apStat);

void AllocTest(void);

#ifdef __cplusplus 
} 
#endif 

#endif /* SEQFUNC_H */