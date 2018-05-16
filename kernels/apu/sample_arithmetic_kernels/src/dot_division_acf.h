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
#ifndef APUDOTDIVISIONKERNEL_H
#define APUDOTDIVISIONKERNEL_H

#include "common_stringify_macros.h"

#define DIV_In32s_Out_32s_K		apu_dot_division
#define DIV_In32s_Out_32s_KN	XSTR(DIV_In32s_Out_32s_K)

#define	LOG2_K				apu_dot_log2
#define	LOG2_KN				XSTR(LOG2_K)

#define COMPUTE_INV_K		apu_dot_inv_NewtonRaphson
#define COMPUTE_INV_KN		XSTR(COMPUTE_INV_K)

#define DIV_N64s_D32s_Q64s_K  apu_dot_division_N64s_D32s_Q64s
#define DIV_N64s_D32s_Q64s_KN XSTR(DIV_N64s_D32s_Q64s_K)


#define DIV_KN_INA		"INPUT_A"
#define DIV_KN_INB		"INPUT_B"
#define DIV_KN_OUT		"OUTPUT"

#define INV_KN_DIV			"Inv_divisor"
#define INV_KN_INV_DIV		"Inv_Inverse_divisor"
#define INV_KN_LOG2Fact		"Inv_log2fact"
#define INV_KN_SHIFTFact	"Inv_shiftFact"

#define LOG2_KN_IN			"Log2_input"
#define LOG2_KN_LOG2Fact	"Log2_fact"

#define DIV_N64s_D32s_Q64s_KN_NOM_HIGH	"Div_N64s_NOM_HIGH"
#define DIV_N64s_D32s_Q64s_KN_NOM_LOW	"Div_N64s_NOM_LOW"
#define DIV_N64s_D32s_Q64s_KN_DIVISOR	"Div_N64s_DIVISOR"
#define DIV_N64s_D32s_Q64s_KN_OUT_HIGH	"Div_N64s_OUT_HIGH"
#define DIV_N64s_D32s_Q64s_KN_OUT_LOW	"Div_N64s_OUT_LOW"
#define DIV_N64s_D32s_Q64s_KN_OUT_REM	"Div_N64s_OUT_REM"


#ifdef APEX2_EMULATE
void DIV_In32s_Out_32s_K(kernel_io_desc lImgInA, kernel_io_desc lImgInB, kernel_io_desc lImgOut);
void LOG2_K(kernel_io_desc lInput, kernel_io_desc lLog2Fact);
void COMPUTE_INV_K(kernel_io_desc lDiv, kernel_io_desc lInvDiv, kernel_io_desc lLog2Fact, kernel_io_desc lShiftFact);
void DIV_N64s_D32s_Q64s_K(kernel_io_desc lNom_high, kernel_io_desc lNom_low, kernel_io_desc lDivisor, kernel_io_desc lDst_high, kernel_io_desc lDst_low, kernel_io_desc lDst_rem);

#endif //APEX2_EMULATE

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(DIV_In32s_Out_32s_K);
extKernelInfoDecl(LOG2_K);
extKernelInfoDecl(COMPUTE_INV_K);
extKernelInfoDecl(DIV_N64s_D32s_Q64s_K);
#endif

#endif /* APUDOTDIVISIONKERNEL_H */

