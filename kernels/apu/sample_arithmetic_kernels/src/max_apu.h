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
* \file max_apu.h 
* \brief element-wise maximum implementation for APEX
* \addtogroup maximum
* @{
****************************************************************************/

#ifndef APUMAXIMPL_H
#define APUMAXIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

/****************************************************************************/
/*!
   Element-wise maximum. out[i] = max(in0[i], in1[i]) 
   \brief Element-wise maximum

   \param dst        - [Output] Pointer to the destination buffer
   \param srcImage0  - [Input]  Pointer to the first source buffer
   \param srcImage1  - [Input]  Pointer to the second source buffer
   \param bw         - [Input]  Width of one data block
   \param bh         - [Input]  Height of one data block
   \param inStrideW  - [Input]  Line stride of the source data
   \param outStrideW - [Input]  Line stride of the destination data
*****************************************************************************/
void max(vec08u* dst, vec08u* srcImage0, vec08u* srcImage1, int bw, int bh, int inStrideW, int outStrideW);


#endif /* APUMAXIMPL_H */


/*! @} */
