/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
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
/*!*********************************************************************************
*  \file apu_arithmetic.h
*  \brief APU basic arithmetic kernels.
***********************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#ifndef APUARITHMETICIMPL_H
#define APUARITHMETICIMPL_H

/**************************************************************************** /
/*!
* \file arithmetic_apu.h
* \brief arithmetic functions implementation for APEX
* \addtogroup apexcv arithmetic implementation for APEX
* @{
****************************************************************************/
#include <stdint.h>



#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

/*! 
  Computes Square root of its input 
  \param a - [Input] Source value
  \return     [Output] Square root of input value
*/
vec32u vsqrt_32(vec32u a);

/*!
   Calculates the absolute value from one sources
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src   - [Input]  Source value block pointer
   \param sstr  - [Input]  Source value block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void 
apu_abs(    vec08u* dst, int dstr,
      const vec08s* src, int sstr,
                 int bw, int bh);            

/*!
   Calculates the absolute difference between two sources
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src1  - [Input]  Source 1 values block pointer
   \param sstr1 - [Input]  Source 1 values block stride
   \param src2  - [Input]  Source 2 values block pointer
   \param sstr2 - [Input]  Source 2 values block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void 
apu_absdiff(    vec08u* dst, int dstr,
         const vec08u* src1, int sstr1,
         const vec08u* src2, int sstr2,
                     int bw, int bh);
           
/*!
   Calculates sum of two images
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src1  - [Input]  Source 1 block pointer
   \param sstr1 - [Input]  Source 1 block stride
   \param src2  - [Input]  Source 2 block pointer
   \param sstr2 - [Input]  Source 2 block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
*/
void 
apu_accumulate(vec16s* dst,  int dstr,
               const vec08u* src1,  int sstr1,
               const vec08u* src2,  int sstr2,
                int bw, int bh);

/*!
   Calculates squared sum of two images
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src1  - [Input]  Source 1 block pointer
   \param sstr1 - [Input]  Source 1 block stride
   \param src2  - [Input]  Source 2 block pointer
   \param sstr2 - [Input]  Source 2 block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
   \param scale - [Input]  Image 2 Scale
*/
void 
apu_accumulate_squared(vec16s* dst,  int dstr,
               const vec08u* src1,  int sstr1,
               const vec08u* src2,  int sstr2,
                int bw, int bh, uint8_t scale);

/*!
   Calculates weighted sum of two images
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src1  - [Input]  Source 1 block pointer
   \param sstr1 - [Input]  Source 1 block stride
   \param src2  - [Input]  Source 2 block pointer
   \param sstr2 - [Input]  Source 2 block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
   \param alpha - [Input]  Weight between [0 and 256]
  
   ASSUMPTION: both sources are of same size. Weight is determined by alpha which ranges from [0-256].
               ie. ~70% weight is 179. Accuracy is lost if user of API wants exactly 70% but result is 
               accurate w.r.t alpha.
*/
void 
apu_accumulate_weighted(vec08u* dst,  int dstr,
                  const vec08u* src1, int sstr1,
                  const vec08u* src2, int sstr2,
                  int bw, int bh, uint8_t alpha);
                  
