/*****************************************************************************
 *
 * Freescale Confidential Proprietary
 *
 * Copyright (c) 2014-16 Freescale Semiconductor;
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
 * @file     fdma_user.c
 * @brief    User space access functions to fdma driver.
 ****************************************************************************/

/*=============================================================================
Revision History:
                        Modification       Tracking 
Author (core ID)        Date D/M/Y         Number     Description of Changes
Tomas Babinec           10-Nov-2014                   Init version
Tomas Babinec           14-May-2017        VSDK-575   Traceability.
Tomas Babinec           26-May-2017        VSDK-575   Misra.  
=============================================================================*/

#include "s32vs234.h"
#include <cstdlib>
#include <csignal>
#include <cstring>
#include <unistd.h>
#include <stdint.h>

#ifndef __STANDALONE__ 
  // only for LINUX
  #include <fcntl.h>      // open 
  #include <sys/types.h>
  #include <sys/ioctl.h>  // ioctl */
  #include "fdma_ioctl.h"
#else 
  #include "fdma_func.h"
  #include "seq_types.h"
#endif // else from ifndef __STANDALONE__

#include "isp_fdma.h"

//****************************************************************************
// global variables
//****************************************************************************

static int32_t sFileDesc = (int32_t)-1;   ///< driver file descriptor

///< user registered event handler
static FDMA_EventHandler_t sEventHandler = NULL;
///< user registerred event handler
static const FDMA_EventHandlerClass *spEventHandlerClass = NULL; 

#ifdef __STANDALONE__
  extern SEQ_FdmaComm_t gSEQ_DRV_FdmaComm; ///< exported from seq driver
  
  static FDMA_DrvInfo_t sFdmaDrvInfo;
#endif // #ifdef __STANDALONE__  

//****************************************************************************
// local function declarations
//****************************************************************************
#ifndef __STANDALONE__
  void FdmaSigHandler(int32_t aNum, 
                      siginfo_t *apSigInfo, 
                      void *apUnused);
#else // #ifndef __STANDALONE__
  extern "C" 
  {
    void FdmaSigHandler(int32_t, int32_t aValue);
  }
#endif // else from #ifndef __STANDALONE__

//****************************************************************************
// function definitions
//****************************************************************************

/****************************************************************************/
/** 
 * @brief Enables FDMA operation for this program. * 
 * @details In Linux the fdma special device file is opened.
 *          In standalone environment the FDMA_SetUp() function is called 
 *          directly to initialize the internal driver structures and HW 
 *          registers.
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
 * @implements FdmaOpen_Activity
 ****************************************************************************/
FDMA_LIB_RESULT FdmaOpen(void)
{
  FDMA_LIB_RESULT lRet = FDMA_LIB_SUCCESS;
   
  if(sFileDesc == -1)
  {
#ifndef __STANDALONE__ 
    if(FdmaEventHandlerSet((FDMA_EventHandler_t)NULL) != FDMA_LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to setup FDMA signal handler.\n");
      lRet = FDMA_LIB_FAILURE;
    } // if failed to set event handler
    else
    {
      sFileDesc = open("/dev/" FDMA_DEVICE_NAME, 0);
      if (sFileDesc < 0) {
        VDB_LOG_ERROR("Can't open device file: %s\n", FDMA_DEVICE_NAME);
        lRet = FDMA_LIB_FAILURE;
      } // if open failed
    } // else from if failed to set event handler
#else // #ifndef __STANDALONE__ 
    
    sFdmaDrvInfo.mpRegs               = (FDMA_Regs_t*) FDMA_BASE;
    sFdmaDrvInfo.mSequencerOn         = SEQ_LESS_MODE;
    sFdmaDrvInfo.mpSEQ_FdmaTdSchedule = NULL;
    sFdmaDrvInfo.mpSigInvoke          = &FdmaSigHandler;
    sFdmaDrvInfo.mpTdtBasePhyHndl     = NULL;
    sFdmaDrvInfo.mTdtBasePhy          = (uint32_t)0;
    sFdmaDrvInfo.mpSeqComm            = &gSEQ_DRV_FdmaComm;
    sFdmaDrvInfo.mIrqDone             = FDMA_IRQ_DONE;
    sFdmaDrvInfo.mIrqErr              = FDMA_IRQ_ERR;
    // rest of the member initialized in FDMA_Setup()

    // *** Setup the driver ***    
    if(FDMA_Setup(&sFdmaDrvInfo) != FDMA_DRV_SUCCESS)
    {
      lRet = FDMA_LIB_FAILURE;
      VDB_LOG_ERROR("FDMA setup failed.\n");
    } // if driver setup failed
    else
    {
      sFileDesc = 1;
    } // else from if driver setup failed
#endif //__STANDALONE__
  } // not openned yet
  else 
  {
    VDB_LOG_WARNING("Fdma already openned.\n");
  } // else from if not openned yet
  
  return lRet;
} // Open()

