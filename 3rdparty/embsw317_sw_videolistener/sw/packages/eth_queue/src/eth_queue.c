/**
  @defgroup         dxgrETHQ Ethernet Queue Driver
  @details          Purpose of the Ethernet Queue module is to provide a partial data and control
                    interface to the ENET networking peripheral within handling of a single allocated
                    HW queue.
                    
                    Architecture
                    ------------
                    The ENET peripheral contains multiple queues used for data reception. ENET driver
                    configures, enables and takes care about the peripheral and may use various
                    number of RX queues. This driver is aimed only to queue handling without need
                    to manage the complete ENET peripheral. It means that this driver can be used
                    only within an environment where some full ENET driver takes care about the HW and
                    a single RX queue is allocated to be handled by this Ethernet Queue module.
                    
                    Theory of operation
                    -------------------
                    Before the module can be used it must be properly initialized by setting values
                    of configuration macros within the eth_queue_cfg.h and providing additional
                    runtime configuration via:
                    - @ref ETHQ_SetBaseAddr()
                    - @ref ETHQ_SetRegionLength()
                    - @ref ETHQ_SetBufferSize()
                    - @ref ETHQ_SetRingAddr()
                    
                    Once configuration is properly prepared, module can be initialized by the
                    @ref ETHQ_Init() call.
                    
                    @remark Since the associated HW queue has not been configured to receive any
                            Ethernet traffic yet, user should at this point enable the reception.
                            It is done via @ref ETHQ_AddVlanClassification() by setting VLAN
                            priority(ies) identifying traffic to be received into the managed queue.
                    
                    Due to nature of the HW, module needs to synchronize its internal state
                    with the HW queue to know entry indicating start of the ring. For this purpose
                    the module provides @ref ETHQ_PreRunIteration() API which needs to be called
                    upon initialization periodically until the pre-run phase is finished (indicated
                    by the ETHQ_E_OK return value). From this point the queue is ready to receive
                    Ethernet frames.
                    
                    Frame reception is done via the @ref ETHQ_GetNextRxBDIdx() returning index
                    of buffer descriptor of a received frame. Each received frame is locked and
                    user shall unlock it by @ref ETHQ_UnlockRxBD() as soon as possible to enable
                    further reception. User should also call @ref ETHQ_WriteRDAR() each time when
                    some descriptor(s) have been unlocked to keep the reception running.
                    
                    The @ref ETHQ_RemoveAllVlanClassifications() can be called to stop reception
                    by instructing the VLAN classifier to not accept any frames. To stop the module
                    with FIFO flush and cleaning the queue the @ref ETHQ_Stop() shall be used.
                    
  
  @addtogroup       dxgrETHQ
  @{
  
  @file             eth_queue.c
  @brief            Ethernet Rx Queue driver
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
* @section eth_queue_c_global
* Violates MISRA 2004 Required Rule 20.2, The names of standard library macros, objects and functions
* shall not be reused.
* Identifiers beginning with 'E' used to match their names with the Ethernet peripheral.
*
* @section eth_queue_c_REF_1
* Violates MISRA 2004 Advisory Rule 11.3, A cast should not be preformed between a pointer type and
* an integral type. Cast is used to access memory mapped resources.
*
* @section eth_queue_c_REF_2
* Violates MISRA 2004 Advisory Rule 11.3, A cast should not be preformed between a pointer type and
* an integral type. Cast is used to get and use address of an object as an integer value.
*
* @section eth_queue_c_REF_3
* Violates MISRA 2004 Advisory Rule 11.4, A cast should not be performed between a pointer to object
* and a different pointer to object type.
* Cast from pointer to pointer is due to accessing a part of byte array as 4-byte wide value.
*
* @section eth_queue_c_REF_4
* Violates MISRA 2004 Required Rule 1.2, No reliance shall be placed on undefined or unspecified
* behaviour.
* This is false detection. The behaviour is perfectly defined here.
* Pointer to the local variable which is passed to the function is used only to return a value from
* that function and is in no way used after the function returns. 
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
#include "mmap.h"
#include "eth_queue_cfg.h"
#include "eth_queue.h"
#include "autolibc.h"

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
#if !defined (ETHQ_CFG_INIT_CHECK)
    #error Please define the ETHQ_CFG_INIT_CHECK value
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
* @struct   SCORE_trEthCfg
* @brief    Ethernet queue configuration data structure (we need this data from ETH controller driver)
* @details  It is used to communicate and store configuration parameters to streaming application.
*           This structure must be written by the Streaming Application before any ETHQ function is run.
*/
typedef struct
{
    uint32_t u32BaseAddr;
    uint32_t u32RegionLength;
    uint32_t u32BuffRingAddr;
    uint16_t u16SizeOfBuff;
    boolean bInitialized;
} ETHQ_trCfg;

typedef enum
{
    PRERUN_ST_INACTIVE = 0,
    PRERUN_ST_SEARCHING = 1,
    PRERUN_ST_FIFOFLUSH = 2
} tenPrerunStage;

/*==================================================================================================
                                       LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       LOCAL VARIABLES
==================================================================================================*/
/*  This variable is used in function ETHQ_PreRunIteration.
    Function ETHQ_Init must be able to reset its value. */
static tenPrerunStage enPrerunStage = PRERUN_ST_INACTIVE;

/*==================================================================================================
                                       GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL VARIABLES
==================================================================================================*/
/*
* @var      ETHQ_aau8BDRing
* @brief    Ethernet Rx queue buffer descriptor ring
* @details  This structure must be written by the Streaming Application before any ETHQ function is
*           run. Otherwise the behaviour is not defined.
*/
uint8_t ETHQ_aau8BDRing[ETHQ_CFG_RX_BD_RING_LEN][ETHQ_RX_BD_LENGTH] MMAP_DATA_NOCACHE(32);

