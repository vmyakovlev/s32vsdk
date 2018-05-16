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
* \file cvt_16low_to_8_apu.h
* \addtogroup byte_conversion
* \brief image 16low_to_8 implementation for APEX
* @{
****************************************************************************/

#ifndef APU16LOWTO8IMPL_H
#define APU16LOWTO8IMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

/****************************************************************************/
/** Extracts lower parts of the 16-bit image pixels into 8-bit image.
   \brief Extracts the lower bytes.

   \param dst  - [Output] Pointer to the destination buffer
   \param src  - [Input]  Pointer to the source buffer
   \param bw   - [Input]  Width of one data block
   \param bh   - [Input]  Height of one data block
*****************************************************************************/
void f16low_to_8(vec08u* dst, vec16u* src, int bw, int bh);


#endif /* APU16LOWTO8IMPL_H */

/*! @} */
