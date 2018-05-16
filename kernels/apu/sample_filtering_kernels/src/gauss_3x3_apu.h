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
* \file gauss_3x3_apu.h
* \brief 3x3 Gaussian filter implementation for APEX
* \addtogroup gauss
* @{
****************************************************************************/

#ifndef APUGAUSS3x3IMPL_H
#define APUGAUSS3x3IMPL_H

#ifdef APEX2_EMULATE 
#include <apu_lib.hpp> // if using the APU emulation library
using namespace APEX2;
#endif

/*****************************************************************************
* prototypes
*****************************************************************************/

/****************************************************************************/
/** Calculate the output of 3x3 gaussian filter.
* \brief 3x3 gaussian filter.
*
* \param apOut		- [Output] pointer to the destination buffer
* \param apcIn		- [Input]  pointer to the source buffer
* \param aOutStride - [Input]  line stride of the destination data
* \param aInStride	- [Input]  line stride of the source data
* \param aTileWidth	 - [Input]  width of one data block
* \param aTileHeight - [Input]  height of one data block
*
* \return number of corners found
*
*****************************************************************************/
void apu_gauss_3x3(vec08u* apOut, const vec08u* apcIn,
 int aOutStride, int aInStride,
 int aTileWidth, int aTileHeight);

#endif /* APUGAUSS3x3IMPL_H */

/*! @} */