/*
* @var      ETHQ_rCfg
* @brief    Configuration of Ethernet Queue driver
* @details  This structure must be written by the Streaming Application before any ETHQ function is
*           run. Otherwise the behaviour is not defined. The values are initialized to zeroes and they
*           shall be written from user-space application before running ETHQ_Init. Values in this
*           structure are not expected to be changed once the are set.
*/
static ETHQ_trCfg ETHQ_rCfg = {0U, 0U, 0U, 0U, FALSE};
/*
* @var      ETHQ_u16RxBDReadIdx
* @brief    Index of Rx BD which shall be read next
* @details  The value is not valid when program is started and after ETHQ_Init() or ETHQ_CleanAll()
*           function is called. The value must be determined before streaming is started. This is
*           done in PRERUN state. Once the value is correct, streaming can switch to FETCH state.
*/
uint16_t ETHQ_u16RxBDReadIdx;

/*==================================================================================================
                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
static void ETHQ_LockAll(void);
static void ETHQ_UnlockAll(void);
static void ETHQ_UnlockAllExcept(uint16_t u16KeptIdx);
static void ETHQ_PrerunUnlockEvenOdd(boolean bIsEven);
static sint32_t ETHQ_PrerunFindEvenOdd(boolean bIsEven);
static boolean ETHQ_IsRxBDLocked(uint32_t u32BDIdx);
static void ETHQ_LockRxBD(uint32_t u32BDIdx);
static boolean ETHQ_ReadRDAR(void);
static void ETHQ_WriteBdRingAddr(tuAddr Addr);
static void ETHQ_WriteMaxFrmSize(uint32_t u32Size);

/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/
/*
* @brief    Loop through all BDs and locks them.
* @note     Call to this function makes the queue unusable for reception.
*           It shall be used only in PRE-RUN mode.
*/
static void ETHQ_LockAll(void)
{
    uint32_t u32ii;
    
    for (u32ii=0U; u32ii<ETHQ_CFG_RX_BD_RING_LEN; u32ii++)
    {
        ETHQ_LockRxBD(u32ii);
    }
}

/*
* @brief        Loop through all BDs and make empty all of them except u16KeptIdx.
* @param[in]    u16KeptIdx Index of BD which shall remain locked.
* @note         Call to this function makes the queue unusable for reception.
*               It shall be used only in PRE-RUN mode.
*/
static void ETHQ_UnlockAllExcept(uint16_t u16KeptIdx)
{
    uint32_t u32ii;
    
    for (u32ii=0U; u32ii<ETHQ_CFG_RX_BD_RING_LEN; u32ii++)
    {
        if(u16KeptIdx != u32ii)
        {
            ETHQ_UnlockRxBD(u32ii);
        }
    }
}

static void ETHQ_PrerunUnlockEvenOdd(boolean bIsEven)
{
    uint32_t u32ii;
    
    for(u32ii = ((TRUE == bIsEven) ? 0U : 1U); u32ii < ETHQ_CFG_RX_BD_RING_LEN; u32ii += 2U)
    {
        ETHQ_UnlockRxBD(u32ii);
    }
}

static sint32_t ETHQ_PrerunFindEvenOdd(boolean bIsEven)
{
    uint32_t u32ii;
    boolean bFound = FALSE;
    
    for(u32ii = ((TRUE == bIsEven) ? 0U : 1U); u32ii < ETHQ_CFG_RX_BD_RING_LEN; u32ii += 2U)
    {
        if(TRUE == ETHQ_IsRxBDLocked(u32ii))
        {   /* Found received (non-empty) BD */
            /* We have to return the next one which is active now */
            u32ii++;
            if(u32ii >= ETHQ_CFG_RX_BD_RING_LEN)
            {
                u32ii = 0U;
            }
            
            bFound = TRUE;
            break;
        }
    }
    
    return (TRUE == bFound) ? (sint32_t)u32ii : -1;
}

/*
* @brief    Loop through all BDs and make them empty
* @note     Call to this function makes the queue unusable for reception.
*           It shall be used only in PRE-RUN mode.
*/
static void ETHQ_UnlockAll(void)
{
    uint32_t u32ii;
    
    for (u32ii=0U; u32ii<ETHQ_CFG_RX_BD_RING_LEN; u32ii++)
    {
        ETHQ_UnlockRxBD(u32ii);
    }
}

static boolean ETHQ_IsRxBDLocked(uint32_t u32BDIdx)
{
    return (0U == (0x80U & ETHQ_aau8BDRing[u32BDIdx][ENET_RXBD_EMPTY_OFFSET])) ? TRUE : FALSE;
}

static void ETHQ_LockRxBD(uint32_t u32BDIdx)
{
    (ETHQ_aau8BDRing[u32BDIdx][ENET_RXBD_EMPTY_OFFSET] &= (uint8_t)~0x80U);
}

static boolean ETHQ_ReadRDAR(void)
{
    /** @violates @ref eth_queue_c_REF_1 MISRA rule 11.3 */
    return (0U != RREG(ETHQ_rCfg.u32BaseAddr+ETHQ_RDAR_OFFSET)) ? TRUE : FALSE;
}

static void ETHQ_WriteBdRingAddr(tuAddr Addr)
{
    /** @violates @ref eth_queue_c_REF_1 MISRA rule 11.3 */
    WREG(ETHQ_rCfg.u32BaseAddr+FEC_R_DES_START, (uint32_t)Addr);
}

