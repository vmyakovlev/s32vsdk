/**
  @file             stream_core_cfg.h
  @version          0.0.0.0

  @brief            The StreamCore module configuration file
  @details

  Project           M4_ETH_PROJECT_NAME
  Platform          M4_ETH_PLATFORM

  SWVersion         M4_ETH_RELEASE_VERSION

  Copyright (c) 2016 NXP Semiconductors
  All Rights Reserved.
*/
/*==================================================================================================
==================================================================================================*/

#ifndef STREAM_CORE_CFG_H
    #define STREAM_CORE_CFG_H


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
/*  Type of stream to be processed */
#define SCORE_CFG_STREAM_TYPE           JPEG_OVER_AVB

/*  If TRUE then stream preprocessing is engaged to prepare compatible input of the decoder HW.
    Only valid for H264_OVER_AVB stream type. */
#define SCORE_PREPROCESS_H264_STREAM    TRUE

/*
* @def      SCORE_CFG_STREAM_BD_RING_LEN
* @brief    Buffer for BDs for each stream
* @details  Frames from Ethernet queue are sorted into stream queues. Their ratio is not fixed,
*           so some reserve could be added.
* @hideinitializer
*/
#define SCORE_CFG_STREAM_BD_RING_LEN    ((ETHQ_CFG_RX_BD_RING_LEN/APP_CFG_MAX_STREAMS) + (ETHQ_CFG_RX_BD_RING_LEN/10U))

/*
* @def      SCORE_CFG_PERMANENT_SOI_SYNC
* @brief    Enables or disables permanent SOI synchronization.
*/
#define SCORE_CFG_PERMANENT_SOI_SYNC    TRUE

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

#endif /* STREAM_CORE_CFG_H */
