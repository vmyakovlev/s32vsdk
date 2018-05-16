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
* \file sat_apu.h
* \brief Summed area table implementation for APEX
* \addtogroup sat
* @{
****************************************************************************/

#ifndef APUSATIMPL_H
#define APUSATIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

/*****************************************************************************
* prototypes
*****************************************************************************/

/****************************************************************************/
/*!
   Computes summed area table (integral image).
   
   out(i,j) = sum(in(k,l)) where k,l := (0,0) to (i,j) inclusive
   \brief Summed area table.
 
\param apDest			- [Output] 32bit the integral image of the input
\param apPrevRow		- [Output] 32bit the last integral row in the tile.
\param apcSrc			- [Input]  8bit source block pointer 
\param aSourceStride	- [Input]  Source block width (in bytes) including padding
\param aDestinationStride- [Input] Destination block width (in bytes) including padding
\param aBlockWidth		- [Input]  Width of one data tile
\param aBlockHeight		- [Input]  Height  of one data tile
\param aFirstTile		- [Input]Boolean. True, if the first tile is computed.
*/
void sat32( vec32u* apDest,  vec32u* apPrevRow,  const vec08u* apcSrc,
  int aSourceStride,  int aDestinationStride,
  int aBlockWidth,  int aBlockHeight,
  int08u aFirstTile
  );


#endif /* APUSATIMPL_H */

/*! @} */
