/*****************************************************************************
 *
 * NXP Confidential Proprietary
 *
 * Copyright (c) 2016-2017 NXP Semiconductor;
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
 ****************************************************************************/

/*!*********************************************************************************
 *  @file
 *  @brief ACF metadata and wrapper function for the L-K optical flow tracker
 ***********************************************************************************/

#ifdef ACF_KERNEL_METADATA
#include "lktracker_apu.h"

/*!*********************************************************************************
 * *  \brief ACF metadata for 7x7 bilinear interpolation.
 * *  \see UG-10267-03 ACF User Guide, Section 3.2.2
 * ***********************************************************************************/

KERNEL_INFO _kernel_info_lktracker_templ_bilinear_08u_7x7
(
	"lktracker_templ_bilinear_08u_7x7",
	3,
	__port(  __index(0),
	__identifier("INPUT_0"),
	__attributes(ACF_ATTR_VEC_IN_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(12, 10)),
	
	__port(  __index(1),
	__identifier("INPUT_1"),
	__attributes(ACF_ATTR_VEC_IN_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(4, 1),
	__ek_size(1, 1)),
	
	__port(  __index(2),
	__identifier("OUTPUT_0"),
	__attributes(ACF_ATTR_VEC_OUT_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(12, 9))
);

/*!*********************************************************************************
 * *  \brief ACF metadata for 7x7 X gradient using 3x3 centraldx/dy.
 * *  \see UG-10267-03 ACF User Guide, Section 3.2.2
 * ***********************************************************************************/
KERNEL_INFO _kernel_info_lktracker_ht_centraldxdy_7x7
(
	"lktracker_ht_centraldxdy_7x7",
	4,
 	__port(  __index(0),
				__identifier("INPUT_0"),
				__attributes(ACF_ATTR_VEC_IN_FIXED),
				__spatial_dep(0, 0, 0, 0),
				__e0_data_type(d08u),
				__e0_size(1, 1),
				__ek_size(12, 9)),
	__port(  __index(1),
				 __identifier("INPUT_1"),
				 __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
				 __spatial_dep(0, 0, 0, 0),
				 __e0_data_type(d08s),
				 __e0_size(1, 1),
				 __ek_size(12, 1)),
	__port(  __index(2),
				__identifier("OUTPUT_0"),
				__attributes(ACF_ATTR_VEC_OUT_FIXED),
				__spatial_dep(0, 0, 0, 0),
				__e0_data_type(d16s),
				__e0_size(1, 1),
				__ek_size(8, 7)),
	__port(  __index(3),
				__identifier("OUTPUT_1"),
				__attributes(ACF_ATTR_VEC_OUT_FIXED),
				__spatial_dep(0, 0, 0, 0),
				__e0_data_type(d16s),
				__e0_size(1, 1),
				__ek_size(8, 7))
);

/*!*********************************************************************************
 * *  \brief ACF metadata for 7x7 L-K tracker core iteration.
 * *  \see UG-10267-03 ACF User Guide, Section 3.2.2
 * ***********************************************************************************/
KERNEL_INFO _kernel_info_lktracker_core_7x7
(
	"lktracker_core_7x7",
	9,
	__port(  __index(0),
	__identifier("INPUT_0"),
	__attributes(ACF_ATTR_VEC_IN_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(LKTRACKER_PATCH_BW_7x7, LKTRACKER_PATCH_BW_7x7)),

	__port(  __index(1),
	__identifier("INPUT_1"),
	__attributes(ACF_ATTR_VEC_IN_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(12, 9)),

	__port(  __index(2),
	__identifier("INPUT_2"),
	__attributes(ACF_ATTR_VEC_IN_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(8, 7)),

	__port(  __index(3),
	__identifier("INPUT_3"),
	__attributes(ACF_ATTR_VEC_IN_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(8, 7)),

	__port(  __index(4),
	__identifier("INPUT_4"),
	__attributes(ACF_ATTR_VEC_IN_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(4, 1),
	__ek_size(1, 1)),

   __port(  __index(5),
   __identifier("INPUT_5"),
   __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
   __spatial_dep(0, 0, 0, 0),
   __e0_data_type(d08s),
   __e0_size(1, 1),
   __ek_size(12, 1)),

	__port(  __index(6),
	__identifier("OUTPUT_0"),
	__attributes(ACF_ATTR_VEC_OUT_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(4, 1),
	__ek_size(1, 1)),

	__port(  __index(7),
	__identifier("OUTPUT_1"),
	__attributes(ACF_ATTR_VEC_OUT_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(8, 7)),

	__port(  __index(8),
	__identifier("OUTPUT_2"),
	__attributes(ACF_ATTR_VEC_OUT_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d16s),
	__e0_size(1, 1),
	__ek_size(8, 7))
);


#endif /* ACF_KERNEL_METADATA */

#ifdef ACF_KERNEL_IMPLEMENTATION
#include "image_filters_apu.h"
#include "lktracker_apu.h"

/*!*********************************************************************************
 * *  \brief ACF wrapper function for 7x7 bilinear interpolation scaled for lktracker.
 * *  \see UG-10267-03 ACF User Guide, Section 3.2.3
 * ***********************************************************************************/
void lktracker_templ_bilinear_08u_7x7(
   kernel_io_desc lIn0      /*!< The padded source image.*/,
   kernel_io_desc lIn1      /*!< coordinate fractional portion.*/,
   kernel_io_desc lOut0     /*!< The destination image.*/
)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem;
   vec32s* lpvIn1  = (vec32s*)lIn1.pMem;
   vec08u* lpvOut0 = (vec08u*)lOut0.pMem+1;

   apu_lktracker_templ_bilinear_Q8(
         lpvOut0, lOut0.chunkSpan,
         lpvIn0,  lIn0.chunkSpan,
         lpvIn1,
         9, 9,
         LKTRACKER_FLOATING_COOR_Q);
}

/*!*********************************************************************************
 * *  \brief ACF wrapper function for Hand-Tuned 3x3 Central DX/DY Operation (signed 16-bit).
 * ***********************************************************************************/
void lktracker_ht_centraldxdy_7x7(
			kernel_io_desc lIn0, 
			kernel_io_desc lIn1, 
			kernel_io_desc lOut0,
			kernel_io_desc lOut1
)
{
	vec08u* lpvIn0    = (vec08u*)lIn0.pMem + 1*lIn0.chunkSpan+2;
   int8_t* lpIn1     = (int8_t*)lIn1.pMem + 9;
	vec16s* lpvOut0   = (vec16s*)lOut0.pMem;
	vec16s* lpvOut1   = (vec16s*)lOut1.pMem;

	ht::apu_flt_centraldx_3x3(
						lpvOut0, lOut0.chunkSpan>>1,
						lpvIn0,  lIn0.chunkSpan, 
						lOut0.chunkWidth, lOut0.chunkHeight,
						lpIn1[0], 
						lpIn1[1], 
						lpIn1[2]);

	ht::apu_flt_centraldy_3x3(
						lpvOut1, lOut1.chunkSpan>>1,
						lpvIn0,  lIn0.chunkSpan, 
						lOut1.chunkWidth, lOut1.chunkHeight,
						lpIn1[0], 
						lpIn1[1], 
						lpIn1[2]);
}

/*!*********************************************************************************
 * *  \brief ACF wrapper function for 7x7 L-K tracker core iteration.
 * ***********************************************************************************/
void lktracker_core_7x7(
   kernel_io_desc lIn0      /*!< patch.*/,
   kernel_io_desc lIn1      /*!< template.*/,
   kernel_io_desc lIn2      /*!< template x gradient.*/,
   kernel_io_desc lIn3      /*!< template y gradient.*/,
   kernel_io_desc lIn4      /*!< input  coordinate vector.*/,
   kernel_io_desc lIn5      /*!< config parameters. */,
   kernel_io_desc lOut0     /*!< output coordinate vector.*/,
   kernel_io_desc lOut1     /*!< patch moving window.*/,
   kernel_io_desc lOut2     /*!< moving window dt.*/
)
{
   vec08u* lpvIn0  = (vec08u*)lIn0.pMem+LKTRACKER_PATCH_BORDER*lIn0.chunkSpan+LKTRACKER_PATCH_BORDER;
	vec08u* lpvIn1  = (vec08u*)lIn1.pMem+1;
	vec16s* lpvIn2  = (vec16s*)lIn2.pMem;
   vec16s* lpvIn3  = (vec16s*)lIn3.pMem;
   vec32s* lpvIn4  = (vec32s*)lIn4.pMem;
   int8_t* lpIn5   = (int8_t*)lIn5.pMem;
   vec32s* lpvOut0 = (vec32s*)lOut0.pMem;
   vec08u* lpvOut1 = (vec08u*)lOut1.pMem;
   vec16s* lpvOut2 = (vec16s*)lOut2.pMem;

	apu_lktracker_core(
            lpvOut0,
            lpvIn0, lIn0.chunkSpan, 
            lpvIn1, lIn1.chunkSpan, 
            lpvIn2, lIn2.chunkSpan>>1,
            lpvIn3, lIn3.chunkSpan>>1,
            lpvIn4,
            lpvOut1, lOut1.chunkSpan,
            lpvOut2, lOut2.chunkSpan>>1,
            7, 7,
            7+2*LKTRACKER_PATCH_BORDER, 7+2*LKTRACKER_PATCH_BORDER,
            *((int8_t*)(lpIn5+8)),
            *((int32_t*)lpIn5), *((int32_t*)lpIn5+1));
}


#endif /* ACF_KERNEL_IMPLEMENTATION */
