/**
  @defgroup         dxgrSCORE Streaming Core
  @details          The Streaming Core module forms a VideoListener application core connecting
                    all underlying modules and provides all the streaming functionality of the
                    VideoListener firmware. Main purpose is to create a bridge between networking
                    peripheral and a chosen hardware decoder to transfer received video data
                    belonging to a logical stream to the input of HW decoder peripheral.
                    
                    Module uses the @ref dxgrETHQ to access received data, @ref dxgrDECFEED to
                    communicate with decoder and @ref dxgrMMU to access and configure memory subsystem.
                    It is state-based SW optimized to be run within a periodically executed context.
                    
                    Theory of operation
                    -------------------
                    Basic configuration of the module is done via the stream_core_cfg.h header which
                    provides configuration options allowing to change stream type or size of internal
                    Ethernet frames storage.
                    When the static configuration is done, user needs to provide the runtime parameters
                    using the @ref SCORE_SetConf() API. Note that many of runtime parameters are
                    required and without setting all the values the module can't be started.
                    Following parameter values need to be set up via the @ref SCORE_SetConf():
                    - APP_KEY_STRM_FETCH_THRESHOLD
                    - APP_KEY_STRM_SOI_MARK_VALUE
                    - APP_KEY_STRM_SOI_MARK_OFFSET
                    - APP_KEY_STRM_SOI_MARK_MASK
                    - APP_KEY_STRM_SEQ_NUM_OFFSET
                    - APP_KEY_STRM_STRM_ID_OFFSET
                    - APP_KEY_STRM_NUMBER_OF_STRMS
                    - APP_KEY_STRM_FRM_DATA_OFFSET
                    - APP_KEY_STRM_STREAM_ID_0
                    - APP_KEY_STRM_STREAM_ID_1
                    - APP_KEY_STRM_STREAM_ID_2
                    - APP_KEY_STRM_STREAM_ID_3
                    - APP_KEY_STRM_VLAN_PRIO_ADD
                    - APP_KEY_STRM_DROP_OUT_THRESHOLD
                    - APP_KEY_ETHQ_BASE_ADDR
                    - APP_KEY_ETHQ_REGION_LENGTH
                    - APP_KEY_ETHQ_SIZE_OF_BUFF
                    - APP_KEY_ETHQ_BUFF_RING_PTR
                    - APP_KEY_DEC_BASE_ADDR
                    - APP_KEY_DEC_REGION_LENGTH
                    
                    Once configured, module must be initialized by the @ref SCORE_Init(). The
                    initialization function checks configuration and returns values different from
                    SCORE_E_OK if an error has been detected. In such case the configuration must
                    be completed/updated until initialization is executed correctly.
                    
                    Module is started by @ref SCORE_Start(). The call changes state of the module
                    which enters the SCORE_ST_PRERUN state. From now the module is driven by
                    execution of the @ref SCORE_Iteration() API. This function is intended to by the
                    body of the module and user shall ensure its periodic calls within a non-preemptive
                    context.
                    
                    Runtime errors are stored internally and are accessible via @ref SCORE_GetCurrentState()
                    and @ref SCORE_GetErrorMask() API. Both function can be called after each iteration
                    (the @ref SCORE_Iteration() call) to verify if no runtime error has occurred.

  @addtogroup       dxgrSCORE
  @{

  @file             stream_core.c
  @brief            Core of the M4_ETH_PROJECT_NAME Streaming Application running in FIQ
  @details

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
* @section stream_core_c_REF_1
* Violates MISRA 2004 Required Rule 1.2, No reliance shall be placed on undefined or unspecified
* behaviour.
* Address of near auto variable is needed to locate data by a sub-routine or to return
* a value via pointer.
*
* @section stream_core_c_REF_2
* Violates MISRA 2004 Required Rule 14.7, A function shall have a single point of exit at the end
* of the function.
* Multiple exit points here decreases code complexity and improves readability.
*
* @section stream_core_c_REF_3
* Violates MISRA 2004 Advisory Rule 11.3, A cast should not be preformed between a pointer type and
* an integral type. Cast is used to access memory mapped resources.
*
* @section stream_core_c_REF_4
* Violates MISRA 2004 Advisory Rule 11.4, A cast should not be performed between a pointer to object
* and a different pointer to object type.
* Cast from pointer to pointer is due to accessing a part of byte array as 4-byte wide value.
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
#include "sm_cfg.h"
#include "sm.h"
#if (TRUE == SM_CFG_ENABLE_MMU)
    #include "mmu_mem_attr.h"
    #include "mmu_cfg.h"
    #include "mmu.h"
#endif /* SM_CFG_ENABLE_MMU */
#include "mmap.h"
#include "application_cfg.h" /* Static application configuration */
#include "eth_queue_cfg.h"
#include "dec_feed_cfg.h"
#include "stream_core_cfg.h"
#include "stream_core.h"
#if (H264_OVER_AVB == SCORE_CFG_STREAM_TYPE)
    #include "h264_proc.h"
#endif /* SCORE_CFG_STREAM_TYPE */
#include "eth_queue.h"       /* Ethernet Rx Queue driver */
#include "dec_feed.h"        /* Video Decoder Input driver */
#include "debug.h"           /* Debugging messages driver */

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
#if !defined (SCORE_CFG_STREAM_TYPE)
    #error StreamCore: Please specify the SCORE_CFG_STREAM_TYPE value
#elif (H264_OVER_AVB == SCORE_CFG_STREAM_TYPE)
    #if !defined (SCORE_PREPROCESS_H264_STREAM)
        #error StreamCore: Please specify the SCORE_PREPROCESS_H264_STREAM value
    #endif
#else /* Not H264 */
    #if TRUE == APP_CFG_ZERO_BUF_ENABLED
        #error Macro APP_CFG_ZERO_BUF_ENABLED may be used only for H264 streams
    #endif
#endif

/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/
#define SC_CFG_DEV_ERROR_DETECTION (FALSE)

/*==================================================================================================
                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
/*
* @struct   SCORE_trCtl
* @brief    Structure for state informations of streaming core.
*/
typedef struct
{
    SCORE_tenState enState;      /* < Current state of streaming application. */
    uint32_t u32ErrMask;         /* < Mask of errors encountered in streaming application since last (re)configuration. */
                                 /* FIXME: now the reconfiguration does not clear this mask */
} SCORE_trCtl;

/*
* @struct   SCORE_trStreamCfg
* @brief    Stream configuration data structure
* @details  It is used to communicate and store configuration parameters to streaming application.
*           In function Configure data are being copied from shared structure to local structure
*           because data in shared structure may change on the run by external software.
*/
typedef struct
{
    uint16_t u16FetchSOIThreshold;  /* < Maximal number of frames stored after SOI in FETCH state, then buffer is freed */
    uint32_t u32SOIMarkerValue;     /* < Specify StartOfImage marker */
    uint32_t u32SOIMarkerMask;      /* < Mask of the StartOfImage marker within the 32 bits */
    uint8_t  u8SOIMarkerOffset;     /* < Offset of the StartOfImage marker within Ethernet frame */
    uint8_t  u8SequenceNumOffset;   /* < Offset of the Sequence Number within Ethernet frame */
    uint8_t  u8StreamIDOffset;      /* < Offset of the Stream ID within Ethernet frame */
    uint8_t  u8FrmDataOffset;       /* < Offset of the Frame Data (payload) within Ethernet frame */
    uint8_t  u8NumberOfStreams;     /* < Requested number of streams */
    uint8_t  au8VlanPrio[4];        /* < Configuration of (at most four) VLAN priorities used by streams */
    uint8_t  u8VlanPrioCnt;         /* < Number of allowed VLAN priorities */
    uint8_t  u8DropOutThreshold;    /* < Number of iterations to tolerate no Rx data in stream */
    struct
    {
        uint64_t u64StreamID;       /* < StreamID (AVB frames) */
        /* Potentially add other stream specific configuration data */
    } arStream[APP_CFG_MAX_STREAMS]; /* < Configuration specific for each stream */
} SCORE_trStreamCfg;

typedef struct
{
    boolean bFetchSOIThresholdSet;
    boolean bSOIMarkerValueSet;
    boolean bSOIMarkerMaskSet;
    boolean bSOIMarkerOffsetSet;
    boolean bSequenceNumOffsetSet;
    boolean bStreamIDOffsetSet;
    boolean bFrmDataOffsetSet;
    boolean bNumberOfStreamsSet;
    boolean abVlanPrioSet[4];
    boolean bVlanPrioCntSet;
    boolean abStreamIdSet[APP_CFG_MAX_STREAMS];
    boolean bDropOutThresholdSet;
} SCORE_rStreamCfgFlags_t;

typedef struct
{
    uint32_t    u32FrameAddr;
    uint16_t    u16DataLength;
    uint32_t    u32AVBTimestamp;
    uint8_t     u8SeqNumber;
    boolean     bAVBTimestampValid;
    boolean     bStartOfImage;
} trStreamBD;

typedef struct
{
    uint16_t u16WrIdx;      /* < Index of next BD to be written (passed from Ethernet) */
    uint16_t u16PassIdx;    /* < Index of next BD to be passed to decoder queue */
    uint16_t u16FreeIdx;    /* < Index of next BD to be freed (the data are not needed any more) */
    uint16_t u16Occupied;   /* < Used to distinguish empty, non-empty, and full queue */
    uint32_t u32LastRxIter; /* < Number of iteration when last frame was received */
} trStreamBDRingTrack;

/*==================================================================================================
                                       LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       LOCAL VARIABLES
==================================================================================================*/
/* Sequence number of next expected frame for each stream */
static uint8_t  au8SeqNumber[APP_CFG_MAX_STREAMS];
/* Count-down of SOI frame validity (used in FETCH state) */
static uint16_t au16StreamSOICountdown[APP_CFG_MAX_STREAMS];

/* Stream BD queues */
static trStreamBD aarStreamBD[APP_CFG_MAX_STREAMS][SCORE_CFG_STREAM_BD_RING_LEN];
/* Current indexes to stream BD queues */
static trStreamBDRingTrack arStreamBDRingTrack[APP_CFG_MAX_STREAMS];

