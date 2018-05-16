/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* (C) Copyright CogniVue Corporation. 2013 All right reserved.
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
* \file resize_vertical_apu.h
* \brief Image vertical resizing implementation for APEX
* \addtogroup resize_vertical
* @{
****************************************************************************/

#ifndef APURESIZEVERTICALIMPL_H
#define APURESIZEVERTICALIMPL_H

#include "resize_definitions.h"

/*! Only for debug on simulator. */
#ifdef __APU_DEBUG__ 
#include "apu_acf_debug_utils.h"
#endif // __APU_DEBUG__ 


#ifdef APEX2_EMULATE
#include "apu_vec.hpp"
using namespace APEX2;
#endif

/****************************************************************************/
/*!
Offset and upscale all values, bounding them to int16 value limits
\brief Offset and upscale: dst[i] = (int16s) ((src[i] + ioff) << iscl)

\param pdst	- [Output] pointer to the destination buffer (signed 16bit)
\param psrc - [Input]  pointer to the source buffer (signed 16bit)
\param bh	- [Input]  height of one input data tile(signed 16bit)
\param bw   - [Input]  width of one input data tile (signed 16bit)
\param ioff	- [Input]  one offset value for all the data (signed 16bit)
\param iscl	- [Input]  one scale value for shifting up all the data (signed 16bit)
*****************************************************************************/
void apu_input_format(vec16s* pdst, const vec16s* psrc, 
                      int16s bh, int16s bw, int16s ioff, int16s iscl);


/****************************************************************************/
/*!
Offset and upscale all values, bounding them to int16 value limits
\brief Offset and upscale: dst[i] = (int16s) ((src[i] + ioff) << iscl)

\param pdst - [Output] pointer to the destination buffer (unsigned 16bit)
\param psrc - [Input]  pointer to the source buffer (unsigned 8bit)
\param bh	- [Input]  height of one input data tile (signed 16bit)
\param bw   - [Input]  width of one input data tile (signed 16bit)
\param ioff - [Input]  one offset value for all the data (signed 16bit)
\param iscl - [Input]  one scale value for shifting up all the data (signed 16bit)
*****************************************************************************/
void apu_input_format(vec16s* pdst, const vec08u* psrc, 
                      int16s bh, int16s bw, int16s ioff, int16s iscl);

/****************************************************************************/
/*!
Offset and downscale all values, bounding them to int16 value limits
\brief Offset and downscale: dst[i] = (int16s) ((src[i] + ooff) >> oscl)

\param pdst	- [Output] pointer to the destination buffer (signed 16bit)
\param psrc	- [Input]  pointer to the source buffer (signed 16bit)
\param bh	- [Input]  height of one input data tile (signed 16bit)
\param bw	- [Input]  width of one input data tile (signed 16bit)
\param ooff	- [Input]  one offset value for all the  data (signed 16bit)
\param oscl - [Input]  one scale value for shifting down all the data (signed 16bit)
*****************************************************************************/
void apu_output_format(vec16s* pdst, const vec16s* psrc, 
                      int16s bh, int16s bw, int16s ooff, int16s oscl);


/****************************************************************************/
/*!
Offset and downscale all values, bounding them to unsigned int08 value limits
\brief Offset and downscale: dst[i] = (int08u) ((src[i] + ooff) >> oscl)

\param pdst	- [Output] pointer to the destination buffer (unsigned 8bit)
\param psrc	- [Input]  pointer to the source buffer (signed 16bit)
\param bh	- [Input]  height of one input data tile (signed 16bit)
\param bw	- [Input]  width of one input data tile (signed 16bit)
\param ooff	- [Input]  one offset value for all the data (signed 16bit)
\param oscl - [Input]  one scaling value for shifting down all the data (signed 16bit)
*****************************************************************************/
void apu_output_format(vec08u* pdst, const vec16s* psrc, 
                      int16s bh, int16s bw, int16s ooff, int16s oscl);                      
// Padding
//void apu_pad_vertical(vec16s* psrc, int16s bh, int16s bw, int16s pmod,    
  //           int psiz, int16s pval);  


/*!
Computes a vertical average over asiz, then adds an offset and downscales the value, bounding them to signed int16 value limits
\brief Average, offset and downscale: dst[i] = (int16s) ( (sum_k(src[asiz*i+k]) + arnd) >> ascl); where k = [0:asiz)

\param pdst	- [Output] pointer to the destination buffer (signed 16bit)
\param psrc	- [Input]  pointer to the source buffer (signed 16bit)
\param bh	- [Input]  height of one input data tile (signed 16bit)
\param bw	- [Input]  width of one input data tile (signed 16bit)
\param asiz	- [Input]  vertical average height (signed 16bit)
\param ascl - [Input]  one scaling value for shifting down all the data (signed 16bit)
\param arnd - [Input]  one offset value for all the data (signed 16bit)
*****************************************************************************/
void apu_average(vec16s* pdst, const vec16s* psrc,
                 int16s bh, int16s bw, int16s asiz, int16s ascl, int16s arnd);

/*!
Filters vertically, then adds an offset and downscales the average, bounding them to signed int16 value limits
\brief Average, offset and downscale: dst[i] = (int16s) ( (sum_k(src[i+k]*flt[k]) + frnd) >> fscl); where k = [0:fsiz)

\param pdst	- [Output] pointer to the destination buffer (signed 16bit)
\param psrc	- [Input]  pointer to the source buffer (signed 16bit)
\param bh	- [Input]  height of one input data tile (signed 16bit)
\param bw	- [Input]  width of one input data tile (signed 16bit)
\param flt	- [Input]  filter coefficients
\param fsiz	- [Input]  number of filter codefficients(signed 16bit)
\param fscl - [Input]  one scaling value for shifting down all the data (signed 16bit)
\param frnd - [Input]  one offset value for all the data (signed 16bit)
*****************************************************************************/
void apu_filter_generic(vec16s* pdst, const vec16s* psrc,
                        int16s bh, int16s bw, const int16s* flt, 
                        int16s fsiz, int16s fscl, int16s frnd);
/* !
Filters vertically, then adds an offset and downscales the average, bounding them to signed int16 value limits
\ brief Average, offset and downscale: dst[i] = (int16s) ( (sum_k(src[i+k]*flt[k]) + frnd) >> fscl); where k = [0:fsiz)

\ param pdst	 - [Output] pointer to the destination buffer (unsigned 8bit)
\ param psrc	 - [Input]  pointer to the source buffer (signed 16bit)
\ param rsz_desc - [Input]  resize descriptor
*****************************************************************************/
#if 0
void apu_resize_polyphase(vec16s* pdst, const vec16s* psrc, RESIZE_DESCRIPTOR* rsz_desc); 
#endif

/*!
Filters vertically with one filter of the polyphase filter bank, then adds an offset (rounding) and downscales the average, bounding them to unsigned int08 value limits
\brief One phase filtering, rounding and downscale: dst[i] = (int16s) ( (sum_k(src[i+k]*flt[k]) + frnd) >> fscl); where k = [0:taps)

\param pdst		- [Output] pointer to the destination buffer (unsigned 8bit)
\param psrc		- [Input]  pointer to the source buffer (signed 8bit)
\param sh		- [Input]  nr of lines increment in the source data.
\param rsz_desc - [Input]  resize descriptor
*****************************************************************************/
void apu_resize_polyphase(vec08u* pdst, const vec08u* psrc, int32s sh, RESIZE_DESCRIPTOR* rsz_desc);


/*! 
Filters vertically with a filter of the polyphase filter bank, then adds an offset (rounding) and downscales the average, bounding them to unsigned int08 value limits
\brief Average, offset and downscale: dst[i] = (int16s) ( (sum_k(src[i+k]*flt[k]) + frnd) >> fscl); where k = [0:fsiz)

\param pdst		- [Output] pointer to the destination buffer (unsigned 8bit)
\param psrc		- [Input]  pointer to the source buffer (unsigned 8bit)
\param rsz_desc - [Input]  resize descriptor (contains chunk sizes, filter sizes/rounding/scaling and filter bank coefficients and the specification of the phase
*****************************************************************************/
void apu_resize_integer(vec08u* pdst, const vec08u* psrc, RESIZE_DESCRIPTOR* rsz_desc); 

/// Used to populate RESIZE_DESCRIPTOR prior to a kernel call

/*!
Used to populate RESIZE_DESCRIPTOR prior to a kernel call
\brief Populates RESIZE_DESCRIPTOR 

\param ssiz		- [Input]	size of the source buffer (signed 16bit)
\param dsiz		- [Input]	size of the destination buffer (signed 16bit)
\param rsz_desc - [Output]  resize descriptor (contains chunk sizes, filter sizes/rounding/scaling and filter bank coefficients and the specification of the phase
*****************************************************************************/
int get_polyphase_params(int16s ssiz, int16s dsiz, RESIZE_DESCRIPTOR* rsz_desc);





#endif /* APURESIZEVERTICALIMPL_H */

/*! @} */
