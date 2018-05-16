/**
  @defgroup         dxgrSM_CORE SM Core
  @details          This module is intended to be a software platform operating on the highest
                    exception level of the CPU and providing all the basic functionality like
                    memory mapping preparation, stack initialization, installation of vector tables
                    and exception handlers as well as providing linker script describing the memory.
                    Module prepares suitable environment and provides API for user's application(s)
                    which can run hooked to the core.
                    
                    List of provided services includes following items:
                    - provides vector table (sm_vector_table.S) as an internal component intended
                      to be installed by a boot-loader SW into specified memory location before
                      module can be used
                    - performs initial, one-time firmware initialization of the environment and
                      user's applications
                    - provides synchronous exception dispatcher and routes system calls to dedicated
                      user's applications handlers
                    - provides FIQ/vFIQ exception dispatcher and routes interrupt requests to
                      dedicated user's applications handlers
                    - provides mechanism of data exchange with SW running on different exception
                      level(s) in form of additional system call parameters
                    - provides mechanism of asynchronous notifications where hosted user's
                      applications can send messages to SW running on different exception levels
                    - provides text debug output accessible by user's applications and directed
                      to shared memory which can be read by applications on different exception
                      levels/contexts
                      
                    The user's application life cycle
                    ---------------------------------
                    An user's application is linked with the SM core using the @ref SM_ENTRY_POINT macro
                    provided by sm.h where parameter is the user's application entry point function.
                    This function is then called single time once the SM Core has started and can
                    contain various one-time initializations.
                    
                    User's application can then hook-in various SMC system call handlers as well as
                    FIQ handlers using dedicated API (@ref sm_install_sc_handler() or
                    @ref sm_install_fiq_handler()). Handlers are then called each time when appropriate
                    system call or interrupt request will reach the exception level the SM Core
                    is running in.
                    
                    @note Currently there is no shut-down procedure or way how to uninstall already
                          installed handlers.
                    
                    System calls and data exchange format
                    -------------------------------------
                    Module expects system calls done via the SMC instruction with ISS equal to
                    the system call identifier. System call identifiers are used by the dispatcher
                    code to route the SC to appropriate handlers if some are installed. Since the
                    argument of the SMC instruction is used to carry that identifier, data exchange
                    is done using CPU registers X0 and X1. Invoker of the system call shall therefore
                    store current values of X0 and X1 and write the registers by values passed to
                    the SM Core before the SMC instruction is executed:
                    \code
                        ...
                        stp x0, x1, [sp, #-16]!
                        smc #0x1002
                        str x0, x15
                        str x1, x16
                        ldp x0, x1, [sp], #16
                        ...
                    \endcode
                    Register values are then
                    accessible during the SC via the @ref sm_sc_get_params() API in form of the
                    @ref sm_sc_param_t structure where are mapped to its members
                    as: u64Val0 = X0, u64Val1 = X1. Each system call returns its status to the
                    invoker within the X0 register and if an hosted application needs to send some
                    additional data back, it is written into the X1. The SC invoker then should read
                    both registers, check return values and recover original values.
                    
                    The asynchronous notification feature
                    -------------------------------------
                    The SM Core module provides API (@ref sm_send_async_msg()) enabling user's
                    applications to send asynchronous signals and data to SW on different exception
                    levels. The mechanism uses software generated interrupts and the approach
                    is fully configurable within the @ref sm_cfg.h as definition of following
                    items:
                    \code
                        sm_cfg_enable_notification()
                        sm_cfg_send_notification()
                        sm_cfg_confirm_notification()
                        sm_cfg_disable_notification()
                    \endcode
                    By default the feature is disabled. If a SW on different exception level or
                    within different context wants to receive asynchronous notifications from the
                    SM Core environment it needs to enable this feature by performing the
                    SM_CFG_LL_ENABLE_EVENT_KEY system call with a non-zero X0 value. From this point
                    each call of @ref sm_send_async_msg() within the SM Core environment will
                    trigger the notification as defined by configuration (mostly an interrupt).
                    
                    If the feature is enabled the receiving SW shall be prepared to receive
                    such notifications and contain related interrupt handler. If the notification
                    is received the handler must confirm the message by performing the
                    SM_CFG_LL_CONFIRM_EVENT_KEY system call. The call does not need additional
                    parameters but returns the message arguments in X0 and X1 registers as:
                    X0 = Reason, X1 = UsersValue.
                    
                    Once the notification is not needed is shall be disabled by invoking
                    the SM_CFG_LL_ENABLE_EVENT_KEY system call with X0 equal to zero.
                    
                    Integration notes
                    -----------------
                    - Before the module can operate, it must be properly written to memory
                      location defined by the linker script.
                    - Once the module is in the memory and the module's target exception level is
                      EL3 then shall be ensured that the SMC instruction is accessible and executable
                      from lower exception levels considering sending system calls to the module.
                    - First system call performed to reach the SM Core is expected to be the
                      one identified by the SM_CFG_LL_INIT_KEY. This call will also return
                      the address (X1) and size (X0) of the text debug buffer.
                    - For proper shut down of the SM Core module user needs to perform the
                      SM_CFG_LL_SHUTDOWN_KEY system call and check X0 value if success (zero).
                      
                      @warning Re-initialization attempt without shutting the SM core down may
                               lead to MMU faults.

  @addtogroup       dxgrSM_CORE
  @{

  @file             sm.c
  @brief            The SM core
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
* @section sm_c_REF_1
* Violates MISRA 2004 Required Rule 20.2, The names of standard library macros, objects and
* functions shall not be reused.
* Leading underscores defines linker objects.
*
* @section sm_c_REF_2
* Violates MISRA 2004 Required Rule 17.4, Array indexing shall be only allowed form of pointer arithmetic.
* Usage of pointer arithmetic other than array indexing improves the code readability.
*
* @section sm_c_REF_3
* Violates MISRA 2004 Advisory Rule 11.4, A cast should not be performed between a pointer to object
* and a different pointer to object type.
* Cast from pointer to pointer is due to accessing linker-exported symbols.
*
* @section sm_c_REF_4
* Violates MISRA 2004 Required Rule 1.2, No reliance shall be placed on undefined or unspecified
* behaviour. Unusual pointer cast has been reviewed.
*
* @section sm_c_REF_5
* Violates MISRA 2004 Advisory Rule 11.3, A cast should not be preformed between a pointer type and
* an integral type. Cast is used to access memory mapped resources.
*
* @section sm_c_REF_6
* Violates MISRA 2004 Advisory Rule 11.3, A cast should not be preformed between a pointer type and
* an integral type. Cast is used to prevent comparison of pointers.
*
* @section sm_c_REF_7
* Violates MISRA 2004 Required Rule 17.1, Pointer arithmetic shall only be applied to pointers that
* address an array or array element. The pointer addresses array of function pointers.
*
* @section sm_c_REF_8
* Violates MISRA 2004 Required Rule 8.7, Objects shall be defined at block scope if they are only
* accessed from within a single function. This object is linker exported symbol and thus cant be
* defined at block scope.
*
* @section sm_c_REF_9
* Violates MISRA 2004 Required Rule 8.10, All declarations and definitions of objects or functions
* at file scope shall have internal linkage unless external linkage is required. External linkage
* is required due to reference from assembly part of the module.
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
#include "dbgb_output.h"     /* Low level debug functions */
#if (PLATFORM == S32V234)
    #include "gic.h"
