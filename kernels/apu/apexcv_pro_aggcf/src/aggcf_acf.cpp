/**************************************************************************************************
 * 
 * NXP Confidential Proprietary
 *
 * Copyright (c) 2016-2017 NXP 
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

#include "aggcf_apu.h"
#ifdef ACF_KERNEL_METADATA

KERNEL_INFO _kernel_info_agg_acf_biresize_pass
(
  "agg_acf_biresize_pass",
    2,
  __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

  __port(  __index(1),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1))

);

KERNEL_INFO _kernel_info_agg_acf_biresize_hori_r1
(
  "agg_acf_biresize_hori_r1",
    2,
  __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(2, 1)),

  __port(  __index(1),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1))

);

KERNEL_INFO _kernel_info_agg_acf_biresize_hori_r2
(
  "agg_acf_biresize_hori_r2",
    2,

  __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(4, 1)),

  __port(  __index(1),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1))

);

KERNEL_INFO _kernel_info_agg_acf_biresize_hori_r3
(
  "agg_acf_biresize_hori_r3",
    2,
  __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(8, 1)),

  __port(  __index(1),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1))

);

KERNEL_INFO _kernel_info_agg_acf_biresize_verti_r1
(
  "agg_acf_biresize_verti_r1",
    2,
  __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 2)),

  __port(  __index(1),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1))

);

KERNEL_INFO _kernel_info_agg_acf_biresize_verti_r2
(
  "agg_acf_biresize_verti_r2",
    2,

  __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 4)),

  __port(  __index(1),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1))

);

KERNEL_INFO _kernel_info_agg_acf_biresize_verti_r3
(
  "agg_acf_biresize_verti_r3",
    2,
  __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 8)),

  __port(  __index(1),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1))

);


KERNEL_INFO _kernel_info_agg_acf_flt
(
    "agg_acf_flt",
    4,
    __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(1, 1, 1, 1),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
      
    __port(  __index(1),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(2),
      __identifier("TEMP_ROW"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(3),
      __identifier("PARAMS"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(2, 1))

);


KERNEL_INFO _kernel_info_agg_acf_flt_norm
(
    "agg_acf_flt_norm",
    4,
    __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(1, 1, 1, 1),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(1),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(2),
      __identifier("TEMP_ROW"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(3),
      __identifier("PARAMS"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(2, 1))

);

KERNEL_INFO _kernel_info_agg_acf_luv
(
    "agg_acf_luv",
    5,
    __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(1, 1)),
      
    __port(  __index(1),
      __identifier("OUTPUTL"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(2),
      __identifier("OUTPUTU"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(3),
      __identifier("OUTPUTV"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(4),
      __identifier("LUV_LUT"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1064, 1),
      __ek_size(1, 1))

);


// following spatial dep is defined
// for triangle filtering of raidus 5
KERNEL_INFO _kernel_info_agg_acf_triflt
(
    "agg_acf_triflt",
    6,
    __port(  __index(0),
      __identifier("INPUT"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(7, 7, 7, 7),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(10, 8)),
      
    __port(  __index(1),
      __identifier("OUTPUT"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(10, 8)),

    __port(  __index(2),
      __identifier("TEMP_ROW"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(12, 24)),

    __port(  __index(3),
      __identifier("PARAMS"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(4),
      __identifier("t_lastline"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(12, 1)),  

    __port(  __index(5),
       __identifier("u_lastline"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(12, 1))

);


KERNEL_INFO _kernel_info_agg_acf
(
    "agg_acf",
    10,
    __port(  __index(0),
      __identifier("INPUTL"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(1, 1, 1, 1),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
      
    __port(  __index(1),
      __identifier("INPUTU"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(1, 1, 1, 1),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),
   
    __port(  __index(2),
      __identifier("INPUTV"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(1, 1, 1, 1),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(3),
      __identifier("OUTPUTM"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(4),
      __identifier("OUTPUTO"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(5),
      __identifier("PARAMS"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(6),
      __identifier("OUTPUTGx"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(64, 1)),

    __port(  __index(7),
      __identifier("OUTPUTGy"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(64, 1)),

    __port(  __index(8),
      __identifier("OUTPUTM2"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(64, 1)),


    __port(  __index(9),
      __identifier("ATANTABLE"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(65, 1))


);

KERNEL_INFO _kernel_info_agg_acfhist_nt_1x1
(
    "agg_acfhist_nt_1x1",
    //14,
    10,
    __port(  __index(0),
      __identifier("INPUTM"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(1),
      __identifier("INPUTO"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(2),
      __identifier("OFFSET"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(128, 1)),

    __port(  __index(3),
      __identifier("OUTPUTH1"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(4),
      __identifier("OUTPUTH2"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(5),
      __identifier("OUTPUTH3"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(6),
      __identifier("OUTPUTH4"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(7),
      __identifier("OUTPUTH5"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(8),
      __identifier("OUTPUTH6"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(9),
      __identifier("PARAMS"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(4, 1))//,

    //__port(  __index(10),
    //  __identifier("OUTPUTM0"),
    //  __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
    //  __spatial_dep(0, 0, 0, 0),
    //  __e0_data_type(d32u),
    //  __e0_size(1, 1),
    //  __ek_size(16, 1)),

    //__port(  __index(11),
    //  __identifier("OUTPUTM1"),
    //  __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
    //  __spatial_dep(0, 0, 0, 0),
    //  __e0_data_type(d32u),
    //  __e0_size(1, 1),
    //  __ek_size(16, 1)),

    //__port(  __index(12),
    //  __identifier("OUTPUTO0"),
    //  __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
    //  __spatial_dep(0, 0, 0, 0),
    //  __e0_data_type(d32u),
    //  __e0_size(1, 1),
    //  __ek_size(16, 1)),

    //__port(  __index(13),
    //  __identifier("OUTPUTO1"),
    //  __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
    //  __spatial_dep(0, 0, 0, 0),
    //  __e0_data_type(d32u),
    //  __e0_size(1, 1),
    //  __ek_size(16, 1))

);

KERNEL_INFO _kernel_info_agg_acfhist_nt_2x2
(
    "agg_acfhist_nt_2x2",
    14,
    __port(  __index(0),
      __identifier("INPUTM"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(2, 2)),

    __port(  __index(1),
      __identifier("INPUTO"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(2, 2)),

    __port(  __index(2),
      __identifier("OFFSET"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(128, 1)),

    __port(  __index(3),
      __identifier("OUTPUTH1"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(4),
      __identifier("OUTPUTH2"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(5),
      __identifier("OUTPUTH3"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(6),
      __identifier("OUTPUTH4"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(7),
      __identifier("OUTPUTH5"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(8),
      __identifier("OUTPUTH6"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(9),
      __identifier("PARAMS"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(4, 1)),

    __port(  __index(10),
      __identifier("OUTPUTM0"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(11),
      __identifier("OUTPUTM1"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(12),
      __identifier("OUTPUTO0"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(13),
      __identifier("OUTPUTO1"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1))

);


KERNEL_INFO _kernel_info_agg_acfhist_nt_3x3
(
    "agg_acfhist_nt_3x3",
    14,
    __port(  __index(0),
      __identifier("INPUTM"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(3, 3)),

    __port(  __index(1),
      __identifier("INPUTO"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(3, 3)),

    __port(  __index(2),
      __identifier("OFFSET"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(128, 1)),

    __port(  __index(3),
      __identifier("OUTPUTH1"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(4),
      __identifier("OUTPUTH2"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(5),
      __identifier("OUTPUTH3"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(6),
      __identifier("OUTPUTH4"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(7),
      __identifier("OUTPUTH5"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(8),
      __identifier("OUTPUTH6"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(9),
      __identifier("PARAMS"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(4, 1)),

    __port(  __index(10),
      __identifier("OUTPUTM0"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(11),
      __identifier("OUTPUTM1"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(12),
      __identifier("OUTPUTO0"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(13),
      __identifier("OUTPUTO1"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1))

);


KERNEL_INFO _kernel_info_agg_acfhist_nt_4x4
(
    "agg_acfhist_nt_4x4",
    14,
    __port(  __index(0),
      __identifier("INPUTM"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(4, 4)),

    __port(  __index(1),
      __identifier("INPUTO"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(4, 4)),

    __port(  __index(2),
      __identifier("OFFSET"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(128, 1)),

    __port(  __index(3),
      __identifier("OUTPUTH1"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(4),
      __identifier("OUTPUTH2"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(5),
      __identifier("OUTPUTH3"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(6),
      __identifier("OUTPUTH4"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(7),
      __identifier("OUTPUTH5"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(8),
      __identifier("OUTPUTH6"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(9),
      __identifier("PARAMS"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(4, 1)),

    __port(  __index(10),
      __identifier("OUTPUTM0"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(11),
      __identifier("OUTPUTM1"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(12),
      __identifier("OUTPUTO0"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(13),
      __identifier("OUTPUTO1"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1))

);


KERNEL_INFO _kernel_info_agg_acfhist_ti_2x2
(
    "agg_acfhist_ti_2x2",
    14,
    __port(  __index(0),
      __identifier("INPUTM"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(2, 0, 2, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(2, 2)),

    __port(  __index(1),
      __identifier("INPUTO"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(2, 0, 2, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(2, 2)),

    __port(  __index(2),
      __identifier("OFFSET"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(128, 1)),

    __port(  __index(3),
      __identifier("OUTPUTH1"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(4),
      __identifier("OUTPUTH2"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(5),
      __identifier("OUTPUTH3"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(6),
      __identifier("OUTPUTH4"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(7),
      __identifier("OUTPUTH5"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(8),
      __identifier("OUTPUTH6"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(1, 1)),

    __port(  __index(9),
      __identifier("PARAMS"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(4, 1)),

    __port(  __index(10),
      __identifier("OUTPUTM0"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(11),
      __identifier("OUTPUTM1"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(12),
      __identifier("OUTPUTO0"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(13),
      __identifier("OUTPUTO1"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1))

);


KERNEL_INFO _kernel_info_agg_acfhist_ti_3x3
(
    "agg_acfhist_ti_3x3",
    14,
    __port(  __index(0),
      __identifier("INPUTM"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(3, 0, 3, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(6, 6)),

    __port(  __index(1),
      __identifier("INPUTO"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(3, 0, 3, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(6, 6)),

    __port(  __index(2),
      __identifier("OFFSET"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(128, 1)),

    __port(  __index(3),
      __identifier("OUTPUTH1"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(2, 2)),

    __port(  __index(4),
      __identifier("OUTPUTH2"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(2, 2)),

    __port(  __index(5),
      __identifier("OUTPUTH3"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(2, 2)),

    __port(  __index(6),
      __identifier("OUTPUTH4"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(2, 2)),

    __port(  __index(7),
      __identifier("OUTPUTH5"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(2, 2)),

    __port(  __index(8),
      __identifier("OUTPUTH6"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(2, 2)),

    __port(  __index(9),
      __identifier("PARAMS"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(4, 1)),

    __port(  __index(10),
      __identifier("OUTPUTM0"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(11),
      __identifier("OUTPUTM1"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(12),
      __identifier("OUTPUTO0"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(13),
      __identifier("OUTPUTO1"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1))

);


KERNEL_INFO _kernel_info_agg_acfhist_ti_4x4
(
    "agg_acfhist_ti_4x4",
    14,
    __port(  __index(0),
      __identifier("INPUTM"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(4, 0, 4, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(8, 8)),

    __port(  __index(1),
      __identifier("INPUTO"),
      __attributes(ACF_ATTR_VEC_IN),
      __spatial_dep(4, 0, 4, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(8, 8)),

    __port(  __index(2),
      __identifier("OFFSET"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(128, 1)),

    __port(  __index(3),
      __identifier("OUTPUTH1"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(2, 2)),

    __port(  __index(4),
      __identifier("OUTPUTH2"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(2, 2)),

    __port(  __index(5),
      __identifier("OUTPUTH3"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(2, 2)),

    __port(  __index(6),
      __identifier("OUTPUTH4"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(2, 2)),

    __port(  __index(7),
      __identifier("OUTPUTH5"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(2, 2)),

    __port(  __index(8),
      __identifier("OUTPUTH6"),
      __attributes(ACF_ATTR_VEC_OUT),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d16u),
      __e0_size(1, 1),
      __ek_size(2, 2)),

    __port(  __index(9),
      __identifier("PARAMS"),
      __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32s),
      __e0_size(1, 1),
      __ek_size(4, 1)),

    __port(  __index(10),
      __identifier("OUTPUTM0"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(11),
      __identifier("OUTPUTM1"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(12),
      __identifier("OUTPUTO0"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1)),

    __port(  __index(13),
      __identifier("OUTPUTO1"),
      __attributes(ACF_ATTR_VEC_OUT_STATIC_FIXED),
      __spatial_dep(0, 0, 0, 0),
      __e0_data_type(d32u),
      __e0_size(1, 1),
      __ek_size(16, 1))

);

#endif /* ACF_KERNEL_METADATA */

