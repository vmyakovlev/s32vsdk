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


#ifndef ARITHMETICKERNELS_H
#define ARITHMETICKERNELS_H


#include "common_stringify_macros.h"

#define ABS_08u_K      abs_08u  
#define ABS_08u_KN      XSTR(ABS_08u_K)

#define ABSDIFF_08u_K    absdiff_08u
#define ABSDIFF_08u_KN    XSTR(ABSDIFF_08u_K)

#define ACCUM_16s_K      accumulate_16s
#define ACCUM_16s_KN    XSTR(ACCUM_16s_K)

#define ACCUM_SQRD_16s_K  accumulate_squared_16s
#define ACCUM_SQRD_16s_KN  XSTR(ACCUM_SQRD_16s_K)

#define ACCUM_WGTD_08u_K  accumulate_weighted_08u
#define ACCUM_WGTD_08u_KN  XSTR(ACCUM_WGTD_08u_K)

#define ADD_08u_K      add_08u
#define ADD_08u_KN      XSTR(ADD_08u_K)

#define ADD_16s_K      add_16s
#define ADD_16s_KN      XSTR(ADD_16s_K)

#define AND_08u_K      and_08u
#define AND_08u_KN      XSTR(AND_08u_K)

#define AND_16u_K      and_16u
#define AND_16u_KN      XSTR(AND_16u_K)

#define AND_32u_K      and_32u
#define AND_32u_KN      XSTR(AND_32u_K)

#define CLZ_08u_K      clz_08u
#define CLZ_08u_KN      XSTR(CLZ_08u_K)

#define CLZ_08s_K      clz_08s
#define CLZ_08s_KN      XSTR(CLZ_08s_K)

#define CLZ_16u_K      clz_16u
#define CLZ_16u_KN      XSTR(CLZ_16u_K)

#define CLZ_16s_K      clz_16s
#define CLZ_16s_KN      XSTR(CLZ_16s_K)

#define MAGNITUDE_16s_K    magnitude_16s
#define MAGNITUDE_16s_KN  XSTR(MAGNITUDE_16s_K)

#define NOT_08u_K      not_08u
#define NOT_08u_KN      XSTR(NOT_08u_K)

#define OR_08u_K      or_08u
#define OR_08u_KN      XSTR(OR_08u_K)

#define OR_16u_K      or_16u
#define OR_16u_KN      XSTR(OR_16u_K)

#define OR_32u_K      or_32u
#define OR_32u_KN      XSTR(OR_32u_K)

#define SUB_08u_K      sub_08u
#define SUB_08u_KN      XSTR(SUB_08u_K)

#define SUB_16s_K      sub_16s
#define SUB_16s_KN      XSTR(SUB_16s_K)

#define TABLE_LOOKUP_K      table_lookup_08u
#define TABLE_LOOKUP_KN     XSTR(TABLE_LOOKUP_K)

#define THRESH_08u_K    threshold_08u
#define THRESH_08u_KN    XSTR(THRESH_08u_K)

#define THRESH_16u_K    threshold_16u
#define THRESH_16u_KN    XSTR(THRESH_16u_K)

#define THRESH_32u_K    threshold_32u
#define THRESH_32u_KN    XSTR(THRESH_32u_K)

#define THRESHOLD_RANGE_08u_K    threshold_range_08u
#define THRESHOLD_RANGE_08u_KN    XSTR(THRESHOLD_RANGE_08u_K)

#define THRESHOLD_RANGE_16u_K    threshold_range_16u
#define THRESHOLD_RANGE_16u_KN    XSTR(THRESHOLD_RANGE_16u_K)

#define THRESHOLD_RANGE_32u_K    threshold_range_32u
#define THRESHOLD_RANGE_32u_KN    XSTR(THRESHOLD_RANGE_32u_K)


#define XOR_08u_K      xor_08u
#define XOR_08u_KN      XSTR(XOR_08u_K)

#define XOR_16u_K      xor_16u
#define XOR_16u_KN      XSTR(XOR_16u_K)

#define XOR_32u_K      xor_32u
#define XOR_32u_KN      XSTR(XOR_32u_K)



