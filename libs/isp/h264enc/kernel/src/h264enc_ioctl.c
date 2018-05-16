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

/*==============================================================================
Revision History:
                              Modification     Tracking
Author (core ID)              Date D/M/Y       Number     Description of Changes
---------------------------   ----------    ------------  ----------------------
Roman Kubica                  14/10/2016                  Initial Version
---------------------------   ----------    ------------  ----------------------
==============================================================================*/

#ifdef __cplusplus
extern "C"{
#endif

/*==============================================================================
*                                        INCLUDE FILES
==============================================================================*/

/**
* @file     h264enc_ioctl.c
* @brief    IOCTL functionality of H264 Encoder driver
*/

#include <asm/uaccess.h>	/* for get_user and put_user */
#include <linux/string.h>	/* for memset */
#include <linux/sched.h>	/* for current->pid */
#include "h264enc.h"  
#include "h264enc_ioctl.h"
#include "h264enc_func.h"
#include "vdb_log.h"

/*==============================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/


/*==============================================================================
*                               LOCAL MACROS
==============================================================================*/

static DEFINE_MUTEX(sH264EncMutex);

/*==============================================================================
*                               LOCAL CONSTANTS
==============================================================================*/


/*==============================================================================
*                               LOCAL VARIABLES
==============================================================================*/


/*==============================================================================
*                               GLOBAL CONSTANTS
==============================================================================*/


/*==============================================================================
*                               GLOBAL VARIABLES
==============================================================================*/


/*==============================================================================
*                               LOCAL FUNCTION PROTOTYPES
==============================================================================*/


/*==============================================================================
*                               LOCAL FUNCTIONS
==============================================================================*/

/**
* @brief          The function manipulates the underlying device parameters.
* @details        The function manipulates the underlying device parameters of
*                 special files. In particular, many operating characteristics
*                 of character special files (e.g., terminals) may be controlled
*                 with fsl_viulite_ioctl() requests.
** 
* @param[in]      apFile    file pointer
* @param[in]      aIoctlNum   ioctl number
* @param[in]      aIoctlParam   ioctl parameter
* 
* @return         long
* @api
* @isr
*
* @pre           
* @post
*
*
* @note
*/

long H264EncIoctl(struct file *apFile, /* ditto */
  unsigned int aIoctlNum,	/* number and param for ioctl */
  unsigned long aIoctlParam)
{
  int lRet = H264ENC_DRV_SUCCESS;
   /* only one IOCTL can be processed at the time */
  if(mutex_lock_interruptible(&sH264EncMutex) > 0)
  {
    VDB_LOG_WARNING("H264Enc mutex down interrupted.\n");
    lRet = -ERESTARTSYS;
  } /* if H264Enc mutex interrupted */
  else
  {
  
    switch (aIoctlNum) /* switch the ioctl command */
    { 
      case IOCTL_H264_ENC_CONFIG_SET:
      {
        H264ENC_Config_t lConfig;
        
        if (copy_from_user( &lConfig, (H264ENC_Config_t *)aIoctlParam, 
          sizeof(H264ENC_Config_t)) != 0)
        {
          VDB_LOG_ERROR("Copy from user failed.\n");
          lRet = -EIO;
        } /* if copy from user failed */
        else
        {
          H264ENC_Config(&lConfig);
          VDB_LOG_NOTE("Config done.\n");
        } /* else from if copy from user failed */            
      }
        break;
        
      case IOCTL_H264_ENC_BUFFERS_SET:
      {
        H264ENC_Buffers_t lBuffers;
        
        if (copy_from_user( &lBuffers, (H264ENC_Buffers_t *)aIoctlParam, \
          sizeof(H264ENC_Buffers_t)) != 0)
        {
          VDB_LOG_ERROR("Copy from user failed.\n");
          lRet = -EIO;
        } /* if copy from user failed */
        else
        {
          H264ENC_BuffersSet(&lBuffers);
          VDB_LOG_NOTE("Buffers set.\n");
        } /* else from if copy from user failed */  
      }
        break;
        
      case IOCTL_H264_ENC_FETCH:
      {
        H264ENC_LinesFetch((uint32_t)aIoctlParam);
        VDB_LOG_NOTE("Fetch invoked.\n");
      }
        break;
        
      case IOCTL_H264_ENC_BS_ALARM_SHIFT:
      {
        H264ENC_BitstreamAlarmShift((uint32_t)aIoctlParam);
      }
        break;
        
      case IOCTL_H264_ENC_STATUS_GET:
      {
        H264ENC_Status_t lStatus;            
        H264ENC_StatusGet(&lStatus);
        
        if (copy_to_user((H264ENC_Status_t *)aIoctlParam, \
          &lStatus, \
          sizeof(H264ENC_Status_t)) != 0)
        {
          VDB_LOG_ERROR("Copy to user failed.\n");
          lRet = -EIO;
        } /* if copy to user failed */
        else
        {
          VDB_LOG_NOTE("Status fetched.\n");
          lRet = H264ENC_DRV_SUCCESS;
        }
      }
        break;
        
      case IOCTL_H264_ENC_STOP:
      {          
        H264ENC_Stop();      
        VDB_LOG_NOTE("Encoding stopped");
        
      }
        break;	    

      case IOCTL_H264_ENC_RESET_VARS:
      {          
        H264ENC_ResetVars();      
        VDB_LOG_NOTE("Encoding stopped");
        
      }
        break;   

      case IOCTL_H264_ENC_BS_ROW_DONE_GET:
      {
        uint8_t mbrw_done_status = H264ENC_RowDoneStatusGet();          
      
        if (copy_to_user((uint8_t *)aIoctlParam, \
        &mbrw_done_status, \
        sizeof(uint8_t)) != 0)
        {
          VDB_LOG_ERROR("Copy to user failed.\n");
          lRet = -EIO;
        } // if copy to user failed
        else
        {
          VDB_LOG_NOTE("Status fetched.\n");
          lRet = H264ENC_DRV_SUCCESS;
        }
      }
        break;            

      default:
        VDB_LOG_WARNING("Unknown ioctl command.\n");
        lRet = -ENOTTY;
        break;
    } /* switch ioctl command */

  /* relase the H264Enc mutex */
  mutex_unlock(&sH264EncMutex);
  VDB_LOG_NOTE("H264Enc mutex released.\n");
  }
  
  return lRet;
} /* H264EncIoctl() */

/*==============================================================================
*                                       GLOBAL FUNCTIONS
==============================================================================*/

#ifdef __cplusplus
}
#endif


/*******************************************************************************
 * EOF
 ******************************************************************************/

/** @} */