/**
  @addtogroup       dxgrMMU
  @{

  @file             mmu_cfg.h
  @brief            The MMU module configuration header file
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
#ifndef __MMU_CFG_H
#define __MMU_CFG_H

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
    #error Please include the "mmu_mem_attr.h" first.
#endif /* __MMU_MEM_ATTR_H */

/*==================================================================================================
                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/
/**
    @def MMU_CFG_ENTRY_POOL_SIZE
    @brief Number of entries allocated to be used by translation tables
    @details If this value is too small the API returns the MMU_E_OUT_OF_MEMORY error code. Value
             shall be a multiple of number of entries of the biggest translation table in the system.
    @hideinitializer
*/
#define MMU_CFG_ENTRY_POOL_SIZE 4096U

/*  Available memory types; mapping to MAIR Attr entries; add more entries when needed;
    max 8-bit values are supported */
/**
 *  @def MTYPE_NORMAL
 *  @brief Memory type: Normal
 *  @hideinitializer
 */
#define MTYPE_NORMAL            1U

/**
 *  @def MTYPE_NORMAL_NC
 *  @brief Memory type: Normal, not cached
 *  @hideinitializer
 */
#define MTYPE_NORMAL_NC         2U

/**
 *  @def MTYPE_DEVICE
 *  @brief Memory type: Device
 *  @hideinitializer
 */
#define MTYPE_DEVICE            3U

/** @cond dxINTERNAL_MACROS */
/*  Set-up particular memory type attributes here: */
#define MTYPE_NORMAL_ATTRIBUTES     ( MATTR_NORMAL_OWB_NTRANS       \
                                        | MATTR_NORMAL_IWB_NTRANS   \
                                        | MATTR_I_R | MATTR_I_W     \
                                        | MATTR_O_R | MATTR_O_W )

#define MTYPE_NORMAL_NC_ATTRIBUTES  ( MATTR_NORMAL_I_NOCACHE        \
                                        | MATTR_NORMAL_O_NOCACHE )

#define MTYPE_DEVICE_ATTRIBUTES     ( MATTR_DEVICE_MEMORY           \
                                        | MATTR_DEVICE_nGnRnE )
/** @endcond */

/*==================================================================================================
                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/
/**
    @brief Configure MAIR_EL3 entries
    @details Function shall contain configuration of all supported memory types according
             to "Available memory types" options.
*/
static inline int mmu_cfg_preconfigure(void) {
    unsigned long long regval = 0ULL;

    /*  MTYPE_NORMAL: */
    regval |= MAIR_ATTR(MTYPE_NORMAL, MTYPE_NORMAL_ATTRIBUTES);
    /*  MTYPE_NORMAL_NC: */
    regval |= MAIR_ATTR(MTYPE_NORMAL_NC, MTYPE_NORMAL_NC_ATTRIBUTES);
    /*  MTYPE_DEVICE: */
    regval |= MAIR_ATTR(MTYPE_DEVICE, MTYPE_DEVICE_ATTRIBUTES);
    
    __asm("msr MAIR_EL3, %0" : : "r"(regval));
    
    return 0;   /* Non-zero value here reports error to the MMU core */
}

#endif /* __MMU_CFG_H */
/** @}*/
