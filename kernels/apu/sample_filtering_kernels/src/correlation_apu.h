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
/*!
* \file correlation_apu.h
* \brief Correlation with general filter 1D/2D
* \addtogroup correlation
* @{
****************************************************************************/
#ifndef APUCORRELATIONKERNELIMPL_H
#define APUCORRELATIONKERNELIMPL_H

#ifdef APEX2_EMULATE
#include "apu_config.hpp"
#include "apu_vec.hpp"
using namespace APEX2;
#endif

#define inputFiltUpScale		3
#define scharrFiltUpscale		1

// Define pointer to correlation functions
typedef void(*corrKernelPtr)(vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s filtScaling, const int16s* filterCoefs);

/*!
	Initializes the array of filter function pointers, to  correspond to the right filters. 
	Should be called once, before filtering.
	\brief Initializes the array of filter function pointers.
*/
void initFilters();

/*!
Based on the symmetry flag, this function chooses automatically the correct filter kernel and calculates the output of correlation with a general filter.
\brief Computes time optimized the correlation with a general filter.

\param filterFlags  - [Input] flags indicating the symmetry of the filter coefficients. For possible values see the definitions in this file
\param dst			- [Output] Destination block pointer
\param src			- [Input]  Source block pointer
\param sstr			- [Input]  Source block stride
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param destBw		- [Input]  Block width (including stride) of the output
\param xSkip, ySkip - [Input] number of pixels to be skipped in x/y direction before convolving again, i.e. if destination has to be directly downsampled (=1 for no downsampling)
\param filtWidth	- [Input] the x-size of the filter
\param filtHeight	- [Input] the y-size of the filter
\param filtScaling	- [Input] the factor to divide the output of the filter with
\param filterCoefs	- [Input] the coefficients of the filter
*/
void performCorrelation(int16u filterFlags, vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s scaleFact, const int16s* filterCoefs);

/*!
   Calculate the output of correlation with a general filter.
   
   \param dst			- [Output] Destination block pointer
   \param src			- [Input]  Source block pointer
   \param sstr			- [Input]  Source block stride
   \param bw			- [Input]  Block width
   \param bh			- [Input]  Block height
   \param destBw		- [Input]  Block width (including stride) of the output
   \param xSkip, ySkip  - [Input] number of pixels to be skipped in x/y direction before convolving again, i.e. if destination has to be directly downsampled (=1 for no downsampling)
   \param filtWidth		- [Input] the x-size of the filter 
   \param filtHeight	- [Input] the y-size of the filter 
   \param filtScaling	- [Input] the factor to divide the output of the filter with
   \param filterCoefs	- [Input] the coefficients of the filter
   */
void correlation_filter(vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s filtScaling, const int16s* filterCoefs);

/*!
Calculate the correlation with a filter, which is anti-symmetric in X direction.
\brief Computes the correlation with an anti-symmetric X filter

\param dst			- [Output] Destination block pointer
\param src			- [Input]  Source block pointer
\param sstr			- [Input]  Source block stride
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param destBw		- [Input]  Block width (including stride) of the output
\param xSkip, ySkip - [Input] number of pixels to be skipped in x/y direction before convolving again, i.e. if destination has to be directly downsampled (=1 for no downsampling)
\param filtWidth	- [Input] the x-size of the filter
\param filtHeight	- [Input] the y-size of the filter
\param filtScaling	- [Input] the factor to divide the output of the filter with
\param filterCoefs	- [Input] the coefficients of the filter
*/
void correlation__antisymXfilter(vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s filtScaling, const int16s* filterCoefs);

/*!
Calculate the correlation with a filter, which is symmetric in X direction.
\brief Computes the correlation with a symmetric X filter

\param dst			- [Output] Destination block pointer
\param src			- [Input]  Source block pointer
\param sstr			- [Input]  Source block stride
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param destBw		- [Input]  Block width (including stride) of the output
\param xSkip, ySkip - [Input] number of pixels to be skipped in x/y direction before convolving again, i.e. if destination has to be directly downsampled (=1 for no downsampling)
\param filtWidth	- [Input] the x-size of the filter
\param filtHeight	- [Input] the y-size of the filter
\param filtScaling	- [Input] the factor to divide the output of the filter with
\param filterCoefs	- [Input] the coefficients of the filter
*/
void correlation__symXfilter(vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s filtScaling, const int16s* filterCoefs);

/*!
Calculate the correlation with a filter, which is symmetric in Y direction.
\brief Computes the correlation with a symmetric Y filter

\param dst			- [Output] Destination block pointer
\param src			- [Input]  Source block pointer
\param sstr			- [Input]  Source block stride
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param destBw		- [Input]  Block width (including stride) of the output
\param xSkip, ySkip - [Input] number of pixels to be skipped in x/y direction before convolving again, i.e. if destination has to be directly downsampled (=1 for no downsampling)
\param filtWidth	- [Input] the x-size of the filter
\param filtHeight	- [Input] the y-size of the filter
\param filtScaling	- [Input] the factor to divide the output of the filter with
\param filterCoefs	- [Input] the coefficients of the filter
*/
void correlation__symYfilter(vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s filtScaling, const int16s* filterCoefs);

