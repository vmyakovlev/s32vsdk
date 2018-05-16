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

#ifndef JPEGDCDIOCTL_H
#define JPEGDCDIOCTL_H

/**
* @file           jpegdcd_linux.h
* brief           Linux declarations for JPEG Decoder driver
*/

#define IOCTL_APP_TYPE  'c'

/************************************************
    JPEGDCD IOCTL function codes
*************************************************/

/************    Set Input Data   ***************/
#define JPEGDCD_IOCTL_INPUTSTREAM_CFG           _IOW(IOCTL_APP_TYPE, 1, uint32_t)

/*************   Get FIFO Status   **************/
#define JPEGDCD_IOCTL_FIFO_STATUS_GET           _IOWR(IOCTL_APP_TYPE, 2, uint32_t)

/***********    Get Data From FIFO   ************/
#define JPEGDCD_IOCTL_FIFO_DATA_GET             _IOWR(IOCTL_APP_TYPE, 3, uint32_t)

/************   Set FIFO Watermark   ************/
#define JPEGDCD_IOCTL_FIFO_WATERMARK_SET        _IOW(IOCTL_APP_TYPE, 4, uint32_t)

/**********   Set Output Buffers    *************/
#define JPEGDCD_IOCTL_OUTSTREAM_CFG             _IOW(IOCTL_APP_TYPE, 5, uint32_t)

/***********   Enable Resample mode   ***********/
#define JPEGDCD_IOCTL_OUTSAMPLEMODE_SET         _IOW(IOCTL_APP_TYPE, 6, uint32_t)

/**************   Set Bankstride    *************/
#define JPEGDCD_IOCTL_BANKSTRIDE_SET            _IOW(IOCTL_APP_TYPE, 7, uint32_t)

/**************   Get Bankstride    *************/
#define JPEGDCD_IOCTL_BANKSTRIDE_GET            _IOW(IOCTL_APP_TYPE, 8, uint32_t)

/******   Configure JPEG Decoder Start   ********/
#define JPEGDCD_IOCTL_JPEG_CFGSTART             _IO(IOCTL_APP_TYPE, 9)

/**********   Enable JPEG Decoder   *************/
#define JPEGDCD_IOCTL_JPEG_DCDENABLE            _IOW(IOCTL_APP_TYPE, 10, uint32_t)

/*********   Configure input stream   ***********/
#define JPEGDCD_IOCTL_JPEG_STREAMCFG_SET        _IOW(IOCTL_APP_TYPE, 11, uint32_t)

/*********   Get Enabled Interrupts    **********/
#define JPEGDCD_IOCTL_IRQ_GET                   _IOR(IOCTL_APP_TYPE, 12, uint32_t)

/********* Set JPEG Number Of Streams  **********/
#define JPEGDCD_IOCTL_JPEG_STREAMNUM_SET        _IOW(IOCTL_APP_TYPE, 13, uint32_t)

/********* Get JPEG Number Of Streams  **********/
#define JPEGDCD_IOCTL_JPEG_STREAMNUM_GET        _IOR(IOCTL_APP_TYPE, 14, uint32_t)

/********* Set JPEG Compatibility Mode  *********/
#define JPEGDCD_IOCTL_JPEG_MODE_SET             _IOW(IOCTL_APP_TYPE, 15, uint32_t)

/********* Get JPEG Compatibility Mode  *********/
#define JPEGDCD_IOCTL_JPEG_MODE_GET             _IOR(IOCTL_APP_TYPE, 16, uint32_t)

/****************    SW Reset    ****************/
#define JPEGDCD_IOCTL_JPEG_SW_RESET             _IO(IOCTL_APP_TYPE, 17)

/************   Get Active Stream   *************/
#define JPEGDCD_IOCTL_JPEG_STREAM_GET           _IOR(IOCTL_APP_TYPE, 18, uint32_t)

/**********   Set Reset Interval    *************/
#define JPEGDCD_IOCTL_JPEG_RSTINTERVAL_SET      _IOR(IOCTL_APP_TYPE, 19, uint32_t)

/**********   Get Reset Interval    *************/
#define JPEGDCD_IOCTL_JPEG_RSTINTERVAL_GET      _IOR(IOCTL_APP_TYPE, 20, uint32_t)

/***********   Get Frame Status    **************/
#define JPEGDCD_IOCTL_JPEG_FRAMESTATUS_GET      _IOR(IOCTL_APP_TYPE, 21, uint32_t)

/**********   Get Error Status    ***************/
#define JPEGDCD_IOCTL_JPEG_DCDCFGERR_GET        _IOR(IOCTL_APP_TYPE, 22, uint32_t)

/***********   Get MCU Row Status    ************/
#define JPEGDCD_IOCTL_JPEG_MCUROWEND_GET        _IOR(IOCTL_APP_TYPE, 23, uint32_t)

/***********   Get Decoded Image    *************/
#define JPEGDCD_IOCTL_JPEG_DCDIMAGE_GET         _IOR(IOCTL_APP_TYPE, 24, uint32_t)

/***********    Set JPEG Timeout    *************/
#define JPEGDCD_IOCTL_JPEG_TIMEOUT_SET          _IOW(IOCTL_APP_TYPE, 25, uint32_t)

/***********   Enable Interrupts    *************/
#define JPEGDCD_IOCTL_IRQ_CONTROL               _IOW(IOCTL_APP_TYPE, 26, uint32_t)

#endif /* JPEGDCDIOCTL_H */
