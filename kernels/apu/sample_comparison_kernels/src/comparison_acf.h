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
#ifndef APUCOMPARISON_H
#define APUCOMPARISON_H

#include "common_stringify_macros.h"

#define LOWER_K			apu_lower
#define LOWER_KN		XSTR(LOWER_K)

#define LOWER_SCALAR_K		apu_lower_scalar
#define LOWER_SCALAR_KN		XSTR(LOWER_SCALAR_K)

#define LOWER_In16s_K	apu_lower_in16s
#define LOWER_In16s_KN	XSTR(LOWER_In16s_K)

#define LOWER_In32s_K	apu_lower_in32s
#define LOWER_In32s_KN  XSTR(LOWER_In32s_K)

#define ABS_LOWER_In32s_K	apu_abs_lower_in32s
#define ABS_LOWER_In32s_KN  XSTR(ABS_LOWER_In32s_K)

#define ABS_LOWER_In32s_scalar16u_K	apu_abs_lower_in32s_scalar16u
#define ABS_LOWER_In32s_scalar16u_KN  XSTR(ABS_LOWER_In32s_scalar16u_K)


#define LOWER_In64s_K	apu_lower_in64s
#define LOWER_In64s_KN	XSTR(LOWER_In64s_K)

#define LOWER_In64u_K	apu_lower_in64u
#define LOWER_In64u_KN	XSTR(LOWER_In64u_K)

#define LOWER_EQUAL_In32s_K apu_lowerEqual_in32s
#define LOWER_EQUAL_In32s_KN XSTR(LOWER_EQUAL_In32s_K)

#define AND_K	 apu_and
#define AND_KN	 XSTR(AND_K)

#define MASK8b_K	 apu_mask8b
#define MASK8b_KN	 XSTR(MASK8b_K)


#define AND_In16u_Out16u_K apu_and_in16u_out16u
#define AND_In16u_Out16u_KN XSTR(AND_In16u_Out16u_K)

#define AND_3Pt_In16u_Out16u_K apu_and_3Pt_in16u_out16u
#define AND_3Pt_In16u_Out16u_KN XSTR(AND_3Pt_In16u_Out16u_K)

#define AND_In08u_Out16u_K apu_and_in08u_out16u
#define AND_In08u_Out16u_KN XSTR(AND_In08u_Out16u_K)

#define AND_In08u_In16u_Out16u_K apu_and_in08u_in16u_out16u
#define AND_In08u_In16u_Out16u_KN XSTR(AND_In08u_In16u_Out16u_K)

#define LOWER_KN_INA "LOWER_KN_IN_0"
#define LOWER_KN_INB "LOWER_KN_IN_1"
#define LOWER_KN_SCALAR "LOWER_KN_Scalar"
#define LOWER_KN_OUT "LOWER_KN_OUT_0"
#define LOWER_KN_OUT_FRAC "LOWER_KN_OUT_Frac"


#define LOWER_KN_INA_HIGH "LOWER_KN_IN_0_HIGH"
#define LOWER_KN_INB_HIGH "LOWER_KN_IN_1_HIGH"
#define LOWER_KN_OUT_HIGH "LOWER_KN_OUT_0_HIGH"

#define LOWER_KN_INA_LOW "LOWER_KN_IN_0_LOW"
#define LOWER_KN_INB_LOW "LOWER_KN_IN_1_LOW"
#define LOWER_KN_OUT_LOW "LOWER_KN_OUT_0_LOW"


#define AND_KN_INA "AND_IN_0"
#define AND_KN_INB "AND_IN_1"
#define AND_KN_INC "AND_IN_2"
#define AND_KN_OUT "AND_OUT"


#define MASK_KN_IN_IMG	"MASK_IN_IMG"
#define MASK_KN_IN_MASK	"MASK_IN_MASK"
#define MASK_KN_OUT		"MASK_OUT"



#ifdef APEX2_EMULATE
#include "acf_kernel.hpp"
using namespace APEX2;


void LOWER_K(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out0);
void LOWER_SCALAR_K(kernel_io_desc in0, kernel_io_desc in_scalar, kernel_io_desc out0);
void LOWER_In16s_K(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out0);
void LOWER_In32s_K(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out0);
void ABS_LOWER_In32s_K(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out0);
void ABS_LOWER_In32s_scalar16u_K(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out0);
void LOWER_In64s_K(kernel_io_desc in0_high, kernel_io_desc in0_low, kernel_io_desc in1_high, kernel_io_desc in1_low, kernel_io_desc out0);
void LOWER_In64u_K(kernel_io_desc in0_high, kernel_io_desc in0_low, kernel_io_desc in1_high, kernel_io_desc in1_low, kernel_io_desc out0);
void LOWER_EQUAL_In32s_K(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out);
void AND_In08u_Out16u_K(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out);
void AND_K(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out);
void MASK8b_K(kernel_io_desc in0, kernel_io_desc in_mask, kernel_io_desc out);
void AND_In08u_In16u_Out16u_K(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out);
void AND_In16u_Out16u_K(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out);
void AND_3Pt_In16u_Out16u_K(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc in2, kernel_io_desc out);

#endif

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(LOWER_K);
extKernelInfoDecl(LOWER_SCALAR_K);
extKernelInfoDecl(LOWER_In16s_K);
extKernelInfoDecl(LOWER_In32s_K);
extKernelInfoDecl(ABS_LOWER_In32s_K);
extKernelInfoDecl(ABS_LOWER_In32s_scalar16u_K);
extKernelInfoDecl(LOWER_In64s_K);
extKernelInfoDecl(LOWER_In64u_K);
extKernelInfoDecl(LOWER_EQUAL_In32s_K);
extKernelInfoDecl(AND_In08u_Out16u_K);
extKernelInfoDecl(AND_K);
extKernelInfoDecl(MASK8b_K);
extKernelInfoDecl(AND_In08u_In16u_Out16u_K);
extKernelInfoDecl(AND_In16u_Out16u_K);
extKernelInfoDecl(AND_3Pt_In16u_Out16u_K);
#endif

#endif /* APUCOMPARISON_H */

