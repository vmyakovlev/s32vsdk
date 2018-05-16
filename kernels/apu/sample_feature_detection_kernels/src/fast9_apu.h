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
* \file fast9_apu.h
* \brief FAST 9 corner detection implementation for APEX
* \addtogroup FAST9
* @{
****************************************************************************/

#ifndef APUFAST9IMPL_H
#define APUFAST9IMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

/****************************************************************************/
/*!
   Finds the corners in the input data using the FAST9 algorithm. Outputs corner scores
   or 0 if not a corner.
   
   For each input pixel a 16-pixel circle centered at the processed pixel is considered. The circle pixels are classified as darker, brighter or similar to the central pixel depending on the provided threshold.
   The central pixel is considered as a corner if and only if there is a contiguous segment of 9 pixels which are all classified as brighter or darker in the circle.
   
   See http://www.edwardrosten.com/work/fast.html
   \brief FAST9 corner detection
 
   \param apcSrc             - [Input]  pointer to the source buffer
   \param apcDst             - [Output] pointer to the destination buffer
   \param aSourceStride      - [Input]  line stride of the source data
   \param aDestinationStride - [Input]  line stride of the destination data
   \param aBlockWidth        - [Input]  width of one data block
   \param aBlockHeight       - [Input]  height of one data block
   \param aThreshold         - [Input]  threshold used for pixel classification
*****************************************************************************/
void apu_fast9_unsuppressed_score(
      const vec08u* apcSrc, vec08u* apDst,
      int   aSourceStride, int aDestinationStride,
      int   aBlockWidth, int aBlockHeight,
      int08u aThreshold);

#endif /* APUFAST9IMPL_H */

/*! @} */
