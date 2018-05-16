/**
  @defgroup         dxgrPIT PIT Driver
  @details          This is driver for the Periodic Interrupt Timer peripheral enabling various
                    PIT channels control and configurations. Package also contains API for getting
                    elapsed time (useful for measurement of short time intervals) and is able to
                    determine the PIT frequency considering current platform, PLL setup and CPU
                    mode so the user does not need to care about that.

  @addtogroup       dxgrPIT
  @{

  @file             pit.c
  @brief            The PIT driver
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
* @section pit_c_REF_1
* Violates MISRA 2004 Advisory Rule 19.7, Function should be defined rather than macro.
* This macro is supposed to be used only with narrow set of input parameters or does not need type
* check. Each usage has been reviewed.
*
* @section pit_c_REF_2
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
#include "pit.h"

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/
/** @cond dxINTERNAL_MACROS */
#define PIT0_BASE_ADDR              0x4003A000U
#define PIT0_REGION_LENGTH          0x1000U
#define PIT1_BASE_ADDR              0x400AA000U
#define PIT1_REGION_LENGTH          0x1000U
/** @violates @ref pit_c_REF_1 MISRA rule 19.7 */
#define PIT_MCR(base)               ((base) + 0x0000U)
/** @violates @ref pit_c_REF_1 MISRA rule 19.7 */
#define PIT_LDVAL(base, n)          ((base) + ((uint32_t)(n) * 0x10U) + 0x0100U)
/** @violates @ref pit_c_REF_1 MISRA rule 19.7 */
#define PIT_CVAL(base, n)           ((base) + ((uint32_t)(n) * 0x10U) + 0x0104U)
/** @violates @ref pit_c_REF_1 MISRA rule 19.7 */
#define PIT_TCTRL(base, n)          ((base) + ((uint32_t)(n) * 0x10U) + 0x0108U)
/** @violates @ref pit_c_REF_1 MISRA rule 19.7 */
#define PIT_TFLG(base, n)           ((base) + ((uint32_t)(n) * 0x10U) + 0x010cU)

#define ARM_PLL_VCO                 1000000000U
#define ARM_PLL_DFS_BASEADDR        (0x4003C000U + 0x0040U)
/** @violates @ref pit_c_REF_1 MISRA rule 19.7 */
#define DFS_DVPORT(n)               (ARM_PLL_DFS_BASEADDR + ((uint32_t)(n) * 4U) + 0x1cU)

#define MC_CGM_0_BASEADDR           0x4003C000U
#define MC_CGM_0_REGION_LENGTH      0x3000U
#define MC_CGM_0_SC_DC1             (MC_CGM_0_BASEADDR + 0x07ecU)

#define MC_ME_BASEADDR              0x4004A000U
#define MC_ME_RANGE_LENGTH          0x1000U
#define MC_ME_GS                    (MC_ME_BASEADDR + 0x0000U)
/** @violates @ref pit_c_REF_1 MISRA rule 19.7 */
#define MC_ME_x_MC(x)               (MC_ME_BASEADDR + ((x) * 4U) + 0x20U)

/*  PIT0 Peripheral Control Register */
#define MC_ME_PCTL58                (MC_ME_BASEADDR + 0xf9U)
/*  PIT1 Peripheral Control Register */
#define MC_ME_PCTL170               (MC_ME_BASEADDR + 0x169U)
/*  Peripheral Configuration Select */
#define MC_ME_PCTL_RUN_PCx(x)       ((x) & 0x7U)

/** @endcond */

/*==================================================================================================
                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
                                       LOCAL CONSTANTS
==================================================================================================*/
static const uint32_t au32PitBase[2] = {PIT0_BASE_ADDR, PIT1_BASE_ADDR};

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
static uint32_t get_pit_input_clock_hz(void);
inline static pit_ret_t pit_check_config(const uint8_t u8Instance, const uint8_t u8Channel);

/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/
/*
    @brief Internal configuration validating function
    @param[in] u8Instance The PIT instance to be checked
    @param[in] u8Channel The channel to be checked
    @retval PIT_E_OK Configuration is OK
    @retval PIT_E_PARAMETER Some configuration parameter(s) is/are invalid
*/
static pit_ret_t pit_check_config(const uint8_t u8Instance, const uint8_t u8Channel)
{
    pit_ret_t eRetVal = PIT_E_OK;
    
    if (u8Instance > (PIT_CFG_INSTANCES_COUNT - 1U))
    {
        eRetVal = PIT_E_PARAMETER;
    }
    else if (u8Channel > PIT_CFG_CHANNELS_COUNT(u8Instance))
    {
        eRetVal = PIT_E_PARAMETER;
    }
    else
    {
        ;   /*  Intentionally empty */
    }
    
    return eRetVal;
}

