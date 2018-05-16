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
/*!
* \file apu_histogram_equalization.cpp
* \addtogroup image_proc
* \addtogroup histogram_equalization
* \ingroup image_proc
* @{
* \brief Histogram
*/

#ifdef ACF_KERNEL_METADATA
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#include "histogram_equalization_apu.h"
#include "histogram_equalization_acf.h"


/****************************************************************************/
/*!
   Histogram_equalization kernel metadata.
   
   \param apu_histogram_equalization        Kernel name
   \param 2              Number of ports
   \param "Port INPUT_0"  input image (unsigned 8bit)
   \param "Port OUTPUT_0" histogram equalization vector output, CU count X 256  (unsigned 8bit)
   \param "Port LUT" histogram equalization look up table, CU count X 256  (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(HIST_EQU_K)
(
  HIST_EQU_KN,
  4,
   __port(__index(0),
   __identifier(HIST_EQU_K_OUT_IMG),
    __attributes(ACF_ATTR_VEC_OUT),
    __spatial_dep(0,0,0,0),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(1, 1)),
  __port(__index(1),
  __identifier(HIST_EQU_K_IN_IMG),
    __attributes(ACF_ATTR_VEC_IN),
    __spatial_dep(0,0,0,0),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(1, 1)),
  __port(  __index(2),
  __identifier(HIST_EQU_K_LUT),
    __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(256, 1)),
  __port(  __index(3),
  __identifier(HIST_EQU_K_LUT_BUF),
    __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(256, 1)
      )
);

/****************************************************************************/
/*!
   Histogram_equalization kernel metadata.
   
   \param apu_equalized_lut         Kernel name
   \param 3             Number of ports
   \param "Port IMAGE_HISTOGRAM"  input image histogram (unsigned 32bit)
   \param "Port LUT" histogram equalization look up table, CU count X 256  (unsigned 8bit)
   \param "Port NUM_PIXELS" number of pixels in input image
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(EQU_LUT_K)
(
   EQU_LUT_KN,
   3,
    __port(__index(0),
    __identifier(HIST_LUT_K_LUT),
          __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED), 
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(256, 1)),
    __port(__index(1),
    __identifier(HIST_LUT_K_HISTO),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32u),
          __e0_size(1, 1),
          __ek_size(256, 1)),
    __port(__index(2),
    __identifier(HIST_LUT_K_NUM_PIXELS),
          __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);
#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
#include "acf_kernel.hpp"
using namespace APEX2;
#endif

#include "histogram_equalization_apu.h"


void apu_histogram_equalization(
  kernel_io_desc lOut0, 
  kernel_io_desc lIn0, 
  kernel_io_desc scalar_lut, 
  kernel_io_desc vector_lut
  )
{
  vec08u*  lpvIn0     = (vec08u*)lIn0.pMem;
  vec08u*  lpvOut0    = (vec08u*)lOut0.pMem;
  uint8_t* lpvScalar_lut = (uint8_t*)scalar_lut.pMem;
  vec08u*  lpvVector_lut = (vec08u*)vector_lut.pMem;
  
  for(int i = 0; i < 256; i++)
  {
    lpvVector_lut[i] = lpvScalar_lut[i];
  }
 
  apu_histogram_equalization_impl(lpvOut0, lOut0.chunkSpan, lpvIn0, lIn0.chunkSpan, lIn0.chunkWidth, lIn0.chunkHeight, lpvVector_lut);

}

void apu_generate_lut
(  
   kernel_io_desc transformed_histogram,
   kernel_io_desc histogram,
   kernel_io_desc lNumPixels
)
{
   uint8_t* lpvTransformed_histogram = (uint8_t*)transformed_histogram.pMem;
   uint32_t* lpvHistogram = (uint32_t*)histogram.pMem;
   uint32_t* lpvNumPixels = (uint32_t*)lNumPixels.pMem;

   if(ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
   {
     apu_generate_lut_impl(lpvTransformed_histogram, lpvHistogram, *lpvNumPixels); 
   }
}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
