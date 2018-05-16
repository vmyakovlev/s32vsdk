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
* \file upsample_apu.h
* \brief Image upsample implementation for APEX
* \addtogroup upsample
* @{
****************************************************************************/

#ifndef APUUPSAMPLEIMPL_H
#define APUUPSAMPLEIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

/****************************************************************************/
/*!
   Upsamples the image by two.
   \brief x2 upsampling.
 
   \param apDest          - [Output] pointer to the destination buffer
   \param apcSrc          - [Input]  pointer to the source buffer
   \param aInBlockWidth   - [Input]  width of one input data tile
   \param aInBlockHeight  - [Input]  height of one input data tile
   \param aInBlockStride  - [Input]  line stride of the source data
   \param aOutBlockStride - [Input]  line stride of the destination data
*****************************************************************************/

void upsample(vec08u* apDest, const vec08u* apcSrc,
  int aInBlockWidth, int aInBlockHeight,
	int aInBlockStride, int aOutBlockStride);


#endif /* APUUPSAMPLEIMPL_H */

/*! @} */
