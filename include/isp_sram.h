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
 * \file     sram_public.h
 * \brief    Declarations of user space access functions to sram driver.
 * \author   Tomas Babinec
 * \version  0.1
 * \date     11.11.2014
 * \note
 ****************************************************************************/

#ifndef SRAMPUBLIC_H
#define SRAMPUBLIC_H

#include "../libs/isp/sram/kernel/include/sram.h"

#include "vdb_log.h"

//*****************************************************************************
// types
//*****************************************************************************

enum SRAM_LIB_RESULT
{
  SRAM_LIB_SUCCESS = 0,
  SRAM_LIB_FAILURE = 1
}; // SRAM_LIB_RESULT

//****************************************************************************
// function declarations
//****************************************************************************

/****************************************************************************/
/** Enables use of SRAM manager functionality from the app.
 * 
 * In Linux opens the SRAM driver special device file.
 * In standalone environment directly invokes SRAM_SetUp function to prepare for
 * future SRAM allocation management. 
 * 
 * \return SRAM_LIB_SUCCESS if all ok
 *         SRAM_LIB_FAILURE if failed
 ****************************************************************************/
SRAM_LIB_RESULT SramOpen(void);

/****************************************************************************/
/** Disables use of SRAM manager functionality from the app.
 * 
 * In Linux closes the SRAM driver special device file.
 * In standalone environment directly invokes SRAM_Close function to destroy
 * SRAM allocation management metadata. 
 * 
 * \return SRAM_LIB_SUCCESS if all ok
 *         SRAM_LIB_FAILURE if failed
 ****************************************************************************/
SRAM_LIB_RESULT SramClose(void);

/****************************************************************************/
/** Allocates requested number of bytes from the single-banked region of the 
 * SRAM.
 * 
 * \param aSize number of bytes to be allocated.
 * 
 * \return physical address of the base of the memory block that was allocated 
 *         in SRAM.
 *         0 if failed
 ****************************************************************************/
uintptr_t SramSingleMalloc(uintptr_t aSize);

/****************************************************************************/
/** Allocates requested number of bytes from the multi-banked region of the 
 * SRAM.
 * 
 * \param aSize number of bytes to be allocated.
 * 
 * \return physical address of the base of the memory block that was allocated 
 *         in SRAM.
 *         0 if failed
 ****************************************************************************/
uintptr_t SramMultiMalloc(uintptr_t aSize);

/****************************************************************************/
/** Releases memory previously allocated through SramMulti/SingleMalloc() calls.
 * 
 * \param aAddr address (received through SramMulti/SingleMalloc() calls) of 
 *              the memory block to be released.
 ****************************************************************************/
void SramFree(uintptr_t aAddr);

//****************************************************************************

#endif /* SRAMPUBLIC_H */
