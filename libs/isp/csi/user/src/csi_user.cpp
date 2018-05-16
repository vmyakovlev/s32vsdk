/*****************************************************************************
 *
 * NXP Confidential Proprietary
 *
 * Copyright (c) 2015-16 Freescale Semiconductor;
 * Copyright (c) 2016-17 NXP
 * All Rights Reserved
 *
 *****************************************************************************
 *
 * THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONCSIUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/**
 * \file     csi_user.cpp
 * \brief    User-space library calls to access CSI driver functionality.
 * 
 ****************************************************************************/

#include "s32vs234.h"
#include <stdint.h>
#include <cstring>

#ifndef __STANDALONE__
  #include <csignal>
  #include <fcntl.h> // open 
  #include <unistd.h> // exit 
  #include <sys/ioctl.h> // ioctl 
  #include <sys/mman.h> // mmap
#else
  #include "csi_func.h"
#endif // else from #ifndef __STANDALONE__

#define VDB_LOG_NOTE_OFF

#include "isp_csi.h"
#include "vdb_log.h"
#include "isp_hw.h"

/*==============================================================================
*                        LOCAL CONSTANTS
==============================================================================*/


/*==============================================================================
*                             TYPES
==============================================================================*/


/*==============================================================================
*                        GLOBAL VARIABLES
==============================================================================*/


/*==============================================================================
*                        LOCAL DATA
==============================================================================*/

static CSI_EventHandler_t sEventHandler = NULL;
// user registerred event handler
static CSI_EventHandlerClass *spEventHandlerClass = NULL; 
//user registerred event handler

// 
// driver info structures 
//

#ifdef __STANDALONE__
  static CSI_DrvInfo_t spCsiDrvInfo[CSI_IDX_CNT];
  static CSI_DrvInfo_t *sppCsiDrvInfo[CSI_IDX_CNT] = {NULL}; 
#endif //#ifdef __STANDALONE__

#ifndef __STANDALONE__
  static int32_t sCsiFD = -1;    // csi driver file descriptor

/*==============================================================================
*                        LOCAL FUNCTIONS DEFINITIONS
==============================================================================*/

  void CsiSigHandler(int aSigNum, siginfo_t *info, void *unused);
#else //#ifndef __STANDALONE__
extern "C" {
  void CsiSigHandler(int aSigNum, int aValue);
}
#endif //else from #ifndef __STANDALONE__

/*==============================================================================
*                        FUNCTIONS DEFINITIONS
==============================================================================*/
/**
* @brief          The function csi open
* @details        The function csi open
* 
* @implements     CSI_Open_Activity
* 
* @return         CSI_LIB_RESULT
*/
CSI_LIB_RESULT CSI_Open(void)
{
  CSI_LIB_RESULT lRet = CSI_LIB_SUCCESS;
  
#ifndef __STANDALONE__  
  if(sCsiFD == -1)
  { 
    if(CSI_EventHandlerSet((CSI_EventHandler_t)NULL) != CSI_LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to setup CSI error signal handler.\n");
      lRet = CSI_LIB_FAILURE;
    } // if failed to set event handler
    else
    {
      sCsiFD = open("/dev/csi", 0);
      if (sCsiFD == -1) 
      {
        VDB_LOG_ERROR("Can't open device file: %s\n", CSI_DEVICE_NAME);
        lRet = CSI_LIB_FAILURE;
      } // if open failed
    }// else from if failed to set event handler
  } // if not opened yet
  else 
  {
    VDB_LOG_WARNING("Device file: %s opened already.\n", CSI_DEVICE_NAME);
  } // else from if not opened yet
#else // from ifndef __STANDALONE__
  spCsiDrvInfo[0].mpRegs         = NULL;
  spCsiDrvInfo[0].mIrqNums.mErr  = CSI0_ERR_IRQ;
  spCsiDrvInfo[0].mIrqNums.mFrm  = CSI0_FRM_IRQ;
  spCsiDrvInfo[0].mEmbedStatus   = CSI_EMBD_STATUS_NA;
  spCsiDrvInfo[0].mCsiIdx        = (CSI_IDX_t)0;
  spCsiDrvInfo[0].mPPErrIrqMask  = 0;
  spCsiDrvInfo[0].mPhyErrIrqMask = 0;
  spCsiDrvInfo[0].mFrameIrqMask  = 0;
  spCsiDrvInfo[0].mUserPid       = 0;
  memset(&(spCsiDrvInfo[0].mErrStatus), 0, sizeof(CSI_ErrStatus_t));
  sppCsiDrvInfo[0] = &(spCsiDrvInfo[0]);
  
  
  spCsiDrvInfo[1].mpRegs         = NULL;
  spCsiDrvInfo[1].mIrqNums.mErr  = CSI1_ERR_IRQ;
  spCsiDrvInfo[1].mIrqNums.mFrm  = CSI1_FRM_IRQ;
  spCsiDrvInfo[1].mEmbedStatus   = CSI_EMBD_STATUS_NA;
  spCsiDrvInfo[1].mCsiIdx        = (CSI_IDX_t)1;
  spCsiDrvInfo[1].mPPErrIrqMask  = 0;
  spCsiDrvInfo[1].mPhyErrIrqMask = 0;
  spCsiDrvInfo[1].mFrameIrqMask  = 0;
  spCsiDrvInfo[1].mUserPid       = 0;
  memset(&(spCsiDrvInfo[1].mErrStatus), 0, sizeof(CSI_ErrStatus_t));
  sppCsiDrvInfo[1] = &(spCsiDrvInfo[1]);
  
  if(CSI_DRV_Setup(sppCsiDrvInfo) != CSI_DRV_SUCCESS)
  {
    lRet = CSI_LIB_FAILURE;
  } // if csi driver setup failed
#endif // #ifndef __STANDALONE__
  
  return lRet;
} // CSI_Reserve()

