/**
  @file          fsl_printf_cfg.h
  @version       0.0.0.0

  @brief         Fsl_printf module configuration file
  @details       

  Project        M4_ETH_PROJECT_NAME
  Platform       M4_ETH_PLATFORM

  SWVersion      M4_ETH_RELEASE_VERSION


  (c) Copyright 2014 Freescale Semiconductor Inc.
  All Rights Reserved.
*/

/*==================================================================================================
==================================================================================================*/

#ifndef FSL_PRINTF_CFG_H
#define FSL_PRINTF_CFG_H

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "dbgb_output.h"

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
/* Size of output buffer */
#define STR_BUF_SIZE                        (2048U)
/* Maximum length of substituted (%s) string */
#define STR_BUF_HALF_SIZE                   (STR_BUF_SIZE/2U)
/* Select which kind of storage shall be used for output buffer.
   * When the buffer is on stack, then the function is re-entrant. But big buffer may not fit into
     the stack and for that this reason this may not be safe.
   * Otherwise the buffer is on the heap which makes the function NOT re-entrant.
*/
#define FSL_PRINT_CFG_PUT_BUFF_ON_STACK     (FALSE)

/* Enable floating point numbers support (time consuming) */
#define FSL_PRINTF_CFG_FLOAT_SUPPORT        (TRUE)
/* Enable long long integer types support */
#define FSL_PRINTF_CFG_LLINT_SUPPORT        (TRUE)
/*  Function to print data out */
#define FSL_PRINTF_CFG_DO_OUTPUT(String)    DBGB_Output(String)

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

/*==================================================================================================*/
#endif /* FSL_PRINTF_CFG_H */
