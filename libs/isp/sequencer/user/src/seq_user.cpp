/*****************************************************************************
 *
 * Freescale Confidential Proprietary
 *
 * Copyright (c) 2016 Freescale Semiconductor;
 * Copyright (c) 2016-17 NXP;
 * All Rights Reserved
 *
 *****************************************************************************
 *
 * THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
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
 * @file     seq_user.cpp
 * @brief    User-space library calls to access Sequencer driver functionality.
 ****************************************************************************/

/*=============================================================================
Revision History:
                        Modification       Tracking 
Author (core ID)        Date D/M/Y         Number     Description of Changes
Tomas Babinec           09-Nov-2014                   Init version
Tomas Babinec           05-Jun-2017        VSDK-572   Traceability.
=============================================================================*/

#include "seq_hw.h"

#include <stdint.h>
#include <errno.h>
#ifndef __STANDALONE__
  #include <string.h>
  #include <signal.h>
  #include <fcntl.h>
  #include <unistd.h>
  #include <sys/ioctl.h>
  #include <sys/mman.h>
#else
  #include "seq_func.h"
#endif // else from #ifndef __STANDALONE__

#define VDB_LOG_NOTE_OFF
#include "vdb_log.h"
#include "isp_seq.h"

#define VSEQ_CTRL_BLK_HW_ADDR (0x7c021000)
#define VSEQ_EVT_CTRL_HW_ADDR (0x7c022000)
#define VSEQ_IPUS_0_HW_ADDR   (0x7c042000)
#define VSEQ_IPUS_1_HW_ADDR   (0x7c043000)
#define VSEQ_IPUS_2_HW_ADDR   (0x7c044000)
#define VSEQ_IPUS_3_HW_ADDR   (0x7c045000)
#define VSEQ_IPUS_4_HW_ADDR   (0x7c046000)
#define VSEQ_IPUS_5_HW_ADDR   (0x7c047000)
#define VSEQ_IPUS_6_HW_ADDR   (0x7c048000)
#define VSEQ_IPUS_7_HW_ADDR   (0x7c049000)
#define VSEQ_IPUV_0_HW_ADDR   (0x7c062000)
#define VSEQ_IPUV_1_HW_ADDR   (0x7c063000)
#define VSEQ_IPUV_2_HW_ADDR   (0x7c064000)
#define VSEQ_IPUV_3_HW_ADDR   (0x7c065000)

#define IPUS_REG_SIZE (IPUS_CNT * (((uintptr_t)&IPUS_1) - ((uintptr_t)&IPUS_0)))
#define IPUV_REG_SIZE (IPUV_CNT * (((uintptr_t)&IPUV_1) - ((uintptr_t)&IPUV_0)))

//****************************************************************************
// types
//****************************************************************************

typedef struct seq_fw_arr
{
  const char  *mpFwData;
  uint32_t    mIndex;
} SEQ_FwArr_t;

//****************************************************************************
// global variables
//****************************************************************************


//*****************************************************************************
// local data
//*****************************************************************************

static SEQ_EventHandler_t sEventHandler = NULL; ///< user registerred event handler
static SEQ_EventHandlerClass *spEventHandlerClass = NULL; ///< user registerred event handler
static uint8_t sInitialized = 0;	///< >0 if initialized

static volatile struct IPUS_tag *spIpus = NULL;  // pointer to IPUS reg region
static volatile struct IPUV_tag *spIpuv = NULL;  // pointer to IPUV reg region

#ifndef __STANDALONE__
static int32_t sSeqFD = -1;   ///< Sequencer driver file descriptor

//****************************************************************************
// local function declarations
//****************************************************************************
void SeqSigHandler(int aSigNum, siginfo_t *info, void *unused);
#else //#ifndef __STANDALONE__
extern "C" {
  void SeqSigHandler(int aSigNum, int aValue);
}

// init Sequencer driver info
static SEQ_DrvInfo_t sSeqDrvInfo = 
{
  .mpSeqCBregs = (SEQ_CtrlBlkRegs_t*)VSEQ_CTRL_BLK_HW_ADDR, 
  .mpSeqECregs = (SEQ_EvtCtrlRegs_t*)VSEQ_EVT_CTRL_HW_ADDR, 
  .mpCram      = (uint8_t*)(uintptr_t)CRAM_BASE,
  .mpDbgPram   = (uint8_t*)(uintptr_t)DBG_PRAM_BASE, 
  .mpPram      = (uint8_t*)(uintptr_t)PRAM_BASE,
  .mpKram      = (uint8_t*)(uintptr_t)KRAM_BASE,
  .mppIpusRegs = {(SEQ_IpusRegs_t*)VSEQ_IPUS_0_HW_ADDR,
                  (SEQ_IpusRegs_t*)VSEQ_IPUS_1_HW_ADDR,
                  (SEQ_IpusRegs_t*)VSEQ_IPUS_2_HW_ADDR,
                  (SEQ_IpusRegs_t*)VSEQ_IPUS_3_HW_ADDR,
                  (SEQ_IpusRegs_t*)VSEQ_IPUS_4_HW_ADDR,
                  (SEQ_IpusRegs_t*)VSEQ_IPUS_5_HW_ADDR,
                  (SEQ_IpusRegs_t*)VSEQ_IPUS_6_HW_ADDR,
                  (SEQ_IpusRegs_t*)VSEQ_IPUS_7_HW_ADDR},
  .mppIpuvRegs = {(SEQ_IpuvRegs_t*)VSEQ_IPUV_0_HW_ADDR,
                  (SEQ_IpuvRegs_t*)VSEQ_IPUV_1_HW_ADDR,
                  (SEQ_IpuvRegs_t*)VSEQ_IPUV_2_HW_ADDR,
                  (SEQ_IpuvRegs_t*)VSEQ_IPUV_3_HW_ADDR},
  .mIrqNums    = {SEQ_IRQ_MSG,SEQ_IRQ_MSG_ACK}, //IrqNums
  .mpCommInfo  = NULL,
  .mpSigInvoke = &SeqSigHandler,  
}; // sSeqDrvInfo 

#endif //else from #ifndef __STANDALONE__

static uint32_t checkIpuRange(uint16_t aIsIPUS, uint16_t aEng);
static SEQ_LIB_RESULT IpuRegsMap(void);
static void IpuRegsUnmap(void);

