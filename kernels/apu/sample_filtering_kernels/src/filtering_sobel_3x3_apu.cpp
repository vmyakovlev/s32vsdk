/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
* Copyright (c) 2017 NXP
*  All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/

/**
* \file filtering_sobel_3x3_apu.cpp
* \brief 3x3 Sobel filter implementation for APEX
* \author Anca Dima
* \version
* \date
****************************************************************************/


#ifdef ACF_KERNEL_IMPLEMENTATION

#ifdef APEX2_EMULATE
#include "apu_lib.hpp"
#include "apu_vec.hpp"
using namespace APEX2;
#endif

#include "filtering_sobel_3x3_apu.h"

// Local helper functions
static inline vec16s apu_filter_sobel_3x3_x(const vec08u* src, int sstr);
static inline vec16s apu_filter_sobel_3x3_y(const vec08u* src, int sstr);

/*!
Calculate first order derivative x using sobel 3x3.

\param dst  - [Output] Destination block pointer
\param dstr - [Input]  Destination block stride
\param src  - [Input]  Source block pointer
\param sstr - [Input]  Source block stride
\param bw   - [Input]  Block width
\param bh   - [Input]  Block height
*/
void
apu_flt_sobel_3x3_x(vec08u* dst, int dstr,const vec08u* src, int sstr,int bw, int bh)
{
	for (int y = 0; y<bh; ++y) chess_loop_range(1, )  //this is a HW optimization construct, which is not yet supported by the emulation library
	{
		for (int x = 0; x<bw; ++x) chess_loop_range(1, )
		{
			vec16s tx = apu_filter_sobel_3x3_x(&src[x], sstr);

			// Absolute Value & Saturation
			tx = vsat(tx, vec16s(0), vec16s(255));
			// above function will be implemented in the next Emulation library release
			//tx = vabs(tx);
			/*
			vif(tx > 255) {
				tx = 255;
			} vendif
*/
			dst[x] = (vec08u)tx;
		}

		src += sstr;
		dst += dstr;
	}
}

/*!
Calculate first order derivative y using sobel 3x3.

\param dst  - [Output] Destination block pointer
\param dstr - [Input]  Destination block stride
\param src  - [Input]  Source block pointer
\param sstr - [Input]  Source block stride
\param bw   - [Input]  Block width
\param bh   - [Input]  Block height
*/
void
apu_flt_sobel_3x3_y(vec08u* dst, int dstr,const vec08u* src, int sstr,int bw, int bh)
{
	for (int y = 0; y<bh; ++y) chess_loop_range(1, ) //this is a HW optimization construct, which is not yet supported by the emulation library
	{
		for (int x = 0; x<bw; ++x) chess_loop_range(1, )
		{
			vec16s ty = apu_filter_sobel_3x3_y(&src[x], sstr);

			// Absolute Value & Saturation
			ty = vsat(ty, vec16s(0), vec16s(255));
			// above function will be implemented in the next Emulation library release
//			ty = vabs(ty);
/*			vif(ty > 255) {
				ty = 255;
			} vendif
			*/
			dst[x] = (vec08u)ty;
		}

		src += sstr;
		dst += dstr;
	}
}

/*!
Calculate sum of absolute values of first order derivatives x and y using sobel 3x3.

\param dst  - [Output] Destination block pointer
\param dstr - [Input]  Destination block stride
\param src  - [Input]  Source block pointer
\param sstr - [Input]  Source block stride
\param bw   - [Input]  Block width
\param bh   - [Input]  Block height
*/
void
apu_flt_sobel_3x3(vec08u* dst, int dstr,const vec08u* src, int sstr,int bw, int bh)
{
	for (int y = 0; y<bh; ++y) chess_loop_range(1, ) // this is a HW optimization construct, which is not yet supported by the emulation library
	{
		for (int x = 0; x<bw; ++x) chess_loop_range(1, )
		{
			vec16s tx = apu_filter_sobel_3x3_x(&src[x], sstr);
			tx = (vec16s)vabs(tx);
			vec16s ty = apu_filter_sobel_3x3_y(&src[x], sstr);
			ty = (vec16s)vabs(ty);
			
			vec16u t = (vec16u)(tx + ty);
			vif(t > 255)  { t = 255; } vendif

				dst[x] = (vec08u)t;
		}

		src += sstr;
		dst += dstr;
	}
}

// LOCAL HELPER FUNCTIONS

//  -1,  0,  1,
//  -2,  0,  2,
//  -1,  0,  1,
static
inline
vec16s
apu_filter_sobel_3x3_x(const vec08u* src, int sstr)
{
	vec16s vd = (vec16s)(+src[-sstr + 1]) - (vec16s)(src[-sstr - 1])
		+ (vec16s)(src[+sstr + 1]) - (vec16s)(src[+sstr - 1])
		+ (((vec16s)(src[1]) - (vec16s)(src[-1])) << 1);

	vd = vd >> 3;
	return vd;
}

// -1, -2, -1,
//  0,  0,  0,
// +1, +2, +1,
static
inline
vec16s
apu_filter_sobel_3x3_y(const vec08u* src, int sstr)
{
	vec16s t7 = vec16s(+src[+sstr + 1]) - (vec16s)(src[-sstr - 1]);
	vec16s t9 = vec16s(+src[+sstr - 1]) - (vec16s)(src[-sstr + 1]);
	vec16s t8 = vec16s(+src[+sstr]) - (vec16s)(src[-sstr]);
	vec16s vd = t7 + t9 + (t8<<1);
	vd = vd >> 3;
	return vd;
}

#endif