#endif /* PLATFORM */
#if (TRUE == SM_CFG_ENABLE_MMU)
    #include "mmu_mem_attr.h"
    #include "mmu_exception.h"
    #include "mmu_cfg.h"
    #include "mmu.h"
#endif /* SM_CFG_ENABLE_MMU */

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/
/** @cond dxINTERNAL_MACROS */
#define Ec_SMC_AARCH64  0x17    /* 0b010111 */
/** @endcond */

/*==================================================================================================
                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
typedef struct {
    uint32_t u32Key;
    sm_handler_t pfHandler;
} sm_handler_entry_t;

typedef unsigned long sm_addr_t;

/*==================================================================================================
                                       LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       LOCAL VARIABLES
==================================================================================================*/
static sm_handler_entry_t arScFncs[SM_CFG_SC_H_COUNT + 1U];
static sm_handler_entry_t arFiqFncs[SM_CFG_FIQ_H_COUNT + 1U];
static sm_sc_param_t rAsyncResStorage;
static sint32_t s32AutoInitStatus = 0;
static uint32_t u32FiqCounter = 0U;
static boolean bAsyncNotificationActive = FALSE;
static boolean bInitialized = FALSE;

/*==================================================================================================
                                       GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL VARIABLES
==================================================================================================*/
/** @violates @ref sm_c_REF_1 MISRA rule 20.2 */
/** @violates @ref sm_c_REF_8 MISRA rule 8.7 */
extern void *__sm_init_vec_start;
/** @violates @ref sm_c_REF_1 MISRA rule 20.2 */
/** @violates @ref sm_c_REF_8 MISRA rule 8.7 */
extern void *__sm_init_vec_end;
/** @violates @ref sm_c_REF_1 MISRA rule 20.2 */
/** @violates @ref sm_c_REF_8 MISRA rule 8.7 */
extern void *__sm_sc_prm_start;
/** @violates @ref sm_c_REF_1 MISRA rule 20.2 */
/** @violates @ref sm_c_REF_8 MISRA rule 8.7 */
extern void *__sm_sc_qr_start;

