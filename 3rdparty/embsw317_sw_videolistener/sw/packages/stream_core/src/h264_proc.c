/**
  @addtogroup       dxgrSCORE
  @{

  @file             h264_proc.c
  @brief            H264 stream processor
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
* @section h264_proc_c_REF_1
* Violates MISRA 2004 Advisory Rule 11.3, A cast should not be preformed between a pointer type and
* an integral type. Cast is used to access memory mapped resources or due to simplified data
* representation.
*
* @section h264_proc_c_REF_2
* Violates MISRA 2004 Advisory Rule 11.4, A cast should not be performed between a pointer to object
* and a different pointer to object type.
* Cast from pointer to pointer is due to accessing memory as different data type.
*
*/

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "common_types.h"
#include "common.h"
#include "h264_proc.h"

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/
#define H264PROC_CFG_MAX_INSTANCES  4
#define H264PROC_CFG_SLOWDOWN_ON    (TRUE)

/*==================================================================================================
                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
typedef enum
{
    UT_CODED_SLICE_IDR_PICTURE  = 5,
    UT_SEQUENCE_PARAMTER_SET    = 7,
    UT_PICTURE_PARAMTER_SET     = 8,
    UT_END_OF_STREAM            = 11,
    UT_FU_A                     = 28U
} nal_unit_type_t;

typedef struct
{
    unsigned int Type   : 5;
    unsigned int NRI    : 2;
    unsigned int F      : 1;
} nal_unit_header_t;

typedef struct
{
    unsigned int Type   : 5;
    unsigned int R      : 1;
    unsigned int E      : 1;
    unsigned int S      : 1;
} f_unit_header_t;

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

/*==================================================================================================
                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/
/**
    @brief  Function indicates whether current packet is a leading packet of a h264 frame.
    @param[in] u8StreamIdx The stream identifier
    @param[in] u32PacketAddr Address of the h264 packet to be checked
    @retval H264PROC_E_AGAIN Start of frame not found
    @retval H264PROC_E_OK Start of frame found
    @retval H264PROC_E_UNSUPPORTED_UNIT Unsupported NAL unit detected
*/
h264_proc_ret_t H264PROC_IsStartOfFrame(const uint8_t u8StreamIdx, const uint32_t u32PacketAddr)
{
    h264_proc_ret_t eRetVal = H264PROC_E_AGAIN;
    /** @violates @ref h264_proc_c_REF_1 MISRA rule 11.3 */
    nal_unit_header_t *prNalHeader = (nal_unit_header_t *)(tuAddr)(u32PacketAddr);
    
    UNUSED(u8StreamIdx);
    
    /*  Search for start of image (synchronization point) */
    switch (prNalHeader->Type)
    {
        case UT_SEQUENCE_PARAMTER_SET:
        {
            /*  Found */
            eRetVal = H264PROC_E_OK;
            break;
        }
        case UT_PICTURE_PARAMTER_SET:
        case UT_END_OF_STREAM:
        case UT_FU_A:
        {
            break;
        }
        default:
        {
            eRetVal = H264PROC_E_UNSUPPORTED_UNIT;
            break;
        }
    }
    
    return eRetVal;
}

