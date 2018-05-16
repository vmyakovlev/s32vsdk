/**
  @addtogroup       dxgrETHQ
  @{

  @file             eth_queue.h
  @brief            API of Ethernet Rx Queue driver
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
* @section eth_queue_h_REF_1
* Violates MISRA 2004 Required Rule 8.7, Objects shall be defined at block scope if they are only
* accessed from within a single function. Object is intended to be shared among multiple modules.
*
*/

#ifndef ETH_QUEUE_H
    #define ETH_QUEUE_H

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
/* Configuration checks */
#if (ETHQ_CFG_USED_RX_QUEUE != 1) && (ETHQ_CFG_USED_RX_QUEUE != 2)
    #error Unsupported value of ETHQ_CFG_USED_RX_QUEUE
#endif

#if !defined (MMAP_DATA_NOCACHE)
    /* Memory mapping not defined */
    #define MMAP_DATA_NOCACHE(x)
#endif

/*==================================================================================================
                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/
/** @cond dxINTERNAL_MACROS */
/* Ethernet BD related macros */
#define ETHQ_RX_BD_LENGTH               32U
#define ETHQ_RX_BUFFER_LENGTH(BDIdx)    (*(uint16_t*)&(ETHQ_aau8BDRing[BDIdx][0U]))
#define ETHQ_RX_BUFFER_ADDR(BDIdx)      (*(uint32_t*)&(ETHQ_aau8BDRing[BDIdx][4U]))
/** @endcond */

/*==================================================================================================
                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
* @enum     eth_queue_ret_t
* @brief    Definition of return values of eth_queue functions
*/
typedef enum
{
    ETHQ_E_OK = 0,
    ETHQ_E_INVALID_CONFIG = -1,
    ETHQ_E_MULTIPLE_CONFIG = -2,
    ETHQ_E_TIMEOUT = -3,
    ETHQ_E_AGAIN = -4,
    ETHQ_E_NOT_INITIALIZED = -5,
    ETHQ_E_OVERFLOW = -6,
    ETHQ_E_NOT_FOUND = -7, 
} eth_queue_ret_t;

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
/** @violates @ref eth_queue_h_REF_1 MISRA rule 8.7 */
extern uint8_t      ETHQ_aau8BDRing[ETHQ_CFG_RX_BD_RING_LEN][ETHQ_RX_BD_LENGTH] MMAP_DATA_NOCACHE(32);
extern uint16_t     ETHQ_u16RxBDReadIdx;

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/
extern eth_queue_ret_t ETHQ_CheckConfig(void);
extern eth_queue_ret_t ETHQ_SetBaseAddr(uint32_t u32Addr);
extern eth_queue_ret_t ETHQ_SetRegionLength(uint32_t u32Len);
extern eth_queue_ret_t ETHQ_GetMemoryBase(uint32_t *pu32Base, uint32_t *pu32Length);
extern eth_queue_ret_t ETHQ_SetBufferSize(uint32_t u32BufSize);
extern eth_queue_ret_t ETHQ_SetRingAddr(uint32_t u32Addr);
extern eth_queue_ret_t ETHQ_GetBufferBase(uint32_t *pu32Base, uint32_t *pu32Length);

extern eth_queue_ret_t ETHQ_Init(void);
extern eth_queue_ret_t ETHQ_Empty(void);
extern eth_queue_ret_t ETHQ_PreRunIteration(void);
extern eth_queue_ret_t ETHQ_AddVlanClassification(uint8_t u8PCP);
extern eth_queue_ret_t ETHQ_RemoveVlanClassification(uint8_t u8PCP);
extern eth_queue_ret_t ETHQ_RemoveAllVlanClassifications(void);
extern eth_queue_ret_t ETHQ_SuspendVlanClassifications(void);
extern eth_queue_ret_t ETHQ_ResumeVlanClassifications(void);
extern eth_queue_ret_t ETHQ_Stop(void);
extern eth_queue_ret_t ETHQ_GetNextRxBDIdx(sint16_t *ps16Idx);

extern eth_queue_ret_t ETHQ_UnlockRxBD(uint32_t u32BDIdx);
extern eth_queue_ret_t ETHQ_WriteRDAR(void);


#endif /* ETH_QUEUE_H */
/** @}*/
