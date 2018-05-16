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

#ifndef APUDOTMULTIPLICKERNEL_H
#define APUDOTMULTIPLICKERNEL_H

#include "common_stringify_macros.h"

#define MULT_In16s_Out32s_K			apu_dot_mult_in16s_out32s
#define MULT_In16s_Out32s_KN		XSTR(MULT_In16s_Out32s_K)

#define MULT_In32s_Out32s_K			apu_dot_mult_in32s_out32s
#define MULT_In32s_Out32s_KN		XSTR(MULT_In32s_Out32s_K)

#define MULT_In32s_Out64s_K			apu_dot_mult_in32s_out64s
#define MULT_In32s_Out64s_KN		XSTR(MULT_In32s_Out64s_K)

#define MULT_In32s_In16s_Out32s_K	apu_dot_mult_in32s_in16s_out32s
#define MULT_In32s_In16s_Out32s_KN	XSTR(MULT_In32s_In16s_Out32s_K)

#define MULT_SCALAR_In08u_Out16s_K	apu_dot_mult_scalar_in08u_out16s
#define MULT_SCALAR_In08u_Out16s_KN	XSTR(MULT_SCALAR_In08u_Out16s_K)

#define MULT_SCALAR_In32s_Out32s_K	apu_dot_mult_scalar_in32s_out32s
#define MULT_SCALAR_In32s_Out32s_KN	XSTR(MULT_SCALAR_In32s_Out32s_K)

#define LEFT_SHIFT_In16u_Out16s_K	apu_dot_left_shift_in16u_out16s
#define LEFT_SHIFT_In16u_Out16s_KN	XSTR(LEFT_SHIFT_In16u_Out16s_K)

#define RIGHT_SHIFT_In64s_Out64s_K	apu_dot_right_shift_in64s_out64s
#define RIGHT_SHIFT_In64s_Out64s_KN	XSTR(RIGHT_SHIFT_In64s_Out64s_K)

#define RIGHT_SHIFT_In64s_Out32s_K	apu_dot_right_shift_in64s_out32s
#define RIGHT_SHIFT_In64s_Out32s_KN	XSTR(RIGHT_SHIFT_In64s_Out32s_K)

#define MULT_BY_2_In32s_Out32s_K	apu_dot_lsh1_in32s_out32s
#define MULT_BY_2_In32s_Out32s_KN	XSTR(MULT_BY_2_In32s_Out32s_K)

#define MULT_BY_2_In32s_Out64s_K	apu_dot_lsh1_in32s_out64s
#define MULT_BY_2_In32s_Out64s_KN	XSTR(MULT_BY_2_In32s_Out64s_K)

#define MULT_BY_2_Q3_28_In32s_Out64s_K	apu_dot_lsh1_in32s_Q3_28_out64s
#define MULT_BY_2_Q3_28_In32s_Out64s_KN	XSTR(MULT_BY_2_Q3_28_In32s_Out64s_K)

#define MULT_KN_INA				"DotMultKn_IN_A"
#define MULT_KN_INB				"DotMultKn_IN_B"
#define MULT_KN_OUT				"DotMultKn_OUT"
#define MULT_KN_OUT_HIGH		"DotMultKn_OUT_High"
#define MULT_KN_OUT_LOW			"DotMultKn_OUT_Low"

#define MULT_SCALAR_KN_IN			"MultScalKn_IN"
#define MULT_SCALAR_KN_IN_SCALAR	"MultScalKn_IN_SCALAR"
#define MULT_SCALAR_KN_OUT			"MultScalKn_OUT"

#define MULT_BY_2_In32s_Out32s_KN_IN	"MultBy2Kn_IN"
#define MULT_BY_2_In32s_Out32s_KN_OUT "MultBy2Kn_OUT"

#define MULT_BY_2_In32s_Out32s_KN_OUT_LOW	"MultBy2Kn_Output_low"
#define MULT_BY_2_In32s_Out32s_KN_OUT_H	"MultBy2Kn_Output_high"

#define MULT_BY_2_Q3_28_KN_OUT_INT	"MultBy2_Q3_28_Kn_Output_int"
#define MULT_BY_2_Q3_28_KN_OUT_FRAC	"MultBy2_Q3_28_Kn_Output_frac"

