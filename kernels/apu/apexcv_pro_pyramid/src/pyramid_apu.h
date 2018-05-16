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
 
/*!*****************************************************************************
*  \file pyramid_apu.h
*  \brief APU pyramid creation algorithm implementation 
*******************************************************************************/
#ifdef ACF_KERNEL_IMPLEMENTATION
#ifndef APUPYRAMID_H
#define APUPYRAMID_H

/*!
   Apply a 5x1 horizontal Gaussian filter to an image.
   (unsigned 8-bit -> signed 16-bit)

   \param lpvIn         - [Input]  Pointer to the source image. The source image is assumed to be padded according to the filter size.
   \param lStrideIn     - [Input]  Stride of the padded source image.
   \param lpvOut        - [Output] Pointer to the destination image. 
   \param lStrideOut    - [Input]  Stride of the destination image.
   \param lChunkWidth   - [Input]  Chunk width.
   \param lChunkHeight  - [Input]  Chunk height.
 */
void apu_pyr_horizontal_gaus(
   const vec08u* lpvIn,
   int16u lStrideIn,
   vec16s* lpvOut,
   int16u lStrideOut,
   int16u lChunkWidth,
   int16u lChunkHeight
);

/*!
   Apply a 5x1 horizontal Gaussian filter to an image and expand.
   (unsigned 8-bit -> signed 16-bit)

   \param lpvIn         - [Input]  Pointer to the source image. The source image is assumed to be padded according to the filter size.
   \param lStrideIn     - [Input]  Stride of the padded source image.
   \param lpvOut        - [Output] Pointer to the destination image. 
   \param lStrideOut    - [Input]  Stride of the destination image.
   \param lChunkWidth   - [Input]  Chunk width.
   \param lChunkHeight  - [Input]  Chunk height.
 */
void apu_pyr_horizontal_gaus_and_expand(
   const vec08u* lpvIn,
   int16u lStrideIn,
   vec16s* lpvOut,
   int16u lStrideOut,
   int16u lChunkWidth,
   int16u lChunkHeight
);

/*!
   Apply a 1x5 vertical Gaussian filter to an image and reduce.
   (signed 16-bit -> unsigned 8-bit)

   \param lpvIn         - [Input]  Pointer to the source image. The source image is assumed to be padded according to the filter size.
   \param lStrideIn     - [Input]  Stride of the padded source image.
   \param lpvOut        - [Output] Pointer to the destination image. 
   \param lStrideOut    - [Input]  Stride of the destination image.
   \param lChunkWidth   - [Input]  Chunk width.
   \param lChunkHeight  - [Input]  Chunk height.
 */
void apu_pyr_vertical_gaus_and_reduce(
   const vec16s* lpvIn,
   int16u lStrideIn,
   vec08u* lpvOut,
   int16u lStrideOut,
   int16u lChunkWidth,
   int16u lChunkHeight
);

/*!
   Apply a 1x5 vertical Gaussian filter to an image.
   (signed 16-bit -> unsigned 8-bit)

   \param lpvIn         - [Input]  Pointer to the source image. The source image is assumed to be padded according to the filter size.
   \param lStrideIn     - [Input]  Stride of the padded source image.
   \param lpvOut        - [Output] Pointer to the destination image. 
   \param lStrideOut    - [Input]  Stride of the destination image.
   \param lChunkWidth   - [Input]  Chunk width.
   \param lChunkHeight  - [Input]  Chunk height.
 */
void apu_pyr_vertical_gaus(
   const vec16s* lpvIn,
   int16u lStrideIn,
   vec08u* lpvOut,
   int16u lStrideOut,
   int16u lChunkWidth,
   int16u lChunkHeight
);

#endif /* APUPYRAMID_H */
#endif // ACF_KERNEL_IMPLEMENTATION
