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
* \file filter_a_apu.h
* \brief filtering with general filter 1D/2D
* \addtogroup general filtering
* @{
****************************************************************************/
#ifndef APUFILTERAIMPL_H
#define APUFILTERAIMPL_H

#include <stdint.h>

#ifdef APEX2_EMULATE
#include "apu_lib.hpp"
using namespace APEX2;
#endif

#include "filter_a_cfg.h"

/*!
Calculate the output of convolution with a general filter.
\brief General filtering function with a 1D/2D-filter

\param src  - [Input]  Source block pointer
\param sstr - [Input]  Source block stride
\param coef - [Input]  The filter coefficients
\param dst  - [Output] Destination block pointer
\param dstr - [Input]  Destination block stride
\param bw   - [Input]  Block width
\param bh   - [Input]  Block height
*/
void apu_filter_a_impl(vec08u* src, int16_t sstr,
                   uint8_t* coef,
                   vec08u* dst, int16_t dstr,
                   int16_t bw, int16_t bh);

#endif /* APUFILTERAIMPL_H */

/*! @} */
