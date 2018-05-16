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
#ifndef APUDIFFERENCEKERNEL_H
#define APUDIFFERENCEKERNEL_H

#include "common_stringify_macros.h"

#define DIFF_In08u_Out16s_K		apu_diff_in08u_out16s
#define DIFF_In08u_Out16s_KN	XSTR(DIFF_In08u_Out16s_K)

#define DIFF_In16s_Out16s_K		apu_diff_in16s_out16s
#define DIFF_In16s_Out16s_KN	XSTR(DIFF_In16s_Out16s_K)

#define DIFF_In16s_Out32s_K		apu_diff_in16s_out32s
#define DIFF_In16s_Out32s_KN	XSTR(DIFF_In16s_Out32s_K)

#define DIFF_In32s_Out32s_K		apu_diff_in32s_out32s
#define DIFF_In32s_Out32s_KN	XSTR(DIFF_In32s_Out32s_K)

#define DIFF_In64s_Out64s_K		apu_diff_in64s_out64s
#define DIFF_In64s_Out64s_KN	XSTR(DIFF_In64s_Out64s_K)

#define DIFF_KN_INA			"INPUT_A"
#define DIFF_KN_INB			"INPUT_B"
#define DIFF_KN_OUT			"OUTPUT"

#define DIFF_KN_INA_HIGH	"INPUT_A_HIGH"
#define DIFF_KN_INB_HIGH	"INPUT_B_HIGH"
#define DIFF_KN_OUT_HIGH	"OUTPUT_HIGH"

#define DIFF_KN_INA_LOW		"INPUT_A_LOW"
#define DIFF_KN_INB_LOW		"INPUT_B_LOW"
#define DIFF_KN_OUT_LOW		"OUTPUT_LOW"

#ifdef APEX2_EMULATE

#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
void DIFF_In08u_Out16s_K(kernel_io_desc lImgInA, kernel_io_desc lImgInB, kernel_io_desc lImgOut);
void DIFF_In16s_Out16s_K(kernel_io_desc lImgInA, kernel_io_desc lImgInB, kernel_io_desc lImgOut);
void DIFF_In16s_Out32s_K(kernel_io_desc lImgInA, kernel_io_desc lImgInB, kernel_io_desc lImgOut);
void DIFF_In32s_Out32s_K(kernel_io_desc lImgInA, kernel_io_desc lImgInB, kernel_io_desc lImgOut);
void DIFF_In64s_Out64s_K(kernel_io_desc lImgInA_high, kernel_io_desc lImgInA, kernel_io_desc lImgInB_high, kernel_io_desc lImgInB, kernel_io_desc lImgOut_high, kernel_io_desc lImgOut);

#endif //APEX2_EMULATE

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(DIFF_In08u_Out16s_K);
extKernelInfoDecl(DIFF_In16s_Out16s_K);
extKernelInfoDecl(DIFF_In16s_Out32s_K);
extKernelInfoDecl(DIFF_In32s_Out32s_K);
extKernelInfoDecl(DIFF_In64s_Out64s_K);
#endif

#endif /* APUDIFFERENCEKERNEL_H */

