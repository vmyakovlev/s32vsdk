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
* \file gauss_3x1_apu.h
* \brief element-wise addition implementation for APEX
* \addtogroup addition
* @{
****************************************************************************/

#ifndef APUGAUSS3X1IMPL_H
#define APUGAUSS3X1IMPL_H

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
void gauss_3x1(vec08u* dst, vec08u* srcImage0, int bw, int bh, int inStrideW, int outStrideW);

#endif /* APUGAUSS3X1IMPL_H */
/*! @} */
