/******************************************************************************
 *  (C) Copyright CogniVue Corporation. 2012 All right reserved.
 * 
 *  Confidential Information
 *
 *  All parts of the CogniVue Program Source are protected by copyright law
 *  and all rights are reserved.
 *  This documentation may not, in whole or in part, be copied, photocopied,
 *  reproduced, translated, or reduced to any electronic medium or machine
 *  readable form without prior consent, in writing, from CogniVue.
 ******************************************************************************/

/*!
* \file filter_median_3x3_acf.cpp
* \addtogroup filtering
* \addtogroup median Median filtering
* \ingroup filtering
* @{
* \brief Median filtering
*/

#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA
#include "filter_median_3x3_acf.h"



/****************************************************************************/
/*!
Filtering with a Median 3x3-filter
\brief Filtering with a Median 3x3-filter
\param MEDIAN_3X3_8BPP_KN			Define for Kernel name
\param 2							Number of ports
\param "Port MEDIAN_3X3_8BPP_IN"	Define for name of input image (unsigned 8bit)
\param "Port MEDIANL_3X3_8BPP_OUT"	Define for name of filtering result (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(MEDIAN_3X3_K)
(
   MEDIAN_3X3_KN,
   2,
   __port(__index(0),
          __identifier(MEDIAN_3X3_KN_OUT),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier(MEDIAN_3X3_KN_IN),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(1,1,1,1),
//          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA


 
#ifdef ACF_KERNEL_IMPLEMENTATION

#include "filter_median_3x3_apu.h"
 
// Wrapper
void median_3x3_8bpp (kernel_io_desc dst, kernel_io_desc src)
{
   int sstride     = src.chunkSpan / sizeof(int08u);  // stride of the src and dst arrays in cmem
   int dstride     = dst.chunkSpan / sizeof(int08u);  // stride of the src and dst arrays in cmem

   apu_flt_median_3x3(
      (vec08u*)dst.pMem,
      dstride,
      (const vec08u*)src.pMem,
      sstride,
      src.chunkWidth,
      src.chunkHeight);
}

#endif // kernel implementation


/*! @} */
