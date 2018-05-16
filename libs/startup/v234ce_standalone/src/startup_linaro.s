/*=================================================================================================*/
/**
*   @file    startup.c
*   @version M4_SRC_SW_VERSION_MAJOR.M4_SRC_SW_VERSION_MINOR.M4_SRC_SW_VERSION_PATCH
*
*   @brief   AUTOSAR M4_SRC_MODULE_NAME - Startup file for ARM.
*   @details Startup file for ARM.
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

',)dnl - DO NOT modify this M4 line!
==================================================================================================*/

.extern el1_vector_table
.extern el2_vector_table
.extern el3_vector_table
.extern _entry
.extern dcd
.extern start_app
.extern boot_data

.global    __startup
.global    IRQ_NUMBER
.global flush_icache_range
.global write_ttbr0_el3
.global write_tcr_el3
.global write_sctlr_el3
.global read_sctlr_el3
.global write_mair_el3
.global tlbialle3

.include "v8_system_linaro.s" 

.section ".startup","ax"

#include "arm64_regs.h" 

.align    16

  
write_ttbr0_el3:
	MSR TTBR0_EL3, X0
  ret

write_tcr_el3:
  MSR TCR_EL3, X0
  ret

write_sctlr_el3:
  MSR SCTLR_EL3, X0
  ret

write_mair_el3:
  MSR MAIR_EL3, X0
  ret

tlbialle3:
  tlbi    alle3
  dsb     sy
  isb
  ret

read_sctlr_el3:
  MRS X0, SCTLR_EL3
  ret
  
  /* ------------------------------------------
   * Clean+Invalidate from base address till
   * size. 'x0' = addr, 'x1' = size
   * ------------------------------------------
   */
.global flush_and_invalidate_dcache_range
flush_and_invalidate_dcache_range:
  mrs x3, CTR_EL0
  ubfx  x3, x3, #16, #4
  mov x2, #4
  lsl x2, x2, x3
  add x1, x0, x1
  sub x3, x2, #1
  bic x0, x0, x3
dflushinvalidate_loop:
  dc  civac, x0
  add x0, x0, x2
  cmp x0, x1
  b.lo    dflushinvalidate_loop
  dsb sy
  ret
  
.global invalidate_dcache_range
invalidate_dcache_range:
  mrs x3, CTR_EL0
  ubfx  x3, x3, #16, #4
  mov x2, #4
  lsl x2, x2, x3
  add x1, x0, x1
  sub x3, x2, #1
  bic x0, x0, x3
inv_dflush_loop:
  dc  ivac, x0
  add x0, x0, x2
  cmp x0, x1
  b.lo    inv_dflush_loop
  dsb ish
  ret
  
.global flush_dcache_range
flush_dcache_range:
  mrs x3, CTR_EL0
  ubfx  x3, x3, #16, #4
  mov x2, #4
  lsl x2, x2, x3
  add x1, x0, x1
  sub x3, x2, #1
  bic x0, x0, x3
dflush_loop:
  dc  cvac, x0
  add x0, x0, x2
  cmp x0, x1
  b.lo    dflush_loop
  dsb sy
  ret

flush_icache_range:
  mrs x3, CTR_EL0
  and x3, x3, #0xf
  mov x2, #4
  lsl x2, x2, x3
  add x1, x0, x1
  sub x3, x2, #1
  bic x0, x0, x3
iflush_loop:
  dc  civac, x0
  add x0, x0, x2
  cmp x0, x1
  b.lo    iflush_loop
  dsb sy
  ret
  
  
.align    16
/*******************************************************************/
/* Freescale Guidance 1 Initialize GP register file                */
/*******************************************************************/
__startup:
		/* Forced link of the boot code */
		adrp x0, _entry
		adrp x0, dcd
		adrp x0, start_app
		adrp x0, boot_data
		
        /* Initialise the register bank */
        mov     x0,  xzr
        mov     x1,  xzr
        mov     x2,  xzr
        mov     x3,  xzr
        mov     x4,  xzr
        mov     x5,  xzr
        mov     x6,  xzr
        mov     x7,  xzr
        mov     x8,  xzr
        mov     x9,  xzr
        mov     x10, xzr
        mov     x11, xzr
        mov     x12, xzr
        mov     x13, xzr
        mov     x14, xzr
        mov     x15, xzr
        mov     x16, xzr
        mov     x17, xzr
        mov     x18, xzr
        mov     x19, xzr
        mov     x20, xzr
        mov     x21, xzr
        mov     x22, xzr
        mov     x23, xzr
        mov     x24, xzr
        mov     x25, xzr
        mov     x26, xzr
        mov     x27, xzr
        mov     x28, xzr
        mov     x29, xzr
        mov     x30, xzr
        
    /* initialize SIMD & FP registers */
        MOVI     d0, #0
        MOVI     d1, #0
        MOVI     d2, #0
        MOVI     d3, #0
        MOVI     d4, #0
        MOVI     d5, #0
        MOVI     d6, #0
        MOVI     d7, #0
        MOVI     d8, #0
        MOVI     d9, #0
        MOVI     d10, #0
        MOVI     d11, #0
        MOVI     d12, #0
        MOVI     d13, #0
        MOVI     d14, #0
        MOVI     d15, #0
        MOVI     d16, #0
        MOVI     d17, #0
        MOVI     d18, #0
        MOVI     d19, #0
        MOVI     d20, #0
        MOVI     d21, #0
        MOVI     d22, #0
        MOVI     d23, #0
        MOVI     d24, #0
        MOVI     d25, #0
        MOVI     d26, #0
        MOVI     d27, #0
        MOVI     d28, #0
        MOVI     d29, #0
        MOVI     d30, #0

