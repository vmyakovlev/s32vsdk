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
* \file sat_acf.cpp
* \addtogroup optimization
* \addtogroup sat Summed area table
* \ingroup optimization
* @{
* \brief Summed area table (integral image) computation
*/
#ifdef APEX2_EMULATE
#include "apu_lib.hpp"
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif



#ifdef ACF_KERNEL_METADATA
#include "sat_acf.h"

/****************************************************************************/
/*!
   Summed area table kernel metadata.
   
   \param SAT_KN				Define for Kernel name
   \param 3						Number of ports
   \param "Port SAT_KN_IN"      Define for name of input image (unsigned 8bit)
   \param "Port SAT_KN_OUT"     Define for name of output image (unsigned 32bit)
   \param "Port SAT_KN_OUT_ROW" Define for name of last row of previous tile buffer (unsigned 32bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(SAT_K)
(
	SAT_KN,
   3,
   __port(__index(0),
   __identifier(SAT_KN_IN),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
    __identifier(SAT_KN_OUT),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   //TODO: enforce height = 1?
   __port(__index(2),
   __identifier(SAT_KN_OUT_ROW),
          __attributes(ACF_ATTR_VEC_OUT_STATIC),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);


#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "sat_apu.h"

void apu_sat(kernel_io_desc lIn, kernel_io_desc lOut, kernel_io_desc lRow)
{
   vec08u*        lpvIn   = (vec08u*) lIn.pMem;
   vec32u*        lpvOut  = (vec32u*)  lOut.pMem;
   vec32u*        lpvRow  = (vec32u*)  lRow.pMem;
   int08u firstTile = (int08u)ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG);

   sat32(lpvOut, lpvRow, lpvIn, lIn.chunkSpan, lOut.chunkSpan, lIn.chunkWidth, lIn.chunkHeight, firstTile);
}


#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
