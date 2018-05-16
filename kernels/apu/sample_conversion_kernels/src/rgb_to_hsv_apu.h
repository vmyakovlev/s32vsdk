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
* \file rgb_to_hsv_apu.h
* \addtogroup color_conversion
* \brief RGB to HSV transformation implementation for APEX
* @{
****************************************************************************/

#ifndef APURGBTOHSVIMPL_H
#define APURGBTOHSVIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library

#include "apu_config.hpp"
#include "apu_vec.hpp"
using namespace APEX2;
#endif

/****************************************************************************/
/*!
   Transforms RGB images to HSV images. Returns the saturation (=(max(R,G,B)-min(R,G,B))/max(R,G,B)) 
   \brief Transforms RGB to HSV => S

   \param apSat		   - [Output] Pointer to the returned saturation buffer
   \param apcSrc       - [Input]  Pointer to the source buffer (in rgb format, where rgb pixels come one after each other)
   \param aBlockWidth  - [Input]  Width of one data block
   \param aBlockHeight - [Input]  Height of one data block
   \param aOutputSpan  - [Input]  Span of the destination data
   \param aInputSpan   - [Input]  Span of the source data
*****************************************************************************/
void rgb_to_hsv_sat(vec08u* apSat, const vec08u* apcSrc, int aBlockWidth, int aBlockHeight, int aOutputSpan, int aInputSpan);


/****************************************************************************/
/*!
Transforms RGB images to HSV images. Returns the saturation(=(max(R,G,B)-min(R,G,B))/max(R,G,B)), the Hue + 90 deg(such that red lies at 90 degrees)
\brief Transforms RGB to HSV => (Hue,Sat)

\param apHue		- [Output] Pointer to the returned hue buffer (values are shifted by 90 degrees)
\param apSat        - [Output] Pointer to the returned saturation buffer
\param apcSrc       - [Input]  Pointer to the source buffer (in rgb format, where rgb pixels come one after each other)
\param aBlockWidth  - [Input]  Width of one data block
\param aBlockHeight - [Input]  Height of one data block
\param aOutputSpan  - [Input]  Span of the destination data
\param aInputSpan   - [Input]  Span of the source data
*****************************************************************************/
void rgb_to_hsv_hue_sat(vec16u* apHue, vec08u* apSat, const vec08u* apcSrc,
	int aBlockWidth, int aBlockHeight, int aOutputSpan, int aInputSpan);



/****************************************************************************/
/*!
Transforms RGB images to an HSV and a grey image. Returns the saturation(=(max(R,G,B)-min(R,G,B))/max(R,G,B)), the Hue + 90 deg(such that red lies at 90 degrees)

\brief Transforms RGB to HSV,Grey => (Hue,Sat, Grey)

\param apHue		- [Output] Pointer to the returned hue buffer (values are shifted by 90 degrees)
\param apSat        - [Output] Pointer to the returned saturation buffer
\param apGrey        - [Output] Pointer to the returned grey buffer
\param apcSrc       - [Input]  Pointer to the source buffer (in rgb format, where rgb pixels come one after each other)
\param aBlockWidth  - [Input]  Width of one data block
\param aBlockHeight - [Input]  Height of one data block
\param aOutputSpan  - [Input]  Span of the destination data
\param aInputSpan   - [Input]  Span of the source data
*****************************************************************************/
void rgb_to_hsv_hue_sat_grey(vec16u* apHue, vec08u* apSat, vec08u* grey, const vec08u* apcSrc,
	int aBlockWidth, int aBlockHeight, int aOutputSpan, int aInputSpan);


/****************************************************************************/
/*!
Transforms RGB images to HSV images. Returns the saturation (=(max(R,G,B)-min(R,G,B))/max(R,G,B)), the Value(==max(R,G,B)) and the R = (Red -(Green+Blue)/2)
\brief Transforms RGB to HSV => (S,V,Red)

\param apSat        - [Output] Pointer to the returned saturation buffer
\param apVal        - [Output] Pointer to the returned value buffer
\param apRed        - [Output] Pointer to the returned red image channel buffer
\param apcSrc       - [Input]  Pointer to the source buffer (in rgb format, where rgb pixels come one after each other)
\param aBlockWidth  - [Input]  Width of one data block
\param aBlockHeight - [Input]  Height of one data block
\param aOutputSpan  - [Input]  Span of the destination data
\param aInputSpan   - [Input]  Span of the source data
*****************************************************************************/
void rgb_to_hsv_svr(vec08u* apSat, vec08u* apVal, vec08u* apRed, const vec08u* apcSrc,
	int aBlockWidth, int aBlockHeight, int aOutputSpan, int aInputSpan);


#endif /* APURGBTOHSVIMPL_H */

/*! @} */
