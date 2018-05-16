/*****************************************************************************
 *
 * Freescale Confidential Proprietary
 *
 * Copyright (c) 2014 Freescale Semiconductor;
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
/**
 * \file    seq_hw.h
 * \brief   ISP sequencer hardware definition file
 * \author
 * \author
 * \version 0.1
 * \date    14-01-2013
 * \note    initial version
 ****************************************************************************/

#ifndef SEQHW_H
#define SEQHW_H

#ifndef __ASSEMBLER__
#include "typedefs.h"
#include "s32vs234.h"

#ifdef  __cplusplus
extern "C" {
#endif

#include "seq_mem_map.h"
#include "ipus_mem_map.h"
#include "ipuv_mem_map.h"
#include "mipi_mem_map.h"
#endif // __ASSEMBLER__

/*****************************************************************************
* publics consts
*****************************************************************************/

#define	INT_INT_ERR	16
#define	INT_INT_DMA	17
#define	INT_IN_MSG	18
#define	INT_IPUS_DONE	19
#define	INT_IPUV_DONE	20
#define	INT_OTHR_DONE	21
#define	INT_OUT_MSG_ACK	22
#define	INT_SW_EVNT	23
#define	INT_FDMA_ERR	24

#define	ISP_NUM_IPUS	8
#define	ISP_NUM_IPUV	4

#define	ISP_IO_DEV_SPACE		0x1000

// *** bits ***********************************************
#define	OTHR_BIT_FDMA		0x00000001
#define	OTHR_BIT_H264_ENC	0x00000002
#define	OTHR_BIT_H264_DEC_0	0x00000004
#define	OTHR_BIT_H264_DEC_1	0x00000008
#define	OTHR_BIT_H264_DEC_2	0x00000010
#define	OTHR_BIT_H264_DEC_3	0x00000020
#define	OTHR_BIT_JPEG_DEC_0	0x00000040
#define	OTHR_BIT_JPEG_DEC_1	0x00000080
#define	OTHR_BIT_JPEG_DEC_2	0x00000100
#define	OTHR_BIT_JPEG_DEC_3	0x00000200
#define	OTHR_BIT_MIPI0_0	0x00000400
#define	OTHR_BIT_MIPI0_1	0x00000800
#define	OTHR_BIT_MIPI0_2	0x00001000
#define	OTHR_BIT_MIPI0_3	0x00002000
#define	OTHR_BIT_MIPI1_0	0x00004000
#define	OTHR_BIT_MIPI1_1	0x00008000
#define	OTHR_BIT_MIPI1_2	0x00010000
#define	OTHR_BIT_MIPI1_3	0x00020000
#define	OTHR_BIT_VIU0		0x00040000
#define	OTHR_BIT_VIU1		0x00080000
#define	OTHR_BIT_DMACHMUX_0	0x00100000
#define	OTHR_BIT_DMACHMUX_1	0x00200000
#define	OTHR_BIT_DMACHMUX_2	0x00400000
#define	OTHR_BIT_DMACHMUX_3	0x00800000
#define	OTHR_BIT_FLEXT_0	0x01000000
#define	OTHR_BIT_FLEXT_1	0x02000000

#define	OTHR_BIT_MIPI0		(OTHR_BIT_MIPI0_0 | OTHR_BIT_MIPI0_1 | OTHR_BIT_MIPI0_2 | OTHR_BIT_MIPI0_3)
#define	OTHR_BIT_MIPI1		(OTHR_BIT_MIPI1_0 | OTHR_BIT_MIPI1_1 | OTHR_BIT_MIPI1_2 | OTHR_BIT_MIPI1_3)

// *** FDMA ***********************************************
#define	FDMA_REG_SPACING	0x10

// *** PRAM ***********************************************
#ifndef SEQ_PRAM_BASE
#define SEQ_PRAM_BASE		0x20000000
#endif

// *** KRAM ***********************************************
#define SEQ_KRAM_BASE		0x20001000UL

// *** Debug **********************************************
#define DBGRAM_BASE			0x1ffff000UL
#define DBGRAM_SIZE			0x00001000UL

// *** CoreM0 *********************************************
#define	COREM0_INT_NUM0			(0x1 << 0)
#define	COREM0_INT_NUM1			(0x1 << 1)
#define	COREM0_INT_NUM2			(0x1 << 2)
#define	COREM0_INT_NUM3			(0x1 << 3)

// *** sequencer ******************************************
#define	SEQUENCER_INT_NUM0		(0x1 << 0)
#define	SEQUENCER_INT_NUM1		(0x1 << 1)
#define	SEQUENCER_INT_NUM2		(0x1 << 2)
#define	SEQUENCER_INT_NUM3		(0x1 << 3)

// *** FileIO sDMA/MIPI ***********************************
#define	MIPICSI0_REG_SZ			(1)*SDMA_REG_SZ

// *** stream DMA *****************************************
#define	SDMA_REG_NUMBYTES		0x0
#define	SDMA_REG_CONTROL		0x4
#define	SDMA_REG_MEMBASE		0x8
#define	SDMA_REG_SZ			0x10

/* ----------------------------------------------------
   -FDMA
   ---------------------------------------------------- */
#define FDMA_BASEADDRESS               ((uint32_t)&FDMA)

#define FDMA_XFR_REC_LIST_PTR_OFFSET   0               /* Transfer Records List Pointer register */
#define FDMA_XFR_REC_CNT_OFFSET        0x4             /* Total entries in Transfer Record List */
#define FDMA_XFR_REC_NUM_OFFSET        0x8             /* Transfer Record number for current Line transfer */
#define FDMA_XFR_LINE_NUM_OFFSET       0xC             /* DDR and SRAM Line numbers for current transfer */
#define FDMA_LINE_INCR_OFFSET          0x10            /* Line increment value for SRAM and DDR */
#define FDMA_IRQ_EN_OFFSET             0x14            /* Interrupt enable register */
#define FDMA_XFR_STAT_OFFSET           0x18            /* Status register */
#define FDMA_CALC_CRC_VAL_OFFSET       0x1C            /* Calculated CRC value */
#define FDMA_CURR_DDR_PTR_OFFSET       0x20            /* Current DDR address */
#define FDMA_CURR_SRAM_PTR_OFFSET      0x24            /* Current SRAM address */
#define FDMA_XFR_REC_NUM_DONE_OFFSET   0x28            /* Last completed Transfer Record Number */
#define FDMA_ERR_XFR_REC_NUM_OFFSET    0x2C            /* Transfer Record Number of an Erroneous Transfer */
#define FDMA_NEXT_LINE_OFFSET          0x30            /* SRAM and DDR next Line number */
#define FDMA_CTRL_OFFSET               0x34            /* Control register */

#define FDMA_XFR_STAT_XFR_CMD_QUEUE_FULL ((1) << 1) 
/* ----------------------------------------------------
   -VSEQ_INT_DMA
   ---------------------------------------------------- */
#define VSEQ_INT_DMA_BASE              ((uint32_t)&VSEQ_INT_DMA)

#define VSEQ_INT_DMA_KRAM_ADDR_OFFSET  0               /* Kernel-RAM Address as Start Address for DMA Transfer */
#define VSEQ_INT_DMA_TARGET_IPUx_ENGN_OFFSET 0x4       /* Target IPUx Engine to be programmed with Kernel code via DMA */
#define VSEQ_INT_DMA_TRANSFER_LEN_OFFSET 0x8           /* DMA Transfer Length or Byte Count */
#define VSEQ_INT_DMA_CTRL_STAT_OFFSET  0xC             /* DMA Control and Status register */

#define	SEQ_ADDR_OFF	0x3C020000UL

#ifndef __ASSEMBLER__
#ifndef CA53 //sth
// *** we need the sequencer's view to the register: ***
#undef VSEQ_CTRL_BLK
#define VSEQ_CTRL_BLK (*(volatile struct VSEQ_CTRL_BLK_tag *)(0x7C021000UL-SEQ_ADDR_OFF))
#undef VSEQ_EVT_CTRL
#define VSEQ_EVT_CTRL (*(volatile struct VSEQ_EVT_CTRL_tag *)(0x7C022000UL-SEQ_ADDR_OFF))
#undef VSEQ_INT_DMA
#define VSEQ_INT_DMA (*(volatile struct VSEQ_INT_DMA_tag *)(0x7C024000UL-SEQ_ADDR_OFF))
#undef FDMA
#define FDMA (*(volatile struct FDMA_tag *)(0x7C040000UL-SEQ_ADDR_OFF))
#undef IPUS_0
#define IPUS_0 (*(volatile struct IPUS_tag *)(0x7C042000UL-SEQ_ADDR_OFF))
#undef IPUS_1
#define IPUS_1 (*(volatile struct IPUS_tag *)(0x7C043000UL-SEQ_ADDR_OFF))
#undef IPUS_2
#define IPUS_2 (*(volatile struct IPUS_tag *)(0x7C044000UL-SEQ_ADDR_OFF))
#undef IPUS_3
#define IPUS_3 (*(volatile struct IPUS_tag *)(0x7C045000UL-SEQ_ADDR_OFF))
#undef IPUS_4
#define IPUS_4 (*(volatile struct IPUS_tag *)(0x7C046000UL-SEQ_ADDR_OFF))
#undef IPUS_5
#define IPUS_5 (*(volatile struct IPUS_tag *)(0x7C047000UL-SEQ_ADDR_OFF))
#undef IPUS_6
#define IPUS_6 (*(volatile struct IPUS_tag *)(0x7C048000UL-SEQ_ADDR_OFF))
#undef IPUS_7
#define IPUS_7 (*(volatile struct IPUS_tag *)(0x7C049000UL-SEQ_ADDR_OFF))
#undef IPUV_0
#define IPUV_0 (*(volatile struct IPUV_tag *)(0x7C062000UL-SEQ_ADDR_OFF))
#undef IPUV_1
#define IPUV_1 (*(volatile struct IPUV_tag *)(0x7C063000UL-SEQ_ADDR_OFF))
#undef IPUV_2
#define IPUV_2 (*(volatile struct IPUV_tag *)(0x7C064000UL-SEQ_ADDR_OFF))
#undef IPUV_3
#define IPUV_3 (*(volatile struct IPUV_tag *)(0x7C065000UL-SEQ_ADDR_OFF))

#endif // sth

/*****************************************************************************
* publics types
*****************************************************************************/
typedef struct SDMA_reg_t
{
  uint32_t	mLineLen;
  uint32_t	mChanCfg;
  uint32_t	mLinePtr;

} SDMA_reg_t;

/*****************************************************************************
* macros
*****************************************************************************/

/*****************************************************************************
* publics functions (prototypes)
*****************************************************************************/

#ifdef  __cplusplus
}
#endif
#endif // __ASSEMBLER__

#endif /* SEQHW_H */
/*EOF*/