/*============================================================================*/
/**
* @brief          The function csi close
* @details        The function csi close
* 
* @implements     CSI_Close_Activity
* 
* @return         CSI_LIB_RESULT
*/
CSI_LIB_RESULT CSI_Close(void)
{
  CSI_LIB_RESULT lRet = CSI_LIB_SUCCESS;
#ifndef __STANDALONE__  
  if(sCsiFD != -1)
  {
    if (close(sCsiFD) != 0) 
    {
      VDB_LOG_ERROR("Failed to close device file %s.\n", CSI_DEVICE_NAME);
      lRet = CSI_LIB_FAILURE;
    } // if close failed
    else
    {
      sCsiFD = -1;
    } // else if close failed
  } // not openned yet
  else 
  {
    VDB_LOG_WARNING("Csi already closed.\n");
  } // else from if not openned yet
#endif // else from #ifndef __STANDALONE__ 
  return lRet;
} // CSI_Release()

/*============================================================================*/
/**
* @brief          The function csi config
* @details        The function csi config
* 
* @param          CSI_Cfg_t
* 
* @implements     CSI_Config_Activity
* 
* @return         CSI_LIB_RESULT
*/
CSI_LIB_RESULT CSI_Config(CSI_Cfg_t *apcCsiCfg)
{
  CSI_LIB_RESULT lRet = CSI_LIB_SUCCESS;

#ifndef __STANDALONE__  
  if(apcCsiCfg != NULL)
  {
    if(sCsiFD == -1)
    {
      VDB_LOG_ERROR("Csi device file not opened.\n");
      lRet = CSI_LIB_FAILURE;
    } // if Csi not opened
    else
    {
      int32_t lRetVal = 0;
      lRetVal = ioctl(sCsiFD, IOCTL_CSI_CONFIG, apcCsiCfg);            

      if(lRetVal < 0) 
      {
        VDB_LOG_ERROR("Csi init failed: %d\n", lRetVal);
        lRet = CSI_LIB_FAILURE;
      } // if ioctl returned failure
    } // else from if Csi not opened
  }// if apcCsiCfg exists
#else // #ifndef __STANDALONE__
  CSI_DRV_Config(apcCsiCfg);
#endif // else #ifndef __STANDALONE__
  
  return lRet;
} // CSI_Config(const CSI_Cfg_t *apcCsiCfg)

