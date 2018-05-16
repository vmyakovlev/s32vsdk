/**
  @addtogroup       dxgrSM_CORE
  @{

  @file             dbgb_output_cfg.h
  @brief            Configuration file for the dbgb_output.c 
  @details          Provides interfaces needed by legacy fsl_printf module

  Project           M4_ETH_PROJECT_NAME

  Platform          M4_ETH_PLATFORM

  SWVersion         M4_ETH_RELEASE_VERSION

  Copyright (c) 2016 NXP Semiconductors
  All Rights Reserved.
*/
/*==================================================================================================
==================================================================================================*/

#ifndef DEB_PRINT_CFG_H
    #define DEB_PRINT_CFG_H

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
/** 
* @def DBGB_CFG_BUF_SIZE
* @brief Configures size of the debug buffer 
* @details Configures size of the buffer used to store debug messages before they are printed
*          out. The value must be reasonably high to avoid buffer overflows. The value 16 is 
*          the absolute minimum being supported. The value 2147483648 is the maximal supported
*          value. The chosen value must be power of 2.
* @warning The macro value must be 4 bytes less than the actual buffer size (in other words the 
*          buffer must be 4 bytes larger than the macro value). These 4 bytes are also used.
* @hideinitializer
*/
#define DBGB_CFG_BUF_SIZE 1024U /* 1 KiB buffer (the macro value is 4 bytes less than actual size) */

#if 0
/** 
* @def DBGB_CFG_BUF_ADDR
* @brief Configures externally supplied buffer address 
* @details The macro value (if it is defined) is used as a base address of the debug buffer
*          of size DBGB_CFG_BUF_SIZE + 4 bytes to write the debug messages. The buffer is 
*          created internally if the macro is not defined.
*
*          It is recommended to not define this macro.
* @hideinitializer
*/

#define DBGB_CFG_BUF_ADDR 0U /* Replace zero with a valid address */
#endif
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


#endif /* DEB_PRINT_CFG_H */
/** @}*/
