/*
 * Copyright (c) 2014-2016 Freescale Semiconductor
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef JPEGDCDCORE_H
#define JPEGDCDCORE_H


/************************************************
    JPEGDCD Register Map
*************************************************/
typedef struct {
  uint32_t      STR1_SRAM_PTR;  /* Stream 1 SRAM Pointer Register */
  uint32_t      STR1_SRAM_LEN_VAL; /* Stream 1 SRAM Length Value Register */
  uint32_t      STR1_PTR_FIFO[16]; /* Stream 1 Pointer FIFO Register */
  uint32_t      STR1_LEN_VAL_FIFO[16]; /* Stream 1 Length Value FIFO register */
  uint32_t      STR2_SRAM_PTR;  /* Stream 2 SRAM Pointer register */
  uint32_t      STR2_SRAM_LEN_VAL; /* Stream 2 SRAM Length Value Register */
  uint32_t      STR2_PTR_FIFO[16]; /* Stream 2 Pointer FIFO Register */
  uint32_t      STR2_LEN_VAL_FIFO[16]; /* Stream 2 Length Value FIFO Register */
  uint32_t      STR3_SRAM_PTR;  /* Stream 3 SRAM Pointer Register */
  uint32_t      STR3_SRAM_LEN_VAL; /* Stream 3 SRAM Length Value Register */
  uint32_t      STR3_PTR_FIFO[16]; /* Stream 3 Pointer FIFO Register */
  uint32_t      STR3_LEN_VAL_FIFO[16]; /* Stream 3 Length Value FIFO Register */
  uint32_t      STR4_SRAM_PTR;  /* Stream 4 SRAM Pointer Register */
  uint32_t      STR4_SRAM_LEN_VAL; /* Stream 4 SRAM Length Value Register */
  uint32_t      STR4_PTR_FIFO[16]; /* Stream 4 Pointer FIFO Register */
  uint32_t      STR4_LEN_VAL_FIFO[16]; /* Stream 4 Length Value FIFO Register */
  uint8_t       JPEGDCD_reserved0[16];
  uint32_t      BUF_C1_ADDR;    /* Buffer Component 1 Address Register */
  uint32_t      BUF_C2_ADDR;    /* Buffer Component 2 Address Register */
  uint32_t      BUF_C3_ADDR;    /* Buffer Component 3 Address Register */
  uint32_t      BUF_C4_ADDR;    /* Buffer Component 4 Address Register */
  uint32_t      NRLINES_B2L;    /* Block to Lines Number of Lines Register */
  uint32_t      NRLINES_B2L_C4; /* Block to Lines Number of Lines for Component 4 Register */
  uint32_t      TIMEOUT;            /* Timeout Register */
  uint8_t       JPEGDCD_reserved1[4];
  uint32_t      W_CTRL1;            /* Wrapper Control Register */
  uint32_t      W_CTRL2;            /* Wrapper Control Register 2 */
  uint8_t       JPEGDCD_reserved2[4];
  uint32_t      INTR_EN;            /* Interrupt Enable */
  uint32_t      W_STATUS;          /* Wrapper Status Register */
  uint32_t      E_STATUS;          /* Error Status Register */
  uint32_t      RST_INTVL;        /* Restart Interval */
  uint32_t      IMG_SZ_STR1;      /* Image Size of Stream 1 */
  uint32_t      IMG_SZ_STR2;      /* Image Size of Stream 2 */
  uint32_t      IMG_SZ_STR3;      /* Image Size of Stream 3 */
  uint32_t      IMG_SZ_STR4;      /* Image Size of Stream 4 */
  uint32_t      SMPL_FCTR_STR1; /* Sampling Factor Stream 1 */
  uint32_t      SMPL_FCTR_STR2; /* Sampling Factor Stream 2 */
  uint32_t      SMPL_FCTR_STR3; /* Sampling Factor Stream 3 */
  uint32_t      SMPL_FCTR_STR4; /* Sampling Factor Stream 4 */
  uint32_t      TST_PXL_LOC;    /* Test Pixel Location */
  uint32_t      TST_PXL_LOC1;  /* Test Pixel Location 1 */
  uint32_t      TST_LINE_LUMA_STR1; /* Test Line Luma Pixel Value for Stream 1 */
  uint32_t      TST_LINE_Cb_STR1; /* Test Line Cb Pixel Value for Stream 1 */
  uint32_t      TST_LINE_Cr_STR1; /* Test Line Cr Pixel Value for Stream 1 */
  uint32_t      TST_LINE_LUMA_STR2; /* Test Line Luma Pixel Value for Stream 2 */
  uint32_t      TST_LINE_Cb_STR2; /* Test Line Cb Pixel Value for Stream 2 */
  uint32_t      TST_LINE_Cr_STR2; /* Test Line Cr Pixel Value for Stream 2 */
  uint32_t      TST_LINE_LUMA_STR3; /* Test Line Luma Pixel Value for Stream 3 */
  uint32_t      TST_LINE_Cb_STR3; /* Test Line Cb Pixel Value for Stream 3 */
  uint32_t      TST_LINE_Cr_STR3; /* Test Line Cr Pixel Value for Stream 3 */
  uint32_t      TST_LINE_LUMA_STR4; /* Test Line Luma Pixel Value for Stream 4 */
  uint32_t      TST_LINE_Cb_STR4; /* Test Line Luma Cb Pixel Value for Stream 4 */
  uint32_t      TST_LINE_Cr_STR4; /* Test Line Cr Pixel Value for Stream 4 */
  uint8_t       JPEGDCD_reserved3[28];
  uint32_t      CTRL;                  /* Control Register */
  uint32_t      STATUS1;            /* Status 1 Register */
  uint32_t      STATUS2;            /* Status 2 Register */
  uint32_t      STATUS3;            /* Status 3 Register */
  uint32_t      STATUS4;            /* Status 4 Register */
  uint32_t      STATUS5;            /* Status 5 Register */
  uint32_t      STATUS6;            /* Status 6 Register */
  uint32_t      STATUS7;            /* Status 7 Register */
  uint32_t      STATUS8;            /* Status 8 Register */
  uint32_t      STATUS9;            /* Status 9 Register */
  uint32_t      STATUS10;          /* Status 10 Register */
  uint32_t      STATUS11;          /* Status 11 Register */
  uint32_t      STATUS12;          /* Status 12 Register */
  uint32_t      STATUS13;          /* Status 13 Register */
} JPEGDCD_REGS;