//*****************************************************************************
// functions definitions
//*****************************************************************************
/****************************************************************************/
/** 
 * @brief Reserves the Sequencer usage for this process.
 * @details In Linux the seq special device file is opened.
 *          In standalone environment the SEQ_SetUp() function is called 
 *          directly to initialize the internal driver structures and 
 *          Sequencer HW.
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if opening of the device file fails
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
 * @implements SEQ_Reserve_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_Reserve()
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__  
  if(sSeqFD == -1)
  {  
    sSeqFD = open("/dev/seq", 0);
    if (sSeqFD == -1) 
    {
      VDB_LOG_ERROR("Can't open device file: %s\n", SEQ_DEVICE_NAME);
      lRet = SEQ_LIB_FAILURE;
    } // if open failed
  } // if not opened yet
  else 
  {
    VDB_LOG_WARNING("Device file: %s opened already.\n", SEQ_DEVICE_NAME);
  } // else from if not opened yet
#else
  // *** Setup the driver ***     
  if(SEQ_DRV_Setup(&sSeqDrvInfo) != SEQ_DRV_SUCCESS)
  {
    lRet = SEQ_LIB_FAILURE;
    VDB_LOG_ERROR("SEQ setup failed.\n");
  } // if driver setup failed
#endif // #ifndef __STANDALONE__
  
  return lRet;
} // SEQ_Reserve()

/****************************************************************************/
/** 
 * @brief Releases the Sequencer usage which was locked to this process.
 * @details In Linux the seq special device file is closed.
 *          In standalone environment the FDMA_Close() function is called 
 *          directly to release internal driver resources and disconnect 
 *          from the HW.
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if the close fails
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
 * @implements SEQ_Release_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_Release()
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;
#ifndef __STANDALONE__  
  if(sSeqFD != -1)
  {
    if (close(sSeqFD) != 0) 
    {
      VDB_LOG_ERROR("Failed to close device file %s.\n", SEQ_DEVICE_NAME);
      lRet = SEQ_LIB_FAILURE;
    } // if close failed
    else
    {
      sSeqFD = -1;
    } // else if close failed
  } // not openned yet
  else 
  {
    VDB_LOG_WARNING("Sequencer already closed.\n");
  } // else from if not openned yet
  
  IpuRegsUnmap();
#else // #ifndef __STANDALONE__ 

  // relase resources
  if(SEQ_DRV_Close() != SEQ_DRV_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to close Sequecer device.\n");
    lRet = SEQ_LIB_FAILURE;
  } // if SEQ_DRV_Close() failed
  
#endif // else from #ifndef __STANDALONE__ 
  return lRet;
} // SEQ_Release()

/****************************************************************************/
/** 
 * @brief Boots up the Sequencer firmware on M0+ core.
 * @details Boots up the Sequencer firmware on M0+ core.
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_Boot_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_Boot()
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__    
  int32_t lRetVal = 0;
  if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_BOOT)) < 0) 
  {
    VDB_LOG_ERROR("Failed to BOOT the Sequencer.\n");
    lRet = SEQ_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__  
  if(SEQ_DRV_Boot() == SEQ_DRV_SUCCESS)
  {
    VDB_LOG_NOTE("Sequencer booted.\n");
  }
  else
  {
    VDB_LOG_ERROR("Sequencer boot failed.\n");
    lRet = SEQ_LIB_FAILURE;
  }
#endif // else from #ifndef __STANDALONE__  
     
  return lRet;
} // SEQ_Boot()

/****************************************************************************/
/** 
 * @brief Resets the Sequencer HW & driver data.
 * @details Resets the Sequencer HW & driver data.
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_Reset_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_Reset()
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__   
  int32_t lRetVal = 0;
  if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_RESET)) < 0) 
  {
    VDB_LOG_ERROR("Failed to RESET the Sequencer.\n");
    lRet = SEQ_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__ 
  SEQ_DRV_Reset();
#endif // else from #ifndef __STANDALONE__ 
     
  return lRet;
} // SEQ_Reset()

/****************************************************************************/
/** 
 * @brief Sends the specified message to Sequencer.
 * @details Sends the specified message to Sequencer.
 * 
 * @param apMessage[in] message to be sent
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_MessageSend_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_MessageSend(SEQ_Message_t *apMessage, uint8_t aWait)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__  
  if(apMessage != NULL)
  {
    int32_t lRetVal = 0;
    if ((lRetVal = ioctl(sSeqFD, (aWait == 0)?IOCTL_SEQ_MSG_SEND:IOCTL_SEQ_MSG_SEND_WAIT, apMessage)) < 0) 
    {
      VDB_LOG_ERROR("Failed to send the message: %d, %u\n", lRetVal, IOCTL_SEQ_MSG_SEND);
      lRet = SEQ_LIB_FAILURE;
    } // if ioctl returned failure
  }
  else
  {
    VDB_LOG_ERROR("Message was a NULL pointer.\n");
    lRet = SEQ_LIB_FAILURE;
  }
#else // #ifndef __STANDALONE__
  
  VDB_LOG_NOTE("Sending a message.\n");
  if(SEQ_DRV_MsgSend(apMessage, aWait) == SEQ_DRV_SUCCESS)
  {
    VDB_LOG_NOTE("The message was sent\n");
  }
  else
  {
    lRet = SEQ_LIB_FAILURE;
    VDB_LOG_NOTE("Failed to send the message.\n");
  }

#endif // else #ifndef __STANDALONE__
  
  return lRet;
} // SEQ_MessageSend()

/****************************************************************************/
/** 
 * @brief Gets HW address of the Graph in PRAM.
 * @details Intended for graph pointers update before the download.
 * 
 * @param apGraphAddress[out] next download address
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_GraphAddressGet_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_GraphAddressGet(uintptr_t *apGraphAddress)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;
  
  *apGraphAddress = SEQ_GRAPH_ADDR;
  
  return lRet;
} // SEQ_GraphAddressGet()

/****************************************************************************/
/** 
 * @brief Gets auxiliary message data from PRAM.
 * @details Gets auxiliary message data from PRAM.
 * 
 * @param apData[out]  pointer to user allocated buffer
 * @param aSize        number of aux data (in bytes) to get
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_MsgAuxDataGet_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_MsgAuxDataGet(void* apData, uint32_t aSize)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__  
  if((apData != NULL) && (aSize > 0))
  {
    if(sSeqFD == -1)
    {
      VDB_LOG_ERROR("Sequencer device file not opened.\n");
      lRet = SEQ_LIB_FAILURE;
    } // if Sequencer not opened
    else
    {
      int32_t lRetVal = 0;
      SEQ_AuxData_t lAuxData = {apData, aSize};
      
      if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_AUX_DATA_GET, &lAuxData)) < 0) 
      {
        VDB_LOG_ERROR("Auxiliary data get failed: %d\n", lRetVal);
        lRet = SEQ_LIB_FAILURE;
      } // if ioctl returned failure
    } // else from if Sequencer not opened
  }// if the Graph package exists
  else
  {
    VDB_LOG_WARNING("Invalid pointer or size.\n");
  }
#else // #ifndef __STANDALONE__
  SEQ_AuxData_t lTmpData = {apData, aSize};
  
  SEQ_DRV_PramAuxDataGet(&lTmpData);
  memcpy(apData, lTmpData.mpData, lTmpData.mSize);
  aSize = lTmpData.mSize;
#endif // else #ifndef __STANDALONE__
  
  return lRet;
} // SEQ_GraphAddressGet()

/****************************************************************************/
/** 
 * @brief Copies given register list to proper PRAM location.
 * @details Copies given register list to proper PRAM location.
 * 
 * @param apRegList  pointer to register list structure
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_RegListSet_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_RegListSet(SEQ_RegList_t* apRegList)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__  
  if(apRegList != NULL)
  {
    if(sSeqFD < 0)
    {
      VDB_LOG_ERROR("Sequencer device file not opened.\n");
      lRet = SEQ_LIB_FAILURE;
    } // if Sequencer not opened
    else
    {
      int32_t lRetVal = 0;
      
      if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_REGLIST_SET, apRegList)) < 0) 
      {
        VDB_LOG_ERROR("Failed to set register list: %d\n", lRetVal);
        lRet = SEQ_LIB_FAILURE;
      } // if ioctl returned failure
    } // else from if Sequencer not opened
  }// if the Graph package exists
  else
  {
    VDB_LOG_WARNING("Invalid pointer.\n");
  }
#else // #ifndef __STANDALONE__
  
  if(SEQ_DRV_RegListSet(apRegList) != SEQ_DRV_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to set register list\n");
    lRet = SEQ_LIB_FAILURE;
  } // if failed to set reg list
#endif // else #ifndef __STANDALONE__
  
  return lRet;
} // SEQ_RegListSet()

/****************************************************************************/
/** 
 * @brief Copies register list from proper PRAM location.
 * @details Copies given register list to proper PRAM location.
 * 
 * @param apRegList  pointer to register list structure
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_RegListGet_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_RegListGet(SEQ_RegList_t* apRegList)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__
  if(apRegList != NULL)
  {
    if(sSeqFD < 0)
    {
      VDB_LOG_ERROR("Sequencer device file not opened.\n");
      lRet = SEQ_LIB_FAILURE;
    } // if Sequencer not opened
    else
    {
      int32_t lRetVal = 0;
      
      if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_REGLIST_GET, apRegList)) < 0) 
      {
        VDB_LOG_ERROR("Failed to read register list: %d\n", lRetVal);
        lRet = SEQ_LIB_FAILURE;
      } // if ioctl returned failure
    } // else from if Sequencer not opened
  }// if the Graph package exists
  else
  {
    VDB_LOG_WARNING("Invalid pointer.\n");
  }
#else // #ifndef __STANDALONE__
  
  if(SEQ_DRV_RegListGet(apRegList) != SEQ_DRV_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to get register list\n");
    lRet = SEQ_LIB_FAILURE;
  } // if failed to get reg list
#endif // else #ifndef __STANDALONE__
  
  return lRet;
} // SEQ_RegListGet()

/****************************************************************************/
/** 
 * @brief Downloads the graph to the PRAM.
 * @details Requires the sequencer to be reserved.
 * 
 * @param apGraphPackage packed graph structure
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_GraphDownload_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_GraphDownload(SEQ_GraphPackage_t *apGraphPackage)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__  
  if(apGraphPackage != NULL)
  {
    if(sSeqFD == -1)
    {
      VDB_LOG_ERROR("Sequencer device file not opened.\n");
      lRet = SEQ_LIB_FAILURE;
    } // if Sequencer not opened
    else
    {
      int32_t lRetVal = 0;
      if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_GRAPH_DOWNLOAD, apGraphPackage)) < 0) 
      {
        VDB_LOG_ERROR("Graph download failed: %d\n", lRetVal);
        lRet = SEQ_LIB_FAILURE;
      } // if ioctl returned failure
    } // else from if Sequencer not opened
  }// if the Graph package exists
#else // #ifndef __STANDALONE__
  if(SEQ_DRV_GraphDownload(apGraphPackage) != SEQ_DRV_SUCCESS)
  {
    VDB_LOG_ERROR("Graph download failed.\n");
    lRet = SEQ_LIB_FAILURE;
  } // if GraphDownload failed
#endif // else #ifndef __STANDALONE__
  
  return lRet;
} // SEQ_GraphDownload(SEQ_GraphPackage_t *apGraphPackage)

/****************************************************************************/
/** 
 * @brief Fetches current state of the graph from the PRAM.
 * @details Requires the sequencer to be reserved.
 * 
 * @param apGraphPackage packed graph structure
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_GraphFetch_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_GraphFetch(SEQ_GraphPackage_t *apGraphPackage)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__
  if(apGraphPackage != NULL)
  {
    if(sSeqFD == -1)
    {
      VDB_LOG_ERROR("Sequencer device file not opened.\n");
      lRet = SEQ_LIB_FAILURE;
    } // if Sequencer not opened
    else
    {
      int32_t lRetVal = 0;
      if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_GRAPH_FETCH, apGraphPackage)) < 0) 
      {
        VDB_LOG_ERROR("Graph fetch failed: %d\n", lRetVal);
        lRet = SEQ_LIB_FAILURE;
      } // if ioctl returned failure
    } // else from if Sequencer not opened
  }// if the Graph package exists
#else // #ifndef __STANDALONE__
  if(SEQ_DRV_GraphFetch(apGraphPackage) != SEQ_DRV_SUCCESS)
  {
    VDB_LOG_ERROR("Graph download failed.\n");
    lRet = SEQ_LIB_FAILURE;
  } // if GraphFetch failed
#endif // else #ifndef __STANDALONE__
  
  return lRet;
} // SEQ_GraphFetch(SEQ_GraphPackage_t *apGraphPackage)

/****************************************************************************/
/** 
 * @brief Requests frame buffers for one frame stream.
 * @details Based on data given in "Request" structure prepares specified stream 
 *          and allocates framebuffer structures. Addresses for buffer data have 
 *          to be included in the request structure.
 *          Request for 0 buffer count makes the previous stream setup to be 
 *          released.
 * 
 * @param apRequest defines request details
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_FrmBuffersRequest_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_FrmBuffersRequest(SEQ_BufferRequest_t *apRequest)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

  if(apRequest != NULL)
  { 
#ifndef __STANDALONE__   
    int32_t lRetVal = 0;
    if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_BUF_REQ, apRequest)) < 0) 
    {
      VDB_LOG_ERROR("Frame buffers' requset failed: %d\n", lRetVal);
      lRet = SEQ_LIB_FAILURE;
    } // if ioctl returned failure
  
#else // #ifndef __STANDALONE__
    if(SEQ_DRV_FrmBuffersRequest(apRequest) != SEQ_DRV_SUCCESS)
    {
        VDB_LOG_ERROR("Frame buffers' request failed.\n");
        lRet = SEQ_LIB_FAILURE;
    } // if request failed
#endif // else #ifndef __STANDALONE__
  }// if Request structure exists
  
  return lRet;
} // SEQ_FrmBuffersRequest(SEQ_BufferList_t *apBuffsList)

/****************************************************************************/
/** 
 * @brief Queries buffer at given location.
 * @details Based on buffer location specified by the stream and buffer index in 
 *          BufferInfo structure the status and size of the buffer is updated in 
 *          the structure.
 * 
 * @param apBufferInfo defines request details
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_FrmBufferQuery_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_FrmBufferQuery(SEQ_FrmBufferInfo_t *apBuffInfo)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

  if(apBuffInfo != NULL)
  { 
#ifndef __STANDALONE__   
    int32_t lRetVal = 0;
    if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_BUF_QRY, apBuffInfo)) < 0) 
    {
      VDB_LOG_WARNING("Frame buffer query failed: %d\n", lRetVal);
      lRet = SEQ_LIB_FAILURE;
    } // if ioctl returned failure
  
#else // #ifndef __STANDALONE__
    if(SEQ_DRV_FrmBufferQuery(apBuffInfo) != SEQ_DRV_SUCCESS)
    {
        VDB_LOG_WARNING("Frame buffer query failed.\n");
        lRet = SEQ_LIB_FAILURE;
    } // if query failed
#endif // else #ifndef __STANDALONE__
  }// if Info structure exists
  
  return lRet;
} // SEQ_FrmBufferQuery(SEQ_FrmBufferInfo_t *apBuffInfo)

/****************************************************************************/
/** 
 * @brief Gets current state of temporal frame statistics. 
 * @details In Linux the fdma special device file is opened.
 *          In standalone environment the FDMA_SetUp() function is called 
 *          directly to initialize the internal driver structures and HW 
 *          registers.
 * 
 * @param apTimeStats buffer identification (stream + buffer index).
 * 
 * @return FDMA_LIB_SUCCESS if all ok
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
 * @implements SEQ_TimeStatsGet_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_TimeStatsGet(SEQ_FrmTemporalStat_t& arTimeStats)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__  
  int32_t lRetVal = (int32_t)0;
  if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_TIME_STAT, &arTimeStats)) < (int32_t)0) 
  {
    VDB_LOG_ERROR("Failed to read temporal statistics: %d\n", lRetVal);
    lRet = SEQ_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__
  arTimeStats = SEQ_DRV_TimeStatsGet();
    
#endif // else #ifndef __STANDALONE__
  
  return lRet;
} // SEQ_TimeStatsGet()

/****************************************************************************/
/** 
 * @brief Adds frame buffer to work queue.
 * @details Performs checks if buffer is available to user.
 * 
 * @param apBuffInfo buffer identification (stream + buffer index).
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_FrmBufferPush_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_FrmBufferPush(SEQ_FrmBufferInfo_t *apBuffInfo)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

  if(apBuffInfo != NULL)
  {  
#ifndef __STANDALONE__  
    int32_t lRetVal = 0;
    if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_BUF_PSH, apBuffInfo)) < 0) 
    {
      VDB_LOG_ERROR("Push failed: %d\n", lRetVal);
      lRet = SEQ_LIB_FAILURE;
    } // if ioctl returned failure
#else // #ifndef __STANDALONE__
    if(SEQ_DRV_FrmBufferPush(apBuffInfo) != SEQ_DRV_SUCCESS)
    {
        VDB_LOG_ERROR("Push failed.\n");
        lRet = SEQ_LIB_FAILURE;
    } // if Push failed
#endif // else #ifndef __STANDALONE__
  }// if Info structure exists 
  
  return lRet;
} // SEQ_FrmBufferPush(SEQ_FrmBufferInfo_t *apBuffInfo)

/****************************************************************************/
/** 
 * @brief Removes frame buffer from DONE queue.
 * @details Performs checks if buffer is available to user.
 * 
 * @param apBufferInfo buffer identification (stream + buffer index).
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_FrmBufferPop_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_FrmBufferPop(SEQ_FrmBufferInfo_t *apBuffInfo)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

  if(apBuffInfo != NULL)
  {  
#ifndef __STANDALONE__  
    int32_t lRetVal = 0;
    if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_BUF_POP, apBuffInfo)) < 0) 
    {
      lRet = SEQ_LIB_FAILURE;
    } // if ioctl returned failure
#else // #ifndef __STANDALONE__
    if(SEQ_DRV_FrmBufferPop(apBuffInfo) != SEQ_DRV_SUCCESS)
    {
      lRet = SEQ_LIB_FAILURE;
    } // if Pop failed
#endif // else #ifndef __STANDALONE__
  }// if Info structure exists 
  
  return lRet;
} // SEQ_FrmBufferPop(SEQ_FrmBufferInfo_t *apBuffInfo)

/****************************************************************************/
/** 
 * @brief Invokes the execution of the graph.
 * @details Performs checks if buffer is available to user.
 * 
 * @param aFrameCnt number of frames to be processed
 * @param aInputLines number of lines per input frame
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_GraphStart_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_GraphStart(uint32_t aFrameCnt, uint32_t aInputLines)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__  
  int32_t lRetVal = 0;
  SEQ_StartCfg_t lStartCfg = {aFrameCnt, aInputLines};
  if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_START, &lStartCfg)) < 0) 
  {
    VDB_LOG_ERROR("Graph START failed: %d\n", lRetVal);
    lRet = SEQ_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__
  SEQ_DRV_GraphStart(aFrameCnt, aInputLines);
#endif // else #ifndef __STANDALONE__
  
  return lRet;
} // SEQ_GraphStart(uint32_t aFrameCnt)

/****************************************************************************/
/** 
 * @brief Requests a graceful stop of the currently executed graph.
 * @details Requests a graceful stop of the currently executed graph.
 * 
 * @param aWait if set to zero function will block till graph stopped
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_GraphStop_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_GraphStop(uint8_t aWait)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__  
  int32_t lRetVal = 0;
  if ((lRetVal = ioctl(sSeqFD, (aWait == 0)?IOCTL_SEQ_STOP:IOCTL_SEQ_STOP_WAIT)) < 0) 
  {
    VDB_LOG_ERROR("Graph STOP failed: %d\n", lRetVal);
    lRet = SEQ_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__
  if(SEQ_DRV_GraphStop(aWait) == SEQ_DRV_SUCCESS)
  {  
    if(aWait == 1)
    {
      while(SEQ_DRV_GraphStateGet() == 0)
      {
      }; // while not stopped
    } // if wait required
  } // if GraphStop ok
  else
  {
    lRet = SEQ_LIB_FAILURE;
  } // else from if GraphStop ok
#endif // else #ifndef __STANDALONE__
    
  return lRet;
} // SEQ_GraphStop()

/****************************************************************************/
/** 
 * @brief Request IPU profiling to be executed for next N frames.
 * @details Request IPU profiling to be executed for next N frames.
 * 
 * @param aFrmCnt number of frames to let the profiling to be on
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_IpuPerformanceInfoRequest(uint32_t aFrmCnt)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__  
  int32_t lRetVal = 0;
  if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_IPU_PROFILE_REQ, aFrmCnt)) < 0) 
  {
    VDB_LOG_ERROR("IPU profiling info request failed: %d\n", lRetVal);
    lRet = SEQ_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__
  if(SEQ_DRV_IpuPerformanceInfoRequest(aFrmCnt) != SEQ_DRV_SUCCESS)
  {
    lRet = SEQ_LIB_FAILURE;
  } // if failed
#endif // else #ifndef __STANDALONE__
    
  return lRet;
} // SEQ_IpuPerformanceInfoRequest()

/****************************************************************************/
/** 
 * @brief Fetches IPU performance info from registers.
 * @details Fetches IPU performance info from registers.
 * 
 * @param apIpuRegs pointer to array of IPU performance register structures
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_IpuPerformanceInfoGet_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_IpuPerformanceInfoGet(SEQ_IpuPerformanceRegs_t *apIpuRegs)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__  
  int32_t lRetVal = 0;
  if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_IPU_PROFILE_GET, apIpuRegs)) < 0) 
  {
    VDB_LOG_ERROR("IPU profiling info fetch failed: %d\n", lRetVal);
    lRet = SEQ_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__
  if(SEQ_DRV_IpuPerformanceInfoGet(apIpuRegs) != SEQ_DRV_SUCCESS)
  {
    lRet = SEQ_LIB_FAILURE;
  } // if failed
#endif // else #ifndef __STANDALONE__
    
  return lRet;
} // SEQ_IpuPerformanceInfoGet()

//****************************************************************************

SEQ_LIB_RESULT SEQ_EventLogEnable()
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__  
  lRet = SEQ_LIB_FAILURE;
#else // #ifndef __STANDALONE__
  if(SEQ_DRV_EventLogEnable() != SEQ_DRV_SUCCESS)
  {
    lRet = SEQ_LIB_FAILURE;
  } // if failed
#endif // else #ifndef __STANDALONE__
    
  return lRet;
} // SEQ_EventLogEnable()

//****************************************************************************

SEQ_LIB_RESULT SEQ_EventLogDisable()
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__  
  lRet = SEQ_LIB_FAILURE;
#else // #ifndef __STANDALONE__
  if(SEQ_DRV_EventLogDisable() != SEQ_DRV_SUCCESS)
  {
    lRet = SEQ_LIB_FAILURE;
  } // if failed
#endif // else #ifndef __STANDALONE__
    
  return lRet;
} // SEQ_EventLogDisable()

//****************************************************************************

SEQ_LIB_RESULT SEQ_EventLogGet(SEQ_EventLog_t **appEventLog)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__  
  appEventLog[0][0].mNextRecIdx = 0;
  lRet = SEQ_LIB_FAILURE;
#else // #ifndef __STANDALONE__
  if(SEQ_DRV_EventLogGet(appEventLog) != SEQ_DRV_SUCCESS)
  {
    lRet = SEQ_LIB_FAILURE;
  } // if failed
#endif // else #ifndef __STANDALONE__
    
  return lRet;
} // SEQ_EventLogGet()

/****************************************************************************/
/** 
 * @brief Enables histogram readout from particular IPU.
 * @details Enables histogram readout from particular IPU.
 * 
 * @param aIpuIdx IPU index
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_HistogramEnable_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_HistogramEnable(SEQ_IpuEngine_t aIpuIdx)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__  
  int32_t lRetVal = 0;
  if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_HIST_ENABLE, aIpuIdx)) < 0) 
  {
    VDB_LOG_ERROR("Histogram enable for IPU %d failed.\n", (uint32_t) aIpuIdx);
    lRet = SEQ_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__
  if(SEQ_DRV_IpuHistogramEnable(aIpuIdx) != SEQ_DRV_SUCCESS)
  {
    lRet = SEQ_LIB_FAILURE;
  } // if failed
#endif // else #ifndef __STANDALONE__
    
  return lRet;
} // SEQ_HistogramEnable()

/****************************************************************************/
/** 
 * @brief Gets histogram data from particular IPU.
 * @details Only IPU 0-3 are capable of generating histogram.
 * 
 * @param apHist pointer to histogram structure
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_HistogramGet_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_HistogramGet(SEQ_IpuHist_t *apHist)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;
  
  if(apHist != NULL)
  {
    SEQ_IpuHistHead_t lHist;
    lHist.mIpuIdx = apHist->mIpuIdx;
    lHist.mpData  = &(apHist->mpData[0]);
    
#ifndef __STANDALONE__  
    int32_t lRetVal = 0;
    if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_HIST_GET, &lHist)) < 0) 
    {
      VDB_LOG_ERROR("Histogram enable for IPU %d failed.\n", (uint32_t) apHist->mIpuIdx);
      lRet = SEQ_LIB_FAILURE;
    } // if ioctl returned failure
#else // #ifndef __STANDALONE__
    
    if(SEQ_DRV_IpuHistogramGet(&lHist) != SEQ_DRV_SUCCESS)
    {
      lRet = SEQ_LIB_FAILURE;
    } // if failed
#endif // else #ifndef __STANDALONE__
    apHist->mPeak      = lHist.mPeak;
    apHist->mPeakAddr  = lHist.mPeakAddr;
    apHist->mFrmSeqNum = lHist.mFrmSeqNum;
  } // if apHist exists
  
  return lRet;
} // SEQ_HistogramGet()

/****************************************************************************/
/** 
 * @brief Disables histogram readout from particular IPU.
 * @details Disables histogram readout from particular IPU.
 * 
 * @param aIpuIdx IPU index
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_HistogramDisable_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_HistogramDisable(SEQ_IpuEngine_t aIpuIdx)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__  
  int32_t lRetVal = 0;
  if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_HIST_DISABLE, aIpuIdx)) < 0) 
  {
    VDB_LOG_ERROR("Histogram disable for IPU %d failed.\n", (uint32_t) aIpuIdx);
    lRet = SEQ_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__
  SEQ_DRV_IpuHistogramDisable(aIpuIdx);
#endif // else #ifndef __STANDALONE__
    
  return lRet;
} // SEQ_HistogramEnable()

/****************************************************************************/
/** 
 * @brief Enables statistics readout from IPUS 7.
 * @details Enables statistics readout from IPUS 7.
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_StatEnable_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_StatEnable()
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__  
  int32_t lRetVal = 0;
  if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_STAT_ENABLE)) < 0) 
  {
    VDB_LOG_ERROR("Statistics enable for IPU 7 failed.\n");
    lRet = SEQ_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__
  if(SEQ_DRV_IpuStatEnable() != SEQ_DRV_SUCCESS)
  {
    lRet = SEQ_LIB_FAILURE;
  } // if failed
#endif // else #ifndef __STANDALONE__
    
  return lRet;
} // SEQ_StatEnable()

/****************************************************************************/
/** 
 * @brief Gets statistic data from IPUS 7.
 * @details Gets statistic data from IPUS 7.
 * 
 * @param apHist pointer to stistic structure
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_StatGet_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_StatGet(SEQ_IpuStat_t *apStat)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;
  
  if(apStat != NULL)
  {
    SEQ_IpuStatHead_t lStat;
    lStat.mpData  = &(apStat->mpData[0]);
#ifndef __STANDALONE__  
    int32_t lRetVal = 0;
    if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_STAT_GET, &lStat)) < 0) 
    {
      VDB_LOG_ERROR("Statistics enable for IPU 7 failed.\n");
      lRet = SEQ_LIB_FAILURE;
    } // if ioctl returned failure
#else // #ifndef __STANDALONE__
    
    if(SEQ_DRV_IpuStatGet(&lStat) != SEQ_DRV_SUCCESS)
    {
      lRet = SEQ_LIB_FAILURE;
    } // if failed
    apStat->mFrmSeqNum = lStat.mFrmSeqNum;
#endif // else #ifndef __STANDALONE__
  } // if apStat exists
  
  return lRet;
} // SEQ_StatGet()

/****************************************************************************/
/** 
 * @brief Disables statistics readout from IPUS 7.
 * @details Disables statistics readout from IPUS 7.
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_StatDisable_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_StatDisable()
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__  
  int32_t lRetVal = 0;
  if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_STAT_DISABLE)) < 0) 
  {
    VDB_LOG_ERROR("Statistics disable for IPU 7 failed.\n");
    lRet = SEQ_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__
  SEQ_DRV_IpuStatDisable();
#endif // else #ifndef __STANDALONE__
    
  return lRet;
} // SEQ_StatDisable()

//***************************************************************************
//DATA.SAVE.BINARY c:\isp_log.csv var.address(*spLogFile)++var.value(lLogSize)
uint32_t SEQ_EventLogProccess(SEQ_EventLog_t &arEventLog,
                              uint32_t aMaxFrames,
                              char* apString, 
                              size_t aStringSize)
{
#if (ISP_EVT_LOG_LEN > 0)
  unsigned long lStartTime = arEventLog.mpRecords[0].mTime;
  sprintf(apString, "IPUS,IPUV,OTHER,SEQ_FD,CSI_FE,SW_EVT,CSI_Line,FDMA_Line,REG,Time\r\n");
  //                   0     1    2      3      4     5       6         7      8    9
  //                   A     B    C      D      E     F       G         H      I    J
  char *lpLogFile = apString + strlen(apString);
  uint32_t lEventCounters[8] = {0};
  uint32_t lUnprocFrames = 0;
  uint32_t i = 0;
  
  if(aMaxFrames > (aStringSize / EVTLOG_STRING_SIZE))
  {
    aMaxFrames = (aStringSize / EVTLOG_STRING_SIZE);
  } // if too many frames requested
  
  for(i = 0; i < ISP_EVT_LOG_LEN; i++)
  {
    SEQ_EventRecord_t &lEventRec = arEventLog.mpRecords[i];
    
    if(lEventRec.mEvent.B.mType != ISP_LOGEVT_CSI_FE)
    {
      break;
    }    
        
    lEventCounters[lEventRec.mEvent.B.mType] ++;
    
    sprintf(lpLogFile, "0,0,0,0,%u,0,0,0,%08x,%lu\r\n",
            lEventCounters[ISP_LOGEVT_CSI_FE],
            lEventRec.mEvent.B.mRegister,
            lEventRec.mTime - lStartTime
            );
    
    // shift file pointer
    lpLogFile += strlen(lpLogFile);
  } // for all unprocessed frames
  
  lUnprocFrames = i;
  lStartTime = arEventLog.mpRecords[i].mTime;
  
  for(1; i < ISP_EVT_LOG_LEN;i++)
  {
    uint32_t lEventFlags[8] = {0};
    SEQ_EventRecord_t &lEventRec = arEventLog.mpRecords[i];
    
    if(lEventRec.mEvent.B.mType == ISP_LOGEVT_OTHER)
    {
      if((lEventRec.mEvent.B.mRegister & 0x00000400) > 0)
      { 
        lEventCounters[ISP_LOGEVT_CSI_LINE]++;
        lEventFlags[ISP_LOGEVT_CSI_LINE] = lEventCounters[ISP_LOGEVT_CSI_LINE];
      } // if CSI line done event
      
      if((lEventRec.mEvent.B.mRegister & 0x00000001) > 0)
      { 
        lEventCounters[ISP_LOGEVT_FDMA_LINE]++;
        lEventFlags[ISP_LOGEVT_FDMA_LINE] = lEventCounters[ISP_LOGEVT_FDMA_LINE];
      } // if FDMA done event
    } // if OTHER event
      
    lEventCounters[lEventRec.mEvent.B.mType] ++;
    lEventFlags[lEventRec.mEvent.B.mType] = lEventCounters[lEventRec.mEvent.B.mType];
    
    
    // *** print the event ***
    sprintf(lpLogFile, "%u,%u,%u,%u,%u,%u,%u,%u,%08x,%lu\r\n",
            lEventFlags[ISP_LOGEVT_IPUS],
            lEventFlags[ISP_LOGEVT_IPUV],
            lEventFlags[ISP_LOGEVT_OTHER],
            lEventFlags[ISP_LOGEVT_SEQ_FD],
            lEventFlags[ISP_LOGEVT_CSI_FE],
            lEventFlags[ISP_LOGEVT_SW_EVT],
            lEventFlags[ISP_LOGEVT_CSI_LINE],
            lEventFlags[ISP_LOGEVT_FDMA_LINE],
            lEventRec.mEvent.B.mRegister,
            lEventRec.mTime - lStartTime
            );
    
    // shift file pointer
    lpLogFile += strlen(lpLogFile);
    
    if(lEventCounters[ISP_LOGEVT_SEQ_FD] >= (aMaxFrames - 1 + lUnprocFrames))
    {
      break;
    } // if Maximum frames reached

  } // for all possible event
  return (lpLogFile - apString);
#else // #if (ISP_EVT_LOG_LEN > 0)
  arEventLog.mNextRecIdx = 0;
  aStringSize = aMaxFrames;
  if((apString != NULL) && (aStringSize != 0))
  {
    apString[0] = '\0';    
  }
  
  return 0;
#endif // else from #if (ISP_EVT_LOG_LEN > 0)
} // SEQ_EventLogProcess()

//****************************************************************************
//   Firmware handling
//****************************************************************************

static int FwArrGetc(SEQ_FwArr_t *apFwArr)
{
  int lRet = EOF;
  if( (apFwArr != NULL) && (apFwArr->mpFwData[apFwArr->mIndex] != '\0'))
  {
    lRet = apFwArr->mpFwData[apFwArr->mIndex++];
  } // if something to get
  return lRet;
} // FwArrGetc(SEQ_FwArr_t *apFwArr)

//****************************************************************************

static void ArrLineSkip(SEQ_FwArr_t *apFwArr, int32_t *apChar) 
{
  do
  { 
    *apChar = FwArrGetc(apFwArr); 
  } while((*apChar != 'S') && (*apChar != EOF));
  
  if(*apChar == 'S')
  {
    apFwArr->mIndex--;
    *apChar = '\n';
  }
} // LineSkip()

//****************************************************************************

static uint32_t ArrHexScan(SEQ_FwArr_t *apFwArr, uint8_t aBytes)
{
  int32_t lData;
  uint32_t lVal = 0;
  
  for(int i= 0; i< aBytes * 2; i++)
  {
    lVal <<= 4;
    lData = FwArrGetc(apFwArr);
    if(lData < 'A') 
    {
      lVal += lData - '0';
    }
    else
    {
      lVal += lData - 'A' + 10;
    }  
  } // for all bytes
    
  return lVal;
} // HexScan()

//****************************************************************************
//   Firmware handling
//****************************************************************************

/****************************************************************************/
/** 
 * @brief Downloads the specified firmware.
 * @details Downloads the specified firmware.
 * 
 * @param  apcFwArr path to the FW file in S-REC format.
 * @param  aFwType  type of the firmware (for CM0 or IPUx) to be downloaded also 
 *                  specifying destination ram (CRAM/KRAM). 
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_FwArrDownload_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_FwArrDownload(const char *apcFwArr, SEQ_FwType_t aFwType)
{
  SEQ_LIB_RESULT lRet      = SEQ_LIB_SUCCESS; 
  uint8_t       *lpMap     = NULL;
  uintptr_t      lMapBase  = 0;
  size_t         lMapBytes = 0;
  bool           lW32;
  SEQ_FwArr_t    lFwArr    = {apcFwArr, 0};
  
  if(aFwType == SEQ_FW_CM0)
  {
    lMapBase  = CRAM_BASE;
    lMapBytes = CRAM_SIZE;
    lW32      = false;
  } // if SEQ_FW_CM0
  else if(aFwType == SEQ_FW_KERNEL)
  {
    lMapBase  = KRAM_BASE;
    lMapBytes = KRAM_SIZE;
    lW32      = true;
  } // else if SEQ_FW_KERNEL
  else
  {
    lRet = SEQ_LIB_FAILURE;
    VDB_LOG_ERROR("Unknown firmware type specified.\n");
  } // else from if SEQ_FW_KERNEL
  
  if(lRet == SEQ_LIB_SUCCESS)
  {    
    if(apcFwArr == NULL)
    {
      VDB_LOG_ERROR("No FW data.\n");
      lRet = SEQ_LIB_FAILURE;
    } // if file open failed
    else
    {
      // open and map memory
      
#ifndef __STANDALONE__
      int32_t lFd = 0;

      lFd = open("/dev/mem", O_RDWR);

      if(lFd < 0)
      {
        VDB_LOG_ERROR("Failed to open /dev/mem\n");
        lRet = SEQ_LIB_FAILURE;
      } //if failed to open /dev/mem
      else
      {
        lpMap = (uint8_t*)mmap(NULL, lMapBytes, PROT_READ | PROT_WRITE, MAP_SHARED, lFd, lMapBase);

        if (lpMap == NULL) 
        {
          lRet = SEQ_LIB_FAILURE;
          VDB_LOG_ERROR("Can't map memory: %s\n", strerror(errno));
        }
        close(lFd);
      } // else from if failed to open /dev/mem
#else // #ifndef __STANDALONE__
      (void) lMapBytes; // to get rid of a compiler warning
      lpMap = (uint8_t*)lMapBase;
#endif // else from #ifndef __STANDALONE__     
    } // else from if no FW data
    
    if(lpMap != NULL)
    {
      // process the file and download the data
      int32_t   lChar = 0;
      uint32_t  lAddr = 0;
      uint32_t  j     = 0;
      uint32_t  lSize = 0;
      uint32_t *lpW32 = NULL;
      uint32_t  lBA;
      uint8_t  *lpBA  = (uint8_t*) &lBA;
      
      while(1)
      {
        j ++;
        lChar = FwArrGetc(&lFwArr);
        if(lChar != 'S')
        {
          // skip the rest of the line
          ArrLineSkip(&lFwArr, &lChar);
          VDB_LOG_NOTE("Not S at the beginning\n");
        } // if not starting with S
        else
        {
          // look for only 32 bits at the moment
          lChar = FwArrGetc(&lFwArr);
          size_t lAddrWidth=0;
          switch(lChar)
          {
                  case '1':	lAddrWidth = 2;
            break;
                  case '2':	lAddrWidth = 3;
            break;
                  case '3':	lAddrWidth = 4;
            break;
            default:	lAddrWidth = 0;
          } // switch address width
          
          if(0 == lAddrWidth)
          {
            // skip the rest of the line
            ArrLineSkip(&lFwArr, &lChar);
            VDB_LOG_NOTE("Not supported\n");
          } // if address width 0
          else
          {
            // read the num. of bytes
            lSize = ArrHexScan(&lFwArr, 1);

            // read the address
            lAddr = ArrHexScan(&lFwArr, lAddrWidth);

            // copy the data to SRAM
            uint32_t i  = 0;
            uint32_t ci = 0;
            bool     run;
            uint8_t  byte;
            
            lpW32 = (uint32_t*)((uintptr_t)lpMap + lAddr);
            
            do
            {
              byte = (uint8_t)ArrHexScan(&lFwArr, 1);
              
              if(!lW32)	// 8 bit accesses
              {
                lpMap[lAddr+i] = byte;
              }
              
              run  = (++i < (lSize - (lAddrWidth+1)));
              
              if(lW32)	// 32 bit accesses
              {
                lpBA[ci++] = byte;
              
                if(!run || (sizeof(lBA) == ci))
                {
                  *lpW32++ = lBA; 
                  ci = 0;
                }
              }
            }while(run);
            ArrLineSkip(&lFwArr, &lChar);
          } // else from if 
        } // else from if not starting with S

        if(lChar == EOF)
        {
          VDB_LOG_NOTE("Copied %d lines\n", (int) j);
          break;
        }
      } // for all lines     
#ifndef __STANDALONE__
      // unmap the memory
      munmap(lpMap, lMapBytes);
#endif //#ifndef __STANDALONE__
      lpMap = NULL;
    } // if memory mapped
  } // if all ok so far
  
  return lRet;  
} // SeqFwArrDownload(const char*, SEQ_FwType_t)

//****************************************************************************

SEQ_LIB_RESULT SEQ_FwArrDownload(const char *apcFwArr)
{
  return SEQ_FwArrDownload(apcFwArr, SEQ_FW_CM0);  
} // sdi::SeqFwArrDownload()

//****************************************************************************

SEQ_LIB_RESULT SEQ_FwFileDownload(const char *apFwPath, SEQ_FwType_t aFwType)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;
  
  FILE *lpFr = fopen(apFwPath, "r");
  if(lpFr == 0)
  {
    VDB_LOG_ERROR("Unable to open the sequencer FW file.\n");
    lRet = SEQ_LIB_FAILURE;
  } // if file open failed
  else
  {
    // Determine file size
    fseek(lpFr, 0, SEEK_END);
    size_t lSize = ftell(lpFr);

    // get the fw data
    char* lpFwArr = new char[lSize];
    rewind(lpFr);
    fread(lpFwArr, sizeof(char), lSize, lpFr);
    fclose(lpFr);

    // download data
    lRet = SEQ_FwArrDownload(lpFwArr, aFwType);
    delete[] lpFwArr;
  } // else from if file open failed
  
  return lRet;  
} // sdi::SeqFwFileDownload(const char*, SEQ_FwType_t)

//****************************************************************************

SEQ_LIB_RESULT SEQ_PatchListSet(SEQ_patch_list_t *apPatchListArr, 
                                uint32_t          aPatchListNum)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;
  
#ifndef __STANDALONE__    
  int32_t lRetVal = 0;
  SEQ_PatchLIstArr_t lPatchListArr = {apPatchListArr, aPatchListNum};
  if ((lRetVal = ioctl(sSeqFD, IOCTL_SEQ_PATCH, &lPatchListArr)) < 0) 
  {
    VDB_LOG_ERROR("Failed to setup patch list.\n");
    lRet = SEQ_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__  
  if(SEQ_DRV_PatchListSet(apPatchListArr, aPatchListNum) == SEQ_DRV_SUCCESS)
  {
    VDB_LOG_NOTE("Patch list applied.\n");
  }
  else
  {
    VDB_LOG_ERROR("Patch list failed.\n");
    lRet = SEQ_LIB_FAILURE;
  }
#endif // else from #ifndef __STANDALONE__  
     
  return lRet;
} // sdi::SeqFwFileDownload(const char*, SEQ_FwType_t)

//****************************************************************************
//     Event handling
//****************************************************************************

/****************************************************************************/
/** 
 * @brief Register event handler.
 * @details NULL is a valid parameter. Removes the previous handler.
 * 
 * @param  aHandler user defined Sequencer event handling function
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements SEQ_EventHandlerSet_Activity
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_EventHandlerSet(SEQ_EventHandler_t aHandler)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;

#ifndef __STANDALONE__   
  if(sInitialized == 0)
  {      
    // prepare internal signal handler
    struct sigaction sig;
    sig.sa_sigaction = SeqSigHandler;
    sig.sa_flags = SA_SIGINFO;
    
    if( sigaction(SEQ_EVENT_SIGNAL, &sig, NULL) != 0)
    {
      VDB_LOG_ERROR("Failed to register signal handler.\n");
      lRet = SEQ_LIB_FAILURE;
    } // if signal not registered
    else
    {
      sInitialized = 1;
    } // else from if signal not registered
  } // if not initialized yet
#else // #ifndef __STANDALONE__ 
  sInitialized = 1;
#endif // else #ifndef __STANDALONE__ 
  
  if(lRet == SEQ_LIB_SUCCESS)
  {
    // set the new handler
    sEventHandler = aHandler;
    VDB_LOG_NOTE("Event handler set.\n");
  } // if all ok so far

  return lRet;
} // FdmaEventHandlerSet()

/****************************************************************************/
/** 
 * @brief   Register event handler.
 * @details NULL is a valid parameter. Removes the previous handler.
 * 
 * @param  apHandler user defined Sequencer event handling object
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
 * 
 ****************************************************************************/
