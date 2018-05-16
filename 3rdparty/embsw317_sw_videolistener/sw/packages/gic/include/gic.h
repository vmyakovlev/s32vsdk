/**
  @addtogroup       dxgrGIC
  @{

  @file             gic.h
  @brief            GIC driver header file
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
#ifndef __GIC_H
#define __GIC_H

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
/** @endcond */

/*==================================================================================================
                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
typedef enum {
    GICD_LEVEL_SENSITIVE = 0,
    GICD_EDGE_TRIGGERED = 2
} gicd_sensitivity_t;

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/
void gic_get_memory_base(uint32_t *pu32Base, uint32_t *pu32Length);
void gicd_disable_all(void);
void gicd_enable_all(void);
void gicd_disable_grp0(void);
void gicd_disable_grp1(void);
void gicd_enable_grp0(void);
void gicd_enable_grp1(void);
void gicd_all_to_grp1(void);
void gicd_set_target(const uint32_t u32Irq, const uint32_t u32Target);
void gicd_set_priority(const uint32_t u32Irq, const uint32_t u32Priority);
void gicd_set_sensitivity(const uint32_t u32Irq, const gicd_sensitivity_t eConfig);
void gicd_set_group0(const uint32_t u32Irq);
void gicd_set_group1(const uint32_t u32Irq);
void gicd_enable(const uint32_t u32Irq);
void gicd_disable(const uint32_t u32Irq);
void gicc_enable_fiq(void);
void gicc_disable_fiq(void);
void gicc_disable_all(void);
void gicc_enable_all(void);
void gicd_send_sgi_to_this_core(const uint32_t u32Irq, const uint8_t u8NSATT);

#endif /* __GIC_H */
/** @}*/
