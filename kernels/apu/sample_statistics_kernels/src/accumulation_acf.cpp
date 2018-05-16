/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
* Copyright (c) 2017 NXP
* 
* All Rights Reserved
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

/*!

* \file accumulation_acf.cpp
* \addtogroup statistics
* \addtogroup accumulation
* \ingroup statistics
* @{
* \brief Element value accumulation
*/
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA

#include "accumulation_acf.h"
#define ACCUM_TILE_SIZE_X  10 // otherwise g++ linker complains
#define ACCUM_TILE_SIZE_Y  10


/****************************************************************************/
/*!
Accumulation kernel metadata. Accumulates the pixels in one a certain neighborhood (XWIDTH, YHEIGHT) of signed 32bit image according to the chunk related xoffs and yoffs
Outputs 16bit signed interpolation vector having as many elements as number of chunks . 

\Caution the accumulation is not crossing the chunk limits.

\param  ACCUM_KN			Define for Kernel name
\param 6					Number of ports
\param "Port ACCUM_IN"		Define for name offirst input image (signed 32bit)
\param "Port ACCUM_OUT"		Define for name of interpolation result of the input image (signed 32bit). 
\param "Port ACCUML_XOFFS"	Define for name of signed 16bit x offset vector (has one element for each chunk of the image) (signed 16bit)
\param "Port ACCUM_YOFFS"	Define for name of signed 16bit y offset vector (has one element for each chunk of the image) (signed 16bit)
\param "Port ACCUM_XWIDTH"	Define for name of scalar value defining the accumulation width (has to be defined by the user) (signed 16bit) 
\param "Port ACCUM_YHEIGHT"	Define for name of scalar value defining the accumulation height (has to be defined by the user) (signed 16bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(ACCUM_K) (
   ACCUM_KN,
   6,
   __port(__index(0),
          __identifier(ACCUM_IN),
		  __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0), 
          __e0_data_type(d32s),
          __e0_size(1, 1),
		  __ek_size(ACCUM_TILE_SIZE_X, ACCUM_TILE_SIZE_Y)),

   __port(__index(1),
          __identifier(ACCUM_OUT),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32s),
          __e0_size(1, 1),
          __ek_size(1, 1)),
  __port(__index(2),
		  __identifier(ACCUM_XOFFS),
		  __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
		  __spatial_dep(0, 0, 0, 0),
		  __e0_data_type(d16s),
		  __e0_size(1, 1),
		  __ek_size(1, 1)),
  __port(__index(3),
		  __identifier(ACCUM_YOFFS),
		  __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
		  __spatial_dep(0, 0, 0, 0),
		  __e0_data_type(d16s),
		  __e0_size(1, 1),
		  __ek_size(1, 1)),
	__port(__index(4),
		  __identifier(ACCUM_XWIDTH),
		  __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
		  __spatial_dep(0, 0, 0, 0),
		  __e0_data_type(d16s),
		  __e0_size(1, 1),
		  __ek_size(1, 1)),
	__port(__index(5),
		  __identifier(ACCUM_YHEIGHT),
		  __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
		  __spatial_dep(0, 0, 0, 0),
		  __e0_data_type(d16s),
		  __e0_size(1, 1),
		  __ek_size(1, 1))


);



#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "accumulation_apu.h"

#ifdef APEX2_EMULATE
#include <assert.h>
#endif

void apu_accumulation(kernel_io_desc lImgInA, kernel_io_desc lValOut, kernel_io_desc lxOffs, kernel_io_desc lyOffs, kernel_io_desc lxWidth, kernel_io_desc lyHeight)
{
   vec32s*  lpvImgInA	= (vec32s*) lImgInA.pMem;
   int dataSz = sizeof(int32s); //sizeof(vec32s)

   vec32s* lpvOut = (vec32s*) lValOut.pMem;
   int16s xOffs = *(int16s*)lxOffs.pMem;
   int16s yOffs = *(int16s*)lyOffs.pMem;
   int16s xWidth = *(int16s*)lxWidth.pMem;
   int16s yHeight = *(int16s*)lyHeight.pMem;

#ifdef APEX2_EMULATE
   assert(xWidth <= lImgInA.chunkWidth);
   assert(yHeight <= lImgInA.chunkHeight);
#endif
   
   accumulation_in32s_filter(lpvOut, lpvImgInA, int16s(lImgInA.chunkSpan / dataSz), xOffs, yOffs, xWidth, yHeight);
}



#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
