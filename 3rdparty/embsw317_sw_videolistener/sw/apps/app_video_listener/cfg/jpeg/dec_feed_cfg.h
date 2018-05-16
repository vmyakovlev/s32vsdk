/**
  @file             dec_feed_cfg.h
  @version          0.0.0.0

  @brief            Statical configuration of video decoder driver
  @details

  Project           M4_ETH_PROJECT_NAME
  Platform          M4_ETH_PLATFORM

  SWVersion         M4_ETH_RELEASE_VERSION

  Copyright (c) 2016 NXP Semiconductors
  All Rights Reserved.
*/
/*==================================================================================================
==================================================================================================*/

#ifndef DEC_FEED_CFG_H
    #define DEC_FEED_CFG_H


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
/*  The desired decoder variant to be supported */
#define DECFEED_CFG_DECODER_VARIANT                     DECODER_JPEG

/*  Enable/Disable module initialization check. If enabled (TRUE) API verifies
    that module already has been initialized and thus requested call can
    be executed. */
#define DECFEED_CFG_INIT_CHECK                          TRUE

#ifdef APP_CFG_MAX_STREAMS
    #define DEC_FEED_CFG_MAX_STREAMS                    APP_CFG_MAX_STREAMS
#else
    #define DEC_FEED_CFG_MAX_STREAMS                    4U
#endif

/*******************************/
/* Pre-compile DECFEED options */
/*******************************/

#if (DECODER_JPEG == DECFEED_CFG_DECODER_VARIANT)
/******************************/
/* JPEG decoder HW definition */
/******************************/
#define DECFEED_CFG_FIFO_LENGTH                         16U
/* The status register */
#define JPEG_W_STATUS                                   (uint32_t)(*(volatile uint32_t*)((tuAddr)DECFEED_rCfg.u32DecoderBaseAddr + (tuAddr)0x260U))
/* Current space available in each stream IPS FIFO */
#define JPEG_W_STATUS_PKTFIFOSTATE_OFFS(StreamNum)      ((6U*(StreamNum))+1U)
#define JPEG_W_STATUS_PKTFIFOSTATE_MASK                 0x1FU
#define JPEG_W_STATUS_PKT_FIFO_STATE(StreamNum)         ((JPEG_W_STATUS >> JPEG_W_STATUS_PKTFIFOSTATE_OFFS(StreamNum)) & JPEG_W_STATUS_PKTFIFOSTATE_MASK)

/* Here put HW address (in SRAM) of next buffer to be decoded: */
#define JPEG_STn_SRAM_PTR(StreamNum)                    *(volatile uint32_t*)((tuAddr)DECFEED_rCfg.u32DecoderBaseAddr + ((tuAddr)(StreamNum)*0x88U))

/* Here put length in bytes of next buffer to be decoded. */
/* Shall be written after JPEG_STn_SRAM_PTR. */
/* Only the lower 16 bits are valid, but there is no need to mask it */
#define JPEG_STn_SRAM_LEN_VAL(StreamNum)                *(volatile uint32_t*)((tuAddr)DECFEED_rCfg.u32DecoderBaseAddr + 0x4U + ((tuAddr)(StreamNum)*0x88U))

#define JPEG_W_CTRL1                                    *(volatile uint32_t*)(tuAddr)(DECFEED_rCfg.u32DecoderBaseAddr + 0x254)
#define JPEG_W_CTRL1_SW_RESET_MASK                      0x1U

#define JPEG_E_STATUS_CR_ERR_MASK                       0x1U

#define DEC_STATUS_OFFSET                               ((tuAddr)0x264U)
#endif /* DECODER_JPEG */

#if (DECODER_H264 == DECFEED_CFG_DECODER_VARIANT)
/******************************/
/* H264 decoder HW definition */
/******************************/
    #define DECFEED_CFG_FIFO_LENGTH                     16U
    #define H264_DEC_STAT                               0x384U
    #define H264_DEC_ISR                                0x3e0U
    #define H264_DEC_STR_01_PKT_FIFO_STAT               0x040U
    #define H264_DEC_STR_23_PKT_FIFO_STAT               0x044U
    #define H264_DEC_STR_PKT_ADDR                       0x030U
    #define H264_DEC_STR_TRIG_PKT_CFG                   0x034U
#endif /* DECODER_H264 */

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

#endif /* DEC_FEED_CFG_H */
