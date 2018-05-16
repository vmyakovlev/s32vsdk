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
*  @file
*  @brief Contains the prototypes for the APU kernels found in cg_kernel.a
***********************************************************************************/

#ifndef CGKERNEL_H
#define CGKERNEL_H

#ifdef ACF_KERNEL_IMPLEMENTATION

/*!***********************************************************************************
*  \brief Performs remap of RGBA pixel data..
*  \returns None.
*************************************************************************************/   
void
remap_bilinear_rgb
               (vec32u __cmem* dst,
                vec32u __cmem* src,
                vec16u __cmem* offset, 
                vec08u __cmem* delta, 
                int sstride, // stride of the src array in cmem
                int dstride, // stride of the src array in cmem
                int bw,
                int bh);

/*!***********************************************************************************
*  \brief Performs remap of 8-bit grayscale pixel data.
*  \returns None.
*************************************************************************************/                               
void
remap_bilinear_grayscale
               (vec08u __cmem* dst,
                vec08u __cmem* src,
                vec16u __cmem* offset, 
                vec08u __cmem* delta, 
                int sstride, // stride of the src array in cmem
                int dstride, // stride of the src array in cmem
                int bw,
                int bh);

void remap_block_size(vec32u *map, int32_t chunk_width, int32_t chunk_height, int32_t tile_width, vec16u *t_width, vec16u *t_height, vec16u *f_width, vec16u *f_height, vec16u *e_width, vec16u *e_height, vec16u *s_width, vec16u *s_height);

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
                            vec16u* cellHistograms       /*!< The sub-block histogram buffer. */,
                            int chunkWidth               /*!< The number of gradient image columns. */,
                            int chunkHeight              /*!< The number of gradient image rows. */,
                            int chunkSpan                /*!< The span of the gradient image. */,
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
                            
/*!***********************************************************************************
*  \brief Define the x-offset for each CU.
*  \returns Signed x-offset for each CU.
*************************************************************************************/   
vec16s apu_hough_get_offset_x(int imageCenterX     /*!< Center horizontal pixel. */,
                              int blockCols,       /*!< Number of block columns. */
                              int cuCount          /*!< Number of computational units. */);

/*!***********************************************************************************
*  \brief Define the (scalar) y-offset.
*  \returns Signed y-offset.
*************************************************************************************/                               
int apu_hough_get_offset_y(int imageCenterY  /*!< Center vertical pixel. */);
      
/*!***********************************************************************************
*  \brief Define the index for each CU.
*  \returns Index for each CU (base 0).
*************************************************************************************/        
vec16s apu_hough_get_cu_index(int indexStart,   /*!< Starting index. */
                              int cuCount       /*!< Number of computational units. */);

/*!***********************************************************************************
*  \brief Store all the pixel coordinates above a threshold.
*  \returns Pixel count in CMEM.
*************************************************************************************/                       
vec16s apu_hough_pixel_threshold(vec16s* vPixelsX        /*!< Pixel x-coordinates. */,
                                 vec16s* vPixelsY        /*!< Pixel y-coordinates. */,
                                 vec16s vOffsetX         /*!< Horizontal offset for each CU. */,
                                 int16s sOffsetY         /*!< Vertical offset (scalar). */,
                                 const vec08u* image     /*!< Image pointer. */,
                                 int stride              /*!< Block stride. */,
                                 int cols                /*!< Block columns. */,
                                 int rows                /*!< Block rows. */,
                                 int threshold           /*!< Pixel threshold value. */);

/*!***********************************************************************************
*  \brief Scalarize a set of pixel coordinates.
*  \returns Pixel count in DMEM.
*************************************************************************************/                                      
int apu_hough_scalarize_pixels(int16s* sPixelsX          /*!< Pixel x-coordinates in DMEM. */,
                               int16s* sPixelsY          /*!< Pixel y-coordinates in DMEM. */,
                               const vec16s* vPixelsX    /*!< Pixel x-coordinates in CMEM. */,
                               const vec16s* vPixelsY    /*!< Pixel y-coordinates in CMEM. */,
                               vec16s vPixelCount        /*!< Pixel count in CMEM. */,
                               int cuCount               /*!< Number of computational units. */);

/*!***********************************************************************************
*  \brief Accumulate the Hough Transform (rho x theta space)
*************************************************************************************/
void apu_hough_accumulate(vec16u* accumulator      /*!< Accumulator data (rho x theta space). */,
                          const int16s* pixelsX    /*!< Pixel x-coordinates in DMEM */,
                          const int16s* pixelsY    /*!< Pixel y-coordinates in DMEM */,
                          int pixelCount           /*!< Pixel count in DMEM. */,
                          vec16s tabCos            /*!< Cosine lookup table. */,
                          vec16s tabSin            /*!< Sin lookup table.*/,
                          int rhoOffset            /*!< Offset for the rho coordinate in the Accumulator.  The offset ensures the rho coodinate is greater or equal to zero. */);

