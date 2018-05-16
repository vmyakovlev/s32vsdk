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

/*****************************************************************************
 *  \file hough_apu.h
 *  \brief Declaration of kernel functions for Hough Transform
 ****************************************************************************/

#ifndef APUHOUGH_H
#define APUHOUGH_H

/*!
   \brief Generates the x-offset for each CU
   \param[in] aImageCenterY Center vertical pixel
   \param[in] aBlockCols    Number of block columns
   \param[in] aCuCount      Number of computational units
   \return Signed x-offfset
 */
vec16s apu_hough_get_offset_x(int aImageCenterX,
                              int aBlockCols,
                              int aCuCount);

/*!
   \brief Generates the (scalar) y-offset
   \param[in] aImageCenterY  Center vertical pixel
   \return Signed y-offfset
 */
int apu_hough_get_offset_y(int aImageCenterY);

/*!
   \brief Define the index for each CU
   \param[in] aIndexStart  First CU id
   \param[in] aCuCount  Number of CUs
   \return CU-ids one after the other
 */
vec16s apu_hough_get_cu_index(int aIndexStart,
                              int aCuCount);

/*!
   \brief Store all the pixel coordinates above a threshold
   \param[in] apPixelsX  Pixel x-coordinates
   \param[in] apPixelsY  Pixel y-coordinates
   \param[in] aOffsetX   Horizontal offset for each CU
   \param[in] aOffsetY   Vertical offset (scalar)
   \param[in] apImage    Image pointer
   \param[in] aStride    Block stride
   \param[in] aCols      Block columns
   \param[in] aRows      Block rows
   \param[in] aThreshold Pixel threshold value
   \return Pixel count in CMEM
 */
vec16s apu_hough_pixel_threshold(vec16s *apPixelsX,
                                 vec16s *apPixelsY,
                                 vec16s aOffsetX,
                                 int16s aOffsetY,
                                 const vec08u *apImage,
                                 int aStride,
                                 int aCols,
                                 int aRows,
                                 int aThreshold);

/*!
   \brief Serializes a set of pixel coordinates
   \param[in] apSclPixelsX Pixel x-coordinates in DMEM
   \param[in] apSclPixelsY Pixel y-coordinates in DMEM
   \param[in] apPixelsX    Pixel x-coordinates in CMEM
   \param[in] apPixelsY    Pixel y-coordinates in CMEM
   \param[in] aPixelCount  Pixel count in CMEM
   \param[in] aCuCount     Number of computational units
 */
int apu_hough_scalarize_pixels(int16s *apSclPixelsX,
                               int16s *apSclPixelsY,
                               const vec16s *apPixelsX,
                               const vec16s *apPixelsY,
                               vec16s aPixelCount,
                               int aCuCount);

/*!
   \brief  Accumulate the Hough Transform (rho x theta space)
   \param[in] apAccumulator Accumulator data - rhos x theta space
   \param[in] apPixelsX     Pixel x-coordinates in DMEM
   \param[in] apPixelsY     Pixel y-coordinates in DMEM
   \param[in] aPixelCount   Pixel count in DMEM
   \param[in] aTabCos       Cosine lookup table
   \param[in] aTabSin       Sin lookup table
   \param[in] rhoOffset     Offset for the rho coordinate in the acc.
                            The offset ensures the rho coodinate is greater or
                            equal to zero
 */
void apu_hough_accumulate(vec16u *apAccumulator,
                          const int16s *apPixelsX,
                          const int16s *apPixelsY,
                          int aPixelCount,
                          vec16s aTabCos,
                          vec16s aTabSin,
                          int aRhoOffset);

/*!
   \brief Store the lines for which the accumulator > threshold and using NMS
   \param[in] apLines        Line destination buffer. Special format !
                             The first 8 bits are the theta ID.
                             The next 12 bits are the rho ID.
                             The last 12 bits are the accumulator value
   \param[in] apAccm         Accumulator data - rhos x theta space
   \param[in] aThetaIndex    Angle index for each CU
   \param[in] aRhoCount      Number of rho coordinates
   \param[in] aThetaCount    Number of angles to detect
                             This must not exceed aCuCount-2 if non-maxima
                             suppression between angles is EN, otherwise it must
                             not exceed aCuCount
   \param[in] aThreshold     Accumulator aThreshold
   \param[in] aMaxLinesPerCU Maximum lines that can be returned
                             for each CU
   \param[in] aNonMaxSupFlag Non-maxima suppression flag
   \param[in] aCuCount       Number of computational units.
   \return Line count in CMEM.
 */
vec16s apu_hough_get_lines(vec32u *apLines,
                           const vec16u *apAccm,
                           vec16s aThetaIndex,
                           int aRhoCount,
                           int aThetaCount,
                           int aThreshold,
                           int aMaxLinesPerCU,
                           int aNonMaxSupFlag,
                           int aCuCount);

#endif  /* APUHOUGH_H */
