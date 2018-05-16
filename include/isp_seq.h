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
* \file     seq_public.h
* \brief    public declarations for sequencer user space library
****************************************************************************/

#ifndef SEQPUBLIC_H
#define SEQPUBLIC_H

#include <stdio.h>  // because of off_t 

#ifndef __STANDALONE__
  #include <sys/types.h>
#endif // #ifndef __STANDALONE__

#include "../libs/isp/sequencer/kernel/include/seq.h"
#include "../isp/inc/seq_graph.h"

//*****************************************************************************
// constants
//*****************************************************************************

#define EVTLOG_STRING_SIZE   (10*10*750) // 10 col * 10 char/col * 750 line/frame 

//*****************************************************************************
// types
//*****************************************************************************

/****************************************************************************/
/** Sequencer user space library return values.
 * 
 ****************************************************************************/
enum SEQ_LIB_RESULT
{
  SEQ_LIB_SUCCESS = 0,
  SEQ_LIB_FAILURE = 1
}; // SEQ_LIB_RESULT

/****************************************************************************/
///< Pointer to a user function handling Sequencer event
typedef void(*SEQ_EventHandler_t)(uint32_t aEventType);

/****************************************************************************/
/** Sequencer even handler class
 * 
 ****************************************************************************/
class SEQ_EventHandlerClass
{
public:
  virtual void SeqEventHandler(uint32_t aEventType) = 0;
}; // SEQ_EventHandlerClass

//*****************************************************************************
// public functionality declarations
//*****************************************************************************

/****************************************************************************/
/** Reserves the Sequencer usage for this process.
 *
 * In Linux the seq special device file is opened.
 * In standalone environment the SEQ_SetUp() function is called directly to
 * initialize the internal driver structures and Sequencer HW.
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_Reserve(void);

/****************************************************************************/
/** Releases the Sequencer usage which was locked to this process.
 * 
 * In Linux the seq special device file is closed.
 * In standalone environment the FDMA_Close() function is called directly to
 * release internal driver resources and disconnect from the HW.
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if the close fails
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_Release(void);

/****************************************************************************/
/** Boots up the Sequencer firmware on M0+ core.
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_Boot(void);

/****************************************************************************/
/** Resets the Sequencer HW & driver data.
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_Reset(void);

/****************************************************************************/
/** Sends the specified message to Sequencer.
* 
* \param apMessage[in] message to be sent
* 
* \return SEQ_LIB_SUCCESS if all ok
*         SEQ_LIB_FAILURE otherwise
*****************************************************************************/
SEQ_LIB_RESULT SEQ_MessageSend(SEQ_Message_t *apMessage, uint8_t aWait=1);

/****************************************************************************/
/** Gets HW address of the Graph in PRAM.
 * Intended for graph pointers update before the download.
 * 
 * \param apGraphAddress[out] next download address
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_GraphAddressGet(uintptr_t *apGraphAddress);

/****************************************************************************/
/** Gets auxiliary message data from PRAM.
 * 
 * \param apData[out]  pointer to user allocated buffer
 * \param aSize        number of aux data (in bytes) to get
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_MsgAuxDataGet(void* apData, uint32_t aSize);

/****************************************************************************/
/** Copies given register list to proper PRAM location.
 * 
 * \param apRegList  pointer to register list structure
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_RegListSet(SEQ_RegList_t* apRegList);

/****************************************************************************/
/** Copies register list from proper PRAM location.
 * 
 * \param apRegList[out]  pointer to register list structure
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_RegListGet(SEQ_RegList_t* apRegList);

/****************************************************************************/
/** Downloads the graph to the PRAM.
 * Requires the sequencer to be reserved.
 * 
 * \param apGraphPackage packed graph structure
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_GraphDownload(SEQ_GraphPackage_t *apGraphPackage);

/****************************************************************************/
/** Fetches current state of the graph from the PRAM.
 * Requires the sequencer to be reserved.
 * 
 * \param apGraphPackage packed graph structure
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_GraphFetch(SEQ_GraphPackage_t *apGraphPackage);

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
* \return SEQ_LIB_SUCCESS if all ok, 
*         SEQ_LIB_FAILURE otherwise
*****************************************************************************/
SEQ_LIB_RESULT SEQ_FrmBuffersRequest(SEQ_BufferRequest_t *apRequest);

/****************************************************************************/
/** Queries buffer at given location.
* 
* Based on buffer location specified by the stream and buffer index in 
* BufferInfo structure the status and size of the buffer is updated in 
* the structure.
* 
* \param apBufferInfo defines request details
* 
* \return SEQ_LIB_SUCCESS if all ok, 
*         SEQ_LIB_FAILURE if no such buffer exists
*
*****************************************************************************/
SEQ_LIB_RESULT SEQ_FrmBufferQuery(SEQ_FrmBufferInfo_t *apBufferInfo);