static void ETHQ_WriteMaxFrmSize(uint32_t u32Size)
{
    /** @violates @ref eth_queue_c_REF_1 MISRA rule 11.3 */
    WREG(ETHQ_rCfg.u32BaseAddr+FEC_MRBR, u32Size);
}

/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/
/**
* @brief        Checks the module configuration for typical errors.
* @retval       ETHQ_E_INVALID_CONFIG Error - At least one parameter has wrong value.
* @retval       ETHQ_E_OK Success.
*/
eth_queue_ret_t ETHQ_CheckConfig(void)
{
    eth_queue_ret_t eRetVal = ETHQ_E_OK;
    
    if (0U == ETHQ_rCfg.u32BaseAddr)
    {
        eRetVal = ETHQ_E_INVALID_CONFIG;
    }
    else if (0U == ETHQ_rCfg.u32RegionLength)
    {
        eRetVal = ETHQ_E_INVALID_CONFIG;
    }
    else if (0U == ETHQ_rCfg.u16SizeOfBuff)
    {
        eRetVal = ETHQ_E_INVALID_CONFIG;
    }
    else if (0U == ETHQ_rCfg.u32BuffRingAddr)
    {
        eRetVal = ETHQ_E_INVALID_CONFIG;
    }
    else
    {
        ;   /*  Intentionally emtpy */
    }
    
    return eRetVal;
}

/**
* @brief    This function returns index of next Rx BD that shall be read first
* @details  If there is at least one received frame, then it returns index of Rx BD of first of those
*           frames and increments internal counter. Next time this function is called, BD index of another
*           frame is returned.
* @return   It returns negative number if there is no received frame, non-negative BD index otherwise.
* @note     Once the caller obtains a valid index, it is responsible for unlocking it.
*/
eth_queue_ret_t ETHQ_GetNextRxBDIdx(sint16_t *ps16Idx)
{
    eth_queue_ret_t eRetVal = ETHQ_E_OK;
    
#if (TRUE == ETHQ_CFG_INIT_CHECK)
    if (FALSE == ETHQ_rCfg.bInitialized)
    {
        eRetVal = ETHQ_E_NOT_INITIALIZED;
    }
    else
#endif /* ETHQ_CFG_INIT_CHECK */
    {
        if( TRUE == ETHQ_IsRxBDLocked((uint32_t)ETHQ_u16RxBDReadIdx) )
        {   /* The BD is locked */
            *ps16Idx = (sint16_t)ETHQ_u16RxBDReadIdx;
            ETHQ_u16RxBDReadIdx = (ETHQ_u16RxBDReadIdx+1U) % ETHQ_CFG_RX_BD_RING_LEN;
        }
        else
        {   /* There is no locked BD */
            *ps16Idx = -1;
        }
    }

    return eRetVal;
}

/**
* @brief        Read configured base address and length of Ethernet controller register region
* @details      It also checks validity of the configuration. Values are provided
*               only if the check passed.
* @param[out]   pu32Base Register region base address will be written here.
* @param[out]   pu32Length Register region length will be written here.
* @return       If configuration check fails, then return value from ETHQ_CheckConfig is
*               passed. Otherwise this function returns ETHQ_E_OK.
*/
eth_queue_ret_t ETHQ_GetMemoryBase(uint32_t *pu32Base, uint32_t *pu32Length)
{
    eth_queue_ret_t eRetVal = ETHQ_CheckConfig();
    
    if (ETHQ_E_OK == eRetVal)
    {
        *pu32Base = ETHQ_rCfg.u32BaseAddr;
        *pu32Length = ETHQ_rCfg.u32RegionLength;
    }
    
    return eRetVal;
}

/**
* @brief        Configure base address of Ethernet controller register region.
* @param[in]    u32Addr Physical base address.
* @retval       ETHQ_E_MULTIPLE_CONFIG Error - attempt to change the address.
* @retval       ETHQ_E_OK Success.
* @note         Once the address is set, it may not be changed.
*/
eth_queue_ret_t ETHQ_SetBaseAddr(uint32_t u32Addr)
{
    eth_queue_ret_t eRetVal = ETHQ_E_MULTIPLE_CONFIG;
    
    if ((0U == ETHQ_rCfg.u32BaseAddr) || (ETHQ_rCfg.u32BaseAddr == u32Addr))
    {
        ETHQ_rCfg.u32BaseAddr = u32Addr;
        eRetVal = ETHQ_E_OK;
    }
    
    return eRetVal;
}

/**
* @brief        Configure length of Ethernet controller register region.
* @param[in]    u32Length Length in bytes.
* @retval       ETHQ_E_MULTIPLE_CONFIG Error - attempt to change the length.
* @retval       ETHQ_E_OK Success.
* @note         Once the length is set, it may not be changed.
*/
eth_queue_ret_t ETHQ_SetRegionLength(uint32_t u32Len)
{
    eth_queue_ret_t eRetVal = ETHQ_E_MULTIPLE_CONFIG;
    
    if ((0U == ETHQ_rCfg.u32RegionLength) || (ETHQ_rCfg.u32RegionLength == u32Len))
    {
        ETHQ_rCfg.u32RegionLength = u32Len;
        eRetVal = ETHQ_E_OK;
    }
    
    return eRetVal;
}

