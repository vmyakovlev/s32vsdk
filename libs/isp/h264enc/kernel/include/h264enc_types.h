/*
 * Copyright (c) 2016 Freescale Semiconductor
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

/*==============================================================================
Revision History:
                        Modification       Tracking
Author (core ID)        Date D/M/Y         Number     Description of Changes
Roman Kubica            13-Oct-2016                   Init version
==============================================================================*/

#ifndef H264ENCTYPES_H
#define H264ENCTYPES_H

#ifdef __cplusplus
extern "C"{
#endif

/**
* @file           h264enc_types.h
* @brief          Declaration of H264 Encoder driver internal functionality
*/

/*==============================================================================
*                               INCLUDE FILES
==============================================================================*/

#include "s32vs234.h"
#include "h264enc.h"

/*==============================================================================
*                                   CONSTANTS
==============================================================================*/

/*==============================================================================
*                          DEFINES AND MACROS
==============================================================================*/

/*==============================================================================
*                                  ENUMS
==============================================================================*/

/*=============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
* ============================================================================*/

/**
* @brief          Declare struct H264ENC_Regs_t
* @details        Declare struct H264ENC_Regs_t
* @pre
* @post
*/
typedef struct H264_ENC_tag H264ENC_Regs_t;


/**
* @brief          Describes one memory buffer.
* @details        Describes one memory buffer.
* @pre
* @post
*/
typedef struct
{
  uint32_t mPhysAddr;
  uint32_t mSize;
} VISION_MemBuf_t;


/**
* @brief          Declare struct H264ENC_Buffers_t
* @details        Declare struct H264ENC_Buffers_t
* @pre
* @post
* @implements     H264ENC_Buffers_t_struct
*/
typedef struct
{
  VISION_MemBuf_t mY; /* input pixel data */
  VISION_MemBuf_t mU;
  VISION_MemBuf_t mV;

  uint32_t mLineNum;

  uint8_t  mBankStride; /* 0-7 (>7 => disabled) */

  VISION_MemBuf_t mBitstream;
} H264ENC_Buffers_t;


/**
* @brief          Defines encoding parameter values.
* @details        Defines encoding parameter values.
* @pre
* @post
* @implements     H264ENC_Config_t_struct
*/
typedef struct
{
  uint8_t  mDataModeChroma; /* Binary offset (0), Two's complement (1) */
  uint8_t  mFetchTrig;      /* Host (1) or Sequencer (0) will trigger */
  uint8_t  mColorFormat;    /* YUV420 (0), Gray (1) */
  uint8_t  mBitWidth;       /* 8 bpp (0), 10 bpp (1), 12 bpp (2) */

  uint32_t mFrameWidth;     /* ceiled to multiple of 16 */
  uint32_t mFrameHeight;    /* ceiled to multiple of 16 */

  uint32_t mInputAlarm;     /* number of MBRW between VI_LINE_INT IRQ */
  uint32_t mNumOutAlarm;    /* number of MBRW without CB_ALARM_INT IRQ to */
                            /* signal VI_LCOUNT_ALARM_INT */
  uint8_t  mFrameRate;      /* in fps */
  uint8_t  mQpInit;
  uint8_t  mQpMin;
  uint8_t  mQpMax;
  uint8_t  mQpFallback;

  uint32_t mBitsPerMbrw;

  uint8_t  mQpIncDiff0;
  uint8_t  mQpIncDiff1;
  uint8_t  mQpIncDiff2;
  uint8_t  mQpIncOffset;
  uint32_t mQpIncThld0;
  uint32_t mQpIncThld1;
  uint32_t mQpIncThld2;

  uint8_t  mQpDecDiff0;
  uint8_t  mQpDecDiff1;
  uint8_t  mQpDecDiff2;
  uint8_t  mQpDecOffset;
  uint32_t mQpDecThld0;
  uint32_t mQpDecThld1;
  uint32_t mQpDecThld2;

  uint8_t  mWaitFrames;

  uint8_t  mDisableDbf;

} H264ENC_Config_t;



/**
* @brief          H264 Encoder IRQ numbers.
* @details        H264 Encoder IRQ numbers.
* @pre
* @post
* @implements     H264Enc_IrqNums_t_struct
*/
typedef struct
{
  uint32_t mH264ENC_VI_FRAME_DONE_INT;
  uint32_t mH264ENC_NEW_ROW_INT;
  uint32_t mH264ENC_CB_ALARM_INT;
  uint32_t mH264ENC_VI_LINE_INT;
  uint32_t mH264ENC_VI_LCOUNT_ALARM_INT;
  uint32_t mH264ENC_VI_OOS_INT;
  uint32_t mH264ENC_MBRW_DONE_INT;
  uint32_t mH264ENC_MBRW_FF_OVF_INT;

} H264Enc_IrqNums_t;

/**
* @brief          H264 Encoder driver info.
* @details        H264 Encoder driver info.
* @pre
* @post
*/
typedef struct
{
  H264ENC_Regs_t *mpH264encCBregs; /* control block registers */
  H264Enc_IrqNums_t mIrqNums;      ///< irqnumbers
#ifdef __KERNEL__
  struct clk    *mpModuleClk;      ///< main module clock structure
  struct clk    *mpPerClk;         ///< peripheral interface clock structure
  struct clk    *mpBclockClk;      ///< SRAM interface clock structure
#endif  
} H264Enc_DrvInfo_t;

/**
* @brief          Provides encoding status.
* @details        Provides encoding status.
* @pre
* @post
* @implements     H264ENC_Status_t_struct
*/
typedef struct
{
  uint32_t mOutCurrentAddress;
  uint32_t mOutAlarmAddress;
  uint32_t mLastFrmEndAddress;
  uint32_t mOutLineCounter;
  uint32_t mInLineCounter;
  uint32_t mInterruptStatus;
  uint32_t mFrameCounter;
  uint32_t mIrqCounter;

} H264ENC_Status_t;


typedef struct 
{
  /* Module Configuration Register */
  uint32_t   MCR;                                  
  uint8_t    H264_ENC_reserved0[12];
  /* Video Configuration Register */
  uint32_t   VCR;                
  /* Video Input Alarm Register */
  uint32_t   VI_ALARM;      
  /* Fetch Macroblock Row Register */
  uint32_t   FETCH_MBRW;  
  uint8_t    H264_ENC_reserved1[20];
  /* Video Input Circular Buffer Luma Start Address Register */
  uint32_t   VI_CBUF_Y_ADDR; 
  /* Video Input Circular Buffer Cb Start Address Register */
  uint32_t   VI_CBUF_CB_ADDR; 
  /* Video Input Circular Buffer Cr Start Address Register */
  uint32_t   VI_CBUF_CR_ADDR; 
  /* Video Input Number of Lines Register */
  uint32_t   VI_NRLINES;  
  /* Rate Flow Control Register */
  uint32_t   RATE_FLOW_CTRL; 
  uint8_t    H264_ENC_reserved2[60];
  /* Output Circular Buffer Start Address Register */
  uint32_t   OUT_CBUF_START_ADDR; 
  /* Output Circular Buffer End Address Register */
  uint32_t   OUT_CBUF_END_ADDR; 
  /* Ouput Circular Buffer Alarm Address Register */
  uint32_t   OUT_CBUF_ALARM_ADDR; 
  uint8_t    H264_ENC_reserved3[4];
  /* Output Circular Buffer Current Address Register */
  uint32_t   OUT_CBUF_CURR_ADDR; 
  /* Output Circular Buffer Vend Address Register */
  uint32_t   OUT_CBUF_VEND_ADDR; 
  uint8_t    H264_ENC_reserved4[108];
  /* Line Counter Status Register */
  uint32_t   LINE_CNTR_STAT; 
  uint8_t    H264_ENC_reserved5[20];
  /* Interrupt Status Register */
  uint32_t   ISR;                
  /* Interrupt Enable Register */
  uint32_t   IER;                
  uint8_t    H264_ENC_reserved6[28];
  /* Testline Configuration Register */
  uint32_t   TESTLINE_CFG; 
  /* Testline Start Location Register */
  uint32_t   TESTLINE_STRT_LOC; 
  /* Testline Luma Value Register */
  uint32_t   TESTLINE_LUMA_VAL; 
  /* Testline Chroma Cb Value Register */
  uint32_t   TESTLINE_CB_VAL; 
  /* Testline Chroma Cr Value Register */
  uint32_t   TESTLINE_CR_VAL; 
  uint8_t    H264_ENC_reserved7[172];
  /* ULLVC Frame Rate Register */
  uint32_t   ULLVC_FRAME_RATE; 
  /* ULLVC Quantization Parameter Initial Register */
  uint32_t   ULLVC_QP_INIT; 
  /* ULLVC Quantization Parameter Range Register */
  uint32_t   ULLVC_QP_RANGE; 
  /* ULLVC Bits Per Macroblock Row Register */
  uint32_t   ULLVC_BITS_PER_MB_ROW; 
  /* ULLVC Fallback QP Limit Register */
  uint32_t   ULLVC_QP_FALLBACK_LIMIT; 
  /* ULLVC Increment QP Register */
  uint32_t   ULLVC_QP_INC; 
  /* ULLVC Increment Threshold Register */
  uint32_t   ULLVC_QP_INC_THLD[3]; 
  /* ULLVC Decrement QP Register */
  uint32_t   ULLVC_QP_DEC; 
  /* ULLVC Decrement Threshold Register */
  uint32_t   ULLVC_QP_DEC_THLD[3]; 
  /* ULLVC Wait Frames Register */
  uint32_t   ULLVC_WAIT_FRAMES; 
  /* ULLVC Disable DBF Register */
  uint32_t   ULLVC_DISABLE_DBF; 
  uint8_t    H264_ENC_reserved8[72];
  /* ULLVC Bitrate Stream Register */
  uint32_t   ULLVC_BITRATE_STREAM; 
} H264ENC_REGS;


#define H264ENC_MCR_SW_RESET_SHIFT         (0U)
#define H264ENC_MCR_SW_RESET_MASK          ((uint32_t)(0x00000001U) << \
                                           (H264ENC_MCR_SW_RESET_SHIFT))