#if TRUE == SCORE_CFG_PERMANENT_SOI_SYNC
    /*  Used for permanent SOI synchronization.
        Negative value means no wait.
        Positive value is the index in the stream queue of first frame that shall wait.
    */
    static int32_t as32StreamSOIWaitIdx[APP_CFG_MAX_STREAMS];
    static int32_t as32StreamSOIWaitIdxBuf[APP_CFG_MAX_STREAMS];
#endif /* SCORE_CFG_PERMANENT_SOI_SYNC */

#if TRUE == APP_CFG_ZERO_BUF_ENABLED
    static uint32_t u32ZeroBufAddr;
#endif /* APP_CFG_ZERO_BUF_ENABLED */

/* Forces (re)configuration before start */
static boolean bConfigHasChanged = TRUE;

/* Forces running init before each start */
static boolean bInitialized = FALSE;

/* Structures for configuration data exchange. They are shared through callback.
   Access them only in the callback and function Configure */
static SCORE_trStreamCfg  rStreamCfgPrepare = {0U};
static SCORE_rStreamCfgFlags_t rStreamCfgFlags = {FALSE};

/* Copy of configuration that shall be used after function Configure is called */
static SCORE_trStreamCfg  SCORE_rStrmCfg = {0U};

/* Counter of iterations of streaming core. Starts from 0; is never reset; may overlap. */
static uint32_t u32IterCtr = 0U;
#define ZDDEBUG
#ifdef ZDDEBUG
    /* Debugging counters */
    static uint32_t u32EthToStrm = 0U, u32StrmToDec = 0U, u32TotalFrames = 0U;
    static uint32_t u32KownStreamID = 0U, u32SOI = 0U, u32NotSOI = 0U;
    static uint32_t u32ProcessedCtr = 0U;
#endif

/* Current state of streaming core */
static SCORE_trCtl SCORE_rCtl =
{
    .enState = SCORE_ST_READY,
    .u32ErrMask = 0U,
};

/*==================================================================================================
                                       GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
static void     FetchIteration(void);
static void     RunIteration(void);
static void     FreeProcessedStreamBDs(void);
static void     PassBDsFromEthToStream(void);
static void     PassBDsFromStreamToDEC(void);
static boolean  FetchDecision(uint8_t u8StreamIdx, boolean bStartOfImage, uint8_t u8FrameSeqNumber);
static void     EmptyStreamQueue(uint8_t u8StreamIdx);
static stream_core_ret_t Configure(void);
#ifdef ZDDEBUG
static void     PrintDebugMsgs(void);
#endif

/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/
#ifdef ZDDEBUG
static void PrintDebugMsgs(void)
{
    boolean bForcePrint = FALSE;
    static SCORE_tenState enPrevState = SCORE_ST_READY;

    /* Print state transitions (not forced) */
    if(SCORE_rCtl.enState != enPrevState)
    {
        char_t *szStateName="UNKNOWN";
        enPrevState = SCORE_rCtl.enState;
        bForcePrint = TRUE;
        switch(SCORE_rCtl.enState)
        {
            case SCORE_ST_READY:
                szStateName="READY==";
                break;
            case SCORE_ST_FETCH:
                szStateName="FETCH==";
                break;
            case SCORE_ST_RUN:
                szStateName="RUN====";
                break;
            case SCORE_ST_PRERUN:
                szStateName="PRERUN=";
                break;
            default:
                szStateName="ERROR==";
                break;
        }
        INFO("===%s===\n", szStateName);
    }
    /* Print counters */
    if ((TRUE == bForcePrint) || ((0U != u32IterCtr) && (0U == (u32IterCtr%600U))))
    {
        INFO("Iter=%5u; TotFrm=%5u; StreamID_OK=%5u; SOI/Not=%5u/%5u; "
             "Eth->Strm=%5u; Strm->Dec=%5u; Decoded=%5u;\n",
             u32IterCtr, u32TotalFrames, u32KownStreamID, u32SOI, u32NotSOI,
             u32EthToStrm, u32StrmToDec, u32ProcessedCtr
            );
    }
}
#endif

static void FetchIteration(void)
{
    /* Pass all available frames from Eth BDs to Stream BDs while looking for SOI
       and discarding all BDs in streams with old/without SOI. */
    PassBDsFromEthToStream();
}

static void RunIteration(void)
{
    /* Free BDs already processed by decoder DMA from stream queues */
    FreeProcessedStreamBDs();
    /* Pass all new BDs from Eth to stream queues */
    PassBDsFromEthToStream();
    /* Pass BDs from streams to decoder's DMA queues */
    PassBDsFromStreamToDEC();
}

static void PassBDsFromStreamToDEC(void)
{
    uint8_t  u8Cnt;
    uint8_t  u8StreamIdx;
    uint32_t u32FreeSpace;
    uint16_t u16WrIdx;
    uint16_t u16PassIdx;
#if (H264_OVER_AVB == SCORE_CFG_STREAM_TYPE)
    uint32_t u32ProcAddr;
    uint16_t u16ProcLength;
#endif /* SCORE_CFG_STREAM_TYPE */
#if TRUE == SCORE_CFG_PERMANENT_SOI_SYNC
    uint8_t  u8HeldStreamCount = 0U;
#endif /* SCORE_CFG_PERMANENT_SOI_SYNC */
    
    for(u8StreamIdx = 0U; u8StreamIdx < SCORE_rStrmCfg.u8NumberOfStreams; u8StreamIdx++)
    {
        /** @violates @ref stream_core_c_REF_1 MISRA rule 1.2 */
        if (DECFEED_E_OK != DECFEED_GetBurstLength(&u8Cnt))
        {
            SCORE_rCtl.u32ErrMask |= SCORE_ERR_DECODER_ERR;
            SCORE_rCtl.enState = SCORE_ST_ERROR;
            ERROR("PassBDsFromStreamToDEC: DECFEED_GetBurstLength failed\n");
            /** @violates @ref stream_core_c_REF_2 MISRA rule 14.7 */
            return;
        }
        /** @violates @ref stream_core_c_REF_1 MISRA rule 1.2 */
        else if (DECFEED_E_OK != DECFEED_GetFreeSpace(u8StreamIdx, &u32FreeSpace))
        {
            SCORE_rCtl.u32ErrMask |= SCORE_ERR_DECODER_ERR;
            SCORE_rCtl.enState = SCORE_ST_ERROR;
            ERROR("PassBDsFromStreamToDEC: DECFEED_GetFreeSpace failed\n");
            /** @violates @ref stream_core_c_REF_2 MISRA rule 14.7 */
            return;
        }
        else
        {
            u16WrIdx = arStreamBDRingTrack[u8StreamIdx].u16WrIdx;
            u16PassIdx = arStreamBDRingTrack[u8StreamIdx].u16PassIdx;
        
            if (0U == u32FreeSpace)
            {
                SCORE_rCtl.u32ErrMask |= SCORE_ERR_DECODER_ERR;
                SCORE_rCtl.enState = SCORE_ST_ERROR;
                ERROR("PassBDsFromStreamToDEC: Decoder stall detected\n");
                return;
            }
        
            while( (0U != u32FreeSpace) &&       /* Is there any free space in output? */
                   ( (u16WrIdx != u16PassIdx) || /* Are there any data in input? Don't confuse empty vs. full queue. */
                     (SCORE_CFG_STREAM_BD_RING_LEN == arStreamBDRingTrack[u8StreamIdx].u16Occupied) 
                   ) &&
                   (0U != u8Cnt)                 /* Intentional limitation */
                 )
            {
    #if TRUE == SCORE_CFG_PERMANENT_SOI_SYNC
                if(as32StreamSOIWaitIdx[u8StreamIdx] == u16PassIdx)
                {   /* Count waiting streams */
                    u8HeldStreamCount++;
                    break;
                }
    #endif /* SCORE_CFG_PERMANENT_SOI_SYNC */

    #ifdef ZDDEBUG
                u32StrmToDec++;
    #endif

    #if (H264_OVER_AVB == SCORE_CFG_STREAM_TYPE) && (TRUE == SCORE_PREPROCESS_H264_STREAM)
                /** @violates @ref stream_core_c_REF_1 MISRA rule 1.2 */
        #if TRUE == APP_CFG_ZERO_BUF_ENABLED
                if( (aarStreamBD[u8StreamIdx][u16PassIdx].u32FrameAddr - APP_CFG_ZERO_BUF_HEAD_LENGTH)
                    == u32ZeroBufAddr
                  )
                {   /* Skip for zero buffer */
                    u32ProcAddr = aarStreamBD[u8StreamIdx][u16PassIdx].u32FrameAddr;
                    u16ProcLength = aarStreamBD[u8StreamIdx][u16PassIdx].u16DataLength;
                }
                else
        #endif /* APP_CFG_ZERO_BUF_ENABLED */
                {   /* Do for normal buffer */
                    if (H264PROC_E_OK != H264PROC_PreprocessPacket(                                 \
                                                u8StreamIdx,                                        \
                                                aarStreamBD[u8StreamIdx][u16PassIdx].u32FrameAddr,  \
                                                aarStreamBD[u8StreamIdx][u16PassIdx].u16DataLength, \
                                                &u32ProcAddr,                                       \
                                                &u16ProcLength ))
                    {
                        ERROR("PassBDsFromStreamToDEC: H264PROC_PreprocessFrame failed\n");
                    }
                }
    #endif /* SCORE_CFG_STREAM_TYPE && SCORE_PREPROCESS_H264_STREAM */

    #if (H264_OVER_AVB == SCORE_CFG_STREAM_TYPE) && (TRUE == SCORE_PREPROCESS_H264_STREAM)
                if (DECFEED_E_OK != DECFEED_Push(u8StreamIdx, u32ProcAddr, u16ProcLength))
    #else
                if (DECFEED_E_OK != DECFEED_Push( u8StreamIdx,                      \
                               aarStreamBD[u8StreamIdx][u16PassIdx].u32FrameAddr,   \
                               aarStreamBD[u8StreamIdx][u16PassIdx].u16DataLength   \
                   ))
    #endif /* SCORE_CFG_STREAM_TYPE && SCORE_PREPROCESS_H264_STREAM */
                {
                    SCORE_rCtl.u32ErrMask |= SCORE_ERR_DECODER_ERR;
                    SCORE_rCtl.enState = SCORE_ST_ERROR;
                    ERROR("PassBDsFromStreamToDEC: DECFEED_Push failed\n");
                    /** @violates @ref stream_core_c_REF_2 MISRA rule 14.7 */
                    return;
                }
                
                u16PassIdx = (uint16_t)((u16PassIdx+1U) % SCORE_CFG_STREAM_BD_RING_LEN);
                u32FreeSpace--;
                u8Cnt--;
            }
            arStreamBDRingTrack[u8StreamIdx].u16PassIdx = u16PassIdx;
        }
    }
    #if TRUE == SCORE_CFG_PERMANENT_SOI_SYNC
    /* Check whether all streams are waiting for new SOI */
    if(u8HeldStreamCount == SCORE_rStrmCfg.u8NumberOfStreams)
    {   /* Yes, release all streams */
        uint8_t u8Ctr;
        
        for(u8Ctr=0U; u8Ctr<SCORE_rStrmCfg.u8NumberOfStreams; u8Ctr++)
        {
            as32StreamSOIWaitIdx[u8Ctr] = as32StreamSOIWaitIdxBuf[u8Ctr];
            as32StreamSOIWaitIdxBuf[u8Ctr] = -1;
        }
    }
    #endif /* SCORE_CFG_PERMANENT_SOI_SYNC */

    return;
}