/**
* @brief        Read configured address and length of Ethernet Rx buffers region
* @details      It also checks validity of the configuration. Values are provided
*               only if the check passed.
* @param[out]   pu32Base Rx buffer region address will be written here.
* @param[out]   pu32Length Rx buffer region length will be written here.
* @return       If configuration check fails, then return value from ETHQ_CheckConfig is
*               passed. Otherwise this function returns ETHQ_E_OK.
*/
eth_queue_ret_t ETHQ_GetBufferBase(uint32_t *pu32Base, uint32_t *pu32Length)
{
    eth_queue_ret_t eRetVal = ETHQ_CheckConfig();
    
    if (ETHQ_E_OK == eRetVal)
    {
        *pu32Base = ETHQ_rCfg.u32BuffRingAddr;
        *pu32Length = (uint32_t)ETHQ_rCfg.u16SizeOfBuff * (uint32_t)ETHQ_CFG_RX_BD_RING_LEN;
    }
    
    return eRetVal;
}

/**
* @brief        Configure length of Ethernet Rx buffers region.
* @param[in]    u32Length Length in bytes.
* @retval       ETHQ_E_MULTIPLE_CONFIG Error - attempt to change the length.
* @retval       ETHQ_E_OK Success.
* @note         Once the length is set, it may not be changed.
*/
eth_queue_ret_t ETHQ_SetBufferSize(uint32_t u32BufSize)
{
    eth_queue_ret_t eRetVal = ETHQ_E_INVALID_CONFIG;
    
    if (0U != (u32BufSize / ETHQ_CFG_RX_BD_RING_LEN))
    {
        ETHQ_rCfg.u16SizeOfBuff = (uint16_t)(u32BufSize / ETHQ_CFG_RX_BD_RING_LEN);
        eRetVal = ETHQ_E_OK;
    }
    
    return eRetVal;
}

/**
* @brief        Configure address of Ethernet Rx buffers region.
* @param[in]    u32Addr Physical address.
* @retval       ETHQ_E_MULTIPLE_CONFIG Error - attempt to change the address.
* @retval       ETHQ_E_OK Success.
* @note         Once the address is set, it may not be changed.
*/
eth_queue_ret_t ETHQ_SetRingAddr(uint32_t u32Addr)
{
    eth_queue_ret_t eRetVal = ETHQ_E_MULTIPLE_CONFIG;
    
    if ((0U == ETHQ_rCfg.u32BuffRingAddr) || (ETHQ_rCfg.u32BuffRingAddr == u32Addr))
    {
        ETHQ_rCfg.u32BuffRingAddr = u32Addr;
        eRetVal = ETHQ_E_OK;
    }
    
    return eRetVal;
}

/**
* @brief    Initialization of Ethernet Rx queue.
* @details  Performs configuration of Rx queue in Ethernet controller. Initializes
*           Rx buffer descriptor (BD) ring.
* @retval   ETHQ_E_INVALID_CONFIG Error - Configuration checking failed.
* @retval   ETHQ_E_OK Success.
*/
eth_queue_ret_t ETHQ_Init(void)
{
    /*  The ETHQ_bShallInit indicates whether to (re)initialize this Ethernet queue driver. If ETH
        controller is (re)initialized, the queue shall be also (re)initialized. It is true in the
        beginning. TODO: make it become true also when the Linux driver is restarted (this
        information shall be passed from upper layers). */
    static boolean ETHQ_bShallInit = TRUE;
    uint32_t u32BDIdx;
    eth_queue_ret_t eRetVal = ETHQ_E_OK;

    /* PRE-RUN must start from beginning */
    enPrerunStage = PRERUN_ST_INACTIVE;
    
    if (ETHQ_E_OK != ETHQ_CheckConfig())
    {
        ETHQ_rCfg.bInitialized = FALSE;
        ETHQ_bShallInit = TRUE;
        eRetVal = ETHQ_E_INVALID_CONFIG;
    }
    else if( TRUE == ETHQ_bShallInit )
    {
        /* We cannot reinitialize it any time because we are tracking the Rx BD index */
        /* Avoid unwanted reinitialization */
        ETHQ_bShallInit = FALSE;
        /* Write ENET registers */
        /** @violates @ref eth_queue_c_REF_2 MISRA rule 11.3 */
        ETHQ_WriteBdRingAddr((tuAddr)ETHQ_aau8BDRing);
        ETHQ_WriteMaxFrmSize((uint32_t)ETHQ_rCfg.u16SizeOfBuff);
        /* Initialize Rx BDs */
        /* Clear all BDs at once */
        fsl_memset(ETHQ_aau8BDRing, 0U, sizeof(ETHQ_aau8BDRing));
        /* For each BD */
        for(u32BDIdx=0U; u32BDIdx<ETHQ_CFG_RX_BD_RING_LEN; u32BDIdx++)
        {
            /* Note that EMPTY bits are not written. Streaming application will do that in PRERUN mode */
            /* Update pointers */
            /** @violates @ref eth_queue_c_REF_3 MISRA rule 11.4 */
            ETHQ_RX_BUFFER_ADDR(u32BDIdx) = ETHQ_rCfg.u32BuffRingAddr + (u32BDIdx * ETHQ_rCfg.u16SizeOfBuff);
        }
        /* Add wrap flag to last BD */
        ETHQ_aau8BDRing[ETHQ_CFG_RX_BD_RING_LEN-1U][ENET_RXBD_EMPTY_OFFSET] |= 0x20U;
        ETHQ_rCfg.bInitialized = TRUE;
    }
    else
    {
        ;   /*  Intentionally empty */
    }
    
    return eRetVal;
}

