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
* \file dot_division_apu.h
* \brief Element-wise division of two vectors/matrices. If divisor is zero, a value of zero is returned (in order not to influence following arithmetics).
* \addtogroup division
* @{
****************************************************************************/
#ifndef APUDOTDIVISIONKERNELIMPL_H
#define APUDOTDIVISIONKERNELIMPL_H

#ifdef APEX2_EMULATE
#include "apu_config.hpp"
#include "apu_vec.hpp"
using namespace APEX2;
#endif

/** Divide 32 bit numbers with the APEX internal division. quot[i] = numerator[i]/denominator[i], foreach i
\brief Elementwise division of signed 32bit integers
\param quot			- [Output] Destination block pointer
\param numerator	- [Input]  Source block pointer of img A
\param denominator	- [Input]  Source block pointer of img B
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param inStrideWidth  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input]  Destination block width (in elements not bytes) including padding
*/
void dot_division_filter(vec32s* res, vec32s* numerator, vec32s* denominator, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);

/**  Pixelwise Computation the log_2 + 1 of all the elements of the input. This is a helper function for computeInv_NewtonRaphson 
\brief res[i] = log2( abs(input[i]) ) + 1, where input is a signed 32bit integer
\param log2Fact - [Output] the values representing 1+log_2(abs(input)) 
\param input	- [Input] the 32bit signed input vector 
\param bw		- [Input]  Block width
\param bh		- [Input]  Block height
\param inStrideWidth  - [Input]  Source block width(in elements not bytes) including padding
\param outStrideWidth - [Input]  Destination block width(in elements not bytes) including padding
*/
void computeLog2(vec08u* log2Fact, vec32s* input, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);

/**  Computes the log_2 + 1 of all the elements of the input. This is a helper function for computeInv_NewtonRaphson 
\brief res[i] = log2(input[i]) + 1, where input is a unsigned 32bit integer
\param log2Fact - [Output]  the values representing 1+log_2(input)
\param input	- [Input] the 32bit unsigned input vector
\param bw		- [Input]  Block width
\param bh		- [Input]  Block height
\param inStrideWidth  - [Input]  Source block width(in elements not bytes) including padding
\param outStrideWidth - [Input]  Destination block width(in elements not bytes) including padding
*/
void computeLog2u(vec08u* log2Fact, vec32u* input, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/**  Computes the log_2 + 1 of all the elements of the signed 64bit input. This is a helper function
\brief res[i] = log2( abs(input[i]) ) + 1, where input is a signed 64bit integer
\param log2Fact		- [Output]  the value representing 1+log_2(input)
\param input_high	- [Input] the high word of the 64bit input vector 
\param input_low	- [Input] the low word of the 64bit input vector
\param bw			- [Input]  Block width
\param bh			- [Input]  Block height
\param inStrideWidth  - [Input]  Source block width(in elements not bytes) including padding
\param outStrideWidth - [Input]  Destination block width(in elements not bytes) including padding
*/
void compute64bitLog2 (vec08u* log2Fact, vec32s* input_high, vec32u* input_low, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/**  Computes the log_2 + 1 of all the elements of the unsigned 64bit input. This is a helper function
\brief res[i] = log2( input[i] ) + 1, where input is a unsigned 64bit integer
\param log2Fact		- [Output]  the value representing 1+log_2(input)
\param input_high	- [Input] the high word of the 64bit input vector
\param input_low	- [Input] the low word of the 64bit input vector
\param bw			- [Input]  Block width
\param bh			- [I nput]  Block height
\param inStrideWidth  - [Input]  Source block width(in elements not bytes) including padding
\param outStrideWidth - [Input]  Destination block width(in elements not bytes) including padding
*/
void compute64bitLog2u(vec08u* log2Fact, vec32u* input_high, vec32u* input_low, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/** Compute the inverse of a 32bit integer in Q<ShiftFact>.(31-<ShiftFact>) format with the NewtonRaphson algorithm
\brief Elementwise inverse of a signed integer vector using the NewtonRaphson algorithm.
\param invDiv	- [Output]  the inverse of div in  Q<ShiftFact>.(31-<ShiftFact>)
\param div		- [Input] the vector containing the numbers to be inverted (signed 32bit)
\param log2Fact - [Output]  Returns the vector of log_2 values of each member of div
\param shiftFact - [Input] the shift factor for the fixed point Q<ShiftFact>.(31-<ShiftFact>) format
\param bw		- [Input]  Block width
\param bh		- [Input]  Block height
\param inStrideWidth  - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input]  Destination block width (in elements not bytes) including padding
*/
void computeInv_NewtonRaphson(vec32s* invDiv, vec32s* div, vec08u* log2Fact, const int08s shiftFact, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);

/** Compute the 64bit division of a 64bit integer with a 32bit integer
	\brief Elementwise division of a 64bit nominator by a 32bit divisor
	\param dst_high - [Output] High word of destination block pointer
	\param dst_low  - [Output] Low word of destination block pointer
	\param dst_rem  - [Output] Remainder of the division (i.e. decimal part as integer) 
	\param nom_high - [Input] Source block pointer to high word of nominator
	\param nom_low	- [Input]  Source block pointer to low word of nominator
	\param bw   - [Input]  Block width
	\param bh   - [Input]  Block height
	\param inStrideWidth  - [Input]  Source block width (in elements not bytes) including padding
	\param outStrideWidth - [Input]  Destination block width (in elements not bytes) including padding
*/
void dot_division_filter_N64s_D32s_Q64s(vec32s* dst_high, vec32u* dst_low, vec32u* dst_rem, vec32s* nom_high, vec32u* nom_low, vec32s* divisor, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


#endif /* APUDOTDIVISIONKERNELIMPL_H */

/*! @} */