/* @brief Free BDs already processed by decoder DMA from stream queues */
static void FreeProcessedStreamBDs(void)
{
    uint8_t  u8StreamIdx;
    uint32_t iu32Ctr;
    uint32_t u32ProcessedBDNumber;
    uint64_t u64StreamID;
    uint8_t  u8SeqNumber;
    uint16_t u16FreeIdx;
    
    /** @violates @ref stream_core_c_REF_1 MISRA rule 1.2 */
    if (DECFEED_E_OK != DECFEED_GetDecoderStatus(&iu32Ctr))
    {
        SCORE_rCtl.u32ErrMask |= SCORE_ERR_DECODER_ERR;
        SCORE_rCtl.enState = SCORE_ST_ERROR;
        ERROR("FreeProcessedStreamBDs: Unable to get decoder status\n");
        /** @violates @ref stream_core_c_REF_2 MISRA rule 14.7 */
        return;
    }
    else if(0U != iu32Ctr)
    {   /* JPEG decoder error was detected */
        SCORE_rCtl.u32ErrMask |= SCORE_ERR_DECODER_ERR;
        SCORE_rCtl.enState = SCORE_ST_ERROR;
        ERROR("FreeProcessedStreamBDs: Decoder error was detected, error mask: 0x%x\n", iu32Ctr);
        /** @violates @ref stream_core_c_REF_2 MISRA rule 14.7 */
        return;
    }
    else
    {
        for(u8StreamIdx = 0U; u8StreamIdx < SCORE_rStrmCfg.u8NumberOfStreams; u8StreamIdx++)
        {
            /* For each stream read how many BDs were processed by decoder's DMA */
            /** @violates @ref stream_core_c_REF_1 MISRA rule 1.2 */
            if (DECFEED_E_OK != DECFEED_HandleProcessedFrms(u8StreamIdx, &u32ProcessedBDNumber))
            {
                SCORE_rCtl.u32ErrMask |= SCORE_ERR_DECODER_ERR;
                SCORE_rCtl.enState = SCORE_ST_ERROR;
                ERROR("FreeProcessedStreamBDs: DECFEED_HandleProcessedFrms failed\n");
                /** @violates @ref stream_core_c_REF_2 MISRA rule 14.7 */
                return;
            }
    #ifdef ZDDEBUG
            u32ProcessedCtr+=u32ProcessedBDNumber;
    #endif
            /* Free the processed BDs from Streams while checking overwrites in Eth buffers */
            for(iu32Ctr = 0U; iu32Ctr < u32ProcessedBDNumber; iu32Ctr++)
            {
                u16FreeIdx = arStreamBDRingTrack[u8StreamIdx].u16FreeIdx; /* Local copy */
                
                /** @violates @ref stream_core_c_REF_3 MISRA rule 11.3 */
                u8SeqNumber = *(uint8_t*)(((tuAddr)aarStreamBD[u8StreamIdx][u16FreeIdx].u32FrameAddr    \
                                - (tuAddr)SCORE_rStrmCfg.u8FrmDataOffset)                               \
                                    + (tuAddr)SCORE_rStrmCfg.u8SequenceNumOffset);
                
    #if (TRUE == SC_CFG_DEV_ERROR_DETECTION)
                if(u16FreeIdx == arStreamBDRingTrack[u8StreamIdx].u16PassIdx)
                {   /* Trying to free BD that was not passed yet */
                    SCORE_rCtl.u32ErrMask |= SCORE_ERR_STREAM_Q_BROKEN;
                    SCORE_rCtl.enState = SCORE_ST_ERROR;
                    ERROR("FreeProcessedStreamBDs: Queue %u is broken\n", u8StreamIdx);
                    /** @violates @ref stream_core_c_REF_2 MISRA rule 14.7 */
                    return;
                }
    #endif
                
                /** @violates @ref stream_core_c_REF_3 MISRA rule 11.3 */
                u64StreamID = ntohll(*(uint64_t*)(((tuAddr)aarStreamBD[u8StreamIdx][u16FreeIdx].u32FrameAddr    \
                                - (tuAddr)SCORE_rStrmCfg.u8FrmDataOffset)                                       \
                                    + (tuAddr)SCORE_rStrmCfg.u8StreamIDOffset));

                if( (aarStreamBD[u8StreamIdx][u16FreeIdx].u8SeqNumber != u8SeqNumber)
    #if FALSE == APP_CFG_ZERO_BUF_ENABLED /* The check does not work with zero buffer */
                 || (SCORE_rStrmCfg.arStream[u8StreamIdx].u64StreamID != u64StreamID)
    #endif /* APP_CFG_ZERO_BUF_ENABLED */
                  )
                {   /* Buffer overwrite was detected */
                    SCORE_rCtl.u32ErrMask |= SCORE_ERR_ETH_BUFFER_OVERWRITE;
                    SCORE_rCtl.enState = SCORE_ST_ERROR;
                    ERROR("FreeProcessedStreamBDs: Eth buffer was overwritten\n");
                    /** @violates @ref stream_core_c_REF_2 MISRA rule 14.7 */
                    return;
                }

                /* Free the BD (increment idx) */
                u16FreeIdx = (uint16_t)((u16FreeIdx+1U) % SCORE_CFG_STREAM_BD_RING_LEN);
                arStreamBDRingTrack[u8StreamIdx].u16FreeIdx = u16FreeIdx; /* Write it back */
                arStreamBDRingTrack[u8StreamIdx].u16Occupied--;
            }
        }
    }
    
    return;
}

