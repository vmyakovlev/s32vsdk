/**
  @addtogroup       dxgrSM_CORE
  @{

  @file             sm.h
  @brief            The SM core header file
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
#ifndef __SM_H
#define __SM_H

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
/** @implements VLREQ038 */
#define SM_ENTRY_POINT(func)    static const sm_entry_point_t func ## _ptr __attribute__ ((section (".sm_init_vectors"))) = &func;

#define SM_ASYNC_TEST           (1ULL << 0)     /*  Test                            */
#define SM_ASYNC_ERROR          (1ULL << 1)     /*  Error                           */
#define SM_ASYNC_IN             (1ULL << 2)     /*  Input data available            */
#define SM_ASYNC_OUT            (1ULL << 3)     /*  Output buffer available         */
#define SM_ASYNC_MESSAGE        (1ULL << 4)     /*  Message available               */
#define SM_ASYNC_DEBUG          (1ULL << 5)     /*  Debug message available         */
#define SM_ASYNC_PRIORITY       (1ULL << 6)     /*  High priority input available   */
/*
    Add more reasons when needed:
    #define ...                 (1ULL << 7)
    #define ...                 ...
*/

#define SM_E_OK                 0
#define SM_E_FAILURE            -1
#define SM_E_MMAP_INIT          -2
#define SM_E_MMAP_START         -3
/** @endcond */

/*==================================================================================================
                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
typedef sint64_t (* sm_entry_point_t)(void);
typedef sint64_t (* sm_handler_t)(void);

/*  Warning: Do not change this definition unless
             you know what else you can affect */
typedef struct {
    uint64_t u64Val0;
    uint64_t u64Val1;
} sm_sc_param_t;

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/
#if (TRUE == SM_CFG_ENABLE_MMU)
sint32_t sm_mmap_init(void);
sint32_t sm_mmap_start(void);
sint32_t sm_mmap_stop(void);
#endif /* SM_CFG_ENABLE_MMU */
sint32_t sm_install_sc_handler(const uint32_t u32Key, const sm_handler_t pfHandler);
sint32_t sm_install_fiq_handler(const uint32_t u32IrqID, const sm_handler_t pfHandler);
sm_sc_param_t * sm_sc_get_params(void);
void sm_sc_set_query_result(uint64_t u64Result);
void sm_send_async_msg(uint64_t u64Reason, uint64_t u64UserVal);

#endif /* __SM_H */
/** @}*/
