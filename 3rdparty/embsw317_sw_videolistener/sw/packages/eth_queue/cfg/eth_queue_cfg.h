/**
  @addtogroup       dxgrETHQ
  @{

  @file             eth_queue_cfg.h
  @brief            Statical configuration of Ethernet Rx Queue driver
  @details

  Project           M4_ETH_PROJECT_NAME

  Platform          M4_ETH_PLATFORM

  SWVersion         M4_ETH_RELEASE_VERSION

  Copyright (c) 2016 NXP Semiconductors
  All Rights Reserved.
*/
/*==================================================================================================
==================================================================================================*/

#ifndef ETH_QUEUE_CFG_H
    #define ETH_QUEUE_CFG_H


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
/****************************/
/* Pre-compile ETHQ options */
/****************************/
/**
* @def      ETHQ_CFG_RX_BD_RING_LEN
* @brief    Number of buffers and BDs in our Rx Ethernet queue
* @details
* @hideinitializer
*/
#define ETHQ_CFG_RX_BD_RING_LEN     512U    /* (300k*4)/2/1.5k=400; 400+reserve=512 */

/**
* @def      ETHQ_CFG_USED_RX_QUEUE
* @brief    Selects which Rx queue shall be used
* @details  It is needed for selection of RDAR and for stream VLAN configuration
* @hideinitializer
*/
#define ETHQ_CFG_USED_RX_QUEUE      1U

/**
* @def      ETHQ_CFG_PRERUN_MAX_CYCLES
* @brief    Limit maximal number of cycles of pre-run code
* @details  Pre-run code determines which Rx buffer descriptor is active. In order to work, it requires certain rate of
*           incoming Ethernet frames. This option sets number of complete pre-run cycles to keep trying. Normally 1
*           cycle is enough. Each pre-run cycle lasts 2*ETHQ_CFG_PRERUN_WAIT_CYCLES of application iterations.
*           Minimum is 1.
* @hideinitializer
*/
#define ETHQ_CFG_PRERUN_MAX_CYCLES  5U

/**
* @def      ETHQ_CFG_PRERUN_MAX_CYCLES
* @brief    Limit maximal number of application iterations that pre-run code shall wait to receive a frame
* @details  Pre-run code determines which Rx buffer descriptor is active. In order to work, it requires certain rate of
*           incoming Ethernet frames. Increasing this option increases chance that a frame will be cough. It may be
*           useful to increase the value in case of lower incoming frame rate. Minimum is 1.
* @hideinitializer
*/
#define ETHQ_CFG_PRERUN_WAIT_CYCLES 1U

/** @cond dxINTERNAL_MACROS */
/*************************************/
/* Ethernet controller HW definition */
/*************************************/
/* General Eth controller register definition */
#define ETHQ_RDAR_OFFSET        (0x1D8U + (8U * ETHQ_CFG_USED_RX_QUEUE))

/* Queue dispatching register definitions */
#define FEC_R_DES_START_1       0x160U /* Receive descriptor ring 1 */
#define FEC_R_DES_START_2       0x16cU /* Receive descriptor ring 2 */
#define FEC_R_DES_START_0       0x180U /* Receive descriptor ring 0 (default) */

#define FEC_MRBR_1              0x168U
#define FEC_MRBR_2              0x174U

#define FEC_RCMR_1              0x1c8U /* Receive classification match ring 1 */
#define FEC_RCMR_2              0x1ccU /* Receive classification match ring 2 */

#if ETHQ_CFG_USED_RX_QUEUE == 0U
    #define FEC_R_DES_START     FEC_R_DES_START_0
    #define FEC_MRBR            FEC_MRBR_1
    #define FEC_RCMR            FEC_RCMR_1
#elif ETHQ_CFG_USED_RX_QUEUE == 1U
    #define FEC_R_DES_START     FEC_R_DES_START_1
    #define FEC_MRBR            FEC_MRBR_1
    #define FEC_RCMR            FEC_RCMR_1
#elif ETHQ_CFG_USED_RX_QUEUE == 2U
    #define FEC_R_DES_START     FEC_R_DES_START_2
    #define FEC_MRBR            FEC_MRBR_2
    #define FEC_RCMR            FEC_RCMR_2
#else
    #error Invalid queue selection
#endif

#define RCMR_MATCHEN            (0x1UL << 16)
#define RCMR_CMP_CFG(v, n)      (((v) & 0x7UL) <<  ((n) << 2UL))

/* Rx BD fields */
#define ENET_RXBD_EMPTY_OFFSET 3U
/** @endcond */

/**
* @def ETHQ_CFG_INIT_CHECK
* @brief Enable/Disable module initialization check. If enabled (TRUE) API verifies
*        that module has already been initialized and thus requested call can
*        be executed. If module has not been initialized yet an error code is returned.
* @hideinitializer
*/
#define ETHQ_CFG_INIT_CHECK     TRUE

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

#endif /* ETH_QUEUE_CFG_H */
/** @}*/
