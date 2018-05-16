/**************************************************************************************************
 * 
 * NXP Confidential Proprietary
 *
 * Copyright (c) 2015, Digital Media Professionals. All rights reserved.
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
/*!*********************************************************************************
*  @file
*  @brief ACF metadata and wrapper function for the \ref secGFTT "Good Features To Track".
***********************************************************************************/
#include "gftt_apu.h"
#ifdef ACF_KERNEL_METADATA
KERNEL_INFO _kernel_info_gftt_wrapper_box7_nms5
(
   "GFTT_wrapper_box7_nms5",
   12,
   __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(2, 2, 1, 1),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(4, 1)),

   __port(  __index(1),
      __identifier("PARAMS"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(6, 1)),

   __port(  __index(2),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(4, 1)),

   __port(__index(3),
      __identifier("SVXX"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH, MAX_FILTER_Y_7+1)),

   __port(__index(4),
      __identifier("SVXY"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH, MAX_FILTER_Y_7+1)),

   __port(__index(5),
      __identifier("SVYY"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH, MAX_FILTER_Y_7+1)),

   __port(__index(6),
      __identifier("NMS_X"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH+MAX_NMS_R-1, MAX_NMS_R+1)),

   __port(__index(7),
      __identifier("NMS"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH, MAX_NMS_R+1)),

   __port(__index(8),
      __identifier("BXX"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH+MAX_FILTER_X_7-1, 1)),

   __port(__index(9),
      __identifier("BXY"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH+MAX_FILTER_X_7-1, 1)),

   __port(__index(10),
      __identifier("BYY"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH+MAX_FILTER_X_7-1, 1)),

   __port(__index(11),
      __identifier("MAX_EIGEN"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(1, 1))

);

KERNEL_INFO _kernel_info_gftt_wrapper_box5_nms5
(
   "GFTT_wrapper_box5_nms5",
   12,
   __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(2, 2, 1, 1),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(4, 1)),

   __port(  __index(1),
      __identifier("PARAMS"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(6, 1)),

   __port(  __index(2),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(4, 1)),

   __port(__index(3),
      __identifier("SVXX"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH, MAX_FILTER_Y_5+1)),

   __port(__index(4),
      __identifier("SVXY"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH, MAX_FILTER_Y_5+1)),

   __port(__index(5),
      __identifier("SVYY"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH, MAX_FILTER_Y_5+1)),

   __port(__index(6),
      __identifier("NMS_X"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH+MAX_NMS_R-1, MAX_NMS_R+1)),

   __port(__index(7),
      __identifier("NMS"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH, MAX_NMS_R+1)),

   __port(__index(8),
      __identifier("BXX"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH+MAX_FILTER_X_5-1, 1)),

   __port(__index(9),
      __identifier("BXY"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH+MAX_FILTER_X_5-1, 1)),

   __port(__index(10),
      __identifier("BYY"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH+MAX_FILTER_X_5-1, 1)),

   __port(__index(11),
      __identifier("MAX_EIGEN"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(1, 1))

);
KERNEL_INFO _kernel_info_gftt_wrapper_box3_nms5
(
   "GFTT_wrapper_box3_nms5",
   12,
   __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(2, 2, 1, 1),
      __e0_data_type(d08u),
      __e0_size(1, 1),
      __ek_size(4, 1)),
      
   __port(  __index(1),
      __identifier("PARAMS"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(6, 1)),
   
   __port(  __index(2),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(4, 1)),
    
   __port(__index(3),
      __identifier("SVXX"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH, MAX_FILTER_Y_3+1)),
    
   __port(__index(4),
      __identifier("SVXY"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH, MAX_FILTER_Y_3+1)),
    
   __port(__index(5),
      __identifier("SVYY"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH, MAX_FILTER_Y_3+1)),
    
   __port(__index(6),
      __identifier("NMS_X"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH+MAX_NMS_R-1, MAX_NMS_R+1)),
    
   __port(__index(7),
      __identifier("NMS"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH, MAX_NMS_R+1)),
          
   __port(__index(8),
      __identifier("BXX"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH+MAX_FILTER_X_3-1, 1)),
          
   __port(__index(9),
      __identifier("BXY"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH+MAX_FILTER_X_3-1, 1)),
          
   __port(__index(10),
      __identifier("BYY"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0,0,0,0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(MAX_BLOCK_WIDTH+MAX_FILTER_X_3-1, 1)),

   __port(__index(11),
      __identifier("MAX_EIGEN"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(1, 1))
   
);

KERNEL_INFO _kernel_info_gftt_extract
(
   "GFTT_extract",
   8,
   __port(  __index(0),
      __identifier("COORD"),
      __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(MAX_CORNERS, 1)),

   __port(  __index(1),
      __identifier("STREN"),
      __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(MAX_CORNERS, 1)),

   __port(  __index(2),
      __identifier("COUNT"),
      __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(1, 1)),

   __port(__index(3),
      __identifier("LOCAL_COORD"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(MAX_CORNER_PER_CHUNK, 1)),

   __port(__index(4),
      __identifier("LOCAL_STREN"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(MAX_CORNER_PER_CHUNK, 1)),

   __port(__index(5),
      __identifier("SRC"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(4, 1)),

   __port(__index(6),
      __identifier("MAX_EIGEN"),
      __attributes(ACF_ATTR_VEC_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(1, 1)),

   __port(  __index(7),
      __identifier("PARAMS"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(6, 1))
);
KERNEL_INFO _kernel_info_gftt_sort_and_filter
(
   "gftt_sort_and_filter",
   7,
   __port(  __index(0),
      __identifier("FEATURE"),
      __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(MAX_CORNERS*2, 1)),

   __port(  __index(1),
      __identifier("FEAT_COUNT"),
      __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(1, 1)),

   __port(  __index(2),
      __identifier("INDEX"),
      __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16s),
      __e0_size(1, 1),
      __ek_size(MAX_CORNERS, 1)),

   __port(  __index(3),
      __identifier("COORD"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(MAX_CORNERS, 1)),

   __port(  __index(4),
      __identifier("STREN"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(MAX_CORNERS, 1)),

   __port(  __index(5),
      __identifier("COUNT"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(1, 1)),

   __port(  __index(6),
      __identifier("PARAMS"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(6, 1))
);
#endif /* ACF_KERNEL_METADATA */

#ifdef ACF_KERNEL_IMPLEMENTATION

void GFTT_wrapper_box3_nms5(
               kernel_io_desc lIn         /*!< The source image buffer.*/,
               kernel_io_desc lParams     /*!< The parameters for gftt.*/,
               kernel_io_desc lOut        /*!< The destination image buffer.*/,
               kernel_io_desc lSvxx,
               kernel_io_desc lSvxy,
               kernel_io_desc lSvyy,
               kernel_io_desc lNms_x,
               kernel_io_desc lNms,
               kernel_io_desc lBxx,
               kernel_io_desc lBxy,
               kernel_io_desc lByy,
               kernel_io_desc lMax_eigen
)
{
   vec08u* lpvIn      = (vec08u*)lIn.pMem;
   int32s* lpvParams  = (int32s*)lParams.pMem;
   vec16s* lpvOut     = (vec16s*)lOut.pMem;
   vec16s* lpvSvxx    = (vec16s*)lSvxx.pMem;
   vec16s* lpvSvxy    = (vec16s*)lSvxy.pMem;
   vec16s* lpvSvyy    = (vec16s*)lSvyy.pMem;
   vec16s* lpvNms_x   = (vec16s*)lNms_x.pMem;
   vec16s* lpvNms     = (vec16s*)lNms.pMem;
   vec32s* lpvBxx     = (vec32s*)lBxx.pMem;
   vec32s* lpvBxy     = (vec32s*)lBxy.pMem;
   vec32s* lpvByy     = (vec32s*)lByy.pMem;
   vec16s* lpvMax_eigen =  (vec16s*)lMax_eigen.pMem;

   int out_count = 0;
   static int16_t lTotalNumLines;
   static vec16s  mask = 0;
   if (ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
   {
      int start_cu  = ACF_RET_VAR(ACF_VAR_FIRST_CUID);
      int end_cu    = start_cu + ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS) - 1;
      mask = vput(mask, (vec16s)1, start_cu); // mark on first CU
      mask = vput(mask, (vec16s)2,   end_cu); // mark on last CU
      lTotalNumLines = ACF_RET_VAR(ACF_VAR_NUM_INPUT_ITERATIONS)*lIn.chunkHeight;
      lpvMax_eigen[0] = 0;
   }
   gftt_wrapper(lpvOut, lOut.chunkSpan>>1,
                  lpvIn, lIn.chunkSpan,
                  lIn.chunkWidth, lIn.chunkHeight,
                  lpvSvxx, lpvSvxy, lpvSvyy,
                  lpvNms_x, lpvNms,
                  lpvBxx, lpvBxy, lpvByy,
                  lpvParams[1], // Box flt_x
                  lpvParams[1], // Box flt_y
                  lpvParams[2], // NMS Radius
                  lTotalNumLines,
                  ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG),//first tile
                  lpvMax_eigen,
                  lpvParams[3], // Down scale shift value for covariance
                  lpvParams[4], // Down scale shift value for box
                  mask          // mark for first cu and last cu for padding
   );
}

void GFTT_wrapper_box5_nms5(
               kernel_io_desc lIn         /*!< The source image buffer.*/,
               kernel_io_desc lParams     /*!< The parameters for gftt.*/,
               kernel_io_desc lOut        /*!< The destination image buffer.*/,
               kernel_io_desc lSvxx,
               kernel_io_desc lSvxy,
               kernel_io_desc lSvyy,
               kernel_io_desc lNms_x,
               kernel_io_desc lNms,
               kernel_io_desc lBxx,
               kernel_io_desc lBxy,
               kernel_io_desc lByy,
               kernel_io_desc lMax_eigen
)
{
   vec08u* lpvIn      = (vec08u*)lIn.pMem;
   int32s* lpvParams  = (int32s*)lParams.pMem;
   vec16s* lpvOut     = (vec16s*)lOut.pMem;
   vec16s* lpvSvxx    = (vec16s*)lSvxx.pMem;
   vec16s* lpvSvxy    = (vec16s*)lSvxy.pMem;
   vec16s* lpvSvyy    = (vec16s*)lSvyy.pMem;
   vec16s* lpvNms_x   = (vec16s*)lNms_x.pMem;
   vec16s* lpvNms     = (vec16s*)lNms.pMem;
   vec32s* lpvBxx     = (vec32s*)lBxx.pMem;
   vec32s* lpvBxy     = (vec32s*)lBxy.pMem;
   vec32s* lpvByy     = (vec32s*)lByy.pMem;
   vec16s* lpvMax_eigen =  (vec16s*)lMax_eigen.pMem;

   int out_count = 0;
   static int16_t lTotalNumLines;
   static vec16s  mask = 0;
   if (ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
   {
      int start_cu  = ACF_RET_VAR(ACF_VAR_FIRST_CUID);
      int end_cu    = start_cu + ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS) - 1;
      mask = vput(mask, (vec16s)1, start_cu); // mark on first CU
      mask = vput(mask, (vec16s)2,   end_cu); // mark on last CU
      lTotalNumLines = ACF_RET_VAR(ACF_VAR_NUM_INPUT_ITERATIONS)*lIn.chunkHeight;
      lpvMax_eigen[0] = 0;
   }
   gftt_wrapper(lpvOut, lOut.chunkSpan>>1,
                  lpvIn, lIn.chunkSpan,
                  lIn.chunkWidth, lIn.chunkHeight,
                  lpvSvxx, lpvSvxy, lpvSvyy,
                  lpvNms_x, lpvNms,
                  lpvBxx, lpvBxy, lpvByy,
                  lpvParams[1], // Box flt_x
                  lpvParams[1], // Box flt_y
                  lpvParams[2], // NMS Radius
                  lTotalNumLines,
                  ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG),//first tile
                  lpvMax_eigen,
                  lpvParams[3], // Down scale shift value for covariance
                  lpvParams[4], // Down scale shift value for box
                  mask          // mark for first cu and last cu for padding
   );
}
void GFTT_wrapper_box7_nms5(
               kernel_io_desc lIn         /*!< The source image buffer.*/,
               kernel_io_desc lParams     /*!< The parameters for gftt.*/,
               kernel_io_desc lOut        /*!< The destination image buffer.*/,
               kernel_io_desc lSvxx,
               kernel_io_desc lSvxy,
               kernel_io_desc lSvyy,
               kernel_io_desc lNms_x,
               kernel_io_desc lNms,
               kernel_io_desc lBxx,
               kernel_io_desc lBxy,
               kernel_io_desc lByy,
               kernel_io_desc lMax_eigen
)
{
   vec08u* lpvIn      = (vec08u*)lIn.pMem;
   int32s* lpvParams  = (int32s*)lParams.pMem;
   vec16s* lpvOut     = (vec16s*)lOut.pMem;
   vec16s* lpvSvxx    = (vec16s*)lSvxx.pMem;
   vec16s* lpvSvxy    = (vec16s*)lSvxy.pMem;
   vec16s* lpvSvyy    = (vec16s*)lSvyy.pMem;
   vec16s* lpvNms_x   = (vec16s*)lNms_x.pMem;
   vec16s* lpvNms     = (vec16s*)lNms.pMem;
   vec32s* lpvBxx     = (vec32s*)lBxx.pMem;
   vec32s* lpvBxy     = (vec32s*)lBxy.pMem;
   vec32s* lpvByy     = (vec32s*)lByy.pMem;
   vec16s* lpvMax_eigen =  (vec16s*)lMax_eigen.pMem;

   int out_count = 0;
   static int16_t lTotalNumLines;
   static vec16s  mask = 0;
   if (ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
   {
      int start_cu  = ACF_RET_VAR(ACF_VAR_FIRST_CUID);
      int end_cu    = start_cu + ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS) - 1;
      mask = vput(mask, (vec16s)1, start_cu); // mark on first CU
      mask = vput(mask, (vec16s)2,   end_cu); // mark on last CU
      lTotalNumLines = ACF_RET_VAR(ACF_VAR_NUM_INPUT_ITERATIONS)*lIn.chunkHeight;
      lpvMax_eigen[0] = 0;
   }
   gftt_wrapper(lpvOut, lOut.chunkSpan>>1,
                  lpvIn, lIn.chunkSpan,
                  lIn.chunkWidth, lIn.chunkHeight,
                  lpvSvxx, lpvSvxy, lpvSvyy,
                  lpvNms_x, lpvNms,
                  lpvBxx, lpvBxy, lpvByy,
                  lpvParams[1], // Box flt_x
                  lpvParams[1], // Box flt_y
                  lpvParams[2], // NMS Radius
                  lTotalNumLines,
                  ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG),//first tile
                  lpvMax_eigen,
                  lpvParams[3], // Down scale shift value for covariance
                  lpvParams[4], // Down scale shift value for box
                  mask          // mark for first cu and last cu for padding
   );
}
void  GFTT_extract(
               kernel_io_desc lCoord         /*!< Extracted coordinates.*/,
               kernel_io_desc lStren         /*!< Extracted strength.*/,
               kernel_io_desc lCount         /*!< Total counts.*/,
               kernel_io_desc lCoord1        /*!< Extracted coordinates in cmem.*/,
               kernel_io_desc lStren1        /*!< Extracted strength in cmem.*/,
               kernel_io_desc lSrc           /*!< Computed strength in cmem.*/,
               kernel_io_desc lMax_eigen     /*!< Max eigen value*/,
               kernel_io_desc lParams        /*!< The parameters for gftt. */
)
{

   int32s* lpvCoord         = (int32s*)lCoord.pMem;
   int16s* lpvStren         = (int16s*)lStren.pMem;
   int32s* lpvCount         = (int32s*)lCount.pMem;
   vec32s* lpvCoord1        = (vec32s*)lCoord1.pMem;
   vec16s* lpvStren1        = (vec16s*)lStren1.pMem;
   vec16s* lpvSrc           = (vec16s*)lSrc.pMem;
   vec16s* lpvMax_eigen     = (vec16s*)lMax_eigen.pMem;
   int32s* lpvParams        = (int32s*)lParams.pMem;

   int32s start_cu  = ACF_RET_VAR(ACF_VAR_FIRST_CUID);
   int32s end_cu    = start_cu + ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS) - 1;

   
   if (ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))    
   {
      lpvCount[0] = 0;
   }

   vec08u lpvChunkId = 255;
   for (int i=start_cu; i<=end_cu; ++i) {
      lpvChunkId = vput(lpvChunkId, i, i);
   }
   extract_corners_gftt(
                     lpvCoord, lpvStren, lpvCount,
                     lpvCoord1, lpvStren1, 
                     lpvSrc, lSrc.chunkSpan>>1,
                     lpvChunkId,
                     start_cu, end_cu,
                     lSrc.chunkWidth, lSrc.chunkHeight,
                     lpvMax_eigen[0],
                     lpvParams[0]// quality level
                     );
}
void  gftt_sort_and_filter(
               kernel_io_desc lFeat         /*!< Final feature list.*/,
               kernel_io_desc lFeat_cnt     /*!< Feature list count.*/,
               kernel_io_desc lIndex        /*!< Index.*/,
               kernel_io_desc lCoord        /*!< Extracted coordinates.*/,
               kernel_io_desc lStren        /*!< Extracted strength.*/,
               kernel_io_desc lTotalCount   /*!< Total corners counts.*/,
               kernel_io_desc lParams       /*!< The parameters for sort and filter. */
)
{

   int32s* lpvFeat            = (int32s*)lFeat.pMem;
   int32s* lpvFeat_cnt        = (int32s*)lFeat_cnt.pMem;
   int16s* lpvIndex           = (int16s*)lIndex.pMem;
   int32s* lpvCoord           = (int32s*)lCoord.pMem;
   int16s* lpvStren           = (int16s*)lStren.pMem;
   int32s* lpvTotalCount      = (int32s*)lTotalCount.pMem;
   int32s* lpvParams          = (int32s*)lParams.pMem;

   if (ACF_RET_VAR(ACF_VAR_LAST_TILE_FLAG))
   {
      sort_and_filter_corners(
               lpvFeat, lpvFeat_cnt[0], 
               lpvCoord, lpvStren,
               lpvParams[0], //quality level
               lpvParams[5], //maximum corners
               lpvIndex, lpvTotalCount[0]);
   }
}
#endif /* ACF_KERNEL_IMPLEMENTATION */
