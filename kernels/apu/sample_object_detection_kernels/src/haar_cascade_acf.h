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

#ifndef APUHAARCASCADE_H
#define APUHAARCASCADE_H

#include "common_stringify_macros.h"

#define	HAAR_CASC_K		apu_haar_cascade
#define HAAR_CASC_KN	XSTR(HAAR_CASC_K)

#define HAAR_CASC_IN_INTEGR_IMG 	"INPUT_INTEGRAL_IMAGE"
#define HAAR_CASC_IN_INTEGR_IMG_SQR "INPUT_INTEGRAL_IMAGE_SQUARED"
#define HAAR_CASC_LINE_IDX 			"LINE_INDEX"
#define HAAR_CASC_WND_BUF			"WINDOW_BUFFER"
#define HAAR_CASC_WND_BUF_SQR		"WINDOW_BUFFER_SQUARED"
#define HAAR_CASC_IN_CASC_SZ 		"INPUT_CASCADE_SIZES"
#define HAAR_CASC_IN_CASC_FEAT 		"INPUT_CASCADE_FEATURES"
#define HAAR_CASC_IN_CASC_STAGES 	"INPUT_CASCADE_STAGES"
#define HAAR_CASC_IN_PIX_SHFT 		"INPUT_PIXEL_SHIFTS"
#define HAAR_CASC_IN_PIX_OFFS 		"INPUT_PIXEL_OFFSETS"
#define HAAR_CASC_OUT				"OUTPUT"

#ifdef APEX2_EMULATE
#include "acf_kernel.hpp"
using namespace APEX2;

void HAAR_CASC_K(
	kernel_io_desc lInII,
	kernel_io_desc lInII2,
	kernel_io_desc lLineIndex,
	kernel_io_desc lWindowBuffer,
	kernel_io_desc lWindowBuffer2,
	kernel_io_desc lInCSizes,
	kernel_io_desc lInCFeatures,
	kernel_io_desc lInCstages,
	kernel_io_desc lInPixelShifts,
	kernel_io_desc lInPixelOffsets,
	kernel_io_desc lOut);


#endif //APEX2_EMULATE

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(HAAR_CASC_K);
#endif



#endif /* APUHAARCASCADE_H */



