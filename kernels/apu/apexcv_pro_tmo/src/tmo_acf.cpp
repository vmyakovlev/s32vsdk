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
#ifdef ACF_KERNEL_METADATA

KERNEL_INFO kernel_info_calc_geo_mean_rgbe
(
"calc_geo_mean_rgbe",
9,
__port(__index(0),
__identifier("INPUT"),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(4, 1),
__ek_size(1, 1)),

__port(__index(1),
__identifier("LOG_TABLE"),
__attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d16s),
__e0_size(1, 1),
__ek_size(64, 4)),

__port(__index(2),
__identifier("EXP_TABLE"),
__attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(128, 2)),

__port(__index(3),
__identifier("INTERMEDIATE_EXP"),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(3, 1),
__ek_size(1, 1)),

__port(__index(4),
__identifier("INTERMEDIATE_MAN"),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(3, 1),
__ek_size(1, 1)),

__port(__index(5),
__identifier("WORLD_EXP"),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),

__port(__index(6),
__identifier("WORLD_MAN"),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),

__port(__index(7),
__identifier("GEO_MEAN_EXP"),
__attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),

__port(__index(8),
__identifier("GEO_MEAN_MAN"),
__attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1))
);

KERNEL_INFO kernel_info_calc_geo_mean_openexr
(
"calc_geo_mean_openexr",
9,
__port(__index(0),
__identifier("INPUT"),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d16u),
__e0_size(3, 1),
__ek_size(1, 1)),

__port(__index(1),
__identifier("LOG_TABLE"),
__attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d16s),
__e0_size(1, 1),
__ek_size(64, 4)),

__port(__index(2),
__identifier("EXP_TABLE"),
__attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(128, 2)),

__port(__index(3),
__identifier("INTERMEDIATE_EXP"),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(3, 1),
__ek_size(1, 1)),

__port(__index(4),
__identifier("INTERMEDIATE_MAN"),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(3, 1),
__ek_size(1, 1)),

__port(__index(5),
__identifier("WORLD_EXP"),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),

__port(__index(6),
__identifier("WORLD_MAN"),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),

__port(__index(7),
__identifier("GEO_MEAN_EXP"),
__attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),

__port(__index(8),
__identifier("GEO_MEAN_MAN"),
__attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1))
);

KERNEL_INFO kernel_info_ldr_transform
(
"ldr_transform",
10,
__port(__index(0),
__identifier("INTERMEDIATE_EXP"),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(3, 1),
__ek_size(1, 1)),

__port(__index(1),
__identifier("INTERMEDIATE_MAN"),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(3, 1),
__ek_size(1, 1)),

__port(__index(2),
__identifier("WORLD_EXP"),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),

__port(__index(3),
__identifier("WORLD_MAN"),
__attributes(ACF_ATTR_VEC_IN),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),

__port(__index(4),
__identifier("GEO_MEAN_EXP"),
__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),

__port(__index(5),
__identifier("LOG_TABLE"),
__attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d16s),
__e0_size(1, 1),
__ek_size(64, 4)),

__port(__index(6),
__identifier("DIV_TABLE"),
__attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d16u),
__e0_size(1, 1),
__ek_size(64, 8)),

__port(__index(7),
__identifier("DIV_VALUE"),
__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d16u),
__e0_size(2, 1),
__ek_size(1, 1)),

__port(__index(8),
__identifier("KEY_VALUE"),
__attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(1, 1),
__ek_size(1, 1)),

__port(__index(9),
__identifier("OUTPUT"),
__attributes(ACF_ATTR_VEC_OUT),
__spatial_dep(0, 0, 0, 0),
__e0_data_type(d08u),
__e0_size(3, 1),
__ek_size(1, 1))
);

#endif // ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION
#include "tmo_apu.h"

void calc_geo_mean_rgbe(kernel_io_desc lIn   /*!< Input image.*/,
   kernel_io_desc lInLogTable                /*!< Input log LUT.*/,
   kernel_io_desc lInExpTable                /*!< Input exponent LUT.*/,
   kernel_io_desc lOutIe                     /*!< Output intermediate exponent.*/,
   kernel_io_desc lOutIm                     /*!< Output intermediate mantissa.*/,
   kernel_io_desc lOutWe                     /*!< Output world luminance exponent.*/,
   kernel_io_desc lOutWm                     /*!< Output world luminance mantissa.*/,
   kernel_io_desc lOutGeoMeanExp             /*!< Output geometric mean exponent.*/,
   kernel_io_desc lOutGeoMeanMan             /*!< Output geometric mean mantissa.*/
   )
{
   vec08u *lpvIn = (vec08u *)lIn.pMem;
   vec16s *logTable = (vec16s *)lInLogTable.pMem;
   int08u *expTable = (int08u *)lInExpTable.pMem;
   vec08u *lpvOutIe = (vec08u *)lOutIe.pMem;
   vec08u *lpvOutIm = (vec08u *)lOutIm.pMem;
   vec08u *lpvOutWe = (vec08u *)lOutWe.pMem;
   vec08u *lpvOutWm = (vec08u *)lOutWm.pMem;
   int08u *lpvOutGeoMeanExp = (int08u *)lOutGeoMeanExp.pMem;
   int08u *lpvOutGeoMeanMan = (int08u *)lOutGeoMeanMan.pMem;
   static vec32s sumWe;
   static vec32s sumWm;
   static vec32u sumCnt;

   int16u start_cu = 0;// ACF_RET_VAR(ACF_VAR_FIRST_CUID);
   int16u end_cu = start_cu + ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS);

   if (ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
   {
      sumWe = 0;
      sumWm = 0;
      sumCnt = 0;
   }

   apu_tmo_calc_world_lum_rgbe(lpvIn,
                               lIn.chunkSpan,
                               lpvOutIe,
                               lpvOutIm,
                               lOutIe.chunkSpan,
                               lpvOutWe,
                               lpvOutWm,
                               lOutWe.chunkSpan,
                               logTable,
                               &sumWe,
                               &sumWm,
                               &sumCnt,
                               lIn.chunkWidth,
                               lIn.chunkHeight,
                               ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG));

   if (ACF_RET_VAR(ACF_VAR_LAST_TILE_FLAG))
   {
      apu_tmo_calc_geo_mean(sumWe,
                            sumWm,
                            sumCnt,
                            start_cu,
                            end_cu,
                            expTable,
                            lpvOutGeoMeanExp,
                            lpvOutGeoMeanMan);
   }
}

