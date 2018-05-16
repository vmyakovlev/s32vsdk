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

/**************************************************************************** /
/*!
* \file interpolation_apu.h
* \brief interpolation functions implementation for APEX
* \addtogroup apexcv interpolation implementation for APEX
* @{
****************************************************************************/

/*!*********************************************************************************
*  \file apu_interpolation.h
*  \brief APU interpolation kernels.
***********************************************************************************/
#ifdef ACF_KERNEL_IMPLEMENTATION
#ifndef APUINTERPOLATIONIMPL_H
#define APUINTERPOLATIONIMPL_H


#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif


/*!
   Applies a Grayscale Linear Interpolation.
   
   \param dst       - [Output] Destination block pointer
   \param dstr      - [Input]  Destination block stride
   \param src       - [Input]  Source block pointer
   \param sstr      - [Input]  Source block stride
   \param bw        - [Input]  Block width
   \param bh        - [Input]  Block height
   \param delta     - [Input]  Delta X Values
 */
void 
apu_interp_linear_grayscale (
            vec08u* dst, int dstr,
      const vec08u* src, int sstr,
                 int bw, int bh,
      const vec08u* delta);
              
/*!
   Applies a Grayscale Bilinear Interpolation.
   
   \param dst       - [Output] Destination block pointer
   \param dstr      - [Input]  Destination block stride
   \param src       - [Input]  Source block pointer
   \param sstr      - [Input]  Source block stride
   \param bw        - [Input]  Block width
   \param bh        - [Input]  Block height
   \param delta     - [Input]  Delta XY Values
 */
void
apu_interp_bilinear_grayscale (
            vec08u* dst, int dstr,
      const vec08u* src, int sstr,
                 int bw, int bh,
      const vec08u* delta);

/*!
   Applies a Bicubic Interpolation.
   
   \param dst       - [Output] Destination block pointer
   \param dstr      - [Input]  Destination block stride
   \param src       - [Input]  Source block pointer
   \param sstr      - [Input]  Source block stride
   \param bw        - [Input]  Block width
   \param bh        - [Input]  Block height
   \param x_offset  - [Input]  X offset from source pixel
   \param y_offset  - [Input]  Y offset from source pixel
 */
void
apu_interp_bicubic_grayscale(
            vec08u* dst, int dstr,
      const vec08u* src, int sstr,
            int bw, int bh,
            uint8_t x_offset, uint8_t y_offset);

#endif /* APUINTERPOLATIONIMPL_H */
#endif /* ACF_KERNEL_IMPLEMENTATION */


/*! @} */