/* @brief Pass all new BDs from Eth to stream queues */
static void PassBDsFromEthToStream(void)
{
    uint8_t  u8StreamIdx;
    sint16_t s16EthBDIdx;
    uint8_t  u8SeqNumber;
    boolean  bStartOfImage;
    uint32_t u32FrameAddr;
#if (JPEG_OVER_AVB == SCORE_CFG_STREAM_TYPE)
    uint32_t u32SOIMark;
#endif /* SCORE_CFG_STREAM_TYPE */
    uint16_t u16WrIdx;
    boolean bRetVal = FALSE;

    /* While there is new received buffer */
    /** @violates @ref stream_core_c_REF_1 MISRA rule 1.2 */
    if (ETHQ_E_OK != ETHQ_GetNextRxBDIdx(&s16EthBDIdx))
    {
        SCORE_rCtl.u32ErrMask |= SCORE_ERR_ETHQ_ERROR;
        SCORE_rCtl.enState = SCORE_ST_ERROR;
        ERROR("PassBDsFromEthToStream: ETHQ_GetNextRxBDIdx() failed\n");
        s16EthBDIdx = -1;
    }
    
    while( 0 <= s16EthBDIdx )
    {
        /** @violates @ref stream_core_c_REF_4 MISRA rule 11.4 */
        u32FrameAddr = ETHQ_RX_BUFFER_ADDR(s16EthBDIdx);
#ifdef ZDDEBUG
        u32TotalFrames++;
#endif
        /* Determine stream */
        for( u8StreamIdx = 0U; u8StreamIdx < SCORE_rStrmCfg.u8NumberOfStreams; u8StreamIdx++)
        {
            /** @violates @ref stream_core_c_REF_3 MISRA rule 11.3 */
            uint64_t u64StreamID = ntohll(*(uint64_t*)((tuAddr)u32FrameAddr + (tuAddr)SCORE_rStrmCfg.u8StreamIDOffset));
            if( SCORE_rStrmCfg.arStream[u8StreamIdx].u64StreamID == u64StreamID )
            {
                break;
            }
        }
        if( u8StreamIdx < SCORE_rStrmCfg.u8NumberOfStreams )
        {   /* Stream index is valid - StreamID matched */
    #ifdef ZDDEBUG
            u32KownStreamID++;
    #endif
            /* Last frame was received now */
            arStreamBDRingTrack[u8StreamIdx].u32LastRxIter = u32IterCtr;
            /* Parse some data from frame */
            /** @implements VLREQ013 */
    #if (JPEG_OVER_AVB == SCORE_CFG_STREAM_TYPE)
            /** @violates @ref stream_core_c_REF_3 MISRA rule 11.3 */
            u32SOIMark = ntohl(*(uint32_t*)((tuAddr)u32FrameAddr + (tuAddr)SCORE_rStrmCfg.u8SOIMarkerOffset));
            bStartOfImage = (SCORE_rStrmCfg.u32SOIMarkerValue == (u32SOIMark & SCORE_rStrmCfg.u32SOIMarkerMask))
                            ? TRUE : FALSE;
    #elif (H264_OVER_AVB == SCORE_CFG_STREAM_TYPE)
            bStartOfImage = (H264PROC_E_OK == H264PROC_IsStartOfFrame(u8StreamIdx, u32FrameAddr + SCORE_rStrmCfg.u8FrmDataOffset)) ? TRUE : FALSE;
    #endif /* SCORE_CFG_STREAM_TYPE */

    #ifdef ZDDEBUG
            if(TRUE == bStartOfImage)
            {
                u32SOI++;
            }
            else
            {
                u32NotSOI++;
            }
    #endif
            /** @violates @ref stream_core_c_REF_3 MISRA rule 11.3 */
            u8SeqNumber = *(uint8_t*)((tuAddr)u32FrameAddr + (tuAddr)SCORE_rStrmCfg.u8SequenceNumOffset);
            /* Checks specific for FETCH state (to store or not to store the BD) */
            /* This condition assumes that this code may run only in RUN or FETCH state */
            if (SCORE_ST_RUN != SCORE_rCtl.enState)
            {
                bRetVal = FetchDecision(u8StreamIdx, bStartOfImage, u8SeqNumber);
        #if TRUE == SCORE_CFG_PERMANENT_SOI_SYNC
                if(TRUE == bRetVal)
                {   /* Initialize the synchronization now */
                    uint8_t u8Ctr;
                    
                    for(u8Ctr=0U; u8Ctr<SCORE_rStrmCfg.u8NumberOfStreams; u8Ctr++)
                    {
                        as32StreamSOIWaitIdx[u8Ctr] = -1;
                        as32StreamSOIWaitIdxBuf[u8Ctr] = -1;
                    }
                }
        #endif /* SCORE_CFG_PERMANENT_SOI_SYNC */
            }
            
            if( (SCORE_ST_RUN == SCORE_rCtl.enState) || (TRUE == bRetVal) )
            {   /* BD shall be stored */
                /* Check that Sequence number is correct */
                if( au8SeqNumber[u8StreamIdx] == u8SeqNumber )
                {   /* SeqNum is in order */
                    au8SeqNumber[u8StreamIdx]++;
                    /* Check that the queue is not full */
                    if( SCORE_CFG_STREAM_BD_RING_LEN > (arStreamBDRingTrack[u8StreamIdx].u16Occupied
        #if TRUE == APP_CFG_ZERO_BUF_ENABLED
                                                        + 1U /* 2 free items might be required instead of 1 */
        #endif /* APP_CFG_ZERO_BUF_ENABLED */
                                                       )
                      )
                    {   /* At least one BD is free */
        #ifdef ZDDEBUG
                        u32EthToStrm++;
        #endif
                        u16WrIdx = arStreamBDRingTrack[u8StreamIdx].u16WrIdx; /* Local copy */

                        if((TRUE == bStartOfImage) && (FALSE == bRetVal)) /* If SOI && ignore the very first SOI */
                        {
            #if TRUE == APP_CFG_ZERO_BUF_ENABLED
                            /* Add the zero buffer before the SOI buffer to the stream queue */
                            aarStreamBD[u8StreamIdx][u16WrIdx].u32FrameAddr = u32ZeroBufAddr + APP_CFG_ZERO_BUF_HEAD_LENGTH;
                            /** @violates @ref stream_core_c_REF_3 MISRA rule 11.3 */
                            aarStreamBD[u8StreamIdx][u16WrIdx].u16DataLength = (uint16_t)128U;
                            aarStreamBD[u8StreamIdx][u16WrIdx].u8SeqNumber = 0;
                            aarStreamBD[u8StreamIdx][u16WrIdx].bAVBTimestampValid = FALSE;
                            aarStreamBD[u8StreamIdx][u16WrIdx].bStartOfImage = FALSE;
                            /* Increment the stream write index */
                            u16WrIdx = (uint16_t)((u16WrIdx+1U) % SCORE_CFG_STREAM_BD_RING_LEN);
                            arStreamBDRingTrack[u8StreamIdx].u16Occupied++;
            #endif /* APP_CFG_ZERO_BUF_ENABLED */

            #if TRUE == SCORE_CFG_PERMANENT_SOI_SYNC
                            if( as32StreamSOIWaitIdx[u8StreamIdx] < 0 )
                            {   /* It is free, use it */
                                as32StreamSOIWaitIdx[u8StreamIdx] = u16WrIdx;
                            }
                            else
                            {   /* Already there. Try to use buffer variable. */
                                if( as32StreamSOIWaitIdxBuf[u8StreamIdx] < 0 )
                                {   /* It is free, use it */
                                    as32StreamSOIWaitIdxBuf[u8StreamIdx] = u16WrIdx;
                                }
                                else
                                {   /* No second buffer. Streams are out of sync or some other issue. */
                                    SCORE_rCtl.u32ErrMask |= SCORE_ERR_STREAM_DROP_OUT;
                                    SCORE_rCtl.enState = SCORE_ST_ERROR;
                                    ERROR("PassBDsFromEthToStream: Synchronization holds too many pictures.\n");
                                }
                            }
            #endif /* SCORE_CFG_PERMANENT_SOI_SYNC */
                        }
                        
                        /* Write the values to stream BD */
                        aarStreamBD[u8StreamIdx][u16WrIdx].u32FrameAddr = u32FrameAddr + SCORE_rStrmCfg.u8FrmDataOffset;
        #if (H264_OVER_AVB == SCORE_CFG_STREAM_TYPE)
                        /* Get data length AS IS GIVEN BY TRANSPORT PROTOCOL */
                        /** @violates @ref stream_core_c_REF_3 MISRA rule 11.3 */
                        aarStreamBD[u8StreamIdx][u16WrIdx].u16DataLength = (uint16_t)ntohs(*((uint16_t *)((tuAddr)u32FrameAddr + 40UL))) - 4U;
        #else
                        /** @violates @ref stream_core_c_REF_4 MISRA rule 11.4 */
                        aarStreamBD[u8StreamIdx][u16WrIdx].u16DataLength = (uint16_t)((uint16_t)(ETHQ_RX_BUFFER_LENGTH(s16EthBDIdx) - SCORE_rStrmCfg.u8FrmDataOffset) - 4U); /* Minus CRC */
        #endif /* SCORE_CFG_STREAM_TYPE */
                        aarStreamBD[u8StreamIdx][u16WrIdx].u8SeqNumber = u8SeqNumber;
                        /* Not supported yet */
/*
                        aarStreamBD[u8StreamIdx][u16WrIdx].u32AVBTimestamp = ntohl(*(uint32_t*)(tuAddr)(u32FrameAddr + AVB_CFG_TST_OFFSET));
*/
                        aarStreamBD[u8StreamIdx][u16WrIdx].bAVBTimestampValid = FALSE;
                        aarStreamBD[u8StreamIdx][u16WrIdx].bStartOfImage = bStartOfImage;
                            /* Increment the stream write index */
                            u16WrIdx = (uint16_t)((u16WrIdx+1U) % SCORE_CFG_STREAM_BD_RING_LEN);
                        arStreamBDRingTrack[u8StreamIdx].u16WrIdx = u16WrIdx; /* Write it back */
                            arStreamBDRingTrack[u8StreamIdx].u16Occupied++;
                        }
                    else
                    {   /* Stream queue is full */
                        SCORE_rCtl.u32ErrMask |= SCORE_ERR_STREAM_Q_FULL;
                        SCORE_rCtl.enState = SCORE_ST_ERROR;
                        ERROR("PassBDsFromEthToStream: Queue of stream %u is full\n", u8StreamIdx);
                    }
                }
                else
                {   /* SeqNum is out of order */
                    SCORE_rCtl.u32ErrMask |= SCORE_ERR_OUT_OF_ORDER_FRAME;
                    SCORE_rCtl.enState = SCORE_ST_ERROR;
                    ERROR("PassBDsFromEthToStream: Out-of-order frame received on stream %u\n", u8StreamIdx);
                    if (ETHQ_E_OK != ETHQ_UnlockRxBD((uint32_t)s16EthBDIdx))
                    {
                        SCORE_rCtl.u32ErrMask |= SCORE_ERR_ETHQ_ERROR;
                        SCORE_rCtl.enState = SCORE_ST_ERROR;
                        ERROR("PassBDsFromEthToStream: ETHQ_UnlockRxBD() failed\n");
                    }
                }
            }
            else
            {   /* BD shall be dropped (not stored) */
                /* Nothing to do */
            }
        }
        else
        {   /* Stream index is NOT valid - unknown StreamID */
            /* Drop the frame and set flag to indicate that */
            SCORE_rCtl.u32ErrMask |= SCORE_WAR_UNKNOWN_STREAM_ID_FRAME;
        }
        
        /* Unlock the Eth BD anyway (fire & forget approach) */
        if (ETHQ_E_OK != ETHQ_UnlockRxBD((uint32_t)s16EthBDIdx))
        {
            SCORE_rCtl.u32ErrMask |= SCORE_ERR_ETHQ_ERROR;
            SCORE_rCtl.enState = SCORE_ST_ERROR;
            ERROR("PassBDsFromEthToStream: ETHQ_UnlockRxBD() failed\n");
        }

        /*  Break the loop in case of error */
        if (SCORE_ST_ERROR == SCORE_rCtl.enState)
        {
            break;
        }

        /*  Get next descriptor */
        /** @violates @ref stream_core_c_REF_1 MISRA rule 1.2 */
        if (ETHQ_E_OK != ETHQ_GetNextRxBDIdx(&s16EthBDIdx))
        {
            SCORE_rCtl.u32ErrMask |= SCORE_ERR_ETHQ_ERROR;
            SCORE_rCtl.enState = SCORE_ST_ERROR;
            ERROR("PassBDsFromEthToStream: ETHQ_GetNextRxBDIdx() failed\n");
            s16EthBDIdx = -1;
        }
    }
    
    if (ETHQ_E_OK != ETHQ_WriteRDAR())
    {
        SCORE_rCtl.u32ErrMask |= SCORE_ERR_ETHQ_ERROR;
        SCORE_rCtl.enState = SCORE_ST_ERROR;
        ERROR("PassBDsFromEthToStream: ETHQ_WriteRDAR() failed\n");
    }
    
    /*  Check stream drop-out */
    for( u8StreamIdx = 0U; u8StreamIdx < SCORE_rStrmCfg.u8NumberOfStreams; u8StreamIdx++)
    {   /* Subtraction result below is valid also when values are wrapping */
        if((uint32_t)(u32IterCtr - arStreamBDRingTrack[u8StreamIdx].u32LastRxIter) > SCORE_rStrmCfg.u8DropOutThreshold)
        {
            SCORE_rCtl.u32ErrMask |= SCORE_ERR_STREAM_DROP_OUT;
            SCORE_rCtl.enState = SCORE_ST_ERROR;
            ERROR("PassBDsFromEthToStream: Stream %hhu is not receiving data.\n", u8StreamIdx);
        }
    }
}

