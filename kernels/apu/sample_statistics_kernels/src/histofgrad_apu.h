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
* \file histofgrad_apu.h
* \brief histogram of gradients computation for APEX
* \addtogroup histogram of gradients
* @{
****************************************************************************/

#ifndef APUHISTOFGRADIMPL_H
#define APUHISTOFGRADIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

/*!
Histogram computation of an input image
\brief Elementwise unsigned 8bit addition => unsigned 16bit 
\param lpvInGradX 	- [Input]  signed integral image of gradient X 
\param lpvInGradY 	- [Input]  signed integral image of gradient Y
\param lpvOut	 	- [Output] histogram of gradients of 4x4 blocks on 2 different scales stored on 4 32bit words
\param isFirstTile	- [Input]  boolean: true if the algorithm is at its first tile
\param lStrideIn  	- [Input]  Source block width (in elements) including padding
\param chunkWidth	- [Input]  Block width
\param chunkHeight	- [Input]  Block height
\param lStrideOut  	- [Input]  Destination block width (in elements) including padding
*/
void hog(vec08s* lpvInGradX, vec08s* lpvInGradY, vec16u* lpvOut, vec32u* lpvOutBlNorm, int lStrideIn, int chunkWidth, int chunkHeight, int lStrideOut);


#endif /* APUHISTOFGRADIMPL_H */

/*! @} */
