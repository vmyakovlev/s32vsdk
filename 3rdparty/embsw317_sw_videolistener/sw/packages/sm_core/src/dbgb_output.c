/**
  @addtogroup       dxgrSM_CORE
  @{

  @file             dbgb_output.c
  @brief            Debug output buffer feeding module
  @details          This module provides an output buffer for the fsl_printf module to
                    write the debug messages.

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
* @section dbgb_output_c_REF_1
* Violates MISRA 2004 Advisory Rule 11.3, A cast should not be preformed between a pointer type and
* an integral type. Cast is used to access memory resources.
*
* @section dbgb_output_c_REF_2
* Violates MISRA 2004 Required Rule 17.4, Array indexing shall be only allowed form of pointer arithmetic.
* Usage of pointer arithmetic other than array indexing improves the code readability.
*
*/

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "common_types.h"  /* Project wide types other than common ones like e.g. int */
#include "sm_cfg.h"
#include "mmap.h"
#include "dbgb_output_cfg.h" /* This module configuration */
#include "dbgb_output.h"     /* This module header */
/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
#ifdef DBGB_CFG_BUF_ADDR
/* Externally supplied buffer */
/* We will dereference a pointer to 32-bit variable which address we set using
   the DBGB_CFG_BUF_ADDR and DBGB_CFG_BUF_SIZE thus we want them properly aligned */
#if !DBGB_CFG_BUF_ADDR
    #error DBGB_CFG_BUF_ADDR cannot be NULL
#endif
#if DBGB_CFG_BUF_ADDR % 4
    #error DBGB_CFG_BUF_ADDR must be divisible by 4
#endif
#if DBGB_CFG_BUF_SIZE % 4
    #error DBGB_CFG_BUF_SIZE must be divisible by 4
#endif
#endif

/* We need to know at least the buffer size */
#ifndef DBGB_CFG_BUF_SIZE
    #error At least DBGB_CFG_BUF_SIZE must be defined
#endif
/* The buffer size must be reasonable */
#if DBGB_CFG_BUF_SIZE < 16U
    #error DBGB_CFG_BUF_SIZE should be at least 16
#endif
/* Check allowed buffer sizes */
#if (DBGB_CFG_BUF_SIZE != 16U) && \
	(DBGB_CFG_BUF_SIZE != 32U) && \
	(DBGB_CFG_BUF_SIZE != 64U) && \
	(DBGB_CFG_BUF_SIZE != 128U) && \
	(DBGB_CFG_BUF_SIZE != 256U) && \
	(DBGB_CFG_BUF_SIZE != 512U) && \
	(DBGB_CFG_BUF_SIZE != 1024U) && \
	(DBGB_CFG_BUF_SIZE != 2048U) && \
	(DBGB_CFG_BUF_SIZE != 4096U) && \
	(DBGB_CFG_BUF_SIZE != 8192U) && \
	(DBGB_CFG_BUF_SIZE != 16384U) && \
	(DBGB_CFG_BUF_SIZE != 32768U) && \
	(DBGB_CFG_BUF_SIZE != 65536U) && \
	(DBGB_CFG_BUF_SIZE != 131072U) && \
	(DBGB_CFG_BUF_SIZE != 262144U) && \
	(DBGB_CFG_BUF_SIZE != 524288U) && \
	(DBGB_CFG_BUF_SIZE != 1048576U) && \
	(DBGB_CFG_BUF_SIZE != 2097152U) && \
	(DBGB_CFG_BUF_SIZE != 4194304U) && \
	(DBGB_CFG_BUF_SIZE != 8388608U) && \
	(DBGB_CFG_BUF_SIZE != 16777216U) && \
	(DBGB_CFG_BUF_SIZE != 33554432U) && \
	(DBGB_CFG_BUF_SIZE != 67108864U) && \
	(DBGB_CFG_BUF_SIZE != 134217728U) && \
	(DBGB_CFG_BUF_SIZE != 268435456U) && \
	(DBGB_CFG_BUF_SIZE != 536870912U) && \
	(DBGB_CFG_BUF_SIZE != 1073741824U) && \
	(DBGB_CFG_BUF_SIZE != 2147483648U)
	#error The DBGB_CFG_BUF_SIZE must be a power from interval 15 to 2147483649
#endif
/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/
/** @cond dxINTERNAL_MACROS */
/** @endcond */

/*==================================================================================================
                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
typedef unsigned long dbgb_addr_t;

/*==================================================================================================
                                       LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       LOCAL VARIABLES
==================================================================================================*/

#ifndef DBGB_CFG_BUF_ADDR
/* No externally supplied buffer - we will make our own one */
static uint32_t u32Dbgb[(DBGB_CFG_BUF_SIZE / 4U) + 1U] MMAP_DATA_NOCACHE(16U) = {0U}; /* Reserve space for written bytes count */
#define DBGB_CFG_BUF_ADDR ((dbgb_addr_t)u32Dbgb)
#endif
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
/*
* @brief Initializes the debug buffer for use
* @details The buffer itself is not touched but the byte-write-count is reset
*/
void DBGB_Init(void)
{
    /** @violates @ref dbgb_output_c_REF_1 MISRA rule 11.3 */
    *(uint32_t *)(DBGB_CFG_BUF_ADDR + DBGB_CFG_BUF_SIZE) = 0U;
}

/*
* @brief Function to write a string into a debug buffer
* @param[in] cString Debug messaged in a form of null terminated string.
* @details The function simply copies the input string into the output buffer and increments
*          the write count pointer.
*
*          This function is not re-entrant. Re-entrant calls will lead to buffer corruption.
*/
void DBGB_Output(const uint8_t *cString)
{
    /* Position to write the next byte (written bytes total count) */
    /** @violates @ref dbgb_output_c_REF_1 MISRA rule 11.3 */
    uint32_t u32WritePos = *(uint32_t *)(DBGB_CFG_BUF_ADDR + DBGB_CFG_BUF_SIZE);

    /* We will do here stupid byte-by-byte copy because otherwise we would have to
       get the string length which means that we would byte-by-byte search for '\0'
       anyway thus we may copy during that process to avoid multiple  */
    while(0x0U != *cString) /* Do not forget to terminate your stings by '\0' character ! */
    {
        /** @violates @ref dbgb_output_c_REF_1 MISRA rule 11.3 */
        *(uint8_t *)(DBGB_CFG_BUF_ADDR + (dbgb_addr_t)(u32WritePos % DBGB_CFG_BUF_SIZE)) = *cString;
        u32WritePos++;
        /** @violates @ref dbgb_output_c_REF_2 MISRA rule 17.4 */
        cString++;
    }
    /* Update the write position */
    /** @violates @ref dbgb_output_c_REF_1 MISRA rule 11.3 */
    *(uint32_t *)(DBGB_CFG_BUF_ADDR + DBGB_CFG_BUF_SIZE) = u32WritePos;

}

/*
* @brief Returns a pointer to the debug buffer start
* @return A pointer where the debug buffer starts
*/
uint32_t *DBGB_GetBuffAddress(void)
{
    /** @violates @ref dbgb_output_c_REF_1 MISRA rule 11.3 */
    return (uint32_t *)DBGB_CFG_BUF_ADDR;
}

/*
* @brief Returns size of the debug buffer
* @return Size of the debug buffer - only the text area size, 4-byte counter at the
*         end is not included.
*/
uint32_t DBGB_GetBuffSize(void)
{
    return DBGB_CFG_BUF_SIZE;

}
/*================================================================================================*/
#ifdef __cplusplus
}
#endif
/** @}*/