/*
    @brief Internal function used to determine PIT input frequency based on
           platform, PLL and CPU mode
    @retval Frequency in Hz as seen by PIT
*/
static uint32_t get_pit_input_clock_hz()
{
    /*  Specific for Treerunner (returns SYS6_CLK if ARM_PLL == ARM_PLL_VCO [Hz]) */
    uint32_t u32RegVal;
    uint64_t u64LongVal, u64OutF;

    /*  Verify that current CPU mode uses ARM_PLL as system clock */
    /** @violates @ref pit_c_REF_2 MISRA rule 11.3 */
    u32RegVal = (RREG(MC_ME_GS) & 0xf0000000U) >> 28;
    /** @violates @ref pit_c_REF_2 MISRA rule 11.3 */
    u32RegVal = RREG(MC_ME_x_MC(u32RegVal));
    
    if ((u32RegVal & 0xfU) != 0x2U)
    {
        u64OutF = 0U;  /*  Unexpected SYSCLK value */
    }
    else
    {
        /*  Get PIT input freq */
        /** @violates @ref pit_c_REF_2 MISRA rule 11.3 */
        u32RegVal = RREG(DFS_DVPORT(0U));
        u64LongVal = (((uint64_t)u32RegVal & 0xffULL) * 1000000ULL) / 256ULL;
        u64LongVal += (((uint64_t)u32RegVal & 0xff00ULL) >> 8) * 1000000ULL;
        u64OutF = (ARM_PLL_VCO * 1000000ULL) / u64LongVal;
        
        /** @violates @ref pit_c_REF_2 MISRA rule 11.3 */
        u32RegVal = RREG(MC_CGM_0_SC_DC1);
        if (u32RegVal & 0x80000000U)
        {
            u64OutF = u64OutF / (1ULL + ((uint64_t)(u32RegVal & 0x30000ULL) >> 16));
        }
    }

    return (uint32_t)(u64OutF & 0xffffffffU);
}

/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/
/**
    @brief Get memory region occupied by the MC_ME peripheral
    @param[out] pu32Base Pointer to memory where base address will be stored
    @param[out] pu32Length  Pointer to memory where length of the region will be stored
*/
void mc_me_get_memory_base(uint32_t *pu32Base, uint32_t *pu32Length)
{
    *pu32Base = MC_ME_BASEADDR;
    *pu32Length = MC_ME_RANGE_LENGTH;
}

/**
    @brief Get memory region occupied by the MC_CGM peripheral
    @param[in] u8Instance The desired MC_CGM instance
    @param[out] pu32Base Pointer to memory where base address will be stored
    @param[out] pu32Length  Pointer to memory where length of the region will be stored
*/
void mc_cgm_get_memory_base(uint8_t u8Instance, uint32_t *pu32Base, uint32_t *pu32Length)
{
    (void)u8Instance;
    
    *pu32Base = MC_CGM_0_BASEADDR;
    *pu32Length = MC_CGM_0_REGION_LENGTH;
}

/**
    @brief Get memory region occupied by the PIT peripheral
    @param[in] u8Instance The desired PIT instance
    @param[out] pu32Base Pointer to memory where base address will be stored
    @param[out] pu32Length  Pointer to memory where length of the region will be stored
*/
void pit_get_memory_base(const uint8_t u8Instance, uint32_t *pu32Base, uint32_t *pu32Length)
{
    *pu32Base = (u8Instance == 0U) ? PIT0_BASE_ADDR : PIT1_BASE_ADDR;
    *pu32Length = (u8Instance == 0U) ? PIT0_REGION_LENGTH : PIT1_REGION_LENGTH;
}

/**
    @brief Initialize PIT peripheral instance
    @param[in] u8Instance The desired PIT instance
    @retval PIT_E_OK Success
    @retval PIT_E_PARAMETER Invalid parameter
*/
pit_ret_t pit_init(const uint8_t u8Instance)
{
    pit_ret_t eRetVal = PIT_E_OK;
    
    /*  Set peripheral control for Run mode */
    switch (u8Instance)
    {
        case 0U:
            WREG_8(MC_ME_PCTL58, MC_ME_PCTL_RUN_PCx(1U));
            break;
        case 1U:
            WREG_8(MC_ME_PCTL170, MC_ME_PCTL_RUN_PCx(1U));
            break;
        default:
            eRetVal = PIT_E_PARAMETER;
            break;
    }
    
    return eRetVal;
}

