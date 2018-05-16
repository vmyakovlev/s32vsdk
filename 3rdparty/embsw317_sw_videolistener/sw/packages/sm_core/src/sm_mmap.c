/**
  @addtogroup       dxgrSM_CORE
  @{

  @file             sm_mmap.c
  @brief            The SM core memory management abstraction and configuration module
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
* @section sm_mmap_c_REF_1
* Violates MISRA 2004 Required Rule 20.2, The names of standard library macros, objects and
* functions shall not be reused.
* Leading underscores defines linker objects.
*
* @section sm_mmap_c_REF_2
* Violates MISRA 2004 Required Rule 14.7, A function shall have a single point of exit at the end
* of the function. Multiple exit points here improves code readability and decreases code complexity.
*
* @section sm_mmap_c_REF_3
* Violates MISRA 2004 Advisory Rule 11.3, A cast should not be preformed between a pointer type and
* an integral type. Cast is being performed due to retrieving address given by linker-defined
* symbols.
*
* @section sm_mmap_c_REF_4
* Violates MISRA 2004 Required Rule 1.2, No reliance shall be placed on undefined or unspecified 
* behaviour. Taking address of near auto variable is safe within this context.
*
* @section sm_mmap_c_REF_5
* Violates MISRA 2004 Required Rule 8.7, Objects shall be defined at block scope if they are only
* accessed from within a single function. This object is linker exported symbol and thus cant be
* defined at block scope.
*/


/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "common_types.h"
#include "common.h"
#include "sm_cfg.h"
#include "sm.h"
#if (TRUE == SM_CFG_ENABLE_MMU)
#include "mmu_mem_attr.h"
#include "mmu_exception.h"
#include "mmu_cfg.h"
#include "mmu.h"
#include "gic.h"

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/
/** @cond dxINTERNAL_MACROS */
/** @endcond */

/*==================================================================================================
                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
typedef unsigned long sm_mmap_long_t;

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
/** @violates @ref sm_mmap_c_REF_1 MISRA rule 20.2 */
/** @violates @ref sm_mmap_c_REF_5 MISRA rule 8.7 */
extern void *__sm_vector_table_start;
/** @violates @ref sm_mmap_c_REF_1 MISRA rule 20.2 */
/** @violates @ref sm_mmap_c_REF_5 MISRA rule 8.7 */
extern sm_mmap_long_t __sm_vector_table_size;
/** @violates @ref sm_mmap_c_REF_1 MISRA rule 20.2 */
/** @violates @ref sm_mmap_c_REF_5 MISRA rule 8.7 */
extern void *__sm_t_tables_start;
/** @violates @ref sm_mmap_c_REF_1 MISRA rule 20.2 */
/** @violates @ref sm_mmap_c_REF_5 MISRA rule 8.7 */
extern sm_mmap_long_t __sm_t_tables_size;
/** @violates @ref sm_mmap_c_REF_1 MISRA rule 20.2 */
/** @violates @ref sm_mmap_c_REF_5 MISRA rule 8.7 */
extern void *__sm_stack_start;
/** @violates @ref sm_mmap_c_REF_1 MISRA rule 20.2 */
/** @violates @ref sm_mmap_c_REF_5 MISRA rule 8.7 */
extern sm_mmap_long_t __sm_stack_size;
/** @violates @ref sm_mmap_c_REF_1 MISRA rule 20.2 */
/** @violates @ref sm_mmap_c_REF_5 MISRA rule 8.7 */
extern void *__sm_rodata_cached_start;
/** @violates @ref sm_mmap_c_REF_1 MISRA rule 20.2 */
/** @violates @ref sm_mmap_c_REF_5 MISRA rule 8.7 */
extern sm_mmap_long_t __sm_rodata_cached_size;
/** @violates @ref sm_mmap_c_REF_1 MISRA rule 20.2 */
/** @violates @ref sm_mmap_c_REF_5 MISRA rule 8.7 */
extern void *__sm_text_cached_start;
/** @violates @ref sm_mmap_c_REF_1 MISRA rule 20.2 */
/** @violates @ref sm_mmap_c_REF_5 MISRA rule 8.7 */
extern sm_mmap_long_t __sm_text_cached_size;
/** @violates @ref sm_mmap_c_REF_1 MISRA rule 20.2 */
/** @violates @ref sm_mmap_c_REF_5 MISRA rule 8.7 */
extern void *__sm_data_cached_start;
/** @violates @ref sm_mmap_c_REF_1 MISRA rule 20.2 */
/** @violates @ref sm_mmap_c_REF_5 MISRA rule 8.7 */
extern sm_mmap_long_t __sm_data_cached_size;
/** @violates @ref sm_mmap_c_REF_1 MISRA rule 20.2 */
/** @violates @ref sm_mmap_c_REF_5 MISRA rule 8.7 */
extern void *__sm_data_non_cached_start;
/** @violates @ref sm_mmap_c_REF_1 MISRA rule 20.2 */
/** @violates @ref sm_mmap_c_REF_5 MISRA rule 8.7 */
extern sm_mmap_long_t __sm_data_non_cached_size;

