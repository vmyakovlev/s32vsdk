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

#ifndef APUSATBOXFILTER_H
#define APUSATBOXFILTER_H

#ifdef APEX2_EMULATE
#include "apu_cond.hpp"
#else
#define APU_VSIZE 32
#endif

#include "common_stringify_macros.h"

#define SAT_BOX_FILTER_K		apu_sat_box_filter // same name as the kernel function
#define SAT_BOX_FILTER_KN		XSTR(SAT_BOX_FILTER_K) 


#define SAT_BOX_FILTER_IN			"INPUT"
#define SAT_BOX_FILTER_OUT			"OUTPUT"


#ifdef APEX2_EMULATE
#include "acf_kernel.hpp"
using namespace APEX2;

void SAT_BOX_FILTER_K(kernel_io_desc lIn0, kernel_io_desc lOut0);

#endif //APEX2_EMULATE

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(SAT_BOX_FILTER_K);
#endif

#endif /* APUSATBOXFILTER_H */


