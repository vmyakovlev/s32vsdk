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
* \file mark_color_channel_apu.h
* \brief color channel image marking implementation for APEX
* \addtogroup marking
* @{
****************************************************************************/

#ifndef APUMARKCOLORCHANNELIMPL_H
#define APUMARKCOLORCHANNELIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

/****************************************************************************/
/*!
   Marks the image. Output pixels are copied from the source image
   in positions where the marker pixels are zero and from the marker image
   otherwise.
   \brief Marks a color channel of the image.

   \param dst            - [Output] Pointer to the destination buffer
   \param srcImage       - [Input]  Pointer to the source buffer
   \param srcMarker      - [Input]  Pointer to the marker buffer
   \param bw             - [Input]  Width of one data block
   \param bh             - [Input]  Height of one data block
   \param channel        - [Input]  Index of the color channel to mark (0, 1 or 2)
   \param inStride       - [Input]  Input stride
   \param inMarkerStride - [Input]  Marker stride
   \param outStride      - [Input]  Output stride
*****************************************************************************/
void mark_color_channel(vec08u* dst, vec08u* srcImage, vec08u* srcMarker, int bw, int bh, int08u channel, int inStride, int inMarkerStride, int outStride);

#endif /* APUMARKCOLORCHANNELIMPL_H */

/*! @} */
