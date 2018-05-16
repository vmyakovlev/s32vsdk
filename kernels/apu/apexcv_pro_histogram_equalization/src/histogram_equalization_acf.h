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
#ifndef APEXCVHISTOGRAMEQUALIZATIONACF_H
#define APEXCVHISTOGRAMEQUALIZATIONACF_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

#include "common_stringify_macros.h"

#define HIST_EQU_K    apu_histogram_equalization
#define HIST_EQU_KN    XSTR(HIST_EQU_K)

#define EQU_LUT_K    apu_generate_lut
#define EQU_LUT_KN      XSTR(EQU_LUT_K)

#define  HIST_EQU_K_OUT_IMG   "OUTPUT_0"
#define  HIST_EQU_K_IN_IMG   "INPUT_0"
#define  HIST_EQU_K_LUT      "LUT_IN"
#define  HIST_EQU_K_LUT_BUF  "VECTORIZED_LUT_BUFFER"

#define  HIST_LUT_K_LUT    "LUT_OUT"
#define  HIST_LUT_K_HISTO    "IMAGE_HISTOGRAM"
#define  HIST_LUT_K_NUM_PIXELS  "NUM_PIXELS"

#ifdef APEX2_EMULATE

void HIST_EQU_K(kernel_io_desc lOut0, kernel_io_desc lIn0,
        kernel_io_desc scalar_lut, kernel_io_desc vector_lut);


void EQU_LUT_K(kernel_io_desc transformed_histogram, kernel_io_desc histogram,
        kernel_io_desc lNumPixels);

#endif //APEX2_EMULATE

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(HIST_EQU_K);
extKernelInfoDecl(EQU_LUT_K);
#endif

#endif /* APEXCVHISTOGRAMEQUALIZATIONACF_H */




/*! @} */