/**
* @brief    This function is called when all Rx frames in the queue shall be discarded.
* @details  It just unlocks all non-empty Rx BDs. This function keeps the queue usable.
* @retval   ETHQ_E_NOT_INITIALIZED Error - Function ETHQ_Init was not run or it failed.
* @retval   ETHQ_E_OK Success.
*/
eth_queue_ret_t ETHQ_Empty(void)
{
    eth_queue_ret_t eRetVal = ETHQ_E_OK;
    
#if (TRUE == ETHQ_CFG_INIT_CHECK)
    if (FALSE == ETHQ_rCfg.bInitialized)
    {
        eRetVal = ETHQ_E_NOT_INITIALIZED;
    }
    else
#endif /* ETHQ_CFG_INIT_CHECK */
    {
        while( TRUE == ETHQ_IsRxBDLocked((uint32_t)ETHQ_u16RxBDReadIdx) )
        {
            ETHQ_UnlockRxBD((uint32_t)ETHQ_u16RxBDReadIdx);
            ETHQ_u16RxBDReadIdx = (ETHQ_u16RxBDReadIdx+1U) % ETHQ_CFG_RX_BD_RING_LEN;
        }
    }
    
    return eRetVal;
}

/**
* @brief    This function sets the Rx queue and this driver into defined state.
* @note     This function requires number of Ethernet buffer descriptors to be odd.
* @details  This function shall be called periodically as long as it returns ETHQ_E_AGAIN. During several iterations it
*           determines active Rx Buffer Descriptor. Then it needs some more iterations to empty the Ethernet FIFO, which
*           might contain old frames. Once it is done, the function returns ETHQ_E_OK and it shall not be called again.
* @retval   ETHQ_E_OK Done
* @retval   ETHQ_E_AGAIN Keep trying
* @retval   ETHQ_E_TIMEOUT Time-out error
* @retval   ETHQ_E_NOT_INITIALIZED Error - Function ETHQ_Init was not run or it failed.
*/
eth_queue_ret_t ETHQ_PreRunIteration(void)
{
    #if (0U != (ETHQ_CFG_RX_BD_RING_LEN % 2U))
        #error "This function requires ETHQ_CFG_RX_BD_RING_LEN to be odd"
    #endif
    #if (0U == ETHQ_CFG_PRERUN_MAX_CYCLES)
        #error "ETHQ_CFG_PRERUN_MAX_CYCLES must be >= 1"
    #endif
    #if (0U == ETHQ_CFG_PRERUN_WAIT_CYCLES)
        #error "ETHQ_CFG_PRERUN_WAIT_CYCLES must be >= 1"
    #endif
    static boolean  bTryingEven; /* Whether even or odd */
    static uint32_t u32WaitCtr;
    static uint32_t u32CycleCtr;
    static uint8_t  u8_MinIterCtr;
    eth_queue_ret_t eRetVal = ETHQ_E_AGAIN;  /* Default: Try again */
    boolean         bPrepare = FALSE;
    sint32_t        s32Result;

#if (TRUE == ETHQ_CFG_INIT_CHECK)
    if (FALSE == ETHQ_rCfg.bInitialized)
    {
        eRetVal = ETHQ_E_NOT_INITIALIZED;
    }
    else
#endif /* ETHQ_CFG_INIT_CHECK */
    {
        if(PRERUN_ST_FIFOFLUSH == enPrerunStage)
        {   /* The final stage */
            sint16_t s16RxBDIdx = -1;

            /** @violates @ref eth_queue_c_REF_4 MISRA rule 1.2 */
            if(ETHQ_E_OK != ETHQ_GetNextRxBDIdx(&s16RxBDIdx))
            {
                eRetVal = ETHQ_E_NOT_INITIALIZED;
            }
            else
            {
                if((0U != u8_MinIterCtr) || (s16RxBDIdx >= 0))
                {   /* Still receiving */
                    if(s16RxBDIdx >= 0)
                    {
                        ETHQ_UnlockRxBD((uint32_t)s16RxBDIdx); /* Unlock the frame */
                        ETHQ_Empty(); /* Unlock the rest if any */
                        ETHQ_WriteRDAR(); /* Just in case */
                    }
                    eRetVal = ETHQ_E_AGAIN; /* Continue waiting for next frames from FIFO */
                }
                else
                {   /* Done */
                    if(ETHQ_E_OK != ETHQ_ResumeVlanClassifications())
                    {
                        eRetVal = ETHQ_E_NOT_INITIALIZED;
                    }
                    else
                    {
                        enPrerunStage = PRERUN_ST_INACTIVE;
                        eRetVal = ETHQ_E_OK;
                    }
                }
                u8_MinIterCtr--;
            }
        }
        else
        {
            if(PRERUN_ST_SEARCHING == enPrerunStage)
            {   /* Check whether frame was received */
                s32Result = ETHQ_PrerunFindEvenOdd(bTryingEven);
                if(s32Result >= 0)
                {   /* Received - DONE searching */
                    ETHQ_u16RxBDReadIdx = (uint16_t)(uint32_t)s32Result;
                    /* Unlock BDs in specific order (active BD is last) */
                    ETHQ_UnlockAllExcept(ETHQ_u16RxBDReadIdx);
                    ETHQ_UnlockRxBD((uint32_t)ETHQ_u16RxBDReadIdx);
                    /* Enable reception */
                    ETHQ_WriteRDAR();
                    /* Switch to FIFO FLUSH stage */
                    enPrerunStage = PRERUN_ST_FIFOFLUSH;
                    u8_MinIterCtr = 3U; /* Force the FIFO flushing stage to take at lease 3 iteration */
                    eRetVal = ETHQ_E_AGAIN; /* Done, but must clear FIFO */
                    if (ETHQ_E_OK != ETHQ_SuspendVlanClassifications())
                    {
                        eRetVal = ETHQ_E_NOT_INITIALIZED;
                    }
                }
                else
                {   /* NOT received yet */
                    if(0U == u32WaitCtr)
                    {   /* End current waiting - consider next waiting */
                        if(bTryingEven)
                        {   /* End of even iteration - middle of cycle - will continue */
                            bPrepare = TRUE;
                        }
                        else
                        {   /* End of odd iteration - whole cycle complete - continue? */
                            u32CycleCtr--;
                            if(0U == u32CycleCtr)
                            {   /* Time out - Abort pre-run and report failure */
                                enPrerunStage = PRERUN_ST_INACTIVE;
                                eRetVal = ETHQ_E_TIMEOUT; /* Failure (time-out) */
                            }
                            else
                            {   /* Will continue */
                                bPrepare = TRUE;
                            }
                        }
                        bTryingEven = (boolean)((TRUE == bTryingEven) ? FALSE : TRUE); /* Switch between even/odd */
                    }
                    else
                    {
                        u32WaitCtr--;
                    }
                }
            }
            else
            {   /* Activate */
                enPrerunStage = PRERUN_ST_SEARCHING;
                u32CycleCtr = ETHQ_CFG_PRERUN_MAX_CYCLES; /* Set limit of pre-run cycles */
                bTryingEven = TRUE;                       /* At first try even BDs */
                bPrepare = TRUE;                          /* Prepare BDs now */
            }
    
            if(bPrepare)
            {
                /* Lock all */
                ETHQ_LockAll();
                /* Unlock the odd/even */
                ETHQ_PrerunUnlockEvenOdd(bTryingEven);
                /* Enable reception */
                ETHQ_WriteRDAR();
                /* Reset wait counter */
                u32WaitCtr = ETHQ_CFG_PRERUN_WAIT_CYCLES;
            }
        }
    }
    
    return eRetVal;
}