/************************************************
    JPEGDCD Register Masks
*************************************************/
#define JPEGDCD_OUT_B2LINES_C23_SHIFT       (16)

#define JPEGDCD_WCTRL1_STREAMNUM_SHIFT      (8)
#define JPEGDCD_WCTRL1_STREAMNUM_MASK       ((uint32_t)(0x00000007) << (JPEGDCD_WCTRL1_STREAMNUM_SHIFT))
#define JPEGDCD_WCTRL1_BNKSTRD_SHIFT        (1)
#define JPEGDCD_WCTRL1_BNKSTRD_MASK         ((uint32_t)(0x00000007) << (JPEGDCD_WCTRL1_BNKSTRD_SHIFT))
#define JPEGDCD_WCTRL1_SWRST_MASK           ((uint32_t)0x00000001)

#define JPEGDCD_WCTRL2_COMMODE_SHIFT        (30)
#define JPEGDCD_WCTRL2_COMMODE_MASK         ((uint32_t)(0x00000001) << (JPEGDCD_WCTRL2_COMMODE_SHIFT))
#define JPEGDCD_WCTRL2_SAMPLEOUTBUF_SHIFT   (29)
#define JPEGDCD_WCTRL2_SAMPLEOUTBUF_MASK    ((uint32_t)(0x00000001) << (JPEGDCD_WCTRL2_SAMPLEOUTBUF_SHIFT))
#define JPEGDCD_WCTRL2_JPEGPRE4_SHIFT       (19)
#define JPEGDCD_WCTRL2_JPEGPRE4_MASK        ((uint32_t)(0x00000001) << (JPEGDCD_WCTRL2_JPEGPRE4_SHIFT))
#define JPEGDCD_WCTRL2_JPEGCOMPNUM4_SHIFT   (16)
#define JPEGDCD_WCTRL2_JPEGCOMPNUM4_MASK    ((uint32_t)(0x00000007) << (JPEGDCD_WCTRL2_JPEGCOMPNUM4_SHIFT))
#define JPEGDCD_WCTRL2_JPEGWRCFG4_SHIFT     (15)
#define JPEGDCD_WCTRL2_JPEGWRCFG4_MASK      ((uint32_t)(0x00000001) << (JPEGDCD_WCTRL2_JPEGWRCFG4_SHIFT))
#define JPEGDCD_WCTRL2_JPEGPRE3_SHIFT       (14)
#define JPEGDCD_WCTRL2_JPEGPRE3_MASK        ((uint32_t)(0x00000001) << (JPEGDCD_WCTRL2_JPEGPRE3_SHIFT))
#define JPEGDCD_WCTRL2_JPEGCOMPNUM3_SHIFT   (11)
#define JPEGDCD_WCTRL2_JPEGCOMPNUM3_MASK    ((uint32_t)(0x00000007) << (JPEGDCD_WCTRL2_JPEGCOMPNUM3_SHIFT))
#define JPEGDCD_WCTRL2_JPEGWRCFG3_SHIFT     (10)
#define JPEGDCD_WCTRL2_JPEGWRCFG3_MASK      ((uint32_t)(0x00000001) << (JPEGDCD_WCTRL2_JPEGWRCFG3_SHIFT))
#define JPEGDCD_WCTRL2_JPEGPRE2_SHIFT       (9)
#define JPEGDCD_WCTRL2_JPEGPRE2_MASK        ((uint32_t)(0x00000001) << (JPEGDCD_WCTRL2_JPEGPRE2_SHIFT))
#define JPEGDCD_WCTRL2_JPEGCOMPNUM2_SHIFT   (6)
#define JPEGDCD_WCTRL2_JPEGCOMPNUM2_MASK    ((uint32_t)(0x00000007) << (JPEGDCD_WCTRL2_JPEGCOMPNUM2_SHIFT))
#define JPEGDCD_WCTRL2_JPEGWRCFG2_SHIFT     (5)
#define JPEGDCD_WCTRL2_JPEGWRCFG2_MASK      ((uint32_t)(0x00000001) << (JPEGDCD_WCTRL2_JPEGWRCFG2_SHIFT))
#define JPEGDCD_WCTRL2_JPEGPRE1_SHIFT       (4)
#define JPEGDCD_WCTRL2_JPEGPRE1_MASK        ((uint32_t)(0x00000001) << (JPEGDCD_WCTRL2_JPEGPRE1_SHIFT))
#define JPEGDCD_WCTRL2_JPEGCOMPNUM1_SHIFT   (1)
#define JPEGDCD_WCTRL2_JPEGCOMPNUM1_MASK    ((uint32_t)(0x00000007) << (JPEGDCD_WCTRL2_JPEGCOMPNUM1_SHIFT))
#define JPEGDCD_WCTRL2_JPEGWRCFG1_SHIFT     (0)
#define JPEGDCD_WCTRL2_JPEGWRCFG1_MASK      ((uint32_t)(0x00000001) << (JPEGDCD_WCTRL2_JPEGWRCFG1_SHIFT))