//#########################################
//# Part 2: Initialize the stack pointer  #
//#########################################
        mov     sp,       x0
        msr     SP_EL0,   x0
        msr     SP_EL1,   x0
        msr     SP_EL2,   x0
        msr     ELR_EL1,  x0 //exception link register 
        msr     ELR_EL2,  x0 //exception link register
        msr     ELR_EL3,  x0 //exception link register
        msr     SPSR_EL1, x0 //saved program status register
        // Initialize the stack pointer
        ldr     x1,=__SP_IRQ2_INIT //load the adress of the end of the stack
        mov     sp, x1		//and store it into stack pointer register
        msr     SPSR_EL2, x0 //saved program status register
        // Initialize the stack pointer
        ldr     x1,=__SP_IRQ_INIT //load the adress of the end of the stack
        mov     sp, x1		//and store it into stack pointer register
        msr     SPSR_EL3, x0 //saved program status register
        // Initialize the stack pointer
        ldr     x1,=__SP_INIT //load the adress of the end of the stack
        mov     sp, x1		//and store it into stack pointer register
		
//################################################
//# Part 3: configuring interrupt related issues #
//################################################
    # Configure SCR_EL3
    # ------------------
    #
    # lower exception levels are in the non-secure world, with no access
    # back to EL2 or EL3, and are AArch64 capable
    # --------------------------
        mov     w3,   #SCR_EL3_RW 
        ORR     w3,w3,#SCR_EL3_EA 
        ORR     w3,w3,#SCR_EL3_FIQ 
        ORR     w3,w3,#SCR_EL3_IRQ 
        ORR     w3,w3,#SCR_EL3_SMD 
        ORR     w3,w3,#SCR_EL3_NS
	    msr     SCR_EL3, x3
        
    #
    # no traps or VM modifications from the Hypervisor, EL1 is AArch64
    # -------------------------
	    mov	x2, #HCR_EL2_RW
	    msr	HCR_EL2, x2
        
    #
    # VMID is still significant, even when virtualisation is not being used: ensure VTTBR_EL2 is properly initialised
    # ------------------------
	    msr	VTTBR_EL2, xzr
        
    #
    # set SCTLRs for lower ELs to safe values
    #
    # note that setting SCTLR_EL2 is not strictly needed, since we're never in EL2
    # ------------------------
	    msr	SCTLR_EL1, xzr
	    msr	SCTLR_EL2, xzr

    #
    # Configure CNTKCTL register 
    # -----------------------
        MOV w1, #0
        ORR w1, w1, #(1 << 9)   
        ORR w1, w1, #(1 << 8)   
        ORR w1, w1, #(1 << 1)   
        ORR w1, w1, #(1 << 0)   
        MSR CNTKCTL_EL1,x1 //Write X1 to CNTKCTL_EL1


    # Configure DAIF
    # ------------------
        MSR DAIFClr,#0xf
        
    # Install EL3 vector table
    # -------------------------
    # VBAR_EL3 needs to be set to the base address of the EL3 vector table.  The label for this is "vector_table"

	    LDR	x1, =el1_vector_table
	    MSR	VBAR_EL1, x1

	    LDR	x2, =el2_vector_table
	    MSR	VBAR_EL2, x2

	    LDR	x3, =el3_vector_table
	    MSR	VBAR_EL3, x3

//###############################################################################
//# Part 4: Enable Floating Point register access. 
//###############################################################################

        MRS      x0, CPTR_EL3
        MOV      w1, #0x400 //TFP bit selection
        BIC      w0, w0, w1 //clearing the TPF bit
        MSR      CPTR_EL3, x0
	  

////###############################################################################
////# Part 6: Enable cahces . 
////###############################################################################

    // @; disabling all MMU, icache and dcache
       MRS x1, SCTLR_EL3       // @; Read System Control Register (SCTLR_EL3)
       BIC x1, x1, x2 ,LSL #0 //(1 <<0)   //  @;           mmu off
       ORR x1, x1, #(1 << 12)  //  @;       i-cache off
       BIC x1, x1, x2, LSL #2 //#(1 << 2)  //  @;      d-cache & L2-$ off
       MSR SCTLR_EL3, x1      //  @; Write System Control Register (SCTLR_EL3)

    // # initialize mmu

	    mov 	x0,         #0
        orr     x0,         x0,        #(1 << 6)     //  @ write back, write allocate
        MSR     TTBR0_EL3,  x0                       // @; Write Xt(r0) to TTBR0_EL3
        ldr     x0,         =0xFFFFFFFF
        MSR     DACR32_EL2, x0 //; Write X0 to DACR32_EL2
	
   // Enable caches and MMU


        mrs     x1, SCTLR_EL3          //     @; get control register (SCTLR_EL3 inot x1)
        orr     x1, x1, #(1 << 0)      //    @; M bit (MMU enabled)
        bic     x1, x1, x2, LSL #1//#(1 << 1)      //    @; A bit (Strict aligment checking)
        orr     x1, x1, #(1 << 11)       //    @; Program flow prediction
        orr     x1, x1, #(1 << 2)      //    @; C bit (Data cache)

        orr     x1, x1, #(1 << 12)     //    @; I bit (Instruction cache)


 
 


/******************************************************************/
/* Call Main Routine                                              */
/******************************************************************/
bl startup

/******************************************************************/
/* Init runtime check data space                                  */
/******************************************************************/
   b . 
   
 IRQ_NUMBER:
    .long 0
	
.align	8
TCR_EL3_SETUP:
   .word   0x80803519, 0

    

