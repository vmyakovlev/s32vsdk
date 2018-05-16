/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2015-2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
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
****************************************************************************/
 
#ifndef APULBP_H
#define APULBP_H

#include <stdint.h>

#ifdef ACF_KERNEL_IMPLEMENTATION

/*!
   Calculate LBP descriptor.
   (unsigned 8-bit -> unsigned 8-bit)
   
   \param lpvIn         - [Input]   Pointer to the input image data.
   \param lStrideIn     - [Input]   Stride of the input image data.
   \param lpvOut        - [Output]  Pointer to the output descriptor data.
   \param lChunkWidth   - [Input]   Processing chunk width.
   \parma lChunkHeight  - [Input]   Processing chunk height.
 */
void apu_lbp_extract_descriptor(
   vec08u *lpvIn,
   int16u lStrideIn,
   vec08u *lpvOut,
   int16u lChunkWidth,
   int16u lChunkHeight,
   int32_t numCU
);

/*!
   Compare two descriptors.
   (unsigned 8-bit -> signed 32-bit)
   
   \param lpvIn0        - [Input]   Pointer to the input image0 data.
   \param lpvIn1        - [Input]   Pointer to the input image1 data.
   \param lpvOut        - [Output]  Pointer to the histogram distance buffer.
   \param lCompareSize  - [Input]   Comparing array size.
 */
void apu_lbp_compare_histogram(
   vec08u *lpvIn0,
   vec08u *lpvIn1,
   vec32s *lpvOut,
   int16u lCompareSize,
   int32_t numCU
);

/*!
   Accumulate local value in each CU to obtain one scalar value.
   (signed 32-bit -> signed 32-bit)
   
   \param lpvIn         - [Input]   Pointer to the local value.
   \param lpvOut        - [Output]  Pointer to the accumulated value.
 */
void apu_lbp_accumulate_distance(
   vec32s lpvIn,
   int32s *lpvOut,
   int32_t numCU
);

#endif // ACF_KERNEL_IMPLEMENTATION
#endif /* APULBP_H */