#define JPEGDCD_INTREN_WTRMARKLEV_SHIFT     (27)
#define JPEGDCD_INTREN_WTRMARKLEV_MASK      ((uint32_t)(0x0000000F) << (JPEGDCD_INTREN_WTRMARKLEV_SHIFT))
#define JPEGDCD_INTREN_WTRMARKLEV_SIZE      (0x0Fu)

#define JPEGDCD_WRST_STREAM_SHIFT           (29)
#define JPEGDCD_WRST_STREAM_MASK            ((uint32_t)(0x00000007) << JPEGDCD_WRST_STREAM_SHIFT)
#define JPEGDCD_WRST_SPACEFIFO4_SHIFT       (19)
#define JPEGDCD_WRST_SPACEFIFO4_MASK        ((uint32_t)(0x0000001F) << (JPEGDCD_WRST_SPACEFIFO4_SHIFT))
#define JPEGDCD_WRST_SPACEFIFO3_SHIFT       (13)
#define JPEGDCD_WRST_SPACEFIFO3_MASK        ((uint32_t)(0x0000001F) << (JPEGDCD_WRST_SPACEFIFO3_SHIFT))
#define JPEGDCD_WRST_SPACEFIFO2_SHIFT       (7)
#define JPEGDCD_WRST_SPACEFIFO2_MASK        ((uint32_t)(0x0000001F) << (JPEGDCD_WRST_SPACEFIFO2_SHIFT))
#define JPEGDCD_WRST_SPACEFIFO1_SHIFT       (1)
#define JPEGDCD_WRST_SPACEFIFO1_MASK        ((uint32_t)(0x0000001F) << (JPEGDCD_WRST_SPACEFIFO1_SHIFT))