SEQ_LIB_RESULT SEQ_EventHandlerSet(SEQ_EventHandlerClass *apHandler)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;
#ifndef __STANDALONE__  
  if(sInitialized == 0)
  {      
    // prepare internal signal handler
    struct sigaction sig;
    sig.sa_sigaction = SeqSigHandler;
    sig.sa_flags     = SA_SIGINFO;
    
    if( sigaction(SEQ_EVENT_SIGNAL, &sig, NULL) != 0)
    {
      VDB_LOG_ERROR("Failed to register signal handler.\n");
      lRet = SEQ_LIB_FAILURE;
    } // if signal not registered
    else
    {
      sInitialized = 1;
    } // else from if signal not registered
  } // if not initialized yet
#else // #ifndef __STANDALONE__
  sInitialized = 1;
#endif // else #ifndef __STANDALONE__  
  
  if(lRet == SEQ_LIB_SUCCESS)
  {
    // set the new handler
    spEventHandlerClass = apHandler;
    VDB_LOG_NOTE("Event handler class set.\n");
  } // if all ok so far


  return lRet;
} // SEQ_EventHandlerSet();

//****************************************************************************
#define UNUSED(_a) ((void*) aUnused)

#ifndef __STANDALONE__
void SeqSigHandler(int aSigNum, siginfo_t *apSigInfo, void *aUnused) 
{
  int *lpVal = &(apSigInfo->si_int);
  VDB_LOG_NOTE("Received event %u through signal %d\n", *lpVal, aSigNum);
  (void)aSigNum;	// *** avoid warning ***
  (void)aUnused;	// *** avoid warning ***
  UNUSED(aUnused);
#else // #ifndef __STANDALONE__
void SeqSigHandler(int aSigNumn, int aValue)
{	
	int *lpVal = &aValue;
#endif //else #ifndef __STANDALONE__

  uint8_t lHandlerAssigned = 0;
  
  // *** call apropriate handler ***
  if(sEventHandler != NULL)
  {
    // call the handler
    sEventHandler(*lpVal);
    lHandlerAssigned = 1;
  } // if handler assigned
  
  if(spEventHandlerClass != NULL)
  {
    // call the handler
    spEventHandlerClass->SeqEventHandler(*lpVal);
    lHandlerAssigned = 1;
  } // if handler assigned
  
  if(lHandlerAssigned == 0)
  {
    VDB_LOG_WARNING("Event handler Object unassigned.\n");
  } // if handler unassigned
  
} //SeqSigHandler()

//****************************************************************************

#define CPY2MDATA(_s)	memcpy(&msg.mData, &_s, sizeof(_s));

/****************************************************************************/
/** 
 * @brief   Instructs the Sequencer to commence SW reset.
 * @details Instructs the Sequencer to commence SW reset.
 * 
 * @param  aReset type of SW reset
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements seq_swreset_Activity
 ****************************************************************************/
SEQ_LIB_RESULT seq_swreset(SEQ_reset_t aReset)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;
  SEQM_swreset_t swr;
  SEQ_Message_t  msg;

  swr.mReset = aReset;
  
  CPY2MDATA(swr);
  msg.mType   = SEQ_MSG_TYPE_SWRESET;
  msg.mStatus = SEQ_MSG_STAT_READY;
 
  lRet = SEQ_MessageSend(&msg, 0);

  return(lRet);

} // seq_swreset()

