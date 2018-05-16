/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2013-2016 Freescale Semiconductor
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

#ifndef APUMATCHDESCRIPTORS_H
#define APUMATCHDESCRIPTORS_H

#include "common_stringify_macros.h"

#define	MATCH_DESCRIPTORS_K		apu_match_descriptors
#define MATCH_DESCRIPTORS_KN	XSTR(MATCH_DESCRIPTORS_K)

#define MATCH_DESCR_KN_IN0		"INPUT_0" 
#define MATCH_DESCR_KN_IN1		"INPUT_1" 
#define MATCH_DESCR_KN_OUT0		"OUTPUT_0"
#define MATCH_DESCR_KN_OUT1		"OUTPUT_1"
#define MATCH_DESCR_KN_CFG 		"INPUT_CONFIG"

#ifdef APEX2_EMULATE
#include "acf_kernel.hpp"
using namespace APEX2;


void MATCH_DESCRIPTORS_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lInConfig, kernel_io_desc lOut0, kernel_io_desc lOut1);

#endif

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(MATCH_DESCRIPTORS_K);
#endif

#endif /* APUMATCHDESCRIPTORS_H */


