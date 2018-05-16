/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
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
  
/*!*********************************************************************************
*  @file color_conversion.c
*  @brief ACF metadata and wrapper function for color conversion kernels
***********************************************************************************/


#ifndef COLORCONVERSIONKERNELS_H
#define COLORCONVERSIONKERNELS_H


#include "common_stringify_macros.h"

#define RGB565_TO_RGB888X_K    rgb565_to_rgb888x
#define RGB565_TO_RGB888X_KN    XSTR(RGB565_TO_RGB888X_K)

#define RGB888X_TO_RGB565_K    rgb888x_to_rgb565
#define RGB888X_TO_RGB565_KN    XSTR(RGB888X_TO_RGB565_K)

#define RGB888X_TO_Y_K      rgb888x_to_y
#define RGB888X_TO_Y_KN      XSTR(RGB888X_TO_Y_K)  


#define RGB888X_TO_YUV_K      rgb888x_to_yuv
#define RGB888X_TO_YUV_KN    XSTR(RGB888X_TO_YUV_K)

#define HT_RGB888X_TO_Y_K      ht_rgb888x_to_y
#define HT_RGB888X_TO_Y_KN      XSTR(HT_RGB888X_TO_Y_K)

#define EXTRACT_CHANNEL_2CH_K      extract_channel_2ch
#define EXTRACT_CHANNEL_2CH_KN     XSTR(EXTRACT_CHANNEL_2CH_K)

#define EXTRACT_CHANNEL_3CH_K      extract_channel_3ch
#define EXTRACT_CHANNEL_3CH_KN     XSTR(EXTRACT_CHANNEL_3CH_K)

#define EXTRACT_CHANNEL_4CH_K      extract_channel_4ch
#define EXTRACT_CHANNEL_4CH_KN     XSTR(EXTRACT_CHANNEL_4CH_K)

#define INSERT_CHANNEL_2CH_K      insert_channel_2ch
#define INSERT_CHANNEL_2CH_KN     XSTR(INSERT_CHANNEL_2CH_K)

#define INSERT_CHANNEL_3CH_K      insert_channel_3ch
#define INSERT_CHANNEL_3CH_KN     XSTR(INSERT_CHANNEL_3CH_K)

#define INSERT_CHANNEL_4CH_K      insert_channel_4ch
#define INSERT_CHANNEL_4CH_KN     XSTR(INSERT_CHANNEL_4CH_K)

#define SPLIT_CHANNEL_2CH_K      split_channel_2ch
#define SPLIT_CHANNEL_2CH_KN     XSTR(SPLIT_CHANNEL_2CH_K)

#define SPLIT_CHANNEL_3CH_K      split_channel_3ch
#define SPLIT_CHANNEL_3CH_KN     XSTR(SPLIT_CHANNEL_3CH_K)

#define SPLIT_CHANNEL_4CH_K      split_channel_4ch
#define SPLIT_CHANNEL_4CH_KN     XSTR(SPLIT_CHANNEL_4CH_K)

#define MERGE_CHANNEL_2CH_K      merge_channel_2ch
#define MERGE_CHANNEL_2CH_KN     XSTR(MERGE_CHANNEL_2CH_K)

#define MERGE_CHANNEL_3CH_K      merge_channel_3ch
#define MERGE_CHANNEL_3CH_KN     XSTR(MERGE_CHANNEL_3CH_K)

#define MERGE_CHANNEL_4CH_K      merge_channel_4ch
#define MERGE_CHANNEL_4CH_KN     XSTR(MERGE_CHANNEL_4CH_K)

#define RGB888_TO_GREY_K     rgb888_to_grey
#define RGB888_TO_GREY_KN    XSTR(RGB888_TO_GREY_K)

#define BGR888_TO_GREY_K     bgr888_to_grey
#define BGR888_TO_GREY_KN    XSTR(BGR888_TO_GREY_K)

#define GREY_TO_RGB888_K     grey_to_rgb888
#define GREY_TO_RGB888_KN    XSTR(GREY_TO_RGB888_K)


