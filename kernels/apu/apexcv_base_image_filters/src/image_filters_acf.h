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
#ifndef IMAGEFILTERSACF_H
#define IMAGEFILTERSACF_H


#include "common_stringify_macros.h"


#define BILATERAL_5X5_K      bilateral_5x5
#define BILATERAL_5X5_KN    XSTR(BILATERAL_5X5_K) 

#define BOX_1X3_16S_K      box_1x3_16s
#define BOX_1X3_16S_KN      XSTR(BOX_1X3_16S_K) 
#define BOX_3X1_16S_K      box_3x1_16s
#define BOX_3X1_16S_KN      XSTR(BOX_3X1_16S_K) 
#define BOX_3X3_08U_K      box_3x3_08u
#define BOX_3X3_08U_KN      XSTR(BOX_3X3_08U_K) 
#define BOX_3X3_16S_K      box_3x3_16s
#define BOX_3X3_16S_KN      XSTR(BOX_3X3_16S_K) 
#define BOX_5X5_16S_K           box_5x5_16s
#define BOX_5X5_16S_KN        XSTR(BOX_5X5_16S_K) 

#define CENSUS_3X3_08U_K        census_3x3_08u
#define CENSUS_3X3_08U_KN       XSTR(CENSUS_3X3_08U_K)

#define DILATE_3X3_08U_K      dilate_3x3_08u
#define DILATE_3X3_08U_KN    XSTR(DILATE_3X3_08U_K) 
#define DILATE_3X3_16S_K    dilate_3x3_16s
#define DILATE_3X3_16S_KN    XSTR(DILATE_3X3_16S_K) 

#define ERODE_3X3_08U_K      erode_3x3_08u
#define ERODE_3X3_08U_KN    XSTR(ERODE_3X3_08U_K) 

#define GAUSSIAN_3X3_08U_K    gaussian_3x3_08u
#define GAUSSIAN_3X3_08U_KN    XSTR(GAUSSIAN_3X3_08U_K) 
#define GAUSSIAN_5X5_08U_K    gaussian_5x5_08u
#define GAUSSIAN_5X5_08U_KN    XSTR(GAUSSIAN_5X5_08U_K)

#define MEDIAN_3X3_08U_K    median_3x3_08u
#define MEDIAN_3X3_08U_KN    XSTR(MEDIAN_3X3_08U_K) 
#define MEDIAN_5X5_08U_K    median_5x5_08u
#define MEDIAN_5X5_08U_KN    XSTR(MEDIAN_5X5_08U_K) 

#define HT_SOBEL_3X3_16S_K    ht_sobel_3x3_16s
#define HT_SOBEL_3X3_16S_KN     XSTR(HT_SOBEL_3X3_16S_K)
#define HT_SOBEL_X_3X3_08S_K   ht_sobel_x_3x3_08s
#define HT_SOBEL_X_3X3_08S_KN   XSTR(HT_SOBEL_X_3X3_08S_K)
#define HT_SOBEL_Y_3X3_08S_K   ht_sobel_y_3x3_08s
#define HT_SOBEL_Y_3X3_08S_KN   XSTR(HT_SOBEL_Y_3X3_08S_K)
#define HT_SOBEL_X_3X3_16S_K   ht_sobel_x_3x3_16s
#define HT_SOBEL_X_3X3_16S_KN   XSTR(HT_SOBEL_X_3X3_16S_K)
#define HT_SOBEL_Y_3X3_16S_K   ht_sobel_y_3x3_16s
#define HT_SOBEL_Y_3X3_16S_KN   XSTR(HT_SOBEL_Y_3X3_16S_K)
#define HT_SOBEL_X_5X5_16S_K   ht_sobel_x_5x5_16s
#define HT_SOBEL_X_5X5_16S_KN   XSTR(HT_SOBEL_X_5X5_16S_K)
#define HT_SOBEL_Y_5X5_16S_K   ht_sobel_y_5x5_16s
#define HT_SOBEL_Y_5X5_16S_KN   XSTR(HT_SOBEL_Y_5X5_16S_K)

#define SOBEL_X_3X3_08U_K    sobel_x_3x3_08u
#define SOBEL_X_3X3_08U_KN   XSTR(SOBEL_X_3X3_08U_K) 
#define SOBEL_X_3X3_08S_K    sobel_x_3x3_08s
#define SOBEL_X_3X3_08S_KN   XSTR(SOBEL_X_3X3_08S_K) 
#define SOBEL_Y_3X3_08U_K    sobel_y_3x3_08u
#define SOBEL_Y_3X3_08U_KN   XSTR(SOBEL_Y_3X3_08U_K) 
#define SOBEL_Y_3X3_08S_K    sobel_y_3x3_08s
#define SOBEL_Y_3X3_08S_KN   XSTR(SOBEL_Y_3X3_08S_K)
#define SOBEL_XY_3X3_08U_K   sobel_xy_3x3_08u
#define SOBEL_XY_3X3_08U_KN   XSTR(SOBEL_XY_3X3_08U_K)

