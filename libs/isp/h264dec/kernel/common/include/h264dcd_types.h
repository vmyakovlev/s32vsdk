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

#ifndef H264DCDTYPES_H
#define H264DCDTYPES_H

/************************************************
    VARIOUS TYPES
*************************************************/
/**
* @brief          Declare enum H264DCD_BOOL
* @details        Enum H264DCD_BOOL define status H264DCD_ON or H264DCD_OFF
* @pre
* @post
* @implements     H264DCD_BOOL_enumeration
*/
typedef enum {
    H264DCD_OFF,
    H264DCD_ON
} H264DCD_BOOL;

/**
* @brief          Declare enum H264DCD_STREAM_ID
* @details        Enum H264DCD_STREAM_ID define the number of each stream
* @pre
* @post
* @implements     H264DCD_STREAM_ID_enumeration
*/
typedef enum {
    H264DCD_DATASTREAM_1,
    H264DCD_DATASTREAM_2,
    H264DCD_DATASTREAM_3,
    H264DCD_DATASTREAM_4
} H264DCD_STREAM_ID;

/**
* @brief          Declare enum H264DCD_FIFO_LEVEL
* @details        Enum H264DCD_FIFO_LEVEL define the level in FIFO
* @pre
* @post
* @implements     H264DCD_FIFO_LEVEL_enumeration
*/
typedef enum {
    H264DCD_FIFO_LEV_0,
    H264DCD_FIFO_LEV_1,
    H264DCD_FIFO_LEV_2,
    H264DCD_FIFO_LEV_3,
    H264DCD_FIFO_LEV_4,
    H264DCD_FIFO_LEV_5,
    H264DCD_FIFO_LEV_6,
    H264DCD_FIFO_LEV_7,
    H264DCD_FIFO_LEV_8,
    H264DCD_FIFO_LEV_9,
    H264DCD_FIFO_LEV_10,
    H264DCD_FIFO_LEV_11,
    H264DCD_FIFO_LEV_12,
    H264DCD_FIFO_LEV_13,
    H264DCD_FIFO_LEV_14,
    H264DCD_FIFO_LEV_15
} H264DCD_FIFO_LEVEL;

/**
* @brief          Declare enum H264DCD_OUTPUT_BIT_WIDTH
* @details        Enum H264DCD_OUTPUT_BIT_WIDTH define sampling precision: 8bit or 12bit
* @pre
* @post
* @implements     H264DCD_OUTPUT_BIT_WIDTH_enumeration
*/
typedef enum {
    MODE_8BITS,
    MODE_10_12BITS
} H264DCD_OUTPUT_BIT_WIDTH;

/**
* @brief          Declare enum H264DCD_SAMPLE_PREC
* @details        Enum H264DCD_SAMPLE_PREC define sampling precision: 8bit,10bit or 12bit
* @pre
* @post
* @implements     H264DCD_SAMPLE_PREC_enumeration
*/
typedef enum {
    H264DCD_PREC_8BITS,
    H264DCD_PREC_10BITS,
    H264DCD_PREC_12BITS
} H264DCD_SAMPLE_PREC;

/**
* @brief          Declare enum H264DCD_COL_FORMAT
* @details        Enum H264DCD_COL_FORMAT define color format
* @pre
* @post
* @implements     H264DCD_COL_FORMAT_enumeration
*/
typedef enum {
    COL_YUV420,
    COL_GREYSCALE
} H264DCD_COL_FORMAT;

/**
* @brief          Declare enum H264DCD_DATA_MODE
* @details        Enum H264DCD_DATA_MODE define the data mode used for
*                 Chroma Cb and Cr components
* @pre
* @post
* @implements     H264DCD_DATA_MODE_enumeration
*/
typedef enum {
    OFFSET_BINARY_FORMAT,
    TWOS_COMPLEMENT_FORMAT
} H264DCD_DATA_MODE;

/**
* @brief          Declare enum H264DCD_DATAFLOW_MODE
* @details        Enum H264DCD_DATAFLOW_MODE define data flow mode
* @pre
* @post
* @implements     H264DCD_DATAFLOW_MODE_enumeration
*/
typedef enum {
    MODE_0,
    MODE_1,
    MODE_2
} H264DCD_DATAFLOW_MODE;

/**
* @brief          Declare enum H264DCD_MEM
* @details        Enum H264DCD_MEM define memory type
* @pre
* @post
* @implements    H264DCD_MEM_enumeration
*/
typedef enum {
    MEMORY_SRAM,
    MEMORY_DDR
} H264DCD_MEM;

/**
* @brief          Declare enum H264DCD_CH_CTRL
* @details        Enum H264DCD_CH_CTRL define channel status
* @pre
* @post
* @implements    H264DCD_CH_CTRL_enumeration
*/
typedef enum {
    CH_STOP,
    CH_START
} H264DCD_CH_CTRL;

