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

#ifndef APUREDUCTION_H
#define APUREDUCTION_H

#include "common_stringify_macros.h" 

#define REDUCTION_FOR_CLMN_K             apu_reduction_for_clmn
#define REDUCTION_FOR_CLMN_KN    XSTR(REDUCTION_FOR_CLMN_K)

#define REDUCTION_FOR_CLMN_KN_IN0  "INPUT_0"
#define REDUCTION_FOR_CLMN_KN_IN1  "INPUT_1"
#define REDUCTION_FOR_CLMN_KN_OUT0 "OUTPUT_0"
#define REDUCTION_FOR_CLMN_KN_OUT1 "OUTPUT_1"

#ifdef APEX2_EMULATE

/****************************************************************************/
/*! Reduction kernel implementation. Reduces 4 vectors on each CU to 256 scalars by accumulation.
\brief Reduction from 4 vectors on each CU to 256 scalars 

\param lIn0             - [Input]  unsigned 32bit vector input
\param lIn1             - [Input]  unsigned 32bit vector input
\param lOut0            - [Output] unsigned 32bit scalar output
\param lOut1            - [Output] unsigned 32bit scalar output
*****************************************************************************/
void REDUCTION_FOR_CLMN_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0, kernel_io_desc lOut1);

#endif //APEX2_EMULATE

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(REDUCTION_FOR_CLMN_K);
#endif

#endif /* APUREDUCTION_H */
