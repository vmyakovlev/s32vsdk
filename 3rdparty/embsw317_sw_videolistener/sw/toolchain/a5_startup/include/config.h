/*****************************************************************************
 *
 * Freescale Confidential Proprietary
 *
 * Copyright (c) 2013 Freescale Semiconductor;
 * All Rights Reserved
 *
 *****************************************************************************
 *
 * THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/**
 * \file    config.h
 * \brief   platform related definitions definitions of Sequencer memories for the silicon
 * \author  Tomas Babinec
 * \version 0.1
 * \date    Oct 23, 2014
 ****************************************************************************/

#ifndef CONFIG_H_
#define CONFIG_H_

/*****************************************************************************
* constants
*****************************************************************************/

//#define OLD_EVB ///< comment out if new EVB version is used

// *** choose what camera is attached ***
#define CAM_SONY_SE ///< uncoment to use SONY SE configuration
//#define CAM_SONY_DOL ///< uncoment to use SONY DOL configuration
//#define CAM_OV10640  ///< uncoment to use OV10640 MIPI camera
//#define CAM_MAXIM 2  ///< uncoment to use MAXIM with N OV10640 cameras

//****************************************************************************
//  *** ISP profiling and logging ***
//****************************************************************************

//#define ISP_FRAME_RATE_MEASURE_NUM  300 ///< number of frames to measure fps
#define ISP_EVT_LOG_LEN 0	//500000     ///< number of events in the ISP log (0 = no log)
#if (ISP_EVT_LOG_LEN > 0)
  #define ISP_FRAME_DONE_LOG  ///< comment out if FRAME_DONE message not logged
#endif // ifdef ISP_EVT_LOG

#define ENABLE_PRINTF	///< Turn on printf function
//#define EXCEPTION_RESET	///< Do a reset in case of exception ("Demo-mode")
//#define USE_ASM_FUNC	///< set to use sequencer assambly implementations
//#define ISP_DEBUG	///< set to avoid timeout inside FrameInputISP::GetFrame()

//****************************************************************************
//  *** Monitor Selection ***
//****************************************************************************

// select one or none for deault (app selected)
//#define PC_MONITOR_1280X1024_60HZ
//#define PC_MONITOR_1920X1080_60HZ
#define SAMSUNG

// select one more or none for default (1920x1080)
//#define HDMI_RES_1280X720
#define HDMI_RES_1920X1080
//#define HDMI_RES_1280X1024


#endif /* CONFIG_H_ */
