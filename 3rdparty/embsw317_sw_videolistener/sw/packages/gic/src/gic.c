/**
  @defgroup         dxgrGIC GIC Driver
  @details          This module contains GICv2 abstraction to provide way to manage interrupts on
                    HW level within both Interrupt Distributor and CPU Interface components of the
                    GIC peripheral.

  @remark           Software expects that API is called on appropriate exception
                    level with regards to the desired security level.

  @addtogroup       dxgrGIC
  @{

  @file             gic.c
  @brief            The GIC driver
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
* @section gic_c_REF_1
* Violates MISRA 2004 Advisory Rule 19.7, Function should be defined rather than macro.
* This macro is supposed to be used only with narrow set of input parameters or does not need type
* check. Each usage has been reviewed.
*
* @section gic_c_REF_2
* Violates MISRA 2004 Advisory Rule 11.3, A cast should not be preformed between a pointer type and
* an integral type. Cast is used to access memory mapped resources.
*/

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "common_types.h"
#include "common.h"
#include "gic.h"

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/
/** @cond dxINTERNAL_MACROS */
#define GIC_BASE_ADDR           0x7d000000U
#define GIC_REGION_LENGTH       0x100000U
#define GICD_BASE_ADDR          0x7d001000U
#define GICD_CTLR               (GICD_BASE_ADDR + 0x0000U)
#define GICD_TYPER              (GICD_BASE_ADDR + 0x0004U)
/** @violates @ref gic_c_REF_1 MISRA rule 19.7 */
#define GICD_IGROUPR(n)         (GICD_BASE_ADDR + (0x0080U + ((n) * 4U)))
/** @violates @ref gic_c_REF_1 MISRA rule 19.7 */
#define GICD_IPRIORITYR(n)      (GICD_BASE_ADDR + (0x0400U + ((n) * 4U)))
/** @violates @ref gic_c_REF_1 MISRA rule 19.7 */
#define GICD_ITARGETSR(n)       (GICD_BASE_ADDR + (0x0800U + ((n) * 4U)))
/** @violates @ref gic_c_REF_1 MISRA rule 19.7 */
#define GICD_ICFGR(n)           (GICD_BASE_ADDR + (0x0c00U + ((n) * 4U)))
/** @violates @ref gic_c_REF_1 MISRA rule 19.7 */
#define GICD_ISENABLER(n)       (GICD_BASE_ADDR + (0x0100U + ((n) * 4U)))
/** @violates @ref gic_c_REF_1 MISRA rule 19.7 */
#define GICD_ICENABLER(n)       (GICD_BASE_ADDR + (0x0180U + ((n) * 4U)))
#define GICD_SGIR               (GICD_BASE_ADDR + 0x0f00U)

#define GICC_BASE_ADDR          0x7d002000U
#define GICC_CTLR               (GICC_BASE_ADDR + 0x0000U)
#define GICC_IAR                (GICC_BASE_ADDR + 0x000cU)
/** @endcond */

/*==================================================================================================
                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

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

/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/
/**
    @brief Get memory region occupied by the GIC peripheral
    @param[out] pu32Base Pointer to memory where base address will be stored
    @param[out] pu32Length  Pointer to memory where length of the region will be stored
*/
void gic_get_memory_base(uint32_t *pu32Base, uint32_t *pu32Length)
{
    *pu32Base = GIC_BASE_ADDR;
    *pu32Length = GIC_REGION_LENGTH;
}

/**
    @brief Disable forwarding of interrupts to CPU interfaces for both groups
*/
void gicd_disable_all(void)
{
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    WREG(GICD_CTLR, RREG(GICD_CTLR) & ~(uint32_t)(0x3U));
}

/**
    @brief Enable forwarding of interrupts to CPU interfaces for both groups
*/
void gicd_enable_all(void)
{
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    WREG(GICD_CTLR, RREG(GICD_CTLR) | 0x3U);
}

/**
    @brief Disable forwarding of interrupts to CPU interfaces for Group0
*/
void gicd_disable_grp0(void)
{
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    WREG(GICD_CTLR, RREG(GICD_CTLR) & ~(uint32_t)(0x1U));
}

/**
    @brief Disable forwarding of interrupts to CPU interfaces for Group1
*/
void gicd_disable_grp1(void)
{
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    WREG(GICD_CTLR, RREG(GICD_CTLR) & ~(uint32_t)(0x2U));
}

/**
    @brief Enable forwarding of interrupts to CPU interfaces for Group0
*/
void gicd_enable_grp0(void)
{
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    WREG(GICD_CTLR, RREG(GICD_CTLR) | 0x1U);
}

/**
    @brief Enable forwarding of interrupts to CPU interfaces for Group1
*/
void gicd_enable_grp1(void)
{
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    WREG(GICD_CTLR, RREG(GICD_CTLR) | 0x2U);
}

/**
    @brief Move all interrupts to Group1
*/
void gicd_all_to_grp1(void)
{
    uint32_t u32ii;
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    uint32_t u32RegVal = RREG(GICD_TYPER);
    
    /*  Migrate interrupts to Group1 */
    for (u32ii=0U; u32ii<((u32RegVal & 0x1fU) + 1U); u32ii++)
    {
        /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
        WREG(GICD_IGROUPR(u32ii), 0xffffffffU);
    }
}

