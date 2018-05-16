/**
  @addtogroup       dxgrPIT
  @{

  @file             pit.h

  @brief            PIT driver header file
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
#ifndef __PIT_H
#define __PIT_H

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
#define PIT_CFG_INSTANCES_COUNT             2U
#define PIT_CFG_CHANNELS_COUNT(instance)    ((0U == instance) ? 5U : 3U)
/** @endcond */

/*==================================================================================================
                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
typedef enum
{
    PIT_E_OK = 0,
    PIT_E_CONFIGURATION = -1,
    PIT_E_PARAMETER = -2,
    PIT_E_TIMEOUT = -3,
} pit_ret_t;

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/
void pit_get_memory_base(const uint8_t u8Instance, uint32_t *pu32Base, uint32_t *pu32Length);
void mc_me_get_memory_base(uint32_t *pu32Base, uint32_t *pu32Length);
void mc_cgm_get_memory_base(uint8_t u8Instance, uint32_t *pu32Base, uint32_t *pu32Length);
pit_ret_t pit_init(const uint8_t u8Instance);
pit_ret_t pit_start(const uint8_t u8Instance, const uint8_t u8Channel);
pit_ret_t pit_stop(const uint8_t u8Instance, const uint8_t u8Channel);
pit_ret_t pit_confirm_irq(const uint8_t u8Instance, const uint8_t u8Channel);
pit_ret_t pit_set_period(const uint8_t u8Instance, const uint8_t u8Channel, const uint32_t u32PeriodNs);
pit_ret_t pit_enable_irq(const uint8_t u8Instance, const uint8_t u8Channel);
pit_ret_t pit_disable_irq(const uint8_t u8Instance, const uint8_t u8Channel);
pit_ret_t pit_is_timeout(const uint8_t u8Instance, const uint8_t u8Channel);
pit_ret_t pit_get_elapsed_ns(const uint8_t u8Instance, const uint8_t u8Channel, uint32_t * const pu32Result);

#endif /* __PIT_H */
/** @}*/