/*==================================================================================================
                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
static sint32_t sm_install_handler(sm_handler_entry_t *prTable, const uint32_t u32Size, const uint32_t u32Key, const sm_handler_t pfHandler);
void sm_hl_init(void);
uint32_t sm_ll_fiq_handler(const uint32_t u32Vector);
void sm_aarch64_sync_dispatcher(void);

/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/
/*
    @brief Internal function to install a handler function.
*/
static sint32_t sm_install_handler(sm_handler_entry_t *prTable, const uint32_t u32Size, const uint32_t u32Key, const sm_handler_t pfHandler)
{
    uint32_t u32ii;
    sint32_t s32RetVal = SM_E_FAILURE;

    for (u32ii=0U; u32ii<u32Size; u32ii++)
    {
        /** @violates @ref sm_c_REF_2 MISRA rule 17.4 */
        if (NULL == prTable[u32ii].pfHandler)
        {
            /** @violates @ref sm_c_REF_2 MISRA rule 17.4 */
            prTable[u32ii].u32Key = u32Key;
            /** @violates @ref sm_c_REF_2 MISRA rule 17.4 */
            prTable[u32ii].pfHandler = pfHandler;
            
            s32RetVal = SM_E_OK;
            
            break;
        }
    }

    return s32RetVal;
}

/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/
/**
    @brief Install system call handler
    @details API to enable user's application to install its own system call handler
    @param[in] u32Key The SC identifier
    @param[in] pfHandler Pointer to the handler
    @retval SM_E_OK Success
    @retval SM_E_FAILURE Failure
    @implements VLREQ035
    @implements VLREQ039
*/
sint32_t sm_install_sc_handler(const uint32_t u32Key, const sm_handler_t pfHandler)
{
    return sm_install_handler(arScFncs, SM_CFG_SC_H_COUNT, u32Key, pfHandler);
}

/**
    @brief Install FIQ handler
    @details API to enable user's application to install its own FIQ handler
    @param[in] u32IrqID The FIQ identifier
    @param[in] pfHandler Pointer to the handler
    @retval SM_E_OK Success
    @retval SM_E_FAILURE Failure
    @implements VLREQ040
*/
sint32_t sm_install_fiq_handler(const uint32_t u32IrqID, const sm_handler_t pfHandler)
{
    return sm_install_handler(arFiqFncs, SM_CFG_FIQ_H_COUNT, u32IrqID, pfHandler);
}