#define RIGHT_SHIFT_In64s_HIGH		"RightShift_64bit_InHigh"
#define RIGHT_SHIFT_In64s_LOW		"RightShift_64bit_InLOW"
#define RIGHT_SHIFT_OUT32s			"RightShift_32bit_Out"

#define RIGHT_SHIFT_Out64s_HIGH	"RightShift_64bit_OutHigh"
#define RIGHT_SHIFT_Out64s_LOW	"RightShift_64bit_OutLOW"

#define RIGHT_SHIFT_Fact	"RghtShft_64bit_ShiftFact"

#ifdef APEX2_EMULATE
#include <acf_kernel.hpp>

void MULT_In16s_Out32s_K(kernel_io_desc lImgInA, kernel_io_desc lImgInB, kernel_io_desc lImgOut);
void MULT_In32s_Out32s_K(kernel_io_desc lImgInA, kernel_io_desc lImgInB, kernel_io_desc lImgOut);
void MULT_In32s_Out64s_K(kernel_io_desc lImgInA, kernel_io_desc lImgInB, kernel_io_desc lImgOut_high, kernel_io_desc lImgOut_low);
void MULT_SCALAR_In08u_Out16s_K(kernel_io_desc lImgInA, kernel_io_desc lImgOut, kernel_io_desc scalarVal);
void MULT_SCALAR_In32s_Out32s_K(kernel_io_desc lImgInA, kernel_io_desc lImgOut, kernel_io_desc scalarVal);
void MULT_In32s_In16s_Out32s_K(kernel_io_desc lImgInA, kernel_io_desc lImgInB, kernel_io_desc lImgOut);
void LEFT_SHIFT_In16u_Out16s_K(kernel_io_desc lImgInA, kernel_io_desc lImgOut, kernel_io_desc scalarVal);
void MULT_BY_2_In32s_Out32s_K(kernel_io_desc lImgInA, kernel_io_desc lImgOut);
void RIGHT_SHIFT_In64s_Out64s_K(kernel_io_desc lImgInA_high, kernel_io_desc lImgInA_low, kernel_io_desc lImgOut_high, kernel_io_desc lImgOut_low, kernel_io_desc scalarVal);
void RIGHT_SHIFT_In64s_Out32s_K(kernel_io_desc lImgInA_high, kernel_io_desc lImgInA_low, kernel_io_desc lImgOut, kernel_io_desc scalarVal);
void MULT_BY_2_64bitu_K(kernel_io_desc lImgInA, kernel_io_desc lImgOut_high, kernel_io_desc lImgOut_low);
void MULT_BY_2_In32s_Out64s_K(kernel_io_desc lImgInA, kernel_io_desc lImgOut_high, kernel_io_desc lImgOut_low);
void MULT_BY_2_Q3_28_In32s_Out64s_K(kernel_io_desc lImgInA, kernel_io_desc lImgOut_high, kernel_io_desc lImgOut_low);

#endif //APEX2_EMULATE

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(MULT_In16s_Out32s_K);
extKernelInfoDecl(MULT_In32s_Out32s_K);
extKernelInfoDecl(MULT_In32s_Out64s_K);
extKernelInfoDecl(MULT_SCALAR_In08u_Out16s_K);
extKernelInfoDecl(MULT_SCALAR_In32s_Out32s_K);
extKernelInfoDecl(MULT_In32s_In16s_Out32s_K);
extKernelInfoDecl(LEFT_SHIFT_In16u_Out16s_K);
extKernelInfoDecl(MULT_BY_2_In32s_Out32s_K);
extKernelInfoDecl(RIGHT_SHIFT_In64s_Out64s_K);
extKernelInfoDecl(RIGHT_SHIFT_In64s_Out32s_K);
extKernelInfoDecl(MULT_BY_2_64bitu_K);
extKernelInfoDecl(MULT_BY_2_In32s_Out64s_K);
extKernelInfoDecl(MULT_BY_2_Q3_28_In32s_Out64s_K);
#endif

#endif /* APUDOTMULTIPLICKERNEL_H */
