/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
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


#ifndef INTERPOLATIONKERNELS_H
#define INTERPOLATIONKERNELS_H


#include "common_stringify_macros.h"

#define INTERP_LIN_GRAYSC_K    interp_linear_grayscale
#define INTERP_LIN_GRAYSC_KN  XSTR(INTERP_LIN_GRAYSC_K )

#define INTERP_BILIN_GRAYSC_K  interp_bilinear_grayscale
#define INTERP_BILIN_GRAYSC_KN  XSTR(INTERP_BILIN_GRAYSC_K)

#define INTERP_BICUBIC_GRAYSC_K    interp_bicubic_grayscale
#define INTERP_BICUBIC_GRAYSC_KN  XSTR(INTERP_BICUBIC_GRAYSC_K)

#define INPUT_0    "INPUT_0"
#define OUTPUT_0  "OUTPUT_0"

#define DELTA_X    "DELTA_X"
#define DELTA_XY  "DELTA_XY"

#define X_OFFSET  "X_OFFSET"
#define Y_OFFSET  "Y_OFFSET"

#ifdef APEX2_EMULATE

#include "acf_kernel.hpp"
using namespace APEX2;

void INTERP_LIN_GRAYSC_K(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lDelta0);

void INTERP_BILIN_GRAYSC_K(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lDelta0);


void INTERP_BICUBIC_GRAYSC_K(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lXOffs, kernel_io_desc lYOffs);

#endif 

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)) && (!defined(__INTEGRITY__))) 
extKernelInfoDecl(INTERP_LIN_GRAYSC_K);
extKernelInfoDecl(INTERP_BILIN_GRAYSC_K);
extKernelInfoDecl(INTERP_BICUBIC_GRAYSC_K);
#endif


#endif /* INTERPOLATIONKERNELS_H */

