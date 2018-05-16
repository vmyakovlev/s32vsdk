/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
* Copyright (c) 2017 NXP
* 
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

/*!
* \file harris_acf.cpp
* \addtogroup feature_detection Feature detection
* \addtogroup harris Harris corner detection
* \ingroup feature_detection
* @{
* \brief Harris feature point detection
*/
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif
#include "harris_defines.h"

#ifdef ACF_KERNEL_METADATA

#include "harris_acf.h"

/****************************************************************************/
/*!
   Harris corner detection kernel metadata.
   
   \param HARRIS_KN						Define for Kernel name
   \param 4								Number of ports
   \param "Port HARRIS_KN_IN_GX"		Define for name of gradient X component input image (signed 16bit)
   \param "Port HARRIS_KN_IN_GY"		Define for name of gradient Y component input image (signed 16bit)
   \param "Port HARRIS_KN_IN_K_RBS_WIN" Define for name of detector sensitivity, response bit shift and window size (unsigned 16bit)
   \param "Port HARRIS_KN_OUT_RESP"		Define for name of output image (unsigned 16bit)
   \param "Port HARRIS_KN_OUT_TEMP_BUF" Define for name of temporary buffer (signed 16bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(HARRIS_K)
(
   HARRIS_KN,
   5,
   __port(__index(0),
          __identifier(HARRIS_KN_IN_GX),
          __attributes(ACF_ATTR_VEC_IN),
		  __spatial_dep(HARRIS_HALF_WINDOW_SIZE, HARRIS_HALF_WINDOW_SIZE, HARRIS_HALF_WINDOW_SIZE, HARRIS_HALF_WINDOW_SIZE),
          __e0_data_type(d16s),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier(HARRIS_KN_IN_GY),
          __attributes(ACF_ATTR_VEC_IN),
		  __spatial_dep(HARRIS_HALF_WINDOW_SIZE, HARRIS_HALF_WINDOW_SIZE, HARRIS_HALF_WINDOW_SIZE, HARRIS_HALF_WINDOW_SIZE),
          __e0_data_type(d16s),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(2),
          __identifier(HARRIS_KN_IN_K_RBS_WIN),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0, 0, 0, 0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(4, 1)),  
   __port(__index(3),
          __identifier(HARRIS_KN_OUT_RESP),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(4),
          __identifier(HARRIS_KN_OUT_TEMP_BUF),
          __attributes(ACF_ATTR_VEC_OUT),
		  __spatial_dep(HARRIS_HALF_WINDOW_SIZE, HARRIS_HALF_WINDOW_SIZE, HARRIS_HALF_WINDOW_SIZE, HARRIS_HALF_WINDOW_SIZE),
          __e0_data_type(d16s),
          __e0_size(1, 1),
          __ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#include "harris_apu.h"

void apu_harris(kernel_io_desc lInGradX, kernel_io_desc lInGradY, kernel_io_desc lIn_K_RBS_Win, kernel_io_desc lOutResp, kernel_io_desc lOutTmp)
{
	vec16s* gradX = (vec16s*)lInGradX.pMem;
	vec16s* gradY = (vec16s*)lInGradY.pMem;
   vec16u* out  = (vec16u*)lOutResp.pMem;
   vec16s* temp  = (vec16s*)lOutTmp.pMem;
   int16u* inkw = (int16u*)lIn_K_RBS_Win.pMem;
   int k = int(inkw[0]);
   int responseBitShift = int(inkw[1]);
   int window = HARRIS_HALF_WINDOW_SIZE > int(inkw[2]) ? int(inkw[2]) : HARRIS_HALF_WINDOW_SIZE;
   int thr = int(inkw[3]);
   
   bool isFirstSlice = ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG);
   apuHarrisResponse(out, gradX, gradY, temp,
	   lOutResp.chunkWidth, lOutResp.chunkHeight, lInGradX.chunkSpan / sizeof(int16s), k, responseBitShift, window, thr, isFirstSlice);
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION 

/*! @} */
