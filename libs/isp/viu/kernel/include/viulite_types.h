/*
 * Copyright (c) 2014-2016 Freescale Semiconductor
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*==============================================================================
Revision History:
                        Modification       Tracking
Author (core ID)        Date D/M/Y         Number     Description of Changes
Cristian Tomescu        04-Nov-2016       VSDK-488       Initial version
Cristian Tomescu        22-Dec-2016       VSDK-317    MISRA 2012 compliant.
Cristian Tomescu        17-Jan-2017       VSDK-694    Clock control improvement
Cristian Tomescu        08-May-2017       VSDK-560    Fix MISRA violations
==============================================================================*/
#ifndef VIULITETYPES_H
#define VIULITETYPES_H

#ifdef __cplusplus
extern "C"{
#endif

/**
* @file           viulite_types.h
* brief           contains the definition of the types used by the viu driver
*                 and the user applications
*/

/*==============================================================================
*                               INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/


/*==============================================================================
*                                   CONSTANTS
==============================================================================*/


/*==============================================================================
*                          DEFINES AND MACROS
==============================================================================*/


/*==============================================================================
*                                     ENUMS
==============================================================================*/
/**
* @brief          Declare enum VIU_BOOL
* @details        Enum VIU_BOOL define status VIU_ON or VIU_OFF
* @pre
* @post
* @implements     VIU_BOOL_enumeration
*/
typedef enum {
  VIU_OFF,
  VIU_ON
} VIU_BOOL; /* VIU_BOOL */

/**
* @brief          Declare enum in_mode_t
* @details        Enum in_mode_t define viu mode
* @pre
* @post
*/
typedef enum {
  PARALLEL_MODE = 0,
  ITU_MODE
} in_mode_t; /* in_mode_t */

/**
* @brief          Declare enum in_width_t
* @details        Enum in_width_t define width bits
* @pre
* @post
*/
typedef enum {
  WIDE_8_BITS = 0,
  WIDE_10_BITS,
  WIDE_12_BITS,
  WIDE_14_BITS,
  WIDE_16_BITS,
  WIDE_20_BITS = 6
} in_width_t; /* in_width_t */

/**
* @brief          Declare enum in_cpp_t
* @details        Enum in_cpp_t define CCP_X_PP
* @pre
* @post
*/
typedef enum {
  CPP_1_PP = 0,
  CPP_2_PP,
  CPP_3_PP
} in_cpp_t; /* in_cpp_t */

/**
* @brief          Declare enum VIU_ITU_ERR
* @details        enum VIU_ITU_ERR define status error
* @pre
* @post
* @implements     VIU_ITU_ERR_enumeration
*/
typedef enum {
  NO_ERR = 0,
  DMA_VERT_ERR,
  DMA_ACT_ERR,
  LINE_TOO_LONG_ERR,
  LINES_TOO_MANY_ERR,
  LINE_TOO_SHORT_ERR,
  LINES_TOO_LESS_ERR,
  FIFO_OVFLW_ERR,
  FIFO_UNFLW_ERR,
  ECC_1BIT_ERR,
  ECC_2BITS_ERR,
  SUBCH_DATA_ERR,
  SUBCH_REEN_ERR
} VIU_ITU_ERR; /* VIU_ITU_ERR */

/*==============================================================================
*                     STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/
/**
* @brief          Declare struct VIU_Registers_t
* @details        struct VIU_Registers_t contains the register map
* @pre
* @post
*
*/
typedef struct{
  volatile uint32_t  SCR;             /* Status And Control Register */
  volatile uint32_t  INTR;            /* Interrupt Register */
  volatile uint32_t  DINVSZ;          /* Detected Input Video Pixel and Line */
  volatile uint32_t  DINVFL;          /* Detected Input Video Frame Length */
  volatile uint32_t  DMA_SIZE;        /* DMA Size Register */
  volatile uint32_t  DMA_ADDR;        /* Address Of The Output Memory Buffer */
  volatile uint32_t  DMA_INC;         /* Horizontal DMA Increment */
  volatile uint32_t  INVSZ;           /* Input Video Pixel and Line Count */
  volatile uint8_t   VIU_reserved[4];
  volatile uint32_t  ALPHA;           /* Programable Alpha Value */
  volatile uint32_t  ACTORG;          /* Active Image Original Coordinate */
  volatile uint32_t  ACTSIZE;         /* Active Image Size */
} VIU_Registers_t; /* VIU register map */

/**
* @brief          Declare struct VIU_FRAME_SIZE
* @details        Declare struct VIU_FRAME_SIZE
* @pre
* @post
* @implements     VIU_FRAME_SIZE_struct
*/
typedef struct {
  uint16_t  nmb_lines;
  uint16_t  nmb_pixells;
} VIU_FRAME_SIZE; /* VIU_FRAME_SIZE */

/**
* @brief          Declare struct VIU_IMAGE_PARAMS
* @details        Declare struct VIU_IMAGE_PARAMS
* @pre
* @post
* @implements     VIU_IMAGE_PARAMS_struct
*/
typedef struct {
  uint16_t  x_origin;
  uint16_t  y_origin;
  uint16_t  x_size;
  uint16_t  y_size;
} VIU_IMAGE_PARAMS; /* VIU_IMAGE_PARAMS */

/**
* @brief          Declare struct VIU_INPUT_FORMAT
* @details        Declare struct VIU_INPUT_FORMAT
* @pre
* @post
* @implements     VIU_INPUT_FORMAT_struct
*/
typedef struct {
  in_mode_t  mode;
  in_width_t width;
  in_cpp_t   clocks_per_pixell;
} VIU_INPUT_FORMAT; /* VIU_INPUT_FORMAT */

/**
* @brief          Declare struct VIU_DATA_INTERFACE
* @details        Declare struct VIU_DATA_INTERFACE
* @pre
* @post
* @implements     VIU_DATA_INTERFACE_struct
*/
typedef struct {
  uint8_t  pclk_pol;
  uint8_t  vsync_pol;
  uint8_t  hsync_pol;
  uint8_t  endianness;
} VIU_DATA_INTERFACE; /* VIU_DATA_INTERFACE */

/**
* @brief          Declare struct DMA_CONFIG
* @details        Declare struct DMA_CONFIG
* @pre
* @post
* @implements     DMA_CONFIG_struct
*/
typedef struct {
  uint32_t  buff_addr;
  uint16_t  tx_size;
  uint16_t  nmb_lines;
  uint16_t  nmb_pixells;
  uint16_t  frame_inc;
  uint8_t   alpha_val;
} DMA_CONFIG; /* DMA_CONFIG */

/**
* @brief          Declare struct VIU_SYNC_STATUS
* @details        Declare struct VIU_SYNC_STATUS
* @pre
* @post
* @implements     VIU_SYNC_STATUS_struct
*/
typedef struct {
  uint8_t  vsync;
  uint8_t  hsync;
} VIU_SYNC_STATUS; /* VIU_SYNC_STATUS */


/*==============================================================================
*                        GLOBAL VARIABLE DECLARATIONS
==============================================================================*/


/*==============================================================================
*                           FUNCTION PROTOTYPES
==============================================================================*/


#ifdef __cplusplus
}
#endif

#endif /* VIULITETYPES_H */

/** @} */