/****************************************************************************/
/** 
 * @brief Disables FDMA operation for this program.
 * @details Disables FDMA operation for this program.
 * 
 * In Linux the fdma special device file is closed.
 * In standalone environment the FDMA_Close() function is called directly to
 * release internal driver resources and disconnect from the HW.
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
 * @implements FdmaClose_Activity
 * 
 ****************************************************************************/
FDMA_LIB_RESULT FdmaClose(void)
{
  FDMA_LIB_RESULT lRet = FDMA_LIB_SUCCESS;
  
  if(sFileDesc != -1)
  {
#ifndef __STANDALONE__  
    if (close(sFileDesc) != 0) {
      VDB_LOG_ERROR("Failed to close device file %s.\n", FDMA_DEVICE_NAME);
      lRet = FDMA_LIB_FAILURE;
    } // if close failed
    else
    {
      sFileDesc = -1;
    } // else if close failed
#else // #ifndef __STANDALONE__ 

    // relase resources
    FDMA_Close();
    sFileDesc = -1;
#endif // else from #ifndef __STANDALONE__ 
  } // not openned yet
  else 
  {
    VDB_LOG_WARNING("Fdma already closed.\n");
  } // else from if not openned yet
  
  return lRet;
} // Close()

/****************************************************************************/
/** 
 * @brief Reserves one FDMA TranferChannel (TC).
 * @details Reserves one FDMA TranferChannel (TC).
 * 
 * @param  apTc  pointerto a TC structure to be filled with the reserved TC 
 *               metadata and setup
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
 * @implements FdmaTcReserve_Activity
 * 
 ****************************************************************************/
FDMA_LIB_RESULT FdmaTcReserve(FDMA_Tc_t *apTc)
{
  FDMA_LIB_RESULT lRet = FDMA_LIB_SUCCESS;

#ifndef __STANDALONE__
  if(apTc != NULL)
  {
    if (ioctl(sFileDesc, IOCTL_FDMA_TD_RESERVE, &(apTc->mTdm)) != FDMA_DRV_SUCCESS) 
    {
      VDB_LOG_ERROR("TcReserve failed:%d\n", (int32_t) lRet);
      // reset the TC
      memset(apTc, 0, sizeof(FDMA_Tc_t));
      lRet = FDMA_LIB_FAILURE;
    } // if ioctl returned failure
  } // if TC exists
  else
  {
    VDB_LOG_ERROR("TC pointer is NULL.\n");
    lRet = FDMA_LIB_FAILURE;
  } // else from if TC exists
#else // #ifndef __STANDALONE__ 
  FDMA_Tdm_t *lpFreeTdm = NULL;
          
  //try to pop free TD
  lpFreeTdm = FDMA_TdmFreePop();
  if( lpFreeTdm )
  {
    // set the TDM
    lpFreeTdm->mReserved = 1;
    lpFreeTdm->mOwnerProcId = 777;
      
    // copy TDM back to user
    apTc->mTdm = *lpFreeTdm;
  } // if pop succeeded
  else
  {
    // return failure (leave TC reset to user space)
    lRet = FDMA_LIB_FAILURE;
  } // if pop failed    
#endif // else from #ifndef __STANDALONE__
  
  return lRet;
} // FdmaTcReserve()

