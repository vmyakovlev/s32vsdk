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
* \file comparison_apu.h
* \brief element-wise comparison implementation for APEX
* \addtogroup el_comparison
* @{
****************************************************************************/

#ifndef APUCOMPARISONIMPL_H
#define APUCOMPARISONIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

/*!
Comparison btw two 8bit matrices:  dst[i] = (srcImage0[i] < srcImage1[i])
\brief Elementwise unsigned 8bit "<" operation => bool

\param dst		 - [Output] boolean destination block pointer
\param srcImage0 - [Input]  unsigned 8bit source block pointer of img 0
\param srcImage1 - [Input]  unsigned 8bit source block pointer of img 1
\param bw		 - [Input]  Block width
\param bh		 - [Input]  Block height
\param inStrideW  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideW - [Input]  Destination block width (in elements not bytes) including padding
*/
void lower(vbool* dst, vec08u* srcImage0, vec08u* srcImage1
		, int bw, int bh, int inStrideW, int outStrideW);

/*!
Comparison of an 8bit image with a scalar value:  dst[i] = (srcImage0[i] < scalar)
\brief Elementwise unsigned 8bit "<" operation => bool

\param dst		 - [Output] boolean destination block pointer
\param srcImage  - [Input]  unsigned 8bit source block pointer of img 0
\param scalar    - [Input]  unsigned 8bit source block pointer of img 1
\param bw		 - [Input]  Block width
\param bh		 - [Input]  Block height
\param inStrideW  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideW - [Input]  Destination block width (in elements not bytes) including padding
*/
void lower_scalar(vec08u* dst, vec08u* srcImage, unsigned char scalar
	, int bw, int bh, int inStrideW, int outStrideW);

/*!
Comparison btw two 16bit matrices:  dst[i] = (srcImage0[i] < srcImage1[i])
\brief Elementwise signed 16bit "<" operation => bool

\param dst		 - [Output] boolean destination block pointer
\param srcImage0 - [Input]  signed 16bit source block pointer of img 0
\param srcImage1 - [Input]  signed 16bit source block pointer of img 1
\param bw		 - [Input]  Block width
\param bh		 - [Input]  Block height
\param inStrideW  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideW - [Input]  Destination block width (in elements not bytes) including padding
*/
void lower_in16s(vbool* dst, vec16s* srcImage0, vec16s* srcImage1
				, int bw, int bh, int inStrideW, int outStrideW);


/*!
 Comparison btw two 32bit matrices:  dst[i] = (srcImage0[i] < srcImage1[i]) 
 \brief Elementwise signed 32bit "<" operation => bool
\param dst		 - [Output] boolean destination block pointer
\param srcImage0 - [Input]  signed 32bit source block pointer of img 0
\param srcImage1 - [Input]  signed 32bit source block pointer of img 1
\param bw		 - [Input]  Block width
\param bh		 - [Input]  Block height
\param instrideW  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideW - [Input]  Destination block width (in elements not bytes) including padding
*/
void lower_in32s(vbool* dst, vec32s* srcImage0, vec32s* srcImage1
				, int bw, int bh, int inStrideW, int outStrideW);


/*!
Comparison btw the absolute values of two 32bit matrices:  dst[i] = (abs(srcImage0[i]) < abs(srcImage1[i]))
\brief Elementwise signed 32bit "<" operation between the absolute values of the operands => bool

\param dst		 - [Output] boolean destination block pointer
\param srcImage0 - [Input]  signed 32bit source block pointer of img 0
\param srcImage1 - [Input]  signed 32bit source block pointer of img 1
\param bw		 - [Input]  Block width
\param bh		 - [Input]  Block height
\param instrideW  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideW - [Input]  Destination block width (in elements not bytes) including padding
*/
void absLower_in32s(vbool* dst, vec32s* srcImage0, vec32s* srcImage1
					, int bw, int bh, int inStrideW, int outStrideW);

/*!
Comparison btw the absolute values of a signed 32bit matrix and a scalar unsigned 16bit value:  dst[i] = (abs(srcImage0[i]) < val)
\brief Elementwise signed 32bit "<" operation between an image and a fixed unsigned 16bit scalar => bool

\param dst		 - [Output] boolean destination block pointer
\param srcImage0 - [Input]  signed 32bit source block pointer of img 0
\param compVal	 - [Input]  unsigned 16bit value to compare to
\param bw		 - [Input]  Block width
\param bh		 - [Input]  Block height
\param instrideW  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideW - [Input]  Destination block width (in elements not bytes) including padding
*/
void absLower_in32s_scalar16u(vbool* dst, vec32s* srcImage0, int16u compVal
							, int bw, int bh, int inStrideW, int outStrideW);

/*!
Comparison btw two 32bit numbers: dst[i] = (srcImage0[i] <= srcImage1[i]) 
\brief Elementwise signed 32bit "<=" operation => bool

\param dst		 - [Output] boolean destination block pointer
\param srcImage0 - [Input]  signed 32bit source block pointer of first image
\param srcImage1 - [Input]  signed 32bit source block pointer of second image
\param bw		 - [Input]  Block width
\param bh		 - [Input]  Block height
\param instrideW  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideW - [Input]  Destination block width (in elements not bytes) including padding
*/
void lowerEqual_in32s(vbool* dst,  vec32s* srcImage0, vec32s* srcImage1
					, int bw, int bh, int inStrideW, int outStrideW);

/*!
Comparison btw two unsigned 64bit matrices: dst[i] = (srcImage0[i] < srcImage1[i])
\brief Elementwise unsigned 64bit "<" operation => bool

\param dst				- [Output] Boolean destination block pointer
\param srcImage0_high	- [Input]  unsigned 32bit high word of source block pointer of img 0
\param srcImage0_low	- [Input]  unsigned 32bit low word of source block pointer of img 0
\param srcImage1_high	- [Input]  unsigned 32bit high word of source block pointer of img 1
\param srcImage1_low	- [Input]  unsigned 32bit low word of source block pointer of img 1
\param bw				- [Input]  Block width
\param bh				- [Input]  Block height
\param inStrideW		- [Input]  Source block width (in elements not bytes) including padding
\param outStrideW		- [Input]  Destination block width (in elements not bytes) including padding
*/
void lower_in64u(vbool* dst
			, vec32u* srcImage0_high, vec32u* srcImage0_low, vec32u* srcImage1_high, vec32u* srcImage1_low
			, int bw, int bh, int inStrideW, int outStrideW);

/*!
Comparison btw two signed 64bit matrices: dst[i] = (srcImage0[i] < srcImage1[i])
\brief Elementwise signed 64bit "<" operation => bool

\param dst				- [Output] Boolean Destination block pointer
\param srcImage0_high	- [Input]  signed 32bit high word  source block pointer of img 0
\param srcImage0_low	- [Input]  unsigned 32bit low word source block pointer of img 0
\param srcImage1_high	- [Input]  signed 32bit high word source block pointer of img 1
\param srcImage1_low	- [Input]  unsigned 32bit low word source block pointer of img 1
\param bw				- [Input]  Block width
\param bh				- [Input]  Block height
\param inStrideW		- [Input]  Source block width (in elements not bytes) including padding
\param outStrideW		- [Input]  Destination block width (in elements not bytes) including padding
*/
void lower_in64s(vbool* dst
			, vec32s* srcImage0_high, vec32u* srcImage0_low, vec32s* srcImage1_high, vec32u* srcImage1_low
			, int bw, int bh, int inStrideW, int outStrideW);



/*!
Elementwise MASK and image : dst[i] = (srcImage[i] if (srcMask[i]!= 0) otherwise 0
\brief Elementwise unsigned 8bit mask operation => unsigned 8bit

\param dst		 - [Output] unsigned 8bit destination block pointer
\param srcImage  - [Input]  unsigned 8bit source block pointer of image
\param srcMask   - [Input]  unsigned 8bit source block pointer of mask
\param bw		 - [Input]  Block width
\param bh		 - [Input]  Block height
\param instrideW  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideW - [Input]  Destination block width (in elements not bytes) including padding
*/
void mask_kn(vec08u* dst, vec08u* srcImage, vec08u* srcMask, int bw, int bh, int inStrideW, int outStrideW);


/*!
Elementwise AND btw two unsigned 8bit numbers: dst[i] = (srcImage0[i] && srcImage1[i])
\brief Elementwise unsigned 8bit "&&" operation => unsigned 8bit

\param dst		 - [Output] unsigned 8bit destination block pointer
\param srcImage0 - [Input]  unsigned 8bit source block pointer of first image
\param srcImage1 - [Input]  unsigned 8bit source block pointer of second image
\param bw		 - [Input]  Block width
\param bh		 - [Input]  Block height
\param instrideW  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideW - [Input]  Destination block width (in elements not bytes) including padding
*/
void and_kn(vec08u* dst, vec08u* srcImage0, vec08u* srcImage1, int bw, int bh, int inStrideW, int outStrideW);


/*!
Elementwise AND btw two unsigned 16bit numbers: dst[i] = (srcImage0[i] && srcImage1[i]) 
\brief Elementwise unsigned 16bit "&&" operation => unsigned 16bit

\param dst		 - [Output] unsigned 16bit destination block pointer
\param srcImage0 - [Input]  unsigned 16bit source block pointer of first image
\param srcImage1 - [Input]  unsigned 16bit source block pointer of second image
\param bw		 - [Input]  Block width
\param bh		 - [Input]  Block height
\param instrideW  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideW - [Input]  Destination block width (in elements not bytes) including padding
*/
void and_in16u_out16u(vec16u* dst, vec16u* srcImage0, vec16u* srcImage1, int bw, int bh, int inStrideW, int outStrideW);

/*!
Elementwise AND btw two unsigned 16bit numbers: dst[i] = (srcImage0[i] && srcImage1[i]). Result is converted to unsigned 16bit
\brief Elementwise unsigned 8bit "&&" operation => unsigned 16bit

\param dst		 - [Output] unsigned 16bit destination block pointer
\param srcImage0 - [Input]  signed 8bit source block pointer of first image
\param srcImage1 - [Input]  signed 8bit source block pointer of second image
\param bw		 - [Input]  Block width
\param bh		 - [Input]  Block height
\param instrideW  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideW - [Input]  Destination block width (in elements not bytes) including padding
*/
void and_in08u_out16u(vec16u* dst, vec08u* srcImage0, vec08u* srcImage1, int bw, int bh, int inStrideW, int outStrideW);

/*!
Elementwise AND btw one unsigned 8bit and one unsigned 16bit blocks: dst[i] = (srcImage0[i] && srcImage1[i]). Result is converted to unsigned 16bit
\brief Elementwise unsigned 8bit "&&" unsigned 16bit operation => unsigned 16bit

\param dst		 - [Output] unsigned 16bit destination block pointer
\param srcImage0 - [Input]  unsigned 8bit source block pointer of first image
\param srcImage1 - [Input]  unsigned 16bit source block pointer of second image
\param bw		 - [Input]  Block width
\param bh		 - [Input]  Block height
\param instrideW  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideW - [Input]  Destination block width (in elements not bytes) including padding
*/
void and_in08u_in16u_out16u(vec16u* dst, vec08u* srcImage0, vec16u* srcImage1, int bw, int bh, int inStrideW, int outStrideW);

/*!
Elementwise AND btw three unsigned 16bit numbers: dst[i] = (srcImage0[i]!= 0 && srcImage1[i]!= 0 && srcImage2[i]!= 0). Result is converted to unsigned 16bit
\brief Elementwise unsigned 3-point 16bit "&&" operation => unsigned 16bit

\param dst		 - [Output] 16bit Destination block pointer
\param srcImage0 - [Input]  16bit source block pointer of first image
\param srcImage1 - [Input]  16bit source block pointer of second image
\param srcImage2 - [Input]  16bit source block pointer of third image
\param bw		 - [Input]  Block width
\param bh		 - [Input]  Block height
\param instrideW  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideW - [Input]  Destination block width (in elements not bytes) including padding
*/
void and_3Pt_in16u_out16u(vec16u* dst, vec16u* srcImage0, vec16u* srcImage1, vec16u* srcImage2, int bw, int bh, int inStrideW, int outStrideW);

#endif /* APUCOMPARISONIMPL_H */

/*! @} */