void calc_geo_mean_openexr(kernel_io_desc lIn/*!< Input image.*/,
   kernel_io_desc lInLogTable                /*!< Input log LUT.*/,
   kernel_io_desc lInExpTable                /*!< Input exponent LUT.*/,
   kernel_io_desc lOutIe                     /*!< Output intermediate exponent.*/,
   kernel_io_desc lOutIm                     /*!< Output intermediate mantissa.*/,
   kernel_io_desc lOutWe                     /*!< Output world luminance exponent.*/,
   kernel_io_desc lOutWm                     /*!< Output world luminance mantissa.*/,
   kernel_io_desc lOutGeoMeanExp             /*!< Output geometric mean exponent.*/,
   kernel_io_desc lOutGeoMeanMan             /*!< Output geometric mean mantissa.*/
   )
{

   vec16u *lpvIn = (vec16u *)lIn.pMem;
   vec16s *logTable = (vec16s *)lInLogTable.pMem;
   int08u *expTable = (int08u *)lInExpTable.pMem;
   vec08u *lpvOutIe = (vec08u *)lOutIe.pMem;
   vec08u *lpvOutIm = (vec08u *)lOutIm.pMem;
   vec08u *lpvOutWe = (vec08u *)lOutWe.pMem;
   vec08u *lpvOutWm = (vec08u *)lOutWm.pMem;
   int08u *lpvOutGeoMeanExp = (int08u *)lOutGeoMeanExp.pMem;
   int08u *lpvOutGeoMeanMan = (int08u *)lOutGeoMeanMan.pMem;
   static vec32s sumWe;
   static vec32s sumWm;
   static vec32u sumCnt;

   int16u start_cu = 0;// ACF_RET_VAR(ACF_VAR_FIRST_CUID);
   int16u end_cu = start_cu + ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS);

   if (ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
   {
      sumWe = 0;
      sumWm = 0;
      sumCnt = 0;
   }

   apu_tmo_calc_world_lum_openexr(lpvIn,
                                  lIn.chunkSpan >> 1,
                                  lpvOutIe,
                                  lpvOutIm,
                                  lOutIe.chunkSpan,
                                  lpvOutWe,
                                  lpvOutWm,
                                  lOutWe.chunkSpan,
                                  logTable,
                                  &sumWe,
                                  &sumWm,
                                  &sumCnt,
                                  lIn.chunkWidth,
                                  lIn.chunkHeight,
                                  ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG));

   if (ACF_RET_VAR(ACF_VAR_LAST_TILE_FLAG))
   {
      apu_tmo_calc_geo_mean(sumWe,
                            sumWm,
                            sumCnt,
                            start_cu,
                            end_cu,
                            expTable,
                            lpvOutGeoMeanExp,
                            lpvOutGeoMeanMan);
   }
}

void ldr_transform(kernel_io_desc lInIe   /*!< Input intermediate exponent.*/,
   kernel_io_desc lInIm                   /*!< Input intermediate mantissa.*/,
   kernel_io_desc lInWe                   /*!< Input world luminance exponent.*/,
   kernel_io_desc lInWm                   /*!< Input world luminance mantissa.*/,
   kernel_io_desc lInGeoMeanExp           /*!< Input geometric mean exponent.*/,
   kernel_io_desc lInLogTable             /*!< Input log LUT.*/,
   kernel_io_desc lInDivTable             /*!< Input division LUT.*/,
   kernel_io_desc lInDivValue             /*!< Input division value.*/,
   kernel_io_desc lInKeyValue             /*!< Input key value.*/,
   kernel_io_desc lOut                    /*!< Output image.*/
   )
{

   vec08u *lpvInIe = (vec08u *)lInIe.pMem;
   vec08u *lpvInIm = (vec08u *)lInIm.pMem;
   vec08u *lpvInWe = (vec08u *)lInWe.pMem;
   vec08u *lpvInWm = (vec08u *)lInWm.pMem;
   int08u *lpvInGeoMeanExp = (int08u *)lInGeoMeanExp.pMem;
   vec16s *logTable = (vec16s *)lInLogTable.pMem;
   vec16u *divTable = (vec16u *)lInDivTable.pMem;
   int16u *divValue = (int16u *)lInDivValue.pMem;
   int08u *keyValue = (int08u *)lInKeyValue.pMem;
   vec08u *lpvOut = (vec08u *)lOut.pMem;

   apu_tmo_ldr_transform(lpvInGeoMeanExp[0],
                         lpvInIe,
                         lpvInIm,
                         lInIe.chunkSpan,
                         lpvInWe,
                         lpvInWm,
                         lInWe.chunkSpan,
                         divValue,
                         logTable,
                         divTable,
                         keyValue[0],
                         lpvOut,
                         lOut.chunkSpan,
                         lInIe.chunkWidth,
                         lInIe.chunkHeight,
                         ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG));
}

#endif // ACF_KERNEL_IMPLEMENTATION
