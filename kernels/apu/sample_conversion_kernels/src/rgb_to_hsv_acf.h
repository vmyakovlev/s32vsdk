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

#ifndef APURGBTOHSV_H
#define APURGBTOHSV_H


#include "common_stringify_macros.h"

#define RGB_HSV_K				apu_rgb_to_hsv_sat	// same name as the kernel function
#define RGB_HSV_KN				XSTR(RGB_HSV_K)

#define RGB_HSV_HS_K			apu_rgb_to_hsv_hue_sat	// same name as the kernel function
#define RGB_HSV_HS_KN			XSTR(RGB_HSV_HS_K)


#define RGB_HSV_HS_GREY_K			apu_rgb_to_hsv_hue_sat_grey	// same name as the kernel function
#define RGB_HSV_HS_GREY_KN			XSTR(RGB_HSV_HS_GREY_K)

#define RGB_HSV_SVR_K			apu_rgb_to_hsv_svr	// same name as the kernel function
#define RGB_HSV_SVR_KN			XSTR(RGB_HSV_SVR_K)

//ports definition
#define RGB_HSV_KN_IN		"INPUT_0"
#define RGB_HSV_KN_SAT		"OUT_SAT"
#define RGB_HSV_KN_HUE		"OUT_HUE"
#define RGB_HSV_KN_VAL		"OUT_VAL"
#define RGB_HSV_KN_RED		"OUT_RED"
#define RGB_HSV_KN_GREY		"OUT_GREY"

#ifdef APEX2_EMULATE
#include "acf_kernel.hpp"
using namespace APEX2;

void RGB_HSV_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void RGB_HSV_HS_K(kernel_io_desc lIn0, kernel_io_desc lOutSat, kernel_io_desc lOutHue);
void RGB_HSV_HS_GREY_K(kernel_io_desc lIn0, kernel_io_desc lOutSat, kernel_io_desc lOutHue, kernel_io_desc lOutGrey);
void RGB_HSV_SVR_K(kernel_io_desc lIn0, kernel_io_desc lOutSat, kernel_io_desc lOutVal, kernel_io_desc lOutRed);
#endif

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(RGB_HSV_K);
extKernelInfoDecl(RGB_HSV_HS_K);
extKernelInfoDecl(RGB_HSV_HS_GREY_K);
extKernelInfoDecl(RGB_HSV_SVR_K);
#endif

#endif /* APURGBTOHSV_H */