#define JPEGDCD_JPEGERR_TIMEOUT_SHIFT       (6)
#define JPEGDCD_JPEGERR_TIMEOUT_MASK        ((uint32_t)(0x00000001) << (JPEGDCD_JPEGERR_TIMEOUT_SHIFT))
#define JPEGDCD_JPEGERR_DRI_SHIFT           (5)
#define JPEGDCD_JPEGERR_DRI_MASK            ((uint32_t)(0x00000001) << (JPEGDCD_JPEGERR_DRI_SHIFT))
#define JPEGDCD_JPEGERR_STREAM4_SHIFT       (4)
#define JPEGDCD_JPEGERR_STREAM4_MASK        ((uint32_t)(0x00000001) << (JPEGDCD_JPEGERR_STREAM4_SHIFT))
#define JPEGDCD_JPEGERR_STREAM3_SHIFT       (3)
#define JPEGDCD_JPEGERR_STREAM3_MASK        ((uint32_t)(0x00000001) << (JPEGDCD_JPEGERR_STREAM3_SHIFT))
#define JPEGDCD_JPEGERR_STREAM2_SHIFT       (2)
#define JPEGDCD_JPEGERR_STREAM2_MASK        ((uint32_t)(0x00000001) << (JPEGDCD_JPEGERR_STREAM2_SHIFT))
#define JPEGDCD_JPEGERR_STREAM1_SHIFT       (1)
#define JPEGDCD_JPEGERR_STREAM1_MASK        ((uint32_t)(0x00000001) << (JPEGDCD_JPEGERR_STREAM1_SHIFT))
#define JPEGDCD_JPEGERR_CORE_SHIFT          (0)
#define JPEGDCD_JPEGERR_CORE_MASK           ((uint32_t)(0x00000001) << (JPEGDCD_JPEGERR_CORE_SHIFT))
#define JPEGDCD_JPEGERR_BYTE                (JPEGDCD_JPEGERR_TIMEOUT_MASK \
                                            | JPEGDCD_JPEGERR_DRI_MASK | JPEGDCD_JPEGERR_CORE_MASK \
                                            | JPEGDCD_JPEGERR_STREAM4_MASK | JPEGDCD_JPEGERR_STREAM3_MASK \
                                            | JPEGDCD_JPEGERR_STREAM2_MASK | JPEGDCD_JPEGERR_STREAM1_MASK)

