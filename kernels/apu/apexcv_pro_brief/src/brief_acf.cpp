/**************************************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright 2014-2017 NXP
* All Rights Reserved
*
*****************************************************************************
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
**************************************************************************************************/

/*!*********************************************************************************
*  @file
*  @brief ACF metadata and wrapper function for the \ref secBrief "BRIEF".
***********************************************************************************/
#ifdef ACF_KERNEL_METADATA


/*!*********************************************************************************
*  \brief ACF metadata for the computation of BRIEF descriptors
*  \see UG-10267-03 ACF User Guide, Section 3.2.2
***********************************************************************************/
KERNEL_INFO kernel_info_compute_brief_descriptor(
  "compute_brief_descriptor",
  11,
  __port(__index(0),
         __identifier("INPUT"),
         __attributes(ACF_ATTR_VEC_IN),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(36, 36)),

  __port(__index(1),
         __identifier("FILTER_TYPE"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(1, 1)),

  __port(__index(2),
         __identifier("SMPL_PACKET"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d32u),
         __e0_size(1, 1),
         __ek_size(2048, 1)),

  __port(__index(3),
         __identifier("NR_PACKETS_UL"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d32u),
         __e0_size(1, 1),
         __ek_size(1, 1)),

  __port(__index(4),
         __identifier("NR_PACKETS_UR"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d32u),
         __e0_size(1, 1),
         __ek_size(1, 1)),

  __port(__index(5),
         __identifier("NR_PACKETS_LL"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d32u),
         __e0_size(1, 1),
         __ek_size(1, 1)),

  __port(__index(6),
         __identifier("NR_PACKETS_LR"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d32u),
         __e0_size(1, 1),
         __ek_size(1, 1)),

  __port(__index(7),
         __identifier("PATCH_SIZE"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(1, 1)),

  __port(__index(8),
         __identifier("DESC_SIZE"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(1, 1)),

  __port(__index(9),
         __identifier("COUNT"),
         __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d32u),
         __e0_size(1, 1),
         __ek_size(1, 1)),

  __port(__index(10),
         __identifier("DESCRIPTOR_OUT"),
         __attributes(ACF_ATTR_VEC_OUT_FIFO_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(64, 1)));

#endif /* ACF_KERNEL_METADATA */

#ifdef ACF_KERNEL_IMPLEMENTATION
#include <stdint.h>
#include "brief_apu.h"
#include "integral_image_apu.h"

volatile unsigned char gState = UPPER_LEFT;

/*!*********************************************************************************
*  \brief ACF wrapper function for the extraction of descriptor with size 16.
*  \see UG-10267-03 ACF User Guide, Section 3.2.3
***********************************************************************************/
void compute_brief_descriptor(kernel_io_desc aImg,
                              kernel_io_desc aFilterType,
                              kernel_io_desc aSamplingPackets,
                              kernel_io_desc aNrOfPacketsUpLeft,
                              kernel_io_desc aNrOfPacketsUpRight,
                              kernel_io_desc aNrOfPacketsLwLeft,
                              kernel_io_desc aNrOfPacketsLwRight,
                              kernel_io_desc aPatchSize,
                              kernel_io_desc aDescSize,
                              kernel_io_desc aCount,
                              kernel_io_desc aOut)
{
  int status              = ERROR;
  vec08u *pChunkStart     = (vec08u *)(aImg.pMem);
  unsigned int *pSmplPtrn = (unsigned int *)(aSamplingPackets.pMem);

  char filterType        = *((char *)(aFilterType.pMem));
  int nrOfPacketsUpLeft  = *((int *)(aNrOfPacketsUpLeft.pMem));
  int nrOfPacketsUpRight = *((int *)(aNrOfPacketsUpRight.pMem));
  int nrOfPacketsLwLeft  = *((int *)(aNrOfPacketsLwLeft.pMem));
  int nrOfPacketsLwRight = *((int *)(aNrOfPacketsLwRight.pMem));
  int descSize           = (int)(*(unsigned char *)aDescSize.pMem);
  int halfPatch          = (int)(*(unsigned char *)aPatchSize.pMem);
  int fltPad             = 4;
  int span               = aImg.chunkSpan;

  vec08u *pOut       = (vec08u *)(aOut.pMem);
  vec08u *pHalfPatch = &pChunkStart[fltPad + fltPad * span];

  status = apu_brief_compute_descriptor(pHalfPatch,
                                        span,
                                        filterType,
                                        (unsigned char *)&gState,
                                        pSmplPtrn,
                                        nrOfPacketsUpLeft,
                                        nrOfPacketsUpRight,
                                        nrOfPacketsLwLeft,
                                        nrOfPacketsLwRight,
                                        pOut,
                                        descSize);

  if(status == GO)
  {
    gState = UPPER_LEFT;
    ACF_PUSH_VEC_BUFFER_TO_FIFO(aOut, (vec16u)descSize, aCount);
  }
  else
  {
    ACF_PUSH_VEC_BUFFER_TO_FIFO(aOut, (vec16u)0, aCount);
  }

  if(status == ERROR)
  {
    *((unsigned int *)aCount.pMem) = 0xFFFFFFFF;
  }

  return;
}
#endif /* ACF_KERNEL_IMPLEMENTATION */