#ifdef ACF_KERNEL_IMPLEMENTATION


void agg_acf_biresize_pass(
               kernel_io_desc lIn,
               kernel_io_desc lOut
)
{
    vec16u* in       = (vec16u*)lIn.pMem;
    vec16u* out      = (vec16u*)lOut.pMem;
    apu_bi_pyramid_pass(in, out, lIn.chunkSpan>>1, lOut.chunkSpan>>1, lIn.chunkWidth, lIn.chunkHeight );   

}


void agg_acf_biresize_hori_r1(
               kernel_io_desc lIn,
               kernel_io_desc lOut
)
{
    vec16u* in       = (vec16u*)lIn.pMem;
    vec16u* out      = (vec16u*)lOut.pMem;
    apu_bi_pyramid_hori(in, out, lIn.chunkSpan>>1, lOut.chunkSpan>>1, lIn.chunkWidth, lIn.chunkHeight, 2, 1 );

}

void agg_acf_biresize_hori_r2(
               kernel_io_desc lIn,
               kernel_io_desc lOut
)
{
    vec16u* in       = (vec16u*)lIn.pMem;
    vec16u* out      = (vec16u*)lOut.pMem;
    apu_bi_pyramid_hori(in, out, lIn.chunkSpan>>1, lOut.chunkSpan>>1, lIn.chunkWidth, lIn.chunkHeight, 4, 2 );

}

