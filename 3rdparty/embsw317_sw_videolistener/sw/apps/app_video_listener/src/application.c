/**
  @defgroup         dxgrVLFIRMWARE The VideoListener Firmware
  @details          This is the final VideoListener Firmware application intended to be run out
                    of Linux context within the highest available exception level as a hosted
                    application of the @ref dxgrSM_CORE module. It does not contain any API to be
                    called from external environment.
                    
                    Application provides an entry point for the @ref dxgrSM_CORE and performs
                    initial initialization of PIT and GIC peripherals which are then configured
                    to be used as periodic interrupt generator to drive the application's core.
                    The application's core consists of the @ref dxgrSCORE which takes care of
                    the main functionality and a simple error handler verifying correct operation.
                    
                    Control events are passed from the @ref dxgrSM_CORE in form of callbacks to
                    start, stop and configure the application. All system calls accepted by the
                    firmware are:
                    - APP_KEY_START: start the application
                    - APP_KEY_STOP: stop the application
                    - APP_KEY_SET_CFG: set a configuration parameter
                    - APP_KEY_GET_CFG: get a configuration parameter
                    
                    and are defined within the application_cfg.h.
                    
                    The APP_CFG_PIT_IRQ_ID is used to identify interrupt request number triggering
                    the application's body and is being configured and enabled within the GIC. User
                    shall ensure that interrupt number configured in APP_CFG_PIT_IRQ_ID does not
                    conflict with any other interrupt within the system.
                    
                    Application performs detection of runtime errors. In case when some is detected
                    it triggers asynchronous notification event as implemented and described by the
                    @ref dxgrSM_CORE module which provides error reason and error flags to a possible
                    event listener.
                    
                    Integration notes
                    -----------------
                    For this application applies the same assumptions as for the @ref dxgrSM_CORE
                    module. Needs to be loaded to memory specified by the current linker script.

  @addtogroup       dxgrVLFIRMWARE
  @{

  @file             application.c
  @brief            Main file of the VideoListener firmware
  @details

  Project           M4_ETH_PROJECT_NAME
  Platform          M4_ETH_PLATFORM

  SWVersion         M4_ETH_RELEASE_VERSION

  Copyright (c) 2016 NXP Semiconductors
  All Rights Reserved.
*/
/*==================================================================================================
==================================================================================================*/