/**
* @brief        Enables given VLAN priority in Rx filter of used Ethernet queue.
* @details      Up to 4 various VLAN priorities may be added. Incoming Ethernet frames with
*               matching priority will then be received into used Ethernet queue.
* @param[in]    u8PCP VLAN priority to be enabled in VLAN classifier.
* @retval       ETHQ_E_OK Done
* @retval       ETHQ_E_MULTIPLE_CONFIG Error - This value was already added.
* @retval       ETHQ_E_OVERFLOW Error - Failed to add, table is full.
* @retval       ETHQ_E_NOT_INITIALIZED Error - Function ETHQ_Init was not run or it failed.
*/
eth_queue_ret_t ETHQ_AddVlanClassification(uint8_t u8PCP)
{
    uint8_t au8PCPs[4];
    uint8_t u8FirstFree;
    uint32_t u32RegVal;
    uint32_t u32Idx;
    eth_queue_ret_t eRetVal = ETHQ_E_OK;

#if (TRUE == ETHQ_CFG_INIT_CHECK)
    if (FALSE == ETHQ_rCfg.bInitialized)
    {
        eRetVal = ETHQ_E_NOT_INITIALIZED;
    }
    else
#endif /* ETHQ_CFG_INIT_CHECK */
    {
        /** @violates @ref eth_queue_c_REF_1 MISRA rule 11.3 */
        u32RegVal = RREG(ETHQ_rCfg.u32BaseAddr + FEC_RCMR);

        if (0U != (u32RegVal & RCMR_MATCHEN))
        {
            u8FirstFree = 4U;
            /* Read the current state */
            au8PCPs[0U] = (uint8_t)((u32RegVal >> 0U) & 0x7U);
            au8PCPs[1U] = (uint8_t)((u32RegVal >> 4U) & 0x7U);
            au8PCPs[2U] = (uint8_t)((u32RegVal >> 8U) & 0x7U);
            au8PCPs[3U] = (uint8_t)((u32RegVal >> 12U) & 0x7U);
            if (au8PCPs[0U] == au8PCPs[3U])
            {
                u8FirstFree--;
            }
            
            if (au8PCPs[0U] == au8PCPs[2U])
            {
                u8FirstFree--;
            }
            
            if (au8PCPs[0U] == au8PCPs[1U])
            {
                u8FirstFree--;
            }
            
            /* Check duplicity */
            for(u32Idx=0U; u32Idx<u8FirstFree; u32Idx++)
            {
                if(u8PCP == au8PCPs[u32Idx])
                {
                    eRetVal = ETHQ_E_MULTIPLE_CONFIG; /* Already added */
                }
            }
            
            if((ETHQ_E_OK == eRetVal) && (4U == u8FirstFree))
            {
                eRetVal = ETHQ_E_OVERFLOW; /* Table full */
            }
        }
        else
        {
            u8FirstFree = 0U;
        }
        
        if (ETHQ_E_OK == eRetVal)
        {
            /* Add new */
            au8PCPs[u8FirstFree] = u8PCP;
            /* Make sure that unused fields equal the first */
            for(u32Idx = (uint32_t)(u8FirstFree+1UL); u32Idx < 4UL; u32Idx++)
            {
                au8PCPs[u32Idx] = au8PCPs[0U];
            }
            
            /* Set the new register value */
            u32RegVal = RCMR_MATCHEN;
            for(u32Idx = 0U; u32Idx < 4U; u32Idx++)
            {
                u32RegVal |= (uint32_t)(RCMR_CMP_CFG(au8PCPs[u32Idx], u32Idx));
            }
            
            /** @violates @ref eth_queue_c_REF_1 MISRA rule 11.3 */
            WREG(ETHQ_rCfg.u32BaseAddr + FEC_RCMR, u32RegVal);
        }
    }

    return eRetVal;
}