/*!*********************************************************************************
*  \brief Store the lines for which the accumulator is above a threshold and using
*  non-maxima suppression.
*  \returns Line count in CMEM.
***********************************************************************************/
vec16s apu_hough_get_lines(vec32u* lines              /*!< Line destination buffer.  Each line is described by 32 bits.
                                                           The first 8 bits are the theta ID.  
                                                           The next 12 bits are the rho ID.
                                                           The last 12 bits are the accumulator value. */,
                           const vec16u* accumulator  /*!< Accumulator data (rho x theta space). */,
                           vec16s thetaIndex          /*!< Angle index for each CU. */,
                           int rhoCount               /*!< Number of rho coordinates. */,
                           int thetaCount             /*!< Number of angles to detect.  This must not exceed cuCount-2 if non-maxima suppression between angles is enabled,
                                                           otherwise it must not exceed cuCount.*/,
                           int threshold              /*!< Accumulator threshold. */,
                           int maxLinesPerCU          /*!< Maximum lines that can be returned for each CU. */,
                           int nonMaxSupFlag          /*!< Non-maxima suppression flag. */,
                           int cuCount                /*!< Number of computational units. */);  

/*!*********************************************************************************
*  \brief Apply a 3x3 sobel x filter to an image.
*
*  3x3 sobel x filter is applied.
*  \snippet harris_corner_impl.cpp apply 3x3 sobel x
*
*  The result pixel value is then converted to signed char.
***********************************************************************************/
void sobel_x_3x3_impl(const vec08u* lpvIn		/*!< Pointer to the source image.
							The source image is assumed to be padded according to the filter size.
							However, \c lpvIn points the top left corner of the \em unpadded image region.*/,
                      int16u lStrideIn		/*!< Stride of the padded source image.*/,
                      vec08s* lpvOut		/*!< Pointer to the destination image.*/,
                      int16u lStrideOut		/*!< Stride of the destination image.*/,
                      int16u lChunkWidth	/*!< Chunk width.*/,
                      int16u lChunkHeight	/*!< Chunk height.*/
                     );

/*!*********************************************************************************
*  \brief Apply a 3x3 sobel y filter to an image.
*
*  3x3 sobel y filter is applied.
*  \snippet harris_corner_impl.cpp apply 3x3 sobel y
*
*  The result pixel value is then converted to signed char.
***********************************************************************************/
void sobel_y_3x3_impl(const vec08u* lpvIn		/*!< Pointer to the source image.
							The source image is assumed to be padded according to the filter size.
							However, \c lpvIn points the top left corner of the \em unpadded image region.*/,
                      int16u lStrideIn		/*!< Stride of the padded source image.*/,
                      vec08s* lpvOut		/*!< Pointer to the destination image.*/,
                      int16u lStrideOut		/*!< Stride of the destination image.*/,
                      int16u lChunkWidth	/*!< Chunk width.*/,
                      int16u lChunkHeight	/*!< Chunk height.*/
                     );

/*!*********************************************************************************
*  \brief Apply a 5x5 sobel x filter to an image.
*
*  5x5 sobel x filter is applied.
*  \snippet harris_corner_impl.cpp apply 5x5 sobel x
*
*  The result pixel value is then converted to signed char.
***********************************************************************************/
void sobel_x_5x5_impl(const vec08u* lpvIn		/*!< Pointer to the source image.
							The source image is assumed to be padded according to the filter size.
							However, \c lpvIn points the top left corner of the \em unpadded image region.*/,
                      int16u lStrideIn		/*!< Stride of the padded source image.*/,
                      vec08s* lpvOut		/*!< Pointer to the destination image.*/,
                      int16u lStrideOut		/*!< Stride of the destination image.*/,
                      int16u lChunkWidth	/*!< Chunk width.*/,
                      int16u lChunkHeight	/*!< Chunk height.*/
                     );

/*!*********************************************************************************
*  \brief Apply a 5x5 sobel y filter to an image.
*
*  5x5 sobel y filter is applied.
*  \snippet harris_corner_impl.cpp apply 5x5 sobel y
*
*  The result pixel value is then converted to signed char.
***********************************************************************************/
void sobel_y_5x5_impl(const vec08u* lpvIn		/*!< Pointer to the source image.
							The source image is assumed to be padded according to the filter size.
							However, \c lpvIn points the top left corner of the \em unpadded image region.*/,
                      int16u lStrideIn		/*!< Stride of the padded source image.*/,
                      vec08s* lpvOut		/*!< Pointer to the destination image.*/,
                      int16u lStrideOut		/*!< Stride of the destination image.*/,
                      int16u lChunkWidth	/*!< Chunk width.*/,
                      int16u lChunkHeight	/*!< Chunk height.*/
                     );

