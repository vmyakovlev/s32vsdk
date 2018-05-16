/**************************************************************************************************
 * 
 * NXP Confidential Proprietary
 *
 * (C) Copyright CogniVue Corporation. 2015 All right reserved.
 * Copyright 2017 NXP 
 * All Rights Reserved
 *
 *****************************************************************************
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
 **************************************************************************************************/

#include "Kernels_Configuration.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef ACF_KERNEL_METADATA

KERNEL_INFO _CgvLdw2_grayscale_threshold
(
   "CgvLdw2_grayscale_threshold",
   3,
   __port(__index(0),
          __identifier("DST"),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier("SRC"),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(2),
		  __identifier("THRESH"),
		  __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
		  __spatial_dep(0,0,0,0),
		  __e0_data_type(d08u),
		  __e0_size(1, 1),
		  __ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION



void CgvLdw2_grayscale_threshold(
		kernel_io_desc dst_param,
		kernel_io_desc src_param,
		kernel_io_desc threshold_param)
{
	// From Cognivue kernel library
	vec08u* dst    = (vec08u*)dst_param.pMem;
	vec08u* src    = (vec08u*)src_param.pMem;
	int08u* threshold = (int08u*)threshold_param.pMem;
	int sstr     = src_param.chunkSpan / sizeof(vec08u);  // stride of the src array in cmem
	int dstr     = dst_param.chunkSpan / sizeof(vec08u);  // stride of the src array in cmem

	int bw         = src_param.chunkWidth;
	int bh         = src_param.chunkHeight;
	vbool c;

	for ( int y=0; y<bh; ++y ) chess_loop_range(1,)
	   {
	      for ( int x=0; x<bw; ++x ) chess_loop_range(1,)
	      {
	    	  dst[x] = (vec08u)vsat((vec16u)src[x], (vec16u) threshold[0], (vec16u)MAX_LUMENS);
	      }

	      src += sstr;
	      dst += dstr;
	   }
}

#endif
