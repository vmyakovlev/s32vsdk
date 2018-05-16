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
#ifndef APUUPSAMPLE_H
#define APUUPSAMPLE_H

#include "common_stringify_macros.h"

#define UPSAMPLE_K			apu_upsample
#define UPSAMPLE_KN			XSTR(UPSAMPLE_K)

#define UPSAMPLE_KN_IN	"INPUT_0"
#define UPSAMPLE_KN_OUT	"OUTPUT_0"

#ifdef APEX2_EMULATE

//extern KERNEL_INFO _kernel_info_upsample;
void UPSAMPLE_K(kernel_io_desc lIn0, kernel_io_desc lOut0);

void UPSAMPLE_16u_K(kernel_io_desc lIn0, kernel_io_desc lOut0);

void UPSAMPLE_GAUSS_K(kernel_io_desc lIn0, kernel_io_desc lOut0);

#endif

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(UPSAMPLE_K);
extKernelInfoDecl(UPSAMPLE_16u_K);
extKernelInfoDecl(UPSAMPLE_GAUSS_K);
#endif




#endif /* APUUPSAMPLE_H */
