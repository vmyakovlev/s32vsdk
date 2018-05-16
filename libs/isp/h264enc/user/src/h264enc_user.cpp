/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright (c) 2016-17, NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, 
 *   this list of conditions and the following disclaimer in the documentation 
 *   and/or other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*==============================================================================
Revision History:
                              Modification     Tracking
Author (core ID)              Date D/M/Y       Number     Description of Changes
---------------------------   ----------    ------------  ----------------------
Roman Kubica                  14/10/2016                  Initial Version
---------------------------   ----------    ------------  ----------------------
Tomas Babinec                 13/03/2017    VSDK-435      open/close call update
==============================================================================*/


/*==============================================================================
*                                        INCLUDE FILES 
==============================================================================*/
/**
* @file           h264enc_user.c
* @brief          User space interface for H264 Encoder driver. 
*/

#include <stdint.h>
#include <cstring>
#include <csignal>
#include <fcntl.h>    /* open */
#include <unistd.h>   /* exit */
#include <sys/ioctl.h>    /* ioctl */
#include <sys/mman.h>

#include "s32vs234.h"
#include "isp_h264enc.h"
#include "vdb_log.h"

/*==============================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/

/*==============================================================================
*                                       LOCAL MACROS
==============================================================================*/


/*==============================================================================
*                                      LOCAL CONSTANTS
==============================================================================*/
#define H264ENC_REG_SIZE 4096U

/*==============================================================================
*                                      LOCAL VARIABLES
==============================================================================*/

/* user registerred event handler */
static H264ENC_EventHandler_t sEventHandler = NULL; 
/* user registerred event handler */
static H264ENC_EventHandlerClass *spEventHandlerClass = NULL; 
static uint8_t sInitialized = 0;  
/* H264Enc driver file descriptor */
static int32_t sFileDesc = -1;    

static struct H264_ENC_tag *spEncReg = NULL;


/*==============================================================================
*                                      GLOBAL CONSTANTS
==============================================================================*/

/*==============================================================================
*                                      GLOBAL VARIABLES
==============================================================================*/


/*==============================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==============================================================================*/

void H264EncSigHandler(int32_t, siginfo_t *info, void *);

/*==============================================================================
*                                       LOCAL FUNCTIONS
==============================================================================*/

/*==============================================================================
*                                       GLOBAL FUNCTIONS
==============================================================================*/

/**
* @brief          Provides information of macroblock encoding status
* @details        Provides information of macroblock encoding status
*
* @param[in]      mbrw_status         

* @return         H264ENC_LIB_RESULT
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/

H264ENC_LIB_RESULT H264ENC_BS_NEW_ROW_Get(uint8_t &mbrw_status);

/**
* @brief         H264 encoder signal handles  
* @details       H264 encoder signal handles
*
* @param[in]      pfile         file pointer
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/
// void H264EncSigHandler(int, siginfo_t *info, void *);

/*============================================================================*/

