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
* \file gradient_acf.cpp
* \addtogroup filtering
* \addtogroup gradient Image gradient
* \ingroup filtering
* @{
* \brief Image gradient
*/
#ifdef ACF_KERNEL_METADATA
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#include "gradient_acf.h"


/****************************************************************************/
/*!
   Image gradient kernel metadata.
   
   \param GRADIENT_KN			  Define for Kernel name
   \param 3						  Number of ports
   \param "Port GRADIENT_KN_IN"	  Define for name of input image (unsigned 8bit)
   \param "Port GRADIENT_KN_OUTX" Define for name of Gradient X component output image (signed 16bit)
   \param "Port GRADIENT_KN_OUTY" Define for name of Gradient Y component output image (signed 16bit)
*****************************************************************************/

KERNEL_INFO kernelInfoConcat(GRADIENT_K)
(
GRADIENT_KN,
3,
__port(__index(0),
__identifier(GRADIENT_KN_IN),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(1, 1, 1, 1),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),
__port(__index(1),
__identifier(GRADIENT_KN_OUTX),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d16s),
__e0_size(1, 1),
__ek_size(1, 1)),
__port(__index(2),
__identifier(GRADIENT_KN_OUTY),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d16s),
__e0_size(1, 1),
__ek_size(1, 1))
);




KERNEL_INFO kernelInfoConcat(GRADIENT_08S_K)
(
GRADIENT_08S_KN,
   3,
   __port(__index(0),
          __identifier(GRADIENT_KN_IN),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(1,1,1,1),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier(GRADIENT_KN_OUTX),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
		  __e0_data_type(d08s),
		  __e0_size(1, 1),
		  __ek_size(1, 1)),
   __port(__index(2),
          __identifier(GRADIENT_KN_OUTY),
		  __attributes(ACF_ATTR_VEC_OUT),
		  __spatial_dep(0, 0, 0, 0),
          __e0_data_type(d08s),
		  __e0_size(1, 1),
		  __ek_size(1, 1))
);





/****************************************************************************/
/*!
Image gradient kernel metadata.

\param GRADIENT_ABS_KN				Define for Kernel name
\param 4							Number of ports
\param "Port GRADIENT_KN_IN"		Define for name of input image (unsigned 8bit)
\param "Port GRADIENT_KN_OUTX"		Define for name of Gradient X component output image (signed 16bit)
\param "Port GRADIENT_KN_OUTY"		Define for name of Gradient Y component output image (signed 16bit)
\param "Port GRADIENT_KN_ABSGRAD"	Define for name of sum of  absolute values of grad X/Y components output image (unsigned 16bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(GRADIENT_ABS_K)
(
GRADIENT_ABS_KN,
4,
__port(__index(0),
	__identifier(GRADIENT_KN_IN),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(1, 1, 1, 1),
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(1),
	__identifier(GRADIENT_KN_OUTX),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d08s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(2),
	__identifier(GRADIENT_KN_OUTY),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d08s),
	__e0_size(1, 1),
	__ek_size(1, 1)),
__port(__index(3),
	__identifier(GRADIENT_KN_ABSSUM),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(1, 1))
);


/****************************************************************************/
/*!
Image gradient kernel metadata.

\param GRADIENT_ABS_KN				Define for Kernel name
\param 2							Number of ports
\param "Port GRADIENT_KN_IN"		Define for name of input image (unsigned 8bit)
\param "Port GRADIENT_KN_ABSGRAD"	Define for name of sum of  absolute values of grad X/Y components output image (unsigned 16bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(GR_ABS_K)
(
GR_ABS_KN,
2,
__port(__index(0),
__identifier(GRADIENT_KN_IN),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(1, 1, 1, 1),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),
__port(__index(1),
__identifier(GRADIENT_KN_ABSSUM),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1))
);
#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#include "gradient_apu.h"

void apu_gradient (kernel_io_desc lIn0, kernel_io_desc lgradX, kernel_io_desc lgradY)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec16s* lpvgradX  = (vec16s*)lgradX.pMem;
   vec16s* lpvgradY  = (vec16s*)lgradY.pMem;

   apuGradient(lpvgradX, lpvgradY, lpvIn0,
     lgradX.chunkWidth, lgradX.chunkHeight, lIn0.chunkSpan);
}


void apu_gradient_out08s(kernel_io_desc lIn0, kernel_io_desc lgradX, kernel_io_desc lgradY)
{
	vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
	vec08s* lpvgradX = (vec08s*)lgradX.pMem;
	vec08s* lpvgradY = (vec08s*)lgradY.pMem;

	apuGradient_out08s(lpvgradX, lpvgradY, lpvIn0,
		lgradX.chunkWidth, lgradX.chunkHeight, lIn0.chunkSpan);
}


void apu_gradient_abs(kernel_io_desc lIn0, kernel_io_desc lgradX, kernel_io_desc lgradY, kernel_io_desc labsGrad)
{
	vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
	vec08s* lpvgradX = (vec08s*)lgradX.pMem;
	vec08s* lpvgradY = (vec08s*)lgradY.pMem;
	vec08u* lpvabsGrad = (vec08u*)labsGrad.pMem;

	apuGradientAbs(lpvgradX, lpvgradY, lpvabsGrad, lpvIn0,
		lgradX.chunkWidth, lgradX.chunkHeight, lIn0.chunkSpan);
}


void apu_gr_abs( kernel_io_desc lIn0, kernel_io_desc labsGrad)
{
	vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
	vec08u* lpvabsGrad = (vec08u*)labsGrad.pMem;

	apuGradAbs(lpvabsGrad, lpvIn0,
		labsGrad.chunkWidth, labsGrad.chunkHeight, lIn0.chunkSpan);
}


#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