/****************************************************************************/
/** Gets current state of temporal frame statistics.
* 
* \param apTimeStats buffer identification (stream + buffer index).
* 
* \return SEQ_DRV_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
*
*****************************************************************************/
SEQ_LIB_RESULT SEQ_TimeStatsGet(SEQ_FrmTemporalStat_t& arTimeStats);

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
SEQ_LIB_RESULT SEQ_FrmBufferPush(SEQ_FrmBufferInfo_t *apBufferInfo);

/****************************************************************************/
/** Removes frame buffer from DONE queue.
* 
* \param apBufferInfo buffer identification (stream + buffer index).
* 
* \return SEQ_LIB_SUCCESS if all ok, otherwise SEQ_DRV_FAILURE
*
*****************************************************************************/
SEQ_LIB_RESULT SEQ_FrmBufferPop(SEQ_FrmBufferInfo_t *apBufferInfo);

/****************************************************************************/
/** Invokes the execution of the graph.
 * 
 * \param aFrameCnt number of frames to be processed
 * \param aInputLines number of lines per input frame
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_GraphStart(uint32_t aFrameCnt, uint32_t aInputLines);

/****************************************************************************/
/** Requests a graceful stop of the currently executed graph.
 * 
 * \param aWait if set to zero function will block till graph stopped
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_GraphStop(uint8_t aWait);

/****************************************************************************/
/** Enables histogram readout from particular IPU.
 * 
 * \param aIpuIdx IPU index
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_HistogramEnable(SEQ_IpuEngine_t aIpuIdx);

/****************************************************************************/
/** Gets histogram data from particular IPU.
 *  Only IPU 0-3 are capable of generating histogram.
 * 
 * \param apHist pointer to histogram structure
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_HistogramGet(SEQ_IpuHist_t *apHist);

/****************************************************************************/
/** Disables histogram readout from particular IPU.
 * 
 * \param aIpuIdx IPU index
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_HistogramDisable(SEQ_IpuEngine_t aIpuIdx);

/****************************************************************************/
/** Enables statistics readout from IPUS 7.
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_StatEnable();

/****************************************************************************/
/** Gets statistic data from IPUS 7.
 *
 * \param apHist pointer to stistic structure
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_StatGet(SEQ_IpuStat_t *apStat);

/****************************************************************************/
/** Disables statistics readout from IPUS 7.
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_StatDisable();

/****************************************************************************/
/** Request IPU profiling to be executed for next N frames.
* 
* \param aFrmCnt number of frames to let the profiling to be on
* 
* \return SEQ_LIB_SUCCESS if all ok, 
*         SEQ_LIB_FAILURE otherwise
*****************************************************************************/
SEQ_LIB_RESULT SEQ_IpuPerformanceInfoRequest(uint32_t aFrmCnt);

/****************************************************************************/
/** Fetches IPU performance info from registers.
* 
* \param apIpuRegs pointer to array of IPU performance register structures
* 
* \return SEQ_LIB_SUCCESS if all ok, 
*         SEQ_LIB_FAILURE if no data ready yet or apIpuRegs was NULL
*****************************************************************************/
SEQ_LIB_RESULT SEQ_IpuPerformanceInfoGet(SEQ_IpuPerformanceRegs_t *apIpuRegs);

/****************************************************************************/
/** Gets the Event series.
* The event series contains so far received events. Max 500 0000 events can be
* collected.
* 
* \param apEventSeries pointer to event series structure
* 
* \return SEQ_DRV_SUCCESS if all ok, 
*         SEQ_DRV_FAILURE if no data ready yet or apIpuRegs was NULL
*****************************************************************************/
SEQ_LIB_RESULT SEQ_EventLogGet(SEQ_EventLog_t **appEventLog);

/****************************************************************************/
/** Enables ISP event logging.
* 
* \return SEQ_LIB_SUCCESS if all ok, 
*         SEQ_LIB_FAILURE otherwise
*****************************************************************************/
SEQ_LIB_RESULT SEQ_EventLogEnable(void);

/****************************************************************************/
/** Disables ISP event logging.
* 
* \return SEQ_LIB_SUCCESS if all ok, 
*         SEQ_LIB_FAILURE otherwise
*****************************************************************************/
SEQ_LIB_RESULT SEQ_EventLogDisable(void);

