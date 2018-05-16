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
* \file sad_apu.h
* \brief SAD - Sum of absolute Differences implementation for APEX
* \addtogroup SAD
* @{
****************************************************************************/

#ifndef APUSADIMPL_H
#define APUSADIMPL_H

#include <stdint.h>


#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

/****************************************************************************/
/*!
\brief Sum of absolute differences. Store the minimum of all differences and the location of the minimum in image0

\param lpvIn0             - [Input]  pointer to the first source buffer
\param lStrideIn0		  - [Input]  line stride of the first source buffer
\param lChunkWidthIn0     - [Input]  width of one data block of the first source buffer
\param lChunkHeightIn0    - [Input]  height of one data block of the first source buffer
\param lpvIn1             - [Input]  pointer to the second source buffer
\param lStrideIn1		  - [Input]  line stride of the second source buffer
\param lChunkWidthIn1     - [Input]  width of one data block of the second source buffer
\param lChunkHeightIn1    - [Input]  height of one data block of the second source buffer
\
\param lpvOut0             - [Output] pointer to the destination buffer
\param lStrideOut0		   - [Input]  line stride of the destination data
\param lChunkWidthOut0     - [Input]  width of one data block of the destination buffer
\param lChunkHeightOut0    - [Input]  height of one data block of the destination buffer
*****************************************************************************/
void apu_sad_impl (vec08u* lpvIn0, int16_t lStrideIn0, int16_t lChunkWidthIn0, int16_t lChunkHeightIn0,
              vec08u* lpvIn1, int16_t lStrideIn1, int16_t lChunkWidthIn1, int16_t lChunkHeightIn1,
              vec32u* lpvOut0, int16_t lStrideOut0, int16_t lChunkWidthOut0, int16_t lChunkHeightOut0);


#endif /* APUSADIMPL_H */

/*! @} */