#ifdef __cplusplus
extern "C"{
#endif
/*==================================================================================================
                                         MISRA VIOLATIONS
==================================================================================================*/
/*
* @page misra_violations MISRA-C:2004 violations
*
* @section application_c_REF_1
* Violates MISRA 2004 Required Rule 1.2, No reliance shall be placed on undefined or unspecified
* behaviour.
* Address of near auto variable is to return a value via pointer.
* 
* @section application_c_REF_2
* Violates MISRA 2004 Required Rule 14.7, A function shall have a single point of exit at the end
* of the function.
* Multiple return statements are used to reduce code complexity.
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
#include "mmu_cfg.h"
#include "mmu.h"
#endif /* SM_CFG_ENABLE_MMU */
#include "mmap.h"
#include "gic.h"
#include "pit.h"
#include "application_cfg.h" /* Static application configuration */
#include "application.h"     /* Own header */
#include "stream_core.h"
#include "debug.h"           /* Debugging messages driver */

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/

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
/* Performance measurement variables */
#if TRUE == APP_CFG_DO_PERF
uint32_t SCORE_u32MaxFiqIterTime = 0U;
uint32_t SCORE_u32AverageFiqIterTime = 0U;
#endif /* APP_CFG_DO_PERF */

/*==================================================================================================
                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
/* Monitor SysCall API */
static sint64_t sm_app_init(void);
static sint64_t sm_app_start(void);
static sint64_t sm_app_stop(void);
static sint64_t sm_app_config(void);
static sint64_t sm_app_query(void);
/* FIQ ISR - the main iteration */
static sint64_t sm_app_body(void);

/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/
static sint64_t sm_app_init(void) {
    sint64_t s64RetVal = 0;
    register uint32_t u32RegVal;
    uint32_t u32Base, u32Length;
    mmu_ret_t mmuRetVal;

#if (TRUE == SM_CFG_ENABLE_MMU)
    /** @violates @ref application_c_REF_1 MISRA rule 1.2 */
    pit_get_memory_base(0U, &u32Base, &u32Length);
    mmuRetVal = mmu_add_mapping((va_t)u32Base, (pa_t)u32Base, (mlen_t)u32Length, mmu_get_attr(MTYPE_DEVICE, MA_EL2_EL3_RW));
    if (MMU_E_OK != mmuRetVal) {
        ERROR("[sm_app] PIT0 memory initialization failed: %d\n", (sint32_t)(mmuRetVal));
        /** @violates @ref application_c_REF_2 MISRA rule 14.7 */
        return -1;
    }
    /** @violates @ref application_c_REF_1 MISRA rule 1.2 */
    pit_get_memory_base(1U, &u32Base, &u32Length);
    mmuRetVal = mmu_add_mapping((va_t)u32Base, (pa_t)u32Base, (mlen_t)u32Length, mmu_get_attr(MTYPE_DEVICE, MA_EL2_EL3_RW));
    if (MMU_E_OK != mmuRetVal) {
        ERROR("[sm_app] PIT1 memory initialization failed: %d\n", (sint32_t)(mmuRetVal));
        /** @violates @ref application_c_REF_2 MISRA rule 14.7 */
        return -1;
    }
    /** @violates @ref application_c_REF_1 MISRA rule 1.2 */
    mc_me_get_memory_base(&u32Base, &u32Length);
    mmuRetVal = mmu_add_mapping((va_t)u32Base, (pa_t)u32Base, (mlen_t)u32Length, mmu_get_attr(MTYPE_DEVICE, MA_EL2_EL3_RW));
    if (MMU_E_OK != mmuRetVal) {
        ERROR("[sm_app] MC_ME memory initialization failed: %d\n", (sint32_t)(mmuRetVal));
        /** @violates @ref application_c_REF_2 MISRA rule 14.7 */
        return -1;
    }
    /** @violates @ref application_c_REF_1 MISRA rule 1.2 */
    mc_cgm_get_memory_base(0U, &u32Base, &u32Length);
    mmuRetVal = mmu_add_mapping((va_t)u32Base, (pa_t)u32Base, (mlen_t)u32Length, mmu_get_attr(MTYPE_DEVICE, MA_EL2_EL3_RW));
    if (MMU_E_OK != mmuRetVal) {
        ERROR("[sm_app] MC_ME memory initialization failed: %d\n", (sint32_t)(mmuRetVal));
        /** @violates @ref application_c_REF_2 MISRA rule 14.7 */
        return -1;
    }
#endif /* SM_CFG_ENABLE_MMU */

    s64RetVal = sm_install_sc_handler(APP_KEY_START, &sm_app_start);
    if (s64RetVal) {
        ERROR("Unable to install start handler: %d\n", s64RetVal);
        /** @violates @ref application_c_REF_2 MISRA rule 14.7 */
        return s64RetVal;
    }

    s64RetVal = sm_install_sc_handler(APP_KEY_STOP, &sm_app_stop);
    if (s64RetVal) {
        ERROR("Unable to install stop handler: %d\n", s64RetVal);
        /** @violates @ref application_c_REF_2 MISRA rule 14.7 */
        return s64RetVal;
    }

    s64RetVal = sm_install_fiq_handler(APP_CFG_PIT_IRQ_ID, &sm_app_body);
    if (s64RetVal) {
        ERROR("Unable to install IRQ handler: %d\n", s64RetVal);
        /** @violates @ref application_c_REF_2 MISRA rule 14.7 */
        return s64RetVal;
    }

    s64RetVal = sm_install_sc_handler(APP_KEY_SET_CFG, &sm_app_config);
    if (s64RetVal) {
        ERROR("Unable to install CFG handler: %d\n", s64RetVal);
        /** @violates @ref application_c_REF_2 MISRA rule 14.7 */
        return s64RetVal;
    }

    s64RetVal = sm_install_sc_handler(APP_KEY_GET_CFG, &sm_app_query);
    if (s64RetVal) {
        ERROR("Unable to install query handler: %d\n", s64RetVal);
        /** @violates @ref application_c_REF_2 MISRA rule 14.7 */
        return s64RetVal;
    }

    gicd_disable_all();
    gicc_disable_all();
    gicd_all_to_grp1();

    /*  Route FIQ to EL3 */
    __asm(" mrs %0, scr_el3;    \
            orr %1, %0, #0x4;   \
            msr scr_el3, %1;" :"=r"(u32RegVal) :"r"(u32RegVal) : );

    gicc_enable_fiq();
    gicd_enable_all();
    gicc_enable_all();
    gicd_set_target(APP_CFG_PIT_IRQ_ID, 0x1U);   /*  Target: CPU interface 0 */
    gicd_set_priority(APP_CFG_PIT_IRQ_ID, 0U);   /* The highest priority */
    gicd_set_sensitivity(APP_CFG_PIT_IRQ_ID, GICD_LEVEL_SENSITIVE);
    gicd_set_group0(APP_CFG_PIT_IRQ_ID);
    gicd_enable(APP_CFG_PIT_IRQ_ID);
    
    /*  Ensure the PIT is clocked */
    if (PIT_E_OK != pit_init(APP_CFG_PIT_INSTANCE)) {
        ERROR("Can't initialize the PIT%d\n", APP_CFG_PIT_INSTANCE);
        /** @violates @ref application_c_REF_2 MISRA rule 14.7 */
        return -1;
    }

    return s64RetVal;
}

static sint64_t sm_app_query(void)
{
	/* Hint: use the following construct to decide which parameter to return
    uint64_t u64Output;
    sm_sc_param_t *sc_params = sm_sc_get_params();;
    switch(sc_params->u64Val0 & 0xFFFFFFFFU)
    {
        case TBD:
            u64Output = TBD();
            sm_sc_set_query_result(u64Output);
            break;
       default:
            return (uint64_t) -1;
            break;
    }

THIS COMMENT IS INTENDED TO BE DELETED ONCE THE FIRST QUERY SUPPORT IS ADDED

    */
	return 0;
}

static sint64_t sm_app_config(void) {
    sm_sc_param_t *sc_params;
    uint32_t new_period_ns;
    sint64_t s64RetVal = 0;

    sc_params = sm_sc_get_params();
    switch (sc_params->u64Val0 & 0xffffffffU)
    {
        /* TIMING PARAMETERS */
        case APP_KEY_CHANGE_PIT_F:
            new_period_ns = (uint32_t)sc_params->u64Val1;
            if (pit_set_period(APP_CFG_PIT_INSTANCE, APP_CFG_PIT_CHANNEL, new_period_ns))
            {
                ERROR("Unable to change PIT period\n");
                s64RetVal = -1;
            }
            break;
        /* TEST */
        case APP_KEY_TEST_EVENT:
            sm_send_async_msg(SM_ASYNC_TEST, 0x1234abcdULL);
            break;
        /* STREAMING PARAMETERS */
        default:
            s64RetVal = SCORE_SetConf(sc_params->u64Val0, sc_params->u64Val1);
            break;
    }

    return s64RetVal;
}

static sint64_t sm_app_start(void) {
    sint64_t s64Status;
    sint64_t s64RetVal = 0;

    INFO("SM APP START\n");
    
    s64Status = SCORE_Init();
    if (s64Status) {
        ERROR("Function SCORE_Init failed: %d\n", s64Status);
        /** @violates @ref application_c_REF_2 MISRA rule 14.7 */
        return s64Status;
    }
    
    s64Status = SCORE_Start();
    if (s64Status) {
        ERROR("Function SCORE_Start failed: %d\n", s64Status);
        /** @violates @ref application_c_REF_2 MISRA rule 14.7 */
        return s64Status; /* Do not continue */
    }

    /* Activate the FIQ */
    s64Status = pit_start(APP_CFG_PIT_INSTANCE, APP_CFG_PIT_CHANNEL);
    if (s64Status) {
        ERROR("pit_start returned %d\n", s64Status);
        s64RetVal = s64Status;
    }
    else {
        s64Status = pit_enable_irq(APP_CFG_PIT_INSTANCE, APP_CFG_PIT_CHANNEL);
        if (s64Status) {
            ERROR("pit_enable_irq failed\n");
            s64RetVal = s64Status;
        }
    }

    return s64RetVal;
}

static sint64_t sm_app_stop(void) {
    sint64_t s64Status;
    sint64_t s64RetVal = 0;
    
#if TRUE == APP_CFG_DO_PERF
    INFO("MaxFiqIterTime = %u ns\n", SCORE_u32MaxFiqIterTime);
    INFO("AverageFiqIterTime = %u ns\n", SCORE_u32AverageFiqIterTime);
#endif

    s64Status = pit_stop(APP_CFG_PIT_INSTANCE, APP_CFG_PIT_CHANNEL);
    if (s64Status) {
        ERROR("pit_stop returned %d\n", s64Status);
        s64RetVal = s64Status;
    }

    /* Confirm the PIT interrupt flag here in case it is pending */
    s64Status = pit_confirm_irq(APP_CFG_PIT_INSTANCE, APP_CFG_PIT_CHANNEL);
    if (s64Status) {
        ERROR("pit_confirm_irq returned %d\n", s64Status);
        s64RetVal = s64Status;
    }

    s64Status = SCORE_Stop();
    if (s64Status) {
        ERROR("SCORE_Stop returned %d\n", s64Status);
        s64RetVal = s64Status;
    }

    INFO("SM APP STOP\n");
    
    return s64RetVal;
}

static sint64_t sm_app_body(void) {
    sint32_t s32Status;
    uint32_t u32ErrMask = 0U;
    uint64_t u64temp;
    /* Time measurement */
#if TRUE == APP_CFG_DO_PERF
    static uint32_t u32ElapsedNs;
    static uint64_t SCORE_u64TotalFiqTime = 0U;
    static uint32_t SCORE_u32IrqIterCount = 0U;
#endif /* APP_CFG_DO_PERF */

    /* Start stop-watch */
#if TRUE == APP_CFG_DO_PERF
    s32Status = pit_set_period(APP_CFG_PIT_INSTANCE, APP_CFG_PIT_AUX1_CHANNEL, 0xffffffffU);   
    if (s32Status) {
        ERROR("AUX1 PIT not configured\n");
        u32ErrMask |= 1U<<0U;
    }
    s32Status = pit_start(APP_CFG_PIT_AUX1_INSTANCE, APP_CFG_PIT_AUX1_CHANNEL);
    if (s32Status) {
        ERROR("AUX1 PIT not started\n");
        u32ErrMask |= 1U<<1U;
    }
#endif /* APP_CFG_DO_PERF */

    /* Intentionally slow it down */
#if APP_CFG_FIQ_DELAY_NS > 0U
    s32Status = pit_set_period(APP_CFG_PIT_INSTANCE, APP_CFG_PIT_AUX0_CHANNEL, APP_CFG_FIQ_DELAY_NS);
    if (s32Status) {
        ERROR("AUX0 PIT not configured\n");
        u32ErrMask |= 1U<<2U;
    }
    s32Status = pit_start(APP_CFG_PIT_AUX0_INSTANCE, APP_CFG_PIT_AUX0_CHANNEL);
    if (s32Status) {
        ERROR("AUX0 PIT not started: %d\n", s32Status);
        u32ErrMask |= 1U<<3U;
    }
    while (!pit_is_timeout(APP_CFG_PIT_AUX0_INSTANCE, APP_CFG_PIT_AUX0_CHANNEL)) ;
    (void)pit_stop(APP_CFG_PIT_AUX0_INSTANCE, APP_CFG_PIT_AUX0_CHANNEL);
    (void)pit_confirm_irq(APP_CFG_PIT_AUX0_INSTANCE, APP_CFG_PIT_AUX0_CHANNEL);
#endif /* APP_CFG_FIQ_DELAY_NS */

    /* Confirm interrupt */
    s32Status = pit_confirm_irq(APP_CFG_PIT_INSTANCE, APP_CFG_PIT_CHANNEL);
    if (s32Status) {
        ERROR("pit_confirm_irq returned %d\n", s32Status);
        u32ErrMask |= 1U<<4U;
    }

    SCORE_Iteration();

    if ((SCORE_ST_ERROR == SCORE_GetCurrentState()) && (FALSE == SCORE_IS_RECOVERABLE(SCORE_GetErrorMask())))
    {
        ERROR("Fatal error detected, stopping\n");
        /* There is some error, stop the interrupts */
        s32Status = pit_stop(APP_CFG_PIT_INSTANCE, APP_CFG_PIT_CHANNEL);
        if (s32Status) {
            ERROR("pit_stop returned %d\n", s32Status);
            u32ErrMask |= 1U<<5U;
        }
        /* Confirm the PIT interrupt flag here in case it is pending */
        s32Status = pit_confirm_irq(APP_CFG_PIT_INSTANCE, APP_CFG_PIT_CHANNEL);
        if (s32Status) {
            ERROR("pit_confirm_irq returned %d\n", s32Status);
            u32ErrMask |= 1U<<6U;
        }
        /* Due to Linux limitation for sigpoll handler only 32bits can be used in 2nd argument: */
        u64temp = (uint64_t)(~(((uint32_t)0x1U)<<31));
        u64temp &= SCORE_GetErrorMask();
        sm_send_async_msg(SM_ASYNC_ERROR, u64temp);
    }

    /* Stop and evaluate stop-watch */
#if TRUE == APP_CFG_DO_PERF
    s32Status = pit_get_elapsed_ns(APP_CFG_PIT_AUX1_INSTANCE, APP_CFG_PIT_AUX1_CHANNEL, &u32ElapsedNs);
    if (s32Status) {
        ERROR("pit_get_elapsed_ns returned %d\n", s32Status);
        u32ErrMask |= 1U<<7U;
    }
    (void)pit_stop(APP_CFG_PIT_AUX1_INSTANCE, APP_CFG_PIT_AUX1_CHANNEL);
    (void)pit_confirm_irq(APP_CFG_PIT_AUX1_INSTANCE, APP_CFG_PIT_AUX1_CHANNEL);
    /* Update iteration time measurements */
    SCORE_u32MaxFiqIterTime = (u32ElapsedNs > SCORE_u32MaxFiqIterTime) ? u32ElapsedNs : SCORE_u32MaxFiqIterTime;
    SCORE_u64TotalFiqTime += u32ElapsedNs;
    SCORE_u32IrqIterCount++;
    SCORE_u32AverageFiqIterTime = (uint32_t)(SCORE_u64TotalFiqTime/SCORE_u32IrqIterCount);
#endif /* APP_CFG_DO_PERF */

    if (u32ErrMask) {
        uint32_t u32PassedFlags = 0U;
        sint64_t s64Status = (sint32_t)sm_app_stop();
        if (s64Status) {
            ERROR("Automatic stop returned error %lld\n", s64Status);
            u32ErrMask |= (((uint16_t)0x0001U)<<8U);
        }
        /* Everything shall stop. Probably configuration error. */
        /* Due to Linux limitation for sigpoll handler only 32bits can be used in 2nd argument: */
        /* Pass either Streaming or SM error mask, the SM is preferred */
        /* Reserved bit set: passing SM app errors */
        u32PassedFlags = (((uint32_t)1U)<<31) | u32ErrMask;

        sm_send_async_msg(SM_ASYNC_ERROR, (uint64_t)u32PassedFlags);
    }

    return 0; /* The return value does not make sense in interrupt */
}

/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
                                           EXPORT
==================================================================================================*/
/*  Lint message #19 ignored because following statement describes application's entry
    point as seen by the SM Core module */
/*lint -e19 */
SM_ENTRY_POINT(sm_app_init);

/*================================================================================================*/
#ifdef __cplusplus
}
#endif
/** @}*/
