/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* (C) Copyright CogniVue Corporation. 2013 All right reserved.
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

#ifndef APURESIZEVERTICAL_H
#define APURESIZEVERTICAL_H

#include "resize_definitions.h"

/*! Only for debug on simulator. */
#ifdef __APU_DEBUG__ 
#include "apu_acf_debug_utils.h"
#endif // __APU_DEBUG__ 


#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#else
#include "apu2_kernel_port.h"
#endif


#include "common_stringify_macros.h"

#define RESZ_VERT_K		resize_vertical
#define RESZ_VERT_KN	XSTR(RESZ_VERT_K)

#define RESZ_VERT_KN_IN		"ReszVert_In"
#define RESZ_VERT_KN_OUT	"ReszVert_Out"
#define RESZ_VERT_KN_DESCR	"ReszVert_Descr"



#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(RESZ_VERT_K);
#endif

#ifdef APEX2_EMULATE
///// ACF wrapper for apu_resize_integer
void RESZ_VERT_K(kernel_io_desc input_image, kernel_io_desc output_image,
	kernel_io_desc rsz_desc);
#endif

#endif /* APURESIZEVERTICAL_H */