#define H264ENC_MCR_SW_RESET_MASK_NEG      (uint32_t)(0xFFFFFFFEU)

#define H264ENC_ISR_VI_FRAME_DONE_INT_SHIFT (0)
#define H264ENC_ISR_VI_FRAME_DONE_INT_MASK  ((uint32_t)(0x00000001U) <<  \
                                          (H264ENC_ISR_VI_FRAME_DONE_INT_SHIFT))

#define H264ENC_ISR_BS_NEW_ROW_INT_SHIFT   (1)
#define H264ENC_ISR_BS_NEW_ROW_INT_MASK    ((uint32_t)(0x00000001U) <<  \
                                           (H264ENC_ISR_BS_NEW_ROW_INT_SHIFT))

#define H264ENC_ISR_ENC_OOS_INT_SHIFT      (5)
#define H264ENC_ISR_ENC_OOS_INT_MASK       ((uint32_t)(0x00000001U) <<  \
                                           (H264ENC_ISR_ENC_OOS_INT_SHIFT))  

#define H264ENC_ISR_MBRW_DONE_INT_SHIFT    (6)
#define H264ENC_ISR_MBRW_DONE_INT_MASK     ((uint32_t)(0x00000001U) <<  \
                                           (H264ENC_ISR_MBRW_DONE_INT_SHIFT))

