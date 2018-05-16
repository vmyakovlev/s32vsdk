/**
  @addtogroup       dxgrMMU
  @{

  @file             mmu_exception.c
  @brief            The MMU exception handler
  @details          

  Project           M4_ETH_PROJECT_NAME

  Platform          M4_ETH_PLATFORM

  SWVersion         M4_ETH_RELEASE_VERSION

  Copyright (c) 2016 NXP Semiconductors
  All Rights Reserved.
*/
/*==================================================================================================
==================================================================================================*/

/*==================================================================================================
                                         MISRA VIOLATIONS
==================================================================================================*/
/*
* @page misra_violations MISRA-C:2004 violations
*
* @section mmu_exception_c_global
* Violates MISRA 2004 Required Rule 20.2, The names of standard library macros, objects and functions
* shall not be reused.
* Identifiers beginning with 'E' used to match their names as given by HW reference manual.
*
* @section mmu_exception_c_REF_1
* Violates MISRA 2004 Required Rule 18.4, Unions shall not be used.
* Use of union to represent data is suitable here and significantly reduces code complexity. It
* has been proven that currently supported compiler accesses memory as expected.
*
* @section mmu_exception_c_REF_2
* Violates MISRA 2004 Required Rule 9.1, All automatic variables shall have been assigned a value
* before being used.
* Variable is set in an __asm statement but usage is not detected.
*
* @section mmu_exception_c_REF_3
* Violates MISRA 2004 Required Rule 8.10, All declarations and definitions of objects or functions
* at file scope shall have internal linkage unless external linkage is required. External linkage
* is required due to reference from assembly part of the module.
*/

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "common_types.h"
#include "common.h"
#include "mmu_exception.h"

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/
/** @cond dxINTERNAL_MACROS */
/*  Supported exception classes: */
#define INSTRUCTION_ABORT_LOWER_EL      0x20    /* 0b100000 */
#define INSTRUCTION_ABORT_CURRENT_EL    0x21    /* 0b100001 */
#define DATA_ABORT_LOWER_EL             0x24    /* 0b100100 */
#define DATA_ABORT_CURRENT_EL           0x25    /* 0b100101 */
/** @endcond */

/*==================================================================================================
                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
/*  Instruction Abort IFSC */
typedef enum {
    IA_ADDR_SIZE_FAULT_0th = 0x0,
    IA_ADDR_SIZE_FAULT_1st = 0x1,
    IA_ADDR_SIZE_FAULT_2nd = 0x2,
} ia_ifsc_t;

/*  Instruction Abort ISS */
/** @violates @ref mmu_exception_c_REF_1 MISRA rule 18.4 */
typedef union {
    uint32_t val;
    struct {
        uint32_t IFSC : 6;
        uint32_t : 1;
        uint32_t S1PTW : 1;
        uint32_t : 1;
        uint32_t EA : 1;
        uint32_t FnV : 1;
        uint32_t : 21;
    } iss;
} ia_iss_t;

/*  Data Abort DFSC */
typedef enum {
    DA_ADDR_SIZE_FAULT_0th         = 0x00, /* 0b000000 */
    DA_ADDR_SIZE_FAULT_1st         = 0x01, /* 0b000001 */
    DA_ADDR_SIZE_FAULT_2nd         = 0x02, /* 0b000010 */
    DA_ADDR_SIZE_FAULT_3rd         = 0x03, /* 0b000011 */
    DA_TRANSLATION_FAULT_0th       = 0x04, /* 0b000100 */
    DA_TRANSLATION_FAULT_1st       = 0x05, /* 0b000101 */
    DA_TRANSLATION_FAULT_2nd       = 0x06, /* 0b000110 */
    DA_TRANSLATION_FAULT_3rd       = 0x07, /* 0b000111 */
    DA_ACCESS_FALG_FAULT_1st       = 0x09, /* 0b001001 */
    DA_ACCESS_FALG_FAULT_2nd       = 0x0a, /* 0b001010 */
    DA_ACCESS_FALG_FAULT_3rd       = 0x0b, /* 0b001011 */
    DA_PERMISSION_FAULT_1st        = 0x0d, /* 0b001101 */
    DA_PERMISSION_FAULT_2nd        = 0x0e, /* 0b001110 */
    DA_PERMISSION_FAULT_3rd        = 0x0f, /* 0b001111 */
    DA_SYNC_EA_NOT_TW              = 0x10, /* 0b010000 */
    DA_SYNC_PARITYECC_NOT_TW       = 0x18, /* 0b011000 */
    DA_SYNC_EA_TW_0th              = 0x14, /* 0b010100 */
    DA_SYNC_EA_TW_1st              = 0x15, /* 0b010101 */
    DA_SYNC_EA_TW_2nd              = 0x16, /* 0b010110 */
    DA_SYNC_EA_TW_3rd              = 0x17, /* 0b010111 */
    DA_SYNC_PARITYECC_TW_0th       = 0x1c, /* 0b011100 */
    DA_SYNC_PARITYECC_TW_1st       = 0x1d, /* 0b011101 */
    DA_SYNC_PARITYECC_TW_2nd       = 0x1e, /* 0b011110 */
    DA_SYNC_PARITYECC_TW_3rd       = 0x1f, /* 0b011111 */
    DA_ALIGNMENT_FAULT             = 0x21, /* 0b100001 */
    DA_TLB_CONFLICT_ABORT          = 0x30, /* 0b110000 */
    DA_LOCKDOWN_FAULT              = 0x34, /* 0b110100 */
    DA_UNSUPPORTED_ExACCESS_FAULT  = 0x35, /* 0b110101 */
    DA_SECTION_DOMAIN_FAULT        = 0x3d, /* 0b111101 */
    DA_PAGE_DOMAIN_FAULT           = 0x3e  /* 0b111110 */
} da_dfsc_t;

