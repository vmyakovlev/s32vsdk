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
/**
* \file rgb_to_grayscale_apu.h
* \addtogroup color_conversion
* \brief RGB to grayscale transformation implementation for APEX
* @{
****************************************************************************/

#ifndef APURGBTOGRAYSCALEIMPL_H
#define APURGBTOGRAYSCALEIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

/****************************************************************************/
/*!
   Transforms RGB images to grayscale images.
   \brief Transforms RGB to grayscale

   \param apDest       - [Output] Pointer to the destination buffer
   \param apcSrc       - [Input]  Pointer to the source buffer
   \param aBlockWidth  - [Input]  Width of one data block
   \param aBlockHeight - [Input]  Height of one data block
   \param aOutputSpan  - [Input]  Span of the destination data
   \param aInputSpan   - [Input]  Span of the source data
*****************************************************************************/
void rgb_to_grayscale(vec08u* apDest, const vec08u* apcSrc,
  int aBlockWidth, int aBlockHeight, int aOutputSpan, int aInputSpan);

#endif /* APURGBTOGRAYSCALEIMPL_H */

/*! @} */
