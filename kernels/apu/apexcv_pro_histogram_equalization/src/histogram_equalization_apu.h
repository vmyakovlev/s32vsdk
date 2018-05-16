/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
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
* \file apu_histogram_equalization_impl.h
* \brief histogram equalization computation for APEX
* \addtogroup histogram_equalization
* @{
****************************************************************************/

#ifndef APUHISTOGRAMEQUALIZATIONIMPL_H
#define APUHISTOGRAMEQUALIZATIONIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif
#ifdef ACF_KERNEL_IMPLEMENTATION 
/*!
Histogram equalization of an input image
\brief Elementwise unsigned 8bit addition => unsigned 16bit 
\param lpvIn0      - [Input]  8bit source block pointer of img 0
\param lpvOut0     - [Output] 8bit destination block pointer
\param lut      - [Input]  8bit look up table for the normalization

*/
void apu_histogram_equalization_impl
(
   vec08u*  dst,
   int dstr,
   vec08u*  src,
   int sstr,
   int  bw, 
   int bh, 
   vec08u* lut
);

/*!
   Transforming the image histogram, into a new histogram used to set the values for the output

   \param transformed_hist       - [Output] Pointer to input images histogram 
   \param hist                   - [Input]  Pointer to input images histogram 
   \param numPixels              - [Output] The number of pixels in the source image
 */
void apu_generate_lut_impl
(
   uint8_t* lut,
   uint32_t* hist,           
   uint32_t numPixels  
);


         
#endif 
#endif /* APUHISTOGRAMEQUALIZATIONIMPL_H */

/*! @} */
