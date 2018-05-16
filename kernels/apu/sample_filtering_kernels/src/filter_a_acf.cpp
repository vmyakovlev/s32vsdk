/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
* Copyright (c) 2017 NXP
*  All Rights Reserved
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
* \file filter_a_acf.cpp
* \addtogroup filtering 
* \addtogroup general filtering
* \ingroup filtering
* @{
* \brief General Filtering
*/

#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif


#ifdef ACF_KERNEL_METADATA

#include "filter_a_acf.h"
#include "filter_a_cfg.h"


/****************************************************************************/
/*! 
General filtering function with a 2D-filter

\param FILTER_A_KN				  Define for Kernel name
\param 3						  Number of ports
\param "Port FILTER_A_KN_IN"	  Define for name of input image (unsigned 8bit)
\param "Port FILTER_A_KN_IN_COEF" Define for name of filter coefficients (totally FILTER_W * FILTER_H coefficients are allowed right now) (unsigned 8bit)
\param "Port FILTER_A_KN_OUT"	  Define for name of filtering result (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(FILTER_A_K)
(
   FILTER_A_KN,
   3,
   __port(__index(0),
          __identifier(FILTER_A_KN_IN),
		  __attributes(ACF_ATTR_VEC_IN),
		  __spatial_dep(FILTER_HW, FILTER_HW, FILTER_HH, FILTER_HH),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier(FILTER_A_KN_IN_COEF),
		  __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
		  __ek_size(FILTER_NRCOEFS, 1)),
   __port(__index(2),
          __identifier(FILTER_A_KN_OUT),
		  __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "filter_a_apu.h"

void apu_filter_a (kernel_io_desc lIn0, kernel_io_desc lInCoef, kernel_io_desc lOut0)
{
   vec08u*        lpvIn0   = (vec08u*)       lIn0.pMem;
   unsigned char* lpInCoef = (unsigned char*)lInCoef.pMem;
   vec08u*        lpvOut0  = (vec08u*)       lOut0.pMem;

   apu_filter_a_impl(lpvIn0, int16s(lIn0.chunkSpan),
                lpInCoef,
				lpvOut0, int16s(lOut0.chunkSpan),
				int16s(lIn0.chunkWidth), int16s(lIn0.chunkHeight));
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
