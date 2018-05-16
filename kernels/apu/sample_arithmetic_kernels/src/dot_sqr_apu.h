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
* \file dot_sqr_apu.h
* \brief Element-wise square of the input matrix.
* \addtogroup square
* @{
****************************************************************************/
#ifndef APUDOTSQRIMPL_H
#define APUDOTSQRIMPL_H


#ifdef APEX2_EMULATE
#include "apu_config.hpp"
#include "apu_vec.hpp"
using namespace APEX2;
#endif

/**
* Computes the 32bit integer square root of the input parameter
*
*@parameter va Source vector
*@return sqrt(va)
*/
vec32u vsqrt_32(vec32u a);


/*!
Computes the 32bit elementwise-square of the 16bit input matrix. dst[i] = srcImageA[i].^2;
\brief Elementwise signed 16bit square => unsigned 32bit 

\warning  Out of range values are not taken into account!
\param dst			- [Output] 32bit Destination block pointer
\param srcA			- [Input]  32bit Source block pointer of img A
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param inStrideWidth  - [Input]  Source block stride width (i.e. nr. of elements including padding)
\param outStrideWidth - [Input]  Destination block stride width (i.e. nr. of elements including padding)
*/
void dot_sqr_in16s_out32u_filter(vec32u* dst, vec16s* srcA, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);

/*!
Computes the 32bit elementwise-square of the signed 16bit input matrix.
\brief Elementwise signed 32bit square => unsigned 32bit

\warning  Out of range values are not taken into account!
\param dst			- [Output] 32bit Destination block pointer
\param srcA			- [Input]  32bit Source block pointer of img A
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param inStrideWidth  - [Input]  Source block stride width (i.e. nr. of elements including padding)
\param outStrideWidth - [Input]  Destination block stride width (i.e. nr. of elements including padding)
*/
void dot_sqr_in32s_out32u_filter(vec32u* dst, vec32s* srcA, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/*!
Computes the 64 bit elementwise-square of the signed 32bit input matrix.
\brief Elementwise signed 32bit square => unsigned 64bit

\param dst_high		- [Output] unsigned 32bit high word destination block pointer
\param dst_low		- [Output] unsigned 32bit low word destination block pointer
\param srcA			- [Input]  signed 32bit source block pointer of img A
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param inStrideWidth  - [Input]  Source block stride width (i.e. nr. of elements including padding)
\param outStrideWidth - [Input]  Destination block stride width (i.e. nr. of elements including padding)
*/
void dot_sqr_in32s_out64u_filter(vec32u* dst_high, vec32u* dst_low, vec32s* srcA, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);

/*!
Computes the 64 bit elementwise-square of the unsigned 32bit input matrix
\brief Elementwise unsigned 32bit square => unsigned 64bit

\param dst_high		- [Output] unsigned 32bit high-word destination block pointer
\param dst_low		- [Output] unsigned 32bit low-word destination block pointer
\param srcA			- [Input]  unsigned 32bit source block pointer of img A
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param inStrideWidth  - [Input]  Source block stride width (i.e. nr. of elements including padding)
\param outStrideWidth - [Input]  Destination block stride width (i.e. nr. of elements including padding)
*/
void dot_sqr_in32u_out64u_filter(vec32u* out_high, vec32u* out_low, vec32u* srcA, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);

/*!
Computes the 64 bit elementwise-square of the signed 64bit input matrix
\brief Elementwise signed 64bit square => unsigned 64bit

\warning  Out of range values are not taken into account!

\param dst_high		- [Output] unsigned 32bit high-word destination block pointer
\param dst_low		- [Output] unsigned 32bit low-word destination block pointer
\param srcA_high	- [Input]  Block pointer to sighed 32bit high word of source 
\param srcA_low		- [Input]  Block pointer to unsigned 32bit low word of source
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param inStrideWidth  - [Input]  Source block stride width (i.e. nr. of elements including padding)
\param outStrideWidth - [Input]  Destination block stride width (i.e. nr. of elements including padding)
*/
void dot_sqr_in64s_out64u_filter(vec32u* dst_high, vec32u* dst_low, vec32s* srcA_high, vec32u* srcA_low, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/*!
Computes the 64 bit elementwise-square of the unsigned 64bit input matrix
\brief Elementwise unsigned 64bit square => unsigned 64bit

\warning  Out of range values are not taken into account!

\param dst_high		- [Output] unsigned 32bit high-word destination block pointer
\param dst_low		- [Output] unsigned 32bit low-word destination block pointer
\param srcA_high	- [Input]  Block pointer to unsigned 32bit high word of source
\param srcA_low		- [Input]  Block pointer to unsigned 32bit low word of source
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param inStrideWidth  - [Input]  Source block stride width (i.e. nr. of elements including padding)
\param outStrideWidth - [Input]  Destination block stride width (i.e. nr. of elements including padding)
*/
void dot_sqr_in64u_out64u_filter(vec32u* dst_high, vec32u* dst_low, vec32u* srcA_high, vec32u* srcA_low, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


#endif /* APUDOTSQRIMPL_H */


/*! @} */
