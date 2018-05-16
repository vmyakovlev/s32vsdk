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
* \file sat_box_filter_acf.cpp
* \addtogroup optimization
* \addtogroup sat_box_filter SAT-based box filter
* \ingroup optimization
* @{
* \brief SAT-based box filter
*/
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA
#include "sat_box_filter_acf.h"

#ifdef APEX2_EMULATE
extern const int BOX_SIZE;
extern const int BOX_AREA;
#else 
const int BOX_SIZE = 5;
const int BOX_AREA = (2 * BOX_SIZE + 1) * (2 * BOX_SIZE + 1);
#endif


/****************************************************************************/
/*!
   Box filter using summed area table kernel metadata.
   
   \param SAT_BOX_FILTER_KN			Define for Kernel name
   \param 2							Number of ports
   \param "Port SAT_BOX_FILTER_IN"  Define for name of input summed area table image (unsigned 32bit)
   \param "Port SAT_BOX_FILTER_OUT"	Define for name of output image (unsigned 8bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(SAT_BOX_FILTER_K) //_kernel_info_sat_box_filter
(
  SAT_BOX_FILTER_KN,
  2,
  __port(__index(0),
    __identifier(SAT_BOX_FILTER_IN),
    __attributes(ACF_ATTR_VEC_IN),
    __spatial_dep(BOX_SIZE + 1, BOX_SIZE, BOX_SIZE + 1, BOX_SIZE),
    __e0_data_type(d32u),
    __e0_size(1, 1),
    __ek_size(1, 1)),
  __port(__index(1),
  __identifier(SAT_BOX_FILTER_OUT),
    __attributes(ACF_ATTR_VEC_OUT),
    __spatial_dep(0,0,0,0),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(1, 1))
);


#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "sat_box_filter_apu.h"
#include <string.h>

void apu_sat_box_filter(kernel_io_desc lIn0, kernel_io_desc lOut0)
{
  vec32u* lpvIn0 = (vec32u*)lIn0.pMem;
  vec08u* lpvOut0 = (vec08u*)lOut0.pMem;

  sat_box_filter_impl(lpvOut0, lpvIn0, lIn0.chunkWidth, lIn0.chunkHeight, lIn0.chunkSpan, lOut0.chunkSpan);
}



#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