void agg_acf_biresize_hori_r3(
               kernel_io_desc lIn,
               kernel_io_desc lOut
)
{
    vec16u* in       = (vec16u*)lIn.pMem;
    vec16u* out      = (vec16u*)lOut.pMem;
    apu_bi_pyramid_hori(in, out, lIn.chunkSpan>>1, lOut.chunkSpan>>1, lIn.chunkWidth, lIn.chunkHeight, 8, 3 );

}

void agg_acf_biresize_verti_r1(
               kernel_io_desc lIn,
               kernel_io_desc lOut
)
{
    vec16u* in       = (vec16u*)lIn.pMem;
    vec16u* out      = (vec16u*)lOut.pMem;
    apu_bi_pyramid_verti(in, out, lIn.chunkSpan>>1, lOut.chunkSpan>>1, lIn.chunkWidth, lIn.chunkHeight, 2, 1 );

}

void agg_acf_biresize_verti_r2(
               kernel_io_desc lIn,
               kernel_io_desc lOut
)
{
    vec16u* in       = (vec16u*)lIn.pMem;
    vec16u* out      = (vec16u*)lOut.pMem;
    apu_bi_pyramid_verti(in, out, lIn.chunkSpan>>1, lOut.chunkSpan>>1, lIn.chunkWidth, lIn.chunkHeight, 4, 2 );

}

void agg_acf_biresize_verti_r3(
               kernel_io_desc lIn,
               kernel_io_desc lOut
)
{
    vec16u* in       = (vec16u*)lIn.pMem;
    vec16u* out      = (vec16u*)lOut.pMem;
    apu_bi_pyramid_verti(in, out, lIn.chunkSpan>>1, lOut.chunkSpan>>1, lIn.chunkWidth, lIn.chunkHeight, 8, 3 );

}