/*!
Calculate the correlation with a filter, which is anti-symmetric in Y direction.
\brief Computes the correlation with an anti-symmetric Y filter

\param dst			- [Output] Destination block pointer
\param src			- [Input]  Source block pointer
\param sstr			- [Input]  Source block stride
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param destBw		- [Input]  Block width (including stride) of the output
\param xSkip, ySkip - [Input] number of pixels to be skipped in x/y direction before convolving again, i.e. if destination has to be directly downsampled (=1 for no downsampling)
\param filtWidth	- [Input] the x-size of the filter
\param filtHeight	- [Input] the y-size of the filter
\param filtScaling	- [Input] the factor to divide the output of the filter with
\param filterCoefs	- [Input] the coefficients of the filter
*/
void correlation__antisymYfilter(vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s filtScaling, const int16s* filterCoefs);

/*!
Calculate the correlation with a filter, which is symmetric in X and in Y directions.
\brief Computes the correlation with a symmetric in X and Y filter

\param dst			- [Output] Destination block pointer
\param src			- [Input]  Source block pointer
\param sstr			- [Input]  Source block stride
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param destBw		- [Input]  Block width (including stride) of the output
\param xSkip, ySkip - [Input] number of pixels to be skipped in x/y direction before convolving again, i.e. if destination has to be directly downsampled (=1 for no downsampling)
\param filtWidth	- [Input] the x-size of the filter
\param filtHeight	- [Input] the y-size of the filter
\param filtScaling	- [Input] the factor to divide the output of the filter with
\param filterCoefs	- [Input] the coefficients of the filter
*/
void correlation__symXYfilter(vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s filtScaling, const int16s* filterCoefs);

/*!
Calculate the correlation with a filter, which is symmetric in X and anti-symmetric in Y directions.
\brief Computes the correlation with a symmetric in X and anti-symmetric in Y filter

\param dst			- [Output] Destination block pointer
\param src			- [Input]  Source block pointer
\param sstr			- [Input]  Source block stride
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param xSkip, ySkip - [Input] number of pixels to be skipped in x/y direction before convolving again, i.e. if destination has to be directly downsampled (=1 for no downsampling)
\param filtWidth	- [Input] the x-size of the filter
\param filtHeight	- [Input] the y-size of the filter
\param filtScaling	- [Input] the factor to divide the output of the filter with
\param filterCoefs	- [Input] the coefficients of the filter
*/
void correlation__symXantisymYfilter(vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s filtScaling, const int16s* filterCoefs);


/*!
Calculate the correlation of a signed 16bit image with a filter, which is symmetric in X and anti-symmetric in Y directions.
\brief Computes the 16bit correlation with a symmetric in X and anti-symmetric in Y filter

\param dst			- [Output] Destination block pointer
\param src			- [Input]  Source block pointer
\param sstr			- [Input]  Source block stride
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param xSkip, ySkip - [Input] number of pixels to be skipped in x/y direction before convolving again, i.e. if destination has to be directly downsampled (=1 for no downsampling)
\param filtWidth	- [Input] the x-size of the filter
\param filtHeight	- [Input] the y-size of the filter
\param filtScaling	- [Input] the factor to divide the output of the filter with
\param filterCoefs	- [Input] the coefficients of the filter
*/
void correlation__symXantisymYfilter_16s(vec16s* dst, vec16s* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s scaleFact, const int16s* filterCoefs);



/*!
Calculate the correlation with a filter, which is anti-symmetric in X and symmetric in Y directions.
\brief Computes the correlation with an anti-symmetric in X and symmetric in Y filter

\param dst			- [Output] Destination block pointer
\param src			- [Input]  Source block pointer
\param sstr			- [Input]  Source block stride
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param xSkip, ySkip - [Input] number of pixels to be skipped in x/y direction before convolving again, i.e. if destination has to be directly downsampled (=1 for no downsampling)
\param filtWidth	- [Input] the x-size of the filter
\param filtHeight	- [Input] the y-size of the filter
\param filtScaling	- [Input] the factor to divide the output of the filter with
\param filterCoefs	- [Input] the coefficients of the filter
*/
void correlation__antisymXsymYfilter(vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s filtScaling, const int16s* filterCoefs);

/*!
Calculate the correlation of a signed 16bit image with a filter, which is anti-symmetric in X and symmetric in Y directions.
\brief Computes the 16bit correlation with an anti-symmetric in X and symmetric in Y filter

\param dst			- [Output] Destination block pointer
\param src			- [Input]  Source block pointer
\param sstr			- [Input]  Source block stride
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param xSkip, ySkip - [Input] number of pixels to be skipped in x/y direction before convolving again, i.e. if destination has to be directly downsampled (=1 for no downsampling)
\param filtWidth	- [Input] the x-size of the filter
\param filtHeight	- [Input] the y-size of the filter
\param filtScaling	- [Input] the factor to divide the output of the filter with
\param filterCoefs	- [Input] the coefficients of the filter
*/
void correlation__antisymXsymYfilter_16s(vec16s* dst, vec16s* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s filtScaling, const int16s* filterCoefs);


/*!
Calculate the output of correlation with a filter, which is anti-symmetric in X and anti-symmetric in Y directions.
\brief Computes the correlation with an anti-symmetric in X and anti-symmetric in Y filter

\param dst			- [Output] Destination block pointer
\param src			- [Input]  Source block pointer
\param sstr			- [Input]  Source block stride
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param xSkip, ySkip - [Input] number of pixels to be skipped in x/y direction before convolving again, i.e. if destination has to be directly downsampled (=1 for no downsampling)
\param filtWidth	- [Input] the x-size of the filter
\param filtHeight	- [Input] the y-size of the filter
\param filtScaling	- [Input] the factor to divide the output of the filter with
\param filterCoefs	- [Input] the coefficients of the filter
*/
void correlation__antisymXYfilter(vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s filtScaling, const int16s* filterCoefs);


#endif /* APUCORRELATIONKERNELIMPL_H */

/*! @} */
