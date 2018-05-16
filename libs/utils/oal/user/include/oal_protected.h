/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* (C) Copyright CogniVue Corporation. 2016 All right reserved.
* Copyright (c) 2016 Freescale Semiconductor
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

/* Information for doxygen */
/**
 *  \file   oal_protected.h
 *  \brief  Interface definition for the OAL (OS abstraction) library.
 *
 *  OAL provides an abstraction of standard OS interface functions used within
 *  MMP the system.
 */

#ifndef OALPROTECTED_H
#define OALPROTECTED_H

#include <oal.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


int64_t OAL_MemorySizeTotal(void); // Function returns total size in bytes being managed by OAL Memory.
int64_t OAL_MemorySizeFree(void);  // Function returns free size in bytes remaining in OAL Memory.

#ifdef __cplusplus
}
#endif

#endif /* OALPROTECTED_H */
