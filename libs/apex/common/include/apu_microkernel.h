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

#ifndef APUMICROKERNEL_H
#define APUMICROKERNEL_H

#include "stdint.h"

//////////////////////////////////////////////////
//            KERNEL RELATED
//////////////////////////////////////////////////
// RESERVED MEMORY SIZE
// 32 bytes per entry
#define APU_DMEM_KERNEL_DB_ENTRY       32


// KERNEL REGISTRATION
#define REGISTERED_FUNC   7
typedef int (*myFunct)(int * const, int const);

typedef struct
{
   int32_t kernel_addr;       // IMEM ADDR: MAX (0x8000)
   char    kernel_name[28];
} KERNEL_INFO;

#ifdef __TCTBUILDER__
extern volatile int32_t DMEM_KERNEL_NUM[];
extern volatile KERNEL_INFO DMEM_KERNEL_DB[];
#else
extern volatile int32_t DMEM_KERNEL_NUM[] __attribute__ ((section (".DMEM_KERNEL_NUM")));
extern volatile KERNEL_INFO DMEM_KERNEL_DB[] __attribute__ ((section (".DMEM_KERNEL_DB")));
#endif

#if __chess__
#define KNDECL static
#else
#define KNDECL
#endif

#endif /* APUMICROKERNEL_H */
