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
* \file add_apu.h
* \brief element-wise addition implementation for APEX
* \addtogroup addition
* @{
****************************************************************************/

#ifndef APUADDIMPL_H
#define APUADDIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

/*!
Addition btw two unsigned 8bit matrices with 16bit result: dst[i] = srcImage0[i] + srcImage1[i]
\brief Elementwise unsigned 8bit addition => unsigned 16bit 
\param dst		 - [Output] 16bit destination block pointer
\param srcImage0 - [Input]  8bit source block pointer of img 0
\param srcImage1 - [Input]  8bit source block pointer of img 1
\param bw		 - [Input]  Block width
\param bh		 - [Input]  Block height
\param inStrideW - [Input]  Source block width (in elements not bytes) including padding
\param outStrideW - [Input]  Destination block width (in elements not bytes) including padding
*/
void add(vec16u* dst, vec08u* srcImage0, vec08u* srcImage1, int bw, int bh, int inStrideW, int outStrideW);

/*!
Addition btw two signed 16bit matrices with 32bit result: dst[i] = srcImage0[i] + srcImage1[i]
\brief Elementwise signed 16bit addition => signed 32bit 
\param dst		 - [Output] 32bit destination block pointer
\param srcImage0 - [Input]  16bit source block pointer of img 0
\param srcImage1 - [Input]  16bit source block pointer of img 1
\param bw		 - [Input]  Block width
\param bh		 - [Input]  Block height
\param inStrideW - [Input]  Source block width (in elements not bytes) including padding
\param outStrideW - [Input]  Destination block width (in elements not bytes) including padding
*/
void add_in16s_out32s(vec32s* dst, vec16s* srcImage0, vec16s* srcImage1, int bw, int bh, int inStrideW, int outStrideW);


/*!
Addition btw two signed 32bit matrices: dst[i] = srcImage0[i] + srcImage1[i]
\brief Elementwise signed 32bit addition => signed 32bit
\warning No out of range above 32bits is checked
\param dst		 - [Output] signed 32bit destination block pointer
\param srcImage0 - [Input]  signed 32bit source block pointer of img 0
\param srcImage1 - [Input]  signed 32bit source block pointer of img 1
\param bw		 - [Input]  Block width
\param bh		 - [Input]  Block height
\param instrideW - [Input]  Source block width (in elements not bytes) including padding
\param outStrideW - [Input]  Destination block width (in elements not bytes) including padding
*/
void add_in32s_out32s(vec32s* dst, vec32s* srcImage0, vec32s* srcImage1, int bw, int bh, int inStrideW, int outStrideW);


/*!
Addition btw two unsigned 32bit matrices: dst[i] = srcImage0[i] + srcImage1[i]
\brief Elementwise unsigned 32bit addition => unsigned 32bit
\warning No out of range above 32bits is checked
\param dst		 - [Output] unsigned 32bit destination block pointer
\param srcImage0 - [Input]  unsigned 32bit source block pointer of img 0
\param srcImage1 - [Input]  unsigned 32bit source block pointer of img 1
\param bw		 - [Input]  Block width
\param bh		 - [Input]  Block height
\param instrideW - [Input]  Source block width (in elements not bytes) including padding
\param outStrideW - [Input]  Destination block width (in elements not bytes) including padding
*/
void add_in32u_out32u(vec32u* dst, vec32u* srcImage0, vec32u* srcImage1, int bw, int bh, int inStrideW, int outStrideW);


/*!
Addition btw two signed 64bit matrices: dst[i] = srcImage0[i] + srcImage1[i]
\brief Elementwise signed 64bit addition => signed 64bit 
\warning No out of range above 64bits is checked
\param dst_high			 - [Output] High word 32bit destination block pointer
\param dst_low			- [Output] Low word 32bit destination block pointer
\param srcImage0_high	- [Input]  High word 32bit source block pointer of img 0
\param srcImage0_low	- [Input]  Low word 32bit source block pointer of img 0
\param srcImage1_high	- [Input]  High word 32bit source block pointer of img 1
\param srcImage1_low	- [Input]  Low word 32bit source block pointer of img 1
\param bw				- [Input]  Block width
\param bh				- [Input]  Block height
\param inStrideW		- [Input]  Source block width (in elements not bytes) including padding
\param outStrideW		- [Input]  Destination block width (in elements not bytes) including padding
*/
void add_in64s_out64s(vec32s* dst_high, vec32u* dst_low, vec32s* srcImage0_high, vec32u* srcImage0_low, vec32s* srcImage1_high, vec32u* srcImage1_low, int bw, int bh, int inStrideW, int outStrideW);


/*!
Addition btw two unsigned 64bit matrices: dst[i] = srcImage0[i] + srcImage1[i]
\brief Elementwise unsigned 64bit addition => unsigned 64bit 
\warning No out of range above 64bits is checked
\param dst_high			 - [Output] High word 32bit destination block pointer
\param dst_low			- [Output] Low word 32bit destination block pointer
\param srcImage0_high	- [Input]  High word 32bit source block pointer of img 0
\param srcImage0_low	- [Input]  Low word 32bit source block pointer of img 0
\param srcImage1_high	- [Input]  High word 32bit source block pointer of img 1
\param srcImage1_low	- [Input]  Low word 32bit source block pointer of img 1
\param bw				- [Input]  Block width
\param bh				- [Input]  Block height
\param inStrideW		- [Input]  Source block width (in elements not bytes) including padding
\param outStrideW		- [Input]  Destination block width (in elements not bytes) including padding
*/
void add_in64u_out64u(vec32u* dst_high, vec32u* dst_low, vec32u* srcImage0_high, vec32u* srcImage0_low, vec32u* srcImage1_high, vec32u* srcImage1_low, int bw, int bh, int inStrideW, int outStrideW);


/*!
Addition btw two 32bit fixed point numbers in Q3.28 format:
dstInt[i] = int(srcImage0[i] + srcImage1[i]);
dstFrac[i] = frac(srcImage0[i] + srcImage1[i]); (the number below zero, which is represented here as 0.n * 2^28

\brief Elementwise 32bit Q3.28 addition => 64bit in [Q3.28] (integer part) and {Q3.28} (fractional part) format

\param dstInt	 - [Output] 32bit the integer part of the addition
\param dstFrac	 - [Output] 32bit the fractional part of the addition in Q3.28 fixed point format
\param srcImage0 - [Input]  32bit source block pointer of vector 0 in Q3.28 fixed point format
\param srcImage1 - [Input]  32bit source block pointer of vector 1 in Q3.28 fixed point format
\param bw		 - [Input]  Block width
\param bh		 - [Input]  Block height
\param instrideW - [Input]  Source block width (in elements not bytes) including padding
\param outStrideW - [Input]  Destination block width (in elements not bytes) including padding
*/
void add_in32Q3_28_out32Q3_28(vec32s* dstInt, vec32s* dstFrac, vec32s* srcImage0, vec32s* srcImage1, int bw, int bh, int inStrideW, int outStrideW);

#endif /* APUADDIMPL_H */ 

/*! @} */