/*============================================================================*/
/**
* @brief          The function csi embedded config
* @details        The function csi embedded config
* 
* @param          CSI_EmbdCfg_t* 
* 
* @implements     CSI_EmbdConfig_Activity
* 
* @return         CSI_LIB_RESULT
*/
CSI_LIB_RESULT CSI_EmbdConfig(CSI_EmbdCfg_t &arEmbdCfg)
{
  CSI_LIB_RESULT lRet = CSI_LIB_SUCCESS;

#ifndef __STANDALONE__  
  if(sCsiFD == -1)
  {
    VDB_LOG_ERROR("Csi device file not opened.\n");
    lRet = CSI_LIB_FAILURE;
  } // if Csi not opened
  else
  {
    int32_t lRetVal = 0;
    lRetVal = ioctl(sCsiFD, IOCTL_CSI_EMBD_CONFIG, &arEmbdCfg);
       
    if(lRetVal < 0) 
    {
      VDB_LOG_ERROR("Csi embedded data configuration failed: %d\n", lRetVal);
      lRet = CSI_LIB_FAILURE;
    } // if ioctl returned failure
  } // else from if Csi not opened
#else // #ifndef __STANDALONE__
  CSI_DRV_EmbdConfig(&arEmbdCfg);
#endif // else #ifndef __STANDALONE__
  
  return lRet;
} // CSI_EmbedConfig()

/*============================================================================*/
/**
* @brief          The function csi embedded recapture
* @details        The function csi embedded recapture
* 
* @param          CSI_IDX
*
* @implements     CSI_EmbdRecap_Activity
* 
* @return         CSI_LIB_RESULT 
*/
CSI_LIB_RESULT CSI_EmbdRecap(const enum CSI_IDX acCsiIdx)
{
  CSI_LIB_RESULT lRet = CSI_LIB_SUCCESS;

#ifndef __STANDALONE__  
  if(sCsiFD == -1)
  {
    VDB_LOG_ERROR("Csi device file not opened.\n");
    lRet = CSI_LIB_FAILURE;
  } // if Csi not opened
  else
  {
    int32_t lRetVal = 0;

    lRetVal = ioctl(sCsiFD, IOCTL_CSI_EMBD_RECAP, acCsiIdx);  
    
    if(lRetVal < 0) 
    {
      VDB_LOG_ERROR("Csi embedded data recapture failed: %d\n", lRetVal);
      lRet = CSI_LIB_FAILURE;
    } // if ioctl returned failure
  } // else from if Csi not opened
#else // #ifndef __STANDALONE__
  CSI_DRV_EmbdRecap(acCsiIdx);
#endif // else #ifndef __STANDALONE__
  
  return lRet;
} // CSI_EmbedConfig()

/*============================================================================*/
/**
* @brief          The function csi embedded status get
* @details        The function si embedded status get
* 
* @param[in]      CSI_IDX 
* 
* @implements     CSI_EmbdStatusGet_Activity
* 
* @return         uint32_t
*/
uint32_t CSI_EmbdStatusGet(const enum CSI_IDX acCsiIdx)
{
  uint32_t lRet = 0U;
#ifndef __STANDALONE__  
  if(sCsiFD == -1)
  {
    VDB_LOG_ERROR("Csi device file not opened.\n");
    lRet = CSI_LIB_FAILURE;
  } // if Csi not opened
  else
  {
    int32_t lRetVal = 0;
    lRetVal = ioctl(sCsiFD, IOCTL_CSI_EMBD_STATUS_GET, acCsiIdx);
   
    if(lRetVal < 0)
    {
      VDB_LOG_ERROR("Csi embedded data configuration failed: %d\n", lRetVal);
      lRet = CSI_LIB_FAILURE;
    } // if ioctl returned failure       
    lRet = (uint32_t)lRetVal;    
  } // else from if Csi not opened
#else // #ifndef __STANDALONE__
  lRet = CSI_DRV_EmbdStatusGet(acCsiIdx);
#endif // else #ifndef __STANDALONE__
  
  return lRet;
} // CSI_EmbedConfig()