/* Interrupts mask for enable bits and flags */
#define JPEGDCD_INTR_CFGERR_SHIFT           (31)
#define JPEGDCD_INTR_CFGERR_MASK            ((uint32_t)(0x00000001) << (JPEGDCD_INTR_CFGERR_SHIFT))
#define JPEGDCD_INTR_STARTFRAME_SHIFT       (26)
#define JPEGDCD_INTR_STARTFRAME_MASK        ((uint32_t)(0x00000001) << (JPEGDCD_INTR_STARTFRAME_SHIFT))
#define JPEGDCD_INTR_STOPFRAME_SHIFT        (25)
#define JPEGDCD_INTR_STOPFRAME_MASK         ((uint32_t)(0x00000001) << (JPEGDCD_INTR_STOPFRAME_SHIFT))
#define JPEGDCD_INTR_MCUROWEND_SHIFT        (24)
#define JPEGDCD_INTR_MCUROWEND_MASK         ((uint32_t)(0x00000001) << (JPEGDCD_INTR_MCUROWEND_SHIFT))
#define JPEGDCD_INTR_WTRMRKFIFO4_SHIFT      (18)
#define JPEGDCD_INTR_WTRMRKFIFO4_MASK       ((uint32_t)(0x00000001) << (JPEGDCD_INTR_WTRMRKFIFO4_SHIFT))
#define JPEGDCD_INTR_WTRMRKFIFO3_SHIFT      (12)
#define JPEGDCD_INTR_WTRMRKFIFO3_MASK       ((uint32_t)(0x00000001) << (JPEGDCD_INTR_WTRMRKFIFO3_SHIFT))
#define JPEGDCD_INTR_WTRMRKFIFO2_SHIFT      (6)
#define JPEGDCD_INTR_WTRMRKFIFO2_MASK       ((uint32_t)(0x00000001) << (JPEGDCD_INTR_WTRMRKFIFO2_SHIFT))
#define JPEGDCD_INTR_WTRMRKFIFO1_SHIFT      (0)
#define JPEGDCD_INTR_WTRMRKFIFO1_MASK       ((uint32_t)(0x00000001) << (JPEGDCD_INTR_WTRMRKFIFO1_SHIFT))
#define JPEGDCD_INTR_FLAGS_MASK             (JPEGDCD_INTR_STARTFRAME_MASK | \
                                            JPEGDCD_INTR_STOPFRAME_MASK  | \
                                            JPEGDCD_INTR_MCUROWEND_MASK  | \
                                            JPEGDCD_INTR_WTRMRKFIFO4_MASK |\
                                            JPEGDCD_INTR_WTRMRKFIFO3_MASK |\
                                            JPEGDCD_INTR_WTRMRKFIFO2_MASK |\
                                            JPEGDCD_INTR_WTRMRKFIFO1_MASK)

#define JPEGDCD_INTR_ALL_MASK               (JPEGDCD_INTR_FLAGS_MASK|JPEGDCD_INTR_CFGERR_MASK)

#define JPEGDCD_JPEG_IMGSIZE_Y_SHIFT        (16)
#define JPEGDCD_JPEG_IMGSIZE_X_SHIFT        (0)

#define JPEGDCD_JPEG_SAMPFAC_V4_SHIFT       (28)
#define JPEGDCD_JPEG_SAMPFAC_V3_SHIFT       (24)
#define JPEGDCD_JPEG_SAMPFAC_V2_SHIFT       (20)
#define JPEGDCD_JPEG_SAMPFAC_V1_SHIFT       (16)

#define JPEGDCD_JPEG_SAMPFAC_H4_SHIFT       (12)
#define JPEGDCD_JPEG_SAMPFAC_H3_SHIFT       (8)
#define JPEGDCD_JPEG_SAMPFAC_H2_SHIFT       (4)
#define JPEGDCD_JPEG_SAMPFAC_H1_SHIFT       (0)

#define JPEGDCD_JPEG_SAMPFAC_MASK           ((uint32_t)0x0F)

#define JPEGDCD_JPEG_GO_SHIFT               (2)
#define JPEGDCD_JPEG_GO_MASK                ((uint32_t)(0x00000001) << (JPEGDCD_JPEG_GO_SHIFT))

#define JPEGDCD_JPEGST_COMPID_SHIFT         (8)
#define JPEGDCD_JPEGST_COMPID_MASK          ((uint32_t)(0x000000FF) << (JPEGDCD_JPEGST_COMPID_SHIFT))
#define JPEGDCD_JPEGST_SAMPLEH_SHIFT        (5)
#define JPEGDCD_JPEGST_SAMPLEH_MASK         ((uint32_t)(0x00000007) << (JPEGDCD_JPEGST_SAMPLEH_SHIFT))
#define JPEGDCD_JPEGST_SAMPLEV_SHIFT        (2)
#define JPEGDCD_JPEGST_SAMPLEV_MASK         ((uint32_t)(0x00000007) << (JPEGDCD_JPEGST_SAMPLEV_SHIFT))
#define JPEGDCD_JPEGST_QOANTT_SHIFT         (0)
#define JPEGDCD_JPEGST_QOANTT_MASK          ((uint32_t)(0x00000001) << (JPEGDCD_JPEGST_QOANTT_SHIFT))

