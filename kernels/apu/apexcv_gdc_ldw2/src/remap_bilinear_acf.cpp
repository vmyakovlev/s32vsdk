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

#include "cg_kernel.h"
#include "Kernels_Configuration.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef ACF_KERNEL_METADATA

KERNEL_INFO _CgvLdw2_remap_bilinear_greyscale
(
   "CgvLdw2_remap_bilinear_grayscale",
   4,
   __port(__index(0),
          __identifier("DST"),
          __attributes(ACF_ATTR_OUT),
          __spatial_dep(0,0,0,0),      // bot and right dependencies are contained in data, not ACF
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
          __identifier("SRC"),
          __attributes(ACF_ATTR_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),            // source block size - simple test case
          
   // todo currently does not match 32 bit size in code, but I think 16 bit is correct
   __port(__index(2),
          __identifier("OFFSET"),
          __attributes(ACF_ATTR_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(3),
          __identifier("DELTA"),
          __attributes(ACF_ATTR_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(2, 1),
          __ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION



void CgvLdw2_remap_bilinear_grayscale(
   kernel_io_desc dst_param,
   kernel_io_desc src_param,
   kernel_io_desc offset_param, 
   kernel_io_desc delta_param)
{
	// From Cognivue kernel library
   vec08u* dst    = (vec08u*)dst_param.pMem;
   vec08u* src    = (vec08u*)src_param.pMem;
   vec16u* offset = (vec16u*)offset_param.pMem;
   vec08u* delta  = (vec08u*)delta_param.pMem;
   int sstride     = src_param.chunkSpan / sizeof(vec08u);  // stride of the src array in cmem
   int dstride     = dst_param.chunkSpan / sizeof(vec08u);  // stride of the src array in cmem
   
   int bw         = dst_param.chunkWidth;
   int bh         = dst_param.chunkHeight;

   remap_bilinear_grayscale(
      dst,
      src,
      offset,
      delta,
      sstride,
      dstride,
      bw,
      bh);
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