#define H264ENC_ISR_MBRW_FF_OVF_INT_SHIFT  (7)
#define H264ENC_ISR_MBRW_FF_OVF_INT_MASK   ((uint32_t)(0x00000001U) <<  \
                                           (H264ENC_ISR_MBRW_FF_OVF_INT_SHIFT))

#define H264ENC_ISR_BR_TOO_HIGH_INT_SHIFT  (10)
#define H264ENC_ISR_BR_TOO_HIGH_INT_MASK   ((uint32_t)(0x00000001U) <<  \
                                           (H264ENC_ISR_BR_TOO_HIGH_INT_SHIFT))

#define H264ENC_ISR_FIFO_AFULL_INT_SHIFT  (11)
#define H264ENC_ISR_FIFO_AFULL_INT_MASK   ((uint32_t)(0x00000001U) <<  \
                                          (H264ENC_ISR_FIFO_AFULL_INT_SHIFT))

#define H264ENC_ISR_BS_FIFO_FULL_INT_SHIFT (12)
#define H264ENC_ISR_BS_FIFO_FULL_INT_MASK  ((uint32_t)(0x00000001U) <<  \
                                           (H264ENC_ISR_BS_FIFO_FULL_INT_SHIFT))

#define ISR_HANDLER_MASK                   (uint32_t)(0x00000023U)

