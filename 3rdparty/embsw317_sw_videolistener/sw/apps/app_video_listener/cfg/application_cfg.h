/**
  @addtogroup       dxgrSCORE
  @{

  @file             application_cfg.h
  @brief            Static part of M4_ETH_PROJECT_NAME streaming application configuration
  @details

  Project           M4_ETH_PROJECT_NAME
  Platform          M4_ETH_PLATFORM

  SWVersion         M4_ETH_RELEASE_VERSION

  Copyright (c) 2016 NXP Semiconductors
  All Rights Reserved.
*/
/*==================================================================================================
==================================================================================================*/

#ifndef APPLICATION_CFG_H
    #define APPLICATION_CFG_H

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
/*
* @def      APP_CFG_ZERO_BUF_ENABLED
* @brief    Enables or disables insertion of one zero buffer after each video frame.
* @details  Needed for H264 decoder on S32V234. In case of JPEG decoder turn it off. */
#define APP_CFG_ZERO_BUF_ENABLED      FALSE
#if TRUE == APP_CFG_ZERO_BUF_ENABLED
    /*
    * @def      APP_CFG_ZERO_BUF_HEAD_LENGTH
    * @brief    Length of the header in zero buffer which is needed for the software.
    * @details  Shall reflect maximal expected length of Ethernet frame header of a video packet.
    *           Software expects that the header is always present in front of the data. */
    #define APP_CFG_ZERO_BUF_HEAD_LENGTH  64U
    /*
    * @def      APP_CFG_ZERO_BUF_DATA_LENGTH
    * @brief    Length of the data in zero buffer which is pushed to the decoder. */
    #define APP_CFG_ZERO_BUF_DATA_LENGTH  128U
#endif /* APP_CFG_ZERO_BUF_ENABLED */

#define APP_CFG_PIT_INSTANCE        0U
#define APP_CFG_PIT_CHANNEL         0U
#define APP_CFG_PIT_IRQ_ID          63U

/*  Non-zero value here enables active waiting within IRQ handler */
#define APP_CFG_FIQ_DELAY_NS        0U
#define APP_CFG_PIT_AUX0_INSTANCE   0U
#define APP_CFG_PIT_AUX0_CHANNEL    1U

/*  If TRUE then a PIT will measure time spent within the IRQ handler */
#define APP_CFG_DO_PERF             TRUE
#define APP_CFG_PIT_AUX1_INSTANCE   0U
#define APP_CFG_PIT_AUX1_CHANNEL    2U

/* Other application wide settings */
#define APP_CFG_MAX_STREAMS         4U

/*  Define KEYs for system calls (start with APP_KEY_).
    Add the CFG_FILE comment after the KEY definition to make the value loadable from configuration file. Also add the
    value to the configuration file then. */
/*  Controls */
#define APP_KEY_START                   0x1002U
#define APP_KEY_STOP                    0x1003U
#define APP_KEY_SET_CFG                 0x1004U
#define APP_KEY_GET_CFG                 0x1005U
#define APP_KEY_CHANGE_PIT_F            0x0001U /*CFG_FILE*/
#define APP_KEY_TEST_EVENT              0x0002U
/*  Streaming configuration */
/**
* @def      APP_KEY_STRM_FETCH_THRESHOLD
* @brief    Maximum number of frames stored after SOI is detected in FETCH state. If state is not
*           changed then the stream buffer is freed.
* @hideinitializer
*/
#define APP_KEY_STRM_FETCH_THRESHOLD    0x1000U /*CFG_FILE*/

/**
* @def      APP_KEY_STRM_SOI_MARK_VALUE
* @brief    Specification of the StartOfImage marker. This value is being compared with the one
*           within the Ethernet frames to determine start of image. Value is being used for the
*           JPEG_OVER_AVB stream type only.
* @hideinitializer
*/
#define APP_KEY_STRM_SOI_MARK_VALUE     0x1001U /*CFG_FILE*/

/**
* @def      APP_KEY_STRM_SOI_MARK_OFFSET
* @brief    Offset of the StartOfImage marker within Ethernet frame. Value is being used for the
*           JPEG_OVER_AVB stream type only.
* @hideinitializer
*/
#define APP_KEY_STRM_SOI_MARK_OFFSET    0x1002U /*CFG_FILE*/

/**
* @def      APP_KEY_STRM_SOI_MARK_MASK
* @brief    Mask of the StartOfImage marker within the 32 bits. Value is being used for the
*           JPEG_OVER_AVB stream type only.
* @hideinitializer
*/
#define APP_KEY_STRM_SOI_MARK_MASK      0x1003U /*CFG_FILE*/

/**
* @def      APP_KEY_STRM_SEQ_NUM_OFFSET
* @brief    Offset of the Sequence Number within Ethernet frame
* @hideinitializer
*/
#define APP_KEY_STRM_SEQ_NUM_OFFSET     0x1004U /*CFG_FILE*/

/**
* @def      APP_KEY_STRM_STRM_ID_OFFSET
* @brief    Offset of the Stream ID within Ethernet frame
* @hideinitializer
*/
#define APP_KEY_STRM_STRM_ID_OFFSET     0x1005U /*CFG_FILE*/

