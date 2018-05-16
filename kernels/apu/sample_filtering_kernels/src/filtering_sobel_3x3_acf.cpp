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
* \file filtering_sobel_3x3_acf.cpp
* \addtogroup filtering
* \addtogroup sobel Sobel Filtering
* \ingroup filtering
* @{
* \brief Sobel filtering
*/


#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA
#include "filtering_sobel_3x3_acf.h"
 

/****************************************************************************/
/*!
\brief Calculate sum of absolute values of first order derivatives x and y using sobel 3x3
\param SOBEL_3X3_8BPP_KN		 Define for Kernel name
\param 2						 Number of ports
\param "Port SOBEL_3X3_8BPP_IN"	 Define for name of input image (unsigned 8bit)
\param "Port SOBEL_3X3_8BPP_OUT" Define for name of filtering result (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(SOBEL_3X3_8BPP_K)
(
  SOBEL_3X3_8BPP_KN,
  2,
 
  __port(__index(0),
	__identifier(SOBEL_3X3_8BPP_IN),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(1,1,1,1),
	__e0_data_type(d08u),
	__e0_size(1,1),
	__ek_size(1,1))
	,
 
  __port(__index(1),
	__identifier(SOBEL_3X3_8BPP_OUT),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0,0,0,0),
	__e0_data_type(d08u),
	__e0_size(1,1),
	__ek_size(1,1))

);

 
#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION
//#include "filtering_sobel_3x3.h"
#include "filtering_sobel_3x3_apu.h"
 
void sobel_3x3_8bpp(kernel_io_desc in, kernel_io_desc out)

{

    vec08u* lpvin = (vec08u* ) in.pMem;
    vec08u* lpvout = (vec08u* ) out.pMem;
     
	int sstride = in.chunkSpan / sizeof(int08u);
	int dstride = out.chunkSpan / sizeof(int08u);

	apu_flt_sobel_3x3(
		lpvout,
		dstride,
		lpvin,
		sstride,
		in.chunkWidth,
		in.chunkHeight);
}

#endif


/*! @} */