/**
    @brief Enable FIQ signalling
*/
void gicc_enable_fiq(void)
{
    /*  Set the AckCtl and FIQEn */
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    WREG(GICC_CTLR, RREG(GICC_CTLR) | 0xcU);
}

/**
    @brief Disable FIQ signalling
*/
void gicc_disable_fiq(void)
{
    /*  Clear the FIQEn but set the AckCtl */
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    uint32_t u32RegVal = RREG(GICC_CTLR) & ~(uint32_t)(0x8U);
    
    u32RegVal |= 0x4U;
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    WREG(GICC_CTLR, u32RegVal | 0x4U);
}

/**
    @brief Set interrupt priority
    @param[in] u32Irq The IRQ number
    @param[in] u32Priority Priority value to be set
*/
void gicd_set_priority(const uint32_t u32Irq, const uint32_t u32Priority)
{
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    uint32_t u32RegVal = RREG(GICD_IPRIORITYR(u32Irq / 4U));
    
    u32RegVal &= ~(uint32_t)(0xffUL << ((u32Irq % 4U) * 8U));
    u32RegVal |= ((u32Priority & 0xffU) << ((u32Irq % 4U) * 8U));
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    WREG(GICD_IPRIORITYR(u32Irq / 4U), u32RegVal);
}

/**
    @brief Set interrupt target
    @param[in] u32Irq The IRQ number
    @param[in] u32Target Target mask to be set
*/
void gicd_set_target(const uint32_t u32Irq, const uint32_t u32Target)
{
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    uint32_t u32RegVal = RREG(GICD_ITARGETSR(u32Irq / 4U));
    
    u32RegVal &= ~(uint32_t)(0xffUL << ((u32Irq % 4U) * 8U));
    u32RegVal |= ((u32Target & 0xffU) << ((u32Irq % 4U) * 8U));
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    WREG(GICD_ITARGETSR(u32Irq / 4U), u32RegVal);
}

/**
    @brief Set interrupt sensitivity
    @param[in] u32Irq The interrupt number
    @param[in] eConfig Sensitivity configuration to be set
*/
void gicd_set_sensitivity(const uint32_t u32Irq, const gicd_sensitivity_t eConfig)
{
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    uint32_t u32RegVal = RREG(GICD_ICFGR(u32Irq / 16U));
    
    u32RegVal &= ~(uint32_t)(0x3UL << ((u32Irq % 16U) * 2U));
    u32RegVal |= (((uint32_t)(eConfig) & 0x3U) << ((u32Irq % 16U) * 2U));
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    WREG(GICD_ICFGR(u32Irq / 16U), u32RegVal);
}

/**
    @brief Enable IRQ within the distributor
    @param[in] u32Irq The interrupt number
*/
void gicd_enable(const uint32_t u32Irq)
{
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    uint32_t u32RegVal = RREG(GICD_ISENABLER(u32Irq / 32U)) | (uint32_t)(0x1UL << (u32Irq % 32U));
    
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    WREG(GICD_ISENABLER(u32Irq / 32U), u32RegVal);
}

/**
    @brief Disable IRQ within the distributor
    @param[in] u32Irq The interrupt number
*/
void gicd_disable(const uint32_t u32Irq)
{
    uint32_t u32RegVal = (uint32_t)(0x1UL << (u32Irq % 32U));
    
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    WREG(GICD_ICENABLER(u32Irq / 32U), u32RegVal);
}

/**
    @brief Assign an interrupt to Group0
    @param[in] u32Irq The interrupt number
*/
void gicd_set_group0(const uint32_t u32Irq)
{
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    uint32_t u32RegVal = RREG(GICD_IGROUPR(u32Irq / 32U)) & ~(uint32_t)(0x1UL << (u32Irq % 32U));
    
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    WREG(GICD_IGROUPR(u32Irq / 32U), u32RegVal);
}

/**
    @brief Assign an interrupt to Group1
    @param[in] u32Irq The interrupt number
*/
void gicd_set_group1(const uint32_t u32Irq)
{
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    uint32_t u32RegVal = RREG(GICD_IGROUPR(u32Irq / 32U)) | (uint32_t)(0x1UL << (u32Irq % 32U));
    
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    WREG(GICD_IGROUPR(u32Irq / 32U), u32RegVal);
}

/**
    @brief Send SGI to the current core
    @param[in] u32Irq The interrupt number (SGIINTID)
    @param[in] u8NSATT Security value of the SGI
*/
void gicd_send_sgi_to_this_core(const uint32_t u32Irq, const uint8_t u8NSATT)
{
    uint32_t u32Cmd;
    
    u32Cmd = (0x2UL << 24);
    u32Cmd |= (uint32_t)((u8NSATT & 0x1UL) << 15);
    u32Cmd |= (uint32_t)(u32Irq & 0xfUL);
    
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    WREG(GICD_SGIR, u32Cmd);
}

/**
    @brief Disable signalling of Group0 and Group1 interrupts to the processor by the CPU interface
*/
void gicc_disable_all(void)
{
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    WREG(GICC_CTLR, RREG(GICC_CTLR) & ~(uint32_t)(0x3U));
}

/**
    @brief Enable signalling of Group0 and Group1 interrupts to the processor by the CPU interface
*/
void gicc_enable_all(void)
{
    /** @violates @ref gic_c_REF_2 MISRA rule 11.3 */
    WREG(GICC_CTLR, RREG(GICC_CTLR) | 0x3U);
}
/** @}*/