/****************************************************************************/
/** 
 * @brief   Instructs the Sequencer to start specified nodes.
 * @details Instructs the Sequencer to start specified nodes.
 * 
 * @param apSN array of nodes to be started
 * @param aNumNodes number of nodes in the node array apSN
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements seq_startNode_Activity
 ****************************************************************************/
SEQ_LIB_RESULT seq_startNode(SEQM_start_node_para_t* apSN, uint16_t aNumNodes)
{
  SEQ_LIB_RESULT          lRet  = SEQ_LIB_FAILURE;
  SEQ_Message_t           msg;
  SEQM_start_node_t*      lpSn  = (SEQM_start_node_t*)msg.mData;
  SEQM_start_node_para_t* lpSnp = &(lpSn->mNodes);
  size_t                  sz    = sizeof(SEQM_start_node_para_t)*aNumNodes;
  
  if((sizeof(uint16_t)+sz) < sizeof(msg.mData))
  {
    lpSn->mNum = aNumNodes;
    for(uint32_t i = 0; i < aNumNodes; i++)
    {
      lpSnp[i] = apSN[i];
    }   
    
    msg.mType   = SEQ_MSG_TYPE_START_NODE;
    msg.mStatus = SEQ_MSG_STAT_READY;
    lRet        = SEQ_MessageSend(&msg);
  }
  
  return(lRet);

} // seq_startNode()

