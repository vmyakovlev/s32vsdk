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
/**
 * \file    asmport.h
 * \brief   ISP runtime firmware definitions
 * \author
 * \author
 * \version 0.1
 * \date    27-04-2016
 * \note    initial version
 ****************************************************************************/

#ifndef ASMPORT_H
#define ASMPORT_H   1

#ifdef __ghs__ /***** GHS ****************************************************/

#define ASM_PREFIX(x)   x

#define ASM_EQUATE(label,value)
#define ASM_EXTERN(label)       import ASM_PREFIX(label)
#define ASM_ALIGN(value)
#define ASM_PUBLIC(label)       EXPORT ASM_PREFIX(label)
#define ASM_CONST16(value)
#define ASM_CONST32(value)      .word value
#define ASM_LABEL(label)        label
#define ASM_LABEL_CONST32(label,value)

#define ASM_CODE_SECTION(name)  SECTION name : CODE
#define ASM_DATA_SECTION(name)  SECTION name : DATA
#define ASM_END

#define ASM_THUMB		THUMB

#else /***** GCC *************************************************************/

#define ASM_PREFIX(x)   x

#define ASM_EQUATE(label,value) .equ label, value
#define ASM_EXTERN(label)       .extern ASM_PREFIX(label)
#define ASM_ALIGN(value)        .balign value
#define ASM_PUBLIC(label)       .global ASM_PREFIX(label)
#define ASM_CONST16(value)      .short value
#define ASM_CONST32(value)      .long value
#define ASM_LABEL(label)        label:
#define ASM_LABEL_CONST32(label,value) label: .long value

#define ASM_CODE_SECTION(name)  .section name, "x"
#define ASM_DATA_SECTION(name)  .section name, "w"
#define ASM_END                 .end

#define ASM_COMP_SPECIFIC_DIRECTIVES .syntax unified

#define ASM_PUBLIC_BEGIN(name) .type name,%function
#define ASM_PUBLIC_FUNC(name)
#define ASM_PUBLIC_END(name)

#define ASM_THUMB		.thumb

#endif /* __ghs__ */

#endif /* ASMPORT_H */
