/*==================================================================================================
*   @file    Vector_table.s
*   @version M4_SRC_SW_VERSION_MAJOR.M4_SRC_SW_VERSION_MINOR.M4_SRC_SW_VERSION_PATCH
*
*   @brief   AUTOSAR M4_SRC_MODULE_NAME - Vector Table
*   @details Vector Table for AARCH64 mode.
*            This file contains sample code only. It is not part of the production code deliverables
*   @addtogroup BUILD_ENV
*   @{
*/
/*==================================================================================================
*   Project              : M4_SRC_PROJECT_NAME
*   Platform             : M4_SRC_MCU_FAMILY
*   Peripheral           : M4_SRC_USED_PERIPHERAL
*   Dependencies         : M4_SRC_AR_MODULE_DEPENDENCY
*
*   Autosar Version      : M4_SRC_AR_SPEC_VERSION_MAJOR.M4_SRC_AR_SPEC_VERSION_MINOR.M4_SRC_AR_SPEC_VERSION_PATCH
*   Autosar Revision     : M4_SRC_AR_RELEASE_REVISION
*   Autosar Conf.Variant :
*   SW Version           : M4_SRC_SW_VERSION_MAJOR.M4_SRC_SW_VERSION_MINOR.M4_SRC_SW_VERSION_PATCH
*   Build Version        : M4_SRC_BUILD_ID
*
*   (c) Copyright M4_SRC_YEAR_ID M4_SRC_COPYRIGHTED_TO
*   All Rights Reserved.
==================================================================================================*/
/*==================================================================================================
ifdef(`M4_SRC_KEEP_REVISION_HISTORY', `dnl - DO NOT modify this M4 line!
Revision History:
                             Modification     Tracking
Author (core ID)              Date D/M/Y       Number     Description of Changes
---------------------------   ----------    ------------  ------------------------------------------

---------------------------   ----------    ------------  ------------------------------------------
',)dnl - DO NOT modify this M4 line!
==================================================================================================*/

/*  Exception table 0x80 aligned */
/*
     0x0 : Synchronous exception from Current EL with SP_EL0
     0x1 : IRQ exception from Current EL with SP_EL0
     0x2 : FIQ exception from Current EL with SP_EL0
     0x3 : System Error exception from Current EL with SP_EL0
     0x4 : Synchronous exception from Current EL with SP_ELx
     0x5 : IRQ exception from Current EL with SP_ELx
     0x6 : FIQ exception from Current EL with SP_ELx
     0x7 : System Error exception from Current EL with SP_ELx
     0x8 : Synchronous exception from Lower EL using aarch64
     0x9 : IRQ exception from Lower EL using aarch64
     0xa : FIQ exception from Lower EL using aarch64
     0xb : System Error exception from Lower EL using aarch64
     0xc : Synchronous exception from Lower EL using aarch32
     0xd : IRQ exception from Lower EL using aarch32
     0xe : FIQ exception from Lower EL using aarch32
     0xf : System Error exception from Lower EL using aarch32
*/      


    .include "v8_system_linaro.s" 
    .global	el1_vector_table
    .global	el2_vector_table
    .global	el3_vector_table
    .global g_INT_vectors
       
    #----------------------------------------------------------------
    .extern curr_el3_sp0_sync_handler
    #.extern irq_handler
    .extern curr_el3_sp0_fiq_handler
    #.extern serror_handler
    .extern curr_el3_spx_sync_handler
    #.extern irq_handler
    .extern curr_el3_spx_fiq_handler
    #.extern serror_handler
    .extern lower_el3_aarch64_sync_handler
    .extern lower_el3_aarch64_irq_handler
    .extern lower_el3_aarch64_fiq_handler
    .extern lower_el3_aarch64_serror_handler
    .extern lower_el3_aarch32_sync_handler
    .extern lower_el3_aarch32_irq_handler
    .extern lower_el3_aarch32_fiq_handler
    .extern lower_el3_aarch32_serror_handler
    #----------------------------------------------------------------
    .extern curr_el2_sp0_sync_handler
    .extern curr_el2_sp0_irq_handler
    .extern curr_el2_sp0_fiq_handler
    .extern curr_el2_sp0_serror_handler
    .extern curr_el2_spx_sync_handler
    #.extern irq_handler
    .extern curr_el2_spx_fiq_handler
    .extern curr_el2_spx_serror_handler
    .extern lower_el2_aarch64_sync_handler
    .extern lower_el2_aarch64_irq_handler
    .extern lower_el2_aarch64_fiq_handler
    .extern lower_el2_aarch64_serror_handler
    .extern lower_el2_aarch32_sync_handler
    .extern lower_el2_aarch32_irq_handler
    .extern lower_el2_aarch32_fiq_handler
    .extern lower_el2_aarch32_serror_handler
    #----------------------------------------------------------------
    .extern curr_el1_sp0_sync_handler
    .extern curr_el1_sp0_irq_handler
    .extern curr_el1_sp0_fiq_handler
    .extern curr_el1_sp0_serror_handler
    .extern curr_el1_spx_sync_handler
    #.extern irq_handler
    .extern curr_el1_spx_fiq_handler
    .extern curr_el1_spx_serror_handler
    .extern lower_el1_aarch64_sync_handler
    .extern lower_el1_aarch64_irq_handler
    .extern lower_el1_aarch64_fiq_handler
    .extern lower_el1_aarch64_serror_handler
    .extern lower_el1_aarch32_sync_handler
    .extern lower_el1_aarch32_irq_handler
    .extern lower_el1_aarch32_fiq_handler
    .extern lower_el1_aarch32_serror_handler

    .equ GIC400_GICC_IAR, 0x7D00200C
    .equ GIC400_GICC_EOIR, 0x7D002010
    
    .section  ".exception_table","ax"

 VECTORS:
	ldr X0,=__startup
    br  X0 
       
      