/****************************************************************************/
/** 
 * @brief   Sends a check_node message to Sequencer.
 * @details Sends a check_node message to Sequencer.
 * 
 * @param aNode id of node to be checked
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements seq_checkNode_Activity
 ****************************************************************************/
SEQ_LIB_RESULT seq_checkNode(uint16_t aNode)
{
  SEQ_LIB_RESULT    lRet = SEQ_LIB_SUCCESS;
  SEQ_Message_t     msg;
  SEQM_check_node_t chk;
  
  chk.mNode = aNode;
  
  CPY2MDATA(chk);
  msg.mType   = SEQ_MSG_TYPE_CHECK_NODE;
  msg.mStatus = SEQ_MSG_STAT_READY;
  
  lRet = SEQ_MessageSend(&msg);

  return(lRet);

} // seq_checkNode()

/****************************************************************************/
/** 
 * @brief   Instructs Sequencer to write given value to specific register in IPU.
 * @details Instructs Sequencer to write given value to specific register in IPU.
 * 
 * @param aIsIPUS 
 * @param aEng 
 * @param aDirect
 * @param aOffset
 * @param aValue
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements seq_setReg_Activity
 ****************************************************************************/
SEQ_LIB_RESULT seq_setReg(uint16_t aIsIPUS, 
                          uint16_t aEng, 
                          uint16_t aDirect, 
                          uint32_t aOffset, 
                          uint32_t aValue)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_FAILURE;
  SEQ_Message_t  msg;
  SEQM_ipu_reg_t set;
  uint32_t       addr;
  
  if(0 != (addr = checkIpuRange(aIsIPUS, aEng)))
  {
    set.mEngBase = addr + ISP_IO_DEV_SPACE*aEng;
    set.mDirect  = aDirect;
    set.mIndex   = aOffset;
    set.mData    = aValue;
  
    CPY2MDATA(set);
    msg.mType    = SEQ_MSG_TYPE_SET_REG;
    msg.mStatus  = SEQ_MSG_STAT_READY;
  
    lRet = SEQ_MessageSend(&msg);
  }
  
  return(lRet);
} // seq_setReg()

