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
 
/* !*********************************************************************************
*  @ file
*  @ brief ACF metadata and wrapper function for the \ref secColumnFilter "column filter".
***********************************************************************************/

/*!
* \file col_filter_a_acf.cpp
* \addtogroup filtering
* \addtogroup col_filter
* \ingroup filtering
* @{
* \brief Filter an image row with a column_filter (i.e. Matrix * Col_Vector multiplication)
*/
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA
#include "col_filter_a_acf.h"

const int FILTER_COLS = 3;                  /*!< The number of columns (i.e. size) of the column filter.*/
const int COL_PADDING = FILTER_COLS >> 1;   /*!< The number of padded columns that must be added to both sides of the source image.*/

/* ! *********************************************************************************
*  \ brief ACF metadata for the \ref secColumnFilter "column filter".
*  \ see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/



/****************************************************************************/
/*!
Column filter kernel metadata.

\param COL_FILTER_KN				Define for Kernel name
\param 3							Number of ports
\param "Port COL_FILTER_KN_SRC"		Define for name of input image (unsigned 8bit)
\param "Port COL_FILTER_KN_COEFFS"  Define for name of filter coefficients(unsigned 8bit)
\param "Port COL_FILTER_KN_DST"		Define for name of output image (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(COL_FILTER_K)
(
   COL_FILTER_KN,
   3,
   __port(__index(0),
      __identifier(COL_FILTER_KN_SRC),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(COL_PADDING,COL_PADDING,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   __port(__index(1),
      __identifier(COL_FILTER_KN_COEFFS),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED/*ACF_ATTR_IN_STATIC_GLB_FIXED*/),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(FILTER_COLS, 1)),
   __port(__index(2),
      __identifier(COL_FILTER_KN_DST),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(1, 1))
);

#endif   // #ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION
#include "col_filter_a_apu.h"

#ifdef APEX2_EMULATE
#include "apu_lib.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

const int FILTER_Q = 8;   /*!< The number of fractional bits for the fixed point coefficients.*/

/*!*********************************************************************************
*  \ brief ACF wrapper function for the \ref secColumnFilter "column filter".
*  \ see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void col_filter(kernel_io_desc src    /*!< The padded source image.*/,
                kernel_io_desc coeffs /*!< The column filter coefficients.*/,
                kernel_io_desc dst    /*!< The destination image.*/)
{   
   const vec08u* srcPtr           = (const vec08u*)src.pMem;
   const unsigned char* coeffsPtr = (const unsigned char*)coeffs.pMem;
         vec08u* dstPtr           = (vec08u*)dst.pMem;

   col_filter(dstPtr, dst.chunkSpan,
              srcPtr, src.chunkSpan, src.chunkHeight, src.chunkWidth,
              coeffsPtr, coeffs.chunkWidth, FILTER_Q);
}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */

