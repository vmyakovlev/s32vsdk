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
 
#ifdef ACF_KERNEL_IMPLEMENTATION
#ifndef APUTMO_H
#define APUTMO_H

/*!
   Calculate world luminance for RGBE input image.
   (unsigned 8-bit -> unsigned 8-bit)

   \param lpvIn         - [Input]   Pointer to the input image data.
   \param lStrideIn     - [Input]   Stride of the input image data.
   \param lpvOutIe      - [Output]  Pointer to the output intermediate exponent data.
   \param lpvOutIm      - [Output]  Pointer to the output intermediate mantissa data.
   \param lStrideInt    - [Input]   Stride of the output intermediate data.
   \param lpvOutWe      - [Output]  Pointer to the output world luminance exponent data.
   \param lpvOutWm      - [Output]  Pointer to the output world luminance mantissa data.
   \param lStrideWor    - [Input]   Stride of the output world luminance data.
   \param logTable      - [Input]   Pointer to the input log LUT data.
   \param lpvSumWe      - [Output]  Pointer to the output geometric sum exponent data.
   \param lpvSumWm      - [Output]  Pointer to the output geometric sum mantissa data.
   \param lpvSumCnt     - [Output]  Pointer to the output geometric count data.
   \param lChunkWidth   - [Input]   Processing chunk width.
   \parma lChunkHeight  - [Input]   Processing chunk height.
   */
void apu_tmo_calc_world_lum_rgbe(
   vec08u *lpvIn,
   int16u lStrideIn,
   vec08u *lpvOutIe,
   vec08u *lpvOutIm,
   int16u lStrideInt,
   vec08u *lpvOutWe,
   vec08u *lpvOutWm,
   int16u lStrideWor,
   vec16s *logTable,
   vec32s *lpvSumWe,
   vec32s *lpvSumWm,
   vec32u *lpvSumCnt,
   int16u lChunkWidth,
   int16u lChunkHeight,
   bool bFirstTile
   );

/*!
   Calculate world luminance for OpenEXR input image.
   (unsigned 16-bit -> unsigned 8-bit)

   \param lpvIn         - [Input]   Pointer to the input image data.
   \param lStrideIn     - [Input]   Stride of the input image data.
   \param lpvOutIe      - [Output]  Pointer to the output intermediate exponent data.
   \param lpvOutIm      - [Output]  Pointer to the output intermediate mantissa data.
   \param lStrideInt    - [Input]   Stride of the output intermediate data.
   \param lpvOutWe      - [Output]  Pointer to the output world luminance exponent data.
   \param lpvOutWm      - [Output]  Pointer to the output world luminance mantissa data.
   \param lStrideWor    - [Input]   Stride of the output world luminance data.
   \param logTable      - [Input]   Pointer to the input log LUT data.
   \param lpvSumWe      - [Output]  Pointer to the output geometric sum exponent data.
   \param lpvSumWm      - [Output]  Pointer to the output geometric sum mantissa data.
   \param lpvSumCnt     - [Output]  Pointer to the output geometric count data.
   \param lChunkWidth   - [Input]   Processing chunk width.
   \parma lChunkHeight  - [Input]   Processing chunk height.
   */
void apu_tmo_calc_world_lum_openexr(
   vec16u *lpvIn,
   int16u lStrideIn,
   vec08u *lpvOutIe,
   vec08u *lpvOutIm,
   int16u lStrideInt,
   vec08u *lpvOutWe,
   vec08u *lpvOutWm,
   int16u lStrideWor,
   vec16s *logTable,
   vec32s *lpvSumWe,
   vec32s *lpvSumWm,
   vec32u *lpvSumCnt,
   int16u lChunkWidth,
   int16u lChunkHeight,
   bool bFirstTile
   );

/*!
   Calculate geometric mean.
   (unsigned 32-bit -> unsigned 8-bit)

   \param lpvInSumWe    - [Input]   Pointer to the input geometric sum exponent data.
   \param lpvInSumWm    - [Input]   Pointer to the input geometric sum mantissa data.
   \param lpvInCnt      - [Input]   Pointer to the input geometric count data.
   \param expTable      - [Input]   Pointer to the input exponent LUT data.
   \param lpvOutGeoMe   - [Output]  Pointer to the output geometric mean exponent data.
   \param lpvOutGeoMm   - [Output]  Pointer to the output geometric mean mantissa data.
   */
void apu_tmo_calc_geo_mean(
   vec32s lpvInSumWe,
   vec32s lpvInSumWm,
   vec32u lpvInCnt,
   int16u start_cu,
   int16u end_cu,
   int08u *expTable,
   int08u *lpvOutGeoMe,
   int08u *lpvOutGeoMm
   );

/*!
   LDR transformation.
   (unsigned 8-bit -> unsigned 8-bit)

   \param lpvInGeoMe    - [Input]   Pointer to the input geometric mean exponent data.
   \param lpvInIe       - [Input]   Pointer to the input intermediate exponent data.
   \param lpvInIm       - [Input]   Pointer to the input intermediate mantissa data.
   \param lStrideIn     - [Input]   Stride of the input intermediate data.
   \param lpvInWe       - [Input]   Pointer to the input world luminance exponent data.
   \param lpvInWm       - [Input]   Pointer to the input world luminance mantissa data.
   \param lStrideInW    - [Input]   Stride of the input world luminance data.
   \param divValue      - [Input]   Pointer to the input division value.
   \param logTable      - [Input]   Pointer to the input log LUT data.
   \param divTable      - [Input]   Pointer to the input division LUT data.
   \param keyValue      - [Input]   Input key value data.
   \param lpvOut        - [Output]  Pointer to the output ldr image data.
   \param lStrideOut    - [Input]   Stride of the output ldr image data.
   \param lChunkWidth   - [Input]   Processing chunk width.
   \param lChunkHeight  - [Input]   Processing chunk height.
   */
void apu_tmo_ldr_transform(
   int08u lpvInGeoMe,
   vec08u *lpvInIe,
   vec08u *lpvInIm,
   int16u lStrideIn,
   vec08u *lpvInWe,
   vec08u *lpvInWm,
   int16u lStrideInW,
   int16u *divValue,
   vec16s *logTable,
   vec16u *divTable,
   int08u keyVal,
   vec08u *lpvOut,
   int16u lStrideOut,
   int16u lChunkWidth,
   int16u lChunkHeight,
   bool bFirstTile
   );

#endif /* APUTMO_H */
#endif // ACF_KERNEL_IMPLEMENTATION