/*!
   Adds source 1 to source 2 (unsigned 8 bit)
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src1  - [Input]  Source 1 values block pointer
   \param sstr1 - [Input]  Source 1 values block stride
   \param src2  - [Input]  Source 2 values block pointer
   \param sstr2 - [Input]  Source 2 values block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void 
apu_add(        vec08u* dst, int dstr,
         const vec08u* src1, int sstr1,
         const vec08u* src2, int sstr2,
                     int bw, int bh);

/*!
   Adds source 1 to source 2 (signed 16 bit)
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src1  - [Input]  Source 1 values block pointer
   \param sstr1 - [Input]  Source 1 values block stride
   \param src2  - [Input]  Source 2 values block pointer
   \param sstr2 - [Input]  Source 2 values block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void 
apu_add(        vec16s* dst, int dstr,
         const vec16s* src1, int sstr1,
         const vec16s* src2, int sstr2,
                     int bw, int bh);

/*!
   Calculates the bitwise AND from two sources (unsigned 8 bit)
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src1  - [Input]  Source 1 values block pointer
   \param sstr1 - [Input]  Source 1 values block stride
   \param src2  - [Input]  Source 2 values block pointer
   \param sstr2 - [Input]  Source 2 values block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void 
apu_and(        vec08u* dst, int dstr,
         const vec08u* src1, int sstr1,
         const vec08u* src2, int sstr2,
                     int bw, int bh);                    
                     
/*!
   Calculates the bitwise AND from two sources (unsigned 16 bit)
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src1  - [Input]  Source 1 values block pointer
   \param sstr1 - [Input]  Source 1 values block stride
   \param src2  - [Input]  Source 2 values block pointer
   \param sstr2 - [Input]  Source 2 values block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void 
apu_and(        vec16u* dst, int dstr,
         const vec16u* src1, int sstr1,
         const vec16u* src2, int sstr2,
                     int bw, int bh);                   

/*!
   Calculates the bitwise AND from two sources (unsigned 32 bit)
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src1  - [Input]  Source 1 values block pointer
   \param sstr1 - [Input]  Source 1 values block stride
   \param src2  - [Input]  Source 2 values block pointer
   \param sstr2 - [Input]  Source 2 values block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void 
apu_and(        vec32u* dst, int dstr,
         const vec32u* src1, int sstr1,
         const vec32u* src2, int sstr2,
                     int bw, int bh);
                     
/*!
   Counts leading zeros (signed 16 bit)

   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src   - [Input]  Source value block pointer
   \param sstr  - [Input]  Source value block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void
apu_clz(       vec08u* dst, int dstr,
               const vec16s* src, int sstr,
               int bw, int bh);

/*!
   Counts leading zeros (unsigned 16 bit)

   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src   - [Input]  Source value block pointer
   \param sstr  - [Input]  Source value block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void
apu_clz(       vec08u* dst, int dstr,
               const vec16u* src, int sstr,
               int bw, int bh);
/*!
   Counts leading zeros (signed 8 bit)

   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src   - [Input]  Source value block pointer
   \param sstr  - [Input]  Source value block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
*/

void
apu_clz(       vec08u* dst, int dstr,
               const vec08s* src, int sstr,
               int bw, int bh);
