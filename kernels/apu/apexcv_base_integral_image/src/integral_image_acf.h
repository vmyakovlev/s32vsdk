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

#ifndef INTEGRALIMAGEKERNELS_H
#define INTEGRALIMAGEKERNELS_H

#include "common_stringify_macros.h"

#define INTEGRAL_IMAGE_K  integral_image
#define INTEGRAL_IMAGE_KN  XSTR(INTEGRAL_IMAGE_K)


#define INPUT_0    "INPUT_0"
#define OUTPUT_0  "OUTPUT_0"
#define T_LST    "T_LST"


#ifdef APEX2_EMULATE

#include "acf_kernel.hpp"
using namespace APEX2;

void INTEGRAL_IMAGE_K(kernel_io_desc lIn0, kernel_io_desc ltlst, kernel_io_desc lOut0);

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(INTEGRAL_IMAGE_K);
#endif

#endif /* INTEGRALIMAGEKERNELS_H */

