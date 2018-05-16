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
#ifndef HARRISAPUWRAPPER_H
#define HARRISAPUWRAPPER_H

#include "apexcv_pro_gftt_harris_common.h"

#ifdef ACF_KERNEL_IMPLEMENTATION

#include <stdint.h>


/*!
   harris_wrapper
*/
void harris_wrapper(
            vec16s* dst, int dstr,
            vec08u* src, int sstr,
            int bw, int bh,
            vec16s* svxx, vec16s* svxy, vec16s* svyy,
            vec16s* nms_x, vec16s* nms,
            vec32s* bxx_y, vec32s* bxy_y, vec32s* byy_y,
            int k, int flt_x, int flt_y, int radius,
            int16_t  lTotalNumLines,
            bool first_tile,
            int8_t covariance_shift, 
            int8_t box_shift,
			vec16s mask);
            
void extract_corners(int32s* coord, int16_t* stren, 
                     int32s* total_cnt, int32_t threshold,
                     vec32s* dst0, vec16s* dst1,
                     vec16s* src,  int sstr,
                     vec08u chunkId,
                     int start_cu, int end_cu,
                     int bw, int bh);
        
void sort_corners(int32s* feature_list, int* coord_list, int16_t* stren, short* index_buff, int total_cnt, int max_corners);
            
#endif /* HARRISAPUWRAPPER_H */

#endif /* ACF_KERNEL_IMPLEMENTATION */
