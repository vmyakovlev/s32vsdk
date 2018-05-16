/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
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

#ifndef CONFIG_H
#define CONFIG_H

/*****************************************************************************
* constants
*****************************************************************************/

//#define OLD_EVB ///< comment out if new EVB version is used

// *** choose what camera is attached ***
//#define OV10640_VIU  ///< uncoment to use OV10640 VIU camera
//#define CAM_OV10640  ///< uncoment to use OV10640 MIPI camera
//#define CAM_MAXIM 4  ///< uncoment to use MAXIM with N cameras
//#define CAM_OV10635  ///< uncoment to use OV10635 VIU configuration
#ifdef CAM_MAXIM
  #if (CAM_MAXIM > 0)
    //#define CAM_SONY_MAXIM ///< uncoment to use SONY with maxim
    //#define CAM_OV10635_MAXIM ///< uncoment to use OV10635 with maxim
    //#define CAM_OV10640_MAXIM ///< uncoment to use OV10640 with maxim
  #endif // #if (CAM_MAXIM > 0)
#endif // #ifdef CAM_MAXIM

//****************************************************************************
//  *** ISP profiling and logging ***
//****************************************************************************

//#define ISP_FRAME_RATE_MEASURE_NUM  300 ///< number of frames to measure fps
#define ISP_EVT_LOG_LEN 0	//500000     ///< number of events in the ISP log (0 = no log)
#if (ISP_EVT_LOG_LEN > 0)
  #define ISP_FRAME_DONE_LOG  ///< comment out if FRAME_DONE message not logged
#endif // ifdef ISP_EVT_LOG

//#define ISP_PERF_PRINTSTALL

//#define USE_ASM_FUNC	///< set to use sequencer assambly implementations
//#define ISP_DEBUG	///< set to avoid timeout inside FrameInputISP::GetFrame()

//****************************************************************************
//  *** Monitor Selection ***
//****************************************************************************

// select one or none for deault (app selected)
//#define PC_MONITOR_1280X1024_60HZ
//#define PC_MONITOR_1920X1080_60HZl
#define SAMSUNG

// select one more or none for default (1920x1080)
//#define HDMI_RES_1280X720
#define HDMI_RES_1920X1080
//#define HDMI_RES_1280X1024

//****************************************************************************
//  *** Misc ***
//****************************************************************************

#define ENABLE_PRINTF	///< Turn on printf function
//#define EXCEPTION_RESET	///< Do a reset in case of exception ("Demo-mode")

#endif /* CONFIG_H */