/*============================================================================*/
/**
* @brief          The function csi embedded status get
* @details        The function csi embedded status get
* 
* @param[in]      CSI_ErrStatus_t
* @param[in]      CSI_IDX
*
* @implements     CSI_ErrStatusGet_Activity
* 
* @return         CSI_LIB_RESULT 
*/
CSI_LIB_RESULT CSI_ErrStatusGet(CSI_ErrStatus_t &arErrStatus, 
                                const enum CSI_IDX acCsiIdx)
{
  CSI_LIB_RESULT lRet = CSI_LIB_SUCCESS;
#ifndef __STANDALONE__  
  if(sCsiFD == -1)
  {
    VDB_LOG_ERROR("Csi device file not opened.\n");
    lRet = CSI_LIB_FAILURE;
  } // if Csi not opened
  else
  {
    int32_t lRetVal = 0;
    // provide csi index as other error frame id
    arErrStatus.mOthrErrFrmId = (uint32_t)acCsiIdx;
    
    lRetVal = ioctl(sCsiFD, IOCTL_CSI_ERR_STATUS_GET, &arErrStatus);
       
    if(lRetVal < 0)
    {
      VDB_LOG_ERROR("Failed to get CSI error status: %d\n", lRetVal);
      lRet = CSI_LIB_FAILURE;
    } // if ioctl returned failure
  } // else from if Csi not opened
#else // #ifndef __STANDALONE__
  arErrStatus = *(CSI_DRV_ErrStatusGet(acCsiIdx));
#endif // else #ifndef __STANDALONE__
  
  return lRet;
} // CSI_EmbedConfig()

/*============================================================================*/
/**
* @brief          The function csi error clear
* @details        The function csi error clear
* 
* @param[in]      CSI_IDX
* 
* @implements     CSI_ErrClear_Activity
*
* @return         CSI_LIB_RESULT 
*/
CSI_LIB_RESULT CSI_ErrClear(const enum CSI_IDX acCsiIdx)
{
  CSI_LIB_RESULT lRet = CSI_LIB_SUCCESS;
#ifndef __STANDALONE__  
  if(sCsiFD == -1)
  {
    VDB_LOG_ERROR("Csi device file not opened.\n");
    lRet = CSI_LIB_FAILURE;
  } // if Csi not opened
  else
  {
    int32_t lRetVal = 0;
    
    lRetVal =  ioctl(sCsiFD, IOCTL_CSI_ERR_CLEAR, acCsiIdx);

    if(lRetVal < 0)
    {
      VDB_LOG_ERROR("Failed to clear CSI error status: %d\n", lRetVal);
      lRet = CSI_LIB_FAILURE;
    } // if ioctl returned failure
  } // else from if Csi not opened
#else // #ifndef __STANDALONE__
  CSI_DRV_ErrClear(acCsiIdx);
#endif // else #ifndef __STANDALONE__
  
  return lRet;
} // CSI_EmbedConfig()

/*============================================================================*/
/**
* @brief          The function csi start
* @details        The function csi start
* 
* @param[in]      CSI_IdKey_t
*
* @implements     CSI_Start_Activity
* 
* @return         CSI_LIB_RESULT 
*/
CSI_LIB_RESULT CSI_Start(CSI_IdKey_t aCsiIdKey)
{
  CSI_LIB_RESULT lRet = CSI_LIB_SUCCESS;

#ifndef __STANDALONE__   
    int32_t lRetVal = 0;
    
    lRetVal = ioctl(sCsiFD, IOCTL_CSI_START, aCsiIdKey);
    
    if(lRetVal < 0)
    {
      VDB_LOG_ERROR("Failed to start CSI interface.\n");
      lRet = CSI_LIB_FAILURE;
    } // if ioctl returned failure
  
#else // #ifndef __STANDALONE__
    CSI_DRV_Start(aCsiIdKey);
#endif // else #ifndef __STANDALONE__
  
  return lRet;
} // CSI_Start(CSI_IdKey_t aCsiIdKey)

/*============================================================================*/
/**
* @brief          The function csi stop
* @details        The function csi stop
* 
* @param[in]      CSI_IdKey_t
* 
* @implements     CSI_Stop_Activity
*
* @return         CSI_LIB_RESULT 
*/
CSI_LIB_RESULT CSI_Stop(CSI_IdKey_t aCsiIdKey)
{
  CSI_LIB_RESULT lRet = CSI_LIB_SUCCESS;

#ifndef __STANDALONE__   
    int32_t lRetVal = 0;
    lRetVal = ioctl(sCsiFD, IOCTL_CSI_STOP, aCsiIdKey);

    if(lRetVal < 0)
    {
      VDB_LOG_ERROR("Failed to stop CSI interface.\n");
      lRet = CSI_LIB_FAILURE;
    } // if ioctl returned failure
  
#else // #ifndef __STANDALONE__
    CSI_DRV_Stop(aCsiIdKey);
#endif // else #ifndef __STANDALONE__
  
  return lRet;
} // CSI_Stop(CSI_IdKey_t aCsiIdKey)

