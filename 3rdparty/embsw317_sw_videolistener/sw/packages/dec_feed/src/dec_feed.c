/**
  @defgroup         dxgrDECFEED Decoder Feeding Driver
  @details          The Decoder Feeding module is intended to act as a JPEG/H264 decoder input
                    handling driver. It manages input queue of the decoder and provides interface
                    allowing user to enqueue frames to be decoded. Module also contains auxiliary
                    API enabling access to current queue and error status of the peripheral.
                    
                    Since only input of the decoder is being handled within this module it needs
                    to be used within an environment where a complete decoder driver takes care of
                    decoder's configuration and output management.
                    
                    @note A supported decoder may contain multiple stream instances what means that
                          is able to process multiple separated data streams in parallel. The feeding
                          driver uses term "stream index" to enumerate particular HW instances.
                    
                    Theory of operation
                    -------------------
                    First of all the module needs to be correctly configured. Configuration is done
                    by setting values of configuration macros within the dec_feed_cfg.h to desired
                    values and providing runtime configuration via:
                    - DECFEED_SetBaseAddress()
                    - DECFEED_SetRegionLength()
                    - DECFEED_SetBurstLength()
                    - DECFEED_SetNumOfStreams()
                    
                    When properly configured, module needs to be initialized by @ref DECFEED_Init()
                    and started by @ref DECFEED_Start(). From now the user can enqueue frames
                    to be decoded via @ref DECFEED_Push(). Queue is being maintained by periodic
                    calls to the @ref DECFEED_HandleProcessedFrms() and user shall ensure that
                    the function is called often enough to keep the operation fluent.
                    
                    Module can be stopped by @ref DECFEED_Stop() and re-initialized by following
                    call to the @ref DECFEED_Init().

  @addtogroup       dxgrDECFEED
  @{

  @file             dec_feed.c
  @brief            The decoder feeding driver
  @details          This driver handles only the input of the decoder. It cooperates with Linux
                    driver which handles the rest, mainly initialization and output of the decoder.

  Project           M4_ETH_PROJECT_NAME

  Platform          M4_ETH_PLATFORM

  SWVersion         M4_ETH_RELEASE_VERSION

  Copyright (c) 2016 NXP Semiconductors
  All Rights Reserved.
*/
/*==================================================================================================
==================================================================================================*/