#define H264ENC_MCR_VI_BIT_WIDTH_SHIFT     (8)
#define H264ENC_MCR_VI_BIT_WIDTH_MASK      ((uint32_t)(0x00000003U) <<  \
                                           (H264ENC_MCR_VI_BIT_WIDTH_SHIFT))
#define H264ENC_MCR_VI_BIT_WIDTH_MASK_NEG  (uint32_t)(0xFFFFFCFFU) 

#define H264ENC_MCR_VI_COLOR_FMT_SHIFT     (12)
#define H264ENC_MCR_VI_COLOR_FMT_MASK      ((uint32_t)(0x00000001U) <<  \
                                           (H264ENC_MCR_VI_COLOR_FMT_SHIFT))
#define H264ENC_MCR_VI_COLOR_FMT_MASK_NEG  (uint32_t)(0xFFFFEFFFU) 

#define H264ENC_MCR_FETCH_TRIG_SHIFT       (16)    
#define H264ENC_MCR_FETCH_TRIG_MASK        ((uint32_t)(0x00000001U) <<  \
                                           (H264ENC_MCR_FETCH_TRIG_SHIFT))  
#define H264ENC_MCR_FETCH_TRIG_MASK_NEG    (uint32_t)(0xFFFEFFFFU) 

#define H264ENC_MCR_DATA_MODE_CHR_SHIFT    (24)
#define H264ENC_MCR_DATA_MODE_CHR_MASK     ((uint32_t)(0x00000001U) <<  \
                                           (H264ENC_MCR_DATA_MODE_CHR_SHIFT))
#define H264ENC_MCR_DATA_MODE_CHR_MASK_NEG (uint32_t)(0xFEFFFFFFU)   

#define H264ENC_VCR_FR_WIDTH_SHIFT         (1)
#define H264ENC_VCR_FR_WIDTH_MASK          ((uint32_t)(0x000007FFU) <<  \
                                           (H264ENC_VCR_FR_WIDTH_SHIFT))
#define H264ENC_VCR_FR_WIDTH_MASK_NEG      (uint32_t)(0xFFFFF001U) 

#define H264ENC_VCR_FR_HEIGHT_SHIFT        (17)
#define H264ENC_VCR_FR_HEIGHT_MASK         ((uint32_t)(0x000007FFU) <<  \
                                           (H264ENC_VCR_FR_HEIGHT_SHIFT))
#define H264ENC_VCR_FR_HEIGHT_MASK_NEG     (uint32_t)(0xF001FFFFU) 

#define ULLVC_QP_RANGE_MIN_SHIFT           (0)
#define ULLVC_QP_RANGE_MIN_MASK            ((uint32_t)(0x0000007FU) <<  \
                                           (ULLVC_QP_RANGE_MIN_SHIFT))
