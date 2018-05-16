/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright (c) 2016-2017  NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
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

/*=============================================================================
Revision History:
                        Modification       Tracking 
Author (core ID)        Date D/M/Y         Number     Description of Changes
Tomas Babinec           18-Sep-2015                   Init version
Xuyen Dao               05-Aug-2016        VSDK-251   Update template
Cristian Tomescu        17-Oct-2016        VSDK-270   Adding the user space drv
Cristian Tomescu        04-Nov-2016        VSDK-488   Split the core drv
Cristian Tomescu        15-Nov-2016        VSDK-495   Remove the 'fsl'
Tomas Babinec           13-Mar-2017        VSDK-435   open/close call update
Cristian Tomescu        08-May-2017        VSDK-560   Fix MISRA violations
=============================================================================*/

#ifdef __cplusplus
extern "C"{
#endif

/*==============================================================================
*                                 INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
/**
* @file       viulite_user.cpp
*/

#ifndef __STANDALONE__
  #include <fcntl.h>        // open 
  #include <unistd.h>       // exit 
  #include <sys/ioctl.h>    // ioctl 
  #include <sys/mman.h>     // mmap
#else
  #include "viu_core.h"
#endif // else from #ifndef __STANDALONE__

#include "isp_viu.h"
#include "vdb_log.h"

/*==============================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/

/*==============================================================================
*                               LOCAL MACROS
==============================================================================*/
#define VIU0_DEVICE_NAME "/dev/viulite0"
#define VIU1_DEVICE_NAME "/dev/viulite1"

/*==============================================================================
*                       STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/

/*==============================================================================
*                                  LOCAL CONSTANTS
==============================================================================*/


/*==============================================================================
*                                  LOCAL VARIABLES
==============================================================================*/
#ifndef __STANDALONE__
  static int32_t spViuFd[2] = {-1, -1};    ///< viu driver file descriptors
#endif //else from #ifndef __STANDALONE__


/*==============================================================================
*                                 GLOBAL CONSTANTS
==============================================================================*/


/*==============================================================================
*                                 GLOBAL VARIABLES
==============================================================================*/


/*==============================================================================
*                                LOCAL FUNCTION PROTOTYPES
==============================================================================*/


/*==============================================================================
*                                       LOCAL FUNCTIONS
==============================================================================*/


/*==============================================================================
*                                       GLOBAL FUNCTIONS
==============================================================================*/

/*============================================================================*/
/**
* @brief          The function Viu Open
* @details        The function Viu Open
*
* @param[in]      aViuIdx    Viulite Id
*
* @return         VIU_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     VIU_Open_Activity
*
* @note
*/
VIU_LIB_RESULT VIU_Open(const VIU_IDX aViuIdx)
{
  VIU_LIB_RESULT lRet = VIU_LIB_SUCCESS;

#ifndef __STANDALONE__  
  const char *lpcDevName = 
                 (aViuIdx == VIU_IDX_0)?VIU0_DEVICE_NAME:VIU1_DEVICE_NAME;

  if(spViuFd[aViuIdx] == -1)
  {  
    spViuFd[aViuIdx] = open(lpcDevName, 0);

    if (spViuFd[aViuIdx] == -1) 
    {
      VDB_LOG_ERROR("Can't open device file: %s\n", lpcDevName);
      lRet = VIU_LIB_FAILURE;
    } // if open failed
  } // if not opened yet
  else 
  {
    VDB_LOG_WARNING("Device file: %s opened already.\n", lpcDevName);
  } // else from if not opened yet
#else // from ifndef __STANDALONE__
  VDB_LOG_NOT_IMPLEMENTED();
  lRet = VIU_LIB_FAILURE;
#endif // #ifndef __STANDALONE__
  
  return lRet;
} // VIU_Open()

/*============================================================================*/
/**
* @brief          The function Viu Close
* @details        The function Viu Close
*
* @param[in]      aViuIdx    Viulite Id
*
* @return         VIU_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     VIU_Close_Activity
*
* @note
*/
VIU_LIB_RESULT VIU_Close(const VIU_IDX aViuIdx)
{
  VIU_LIB_RESULT lRet = VIU_LIB_SUCCESS;

#ifndef __STANDALONE__  

  if(spViuFd[aViuIdx] != -1)
  {
    if (close(spViuFd[aViuIdx]) != 0) 
    {
      VDB_LOG_ERROR("Failed to close device file %s.\n", 
                    (aViuIdx == VIU_IDX_0)?VIU0_DEVICE_NAME:VIU1_DEVICE_NAME);
      lRet = VIU_LIB_FAILURE;
    } // if open failed
    else
    {
      spViuFd[aViuIdx] = -1;
    } // else if open failed
  } // not openned yet
  else 
  {
    VDB_LOG_WARNING("Viu already closed.\n");
  } // else from if not openned yet
#else // from ifndef __STANDALONE__
  VDB_LOG_NOT_IMPLEMENTED();
  lRet = VIU_LIB_FAILURE;
#endif // #ifndef __STANDALONE__
  return lRet;
} // VIU_Close()

/*============================================================================*/
/**
* @brief          The function Viu Config
* @details        The function Viu Config
*
* @param[in]      aViuIdx                Viulite Id
* @param[in]      apcViuDmaConfig        Viulite Dma config struct
* @param[in]      apcViuDataInterface    Viulite Data interface struct
* @param[in]      apcViuInputFormat      Viulite input format struct
* @param[in]      aIrqMask               input interrupt mask
*
* @return         VIU_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     VIU_Config_Activity
*
* @note
*/
VIU_LIB_RESULT VIU_Config(const VIU_IDX aViuIdx,
                          DMA_CONFIG *apcViuDmaConfig,
                          VIU_DATA_INTERFACE *apcViuDataInterface,
                          VIU_INPUT_FORMAT *apcViuInputFormat,
                          const uint8_t aIrqMask
                         )
{
  VIU_LIB_RESULT lRet = VIU_LIB_SUCCESS;

#ifndef __STANDALONE__  

  if((apcViuDmaConfig != NULL) && 
     (apcViuDataInterface != NULL) &&
     (apcViuInputFormat != NULL))
  {
    if(spViuFd[aViuIdx] == -1)
    {
      VDB_LOG_ERROR("Viu device file not opened.\n");
      lRet = VIU_LIB_FAILURE;
    } // if Viu not opened
    else
    {
      int32_t lRetVal = ioctl(spViuFd[aViuIdx], VIULITE_IOCTL_DMA_CONFIG, 
                                                              apcViuDmaConfig);
      if ((lRetVal < 0))
      {
        VDB_LOG_ERROR("Failed to config VIU DMA.\n");
        lRet = VIU_LIB_FAILURE;
      } // if ioctl returned failure
      else
      {
        lRetVal = ioctl(spViuFd[aViuIdx], VIULITE_IOCTL_SET_DATA_INTERFACE, 
                                                           apcViuDataInterface);
        if (lRetVal < 0)
        {
          VDB_LOG_ERROR("Failed to config VIU data interface.\n");
          lRet = VIU_LIB_FAILURE;
        } // if ioctl returned failure
        else
        {
          lRetVal = ioctl(spViuFd[aViuIdx], VIULITE_IOCTL_SET_VIDEOIN_FORMAT, 
                                                             apcViuInputFormat);

          if (lRetVal < 0)
          {
            VDB_LOG_ERROR("Failed to config VIU video format.\n");
            lRet = VIU_LIB_FAILURE;
          } // if ioctl returned failure
          else
          {
            lRetVal = ioctl(spViuFd[aViuIdx], VIULITE_IOCTL_CONFIG_IRQS, 
                                                                     &aIrqMask);
            if (lRetVal < 0)
            {
              VDB_LOG_ERROR("Failed to configure VIU irq mask.\n");
              lRet = VIU_LIB_FAILURE;
            } // if ioctl returned failure
            else
            {
              lRet = VIU_LIB_SUCCESS;
            } // if ioctl returned OK
          } // if ioctl returned OK
        } // if ioctl returned OK
      } // if ioctl returned OK
    } // else from if Viu not opened
  }// if apcViuCfg exists
  else
  {
    VDB_LOG_ERROR("Wromg configuration data.\n");
    lRet = VIU_LIB_FAILURE;
  } // if ioctl returned failure
#else // from ifndef __STANDALONE__
  VDB_LOG_NOT_IMPLEMENTED();
  lRet = VIU_LIB_FAILURE;
#endif // #ifndef __STANDALONE__

  return lRet;
} // VIU_Config(const VIU_Cfg_t *apcViuCfg)

/*============================================================================*/
/**
* @brief          The function Viu Interrupt config
* @details        The function Viu Interrupt config
*
* @param[in]      aViuIdx    Viulite Id
* @param[in]      aIrqMask   Input Interrupt mask
*
* @return         VIU_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     VIU_IrqConfig_Activity
*
* @note
*/
VIU_LIB_RESULT VIU_IrqConfig(const VIU_IDX aViuIdx, const uint8_t aIrqMask)
{
  VIU_LIB_RESULT lRet = VIU_LIB_SUCCESS;

#ifndef __STANDALONE__

  if(spViuFd[aViuIdx] == -1)
  {
    VDB_LOG_ERROR("Viu device file not opened.\n");
    lRet = VIU_LIB_FAILURE;
  } // if Viu not opened
  else
  {
    int32_t lRetVal = 0;
    lRetVal = ioctl(spViuFd[aViuIdx], VIULITE_IOCTL_CONFIG_IRQS, &aIrqMask);

    if (lRetVal < 0)
    {
      VDB_LOG_ERROR("Failed to configure VIU irq mask.\n");
      lRet = VIU_LIB_FAILURE;
    } // if ioctl returned failure
  } // else from if Viu not opened

#else // from ifndef __STANDALONE__
  VDB_LOG_NOT_IMPLEMENTED();
  lRet = VIU_LIB_FAILURE;
#endif // #ifndef __STANDALONE__

  return lRet;
} // VIU_Config(const VIU_Cfg_t *apcViuCfg)        
  
/*============================================================================*/
/**
* @brief          The function Viu Start
* @details        The function Viu Start
*
* @param[in]      aViuIdx    Viulite Id
*
* @return         VIU_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     VIU_Start_Activity
*
* @note
*/
VIU_LIB_RESULT VIU_Start(const VIU_IDX aViuIdx)
{
  VIU_LIB_RESULT lRet = VIU_LIB_SUCCESS;

#ifndef __STANDALONE__   
  int32_t lRetVal = 0;
  uint8_t lIrqMask = (uint8_t)2;
  // start VIU receiving by enabling VIU interrupts
  lRetVal = ioctl(spViuFd[aViuIdx], VIULITE_IOCTL_CONFIG_IRQS, &lIrqMask);

  if (lRetVal < 0)
  {
    VDB_LOG_ERROR("Failed to start VIU interface.\n");
    lRet = VIU_LIB_FAILURE;
  } // if ioctl returned failure

#else // from ifndef __STANDALONE__
  VDB_LOG_NOT_IMPLEMENTED();
  lRet = VIU_LIB_FAILURE;
#endif // #ifndef __STANDALONE__

  return lRet;
} // VIU_Start(VIU_IdKey_t aViuIdKey)

/*============================================================================*/
/**
* @brief          The function Viu Stop
* @details        The function Viu Stop
*
* @param[in]      aViuIdx    Viulite Id
*
* @return         VIU_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     VIU_Stop_Activity
*
* @note
*/
VIU_LIB_RESULT VIU_Stop(const VIU_IDX aViuIdx)
{
  VIU_LIB_RESULT lRet = VIU_LIB_SUCCESS;

#ifndef __STANDALONE__   
  int32_t lRetVal = 0;
  uint8_t lIrqMask = (uint8_t)0;
  // stop VIU receiving by disabling VIU interrupts
  lRetVal = ioctl(spViuFd[aViuIdx], VIULITE_IOCTL_CONFIG_IRQS, &lIrqMask); 

  if (lRetVal < 0) 
  {
    VDB_LOG_ERROR("Failed to start VIU interface.\n");
    lRet = VIU_LIB_FAILURE;
  } // if ioctl returned failure

#else // from ifndef __STANDALONE__
  VDB_LOG_NOT_IMPLEMENTED();
  lRet = VIU_LIB_FAILURE;
#endif // #ifndef __STANDALONE__

  return lRet;
} // VIU_Stop(VIU_IdKey_t aViuIdKey)

/*============================================================================*/
/**
* @brief          The function Viu Set or Reset
* @details        The function Viu Set or Reset
*
* @param[in]      aViuIdx    Viulite Id
*
* @return         VIU_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     VIU_SwReset_Activity
*
* @note
*/
VIU_LIB_RESULT VIU_SwReset(const VIU_IDX aViuIdx)
{
  VIU_LIB_RESULT lRet = VIU_LIB_SUCCESS;

#ifndef __STANDALONE__   
    int32_t lRetVal = 0;
    // stop VIU receiving by disabling VIU interrupts
    lRetVal = ioctl(spViuFd[aViuIdx], VIULITE_IOCTL_SW_RESET, 0);

    if (lRetVal < 0) 
    {
      VDB_LOG_ERROR("Failed to sw reset viu.\n");
      lRet = VIU_LIB_FAILURE;
    } // if ioctl returned failure

#else // #ifndef __STANDALONE__
    VDB_LOG_NOT_IMPLEMENTED();
#endif // else #ifndef __STANDALONE__
  
  return lRet;
} // VIU_Stop(VIU_IdKey_t aViuIdKey)

/*============================================================================*/
/**
* @brief          The function Viu Dma Start
* @details        The function Viu Dma Start
*
* @param[in]      aViuIdx    Viulite Id
*
* @return         VIU_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     VIU_DmaStart_Activity
*
* @note
*/
VIU_LIB_RESULT VIU_DmaStart(const VIU_IDX aViuIdx)
{
  VIU_LIB_RESULT lRet = VIU_LIB_SUCCESS;

#ifndef __STANDALONE__   
    int32_t lRetVal = 0;
    // stop VIU receiving by disabling VIU interrupts
    lRetVal = ioctl(spViuFd[aViuIdx], VIULITE_IOCTL_DMA_START); 

    if (lRetVal < 0) 
    {
      VDB_LOG_ERROR("Failed to start DMA.\n");
      lRet = VIU_LIB_FAILURE;
    } // if ioctl returned failure

#else // #ifndef __STANDALONE__
    VDB_LOG_NOT_IMPLEMENTED();
#endif // else #ifndef __STANDALONE__

  return lRet;
} // VIU_DmaStart(VIU_IdKey_t aViuIdKey)

/*============================================================================*/
/**
* @brief          The function Viu Dma Stop
* @details        The function Viu Dma Stop
*
* @param[in]      aViuIdx    Viulite Id
*
* @return         VIU_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     VIU_DmaStop_Activity
*
* @note
*/
VIU_LIB_RESULT VIU_DmaStop(const VIU_IDX aViuIdx)
{
  VIU_LIB_RESULT lRet = VIU_LIB_SUCCESS;

#ifndef __STANDALONE__   
    int32_t lRetVal = 0;
    // stop VIU receiving by disabling VIU interrupts
    lRetVal = ioctl(spViuFd[aViuIdx], VIULITE_IOCTL_DMA_STOP);

    if (lRetVal < 0)
    {
      VDB_LOG_ERROR("Failed to stop DMA.\n");
      lRet = VIU_LIB_FAILURE;
    } // if ioctl returned failure

#else // #ifndef __STANDALONE__
    VDB_LOG_NOT_IMPLEMENTED();
#endif // else #ifndef __STANDALONE__

  return lRet;
} // VIU_DmaStop(VIU_IdKey_t aViuIdKey)

#ifdef __cplusplus
}
#endif


/*******************************************************************************
 * EOF
 ******************************************************************************/

/** @} */
