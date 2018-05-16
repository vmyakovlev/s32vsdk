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
* \file dot_multiplic_apu.h
* \brief Element-wise multiplication of two vectors/matrices.
* \addtogroup multiplication
* @{
****************************************************************************/
#ifndef APUDOTMULTIPLICKERNELIMPL_H
#define APUDOTMULTIPLICKERNELIMPL_H

#ifdef APEX2_EMULATE
#include "apu_config.hpp"
#include "apu_vec.hpp"
using namespace APEX2;
#endif

// Helper functions

/*! Tests if a times/div b would give a negative result
\brief sign(a) * sign(b) == -1
\param a - [Input] first operand
\param b - [Input] second operand
\return a boolean vector where the elements reflect the test operation result
*/
vbool hasSign(vec32s& a, vec32s& b);


/*! Change the sign of 64bit values stored in two vectors, a high and a low-word vector.
\brief In place sign change of a 64bit integer, i.e. a = -a
\param highWord - [Input/output] high word input/output. Result is stored directly into same vector
\param lowhWord - [Input/output] low word input/output. Result is stored directly into same vector
*/
void change64bitSign(vec32s& highWord, vec32u& lowWord);

// Pixel Mutliplication Kernels

/*!
Dot multiplication between two 16bit matrices (i.e multiply elementwise the matrix coeffs), with signed 32bit result. dst[i] = srcImageA[i] * srcImageB[i]
\brief Elementwise signed 16bit multiplication  => signed 32bit

\param dst  - [Output] signed 32bit destination block pointer
\param srcA - [Input]  signed 16bit source block pointer of img A
\param srcB - [Input]  signed 16bit source block pointer of img B
\param bw   - [Input]  Block width
\param bh   - [Input]  Block height
\param inStrideWidth  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input]  Destination block width (in elements not bytes) including padding
*/
void dot_mult_in16s_out32s_filter(vec32s* dst, vec16s* srcA, vec16s* srcB
								, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);

/*!
Dot multiplication between two 32bit matrices (i.e multiply elementwise the matrix coeffs), with signed 32bit result
\brief Elementwise signed 32bit multiplication  => signed 32bit

\warning  No out of range is taken into consideration!

\param dst  - [Output] signed 32bit destination block pointer  
\param srcA - [Input]  signed 32bit source block pointer of img A
\param srcB - [Input]  signed 32bit source block pointer of img B
\param bw   - [Input]  Block width
\param bh   - [Input]  Block height
\param inStrideWidth  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input]  Destination block width (in elements not bytes) including padding
*/
void dot_mult_in32s_out32s_filter(vec32s* dst, vec32s* srcA, vec32s* srcB
								, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/*!
Dot multiplication between a signed 32bit and a signed 16bit matrix (i.e multiply elementwise the matrix coeffs), with signed 32bit result
\brief Elementwise signed 32bit and 16bit multiplication  => signed 32bit

\warning  No out of range is taken into consideration!

\param dst  - [Output] signed 32bit destination block pointer
\param srcA - [Input]  signed 32bit source block pointer of img A
\param srcB - [Input]  signed 16bit source block pointer of img B
\param bw   - [Input]  Block width
\param bh   - [Input]  Block height
\param inStrideWidth  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input]  Destination block width (in elements not bytes) including padding
*/
void dot_mult_in32s_in16s_out32s_filter(vec32s* dst, vec32s* srcA, vec16s* srcB
									  , int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/*!
Dot 64bit multiplication between two signed 32bit matrices (i.e multiply elementwise the matrix coeffs). 
Results have 64bits and are organized into two 32bit matrices (i.e the lower and the higher word matrices of the multiplication result)

\brief Elementwise signed 32bit multiplication  => signed 64bit

\param dst_high	- [Output] signed 32bit high word of destination block pointer
\param dst_low  - [Output] signed 32bit low word of destination block pointer
\param srcA		- [Input]  signed 32bit source block pointer of img A
\param srcB		- [Input]  signed 32bit source block pointer of img B
\param bw		- [Input]  Block width
\param bh		- [Input]  Block height
\param inStrideWidth  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input]  Destination block width (in elements not bytes) including padding
*/
void dot_mult_in32s_out64s_filter(vec32s* dst_high, vec32u* dst_low, vec32s* srcA, vec32s* srcB
						, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/*!
Dot 64bit multiplication between two unsigned 32bit matrices (i.e multiply elementwise the matrix coeffs).
Results have 64bits and are organized into two 32bit matrices (i.e the lower and the higher word matrices of the multiplication result)

\brief Elementwise unsigned 32bit multiplication  => unsigned 64bit

\param dst_high - [Output] unsigned 32bit high word of destination block pointer
\param dst_low  - [Output] signed 32bit low word of destination block pointer
\param srcA		- [Input]  unsigned 32bit source block pointer of img A
\param srcB		- [Input]  unsigned 32bit source block pointer of img B
\param bw		- [Input] Block width
\param bh		- [Input] Block height
\param inStrideWidth  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input]  Destination block width (in elements not bytes) including padding
*/
void dot_mult_in32u_out64u_filter(vec32u* dst_high, vec32u* dst_low, vec32u* srcA, vec32u* srcB
						, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/*!
Dot 64bit multiplication between two signed 64bit matrices (i.e multiply elementwise the matrix coeffs).
Results have 64bits and are organized into two 32bit matrices (i.e the lower and the higher word matrices of the multiplication result)

\brief Elementwise signed 64bit multiplication  => signed 64bit

\warning out of ranges over the 64bit limit are not taken into account!

\param dst_high		- [Output] signed 32bit high word of destination block pointer
\param dst_low		- [Output] unsigned 32bit low word of destination block pointer
\param srcA_high	- [Input]  signed 32bit high word of 64bit source block pointer of img A
\param srcA_low		- [Input]  unsigned 32bit low word of 64bit source block pointer of img A
\param srcB_high	- [Input]  signed 32bit high word of 64bit source block pointer of img B
\param srcB_low		- [Input]  unsigned 32bit low word of 64bit source block pointer of img B
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param inStrideWidth  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input]  Destination block width (in elements not bytes) including padding
*/
void dot_mult_in64s_out64s_filter(vec32s* dst_high, vec32u* dst_low
						, vec32s* srcA_high, vec32u* srcA_low, vec32s* srcB_high, vec32u* srcB_low
						, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/*!
Dot 64bit multiplication between two unsigned 64bit matrices (i.e multiply elementwise the matrix coeffs).
Results have 64bits and are organized into two 32bit matrices (i.e the lower and the higher word matrices of the multiplication result)

\brief Elementwise unsigned 64bit multiplication  => unsigned 64bit

\warning out of ranges over the 64bit limit are not taken into account!

\param dst_high		- [Output] unsigned 32bit high word of 64bit destination block pointer
\param dst_low		- [Output] unsigned 32bit low word of 64bit destination block pointer
\param srcA_high	- [Input]  unsigned 32bit high word of 64bit source block pointer of img A
\param srcA_low		- [Input]  unsigned 32bit low word of 64bit source block pointer of img A
\param srcB_high	- [Input]  unsigned 32bit high word of 64bit source block pointer of img B
\param srcB_low		- [Input]  unsigned 32bit low word of 64bit source block pointer of img B
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param inStrideWidth  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input]  Destination block width (in elements not bytes) including padding
*/
void dot_mult_in64u_out64u_filter(vec32u* dst_high, vec32u* dst_low
						, vec32u* srcA_high, vec32u* srcA_low, vec32u* srcB_high, vec32u* srcB_low
						, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/*!
Dot multiplication of all elements of a 8bit matrix with a scalar value, with signed 16bit output. dst[i] = srcImage0[i] * scalar;
\brief Elementwise unsigned 8bit multiplication with a fixed scalar  => signed 16bit

\warning The scalar value should have ideally a 8bit value, or maximally a 16bit value. 
\warning No out of range is checked!

\param dst		- [Output] signed 16bit destination block pointer
\param srcA		- [Input]  unsigned 8bit source block pointer of img A
\param scalar	- [Input]  signed 32bit the scalar value to be multiplied with the matrix
\param bw		- [Input]  Block width
\param bh		- [Input]  Block height
\param inStrideWidth  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input]  Destination block width (in elements not bytes) including padding
*/
void dot_mult_scalar_in08u_out16s_filter(vec16s* dst, vec08u* srcA, int32s scalar
	, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/*!
Dot multiplication of all elements of a 32bit matrix with a 32bit scalar value. Output is of signed 32bits

\brief Elementwise signed 32bit multiplication with a fixed scalar  => signed 32bit

\warning No out of range is checked above the 32bit value range!

\param dst		- [Output] 16bit destination block pointer
\param srcA		- [Input]  8bit source block pointer of img A
\param scalar	- [Input]  32bit the scalar value to be multiplied with the matrix
\param bw		- [Input]  Block width
\param bh		- [Input]  Block height
\param inStrideWidth  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input]  Destination block width (in elements not bytes) including padding
*/
void dot_mult_scalar_in32s_out32s_filter(vec32s* dst, vec32s* srcA, int32s scalar
	, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);



/*!
Dot left-shift unsigned 16bit operator by a signed 16bit input vector (each element of one chunk_i is shifted by the shift_vect(i) factor) with signed 16bit output.
dst_chunk[i] = srcImage0_chunk[i] << leftShiftFact[i]

\brief Chunk-wise unsigned 16bit left shift with a signed 16bit shift vector => signed 16bit

\warning No out of range is checked above the 16bit value range!

\param upShifted		- [Output] signed 16bit destination block pointer for the upShifteded result
\param src				- [Input]  unsigned 16bit source block pointer
\param leftShiftFact	- [Input]  signed 16bit vector of shift values, one for each chunk
\param bw				- [Input]  Block width
\param bh				- [Input]  Block height
\param inStrideWidth	- [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth	- [Input]  Destination block width (in elements not bytes) including padding
*/
void lsh_in16u_out16s_filter(vec16s* upShifted, vec16u* src, vec16s* leftShiftFact
	, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/*!
Dot left-shift unsigned 32bit operator (each element of one chunk_i is shifted by the shift_vect(i) factor)  with unsigned 32bit output
\brief Chunk-wise unsigned 32bit left shift with an unsigned 8bit shift vector => unsigned 32bit

\warning No out of range is checked above the 32bit value range!

\param upShifted		- [Output] unsigned 32bit destination block pointer for the upShifteded result
\param src				- [Input]  unsigned 32bit source block pointer
\param leftShiftFact	- [Input]  unsigned 8bit vector of shift values, one for each chunk
\param bw				- [Input]  Block width
\param bh				- [Input]  Block height
\param inStrideWidth	- [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth	- [Input]  Destination block width (in elements not bytes) including padding
*/
void lsh_in32u_out32u_filter(vec32u* upShifted, vec32u* src, vec08u* leftShiftFact
	, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);

/*!
Dot left-shift signed 32bit operator (each element of one chunk_i is shifted by the shift_vect(i) factor)  with signed 32bit output
\brief Chunk-wise signed 32bit left shift with an unsigned 8bit shift vector => signed 32bit

\warning No out of range is checked above the 32bit value range!

\param upShifted		- [Output] signed 32bit destination block pointer for the upShifteded result
\param src				- [Input]  signed 32bit source block pointer
\param leftShiftFact	- [Input]  unsigned 8bit vector of shift values, one for each chunk
\param bw				- [Input]  Block width
\param bh				- [Input]  Block height
\param inStrideWidth	- [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth	- [Input]  Destination block width (in elements not bytes) including padding
*/
void lsh_in32s_out32s_filter(vec32s* upShifted, vec32s* src, vec08u* leftShiftFact
	, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/*!
Dot left-shift signed 32bit operator (each element of one chunk_i is shifted by the shift_vect(i) factor) with signed 64bit output
\brief Chunk-wise signed 32bit left shift with an unsigned 8bit shift vector => signed 64bit

\param upShifted_high	- [Output] signed 32bit high word of destination block pointer for the upShifted result
\param upShifted_low	- [Output] unsigned 32bit low word of destination block pointer for the upShifted result
\param src				- [Input]  signed 32bit source block pointer
\param leftShiftFact	- [Input]  unsigned 8bit vector of shift values, one for each chunk
\param bw				- [Input]  Block width
\param bh				- [Input]  Block height
\param inStrideWidth	- [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth	- [Input]  Destination block width (in elements not bytes) including padding
*/
void lsh_in32s_out64s_filter(vec32s* upShifted_high, vec32u* upShifted_low, vec32s* src, vec08u* leftShiftFact
	, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/*!
Dot left-shift unsigned 32bit operator (each element of one chunk_i is shifted by the shift_vect(i) factor) with unsigned 64bit output
\brief Chunk-wise unsigned 32bit left shift with an unsigned 8bit shift vector => unsigned 64bit

\param upShifted		- [Output] unsigned 32bit destination block pointer for the upShifted result
\param src				- [Input]  unsigned 32bit source block pointer
\param leftShiftFact	- [Input]  the vector of shift values, one for each chunk
\param bw				- [Input]  Block width
\param bh				- [Input]  Block height
\param inStrideWidth	- [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth	- [Input]  Destination block width (in elements not bytes) including padding
*/
void lsh_in32u_out64u_filter(vec32u* upShifted_high, vec32u* upShifted_low, vec32u* src, vec08u* leftShiftFact
	, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);

/*!
Dot left-shift operator for a Q3_28 fixed point input parameter format with 64bit output
The output "high" word contains  [x] - integer part of the left shifted fixed point input number 
and the output "low" word contains  {x} - fractional part of the left shifted fixed point input number

\brief Chunk-wise unsigned 32bit left shift of a 32bit matrix in Q3_28 format with an unsigned 8bit shift vector => signed 64bit

\param upShifted_int	- [Output] [x] - signed 32bit integer part of the left shifted fixed point input number 
\param upShifted_frac	- [Output] {x} - signed 32bit fractional part of the left shifted fixed point input number
\param src				- [Input]  signed 32bit source block pointer containing fixed point numbers in Q3_28 format
\param leftShiftFact	- [Input]  unsigned 8bit vector of shift values, one for each chunk
\param bw				- [Input]  Block width
\param bh				- [Input]  Block height
\param inStrideWidth	- [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth	- [Input]  Destination block width (in elements not bytes) including padding
*/
void lsh_in32s_Q3_28_out64s_filter(vec32s* upShifted_int, vec32s* upShifted_frac, vec32s* src, vec08u* leftShiftFact
	, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/*!
Dot right-shift unsigned 32bit operators (each element of one chunk_i is shifted by the shift_vect(i) factor) with unsigned 32bit output
dst_chunk[i] = srcImage0_chunk[i] >> rightShiftFact[i]

\brief Chunk-wise unsigned 32bit right shift with an unsigned 8bit shift vector => unsigned 32bit

\param downShift	- [Output] 32bit destination block pointer for the downshifted result
\param src			- [Input]  32bit source block pointer 
\param rightShiftFact -[Input] the vector of shift values, one for each chunk
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param inStrideWidth  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input]  Destination block width (in elements not bytes) including padding
*/
void rsh_in32u_out32u_filter(vec32u* downShift, vec32u* src, vec08u* rightShiftFact
					, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/*!
Dot right-shift signed 32bit operators (each element of one chunk_i is shifted by the shift_vect(i) factor) with signed 32bit output
\brief Chunk-wise signed 32bit right shift with a signed 8bit shift vector => signed 32bit

\param downShift	- [Output] 32bit destination block pointer for the downshifted result
\param src			- [Input]  32bit source block pointer
\param rightShiftFact - [Input] the vector of shift values, one for each chunk
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param inStrideWidth  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input]  Destination block width (in elements not bytes) including padding
*/
void rsh_in32s_out32s_filter(vec32s* downShift, vec32s* src, vec08u* rightShiftFact
					, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);



/*!
Dot right-shift operator for 64bit signed input (each element of one chunk_i is shifted by the shift_vect(i) factor)
\brief Chunk-wise signed 64bit right shift with a signed 8bit shift vector => signed 64bit

\param dst_high		- [Output] 32bit signed high word of destination block pointer for the downshifted result
\param dst_low		- [Output] 32bit unsigned low word of destination block pointer for the downshifted result
\param in_high		- [Input]  32bit signed high word of source block pointer
\param in_low		- [Input]  32bit unsigned low word of source block pointer
\param rightShiftFact - [Input] the vector of shift values, one for each chunk
\param bw			- [Input] Block width
\param bh			- [Input] Block height
\param inStrideWidth  - [Input] Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input] Destination block width (in elements not bytes) including padding
*/
void rsh_in64s_out64s_filter(vec32s* dst_high, vec32u* dst_low, vec32s* in_high, vec32u* in_low, vec08u* rightShiftFact
							, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/*!
Dot right-shift operator for 64bit unsigned input (each element of one chunk_i is shifted by the shift_vect(i) factor)
\brief Chunk-wise signed 64bit right shift with a signed 8bit shift vector => signed 64bit

\param dst_high		- [Output] unsigned 32bit high word of destination block pointer for the downshifted result
\param dst_low		- [Output] unsigned 32bit low word of destination block pointer for the downshifted result
\param in_high		- [Input]  unsigned 32bit high word of source block pointer
\param in_low		- [Input]  unsigned 32bit low word of source block pointer
\param rightShiftFact - [Input] unsigned 8bit  shift values, one for each block
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param inStrideWidth  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input]  Destination block width (in elements not bytes) including padding
*/
void rsh_in64u_out64u_filter(vec32u* dst_high, vec32u* dst_low, vec32u* in_high, vec32u* in_low, vec08u* rightShiftFact
							, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);



/*!
Dot right-shift operator for 64bit signed input (each element of one chunk_i is shifted by the shift_vect(i) factor)
\brief Chunk-wise signed 64bit right shift with a signed 8bit shift vector => signed 32bit

\warning No check is performed to see that the right shifted values became less than 2^32
\param dst			- [Output] signed 32bit destination block pointer for the downshifted result
\param in_high		- [Input]  signed 32bit high word of source block pointer
\param in_low		- [Input]  unsigned 32bit low word of source block pointer
\param rightShiftFact - [Input] unsigned 8bit shift values, one for each block
\param bw			- [Input] Block width
\param bh			- [Input] Block height
\param inStrideWidth  - [Input] Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input] Destination block width (in elements not bytes) including padding
*/
void rsh_in64s_out32s_filter(vec32s* dst, vec32s* in_high, vec32u* in_low, vec08u *rightShiftFact, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);

#endif /* APUDOTMULTIPLICKERNELIMPL_H */

/*! @} */