#++++++++++++++++++++++++++++++++++++++++++++++++#
#+++++++++++++Vector table start+++++++++++++++++#
#++++++++++++++++++++++++++++++++++++++++++++++++#
         #Start of vectors
        .balign 0x800
el3_vector_table:
#
#Current EL with SP0
#
curr_el3_sp0_sync:
        b       curr_el3_sp0_sync_vector
         #Ensure next vector is aligned to the correct boundary
        .balign 0x80
curr_el3_sp0_irq:
        b       curr_el3_sp0_irq_vector
        .balign 0x80
curr_el3_sp0_fiq:
        b       curr_el3_sp0_fiq_vector
        .balign 0x80
curr_el3_sp0_serror:
        b       curr_el3_sp0_serror_vector
        .balign 0x80
#        
#Current EL with SPx
#
curr_el3_spx_sync:
        b       curr_el3_spx_sync_vector
        .balign 0x80
curr_el3_spx_irq:
        b       curr_el3_spx_irq_vector
        .balign 0x80
curr_el3_spx_fiq:
        b       curr_el3_spx_fiq_vector
        .balign 0x80
curr_el3_spx_serror:
        b       curr_el3_spx_serror_vector
        .balign 0x80

#
#Lower EL using AArch64
#
lower_el3_aarch64_sync:
        b       lower_el3_aarch64_sync_vector
        .balign 0x80
lower_el3_aarch64_irq:
        b       lower_el3_aarch64_irq_vector
        .balign 0x80
lower_el3_aarch64_fiq:
        b       lower_el3_aarch64_fiq_vector
        .balign 0x80
lower_el3_aarch64_serror:
        b       lower_el3_aarch64_serror_vector
        .balign 0x80

#
#Lower EL using AArch32
#
lower_el3_aarch32_sync:
        b       lower_el3_aarch32_sync_vector
        .balign 0x80
lower_el3_aarch32_irq:
        b       lower_el3_aarch32_irq_vector
        .balign 0x80
lower_el3_aarch32_fiq:
        b       lower_el3_aarch32_fiq_vector
        .balign 0x80
lower_el3_aarch32_serror:
        b       lower_el3_aarch32_serror_vector
        .balign 0x80

el2_vector_table:
#
#Current EL with SP0
#
curr_el2_sp0_sync:
        b       curr_el2_sp0_sync_vector
        // Ensure next vector is aligned to the correct boundary
        .balign 0x80
curr_el2_sp0_irq:
        b       curr_el2_sp0_irq_vector
        .balign 0x80
