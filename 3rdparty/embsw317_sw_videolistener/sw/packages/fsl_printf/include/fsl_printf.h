/**
 @file             fsl_printf.h
 @version          0.0.0.0

 @brief            Module serves to printing debug messages
 @details          

 Project           M4_ETH_PROJECT_NAME
 Platform          M4_ETH_PLATFORM

 SWVersion         M4_ETH_RELEASE_VERSION

 
 Copyright (c) 2014-2016 Freescale Semiconductor Inc.
 Copyright (c) 2016 NXP Semiconductors
 All Rights Reserved
*/
/*==================================================================================================
==================================================================================================*/

#ifndef FSL_PRINTF_H
#define FSL_PRINTF_H

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "common_types.h"
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

/*==================================================================================================
                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/* Structure that keeps informations about parsed format specifier */
typedef struct {
    uint8_t u8Flags;       /* Flags modifying the result number look */
    sint16_t s16Width;     /* Number of digits or spaces - minimal width of the output */
    sint16_t s16Precision; /* Number of digits (0s emitted instead of spaces to reach this limit)*/
    uint8_t u8Length;      /* Length of the type to be converted (in bytes) */
    uint8_t u8Type;        /* Type to be converted (char, short, int, long ...) */
    uint8_t u8Specifier;   /* The letter specifying the format */
} FormatSpecifierType;

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
    
/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/
/* Initialization function */
extern void fsl_printf_init(void);
/* Function for debug print */
extern void fsl_printf(const char_t *pcocStr, ...);

#endif /* FSL_PRINTF_H */