/**
* @brief        Disables given VLAN priority in Rx filter of used Ethernet queue.
* @details      Removes a value that was previously added. Ethernet frames with given
*               VLAN priority will no longer be received, except frames that are already
*               in Ethernet controller FIFO.
* @param[in]    u8PCP VLAN priority to be disabled in VLAN classifier.
* @retval       ETHQ_E_OK Done
* @retval       ETHQ_E_NOT_FOUND Error - There is no such priority in the table.
* @retval       ETHQ_E_NOT_INITIALIZED Error - Function ETHQ_Init was not run or it failed.
*/
eth_queue_ret_t ETHQ_RemoveVlanClassification(uint8_t u8PCP)
{
    uint8_t au8PCPs[4U];
    uint8_t u8LastUsed;
    uint32_t u32RegVal;
    eth_queue_ret_t eRetVal = ETHQ_E_NOT_FOUND;
    uint32_t u32Idx;

#if (TRUE == ETHQ_CFG_INIT_CHECK)
    if (FALSE == ETHQ_rCfg.bInitialized)
    {
        eRetVal = ETHQ_E_NOT_INITIALIZED;
    }
    else
#endif /* ETHQ_CFG_INIT_CHECK */
    {
        /** @violates @ref eth_queue_c_REF_1 MISRA rule 11.3 */
        u32RegVal = RREG(ETHQ_rCfg.u32BaseAddr + FEC_RCMR);
        if (u32RegVal & RCMR_MATCHEN)
        {   /* There is at least one valid entry, find/remove matching and reorder rest */
            /* Read the current state */
            au8PCPs[0U] = (uint8_t)((u32RegVal >> 0U) & 0x7U);
            au8PCPs[1U] = (uint8_t)((u32RegVal >> 4U) & 0x7U);
            au8PCPs[2U] = (uint8_t)((u32RegVal >> 8U) & 0x7U);
            au8PCPs[3U] = (uint8_t)((u32RegVal >> 12U) & 0x7U);
            /* Count valid */
            u8LastUsed = 3U;
            if (au8PCPs[0U] == au8PCPs[3U])
            {
                u8LastUsed--;
            }
            
            if (au8PCPs[0U] == au8PCPs[2U])
            {
                u8LastUsed--;
            }
            
            if (au8PCPs[0U] == au8PCPs[1U])
            {
                u8LastUsed--;
            }

            u32Idx = 0U;
            /* Find matching */
            while( u32Idx <= u8LastUsed )
            {
                if(u8PCP == au8PCPs[u32Idx])
                {   /* Found */
                    /* Shift the rest */
                    while( u32Idx < u8LastUsed )
                    {
                        au8PCPs[u32Idx] = au8PCPs[u32Idx+1U];
                        u32Idx++;
                    }
                    /* And pad with first entry value */
                    while( u32Idx < 4U )
                    {
                        au8PCPs[u32Idx] = au8PCPs[0U];
                        u32Idx++;
                    }
                    u32RegVal = 0U; /* Default - disable */
                    if( u8LastUsed > 0U )
                    {   /* Add remaining */
                        u32RegVal = RCMR_MATCHEN;
                        for(u32Idx = 0U; u32Idx < 4U; u32Idx++)
                        {
                            u32RegVal |= (uint32_t)(RCMR_CMP_CFG(au8PCPs[u32Idx], u32Idx));
                        }
                    }
                    /* Write changes */
                    /** @violates @ref eth_queue_c_REF_1 MISRA rule 11.3 */
                    WREG(ETHQ_rCfg.u32BaseAddr + FEC_RCMR, u32RegVal);
                    eRetVal = ETHQ_E_OK;
                    break; /* Done */
                }
                u32Idx++;
            }
        }
    }
    return eRetVal;
}

/**
* @brief        Disables all VLAN priorities in Rx filter of used Ethernet queue.
* @details      Removes all values. Ethernet frames will no longer be received, except frames
*               that are already in Ethernet controller FIFO.
* @retval       ETHQ_E_OK Done
* @retval       ETHQ_E_NOT_INITIALIZED Error - Function ETHQ_Init was not run or it failed.
*/
eth_queue_ret_t ETHQ_RemoveAllVlanClassifications(void)
{
    eth_queue_ret_t eRetVal = ETHQ_E_OK;
    
#if (TRUE == ETHQ_CFG_INIT_CHECK)
    if (FALSE == ETHQ_rCfg.bInitialized)
    {
        eRetVal = ETHQ_E_NOT_INITIALIZED;
    }
    else
#endif /* ETHQ_CFG_INIT_CHECK */
    {
        /** @violates @ref eth_queue_c_REF_1 MISRA rule 11.3 */
        WREG(ETHQ_rCfg.u32BaseAddr + FEC_RCMR, 0x0U);
    }
    
    return eRetVal;
}

/**
* @brief        Temporarily disables whole VLAN classifier without clearing table of enabled priorities.
* @details      Ethernet frames will no longer be received, except frames that are already in Ethernet
*               controller FIFO.
* @retval       ETHQ_E_OK Done.
* @retval       ETHQ_E_NOT_INITIALIZED Error - Function ETHQ_Init was not run or it failed.
* @note         If this function is called with empty filter, state of the filter will be undefined
*               after resume function will be called.
* @note         After the filter is suspended, filter configuration must NOT be changed before it is resumed.
*               Otherwise the state of the filter will be undefined.
*/
eth_queue_ret_t ETHQ_SuspendVlanClassifications(void)
{
    eth_queue_ret_t eRetVal = ETHQ_E_OK;
    uint32_t u32RegisterValue;
    
#if (TRUE == ETHQ_CFG_INIT_CHECK)
    if (FALSE == ETHQ_rCfg.bInitialized)
    {
        eRetVal = ETHQ_E_NOT_INITIALIZED;
    }
    else
#endif /* ETHQ_CFG_INIT_CHECK */
    {
        /** @violates @ref eth_queue_c_REF_1 MISRA rule 11.3 */
        u32RegisterValue = RREG(ETHQ_rCfg.u32BaseAddr + FEC_RCMR);
        u32RegisterValue &= ~((uint32_t)RCMR_MATCHEN); /* Clear enable bit */
        /** @violates @ref eth_queue_c_REF_1 MISRA rule 11.3 */
        WREG(ETHQ_rCfg.u32BaseAddr + FEC_RCMR, u32RegisterValue);
    }
    
    return eRetVal;
}

