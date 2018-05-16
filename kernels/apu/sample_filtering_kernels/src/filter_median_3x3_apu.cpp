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
* \file filter_median_3x3_apu.cpp
* \brief 3x3 Median filter implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/
#ifdef ACF_KERNEL_IMPLEMENTATION

#include "filter_median_3x3_apu.h"


#ifdef APEX2_EMULATE
#include "apu_vec.hpp"
#endif

// Do we need this?
// #include "apu_lib.h"

/*#define SWAP(_A,_B)           \
vif (v##_A < v##_B)        \
{                          \
vt    = v##_A;          \
v##_A = v##_B;          \
v##_B = vt;             \
}                          \
vendif
*/

#define SWAP(_A,_B) vswap((v##_A),(v##_B),(v##_A)<(v##_B))

/*!
Calculate a 3x3 median filter.

\param dst  - [Output] Destination block pointer
\param dstr - [Input]  Destination block stride
\param src  - [Input]  Source block pointer
\param sstr - [Input]  Source block stride
\param bw   - [Input]  Block width
\param bh   - [Input]  Block height
*/
void
apu_flt_median_3x3(
vec08u* dst, int dstr,
const vec08u* src, int sstr,
int bw, int bh)
{
	const vec08u* s0 = src - sstr;
	const vec08u* s1 = src;
	const vec08u* s2 = src + sstr;

	//vec08u vt, v0, v1, v2, v3, v4, v5, v6, v7, v8;
	vec16s vt, v0, v1, v2, v3, v4, v5, v6, v7, v8;
	for (int y = 0; y<bh; ++y)
	{
		for (int x = 0; x<bw; ++x) chess_loop_range(2, )
		{
			// 
			v0 = (vec16s)s0[+x - 1];
			v1 = (vec16s)s0[+x];
			v3 = (vec16s)s0[+x + 1];
			v4 = (vec16s)s1[+x - 1];

			SWAP(0, 1);       //  0

			v6 = (vec16s)s1[+x];
			v7 = (vec16s)s1[+x + 1];
			SWAP(3, 4);       //  1

			v2 = (vec16s)s2[+x - 1];
			SWAP(6, 7);       //  2

			v5 = (vec16s)s2[+x];
			v8 = (vec16s)s2[+x + 1];

			SWAP(1, 2);       //  3  0

			SWAP(4, 5);       //  4  1

			SWAP(7, 8);       //  5  2

			SWAP(0, 1);       //  6  3
			SWAP(3, 4);       //  7  4
			SWAP(6, 7);       //  8  5
			SWAP(0, 3);       //  9  7

			// SWAP(3, 6);       // 10  9 ***

			// SWAP(0, 3);

			SWAP(1, 4);       // 11  7

			SWAP(3, 6);       // 10  9 ***

			SWAP(4, 7);       // 12 11
			// SWAP(1, 4);       // 13 12 ***
			SWAP(2, 5);       // 14  4

			SWAP(1, 4);       // 13 12 ***

			// SWAP(2, 5);       // 14  4
			SWAP(5, 8);       // 15 14
			SWAP(2, 5);       // 16 15 ***
			// SWAP(1, 3);
			// SWAP(5, 7);
			SWAP(2, 6);       // 17 16 
			SWAP(4, 6);       // 18 17 
			SWAP(2, 4);       // 19 18 ***

			// SWAP(2, 3);
			// SWAP(5, 6);

			dst[x] = (vec08u)v4;
		}

		// src += sstr;
		s0 = s1;
		s1 = s2;
		s2 += sstr;
		dst += dstr;
	}
}



#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
