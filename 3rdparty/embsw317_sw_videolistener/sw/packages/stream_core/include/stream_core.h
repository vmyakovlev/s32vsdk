/**
  @addtogroup       dxgrSCORE
  @{

  @file             stream_core.h
  @brief            API of core of the M4_ETH_PROJECT_NAME Streaming Application running in FIQ
  @details

  Project           M4_ETH_PROJECT_NAME

  Platform          M4_ETH_PLATFORM

  SWVersion         M4_ETH_RELEASE_VERSION

  Copyright (c) 2016 NXP Semiconductors
  All Rights Reserved.
*/
/*==================================================================================================
==================================================================================================*/

#ifndef STREAM_CORE_H
    #define STREAM_CORE_H

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
/* No include shall be here. Exceptions must be approved */

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
/* Supported stream formats */
#define JPEG_OVER_AVB                       1
#define H264_OVER_AVB                       2

/* Commands (bits) to be used in SCORE_trCtl.u32Command */
#define SCORE_CMD_STOP                      (1U<<0)
#define SCORE_CMD_CONFIGURE                 (1U<<1)
#define SCORE_CMD_START                     (1U<<2)

/* Error bits to be used in SCORE_trCtl.u32ErrMask */
#define SCORE_ERR_STREAM_Q_BROKEN           (uint32_t)(1UL<<0)
#define SCORE_ERR_ETH_BUFFER_OVERWRITE      (uint32_t)(1UL<<1)
#define SCORE_ERR_STREAM_Q_FULL             (uint32_t)(1UL<<2)
#define SCORE_ERR_OUT_OF_ORDER_FRAME        (uint32_t)(1UL<<3)
#define SCORE_ERR_INVALID_STATE             (uint32_t)(1UL<<4)
#define SCORE_ERR_INVALID_CONFIGURATION     (uint32_t)(1UL<<5)
#define SCORE_ERR_FAILED_TO_ADD_VLAN_TAG    (uint32_t)(1UL<<6)
#define SCORE_ERR_ETHQ_INCOMPLETE_CONFIG    (uint32_t)(1UL<<7)
#define SCORE_ERR_ONE_TIME_CONF_CHANGE      (uint32_t)(1UL<<8) /* One time configurable value was changed */
#define SCORE_ERR_ETHQ_FIELD_NOT_ALIGNED    (uint32_t)(1UL<<9) /* One of fields in header is not aligned to data type size */
#define SCORE_ERR_UNKNOWN_PARAM             (uint32_t)(1UL<<10)
#define SCORE_ERR_UNSUPPORTED_PARAM_VAL     (uint32_t)(1UL<<11)
#define SCORE_ERR_DECODER_ERR               (uint32_t)(1UL<<12)
#define SCORE_ERR_MEMORY_MAPPING            (uint32_t)(1UL<<13)
#define SCORE_ERR_PREINIT_FAILED            (uint32_t)(1UL<<14)
#define SCORE_ERR_ETHQ_ERROR                (uint32_t)(1UL<<15)
#define SCORE_ERR_NOT_INITIALIZED           (uint32_t)(1UL<<16)
#define SCORE_ERR_STREAM_DROP_OUT           (uint32_t)(1UL<<17)
#define SCORE_ERR_UNSUPPORTED_STREAM        (uint32_t)(1UL<<18)

/* Warning bits to be used in SCORE_trCtl.u32ErrMask */
#define SCORE_ERR_WARNINGS_MASK             0xFF000000U
#define SCORE_WAR_UNKNOWN_STREAM_ID_FRAME   (uint32_t)(1UL<<24)
#define SCORE_WAR_RECONFIG_WHILE_RUNNING    (uint32_t)(1UL<<25)
#define SCORE_WAR_AUTOMATIC_RESTART         (uint32_t)(1UL<<26)
#define SCORE_WAR_TOO_MANY_VLAN_PRIO        (uint32_t)(1UL<<27)
#define SCORE_WAR_INACTIVE_NOT_SUSPENDED    (uint32_t)(1UL<<28) /* In READY or ERROR state the timer shall be suspended
                                                                 so the main iteration function shall not be called */
#define SCORE_WAR_RESERVED                  (uint32_t)(1UL<<31) /* One free bit reserved for upper layers */

/*  NOTE that once the decoder feeding was started and any error damages the stream data, whole application must be
    restarted. For now only one error and only in FETCH state is automatically recoverable. There is no way to determine
    the state since it was overwritten by error state, so no error is automatically recoverable for now. */
#define SCORE_IS_RECOVERABLE(errMask)                                                                                  \
                                   ( ( 0U == (errMask & ~(SCORE_ERR_WARNINGS_MASK /*| SCORE_ERR_OUT_OF_ORDER_FRAME*/)) \
                                     ) ? TRUE : FALSE                                                                  \
                                   )

/*==================================================================================================
                                             ENUMS
==================================================================================================*/
/**
* @enum     SCORE_tenState
* @brief    Definition of possible values of current stream_core state
*/
typedef enum
{
    SCORE_ST_READY,   /**< Waiting for START or CONFIGURE command */
    SCORE_ST_FETCH,   /**< Waiting for frames with start of image/key-frame */
    SCORE_ST_PRERUN,  /**< The pre-run phase */
    SCORE_ST_RUN,     /**< Running */
    SCORE_ST_ERROR    /**< Fatal error encountered, reconfiguration needed */
} SCORE_tenState;

/**
* @enum     stream_core_ret_t
* @brief    Definition of return values of stream_core functions
* @details  This values are used by functions which return their error state. There are also
*           functions that can not fail and they return void or different information. 
*/
typedef enum
{
    /* Error codes */
    SCORE_E_OK = 0,
    SCORE_E_INVALID_CONFIG = -1,
    SCORE_E_UNKNOWN_PARAMETER = -2,
    SCORE_E_MAPPING_ERROR = -3,
    SCORE_E_NOT_INITIALIZED = -4,
    SCORE_E_ALREADY_INITIALIZED = -5
} stream_core_ret_t;

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/
/* Called from Monitor SysCalls */
extern stream_core_ret_t SCORE_CheckConfig(void);
extern stream_core_ret_t SCORE_Init(void);
extern stream_core_ret_t SCORE_GetConf(uint64_t u64Var, uint64_t *pu64Val);
extern stream_core_ret_t SCORE_SetConf(uint64_t u64Var, uint64_t u64Val);
extern stream_core_ret_t SCORE_Start(void);
extern stream_core_ret_t SCORE_Stop(void);
extern uint32_t SCORE_GetErrorMask(void);
extern void SCORE_ClearErrorMask(void);
extern SCORE_tenState SCORE_GetCurrentState(void);
/* Called from FIQ */
extern void SCORE_Iteration(void);

#endif /* STREAM_CORE_H */
/** @}*/
