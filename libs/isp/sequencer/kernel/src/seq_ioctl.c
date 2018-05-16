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
 * @file     seq_ioctl.c
 * @brief    IOCTL functionality of Sequencer driver
 ****************************************************************************/

#include <asm/uaccess.h>	// for get_user and put_user 
#include "s32vs234.h"
#include <linux/mutex.h>	// for semaphore/mutex
#include <linux/string.h>	// for memset
#include <linux/sched.h>	// for current->pid
#include <linux/slab.h>		// for malloc and free


#include "seq.h"
#include "seq_types.h"
#include "seq_ioctl.h"
#include "seq_func.h"
#include "vdb_log.h"
// 
//*****************************************************************************
// external declarations
//*****************************************************************************

// pointer to fdma driver event handler
extern int32_t (*gpSEQ_FdmaEventHandler)(uint32_t aTdIdx, uint32_t aEventType);

//*****************************************************************************
// global data
//*****************************************************************************

//*****************************************************************************
// local function declarations
//*****************************************************************************

/****************************************************************************/
/** Sends message to Sequencer.
* 
* \param aIoctlParam the IOCT parameter.
* \param aWait       0 if wait for ACK not required, > 0 otherwise
* 
* \return SEQ_DRV_SUCCESS if all ok
*         SEQ_DRV_FAILURE otherwise
*****************************************************************************/
int32_t MsgSend(unsigned long aIoctlParam, uint8_t aWait);

/****************************************************************************/
/** Requests buffers
* 
* \param aIoctlParam the IOCT parameter.
* 
* \return SEQ_DRV_SUCCESS if all ok
*         SEQ_DRV_FAILURE otherwise
*****************************************************************************/
int32_t BuffersRequest(unsigned long aIoctlParam);

/****************************************************************************/
/** Queries buffer status.
* 
* \param aIoctlParam the IOCT parameter.
* 
* \return SEQ_DRV_SUCCESS if all ok
*         SEQ_DRV_FAILURE otherwise
*****************************************************************************/
int32_t BufferQuery(unsigned long aIoctlParam);

/****************************************************************************/
/** Push the buffer.
* 
* \param aIoctlParam the IOCT parameter.
* 
* \return SEQ_DRV_SUCCESS if all ok
*         SEQ_DRV_FAILURE otherwise
*****************************************************************************/
int32_t BufferPush(unsigned long aIoctlParam);

/****************************************************************************/
/** Pop the buffer
* 
* \param aIoctlParam the IOCT parameter.
* 
* \return SEQ_DRV_SUCCESS if all ok
*         SEQ_DRV_FAILURE otherwise
*****************************************************************************/
int32_t BufferPop(unsigned long aIoctlParam);

/****************************************************************************/
/** Copies the aux data to user space buffer.
* 
* \param aIoctlParam the IOCT parameter.
* 
* \return SEQ_DRV_SUCCESS if all ok
*         SEQ_DRV_FAILURE otherwise
*****************************************************************************/
int32_t AuxDataGet(unsigned long aIoctlParam);

/****************************************************************************/
/** Copies provided register list to PRAM.
* 
* \param  aIoctlParam the IOCT parameter.
* \return SEQ_DRV_SUCCESS if all ok
*         SEQ_DRV_FAILURE otherwise
*****************************************************************************/
int32_t RegListSet(unsigned long aIoctlParam);

/****************************************************************************/
/** Copies data from PRAM to provided register list.
* Data copied only if there is something to be read.
* 
* \param  aIoctlParam the IOCT parameter.
* \return SEQ_DRV_SUCCESS if all ok
*         SEQ_DRV_FAILURE otherwise
*****************************************************************************/
int32_t RegListGet(unsigned long aIoctlParam);

/****************************************************************************/
/** Requests the graph execution to be started
* 
* \return SEQ_DRV_SUCCESS if all ok
*     SEQ_DRV_FAILURE otherwise
*****************************************************************************/
int32_t GraphStart(unsigned long aIoctlParam);

/****************************************************************************/
/** Get histogram
* 
* \return SEQ_DRV_SUCCESS if all ok
*         SEQ_DRV_FAILURE otherwise
*****************************************************************************/
int32_t HistogramGet(unsigned long aIoctlParam);

/****************************************************************************/
/** Get statistics
* 
* \return SEQ_DRV_SUCCESS if all ok
*         SEQ_DRV_FAILURE otherwise
*****************************************************************************/
int32_t StatGet(unsigned long aIoctlParam);

