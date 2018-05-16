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
* \file filtering_sobel_3x3_apu.h
* \brief 3x3 Sobel filter implementation for APEX
* \addtogroup sobel
* @{
****************************************************************************/

#ifndef  APUFILTERINGSOBEL3X3IMPL_H
#define  APUFILTERINGSOBEL3X3IMPL_H


 
#ifdef APEX2_EMULATE
#include "apu_config.hpp"
#include "apu_vec.hpp"
using namespace APEX2;
#endif

/*!
\brief Calculate first order derivative x using sobel 3x3.

\param dst  - [Output] Destination block pointer
\param dstr - [Input]  Destination block stride
\param src  - [Input]  Source block pointer
\param sstr - [Input]  Source block stride
\param bw   - [Input]  Block width
\param bh   - [Input]  Block height
*/
void apu_flt_sobel_3x3_x(vec08u* dst, int dstr, const vec08u* src, int sstr, int bw, int bh);


/*!
\brief Calculate first order derivative y using sobel 3x3.

\param dst  - [Output] Destination block pointer
\param dstr - [Input]  Destination block stride
\param src  - [Input]  Source block pointer
\param sstr - [Input]  Source block stride
\param bw   - [Input]  Block width
\param bh   - [Input]  Block height
*/
void apu_flt_sobel_3x3_y(vec08u* dst, int dstr,const vec08u* src, int sstr,int bw, int bh);

/*!
\brief Calculate sum of absolute values of first order derivatives x and y using sobel 3x3.

\param dst  - [Output] Destination block pointer
\param dstr - [Input]  Destination block stride
\param src  - [Input]  Source block pointer
\param sstr - [Input]  Source block stride
\param bw   - [Input]  Block width
\param bh   - [Input]  Block height
*/
void apu_flt_sobel_3x3(vec08u* dst, int dstr, const vec08u* src, int sstr, int bw, int bh);
#endif /* APUFILTERINGSOBEL3X3IMPL_H */


/*! @} */