/**
* @brief         H264Encoder registers mapping 
* @details       H264Encoder registers mapping 
*
* 
* @return         H264ENC_LIB_RESULT
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/

H264ENC_LIB_RESULT MapH264()
{
  H264ENC_LIB_RESULT lRet = H264ENC_LIB_SUCCESS;
  
  if(spEncReg == NULL)
  {
    int32_t Fd;
    Fd = open("/dev/mem", O_RDWR);
  
    if(Fd == -1)
    {
      VDB_LOG_ERROR("Failed to open /dev/mem\n");
      lRet = H264ENC_LIB_FAILURE;
      printf("Failed to open /dev/mem\n");
    } // if failed to open device   
    else
    {          
      spEncReg = (H264_ENC_tag*) mmap(NULL, 
                                      H264ENC_REG_SIZE, 
                                      PROT_READ | PROT_WRITE, 
                                      MAP_SHARED, 
                                      Fd, 
                                      0x400DC000);
      
      if(spEncReg == MAP_FAILED)
      {
        VDB_LOG_ERROR("Failed to map H264Enc registers.\n");
        lRet = H264ENC_LIB_FAILURE;
        printf("Failed to map H264Enc registers.\n");
      } // if mmap failed      
      
      if(close(Fd) != 0)
      {
        VDB_LOG_WARNING("Failed to close /dev/mem file.\n");
      } // if failed to close /dev/mem file
    } // else from if failed to open device
  } // if not mapped yet
        
  return lRet;  
}

/*============================================================================*/
/**
* @brief         H264Encoder FrameIrqDone
* @details       H264Encoder FrameIrqDone
*
* 
* @return         H264ENC_LIB_RESULT
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/

H264ENC_LIB_RESULT FrameIrqDone() 
{       
  H264ENC_LIB_RESULT lRet;
  volatile uint32_t i = 0;
  H264ENC_Status_t lEncStatus;
   
  (void)memset(&lEncStatus, 0, sizeof(H264ENC_Status_t));
  
  lRet = H264ENC_StatusGet(lEncStatus);
  
  while((lEncStatus.mInterruptStatus & 0x100U) != 0x100U)
  {
    
    if(i > 1000U)
    {
      break;
    }
    lRet = H264ENC_StatusGet(lEncStatus);
    i++;
  }
  
  spEncReg->ISR.R = 0x100U;
  lRet = H264ENC_LIB_SUCCESS;

  return lRet;
}
/*============================================================================*/
/**
* @brief         H264EncoderMacroblock row done status
* @details       H264EncoderMacroblock row done status
*
* 
* @return         H264ENC_LIB_RESULT
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/
H264ENC_LIB_RESULT BS_NEW_ROW_INT_control() 
{     
  uint8_t mbrw_parametr = 0U;
  H264ENC_LIB_RESULT lRet;

  while(mbrw_parametr != 1U)
  {
    H264ENC_BS_NEW_ROW_Get(mbrw_parametr);   
  }
  
  lRet = H264ENC_LIB_SUCCESS;

  return lRet;
}

/*============================================================================*/
/**
* @brief         H264Encoder Software reset
* @details       H264Encoder Software reset
*
* 
* @return        H264ENC_LIB_RESULT
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/

H264ENC_LIB_RESULT H264ENC_SwReset() 
{      
  H264ENC_LIB_RESULT lRet = H264ENC_LIB_SUCCESS;
  
  if(spEncReg)
  {
    spEncReg->MCR.B.SW_RESET     = 1U;
    sleep((uint32_t)1);
    spEncReg->MCR.B.SW_RESET     = 0U;
    spEncReg->ISR.R          = 0xFFFFFFFFU;
    spEncReg->VCR.B.FR_WIDTH         = 0x0U;   
    spEncReg->VCR.B.FR_HEIGHT        = 0x0U;  
    spEncReg->VI_CBUF_Y_ADDR.R       = 0x0U;
    spEncReg->VI_CBUF_CB_ADDR.R      = 0x0U;
    spEncReg->VI_CBUF_CR_ADDR.R      = 0x0U;
    spEncReg->VI_NRLINES.B.Y_LINES   = 0x0U;
    
    spEncReg->OUT_CBUF_START_ADDR.R  = 0x0U;
    spEncReg->OUT_CBUF_END_ADDR.R    = 0x0U;
    spEncReg->MCR.B.DATA_MODE_CHR    = 0x0U;
    spEncReg->MCR.B.FETCH_TRIG       = 0x0U;
  } // if regs mapped
  else
  {
    lRet = H264ENC_LIB_FAILURE;
  } // else from if regs mapped  

  return lRet;
}

/*============================================================================*/
/**
* @brief         H264Encoder LastFrameWait
* @details       Waits till the last frame is encoded. 
*
* 
* @return         H264ENC_LIB_RESULT
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/

H264ENC_LIB_RESULT Last_frame_wait() 
{  
  H264ENC_LIB_RESULT lRet;
  volatile uint32_t i = 0U;
 
  while(i < 1000000U)
  {
     i++;
  }  

  lRet = H264ENC_LIB_SUCCESS;

  return lRet;
}

/*============================================================================*/

