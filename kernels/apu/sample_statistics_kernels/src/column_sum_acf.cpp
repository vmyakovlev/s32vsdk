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
* \file column_sum_acf.cpp
* \addtogroup statistics
* \ingroup statistics
* @{
* \brief sum of columns thru each row
*/

#ifdef ACF_KERNEL_METADATA
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#include "column_sum_acf.h"

/****************************************************************************/
/*!
   Histogram kernel metadata.
   
   \param HISTOGRAM_KN                    Define for Kernel name
   \param 2                                               Number of ports
   \param "Port HISTOGRAM_KN_IN"  Define for name of input image (unsigned 8bit)
   \param "Port HISTOGRAM_KN_OUT" Define for name of histogram vector output, CU count X 256  (unsigned 32bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(COLUMNS_SUM_K)
(
  COLUMNS_SUM_KN,
  6,
  __port(__index(0),
    __identifier(COLUMNS_SUM_KN_IN),
    __attributes(ACF_ATTR_VEC_IN_FIXED),
    __spatial_dep(0,0,0,0),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(4, 10)),
 __port(__index(1),
    __identifier(COLUMNS_SUM_KN_OUT_DOWN),
    __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
    __spatial_dep(0,0,0,0),
    __e0_data_type(d32u),
    __e0_size(1, 1),
    __ek_size(4, 1)),
 __port(__index(2),
    __identifier(COLUMNS_SUM_KN_OUT_UP),
    __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
    __spatial_dep(0,0,0,0),
    __e0_data_type(d32u),
    __e0_size(1, 1),
    __ek_size(4, 1)),
 __port(__index(3),
    __identifier(COLUMNS_SUM_KN_IN_PRIOR_DOWN),
    __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
    __spatial_dep(0,0,0,0),
    __e0_data_type(d32u),
    __e0_size(1, 1),
    __ek_size(1, 1)),
 __port(__index(4),
    __identifier(COLUMNS_SUM_KN_IN_PRIOR_UP),
    __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
    __spatial_dep(0,0,0,0),
    __e0_data_type(d32u),
    __e0_size(1, 1),
    __ek_size(1, 1)),
 __port(__index(5),
    __identifier(COLUMNS_SUM_KN_IN_START_TILE),
    __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
    __spatial_dep(0,0,0,0),
    __e0_data_type(d32u),
    __e0_size(1, 1),
    __ek_size(1, 1)) 
 
  
);
#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
#include "acf_kernel.hpp"
using namespace APEX2;
#endif

#include "column_sum_apu.h"


void apu_columns_sum(kernel_io_desc lIn0, 
                     kernel_io_desc lOutDown, 
                     kernel_io_desc lOutUp,
                     kernel_io_desc lpriorityDown,
                     kernel_io_desc lpriorityUp,
                     kernel_io_desc lindexOfTileStart)
{
  
  vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
  vec32u* lpvOutDown = (vec32u*)lOutDown.pMem;
  vec32u* lpvOutUp = (vec32u*)lOutUp.pMem;
  
  int priorityDown = ((int*)lpriorityDown.pMem)[0];
  int priorityUp = ((int*)lpriorityUp.pMem)[0];
  int indexOfTileStart = ((int*)lindexOfTileStart.pMem)[0];
  
  int lStrideIn0 = lIn0.chunkSpan;
  
  bool isFirstTile = ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG);
  
  column_sum(lpvIn0,
              lpvOutDown,
              lpvOutUp,
              isFirstTile,
              lStrideIn0,
              lIn0.chunkWidth,
              lIn0.chunkHeight,
              lOutDown.chunkWidth,
              priorityDown,
              priorityUp,
              indexOfTileStart
            );

}
#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