/****************************************************************************/
/** 
 * @brief Applies the given TC setup to the USER TransferDescriptorTable (TDT) in SRAM.
 * @details Applies the given TC setup to the USER TransferDescriptorTable (TDT) in SRAM.
 * 
 * @param  acpTc pointer to the TC to be configured
 * 
 * @return FDMA_LIB_SUCCESS if all ok
 *         FDMA_LIB_FAILURE if failed
 * 
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
 * @implements FdmaTcConfig_Activity
 * 
 ****************************************************************************/
FDMA_LIB_RESULT FdmaTcConfig(const FDMA_Tc_t *acpTc)
{
  FDMA_LIB_RESULT lRet = FDMA_LIB_SUCCESS;

#ifndef __STANDALONE__  
  if (ioctl(sFileDesc, IOCTL_FDMA_TD_CONFIG, acpTc) != FDMA_DRV_SUCCESS) 
  {
	  VDB_LOG_ERROR("TcConfig failed:%d\n", (int32_t)lRet);
	  lRet = FDMA_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__ 
  // set the new TD values       
  if(FDMA_TdConfig(acpTc) != FDMA_DRV_SUCCESS)
  {
    VDB_LOG_ERROR("TD config failed.\n");
    lRet = FDMA_LIB_FAILURE;
  }
#endif // else from #ifndef __STANDALONE__ 

  return lRet;
} // FdmaTcConfig()

/****************************************************************************/
/** 
 * @brief Applies the given TC setup to the ISP TransferDescriptorTable (TDT) in SRAM.
 * @details Applies the given TC setup to the ISP TransferDescriptorTable (TDT) in SRAM.
 * 
 * @param  acpTc pointer to the TC to be configured
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
 * @implements FdmaTcIspConfig_Activity
 * 
 ****************************************************************************/
FDMA_LIB_RESULT FdmaTcIspConfig(const FDMA_Tc_t *acpTc)
{
  FDMA_LIB_RESULT lRet = FDMA_LIB_SUCCESS;

#ifndef __STANDALONE__  
  if (ioctl(sFileDesc, IOCTL_FDMA_TD_ISP_CONFIG, acpTc) != FDMA_DRV_SUCCESS) 
  {
    VDB_LOG_ERROR("ISP TC Config failed:%d\n", (int32_t)lRet);
    lRet = FDMA_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__ 
  // set the new TD values       
  if(FDMA_TdIspConfig(acpTc) != FDMA_DRV_SUCCESS)
  {
    VDB_LOG_ERROR("ISP TC config failed.\n");
    lRet = FDMA_LIB_FAILURE;
  }
#endif // else from #ifndef __STANDALONE__ 

  return lRet;
} // FdmaTcConfig()

/****************************************************************************/
/** 
 * @brief Schedules one FDMA transfer described by the given TC.
 * @details Schedule only. No TC configuration done internally. Has to be 
 *          preceeded by call to FdmaTcConfig().
 * 
 * @param  acpTc pointer to the TC to be scheduled
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
 * @implements FdmaTcSchedule_Activity
 * 
 ****************************************************************************/
FDMA_LIB_RESULT FdmaTcSchedule(const FDMA_Tc_t *acpTc)
{
  FDMA_LIB_RESULT lRet = FDMA_LIB_SUCCESS;

#ifndef __STANDALONE__
  if (ioctl(sFileDesc, IOCTL_FDMA_TD_SCHEDULE, acpTc) != FDMA_DRV_SUCCESS) 
  {
	  VDB_LOG_ERROR("TcSchedule failed:%d\n", (int32_t) lRet);
	  lRet = FDMA_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__ 
  FDMA_TcSchedule(acpTc);
#endif // else from #ifndef __STANDALONE__ 

  return lRet;
} // FdmaTcSchedule()

/****************************************************************************/
/** 
 * @brief Releases the specified TC.
 * @details Releases the specified TC.
 * 
 * @param  apTc reference to the TC to be released
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
 * @implements FdmaTcRelease_Activity
 * 
 ****************************************************************************/
FDMA_LIB_RESULT FdmaTcRelease( FDMA_Tc_t *apTc)
{
  FDMA_LIB_RESULT lRet = FDMA_LIB_SUCCESS;

#ifndef __STANDALONE__
  if (ioctl(sFileDesc, IOCTL_FDMA_TD_RELEASE, &(apTc->mTdm)) != FDMA_DRV_SUCCESS) 
  {
    VDB_LOG_ERROR("TcRelease failed:%d\n", (int32_t) lRet);
    lRet = FDMA_LIB_FAILURE;
  } // if ioctl returned failure
  else
  {
    apTc->mTdm.mReserved = (uint8_t)0;
  } // else from if ioctl returned failure
#else // #ifndef __STANDALONE__ 

  //try to push free TDM
  if( FDMA_TdRelease(&(apTc->mTdm)) != FDMA_DRV_SUCCESS )
  {
    VDB_LOG_ERROR("Tdm release failed.\n");
    lRet = FDMA_LIB_FAILURE;
  } // if push failed
#endif // else from #ifndef __STANDALONE__ 
  
  return lRet;
} // FdmaTcRelease()

/****************************************************************************/
/** 
 * @brief Finds out if the Sequencer is ON.
 * @details Finds out if the Sequencer is ON.
 * 
 * @note For debug only at the moment.
 * 
 * @return 1 if Sequencer ON
 *         0 otherwise
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
 * @implements FdmaSeqModeGet_Activity
 * 
 ****************************************************************************/
int32_t FdmaSeqModeGet(void)
{
#ifndef __STANDALONE__
  return ioctl(sFileDesc, (uint64_t)IOCTL_FDMA_SEQ_MODE_GET);
#else // #ifndef __STANDALONE__ 
  return FDMA_SeqModeGet();
#endif // else from #ifndef __STANDALONE__ 
} // FdmaSeqModeGet()

/****************************************************************************/
/** 
 * @brief Gets current content of FDMA registers.
 * @details Gets current content of FDMA registers.
 * 
 * @param  apFdmaRegs pointer to FDMA register structure to be filled in
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
 * @implements FdmaStatusGet_Activity
 * 
 ****************************************************************************/
FDMA_LIB_RESULT FdmaStatusGet(FDMA_Regs_t *apFdmaRegs)
{
  FDMA_LIB_RESULT lRet = FDMA_LIB_SUCCESS;

#ifndef __STANDALONE__
  if (ioctl(sFileDesc, IOCTL_FDMA_STATUS_GET, apFdmaRegs) != FDMA_DRV_SUCCESS) 
  {
    VDB_LOG_ERROR("StatusGet failed:%d\n", (int32_t) lRet);
    lRet = FDMA_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__ 
  FDMA_Regs_t *lpFdmaRegs = FDMA_RegsGet();
        
  if(lpFdmaRegs != NULL)
  {
    // copy fdma registers back to user
    memcpy((void*)apFdmaRegs,(const void*) lpFdmaRegs, sizeof(FDMA_Regs_t));
  }else
  {
    VDB_LOG_ERROR("FDMA registers not mapped.\n");
    lRet = FDMA_LIB_FAILURE;
  }
  
#endif // else from #ifndef __STANDALONE__ 
  return lRet;
} // FdmaStatusGet()

/****************************************************************************/
/** 
 * @brief Gets latest event context for given TC.
 * @details TC specified with index in arEvent.mXfrRecNum number.
 * 
 * @param  arEvent reference to FDMA event
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
 * @implements FdmaEventGet_Activity
 * 
 ****************************************************************************/
FDMA_LIB_RESULT FdmaEventGet(FDMA_TcEvent_t &arEvent)
{
  FDMA_LIB_RESULT lRet = FDMA_LIB_SUCCESS;

#ifndef __STANDALONE__
  if (ioctl(sFileDesc, IOCTL_FDMA_EVENT_GET, &arEvent) != FDMA_DRV_SUCCESS) 
  {
    VDB_LOG_ERROR("EventGet failed:%d\n", (int32_t) lRet);
    lRet = FDMA_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__ 
  
  if(FDMA_EventGet(&arEvent) != FDMA_DRV_SUCCESS)
  {
    VDB_LOG_ERROR("EventGet failed.\n");
    lRet = FDMA_LIB_FAILURE;
  } // if failed to get event
  
#endif // else from #ifndef __STANDALONE__ 
  return lRet;
} // FdmaStatusGet()

/****************************************************************************/
/** 
 * @brief Gets latest state of the given TC. 
 * @details Reads the most resent TD setup directly from SRAM TD table.
 * 
 * @param  apTc pointer to a TC
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
 * @implements FdmaTcGet_Activity
 * 
 ****************************************************************************/
FDMA_LIB_RESULT FdmaTcGet(FDMA_Tc_t *apTc)
{
  FDMA_LIB_RESULT lRet = FDMA_LIB_SUCCESS;

#ifndef __STANDALONE__
  if (ioctl(sFileDesc, IOCTL_FDMA_TC_GET, apTc) != FDMA_DRV_SUCCESS) 
  {
	  VDB_LOG_ERROR("TcGet failed:%d\n", (int32_t) lRet);
	  lRet = FDMA_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__    
  // read the TD  
  if(FDMA_TcGet(apTc) != FDMA_DRV_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to get the TC.\n");
    lRet = FDMA_LIB_FAILURE;
  }  
#endif // else from #ifndef __STANDALONE__ 
  return lRet;
} // FdmaTcGet()

/****************************************************************************/
/** 
 * @brief Gets the current number of reserved TCs.
 * @details Gets the current number of reserved TCs.
 * 
 * @param  arTcUsedCnt reference to the "used count" output variable
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
 * @implements FdmaTcUsed_Activity
 * 
 ****************************************************************************/
FDMA_LIB_RESULT FdmaTcUsed(uint32_t &arTcUsedCnt)
{
  FDMA_LIB_RESULT lRet = FDMA_LIB_SUCCESS;

#ifndef __STANDALONE__
  if (ioctl(sFileDesc, IOCTL_FDMA_TD_USED, &arTcUsedCnt) < 0) 
  {
	  VDB_LOG_ERROR("TcUsed failed: %u\n", arTcUsedCnt);
	  lRet = FDMA_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__ 
  int32_t lTdFree = FDMA_TdmFreeCntGet();
  arTcUsedCnt = FDMA_TD_CNT - lTdFree;
  VDB_LOG_NOTE("TD_CNT: %u, Free: %u, Used: %u\n", (unsigned) FDMA_TD_CNT, (unsigned) lTdFree, (unsigned) arTcUsedCnt);
#endif // else from #ifndef __STANDALONE__ 
  
  return lRet;
} // FdmaTcUsed()

/****************************************************************************/
/** 
 * @brief Gets the physical address of the TDT in SRAM.
 * @details Gets the physical address of the TDT in SRAM.
 * 
 * @note For debug only at the moment.
 * 
 * @param  arTdtAddr reference to the address output variable
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
 * @implements FdmaTdtAddrGet_Activity
 * 
 ****************************************************************************/
FDMA_LIB_RESULT FdmaTdtAddrGet(uint64_t &arTdtAddr)
{
  FDMA_LIB_RESULT lRet = FDMA_LIB_SUCCESS;

#ifndef __STANDALONE__
  if (ioctl(sFileDesc, IOCTL_FDMA_TDT_GET, &arTdtAddr) < 0) 
  {
	  VDB_LOG_ERROR("TdtAddrGet failed:%d\n", (int32_t) arTdtAddr);
	  lRet = FDMA_LIB_FAILURE;
  } // if ioctl returned failure
#else // #ifndef __STANDALONE__ 
  arTdtAddr = FDMA_TdtPhysAddrGet();
#endif // else from #ifndef __STANDALONE__ 
  return lRet;
} // FdmaTdtAddrGet()

/****************************************************************************/
/** 
 * @brief Sets FDMA event handler to a specific transfer channel (TC).
 * @details Sets FDMA event handler to a specific transfer channel (TC).
 * 
 * @param aHandler pointer to a function to handle the event
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
 * @implements FdmaEventHandlerSet_Activity
 * 
 ****************************************************************************/
FDMA_LIB_RESULT FdmaEventHandlerSet(const FDMA_EventHandler_t acHandler)
{
  FDMA_LIB_RESULT lRet = FDMA_LIB_SUCCESS;

#ifndef __STANDALONE__  
  // prepare internal signal handler
  struct sigaction sig;
  (void)sigemptyset(&sig.sa_mask);
  sig.sa_handler   = NULL;
  sig.sa_sigaction = FdmaSigHandler;
  sig.sa_flags     = SA_SIGINFO;
  sig.sa_restorer  = NULL;
  if( sigaction((int32_t)FDMA_EVENT_SIGNAL, &sig, NULL) != (int32_t)0)
  {
    VDB_LOG_ERROR("Failed to register signal handler.\n");
    lRet = FDMA_LIB_FAILURE;
  } // if signal not registered
  
#endif // #ifndef __STANDALONE__

  if(lRet == FDMA_LIB_SUCCESS)
  {
    // set the new handler
    sEventHandler = acHandler;
  } // if all ok so far
  
  return lRet;
} // FdmaEventHandlerSet()

/****************************************************************************/
/** 
 * @brief Register event handler object.
 * @details NULL is a valid parameter. Removes the previous handler.
 * 
 * @param apcHandler user defined FDMA event handling object
 * 
 * @return FDMA_LIB_SUCCESS if all ok
 *         FDMA_LIB_FAILURE if opening of the device file fails
 * 
 ****************************************************************************/
FDMA_LIB_RESULT FDMA_EventHandlerSet(const FDMA_EventHandlerClass *apcHandler)
{
  FDMA_LIB_RESULT lRet = FDMA_LIB_SUCCESS;
#ifndef __STANDALONE__
  // prepare internal signal handler
  struct sigaction sig;
  (void)sigemptyset(&sig.sa_mask);
  sig.sa_handler   = NULL;
  sig.sa_sigaction = FdmaSigHandler;
  sig.sa_flags     = SA_SIGINFO;
  sig.sa_restorer  = NULL;
  
  if( sigaction((int32_t)FDMA_EVENT_SIGNAL, &sig, NULL) != 0)
  {
    VDB_LOG_ERROR("Failed to register signal handler.\n");
    lRet =FDMA_LIB_FAILURE;
  } // if signal not registered
#endif // #ifndef __STANDALONE__  
  
  if(lRet == FDMA_LIB_SUCCESS)
  {
    // set the new handler
    spEventHandlerClass = apcHandler;
    VDB_LOG_NOTE("Event handler class set.\n");
  } // if all ok so far

  return lRet;
} // FDMA_EventHandlerSet()

/****************************************************************************/
/** 
 * @brief Fdma driver signal handler for Linux.
 * @details Fdma driver signal handler for Linux.
 * 
 * @param  aNum signal number
 * @param  apSigInfo additional signal information
 * @param  apUnused
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
 * @implements FdmaSigHandler_Activity
 * 
 ****************************************************************************/
#ifndef __STANDALONE__
void FdmaSigHandler(int32_t, siginfo_t *apSigInfo, void *) 
{
  int32_t lVal = (apSigInfo->si_int);
#else // #ifndef __STANDALONE__
void FdmaSigHandler(int32_t, int32_t aValue)
{ 
  int32_t lVal = aValue;
#endif //else #ifndef __STANDALONE__ 
  uint32_t lTdIdx     = ((uint32_t)lVal &  (uint32_t)0xff);
  uint32_t lEventType = ((uint32_t)lVal >> (uint32_t)8);
  
  uint8_t lHandlerAssigned = (uint8_t)0;

  // *** call apropriate handler ***
  if(sEventHandler != NULL)
  {
    // call the handler
    sEventHandler(lTdIdx, lEventType);
    lHandlerAssigned = (uint8_t)1;
  } // if handler assigned
  
  if(spEventHandlerClass != NULL)
  {
    // call the handler
    spEventHandlerClass->FdmaEventHandler(lTdIdx, lEventType);
    lHandlerAssigned = (uint8_t)1;
  } // if handler assigned
  
  if(lHandlerAssigned == (uint8_t)0)
  {
    VDB_LOG_WARNING("Event handler Object unassigned.\n");
  } // if handler unassigned  
} //FdmaSigHandler()

//****************************************************************************