#define INPUT_0    "INPUT_0"
#define INPUT_1    "INPUT_1"
#define INPUT_2    "INPUT_2"
#define OUTPUT_0  "OUTPUT_0"
#define OUTPUT_1  "OUTPUT_1"
#define ALPHA    "ALPHA"



#ifdef APEX2_EMULATE

#include "acf_kernel.hpp"
using namespace APEX2;

void ABS_08u_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void ABSDIFF_08u_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0);
void ACCUM_16s_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0);
void ACCUM_SQRD_16s_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lAlpha, kernel_io_desc lOut0);
void ACCUM_WGTD_08u_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lAlpha, kernel_io_desc lOut0);
void ADD_08u_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0);
void ADD_16s_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0);
void AND_08u_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0);
void AND_16u_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0);
void AND_32u_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0);
void CLZ_08u_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void CLZ_08s_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void CLZ_16u_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void CLZ_16s_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void MAGNITUDE_16s_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0);
void NOT_08u_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void OR_08u_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0);
void OR_16u_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0);
void OR_32u_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0);
void SUB_08u_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0);
void SUB_16s_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0);
void TABLE_LOOKUP_K(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lLut0, kernel_io_desc lvLut);
void THRESH_08u_K(kernel_io_desc lIn0, kernel_io_desc lThreshold, kernel_io_desc lOutVals, kernel_io_desc lOut0);
void THRESH_16u_K(kernel_io_desc lIn0, kernel_io_desc lThreshold, kernel_io_desc lOutVals, kernel_io_desc lOut0);
void THRESH_32u_K(kernel_io_desc lIn0, kernel_io_desc lThreshold, kernel_io_desc lOutVals, kernel_io_desc lOut0);
void THRESHOLD_RANGE_08u_K(kernel_io_desc lIn0, kernel_io_desc lThreshold, kernel_io_desc lOutVals, kernel_io_desc lOut0);
void THRESHOLD_RANGE_16u_K(kernel_io_desc lIn0, kernel_io_desc lThreshold, kernel_io_desc lOutVals, kernel_io_desc lOut0);
void THRESHOLD_RANGE_32u_K(kernel_io_desc lIn0, kernel_io_desc lThreshold, kernel_io_desc lOutVals, kernel_io_desc lOut0);
void XOR_08u_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0);
void XOR_16u_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0);
void XOR_32u_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0);

#endif //APEX2_EMULATE

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(ABS_08u_K);
extKernelInfoDecl(ABSDIFF_08u_K);
extKernelInfoDecl(ACCUM_16s_K);
extKernelInfoDecl(ACCUM_SQRD_16s_K);
extKernelInfoDecl(ACCUM_WGTD_08u_K);
extKernelInfoDecl(ADD_08u_K);
extKernelInfoDecl(ADD_16s_K);
extKernelInfoDecl(AND_08u_K);
extKernelInfoDecl(AND_16u_K);
extKernelInfoDecl(AND_32u_K);
extKernelInfoDecl(CLZ_08u_K);
extKernelInfoDecl(CLZ_08s_K);
extKernelInfoDecl(CLZ_16u_K);
extKernelInfoDecl(CLZ_16s_K);
extKernelInfoDecl(MAGNITUDE_16s_K);
extKernelInfoDecl(NOT_08u_K);
extKernelInfoDecl(OR_08u_K);
extKernelInfoDecl(OR_16u_K);
extKernelInfoDecl(OR_32u_K);
extKernelInfoDecl(SUB_08u_K);
extKernelInfoDecl(SUB_16s_K);
extKernelInfoDecl(TABLE_LOOKUP_K);
extKernelInfoDecl(THRESH_08u_K);
extKernelInfoDecl(THRESH_16u_K);
extKernelInfoDecl(THRESH_32u_K);
extKernelInfoDecl(THRESHOLD_RANGE_08u_K);
extKernelInfoDecl(THRESHOLD_RANGE_16u_K);
extKernelInfoDecl(THRESHOLD_RANGE_32u_K);
extKernelInfoDecl(XOR_08u_K);
extKernelInfoDecl(XOR_16u_K);
extKernelInfoDecl(XOR_32u_K);
#endif

#endif /* ARITHMETICKERNELS_H */