/*
* @brief        In FETCH state it decides which frames shall be queued. May switch to RUN state.
* @details      BDs shall be stored only in streams that already have their SOI frame.
*               The SOI frame is valid till SCORE_rStrmCfg.u16FetchSOIThreshold BDs are queued.
*               Then the queue is discarded and waiting for new SOI.
*               When all streams have the SOI then it switches to RUN state.
* @param[in]    u8StreamIdx Selects stream to work with.
* @param[in]    bStartOfImage Whether received frame is Start of Image.
* @param[in]    u8FrameSeqNumber Sequence number of received frame.
* @retval       TRUE This frame shall be stored into stream queue.
* @retval       FALSE This frame shall be discarded.
*/
static boolean FetchDecision(uint8_t u8StreamIdx, boolean bStartOfImage, uint8_t u8FrameSeqNumber)
{
    boolean bToStore;
    boolean bAllSOIs;
    uint8_t iu8LocalStreamIdx;

    if( TRUE == bStartOfImage )
    {   /* New SOI */
        /* Empty the stream queue */
        EmptyStreamQueue(u8StreamIdx);
        /* Store the new BD */
        bToStore = TRUE;
        /* Reset countdown */
        au16StreamSOICountdown[u8StreamIdx] = (uint16_t)(SCORE_rStrmCfg.u16FetchSOIThreshold + 1U);
        /* Initialize Sequence Number */
        au8SeqNumber[u8StreamIdx] = u8FrameSeqNumber;
        /* Check whether all streams have the SOI */
        bAllSOIs = TRUE;
        for( iu8LocalStreamIdx = 0U; iu8LocalStreamIdx < SCORE_rStrmCfg.u8NumberOfStreams; iu8LocalStreamIdx++)
        {
            if( au16StreamSOICountdown[iu8LocalStreamIdx] == 0U )
            {   /* This stream does not have the SOI */
                bAllSOIs = FALSE;
            }
        }
        if( TRUE == bAllSOIs )
        {   /* Switch to next state */
            SCORE_rCtl.enState = SCORE_ST_RUN;
        }
    }
    else
    {
        /* Check whether current stream already has enqueued SOI and it is not too old */
        if( au16StreamSOICountdown[u8StreamIdx] > 0U )
        {   /* There is the SOI in the stream queue */
            au16StreamSOICountdown[u8StreamIdx]--;
            /* Check whether the SOI expired now */
            if( au16StreamSOICountdown[u8StreamIdx] > 0U )
            {   /* Not expired; Store the received BD */
                bToStore = TRUE;
            }
            else
            {   /* Expired; Empty the stream queue and discard the BD */
                EmptyStreamQueue(u8StreamIdx);
                bToStore = FALSE;
            }
        }
        else
        {   /* There is no SOI; Discard the BD */
            bToStore = FALSE;
        }
    }

    return bToStore;
}

/*
* @brief        This function is called during initialization and in FETCH state to remove all BDs from given stream queue.
* @param[in]    u8StreamIdx Which stream to empty.
*/
static void EmptyStreamQueue(uint8_t u8StreamIdx)
{
    arStreamBDRingTrack[u8StreamIdx].u16WrIdx = 0U;
    arStreamBDRingTrack[u8StreamIdx].u16PassIdx = 0U;
    arStreamBDRingTrack[u8StreamIdx].u16FreeIdx = 0U;
    arStreamBDRingTrack[u8StreamIdx].u16Occupied = 0U;
}

/*
* @brief    This function is handling configuration that may change in runtime.
* @retval   SCORE_E_OK Success.
* @retval   SCORE_E_INVALID_CONFIG Error - Invalid configuration.
* @retval   SCORE_E_MAPPING_ERROR Error - Failed to create memory mapping.
*/
static stream_core_ret_t Configure(void)
{
    uint8_t   u8VlanPrioCtr;
#if (TRUE == SM_CFG_ENABLE_MMU)
    mmu_ret_t mmuRetVal;
    uint32_t  u32Base;
    uint32_t  u32Length;
#endif /* SM_CFG_ENABLE_MMU */

    /* === APPLY CONFIGURATION === */
    /* Copy buffered values to active structures */
    SCORE_rStrmCfg = rStreamCfgPrepare;
    
#if (TRUE == SM_CFG_ENABLE_MMU)
    /*  Create non-cached, read-write mapping for decoder registers */
    /* FIXME: as long as base address and region length may never be changed, this shall be done only once, not in case of reconfiguration */
    /** @violates @ref stream_core_c_REF_1 MISRA rule 1.2 */
    if (DECFEED_E_OK == DECFEED_GetMemoryBase(&u32Base, &u32Length))
    {
        mmuRetVal = mmu_add_mapping((va_t)u32Base, (pa_t)u32Base, (mlen_t)u32Length, mmu_get_attr(MTYPE_DEVICE, MA_EL2_EL3_RW));
        if (MMU_E_OK != mmuRetVal)
        {
            SCORE_rCtl.u32ErrMask |= SCORE_ERR_MEMORY_MAPPING;
            SCORE_rCtl.enState = SCORE_ST_ERROR;
            ERROR("Configure: JPEG decoder register mapping failed: %d\n", (sint32_t)(mmuRetVal));
            /** @violates @ref stream_core_c_REF_2 MISRA rule 14.7 */
            return SCORE_E_MAPPING_ERROR;
        }
    }
    else
    {
        ERROR("Configure: Can't configure DECFEED memory spaces, invalid or missing configuration\n");
        /** @violates @ref stream_core_c_REF_2 MISRA rule 14.7 */
        return SCORE_E_INVALID_CONFIG;
    }

    /** @violates @ref stream_core_c_REF_1 MISRA rule 1.2 */
    if (ETHQ_E_OK == ETHQ_GetMemoryBase(&u32Base, &u32Length))
    {
        /*  Create non-cached, read-write mapping for Ethernet controller registers */
        /* FIXME: as long as base address and region length may never be changed, this shall be done only once, not in case of reconfiguration */
        mmuRetVal = mmu_add_mapping((va_t)u32Base, (pa_t)u32Base, (mlen_t)u32Length, mmu_get_attr(MTYPE_DEVICE, MA_EL2_EL3_RW));
        if (MMU_E_OK != mmuRetVal)
        {
            SCORE_rCtl.u32ErrMask |= SCORE_ERR_MEMORY_MAPPING;
            SCORE_rCtl.enState = SCORE_ST_ERROR;
            ERROR("Configure: ETH_QUEUE register mapping failed: %d\n", (sint32_t)(mmuRetVal));
            /** @violates @ref stream_core_c_REF_2 MISRA rule 14.7 */
            return SCORE_E_MAPPING_ERROR;
        }
    }
    else
    {
        ERROR("Configure: Can't configure ETHQ memory space, invalid or missing configuration\n");
        /** @violates @ref stream_core_c_REF_2 MISRA rule 14.7 */
        return SCORE_E_INVALID_CONFIG;
    }
    
    /** @violates @ref stream_core_c_REF_1 MISRA rule 1.2 */
    if (ETHQ_E_OK ==  ETHQ_GetBufferBase(&u32Base, &u32Length))
    {
        /*  Create non-cached, read-only mapping for the Ethernet data memory */
        /* FIXME: The address/length may be changed, so this code is necessary. But the mapping is never removed. Consider adding removal into SCORE_Stop */
#if (TRUE == SCORE_PREPROCESS_H264_STREAM)
        mmuRetVal = mmu_add_mapping( (va_t)u32Base, (pa_t)u32Base, (mlen_t)u32Length, mmu_get_attr(MTYPE_NORMAL_NC, MA_EL2_EL3_RW));
#else
        mmuRetVal = mmu_add_mapping( (va_t)u32Base, (pa_t)u32Base, (mlen_t)u32Length, mmu_get_attr(MTYPE_NORMAL_NC, MA_EL2_EL3_RO));
#endif /* SCORE_PREPROCESS_H264_STREAM */
        if (MMU_E_OK != mmuRetVal)
        {
            SCORE_rCtl.u32ErrMask |= SCORE_ERR_MEMORY_MAPPING;
            SCORE_rCtl.enState = SCORE_ST_ERROR;
            ERROR("Configure: Memory mapping for Eth buffers failed\n");
            /** @violates @ref stream_core_c_REF_2 MISRA rule 14.7 */
            return SCORE_E_MAPPING_ERROR;
        }
    }
    else
    {
        ERROR("Configure: Can't configure ETHQ buffer space, invalid or missing configuration\n");
        /** @violates @ref stream_core_c_REF_2 MISRA rule 14.7 */
        return SCORE_E_INVALID_CONFIG;
    }
    
#if TRUE == APP_CFG_ZERO_BUF_ENABLED
    mmuRetVal = mmu_add_mapping( (va_t)u32ZeroBufAddr, (pa_t)u32ZeroBufAddr, (mlen_t)512U, mmu_get_attr(MTYPE_NORMAL_NC, MA_EL2_EL3_RW));
    if (MMU_E_OK != mmuRetVal)
    {
        SCORE_rCtl.u32ErrMask |= SCORE_ERR_MEMORY_MAPPING;
        SCORE_rCtl.enState = SCORE_ST_ERROR;
        ERROR("Configure: Memory mapping for u32ZeroBufAddr failed\n");
        /** @violates @ref stream_core_c_REF_2 MISRA rule 14.7 */
        return SCORE_E_MAPPING_ERROR;
    }
#endif /* APP_CFG_ZERO_BUF_ENABLED */
    
    
    /*  Non-cached mapping for Ethernet buffer descriptor ring was created automatically (declared as nocache)*/
#endif /* SM_CFG_ENABLE_MMU */

    if (DECFEED_E_OK != DECFEED_Init())
    {
        SCORE_rCtl.u32ErrMask |= SCORE_ERR_DECODER_ERR;
        SCORE_rCtl.enState = SCORE_ST_ERROR;
        ERROR("Configure: DECFEED_Init() failed\n");
        /** @violates @ref stream_core_c_REF_2 MISRA rule 14.7 */
        return SCORE_E_INVALID_CONFIG;
    }

    if (ETHQ_E_OK != ETHQ_Init())
    {
        /* Ethernet Queue initialization failed */
        SCORE_rCtl.u32ErrMask |= SCORE_ERR_ETHQ_INCOMPLETE_CONFIG;
        SCORE_rCtl.enState = SCORE_ST_ERROR;
        ERROR("Configure: ETHQ_Init: Incomplete/wrong/missing configuration parameters\n");
        /** @violates @ref stream_core_c_REF_2 MISRA rule 14.7 */
        return SCORE_E_INVALID_CONFIG;
    }

    /* Configure HW Rx dispatching based on VLAN priorities */
    /* Clear previously configured VLAN tags */
    if (ETHQ_E_OK != ETHQ_RemoveAllVlanClassifications())
    {
        SCORE_rCtl.u32ErrMask |= SCORE_ERR_ETHQ_ERROR;
        SCORE_rCtl.enState = SCORE_ST_ERROR;
        ERROR("Configure: ETHQ_RemoveAllVlanClassifications() failed\n");
        /** @violates @ref stream_core_c_REF_2 MISRA rule 14.7 */
        return SCORE_E_INVALID_CONFIG;
    }
    else
    {
        for(u8VlanPrioCtr = 0U; u8VlanPrioCtr < rStreamCfgPrepare.u8VlanPrioCnt; u8VlanPrioCtr++)  /* Add new VLAN tags */
        {
            if( ETHQ_E_OK != ETHQ_AddVlanClassification(rStreamCfgPrepare.au8VlanPrio[u8VlanPrioCtr]) )
            {
                SCORE_rCtl.u32ErrMask |= SCORE_ERR_FAILED_TO_ADD_VLAN_TAG;
                SCORE_rCtl.enState = SCORE_ST_ERROR;
                ERROR("Configure: Failed to add VLAN priority to HW classifier\n");
                /** @violates @ref stream_core_c_REF_2 MISRA rule 14.7 */
                return SCORE_E_INVALID_CONFIG;
            }
        }
    }
    
#if TRUE == APP_CFG_ZERO_BUF_ENABLED
    /* Initialize with zeros */
    uint32_t u32TmpCtr;
    for(u32TmpCtr=0U; u32TmpCtr<512U; u32TmpCtr++)
        ((uint8_t *)u32ZeroBufAddr)[u32TmpCtr] = 0U;
#endif /* APP_CFG_ZERO_BUF_ENABLED */

    return SCORE_E_OK;
}