/**
* @def      APP_KEY_STRM_NUMBER_OF_STRMS
* @brief    Requested number of streams
* @hideinitializer
*/
#define APP_KEY_STRM_NUMBER_OF_STRMS    0x1006U /*CFG_FILE*/

/**
* @def      APP_KEY_STRM_FRM_DATA_OFFSET
* @brief    Offset of the Frame Data (payload) within Ethernet frame. If AVB is used as transport
*           protocol this is the AVB payload offset.
* @hideinitializer
*/
#define APP_KEY_STRM_FRM_DATA_OFFSET    0x1008U /*CFG_FILE*/

/**
* @def      APP_KEY_STRM_VLAN_PRIO_CLEAN
* @brief    Request to clear all VLAN entries
* @hideinitializer
*/
#define APP_KEY_STRM_VLAN_PRIO_CLEAN    0x1009U /*CFG_FILE*/

/**
* @def      APP_KEY_STRM_VLAN_PRIO_ADD
* @brief    Request to add VLAN priority value to identify traffic to be received as a video stream.
*           Frames without VLAN tag or with VLAN priority value not equal to this added value will
*           not be accepted.
* @hideinitializer
*/
#define APP_KEY_STRM_VLAN_PRIO_ADD      0x100AU /*CFG_FILE*/

/**
* @def      APP_KEY_STRM_DROP_OUT_THRESHOLD
* @brief    Number of iterations to tolerate while no stream data is being received. Used to
*           detect stream drop-outs.
* @hideinitializer
*/
#define APP_KEY_STRM_DROP_OUT_THRESHOLD 0x100BU /*CFG_FILE*/

/**
* @def      APP_KEY_STRM_STREAM_ID_0
* @brief    Stream ID value of AVB frames identifying packets for logical stream number 0
* @hideinitializer
*/
#define APP_KEY_STRM_STREAM_ID_0        0x1100U /*CFG_FILE*/

/**
* @def      APP_KEY_STRM_STREAM_ID_1
* @brief    Stream ID value of AVB frames identifying packets for logical stream number 1
* @hideinitializer
*/
#define APP_KEY_STRM_STREAM_ID_1        0x1101U /*CFG_FILE*/

/**
* @def      APP_KEY_STRM_STREAM_ID_2
* @brief    Stream ID value of AVB frames identifying packets for logical stream number 2
* @hideinitializer
*/
#define APP_KEY_STRM_STREAM_ID_2        0x1102U /*CFG_FILE*/

/**
* @def      APP_KEY_STRM_STREAM_ID_3
* @brief    Stream ID value of AVB frames identifying packets for logical stream number 3
* @hideinitializer
*/
#define APP_KEY_STRM_STREAM_ID_3        0x1103U /*CFG_FILE*/

/*  Ethernet Queue configuration */
/**
* @def      APP_KEY_ETHQ_BASE_ADDR
* @brief    Base address of the ENET peripheral to be used by the ETHQ module
* @hideinitializer
*/
#define APP_KEY_ETHQ_BASE_ADDR          0x2000U /*CFG_FILE*/

/**
* @def      APP_KEY_ETHQ_REGION_LENGTH
* @brief    Length of the ENET peripheral registers region to be used by the ETHQ module
* @hideinitializer
*/
#define APP_KEY_ETHQ_REGION_LENGTH      0x2001U /*CFG_FILE*/

/**
* @def      APP_KEY_ETHQ_SIZE_OF_BUFF
* @brief    Size of the Ethernet RX buffers region. This is the memory buffer used to store
*           received Ethernet frames.
* @hideinitializer
*/
#define APP_KEY_ETHQ_SIZE_OF_BUFF       0x2002U

/**
* @def      APP_KEY_ETHQ_BUFF_RING_PTR
* @brief    Starting address of the Ethernet RX buffers memory
* @hideinitializer
*/
#define APP_KEY_ETHQ_BUFF_RING_PTR      0x2003U

/*  Decoder Feeding configuration */
/**
* @def      APP_KEY_DEC_BASE_ADDR
* @brief    Base address of the desired HW decoder peripheral to be used by the DECFEED module
* @hideinitializer
*/
#define APP_KEY_DEC_BASE_ADDR           0x3000U /* Note: value passed from decoder driver, not from CFG file */

/**
* @def      APP_KEY_DEC_REGION_LENGTH
* @brief    Length of the HW decoder registers region to be used by the DECFEED module
* @hideinitializer
*/
#define APP_KEY_DEC_REGION_LENGTH       0x3001U /*CFG_FILE*/

/**
* @def      APP_KEY_DEC_FEED_AT_ONCE
* @brief    Configuration parameter specifying number of frames to be put into the decoder's input
*           FIFO within a single iteration. Can be used to prevent decoder feeding bursts.
* @hideinitializer
*/
#define APP_KEY_DEC_FEED_AT_ONCE        0x3002U /*CFG_FILE*/

/**
* @def      APP_KEY_SRAM_BUFFER
* @brief    Used to pass allocated SRAM buffer address from userspace to firmware.
* @hideinitializer
*/
#define APP_KEY_SRAM_BUFFER 0x4000U
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

#endif /* APPLICATION_CFG_H */
/** @}*/