curr_el2_sp0_fiq:
        b       curr_el2_sp0_fiq_vector
        .balign 0x80
curr_el2_sp0_serror:
        b       curr_el2_sp0_serror_vector
        .balign 0x80

#
#Current EL with SPx
#
curr_el2_spx_sync:
        b       curr_el2_spx_sync_vector
        .balign 0x80
curr_el2_spx_irq:
        b       curr_el2_spx_irq_vector
        .balign 0x80
curr_el2_spx_fiq:
        b       curr_el2_spx_fiq_vector
        .balign 0x80
curr_el2_spx_serror:
        b       curr_el2_spx_serror_vector
        .balign 0x80

#
#Lower EL using AArch64
#
lower_el2_aarch64_sync:
        b       lower_el2_aarch64_sync_vector
        .balign 0x80
lower_el2_aarch64_irq:
        b       lower_el2_aarch64_irq_vector
        .balign 0x80
lower_el2_aarch64_fiq:
        b       lower_el2_aarch64_fiq_vector
        .balign 0x80
lower_el2_aarch64_serror:
        b       lower_el2_aarch64_serror_vector
        .balign 0x80

#
#Lower EL using AArch32
#
lower_el2_aarch32_sync:
        b       lower_el2_aarch32_sync_vector
        .balign 0x80
lower_el2_aarch32_irq:
        b       lower_el2_aarch32_irq_vector
        .balign 0x80
lower_el2_aarch32_fiq:
        b       lower_el2_aarch32_fiq_vector
        .balign 0x80
lower_el2_aarch32_serror:
        b       lower_el2_aarch32_serror_vector
        .balign 0x80


el1_vector_table:
#
#Current EL with SP0
#
curr_el1_sp0_sync:
        b       curr_el1_sp0_sync_vector
        // Ensure next vector is aligned to the correct boundary
        .balign 0x80
curr_el1_sp0_irq:
        b       curr_el1_sp0_irq_vector
        .balign 0x80
curr_el1_sp0_fiq:
        b       curr_el1_sp0_fiq_vector
        .balign 0x80
curr_el1_sp0_serror:
        b       curr_el1_sp0_serror_vector
        .balign 0x80

#
#Current EL with SPx
#
curr_el1_spx_sync:
        b       curr_el1_spx_sync_vector
        .balign 0x80
curr_el1_spx_irq:
        b       curr_el1_spx_irq_vector
        .balign 0x80
curr_el1_spx_fiq:
        b       curr_el1_spx_fiq_vector
        .balign 0x80
curr_el1_spx_serror:
        b       curr_el1_spx_serror_vector
        .balign 0x80

#
#Lower EL using AArch64
#
lower_el1_aarch64_sync:
        b       lower_el1_aarch64_sync_vector
        .balign 0x80
lower_el1_aarch64_irq:
        b       lower_el1_aarch64_irq_vector
        .balign 0x80
lower_el1_aarch64_fiq:
        b       lower_el1_aarch64_fiq_vector
        .balign 0x80
lower_el1_aarch64_serror:
        b       lower_el1_aarch64_serror_vector
        .balign 0x80

#
#Lower EL using AArch32
#
lower_el1_aarch32_sync:
        b       lower_el1_aarch32_sync_vector
        .balign 0x80
lower_el1_aarch32_irq:
        b       lower_el1_aarch32_irq_vector
        .balign 0x80
lower_el1_aarch32_fiq:
        b       lower_el1_aarch32_fiq_vector
        .balign 0x80
lower_el1_aarch32_serror:
        b       lower_el1_aarch32_serror_vector
        .balign 0x80

