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

#ifndef APUCORRELATIONKERNEL_H
#define APUCORRELATIONKERNEL_H

#include "common_stringify_macros.h"

#define CORR_Kernel_Input		"INPUT_Img"
#define CORR_Kernel_Output		"OUTPUT_Img"
#define CORR_Kernel_Filter		"Corr_IN_Filter"
#define CORR_Kernel_FilterWidth "INPUT_FilterWidth" 
#define CORR_Kernel_FilterHeight "INPUT_FilterHeight" 
#define CORR_Kernel_FilterScale	"INPUT_FilterScale" 
#define CORR_Kernel_FiltSymmFl	"INPUT_FiltSymmFl"

/// FW, FH, HFW, HFH need to be adapted if the CORR_K kernel is used 
/// to the size of the currently used filter
#ifndef FW 
#define FW 3  // filter width 
#define FH 3  //filter height
#define HFW 1 //(FW >> 1); // half filter height. Formula does not work in the constructor of the Kernel Meta data.
#define HFH 1 //(FH >> 1); //half filter width
#endif

#define CORR_K				apu_correlation
#define CORR_KN				XSTR(CORR_K)

#define CORR_GRAD_X_K		apu_gradient_x
#define CORR_GRAD_X_KN		XSTR(CORR_GRAD_X_K)

#define CORR_GRAD_Y_K		apu_gradient_y
#define CORR_GRAD_Y_KN		XSTR(CORR_GRAD_Y_K)

#define CORR_SCHARR_X_K		apu_scharr_x
#define CORR_SCHARR_X_KN	XSTR(CORR_SCHARR_X_K)

#define CORR_SCHARR_Y_K		apu_scharr_y
#define CORR_SCHARR_Y_KN	XSTR(CORR_SCHARR_Y_K)

#define CORR_KN_1x3_K		apu_correlation_1x3
#define CORR_KN_1x3_KN		XSTR(CORR_KN_1x3_K)

#define CORR_KN_3x1_K		apu_correlation_3x1
#define CORR_KN_3x1_KN		XSTR(CORR_KN_3x1_K)

#define CORR_KN_3x3_K		apu_correlation_3x3
#define CORR_KN_3x3_KN		XSTR(CORR_KN_3x3_K)

#define CORR_KN_5x5_K		apu_correlation_5x5
#define CORR_KN_5x5_KN		XSTR(CORR_KN_5x5_K)

#define CORR_KN_7x7_K		apu_correlation_7x7
#define CORR_KN_7x7_KN		XSTR(CORR_KN_7x7_K)


#ifdef APEX2_EMULATE

void CORR_K(kernel_io_desc lImgIn, kernel_io_desc lImgOut, kernel_io_desc lFilterIn, /*kernel_io_desc lFilterWidth, kernel_io_desc lFilterHeight,*/ kernel_io_desc lFilterScale, kernel_io_desc lFilterSymFlags);
void CORR_GRAD_X_K(kernel_io_desc lImgIn, kernel_io_desc lImgOut);
void CORR_GRAD_Y_K(kernel_io_desc lImgIn, kernel_io_desc lImgOut);
void CORR_KN_1x3_K(kernel_io_desc lImgIn, kernel_io_desc lImgOut, kernel_io_desc lFilterIn, kernel_io_desc lFilterScale, kernel_io_desc lFilterSymFlags);
void CORR_KN_3x1_K(kernel_io_desc lImgIn, kernel_io_desc lImgOut, kernel_io_desc lFilterIn, kernel_io_desc lFilterScale, kernel_io_desc lFilterSymFlags);
void CORR_KN_3x3_K(kernel_io_desc lImgIn, kernel_io_desc lImgOut, kernel_io_desc lFilterIn, kernel_io_desc lFilterScale, kernel_io_desc lFilterSymFlags);
void CORR_KN_5x5_K(kernel_io_desc lImgIn, kernel_io_desc lImgOut, kernel_io_desc lFilterIn, kernel_io_desc lFilterScale, kernel_io_desc lFilterSymFlags);
void CORR_KN_7x7_K(kernel_io_desc lImgIn, kernel_io_desc lImgOut, kernel_io_desc lFilterIn, kernel_io_desc lFilterScale, kernel_io_desc lFilterSymFlags);
void CORR_SCHARR_X_K(kernel_io_desc lImgIn, kernel_io_desc lImgOut);
void CORR_SCHARR_Y_K(kernel_io_desc lImgIn, kernel_io_desc lImgOut);

#endif

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(CORR_K);
extKernelInfoDecl(CORR_GRAD_X_K);
extKernelInfoDecl(CORR_GRAD_Y_K);
extKernelInfoDecl(CORR_KN_1x3_K);
extKernelInfoDecl(CORR_KN_3x1_K);
extKernelInfoDecl(CORR_KN_3x3_K);
extKernelInfoDecl(CORR_KN_5x5_K);
extKernelInfoDecl(CORR_KN_7x7_K);
extKernelInfoDecl(CORR_SCHARR_X_K);
extKernelInfoDecl(CORR_SCHARR_Y_K);
#endif

#endif /* APUCORRELATIONKERNEL_H */