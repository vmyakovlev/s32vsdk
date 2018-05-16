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
* \file     gdi_com_privat.hpp
* \brief    privat gdi api server/client communication functionality header
* \author   Tomas Babinec
* \version  0.1
* \date     31.7.2013
* \note
****************************************************************************/

#ifndef GDICOMPRIVATE_HPP
#define GDICOMPRIVATE_HPP

#include <global_errors.h>

/****************************************************************************/
/** Initializes server side of GDI API.
 *
 * Creates server procces, creates and opens shared memory and required
 * semaphores. Starts message loop.
 *
 * \return LIB_SUCCESS after fork if client (= parent process). If on server
 * side after fork LIB_FAILURE returned if rror is encountered during init.
 * Otherwise doesn't return at all - calls _exit(0).
 *****************************************************************************/
static LIB_RESULT SrvInit();

/****************************************************************************/
/** Server loop for clients' message processing.
*
* Waits on server semaphor for clients' messages, and calls message processing
* function. Runs until last client deinitilizes.
*
* \return LIB_SUCCESS when last client deinit message processed. LIB_FAILURE
* if some error encountered.
*****************************************************************************/
static LIB_RESULT SrvLoop();

/****************************************************************************/
/** Clients' messages processing.
*
* Calls message handler appropriate to specified message type.
*
* \param aCliCnt reference to an output variable where current clients' count
* will be set if GDI_MSG_CLIENT_DEINIT is called.
*
* \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
*****************************************************************************/
static LIB_RESULT SrvMsgProc(uint32_t &aCliCnt);

/****************************************************************************/
/** Test message handler.
*
* For GDI communication testing purposes only. Might be removed in the future.
*
* \return LIB_SUCCESS always.
*****************************************************************************/
static LIB_RESULT SrvMsgProcTest();

/****************************************************************************/
/** Close & unlink all opened server and shared resources.
*
* Called after last client deinit message processed. Server shutdown follows.
*
* \return LIB_SUCCESS of all ok, otherwise LIB_FAILURE.
*****************************************************************************/

static LIB_RESULT SrvClose(void);
/****************************************************************************/
/** Client deinit message handler.
*
* Finds out current number of clients. If
*
* \param aA first summand
* \param aB second summand
*
* \return sum of aA and aB
*
*****************************************************************************/
static LIB_RESULT ClientDeInit(uint32_t &aClientCnt);


#endif /* GDICOMPRIVATE_HPP */