/*  Data Abort WnR */
typedef enum {
    DA_BY_READ_INSTRUCTION = 0,
    DA_BY_WRITE_INSTRUCTION = 1
} da_WnR_t;

/*  Data Abort ISS */
/** @violates @ref mmu_exception_c_REF_1 MISRA rule 18.4 */
typedef union {
    uint32_t val;
    struct {
        uint32_t DFSC : 6;      /*  Data fault status code */
        uint32_t WnR : 1;       /*  Write not Read */
        uint32_t S1PTW : 1;     /*  Fault on stage 2 */
        uint32_t CM : 1;        /*  Fault from Cache Maintenance Instruction */
        uint32_t EA : 1;        /*  External abort type */
        uint32_t FnV : 1;       /*  FAR not Valid */
        uint32_t : 3;
        uint32_t AR : 1;        /*  Acquire/Release - instruction had/had not acquire/release semantics*/
        uint32_t SF : 1;        /*  Sixty-Four register accessed */
        uint32_t SRT : 5;       /*  Syndrome Register Transfer */
        uint32_t SSE : 1;       /*  Syndrome Sign Extend */
        uint32_t SAS : 2;       /*  Syndrome Access Size */
        uint32_t ISV : 1;       /*  Instruction Syndrome Valid */
        uint32_t : 7;
    } iss;
} da_iss_t;

/*==================================================================================================
                                       LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
void mmu_exception_handler(uint32_t u32esr_elx);

/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/
/**
    @brief The exception handler
    @details Function in intended to process MMU faults and shall be hooked to
             the sync exception handler. Note that current implementation is just a stub
             and can be used for debugging purposes only.
    @param[in] u32esr_elx The current ESR_ELx value
*/
/** @violates @ref mmu_exception_c_REF_3 MISRA rule 8.10 */
void mmu_exception_handler(uint32_t u32esr_elx)
{
    uint32_t esr, iss, il, ec;
    uint64_t u64Far = 0ULL;
    ia_iss_t ia_syndrome;
    da_iss_t da_syndrome;
    
    (void)u32esr_elx;
    
    ia_syndrome.val = 0U;
    da_syndrome.val = 0U;
    
    __asm(" mrs %0, esr_el3"
            :"=r"(esr) : : );
    
    /** @violates @ref mmu_exception_c_REF_2 MISRA rule 9.1 */
    iss = esr & 0x1ffffffU;
    il = (esr >> 25) & 0x1U;
    ec = (esr >> 26);
    
    __asm(" mrs %0, far_el3"
            :"=r"(u64Far) : : );
    
    switch (ec)
    {
        case INSTRUCTION_ABORT_LOWER_EL:
        case INSTRUCTION_ABORT_CURRENT_EL:
            ia_syndrome.val = iss;
            /*  TODO: Perform appropriate action */
            break;
        case DATA_ABORT_LOWER_EL:
        case DATA_ABORT_CURRENT_EL:
            da_syndrome.val = iss;
            /*  TODO: Perform appropriate action */
            break;
        default:
            break; /* Unknown class */
    }
    
    UNUSED(ia_syndrome);
    UNUSED(da_syndrome);
    UNUSED(il);
    UNUSED(u64Far);
    
    /*
    while(1)
    {
        ;
    }
    */
}

/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/

/** @}*/