#define SOBEL_3X3_08U_K     sobel_3x3_08u
#define SOBEL_3X3_08U_KN   XSTR(SOBEL_3X3_08U_K) 
#define SOBEL_X_5X5_08U_K    sobel_x_5x5_08u
#define SOBEL_X_5X5_08U_KN   XSTR(SOBEL_X_5X5_08U_K) 
#define SOBEL_X_5X5_08S_K    sobel_x_5x5_08s
#define SOBEL_X_5X5_08S_KN   XSTR(SOBEL_X_5X5_08S_K)
#define SOBEL_Y_5X5_08U_K    sobel_y_5x5_08u
#define SOBEL_Y_5X5_08U_KN   XSTR(SOBEL_Y_5X5_08U_K)
#define SOBEL_Y_5X5_08S_K    sobel_y_5x5_08s
#define SOBEL_Y_5X5_08S_KN   XSTR(SOBEL_Y_5X5_08S_K) 
#define SOBEL_5X5_08U_K     sobel_5x5_08u
#define SOBEL_5X5_08U_KN   XSTR(SOBEL_5X5_08U_K) 

#define HT_BOX_3X3_08U_K    ht_box_3x3_08u
#define HT_BOX_3X3_08U_KN    XSTR(HT_BOX_3X3_08U_K)  
#define HT_GENERIC_3X3_08U_K  ht_generic_3x3_08u
#define HT_GENERIC_3X3_08U_KN  XSTR(HT_GENERIC_3X3_08U_K)
#define HT_GENERIC_5X5_08U_K  ht_generic_5x5_08u
#define HT_GENERIC_5X5_08U_KN  XSTR(HT_GENERIC_5X5_08U_K)

#define HT_CENTRALDX_3X3_16S_K    ht_centraldx_3x3_16s
#define HT_CENTRALDX_3X3_16S_KN    XSTR(HT_CENTRALDX_3X3_16S_K)  
#define HT_CENTRALDY_3X3_16S_K    ht_centraldy_3x3_16s
#define HT_CENTRALDY_3X3_16S_KN    XSTR(HT_CENTRALDY_3X3_16S_K)  

#define CONVOLVE_3X3_08U_K      convolve_3x3_08u
#define CONVOLVE_3X3_08U_KN      XSTR(CONVOLVE_3X3_08U_K) 
#define CONVOLVE_SCALE_3X3_08U_K    convolve_scale_3x3_08u
#define CONVOLVE_SCALE_3X3_08U_KN   XSTR(CONVOLVE_SCALE_3X3_08U_K)
#define CONVOLVE_3X3_16S_K      convolve_3x3_16s
#define CONVOLVE_3X3_16S_KN      XSTR(CONVOLVE_3X3_16S_K)
#define CONVOLVE_SCALE_3X3_16S_K    convolve_scale_3x3_16s
#define CONVOLVE_SCALE_3X3_16S_KN   XSTR(CONVOLVE_SCALE_3X3_16S_K)
#define CONVOLVE_5X5_08U_K      convolve_5x5_08u
#define CONVOLVE_5X5_08U_KN      XSTR(CONVOLVE_5X5_08U_K)
#define CONVOLVE_SCALE_5X5_08U_K    convolve_scale_5x5_08u
#define CONVOLVE_SCALE_5X5_08U_KN   XSTR(CONVOLVE_SCALE_5X5_08U_K)
#define CONVOLVE_5X5_16S_K      convolve_5x5_16s
#define CONVOLVE_5X5_16S_KN      XSTR(CONVOLVE_5X5_16S_K) 
#define CONVOLVE_SCALE_5X5_16S_K    convolve_scale_5x5_16s
#define CONVOLVE_SCALE_5X5_16S_KN   XSTR(CONVOLVE_SCALE_5X5_16S_K)

#define PREWITT_X_3X3_K    prewitt_x_3x3
#define PREWITT_X_3X3_KN  XSTR(PREWITT_X_3X3_K) 
#define PREWITT_Y_3X3_K    prewitt_y_3x3
#define PREWITT_Y_3X3_KN  XSTR(PREWITT_Y_3X3_K) 

#define HT_SATURATE_08S_K  ht_saturate_08s
#define HT_SATURATE_08S_KN  XSTR(HT_SATURATE_08S_K)

