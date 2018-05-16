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
* \file dot_sqr_acf.cpp
* \addtogroup arithmetic
* \addtogroup square
* \ingroup arithmetic
* @{
* \brief Element-wise square
*/
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA
#include "dot_sqr_acf.h"


/****************************************************************************/
/*!
Square kernel metadata. Computespixelwise the square of a input signed 16bit images.
Outputs unsigned 32bit square result

\param  SQR_In16s_Out32u_KN	Define for Kernel name
\param 2					Number of ports
\param "Port SQR_KN_IN"		Define for name of input image (signed 16bit)
\param "Port SQR_KN_OUT"	Define for name of square of input result (unsigned 32bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(SQR_In16s_Out32u_K) 
(
SQR_In16s_Out32u_KN,
2,
__port(__index(0),
__identifier(SQR_KN_IN),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0), 
__e0_data_type(d16s),
__e0_size(1, 1),
__ek_size(1, 1)),
__port(__index(1),
__identifier(SQR_KN_OUT),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d32u),
__e0_size(1, 1),
__ek_size(1, 1))
);

/****************************************************************************/
/*!
Square kernel metadata. Computespixelwise the square of a input signed 32bit images.
Outputs unsigned 32bit square result

\warning Does not check out of range values above/below +/-2^(31-1)
\param  SQR_In32s_Out32u_KN	Define for Kernel name
\param 2					Number of ports
\param "Port SQR_KN_IN"		Define for name of input image (signed 32bit)
\param "Port SQR_KN_OUT"	Define for name of square of input result (unsigned 32bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(SQR_In32s_Out32u_K)
(
SQR_In32s_Out32u_KN,
2,
__port(__index(0),
__identifier(SQR_KN_IN),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(1),
__identifier(SQR_KN_OUT),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32u),
	__e0_size(1, 1),
	__ek_size(1, 1))
);

/****************************************************************************/
/*!
Square kernel metadata. Computespixelwise the square of a input signed 32bit images.
Outputs unsigned 64bit square result

\param  SQR_In32s_Out32u_KN		Define for Kernel name
\param 2						Number of ports
\param "Port SQR_KN_IN"			Define for name of input image (signed 32bit)
\param "Port SQR_KN_OUT_HIGH"	Define for name of high word of square of input result (unsigned 32bit) 
\param "Port SQR_KN_OUT_LOW"	Define for name of low word of square of input result (unsigned 32bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(SQR_In32s_Out64u_K)
(
   SQR_In32s_Out64u_KN,
   3,
   __port(__index(0),
          __identifier(SQR_KN_IN),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0), 
          __e0_data_type(d32s),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier(SQR_KN_OUT_HIGH),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
  __port(__index(2),
		  __identifier(SQR_KN_OUT_LOW),
		  __attributes(ACF_ATTR_VEC_OUT),
		  __spatial_dep(0, 0, 0, 0),
		  __e0_data_type(d32u),
		  __e0_size(1, 1),
		  __ek_size(1, 1))

);




#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "dot_sqr_apu.h"
 
void apu_dot_sqr_in16s_out32u(kernel_io_desc lImgInA, kernel_io_desc lImgOut)
{
	vec16s*  lpvImgInA = (vec16s*) lImgInA.pMem;
	vec32u*  lpvImgOut = (vec32u*) lImgOut.pMem;
	int inDataSz = sizeof(int16s); //sizeof(vec16s)
	int outDataSz = sizeof(int32u);

	dot_sqr_in16s_out32u_filter(lpvImgOut, lpvImgInA, lImgInA.chunkWidth, lImgInA.chunkHeight, lImgInA.chunkSpan / inDataSz, lImgOut.chunkSpan / outDataSz);
}


void apu_dot_sqr_in32s_out32u(kernel_io_desc lImgInA, kernel_io_desc lImgOut)
{
	vec32s*  lpvImgInA = (vec32s*)lImgInA.pMem;
	vec32u*  lpvImgOut = (vec32u*)lImgOut.pMem;

	int inDataSz = sizeof(int32s); 
	int outDataSz = sizeof(int32u);

	dot_sqr_in32s_out32u_filter(lpvImgOut, lpvImgInA, lImgInA.chunkWidth, lImgInA.chunkHeight, lImgInA.chunkSpan / inDataSz, lImgOut.chunkSpan / outDataSz);
}


void apu_dot_sqr_in32s_out64u(kernel_io_desc lImgInA, kernel_io_desc lImgOut_high, kernel_io_desc lImgOut_low)
{
   vec32s*  lpvImgInA	= (vec32s*) lImgInA.pMem;
   vec32u*  lpvImgOut_high	= (vec32u*) lImgOut_high.pMem;
   vec32u*  lpvImgOut_low = (vec32u*)lImgOut_low.pMem;
   int inDataSz = sizeof(int32s); //sizeof(ve16s)
   int outDataSz = sizeof(int32u);

   
   dot_sqr_in32s_out64u_filter(lpvImgOut_high, lpvImgOut_low, lpvImgInA, lImgInA.chunkWidth, lImgInA.chunkHeight, lImgInA.chunkSpan / inDataSz, lImgOut_low.chunkSpan / outDataSz);
}



#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
