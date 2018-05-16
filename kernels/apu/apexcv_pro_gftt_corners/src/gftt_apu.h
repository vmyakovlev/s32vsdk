/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
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
#ifndef APUGFTTWRAPPER_H
#define APUGFTTWRAPPER_H

#include "apexcv_pro_gftt_harris_common.h"

#ifdef ACF_KERNEL_IMPLEMENTATION


#include <stdint.h>


/*!
   Gftt_wrapper
*/
void gftt_wrapper(
            vec16s* dst, int dstr,
            vec08u* src, int sstr,
            int bw, int bh,
            vec16s* svxx, vec16s* svxy, vec16s* svyy,
            vec16s* nms_x, vec16s* nms,
            vec32s* bxx_y, vec32s* bxy_y, vec32s* byy_y,
            int flt_x, int flt_y, int radius,
            int16_t  lTotalNumLines,
            bool first_tile,
            vec16s* maxEigen,
            int8_t covariance_shift, 
            int8_t box_shift,
			vec16s mark);
/*!
   Extract corner from CMEM and merge to global corners list
   Similar to extract_corner from harris but slightly modified for gftt
*/
void extract_corners_gftt(int32s* coord, int16_t* stren, int32s* total_cnt,
                     vec32s* dst0, vec16s* dst1,
                     vec16s* src,  int sstr,
                     vec08u chunkId,
                     int start_cu, int end_cu,
                     int bw, int bh, vec16s max_eigen, int quality_level);
/*!
   Sort corners
   Filter out corners that does not meet minimum distance
*/
void sort_and_filter_corners(
      int32s* feature_list,
      int32s& feat_cnt, 
      int32s* coord_list, 
      int16_t* stren,
      int quality_level, int maxCorners, 
      short* index_buff, int total_cnt);
#endif /* APUGFTTWRAPPER_H */
#endif /* ACF_KERNEL_IMPLEMENTATION */
