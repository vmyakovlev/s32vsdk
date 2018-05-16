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

#ifndef APUADD_H
#define APUADD_H

#include "common_stringify_macros.h"

#define ADD_K				apu_add			// same name as the kernel function
#define ADD_KN				XSTR(ADD_K)

#define ADD_In16s_Out32s_K	apu_add_in16s_out32s
#define ADD_In16s_Out32s_KN	XSTR(ADD_In16s_Out32s_K)

#define ADD_In32s_Out32s_K	apu_add_in32s_out32s
#define ADD_In32s_Out32s_KN  XSTR(ADD_In32s_Out32s_K)

#define ADD_32S_Q3_28_K		apu_add_in32Q3_28_out32Q3_28
#define ADD_32S_Q3_28_KN	XSTR(ADD_32S_Q3_28_K)

#define ADD_In64s_Out64s_K	apu_add_in64s_out64s
#define ADD_In64s_Out64s_KN	XSTR(ADD_In64s_Out64s_K)


#define ADD_KN_INA "INPUT_0"
#define ADD_KN_INB "INPUT_1"
#define ADD_KN_OUT "OUTPUT_0"
#define ADD_KN_OUT_FRAC "OUTPUT_Frac"


#define ADD_KN_INA_HIGH "INPUT_0_HIGH"
#define ADD_KN_INB_HIGH "INPUT_1_HIGH"
#define ADD_KN_OUT_HIGH "OUTPUT_0_HIGH"

#define ADD_KN_INA_LOW "INPUT_0_LOW"
#define ADD_KN_INB_LOW "INPUT_1_LOW"
#define ADD_KN_OUT_LOW "OUTPUT_0_LOW"

#ifdef APEX2_EMULATE

#include "acf_kernel.hpp"
using namespace APEX2;

void ADD_K(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out0);
void ADD_In16s_Out32s_K(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out0);
void ADD_In32s_Out32s_K(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out0);
void ADD_32S_Q3_28_K(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc outInt, kernel_io_desc outFrac);
void ADD_In64s_Out64s_K(kernel_io_desc in0_high, kernel_io_desc in0_low, kernel_io_desc in1_high, kernel_io_desc in1_low, kernel_io_desc out0_high, kernel_io_desc out0_low);

#endif //APEX2_EMULATE

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(ADD_K);
extKernelInfoDecl(ADD_In16s_Out32s_K);
extKernelInfoDecl(ADD_In32s_Out32s_K);
extKernelInfoDecl(ADD_32S_Q3_28_K);
extKernelInfoDecl(ADD_In64s_Out64s_K);
#endif

#endif /* APUADD_H */