/**
    @brief Start the PIT
    @param[in] u8Instance The desired PIT instance
    @param[in] u8Channel Channel within the instance to be started
    @retval PIT_E_OK Success
    @retval PIT_E_PARAMETER Invalid configuration
*/
pit_ret_t pit_start(const uint8_t u8Instance, const uint8_t u8Channel)
{
    pit_ret_t eRetVal = PIT_E_OK;

    eRetVal = pit_check_config(u8Instance, u8Channel);
    
    if (PIT_E_OK == eRetVal)
    {
        /*  Clear the timeout flag */
        /** @violates @ref pit_c_REF_2 MISRA rule 11.3 */
        WREG(PIT_TFLG(au32PitBase[u8Instance], u8Channel), 0x1U);
        
        /** @violates @ref pit_c_REF_2 MISRA rule 11.3 */
        WREG(PIT_MCR(au32PitBase[u8Instance]), 0x0U);
        
        /** @violates @ref pit_c_REF_2 MISRA rule 11.3 */
        WREG(PIT_TCTRL(au32PitBase[u8Instance], u8Channel), 0x1U);
    }

    return eRetVal;
}

/**
    @brief Enable interrupt request generation
    @param[in] u8Instance The desired PIT instance
    @param[in] u8Channel Channel to be enabled to generate IRQ
    @retval PIT_E_OK Success
    @retval PIT_E_PARAMETER Invalid configuration
*/
pit_ret_t pit_enable_irq(const uint8_t u8Instance, const uint8_t u8Channel)
{
    pit_ret_t eRetVal = PIT_E_OK;
    
    eRetVal = pit_check_config(u8Instance, u8Channel);
    
    if (PIT_E_OK == eRetVal)
    {
        /** @violates @ref pit_c_REF_2 MISRA rule 11.3 */
        WREG(PIT_TCTRL(au32PitBase[u8Instance], u8Channel), (RREG(PIT_TCTRL(au32PitBase[u8Instance], u8Channel)) | 0x2U));
    }
    
    return eRetVal;
}

/**
    @brief Disable interrupt request generation
    @param[in] u8Instance The desired PIT instance
    @param[in] u8Channel Channel to be disabled to generate IRQ
    @retval PIT_E_OK Success
    @retval PIT_E_PARAMETER Invalid configuration
*/
pit_ret_t pit_disable_irq(const uint8_t u8Instance, const uint8_t u8Channel)
{
    pit_ret_t eRetVal = PIT_E_OK;
    
    eRetVal = pit_check_config(u8Instance, u8Channel);
    
    if (PIT_E_OK == eRetVal)
    {
        /** @violates @ref pit_c_REF_2 MISRA rule 11.3 */
        WREG(PIT_TCTRL(au32PitBase[u8Instance], u8Channel), (RREG(PIT_TCTRL(au32PitBase[u8Instance], u8Channel)) & ~(uint32_t)0x2U));
    }
    
    return eRetVal;
}

/**
    @brief Stop PIT
    @param[in] u8Instance The desired PIT instance
    @param[in] u8Channel Channel to be stopped
    @retval PIT_E_OK Success
    @retval PIT_E_PARAMETER Invalid configuration
*/
pit_ret_t pit_stop(const uint8_t u8Instance, const uint8_t u8Channel)
{
    pit_ret_t eRetVal = PIT_E_OK;
    
    eRetVal = pit_check_config(u8Instance, u8Channel);
    
    if (PIT_E_OK == eRetVal)
    {
        /** @violates @ref pit_c_REF_2 MISRA rule 11.3 */
        WREG(PIT_TCTRL(au32PitBase[u8Instance], u8Channel), 0x0U);
    }
    
    return eRetVal;
}