void agg_acf_flt(
               kernel_io_desc lIn,  
               kernel_io_desc lout,  
               kernel_io_desc ltemp_row,  
               kernel_io_desc params
)
{
    vec16u* in       = (vec16u*)lIn.pMem;
    vec16u* out      = (vec16u*)lout.pMem;
    vec32u* temp_row = (vec32u*)ltemp_row.pMem;
    int32_t *lparams = (int32_t *)params.pMem;
    uint32_t p = (uint32_t)lparams[0];
    triangleFilt1_c_row(in, out, lIn.chunkHeight, lIn.chunkWidth, lIn.chunkSpan>>1, lout.chunkSpan>>1, temp_row, p, 1);
}

void agg_acf_flt_norm(
               kernel_io_desc lIn,
               kernel_io_desc lout,
               kernel_io_desc ltemp_row,
               kernel_io_desc params
)
{
    vec16u* in         = (vec16u*)lIn.pMem;
    vec16u* out        = (vec16u*)lout.pMem;
    vec32u* temp_row   = (vec32u*)ltemp_row.pMem;
    int32_t *lparams  = (int32_t *)params.pMem;
    uint32_t p         = (uint32_t)lparams[0];
    uint32_t normconst = (uint32_t)lparams[1];
    triangleFilt1_c_row_norm(in, out, lIn.chunkHeight, lIn.chunkWidth, lIn.chunkSpan>>1, lout.chunkSpan>>1, temp_row, p, 1, normconst);
}


void agg_acf_luv(
               kernel_io_desc  rgbIn_desc,  
               kernel_io_desc lout_desc,  
               kernel_io_desc uout_desc,  
               kernel_io_desc vout_desc,
               kernel_io_desc luv_lut_desc
)
{
    vec32s* in       = (vec32s*)rgbIn_desc.pMem;
    vec16u* lout     = (vec16u*)lout_desc.pMem;
    vec16u* uout     = (vec16u*)uout_desc.pMem;
    vec16u* vout     = (vec16u*)vout_desc.pMem;
    
    vec16u* luvlut_out= (vec16u*)luv_lut_desc.pMem;
   
    int16u height    = rgbIn_desc.chunkHeight;
    int16u width     = rgbIn_desc.chunkWidth;

    apu_aggcf_rgb2luv(in, lout, uout, vout, rgbIn_desc.chunkSpan>>2, lout_desc.chunkSpan>>1, width, height, luvlut_out);
}


void agg_acf_triflt(
               kernel_io_desc lIn,  
               kernel_io_desc lout,  
               kernel_io_desc ltemp_out,  
               kernel_io_desc params,
               kernel_io_desc t_lastline,
               kernel_io_desc u_lastline

)
{
    vec16u* in       = (vec16u*)lIn.pMem;
    vec16u* out      = (vec16u*)lout.pMem;
    vec32s* temp_out = (vec32s*)ltemp_out.pMem;
    int32_t *lparams = (int32_t *)params.pMem;
    int32_t raidus = lparams[0];

    vec32s *T_lastline_buf = (vec32s *)t_lastline.pMem;
    vec32s *U_lastline_buf = (vec32s *)u_lastline.pMem;


    vec32u firstchunk = 0;
    firstchunk = vput(firstchunk, (vec32u)1, ACF_RET_VAR(ACF_VAR_FIRST_CUID));

    vec32u lastchunk = 0;
    lastchunk = vput(lastchunk, (vec32u)1, ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS)+ACF_RET_VAR(ACF_VAR_FIRST_CUID)-1);

    // hard coded for radius = 5 case 
    triangleFilter(in, lIn.chunkSpan>>1, temp_out, ltemp_out.chunkSpan>>2, out, lout.chunkSpan>>1, lIn.chunkHeight, lout.chunkWidth, 7,
                  T_lastline_buf, U_lastline_buf, firstchunk, lastchunk  );
}

extern int global_debug[128];

void agg_acf(
               kernel_io_desc lInl          /*!< The source image L buffer.*/,
               kernel_io_desc lInu          /*!< The source image U buffer.*/,
               kernel_io_desc lInv          /*!< The source image V buffer.*/,
               kernel_io_desc loutM         /*!< The output image M(magnitude)  buffer.*/,
               kernel_io_desc loutO         /*!< The source image O(Orientation)buffer.*/,
               kernel_io_desc lParams       /*!< The parameters for M/O calculation.*/,
               kernel_io_desc lOutGx        /*!< The buffer of Gx calculation.*/,
               kernel_io_desc lOutGy        /*!< The buffer of Gy calculation.*/,
               kernel_io_desc lOutM2        /*!< The buffer of Mag calculation.*/,
               kernel_io_desc lLutTable     /*!< atan calculation lookup table.*/
)
{
    vec16u* L = (vec16u*)lInl.pMem; 
    vec16u* U = (vec16u*)lInu.pMem; 
    vec16u* V = (vec16u*)lInv.pMem; 
    vec16u* M = (vec16u*)loutM.pMem; 
    vec16u* O = (vec16u*)loutO.pMem; 
    int32s* lpvParams  = (int32s*)lParams.pMem;
    vec32s* Gx = (vec32s*)lOutGx.pMem; 
    vec32s* Gy = (vec32s*)lOutGy.pMem; 
    vec32u* M2 = (vec32u*)lOutM2.pMem; 
    vec32u* lut = (vec32u*)lLutTable.pMem; 
    
    fxgradMag_c_row_opt(L, U, V, lInl.chunkSpan>>1, loutM.chunkSpan>>1,
                        M, O, 
                        lInl.chunkHeight, lInl.chunkWidth,
                        Gx, Gy, M2, lut );

    // slow scalar version fx_sqrt/fx_atamag 
    //fxgradMag_c_row(L, U, V, lInl.chunkSpan>>1, loutM.chunkSpan>>1,
    //                M, O, 
    //                lInl.chunkHeight, lInl.chunkWidth, 0,
    //                Gx, Gy, M2);


}


