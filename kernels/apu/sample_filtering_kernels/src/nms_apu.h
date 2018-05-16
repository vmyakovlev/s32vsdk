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
* \file nms_apu.h
* \brief non-maximum suppression implementation for APEX
* \addtogroup nms
* @{
****************************************************************************/

#ifndef APUNMSIMPL_H
#define APUNMSIMPL_H

#include <stdint.h>

#ifdef APEX2_EMULATE 
#include <apu_lib.hpp> // if using the APU emulation library
using namespace APEX2;
#endif

/*****************************************************************************
* prototypes
*****************************************************************************/

/****************************************************************************/
/*!
   Sets values which are not maximal in their 3x3 neighborhood (8 pixels) to 0. 8-bit version
   \brief Non-maximum suppression.

   \param apcIn       - [Input]  Pointer to the source buffer
   \param apOut       - [Output] Pointer to the destination buffer
   \param aInStride   - [Input]  Line stride of the source data
   \param aOutStride  - [Input]  Line stride of the destination data
   \param aTileWidth  - [Input]  Width of one data block
   \param aTileHeight - [Input]  Height of one data block
*****************************************************************************/
void apu_nms_impl(const vec08u* apcIn, vec08u* apOut,
 int aInStride, int aOutStride,
 int aTileWidth, int aTileHeight);
 
 
 
/****************************************************************************/
/*!
   Sets values which are not maximal in their 3x3 neighborhood (8 pixels) to 0. 16-bit version.
   \brief Non-maximum suppression, 16-bit.

   \param apcIn       - [Input]  Pointer to the source buffer
   \param apOut       - [Output] Pointer to the destination buffer
   \param aInStride   - [Input]  Line stride of the source data
   \param aOutStride  - [Input]  Line stride of the destination data
   \param aTileWidth  - [Input]  Width of one data block
   \param aTileHeight - [Input]  Height of one data block
*****************************************************************************/
void apu_nms16(const vec16u* apcIn, vec16u* apOut,
 int aInStride, int aOutStride,
 int aTileWidth, int aTileHeight);

#endif /* APUNMSIMPL_H */

/*! @} */