/*!
   Counts leading zeros (unsigned 8 bit)

   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src   - [Input]  Source value block pointer
   \param sstr  - [Input]  Source value block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void
apu_clz(       vec08u* dst, int dstr,
               const vec08u* src, int sstr,
               int bw, int bh);


/*!
   Calculates magnitude of image matrix
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param srcx  - [Input]  Source x values block pointer
   \param sstrx - [Input]  Source x values block stride
   \param srcy  - [Input]  Source y values block pointer
   \param sstry - [Input]  Source y values block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void 
apu_magnitude( vec16u* dst, int dstr,
        const vec16s* srcx, int sstrx,
        const vec16s* srcy, int sstry,
                    int bw, int bh);
                    
/*!
   Calculates the bitwise NOT of the source.
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src   - [Input]  Source values block pointer
   \param sstr  - [Input]  Source values block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void 
apu_not(       vec08u* dst, int dstr,
         const vec08u* src, int sstr,
                    int bw, int bh);
                    
/*!
   Calculates the bitwise OR from two sources (unsigned 8 bit)
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src1  - [Input]  Source 1 values block pointer
   \param sstr1 - [Input]  Source 1 values block stride
   \param src2  - [Input]  Source 2 values block pointer
   \param sstr2 - [Input]  Source 2 values block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void 
apu_or(        vec08u* dst, int dstr,
         const vec08u* src1, int sstr1,
         const vec08u* src2, int sstr2,
                     int bw, int bh);                 
                     
/*!
   Calculates the bitwise OR from two sources (unsigned 16 bit)
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src1  - [Input]  Source 1 values block pointer
   \param sstr1 - [Input]  Source 1 values block stride
   \param src2  - [Input]  Source 2 values block pointer
   \param sstr2 - [Input]  Source 2 values block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void 
apu_or(        vec16u* dst, int dstr,
         const vec16u* src1, int sstr1,
         const vec16u* src2, int sstr2,
                     int bw, int bh);              

/*!
   Calculates the bitwise OR from two sources (unsigned 32 bit)
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src1  - [Input]  Source 1 values block pointer
   \param sstr1 - [Input]  Source 1 values block stride
   \param src2  - [Input]  Source 2 values block pointer
   \param sstr2 - [Input]  Source 2 values block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void 
apu_or(        vec32u* dst, int dstr,
         const vec32u* src1, int sstr1,
         const vec32u* src2, int sstr2,
                     int bw, int bh);

/*!
   Subtracts source 2 from source 1 (unsigned 8 bit)
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src1  - [Input]  Source 1 values block pointer
   \param sstr1 - [Input]  Source 1 values block stride
   \param src2  - [Input]  Source 2 values block pointer
   \param sstr2 - [Input]  Source 2 values block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void 
apu_sub(       vec16s* dst,  int dstr,
         const vec08u* src1, int sstr1,
         const vec08u* src2, int sstr2,
                     int bw, int bh);       

/*!
   Subtracts source 2 from source 1 (signed 16 bit)
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src1  - [Input]  Source 1 values block pointer
   \param sstr1 - [Input]  Source 1 values block stride
   \param src2  - [Input]  Source 2 values block pointer
   \param sstr2 - [Input]  Source 2 values block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void 
apu_sub(       vec16s* dst,  int dstr,
         const vec16s* src1, int sstr1,
         const vec16s* src2, int sstr2,
                     int bw, int bh);

/*!
   Calculates an unsigned 8 bit output by substituting each of the unsigned 8 bit input values using a provided loop-up table.
 
   \param dst    - [Output] Destination block pointer
   \param dstr   - [Input]  Destination block stride
   \param src    - [Input]  Source values block pointer
   \param sstr   - [Input]  Source values block stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
   \param lpvLUT - [Input]  Vectorized LUT to be used for stubstitution
 */
void apu_lut_8b( vec08u* dst, int16u  dstr,
           const vec08u* src, int16u  sstr,
                 int16u  bw , int16u  bh,
                 vec08u* lpvLUT);
                     
 

/*!
Calculates an unsigned 8 bit boolean output image based on a threshold with configurable true and false values

\param dst       - [Output] Destination block pointer
\param dstr      - [Input]  Destination block stride
\param src       - [Input]  Source values block pointer
\param sstr      - [Input]  Source values block stride
\param bw        - [Input]  Block width
\param bh        - [Input]  Block height
\param threshold - [Input]  Threshold value
\param val_true  - [Input]  Value for TRUE pixels
\param val_false - [Input]  Value for FALSE fixels

https://www.khronos.org/registry/OpenVX/specs/1.0/html/d3/d1e/group__group__vision__function__threshold.html
255 if src(x,y)>threshold
0   otherwise

Kernel has no dependencies, so data is available as one continuous block of data
*/

void apu_binary_threshold(vec08u* dst, int16u  dstr,
  const vec08u* src, int16u  sstr,
  int16u bw, int16u bh,
  int08u val_true, int08u val_false, uint8_t threshold);

void apu_binary_threshold(vec08u* dst, int16u  dstr,
  const vec16u* src, int16u  sstr,
  int16u bw, int16u bh,
  int08u val_true, int08u val_false, uint16_t threshold);

void apu_binary_threshold(vec08u* dst, int16u  dstr,
  const vec32u* src, int16u  sstr,
  int16u bw, int16u bh,
  int08u val_true, int08u val_false, uint32_t threshold);

