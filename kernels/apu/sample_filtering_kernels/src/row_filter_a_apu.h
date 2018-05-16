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
* \file row_filter_a_apu.h
* \brief Row filter implementation for APU2.
* \addtogroup row_filter
* @{
****************************************************************************/

#ifndef ROWFILTERIMPL_H
#define ROWFILTERIMPL_H

#ifdef APEX2_EMULATE
#include "apu_lib.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

/* ! *********************************************************************************
*  Apply a row filter to an image.
***********************************************************************************/
void row_filter_impl(vec08u* dst       /*! Pointer to the destination image.*/,
                int dstStride     /*! Stride of the destination image.*/,
                const vec08u* src /*! Pointer to the source image.*/,
                int srcStride     /*! Stride of the source image.*/,
                int rows          /*! Rows of the source image, not including padding.*/,
                int cols          /*! Columns of the source image, not including padding.*/,
                const unsigned char* filter /*! Pointer to the row filter coefficients.  
                                                The coefficients are unsigned 8-bit fixed point numbers.*/,
                int filterSize    /*! Size of the column filter.*/,
                int filterQ       /*! Number of fractional bits for the filter coefficients.*/);
               
#endif /* ROWFILTERIMPL_H */

/*! @} */
