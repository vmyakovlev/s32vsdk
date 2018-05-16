
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
* \file cvt_16low_to_8_apu.cpp
* \brief 16low_to_8 implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/
#ifdef ACF_KERNEL_IMPLEMENTATION

#include "cvt_16low_to_8_apu.h"

/*****************************************************************************
* functions
*****************************************************************************/
void f16low_to_8(vec08u* dst, vec16u* src, int bw, int bh)
{
	for (int by = 0; by<bh; ++by)  // y data row
	{
		for (int bx = 0; bx<bw; ++bx) // x in the blck_tile
		{
			vec16u r = src[bx];
			vif(r > 255)
			{
				r = 255;
			}
			vendif;

			dst[bx] = (vec08u)r;
		}
		dst += bw;
		src += bw;
	}
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION 
