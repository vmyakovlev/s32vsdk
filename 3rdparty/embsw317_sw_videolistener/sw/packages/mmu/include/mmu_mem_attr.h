/**
  @addtogroup       dxgrMMU
  @{

  @file             mmu_mem_attr.h
  @brief            Memory attributes header file
  @details          File contains definitions of supported memory attributes and related internal
                    constants

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

#ifndef __MMU_MEM_ATTR_H
#define __MMU_MEM_ATTR_H
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

/*==================================================================================================
                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/
/** @cond dxINTERNAL_MACROS */
/*  Memory attributes */
#define MATTR_DEVICE_MEMORY             0x00U   /* 0b00000000U */ /*  Device memory. Set device memory type by Attr[3:0]. */
#define MATTR_NORMAL_OWT_TRANS          0x00U   /* 0b00000000U */ /*  Normal memory, Outer Write-through transient, R ([5]) or W ([4]) must be set */
#define MATTR_NORMAL_O_NOCACHE          0x40U   /* 0b01000000U */ /*  Normal memory, Outer Non-Cacheable */
#define MATTR_NORMAL_OWB_TRANS          0x40U   /* 0b01000000U */ /*  Normal memory, Outer Write-back transient, R ([5]) or W ([4]) must be set */
#define MATTR_NORMAL_OWT_NTRANS         0x80U   /* 0b10000000U */ /*  Normal memory, Outer Write-through non-transient, R/W can be set */
#define MATTR_NORMAL_OWB_NTRANS         0xc0U   /* 0b11000000U */ /*  Normal memory, Outer Write-back non-transient, R/W can be set */
#define MATTR_O_R                       0x10U   /* 0b00010000U */ /*  Outer Read Allocate Policy */
#define MATTR_O_W                       0x20U   /* 0b00100000U */ /*  Outer Write Allocate Policy */

/*  Normal memory types */
#define MATTR_NORMAL_IWT_TRANS          0x00U   /* 0b00000000U */ /*  Normal memory, Inner Write-through transient, R ([1]) or W ([0]) must be set */
#define MATTR_NORMAL_I_NOCACHE          0x04U   /* 0b00000100U */ /*  Normal memory, Inner Non-Cacheable */
#define MATTR_NORMAL_IWB_TRANS          0x04U   /* 0b00000100U */ /*  Normal memory, Inner Write-back transient, R ([1]) or W ([0]) must be set */
#define MATTR_NORMAL_IWT_NTRANS         0x08U   /* 0b00001000U */ /*  Normal memory, Inner Write-through non-transient, R/W can be set */
#define MATTR_NORMAL_IWB_NTRANS         0x0cU   /* 0b00001100U */ /*  Normal memory, Inner Write-back non-transient, R/W can be set */
#define MATTR_I_R                       0x02U   /* 0b00000010U */ /*  Inner Read Allocate Policy */
#define MATTR_I_W                       0x01U   /* 0b00000001U */ /*  Inner Write Allocate Policy */

/*  Device memory types */
#define MATTR_DEVICE_nGnRnE             0x00U   /* 0b00000000U */ /*  Device non-Gathering, non-Reordering, No Early write acknowledgement */
#define MATTR_DEVICE_nGnRE              0x04U   /* 0b00000100U */ /*  Device non-Gathering, non-Reordering, Early Write Acknowledgement */
#define MATTR_DEVICE_nGRE               0x08U   /* 0b00001000U */ /*  Device non-Gathering, Reordering, Early Write Acknowledgement */
#define MATTR_DEVICE_GRE                0x0cU   /* 0b00001100U */ /*  Device Gathering, Reordering, Early Write Acknowledgement */

/*  Upper/Lower attributes */
#define MATTR_UPA_UXN_XN                (0x1UL << 2)    /*  The Execute-never bit */
#define MATTR_UPA_PXN                   (0x1UL << 1)    /*  The Privileged execute-never bit */
#define MATTR_UPA_CONTIGUOUS            (0x1UL << 0)    /*  Contiguous entry indicator */
#define MATTR_LOA_nG                    (0x1UL << 9)    /*  The not global bit */
#define MATTR_LOA_AF                    (0x1UL << 8)    /*  Access flag */
#define MATTR_LOA_SH_NONSHAREABLE       (0x0UL << 6)    /*  Shareability field */
#define MATTR_LOA_SH_OUTERSHAREABLE     (0x2UL << 6)    /*  Shareability field */
#define MATTR_LOA_SH_INNERSHAREABLE     (0x3UL << 6)    /*  Shareability field */
#define MATTR_LOA_AP_EL0_NONE_EL1_RW    (0x0UL << 4)    /*  Data access permissions bits A[2:1] (Stage 1) */
#define MATTR_LOA_AP_EL0_RW_EL1_RW      (0x1UL << 4)
#define MATTR_LOA_AP_EL0_NONE_EL1_RO    (0x2UL << 4)
#define MATTR_LOA_AP_EL0_RO_EL1_RO      (0x3UL << 4)
#define MATTR_LOA_AP_EL2EL3_RW          (0x0UL << 4)
#define MATTR_LOA_AP_EL2EL3_RO          (0x2UL << 4)
#define MATTR_LOA_NS                    (0x1U << 3)     /*  Non-secure bit */
#define MATTR_LOA_ATTRIDX(x)            ((x))           /*  Stage 1 memory attributes index field */

/*  Long descriptor attributes access macros */
/*  Table Attributes */
#define MATTR_TA(x)     (((uint64_t)(x) & 0x1fULL) << 59)
/*  Block Upper Attributes */
#define MATTR_BUA(x)    (((uint64_t)(x) & 0x7ULL) << 52)
/*  Page Upper Attributes */
#define MATTR_PUA(x)    MATTR_BUA(x)
/*  Block Lower Attributes */
#define MATTR_BLA(x)    (((uint64_t)(x) & 0x3ffULL) << 2)
/*  Page Lower Attributes */
#define MATTR_PLA(x)    MATTR_BLA(x)

/*  MAIR manipulation macro */
#define MAIR_ATTR(index, val)           (((uint64_t)(val) & 0xffULL) << (index * 8))
/** @endcond */

/*  Supported memory attributes list */

/**
 * @def MA_EL2_EL3_RW
 * @brief Memory attribute for EL2/EL3 read/write access
 * @hideinitializer
 */
#define MA_EL2_EL3_RW   ((1UL << 0))   /*  Region is EL2/EL3 RW */

/**
 * @def MA_EL2_EL3_RO
 * @brief Memory attribute for EL2/EL3 read-only access
 * @hideinitializer
 */
#define MA_EL2_EL3_RO   ((1UL << 1))   /*  Region is EL2/EL3 RO */

/**
 * @def MA_NON_SECURE
 * @brief Memory attribute to create non-secure mapping
 * @hideinitializer
 */
#define MA_NON_SECURE   ((1UL << 2))   /*  Region is non-secure */

/**
 * @def MA_NON_EXEC
 * @brief Memory attribute to create non-executable mapping
 * @hideinitializer
 */
#define MA_NON_EXEC     ((1UL << 3))   /*  Region is non-executable */

/*==================================================================================================
                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/*  Type for memory attributes */
typedef unsigned int mem_attr_t;

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/

#endif /* __MMU_MEM_ATTR_H */
/** @}*/