#define ULLVC_QP_RANGE_MIN_MASK_NEG        (uint32_t)(0xFFFFFF80U)

#define ULLVC_QP_RANGE_MAX_SHIFT           (16)
#define ULLVC_QP_RANGE_MAX_MASK            ((uint32_t)(0x0000007FU) <<  \
                                           (ULLVC_QP_RANGE_MAX_SHIFT))
#define ULLVC_QP_RANGE_MAX_MASK_NEG        (uint32_t)(0xFF80FFFFU)

#define ULLVC_QP_INC_OFFSET_SHIFT          (0)
#define ULLVC_QP_INC_OFFSET_MASK           ((uint32_t)(0x0000007FU) <<  \
                                           (ULLVC_QP_INC_OFFSET_SHIFT)) 
#define ULLVC_QP_INC_OFFSET_MASK_NEG       (uint32_t)(0xFFFFFF80U)

#define ULLVC_QP_INC_DIFF_0_SHIFT          (8)
#define ULLVC_QP_INC_DIFF_0_MASK           ((uint32_t)(0x0000007FU) <<  \
                                           (ULLVC_QP_INC_DIFF_0_SHIFT)) 
#define ULLVC_QP_INC_DIFF_0_MASK_NEG       (uint32_t)(0xFFFF80FFU)

#define ULLVC_QP_INC_DIFF_1_SHIFT          (16)
#define ULLVC_QP_INC_DIFF_1_MASK           ((uint32_t)(0x0000007FU) <<  \
                                           (ULLVC_QP_INC_DIFF_1_SHIFT)) 
#define ULLVC_QP_INC_DIFF_1_MASK_NEG       (uint32_t)(0xFF80FFFFU) 

#define ULLVC_QP_INC_DIFF_2_SHIFT          (24)
#define ULLVC_QP_INC_DIFF_2_MASK           ((uint32_t)(0x0000007FU) <<  \
                                           (ULLVC_QP_INC_DIFF_2_SHIFT)) 
#define ULLVC_QP_INC_DIFF_2_MASK_NEG       (uint32_t)(0x80FFFFFFU)   

#define ULLVC_QP_DEC_OFFSET_SHIFT          (0)
#define ULLVC_QP_DEC_OFFSET_MASK           ((uint32_t)(0x0000007FU) <<  \
                                           (ULLVC_QP_DEC_OFFSET_SHIFT)) 
#define ULLVC_QP_DEC_OFFSET_MASK_NEG       (uint32_t)(0xFFFFFF80U)

#define ULLVC_QP_DEC_DIFF_0_SHIFT          (8)
#define ULLVC_QP_DEC_DIFF_0_MASK           ((uint32_t)(0x0000007FU) <<  \
                                           (ULLVC_QP_DEC_DIFF_0_SHIFT))
#define ULLVC_QP_DEC_DIFF_0_MASK_NEG       (uint32_t)(0xFFFF80FFU)

#define ULLVC_QP_DEC_DIFF_1_SHIFT          (16)
#define ULLVC_QP_DEC_DIFF_1_MASK           ((uint32_t)(0x0000007FU) <<  \
                                           (ULLVC_QP_DEC_DIFF_1_SHIFT)) 
#define ULLVC_QP_DEC_DIFF_1_MASK_NEG       (uint32_t)(0xFF80FFFFU)

#define ULLVC_QP_DEC_DIFF_2_SHIFT          (24)
#define ULLVC_QP_DEC_DIFF_2_MASK           ((uint32_t)(0x0000007FU) <<  \
                                           (ULLVC_QP_DEC_DIFF_2_SHIFT)) 
#define ULLVC_QP_DEC_DIFF_2_MASK_NEG       (uint32_t)(0x80FFFFFFU) 

