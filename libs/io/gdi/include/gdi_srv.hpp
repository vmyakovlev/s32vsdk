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
* \file 		gdi_srv.hpp
* \brief 		gdi server proccess exported functionality
* \author 	Tomas Babinec
* \version 	0.1
* \date 		05-September-2013
****************************************************************************/

#ifndef GDISRV_HPP
#define GDISRV_HPP

#include <stddef.h>
#include "oal_process.h"

/*****************************************************************************
* shared variables
*****************************************************************************/

/// general shared memory name
static const char *spOalShmName = "gdiMgen";
/// general shared memory representation
static OAL_SHARED_MEM sOalShm;

///  pointer to beginning of whole general shared memory mapping address
static void* spShm = NULL;
///  pointer to ServerClient stats at the beginning of general shared memory
static GDI_SrvCliStats* spSrvCliStats;
///  pointer to ServerClient message buffer in general shared memory
///  located directly after stats structure
static void* spSrvCliMsgBuf = NULL;

/// image data exchange shared memory name
static const char *spOalShmIdxName = "gdiMidx";
/// image data exchange shared memory representation
static OAL_SHARED_MEM sOalShmIdx;

/// pointer to image data exchange shared memory mapping address
static void* spShmIdx = NULL;

// semaphores
/// init semaphore name
static const char *spSemInitName = "gdiSini";
/// init semaphore representation
static OAL_SEMAPHORE sOalSemInit;

/// server semaphore name
static const char *spSemSrvName = "gdiSsrv";
/// server semaphore representation
static OAL_SEMAPHORE sOalSemSrv;

/// client semaphore name
static const char *spSemCliName = "gdiScli";
/// client semaphore representation
static OAL_SEMAPHORE sOalSemCli;

/// response semaphore name
static const char *spSemRespName = "gdiSrsp";
/// response semaphore representation
static OAL_SEMAPHORE sOalSemResp;

#endif /* GDISRV_HPP */