/****************************************************************************/
/** Apply patch list
* 
* \return SEQ_DRV_SUCCESS if all ok
*         SEQ_DRV_FAILURE otherwise
*****************************************************************************/
int32_t PatchListSet(unsigned long aIoctlParam);

//*****************************************************************************

long SeqIoctl(struct file *apFile,	/* ditto */
		 unsigned int aIoctlNum,	/* number and param for ioctl */
		 unsigned long aIoctlParam)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  uint8_t lWait = 0;
  
  switch (aIoctlNum) {
   
    // *** send message ***
    case IOCTL_SEQ_MSG_SEND_WAIT:
      lWait = 1;
    case IOCTL_SEQ_MSG_SEND:
    {
      MsgSend(aIoctlParam, lWait);
      lWait = 0;
    } // case IOCTL_SEQ_MSG_SEND
      break;
      
    // *** download/fetch graph ***
    case IOCTL_SEQ_GRAPH_DOWNLOAD:
    {
      lRet = SEQ_DRV_GraphDownload((SEQ_GraphPackage_t*)aIoctlParam);      
    } // case IOCTL_SEQ_GRAPH_DOWNLOAD
      break;
      
    case IOCTL_SEQ_GRAPH_FETCH:
    {
      lRet = SEQ_DRV_GraphFetch((SEQ_GraphPackage_t*)aIoctlParam);
    } // case IOCTL_SEQ_GRAPH_FETCH
      break;      
      
    // *** Request framebuffer stream to be created ***
    case IOCTL_SEQ_BUF_REQ:
    {
      lRet = BuffersRequest(aIoctlParam);      
    } // case IOCTL_SEQ_BUF_REQ
      break;
      
    // *** Query buffer state ***
    case IOCTL_SEQ_BUF_QRY:
    {
      lRet = BufferQuery(aIoctlParam);
    } // case IOCTL_SEQ_BUF_QRY
      break;
    
    // *** Query buffer state ***
    case IOCTL_SEQ_TIME_STAT:
    {
      SEQ_FrmTemporalStat_t lTimeStat = SEQ_DRV_TimeStatsGet();
      if (copy_to_user( (uint8_t *)aIoctlParam, 
                        (uint8_t*)&lTimeStat, 
                        sizeof(SEQ_FrmTemporalStat_t)) != 0)
      {
        VDB_LOG_ERROR("Failed to copy temporal statistics to user space.\n");
        lRet = -EIO;
      } // if copy to user failed
    } // case IOCTL_SEQ_TIME_STAT
      break;      
      
    case IOCTL_SEQ_BUF_PSH:
    {
      lRet = BufferPush(aIoctlParam);
    } // case IOCTL_SEQ_BUF_PSH
      break;
      
    case IOCTL_SEQ_BUF_POP:
    {
      lRet = BufferPop(aIoctlParam);
    } // case IOCTL_SEQ_BUF_POP
      break;
      
    case IOCTL_SEQ_IPU_PROFILE_REQ:
    {
      lRet = SEQ_DRV_IpuPerformanceInfoRequest((uint32_t) aIoctlParam);
    } // case IOCTL_SEQ_IPU_PROFILE_REQ
      break;
      
    case IOCTL_SEQ_IPU_PROFILE_GET:
    {
      SEQ_IpuPerformanceRegs_t lpIpuRegs[IPUS_CNT + IPUV_CNT];
      lRet = SEQ_DRV_IpuPerformanceInfoGet(lpIpuRegs);
      if (copy_to_user( (uint8_t *)aIoctlParam, 
                        (uint8_t*)lpIpuRegs, 
                        sizeof(SEQ_IpuPerformanceRegs_t)*(IPUS_CNT + IPUV_CNT)) 
          != 0)
      {
        VDB_LOG_ERROR("Failed to copy the IPU regs structure to user space.\n");
        lRet = -EIO;
      } // if copy to user failed
    } // case IOCTL_SEQ_IPU_PROFILE_GET
      break;
      
    // *** starts the graph execution ***
    case IOCTL_SEQ_START:
    {
      // send start message
      lRet = GraphStart(aIoctlParam);    
      
    } // case IOCTL_SEQ_START
      break;
      
    // *** copies aux data to userspace buffer ***
    case IOCTL_SEQ_AUX_DATA_GET:
    {
      // send start message
      lRet = AuxDataGet(aIoctlParam);      
      
    } // case IOCTL_SEQ_START
      break;
    
    // *** copies data from PRAM to provided register list ***
    case IOCTL_SEQ_REGLIST_SET:
    {
      // send start message
      lRet = RegListSet(aIoctlParam);     
    } // case IOCTL_SEQ_REGLIST_SET
      break;
      
    // *** copies provided register list to PRAM ***
    case IOCTL_SEQ_REGLIST_GET:
    {
      // send start message
      lRet = RegListGet(aIoctlParam);      
    } // case IOCTL_SEQ_REGLIST_GET
      break;
      
    // *** request the sequencer to stop ***
    case IOCTL_SEQ_STOP_WAIT:
      lWait = 1;
    case IOCTL_SEQ_STOP:
    {  
      // send stop message
      lRet = SEQ_DRV_GraphStop(lWait);     
      lWait = 0;
      VDB_LOG_NOTE("Stop message sent.\n");
    } // case IOCTL_SEQ_STOP
      break;
      
    // *** resets the SEQ HW and driver ***
    case IOCTL_SEQ_RESET:
    {  
      SEQ_DRV_Reset();      
      VDB_LOG_NOTE("Reset done.\n");
    } // case IOCTL_SEQ_STOP
      break;
    
    // *** boots up the SEQ HW ***
    case IOCTL_SEQ_BOOT:
    {  
      lRet = SEQ_DRV_Boot();
      if(lRet == SEQ_DRV_SUCCESS)
      {
        VDB_LOG_NOTE("Sequencer booted.\n");
      }
      else
      {
        VDB_LOG_ERROR("Sequencer boot failed.\n");
      }
    } // case IOCTL_SEQ_STOP
      break;
    
    case IOCTL_SEQ_HIST_ENABLE:
    { 
      SEQ_IpuEngine_t lIpuIdx = (SEQ_IpuEngine_t)aIoctlParam;
      lRet = SEQ_DRV_IpuHistogramEnable(lIpuIdx);
    } // case IOCTL_SEQ_HIST_ENABLE
      break;
      
    case IOCTL_SEQ_HIST_DISABLE:
    { 
      SEQ_IpuEngine_t lIpuIdx = (SEQ_IpuEngine_t)aIoctlParam;
      SEQ_DRV_IpuHistogramDisable(lIpuIdx);
    } // case IOCTL_SEQ_HIST_DISABLE
      break;
      
    case IOCTL_SEQ_HIST_GET:
    { 
      lRet = HistogramGet(aIoctlParam);
    } // case IOCTL_SEQ_HIST_GET
      break;
      
    case IOCTL_SEQ_STAT_ENABLE:
    { 
      lRet = SEQ_DRV_IpuStatEnable();
    } // case IOCTL_SEQ_STAT_ENABLE
      break;
      
    case IOCTL_SEQ_STAT_DISABLE:
    { 
      SEQ_DRV_IpuStatDisable();
    } // case IOCTL_SEQ_STAT_DISABLE
      break;
      
    case IOCTL_SEQ_STAT_GET:
    { 
      lRet = StatGet(aIoctlParam);
    } // case IOCTL_SEQ_STAT_GET
      break;

    case IOCTL_SEQ_PATCH:
    { 
      lRet = PatchListSet(aIoctlParam);
    } // case IOCTL_SEQ_PATCH
      break;
    
    // *** unknown message ***
    default:
    {     
      VDB_LOG_ERROR("Unknown ioctl command %u.\n", aIoctlNum);
      lRet = SEQ_DRV_FAILURE;
    } // default
      break;
  } // switch ioctl command
  
  return lRet;
} // SeqIoctl()

