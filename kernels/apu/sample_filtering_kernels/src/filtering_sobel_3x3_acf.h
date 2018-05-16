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

#ifndef  APUFILTERINGSOBEL3X3_H
#define  APUFILTERINGSOBEL3X3_H
 
#include "common_stringify_macros.h"
 
#ifdef APEX2_EMULATE
#include "apu_config.hpp"
#endif

#define SOBEL_3X3_8BPP_K		sobel_3x3_8bpp
#define SOBEL_3X3_8BPP_KN		XSTR(SOBEL_3X3_8BPP_K)

#define SOBEL_3X3_8BPP_IN		"SRC"
#define SOBEL_3X3_8BPP_OUT		"DST"
#ifdef APEX2_EMULATE

void SOBEL_3X3_8BPP_K(kernel_io_desc in, kernel_io_desc out);
#endif

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(SOBEL_3X3_8BPP_K);
#endif

#endif /* APUFILTERINGSOBEL3X3_H */



