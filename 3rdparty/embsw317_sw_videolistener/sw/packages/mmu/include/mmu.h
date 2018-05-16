/**
  @addtogroup       dxgrMMU
  @{

  @file             mmu.h
  @brief            The MMU driver header file
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
#ifndef __MMU_H
#define __MMU_H

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/

/*==================================================================================================
                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
#ifndef __MMU_MEM_ATTR_H
    #error Please include "mmu_mem_attr.h" first.
#endif /* __MMU_MEM_ATTR_H */

#ifndef __MMU_CFG_H
    #error Please include "mmu_cfg.h" first.
#endif /* __MMU_CFG_H */

/*==================================================================================================
                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/
/** @def mmu_get_attr(type, attr)
 *  @brief Function to prepare attribute mask combining memory type and memory attributes values
 *  @param[in] type The type of memory as defined within the mmu_cfg.h
 *  @param[in] attr Memory attributes as defined by mmu_mem_attr.h
 *  @return The attribute mask in form suitable for the mmu_add_mapping() API
 *  @hideinitializer
 */
#define mmu_get_attr(type, attr) ((mem_attr_t)( (uint32_t)(((uint32_t)(type) & 0xffU) << 24) | (uint32_t)((uint32_t)(attr) & 0x00ffffffU)))
/** @cond dxINTERNAL_MACROS */
#define mmu_attr_get_type(attr) ((mem_attr_t)((attr & 0xff000000U) >> 24))
#define mmu_attr_get_attr(attr) ((mem_attr_t)(attr & 0x00ffffffU))
/** @endcond */

/*==================================================================================================
                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/*  Generic address type */
typedef unsigned long long addr_t;

/*  Virtual address type */
typedef unsigned long long va_t;

/*  Physical address type */
typedef unsigned long pa_t;

/*  Memory length type */
typedef addr_t mlen_t;

/*  Boolean TODO: use general type */
typedef unsigned int boolean_t;

/*  Error codes */
typedef enum {
    MMU_E_OK                    = 0,
    MMU_E_SIZE_MISMATCH         = -1,
    MMU_E_ST_FAILED             = -2,
    MMU_E_CONFLICT              = -3,
    MMU_E_TABLE_ERROR           = -4,
    MMU_E_INVALID_DESCRIPTOR    = -5,
    MMU_E_OUT_OF_MEMORY         = -6,
    MMU_E_CONFIGURATION         = -7,
    MMU_E_NOT_MAPPED            = -8,
    MMU_E_SYSTEM                = -9,
    MMU_E_INVALID_ADDRESS       = -10,
    MMU_E_INVALID_PARAMETER     = -11,
    MMU_E_NOT_INITIALIZED       = -12
} mmu_ret_t;

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/
mmu_ret_t mmu_init(void);
mmu_ret_t mmu_add_mapping(const va_t VA, const pa_t PA, const mlen_t Size, const mem_attr_t Attr);
mmu_ret_t mmu_check_mapping(const va_t VA, const pa_t PA, const mlen_t Size);
mmu_ret_t mmu_start(void);
mmu_ret_t mmu_stop(void);
mmu_ret_t mmu_get_region_size(const uint32_t u32Level, mlen_t *const RegionSize);
void cache_d_clean_by_va_range(va_t VA, mlen_t length);

#endif /* __MMU_H */
/** @}*/
