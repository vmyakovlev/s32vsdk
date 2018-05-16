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

#ifndef HRSZHAL_H
#define HRSZHAL_H

// #include "GlobalBuildOptions.h"
#include "global_errors.h"
////#include "apex_cfg.h"

////#include "apex_642cl_hrsz_map.h"
////#include "apex_642cl_map.h"

#define HRSZ_TIMEOUT   20000 // default timeout for polling

#define HRSZ_MAX_CORE   2

// #define HRSZ_MAX_NU_OF_FORMAT              9
#define HRSZ_MAX_AVG_RATIO                 5
#define HRSZ_MAX_LPF_PARAM                 11

#define HRSZ_DEFAULT_LPF_ROUNDING_FACTOR   3
#define HRSZ_DEFAULT_PPF_ROUNDING_FACTOR   3
////#define HRSZ_SIZE_OF_CH_CFG_DATA           sizeof(APEX_642CL_HRSZ_CORE_DCR)
// #define HRSZ_SIZE_OF_CH_CFG_DATA   0x70

#define HRSZ_IRQ_DONE              0x01
#define HRSZ_IRQ_IN_FIFO_RD_ERR    0x02
#define HRSZ_IRQ_IN_FIFO_WR_ERR    0x04
#define HRSZ_IRQ_OUT_FIFO_RD_ERR   0x08
#define HRSZ_IRQ_OUT_FIFO_WR_ERR   0x10

#define HRSZ_IRQ_ALL_ERROR         (HRSZ_IRQ_IN_FIFO_RD_ERR | HRSZ_IRQ_IN_FIFO_WR_ERR | \
                                    HRSZ_IRQ_OUT_FIFO_RD_ERR | HRSZ_IRQ_OUT_FIFO_WR_ERR)

typedef enum
{
   HRSZ_DATA_FORMAT_U8 = 0,
   HRSZ_DATA_FORMAT_U10,
   HRSZ_DATA_FORMAT_U12,
   HRSZ_DATA_FORMAT_U14,
   HRSZ_DATA_FORMAT_U16,
   HRSZ_DATA_FORMAT_S8,
   HRSZ_DATA_FORMAT_S9,
   HRSZ_DATA_FORMAT_S10,
   HRSZ_DATA_FORMAT_S16,
   HRSZ_DATA_FORMAT_MAX
} HRSZ_DATA_FORMAT;

typedef struct
{
   // Src information
   HRSZ_DATA_FORMAT src_format;
   unsigned int     src_width;  // Width in pixels
   unsigned int     src_height; // Number of lines
   unsigned int     src_h_offset;
   unsigned int     src_v_offset;
   unsigned int     src_h_valid;
   unsigned int     src_v_valid;

   // Dst information
   HRSZ_DATA_FORMAT dst_format;
   unsigned int     dst_scale_width;
   unsigned int     dst_width;  // Width in pixels
   unsigned int     dst_height; // Number of lines
   unsigned int     dst_h_offset;
   unsigned int     dst_v_offset;
   unsigned int     dst_h_valid;
   unsigned int     dst_v_valid;

   // unsigned int dst_clamp_level;
   unsigned short dst_clamp_min;
   unsigned short dst_clamp_max;

   unsigned int en_lpf;

   // Polyphase filter
   unsigned int lpf_rounding_factor;
   unsigned int ppf_rounding_factor;

   // Low Pass Filter
   unsigned short lpfCoeff[HRSZ_MAX_LPF_PARAM];
} HRSZ_CFG;
// =============================================================================
// HRSZ HAL API Prototypes
// =============================================================================
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// Please do not use these HAL APIs directly!
#if (defined(__HRSZ_HAL_SCOPE__) || defined(__HRSZ_DRV_SCOPE__) || defined(__APU_SCOPE__))