/****************************************************************************/
/** 
 * @brief   Instructs the Sequencer to read specific register in IPU.
 * @details Instructs the Sequencer to read specific register in IPU.
 * 
 * @param aIsIPUS 
 * @param aEng 
 * @param aDirect
 * @param aOffset
 * @param aValue
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements seq_getReg_Activity
 ****************************************************************************/
SEQ_LIB_RESULT seq_getReg(uint16_t  aIsIPUS, 
                          uint16_t  aEng, 
                          uint16_t  aDirect, 
                          uint32_t  aOffset, 
                          uint32_t* apValue)
{
  SEQ_LIB_RESULT  lRet = SEQ_LIB_FAILURE;
  SEQ_Message_t   msg;
  SEQM_ipu_reg_t  get;
  uint32_t        addr;
  
  if(0 != (addr = checkIpuRange(aIsIPUS, aEng)))
  {
    get.mEngBase = addr + ISP_IO_DEV_SPACE*aEng;
    get.mDirect  = aDirect;
    get.mIndex   = aOffset;
  
    CPY2MDATA(get);
    msg.mType   = SEQ_MSG_TYPE_GET_REG;
    msg.mStatus = SEQ_MSG_STAT_READY;
  
    lRet = SEQ_MessageSend(&msg);
    
    if(SEQ_MSG_STAT_RESULT == msg.mStatus)
    {
      *apValue = ((SEQM_ipu_reg_t*)msg.mData)->mData;
      lRet = SEQ_LIB_SUCCESS;
    } // if result available
  } // checkIpuRange ok
  
  return(lRet);
} // seq_getReg()