/**
    @brief Confirm interrupt
    @details Function is intended to be called each time the PIT interrupt has occurred to
             clear the interrupt flag.
    @param[in] u8Instance The desired PIT instance
    @param[in] u8Channel Channel to confirm interrupt for
    @retval PIT_E_OK Success
    @retval PIT_E_PARAMETER Invalid configuration
*/
pit_ret_t pit_confirm_irq(const uint8_t u8Instance, const uint8_t u8Channel)
{
    pit_ret_t eRetVal = PIT_E_OK;
    
    eRetVal = pit_check_config(u8Instance, u8Channel);
    
    if (PIT_E_OK == eRetVal)
    {
        /** @violates @ref pit_c_REF_2 MISRA rule 11.3 */
        WREG(PIT_TFLG(au32PitBase[u8Instance], u8Channel), 0x1U);
    }
    
    return eRetVal;
}

/**
    @brief Check if timeout has occurred
    @param[in] u8Instance The desired PIT instance
    @param[in] u8Channel Channel to be checked
    @retval PIT_E_OK Success, no timeout
    @retval PIT_E_TIMEOUT Timeout has occurred
    @retval PIT_E_PARAMETER Invalid configuration
*/
pit_ret_t pit_is_timeout(const uint8_t u8Instance, const uint8_t u8Channel)
{
    pit_ret_t eRetVal = PIT_E_OK;
    
    eRetVal = pit_check_config(u8Instance, u8Channel);
    
    if (PIT_E_OK == eRetVal)
    {
        /** @violates @ref pit_c_REF_2 MISRA rule 11.3 */
        uint32_t u32Flag = RREG(PIT_TFLG(au32PitBase[u8Instance], u8Channel)) & 0x1U;
        
        if (0U != u32Flag)
        {
            eRetVal = PIT_E_TIMEOUT;
        }
    }
    
    return eRetVal;
}

/**
    @brief Get number of ns since timer has been started
    @param[in] u8Instance The desired PIT instance
    @param[in] u8Channel Channel to be checked
    @param[out] pu32Result Pointer to memory where number of nanoseconds will be written
    @retval PIT_E_OK Success, no timeout
    @retval PIT_E_TIMEOUT Timeout has occurred, result is not accurate
    @retval PIT_E_PARAMETER Invalid configuration
*/
pit_ret_t pit_get_elapsed_ns(const uint8_t u8Instance, const uint8_t u8Channel, uint32_t * const pu32Result)
{
    uint32_t u32LDVal, u32CVal, u32Res;
    pit_ret_t eRetVal = PIT_E_OK;

    eRetVal = pit_check_config(u8Instance, u8Channel);
    
    if (PIT_E_OK == eRetVal)
    {
        /** @violates @ref pit_c_REF_2 MISRA rule 11.3 */
        u32Res = RREG(PIT_TFLG(au32PitBase[u8Instance], u8Channel));
        if (0U != (u32Res & 0x1U))
        {
            eRetVal = PIT_E_TIMEOUT;
        }
        else
        {
            /** @violates @ref pit_c_REF_2 MISRA rule 11.3 */
            u32LDVal = RREG(PIT_LDVAL(au32PitBase[u8Instance], u8Channel));
            /** @violates @ref pit_c_REF_2 MISRA rule 11.3 */
            u32CVal = RREG(PIT_CVAL(au32PitBase[u8Instance], u8Channel));
            *pu32Result = (u32LDVal - u32CVal) * (1000000000U / get_pit_input_clock_hz());
        }
    }

    return eRetVal;
}

/**
    @brief Set up timer period
    @param[in] u8Instance The desired PIT instance
    @param[in] u8Channel Channel to be configured
    @param[in] u32PeriodNs New timer period in ns
    @retval PIT_E_OK Success, no timeout
    @retval PIT_E_PARAMETER Invalid configuration
*/
pit_ret_t pit_set_period(const uint8_t u8Instance, const uint8_t u8Channel, const uint32_t u32PeriodNs)
{
    uint32_t u32ClkNs;
    pit_ret_t eRetVal = PIT_E_OK;

    eRetVal = pit_check_config(u8Instance, u8Channel);
    
    if (PIT_E_OK == eRetVal)
    {
        if (0xffffffffU == u32PeriodNs)
        {
            /** @violates @ref pit_c_REF_2 MISRA rule 11.3 */
            WREG(PIT_LDVAL(au32PitBase[u8Instance], u8Channel), 0xffffffffU);
        }
        else
        {
            u32ClkNs = 1000000000U / get_pit_input_clock_hz();
            /** @violates @ref pit_c_REF_2 MISRA rule 11.3 */
            WREG(PIT_LDVAL(au32PitBase[u8Instance], u8Channel), (u32PeriodNs / u32ClkNs));
        }
    }
    
    return eRetVal;
}
/** @}*/