#ifdef __cplusplus
extern "C"{
#endif
/*==================================================================================================
                                         MISRA VIOLATIONS
==================================================================================================*/
/*
* @page misra_violations MISRA-C:2004 violations
*
* @section dec_feed_c_REF_1
* Violates MISRA 2004 Advisory Rule 11.3, A cast should not be preformed between a pointer type and
* an integral type. Cast is used to access memory mapped resources.
*/

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "common_types.h"
#include "dec_feed_cfg.h"
#include "dec_feed.h"

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
#if !defined (DECFEED_CFG_INIT_CHECK)
    #error Please define the DECFEED_CFG_INIT_CHECK value
#endif

/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/
/** @cond dxINTERNAL_MACROS */
/** @endcond */

/*==================================================================================================
                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
/*
* @struct   DECFEED_trCfg
* @brief    Decoder feeding driver configuration data structure
* @details  It is used to communicate and store configuration parameters to streaming application.
*           In function Configure data are being copied from shared structure to local structure
*           because data in shared structure may change on the run by external software.
*/
typedef struct
{
    uint32_t u32DecoderBaseAddr;
    uint32_t u32DecoderRegionLength;
    uint8_t  u8FeedAtOnce;
    uint8_t  u8NumberOfStreams;
    boolean  bInitialized;
} DECFEED_trCfg;

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
/*
* @brief    Configuration of Decoder Feed driver
* @details  This structure must be written by the Streaming Application before any DECFEED function
*           is run. Otherwise the behaviour is not defined.
*/
static DECFEED_trCfg DECFEED_rCfg = {0U, 0U, DECFEED_CFG_FIFO_LENGTH, 0U, FALSE};
static uint32_t DECFEED_au32Queued[DEC_FEED_CFG_MAX_STREAMS];
static uint32_t DECFEED_au32Freed[DEC_FEED_CFG_MAX_STREAMS];

/*==================================================================================================
                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
#if (DECODER_H264 == DECFEED_CFG_DECODER_VARIANT)
static uint8_t H264DEC_GetFreeEntriesCnt(const uint8_t u8StreamIdx);
#endif /* DECFEED_CFG_DECODER_VARIANT */

/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/
#if (DECODER_H264 == DECFEED_CFG_DECODER_VARIANT)
static uint8_t H264DEC_GetFreeEntriesCnt(const uint8_t u8StreamIdx)
{
    uint32_t u32RegVal;
    uint8_t u8RetVal;
    
    if (u8StreamIdx <= 1U)
    {
        /** @violates @ref dec_feed_c_REF_1 MISRA rule 11.3 */
        u32RegVal = *(volatile uint32_t *)((tuAddr)DECFEED_rCfg.u32DecoderBaseAddr + H264_DEC_STR_01_PKT_FIFO_STAT);
    }
    else
    {
        /** @violates @ref dec_feed_c_REF_1 MISRA rule 11.3 */
        u32RegVal = *(volatile uint32_t *)((tuAddr)DECFEED_rCfg.u32DecoderBaseAddr + H264_DEC_STR_23_PKT_FIFO_STAT);
    }
    
    u8RetVal = (uint8_t)((u32RegVal >> ((u8StreamIdx & 0x1U) * 8U)) & 0xffU);
    u8RetVal = (uint8_t)(DECFEED_CFG_FIFO_LENGTH - u8RetVal);
    
    return u8RetVal;
}
#endif /* DECFEED_CFG_DECODER_VARIANT */

/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/
/**
* @brief        Function checks module configuration for typical errors.
* @retval       DECFEED_E_INVALID_CONFIG Error - At least one parameter has wrong value.
* @retval       DECFEED_E_OK Success.
*/
dec_feed_ret_t DECFEED_CheckConfig(void)
{
    dec_feed_ret_t eRetVal = DECFEED_E_OK;
    
    if (0U == DECFEED_rCfg.u32DecoderBaseAddr)
    {
        eRetVal = DECFEED_E_INVALID_CONFIG;
    }
    else if (0U == DECFEED_rCfg.u32DecoderRegionLength)
    {
        eRetVal = DECFEED_E_INVALID_CONFIG;
    }
    else if (0U == DECFEED_rCfg.u8NumberOfStreams)
    {
        eRetVal = DECFEED_E_INVALID_CONFIG;
    }
    else
    {
        ;   /*  Intentionally empty */
    }
    
    return eRetVal;
}

/**
* @brief        Configure base address of decoder register region.
* @param[in]    u32Addr Physical base address.
* @retval       DECFEED_E_MULTIPLE_CONFIG Error - attempt to change the address.
* @retval       DECFEED_E_OK Success.
* @note         Once the address is set, it may not be changed.
*/
dec_feed_ret_t DECFEED_SetBaseAddress(uint32_t u32Addr)
{
    dec_feed_ret_t eRetVal = DECFEED_E_MULTIPLE_CONFIG;
    
    if ((0U == DECFEED_rCfg.u32DecoderBaseAddr) || (DECFEED_rCfg.u32DecoderBaseAddr == u32Addr))
    {
        DECFEED_rCfg.u32DecoderBaseAddr = u32Addr;
        eRetVal = DECFEED_E_OK;
    }
    
    return eRetVal;
}

/**
* @brief        Configure length of decoder register region.
* @param[in]    u32Length Length in bytes.
* @retval       DECFEED_E_MULTIPLE_CONFIG Error - attempt to change the length.
* @retval       DECFEED_E_OK Success.
* @note         Once the length is set, it may not be changed.
*/
dec_feed_ret_t DECFEED_SetRegionLength(uint32_t u32Length)
{
    dec_feed_ret_t eRetVal = DECFEED_E_MULTIPLE_CONFIG;
    
    if ((0U == DECFEED_rCfg.u32DecoderRegionLength) || (DECFEED_rCfg.u32DecoderRegionLength == u32Length))
    {
        DECFEED_rCfg.u32DecoderRegionLength = u32Length;
        eRetVal = DECFEED_E_OK;
    }
    
    return eRetVal;
}

/**
* @brief        Read configured base address and length of decoder register region.
* @details      It also checks validity of the configuration. Values are provided
*               only if the check passed..
* @param[out]   pu32Base Register region base address will be written here.
* @param[out]   pu32Length Register region length will be written here.
* @return       If configuration check fails, then return value from DECFEED_CheckConfig is
*               passed. Otherwise this function returns DECFEED_E_OK.
*/
dec_feed_ret_t DECFEED_GetMemoryBase(uint32_t *pu32Base, uint32_t *pu32Length)
{
    dec_feed_ret_t eRetVal = DECFEED_CheckConfig();
    
    if (DECFEED_E_OK == eRetVal)
    {
        *pu32Base = DECFEED_rCfg.u32DecoderBaseAddr;
        *pu32Length = DECFEED_rCfg.u32DecoderRegionLength;
    }
    
    return eRetVal;
}

/**
* @brief        Configure Burst Length.
* @param[in]    u8Count Maximal number of Ethernet frames that may be passed to decoder
*               in one application iteration.
* @retval       DECFEED_E_OK Success.
*/
dec_feed_ret_t DECFEED_SetBurstLength(uint8_t u8Count)
{
    DECFEED_rCfg.u8FeedAtOnce = u8Count;
    
    return DECFEED_E_OK;
}

/**
* @brief        Read configured Burst Length.
* @param[in]    pu8Count Pointer to variable where result shall be written.
* @retval       DECFEED_E_OK Success.
*/
dec_feed_ret_t DECFEED_GetBurstLength(uint8_t *pu8Count)
{
    *pu8Count = DECFEED_rCfg.u8FeedAtOnce;
    
    return DECFEED_E_OK;
}

/**
* @brief        Get current error status of decoder hardware.
* @details      This function shall be used to periodically check HW error flags.
*               DEC_FEED module must be initialized before this function is called.
* @param[out]   pu32StatusReg Hardware specific error mask will be written here. Zero means no error.
* @retval       DECFEED_E_NOT_INITIALIZED Unable to read the status due to missing configuration.
* @retval       DECFEED_E_OK Success.
*/
dec_feed_ret_t DECFEED_GetDecoderStatus(uint32_t *pu32StatusReg)
{
    dec_feed_ret_t eRetVal = DECFEED_E_OK;
    
#if (TRUE == DECFEED_CFG_INIT_CHECK)
    if (FALSE == DECFEED_rCfg.bInitialized)
    {
        eRetVal = DECFEED_E_NOT_INITIALIZED;
    }
    else
#endif /* DECFEED_CFG_INIT_CHECK */
    {
#if (DECODER_JPEG == DECFEED_CFG_DECODER_VARIANT)
        /** @violates @ref dec_feed_c_REF_1 MISRA rule 11.3 */
        *pu32StatusReg = *(volatile uint32_t*)((tuAddr)DECFEED_rCfg.u32DecoderBaseAddr + DEC_STATUS_OFFSET);
#elif (DECODER_H264 == DECFEED_CFG_DECODER_VARIANT)
        /*  Get TimeOut status for each stream (pu32StatusReg[0..3]) */
        /** @violates @ref dec_feed_c_REF_1 MISRA rule 11.3 */
        *pu32StatusReg = *(volatile uint32_t *)((tuAddr)DECFEED_rCfg.u32DecoderBaseAddr + H264_DEC_STAT);
        /*  Get error request interrupt flag (pu32StatusReg[31]) */
        /** @violates @ref dec_feed_c_REF_1 MISRA rule 11.3 */
        *pu32StatusReg |= \
            *(volatile uint32_t *)((tuAddr)DECFEED_rCfg.u32DecoderBaseAddr + H264_DEC_ISR) & 0x80000000U;
#endif /* DECFEED_CFG_DECODER_VARIANT */
    }
    
    return eRetVal;
}

/**
* @brief        Gets number of buffers which may be passed to the decoder.
* @details      This function calculates free space based on number of buffers that were passed into 
*               the decoder and number of processed buffers that were freed by function
*               DECFEED_HandleProcessedFrms. 
* @param[in]    u8StreamIdx Selects stream to calculate with.
* @param[out]   pu32FreeEntriesNum Number of free entries will be written here.
* @retval       DECFEED_E_NOT_INITIALIZED Error - module was not properly initialized.
* @retval       DECFEED_E_INVALID_PARAMETER Error - invalid stream index.
* @retval       DECFEED_E_OK Success.
*/
dec_feed_ret_t DECFEED_GetFreeSpace(uint8_t u8StreamIdx, uint32_t *pu32FreeEntriesNum)
{
    dec_feed_ret_t eRetVal = DECFEED_E_OK;
    
#if (TRUE == DECFEED_CFG_INIT_CHECK)
    if (FALSE == DECFEED_rCfg.bInitialized)
    {
        eRetVal = DECFEED_E_NOT_INITIALIZED;
    }
    else
#endif /* DECFEED_CFG_INIT_CHECK */
    {
        if (u8StreamIdx >= DECFEED_rCfg.u8NumberOfStreams)
        {
            eRetVal = DECFEED_E_INVALID_PARAMETER;
        }
        else
        {
            *pu32FreeEntriesNum = DECFEED_CFG_FIFO_LENGTH-(DECFEED_au32Queued[u8StreamIdx]-DECFEED_au32Freed[u8StreamIdx]);
        }
    }
    
    return eRetVal;
}

/**
* @brief        Configure number of streams to work with.
* @param[in]    u8NumberOfStreams Number of streams.
* @retval       DECFEED_E_INVALID_CONFIG Error - unsupported number of streams.
* @retval       DECFEED_E_OK Success.
*/
dec_feed_ret_t DECFEED_SetNumOfStreams(uint8_t u8NumberOfStreams)
{
    dec_feed_ret_t eRetVal = DECFEED_E_OK;
    
    if ((u8NumberOfStreams > DEC_FEED_CFG_MAX_STREAMS) || (0U == u8NumberOfStreams))
    {
        eRetVal = DECFEED_E_INVALID_CONFIG;
    }
    else
    {
        DECFEED_rCfg.u8NumberOfStreams = u8NumberOfStreams;
    }
    
    return eRetVal;
}

/**
* @brief        Processing of number of processed frames.
* @details      This function calculates number of frames that HW decoder processed since last call
*               and updates counter of freed frames (side effect).
* @param[in]    u8StreamIdx Selects stream to work with.
* @param[out]   pu32ProcessedNum Number of buffers processed by HW decoder since last call will be written here.
* @retval       DECFEED_E_NOT_INITIALIZED Error - module was not properly initialized.
* @retval       DECFEED_E_INVALID_PARAMETER Error - invalid stream index.
* @retval       DECFEED_E_FIFO_ERROR Error - Number of processed frames seems to be negative. Unexpected state.
* @retval       DECFEED_E_OK Success.
*/
dec_feed_ret_t DECFEED_HandleProcessedFrms(uint8_t u8StreamIdx, uint32_t *pu32ProcessedNum)
{
    uint32_t u32InFifo;
    uint32_t u32Processed;
    uint8_t u8FifoFreeSpace;
    dec_feed_ret_t eRetVal = DECFEED_E_OK;
    
#if (TRUE == DECFEED_CFG_INIT_CHECK)
    if (FALSE == DECFEED_rCfg.bInitialized)
    {
        eRetVal = DECFEED_E_NOT_INITIALIZED;
    }
    else
#endif /* DECFEED_CFG_INIT_CHECK */
    {
        if (u8StreamIdx >= DECFEED_rCfg.u8NumberOfStreams)
        {
            eRetVal = DECFEED_E_INVALID_PARAMETER;
        }
        else
        {
#if (DECODER_JPEG == DECFEED_CFG_DECODER_VARIANT)
            /** @violates @ref dec_feed_c_REF_1 MISRA rule 11.3 */
            u8FifoFreeSpace = (uint8_t)JPEG_W_STATUS_PKT_FIFO_STATE(u8StreamIdx);
#elif (DECODER_H264 == DECFEED_CFG_DECODER_VARIANT)
            u8FifoFreeSpace = H264DEC_GetFreeEntriesCnt(u8StreamIdx);
#endif /* DECFEED_CFG_DECODER_VARIANT */
            u32InFifo = DECFEED_au32Queued[u8StreamIdx] - DECFEED_au32Freed[u8StreamIdx];
            if (u8FifoFreeSpace < (DECFEED_CFG_FIFO_LENGTH - u32InFifo))
            {
                eRetVal = DECFEED_E_FIFO_ERROR;
                *pu32ProcessedNum = 0U;
            }
            else
            {
                u32Processed = u8FifoFreeSpace - (DECFEED_CFG_FIFO_LENGTH - u32InFifo); /* RealFreeSpace - PreviousFreeSpace */
                /* The processed frames will be freed */
                DECFEED_au32Freed[u8StreamIdx] += u32Processed;
                *pu32ProcessedNum = u32Processed;
            }
        }
    }

    return eRetVal;
}

/**
* @brief        Push one buffer to decoder HW queue.
* @param[in]    u8StreamIdx Selects stream to work with.
* @param[in]    u32Addr Address of the buffer.
* @param[in]    u16Length Length of the buffer in bytes.
* @retval       DECFEED_E_NOT_INITIALIZED Error - module was not properly initialized.
* @retval       DECFEED_E_INVALID_PARAMETER Error - invalid stream index.
* @retval       DECFEED_E_FIFO_FULL Error - There is no free space in the FIFO.
* @retval       DECFEED_E_OK Success.
*/
dec_feed_ret_t DECFEED_Push(uint8_t u8StreamIdx, uint32_t u32Addr, uint16_t u16Length)
{
    dec_feed_ret_t eRetVal = DECFEED_E_OK;
    
#if (TRUE == DECFEED_CFG_INIT_CHECK)
    if (FALSE == DECFEED_rCfg.bInitialized)
    {
        eRetVal = DECFEED_E_NOT_INITIALIZED;
    }
    else
#endif /* DECFEED_CFG_INIT_CHECK */
    {
        if (u8StreamIdx >= DECFEED_rCfg.u8NumberOfStreams)
        {
            eRetVal = DECFEED_E_INVALID_PARAMETER;
        }
        else
        {
#if (DECODER_JPEG == DECFEED_CFG_DECODER_VARIANT)
            /** @violates @ref dec_feed_c_REF_1 MISRA rule 11.3 */
            if (JPEG_W_STATUS_PKT_FIFO_STATE(u8StreamIdx) > 0U)
            {
                /** @violates @ref dec_feed_c_REF_1 MISRA rule 11.3 */
                JPEG_STn_SRAM_PTR(u8StreamIdx) = u32Addr;
                /** @violates @ref dec_feed_c_REF_1 MISRA rule 11.3 */
                JPEG_STn_SRAM_LEN_VAL(u8StreamIdx) = u16Length;
                DECFEED_au32Queued[u8StreamIdx]++;
            }
            else
            {
                eRetVal = DECFEED_E_FIFO_FULL;
            }
#elif (DECODER_H264 == DECFEED_CFG_DECODER_VARIANT)
            if (H264DEC_GetFreeEntriesCnt(u8StreamIdx) > 0U)
            {
                uint32_t u32RegVal = (uint32_t)(u16Length & 0x7fffUL);
                
                u32RegVal |= (uint32_t)((u8StreamIdx & 0x3UL) << 30);
                /** @violates @ref dec_feed_c_REF_1 MISRA rule 11.3 */
                *(volatile uint32_t*) \
                    ((tuAddr)DECFEED_rCfg.u32DecoderBaseAddr + H264_DEC_STR_PKT_ADDR) = u32Addr;
                /** @violates @ref dec_feed_c_REF_1 MISRA rule 11.3 */
                *(volatile uint32_t*) \
                    ((tuAddr)DECFEED_rCfg.u32DecoderBaseAddr + H264_DEC_STR_TRIG_PKT_CFG) = u32RegVal;
                DECFEED_au32Queued[u8StreamIdx]++;
            }
            else
            {
                eRetVal = DECFEED_E_FIFO_FULL;
            }
#endif /* DECFEED_CFG_DECODER_VARIANT */
        }
    }
    
    return eRetVal;
}

/**
* @brief        Module initialization.
* @details      This function checks configuration of the module and resets queue counters.
* @return       On success it returns DECFEED_E_OK, otherwise return value from DECFEED_CheckConfig is passed.
* @note         The decoder HW is not restarted here. It must be done from Linux side, from VSDK.
*/
dec_feed_ret_t DECFEED_Init(void)
{
    uint8_t  u8StreamIdx;
    dec_feed_ret_t eRetVal;
    
    eRetVal = DECFEED_CheckConfig();
    
    if (DECFEED_E_OK == eRetVal)
    {
        /* Reset counters */
        for(u8StreamIdx = 0U; u8StreamIdx < DECFEED_rCfg.u8NumberOfStreams; u8StreamIdx++)
        {
            DECFEED_au32Queued[u8StreamIdx] = 0U;
            DECFEED_au32Freed[u8StreamIdx] = 0U;
        }   /* TODO: make sure no frame remained in the FIFO after last run */

        DECFEED_rCfg.bInitialized = TRUE;
    }
    
    return eRetVal;
}

/**
* @brief        Start feeding decoder.
* @details      Currently this function just checks whether module is initialized.
* @retval       DECFEED_E_NOT_INITIALIZED Error - module was not properly initialized.
* @retval       DECFEED_E_OK Success.
*/
dec_feed_ret_t DECFEED_Start(void)
{
    dec_feed_ret_t eRetVal = DECFEED_E_OK;
    
#if (TRUE == DECFEED_CFG_INIT_CHECK)
    if (FALSE == DECFEED_rCfg.bInitialized)
    {
        eRetVal = DECFEED_E_NOT_INITIALIZED;
    }
    else
#endif /* DECFEED_CFG_INIT_CHECK */
    {
        /* TODO: shall this function activate the HW? */
    }
    
    return eRetVal;
}

/**
* @brief        Stop feeding decoder.
* @details      Currently this function just checks whether module is initialized and clears this flag,
*               so then DECFEED_Init must be called before the module is started again.
* @retval       DECFEED_E_NOT_INITIALIZED Error - module was not properly initialized.
* @retval       DECFEED_E_OK Success.
*/
dec_feed_ret_t DECFEED_Stop(void)
{
    dec_feed_ret_t eRetVal = DECFEED_E_OK;
    
#if (TRUE == DECFEED_CFG_INIT_CHECK)
    if (FALSE == DECFEED_rCfg.bInitialized)
    {
        eRetVal = DECFEED_E_NOT_INITIALIZED;
    }
    else
#endif /* DECFEED_CFG_INIT_CHECK */
    {
        /* TODO: shall this function deactivate the HW? */
        DECFEED_rCfg.bInitialized = FALSE;
    }
    
    return eRetVal;
}

/*================================================================================================*/
#ifdef __cplusplus
}
#endif
/** @}*/
