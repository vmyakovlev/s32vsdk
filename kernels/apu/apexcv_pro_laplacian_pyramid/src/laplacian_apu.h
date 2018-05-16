/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor
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

/*==================================================================================================
Revision History:
                              Modification     Tracking
Author (core ID)              Date D/M/Y       Number     Description of Changes
---------------------------   ----------    ------------  ------------------------------------------
Adrian Grigore (nxa11567)     09/12/2016    VSDK-554      Created
---------------------------   ----------    ------------  ------------------------------------------
==================================================================================================*/

/*!*********************************************************************************
*  \file laplacian_apu.h
*  \brief APU Laplacian Pyramid Header.
***********************************************************************************/

#ifndef APULAPLACIAN_H
#define APULAPLACIAN_H

/*!*********************************************************************************
*  @file
*  @brief \ref secImagePyramid "image pyramid creation" implementation for the APU.
*
*  \section secImagePyramid Image Pyramid Creation
*
*  There are two common kinds of image pyramids: Gaussian pyramids and Laplacian pyramids.
*  Here, we present the implementation of Laplacian pyramid creation.
*
*  To downsample an image (pyramid down), first the source image is convolved with a
*  5x5 Gaussian kernel, and then every even-numbered row and column is removed. As a
*  result, the area is reduced to exactly one-quarter the area of the source image.
*
*  Laplacian output for each pyramid level is obtained by subtracting the convoluted
*  image from the source image.
*  The convoluted image is also used for the final output, which is necessasry for
*  the input image reconstruction.
*
***********************************************************************************/

/*!*********************************************************************************
*  \brief Apply a 5x1 horizontal gaussian filter to an image.
*
*  5x1 horizontal gaussian filter is applied.
*  \snippet laplacian_apu.cpp horizontal gaus
*  \snippet laplacian_apu.cpp horizontal gaus helper
***********************************************************************************/
void apu_pyr_horizontal_gaus_laplacian(
         const vec08u* lpvIn        /*!< Pointer to the source image. The source image 
                                         is assumed to be padded according to the filter size.
                                         However, \c lpvIn points the top left corner of the 
                                         \em unpadded image region.*/,
               int16u lStrideIn     /*!< Stride of the padded source image.*/,
               vec16u* lpvOut       /*!< Pointer to the destination image.*/,
               int16u lStrideOut    /*!< Stride of the destination image.*/,
               int16u lChunkWidth   /*!< Chunk width.*/,
               int16u lChunkHeight  /*!< Chunk height.*/
);

/*!*********************************************************************************
*  \brief Apply a 1x5 vertical gaussian filter to an image and generate laplacian and reduced outputs.
*
*  1x5 horizontal gaussian filter is applied. Then the filered image is subtracted 
*  from de input image to generate laplacian output and it is reduced to 1/2 to
*  generate input for the next pyramid level.
*  \snippet laplacian_apu.cpp vertical gaus laplacian reduce
*  \snippet laplacian_apu.cpp vertical gaus laplacian reduce helper
***********************************************************************************/
void apu_pyr_vertical_gaus_laplacian_mid(
         const vec08u* lpvIn        /*!< Pointer to the source image.*/,
               int16u lStrideIn     /*!< Stride of the source image.*/,
         const vec16u* lpvIn_gauss   /*!< Pointer to the horizontally filtered source image.
                                         The image is assumed to be padded according to the
                                         filter size. However, \c lpvIn points the top left 
                                         corner of the \em unpadded image region.*/,
               int16u lStrideIn_gauss /*!< Stride of the padded filtered source image.*/,
               vec16s* lpvOut_lap   /*!< Pointer to the destination image - laplacian pyramid.*/,
               int16u lStrideOut_lap/*!< Stride of the destination image - laplacian pyramid.*/,
               vec08u* lpvOut_nex   /*!< Pointer to the destination image - input for next level.*/,
               int16u lStrideOut_nex/*!< Stride of the destination image - input for next level.*/,
               int16u lChunkWidth   /*!< Chunk width.*/,
               int16u lChunkHeight  /*!< Chunk height.*/
);

/*!*********************************************************************************
*  \brief Apply a 1x5 vertical gaussian filter to an image and generate laplacian and filtered outputs.
*
*  1x5 horizontal gaussian filter is applied. The filtered image is one of the
*  outputs. Then the filered image is subtracted from de input image to generate
*  the laplacian output.
*  \snippet laplacian_apu.cpp vertical gaus laplacian output
*  \snippet laplacian_apu.cpp vertical gaus laplacian output helper
***********************************************************************************/
void apu_pyr_vertical_gaus_laplacian_last(
         const vec08u* lpvIn        /*!< Pointer to the source image.*/,
               int16u lStrideIn     /*!< Stride of the source image.*/,
         const vec16u* lpvIn_gauss   /*!< Pointer to the horizontally filtered source image.
                                         The image is assumed to be padded according to the
                                         filter size. However, \c lpvIn points the top left 
                                         corner of the \em unpadded image region.*/,
               int16u lStrideIn_gauss /*!< Stride of the padded filtered source image.*/,
               vec16s* lpvOut_lap   /*!< Pointer to the destination image - laplacian pyramid.*/,
               int16u lStrideOut_lap/*!< Stride of the destination image - laplacian pyramid.*/,
               vec16s* lpvOut_out   /*!< Pointer to the destination image - last level output.*/,
               int16u lStrideOut_out/*!< Stride of the destination image - last level output.*/,
               int16u lChunkWidth   /*!< Chunk width.*/,
               int16u lChunkHeight  /*!< Chunk height.*/
);


#endif /* APULAPLACIAN_H */
