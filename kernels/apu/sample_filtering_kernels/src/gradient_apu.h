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
* \file gradient_apu.h
* \brief image gradient implementation for APEX
* \addtogroup gradient
* @{
****************************************************************************/

#ifndef APUGRADIENTIMPL_H
#define APUGRADIENTIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

/****************************************************************************/
/*!
   Computes image gradient (horizontal and vertical Sobel filter). Returns Gradient X/Y 
   \brief Image gradient. ==> GradX, GradY

   \param apcSobelX    - [Output] Horizontal Sobel Filter destination block pointer (signed 16bit)
   \param apcSobelY    - [Output] Vertical Sobel Filter destination block pointer   (signed 16bit)
   \param apInput      - [Input]  Source block pointer (unsigned 8bit)
   \param aBlockWidth  - [Input]  Block width
   \param aBlockHeight - [Input]  Block height
   \param aStride      - [Input]  Source block stride
*****************************************************************************/
void apuGradient(vec16s* apcSobelX, vec16s* apcSobelY, const vec08u* apInput,
	int aBlockWidth, int aBlockHeight, int aStride);

/****************************************************************************/
/*!
Computes image gradient (horizontal and vertical Sobel filter). Returns Gradient X/Y
\brief Image gradient. ==> GradX, GradY

\param apcSobelX    - [Output] Horizontal Sobel Filter destination block pointer (signed 08bit)
\param apcSobelY    - [Output] Vertical Sobel Filter destination block pointer   (signed 08bit)
\param apInput      - [Input]  Source block pointer (unsigned 8bit)
\param aBlockWidth  - [Input]  Block width
\param aBlockHeight - [Input]  Block height
\param aStride      - [Input]  Source block stride
*****************************************************************************/
void apuGradient_out08s(vec08s* apcSobelX, vec08s* apcSobelY, const vec08u* apInput,
	int aBlockWidth, int aBlockHeight, int aStride);

/****************************************************************************/
/*!
Computes image gradient (horizontal and vertical Sobel filter). Returns Gradient X/Y and sum of their absolute values
\brief Image gradient ==> GradX, GradY, |GradX|+|GradY|

\param apcSobelX    - [Output] Horizontal Sobel Filter destination block pointer (signed 8bit)
\param apcSobelY    - [Output] Vertical Sobel Filter destination block pointer   (signed 8bit)
\param apcSobelY    - [Output] Sum of absolute values of GradX/Y destination block pointer   (unsigned 16bit)
\param apInput      - [Input]  Source block pointer (unsigned 8bit)
\param aBlockWidth  - [Input]  Block width
\param aBlockHeight - [Input]  Block height
\param aStride      - [Input]  Source block stride
*****************************************************************************/
void apuGradientAbs(vec08s* apcSobelX, vec08s* apcSobelY, vec08u* apcAbsSum, const vec08u* apInput,
	int aBlockWidth, int aBlockHeight, int aStride);


/****************************************************************************/
/*!
Computes image gradient (horizontal and vertical Sobel filter). Returns sum of their absolute values
\brief Image gradient absulute sum ==> |GradX|+|GradY|
\param apcAbsSum    - [Output] Sum of absolute values of GradX/Y destination block pointer   (unsigned 16bit)
\param apInput      - [Input]  Source block pointer (unsigned 8bit)
\param aBlockWidth  - [Input]  Block width
\param aBlockHeight - [Input]  Block height
\param aStride      - [Input]  Source block stride
*****************************************************************************/
void apuGradAbs(vec08u* apcAbsSum, const vec08u* apInput,
	int aBlockWidth, int aBlockHeight, int aStride);

#endif /* APUGRADIENTIMPL_H */

/*! @} */