// non-trilinear histogram calculation kernel
// support bin size  [1,2,3,4]

void agg_acfhist_nt_1x1(
               kernel_io_desc InM            /*!< input magnitude.   */,
               kernel_io_desc InO            /*!< input orientation. */,
               kernel_io_desc Inoffset       /*!< index offset array.*/,
               kernel_io_desc outH0          /*!< output histogram.  */,
               kernel_io_desc outH1          /*!< output histogram.  */,
               kernel_io_desc outH2          /*!< output histogram.  */,
               kernel_io_desc outH3          /*!< output histogram.  */,
               kernel_io_desc outH4          /*!< output histogram.  */,
               kernel_io_desc outH5          /*!< output histogram.  */,
               kernel_io_desc Params         /*!< input patameters.  *///,
               //kernel_io_desc outM0        /*!< temp row buffer for magnitude interpolation component at bin0.*/,
               //kernel_io_desc outM1        /*!< temp row buffer for magnitude interpolation component at bin1.*/,
               //kernel_io_desc outO0        /*!< temp row buffer of orientation distance to bin0.*/,
               //kernel_io_desc outO1        /*!< temp row buffer of orientation distance to bin1.*/
)
{
   // input/output buffer
   vec16u* M             = (vec16u*)InM.pMem;
   vec16u* O             = (vec16u*)InO.pMem; 
   int32_t* offset       = (int32_t*)Inoffset.pMem; 
   
   vec16u *H0 = (vec16u *)outH0.pMem;
   vec16u *H1 = (vec16u *)outH1.pMem;
   vec16u *H2 = (vec16u *)outH2.pMem;
   vec16u *H3 = (vec16u *)outH3.pMem;
   vec16u *H4 = (vec16u *)outH4.pMem;
   vec16u *H5 = (vec16u *)outH5.pMem;
   
   // chunk width/height
   int32_t h             = InM.chunkHeight; 
   int32_t w             = InM.chunkWidth;
   
   // parameter set
   int32_t *lParams      = (int32_t*)Params.pMem; 
   int32_t full          = lParams[0];
   //assert(bin == 1); 
   int32_t nOrientations = lParams[1]; 
   int32_t bin           = lParams[2];
   int32_t softBin       = lParams[3]; 
   
   int32_t nb = w/bin*h/bin; 
   // temp row buffer for M/O interpolation calculation
   vec32u* M0           = 0;//(vec32u*)outM0.pMem;  
   vec32u* M1           = 0;//(vec32u*)outM1.pMem; 
   vec32u* O0           = 0;//(vec32u*)outO0.pMem; 
   vec32u* O1           = 0;//(vec32u*)outO1.pMem; 

   // histogram calculation(non-trilinear) 
   //fxgradHist_c_row_nontrilinear( M, O, H0, H1, H2, H3, H4, H5, h, w, InM.chunSpan>>1, outH0.chunkSpan>>1, h/bin, w/bin, nb, bin, nOrientations, softBin, full, M0, M1, O0, O1, offset );

   fxgradHist_c_row_nontrilinear_pixel( M, O, H0, H1, H2, H3, H4, H5, h, w, InM.chunkSpan>>1, outH0.chunkSpan>>1, h/bin, w/bin, nb, bin, nOrientations, softBin, full, M0, M1, O0, O1, offset );

}


