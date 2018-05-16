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
* \file saturate_nonzero_apu.h
* \brief non-zero saturation implementation for APEX
* \addtogroup saturation
* @{
****************************************************************************/
#ifndef APUSATURATENONZEROIMPL_H
#define APUSATURATENONZEROIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

/****************************************************************************/
/*!
   Changes non-zero pixel values to maximal values.
   \brief Non-zero pixel saturation.

   \param dst  - [Output] Pointer to the destination buffer (unsigned 8bit)
   \param src  - [Input]  Pointer to the source buffer		(unsigned 8bit)
   \param dstr - [Input]  Line stride of the destination data
   \param sstr - [Input]  Line stride of the source data
   \param bw   - [Input]  Width of one data block
   \param bh   - [Input]  Height of one data block
*****************************************************************************/
void saturate_nonzero(vec08u* dst, vec08u* src, int dstr, int sstr, int bw, int bh);


/****************************************************************************/
/*!
Changes non-zero pixel values to maximal values.
\brief Non-zero pixel binarization.

\param dst  - [Output] Pointer to the destination buffer (unsigned 8bit)
\param src  - [Input]  Pointer to the source buffer		 (unsigned 32bit)
\param dstr - [Input]  Line stride of the destination data
\param sstr - [Input]  Line stride of the source data
\param bw   - [Input]  Width of one data block
\param bh   - [Input]  Height of one data block
*****************************************************************************/
void binarize(vec08u* dst, vec32u* src, int /*dstr*/, int /*sstr*/, int bw, int bh);


/****************************************************************************/
/*!
Masks the output pixel values to zero, if the srcFlags are zero. Useful to mask thresholded GradientX/Y
\brief Masking to zero with the srcFlags.

\param dstX  - [Output] Pointer to the X destination buffer (signed 16bit)
\param dstY  - [Output] Pointer to the Y destination buffer (signed 16bit)
\param srcFlags- [Input]  Pointer to the mask source buffer (unsigned 32bit)
\param inX  - [Output] Pointer to the X source buffer to be masked (signed 16bit)
\param inY  - [Output] Pointer to the Y source buffer to be masked(signed 16bit)
\param dstr - [Input]  Line stride of the destination data
\param sstr - [Input]  Line stride of the source data
\param bw   - [Input]  Width of one data block
\param bh   - [Input]  Height of one data block
*****************************************************************************/
void mask(vec16s* dstX, vec16s* dstY, vec32u* srcFlags, vec16s* inX, vec16s* inY, int /*dstr*/, int /*sstr*/, int bw, int bh);

#endif /* APUSATURATENONZEROIMPL_H */

/*! @} */