#----------------------------------------------------------------
#
#Branching to C side for handlers
#
curr_el3_sp0_sync_vector       :     b curr_el3_sp0_sync_handler
curr_el3_sp0_irq_vector        :     b irq_handler
curr_el3_sp0_fiq_vector        :     b curr_el3_sp0_fiq_handler
curr_el3_sp0_serror_vector     :     b serror_handler
curr_el3_spx_sync_vector       :     b curr_el3_spx_sync_handler
curr_el3_spx_irq_vector        :     b irq_handler
curr_el3_spx_fiq_vector        :     b curr_el3_spx_fiq_handler
curr_el3_spx_serror_vector     :     b serror_handler
lower_el3_aarch64_sync_vector  :     b lower_el3_aarch64_sync_handler
lower_el3_aarch64_irq_vector   :     b lower_el3_aarch64_irq_handler
lower_el3_aarch64_fiq_vector   :     b lower_el3_aarch64_fiq_handler
lower_el3_aarch64_serror_vector:     b lower_el3_aarch64_serror_handler
lower_el3_aarch32_sync_vector  :     b lower_el3_aarch32_sync_handler
lower_el3_aarch32_irq_vector   :     b lower_el3_aarch32_irq_handler
lower_el3_aarch32_fiq_vector   :     b lower_el3_aarch32_fiq_handler
lower_el3_aarch32_serror_vector:     b lower_el3_aarch32_serror_handler
#
#----------------------------------------------------------------
#
curr_el2_sp0_sync_vector       :     b curr_el2_sp0_sync_handler
curr_el2_sp0_irq_vector        :     b curr_el2_sp0_irq_handler
curr_el2_sp0_fiq_vector        :     b curr_el2_sp0_fiq_handler
curr_el2_sp0_serror_vector     :     b curr_el2_sp0_serror_handler
curr_el2_spx_sync_vector       :     b curr_el2_spx_sync_handler
curr_el2_spx_irq_vector        :     b irq_handler
curr_el2_spx_fiq_vector        :     b curr_el2_spx_fiq_handler
curr_el2_spx_serror_vector     :     b curr_el2_spx_serror_handler
lower_el2_aarch64_sync_vector  :     b lower_el2_aarch64_sync_handler
lower_el2_aarch64_irq_vector   :     b lower_el2_aarch64_irq_handler
lower_el2_aarch64_fiq_vector   :     b lower_el2_aarch64_fiq_handler
lower_el2_aarch64_serror_vector:     b lower_el2_aarch64_serror_handler
lower_el2_aarch32_sync_vector  :     b lower_el2_aarch32_sync_handler
lower_el2_aarch32_irq_vector   :     b lower_el2_aarch32_irq_handler
lower_el2_aarch32_fiq_vector   :     b lower_el2_aarch32_fiq_handler
lower_el2_aarch32_serror_vector:     b lower_el2_aarch32_serror_handler
#
#----------------------------------------------------------------
#
curr_el1_sp0_sync_vector       :     b curr_el1_sp0_sync_handler
curr_el1_sp0_irq_vector        :     b curr_el1_sp0_irq_handler
curr_el1_sp0_fiq_vector        :     b curr_el1_sp0_fiq_handler
curr_el1_sp0_serror_vector     :     b curr_el1_sp0_serror_handler
curr_el1_spx_sync_vector       :     b curr_el1_spx_sync_handler
curr_el1_spx_irq_vector        :     b irq_handler
curr_el1_spx_fiq_vector        :     b curr_el1_spx_fiq_handler
curr_el1_spx_serror_vector     :     b curr_el1_spx_serror_handler
lower_el1_aarch64_sync_vector  :     b lower_el1_aarch64_sync_handler
lower_el1_aarch64_irq_vector   :     b lower_el1_aarch64_irq_handler
lower_el1_aarch64_fiq_vector   :     b lower_el1_aarch64_fiq_handler
lower_el1_aarch64_serror_vector:     b lower_el1_aarch64_serror_handler
lower_el1_aarch32_sync_vector  :     b lower_el1_aarch32_sync_handler
lower_el1_aarch32_irq_vector   :     b lower_el1_aarch32_irq_handler
lower_el1_aarch32_fiq_vector   :     b lower_el1_aarch32_fiq_handler
lower_el1_aarch32_serror_vector:     b lower_el1_aarch32_serror_handler
#
#----------------------------------------------------------------
#