/****************************************************************************/
/** 
 * @brief   Sends an echo message to Sequencer.
 * @details Sends an echo message to Sequencer.
 * 
 * @param aDelay time interval the Sequencer should wait before replying to 
 *               the message.
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements seq_echo_Activity
 ****************************************************************************/
SEQ_LIB_RESULT seq_echo(uint32_t aDelay)
{
  SEQ_LIB_RESULT lRet = SEQ_LIB_SUCCESS;
  SEQ_Message_t  msg;
  SEQM_echo_t    ech;
  
  ech.mReplayDly= aDelay;
  
  CPY2MDATA(ech);
  msg.mType   = SEQ_MSG_TYPE_ECHO;
  msg.mStatus = SEQ_MSG_STAT_READY;
  
  lRet = SEQ_MessageSend(&msg);

  return(lRet);

} // seq_checkNode()

/****************************************************************************/

static uint32_t checkIpuRange(uint16_t aIsIPUS, uint16_t aEng)
{
  uint32_t addr=0;
  
  if(aIsIPUS)
  {
    if(aEng < ISP_NUM_IPUS)
    {
      addr = (uint32_t)((uintptr_t)&IPUS_0);
    }
  }
  else
  {
    if(aEng < ISP_NUM_IPUV)
    {
      addr = (uint32_t)((uintptr_t)&IPUV_0);
    }
  }
  
  return(addr);

} // checkIpuRange()

/****************************************************************************/

static SEQ_LIB_RESULT IpuRegsMap(void)
{
  SEQ_LIB_RESULT  lRet = SEQ_LIB_SUCCESS;
  
#ifdef __STANDALONE__  
  spIpus = (volatile struct IPUS_tag *)((uintptr_t)&IPUS_0 + SEQ_ADDR_OFF);
  spIpuv = (volatile struct IPUV_tag *)((uintptr_t)&IPUV_0 + SEQ_ADDR_OFF);
#else
  int32_t lFd = -1;
  lFd = open("/dev/mem", O_RDWR);

  if(lFd < 0)
  {
    VDB_LOG_ERROR("Failed to open /dev/mem\n");
    lRet = SEQ_LIB_FAILURE;
  } //if failed to open /dev/mem
  else
  {
    if(spIpus == NULL)
    {
      spIpus = (volatile struct IPUS_tag *) \
                         mmap(
                              NULL, 
                              IPUS_REG_SIZE, 
                              PROT_READ | PROT_WRITE, 
                              MAP_SHARED, 
                              lFd,
                              (uintptr_t)&IPUS_0 + SEQ_ADDR_OFF);

      if (spIpus == NULL) 
      {
        printf("FAILED\n");
        lRet = SEQ_LIB_FAILURE;
        VDB_LOG_ERROR("Can't map IPUS registers memory: %s\n", strerror(errno));
      } // if failed to map IPUS registers
    } // if IPUS regs not mapped yet
      
    if(spIpuv == NULL)
    {
      spIpuv = (volatile struct IPUV_tag *) \
                         mmap(
                              NULL, 
                              IPUV_REG_SIZE,
                              PROT_READ | PROT_WRITE, 
                              MAP_SHARED, 
                              lFd,
                              (uintptr_t)&IPUV_0 + SEQ_ADDR_OFF);

      if (spIpuv == NULL) 
      {
        printf("FAILED\n");
        lRet = SEQ_LIB_FAILURE;
        VDB_LOG_ERROR("Can't map IPUV registers memory: %s\n", strerror(errno));
      } // if failed to map IPUV registers
    } // if Ipuv regs not mapped yet
    
    close(lFd);
  } // else from if failed to open /dev/mem
#endif
  
  return lRet;
} // IpuRegsMap()

/****************************************************************************/

static void IpuRegsUnmap(void)
{
#ifndef __STANDALONE__
  if(spIpus != NULL)
  {
    munmap((void*)spIpus, IPUS_REG_SIZE);
  }
    
  if(spIpuv == NULL)
  {
    munmap((void*)spIpus, IPUV_REG_SIZE);
  }  
#endif //ifdef __STANDALONE__
  
  spIpus = NULL;
  spIpuv = NULL;
} // IpuRegsUnmap

/****************************************************************************/
/** 
 * @brief   Prints out IPU performance counters.
 * @details Prints out IPU performance counters.
 * 
 * @param aFramecount number of frames the performance counter are valid for.
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements ISP_DumpPerfCounters_Activity
 ****************************************************************************/
void ISP_DumpPerfCounters(uint16_t aFramecount)
{
  unsigned long exec_ipu[12];
#ifdef ISP_PERF_PRINTSTALL  
  unsigned long stall_ipu[12];
#endif // #ifdef ISP_PERF_PRINTSTALL
  float exec_mcpf;
  char engine_name[12][3]= {
    "s0","s1","s2","s3","s4","s5","s6","s7",
    "v0","v1","v2","v3"
  };
  
  // map IPUx registers if not mapped yet
  if((spIpus == NULL) || (spIpuv == NULL))
  {
    IpuRegsMap();
  } // if spIpus registers not mapped yet
  
  if((spIpus != NULL) && (spIpuv != NULL))
  {
    volatile struct IPUS_tag *lpIpus = spIpus;
    volatile struct IPUV_tag *lpIpuv = spIpuv;
    uint32_t i;
    
    // IPUS readout
    
    for(i = 0; i < IPUS_CNT; i++)
    {
      exec_ipu[i]  = lpIpus->EXEC_COUNTER.R;
 #ifdef ISP_PERF_PRINTSTALL
      stall_ipu[i] = lpIpus->STALL_DATAHAZARD.R;
#endif // #ifdef ISP_PERF_PRINTSTALL
      lpIpus       = (struct IPUS_tag *)((uintptr_t)lpIpus + 0x1000);
    } // for all IPUS
  
    // IPUV readout

    for(i = IPUS_CNT; i < (IPUS_CNT + IPUV_CNT); i++)
    {
      exec_ipu[i]  = lpIpuv->EXEC_COUNTER.R;
#ifdef ISP_PERF_PRINTSTALL      
      stall_ipu[i] = lpIpuv->STALL_DATAHAZARD.R;
#endif // #ifdef ISP_PERF_PRINTSTALL
      lpIpuv       = (struct IPUV_tag *)((uintptr_t)lpIpuv + 0x1000);
    } // for all IPUV
    
    // generate report
    
    for (i = 0; i < (IPUS_CNT + IPUV_CNT); i++) 
    {
      if (exec_ipu[i]) 
      {
        exec_mcpf=exec_ipu[i]/aFramecount/1000000.0;
        printf("ipu%s took %9.2f Mcylces/frame;"
  #ifdef ISP_PERF_PRINTSTALL
        " %4.1f%% stalls;"
  #endif //ISP_PERF_PRINTSTALL
        " %6.2ffps\n",
        engine_name[i],exec_mcpf,
  #ifdef ISP_PERF_PRINTSTALL
        stall_ipu[i]*100.0/exec_ipu[i],
  #endif //ISP_PERF_PRINTSTALL
        500.0/(float)exec_mcpf);
      } // if exec_ipu not 0
    } // for all possible IPUs
  } // if IPUx registers available
  else
  {
    printf("Unable to access IPUx registers.\n");
  } // else from if IPUx registers available
} // ISP_DumpPerfCounters()