/**
* @brief          Declare enum H264DCD_CH_STATUS
* @details        Enum H264DCD_CH_STATUS define status of channel
* @pre
* @post
* @implements    H264DCD_CH_STATUS_enumeration
*/
typedef enum{
    CH_IDLE_ST = 0,
    CH_NAL_LOOKING = 1,
    CH_SPS_DECOD = 4,
    CH_PPS_DECOD = 5,
    CH_SLICE_DECOD = 6,
    CH_SPS_ERROR = 8,
    CH_PPS_ERROR = 9,
    CH_SLICE_ERROR = 10
} H264DCD_CH_STATUS;

/**
* @brief          Declare struct H264DCD_STREAM_DATA_CONFIG
* @details        Declare struct H264DCD_STREAM_DATA_CONFIG
* @pre
* @post
* @implements     H264DCD_STREAM_DATA_CONFIG_struct
*/
typedef struct{
    H264DCD_STREAM_ID stream_no;
    uint32_t buff_addr;
    uint16_t buff_size;
    uint32_t wr_delay;
} H264DCD_STREAM_DATA_CONFIG;

/**
* @brief          Declare struct H264DCD_STREAM_FIFO_STATUS
* @details        Declare struct H264DCD_STREAM_FIFO_STATUS
* @pre
* @post
* @implements     H264DCD_STREAM_FIFO_STATUS_struct
*/
typedef struct {
    H264DCD_STREAM_ID stream_no;
    uint8_t  fifo_loc;
    uint8_t  fifo_cfgnum;
} H264DCD_STREAM_FIFO_STATUS;

/**
* @brief          Declare struct H264DCD_STREAM_DATA_STATUS
* @details        Declare struct H264DCD_STREAM_DATA_STATUS
* @pre
* @post
* @implements     H264DCD_STREAM_DATA_STATUS_struct
*/
typedef struct{
    H264DCD_STREAM_ID stream_no;
    H264DCD_FIFO_LEVEL   fifo_lev;
    uint32_t  pck_addr;
    H264DCD_BOOL pck_valid;
} H264DCD_STREAM_DATA_STATUS;

/**
* @brief          Declare struct H264DCD_FIFO_WTM
* @details        Declare struct H264DCD_FIFO_WTM
* @pre
* @post
* @implements     H264DCD_FIFO_WTM_struct
*/
typedef struct{
    H264DCD_STREAM_ID stream_no;
    uint8_t   fifo_wtmark;
} H264DCD_FIFO_WTM;

/**
* @brief          Declare struct H264DCD_TIMEOUT_ENABLE
* @details        Declare struct H264DCD_TIMEOUT_ENABLE
* @pre
* @post
* @implements     H264DCD_TIMEOUT_ENABLE_struct
*/
typedef struct {
    H264DCD_STREAM_ID stream_no;
    H264DCD_BOOL  to_enable;
} H264DCD_TIMEOUT_ENABLE;

/**
* @brief          Declare struct H264DCD_OUTPUT_BUFFCFG
* @details        Declare struct H264DCD_OUTPUT_BUFFCFG
* @pre
* @post
* @implements     H264DCD_OUTPUT_BUFFCFG_struct
*/
typedef struct {
    H264DCD_STREAM_ID stream_no;
    uint32_t y_outbuffer;
    uint32_t cb_outbuffer;
    uint32_t cr_outbuffer;
    uint16_t lines_num;
} H264DCD_OUTPUT_BUFFCFG;

/**
* @brief          Declare struct H264DCD_OUTPUT_SAMPLEMODE
* @details        Declare struct H264DCD_OUTPUT_SAMPLEMODE
* @pre
* @post
* @implements     H264DCD_OUTPUT_SAMPLEMODE_struct
*/
typedef struct {
    H264DCD_STREAM_ID stream_no;
    H264DCD_BOOL outresmpl_mode;
} H264DCD_OUTPUT_SAMPLEMODE;

/**
* @brief          Declare struct H264DCD_OUTPUT_COLOURDATA
* @details        Declare struct H264DCD_OUTPUT_COLOURDATA
* @pre
* @post
* @implements     H264DCD_OUTPUT_COLOURDATA_struct
*/
typedef struct {
    H264DCD_STREAM_ID stream_no;
    H264DCD_COL_FORMAT  col_format;
    H264DCD_OUTPUT_BIT_WIDTH bit_width_mode;
    H264DCD_DATA_MODE data_mode_format;
} H264DCD_OUTPUT_COLOURDATA;

/**
* @brief          Declare struct H264DCD_OUTPUT_BKSTRIDE
* @details        Declare struct H264DCD_OUTPUT_BKSTRIDE
* @pre
* @post
* @implements     H264DCD_OUTPUT_BKSTRIDE_struct
*/
typedef struct {
    uint8_t  bytes_num;
    H264DCD_BOOL bks_enable;
} H264DCD_OUTPUT_BKSTRIDE;

/**
* @brief          Declare struct H264DCD_CH_CONTROL
* @details        Declare struct H264DCD_CH_CONTROL
* @pre
* @post
* @implements     H264DCD_CH_CONTROL_struct
*/
typedef struct {
    H264DCD_STREAM_ID stream_no;
    H264DCD_CH_CTRL channel_status;
} H264DCD_CH_CONTROL;