/**
    @brief      When data stream from sensor does not match input format of the HW decoder this
                function may be used to prepare data for the decoder in acceptable format.
    @details    As stream packets are flowing in, function is sequentially modifying headers and
                the StreamIdx information is used to distinguish between various stream instances.
    @param[in]  u8StreamIdx Identifier of stream the preprocessing is being performed for
    @param[in]  u32PacketAddr Pointer to the original h264 packet
    @param[in]  u16Length Length of the original packet
    @param[in]  pu32ProcPacketAddr Pointer to the preprocessed h264 packet
    @param[in]  pu16ProcLength Length of the preprocessed packet
    @retval     H264PROC_E_OK Success
    @retval     H264PROC_E_UNSUPPORTED_UNIT Unsupported NAL unit detected
*/
h264_proc_ret_t H264PROC_PreprocessPacket(const uint8_t u8StreamIdx, const uint32_t u32PacketAddr,
                                            const uint16_t u16Length, uint32_t *const pu32ProcPacketAddr,
                                                uint16_t *const pu16ProcLength)
{
    static boolean abIsFirstChunk[H264PROC_CFG_MAX_INSTANCES] = {FALSE};
    h264_proc_ret_t eRetVal = H264PROC_E_OK;
    uint32_t u32NewAddr = u32PacketAddr;
    uint16_t u16NewLength = u16Length;
#if (TRUE == H264PROC_CFG_SLOWDOWN_ON)
    uint32_t u32ii = 0U;
#endif /* H264PROC_CFG_SLOWDOWN_ON */
    
    /*  Insert NAL unit separator */
    /** @violates @ref h264_proc_c_REF_1 MISRA rule 11.3 */
    nal_unit_header_t *prNalHeader = (nal_unit_header_t *)((tuAddr)u32NewAddr);
    
    if ((UT_SEQUENCE_PARAMTER_SET == (nal_unit_type_t)(prNalHeader->Type)) \
            || (UT_PICTURE_PARAMTER_SET == (nal_unit_type_t)(prNalHeader->Type)))
    {
        /*  Insert the synchronization pattern */
        /** @violates @ref h264_proc_c_REF_1 MISRA rule 11.3 */
        *(uint8_t *)((tuAddr)u32NewAddr - 4U) = 0x00U;
        /** @violates @ref h264_proc_c_REF_1 MISRA rule 11.3 */
        *(uint8_t *)((tuAddr)u32NewAddr - 3U) = 0x00U;
        /** @violates @ref h264_proc_c_REF_1 MISRA rule 11.3 */
        *(uint8_t *)((tuAddr)u32NewAddr - 2U) = 0x00U;
        /** @violates @ref h264_proc_c_REF_1 MISRA rule 11.3 */
        *(uint8_t *)((tuAddr)u32NewAddr - 1U) = 0x01U;
        
        u32NewAddr -= 4U;
        u16NewLength = (uint16_t)(u16NewLength + 4U);
        
        /*  Data follows parameter sets so next FU_A should contain synchronization pattern */
        abIsFirstChunk[u8StreamIdx] = TRUE;
    }
    else if (UT_FU_A == (nal_unit_type_t)(prNalHeader->Type))
    {
        /** @violates @ref h264_proc_c_REF_1 MISRA rule 11.3 */
        f_unit_header_t *prFuHeader = (f_unit_header_t *)((tuAddr)u32NewAddr + 1UL);
        
        /*  Only leading chunk shall contain synchronization pattern and header */
        if (TRUE == abIsFirstChunk[u8StreamIdx])
        {
            /*  Rework the FU header */
            prNalHeader->Type = prFuHeader->Type;
            
            /*  Replace FU header with NAL unit header */
            /** @violates @ref h264_proc_c_REF_2 MISRA rule 11.4 */
            *((uint8_t *)(prFuHeader)) = *((uint8_t *)(prNalHeader));
            u32NewAddr++;
            u16NewLength--;
            
            /*  Insert the synchronization pattern */
            /** @violates @ref h264_proc_c_REF_1 MISRA rule 11.3 */
            *(uint8_t *)((tuAddr)u32NewAddr - 4UL) = 0x00U;
            /** @violates @ref h264_proc_c_REF_1 MISRA rule 11.3 */
            *(uint8_t *)((tuAddr)u32NewAddr - 3UL) = 0x00U;
            /** @violates @ref h264_proc_c_REF_1 MISRA rule 11.3 */
            *(uint8_t *)((tuAddr)u32NewAddr - 2UL) = 0x00U;
            /** @violates @ref h264_proc_c_REF_1 MISRA rule 11.3 */
            *(uint8_t *)((tuAddr)u32NewAddr - 1UL) = 0x01U;
            
            u32NewAddr -= 4U;
            u16NewLength = (uint16_t)(u16NewLength + 4U);
            abIsFirstChunk[u8StreamIdx] = FALSE;
        }
        else
        {
            /*  Skip all headers */
            u32NewAddr += 2U;
            u16NewLength = (uint16_t)(u16NewLength - 2U);
        }
    }
    else
    {
        eRetVal = H264PROC_E_UNSUPPORTED_UNIT;
    }
    
    *pu32ProcPacketAddr = u32NewAddr;
    *pu16ProcLength = u16NewLength;

#if (TRUE == H264PROC_CFG_SLOWDOWN_ON)
    /*  Slow the feeding down - sequencer is crying "Buffer overruns" when
        a single stream is being converted */
    /*  In case when 4 streams are being fed, this feature shall be turned
        off to prevent ethernet queue overflows */
    /*  TODO: Remove this when the sequencer will become fast enough.... */
    for (u32ii=0U; u32ii<(100000U/4U); u32ii++)
    {
        __asm("nop");
    }
#endif /* H264PROC_CFG_SLOWDOWN_ON */
    
    return eRetVal;
}
/** @}*/