/**
* @brief         H264ENC_Reserve
* @details       Open H264encoder device files  
*
* 
* @return        H264ENC_LIB_RESULT
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/
H264ENC_LIB_RESULT H264ENC_Reserve()
{
  H264ENC_LIB_RESULT lRet = H264ENC_LIB_SUCCESS;
  
  if(sFileDesc == -1)
  {   
    sFileDesc = open("/dev/h264enc", 0);
    
    if (sFileDesc == -1) 
    {
      VDB_LOG_ERROR("Can't open device file: %s\n", H264ENC_DEVICE_NAME);
      lRet = H264ENC_LIB_FAILURE;
    } // if open failed
    else
    {
      lRet = MapH264();
    } // else from if open failed
  } // if not opened yet
  else 
  {
    VDB_LOG_WARNING("Device file: %s opened already.\n", H264ENC_DEVICE_NAME);
  } // else from if not opened yet
  
  return lRet;
} // H264ENC_Reserve()

/*============================================================================*/

/**
* @brief         H264ENC_Release
* @details       Close H264 Encoder device files  
*
* 
* @return        H264ENC_LIB_RESULT
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/


void H264ENC_Release()
{
  
  if(spEncReg)
  {
    munmap(spEncReg, H264ENC_REG_SIZE);
    spEncReg = NULL;
  }// if regs mapped
  
  if(sFileDesc!= -1)
  {
    close(sFileDesc);
    sFileDesc = -1;
  } // if opened
  
} // H264ENC_Release()

/*============================================================================*/
/**
* @brief         Sets parameters for encoding
* @details       Sets parameters for encoding
*
* @param[in]     arcConfig struct with parametrs
* 
* @return        H264ENC_LIB_RESULT
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/


H264ENC_LIB_RESULT H264ENC_Configure(const H264ENC_Config_t &arcConfig)
{
  H264ENC_LIB_RESULT lRet = H264ENC_LIB_SUCCESS;
  
  int32_t lRetVal = 0;

  lRetVal = ioctl(sFileDesc, IOCTL_H264_ENC_CONFIG_SET, &arcConfig);
  if(lRetVal < 0)
  {
    VDB_LOG_ERROR("Encoder HW config failed.\n");
    lRet = H264ENC_LIB_FAILURE;
  } // if ioctl returned failure
  
  return lRet;
}

/*============================================================================*/
/**
* @brief         Sets buffers parameters for encoding
* @details       Sets buffers parameters for encoding. Addresses and sizes.
*
* @param[in]     arcBuffers struct with buffers parametrs
* 
* @return        H264ENC_LIB_RESULT
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/

H264ENC_LIB_RESULT H264ENC_BuffersSet(const H264ENC_Buffers_t &arcBuffers)
{
  H264ENC_LIB_RESULT lRet = H264ENC_LIB_SUCCESS;
  
  int32_t lRetVal = 0;

  lRetVal = ioctl(sFileDesc, IOCTL_H264_ENC_BUFFERS_SET, &arcBuffers);
  if(lRetVal < 0)
  {
    VDB_LOG_ERROR("Buffer setup failed. \n");
    lRet = H264ENC_LIB_FAILURE;
  } // if ioctl returned failure
  
  return lRet;
}

/*============================================================================*/
/**
* @brief         Fetches lines for encoding
* @details       Fetches lines for encoding  
*
* @param[in]     aMoreLines number of lines ready for encoding
* 
* @return        H264ENC_LIB_RESULT
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/

H264ENC_LIB_RESULT H264ENC_LinesFetch(uint32_t aMoreLines)
{
  H264ENC_LIB_RESULT lRet = H264ENC_LIB_SUCCESS;
  
  if(aMoreLines)
  {    
    int32_t lRetVal = ioctl(sFileDesc, IOCTL_H264_ENC_FETCH, aMoreLines);
    if(lRetVal < 0)
    {
      VDB_LOG_ERROR("Fetch %u lines failed.\n", aMoreLines);
      lRet = H264ENC_LIB_FAILURE;
    } // if ioctl returned failure
  } // if there are more lines
  else
  {
    VDB_LOG_WARNING("No lines to be fetched.\n");
    lRet = H264ENC_LIB_FAILURE;
  } // else from if there are more lines
  
  return lRet;
}

/*============================================================================*/
/**
* @brief         Shift value for alarm
* @details       Shift value for alarm
*
* @param[in]     aAlarmShift 
* 
* @return        H264ENC_LIB_RESULT
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/

H264ENC_LIB_RESULT H264ENC_AlarmShift(uint32_t aAlarmShift)
{
  H264ENC_LIB_RESULT lRet = H264ENC_LIB_SUCCESS;
    
  if(aAlarmShift)
  {    
    int32_t lRetVal = ioctl(sFileDesc, IOCTL_H264_ENC_BS_ALARM_SHIFT, 
                            aAlarmShift);   
   
    if(lRetVal < 0) 
    {
      VDB_LOG_ERROR("Faled to shift alarm addres by %u bytes. \n", \
      aAlarmShift);
      lRet = H264ENC_LIB_FAILURE;
    } // if ioctl returned failure
  } // if there are more lines
  else
  {
    VDB_LOG_WARNING("No shift.\n");
    lRet = H264ENC_LIB_FAILURE;
  } // else from if there are more lines
  
  return lRet;
}

/*============================================================================*/
/**
* @brief         Provides encoding status
* @details       Provides encoding status
*
* @param[in]     arStatus 
* 
* @return        H264ENC_LIB_RESULT
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/

H264ENC_LIB_RESULT H264ENC_StatusGet(H264ENC_Status_t &arStatus)
{
  H264ENC_LIB_RESULT lRet = H264ENC_LIB_SUCCESS; 
 
  int32_t lRetVal = 0;
  
  lRetVal = ioctl(sFileDesc, IOCTL_H264_ENC_STATUS_GET, &arStatus);
  if(lRetVal < 0)
  {
    VDB_LOG_ERROR("Failed to get current encoder status.\n");
    lRet = H264ENC_LIB_FAILURE;
  } // if ioctl returned failure
  
  return lRet;
}

/*============================================================================*/
/**
* @brief         Provides information if complete macroblock has been encoded
* @details       Provides information if complete macroblock has been encoded
*
* @param[in]     mbrw_status 
* 
* @return        H264ENC_LIB_RESULT
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/


H264ENC_LIB_RESULT H264ENC_BS_NEW_ROW_Get(uint8_t &mbrw_status)
{
  H264ENC_LIB_RESULT lRet = H264ENC_LIB_SUCCESS; 
 
  int32_t lRetVal = 0;
  
  lRetVal = ioctl(sFileDesc, IOCTL_H264_ENC_BS_ROW_DONE_GET, &mbrw_status);  
  if(lRetVal < 0)
  {
    VDB_LOG_ERROR("Failed to get MBRW_DONE status.\n");
    lRet = H264ENC_LIB_FAILURE;
  } // if ioctl returned failure
  
  return lRet;
}

/*============================================================================*/
/**
* @brief         Stops the encoder.
* @details       Stops the encoder and resets the encoder core.
*
* @param[in]     
* 
* @return        H264ENC_LIB_RESULT
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/

H264ENC_LIB_RESULT H264ENC_Stop()
{
  H264ENC_LIB_RESULT lRet = H264ENC_LIB_SUCCESS; 
  
  int32_t lRetVal = 0;
 
  lRetVal = ioctl(sFileDesc, IOCTL_H264_ENC_STOP);
  if(lRetVal < 0)  
  {
    VDB_LOG_ERROR("Failed to stop the encoding.\n");
    lRet = H264ENC_LIB_FAILURE;
  } // if ioctl returned failure
  
  return lRet;
}


/*============================================================================*/
/**
* @brief         Resets internal variable data
* @details       Resets internal variable data
*
* @param[in]     
* 
* @return        H264ENC_LIB_RESULT
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/

H264ENC_LIB_RESULT H264ENC_ResetVars()
{
  H264ENC_LIB_RESULT lRet = H264ENC_LIB_SUCCESS; 
 
  int16_t lRetVal = 0;
  
  lRetVal = ioctl(sFileDesc, IOCTL_H264_ENC_RESET_VARS);
  if(lRetVal < 0)
  {
    VDB_LOG_ERROR("Failed to stop the encoding.\n");
    lRet = H264ENC_LIB_FAILURE;
  } // if ioctl returned failure
  
  return lRet;
}

/*============================================================================*/
/**
* @brief         Internal signal handler
* @details       Internal signal handler
*
* @param[in]     aHandler 
* 
* @return        H264ENC_LIB_RESULT
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/


H264ENC_LIB_RESULT H264ENC_EventHandlerSet(const H264ENC_EventHandler_t 
                                           aHandler)
{
  H264ENC_LIB_RESULT lRet = H264ENC_LIB_SUCCESS;
  
  if(sInitialized == 0U)
  {      
    // prepare internal signal handler
    struct sigaction sig;
    sig.sa_sigaction = H264EncSigHandler;
    sig.sa_flags = SA_SIGINFO;
    
    if( (uint32_t)sigaction(H264ENC_FRAME_DONE_SIGNAL, &sig, NULL) != 0U)
    {
      VDB_LOG_ERROR("Failed to register Frame done signal handler.\n");
      lRet = H264ENC_LIB_FAILURE;
    } // if signal not registered
    else
    {
      sInitialized = 1U;
    } // else from if signal not registered
  } // if not initialized yet
  
  if(lRet == H264ENC_LIB_SUCCESS)
  {
    // set the new handler
    sEventHandler = aHandler;
    VDB_LOG_NOTE("Event handler set.\n");
  } // if all ok so far

  return lRet;
} // H264EncEventHandlerSet

/*============================================================================*/
/**
* @brief         Internal signal handler
* @details       Internal signal handler
*
* @param[in]     apHandler 
* 
* @return        H264ENC_LIB_RESULT
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/

H264ENC_LIB_RESULT H264ENC_EventHandlerSet(H264ENC_EventHandlerClass *apHandler)
{
  H264ENC_LIB_RESULT lRet = H264ENC_LIB_SUCCESS;
  
  if(sInitialized == 0U)
  {      
    // prepare internal signal handler
    struct sigaction sig;
    sig.sa_sigaction = H264EncSigHandler;
    sig.sa_flags = SA_SIGINFO;
    
    if( (uint32_t)(sigaction(H264ENC_FRAME_DONE_SIGNAL, &sig, NULL)) != 0U)
    {
      VDB_LOG_ERROR("Failed to register signal handler.\n");
      lRet = H264ENC_LIB_FAILURE;
    } // if signal not registered
    else
    {
      sInitialized = 1U;
    } // else from if signal not registered
  } // if not initialized yet
  
  if(lRet == H264ENC_LIB_SUCCESS)
  {
    // set the new handler
    spEventHandlerClass = apHandler;
    VDB_LOG_NOTE("Event handler class set.\n");
  } // if all ok so far

  return lRet;
} // H264ENC_EventHandlerSet

/*============================================================================*/
/**
* @brief         Internal signal handler
* @details       Internal signal handler
*
* @param[in]     apSigInfo 
* 
* @return        H264ENC_LIB_RESULT
* 
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/

void H264EncSigHandler(int32_t, siginfo_t *apSigInfo, void *) 
{
  VDB_LOG_NOTE("Received event %u\n", apSigInfo->si_int);
  uint8_t lHandlerAssigned = 0U;
  
  // *** call apropriate handler ***
  if(sEventHandler != NULL)
  {
    // call the handler
    sEventHandler((uint32_t)(apSigInfo->si_int));
    lHandlerAssigned = 1U;
  } // if handler assigned
  
  if(spEventHandlerClass != NULL)
  {
    // call the handler
    spEventHandlerClass->H264EncEventHandler((uint32_t)(apSigInfo->si_int));
    lHandlerAssigned = 1U;
  } // if handler assigned
  
  if(lHandlerAssigned == 0U)
  {
    VDB_LOG_WARNING("Event handler Object unassigned.\n");
  } // if handler unassigned
  
} //H264EncSigHandler

/*============================================================================*/


/*******************************************************************************
 * EOF
 ******************************************************************************/

/** @} */
  