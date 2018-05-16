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

/*==============================================================================
*                                 INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
/**
* @file            csi_ioctl.c
*/

#include <asm/uaccess.h>	// for get_user and put_user 
#include <linux/mutex.h>	// for semaphore/mutex
#include <linux/string.h>	// for memset
#include <linux/sched.h>	// for current->pid

#include "csi.h"
#include "csi_ioctl.h"
#include "csi_func.h"

#include "vdb_log.h"


/*==============================================================================
*                                MACROS
==============================================================================*/
// CSI locking mutex
static DEFINE_MUTEX(sCsiMutex);

/*==============================================================================
*                                GLOBAL FUNCTIONS
==============================================================================*/

/**
* @brief          Gets current error status
* @details        Gets current error status
*
* @param[in]      aIoctlParam the IOCT parameter.
* @return         int
* @api
* @isr
*
* @pre
*
* @post
*
* 
* @note
*/
static int32_t CsiErrStatusGet(unsigned long aIoctlParam);

/*============================================================================*/
/**
* @brief          The function manipulates the underlying device parameters.
* @details        The function manipulates the underlying device parameters of
*                 special files. In particular, many operating characteristics
*                 of character special files (e.g., terminals) may be controlled
*                 with viulite_ioctl() requests.
*
* @param[in]      pfile         file pointer
* @param[in]      ioctl_cmd     ioctl command
* @param[in]      arg           arg number
* @return         long
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/

long CsiIoctl( struct file *apFile,	/* ditto */
                unsigned int aIoctlNum,	/* number and param for ioctl */
                unsigned long aIoctlParam)
{
  int lRet = CSI_DRV_SUCCESS;

  // only one IOCTL can be processed at the time
  if(mutex_lock_interruptible(&sCsiMutex) > 0)
  {
    VDB_LOG_WARNING("CSI mutex down interrupted.\n");
    lRet = -ERESTARTSYS;
  } // if CSI mutex interrupted
  else
  {
    VDB_LOG_NOTE("CSI mutex locked.\n");

    // switch the ioctl command
    switch (aIoctlNum) {
      case IOCTL_CSI_CONFIG:
      {
        CSI_Cfg_t lCfg;
        if (copy_from_user( &lCfg, (CSI_Cfg_t *)aIoctlParam, 
            sizeof(CSI_Cfg_t)) != 0)
        {
          VDB_LOG_ERROR("Copy from user failed.\n");
          lRet = -EIO;
        } // if copy from user failed
        else
        {   
          CSI_DRV_Config(&lCfg);
        } // else from if copy from user failed
      } // case IOCTL_CSI_INIT
      break;
      
      case IOCTL_CSI_EMBD_CONFIG:
      {
        CSI_EmbdCfg_t lCfg;
        if (copy_from_user( &lCfg, (CSI_EmbdCfg_t *)aIoctlParam, 
            sizeof(CSI_EmbdCfg_t)) != 0)
        {
          VDB_LOG_ERROR("Copy from user failed.\n");
          lRet = -EIO;
        } // if copy from user failed
        else
        {   
          CSI_DRV_EmbdConfig(&lCfg);
        } // else from if copy from user failed
      } // case IOCTL_CSI_INIT
      break;
      
      case IOCTL_CSI_EMBD_RECAP:
      {
        enum CSI_IDX lCsiIdx = (enum CSI_IDX)aIoctlParam;
        CSI_DRV_EmbdRecap(lCsiIdx);
      } // case IOCTL_CSI_EMBD_RECAP
      break;
      
      case IOCTL_CSI_EMBD_STATUS_GET:
      {
        enum CSI_IDX lCsiIdx = (enum CSI_IDX)aIoctlParam;
        lRet = CSI_DRV_EmbdStatusGet(lCsiIdx);        
      } // case IOCTL_CSI_EMBD_STATUS_GET
      break;
      
      case IOCTL_CSI_ERR_STATUS_GET:
      {
        lRet = CsiErrStatusGet(aIoctlParam);                  
      } // IOCTL_CSI_ERR_STATUS_GET
      break;
      
      case IOCTL_CSI_ERR_CLEAR:
      {
        enum CSI_IDX lCsiIdx = (enum CSI_IDX)aIoctlParam;
        CSI_DRV_ErrClear(lCsiIdx);        
      } // case IOCTL_CSI_ERR_CLEAR
      break;

      case IOCTL_CSI_STOP:
      {
        CSI_IdKey_t lCsiIdKey;
        lCsiIdKey.mKey = (uint32_t)aIoctlParam;
        
        CSI_DRV_Stop(lCsiIdKey);       
      } // case IOCTL_CSI_STOP
      break;
      
      case IOCTL_CSI_START:
      {
        CSI_IdKey_t lCsiIdKey;
        lCsiIdKey.mKey = (uint32_t)aIoctlParam;
                
        CSI_DRV_Start(lCsiIdKey);       
      } // case IOCTL_CSI_START
      break;

      //***********************************************************************
      
      default:
      {
        VDB_LOG_WARNING("Unknown ioctl command.\n");
        
        lRet = -ENOTTY;
      } // default
        break;            
      
    } // switch ioctl command

    // relase the CSI mutex
    mutex_unlock(&sCsiMutex);
    VDB_LOG_NOTE("Csi mutex released.\n");
  }// else from if mutex down interrupted
  return lRet;
} // CsiIoctl()

/*============================================================================*/
/**
* @brief          The function privides CSI error status.
* @details        The function privides CSI error status.
*                 *
* @param[in]      aIoctlParam the IOCT parameter.
* @return         int
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/

static int32_t CsiErrStatusGet(unsigned long aIoctlParam)
{
  int32_t lRet = 0;
  CSI_ErrStatus_t lErrStat;
  
  if (copy_from_user( &lErrStat, (CSI_ErrStatus_t *)aIoctlParam, 
      sizeof(CSI_ErrStatus_t)) != 0)
  {
    VDB_LOG_ERROR("Copy from user failed.\n");
    lRet = -EIO;
  } // if copy from user failed
  else
  {
    // for input mOthrErrId used as CSI_IDX
    enum CSI_IDX lCsiIdx = (enum CSI_IDX) lErrStat.mOthrErrFrmId;
    if(lCsiIdx < CSI_IDX_INVALID)
    {
      CSI_ErrStatus_t *lpErrStat = CSI_DRV_ErrStatusGet(lCsiIdx);
      
      if (copy_to_user((CSI_ErrStatus_t*)aIoctlParam, 
                      lpErrStat,
                      sizeof(CSI_ErrStatus_t)) != 0)
      {
        VDB_LOG_ERROR("Copy to user failed.\n");
        lRet = -EIO;
      } // if copy from user failed
    } // if idx in range
    else
    {
      lRet = CSI_DRV_FAILURE;
      VDB_LOG_ERROR("CSI idx out of range.\n");
    }// else from if idx in range
  } // else from if copy from user failed
    
  return lRet;
} // CsiErrStatusGet()

//***************************************************************************