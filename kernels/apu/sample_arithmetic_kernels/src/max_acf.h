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

#ifndef APUMAX_H
#define APUMAX_H

#include "common_stringify_macros.h"

#define MAX_K apu_max
#define MAX_KN XSTR(MAX_K)

#define MAX_KN_INA "INPUT_0"
#define MAX_KN_INB "INPUT_1"
#define MAX_KN_OUT "OUTPUT_0"

#define MAX_KN_INA_HIGH "INPUT_0_HIGH"
#define MAX_KN_INB_HIGH "INPUT_1_HIGH"
#define MAX_KN_OUT_HIGH "OUTPUT_0_HIGH"

#define MAX_KN_INA_LOW "INPUT_0_LOW"
#define MAX_KN_INB_LOW "INPUT_1_LOW"
#define MAX_KN_OUT_LOW "OUTPUT_0_LOW"

#ifdef APEX2_EMULATE
#include "acf_kernel.hpp"
using namespace APEX2;


void MAX_K(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out0);


#endif

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(MAX_K);
#endif

#endif /* APUMAX_H */

