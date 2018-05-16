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
* \file difference_apu.h
* \brief Computes the pixelwise difference btw. two images.
* \addtogroup difference
* @{
****************************************************************************/
#ifndef APUDIFFERENCEKERNELIMPL_H
#define APUDIFFERENCEKERNELIMPL_H


#ifdef APEX2_EMULATE
#include "apu_config.hpp"
#include "apu_vec.hpp"
using namespace APEX2;
#endif

/*!	Pixel-wise difference between two unsigned 8bit images with signed 16bit result. dst[i] = srcImageA[i] - srcImageB[i];
\brief Pixel-wise difference between two unsigned 8bit images => signed 16bit

\param dst  - [Output] 16bit Destination block pointer
\param srcA - [Input]  8bit Source block pointer of img A
\param srcB - [Input]  8bit Source block pointer of img B
\param bw   - [Input]  Block width
\param bh   - [Input]  Block height
\param inStrideWidth - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input] Output block width (in elements not bytes) including padding
*/
void difference_filter_in08u_out16s(vec16s* dst, vec08u* srcA, vec08u* srcB, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);

/*!	Pixel-wise difference between two signed 16bit images with 16bit result. 
\brief Pixel-wise difference between two signed 16bit  images => signed 16bit

\warning No out of range is checked
\param dst  - [Output] signed 16bit Destination block pointer
\param srcA - [Input]  signed 16bit Source block pointer of img A
\param srcB - [Input]  signed 16bit Source block pointer of img B
\param bw   - [Input]  Block width
\param bh   - [Input]  Block height
\param inStrideWidth - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input] Output block width (in elements not bytes) including padding
*/
void difference_filter_in16s_out16s(vec16s* dst, vec16s* srcA, vec16s* srcB, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);

/*!	Pixel-wise difference between two signed 16bit images with signed 32bit result
\brief Pixel-wise difference between two signed 16bit  images => signed 32bit

\param dst  - [Output] signed 32bit Destination block pointer
\param srcA - [Input]  signed 16bit Source block pointer of img A
\param srcB - [Input]  signed 16bit Source block pointer of img B
\param bw   - [Input]  Block width
\param bh   - [Input]  Block height
\param inStrideWidth - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input] Output block width (in elements not bytes) including padding
*/
void difference_filter_in16s_out32s(vec32s* dst, vec16s* srcA, vec16s* srcB, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);

/*!	Pixel-wise difference between two signed 32bit images with signed 32bit result
\brief Pixel-wise difference between two signed 16bit  images => signed 32bit

\warning No out of range is checked!
\param dst  - [Output] signed 32bit Destination block pointer
\param srcA - [Input]  signed 32bit source block pointer of img A
\param srcB - [Input]  signed 32bit source block pointer of img B
\param bw   - [Input]  Block width
\param bh   - [Input]  Block height
\param inStrideWidth - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input] Output block width (in elements not bytes) including padding
*/
void difference_filter_in32s_out32s(vec32s* dst, vec32s* srcA, vec32s* srcB, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/*!	Pixel-wise difference between two unsigned 32bit images with signed 32bit result.
\brief Pixel-wise difference between two unsigned 32bit  images => signed 32bit

\warning No out of range is checked. If the unsigned inputs are higher than 0x7fff ffff, wrong results will arise
\param dst - [Output] 32bit high word of destination block pointer
\param srcA	- [Input]  unsigned 32bit source block pointer of img A
\param srcB	- [Input]  unsigned 32bit source block pointer of img B
\param bw   - [Input]  Block width
\param bh   - [Input]  Block height
\param inStrideWidth - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input] Output block width (in elements not bytes) including padding
*/
void difference_filter_in32u_out32s(vec32s* dst, vec32u* srcA, vec32u* srcB, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);

/*!	Pixel-wise difference between two signed 32bit images with signed 64 bit result
\brief Pixel-wise difference between two signed 32bit  images => signed 64bit

\param dst_high	- [Output] signed 32bit high word of destination block pointer
\param dst_low	- [Output] unsigned 32bit low word of destination block pointer
\param srcA		- [Input]  signed 32bit source block pointer of img A
\param srcB		- [Input]  signed 32bit source block pointer of img B
\param bw		- [Input]  Block width
\param bh		- [Input]  Block height
\param inStrideWidth - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input] Output block width (in elements not bytes) including padding
*/
void difference_filter_in32s_out64s(vec32s* dst_high, vec32u* dst_low, vec32s* srcA, vec32s* srcB
									, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


/*!	Pixel-wise difference between two signed 64bit images, given as low and high word blocks with signed 64bit result
\brief Pixel-wise difference between two signed 64bit  images => signed 64bit

\param dst_high	- [Output] signed 32bit high word of destination block pointer
\param dst_low	- [Output] unsigned 32bit low word of destination block pointer
\param srcA_high - [Input]  signed 32bit high word source block pointer of img A
\param srcA_low - [Input]  unsigned 32bit low word source block pointer of img A
\param srcB_high - [Input]  signed 32bit high word source block pointer of img B
\param srcB_low - [Input]  unsigned 32bit low word source block pointer of img B
\param bw		- [Input]  Block width
\param bh		- [Input]  Block height
\param inStrideWidth - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input] Output block width (in elements not bytes) including padding
*/
void difference_filter_in64s_out64s(vec32s* dst_high, vec32u* dst_low
									, vec32s* srcA_high, vec32u* srcA_low, vec32s* srcB_high, vec32u* srcB_low
									, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);

/*!	Pixel-wise difference between two unsigned 64bit images, given as low and high word blocks with signed 64bit result
\brief Pixel-wise difference between two unsigned 64bit  images => signed 64bit

\param dst_high	- [Output] signed 32bit high word of destination block pointer
\param dst_low	- [Output] unsigned 32bit low word of destination block pointer
\param srcA_high - [Input]  unsigned 32bit high word source block pointer of img A
\param srcA_low - [Input]  unsigned 32bit low word source block pointer of img A
\param srcB_high - [Input]  unsigned 32bit high word source block pointer of img B
\param srcB_low - [Input]  unsigned 32bit low word source block pointer of img B
\param bw		- [Input]  Block width
\param bh		- [Input]  Block height
\param inStrideWidth - [Input]  Source block width (in elements not bytes) including padding
\param outStrideWidth - [Input] Output block width (in elements not bytes) including padding
*/
void difference_filter_in64u_out64s(vec32s* dst_high, vec32u* dst_low
									, vec32u* srcA_high, vec32u* srcA_low, vec32u* srcB_high, vec32u* srcB_low
									, int16s bw, int16s bh, int16s inStrideWidth, int16s outStrideWidth);


#endif /* APUDIFFERENCEKERNELIMPL_H */

/*! @} */