/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/
/**
* @brief        Function for getting current value of stream_core error mask.
* @return       Value of stream_core error mask. Each set bit determines that corresponding kind
*               of error or warning was detected in the stream_core during run-time.
*/
uint32_t SCORE_GetErrorMask(void)
{
    return SCORE_rCtl.u32ErrMask;
}

/**
* @brief        Function for cleaning stream_core error mask.
* @details      This function shall be used to delete all error flags after all errors were handled
*               by upper layers.
*/
void SCORE_ClearErrorMask(void)
{
    SCORE_rCtl.u32ErrMask = 0UL;
}

/**
* @brief        Function for getting current state of stream_core.
* @return       Current state of stream_core. Find possible values in documentation of enum SCORE_tenState.
*/
SCORE_tenState SCORE_GetCurrentState(void)
{
    return SCORE_rCtl.enState;
}

/**
* @brief        Initialization function of stream_core module.
* @details      This function must be called before each call of SCORE_Start. It checks validity of
*               configurations of stream_core, dec_feed, and eth_queue modules, applies stream_core
*               configuration, and initializes queues and other variables in all 3 mentioned modules.
* @return       If configuration check fails, then return value from SCORE_CheckConfig is
*               passed. Otherwise this function returns either error value or on success SCORE_E_OK.
*/
stream_core_ret_t SCORE_Init(void)
{
    uint8_t  u8StreamIdx;
    stream_core_ret_t eRetVal;
    
    eRetVal = SCORE_CheckConfig();
    
    if (SCORE_E_OK == eRetVal)
    {
        if (TRUE == bInitialized)
        {
            /*  Module should be stopped before new initialization is requested */
            eRetVal = SCORE_E_ALREADY_INITIALIZED;
        }
        else
        {
            /* Clean application data */
            for( u8StreamIdx=0U; u8StreamIdx<APP_CFG_MAX_STREAMS; u8StreamIdx++ )
            {
                au16StreamSOICountdown[u8StreamIdx] = 0U;
                EmptyStreamQueue(u8StreamIdx);
            }
            
            /* Needed only in case configuration has changed or error has occurred, reconfigure */
            if( (TRUE == bConfigHasChanged) || (SCORE_ST_ERROR == SCORE_rCtl.enState) )
            {   /* Command not valid in PRERUN/FETCH/RUN state */
                /* Copy the configuration from rStreamCfgPrepare */
                /* NOTE: Ethernet controller and JPEG decoder registers MUST NOT BE ACCESSED before this is called */
                if(SCORE_E_OK != Configure())
                {
                    /* (Re)configuration failed */
                    /* Error was already reported */
                    eRetVal = SCORE_E_INVALID_CONFIG;
                }
                else
                {
                    bConfigHasChanged = FALSE;
                }
            }
            
            if (SCORE_E_OK == eRetVal)
            {
                bInitialized = TRUE;
            }
        }
    }
    
    return eRetVal;
}

/**
* @brief        Function for getting current values of runtime configuration parameters.
* @param[in]    u64Var Selects which parameter (which variable) shall be read.
* @param[in]    pu64Val Current value of selected parameter will be written here.
* @retval       SCORE_E_UNKNOWN_PARAMETER Error - parameter u64Var was not recognized. Since this
*               function is not implemented yet, this is the only possible return value.
*/
stream_core_ret_t SCORE_GetConf(uint64_t u64Var, uint64_t *pu64Val)
{
    /* FIXME */
    UNUSED(u64Var);
    UNUSED(pu64Val);
    
    return SCORE_E_UNKNOWN_PARAMETER;
}