#define INPUT_0    "INPUT_0"
#define INPUT_1    "INPUT_1"
#define INPUT_2    "INPUT_2"
#define INPUT_3    "INPUT_3"
#define OUTPUT_0  "OUTPUT_0"
#define OUTPUT_1  "OUTPUT_1"
#define OUTPUT_2  "OUTPUT_2"
#define OUTPUT_3  "OUTPUT_3"
#define R2Y      "R2Y"
#define G2Y      "G2Y"
#define B2Y      "B2Y"
#define KA      "KA"
#define KB      "KB"
#define KC      "KC"
#define KR      "KR"
#define CH_ID    "CH_ID"

#ifdef APEX2_EMULATE

#include "acf_kernel.hpp"
using namespace APEX2;

void RGB565_TO_RGB888X_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void RGB888X_TO_RGB565_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void RGB888X_TO_Y_K(kernel_io_desc lIn0, kernel_io_desc lR2Y, kernel_io_desc lG2Y, kernel_io_desc lB2Y, kernel_io_desc lOut0);
void RGB888X_TO_YUV_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void EXTRACT_CHANNEL_2CH_K(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lChId);
void EXTRACT_CHANNEL_3CH_K(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lChId);
void EXTRACT_CHANNEL_4CH_K(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lChId);
void INSERT_CHANNEL_2CH_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0, kernel_io_desc lChId);
void INSERT_CHANNEL_3CH_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0, kernel_io_desc lChId);
void INSERT_CHANNEL_4CH_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0, kernel_io_desc lChId);
void HT_RGB888X_TO_Y_K(kernel_io_desc lIn0, kernel_io_desc lKA, kernel_io_desc lKB, kernel_io_desc lKC, kernel_io_desc lKR, kernel_io_desc lOut0);
void RGB888_TO_GREY_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void BGR888_TO_GREY_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void GREY_TO_RGB888_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void SPLIT_CHANNEL_2CH_K(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lOut1);
void SPLIT_CHANNEL_3CH_K(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lOut1, kernel_io_desc lOut2);
void SPLIT_CHANNEL_4CH_K(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lOut1, kernel_io_desc lOut2, kernel_io_desc lOut3);
void MERGE_CHANNEL_2CH_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lOut0);
void MERGE_CHANNEL_3CH_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lIn2, kernel_io_desc lOut0);
void MERGE_CHANNEL_4CH_K(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lIn2, kernel_io_desc lIn3, kernel_io_desc lOut0);

#endif //#ifdef APEX2_EMULATE

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(RGB565_TO_RGB888X_K);
extKernelInfoDecl(RGB888X_TO_RGB565_K);
extKernelInfoDecl(RGB888X_TO_Y_K);
extKernelInfoDecl(RGB888X_TO_YUV_K);
extKernelInfoDecl(EXTRACT_CHANNEL_2CH_K);
extKernelInfoDecl(EXTRACT_CHANNEL_3CH_K);
extKernelInfoDecl(EXTRACT_CHANNEL_4CH_K);
extKernelInfoDecl(INSERT_CHANNEL_2CH_K);
extKernelInfoDecl(INSERT_CHANNEL_3CH_K);
extKernelInfoDecl(INSERT_CHANNEL_4CH_K);
extKernelInfoDecl(SPLIT_CHANNEL_2CH_K);
extKernelInfoDecl(SPLIT_CHANNEL_3CH_K);
extKernelInfoDecl(SPLIT_CHANNEL_4CH_K);
extKernelInfoDecl(MERGE_CHANNEL_2CH_K);
extKernelInfoDecl(MERGE_CHANNEL_3CH_K);
extKernelInfoDecl(MERGE_CHANNEL_4CH_K);
extKernelInfoDecl(HT_RGB888X_TO_Y_K);
extKernelInfoDecl(RGB888_TO_GREY_K);
extKernelInfoDecl(BGR888_TO_GREY_K);
extKernelInfoDecl(GREY_TO_RGB888_K);
#endif

#endif /* COLORCONVERSIONKERNELS_H */ 