//*****************************************************************************

int32_t MsgSend(unsigned long aIoctlParam, uint8_t aWait)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  SEQ_Message_t lMsg;
  if (copy_from_user( &lMsg, (SEQ_Message_t *)aIoctlParam, 
      sizeof(SEQ_Message_t)) != 0)
  {
    VDB_LOG_ERROR("Copy from user failed.\n");
    lRet = -EIO;
  } // if copy from user failed
  else
  {   
    VDB_LOG_NOTE("Sending a message.\n");

    if(SEQ_DRV_MsgSend(&lMsg, aWait) == SEQ_DRV_SUCCESS)
    {
      VDB_LOG_NOTE("The message was sent\n");
    }
    else
    {
      VDB_LOG_NOTE("Failed to send the message.\n");
      lRet = SEQ_DRV_FAILURE;
    }
    
    if(aWait)
    {
      if (copy_to_user( (uint8_t *)aIoctlParam, 
                        (uint8_t*)&lMsg, 
                        sizeof(SEQ_Message_t)) != 0)
      {
        VDB_LOG_ERROR("Failed to copy the message data back to user space.\n");
        lRet = -EIO;
      } // if copy to user failed
    }
    
  } // else from if copy from user failed
  
  return lRet;
} // MsgSend()

//*****************************************************************************

