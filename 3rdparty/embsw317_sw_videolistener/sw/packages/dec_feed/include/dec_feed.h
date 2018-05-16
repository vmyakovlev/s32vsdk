/**
  @addtogroup       dxgrDECFEED
  @{

  @file             dec_feed.h
  @brief            API of the decoder driver
  @details          This driver handles only the input of the decoder. It cooperates with Linux
                    driver which handles the rest, mainly output of the decoder.

  Project           M4_ETH_PROJECT_NAME

  Platform          M4_ETH_PLATFORM

  SWVersion         M4_ETH_RELEASE_VERSION

  Copyright (c) 2016 NXP Semiconductors
  All Rights Reserved.
*/
/*==================================================================================================
==================================================================================================*/

#ifndef DEC_FEED_H
    #define DEC_FEED_H

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
/** @cond dxINTERNAL_MACROS */
/*  Supported decoders */
#define DECODER_JPEG    1
#define DECODER_H264    2
/** @endcond */

/*==================================================================================================
                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
* @enum     dec_feed_ret_t
* @brief    Definition of return values of dec_feed functions
*/
typedef enum
{
    DECFEED_E_OK = 0,
    DECFEED_E_INVALID_CONFIG = -1,
    DECFEED_E_MULTIPLE_CONFIG = -2,
    DECFEED_E_NOT_INITIALIZED = -3,
    DECFEED_E_INVALID_PARAMETER = -4,
    DECFEED_E_FIFO_FULL = -5,
    DECFEED_E_FIFO_ERROR = -6
} dec_feed_ret_t;

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/
extern dec_feed_ret_t DECFEED_CheckConfig(void);
extern dec_feed_ret_t DECFEED_SetBaseAddress(uint32_t u32Addr);
extern dec_feed_ret_t DECFEED_SetRegionLength(uint32_t u32Length);
extern dec_feed_ret_t DECFEED_GetMemoryBase(uint32_t *pu32Base, uint32_t *pu32Length);
extern dec_feed_ret_t DECFEED_SetBurstLength(uint8_t u8Count);
extern dec_feed_ret_t DECFEED_GetBurstLength(uint8_t *pu8Count);
extern dec_feed_ret_t DECFEED_SetNumOfStreams(uint8_t u8NumberOfStreams);
extern dec_feed_ret_t DECFEED_GetDecoderStatus(uint32_t *pu32StatusReg);
extern dec_feed_ret_t DECFEED_GetFreeSpace(uint8_t u8StreamIdx, uint32_t *pu32FreeEntriesNum);
extern dec_feed_ret_t DECFEED_HandleProcessedFrms(uint8_t u8StreamIdx, uint32_t *pu32ProcessedNum);
extern dec_feed_ret_t DECFEED_Push(uint8_t u8StreamIdx, uint32_t u32Addr, uint16_t u16Length);
extern dec_feed_ret_t DECFEED_Init(void);
extern dec_feed_ret_t DECFEED_Start(void);
extern dec_feed_ret_t DECFEED_Stop(void);

#endif /* DEC_FEED_H */
/** @}*/