void agg_acfhist_nt_2x2(
               kernel_io_desc InM          /*!< input magnitude.   */,
               kernel_io_desc InO          /*!< input orientation. */,
               kernel_io_desc Inoffset     /*!< index offset array.*/,
               kernel_io_desc outH0        /*!< output histogram.  */,
               kernel_io_desc outH1        /*!< output histogram.  */,
               kernel_io_desc outH2        /*!< output histogram.  */,
               kernel_io_desc outH3        /*!< output histogram.  */,
               kernel_io_desc outH4        /*!< output histogram.  */,
               kernel_io_desc outH5        /*!< output histogram.  */,
               kernel_io_desc Params       /*!< input patameters.  */,
               kernel_io_desc outM0        /*!< temp row buffer for magnitude interpolation component at bin0.*/,
               kernel_io_desc outM1        /*!< temp row buffer for magnitude interpolation component at bin1.*/,
               kernel_io_desc outO0        /*!< temp row buffer of orientation distance to bin0.*/,
               kernel_io_desc outO1        /*!< temp row buffer of orientation distance to bin1.*/
)
{
   // input/output buffer
   vec16u* M             = (vec16u*)InM.pMem;
   vec16u* O             = (vec16u*)InO.pMem; 
   int32_t* offset       = (int32_t*)Inoffset.pMem; 
   
   vec16u *H0 = (vec16u *)outH0.pMem;
   vec16u *H1 = (vec16u *)outH1.pMem;
   vec16u *H2 = (vec16u *)outH2.pMem;
   vec16u *H3 = (vec16u *)outH3.pMem;
   vec16u *H4 = (vec16u *)outH4.pMem;
   vec16u *H5 = (vec16u *)outH5.pMem;


   // chunk width/height
   int32_t h             = InM.chunkHeight; 
   int32_t w             = InM.chunkWidth;
   
   // parameter set
   int32_t *lParams      = (int32_t*)Params.pMem; 
   int32_t full          = lParams[0];
   //assert(bin == 2); 
   int32_t nOrientations = lParams[1]; 
   int32_t bin           = lParams[2];
   int32_t softBin       = lParams[3]; 

   int32_t nb = w/bin*h/bin; 
   // temp row buffer for M/O interpolation calculation
   vec32u* M0           = (vec32u*)outM0.pMem;  
   vec32u* M1           = (vec32u*)outM1.pMem; 
   vec32u* O0           = (vec32u*)outO0.pMem; 
   vec32u* O1           = (vec32u*)outO1.pMem; 


   // histogram calculation(non-trilinear) 
   fxgradHist_c_row_nontrilinear( M, O, H0, H1, H2, H3, H4, H5, h, w, InM.chunkSpan>>1, outH0.chunkSpan>>1, h/bin, w/bin, nb, bin, nOrientations, softBin, full, M0, M1, O0, O1, offset );

}


void agg_acfhist_nt_3x3(
               kernel_io_desc InM          /*!< input magnitude.   */,
               kernel_io_desc InO          /*!< input orientation. */,
               kernel_io_desc Inoffset     /*!< index offset array.*/,
               kernel_io_desc outH0        /*!< output histogram.  */,
               kernel_io_desc outH1        /*!< output histogram.  */,
               kernel_io_desc outH2        /*!< output histogram.  */,
               kernel_io_desc outH3        /*!< output histogram.  */,
               kernel_io_desc outH4        /*!< output histogram.  */,
               kernel_io_desc outH5         /*!< output histogram.  */,
               kernel_io_desc Params       /*!< input patameters.  */,
               kernel_io_desc outM0        /*!< temp row buffer for magnitude interpolation component at bin0.*/,
               kernel_io_desc outM1        /*!< temp row buffer for magnitude interpolation component at bin1.*/,
               kernel_io_desc outO0        /*!< temp row buffer of orientation distance to bin0.*/,
               kernel_io_desc outO1        /*!< temp row buffer of orientation distance to bin1.*/
)
{
   // input/output buffer
   vec16u* M             = (vec16u*)InM.pMem;
   vec16u* O             = (vec16u*)InO.pMem; 
   int32_t* offset       = (int32_t*)Inoffset.pMem; 
   
   vec16u *H0 = (vec16u *)outH0.pMem;
   vec16u *H1 = (vec16u *)outH1.pMem;
   vec16u *H2 = (vec16u *)outH2.pMem;
   vec16u *H3 = (vec16u *)outH3.pMem;
   vec16u *H4 = (vec16u *)outH4.pMem;
   vec16u *H5 = (vec16u *)outH5.pMem;
 
   // chunk width/height
   int32_t h             = InM.chunkHeight; 
   int32_t w             = InM.chunkWidth;
   
   // parameter set
   int32_t *lParams      = (int32_t*)Params.pMem; 
   int32_t full          = lParams[0];
   //assert(bin == 3); 
   int32_t nOrientations = lParams[1]; 
   int32_t bin           = lParams[2];
   int32_t softBin       = lParams[3]; 

   int32_t nb = w/bin*h/bin; 
   // temp row buffer for M/O interpolation calculation
   vec32u* M0           = (vec32u*)outM0.pMem;  
   vec32u* M1           = (vec32u*)outM1.pMem; 
   vec32u* O0           = (vec32u*)outO0.pMem; 
   vec32u* O1           = (vec32u*)outO1.pMem; 


   // histogram calculation(non-trilinear) 
   fxgradHist_c_row_nontrilinear( M, O, H0, H1, H2, H3, H4, H5, h, w, InM.chunkSpan>>1, outH0.chunkSpan>>1, h/bin, w/bin, nb, bin, nOrientations, softBin, full, M0, M1, O0, O1, offset );
}