/****************************************************************************/
/** Processes the provided ISP event log to a formated string that can be stored
 * as csv file.
 * 
 * \param arEventLog the EventLog to be processed
 * \param aMaxFrames maximum number of frames to be stared to the string
 * \param apString   Prealocated character array
 * \param aStringSize size of the prealocated charracter array
 *          
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
uint32_t SEQ_EventLogProccess(SEQ_EventLog_t &arEventLog,
                              uint32_t aMaxFrames,
                              char* apString, 
                              size_t aStringSize);

/****************************************************************************/
/** Downloads the Sequencer firmware to CRAM.
 * 
 * \param  apcFwArr pointer to null-terminated array of characters with 
 *                  Sequencer firmware in S-record text format.
 *					
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_FwArrDownload(const char *apcFwArr);

/****************************************************************************/
/** Downloads the specified firmware.
 * 
 * \param  apcFwArr pointer to null-terminated array of characters with 
 *                  firmware in S-record text format.
 * \param aFwType   type of the firmware (for CM0 or IPUx) to be downloaded also 
 *                  specifying destination ram (CRAM/KRAM). 
 *					
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_FwArrDownload(const char *apcFwArr, SEQ_FwType_t aFwType);

/****************************************************************************/
/** Downloads the specified firmware.
 * 
 * \param  apcFwArr path to the FW file in S-REC format.
 * \param aFwType   type of the firmware (for CM0 or IPUx) to be downloaded also 
 *                  specifying destination ram (CRAM/KRAM). 
 *					
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_FwFileDownload(const char *apFwPath, SEQ_FwType_t aFwType);

/****************************************************************************/
/** Downloads the Sequencer firmware to CRAM.
 * 
 * \param  apcFwArr path to the FW file in S-REC format.
 *					
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_FwFileDownload(const char *apFwPath);

/****************************************************************************/
/** Downloads the Patch list to PRAM.
 * 
 * \param  apPatchListArr patch array pointer
 * \param  aPatchListNum  number of patch lists in the array         
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_PatchListSet(SEQ_patch_list_t *apPatchListArr, 
                                uint32_t          aPatchListNum);
 
/****************************************************************************/
/** Register event handler.
 * NULL is a valid parameter. Removes the previous handler.
 * 
 * \param aHandler user defined Sequencer event handling function
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_EventHandlerSet(SEQ_EventHandler_t aHandler);

/****************************************************************************/
/** Register event handler object.
 * NULL is a valid parameter. Removes the previous handler.
 * 
 * \param arHandler user defined Sequencer event handling object
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_EventHandlerSet(SEQ_EventHandlerClass *apHandler);

/****************************************************************************/
/** Instructs the Sequencer to commence SW reset.
 * 
 * \param  aReset type of SW reset
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
 ****************************************************************************/
SEQ_LIB_RESULT seq_swreset(SEQ_reset_t aReset);

/****************************************************************************/
/** Instructs the Sequencer to start specified nodes.
 * 
 * \param apSN array of nodes to be started
 * \param aNumNodes number of nodes in the node array apSN
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT seq_startNode(SEQM_start_node_para_t* apSN, uint16_t aNumNodes);

/****************************************************************************/
/** Sends a check_node message to Sequencer.
 * 
 * \param aNode id of node to be checked
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT seq_checkNode(uint16_t aNode);

/****************************************************************************/
/** Instructs Sequencer to write given value to specific register in IPU.
 * 
 * \param aIsIPUS 
 * \param aEng 
 * \param aDirect
 * \param aOffset
 * \param aValue
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT seq_setReg(uint16_t aIsIPUS, 
                          uint16_t aEng, 
                          uint16_t aDirect, 
                          uint32_t aOffset, 
                          uint32_t aValue);

/****************************************************************************/
/** Instructs the Sequencer to read specific register in IPU.
 * 
 * \param aIsIPUS 
 * \param aEng 
 * \param aDirect
 * \param aOffset
 * \param apValue
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT seq_getReg(uint16_t aIsIPUS, 
                          uint16_t aEng, 
                          uint16_t aDirect, 
                          uint32_t aOffset, 
                          uint32_t* apValue);

/****************************************************************************/
/** Sends an echo message to Sequencer.
 * 
 * \param aDelay time interval the Sequencer should wait before replying to 
 *               the message.
 * 
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
SEQ_LIB_RESULT seq_echo(uint32_t aDelay);

/****************************************************************************/
/** Prints out IPU performance counters.
 * 
 * \param aFramecount number of frames the performance counter are valid for. 
 * 
 ****************************************************************************/
void ISP_DumpPerfCounters(uint16_t aFramecount);

/****************************************************************************/
/** Prints out IPU state.
 * 
 ****************************************************************************/
void ISP_DumpIpuState(void);

/****************************************************************************/
/** Prints out Graph bufers statics.
 * 
 * \param appFetchedBufferList List of graph buffer pointers returned by 
 *           io::FrameInputIsp::GraphFetch() method.
 * 
 ****************************************************************************/
void ISP_PrintGraphStat(SEQ_Buf_t** appFetchedBufferList);

#endif /* SEQPUBLIC_H */