/*!*********************************************************************************
*  \brief Multiply pixel values.
*
*  Pixels are multiplied, and shifted to right by 7bits.
*  (7bit*7bit)>>7 => 7bit.
*  The output value is signed short.
*  \snippet harris_corner_impl.cpp multiply
***********************************************************************************/
void multiply_impl(vec08s* lpvIn0		/*!< Pointer to the source image0.*/,
                   int16u lStrideIn0	        /*!< Stride of the padded source image0.*/,
                   vec08s* lpvIn1		/*!< Pointer to the source image1.*/,
                   int16u lStrideIn1	        /*!< Stride of the padded source image1.*/,
                   vec16s* lpvOut	        /*!< Pointer to the destination image.*/,
                   int16u lStrideOut	        /*!< Stride of the destination image.*/,
                   int16u lChunkWidth	        /*!< Chunk width.*/,
                   int16u lChunkHeight	        /*!< Chunk height.*/
                  );


/*!*********************************************************************************
*  \brief Apply a 3x3 box filter to an image.
*
*  3x3 box filter is applied. Division is omitted.
*  \snippet harris_corner_impl.cpp apply 3x3 box
*
***********************************************************************************/
void box_3x3_impl(vec16s* lpvIn			/*!< Pointer to the source image.*/,
              int16u lStrideIn		        /*!< Stride of the source image.*/,
              vec16s* lpvOut			/*!< Pointer to the destination image.*/,
              int16u lStrideOut		        /*!< Stride of the destination image.*/,
              int16u lChunkWidth		/*!< Chunk width.*/,
              int16u lChunkHeight		/*!< Chunk height.*/
             );
             
             
/*!*********************************************************************************
*  \brief Apply a 5x5 box filter to an image.
*
*  5x5 box filter is applied. Division is omitted.
*  \snippet harris_corner_impl.cpp apply 3x3 box
*
***********************************************************************************/
void box_5x5_impl(vec16s* lpvIn			/*!< Pointer to the source image.*/,
              int16u lStrideIn		        /*!< Stride of the source image.*/,
              vec16s* lpvOut			/*!< Pointer to the destination image.*/,
              int16u lStrideOut		        /*!< Stride of the destination image.*/,
              int16u lChunkWidth		/*!< Chunk width.*/,
              int16u lChunkHeight		/*!< Chunk height.*/
             );


/*!*********************************************************************************
*  \brief Calculate corner score with 3x3 box filter-applied value.
*
*  Corner score is calculated. Score = det(A) - k*(trace(A))^2.
*  \snippet harris_corner_impl.cpp calculate score with box 3x3
***********************************************************************************/
void calculate_score_box_3x3_impl(vec16s* lpvInFxx		/*!< Pointer to the Fxx source image.*/,
                          int16u lStrideInFxx				/*!< Stride of the Fxx source image.*/,
                          vec16s* lpvInFyy				/*!< Pointer to the Fyy source image.*/,
                          int16u lStrideInFyy				/*!< Stride of the Fyy source image.*/,
                          vec16s* lpvInFxy				/*!< Pointer to the Fxy source image.*/,
                          int16u lStrideInFxy				/*!< Stride of the Fxy source image.*/,
                          int08u lpvInCornerCoef			/*!< Corner coefficient k used in score calculation.*/,
                          vec16s* lpvOut				/*!< Pointer to the destination image.*/,
                          int16u lStrideOut				/*!< Stride of the destination image.*/,
                          int16u lChunkWidth				/*!< Chunk width.*/,
                          int16u lChunkHeight				/*!< Chunk height.*/
                         );


/*!*********************************************************************************
*  \brief Calculate corner score with 5x5 box filter-applied value.
*
*  Corner score is calculated. Score = det(A) - k*(trace(A))^2.
*  \snippet harris_corner_impl.cpp calculate score with box 5x5
***********************************************************************************/
void calculate_score_box_5x5_impl(vec16s* lpvInFxx		/*!< Pointer to the Fxx source image.*/,
                          int16u lStrideInFxx				/*!< Stride of the Fxx source image.*/,
                          vec16s* lpvInFyy				/*!< Pointer to the Fyy source image.*/,
                          int16u lStrideInFyy				/*!< Stride of the Fyy source image.*/,
                          vec16s* lpvInFxy				/*!< Pointer to the Fxy source image.*/,
                          int16u lStrideInFxy				/*!< Stride of the Fxy source image.*/,
                          int08u lpvInCornerCoef			/*!< Corner coefficient k used in score calculation.*/,
                          vec16s* lpvOut				/*!< Pointer to the destination image.*/,
                          int16u lStrideOut				/*!< Stride of the destination image.*/,
                          int16u lChunkWidth				/*!< Chunk width.*/,
                          int16u lChunkHeight				/*!< Chunk height.*/
                         );

#endif	
#endif /* CGKERNEL_H */

