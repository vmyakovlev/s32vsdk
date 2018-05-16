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

#ifndef APUHISTOFGRAD_H
#define APUHISTOFGRAD_H

#include "common_stringify_macros.h"

#define HISTOFGRAD_K		apu_histofgrad
#define HISTOFGRAD_KN	XSTR(HISTOFGRAD_K)

#define HOG_KN_InGradX	 "HOG_InGradX"
#define HOG_KN_InGradY	 "HOG_InGradY"
#define HOG_KN_OUT		 "HOG_OUT"
#define HOG_KN_InAbsGradX	 "HOG_InAbsGradX"
#define HOG_KN_InAbsGradY	 "HOG_InAbsGradY"
#define HOG_KN_OUT_BL_NORM	 "HOG_OUT_BlNorm"


#ifdef APEX2_EMULATE
void HISTOFGRAD_K(kernel_io_desc lInGradX, kernel_io_desc lInGradY, kernel_io_desc lOut, kernel_io_desc lOutBlNorm);
#endif //APEX2_EMULATE

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(HISTOFGRAD_K);
#endif

#endif /* APUHISTOFGRAD_H */