/*============================================================================*/
//                              Event handling
/*============================================================================*/
/**
* @brief          The event handler setup
* @details        The event handler setup
* 
* @param[in]      CSI_EventHandler_t
* 
* @implements     CSI_EventHandlerSet_Activity
* 
* @return         CSI_LIB_RESULT 
*/
CSI_LIB_RESULT CSI_EventHandlerSet(CSI_EventHandler_t aHandler)
{
  CSI_LIB_RESULT lRet = CSI_LIB_SUCCESS;

#ifndef __STANDALONE__   
  // prepare internal signal handler
  struct sigaction sig;
  sig.sa_sigaction = CsiSigHandler;
  sig.sa_flags = SA_SIGINFO;
  
  if( sigaction(CSI_ERR_SIGNAL, &sig, NULL) != 0)
  {
    VDB_LOG_ERROR("Failed to register signal handler.\n");
    lRet = CSI_LIB_FAILURE;
  } // if signal not registered 
#endif // #ifndef __STANDALONE__ 
  
  if(lRet == CSI_LIB_SUCCESS)
  {
    // set the new handler
    sEventHandler = aHandler;
    VDB_LOG_NOTE("Event handler set.\n");
  } // if all ok so far

  return lRet;
} // FdmaEventHandlerSet()

/*============================================================================*/
/**
* @brief          The event handler setup
* @details        The event handler setup
* 
* @param[in]      CSI_EventHandlerClass*
* 
* @implements     CSI_EventHandlerSetClass_Activity
*
* @return         CSI_LIB_RESULT 
*/
CSI_LIB_RESULT CSI_EventHandlerSet(CSI_EventHandlerClass *apHandler)
{
  CSI_LIB_RESULT lRet = CSI_LIB_SUCCESS;
#ifndef __STANDALONE__
  // prepare internal signal handler
  struct sigaction sig;
  sig.sa_sigaction = CsiSigHandler;
  sig.sa_flags     = SA_SIGINFO;
  
  if( sigaction(CSI_ERR_SIGNAL, &sig, NULL) != 0)
  {
    VDB_LOG_ERROR("Failed to register signal handler.\n");
    lRet = CSI_LIB_FAILURE;
  } // if signal not registered
#endif // #ifndef __STANDALONE__  
  
  if(lRet == CSI_LIB_SUCCESS)
  {
    // set the new handler
    spEventHandlerClass = apHandler;
    VDB_LOG_NOTE("Event handler class set.\n");
  } // if all ok so far

  return lRet;
} // CSI_EventHandlerSet()

//****************************************************************************
/**
* @brief          The csi signal handler
* @details        The csi signal handler
* 
* @param[in]      int
* @param[in]      siginfo_t
* @param[in]      void*
*
* @return         void 
*/
#ifndef __STANDALONE__
void CsiSigHandler(int, siginfo_t *apSigInfo, void*) 
{
  int *lpVal = &(apSigInfo->si_int);
#else // #ifndef __STANDALONE__
void CsiSigHandler(int aSigNumn, int aValue)
{ 
  uint32_t *lpVal = (uint32_t*)&aValue;
#endif //else #ifndef __STANDALONE__

  uint8_t lHandlerAssigned = 0U;

  // *** call apropriate handler ***
  if(sEventHandler != NULL)
  {
    // call the handler
    sEventHandler((uint32_t)*lpVal);
    lHandlerAssigned = 1U;
  } // if handler assigned
  
  if(spEventHandlerClass != NULL)
  {
    // call the handler
    spEventHandlerClass->CsiEventHandler((uint32_t)*lpVal);
    lHandlerAssigned = 1U;
  } // if handler assigned
  
  if(lHandlerAssigned == 0U)
  {
    VDB_LOG_WARNING("Event handler Object unassigned.\n");
  } // if handler unassigned
  
} //SeqSigHandler()

//*****************************************************************************

/*EOF*/