#define JPEGDCD_JPEGST_PRECISION_SHIFT      (8)
#define JPEGDCD_JPEGST_PRECISION_MASK       ((uint32_t)(0x000000FF) << (JPEGDCD_JPEGST_PRECISION_SHIFT))
#define JPEGDCD_JPEGST_NUMOFCOMP_SHIFT      (0)
#define JPEGDCD_JPEGST_NUMOFCOMP_MASK       ((uint32_t)(0x000000FF) << (JPEGDCD_JPEGST_NUMOFCOMP_SHIFT))


/************************************************
    JPEGDCDDCD Functions Low Level
*************************************************/
void JPEGdcd_inputstream_cfg(JPEGDCD_REGS *reg_base, const STREAM_DATA_CONFIG * const data_config);
void JPEGdcd_fifostatus_get(JPEGDCD_REGS *reg_base, STREAM_FIFO_STATUS * const fifo_st);
void JPEGdcd_fifodata_get(const JPEGDCD_REGS *reg_base, STREAM_DATA_CONFIG * const pck_cfgdata);
void JPEGdcd_fifo_wtmklevel_set(JPEGDCD_REGS *reg_base, const uint8_t wtmark);
void JPEGdcd_output_cfg(JPEGDCD_REGS *reg_base, const OUTPUT_BUFFCFG * const output_cfg);
void JPEGdcd_outsamplemode_set(JPEGDCD_REGS *reg_base, const JPEGDCD_BOOL outputregsmpl_mode);
void JPEGdcd_bankstride_set(JPEGDCD_REGS *reg_base, const uint8_t bytes_num);
void JPEGdcd_bankstride_get(const JPEGDCD_REGS *reg_base, uint8_t * const bytes_num);
void JPEGdcd_jpeg_configstart(JPEGDCD_REGS *reg_base);
void JPEGdcd_jpeg_timeoutset(JPEGDCD_REGS *reg_base, const uint32_t jpeg_errtimeout);
void JPEGdcd_jpeg_dcdenable(JPEGDCD_REGS *reg_base, const JPEGDCD_BOOL go_bit);
void JPEGdcd_jpeg_streamdatacfg(JPEGDCD_REGS *reg_base, const STREAM_JPEG_PARAM * const image_data);
void JPEGdcd_jpeg_streamnum_set(JPEGDCD_REGS *reg_base, const uint8_t num_of_streams);
void JPEGdcd_jpeg_streamnum_get(const JPEGDCD_REGS *reg_base, uint8_t * const num_of_streams);
void JPEGdcd_jpeg_mode_set(JPEGDCD_REGS *reg_base, const COMPAT_MODE switch_m);
void JPEGdcd_jpeg_mode_get(const JPEGDCD_REGS *reg_base, COMPAT_MODE * const switch_m);
void JPEGdcd_jpeg_swreset(JPEGDCD_REGS *reg_base);
void JPEGdcd_jpeg_streamactive_get(const JPEGDCD_REGS *reg_base, STREAM_ID * const str_no);
void JPEGdcd_jpeg_rstinterval_set(JPEGDCD_REGS *reg_base, const uint16_t lines_num);
void JPEGdcd_jpeg_rstinterval_get(const JPEGDCD_REGS *reg_base, uint16_t * const lines_num);
void JPEGdcd_jpeg_framest_get(JPEGDCD_REGS *reg_base, JPEGDCD_FRAMEST * const frame_st);
void JPEGdcd_jpeg_cfgerr_get(const JPEGDCD_REGS *reg_base, JPEGDCD_CFGERR * const cfg_err);
void JPEGdcd_jpeg_mcurow_get(JPEGDCD_REGS *reg_base, JPEGDCD_BOOL * const mcurow_end_count);
void JPEGdcd_jpeg_dcdimage_get(const JPEGDCD_REGS *reg_base, JPEGDCD_DECODED_IMAGE * const dcd_image);
void JPEGdcd_interrupt_ctrl(JPEGDCD_REGS *reg_base, const uint32_t intr_mask);
void JPEGdcd_interrupt_get(const JPEGDCD_REGS *reg_base, uint32_t * const intr_mask);

#endif /* JPEGDCDCORE_H */
