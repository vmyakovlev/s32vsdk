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
* \file sat_box_filter_apu.h
* \brief Box filter using SAT implementation for APEX
* \addtogroup sat_box_filter
* @{
****************************************************************************/

#ifndef APUSATBOXFILTERIMPL_H
#define APUSATBOXFILTERIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
extern const int BOX_SIZE; // = 5;
extern const int BOX_AREA; //= (2 * BOX_SIZE + 1) * (2 * BOX_SIZE + 1);

#endif

/****************************************************************************/
/** 
  Applies a box filter (== sum over that patch) to the image using its summed area table (integral
   image).
   
   out(i,j) = in(i - box_size - 1, j - box_size - 1) + in(i + box_size, j + box_size) - in(i - box_size - 1, j + box_size) - in(i + box_size, j - box_size - 1)

\brief Sum of values over one patch
\precondition the input image is a SAT image (i.e. integral computed with the sat32() function
\param apDest		- [Output] 8bit destination image containing sum of values over one patch
\param apcSrc		- [Input]  32bit source block pointer
\param aBlockWidth		- [Input]  Block width
\param aBlockHeight		- [Input]  Block height
\param aSourceStride	- [Input]  Source block width (in bytes) including padding
\param aDestinationStride- [Input]  Destination block width (in bytes) including padding

******************************************************************************/
void sat_box_filter_impl(
  vec08u* apDest,
  const vec32u* apcSrc,
  int aBlockWidth,
  int aBlockHeight,
  int aSourceStride,
  int aDestStride
  );


#endif /* APUSATBOXFILTERIMPL_H */

/*! @} */
