/**
  @file             common_types.h
  @version          0.0.0.0

  @brief            Common types
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

/**
* @page misra_violations MISRA-C:2004 violations
*
* @section common_types_h_REF_1
* Violates MISRA 2004 Advisory Rule 19.7, A function should be used in preference to a function-like macro.
* Function-like macros does not support type control of parameters but inline functions usage causes
* problems with some kind of compilers. Was also checked that macros are used correctly!
*/

#ifndef COMMON_TYPES_H
    #define COMMON_TYPES_H
/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/

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
#ifndef NULL
    #define NULL ((void *)0)
#endif  /* NULL */

#ifndef FALSE
    #define FALSE 0U
#endif /* FALSE */

#ifndef TRUE
    #define TRUE 1U
#endif /* TRUE */

#ifndef LITTLE
    #define LITTLE 1
#endif

#ifndef BIG
    #define BIG 2
#endif

#ifndef MACHINE_ENDIAN
    #define MACHINE_ENDIAN LITTLE
#endif


#if(LITTLE == MACHINE_ENDIAN)
/** @violates @ref common_types_h_REF_1 MISRA rule 19.7 */
#define ntohs(a) (((uint16_t)(((uint16_t)(a)) >> 8) & 0x00ffU) | \
                    ((uint16_t)(((uint16_t)(a)) << 8) & 0xff00U))
/** @violates @ref common_types_h_REF_1 MISRA rule 19.7 */
#define ntohl(a) (((((uint32_t)(a)) >> 24) & 0x000000ffU) | \
                  ((((uint32_t)(a)) >> 8 ) & 0x0000ff00U) | \
                  ((((uint32_t)(a)) << 8 ) & 0x00ff0000U) | \
                  ((((uint32_t)(a)) << 24) & 0xff000000U))
/** @violates @ref common_types_h_REF_1 MISRA rule 19.7 */
#define ntohll(a) (((((uint64_t)(a)) & 0xFF00000000000000ULL) >> 56U) | \
                  ((((uint64_t)(a)) & 0x00FF000000000000ULL) >> 40U) | \
                  ((((uint64_t)(a)) & 0x0000FF0000000000ULL) >> 24U) | \
                  ((((uint64_t)(a)) & 0x000000FF00000000ULL) >> 8U)  | \
                  ((((uint64_t)(a)) & 0x00000000FF000000ULL) << 8U)  | \
                  ((((uint64_t)(a)) & 0x0000000000FF0000ULL) << 24U) | \
                  ((((uint64_t)(a)) & 0x000000000000FF00ULL) << 40U) | \
                  ((((uint64_t)(a)) & 0x00000000000000FFULL) << 56U))
/** @violates @ref common_types_h_REF_1 MISRA rule 19.7 */
#define htons(a) ntohs(a)
/** @violates @ref common_types_h_REF_1 MISRA rule 19.7 */
#define htonl(a) ntohl(a)
/** @violates @ref common_types_h_REF_1 MISRA rule 19.7 */
#define htonll(a) ntohll(a)
#elif (BIG == MACHINE_ENDIAN)
#error s32v234 is little endian machine.
/** @violates @ref common_types_h_REF_1 MISRA rule 19.7 */
#define ntohs(a) (a)
/** @violates @ref common_types_h_REF_1 MISRA rule 19.7 */
#define ntohl(a) (a)
/** @violates @ref common_types_h_REF_1 MISRA rule 19.7 */
#define ntohll(a) (a)
/** @violates @ref common_types_h_REF_1 MISRA rule 19.7 */
#define htons(a) (a)
/** @violates @ref common_types_h_REF_1 MISRA rule 19.7 */
#define htonl(a) (a)
/** @violates @ref common_types_h_REF_1 MISRA rule 19.7 */
#define htonll(a) (a)
#else
    #error "Setup MACHINE_ENDIAN! Options: BIG or LITTLE."
#endif
/*==================================================================================================
                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/* Definition of the platform specific types */
typedef signed char sint8_t;
typedef signed short sint16_t;
typedef signed int sint32_t;
typedef signed long long sint64_t;
typedef sint8_t  int8_t;
typedef sint16_t int16_t;
typedef sint32_t int32_t;
typedef sint64_t int64_t;
typedef unsigned char boolean;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef volatile unsigned char vuint8_t;
typedef volatile unsigned short vuint16_t;
typedef volatile unsigned int vuint32_t;
typedef volatile unsigned long long vuint64_t;
typedef volatile char vint8_t;
typedef volatile short vint16_t;
typedef volatile int vint32_t;
typedef volatile long long vint64_t;

typedef int int_t;
typedef char char_t;
typedef unsigned long ulong_t;

/* Intended for calculations with pointer values.
   It is not pointer, but it shall have the same size as pointer.
   Size depends on architecture */
typedef sint64_t tsAddr; /* TODO: remove, obsolete: signed address seems not to be needed */
typedef uint64_t tuAddr; /* TODO: remove, obsolete. Replaced by uaddr_t */
typedef uint64_t uaddr_t; 

/* Redefinition of basic types for cases that types above are not appropriate */
typedef char char_t;
typedef float float32_t;
typedef double float64_t;
/* Note that it is difficult to predict width. It may be either 64 or 80 or 128 bits. */
typedef long double long_double_t;

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/

#endif /* COMMON_TYPES_H */
