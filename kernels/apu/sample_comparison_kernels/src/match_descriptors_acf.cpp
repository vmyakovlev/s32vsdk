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
* \file match_descriptors_acf.cpp
* \addtogroup comparison
* \addtogroup bdm Binary descriptor matching
* \ingroup comparison
* @{
* \brief Binary descriptor matching
*/

#ifdef APEX2_EMULATE
#include <apu_lib.hpp>
#include <acf_kernel.hpp>
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA
#include "match_descriptors_acf.h"

/****************************************************************************/
/*!
   Descriptor matching kernel metadata.
   
   \param MATCH_DESCRIPTORS_KN		Define for Kernel name
   \param 5							Number of ports
   \param "Port MATCH_DESCR_KN_IN0" Define for name of first input descriptors array (unsigned 8bit)
   \param "Port MATCH_DESCR_KN_IN1" Define for name of second input descriptors array (unsigned 8bit)
   \param "Port MATCH_DESCR_KN_CFG" Define for name of configuration data:
										number of descriptors in IN0 (16-bit)
										number of descriptors in IN1 (16-bit)
										matching threshold (max Hamming distance) (8-bit)
										range check (min Hamming distance between the closest and the second closest descriptors found) (8-bit)
   \param "Port MATCH_DESCR_KN_OUT0" Define for name of first elements of match pairs array (unsigned 16bit)
   \param "Port MATCH_DESCR_KN_OUT1" Define for name of second elements of match pairs array (unsigned 16bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(MATCH_DESCRIPTORS_K)
(
    MATCH_DESCRIPTORS_KN,
    5,
  __port(__index(0),
    __identifier(MATCH_DESCR_KN_IN0),
    __attributes(ACF_ATTR_VEC_IN),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(1, 1)),
  __port(__index(1),
    __identifier(MATCH_DESCR_KN_IN1),
    __attributes(ACF_ATTR_VEC_IN),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d08u),
    __e0_size(1, 1),
    __ek_size(1, 1)),
  __port(__index(2),
    __identifier(MATCH_DESCR_KN_CFG),
    __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d16u),
    __e0_size(1, 1),
    __ek_size(4, 1)),
  __port(__index(3),
    __identifier(MATCH_DESCR_KN_OUT0),
    __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d16s),
    __e0_size(1, 1),
    __ek_size(512, 1)),
  __port(__index(4),
    __identifier(MATCH_DESCR_KN_OUT1),
    __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
    __spatial_dep(0, 0, 0, 0),
    __e0_data_type(d16s),
    __e0_size(1, 1),
    __ek_size(512, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "match_descriptors_apu.h"

//TODO: lOut1 really needed only for the is-already-matched checks inside the algorithm
void apu_match_descriptors(kernel_io_desc lIn0, kernel_io_desc lIn1, kernel_io_desc lInConfig, kernel_io_desc lOut0, kernel_io_desc lOut1)
{
  vec08u* lpvIn0 = (vec08u*)lIn0.pMem;
  vec08u* lpvIn1 = (vec08u*)lIn1.pMem;
  int16u* lpvCfg = (int16u*)lInConfig.pMem;
  int16s* lpvOut0 = (int16s*)lOut0.pMem;
  int16s* lpvOut1 = (int16s*)lOut1.pMem;
  int16u descriptorCount0 = lpvCfg[0];
  int16u descriptorCount1 = lpvCfg[1];
  int08u threshold = (int08u)lpvCfg[2];
  int08u rangeCheck = (int08u)lpvCfg[3];

  Match(lpvIn0, descriptorCount0, lpvIn1, descriptorCount1, lpvOut0, lpvOut1, threshold, rangeCheck);
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