/**
* @brief        Re-enables whole VLAN classifier after it was previously suspended.
* @details      Will restore state before function ETHQ_SuspendVlanClassifications was called.
*               Frames will be received again.
* @retval       ETHQ_E_OK Done
* @retval       ETHQ_E_NOT_INITIALIZED Error - Function ETHQ_Init was not run or it failed.
*/
eth_queue_ret_t ETHQ_ResumeVlanClassifications(void)
{
    eth_queue_ret_t eRetVal = ETHQ_E_OK;
    uint32_t u32RegisterValue;
    
#if (TRUE == ETHQ_CFG_INIT_CHECK)
    if (FALSE == ETHQ_rCfg.bInitialized)
    {
        eRetVal = ETHQ_E_NOT_INITIALIZED;
    }
    else
#endif /* ETHQ_CFG_INIT_CHECK */
    {
        /** @violates @ref eth_queue_c_REF_1 MISRA rule 11.3 */
        u32RegisterValue = RREG(ETHQ_rCfg.u32BaseAddr + FEC_RCMR);
        u32RegisterValue |= (uint32_t)RCMR_MATCHEN; /* Set enable bit */
        /** @violates @ref eth_queue_c_REF_1 MISRA rule 11.3 */
        WREG(ETHQ_rCfg.u32BaseAddr + FEC_RCMR, u32RegisterValue);
    }
    
    return eRetVal;
}

/**
* @brief        Stops reception on used queue and empties Ethernet controller FIFO.
* @retval       ETHQ_E_OK Done
* @retval       ETHQ_E_NOT_INITIALIZED Error - Function ETHQ_Init was not run or it failed.
* @note         This function takes longer time, it busy-waits for the Ethernet FIFO to flush.
*/
eth_queue_ret_t ETHQ_Stop(void)
{
    uint32_t u32ii;
    eth_queue_ret_t eRetVal = ETHQ_E_OK;
    
#if (TRUE == ETHQ_CFG_INIT_CHECK)
    if (FALSE == ETHQ_rCfg.bInitialized)
    {
        eRetVal = ETHQ_E_NOT_INITIALIZED;
    }
    else
#endif /* ETHQ_CFG_INIT_CHECK */
    {
        /* Stop reception */
        (void)ETHQ_RemoveAllVlanClassifications();
        /* Empty the queue including the ENET FIFO */
        do
        {
            ETHQ_UnlockAll();
            ETHQ_WriteRDAR();
            for (u32ii=0U; u32ii<300U; u32ii++)
            {
                __asm("nop");
            }
            
        } while (0U == ETHQ_ReadRDAR());
        ETHQ_UnlockAll();
    }
    
    return eRetVal;
}

/**
* @brief        Unlocks given Ethernet Rx BD so it can receive another frame.
* @param[in]    u32BDIdx Index of buffer descriptor to be unlocked.
* @retval       ETHQ_E_OK Done
* @retval       ETHQ_E_NOT_INITIALIZED Error - Function ETHQ_Init was not run or it failed.
*/
eth_queue_ret_t ETHQ_UnlockRxBD(uint32_t u32BDIdx)
{
    eth_queue_ret_t eRetVal = ETHQ_E_OK;
    
#if (TRUE == ETHQ_CFG_INIT_CHECK)
    if (FALSE == ETHQ_rCfg.bInitialized)
    {
        eRetVal = ETHQ_E_NOT_INITIALIZED;
    }
    else
#endif /* ETHQ_CFG_INIT_CHECK */
    {
        ETHQ_aau8BDRing[u32BDIdx][ENET_RXBD_EMPTY_OFFSET] |= 0x80U;
    }
    
    return eRetVal;
}

/**
* @brief        Notifies Ethernet controller that there are new free BDs.
* @develop      The Ethernet controller stops if it encounters locked BD. In this case this
*               function will make it resume.
* @retval       ETHQ_E_OK Done
* @retval       ETHQ_E_NOT_INITIALIZED Error - Function ETHQ_Init was not run or it failed.
*/
eth_queue_ret_t ETHQ_WriteRDAR(void)
{
    eth_queue_ret_t eRetVal = ETHQ_E_OK;
    
#if (TRUE == ETHQ_CFG_INIT_CHECK)
    if (FALSE == ETHQ_rCfg.bInitialized)
    {
        eRetVal = ETHQ_E_NOT_INITIALIZED;
    }
    else
#endif /* ETHQ_CFG_INIT_CHECK */
    {
        /** @violates @ref eth_queue_c_REF_1 MISRA rule 11.3 */
        WREG(ETHQ_rCfg.u32BaseAddr+ETHQ_RDAR_OFFSET, 0x01000000UL);
    }
    
    return eRetVal;
}

/*================================================================================================*/
#ifdef __cplusplus
}
#endif
/** @}*/