#define VI_NRLINES_SHIFT                   (1)
#define VI_NRLINES_MASK                    ((uint32_t)(0x0000007FU) << \
                                           (VI_NRLINES_SHIFT))  
#define VI_NRLINES_MASK_NEG                (uint32_t)(0xFFFFFF01U)

#define RATE_FLOW_CB_STRIDE_SHIFT          (0)
#define RATE_FLOW_CB_STRIDE_MASK           ((uint32_t)(0x00000007U) <<  \
                                           (RATE_FLOW_CB_STRIDE_SHIFT))  
#define RATE_FLOW_CB_STRIDE_MASK_NEG       (uint32_t)(0xFFFFFFF8U)  

#define RATE_FLOW_CTRL_ENABLE_SHIFT        (31)
#define RATE_FLOW_CTRL_ENABLE_MASK         ((uint32_t)(0x00000001U) <<  \
                                           (RATE_FLOW_CTRL_ENABLE_SHIFT)) 
#define RATE_FLOW_CTRL_ENABLE_MASK_NEG     (uint32_t)(0x7FFFFFFFU)  

#define IER_BR_TOO_HIGH_INT_EN_SHIFT       (10)    
#define IER_BR_TOO_HIGH_INT_EN_MASK        ((uint32_t)(0x00000001U) <<  \
                                           (IER_BR_TOO_HIGH_INT_EN_SHIFT)) 
#define IER_BR_TOO_HIGH_INT_EN_MASK_NEG    (uint32_t)(0xFFFFFBFFU)

#define IER_OUT_CBUF_OVF_INT_EN_SHIFT      (9)
#define IER_OUT_CBUF_OVF_INT_EN_MASK       ((uint32_t)(0x00000001U) <<  \
                                           (IER_OUT_CBUF_OVF_INT_EN_SHIFT)) 
#define IER_OUT_CBUF_OVF_INT_EN_MASK_NEG   (uint32_t)(0xFFFFFDFFU)

#define IER_MBRW_FF_OVF_INT_EN_SHIFT       (7) 
#define IER_MBRW_FF_OVF_INT_EN_MASK        ((uint32_t)(0x00000001U) <<  \
                                           (IER_MBRW_FF_OVF_INT_EN_SHIFT)) 
#define IER_MBRW_FF_OVF_INT_EN_MASK_NEG    (uint32_t)(0xFFFFFF7FU)  

#define IER_ENC_OOS_INT_EN_SHIFT           (5) 
#define IER_ENC_OOS_INT_EN_MASK            ((uint32_t)(0x00000001) <<  \
                                           (IER_ENC_OOS_INT_EN_SHIFT)) 
#define IER_ENC_OOS_INT_EN_MASK_NEG        (uint32_t)(0xFFFFFFDFU) 

#define IER_BS_NEW_ROW_INT_EN_SHIFT        (1)
#define IER_BS_NEW_ROW_INT_EN_MASK         ((uint32_t)(0x00000001U) <<  \
                                           (IER_BS_NEW_ROW_INT_EN_SHIFT)) 
#define IER_BS_NEW_ROW_INT_EN_MASK_NEG     (uint32_t)(0xFFFFFFFDU) 

#define IER_VI_FRAME_DONE_INT_EN_SHIFT     (0)       
#define IER_VI_FRAME_DONE_INT_EN_MASK      ((uint32_t)(0x00000001U) <<  \
                                           (IER_VI_FRAME_DONE_INT_EN_SHIFT)) 
#define IER_VI_FRAME_DONE_INT_EN_MASK_NEG  (uint32_t)(0xFFFFFFFEU) 

#define OUT_CBUF_CURR_ADDR_SHIFT           (3)
#define OUT_CBUF_CURR_ADDR_MASK            ((uint32_t)(0x0007FFFFU) <<  \
                                           (OUT_CBUF_CURR_ADDR_SHIFT)) 

#ifdef __cplusplus
}
#endif

#endif /* H264ENCTYPES_H */