/**
* @brief          Declare struct H264DCD_INFRAME_SIZE
* @details        Declare struct H264DCD_INFRAME_SIZE
* @pre
* @post
* @implements     H264DCD_INFRAME_SIZE_struct
*/
typedef struct {
    H264DCD_STREAM_ID stream_no;
    uint8_t x_size;
    uint8_t y_size;
} H264DCD_INFRAME_SIZE;

/**
* @brief          Declare struct H264DCD_DECOD_CONFIG
* @details        Declare struct H264DCD_DECOD_CONFIG
* @pre
* @post
* @implements     H264DCD_DECOD_CONFIG_struct
*/
typedef struct {
    H264DCD_MEM   mem_type;
    H264DCD_BOOL filter_en;
} H264DCD_DECOD_CONFIG;

/**
* @brief          Declare struct H264DCD_DECOD_THRLEVELS
* @details        Declare struct H264DCD_DECOD_THRLEVELS
* @pre
* @post
* @implements     H264DCD_DECOD_THRLEVELS_struct
*/
typedef struct {
    uint32_t level1;
    uint32_t level2;
    uint32_t level3;
} H264DCD_DECOD_THRLEVELS;

/**
* @brief          Declare struct H264DCD_DECOD_REFMEM
* @details        Declare struct H264DCD_DECOD_REFMEM
* @pre
* @post
* @implements     H264DCD_DECOD_REFMEM_struct
*/
typedef struct {
    H264DCD_STREAM_ID stream_no;
    uint32_t refmem_addr;
} H264DCD_DECOD_REFMEM;

/**
* @brief          Declare struct H264DCD_DECOD_CHANNELST
* @details        Declare struct H264DCD_DECOD_CHANNELST
* @pre
* @post
* @implements     H264DCD_DECOD_CHANNELST_struct
*/
typedef struct {
    H264DCD_STREAM_ID stream_no;
    H264DCD_CH_STATUS dcd_chst;
} H264DCD_DECOD_CHANNELST;

/**
* @brief          Declare struct H264DCD_PIC_PARAMS
* @details        Declare struct H264DCD_PIC_PARAMS
* @pre
* @post
* @implements     H264DCD_PIC_PARAMS_struct
*/
typedef struct{
    H264DCD_STREAM_ID stream_no;
    H264DCD_SAMPLE_PREC sample_bit;
    uint8_t width_mbc;
    uint8_t height_mbc;
    uint16_t crop_left;
    uint16_t crop_right;
    uint16_t crop_top;
    uint16_t crop_bottom;
} H264DCD_PIC_PARAMS;

/**
* @brief          Declare struct H264DCD_PIC_STATUS
* @details        Declare struct H264DCD_PIC_STATUS
* @pre
* @post
* @implements     H264DCD_PIC_STATUS_struct
*/
typedef struct{
    H264DCD_STREAM_ID stream_no;
    H264DCD_BOOL pic_prmvalid;
} H264DCD_PIC_STATUS;

/**
* @brief          Declare struct H264DCD_PIC_GENPARAMS
* @details        Declare struct H264DCD_PIC_GENPARAMS
* @pre
* @post
* @implements     H264DCD_PIC_GENPARAMS_struct
*/
typedef struct{
    uint32_t frame_count;
    H264DCD_BOOL frame_poccycle;
    uint8_t frame_nummax;
} H264DCD_PIC_GENPARAMS;

/**
* @brief          Declare struct H264DCD_IRQ_TIMEOUT
* @details        Declare struct H264DCD_IRQ_TIMEOUT
* @pre
* @post
* @implements     H264DCD_IRQ_TIMEOUT_struct
*/
typedef struct{
    H264DCD_BOOL timeout_flag;
    uint8_t timeout_st;
} H264DCD_IRQ_TIMEOUT;

/**
* @brief          Declare struct H264DCD_IRQ_ERROR
* @details        Declare struct H264DCD_IRQ_ERROR
* @pre
* @post
* @implements     H264DCD_IRQ_ERROR_struct
*/
typedef struct{
    H264DCD_BOOL error_flag;
    H264DCD_CH_STATUS status_ch0;
    H264DCD_CH_STATUS status_ch1;
    H264DCD_CH_STATUS status_ch2;
    H264DCD_CH_STATUS status_ch3;
} H264DCD_IRQ_ERROR;

/**
* @brief          Declare struct H264DCD_IRQ_STREAM
* @details        Declare struct H264DCD_IRQ_STREAM
* @pre
* @post
* @implements     H264DCD_IRQ_STREAM_struct
*/
typedef struct{
    H264DCD_STREAM_ID stream_no;
    H264DCD_BOOL timeout_flag;
    H264DCD_BOOL thr0_flag;
    H264DCD_BOOL thr1_flag;
    H264DCD_BOOL thr2_flag;
    H264DCD_BOOL oos_flag;
    H264DCD_BOOL fifo_wtm_flag;
    H264DCD_BOOL mbrow_flag;
    H264DCD_BOOL eof_flag;
} H264DCD_IRQ_STREAM;

#endif /* H264DCDTYPES_H */