/**
    @brief Retrieve additional system call parameters
    @details Additional parameters of a system call can be passed to the SM Core
             via dedicated registers at the time the system call is performed. This
             function is intended to get those and provide them to the user's application.
    @return Pointer to structure containing the parameters
    @implements VLREQ041
*/
sm_sc_param_t * sm_sc_get_params(void)
{
    /** @violates @ref sm_c_REF_3 MISRA rule 11.4 */
    /** @violates @ref sm_c_REF_4 MISRA rule 1.2 */
    return (sm_sc_param_t *)&__sm_sc_prm_start;
}

/**
    @brief Set additional system call return value
    @details An user's application may need to return custom value at the end of a system call (for
             example when the system call invoker needs to get some user's application specific
             value). This function provides way how to set up the additional return value
             to be accessible by the current system call invoker.
    @param[in] u64Result Value to be returned to the system call invoker when the system call
                         is finished
    @implements VLREQ042
*/
void sm_sc_set_query_result(uint64_t u64Result)
{
    /* Use the second register to transfer the data,
       the first one is reserved for return values */
    /** @violates @ref sm_c_REF_3 MISRA rule 11.4 */
    /** @violates @ref sm_c_REF_4 MISRA rule 1.2 */
    ((sm_sc_param_t *)&__sm_sc_qr_start)->u64Val1 = u64Result;
}

/**
    @brief Send asynchronous message to the current core
    @details This is the way how an user's application can notify current core that some
             event has occurred or when just needs to send some data to SW at different
             exception level. The receiving environment shall be prepared to receive such
             message according to chosen notification approach (if SM Core is configured
             to send asynchronous messages in form of interrupt the receiving SW needs
             to contain related interrupt handler; please see the sm_cfg_send_notification()
             implementation within the sm_cfg.h).
    @param[in] u64Reason Value interpreted as reason. Will be accessible via the
                         SM_CFG_LL_CONFIRM_EVENT system call.
    @param[in] u64UserVal Some user's value. Will be accessible via the SM_CFG_LL_CONFIRM_EVENT too.
    @implements VLREQ043
*/
void sm_send_async_msg(uint64_t u64Reason, uint64_t u64UserVal)
{
    /*  TODO: The reason could be ORed to provide various reasons */
    rAsyncResStorage.u64Val0 = u64Reason;
    rAsyncResStorage.u64Val1 = u64UserVal;
    if (TRUE == bAsyncNotificationActive)
    {
        /** @violates @ref sm_c_REF_5 MISRA rule 11.3 */
        sm_cfg_send_notification();
    }
    
    return;
}

