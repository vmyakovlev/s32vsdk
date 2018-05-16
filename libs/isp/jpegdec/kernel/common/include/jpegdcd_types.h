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

#ifndef JPEGDCDTYPES_H
#define JPEGDCDTYPES_H


/************************************************
    VARIOUS TYPES
*************************************************/
/**
* @brief          Declare enum JPEGDCD_BOOL
* @details        Enum JPEGDCD_BOOL define status JPEGDCD_ON or JPEGDCD_OFF
* @pre
* @post
* @implements    JPEGDCD_BOOL_enumeration
*/
typedef enum {
    JPEGDCD_OFF,
    JPEGDCD_ON
} JPEGDCD_BOOL;

/**
* @brief          Declare enum STREAM_ID
* @details        Enum STREAM_ID define the number of each stream
* @pre
* @post
* @implements    STREAM_ID_enumeration
*/
typedef enum {
    DATASTREAM_1,
    DATASTREAM_2,
    DATASTREAM_3,
    DATASTREAM_4
} STREAM_ID;

/**
* @brief          Declare enum FIFO_LEVEL
* @details        Enum FIFO_LEVEL define the level in FIFO
* @pre
* @post
* @implements    FIFO_LEVEL_enumeration
*/
typedef enum {
    FIFO_LEV_0,
    FIFO_LEV_1,
    FIFO_LEV_2,
    FIFO_LEV_3,
    FIFO_LEV_4,
    FIFO_LEV_5,
    FIFO_LEV_6,
    FIFO_LEV_7,
    FIFO_LEV_8,
    FIFO_LEV_9,
    FIFO_LEV_10,
    FIFO_LEV_11,
    FIFO_LEV_12,
    FIFO_LEV_13,
    FIFO_LEV_14,
    FIFO_LEV_15
} FIFO_LEVEL;

/**
* @brief          Declare enum SAMPLE_PREC
* @details        Enum SAMPLE_PREC define sampling precision: 8bit or 12bit
* @pre
* @post
* @implements    SAMPLE_PREC_enumeration
*/
typedef enum {
    PREC_8BITS,
    PREC_12BITS
} SAMPLE_PREC;

/**
* @brief          Declare enum JPEGDCD_FRAMEST
* @details        Enum JPEGDCD_FRAMEST define status JPEG_FR_START,
*                 JPEG_FR_END or JPEG_FR_REPEAT
* @pre
* @post
* @implements    JPEGDCD_FRAMEST_enumeration
*/
typedef enum {
    JPEG_FR_END,
    JPEG_FR_START,
    JPEG_FR_REPEAT
} JPEGDCD_FRAMEST;

/**
* @brief          Declare enum COMPAT_MODE
* @details        Enum COMPAT_MODE define status CONTEXT_SWITCH or COMPATIBILITY
* @pre
* @post
* @implements    COMPAT_MODE_enumeration
*/
typedef enum {
    CONTEXT_SWITCH,
    COMPATIBILITY
} COMPAT_MODE;

/**
* @brief          Declare struct STREAM_DATA_CONFIG
* @details        Declare struct STREAM_DATA_CONFIG
* @pre
* @post
* @implements     STREAM_DATA_CONFIG_struct
*/
typedef struct {
    STREAM_ID   stream_no;
    FIFO_LEVEL  fifo_lev;
    uint32_t    buff_addr;
    uint16_t    buff_size;
    uint32_t    wr_delay;
} STREAM_DATA_CONFIG;

/**
* @brief          Declare struct STREAM_FIFO_STATUS
* @details        Declare struct STREAM_FIFO_STATUS
* @pre
* @post
* @implements     STREAM_FIFO_STATUS_struct
*/
typedef struct {
    STREAM_ID stream_no;
    uint8_t  fifo_wtm;
    uint8_t  fifo_space;
} STREAM_FIFO_STATUS;

/**
* @brief          Declare struct OUTPUT_BUFFCFG
* @details        Declare struct OUTPUT_BUFFCFG
* @pre
* @post
* @implements     OUTPUT_BUFFCFG_struct
*/
typedef struct {
    uint32_t c1_outbuffer;
    uint32_t c2_outbuffer;
    uint32_t c3_outbuffer;
    uint32_t c4_outbuffer;
    uint16_t c1_b2lines;
    uint16_t c2_c3_b2lines;
    uint16_t c4_b2lines;
} OUTPUT_BUFFCFG;

/**
* @brief          Declare struct STREAM_JPEG_PARAM
* @details        Declare struct STREAM_JPEG_PARAM
* @pre
* @post
* @implements     STREAM_JPEG_PARAM_struct
*/
typedef struct{
    STREAM_ID       stream_no;  /* the stream number */
    uint16_t        x_size; /* coded image size x, y */
    uint16_t        y_size;
    SAMPLE_PREC     precision;  /* 8 or 12 bits */
    uint8_t         component_num;  /* number of color components */
    JPEGDCD_BOOL    wr_config;  /*1 wrapper config uses static values, 0 encod in data stream  */
    uint8_t         c1_samplingfactor_h;
    uint8_t         c1_samplingfactor_v;
    uint8_t         c2_samplingfactor_h;
    uint8_t         c2_samplingfactor_v;
    uint8_t         c3_samplingfactor_h;
    uint8_t         c3_samplingfactor_v;
    uint8_t         c4_samplingfactor_h;
    uint8_t         c4_samplingfactor_v;
} STREAM_JPEG_PARAM;

/**
* @brief          Declare struct JPEGDCD_CFGERR
* @details        Declare struct JPEGDCD_CFGERR
* @pre
* @post
* @implements     JPEGDCD_CFGERR_struct
*/
typedef struct{
    uint8_t  jpegerr_count; /* the error rising count */
    uint8_t  jpegerr_byte;  /* the errors */
} JPEGDCD_CFGERR;

/**
* @brief          Declare struct JPEGDCD_DECODED_IMAGE
* @details        Declare struct JPEGDCD_DECODED_IMAGE
* @pre
* @post
* @implements     JPEGDCD_DECODED_IMAGE_struct
*/
typedef struct{
    uint16_t x_size;    /* decoded image size x, y */
    uint16_t y_size;
    uint16_t hmcu_num;  /* number of MCUs in horizontal direction */
    uint16_t vmcu_num;  /* number of MCUs in vertical direction */
    SAMPLE_PREC sample_precision;   /* 8 or 12 bits */
    uint16_t rst_interval;      /* restart interval */
    uint8_t  component_num;
    uint8_t  c1_samplingfactor_h;
    uint8_t  c1_samplingfactor_v;
    uint8_t  c1_identifier;
    uint8_t  c1_quantization_table;
    uint8_t  c2_samplingfactor_h;
    uint8_t  c2_samplingfactor_v;
    uint8_t  c2_identifier;
    uint8_t  c2_quantization_table;
    uint8_t  c3_samplingfactor_h;
    uint8_t  c3_samplingfactor_v;
    uint8_t  c3_identifier;
    uint8_t  c3_quantization_table;
    uint8_t  c4_samplingfactor_h;
    uint8_t  c4_samplingfactor_v;
    uint8_t  c4_identifier;
    uint8_t  c4_quantization_table;
} JPEGDCD_DECODED_IMAGE;

#endif /* JPEGDCDTYPES_H */