LIB_RESULT   hrsz_hal_chk_hw(void);
unsigned int hrsz_hal_get_max_channel(void);
////LIB_RESULT   hrsz_hal_init(void);
void         hrsz_hal_flush_fifo(unsigned int core_id);
LIB_RESULT   hrsz_hal_en(unsigned int core_id);
LIB_RESULT   hrsz_hal_dis(unsigned int core_id);
void         hrsz_hal_set_src_woi(unsigned int core_id,
                                  unsigned int width,
                                  unsigned int height,
                                  unsigned int h_offset,
                                  unsigned int v_offset,
                                  unsigned int h_valid,
                                  unsigned int v_valid);
void hrsz_hal_set_dst_woi(unsigned int core_id,
                          unsigned int width,
                          unsigned int height,
                          unsigned int h_offset,
                          unsigned int v_offset,
                          unsigned int h_valid,
                          unsigned int v_valid);
#if 0
void hrsz_hal_set_inst(unsigned int core_id,
                       unsigned int in_format,
                       unsigned int out_format,
                       unsigned int en_lpf,
                       unsigned int en_ppf);
#endif
void hrsz_hal_set_data_format(unsigned int core_id,
                              unsigned int in_format,
                              unsigned int out_format);
#if 0
void hrsz_hal_set_clamp(unsigned int core_id, unsigned short format, HRSZ_CLAMP_LEVEL level);
void hrsz_hal_start(unsigned int core_id,
                    unsigned int in_format,
                    unsigned int out_format,
                    unsigned int en_lpf,
                    unsigned int en_ppf);
#else
void hrsz_hal_start(unsigned int core_id);
#endif
void hrsz_hal_set_lpf_coeff(unsigned int   core_id,
                            unsigned char *pLpfCoeff,
                            unsigned int   rounding_factor);
void  hrsz_hal_set_avg_mode(unsigned int core_id, unsigned char avg_mode);
float hrsz_hal_get_scale_factor(unsigned int in_res, unsigned int out_res);
// void  hrsz_hal_set_ppf_coeff(unsigned int core_id, unsigned int lut_to_use, unsigned int rounding_factor);
////void hrsz_hal_set_ppf(unsigned int core_id,
////                      unsigned int in_res,
////                      unsigned int out_res,
////                      unsigned int rounding_factor);

// LIB_RESULT hrsz_hal_reset(void);
void hrsz_hal_cfg(unsigned int core_id, APEX_642CL_HRSZ_CORE_DCR *pHrszCfg);
// LIB_RESULT   hrsz_hal_wait_polling(int timeout_ticks);

void         hrsz_hal_en_irq(unsigned int core_id, unsigned int mask);  // Unmask irq
void         hrsz_hal_dis_irq(unsigned int core_id, unsigned int mask); // Mask irq
unsigned int hrsz_hal_get_int_mask(unsigned int core_id);
unsigned int hrsz_hal_get_irq(unsigned int core_id);
void         hrsz_hal_clr_irq(unsigned int core_id, unsigned int mask);
unsigned int hrsz_hal_is_en_irq(unsigned int core_id, unsigned int mask);
unsigned int hrsz_hal_get_fifo_status(unsigned int core_id);
void         hrsz_hal_push_fifo(unsigned int core_id, unsigned int data);
unsigned int hrsz_hal_pop_fifo(unsigned int core_id);
//

#else
// Show compiler error message when this header file is included except from sequencer_hal.cpp and
// sequencer_drv.cpp
// #error "Please do not include this sequencer_hal.h header file except sequencer_hal.cpp and sequencer_drv.cpp"
#endif

// Publicly available hrsz_hal functions
LIB_RESULT   hrsz_hal_init(void);
void         hrsz_hal_wait_polling(unsigned int core_id);
int *hrsz_hal_get_cfg_data(uint32_t core_id);
LIB_RESULT hrsz_hal_cfgData(unsigned int core_id, HRSZ_CFG *pCfgData);
LIB_RESULT hrsz_hal_get_sideband(uint32_t *pSideband, HRSZ_CFG *pCfgData);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif /* HRSZHAL_H */