#++++++++++++++++++++++++++++++++++++++++++++++++#
#+++++++++++++++++INT HANDLERS+++++++++++++++++++#
#++++++++++++++++++++++++++++++++++++++++++++++++#
#
#----------------------------------------------------------------
#
#interrupt handler written in Assembly
#
irq_handler:
     SUB SP, SP, #176
     STP X0, X1, [SP, #16*0]
     STP X2, X3, [SP, #16*1]
     STP X4, X5, [SP, #16*2]
     STP X6, X7, [SP, #16*3]
     STP X8, X9, [SP, #16*4]
     STP X10, X11, [SP, #16*5]
     STP X12, X13, [SP, #16*6]
     STP X14, X15, [SP, #16*7]
     STP X16, X17, [SP, #16*8] 
     str x18, [SP,#144] //144
     str x30, [SP,#152]

     ldr     w0,= GIC400_GICC_IAR    //@; Interrupt Acknowledge register
     ldr     w3,[x0]                    //@; r0[12:10] = CPU_ID of software interrupts, r0[9:0] = INT_ID
     ldr     w0,=IRQ_NUMBER
     str     w3,[x0]
     ldr     w2,=0x00001C00
     and     w1,w3,w2
     lsr     w1,w1,#10       //@; Keep CPU_ID at r1
     ldr     w2,=0x00000200
     and     w2,w3,w2        
     cmp     w2,#0           //@; Check that INT_ID isn't 1023 or 1022 (spurious interrupt)
     b.ne     END_IRQ_HLDR
     ldr     w2,=0x000003FF
     and     w0,w3,w2        //@; Keep INT_ID at r0
     //@; Save original ICCIAR value
     str x3, [SP,#160]
 
     lsl     w2,w0,#3      // Interrupts are 64 bit aligned

     ldr w9, =g_INT_vectors     ///* ;      IRQ */
     add w9,w9,w2
     ldr w9, [x9]              ///* ;      IRQ */

     blr X9

 END_IRQ_HLDR:
     ldr x30, [SP,#152]
     ldr x18, [SP,#144]         //144
     LDP X16, X17, [SP, #16*8] 
     LDP X14, X15, [SP, #16*7]
     LDP X12, X13, [SP, #16*6]
     LDP X10, X11, [SP, #16*5]
     LDP X8, X9, [SP, #16*4]
     LDP X6, X7, [SP, #16*3]
     LDP X4, X5, [SP, #16*2]
     LDP X2, X3, [SP, #16*1]
     //@; Restore original ICCIAR value
     ldr x1, [SP, 160]
     ldr     x0,=GIC400_GICC_EOIR
     str     x1,[x0]         //@; Update End of Interrupt register with original value from ICCIAR
     LDP X0, X1, [SP, #16*0] //restore X0, X1
     ADD SP, SP, 176
     eret

     #
#----------------------------------------------------------------
#
serror_handler:
     stp x29, x30, [sp,#-192]!       /* SP should always be aligned to 16 byte boundary sp[3:0]=0. Otherwise, exception occurs*/
     mov x29, sp
     str x0, [x29,#32]
     str x1, [x29,#40]
     str x2, [x29,#48]
     str x3, [x29,#56]
     str x4, [x29,#64]
     str x5, [x29,#72]
     str x6, [x29,#80]
     str x7, [x29,#88]
     str x8, [x29,#96]
     str x9, [x29,#104]
     str x10, [x29,#112]
     str x11, [x29,#120]
     str x12, [x29,#128]
     str x13, [x29,#136]
     str x14, [x29,#144]
     str x15, [x29,#152]
     str x16, [x29,#160]
     str x17, [x29,#168]
     str x18, [x29,#176]
     ldr x9,  =curr_el3_spx_serror_handler
     blr X9
     ldr x0, [x29,#32]
     ldr x1, [x29,#40]
     ldr x2, [x29,#48]
     ldr x3, [x29,#56]
     ldr x4, [x29,#64]
     ldr x5, [x29,#72]
     ldr x6, [x29,#80]
     ldr x7, [x29,#88]
     ldr x8, [x29,#96]
     ldr x9, [x29,#104]
     ldr x10, [x29,#112]
     ldr x11, [x29,#120]
     ldr x12, [x29,#128]
     ldr x13, [x29,#136]
     ldr x14, [x29,#144]
     ldr x15, [x29,#152]
     ldr x16, [x29,#160]
     ldr x17, [x29,#168]
     ldr x18, [x29,#176]
     ldp x29,x30, [sp],#192
     eret
#
#----------------------------------------------------------------
#

INTERRUPT_0:    
.word 0