int32_t AuxDataGet(unsigned long aIoctlParam)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  SEQ_AuxData_t lAuxData;
  
  // copy the aux data destination descriptor
  if (copy_from_user(&lAuxData, (uint8_t *)aIoctlParam, 
      sizeof(SEQ_AuxData_t)) != 0)
  {
    VDB_LOG_ERROR("Failed to read aux data paramateres.\n");
  } // if graph data download failed
  else 
  {
    SEQ_AuxData_t lTmpData;
    lTmpData.mSize  = lAuxData.mSize;
    lTmpData.mpData = lAuxData.mpData;
    SEQ_DRV_PramAuxDataGet(&lTmpData);
  
    // copy PRAM back to user
    if (copy_to_user(lAuxData.mpData, lTmpData.mpData, lTmpData.mSize) != 0)
    {
      VDB_LOG_ERROR("Failed to copy the aux data to user space.\n");
      lRet = -EIO;
    } // if copy to user failed
    else
    {
      if (copy_to_user( (uint8_t *)aIoctlParam, 
                        (uint8_t*)&lAuxData, 
                        sizeof(SEQ_AuxData_t)) != 0)
      {
        VDB_LOG_ERROR("Failed to copy the aux data structure to user space.\n");
        lRet = -EIO;
      } // if copy to user failed
    }
    
    VDB_LOG_NOTE("AuxDataPushed to user-space.\n");    
  } // else form if graph data download failed
  
  return lRet;
} // AuxDataGet(SEQ_AuxData_t *apData)

//*****************************************************************************

int32_t RegListSet(unsigned long aIoctlParam)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  SEQ_RegList_t lRegList;
  
  // copy the reglist
  if (copy_from_user(&lRegList, (uint8_t *)aIoctlParam, 
      sizeof(SEQ_RegList_t)) != 0)
  {
    VDB_LOG_ERROR("Failed to read register list.\n");
    lRet = SEQ_DRV_FAILURE;
  } // if graph data download failed
  else 
  {  
    // copy to PRAM
    if (SEQ_DRV_RegListSet(&lRegList) != SEQ_DRV_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to setup register list.\n");
      lRet = -EIO;
    } // if copy to pram failed
    
    // copy the reglist
    if (copy_to_user((uint8_t *)aIoctlParam, 
          &lRegList, 
          sizeof(SEQ_RegList_t)) != 0)
    {
      VDB_LOG_ERROR("Failed to write register list back to userspace.\n");
      lRet = SEQ_DRV_FAILURE;
    } // if graph data download failed
  } // else form if graph data download failed
  
  return lRet;
} // RegListSet()

//*****************************************************************************

int32_t RegListGet(unsigned long aIoctlParam)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  SEQ_RegList_t lRegList;
  
  // copy the reglist
  if (copy_from_user(&lRegList, (uint8_t *)aIoctlParam, 
      sizeof(SEQ_RegList_t)) != 0)
  {
    VDB_LOG_ERROR("Failed to read register list.\n");
  } // if graph data download failed
  else 
  {  
    // copy from PRAM
    if (SEQ_DRV_RegListGet(&lRegList) != SEQ_DRV_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to readback register list.\n");
      lRet = -EIO;
    } // if copy to pram failed
  } // else form if graph data download failed
  
  return lRet;
} // RegListSet()

//*****************************************************************************

