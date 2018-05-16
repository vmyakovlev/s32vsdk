/******************************************************************************
 *  (C) Copyright CogniVue Corporation. 2013 All right reserved.
 *  Copyright (c) 2017 NXP
 *  Confidential Information
 *
 *  All parts of the CogniVue Program Source are protected by copyright law
 *  and all rights are reserved.
 *  This documentation may not, in whole or in part, be copied, photocopied,
 *  reproduced, translated, or reduced to any electronic medium or machine
 *  readable form without prior consent, in writing, from CogniVue.
 *
 *  Created by: Ramin Azarmehr
 *  Based on code from: Daniel Laroche
 ******************************************************************************/


/*!
* \file resize_vertical_acf.cpp
* \addtogroup resizing
* \addtogroup resize_vertical Image vertical downsampling
* \ingroup resizing
* @{
* \brief Image downsampling
*/


//! Define as preprocessor in simulation project for debugging
#ifdef __APU_DEBUG__ 
  #ifndef ACF_KERNEL_IMPLEMENTATION 
    #define ACF_KERNEL_IMPLEMENTATION 
  #endif
#endif

#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA

#include "resize_vertical_acf.h"


/****************************************************************************/
/*!
Image 2x downsampling kernel metadata.

\param DOWNSAMPLE_KN			Define for Kernel name
\param 2						Number of ports
\param "Port DOWNSAMPLE_KN_IN"	Define for name of input image (unsigned 8bit)
\param "Port DOWNSAMPLE_KN_OUT"	Define for name of output image (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(RESZ_VERT_K)
(
	RESZ_VERT_KN,
	3,
	__port(__index(0),
		__identifier(RESZ_VERT_KN_IN),
		__attributes(ACF_ATTR_VEC_IN),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d08u),
		__e0_size(1, 1),
		__ek_size(2, 2)),
	__port(__index(1),
		__identifier(RESZ_VERT_KN_OUT),
		__attributes(ACF_ATTR_VEC_OUT),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d08u),
		__e0_size(1, 1),
		__ek_size(1, 1)),
	__port(__index(2),
		__identifier(RESZ_VERT_KN_DESCR),
		__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
		__spatial_dep(0, 0, 0, 0),
		__e0_data_type(d08u),
		__e0_size(1, 1),
		__ek_size(sizeof(RESIZE_DESCRIPTOR), 1))
);

#endif

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "resize_vertical_apu.h"

#ifdef APEX2_EMULATE
#include "apu_vec.hpp"
using namespace APEX2;
#endif

//// ACF Wrapper for apu_resize_integer
void resize_vertical(kernel_io_desc input_image, kernel_io_desc output_image,
                     kernel_io_desc rsz_desc)
{
   vec08u* lpInputImage  = (vec08u*) input_image.pMem;
   vec08u* lpOutputImage = (vec08u*) output_image.pMem;
   int32s sourceBlockHeight = input_image.chunkHeight;
   
   RESIZE_DESCRIPTOR* lpResizeDesc = (RESIZE_DESCRIPTOR*) rsz_desc.pMem;
   
//   apu_resize_polyphase(lpOutputImage, lpInputImage, lpResizeDesc);
   apu_resize_polyphase(lpOutputImage, lpInputImage, sourceBlockHeight, lpResizeDesc);
}



#endif //ACF_KERNEL_IMPLEMENTATION

/*! @} */