/****************************************************************************/
/** 
 * @brief   Prints out IPU state.
 * @details Prints out IPU state.
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements ISP_DumpIpuState_Activity
 ****************************************************************************/
void ISP_DumpIpuState()
{
  uint32_t lines_total=0;
  uint32_t pos;
  
  // map IPUx registers if not mapped yet
  if((spIpus == NULL) || (spIpuv == NULL))
  {
    IpuRegsMap();
  } // if spIpus registers not mapped yet
  
  if((spIpus != NULL) || (spIpuv != NULL))
  {
    volatile struct IPUS_tag *lpIpus = spIpus;
    volatile struct IPUV_tag *lpIpuv = spIpuv;
   
    // IPUS readout
  
    if ((pos=lpIpus->HOST_POS.R)) {
      printf("final position ipus0 (%4d,%4d)\n",pos>>16,pos&0xffff);
      lines_total+=pos&0xffff;
    }
    lpIpus       = (struct IPUS_tag *)((uintptr_t)lpIpus + 0x1000);
    if ((pos=lpIpus->HOST_POS.R)) {
      printf("final position ipus1 (%4d,%4d)\n",pos>>16,pos&0xffff);
      lines_total+=pos&0xffff;
    }
    lpIpus       = (struct IPUS_tag *)((uintptr_t)lpIpus + 0x1000);
    if ((pos=lpIpus->HOST_POS.R)) {
      printf("final position ipus2 (%4d,%4d)\n",pos>>16,pos&0xffff);
      lines_total+=pos&0xffff;
    }
    lpIpus       = (struct IPUS_tag *)((uintptr_t)lpIpus + 0x1000);
    if ((pos=lpIpus->HOST_POS.R)) {
      printf("final position ipus3 (%4d,%4d)\n",pos>>16,pos&0xffff);
      lines_total+=pos&0xffff;
    }
    lpIpus       = (struct IPUS_tag *)((uintptr_t)lpIpus + 0x1000);
    if ((pos=lpIpus->HOST_POS.R)) {
      printf("final position ipus4 (%4d,%4d)\n",pos>>16,pos&0xffff);
      lines_total+=pos&0xffff;
    }
    lpIpus       = (struct IPUS_tag *)((uintptr_t)lpIpus + 0x1000);
    if ((pos=lpIpus->HOST_POS.R)) {
      printf("final position ipus5 (%4d,%4d)\n",pos>>16,pos&0xffff);
      lines_total+=pos&0xffff;
    }
    lpIpus       = (struct IPUS_tag *)((uintptr_t)lpIpus + 0x1000);
    if ((pos=lpIpus->HOST_POS.R)) {
      printf("final position ipus6 (%4d,%4d)\n",pos>>16,pos&0xffff);
      lines_total+=pos&0xffff;
    }
    lpIpus       = (struct IPUS_tag *)((uintptr_t)lpIpus + 0x1000);
    if ((pos=lpIpus->HOST_POS.R)) {
      printf("final position ipus7 (%4d,%4d)\n",pos>>16,pos&0xffff);
      lines_total+=pos&0xffff;
    }

    if ((pos=lpIpuv->HOST_POS.R)) {
      printf("final position ipuv0 (%4d,%4d)\n",pos>>16,pos&0xffff);
      lines_total+=pos&0xffff;
    }
    lpIpuv        = (struct IPUV_tag *)((uintptr_t)lpIpuv + 0x1000);
    
    if ((pos=lpIpuv->HOST_POS.R)) {
      printf("final position ipuv1 (%4d,%4d)\n",pos>>16,pos&0xffff);
      lines_total+=pos&0xffff;
    }
    lpIpuv        = (struct IPUV_tag *)((uintptr_t)lpIpuv + 0x1000);
    
    if ((pos=lpIpuv->HOST_POS.R)) {
      printf("final position ipuv2 (%4d,%4d)\n",pos>>16,pos&0xffff);
      lines_total+=pos&0xffff;
    }
    lpIpuv        = (struct IPUV_tag *)((uintptr_t)lpIpuv + 0x1000);
    
    if ((pos=lpIpuv->HOST_POS.R)) {
      printf("final position ipuv3 (%4d,%4d)\n",pos>>16,pos&0xffff);
      lines_total+=pos&0xffff;
    }
    
    if (!lines_total) {
      printf("\nNo lines started. Is the Start Trigger correct?\n");
    }
  } // if IPUx registers available
  else
  {
    printf("Unable to access IPUx registers.\n");
  } // else from if IPUx registers available
} // ISP_DumpIpuState()

extern GraphMetadata_t  gGraphMetadata;

/****************************************************************************/
/** 
 * @brief   Prints out Graph bufers statics.
 * @details Prints out Graph bufers statics.
 * 
 * @param appFetchedBufferList  List of graph buffer pointers returned by 
 *                              io::FrameInputIsp::GraphFetch() method.
 * 
 * @return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE otherwise
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
 * @implements ISP_PrintGraphStat_Activity
 ****************************************************************************/
void ISP_PrintGraphStat(SEQ_Buf_t** appFetchedBufferList)
{ 
#ifdef __STANDALONE__
  for(volatile uint32_t w=0; w<100000; w++)
  {
    ;
  } // for
#endif // ifdef __STANDALONE__
  static SEQ_FrmTemporalStat_t sTimeStatsOld = {0,0,0};
  SEQ_FrmTemporalStat_t lTimeStats;
  if(SEQ_TimeStatsGet(lTimeStats) == SEQ_LIB_SUCCESS)
  {
    uint64_t lTimeDiff     = lTimeStats.mLastDoneTime - 
                             sTimeStatsOld.mLastDoneTime;
    uint64_t lFullTimeDiff = lTimeStats.mLastDoneTime - 
                             lTimeStats.mStartTime;
    uint64_t lFrmCntDiff   = lTimeStats.mFrmDoneCnt - sTimeStatsOld.mFrmDoneCnt;
    
    if(sTimeStatsOld.mFrmDoneCnt == 0)
    {
      lTimeDiff -= lTimeStats.mStartTime;
    } // if first check
    
    sTimeStatsOld = lTimeStats;
    printf("FPS: %f, overall FPS: %f, frames done %lu, overall frames done %lu\n", 
          ((float)lFrmCntDiff)/((float)lTimeDiff/1000000.0f),            
          ((float)lTimeStats.mFrmDoneCnt)/((float)lFullTimeDiff/1000000.0f),
          lFrmCntDiff,
          lTimeStats.mFrmDoneCnt
          );
  }
  else
  {
    printf("Failed to get temporal staistics.\n");
  }

  SEQ_Buf_t*  pBuf;
  uint32_t    i=0;
  uint16_t    hw;
  
  while(NULL != (pBuf=appFetchedBufferList[i]))
  {
    if(pBuf->mpBuf > 1)
    {
      printf("--- %s ---\n", gGraphMetadata.mcppBufferNames[i]);
      printf("Size: %d\n", pBuf->mcLineMax);
      for(uint32_t j=0; j<MAX_CONSUMER; j++)
      {
        if(pBuf->mConsumer[j].mpCons)
        {
          hw = pBuf->mConsumer[j].mHighWater;
          printf("[%d].max: %d%s\n", j, hw, (hw>=pBuf->mcLineMax) ? " !!!!!":"");
        } // if consumer exists
      } // for all possible consumers
    } // if SRAM buffer allocated
    i++;
  } // while any buffers left

  printf("----------------------\n");
  
  // *** Seq Msg ***
  SEQ_Message_t lMsg;
  lMsg.mStatus      = SEQ_MSG_STAT_READY;
  lMsg.mType        = SEQ_MSG_TYPE_GET_IDLE_CNT;
  SEQM_idle_cnt_t* lpIdle = (SEQM_idle_cnt_t*)lMsg.mData;
  
  if(SEQ_MessageSend(&lMsg) != SEQ_LIB_SUCCESS)
  {
    VDB_LOG_NOTE("Failed to send a message.\n");
  } // if message send failed

  printf("Idle-L: 0x%08x\n", lpIdle->mIdleL);
  printf("Idle-H: 0x%08x\n", lpIdle->mIdleH);
} // ISP_PrintGraphStat()

//***************************************************************************
/*EOF*/