/*==================================================================================================
                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/
/*
    @brief Memory map initialization function
    @details Function is used by the SM Core module to initialize basic memory mapping. Symbols
             describing particular sections are delivered by the linker script.
    @warning It is not recommended to call this API without deep knowledge of what it is doing.
    @retval Zero Success
    @retval <0 Initialization has failed
*/
sint32_t sm_mmap_init(void)
{
    mmu_ret_t mmuRetVal;
    uint32_t u32Base, u32Length;
    
    if (MMU_E_OK != mmu_init())
    {
        /** @violates @ref sm_mmap_c_REF_2 MISRA rule 14.7 */
        return -100;
    }
    
    /*
        Vectors
        Note: Section could be RO but the SM SW writes here to indicate that initial
              initialization is done. This could be treated by disabling the MMU during
              the write (single per whole operation) or rework the indication to not
              use writes to the vectors area.
    */
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
    mmuRetVal = mmu_add_mapping(    (va_t)&__sm_vector_table_start,                         \
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
                                    (pa_t)&__sm_vector_table_start,                         \
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
                                    (mlen_t)&__sm_vector_table_size,                        \
                                    mmu_get_attr(MTYPE_NORMAL, MA_EL2_EL3_RW)   );
    if (MMU_E_OK != mmuRetVal)
    {
        /** @violates @ref sm_mmap_c_REF_2 MISRA rule 14.7 */
        return -102;
    }
    
    /*
        T-Tables
        Rationale: New mappings can be created during runtime so the map must exists.
    */
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
    mmuRetVal = mmu_add_mapping(    (va_t)&__sm_t_tables_start,                             \
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
                                    (pa_t)&__sm_t_tables_start,                             \
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
                                    (mlen_t)&__sm_t_tables_size,                            \
                                    mmu_get_attr(MTYPE_NORMAL, MA_EL2_EL3_RW | MA_NON_EXEC) );
    if (MMU_E_OK != mmuRetVal)
    {
        /** @violates @ref sm_mmap_c_REF_2 MISRA rule 14.7 */
        return -103;
    }
    
    /*
        Stack
    */
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
    mmuRetVal = mmu_add_mapping(    (va_t)&__sm_stack_start,                                \
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
                                    (pa_t)&__sm_stack_start,                                \
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
                                    (mlen_t)&__sm_stack_size,                               \
                                    mmu_get_attr(MTYPE_NORMAL, MA_EL2_EL3_RW | MA_NON_EXEC) );
    if (MMU_E_OK != mmuRetVal)
    {
        /** @violates @ref sm_mmap_c_REF_2 MISRA rule 14.7 */
        return -104;
    }
    
    /*
        RODATA section
    */
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
    mmuRetVal = mmu_add_mapping(    (va_t)&__sm_rodata_cached_start,                        \
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
                                    (pa_t)&__sm_rodata_cached_start,                        \
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
                                    (mlen_t)&__sm_rodata_cached_size,                       \
                                    mmu_get_attr(MTYPE_NORMAL, MA_EL2_EL3_RO | MA_NON_EXEC) );
    if (MMU_E_OK != mmuRetVal)
    {
        /** @violates @ref sm_mmap_c_REF_2 MISRA rule 14.7 */
        return -105;
    }
    
    /*
        TEXT section
    */
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
    mmuRetVal = mmu_add_mapping(    (va_t)&__sm_text_cached_start,                          \
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
                                    (pa_t)&__sm_text_cached_start,                          \
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
                                    (mlen_t)&__sm_text_cached_size,                         \
                                    mmu_get_attr(MTYPE_NORMAL, MA_EL2_EL3_RW)   );
    if (MMU_E_OK != mmuRetVal)
    {
        /** @violates @ref sm_mmap_c_REF_2 MISRA rule 14.7 */
        return -106;
    }
    
    /*
        DATA section
    */
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
    mmuRetVal = mmu_add_mapping(    (va_t)&__sm_data_cached_start,                          \
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
                                    (pa_t)&__sm_data_cached_start,                          \
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
                                    (mlen_t)&__sm_data_cached_size,                         \
                                    mmu_get_attr(MTYPE_NORMAL, MA_EL2_EL3_RW | MA_NON_EXEC) );
    if (MMU_E_OK != mmuRetVal)
    {
        /** @violates @ref sm_mmap_c_REF_2 MISRA rule 14.7 */
        return -107;
    }
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
    mmuRetVal = mmu_add_mapping(    (va_t)&__sm_data_non_cached_start,                      \
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
                                    (pa_t)&__sm_data_non_cached_start,                      \
                                    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
                                    (mlen_t)&__sm_data_non_cached_size,                     \
                                    mmu_get_attr(MTYPE_NORMAL_NC, MA_EL2_EL3_RW | MA_NON_EXEC) );
    if (MMU_E_OK != mmuRetVal)
    {
        /** @violates @ref sm_mmap_c_REF_2 MISRA rule 14.7 */
        return -108;
    }
    
    /*  GIC */
    /** @violates @ref sm_mmap_c_REF_4 MISRA rule 1.2 */
    gic_get_memory_base(&u32Base, &u32Length);
    mmuRetVal = mmu_add_mapping(    (va_t)u32Base,                                          \
                                    (pa_t)u32Base,                                          \
                                    (mlen_t)u32Length,                                      \
                                    mmu_get_attr(MTYPE_DEVICE, MA_EL2_EL3_RW | MA_NON_EXEC) );
    if (MMU_E_OK != mmuRetVal)
    {
        /** @violates @ref sm_mmap_c_REF_2 MISRA rule 14.7 */
        return -109;
    }
    
#if (PLATFORM == S32V234)
    /*  Auxiliary mapping for MSCM */
    mmuRetVal = mmu_add_mapping(    (va_t)MSCM_BASE_ADDR,                                   \
                                    (pa_t)MSCM_BASE_ADDR,                                   \
                                    (mlen_t)MSCM_REGION_LENGTH,                             \
                                    mmu_get_attr(MTYPE_DEVICE, MA_EL2_EL3_RW | MA_NON_EXEC) );
    if (MMU_E_OK != mmuRetVal)
    {
        /** @violates @ref sm_mmap_c_REF_2 MISRA rule 14.7 */
        return -110;
    }
#endif /* PLATFORM */

    return 0;
}

/*
    @brief Memory map start function
    @details Enables the MMU-driven memory mapping initialized by the sm_mmap_init()
    @retval Zero Success
    @retval <0 Start has failed
*/
sint32_t sm_mmap_start(void)
{
    sint32_t s32RetVal = (MMU_E_OK == mmu_start()) ? 0 : -1;
    return s32RetVal;
}

/*
    @brief Memory map stop function
    @details Stops memory mapping by MMU
    @retval Zero Success
    @retval <0 Call has failed
*/
sint32_t sm_mmap_stop(void)
{
    sint32_t s32RetVal;
    
    /** @violates @ref sm_mmap_c_REF_3 MISRA rule 11.3 */
    cache_d_clean_by_va_range((addr_t)&__sm_stack_start, (addr_t)&__sm_stack_size);
    s32RetVal = (MMU_E_OK == mmu_stop()) ? 0 : -1;
    
    return s32RetVal;
}
#endif /* SM_CFG_ENABLE_MMU */
/** @}*/
