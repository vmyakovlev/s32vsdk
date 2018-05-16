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
* \file downsample_apu.h
* \brief Image downsample implementation for APEX
* \addtogroup downsample
* @{
****************************************************************************/

#ifndef APUDOWNSAMPLEIMPL_H
#define APUDOWNSAMPLEIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

/****************************************************************************/
/*!
   Downsamples the image by two.
   \brief x2 downsampling.
 
   \param apDest          - [Output] pointer to the destination buffer
   \param apcSrc          - [Input]  pointer to the source buffer
   \param aOutBlockWidth  - [Input]  width of one output data tile
   \param aOutBlockHeight - [Input]  height of one output data tile
   \param aInBlockStride  - [Input]  line stride of the source data
   \param aOutBlockStride - [Input]  line stride of the destination data
*****************************************************************************/

void
downsample(vec08u* apDest, const vec08u* apcSrc,
int aOutBlockWidth, int aOutBlockHeight,
int aInBlockStride, int aOutBlockStride);

/****************************************************************************/
/*!
   Downsamples the image by two. 16-bit version
   \brief x2 downsampling, 16-bit.
 
   \param apDest          - [Output] pointer to the destination buffer
   \param apcSrc          - [Input]  pointer to the source buffer
   \param aOutBlockWidth  - [Input]  width of one output data tile
   \param aOutBlockHeight - [Input]  height of one output data tile
   \param aInBlockStride  - [Input]  line stride of the source data
   \param aOutBlockStride - [Input]  line stride of the destination data
*****************************************************************************/

void downsample_16u(vec16u* apDest, const vec16u* apcSrc,
	int aInBlockWidth, int aInBlockHeight,
	int aOutBlockWidth, int aOutBlockHeight,
	int aInBlockStride, int aOutBlockStride);

/****************************************************************************/
/*!
   Downsamples the image by two using Gaussian blur.
   \brief x2 downsampling using Gaussian blur.
 
   \param apDest          - [Output] pointer to the destination buffer
   \param apcSrc          - [Input]  pointer to the source buffer
   \param aOutBlockWidth  - [Input]  width of one output data tile
   \param aOutBlockHeight - [Input]  height of one output data tile
   \param aInBlockStride  - [Input]  line stride of the source data
   \param aOutBlockStride - [Input]  line stride of the destination data
*****************************************************************************/
	
void
downsample_gauss(vec08u* apDest, const vec08u* apcSrc,
int32s aOutBlockWidth, int32s aOutBlockHeight,
int32s aInBlockStride, int32s aOutBlockStride);

#endif /* APUDOWNSAMPLEIMPL_H */

/*! @} */