int32_t BuffersRequest(unsigned long aIoctlParam)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  SEQ_BufferRequest_t lRequest;
  
  if (copy_from_user( &lRequest, (SEQ_BufferRequest_t *)aIoctlParam, 
      sizeof(SEQ_BufferRequest_t)) != 0)
  {
    VDB_LOG_ERROR("Copy from user failed.\n");
    lRet = -EIO;
  } // if copy from user failed
  else
  {
    size_t lBufferListSz = lRequest.mCnt * sizeof(uint32_t);
    uint32_t *lpBufferArr = kmalloc(lBufferListSz, GFP_KERNEL);
    if( lpBufferArr != NULL)
    {
      if (copy_from_user( lpBufferArr, (uint32_t*)lRequest.mpBuffers, 
          lBufferListSz) != 0)
      {
        VDB_LOG_ERROR("Copy from user failed.\n");
        lRet = -EIO;
      } // if copy from user failed
      else
      { 
        lRequest.mpBuffers = lpBufferArr;
        if(SEQ_DRV_FrmBuffersRequest(&lRequest) != SEQ_DRV_SUCCESS)
        {
          VDB_LOG_ERROR("Frame buffers request failed.\n");
          lRet = SEQ_DRV_FAILURE;
        } // if request failed
        kfree(lpBufferArr);
      } // else from if copy from user failed
    } // if buffer array allocated
    else
    {
      VDB_LOG_ERROR("Failed to allocate buffer array.\n");
      lRet = SEQ_DRV_FAILURE;
    } // else from if buffer array allocated
  } // else from if copy from user failed
  
  return lRet;
} // BuffersRequest()

//*****************************************************************************

int32_t BufferQuery(unsigned long aIoctlParam)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  SEQ_FrmBufferInfo_t lBuffInfo;
      
  if (copy_from_user( &lBuffInfo, (SEQ_FrmBufferInfo_t *)aIoctlParam, 
      sizeof(SEQ_FrmBufferInfo_t)) != 0)
  {
    VDB_LOG_ERROR("Copy from user failed.\n");
    lRet = -EIO;
  } // if copy from user failed
  else
  { 
    // query the buffer
    if(SEQ_DRV_FrmBufferQuery(&lBuffInfo) != SEQ_DRV_SUCCESS)
    {
      VDB_LOG_WARNING("Frame buffers query failed.\n");
      lRet = SEQ_DRV_FAILURE;
    }
    
    // copy BufferInfo back to user
    if (copy_to_user((SEQ_FrmBufferInfo_t *)aIoctlParam, &lBuffInfo, 
        sizeof(SEQ_FrmBufferInfo_t)) != 0)
    {
      VDB_LOG_ERROR("Failed to copy the buffer info to user space.\n");
      lRet = -EIO;
    } // if copy to user failed
  } // else from if copy from user failed
  
  return lRet;
} // BufferQuery()

//*****************************************************************************

int32_t BufferPush(unsigned long aIoctlParam)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  SEQ_FrmBufferInfo_t lBuffInfo;
      
  if (copy_from_user( &lBuffInfo, (SEQ_FrmBufferInfo_t *)aIoctlParam, 
      sizeof(SEQ_FrmBufferInfo_t)) != 0)
  {
    VDB_LOG_ERROR("Copy from user failed.\n");
    lRet = -EIO;
  } // if copy from user failed
  else
  { 
    // Push the buffer
    if(SEQ_DRV_FrmBufferPush(&lBuffInfo) != SEQ_DRV_SUCCESS)
    {
      VDB_LOG_ERROR("Frame buffers query failed.\n");
      lRet = SEQ_DRV_FAILURE;
    }
  } // else from if copy from user failed
  
  return lRet;
} // BufferPush()

//*****************************************************************************

int32_t BufferPop(unsigned long aIoctlParam)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  SEQ_FrmBufferInfo_t lBuffInfo;
      
  if (copy_from_user( &lBuffInfo, (SEQ_FrmBufferInfo_t *)aIoctlParam, 
      sizeof(SEQ_FrmBufferInfo_t)) != 0)
  {
    VDB_LOG_ERROR("Copy from user failed.\n");
    lRet = -EIO;
  } // if copy from user failed
  else
  { 
    // Pop the buffer
    if(SEQ_DRV_FrmBufferPop(&lBuffInfo) != SEQ_DRV_SUCCESS)
    {
      VDB_LOG_NOTE("Frame buffers query failed.\n");
      lRet = SEQ_DRV_FAILURE;
    }
    
    // copy BufferInfo back to user
    if (copy_to_user((SEQ_FrmBufferInfo_t *)aIoctlParam, &lBuffInfo, 
        sizeof(SEQ_FrmBufferInfo_t)) != 0)
    {
      VDB_LOG_ERROR("Failed to copy the buffer info to user space.\n");
      lRet = -EIO;
    } // if copy to user failed
  } // else from if copy from user failed
  
  return lRet;
} // BufferPush()

//*****************************************************************************

