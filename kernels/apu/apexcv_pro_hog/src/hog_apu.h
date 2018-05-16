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
*  \file hog_apu.h
*  \brief HOG APU kernel implementation.
***********************************************************************************/

#ifndef HOGAPU_H
#define HOGAPU_H

#ifdef ACF_KERNEL_IMPLEMENTATION

void HOG_convertFunction(int08s* dst, double* src, int size);

/*!*********************************************************************************
*  \brief Create the lookup tables for block histogram management and score calculation.
*         See documentation for more information.
***********************************************************************************/
void HOG_createLUTs(int08u* xOffsetLUT          /*!< The x-axis LUT for the detection window */,
                    int08u* rotateLUT           /*!< The rotate LUT for score transfer across CU boundaries */,
                    int08u* yOffsetLUT          /*!< The y-axis LUT for the detection window */,
                    int stridesPerChunkX        /*!< Number of detection window strides per CU on the X axis */,
                    int stridesPerBlockX        /*!< Number of detection window strides per block width */,
                    int stridesPerBlockY        /*!< Number of detection window strides per block height */,
                    int numBlocksX              /*!< Number of blocks that make up the width of a detection window */,
                    int numBlocksY              /*!< Number of blocks that make up the height of a detection window */);

/*!*********************************************************************************
*  \brief Compute the gradient angle and respective histogram bin as well as magnitude
***********************************************************************************/
void HOG_getGradient(vec08u* gradMag            /*!< The output gradient magnitude */,
                     int mStride                /*!< The stride of the magnitude buffer */,
                     vec08u* gradBin            /*!< The output gradient bin */,
                     int bStride                /*!< The stride of the bin buffer */,
                     const vec08u* image        /*!< The input image */,
                     int iStride                /*!< The stride of the input buffer */,
                     int rows                   /*!< The height of the input buffer */,
                     int cols                   /*!< The width of the input buffer */);

/*!*********************************************************************************
*  \brief Compute the cell histogram of oriented gradients.
***********************************************************************************/
void HOG_getCellHistograms(vec16u* cellHistograms       /*!< The sub-block histogram buffer. */,
                           int cStride                  /*!< The sub-block histogram stride. */,
                           const vec08u* gradMag        /*!< The gradient magnitude image. */,
                           int mStride                  /*!< The gradient magnitude stride. */,
                           const vec08u* gradBin        /*!< The gradient angle bin image. */,
                           int bStride                  /*!< The gradient angle bin stride. */,
                           int chunkWidth               /*!< The number of gradient image columns. */,
                           int chunkHeight              /*!< The number of gradient image rows. */,
                           int windowStrideX            /*!< The horizontal detection window stride. */,
                           int windowStrideY            /*!< The horizontal detection window stride. */);

/*!*********************************************************************************
*  \brief Compute the normalized block histogram of cell histograms.
***********************************************************************************/
void HOG_getBlockHistograms(vec08u* blockHistograms      /*!< The block histograms. */,
                            int blockStride              /*!< The block histogram stride. */,
                            vec16u* cellHistograms       /*!< The sub-block histogram buffer. */,
                            int cellStride               /*!< The sub-block histogram stride. */,
                            int chunkWidth               /*!< The number of gradient image columns. */,
                            int chunkHeight              /*!< The number of gradient image rows. */,
                            int stridesPerBlockX         /*!< The number of horizontal detection window strides per block. */,
                            int stridesPerBlockY         /*!< The number of vertical detection window strides per block. */);

/*!*********************************************************************************
*  \brief Compute the scores of each detection window
***********************************************************************************/
void HOG_getScores(vec16s* scores                  /*!< The SVM scores. */,
                   const vec08u* histograms        /*!< The block histograms. */,
                   int chunkWidth                  /*!< The block histogram width. */,
                   int chunkHeight                 /*!< The block histogram height. */,
                   int chunkSpan                   /*!< The block histogram stride. */,
                   const int08s* svmFunction       /*!< The SVM decision function. */,
                   const int08u* xOffsetLUT        /*!< The x offset lookup table used to compute the SVM score. */,
                   const int08u* yOffsetLUT        /*!< The x offset lookup table used to compute the SVM score. */,
                   const int08u* rotateLUT         /*!< The vector rotate lookup table used to compute the SVM score. */,
                   int histogramRow                /*!< The current block histogram row. */,
                   int stridesPerBlockY            /*!< The number of horizontal detection window strides per chunk. */,
                   int numBlocksX                  /*!< Number of blocks that make up the width of a detection window */,
                   int numBlocksY                  /*!< Number of blocks that make up the height of a detection window */,
                   int svmSize                     /*!< The size in bytes of the svm decision function */);

#endif
#endif /* HOGAPU_H */

