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
 * \file     sram_user.c
 * \brief    User space access functions to sram driver.
 * \author   Tomas Babinec
 * \version  0.1
 * \date     11.11.2014
 * \note
 ****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>		// memset
#include <unistd.h>		// exit 

#include <stdint.h>

#ifndef __STANDALONE__
// linux only
#include <signal.h>
#include <fcntl.h>		// open 
#include <sys/types.h>
#include <sys/ioctl.h>		// ioctl 
#else //#ifndef __STANDALONE__
#include <stdio.h>
#include <stdint.h>
#include <string.h>
// only for UBOOT
//#include <common.h>
//#include <exports.h>
#endif //#ifndef __STANDALONE__

#include "sram_func.h"

#include "isp_sram.h"

//****************************************************************************
// global variables
//****************************************************************************

#ifndef __STANDALONE__
static int32_t sFileDesc = -1;		///< driver file descriptor
#endif // #ifndef __STANDALONE__

//****************************************************************************
// local function declarations
//****************************************************************************

//****************************************************************************
// function definitions
//****************************************************************************

SRAM_LIB_RESULT SramOpen()
{
  
  SRAM_LIB_RESULT lRet = SRAM_LIB_SUCCESS;
#ifndef __STANDALONE__
  if(sFileDesc == -1)
  {
    sFileDesc = open("/dev/" SRAM_DEVICE_NAME, 0);
    if (sFileDesc < 0) {
      VDB_LOG_ERROR("Can't open device file: %s\n", SRAM_DEVICE_NAME);
      lRet = SRAM_LIB_FAILURE;
    } // if open failed
  } // not openned yet
  else 
  {
    VDB_LOG_WARNING("SRAM already openned.\n");
  } // else from if not openned yet
#else // #ifndef __STANDALONE__
  if(SRAM_SetUp() != SRAM_DRV_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to setup SRAM manager\n");
    lRet = SRAM_LIB_FAILURE;
  }
#endif  // #ifndef __STANDALONE__
  
  return lRet;
} // SramOpen()

//****************************************************************************

SRAM_LIB_RESULT SramClose()
{
  SRAM_LIB_RESULT lRet = SRAM_LIB_SUCCESS;
#ifndef __STANDALONE__
  if(sFileDesc == -1)
  {
    VDB_LOG_WARNING("Sram already closed.\n");
  } // not openned yet
  else 
  {
    if (close(sFileDesc) != 0) {
      VDB_LOG_ERROR("Failed to close device file %s.\n", SRAM_DEVICE_NAME);
      lRet = SRAM_LIB_FAILURE;
    } // if close() failed
    else
    {
      sFileDesc = -1;
    } // else if close() failed
  } // else from if not openned yet
#else // #ifndef __STANDALONE__
  SRAM_Close();
#endif // #ifndef __STANDALONE__
  
  return lRet;
} // SramClose()

//****************************************************************************

uintptr_t SramSingleMalloc(uintptr_t aSize)
{
  int ret_val = 0;

#ifndef __STANDALONE__
  ret_val = ioctl(sFileDesc, IOCTL_SRAM_S_MALLOC, &aSize);
#else // #ifndef __STANDALONE__
  aSize = SRAM_SingleMalloc(aSize);
#endif // #ifndef __STANDALONE__

  if (ret_val < 0) 
  {
    VDB_LOG_ERROR("SramSingleMalloc failed:%d\n", ret_val);
    return 0;
  } // if ioctl returned failure
  
  return aSize;
} // SramSingleMalloc()

//****************************************************************************

uintptr_t SramMultiMalloc(uintptr_t aSize)
{
  int ret_val = 0;

#ifndef __STANDALONE__
  ret_val = ioctl(sFileDesc, IOCTL_SRAM_M_MALLOC, &aSize);
#else // #ifndef __STANDALONE__
  aSize = SRAM_MultiMalloc(aSize);
#endif // #ifndef __STANDALONE__

  if (ret_val < 0) 
  {
    VDB_LOG_ERROR("SramMultiMalloc failed:%d\n", ret_val);
    return 0;
  } // if ioctl returned failure
  
  return aSize;
} // SramMultiMalloc()

//****************************************************************************

void SramFree(uintptr_t aAddr)
{
  int ret_val = 0;

  if((aAddr >= SRAM_BASE) && (aAddr < (SRAM_BASE + SRAM_SIZE)))
#ifndef __STANDALONE__
  ret_val = ioctl(sFileDesc, IOCTL_SRAM_FREE, &aAddr);
#else // #ifndef __STANDALONE__
  SRAM_Free(aAddr);
#endif // #ifndef __STANDALONE__

  if (ret_val < 0) 
  {
    VDB_LOG_ERROR("SramFree failed:%d\n", ret_val);
    return;
  } // if ioctl returned failure
} // SramFree()

//****************************************************************************