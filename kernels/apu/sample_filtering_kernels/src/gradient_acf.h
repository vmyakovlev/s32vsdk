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

#ifndef APUGRADIENT_H
#define APUGRADIENT_H

#include "common_stringify_macros.h"

#define	GRADIENT_K			apu_gradient
#define GRADIENT_KN			XSTR(GRADIENT_K)

#define	GRADIENT_08S_K			apu_gradient_out08s
#define GRADIENT_08S_KN			XSTR(GRADIENT_08S_K)

#define	GRADIENT_ABS_K		apu_gradient_abs
#define GRADIENT_ABS_KN		XSTR(GRADIENT_ABS_K)

#define GR_ABS_K			apu_gr_abs
#define GR_ABS_KN			XSTR(GR_ABS_K)

#define GRADIENT_KN_IN		"INPUT_0"
#define GRADIENT_KN_OUTX	"OUT_GX"
#define GRADIENT_KN_OUTY	"OUT_GY"
#define GRADIENT_KN_ABSSUM  "OUT_ABSSUM"

#ifdef APEX2_EMULATE
#include "acf_kernel.hpp"
using namespace APEX2;

void GRADIENT_K(kernel_io_desc lIn0, kernel_io_desc lgradX, kernel_io_desc lgradY);
void GRADIENT_08S_K(kernel_io_desc lIn0, kernel_io_desc lgradX, kernel_io_desc lgradY);
void GRADIENT_ABS_K(kernel_io_desc lIn0, kernel_io_desc lgradX, kernel_io_desc lgradY, kernel_io_desc labsSum);
void GR_ABS_K(kernel_io_desc lIn0, kernel_io_desc labsSum);

#endif

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(GRADIENT_K);
extKernelInfoDecl(GRADIENT_08S_K);
extKernelInfoDecl(GRADIENT_ABS_K);
extKernelInfoDecl(GR_ABS_K);
#endif

#endif /* APUGRADIENT_H */



