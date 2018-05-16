/**
  @addtogroup       dxgrSM_CORE
  @{

  @file             sm_cfg.h
  @brief            The SM core configuration header file
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
#ifndef __SM_CONFIG_H
#define __SM_CONFIG_H

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
#define SM_CFG_PLATFORM_S32V234             1
/** @endcond */

/**
*   @def SM_CFG_SC_H_COUNT
*   @brief Maximum number of SC handlers
*   @hideinitializer
*/
#define SM_CFG_SC_H_COUNT                   6U

/**
*   @def SM_CFG_FIQ_H_COUNT
*   @brief Maximum number of FIQ handlers
*   @hideinitializer
*/
#define SM_CFG_FIQ_H_COUNT                  3U

/**
*   @def SM_CFG_ENABLE_MMU
*   @brief If TRUE then MMU is engaged @ EL3
*   @hideinitializer
*/
#define SM_CFG_ENABLE_MMU                   TRUE

/**
*   @def SM_CFG_LL_INIT_KEY
*   @brief SC identifier of the low-level initialization status request
*/
#define SM_CFG_LL_INIT_KEY                  0xffffU

/**
*   @def SM_CFG_LL_CONFIRM_EVENT_KEY
*   @brief SC identifier of the event confirmation
*/
#define SM_CFG_LL_CONFIRM_EVENT_KEY         0xfffeU

/**
*   @def SM_CFG_LL_ENABLE_EVENT_KEY
*   @brief SC identifier of the enable/disable request
*/
#define SM_CFG_LL_ENABLE_EVENT_KEY          0xfffdU

/**
*   @def SM_CFG_LL_SHUTDOWN_KEY
*   @brief SC identifier of the SM code shut-down request
*/
#define SM_CFG_LL_SHUTDOWN_KEY              0xfffcU

/*  Platform-specific configuration (S32V234) */
#if (PLATFORM == S32V234)
/** @cond dxINTERNAL_MACROS */
    #define SM_CFG_VT_SECTION                   ".monitor_vector_table","ax"
    #define SM_CFG_INSTALLER_SECTION            ".text","ax"
    #define SM_CFG_LR                           x30
    #define SM_CFG_IAR_ADDR                     0x7d00200c
    #define SM_CFG_EOIR_ADDR                    0x7d002010
    #define SM_CFG_LL_INIT_LOCK_VALUE           =0xd503201f
/** @endcond */

/** @cond dxINTERNAL_MACROS */
    /*  Asynchronous event indication */
    #define SM_CFG_ASYNC_IRQ                32U
    #define MSCM_BASE_ADDR                  0x40081000U
    #define MSCM_REGION_LENGTH              0x1000U
    #define MSCM_IRCP0IR                    (MSCM_BASE_ADDR + 0x0800U)
    #define MSCM_IRCP1IR                    (MSCM_BASE_ADDR + 0x0804U)
    #define MSCM_IRCPGIR                    (MSCM_BASE_ADDR + 0x0820U)
/** @endcond */
    
    /*  Direct interrupt 0 to THIS (CP1) processor only: TLF=0x2, CPUTL=0x0, INTID=0 */
    
    /**
    *   @def sm_cfg_send_notification()
    *   @brief Called when module needs to send asynchronous notification
    *   @hideinitializer
    */
    #define sm_cfg_send_notification()      WREG(MSCM_IRCPGIR, 0x02000000U)
    
    /**
    *   @def sm_cfg_confirm_notification()
    *   @brief Called by the SM Core module to confirm notification
    *   @hideinitializer
    */
    #define sm_cfg_confirm_notification()   WREG(MSCM_IRCP1IR, 0x1U)
    
    /**
    *   @def sm_cfg_enable_notification()
    *   @brief Called by the SM Core module when asynchronous notification feature shall be enabled
    *   @hideinitializer
    */
    #define sm_cfg_enable_notification()                                \
        gicd_set_sensitivity(SM_CFG_ASYNC_IRQ, GICD_LEVEL_SENSITIVE);   \
        gicd_set_target(SM_CFG_ASYNC_IRQ, 0x1U);                        \
        gicd_set_priority(SM_CFG_ASYNC_IRQ, 0xfU);                      \
        gicd_set_group1(SM_CFG_ASYNC_IRQ);                              \
        gicd_enable(SM_CFG_ASYNC_IRQ)
    
    /**
    *   @def sm_cfg_disable_notification()
    *   @brief Called by the SM Core module to disable the notification
    *   @hideinitializer
    */
    #define sm_cfg_disable_notification()   gicd_disable(SM_CFG_ASYNC_IRQ)
    
#else
    #error Please specify a supported platform
#endif

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

#endif /* __SM_CONFIG_H */
/** @}*/
