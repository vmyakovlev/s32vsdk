/*******************************************************************************
 *
 * NXP Confidential Proprietary
 *
 * Copyright (c) 2016-2017 NXP
 * All Rights Reserved
 *
 *******************************************************************************
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "common_stringify_macros.h"
#include "orb_apu.h"

#define ORB_ORIENTATION_K orb_orientation
#define ORB_ORIENTATION_KN XSTR(ORB_ORIENTATION_K)

#define ORB_RBRIEF_K orb_rbrief
#define ORB_RBRIEF_KN XSTR(ORB_RBRIEF_K)

#ifdef APEX2_EMULATE
#include "acf_kernel.hpp"
using namespace APEX2;
void ORB_ORIENTATION_K(kernel_io_desc aIn,
                       kernel_io_desc aPatch,
                       kernel_io_desc aRadius,
                       kernel_io_desc aOut);

void ORB_RBRIEF_K(kernel_io_desc aIn,
                  kernel_io_desc aOrientation,
                  kernel_io_desc aBitPattern,
                  kernel_io_desc aDescrSizeB,
                  kernel_io_desc aPatch,
                  kernel_io_desc aOut,
                  kernel_io_desc aCount);
#endif

#if (defined(ACF_KERNEL_METADATA) || (!defined(__chess__)))
extKernelInfoDecl(ORB_ORIENTATION_K);
extKernelInfoDecl(ORB_RBRIEF_K);
#endif /* ACF_KERNEL_METADATA  */

#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif /* APEX2_EMULATE */

#ifdef ACF_KERNEL_METADATA
KERNEL_INFO kernelInfoConcat(ORB_ORIENTATION_K)(
  ORB_ORIENTATION_KN,
  4,
  __port(__index(0),
         __identifier("INPUT"),
         __attributes(ACF_ATTR_VEC_IN),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(40, 40)),
  __port(__index(1),
         __identifier("PATCH_SIZE"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(2),
         __identifier("RADIUS"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(3),
         __identifier("OUTPUT"),
         __attributes(ACF_ATTR_VEC_OUT),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16u),
         __e0_size(1, 1),
         __ek_size(1, 1)));

KERNEL_INFO kernelInfoConcat(ORB_RBRIEF_K)(
  ORB_RBRIEF_KN,
  7,
  __port(__index(0),
         __identifier("INPUT"),
         __attributes(ACF_ATTR_VEC_IN),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(40, 40)),
  __port(__index(1),
         __identifier("ORIENTATION"),
         __attributes(ACF_ATTR_VEC_IN),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16u),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(2),
         __identifier("BIT_PATTERN"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08s),
         __e0_size(1, 1),
         __ek_size(2048, 1)),
  __port(__index(3),
         __identifier("DESCRIPTOR_SIZE_B"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(4),
         __identifier("PATCH_SIZE"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(5),
         __identifier("OUTPUT"),
         __attributes(ACF_ATTR_VEC_OUT_FIFO_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(64, 1)),
  __port(__index(6),
         __identifier("COUNT"),
         __attributes(ACF_ATTR_SCL_OUT_STATIC),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d32u),
         __e0_size(1, 1),
         __ek_size(1, 1)));

#endif /* ACF_KERNEL_METADATA */

#ifdef ACF_KERNEL_IMPLEMENTATION

void orb_orientation(kernel_io_desc aIn,
                     kernel_io_desc aPatch,
                     kernel_io_desc aRadius,
                     kernel_io_desc aOut)
{
  // Input
  vec08u *pvIn0     = (vec08u *)aIn.pMem;
  int8_t *patchSize = (int8_t *)aPatch.pMem;
  int8_t *rad       = (int8_t *)aRadius.pMem;

  // Output
  vec16u *pvOut0 = (vec16u *)aOut.pMem;
  apu_orb_orientation(pvOut0,
                      pvIn0,
                      aIn.chunkSpan,
                      *patchSize,
                      *rad);
}

void orb_rbrief(kernel_io_desc aIn,
                kernel_io_desc aOrientation,
                kernel_io_desc aBitPattern,
                kernel_io_desc aDescrSizeB,
                kernel_io_desc aPatch,
                kernel_io_desc aOut,
                kernel_io_desc aCount)
{
  // Input
  vec08u *pvIn0         = (vec08u *)aIn.pMem;
  vec16u *pvOrientation = (vec16u *)aOrientation.pMem;

  int inChunkSpan                = aIn.chunkSpan;
  int8_t *pBitPattern            = (int8_t *)aBitPattern.pMem;
  int8_t *pDescriptorSizeInBytes = (int8_t *)aDescrSizeB.pMem;
  int8_t *pPatchSize             = (int8_t *)aPatch.pMem;

  // Output
  vec08u *pvOut = (vec08u *)aOut.pMem;

  apu_orb_rbrief(pvIn0,
                 inChunkSpan,
                 pvOrientation,
                 pBitPattern,
                 pvOut,
                 *pDescriptorSizeInBytes,
                 *pPatchSize);
  ACF_PUSH_VEC_BUFFER_TO_FIFO(aOut, (vec16u)(*pDescriptorSizeInBytes), aCount);
}

#endif /* ACF_KERNEL_IMPLEMENTATION */