void agg_acfhist_nt_4x4(
               kernel_io_desc InM          /*!< input magnitude.   */,
               kernel_io_desc InO          /*!< input orientation. */,
               kernel_io_desc Inoffset     /*!< index offset array.*/,
               kernel_io_desc outH0        /*!< output histogram.  */,
               kernel_io_desc outH1        /*!< output histogram.  */,
               kernel_io_desc outH2        /*!< output histogram.  */,
               kernel_io_desc outH3        /*!< output histogram.  */,
               kernel_io_desc outH4        /*!< output histogram.  */,
               kernel_io_desc outH5        /*!< output histogram.  */,
               kernel_io_desc Params       /*!< input patameters.  */,
               kernel_io_desc outM0        /*!< temp row buffer for magnitude interpolation component at bin0.*/,
               kernel_io_desc outM1        /*!< temp row buffer for magnitude interpolation component at bin1.*/,
               kernel_io_desc outO0        /*!< temp row buffer of orientation distance to bin0.*/,
               kernel_io_desc outO1        /*!< temp row buffer of orientation distance to bin1.*/
)
{
   // input/output buffer
   vec16u* M             = (vec16u*)InM.pMem;
   vec16u* O             = (vec16u*)InO.pMem; 
   int32_t* offset       = (int32_t*)Inoffset.pMem; 
   
   vec16u *H0 = (vec16u *)outH0.pMem;
   vec16u *H1 = (vec16u *)outH1.pMem;
   vec16u *H2 = (vec16u *)outH2.pMem;
   vec16u *H3 = (vec16u *)outH3.pMem;
   vec16u *H4 = (vec16u *)outH4.pMem;
   vec16u *H5 = (vec16u *)outH5.pMem;
 
   // chunk width/height
   int32_t h             = InM.chunkHeight; 
   int32_t w             = InM.chunkWidth;
   
   // parameter set
   int32_t *lParams      = (int32_t*)Params.pMem; 
   int32_t full          = lParams[0];
   //assert(bin == 4); 
   int32_t nOrientations = lParams[1]; 
   int32_t bin           = lParams[2];
   int32_t softBin       = lParams[3]; 

   int32_t nb = w/bin*h/bin; 
   // temp row buffer for M/O interpolation calculation
   vec32u* M0           = (vec32u*)outM0.pMem;  
   vec32u* M1           = (vec32u*)outM1.pMem; 
   vec32u* O0           = (vec32u*)outO0.pMem; 
   vec32u* O1           = (vec32u*)outO1.pMem; 


   // histogram calculation(non-trilinear) 
   fxgradHist_c_row_nontrilinear( M, O, H0, H1, H2, H3, H4, H5, h, w, InM.chunkSpan>>1, outH0.chunkSpan>>1, h/bin, w/bin, nb, bin, nOrientations, softBin, full, M0, M1, O0, O1, offset );

}

// trilinear interpolation histogram calculation kernel
// support bin size [2,3,4] 