#define HT_SEPARABLE_3X3_16S_K  ht_separable_3x3_16s
#define HT_SEPARABLE_3X3_16S_KN XSTR(HT_SEPARABLE_3X3_16S_K)

#define HT_SEPARABLE_5X5_16S_K  ht_separable_5x5_16s
#define HT_SEPARABLE_5X5_16S_KN XSTR(HT_SEPARABLE_5X5_16S_K)

#define INPUT_0    "INPUT_0"
#define INPUT_1    "INPUT_1"
#define OUTPUT_0  "OUTPUT_0"
#define OUTPUT_1  "OUTPUT_1"
#define R_LUT    "R_LUT"
#define D_LUT    "D_LUT"
#define SIGMA_R    "SIGMA_R"
#define SIGMA_D    "SIGMA_D"
#define F_COEFF    "F_COEFF"
#define F_SCALE    "F_SCALE"
#define F_ROW    "F_ROW"
#define F_COL    "F_COL"
#define BUFFER_0  "BUFFER_0"



#ifdef APEX2_EMULATE

#include "acf_kernel.hpp"
using namespace APEX2;

void BILATERAL_5X5_K(kernel_io_desc lIn0, kernel_io_desc lRLut, kernel_io_desc lDLut, kernel_io_desc lSigmaR, kernel_io_desc lSigmaD, kernel_io_desc lOut0);
void HT_SATURATE_08S_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void HT_SEPARABLE_5X5_16S_K(kernel_io_desc lIn0, kernel_io_desc row, kernel_io_desc col, kernel_io_desc lOut0);
void HT_SEPARABLE_3X3_16S_K(kernel_io_desc lIn0, kernel_io_desc f_row, kernel_io_desc f_col, kernel_io_desc lOut0);
void HT_BOX_3X3_08U_K(kernel_io_desc lIn0,  kernel_io_desc lOut0);
void HT_GENERIC_3X3_08U_K(kernel_io_desc lIn0, kernel_io_desc fcoef, kernel_io_desc fscale, kernel_io_desc lOut0);
void HT_GENERIC_5X5_08U_K(kernel_io_desc lIn0, kernel_io_desc fcoef, kernel_io_desc fscale, kernel_io_desc lOut0);
void HT_CENTRALDX_3X3_16S_K(kernel_io_desc lIn0, kernel_io_desc k0, kernel_io_desc k1, kernel_io_desc k2, kernel_io_desc lOut0);
void HT_CENTRALDY_3X3_16S_K(kernel_io_desc lIn0, kernel_io_desc k0, kernel_io_desc k1, kernel_io_desc k2, kernel_io_desc lOut0);
void BOX_1X3_16S_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void BOX_3X1_16S_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void BOX_3X3_08U_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void BOX_3X3_16S_K(kernel_io_desc lIn, kernel_io_desc lOut);
void BOX_5X5_16S_K(kernel_io_desc lIn, kernel_io_desc lOut);
void CENSUS_3X3_08U_K(kernel_io_desc lIn0, kernel_io_desc lOut0);                 
void DILATE_3X3_08U_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void DILATE_3X3_16S_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void ERODE_3X3_08U_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void GAUSSIAN_3X3_08U_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void GAUSSIAN_5X5_08U_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void MEDIAN_3X3_08U_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void MEDIAN_5X5_08U_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void HT_SOBEL_3X3_16S_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void HT_SOBEL_X_3X3_08S_K(kernel_io_desc lIn0, kernel_io_desc buffer,  kernel_io_desc lOut0);
void HT_SOBEL_Y_3X3_08S_K(kernel_io_desc lIn0, kernel_io_desc buffer, kernel_io_desc lOut0);
void HT_SOBEL_X_3X3_16S_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void HT_SOBEL_Y_3X3_16S_K(kernel_io_desc lIn0,  kernel_io_desc lOut0);
void HT_SOBEL_X_5X5_16S_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void HT_SOBEL_Y_5X5_16S_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void SOBEL_X_3X3_08U_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void SOBEL_Y_3X3_08U_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void SOBEL_X_3X3_08S_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void SOBEL_Y_3X3_08S_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void SOBEL_XY_3X3_08U_K(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lOut1);
void SOBEL_3X3_08U_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void SOBEL_X_5X5_08U_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void SOBEL_X_5X5_08S_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void SOBEL_Y_5X5_08U_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void SOBEL_Y_5X5_08S_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void SOBEL_5X5_08U_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void CONVOLVE_3X3_08U_K(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lFilterCoeff);
void CONVOLVE_SCALE_3X3_08U_K(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lFilterCoeff, kernel_io_desc lFilterScale);
void CONVOLVE_3X3_16S_K(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lFilterCoeff);
void CONVOLVE_SCALE_3X3_16S_K(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lFilterCoeff, kernel_io_desc lFilterScale);
void CONVOLVE_5X5_08U_K(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lFilterCoeff);
void CONVOLVE_SCALE_5X5_08U_K(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lFilterCoeff, kernel_io_desc lFilterScale);
void CONVOLVE_5X5_16S_K(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lFilterCoeff);
void CONVOLVE_SCALE_5X5_16S_K(kernel_io_desc lIn0, kernel_io_desc lOut0, kernel_io_desc lFilterCoeff, kernel_io_desc lFilterScale);
void PREWITT_X_3X3_K(kernel_io_desc lIn0, kernel_io_desc lOut0);
void PREWITT_Y_3X3_K(kernel_io_desc lIn0, kernel_io_desc lOut0);

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(BILATERAL_5X5_K);
extKernelInfoDecl(HT_SATURATE_08S_K);
extKernelInfoDecl(HT_SEPARABLE_5X5_16S_K);
extKernelInfoDecl(HT_SEPARABLE_3X3_16S_K);
extKernelInfoDecl(HT_BOX_3X3_08U_K);
extKernelInfoDecl(HT_GENERIC_3X3_08U_K);
extKernelInfoDecl(HT_GENERIC_5X5_08U_K);
extKernelInfoDecl(HT_CENTRALDX_3X3_16S_K);
extKernelInfoDecl(HT_CENTRALDY_3X3_16S_K);
extKernelInfoDecl(BOX_1X3_16S_K);
extKernelInfoDecl(BOX_3X1_16S_K);
extKernelInfoDecl(BOX_3X3_08U_K);
extKernelInfoDecl(BOX_3X3_16S_K);
extKernelInfoDecl(BOX_5X5_16S_K);
extKernelInfoDecl(CENSUS_3X3_08U_K);
extKernelInfoDecl(DILATE_3X3_08U_K);
extKernelInfoDecl(DILATE_3X3_16S_K);
extKernelInfoDecl(ERODE_3X3_08U_K);
extKernelInfoDecl(GAUSSIAN_3X3_08U_K);
extKernelInfoDecl(GAUSSIAN_5X5_08U_K);
extKernelInfoDecl(MEDIAN_3X3_08U_K);
extKernelInfoDecl(MEDIAN_5X5_08U_K);
extKernelInfoDecl(HT_SOBEL_3X3_16S_K);
extKernelInfoDecl(HT_SOBEL_X_3X3_08S_K);
extKernelInfoDecl(HT_SOBEL_Y_3X3_08S_K);
extKernelInfoDecl(HT_SOBEL_X_3X3_16S_K);
extKernelInfoDecl(HT_SOBEL_Y_3X3_16S_K);
extKernelInfoDecl(HT_SOBEL_X_5X5_16S_K);
extKernelInfoDecl(HT_SOBEL_Y_5X5_16S_K);
extKernelInfoDecl(SOBEL_X_3X3_08U_K);
extKernelInfoDecl(SOBEL_Y_3X3_08U_K);
extKernelInfoDecl(SOBEL_X_3X3_08S_K);
extKernelInfoDecl(SOBEL_Y_3X3_08S_K);
extKernelInfoDecl(SOBEL_XY_3X3_08U_K);
extKernelInfoDecl(SOBEL_3X3_08U_K);
extKernelInfoDecl(SOBEL_X_5X5_08U_K);
extKernelInfoDecl(SOBEL_X_5X5_08S_K);
extKernelInfoDecl(SOBEL_Y_5X5_08U_K);
extKernelInfoDecl(SOBEL_Y_5X5_08S_K);
extKernelInfoDecl(SOBEL_5X5_08U_K);
extKernelInfoDecl(CONVOLVE_3X3_08U_K);
extKernelInfoDecl(CONVOLVE_SCALE_3X3_08U_K);
extKernelInfoDecl(CONVOLVE_3X3_16S_K);
extKernelInfoDecl(CONVOLVE_SCALE_3X3_16S_K);
extKernelInfoDecl(CONVOLVE_5X5_08U_K);
extKernelInfoDecl(CONVOLVE_SCALE_5X5_08U_K);
extKernelInfoDecl(CONVOLVE_5X5_16S_K);
extKernelInfoDecl(CONVOLVE_SCALE_5X5_16S_K);
extKernelInfoDecl(PREWITT_X_3X3_K);
extKernelInfoDecl(PREWITT_Y_3X3_K);
#endif

#endif /* IMAGEFILTERSACF_H */
