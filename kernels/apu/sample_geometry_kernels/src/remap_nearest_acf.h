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

#ifndef APUREMAPNEAREST_H
#define APUREMAPNEAREST_H

#include "common_stringify_macros.h"

#define	REMAP_NEAREST_GRAY_K		remap_nearest_grayscale
#define REMAP_NEAREST_GRAY_KN		XSTR(REMAP_NEAREST_GRAY_K)

#define REMAP_NEAREST_KN_SRC		"SRC"
#define REMAP_NEAREST_KN_DST		"DST"
#define REMAP_NEAREST_KN_OFFS 	"OFFSET"

#ifdef APEX2_EMULATE
#include "acf_kernel.hpp"
using namespace APEX2;

void REMAP_NEAREST_GRAY_K(kernel_io_desc dst_param,
                    kernel_io_desc src_param,
                    kernel_io_desc offset_param);

#endif

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(REMAP_NEAREST_GRAY_K);
#endif

#endif /* APUREMAPNEAREST_H */



