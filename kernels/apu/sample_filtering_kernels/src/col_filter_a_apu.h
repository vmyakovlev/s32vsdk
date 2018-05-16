/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2013-2016 Freescale Semiconductor
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
*  @brief A column filter implementation for the APU.
*
*  \section secColumnFilter The Column Filter
*
*  A column filter is a 1-dimension filter applied to an image where each pixel
*  becomes a weighted sum of itself and neighbouring pixels in the same row.  The
*  weighted sum is determined by a set of filter coefficients.  The filter
*  are pixel-centered (i.e. the middle coefficient lines up with the pixel under 
*  consideration), so the number of coefficients must be odd.  For example, consider
*  the column filter with \f$N_c = 5\f$ shown below
*
*  \image html  col_filter.svg "A 5-coefficient column filter centered on pixel p_ij"
*  \image latex col_filter.pdf "A 5-coefficient column filter centered on pixel p_ij" width = 8cm
*
*  The filter, shown in red, has five columns and a single row of coefficients.  If
*  we apply this filter to pixel \f$p_{ij}\f$, the first filter coefficient \f$c_0\f$
*  lines up with \f$p_{i,j-2}\f$, the second coefficient \f$c_1\f$ lines up with
*  \f$p_{i,j-1}\f$, and so on.  So the weighted sum is
*  \f[
*      p_{ij}' \equiv c_0 p_{i,j-2} + c_1 p_{i,j-1} + c_2 p_{i,j} + c_3 p_{i,j+1} + 
*                     c_4 p_{i,j+2} 
*  \f]
*
*  In order to filter pixels at the left and right edges of the image, we must pad
*  the image.  The number of columns padded on either side of the image is
*  \f[
*      N_{col \ padding} = \textrm{floor}(N_c/2)
*  \f]
*  and so the total number of columns in the padded image is \f$N_{cols} + 
*  2 N_{col \ padding}\f$, where \f$N_{cols}\f$ is the original number of image
*  columns.  Note that each row of the image is filtered independently of the others
*  and the no row padding is required.
*
*  The filter coefficients are expressed as 8-bit unsigned fixed point numbers with
*  \f$Q\f$ fractional bits, where \f$Q\f$ is chosen by the user.
*
*  \see \ref pagFixedPoint
***********************************************************************************/

/*!
* \file col_filter_a_apu.h
* \brief Filter an image column with a certain filter.
* \addtogroup col_filter
* @{
****************************************************************************/

#ifndef COLFILTERIMPL_H
#define COLFILTERIMPL_H

#ifdef APEX2_EMULATE
#include "apu_lib.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

/*!*********************************************************************************
*  \brief Apply a column filter to an image.
*
*  First we decrement the number of fractional bits in anticipation of rounding the
*  final result.
*  \snippet col_filter_apu.c rounding
*
*  Next, we offset the source pointer so that it points to the beginning of the
*  padded region of the source image.  The offset amount is the number of padded
*  columns (\a see \ref secColumnFilter).
*  \snippet col_filter_apu.c source offset
*
*  For each pixel, we accumulate the weighted sum in a 16-bit register to avoid 
*  overflow.  When casting the weighted sum to an integer we round the value.
*  This means we right bit shift the sum by 1 less the fractional bits, add 1, then
*  right shift one more.
*  \snippet col_filter_apu.c weighted sum
***********************************************************************************/
void col_filter(vec08u* dst       /*!< Pointer to the destination image.*/,
                int dstStride     /*!< Stride of the destination image.*/,
                const vec08u* src /*!< Pointer to the source image.
				                       The source image is assumed to be padded according to the filter size.
									   However, \c src points the top left corner of the \em unpadded image region.*/,
                int srcStride     /*!< Stride of the padded source image.*/,
                int rows          /*!< Rows of the source image, not including padding.*/,
                int cols          /*!< Columns of the source image, not including padding.*/,
                const unsigned char* filter /*!< Pointer to column filter coefficients.  
                                                The coefficients are unsigned 8-bit \ref pagFixedPoint "fixed point numbers".*/,
                int filterSize    /*!< Size of the column filter.  The size must be odd so that the filter is centered about the pixel.*/,
                int filterQ       /*!< Number of fractional bits for the filter coefficients.  */);
				

               
#endif  /* COLFILTERIMPL_H */

/*! @} */
