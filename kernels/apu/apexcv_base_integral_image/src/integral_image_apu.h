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
/*!*********************************************************************************
*  \file apu_integral_image.h
*  \brief APU integral image kernel.
***********************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION
#ifndef APUINTEGRALIMAGEIMPL_H
#define APUINTEGRALIMAGEIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

#include <stdint.h>

/*!
   Intiialize line buffer for temporary data required to compute integral image.

   \param tlst - [Output] Destination block pointer
   \param bw   - [Input]  Block width
 */
void
apu_integral_init(vec32u __cmem* tlst, int bw);

/*!
   Calculate integral images.

   \param dst  - [Output] Destination block pointer
   \param dstr - [Input]  Destination block stride
   \param src  - [Input]  Source block pointer
   \param tlst - [Input]  Temporary line pointer
   \param sstr - [Input]  Source block stride
   \param bw   - [Input]  Block width
   \param bh   - [Input]  Block height
   \param ncu  - [Input]  Number of CUs (32/64)
 */
void
apu_integral_image(vec32u __cmem* dst, int dstr, const vec08u __cmem* src, vec32u __cmem* tlst, int sstr, int bw, int bh, int ncu);

#endif /* APUINTEGRALIMAGEIMPL_H */
#endif /* ACF_KERNEL_IMPLEMENTATION */
