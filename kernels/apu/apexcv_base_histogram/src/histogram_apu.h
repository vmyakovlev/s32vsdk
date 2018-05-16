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
*  \file apu_histogram_acf.h
*  \brief APU histogram kernels.
***********************************************************************************/
#ifdef ACF_KERNEL_IMPLEMENTATION

#ifndef APEXCVAPUHISTOGRAMIMPL_H
#define APEXCVAPUHISTOGRAMIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

#include <stdint.h>

/*!
   Initialize Global & Local Histograms (CMEM) - HIST_BIN_NBR bins

   \param l_hist - [Output] Local Histograms (CMEM)
   \param g_hist - [Output] Global Histogram (DMEM)
 */
void  apu_hist_init(vec16u* l_hst, uint32_t* g_hst);

/*!
   Initialize Global & Local Histograms (CMEM)

   \param l_hist - [Output] Local Histograms (CMEM)
   \param g_hist - [Output] Global Histogram (DMEM)
   \param nbr    - [Input]  Number of Bins
 */
 void  apu_hist_init(vec16u* l_hst, uint32_t* g_hst, int nbr);

/*!
   Calculate a local histogram for an image tile.  
   This input image is 8 bits per pixel, 
   and the histogram must have at enough bins to contain maximum source value 
   (256 bins, for [0-255]).

   \param lhist - [Output] 256-bin Local Histogram (CMEM)
   \param src   - [Input]  Source Pointer buffer
   \param sstr  - [Input]  Source Stride
   \param bw    - [Input]  Source Width, in pixels (must be a multiple of 2, 16-bit aligned)
   \param bh    - [Input]  Source Height
 */
void  apu_hist1_l(vec16u __cmem* lhist, const vec08u __cmem* src, int sstr, int bw, int bh);

/*!
   Accumulate in Global Histogram (DMEM) all the Local Histograms (CMEM)

   \param ghist - [Output] 256-bin Global Histogram (DMEM)
   \param lhist - [Input]  256-bin Local  Histogram (CMEM)
   \param bcu   - [Input]  ID of first CU used
   \param ecu   - [Input]  ID of first unused CU (last used + 1)
 */
void  apu_hist1_gn(uint32_t* ghist, const vec16u __cmem* lhist, int bcu, int ecu);

#endif /* APEXCVAPUHISTOGRAMIMPL_H */
#endif /* ACF_KERNEL_IMPLEMENTATION */
