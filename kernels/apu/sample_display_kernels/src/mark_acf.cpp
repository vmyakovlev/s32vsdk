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
* \file mark_acf.cpp
* \addtogroup display
* \addtogroup marking Marking on images
* \ingroup display
* @{
* \brief Image marking
*/
#ifdef ACF_KERNEL_METADATA
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#include "mark_acf.h"


/****************************************************************************/
/*!
   Image marking kernel metadata.
   
   \param MARK_KN				Define for Kernel name
   \param 3                     Number of ports
   \param "Port MARK_KN_IN"		Define for name of input image (unsigned 8bit)
   \param "Port MARK_KN_MARKER" Define for name of input marker image (unsigned 8bit)
   \param "Port MARK_KN_OUT"    Define for name of output image (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(MARK_K)
(
   MARK_KN,
   3,
   __port(__index(0),
          __identifier(MARK_KN_IN),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier(MARK_KN_MARKER),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(2),
          __identifier(MARK_KN_OUT),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
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

#include "mark_apu.h"

void apu_mark (kernel_io_desc lInImage, kernel_io_desc lInMarker, kernel_io_desc lOut)
{
   vec08u*        lpvInImage   = (vec08u*) lInImage.pMem;
   vec08u*        lpvInMarker   = (vec08u*) lInMarker.pMem;
   vec08u*        lpvOut  = (vec08u*)  lOut.pMem;

   mark(lpvOut, lpvInImage, lpvInMarker, lInImage.chunkWidth, lInImage.chunkHeight, lInImage.chunkSpan, lInMarker.chunkSpan);

}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