/**
* @brief        Function for setting values of runtime configuration parameters.
* @param[in]    u64Var Selects which parameter (which variable) shall be written.
* @param[in]    u64Val Value to be assigned to selected parameter.
* @retval       SCORE_E_UNKNOWN_PARAMETER Error - parameter given in argument u64Var was not recognized.
* @retval       SCORE_E_INVALID_CONFIG Error - value of parameter given in argument pu64Val is not valid.
* @retval       SCORE_E_OK Parameter value was successfully set.
*/
stream_core_ret_t SCORE_SetConf(uint64_t u64Var, uint64_t u64Val)
{
    stream_core_ret_t eRetVal = SCORE_E_OK;

    switch (u64Var & 0xffffffffU)
    {
        /* STREAMING PARAMETERS */
        case APP_KEY_STRM_FETCH_THRESHOLD:
            if((((uint16_t)u64Val + 2U) <= SCORE_CFG_STREAM_BD_RING_LEN) && (0U != (uint16_t)u64Val))
            {
                rStreamCfgPrepare.u16FetchSOIThreshold = (uint16_t)u64Val;
                rStreamCfgFlags.bFetchSOIThresholdSet = TRUE;
                bConfigHasChanged = TRUE;
            }
            else
            {
                SCORE_rCtl.u32ErrMask |= SCORE_ERR_INVALID_CONFIGURATION;
                SCORE_rCtl.enState = SCORE_ST_ERROR;
                eRetVal = SCORE_E_INVALID_CONFIG;
                ERROR("SCORE_SetConf: Fetch threshold shall be between 1 and %d inclusive\n", SCORE_CFG_STREAM_BD_RING_LEN-2U);
            }
            break;
        case APP_KEY_STRM_SOI_MARK_VALUE:
            rStreamCfgPrepare.u32SOIMarkerValue = (uint32_t)u64Val;
            rStreamCfgFlags.bSOIMarkerValueSet = TRUE;
            bConfigHasChanged = TRUE;
            break;
        case APP_KEY_STRM_SOI_MARK_OFFSET:
            if (0U == ((uint8_t)u64Val & 0x3U))
            {   /* Alignment OK */
                rStreamCfgPrepare.u8SOIMarkerOffset = (uint8_t)u64Val;
                rStreamCfgFlags.bSOIMarkerOffsetSet = TRUE;
                bConfigHasChanged = TRUE;
            }
            else
            {
                SCORE_rCtl.u32ErrMask |= SCORE_ERR_ETHQ_FIELD_NOT_ALIGNED;
                eRetVal = SCORE_E_INVALID_CONFIG;
                ERROR("SCORE_SetConf: SOI marker is not aligned to 4B\n");
            }
            break;
        case APP_KEY_STRM_SOI_MARK_MASK:
            rStreamCfgPrepare.u32SOIMarkerMask = (uint32_t)u64Val;
            rStreamCfgFlags.bSOIMarkerMaskSet = TRUE;
            bConfigHasChanged = TRUE;
            break;
        case APP_KEY_STRM_SEQ_NUM_OFFSET:
            rStreamCfgPrepare.u8SequenceNumOffset = (uint8_t)u64Val;
            rStreamCfgFlags.bSequenceNumOffsetSet = TRUE;
            bConfigHasChanged = TRUE;
            break;
        case APP_KEY_STRM_STRM_ID_OFFSET:
            if(0U == ((uint8_t)u64Val & 0x7U))
            {   /* Alignment OK */
                rStreamCfgPrepare.u8StreamIDOffset = (uint8_t)u64Val;
                rStreamCfgFlags.bStreamIDOffsetSet = TRUE;
                bConfigHasChanged = TRUE;
            }
            else
            {
                SCORE_rCtl.u32ErrMask |= SCORE_ERR_ETHQ_FIELD_NOT_ALIGNED;
                eRetVal = SCORE_E_INVALID_CONFIG;
                ERROR("SCORE_SetConf: Stream ID is not aligned to 8B\n");
            }
            break;
        case APP_KEY_STRM_FRM_DATA_OFFSET:
            rStreamCfgPrepare.u8FrmDataOffset = (uint8_t)u64Val;
            rStreamCfgFlags.bFrmDataOffsetSet = TRUE;
            bConfigHasChanged = TRUE;
            break;
        case APP_KEY_STRM_VLAN_PRIO_CLEAN:
            rStreamCfgPrepare.u8VlanPrioCnt = 0U;
            rStreamCfgFlags.bVlanPrioCntSet = FALSE;
            bConfigHasChanged = TRUE;
            break;
        case APP_KEY_STRM_VLAN_PRIO_ADD:
            if(rStreamCfgPrepare.u8VlanPrioCnt < 4U)
            {
                rStreamCfgPrepare.au8VlanPrio[rStreamCfgPrepare.u8VlanPrioCnt] = (uint8_t)u64Val;
                rStreamCfgFlags.abVlanPrioSet[rStreamCfgPrepare.u8VlanPrioCnt] = TRUE;
                rStreamCfgPrepare.u8VlanPrioCnt++;
                rStreamCfgFlags.bVlanPrioCntSet = TRUE;
                bConfigHasChanged = TRUE;
            }
            else
            {
                SCORE_rCtl.u32ErrMask |= SCORE_WAR_TOO_MANY_VLAN_PRIO;
                WARNING("SCORE_SetConf: Added too many VLAN priorities, maximum is 4, ignoring\n");
                eRetVal = SCORE_E_INVALID_CONFIG;
            }
            break;
        case APP_KEY_STRM_NUMBER_OF_STRMS:
            if (DECFEED_E_OK != DECFEED_SetNumOfStreams((uint8_t)u64Val))
            {
                SCORE_rCtl.u32ErrMask |= SCORE_ERR_UNSUPPORTED_PARAM_VAL;
                eRetVal = SCORE_E_INVALID_CONFIG;
                ERROR("SCORE_SetConf: DECFEED_SetNumOfStreams() failed\n");
            }
            
            if(((uint8_t)u64Val >= 1U) && ((uint8_t)u64Val <= APP_CFG_MAX_STREAMS))
            {
                rStreamCfgPrepare.u8NumberOfStreams = (uint8_t)u64Val;
                rStreamCfgFlags.bNumberOfStreamsSet = TRUE;
                bConfigHasChanged = TRUE;
            }
            else
            {
                SCORE_rCtl.u32ErrMask |= SCORE_ERR_UNSUPPORTED_PARAM_VAL;
                eRetVal = SCORE_E_INVALID_CONFIG;
                ERROR("SCORE_SetConf: Unsupported number of streams: %u\n", (uint8_t)u64Val);
            }
            break;
        case APP_KEY_STRM_STREAM_ID_0:
            rStreamCfgPrepare.arStream[0U].u64StreamID = u64Val;
            rStreamCfgFlags.abStreamIdSet[0U] = TRUE;
            bConfigHasChanged = TRUE;
            break;
        case APP_KEY_STRM_STREAM_ID_1:
#if (APP_CFG_MAX_STREAMS > 1U)
            rStreamCfgPrepare.arStream[1U].u64StreamID = u64Val;
            rStreamCfgFlags.abStreamIdSet[1U] = TRUE;
            bConfigHasChanged = TRUE;
            /* else: silently ignore */
#endif /* APP_CFG_MAX_STREAMS */
            break;
        case APP_KEY_STRM_STREAM_ID_2:
#if (APP_CFG_MAX_STREAMS > 2U)
            rStreamCfgPrepare.arStream[2U].u64StreamID = u64Val;
            rStreamCfgFlags.abStreamIdSet[2U] = TRUE;
            bConfigHasChanged = TRUE;
            /* else: silently ignore */
#endif /* APP_CFG_MAX_STREAMS */
            break;
        case APP_KEY_STRM_STREAM_ID_3:
#if (APP_CFG_MAX_STREAMS > 3U)
            rStreamCfgPrepare.arStream[3U].u64StreamID = u64Val;
            rStreamCfgFlags.abStreamIdSet[3U] = TRUE;
            bConfigHasChanged = TRUE;
            /* else: silently ignore */
#endif /* APP_CFG_MAX_STREAMS */
            break;
        case APP_KEY_STRM_DROP_OUT_THRESHOLD:
            if((u64Val > 0U) && (u64Val <= 255U)) /*  */
            {   /* Value OK */
                rStreamCfgPrepare.u8DropOutThreshold = (uint8_t)u64Val;
                rStreamCfgFlags.bDropOutThresholdSet = TRUE;
                bConfigHasChanged = TRUE;
            }
            else
            {
                SCORE_rCtl.u32ErrMask |= SCORE_ERR_INVALID_CONFIGURATION;
                eRetVal = SCORE_E_INVALID_CONFIG;
                ERROR("SCORE_SetConf: Threshold for stream drop-out is out of range 1..255\n");
            }
            break;


        /* ETHERNET QUEUE PARAMETERS */
        case APP_KEY_ETHQ_BASE_ADDR:
            if (ETHQ_E_OK != ETHQ_SetBaseAddr((uint32_t)u64Val))
            {
                SCORE_rCtl.u32ErrMask |= SCORE_ERR_INVALID_CONFIGURATION;
                SCORE_rCtl.enState = SCORE_ST_ERROR;
                ERROR("SCORE_SetConf: ETHQ_SetBaseAddr() failed\n");
                eRetVal = SCORE_E_INVALID_CONFIG;
            }
            break;
        case APP_KEY_ETHQ_REGION_LENGTH:
            if (ETHQ_E_OK != ETHQ_SetRegionLength((uint32_t)u64Val))
            {
                SCORE_rCtl.u32ErrMask |= SCORE_ERR_INVALID_CONFIGURATION;
                SCORE_rCtl.enState = SCORE_ST_ERROR;
                ERROR("SCORE_SetConf: ETHQ_SetRegionLength() failed\n");
                eRetVal = SCORE_E_INVALID_CONFIG;
            }
            break;
        case APP_KEY_ETHQ_SIZE_OF_BUFF:
            if (ETHQ_E_OK != ETHQ_SetBufferSize((uint32_t)u64Val))
            {
                SCORE_rCtl.u32ErrMask |= SCORE_ERR_INVALID_CONFIGURATION;
                SCORE_rCtl.enState = SCORE_ST_ERROR;
                ERROR("SCORE_SetConf: ETHQ_SetBufferSize() failed\n");
                eRetVal = SCORE_E_INVALID_CONFIG;
            }
            break;
        case APP_KEY_ETHQ_BUFF_RING_PTR:
            if (ETHQ_E_OK != ETHQ_SetRingAddr((uint32_t)u64Val))
            {
                SCORE_rCtl.u32ErrMask |= SCORE_ERR_INVALID_CONFIGURATION;
                SCORE_rCtl.enState = SCORE_ST_ERROR;
                ERROR("SCORE_SetConf: ETHQ_SetRingAddr() failed\n");
                eRetVal = SCORE_E_INVALID_CONFIG;
            }
            break;

        /* VIDEO DECODER PARAMETERS */
        case APP_KEY_DEC_FEED_AT_ONCE:
            if (DECFEED_E_OK != DECFEED_SetBurstLength((uint8_t)u64Val))
            {
                SCORE_rCtl.u32ErrMask |= SCORE_ERR_INVALID_CONFIGURATION;
                SCORE_rCtl.enState = SCORE_ST_ERROR;
                ERROR("SCORE_SetConf: DECFEED_SetBurstLength() failed\n");
                eRetVal = SCORE_E_INVALID_CONFIG;
            }
            break;
        case APP_KEY_DEC_BASE_ADDR:
            if (DECFEED_E_OK != DECFEED_SetBaseAddress((uint32_t)u64Val))
            {
                SCORE_rCtl.u32ErrMask |= SCORE_ERR_INVALID_CONFIGURATION;
                SCORE_rCtl.enState = SCORE_ST_ERROR;
                ERROR("SCORE_SetConf: DECFEED_SetBaseAddress() failed\n");
                eRetVal = SCORE_E_INVALID_CONFIG;
            }
            break;
        case APP_KEY_DEC_REGION_LENGTH:
            if (DECFEED_E_OK != DECFEED_SetRegionLength((uint32_t)u64Val))
            {
                SCORE_rCtl.u32ErrMask |= SCORE_ERR_INVALID_CONFIGURATION;
                SCORE_rCtl.enState = SCORE_ST_ERROR;
                ERROR("SCORE_SetConf: DECFEED_SetRegionLength() failed\n");
                eRetVal = SCORE_E_INVALID_CONFIG;
            }
            break;
    #if TRUE == APP_CFG_ZERO_BUF_ENABLED            
        case APP_KEY_SRAM_BUFFER:
        {
            u32ZeroBufAddr = (uint32_t)(u64Val);
            INFO("Got experimental buffer at 0x%x\n", u32ZeroBufAddr);
            break;
        }
    #endif /* APP_CFG_ZERO_BUF_ENABLED */
        /* UNKNOWN PARAMETER */
        default:
            SCORE_rCtl.u32ErrMask |= SCORE_ERR_UNKNOWN_PARAM;
            eRetVal = SCORE_E_UNKNOWN_PARAMETER;
            ERROR("SCORE_SetConf: Configuration parameter (key 0x%x) was not recognized\n", (uint32_t)(u64Var & 0xffffffffU));
            break;
    }

    return eRetVal;
}

