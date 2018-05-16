/******************************************************************************
 *  (C) Copyright CogniVue Corporation. 2013 All right reserved.
 *  Copyright (c) 2017 NXP
 *   
 *  Confidential Information
 *
 *  All parts of the CogniVue Program Source are protected by copyright law
 *  and all rights are reserved.
 *  This documentation may not, in whole or in part, be copied, photocopied,
 *  reproduced, translated, or reduced to any electronic medium or machine
 *  readable form without prior consent, in writing, from CogniVue.
 *
 *  Created by Jeff Lalonde <jlalonde@cognivue.com>
 ******************************************************************************/
 
/* ! *********************************************************************************
*  \ file
*  \ brief ACF metadata and wrapper function for the row filter.
***********************************************************************************/


/*!
* \file row_filter_a_acf.cpp
* \addtogroup filtering
* \addtogroup row_filter
* \ingroup filtering
* @{
* \brief Filter an image column with a row_filter(i.e. Row_Vector * Matrix multiplication)
*/
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA
#include "row_filter_a_acf.h"

const int FILTER_ROWS = 5;
const int ROW_PADDING = FILTER_ROWS >> 1;



/****************************************************************************/
/*! ROW filter kernel metadata.

\param ROW_FILTER_KN				Define for Kernel name
\param 3							Number of ports
\param "Port ROW_FILTER_KN_SRC"		Define for name of input image (unsigned 8bit)
\param "Port ROW_FILTER_KN_COEFFS"  Define for name of filter coefficients (unsigned 8bit)
\param "Port ROW_FILTER_KN_DST"		Define for name of output image (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(ROW_FILTER_K)
(
   ROW_FILTER_KN,
   3,
   __port(__index(0),
      __identifier(ROW_FILTER_KN_SRC),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0,0,ROW_PADDING,ROW_PADDING),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   __port(__index(1),
      __identifier(ROW_FILTER_KN_COEFFS),
	  __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED/*ACF_ATTR_IN_STATIC_GLB_FIXED*/),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(FILTER_ROWS, 1)),
   __port(__index(2),
      __identifier(ROW_FILTER_KN_DST),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1))
);

#endif   // #ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION
#include "row_filter_a_apu.h"

const int FILTER_Q = 8;

void row_filter(kernel_io_desc src,
                kernel_io_desc coeffs,
                kernel_io_desc dst)
{   
   const vec08u* srcPtr           = (const vec08u*)src.pMem;
   const unsigned char* coeffsPtr = (const unsigned char*)coeffs.pMem;
         vec08u* dstPtr           = (vec08u*)dst.pMem;

   row_filter_impl(dstPtr, dst.chunkSpan,
              srcPtr, src.chunkSpan, src.chunkHeight, src.chunkWidth,
              coeffsPtr, coeffs.chunkWidth, FILTER_Q);
}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION


/*! @} */
