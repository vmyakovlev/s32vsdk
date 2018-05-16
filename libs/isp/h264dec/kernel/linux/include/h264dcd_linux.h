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

#ifndef H264DCDIOCTL_H
#define H264DCDIOCTL_H

/**
* @file           h264dcd_linux.h
* brief           Linux declarations for H264 Decoder driver
*/

#define IOCTL_APP_TYPE  'c'

/************************************************
    H264DCD IOCTL function codes
*************************************************/

/************    Set Input Data   ***************/
#define H264DCD_IOCTL_INPUTSTREAM_CFG           _IOW(IOCTL_APP_TYPE, 1, uint32_t)

/*************   Get FIFO Status   **************/
#define H264DCD_IOCTL_FIFO_STATUS_GET           _IOWR(IOCTL_APP_TYPE, 2, uint32_t)

/***********    Get Data From FIFO   ************/
#define H264DCD_IOCTL_PCK_STATUS_GET            _IOWR(IOCTL_APP_TYPE, 3, uint32_t)

/********   Set FIFO Watermark Level  ***********/
#define H264DCD_IOCTL_FIFO_WATERMARK_SET        _IOW(IOCTL_APP_TYPE, 4, uint32_t)

/**************      Clear FIFO     *************/
#define H264DCD_IOCTL_FIFO_CLEAR                _IOW(IOCTL_APP_TYPE, 5, uint32_t)

/**********      Set H264 Timeout      **********/
#define H264DCD_IOCTL_TIMEOUT_SET               _IOW(IOCTL_APP_TYPE, 6, uint32_t)

/**********  Enable The TimeOut Check ***********/
#define H264DCD_IOCTL_TIMEOUT_ENABLE            _IOW(IOCTL_APP_TYPE, 7, uint32_t)

/****************    Sw Reset    ****************/
#define H264DCD_IOCTL_SW_RESET                  _IO(IOCTL_APP_TYPE, 8)

/****** Set The Ouput Stream Configuration ******/
#define H264DCD_IOCTL_OUTSTREAM_CFG             _IOW(IOCTL_APP_TYPE, 9, uint32_t)

/********       Set Sampling Mode      **********/
#define H264DCD_IOCTL_OUTSAMPLEMODE_SET         _IOW(IOCTL_APP_TYPE, 10, uint32_t)

/*******   Set Output Image Colour Params  ******/
#define H264DCD_IOCTL_OUTCOLOURDATA_SET         _IOW(IOCTL_APP_TYPE, 11, uint32_t)

/*********   Set The Data Flow Mode   ***********/
#define H264DCD_IOCTL_DATAFLOWMODE_SET          _IOW(IOCTL_APP_TYPE, 12, uint32_t)

/*********   Get The Data Flow Mode   ***********/
#define H264DCD_IOCTL_DATAFLOWMODE_GET          _IOR(IOCTL_APP_TYPE, 13, uint32_t)

/*******    Set The Bankstride Length    ********/
#define H264DCD_IOCTL_BANKSTRIDE_SET            _IOW(IOCTL_APP_TYPE, 14, uint32_t)

/********   Get The Bankstride Length   *********/
#define H264DCD_IOCTL_BANKSTRIDE_GET            _IOR(IOCTL_APP_TYPE, 15, uint32_t)

/***********      Channel Stop      *************/
#define H264DCD_IOCTL_DCD_CH_STOP               _IOW(IOCTL_APP_TYPE, 16, uint32_t)

/*********  Set The Codded Image Size  **********/
#define H264DCD_IOCTL_DCD_INFRAMESIZE_SET       _IOW(IOCTL_APP_TYPE, 17, uint32_t)

/*********   Set Memory Type & Filter    ********/
#define H264DCD_IOCTL_DCD_CFG_SET               _IOW(IOCTL_APP_TYPE, 18, uint32_t)

/*********   Get Memory Type & Filter   *********/
#define H264DCD_IOCTL_DCD_CFG_GET               _IOR(IOCTL_APP_TYPE, 19, uint32_t)

/*********   Set Threshold Levels    ************/
#define H264DCD_IOCTL_DCD_THRLEVELS_SET         _IOW(IOCTL_APP_TYPE, 20, uint32_t)

/*********   Get Threshold Levels   *************/
#define H264DCD_IOCTL_DCD_THRLEVELS_GET         _IOR(IOCTL_APP_TYPE, 21, uint32_t)

/********   Set Reference Memory Address ********/
#define H264DCD_IOCTL_DCD_REFMEMORY_SET         _IOW(IOCTL_APP_TYPE, 22, uint32_t)

/********   Get Reference Memory Address ********/
#define H264DCD_IOCTL_DCD_REFMEMORY_GET         _IOWR(IOCTL_APP_TYPE, 23, uint32_t)

/**********   Get The Channel Status    *********/
#define H264DCD_IOCTL_CH_STATUS_GET             _IOWR(IOCTL_APP_TYPE, 24, uint32_t)

/********* Get Decoded Image Paramters  *********/
#define H264DCD_IOCTL_PICDCD_PARAM_GET          _IOWR(IOCTL_APP_TYPE, 25, uint32_t)

/********    Get Decoded Image Status    ********/
#define H264DCD_IOCTL_PICDCD_STATUS_GET         _IOWR(IOCTL_APP_TYPE, 26, uint32_t)

/******** Get Decoded Image Gen Params  *********/
#define H264DCD_IOCTL_PICDCD_GENPARAMS_GET      _IOR(IOCTL_APP_TYPE, 27, uint32_t)

/*******    Enable/Disable Interrupts    ********/
#define H264DCD_IOCTL_IRQ_CONTROL               _IOW(IOCTL_APP_TYPE, 28, uint32_t)

/*********  Get The Enabled Interrupts  *********/
#define H264DCD_IOCTL_IRQ_GET                   _IOR(IOCTL_APP_TYPE, 29, uint32_t)

/*********  Get The TimeOut Intr Status  ********/
#define H264DCD_IOCTL_IRQ_TIMEOUT_GET           _IOR(IOCTL_APP_TYPE, 30, uint32_t)

/*********   Get The Error Intr Status   ********/
#define H264DCD_IOCTL_IRQ_ERROR_GET             _IOR(IOCTL_APP_TYPE, 31, uint32_t)

/*********  Get The Interrupts Flags   **********/
#define H264DCD_IOCTL_IRQ_STREAMSTATUS_GET      _IOWR(IOCTL_APP_TYPE, 32, uint32_t)

#endif /* H264DCDIOCTL_H */