void agg_acfhist_ti_2x2(
               kernel_io_desc InM          /*!< input magnitude.   */,
               kernel_io_desc InO          /*!< input orientation. */,
               kernel_io_desc Inoffset     /*!< index offset array.*/,
               kernel_io_desc outH0        /*!< output histogram.  */,
               kernel_io_desc outH1        /*!< output histogram.  */,
               kernel_io_desc outH2        /*!< output histogram.  */,
               kernel_io_desc outH3        /*!< output histogram.  */,
               kernel_io_desc outH4        /*!< output histogram.  */,
               kernel_io_desc outH5        /*!< output histogram.  */,
               kernel_io_desc Params       /*!< input patameters.  */,
               kernel_io_desc outM0        /*!< temp row buffer for magnitude interpolation component at bin0.*/,
               kernel_io_desc outM1        /*!< temp row buffer for magnitude interpolation component at bin1.*/,
               kernel_io_desc outO0        /*!< temp row buffer of orientation distance to bin0.*/,
               kernel_io_desc outO1        /*!< temp row buffer of orientation distance to bin1.*/
)
{
   // input/output buffer
   vec16u* M             = (vec16u*)InM.pMem;
   vec16u* O             = (vec16u*)InO.pMem; 
   int32_t* offset       = (int32_t*)Inoffset.pMem; 
   
   vec16u *H0 = (vec16u *)outH0.pMem;
   vec16u *H1 = (vec16u *)outH1.pMem;
   vec16u *H2 = (vec16u *)outH2.pMem;
   vec16u *H3 = (vec16u *)outH3.pMem;
   vec16u *H4 = (vec16u *)outH4.pMem;
   vec16u *H5 = (vec16u *)outH5.pMem;
 
   // chunk width/height
   int32_t h             = InM.chunkHeight; 
   int32_t w             = InM.chunkWidth;
   
   // parameter set
   int32_t *lParams      = (int32_t*)Params.pMem; 
   int32_t full          = lParams[0];
   int32_t nOrientations = lParams[1]; 
   int32_t bin           = lParams[2];
   int32_t softBin       = lParams[3]; 

   int32_t nb = w/bin*h/bin; 
   // control condition checking
   // trilinear interpolation with bin size == 2  
   //assert(bin == 2 && softBin >= 0 && ((softBin%2) == 1)); 
   // temp row buffer for M/O interpolation calculation
   vec32u* M0           = (vec32u*)outM0.pMem;  
   vec32u* M1           = (vec32u*)outM1.pMem; 
   vec32u* O0           = (vec32u*)outO0.pMem; 
   vec32u* O1           = (vec32u*)outO1.pMem; 


   // histogram calculation(trilinear) 
   fxgradHist_c_row_trilinear( M, O, H0, H1, H2, H3, H4, H5, h, w, InM.chunkSpan>>1, outH0.chunkSpan>>1, w/bin, h/bin, nb, bin, nOrientations, softBin, full, M0, M1, O0, O1 );


}
void agg_acfhist_ti_3x3(
               kernel_io_desc InM          /*!< input magnitude.   */,
               kernel_io_desc InO          /*!< input orientation. */,
               kernel_io_desc Inoffset     /*!< index offset array.*/,
               kernel_io_desc outH0        /*!< output histogram.  */,
               kernel_io_desc outH1        /*!< output histogram.  */,
               kernel_io_desc outH2        /*!< output histogram.  */,
               kernel_io_desc outH3        /*!< output histogram.  */,
               kernel_io_desc outH4        /*!< output histogram.  */,
               kernel_io_desc outH5        /*!< output histogram.  */,
               kernel_io_desc Params       /*!< input patameters.  */,
               kernel_io_desc outM0        /*!< temp row buffer for magnitude interpolation component at bin0.*/,
               kernel_io_desc outM1        /*!< temp row buffer for magnitude interpolation component at bin1.*/,
               kernel_io_desc outO0        /*!< temp row buffer of orientation distance to bin0.*/,
               kernel_io_desc outO1        /*!< temp row buffer of orientation distance to bin1.*/
)
{
   // input/output buffer
   vec16u* M             = (vec16u*)InM.pMem;
   vec16u* O             = (vec16u*)InO.pMem; 
   int32_t* offset       = (int32_t*)Inoffset.pMem; 
  
   vec16u *H0 = (vec16u *)outH0.pMem;
   vec16u *H1 = (vec16u *)outH1.pMem;
   vec16u *H2 = (vec16u *)outH2.pMem;
   vec16u *H3 = (vec16u *)outH3.pMem;
   vec16u *H4 = (vec16u *)outH4.pMem;
   vec16u *H5 = (vec16u *)outH5.pMem;
 
   
   // chunk width/height
   int32_t h             = InM.chunkHeight; 
   int32_t w             = InM.chunkWidth;
   
   // parameter set
   int32_t *lParams      = (int32_t*)Params.pMem; 
   int32_t full          = lParams[0];
   int32_t nOrientations = lParams[1]; 
   int32_t bin           = lParams[2];
   int32_t softBin       = lParams[3]; 

   int32_t nb = w/bin*h/bin; 
   // control condition checking
   // trilinear interpolation with bin size == 3  
   //assert(bin == 3 && softBin >= 0 && ((softBin%2) == 1)); 
   // temp row buffer for M/O interpolation calculation
   vec32u* M0           = (vec32u*)outM0.pMem;  
   vec32u* M1           = (vec32u*)outM1.pMem; 
   vec32u* O0           = (vec32u*)outO0.pMem; 
   vec32u* O1           = (vec32u*)outO1.pMem; 


   // histogram calculation(trilinear) 
   fxgradHist_c_row_trilinear( M, O, H0, H1, H2, H3, H4, H5, h, w, InM.chunkSpan>>1, outH0.chunkSpan>>1, w/bin, h/bin, nb, bin, nOrientations, softBin, full, M0, M1, O0, O1 );

}
void agg_acfhist_ti_4x4(
               kernel_io_desc InM          /*!< input magnitude.   */,
               kernel_io_desc InO          /*!< input orientation. */,
               kernel_io_desc Inoffset     /*!< index offset array.*/,
               kernel_io_desc outH0        /*!< output histogram.  */,
               kernel_io_desc outH1        /*!< output histogram.  */,
               kernel_io_desc outH2        /*!< output histogram.  */,
               kernel_io_desc outH3        /*!< output histogram.  */,
               kernel_io_desc outH4        /*!< output histogram.  */,
               kernel_io_desc outH5        /*!< output histogram.  */,
               kernel_io_desc Params       /*!< input patameters.  */,
               kernel_io_desc outM0        /*!< temp row buffer for magnitude interpolation component at bin0.*/,
               kernel_io_desc outM1        /*!< temp row buffer for magnitude interpolation component at bin1.*/,
               kernel_io_desc outO0        /*!< temp row buffer of orientation distance to bin0.*/,
               kernel_io_desc outO1        /*!< temp row buffer of orientation distance to bin1.*/
)
{
   // input/output buffer
   vec16u* M             = (vec16u*)InM.pMem;
   vec16u* O             = (vec16u*)InO.pMem; 
   int32_t* offset       = (int32_t*)Inoffset.pMem; 
   

   vec16u *H0 = (vec16u *)outH0.pMem;
   vec16u *H1 = (vec16u *)outH1.pMem;
   vec16u *H2 = (vec16u *)outH2.pMem;
   vec16u *H3 = (vec16u *)outH3.pMem;
   vec16u *H4 = (vec16u *)outH4.pMem;
   vec16u *H5 = (vec16u *)outH5.pMem;
   
   // chunk width/height
   int32_t h             = InM.chunkHeight; 
   int32_t w             = InM.chunkWidth;
   
   // parameter set
   int32_t *lParams      = (int32_t*)Params.pMem; 
   int32_t full          = lParams[0];
   int32_t nOrientations = lParams[1]; 
   int32_t bin           = lParams[2];
   int32_t softBin       = lParams[3]; 

  
   int32_t nb = w/bin*h/bin; 
   // control condition checking
   // trilinear interpolation with bin size == 4  
   //assert(bin == 4 && softBin >= 0 && ((softBin%2) == 1)); 
   // temp row buffer for M/O interpolation calculation
   vec32u* M0           = (vec32u*)outM0.pMem;  
   vec32u* M1           = (vec32u*)outM1.pMem; 
   vec32u* O0           = (vec32u*)outO0.pMem; 
   vec32u* O1           = (vec32u*)outO1.pMem; 


   // histogram calculation(trilinear) 
   fxgradHist_c_row_trilinear( M, O, H0, H1, H2, H3, H4, H5, h, w, InM.chunkSpan>>1, outH0.chunkSpan>>1, w/bin, h/bin, nb, bin, nOrientations, softBin, full, M0, M1, O0, O1 );

}


#endif /* ACF_KENREL_IMPLEMENTATION */



