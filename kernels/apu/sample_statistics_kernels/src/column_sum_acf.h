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

#ifndef APUCOLUMNSUM_H
#define APUCOLUMNSUM_H

#include "common_stringify_macros.h"

#define COLUMNS_SUM_K             apu_columns_sum
#define COLUMNS_SUM_KN    XSTR(COLUMNS_SUM_K)

#define COLUMNS_SUM_KN_IN  "INPUT_0"
#define COLUMNS_SUM_KN_IN_PRIOR_DOWN  "INPUT_1"
#define COLUMNS_SUM_KN_IN_PRIOR_UP  "INPUT_2"
#define COLUMNS_SUM_KN_IN_START_TILE  "INPUT_3"

#define COLUMNS_SUM_KN_OUT_DOWN "OUTPUT_0"
#define COLUMNS_SUM_KN_OUT_UP "OUTPUT_1"

#ifdef APEX2_EMULATE


void COLUMNS_SUM_K(kernel_io_desc lIn0, 
                   kernel_io_desc lOutDown, 
                   kernel_io_desc lOutUp,
                   kernel_io_desc lpriorityDown,
                   kernel_io_desc lpriorityUp,
                   kernel_io_desc lindexOfTileStart);
#endif //APEX2_EMULATE

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(COLUMNS_SUM_K);
#endif

#endif /* APUCOLUMNSUM_H */