/*
  @brief Initialization function
  @details This function is called from the assembly code at the very beginning. It is not
           considered as a part of API but needs to stay visible for other modules due to
           reference from assembly part.
  
  @violates @ref sm_c_REF_9 MISRA rule 8.10
*/
void sm_hl_init(void)
{
    static sm_sc_param_t rScParams __attribute__ ((section (".sm_sc_param_storage")));
    static sm_sc_param_t rScQRes __attribute__ ((section (".sm_query_result_storage")));
    uint32_t u32ii;
	uint64_t u64Output;
    /** @violates @ref sm_c_REF_3 MISRA rule 11.4 */
    sm_entry_point_t *ptr = (sm_entry_point_t *)&__sm_init_vec_start;
    boolean bFailure = FALSE;

    /*  Unused but required */
    UNUSED(rScParams);
    UNUSED(rScQRes);
    
#if (TRUE == SM_CFG_ENABLE_MMU)
    if (sm_mmap_init())
    {
        s32AutoInitStatus = SM_E_MMAP_INIT;
        bFailure = TRUE;
    }
    else if (sm_mmap_start())
    {
        s32AutoInitStatus = SM_E_MMAP_START;
        bFailure = TRUE;
    }
    else
    {
        ;   /* Intentionally empty */
    }
#endif /* SM_CFG_ENABLE_MMU */
    
    if (FALSE == bFailure)
    {
        u32FiqCounter = 0U;
        s32AutoInitStatus = 0;
        rAsyncResStorage.u64Val0 = 0x0ULL;
        rAsyncResStorage.u64Val1 = 0x0ULL;
    
        for (u32ii=0U; u32ii<(SM_CFG_SC_H_COUNT + 1U); u32ii++)
        {
            arScFncs[u32ii].u32Key = 0xffffffffU;
            arScFncs[u32ii].pfHandler = NULL;
        }
    
        for (u32ii=0U; u32ii<(SM_CFG_FIQ_H_COUNT + 1U); u32ii++)
        {
            arFiqFncs[u32ii].u32Key = 0xffffffffU;
            arFiqFncs[u32ii].pfHandler = NULL;
        }
        
        /** @violates @ref sm_c_REF_6 MISRA rule 11.3 */
        while((sm_addr_t)ptr < (sm_addr_t)&__sm_init_vec_end)
        {
            /** @violates @ref sm_c_REF_3 MISRA rule 11.4 */
            s32AutoInitStatus += (sint32_t)((sm_entry_point_t)(*ptr))();
            /** @violates @ref sm_c_REF_2 MISRA rule 17.4 */
            /** @violates @ref sm_c_REF_7 MISRA rule 17.1 */
            ptr++;
        }
        
        if (s32AutoInitStatus)
        {
            s32AutoInitStatus = SM_E_FAILURE; /* Automatic initialization failed */
        }
        else
        {
            DBGB_Init();
            /** @violates @ref sm_c_REF_6 MISRA rule 11.3 */
            u64Output = (uint64_t)DBGB_GetBuffAddress();
            sm_sc_set_query_result(u64Output);
            s32AutoInitStatus = (sint32_t)DBGB_GetBuffSize();
            bInitialized = TRUE;
        }
    }
    
    return;
}