/**
* @brief        Changes stream_core state from READY to PRERUN.
* @retval       SCORE_E_NOT_INITIALIZED Error - function SCORE_Init shall be executed before and it shall not fail.
* @retval       SCORE_E_OK Success.
*/
stream_core_ret_t SCORE_Start(void)
{
    stream_core_ret_t eRetVal = SCORE_E_OK;

    if (TRUE == bInitialized)
    {
#ifdef ZDDEBUG
        u32EthToStrm = 0U;
        u32StrmToDec = 0U;
        u32TotalFrames = 0U;
        u32KownStreamID = 0U;
        u32SOI = 0U;
        u32NotSOI = 0U;
        u32ProcessedCtr = 0U;
#endif
        /* PRE-START the streaming */
        SCORE_rCtl.enState = SCORE_ST_PRERUN;
    }
    else
    {
        eRetVal = SCORE_E_NOT_INITIALIZED;
    }

    return eRetVal;
}

/**
* @brief        Stops other modules and changes stream_core state to READY.
* @retval       SCORE_E_NOT_INITIALIZED Error - function SCORE_Init shall be executed before and it shall not fail.
* @retval       SCORE_E_OK Success.
*/
stream_core_ret_t SCORE_Stop(void)
{
    stream_core_ret_t eRetVal = SCORE_E_OK;
    
#ifdef ZDDEBUG
    INFO("SCORE_Stop: Error mask is: 0x%x\n", SCORE_rCtl.u32ErrMask);
#endif
    
    if (FALSE == bInitialized)
    {
        eRetVal = SCORE_E_NOT_INITIALIZED;
    }
    else
    {
        /*  This command is valid only in PRERUN, FETCH, or RUN state, otherwise it is not needed
            but will be executed nevertheless to ensure known state after SCORE_Stop is called. */
        if (DECFEED_E_OK != DECFEED_Stop())
        {
            WARNING("SCORE_Stop: DECFEED_Stop() failed\n");
        }

        /*  Properly close/stop the networking interface */
        if (ETHQ_E_OK != ETHQ_Stop())
        {
            WARNING("SCORE_Stop: ETHQ_Stop() failed\n");
        }

        /*  Invoke full re-configuration */
        /* FIXME: since it is always set here, full reconfiguration will always be executed in initialization. Either:
           a) Remove this variable and corresponding checks.
           b) Consider situations where reconfiguration is not necessary (e.g. stop-init-start sequence after error
              in received stream) */
        bConfigHasChanged = TRUE;
    }
    
    SCORE_rCtl.enState = SCORE_ST_READY;
    /*  Enforce reinitialization */
    bInitialized = FALSE;

#ifdef ZDDEBUG
    PrintDebugMsgs();
#endif

    return eRetVal;
}

/**
* @brief        Checks whether all required configuration parameters were set.
* @retval       SCORE_E_INVALID_CONFIG Error - At least one parameter was not set.
* @retval       SCORE_E_OK Success.
*/
stream_core_ret_t SCORE_CheckConfig(void)
{
    stream_core_ret_t enRetVal = SCORE_E_OK;
    uint32_t u32ii;
    
    /*  TODO: Perform configuration check to ensure that module setup is done */
    if (FALSE == rStreamCfgFlags.bFetchSOIThresholdSet)
    {
        ERROR("[SCORE] FetchSOIThreshold not set.\n");
        enRetVal = SCORE_E_INVALID_CONFIG;
    }
    
    if (FALSE == rStreamCfgFlags.bSOIMarkerValueSet)
    {
        ERROR("[SCORE] SOIMarkerValue not set.\n");
        enRetVal = SCORE_E_INVALID_CONFIG;
    }
    
    if (FALSE == rStreamCfgFlags.bSOIMarkerMaskSet)
    {
        ERROR("[SCORE] SOIMarkerMask not set.\n");
        enRetVal = SCORE_E_INVALID_CONFIG;
    }
    
    if (FALSE == rStreamCfgFlags.bSOIMarkerOffsetSet)
    {
        ERROR("[SCORE] SOIMarkerOffset not set.\n");
        enRetVal = SCORE_E_INVALID_CONFIG;
    }
    
    if (FALSE == rStreamCfgFlags.bSequenceNumOffsetSet)
    {
        ERROR("[SCORE] SequenceNumOffset not set.\n");
        enRetVal = SCORE_E_INVALID_CONFIG;
    }
    
    if (FALSE == rStreamCfgFlags.bStreamIDOffsetSet)
    {
        ERROR("[SCORE] StreamIDOffset not set.\n");
        enRetVal = SCORE_E_INVALID_CONFIG;
    }
    
    if (FALSE == rStreamCfgFlags.bFrmDataOffsetSet)
    {
        ERROR("[SCORE] FrmDataOffset not set.\n");
        enRetVal = SCORE_E_INVALID_CONFIG;
    }
    
    if (FALSE == rStreamCfgFlags.bNumberOfStreamsSet)
    {
        ERROR("[SCORE] NumberOfStreams not set.\n");
        enRetVal = SCORE_E_INVALID_CONFIG;
    }
    else
    {
        for (u32ii=0U; u32ii<rStreamCfgPrepare.u8NumberOfStreams; u32ii++)
        {
            if (FALSE == rStreamCfgFlags.abStreamIdSet[u32ii])
            {
                ERROR("[SCORE] StreamId[%d] not set.\n", u32ii);
                enRetVal = SCORE_E_INVALID_CONFIG;
            }
        }
    }
    
    if (FALSE == rStreamCfgFlags.bVlanPrioCntSet)
    {
        ERROR("[SCORE] VlanPrioCnt not set.\n");
        enRetVal = SCORE_E_INVALID_CONFIG;
    }
    
    if (FALSE == rStreamCfgFlags.bDropOutThresholdSet)
    {
        ERROR("[SCORE] DropOutThreshold not set.\n");
        enRetVal = SCORE_E_INVALID_CONFIG;
    }
    
    return enRetVal;
}

/**
* @brief        Performs single iteration of autonomous stream processing.
* @details      Once everything is configured, SCORE_Start was successfully executed,
*               and stream packets are coming into our Ethernet queue,
*               stream_core is able to work autonomously. It requires just periodical calling of this function
*               and checking stream_core state accessible through function SCORE_GetCurrentState.
*               In case of error: main application shall be notified and calling of this function stopped.
*/
void SCORE_Iteration(void)
{
    uint8_t u8StreamIdx;
    
    /* Run state specific streaming code */
    switch(SCORE_rCtl.enState)
    {
        case SCORE_ST_READY:
            /* This should not happen, PIT shall not run (iteration shall not run) in this state */
            SCORE_rCtl.u32ErrMask |= SCORE_WAR_INACTIVE_NOT_SUSPENDED;
        break;
        case SCORE_ST_FETCH:
            FetchIteration();
            if(SCORE_ST_RUN == SCORE_rCtl.enState)
            {   /* Fetching was finished */
                if (DECFEED_E_OK != DECFEED_Start())
                {
                    SCORE_rCtl.u32ErrMask |= SCORE_ERR_DECODER_ERR;
                    SCORE_rCtl.enState = SCORE_ST_ERROR;
                    ERROR("SCORE_Iteration: DECFEED_Start() failed\n");
                }
                else
                {
                    /* Do first RUN iteration immediately */
                    RunIteration();
                }
            }
        break;
        case SCORE_ST_PRERUN:
            switch (ETHQ_PreRunIteration())
            {
                case ETHQ_E_TIMEOUT:
                    /* Failed, time-out */
                    SCORE_rCtl.u32ErrMask |= SCORE_ERR_PREINIT_FAILED;
                    SCORE_rCtl.enState = SCORE_ST_ERROR;
                    ERROR("SCORE_Iteration: PRERUN failed to synchronize with Ethernet Rx queue\n");
                    break;
                case ETHQ_E_NOT_INITIALIZED:
                    SCORE_rCtl.u32ErrMask |= SCORE_ERR_ETHQ_ERROR;
                    SCORE_rCtl.enState = SCORE_ST_ERROR;
                    ERROR("SCORE_Iteration: ETHQ_PreRunIteration() failed\n");
                    break;
                case ETHQ_E_OK:
                    /* DONE */
                    SCORE_rCtl.enState = SCORE_ST_FETCH;
                    /* Initial values for checking stream drop-outs */
                    for( u8StreamIdx = 0U; u8StreamIdx < SCORE_rStrmCfg.u8NumberOfStreams; u8StreamIdx++)
                    {
                        arStreamBDRingTrack[u8StreamIdx].u32LastRxIter = u32IterCtr;
                    }
                    break;
                default:
                    break; /* Else we will try again later */
            }
        break;
        case SCORE_ST_RUN:
            RunIteration();
        break;
        default: /* SCORE_ST_ERROR and all invalid values */
            /* Automatically recover from errors which are recoverable without reconfiguration */
            if (TRUE == SCORE_IS_RECOVERABLE(SCORE_rCtl.u32ErrMask))
            {
                boolean bError = FALSE;
                NOTICE("Automatically recovering from error(s), the error mask was: 0x%x\n", SCORE_rCtl.u32ErrMask);
                SCORE_rCtl.u32ErrMask &= SCORE_ERR_WARNINGS_MASK; /* Keep only warnings */
                SCORE_rCtl.u32ErrMask |= SCORE_WAR_AUTOMATIC_RESTART;
                
                if(SCORE_E_OK != SCORE_Stop())
                {
                    bError = TRUE;
                }
                else if(SCORE_E_OK != SCORE_Init())
                {
                    bError = TRUE;
                }
                else if(SCORE_E_OK != SCORE_Start())
                {
                    bError = TRUE;
                }
                else
                {
                    /* Intentionally empty */
                }
                if(TRUE == bError)
                {
                    SCORE_rCtl.u32ErrMask |= SCORE_ERR_NOT_INITIALIZED;
                    SCORE_rCtl.enState = SCORE_ST_ERROR;
                    ERROR("SCORE_Iteration: Failed to automatically restart streaming\n");
                }
            }
            else
            {
                SCORE_rCtl.u32ErrMask |= SCORE_WAR_INACTIVE_NOT_SUSPENDED;
            }
        break;
    }
#ifdef ZDDEBUG
    PrintDebugMsgs();
#endif
    u32IterCtr++;
}

/*================================================================================================*/
#ifdef __cplusplus
}
#endif
/** @}*/