/*!
Calculates an unsigned 8 bit boolean output image based on a threshold with configurable true and false values

\param dst       - [Output] Destination block pointer
\param dstr      - [Input]  Destination block stride
\param src       - [Input]  Source values block pointer
\param sstr      - [Input]  Source values block stride
\param bw        - [Input]  Block width
\param bh        - [Input]  Block height
\param val_true  - [Input]  Value for TRUE pixels
\param val_false - [Input]  Value for FALSE fixels
\param threshold_low - [Input]  Low Threshold value
\param threshold2 - [Input]  Hight Threshold value

https://www.khronos.org/registry/OpenVX/specs/1.0/html/d3/d1e/group__group__vision__function__threshold.html
0   if src(x,y)>upper
0   if src(x,y)<lower
255 otherwise
Kernel has no dependencies, so data is available as one continuous block of data
*/
void apu_range_threshold(vec08u* dst, int16u  dstr,
  const vec08u* src, int16u  sstr,
  int16u bw, int16u bh,
  int08u val_true, int08u val_false, uint8_t threshold_low, uint8_t threshold2);

void apu_range_threshold(vec08u* dst, int16u  dstr,
  const vec16u* src, int16u  sstr,
  int16u bw, int16u bh,
  int08u val_true, int08u val_false, uint16_t threshold_low, uint16_t threshold2);

void apu_range_threshold(vec08u* dst, int16u  dstr,
  const vec32u* src, int16u  sstr,
  int16u bw, int16u bh,
  int08u val_true, int08u val_false, uint32_t threshold_low, uint32_t threshold2);



            
/*!
   Calculates the bitwise XOR from two sources (unsigned 8 bit)
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src1  - [Input]  Source 1 values block pointer
   \param sstr1 - [Input]  Source 1 values block stride
   \param src2  - [Input]  Source 2 values block pointer
   \param sstr2 - [Input]  Source 2 values block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void 
apu_xor(        vec08u* dst, int dstr,
         const vec08u* src1, int sstr1,
         const vec08u* src2, int sstr2,
                     int bw, int bh);              
                     
/*!
   Calculates the bitwise XOR from two sources (unsigned 16 bit)
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src1  - [Input]  Source 1 values block pointer
   \param sstr1 - [Input]  Source 1 values block stride
   \param src2  - [Input]  Source 2 values block pointer
   \param sstr2 - [Input]  Source 2 values block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void 
apu_xor(        vec16u* dst, int dstr,
         const vec16u* src1, int sstr1,
         const vec16u* src2, int sstr2,
                     int bw, int bh); 

/*!
   Calculates the bitwise XOR from two sources (unsigned 32 bit)
 
   \param dst   - [Output] Destination block pointer
   \param dstr  - [Input]  Destination block stride
   \param src1  - [Input]  Source 1 values block pointer
   \param sstr1 - [Input]  Source 1 values block stride
   \param src2  - [Input]  Source 2 values block pointer
   \param sstr2 - [Input]  Source 2 values block stride
   \param bw    - [Input]  Block width
   \param bh    - [Input]  Block height
 */
void 
apu_xor(        vec32u* dst, int dstr,
         const vec32u* src1, int sstr1,
         const vec32u* src2, int sstr2,
                     int bw, int bh);
                     
/*!
   Calculate minimum SAD & location given a template and a window
 
   \param lpvIn0           - [Input]  Source template block pointer
   \param lStrideIn0       - [Input]  Template block stride
   \param lChunkWidthIn0   - [Input]  Template block width
   \param lChunkHeightIn0  - [Input]  Template block height
   \param lpvIn1           - [Input]  Source window block pointer
   \param lStrideIn1       - [Input]  Window block stride
   \param lChunkWidthIn1   - [Input]  Window block width
   \param lChunkHeightIn1  - [Input]  Window block height
   \param lpvOut0          - [Output] Destination block pointer
   \param lStrideOut0      - [Input]  Destination block stride
   \param lChunkWidthOut0  - [Input]  Template block width
   \param lChunkHeightOut0 - [Input]  Template block height
 */
void
apu_sad (vec08u* lpvIn0, int16_t lStrideIn0, int16_t lChunkWidthIn0, int16_t lChunkHeightIn0,
         vec08u* lpvIn1, int16_t lStrideIn1, int16_t lChunkWidthIn1, int16_t lChunkHeightIn1,
         vec32u* lpvOut0, int16_t lStrideOut0, int16_t lChunkWidthOut0, int16_t lChunkHeightOut0);
                    
#endif /* APUARITHMETICIMPL_H */
/*! @} */
#endif /* ACF_KERNEL_IMPLEMENTATION */