/*
    @brief Synchronous exception dispatcher
    @details This is internal function acting as exception dispatcher. Since it is not
             considered to be a part of API it needs to stay visible due to reference
             from the assembly part of module.
    
    @violates @ref sm_c_REF_9 MISRA rule 8.10
*/
void sm_aarch64_sync_dispatcher(void)
{
    uint32_t u32RegVal = 0U, u32EC, u32IL;
    volatile uint32_t u32ISS;
    sm_handler_entry_t *prP = arScFncs;

    __asm(" mrs %0, esr_el3"
            :"=r"(u32RegVal) : : );
    
    u32ISS = u32RegVal & 0x1ffffffU;
    u32IL = (u32RegVal >> 25) & 0x1U;
    u32EC = (u32RegVal >> 26);
    
    UNUSED(u32IL);

    switch (u32EC)
    {
        case Ec_SMC_AARCH64:
            if (SM_CFG_LL_INIT_KEY == u32ISS)
            {
                if (FALSE == bInitialized)
                {
                    sm_hl_init();
                }
                
                /** @violates @ref sm_c_REF_3 MISRA rule 11.4 */
                /** @violates @ref sm_c_REF_4 MISRA rule 1.2 */
                ((sm_sc_param_t *)&__sm_sc_qr_start)->u64Val0 = (uint64_t)s32AutoInitStatus;
                break;
            }
            
            if (FALSE == bInitialized)
            {
                /*  Don't continue if initialization has not been done yet */
                /** @violates @ref sm_c_REF_3 MISRA rule 11.4 */
                /** @violates @ref sm_c_REF_4 MISRA rule 1.2 */
                ((sm_sc_param_t *)&__sm_sc_qr_start)->u64Val0 = (uint64_t)(-10);
                break;
            }
            
            if (SM_CFG_LL_ENABLE_EVENT_KEY == u32ISS)
            {
                sm_sc_param_t *sc_params = sm_sc_get_params();
                
                if (sc_params->u64Val0) {
                    sm_cfg_enable_notification();
                    bAsyncNotificationActive = TRUE;
                }
                else if (TRUE == bAsyncNotificationActive)
                {
                    sm_cfg_disable_notification();
                    /** @violates @ref sm_c_REF_6 MISRA rule 11.3 */
                    sm_cfg_confirm_notification();
                    bAsyncNotificationActive = FALSE;
                }
                else
                {
                    ;   /*  Intentionally empty */
                }
                
                /** @violates @ref sm_c_REF_3 MISRA rule 11.4 */
                /** @violates @ref sm_c_REF_4 MISRA rule 1.2 */
                ((sm_sc_param_t *)&__sm_sc_qr_start)->u64Val0 = 0ULL; /* Success */
                break;
            }
            
            if (SM_CFG_LL_CONFIRM_EVENT_KEY == u32ISS)
            {
                /** @violates @ref sm_c_REF_6 MISRA rule 11.3 */
                sm_cfg_confirm_notification();
                /*  Provide info about previous asynchronous event */
                /** @violates @ref sm_c_REF_3 MISRA rule 11.4 */
                /** @violates @ref sm_c_REF_4 MISRA rule 1.2 */
                *((sm_sc_param_t *)&__sm_sc_qr_start) = rAsyncResStorage;
                rAsyncResStorage.u64Val0 = 0ULL;
                rAsyncResStorage.u64Val1 = 0ULL;
                break;
            }
            
            if (SM_CFG_LL_SHUTDOWN_KEY == u32ISS)
            {
                /*  Shut-down the SM application */
                sm_cfg_disable_notification();
                /** @violates @ref sm_c_REF_6 MISRA rule 11.3 */
                sm_cfg_confirm_notification();
                bAsyncNotificationActive = FALSE;
                
#if (TRUE == SM_CFG_ENABLE_MMU)
                if (sm_mmap_stop())
                {
                    /** @violates @ref sm_c_REF_3 MISRA rule 11.4 */
                    /** @violates @ref sm_c_REF_4 MISRA rule 1.2 */
                    ((sm_sc_param_t *)&__sm_sc_qr_start)->u64Val0 = (uint64_t)(SM_E_FAILURE); /* Failure */
                    break;
                }
#endif /* SM_CFG_ENABLE_MMU */
                
                /** @violates @ref sm_c_REF_3 MISRA rule 11.4 */
                /** @violates @ref sm_c_REF_4 MISRA rule 1.2 */
                ((sm_sc_param_t *)&__sm_sc_qr_start)->u64Val0 = 0ULL; /* Success */
                bInitialized = FALSE;
                break;
            }

            while (prP->pfHandler)
            {
                /*
                    This implementation allows to have multiple handlers related
                    to a single system call. Handling of return values is however
                    not so straightforward. Currently only the return value of the
                    latest handler is stored and passed to the caller.
                */
                if (prP->u32Key == u32ISS)
                {
                    /** @violates @ref sm_c_REF_3 MISRA rule 11.4 */
                    /** @violates @ref sm_c_REF_4 MISRA rule 1.2 */
                    ((sm_sc_param_t *)&__sm_sc_qr_start)->u64Val0 = (uint64_t)(prP->pfHandler());
                }
                
                /** @violates @ref sm_c_REF_2 MISRA rule 17.4 */
                prP++;
            }
            break;
        default:
            /*  TODO: Unexpected case */
            break;
    }

    return;
}

/*
    @brief Fast interrupt dispatcher
    @details This is internal function acting as interrupt dispatcher. Since it is not
             considered to be a part of API it needs to stay visible due to reference
             from the assembly part of module.
    
    @violates @ref sm_c_REF_9 MISRA rule 8.10
*/
uint32_t sm_ll_fiq_handler(const uint32_t u32Vector)
{
    uint32_t u32IntID = u32Vector & 0x3ffU;
    sm_handler_entry_t *prP = arFiqFncs;

    if (1023U == u32IntID)
    {
        ;   /*  TODO: Spurious interrupt */
    }
    else
    {
        while (prP->pfHandler)
        {
            if (prP->u32Key == u32IntID)
            {
                (void)prP->pfHandler();
            }
            
            /** @violates @ref sm_c_REF_2 MISRA rule 17.4 */
            prP++;
        }
    }

    u32FiqCounter++;
    return (u32Vector & 0x1fffU);
}
/** @}*/
