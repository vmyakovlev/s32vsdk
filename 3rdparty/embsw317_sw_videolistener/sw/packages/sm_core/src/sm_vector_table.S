/**
  @file             sm_vector_table.S
  @version          0.0.0.0

  @brief            The Secure Monitor vector table
  @details          

  Project           M4_ETH_PROJECT_NAME
  Platform          M4_ETH_PLATFORM

  SWVersion         M4_ETH_RELEASE_VERSION


  Copyright (c) 2016 NXP Semiconductors
  All Rights Reserved
*/

#include "sm_cfg.h"

.global sm_vector_table
.global sm_install_table
.global sm_vector_table_version

.extern el3_lowel_aarch64_dispatcher
.extern __sm_sc_prm_start

.section SM_CFG_VT_SECTION
.balign 0x800
/** @implements VLREQ034 */
/** @implements VLREQ036 */
sm_vector_table:
/*  Current Exception level with SP_EL0 */
    b       .                           /*  Sync,           +0x000 */
    .balign 0x80
    b       .                           /*  IRQ/vIRQ,       +0x080 */
    .balign 0x80
    b       .                           /*  FIQ/vFIQ,       +0x100 */
    .balign 0x80
    b       .                           /*  SError/vSError  +0x180 */
    .balign 0x80
        
/*  Current Exception level with SP_ELx, x>0 */
    /*  Return from here is not expected yet... */
#if (TRUE == SM_CFG_ENABLE_MMU)         /*  Sync,           +0x200 */
    bl mmu_exception_handler
#endif /* SM_CFG_ENABLE_MMU */
    b   .
    .balign 0x80
    b   .                               /*  IRQ/vIRQ,       +0x280 */
    .balign 0x80
    b   .                               /*  FIQ/vFIQ,       +0x300 */
    .balign 0x80
    b   .                               /*  SError/vSError  +0x380 */
    .balign 0x80

/*  Lower Exception level AArch64 */
__sm_init:
    b   sm_init                         /*  Sync,           +0x400 */
    stp x2, x3, [sp, #-16]!
    ldr x2, =__sm_sc_prm_start
    stp x0, x1, [x2]
    stp x4, x5, [sp, #-16]!
    stp x6, x7, [sp, #-16]!
    stp x8, x9, [sp, #-16]!
    stp x10, x11, [sp, #-16]!
    stp x12, x13, [sp, #-16]!
    stp x14, x15, [sp, #-16]!
    stp x16, x17, [sp, #-16]!
    stp x18, SM_CFG_LR, [sp, #-16]!
    bl  sm_aarch64_sync_dispatcher
    ldp x18, SM_CFG_LR, [sp], #16
    ldp x16, x17, [sp], #16
    ldp x14, x15, [sp], #16
    ldp x12, x13, [sp], #16
    ldp x10, x11, [sp], #16
    ldp x8, x9, [sp], #16
    ldp x6, x7, [sp], #16
    ldp x4, x5, [sp], #16
    ldr x2, =__sm_sc_qr_start
    ldp x0, x1, [x2]
    ldp x2, x3, [sp], #16
    eret
    .balign 0x80
    b   .                               /*  IRQ/vIRQ,       +0x480 */
    .balign 0x80
    stp SM_CFG_LR, x0, [sp, #-16]!      /*  FIQ/vFIQ,       +0x500 */
    stp x1, x2, [sp, #-16]!
    stp x3, x4, [sp, #-16]!
    stp x5, x6, [sp, #-16]!
    stp x7, x8, [sp, #-16]!
    stp x9, x10, [sp, #-16]!
    stp x11, x12, [sp, #-16]!
    stp x13, x14, [sp, #-16]!
    stp x15, x16, [sp, #-16]!
    stp x17, x18, [sp, #-16]!
    ldr x1, =SM_CFG_IAR_ADDR
    ldr w0, [x1]
    bl sm_ll_fiq_handler
    cmp w0, 0x3ff
    b.eq 0f
    ldr x1, =SM_CFG_EOIR_ADDR
    str w0, [x1]
    0:
    ldp x17, x18, [sp], #16
    ldp x15, x16, [sp], #16
    ldp x13, x14, [sp], #16
    ldp x11, x12, [sp], #16
    ldp x9, x10, [sp], #16
    ldp x7, x8, [sp], #16
    ldp x5, x6, [sp], #16
    ldp x3, x4, [sp], #16
    ldp x1, x2, [sp], #16
    ldp SM_CFG_LR, x0, [sp], #16
    eret
sm_vector_table_version:
  .quad 0x00010003
  .balign 0x80
    b   .                               /*  SError/vSError  +0x580 */
    .balign 0x80

/*  Lower Exception level AArch32 */
    b   .                               /*  Sync,           +0x600 */
    .balign 0x80
    b   .                               /*  IRQ/vIRQ,       +0x680 */
    .balign 0x80
    b   .                               /*  FIQ/vFIQ,       +0x700 */
    .balign 0x80
    b   .                               /*  SError/vSError  +0x780 */
    .balign 0x80

.section SM_CFG_INSTALLER_SECTION
sm_install_table:
    stp x0, x1, [sp, #-16]!
    ldr x0, =sm_vector_table
    msr vbar_el3, x0
    ldp x0, x1, [sp], #16
    ret

/** @implements VLREQ037 */
sm_init:
    /*  Code expects that the entry here is treated so x7, x8 can be corrupted */
    ldr x7, =__SM_STACK_START
    mov sp, x7
    adr x7, __sm_init
    ldr x8, SM_CFG_LL_INIT_LOCK_VALUE
    str w8, [x7]
    dsb sy
    stp x0, SM_CFG_LR, [sp, #-16]!
    stp x1, x2, [sp, #-16]!
    stp x3, x4, [sp, #-16]!
    stp x5, x6, [sp, #-16]!
    stp x9, x10, [sp, #-16]!
    stp x11, x12, [sp, #-16]!
    stp x13, x14, [sp, #-16]!
    stp x15, x16, [sp, #-16]!
    stp x17, x18, [sp, #-16]!
    bl sm_hl_init
    ldp x17, x18, [sp], #16
    ldp x15, x16, [sp], #16
    ldp x13, x14, [sp], #16
    ldp x11, x12, [sp], #16
    ldp x9, x10, [sp], #16
    ldp x5, x6, [sp], #16
    ldp x3, x4, [sp], #16
    ldp x1, x2, [sp], #16
    ldp SM_CFG_LR, x0, [sp], #16
    
    adr x7, __sm_init
    add x7, x7, 0x4
    ret x7
