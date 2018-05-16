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

#ifndef APUNMS_H
#define APUNMS_H

#include "common_stringify_macros.h"

#define	NMS_K		apu_nms
#define NMS_KN		XSTR(NMS_K)

#define	NMS16_K		apu_nms16
#define NMS16_KN	XSTR(NMS16_K)

#define NMS_KN_IN		"INPUT_0"
#define NMS_KN_OUT		"OUTPUT_0"



#define NMS16_KN_IN		"INPUT_0"
#define NMS16_KN_OUT	"OUTPUT_0"

#ifdef APEX2_EMULATE
#include "acf_kernel.hpp"
using namespace APEX2;

void NMS_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void NMS16_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
#endif

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(NMS_K);
extKernelInfoDecl(NMS16_K);
#endif


#endif /* APUNMS_H */