int32_t GraphStart(unsigned long aIoctlParam)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  SEQ_StartCfg_t lStartCfg = {0};
  
  if (copy_from_user( &lStartCfg, (SEQ_StartCfg_t *)aIoctlParam, 
      sizeof(SEQ_StartCfg_t)) != 0)
  {
    VDB_LOG_ERROR("Copy from user failed.\n");
    lRet = -EIO;
  } // if copy from user failed
  else
  {
    lRet = SEQ_DRV_GraphStart(lStartCfg.mFrames2Grab, lStartCfg.mInputLines);
    if( lRet != SEQ_DRV_SUCCESS)
    {
      VDB_LOG_ERROR("Frame buffers request failed.\n");
    } // if start failed
  } // else from if copy from user failed
  
  return lRet;
} // GraphStart()

//*****************************************************************************

int32_t HistogramGet(unsigned long aIoctlParam)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  SEQ_IpuHistHead_t lHist;
  
  if (copy_from_user( &lHist, (uint8_t*)aIoctlParam, 
      sizeof(SEQ_IpuHistHead_t)) != 0)
  {
    VDB_LOG_ERROR("Copy from user failed.\n");
    lRet = -EIO;
  } // if copy from user failed
  else
  { 
    
    lRet = SEQ_DRV_IpuHistogramGet(&lHist);
    
    // copy peak & peak address to user
    if (copy_to_user((uint8_t*)aIoctlParam, &lHist, 
        sizeof(uint32_t) * 3) != 0)
    {
      VDB_LOG_ERROR("Failed to copy hidtogram peak info to user.\n");
      lRet = -EIO;
    } // if copy to user failed        
  } // else from if copy from user failed
  return lRet;
} // HistogramGet()

//*****************************************************************************

int32_t StatGet(unsigned long aIoctlParam)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  SEQ_IpuStatHead_t lStat;
  
  if (copy_from_user( &lStat, 
                      (uint8_t*)aIoctlParam, 
                      sizeof(SEQ_IpuStatHead_t)) != 0)
  {
    VDB_LOG_ERROR("Copy from user failed.\n");
    lRet = -EIO;
  } // if copy from user failed
  else
  { 
    lRet = SEQ_DRV_IpuStatGet(&lStat);
    
    // copy structure content
    if (copy_to_user((uint8_t*)aIoctlParam, 
                     &lStat, 
                     sizeof(uint32_t)) != 0)
    {
      VDB_LOG_ERROR("Failed to copy hidtogram peak info to user.\n");
      lRet = -EIO;
    } // if copy to user failed        
  } // else from if copy from user failed
  return lRet;
} // StatGet()

//*****************************************************************************

int32_t PatchListSet(unsigned long aIoctlParam)
{
  int32_t lRet = SEQ_DRV_SUCCESS;
  SEQ_PatchLIstArr_t lPatchListArr;
  
  if (copy_from_user( &lPatchListArr, (uint8_t*)aIoctlParam, 
      sizeof(SEQ_PatchLIstArr_t)) != 0)
  {
    VDB_LOG_ERROR("Copy from user failed.\n");
    lRet = -EIO;
  } // if copy from user failed
  else
  { 
    SEQ_patch_list_t lpPatchList[SEQ_PATCH_LIST_NUM];
    // check if not too many patch lists
    if(lPatchListArr.mPatchListNum > SEQ_PATCH_LIST_NUM)
    {
      VDB_LOG_ERROR("Patch list too long. (%u/%u given/allowed)\n", 
                    lPatchListArr.mPatchListNum,
                    SEQ_PATCH_LIST_NUM
                   );
      lRet = SEQ_DRV_FAILURE;;
    } // if patch list num too big
    else
    {
      // copy peak & peak address to user
      if (copy_from_user( (uint8_t*)lpPatchList, 
                          (uint8_t*)lPatchListArr.mpPatchListArr, 
                          sizeof(SEQ_patch_list_t) * lPatchListArr.mPatchListNum
                        ) != 0)
      {
        VDB_LOG_ERROR("Failed to copy hidtogram peak info to user.\n");
        lRet = -EIO;
      } // if copy to user failed   
      else
      {
        // apply the patch list
        SEQ_DRV_PatchListSet(lpPatchList, lPatchListArr.mPatchListNum); 
      } // else from if copy from user failed
    } // else from if patch list num too big
  } // else from if copy from user failed
  return lRet;
} // PatchListSet()

//*****************************************************************************