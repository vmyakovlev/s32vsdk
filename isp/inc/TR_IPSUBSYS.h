	/**************************************************************************
	* FILE NAME: TR_IPSUBSYS.h                    COPYRIGHT (c) Freescale 2013 *
	*                                                    All Rights Reserved *
	*                                                                        *
	* DESCRIPTION:                                                           *
	* This file contains all of the register and bit field definitions for   
	TR_IPSUBSYS.                                                                 *

	* AUTHOR : Rajesh Udenia(B34390)                                          *

	*========================================================================*
	* NOTE: Any release number followed by a lower case alpha character      *
	* indicates a hand edit between automated releases. Automated releases   *
	* are indicated by incrementing the numeric portion of the revision.     *
	*========================================================================*
	* COPYRIGHT:                                                             *
	*  Freescale Semiconductor, INC. All Rights Reserved. You are hereby     *
	*  granted a copyright license to use, modify, and distribute the        *
	*  SOFTWARE so long as this entire notice is retained without alteration *
	*  in any modified and/or redistributed versions, and that such modified *
	*  versions are clearly identified as such. No licenses are granted by   *
	*  implication, estoppel or otherwise under any patentsor trademarks     *
	*  of Freescale Semiconductor, Inc. This software is provided on an      *
	*  "AS IS" basis and without warranty.                                 *
	*                                                                        *
	*  To the maximum extent permitted by applicable law, Freescale          *
	*  Semiconductor DISCLAIMS ALL WARRANTIES WHETHER EXPRESS OR IMPLIED,    *
	*  INCLUDING IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A      *
	*  PARTICULAR PURPOSE AND ANY WARRANTY AGAINST INFRINGEMENT WITH REGARD  *
	*  TO THE SOFTWARE (INCLUDING ANY MODIFIED VERSIONS THEREOF) AND ANY     *
	*  ACCOMPANYING WRITTEN MATERIALS.                                       *
	*                                                                        *
	*  To the maximum extent permitted by applicable law, IN NO EVENT        *
	*  SHALL Freescale Semiconductor BE LIABLE FOR ANY DAMAGES WHATSOEVER    *
	*  (INCLUDING WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,  *
	*  BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR OTHER         *
	*  PECUNIARY LOSS) ARISING OF THE USE OR INABILITY TO USE THE SOFTWARE.  *
	*                                                                        *
	*  Freescale Semiconductor assumes no responsibility for the             *
	*  maintenance and support of this software                              *
	*                                                                        *
	**************************************************************************/

	/*>>>>>>>NOTE! this file is auto-generated please do not edit it! <<<<<<<*/

	/**************************************************************************/

#ifndef TRIPSUBSYS_H
#define TRIPSUBSYS_H

#include "typedefs.h"

#define IPUS_INSTANCE_COUNT 8
#define IPUV_INSTANCE_COUNT 4

/* ============================================================================
   ============================= Module: FastDMA =============================
   ============================================================================ */
struct FastDMA_tag {
	union {
		vuint32_t R;
		struct {
  		vuint32_t XFR_REC_LIST_PTR: 32;				/* 1 */
 
	} B;
	} XFR_REC_LIST_PTR;	/* Register offset is 0x000 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t XFR_REC_CNT: 7;				/* 1 */
  		vuint32_t: 25;
 
	} B;
	} XFR_REC_CNT;	/* Register offset is 0x004 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t XFR_REC_NUM: 7;				/* 1 */
  		vuint32_t: 25;
	} B;
	} XFR_REC_NUM;	/* Register offset is 0x008 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t DDR_IMG_LINE_NUM: 16;				/* 1 */
  		vuint32_t SRAM_LINE_NUM: 16;				/* 1 */
 
	} B;
	} XFR_LINE_NUM;	/* Register offset is 0x00C */

	union {
		vuint32_t R;
		struct {
 		vuint32_t DDR_LINE_INCR: 8;				/* 1 */
 		vuint32_t: 8;
 		vuint32_t SRAM_LINE_INCR: 8;				/* 1 */
  		vuint32_t: 8;
 
	} B;
	} LINE_INCR;	/* Register offset is 0x010 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t FDMA_DONE_IRQ_EN: 1;				/* 1 */
 		vuint32_t FDMA_ERR_IRQ_EN: 1;				/* 1 */
 		vuint32_t FDMA_CRC_ERR_IRQ_EN: 1;				/* 1 */
 		vuint32_t EDMA_TRIG_EN: 1;				/* 1 */
  		vuint32_t: 28;
 
	} B;
	} IRQ_EN;	/* Register offset is 0x014 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t XFR_QUEUE_EMPTY: 1;				/* 1 */
 		vuint32_t XFR_CMD_QUEUE_FULL: 1;				/* 1 */
 		vuint32_t CRC_ERR: 1;				/* 1 */
 		vuint32_t CMD_CFG_ERR: 1;				/* 1 */
 		vuint32_t XFR_STP_ERR_TR: 1;				/* 1 */
 		vuint32_t XFR_STP_ERR_CT: 1;				/* 1 */
 		vuint32_t XFR_CFG_ERR: 1;				/* 1 */
 		vuint32_t XFR_ERR_DDR: 1;				/* 1 */
 		vuint32_t XFR_ERR_SRAM: 1;				/* 1 */
 		vuint32_t: 7;
 		vuint32_t XFR_DONE: 1;				/* 1 */
 		vuint32_t: 7;
  		vuint32_t DONE_CNT: 8;				/* 1 */
	} B;
	} XFR_STAT;	/* Register offset is 0x018 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CALC_CRC_VAL: 32;				/* 1 */
 
	} B;
	} CALC_CRC_VAL;	/* Register offset is 0x01C */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CURR_DDR_PTR: 32;				/* 1 */
 
	} B;
	} CURR_DDR_PTR;	/* Register offset is 0x020 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CURR_SRAM_PTR: 32;				/* 1 */
 
	} B;
	} CURR_SRAM_PTR;	/* Register offset is 0x024 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t XFR_REC_NUM_DONE: 7;				/* 1 */
  		vuint32_t: 25;
 
	} B;
	} XFR_REC_NUM_DONE;	/* Register offset is 0x028 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ERR_XFR_REC_NUM: 7;				/* 1 */
  		vuint32_t: 25;
	} B;
	} ERR_XFR_REC_NUM;	/* Register offset is 0x028 */

	uint8_t FastDMA_reserved_0[4] ; 
	union {
		vuint32_t R;
		struct {
 		vuint32_t SRAM_NEXT_LINE_NUM: 16;				/* 1 */
  		vuint32_t: 16;
 
	} B;
	} SRAM_NEXT_LINE;	/* Register offset is 0x030 */
}; 
 

/* ============================================================================
   ============================= Module: HPSMI =============================
   ============================================================================ */
struct HPSMI_tag {
	union {
		vuint32_t R;
		struct {
 		vuint32_t: 1;
 		vuint32_t WR_ENABLE: 1;				/* 1 */
 		vuint32_t STATUSREG_RESET: 1;				/* 1 */
 		vuint32_t CTRLREG_RESET: 1;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t PROFILE_CTRLREG_RESET: 1;				/* 1 */
  		vuint32_t: 26;
 
	} B;
	} GBL_CTRL;	/* Register offset is 0x000 */

	uint8_t HPSMI_reserved_0[12] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t SELVAL: 32;				/* 1 */
 
	} B;
	} MPU_SEL0;	/* Register offset is 0x010 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t SELVAL: 32;				/* 1 */
 
	} B;
	} MPU_SEL1;	/* Register offset is 0x014 */

	uint8_t HPSMI_reserved_1[8] ; 
	union {
		vuint32_t R;
		struct {
 		vuint32_t SEL_PRIO: 7;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t PRIO_MASK: 7;				/* 1 */
 		vuint32_t CNTR_EN: 1;				/* 1 */
 		vuint32_t CNTR_MODE: 1;				/* 1 */
  		vuint32_t: 15;
 
	} B;
	} PRIO_CHANGE;	/* Register offset is 0x020 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_LSB2AXICYCCNTR_0;	/* Register offset is 0x024 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_LSB2AXICYCCNTR_1;	/* Register offset is 0x028 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t AXI_CYCCNTR_OVF0: 1;				/* 1 */
 		vuint32_t AXI_CYCCNTR_OVF1: 1;				/* 1 */
 		vuint32_t AXI_CYCCNTR_OVF2: 1;				/* 1 */
 		vuint32_t AXi_CYCCNTR_OVF3: 1;				/* 1 */
 		vuint32_t AXI_CYCCNTR_OVF4: 1;				/* 1 */
 		vuint32_t AXI_CYCCNTR_OVF5: 1;				/* 1 */
 		vuint32_t AXI_CYCCNTR_OVF6: 1;				/* 1 */
 		vuint32_t LSB2AXI_CYCCNTR_OVF0: 1;				/* 1 */
 		vuint32_t LSB2AXI_CYCCNTR_OVF1: 1;				/* 1 */
  		vuint32_t: 23;
 
	} B;
	} PROFILE_CYCCNTR_OVF;	/* Register offset is 0x02c */

	union {
		vuint32_t R;
		struct {
 		vuint32_t WEN: 1;				/* 1 */
 		vuint32_t STOP_TOTAL: 1;				/* 1 */
 		vuint32_t: 6;
 		vuint32_t STOP_AXIMASTER: 7;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t STOP_LSB2AXIMASTER: 2;				/* 1 */
  		vuint32_t: 14;
 
	} B;
	} PROFILE_STOP;	/* Register offset is 0x030 */

	uint8_t HPSMI_reserved_2[28] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ADDR_ERR: 1;				/* 1 */
 		vuint32_t MPU_ERR: 1;				/* 1 */
 		vuint32_t AXICMD_ERR: 1;				/* 1 */
 		vuint32_t PM_ERR: 1;				/* 1 */
 		vuint32_t ECC_ERR_SEG0: 1;				/* 1 */
 		vuint32_t ECC_ERR_SEG1: 1;				/* 1 */
 		vuint32_t ECC_ERR_SEG2: 1;				/* 1 */
 		vuint32_t WTPT: 1;				/* 1 */
  		vuint32_t: 24;
	} B;
	} INTR_EN0;	/* Register offset is 0x050 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ECC_SINGLEERR_SEG0_OVF: 1;				/* 1 */
 		vuint32_t ECC_SINGLEERR_SEG1_OVF: 1;				/* 1 */
 		vuint32_t ECC_SINGLEERR_SEG2_OVF: 1;				/* 1 */
 		vuint32_t ECC_UNCORRERR_SEG0_OVF: 1;				/* 1 */
 		vuint32_t ECC_UNCORRERR_SEG1_OVF: 1;				/* 1 */
 		vuint32_t ECC_UNCORERR_SEG2_OVF: 1;				/* 1 */
  		vuint32_t: 26;
	} B;
	} INTR_EN1;	/* Register offset is 0x054 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t WEN: 1;				/* 1 */
 		vuint32_t START_TOTAL: 1;				/* 1 */
 		vuint32_t: 6;
 		vuint32_t START_AXIMASTER: 7;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t START_LSB2AXIMASTER: 2;				/* 1 */
  		vuint32_t: 14;
 
	} B;
	} PROFILE_START;	/* Register offset is 0x058 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t WEN: 1;				/* 1 */
 		vuint32_t RESET_TOTAL: 1;				/* 1 */
 		vuint32_t: 6;
 		vuint32_t RESET_AXIMASTER: 7;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t RESET_LSB2AXIMASTER: 2;				/* 1 */
  		vuint32_t: 14;
 
	} B;
	} PROFILE_RESET;	/* Register offset is 0x05c */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t WEN: 1;				/* 1 */
 		vuint32_t FREEZE_TOTAL: 1;				/* 1 */
 		vuint32_t: 6;
 		vuint32_t FREEZE_AXIMASTER: 7;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t FREEZE_LSB2AXIMASTER: 2;				/* 1 */
  		vuint32_t: 14;
	} B;
	} PROFILE_FREEZE;	/* Register offset is 0x060 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t DECVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_DECVAL;	/* Register offset is 0x064 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t CNTRNUM: 4;				/* 1 */
 		vuint32_t DEC: 1;				/* 1 */
  		vuint32_t: 27;
 
	} B;
	} PROFILE_DECVAL_CNTRNUM;	/* Register offset is 0x068 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_AXIRDCNTRVAL_0;	/* Register offset is 0x06c */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_AXIWRCNTRVAL_0;	/* Register offset is 0x070 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_AXIRDCNTRVAL_1;	/* Register offset is 0x074 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_AXIWRCNTRVAL_1;	/* Register offset is 0x078 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_AXIRDCNTRVAL_2;	/* Register offset is 0x07c */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_AXIWRCNTRVAL_2;	/* Register offset is 0x080 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t CNTRVAL: 32;				/* 1 */
	} B;
	} PROFILE_AXIRDCNTRVAL_3;	/* Register offset is 0x084 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_AXIWRCNTRVAL_3;	/* Register offset is 0x088 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_AXIRDCNTRVAL_4;	/* Register offset is 0x08c */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_AXIWRCNTRVAL_4;	/* Register offset is 0x090 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_AXIRDCNTRVAL_5;	/* Register offset is 0x094 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t CNTRVAL: 32;				/* 1 */
	} B;
	} PROFILE_AXIWRCNTRVAL_5;	/* Register offset is 0x098 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_AXIRDCNTRVAL_6;	/* Register offset is 0x09c */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_AXIWRCNTRVAL_6;	/* Register offset is 0x0a0 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_LSB2AXIRDCNTRVAL_0;	/* Register offset is 0x0a4 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_LSB2AXIWRCNTRVAL_0;	/* Register offset is 0x0a8 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t CNTRVAL: 32;				/* 1 */
	} B;
	} PROFILE_LSB2AXIRDCNTRVAL_1;	/* Register offset is 0x0ac */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_LSB2AXIWRCNTRVAL_1;	/* Register offset is 0x0b0 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_AXICYCCNTRVAL_0;	/* Register offset is 0x0b4 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_AXICYCCNTRVAL_1;	/* Register offset is 0x0b8 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_AXICYCCNTRVAL_2;	/* Register offset is 0x0bc */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t CNTRVAL: 32;				/* 1 */
	} B;
	} PROFILE_AXICYCCNTRVAL_3;	/* Register offset is 0x0c0 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_AXICYCCNTRVAL_4;	/* Register offset is 0x0c4 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_AXICYCCNTRVAL_5;	/* Register offset is 0x0c8 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CNTRVAL: 32;				/* 1 */
 
	} B;
	} PROFILE_AXICYCCNTRVAL_6;	/* Register offset is 0x0cc */

	union {
		vuint32_t R;
		struct {
  		vuint32_t MASTERNUM: 32;				/* 1 */
 
	} B;
	} RD_PL_LVL_31_0;	/* Register offset is 0x0d0 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t MASTERNUM: 32;				/* 1 */
	} B;
	} RD_PL_LVL_63_32;	/* Register offset is 0x0d4 */

	uint8_t HPSMI_reserved_3[8] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t MASTERNUM: 32;				/* 1 */
 
	} B;
	} WR_PL_LVL_31_0;	/* Register offset is 0x0e0 */

	uint8_t HPSMI_reserved_4[28] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t DMA_RD_ADDR_ERROR: 32;				/* 1 */
 
	} B;
	} DSRD_ADDRESS_ERROR0;	/* Register offset is 0x100 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t DMA_RD_ADDR_ERROR: 32;				/* 1 */
 
	} B;
	} DSRD_ADDRESS_ERROR1;	/* Register offset is 0x104 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t DMA_WR_ADDR_ERROR: 32;				/* 1 */
 
	} B;
	} DSWR_ADDRESS_ERROR0;	/* Register offset is 0x108 */

	uint8_t HPSMI_reserved_5[4] ; 
	union {
		vuint32_t R;
		struct {
 		vuint32_t AXI_ADDR_ERROR: 14;				/* 1 */
  		vuint32_t: 18;
 
	} B;
	} AXI_ADDRESS_ERROR;	/* Register offset is 0x110 */

	uint8_t HPSMI_reserved_6[8] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t ERROR_ADDRESS: 32;				/* 1 */
 
	} B;
	} ADDR_ERROR_ADDRVAL;	/* Register offset is 0x11c */

	union {
		vuint32_t R;
		struct {
 		vuint32_t MASTERNUM: 6;				/* 1 */
 		vuint32_t MASTER_TYPE: 2;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t ADDR_ERR: 1;				/* 1 */
  		vuint32_t: 22;
 
	} B;
	} ADDR_ERROR_INFO;	/* Register offset is 0x120 */

	uint8_t HPSMI_reserved_7[16] ; 
	union {
		vuint32_t R;
		struct {
 
  		vuint32_t DMA_WR_MPU_ERR: 32;				/* 1 */
	} B;
	} WRMPU_ERROR0;	/* Register offset is 0x134 */

	uint8_t HPSMI_reserved_8[4] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t ERROR_ADDRESS: 32;				/* 1 */
 
	} B;
	} WRMPU_ERROR_ADDR;	/* Register offset is 0x13c */

	union {
		vuint32_t R;
		struct {
 		vuint32_t MASTERNUM: 6;				/* 1 */
 		vuint32_t: 3;
 		vuint32_t MPU_ERR: 1;				/* 1 */
  		vuint32_t: 22;
 
	} B;
	} WRMPU_ERROR_INFO;	/* Register offset is 0x140 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t S0_WRCMD_ERR: 1;				/* 1 */
 		vuint32_t S0_RDCMD_ERR: 1;				/* 1 */
 		vuint32_t S1_WRCMD_ERR: 1;				/* 1 */
 		vuint32_t S1_RDCMD_ERR: 1;				/* 1 */
  		vuint32_t: 28;
 
	} B;
	} AXICMD_ERROR;	/* Register offset is 0x144 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ERROR_ADDRESS: 32;				/* 1 */
	} B;
	} AXICMD_ERROR_ADDR;	/* Register offset is 0x148 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t MASTERNUM: 2;				/* 1 */
 		vuint32_t: 7;
 		vuint32_t CMD_ERR: 1;				/* 1 */
  		vuint32_t: 22;
 
	} B;
	} AXICMD_ERROR_INFO;	/* Register offset is 0x14c */

	union {
		vuint32_t R;
		struct {
 		vuint32_t ERROR_BANK: 16;				/* 1 */
  		vuint32_t: 16;
 
	} B;
	} PM_ERROR_1_2;	/* Register offset is 0x150 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ERROR_ADDR: 32;				/* 1 */
 
	} B;
	} PM_ERROR_ADDR_1_2;	/* Register offset is 0x154 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t MASTERNUM: 6;				/* 1 */
 		vuint32_t MASTER_TYPE: 2;				/* 1 */
 		vuint32_t BANKNUM: 4;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t PM_ERR: 1;				/* 1 */
  		vuint32_t: 18;
	} B;
	} PM_ERROR_INFO_1_2;	/* Register offset is 0x158 */

	uint8_t HPSMI_reserved_9[36] ; 
	union {
		vuint32_t R;
		struct {
 		vuint32_t ERR_INJECT: 8;				/* 1 */
  		vuint32_t: 24;
 
	} B;
	} ECC_ERR_INJECT;	/* Register offset is 0x180 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t ERR_CNTR_SEG1: 6;				/* 1 */
 		vuint32_t ECCERR_OVF_SEG1: 1;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t ERR_CNTR_SEG2: 6;				/* 1 */
 		vuint32_t ECCERR_OVF_SEG2: 1;				/* 1 */
  		vuint32_t: 17;
 
	} B;
	} ECC_SINGLE_ERRCNTR_1_2;	/* Register offset is 0x184 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t ERR_CNTR_SEG1: 6;				/* 1 */
 		vuint32_t ECCERR_OVF_SEG1: 1;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t ERR_CNTR_SEG2: 6;				/* 1 */
 		vuint32_t ECCERR_OVF_SEG2: 1;				/* 1 */
  		vuint32_t: 17;
 
	} B;
	} ECC_UNCORR_ERRCNTR_1_2;	/* Register offset is 0x188 */

	uint8_t HPSMI_reserved_10[12] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t ERROR_ADDR: 32;				/* 1 */
 
	} B;
	} ECC_ERRADDR_SEG1;	/* Register offset is 0x198 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ERROR_DATA0: 32;				/* 1 */
 
	} B;
	} ECC_ERRDATA0_SEG1;	/* Register offset is 0x19c */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ERROR_DATA1: 32;				/* 1 */
 
	} B;
	} ECC_ERRDATA1_SEG1;	/* Register offset is 0x1a0 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t SYNDROME: 8;				/* 1 */
 		vuint32_t ERR_TYPE: 1;				/* 1 */
 		vuint32_t ERR_BANK: 3;				/* 1 */
 		vuint32_t ECC_ERR: 1;				/* 1 */
  		vuint32_t: 19;
 
	} B;
	} ECC_ERRINFO_SEG1;	/* Register offset is 0x1a4 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ERROR_ADDR: 32;				/* 1 */
 
	} B;
	} ECC_ERRADDR_SEG2;	/* Register offset is 0x1a8 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ERROR_DATA0: 32;				/* 1 */
 
	} B;
	} ECC_ERRDATA0_SEG2;	/* Register offset is 0x1ac */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ERROR_DATA1: 32;				/* 1 */
 
	} B;
	} ECC_ERRDATA1_SEG2;	/* Register offset is 0x1b0 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t SYNDROME: 8;				/* 1 */
 		vuint32_t ERR_TYPE: 1;				/* 1 */
 		vuint32_t ERR_BANK: 3;				/* 1 */
 		vuint32_t ECC_ERR: 1;				/* 1 */
  		vuint32_t: 19;
 
	} B;
	} ECC_ERRINFO_SEG2;	/* Register offset is 0x1b4 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t ERR: 16;				/* 1 */
  		vuint32_t: 16;
 
	} B;
	} ECC_ERROCCURRED_SEG1_2;	/* Register offset is 0x1b8 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ERR_ADDR_EVEN: 32;				/* 1 */
 
	} B;
	} ECC_ERRADDR_SEG0_EVEN;	/* Register offset is 0x1bc */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ERR_ADDR_ODD: 32;				/* 1 */
 
	} B;
	} ECC_ERRADDR_SEG0_ODD;	/* Register offset is 0x1c0 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ERR_DATA0_EVEN: 32;				/* 1 */
 
	} B;
	} ECC_ERRDATA0_SEG0_EVEN;	/* Register offset is 0x1c4 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ERR_DATA1_EVEN: 32;				/* 1 */
 
	} B;
	} ECC_ERRDATA1_SEG0_EVEN;	/* Register offset is 0x1c8 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ERR_DATA0_ODD: 32;				/* 1 */
 
	} B;
	} ECC_ERRDATA0_SEG0_ODD;	/* Register offset is 0x1cc */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ERR_DATA1_ODD: 32;				/* 1 */
 
	} B;
	} ECC_ERRDATA1_SEG0_ODD;	/* Register offset is 0x1d0 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t SYND_EVEN: 8;				/* 1 */
 		vuint32_t ERR_TYPE_EVEN: 1;				/* 1 */
 		vuint32_t ERR_EVEN_BANK: 4;				/* 1 */
 		vuint32_t ECC_ERR_EVEN: 1;				/* 1 */
 		vuint32_t: 2;
 		vuint32_t SYND_ODD: 8;				/* 1 */
 		vuint32_t ERR_TYPE_ODD: 1;				/* 1 */
 		vuint32_t ERR_ODD_BANK: 4;				/* 1 */
 		vuint32_t ECC_ERR_ODD: 1;				/* 1 */
  		vuint32_t: 2;
 
	} B;
	} ECC_ERRINFO_SEG0;	/* Register offset is 0x1d4 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t ERR: 24;				/* 1 */
  		vuint32_t: 8;
 
	} B;
	} ECC_ERROCCURED_SEG0;	/* Register offset is 0x1d8 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t ECC_ERR_CNTR_ODD: 6;				/* 1 */
 		vuint32_t ECC_ERR_OVF_ODD: 1;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t ECC_ERR_CNTR_EVEN: 6;				/* 1 */
 		vuint32_t ECC_ERR_OVF_EVEN: 1;				/* 1 */
  		vuint32_t: 17;
 
	} B;
	} ECC_SINGLE_CNTR_ODD_EVEN_0;	/* Register offset is 0x1dc */

	union {
		vuint32_t R;
		struct {
 		vuint32_t ECC_ERR_CNTR_ODD: 6;				/* 1 */
 		vuint32_t ECC_ERR_OVF_ODD: 1;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t ECC_ERR_CNTR_EVEN: 6;				/* 1 */
 		vuint32_t ECC_ERR_OVF_EVEN: 1;				/* 1 */
  		vuint32_t: 17;
 
	} B;
	} ECC_UNCORR_CNTR_ODD_EVEN_0;	/* Register offset is 0x1e0 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t ERROR_BANK: 12;				/* 1 */
  		vuint32_t: 20;
 
	} B;
	} PM_ERROR_0;	/* Register offset is 0x1e4 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ERROR_ADDR: 32;				/* 1 */
 
	} B;
	} PM_ERROR_ADDR_0;	/* Register offset is 0x1e8 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t BANKNUM: 4;				/* 1 */
 		vuint32_t PM_ERR: 1;				/* 1 */
 		vuint32_t: 6;
  		vuint32_t TRANSACTION_ID: 21;				/* 1 */
 
	} B;
	} PM_ERROR_INFO_0;	/* Register offset is 0x1ec */

	union {
		vuint32_t R;
		struct {
 		vuint32_t PM_BANK0: 3;				/* 1 */
 		vuint32_t PM_BANK1: 3;				/* 1 */
 		vuint32_t: 2;
 		vuint32_t PM_BANK2: 3;				/* 1 */
 		vuint32_t PM_BANK3: 3;				/* 1 */
 		vuint32_t: 2;
 		vuint32_t PM_BANK4: 3;				/* 1 */
 		vuint32_t PM_BANK5: 3;				/* 1 */
 		vuint32_t: 2;
 		vuint32_t PM_BANK6: 3;				/* 1 */
 		vuint32_t PM_BANK7: 3;				/* 1 */
  		vuint32_t: 2;
 
	} B;
	} POWMOD1_CTRL_0;	/* Register offset is 0x1f0 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t PM_BANK8: 3;				/* 1 */
 		vuint32_t PM_BANK9: 3;				/* 1 */
 		vuint32_t: 2;
 		vuint32_t PM_BANK10: 3;				/* 1 */
 		vuint32_t PM_BANK11: 3;				/* 1 */
  		vuint32_t: 18;
 
	} B;
	} POWMOD1_CTRL_0_10_11;	/* Register offset is 0x1f4 */

	uint8_t HPSMI_reserved_11[8] ; 
	union {
		vuint32_t R;
		struct {
 		vuint32_t: 16;
 		vuint32_t PM_SEG1: 3;				/* 1 */
 		vuint32_t PM_SEG2: 3;				/* 1 */
  		vuint32_t: 10;
 
	} B;
	} POWMOD1_CTRL;	/* Register offset is 0x200 */

	uint8_t HPSMI_reserved_12[4] ; 
	union {
		vuint32_t R;
		struct {
 		vuint32_t WTPT_INTREG: 1;				/* 1 */
  		vuint32_t: 31;
 
	} B;
	} WTPT_INTR;	/* Register offset is 0x208 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t m_cores_axi4: 4;				/* 1 */
 		vuint32_t m_pdi_axi5: 4;				/* 1 */
 		vuint32_t m_fastdma_axi6: 4;				/* 1 */
 		vuint32_t m_lsb0_axi7: 4;				/* 1 */
 		vuint32_t m_lsb1_axi8: 4;				/* 1 */
 		vuint32_t: 11;
  		vuint32_t WEN: 1;				/* 1 */
 
	} B;
	} QoS_Register;	/* Register offset is 0x20c */
}; 
 

/* ============================================================================
   ============================= Module: H264Encoder =============================
   ============================================================================ */
struct H264Encoder_tag {
	union {
		vuint32_t R;
		struct {
  		vuint32_t SW_RESET: 1;				/* 1 */
 		vuint32_t: 3;
 		vuint32_t VI_BUF_RESTART: 1;				/* 1 */
 		vuint32_t OUT_BUF_RESTART: 1;				/* 1 */
 		vuint32_t: 2;
 		vuint32_t VI_BIT_WIDTH: 2;				/* 1 */
 		vuint32_t: 2;
 		vuint32_t VI_COLOR_FMT: 1;				/* 1 */
 		vuint32_t: 3;
 		vuint32_t FETCH_TRIG: 1;				/* 1 */
 		vuint32_t: 7;
 		vuint32_t DATA_MODE_CHR: 1;				/* 1 */
 		vuint32_t: 7;
 
	} B;
	} MCR;	/* Register offset is 0x000 */

	uint8_t H264Encoder_reserved_0[12] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t: 4;
 		vuint32_t FR_WIDTH: 8;				/* 1 */
 		vuint32_t: 8;
 		vuint32_t FR_HEIGHT: 8;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} VCR;	/* Register offset is 0x010 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t MBRW_THLD: 8;				/* 1 */
 		vuint32_t: 8;
 		vuint32_t MBRW_ALARM_THLD: 8;				/* 1 */
 		vuint32_t: 8;
 
	} B;
	} VI_ALARM;	/* Register offset is 0x014 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t FETCH: 1;				/* 1 */
 		vuint32_t: 31;
 
	} B;
	} FETCH_MBRW;	/* Register offset is 0x018 */

	uint8_t H264Encoder_reserved_1[20] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 28;				/* 1 */
 		vuint32_t: 1;
 
	} B;
	} VI_CBUF_Y_ADDR;	/* Register offset is 0x030 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 28;				/* 1 */
 		vuint32_t: 1;
 
	} B;
	} VI_CBUF_CB_ADDR;	/* Register offset is 0x034 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 28;				/* 1 */
 		vuint32_t: 1;
 
	} B;
	} VI_CBUF_CR_ADDR;	/* Register offset is 0x038 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t: 1;
 		vuint32_t Y_LINES: 7;				/* 1 */
 		vuint32_t: 24;
 
	} B;
	} VI_NRLINES;	/* Register offset is 0x03C */

	union {
		vuint32_t R;
		struct {
  		vuint32_t BANK_STRIDE_LTH: 3;				/* 1 */
 		vuint32_t: 28;
 		vuint32_t EN: 1;				/* 1 */
 
	} B;
	} RATE_FLOW_CTRL;	/* Register offset is 0x040 */

	uint8_t H264Encoder_reserved_2[60] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 28;				/* 1 */
 		vuint32_t: 1;
 
	} B;
	} OUT_CBUF_START_ADDR;	/* Register offset is 0x080 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 28;				/* 1 */
 		vuint32_t: 1;
 
	} B;
	} OUT_CBUF_END_ADDR;	/* Register offset is 0x084 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 28;				/* 1 */
 		vuint32_t WRAP: 1;				/* 1 */
 
	} B;
	} OUT_CBUF_ALARM_ADDR;	/* Register offset is 0x088 */

	uint8_t H264Encoder_reserved_3[4] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 19;				/* 1 */
 		vuint32_t WRAP: 1;				/* 1 */
  		vuint32_t: 9;
	} B;
	} OUT_CBUF_CURR_ADDR;	/* Register offset is 0x090 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 28;				/* 1 */
 		vuint32_t: 1;
 
	} B;
	} OUT_CBUF_VEND_ADDR;	/* Register offset is 0x094 */

	uint8_t H264Encoder_reserved_4[108] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t VAL: 12;				/* 1 */
 		vuint32_t: 20;
 
	} B;
	} LINE_CNTR_STAT;	/* Register offset is 0x104 */

	uint8_t H264Encoder_reserved_5[20] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t VI_FRAME_DONE_INT: 1;				/* 1 */
 		vuint32_t BS_NEW_ROW_INT: 1;				/* 1 */
 		vuint32_t CB_ALARM_INT: 1;				/* 1 */
 		vuint32_t VI_LINE_INT: 1;				/* 1 */
 		vuint32_t VI_LCOUNT_ALARM_INT: 1;				/* 1 */
 		vuint32_t ENC_OOS_INT: 1;				/* 1 */
 		vuint32_t MBRW_DONE_INT: 1;				/* 1 */
 		vuint32_t MBRW_FF_OVF_INT: 1;				/* 1 */
 		vuint32_t BS_FRAME_DONE_INT: 1;				/* 1 */
 		vuint32_t OUT_CBUF_OVF_INT: 1;				/* 1 */
 		vuint32_t BR_TOO_HIGH_INT: 1;				/* 1 */
 		vuint32_t BS_FIFO_ALMOST_FULL_INT: 1;				/* 1 */
 		vuint32_t BS_FIFO_FULL_INT: 1;				/* 1 */
 		vuint32_t: 19;
 
	} B;
	} ISR;	/* Register offset is 0x11C */

	union {
		vuint32_t R;
		struct {
  		vuint32_t VI_FRAME_DONE_INT_EN: 1;				/* 1 */
 		vuint32_t BS_NEW_ROW_INT_EN: 1;				/* 1 */
 		vuint32_t CB_ALARM_INT_EN: 1;				/* 1 */
 		vuint32_t VI_LINE_INT_EN: 1;				/* 1 */
 		vuint32_t VI_LCOUNT_ALARM_INT_EN: 1;				/* 1 */
 		vuint32_t ENC_OOS_INT_EN: 1;				/* 1 */
 		vuint32_t MBRW_DONE_INT_EN: 1;				/* 1 */
 		vuint32_t MBRW_FF_OVF_EN: 1;				/* 1 */
 		vuint32_t BS_FRAME_DONE_INT_EN: 1;				/* 1 */
 		vuint32_t OUT_CBUF_OVF_INT_EN: 1;				/* 1 */
 		vuint32_t BR_TOO_HIGH_INT_EN: 1;				/* 1 */
 		vuint32_t BS_FIFO_ALMOST_FULL_INT_EN: 1;				/* 1 */
 		vuint32_t BS_FIFO_FULL_INT_EN: 1;				/* 1 */
 		vuint32_t: 19;
 
	} B;
	} IER;	/* Register offset is 0x120 */

	uint8_t H264Encoder_reserved_6[28] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t NR_W: 6;				/* 1 */
 		vuint32_t: 25;
 		vuint32_t EN: 1;				/* 1 */
 
	} B;
	} TESTLINE_CFG;	/* Register offset is 0x140 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STRT_LOC: 12;				/* 1 */
 		vuint32_t: 20;
 
	} B;
	} TESTLINE_STRT_LOC;	/* Register offset is 0x144 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t VAL_1: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t VAL_2: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} TESTLINE_LUMA_VAL;	/* Register offset is 0x148 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t VAL_1: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t VAL_2: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} TESTLINE_CB_VAL;	/* Register offset is 0x14C */

	union {
		vuint32_t R;
		struct {
  		vuint32_t VAL_1: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t VAL_2: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} TESTLINE_CR_VAL;	/* Register offset is 0x150 */

	uint8_t H264Encoder_reserved_7[172] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t VAL: 8;				/* 1 */
 		vuint32_t: 24;
 
	} B;
	} ULLVC_FRAME_RATE;	/* Register offset is 0x200 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t INITIAL: 7;				/* 1 */
 		vuint32_t: 25;
 
	} B;
	} ULLVC_QP_INIT;	/* Register offset is 0x204 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t MIN: 7;				/* 1 */
 		vuint32_t: 9;
 		vuint32_t MAX: 7;				/* 1 */
 		vuint32_t: 9;
 
	} B;
	} ULLVC_QP_RANGE;	/* Register offset is 0x208 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t BITRATE: 31;				/* 1 */
 		vuint32_t: 1;
 
	} B;
	} ULLVC_BITS_PER_MB_ROW;	/* Register offset is 0x20C */

	union {
		vuint32_t R;
		struct {
  		vuint32_t LIMIT: 8;				/* 1 */
 		vuint32_t: 24;
 
	} B;
	} ULLVC_QP_FALLBACK_LIMIT;	/* Register offset is 0x210 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t OFFSET: 7;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t DIFF_0: 7;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t DIFF_1: 7;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t DIFF_2: 7;				/* 1 */
 		vuint32_t: 1;
 
	} B;
	} ULLVC_QP_INC;	/* Register offset is 0x214 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t THLD: 31;				/* 1 */
 		vuint32_t: 1;
 
	} B;
	} ULLVC_QP_INC_THLD0;	/* Register offset is 0x0218 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t THLD: 31;				/* 1 */
 		vuint32_t: 1;
 
	} B;
	} ULLVC_QP_INC_THLD1;	/* Register offset is 0x021c */

	union {
		vuint32_t R;
		struct {
  		vuint32_t THLD: 31;				/* 1 */
 		vuint32_t: 1;
 
	} B;
	} ULLVC_QP_INC_THLD2;	/* Register offset is 0x0220 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t OFFSET: 7;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t DIFF_0: 7;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t DIFF_1: 7;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t DIFF_2: 7;				/* 1 */
 		vuint32_t: 1;
 
	} B;
	} ULLVC_QP_DEC;	/* Register offset is 0x224 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t THLD: 31;				/* 1 */
 		vuint32_t: 1;
 
	} B;
	} ULLVC_QP_DEC_THLD0;	/* Register offset is 0x0228 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t THLD: 31;				/* 1 */
 		vuint32_t: 1;
 
	} B;
	} ULLVC_QP_DEC_THLD1;	/* Register offset is 0x022c */

	union {
		vuint32_t R;
		struct {
  		vuint32_t THLD: 31;				/* 1 */
 		vuint32_t: 1;
 
	} B;
	} ULLVC_QP_DEC_THLD2;	/* Register offset is 0x0230 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t WAIT_FRAMES: 6;				/* 1 */
 		vuint32_t: 26;
 
	} B;
	} ULLVC_WAIT_FRAMES;	/* Register offset is 0x234 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t DISABLE: 1;				/* 1 */
 		vuint32_t: 31;
 
	} B;
	} ULLVC_DISABLE_DBF;	/* Register offset is 0x238 */

	uint8_t H264Encoder_reserved_8[72] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t VAL: 32;				/* 1 */
 
	} B;
	} ULLVC_BITRATE_STREAM;	/* Register offset is 0x284 */
}; 
 

/* ============================================================================
   ============================= Module: H264Decoder =============================
   ============================================================================ */
struct H264Decoder_tag {
	union {
		vuint32_t R;
		struct {
  		vuint32_t SW_RESET: 1;				/* 1 */
 		vuint32_t VO_BUF_0_RSTRT: 1;				/* 1 */
 		vuint32_t VO_BUF_1_RSTRT: 1;				/* 1 */
 		vuint32_t VO_BUF_2_RSTRT: 1;				/* 1 */
 		vuint32_t VO_BUF_3_RSTRT: 1;				/* 1 */
 		vuint32_t: 3;
 		vuint32_t VO_BIT_WIDTH_STR_0: 1;				/* 1 */
 		vuint32_t VO_BIT_WIDTH_STR_1: 1;				/* 1 */
 		vuint32_t VO_BIT_WIDTH_STR_2: 1;				/* 1 */
 		vuint32_t VO_BIT_WIDTH_STR_3: 1;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t DATA_FLOW_MODE: 2;				/* 1 */
 		vuint32_t: 2;
 		vuint32_t VO_COLOR_FMT_STR_0: 1;				/* 1 */
 		vuint32_t VO_COLOR_FMT_STR_1: 1;				/* 1 */
 		vuint32_t VO_COLOR_FMT_STR_2: 1;				/* 1 */
 		vuint32_t VO_COLOR_FMT_STR_3: 1;				/* 1 */
 		vuint32_t DATA_MODE_CHR: 1;				/* 1 */
 		vuint32_t: 7;
 
	} B;
	} MCR;	/* Register offset is 0x000 */

	uint8_t H264Decoder_reserved_0[12] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_VCR_FR_WIDTH: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t STR_VCR_FR_HEIGHT: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} STR_VCR0;	/* Register offset is 0x0010 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_VCR_FR_WIDTH: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t STR_VCR_FR_HEIGHT: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} STR_VCR1;	/* Register offset is 0x0014 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_VCR_FR_WIDTH: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t STR_VCR_FR_HEIGHT: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} STR_VCR2;	/* Register offset is 0x0018 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_VCR_FR_WIDTH: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t STR_VCR_FR_HEIGHT: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} STR_VCR3;	/* Register offset is 0x001c */

	uint8_t H264Decoder_reserved_1[16] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t ADDR: 21;				/* 1 */
 		vuint32_t: 11;
 
	} B;
	} STR_PKT_ADDR;	/* Register offset is 0x030 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_PKT_LTH: 15;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_NUM: 2;				/* 1 */
 
	} B;
	} STR_TRIG_PKT_CFG;	/* Register offset is 0x034 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_0: 8;				/* 1 */
 		vuint32_t STR_1: 8;				/* 1 */
 		vuint32_t STR_2: 8;				/* 1 */
 		vuint32_t STR_3: 8;				/* 1 */
 
	} B;
	} STR_PKT_FIFO_WMRK;	/* Register offset is 0x038 */

	uint8_t H264Decoder_reserved_2[4] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_0_LVL: 8;				/* 1 */
 		vuint32_t STR_1_LVL: 8;				/* 1 */
 		vuint32_t STR_0_ACTIV_LOC: 8;				/* 1 */
 		vuint32_t STR_1_ACTIV_LOC: 8;				/* 1 */
 
	} B;
	} STR_01_PKT_FIFO_STAT;	/* Register offset is 0x040 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_2_LVL: 8;				/* 1 */
 		vuint32_t STR_3_LVL: 8;				/* 1 */
 		vuint32_t STR_2_ACTIV_LOC: 8;				/* 1 */
 		vuint32_t STR_3_ACTIV_LOC: 8;				/* 1 */
 
	} B;
	} STR_23_PKT_FIFO_STAT;	/* Register offset is 0x044 */

	uint8_t H264Decoder_reserved_3[8] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_0_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_0_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_0_PKT_STATUS0;	/* Register offset is 0x0050 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_0_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_0_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_0_PKT_STATUS1;	/* Register offset is 0x0054 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_0_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_0_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_0_PKT_STATUS2;	/* Register offset is 0x0058 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_0_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_0_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_0_PKT_STATUS3;	/* Register offset is 0x005c */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_0_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_0_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_0_PKT_STATUS4;	/* Register offset is 0x0060 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_0_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_0_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_0_PKT_STATUS5;	/* Register offset is 0x0064 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_0_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_0_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_0_PKT_STATUS6;	/* Register offset is 0x0068 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_0_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_0_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_0_PKT_STATUS7;	/* Register offset is 0x006c */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_0_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_0_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_0_PKT_STATUS8;	/* Register offset is 0x0070 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_0_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_0_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_0_PKT_STATUS9;	/* Register offset is 0x0074 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_0_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_0_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_0_PKT_STATUS10;	/* Register offset is 0x0078 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_0_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_0_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_0_PKT_STATUS11;	/* Register offset is 0x007c */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_0_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_0_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_0_PKT_STATUS12;	/* Register offset is 0x0080 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_0_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_0_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_0_PKT_STATUS13;	/* Register offset is 0x0084 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_0_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_0_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_0_PKT_STATUS14;	/* Register offset is 0x0088 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_0_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_0_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_0_PKT_STATUS15;	/* Register offset is 0x008c */

	uint8_t H264Decoder_reserved_4[64] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_1_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_1_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_1_PKT_STATUS0;	/* Register offset is 0x00d0 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_1_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_1_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_1_PKT_STATUS1;	/* Register offset is 0x00d4 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_1_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_1_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_1_PKT_STATUS2;	/* Register offset is 0x00d8 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_1_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_1_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_1_PKT_STATUS3;	/* Register offset is 0x00dc */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_1_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_1_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_1_PKT_STATUS4;	/* Register offset is 0x00e0 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_1_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_1_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_1_PKT_STATUS5;	/* Register offset is 0x00e4 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_1_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_1_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_1_PKT_STATUS6;	/* Register offset is 0x00e8 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_1_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_1_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_1_PKT_STATUS7;	/* Register offset is 0x00ec */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_1_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_1_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_1_PKT_STATUS8;	/* Register offset is 0x00f0 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_1_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_1_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_1_PKT_STATUS9;	/* Register offset is 0x00f4 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_1_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_1_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_1_PKT_STATUS10;	/* Register offset is 0x00f8 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_1_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_1_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_1_PKT_STATUS11;	/* Register offset is 0x00fc */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_1_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_1_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_1_PKT_STATUS12;	/* Register offset is 0x0100 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_1_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_1_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_1_PKT_STATUS13;	/* Register offset is 0x0104 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_1_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_1_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_1_PKT_STATUS14;	/* Register offset is 0x0108 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_1_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_1_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_1_PKT_STATUS15;	/* Register offset is 0x010c */

	uint8_t H264Decoder_reserved_5[64] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_2_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_2_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_2_PKT_STATUS0;	/* Register offset is 0x0150 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_2_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_2_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_2_PKT_STATUS1;	/* Register offset is 0x0154 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_2_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_2_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_2_PKT_STATUS2;	/* Register offset is 0x0158 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_2_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_2_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_2_PKT_STATUS3;	/* Register offset is 0x015c */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_2_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_2_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_2_PKT_STATUS4;	/* Register offset is 0x0160 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_2_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_2_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_2_PKT_STATUS5;	/* Register offset is 0x0164 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_2_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_2_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_2_PKT_STATUS6;	/* Register offset is 0x0168 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_2_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_2_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_2_PKT_STATUS7;	/* Register offset is 0x016c */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_2_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_2_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_2_PKT_STATUS8;	/* Register offset is 0x0170 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_2_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_2_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_2_PKT_STATUS9;	/* Register offset is 0x0174 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_2_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_2_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_2_PKT_STATUS10;	/* Register offset is 0x0178 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_2_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_2_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_2_PKT_STATUS11;	/* Register offset is 0x017c */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_2_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_2_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_2_PKT_STATUS12;	/* Register offset is 0x0180 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_2_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_2_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_2_PKT_STATUS13;	/* Register offset is 0x0184 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_2_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_2_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_2_PKT_STATUS14;	/* Register offset is 0x0188 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_2_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_2_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_2_PKT_STATUS15;	/* Register offset is 0x018c */

	uint8_t H264Decoder_reserved_6[64] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_3_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_3_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_3_PKT_STATUS0;	/* Register offset is 0x01d0 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_3_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_3_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_3_PKT_STATUS1;	/* Register offset is 0x01d4 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_3_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_3_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_3_PKT_STATUS2;	/* Register offset is 0x01d8 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_3_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_3_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_3_PKT_STATUS3;	/* Register offset is 0x01dc */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_3_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_3_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_3_PKT_STATUS4;	/* Register offset is 0x01e0 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_3_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_3_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_3_PKT_STATUS5;	/* Register offset is 0x01e4 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_3_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_3_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_3_PKT_STATUS6;	/* Register offset is 0x01e8 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_3_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_3_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_3_PKT_STATUS7;	/* Register offset is 0x01ec */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_3_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_3_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_3_PKT_STATUS8;	/* Register offset is 0x01f0 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_3_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_3_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_3_PKT_STATUS9;	/* Register offset is 0x01f4 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_3_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_3_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_3_PKT_STATUS10;	/* Register offset is 0x01f8 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_3_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_3_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_3_PKT_STATUS11;	/* Register offset is 0x01fc */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_3_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_3_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_3_PKT_STATUS12;	/* Register offset is 0x0200 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_3_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_3_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_3_PKT_STATUS13;	/* Register offset is 0x0204 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_3_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_3_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_3_PKT_STATUS14;	/* Register offset is 0x0208 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_3_PKT_STATUS_ADDR_OFFSET: 16;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STR_3_PKT_STATUS_VALID: 1;				/* 1 */
 
	} B;
	} STR_3_PKT_STATUS15;	/* Register offset is 0x020c */

	uint8_t H264Decoder_reserved_7[240] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 19;				/* 1 */
 		vuint32_t: 10;
 
	} B;
	} VO_STR0_Y_ADDR;	/* Register offset is 0x0300 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 19;				/* 1 */
 		vuint32_t: 10;
 
	} B;
	} VO_STR0_CB_ADDR;	/* Register offset is 0x0304 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 19;				/* 1 */
 		vuint32_t: 10;
 
	} B;
	} VO_STR0_CR_ADDR;	/* Register offset is 0x0308 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t VO_STR_NRLINES_Y_LINES: 16;				/* 1 */
 		vuint32_t: 16;
 
	} B;
	} VO_STR0_NRLINES;	/* Register offset is 0x030c */

	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 19;				/* 1 */
 		vuint32_t: 10;
 
	} B;
	} VO_STR1_Y_ADDR;	/* Register offset is 0x0310 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 19;				/* 1 */
 		vuint32_t: 10;
 
	} B;
	} VO_STR1_CB_ADDR;	/* Register offset is 0x0314 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 19;				/* 1 */
 		vuint32_t: 10;
 
	} B;
	} VO_STR1_CR_ADDR;	/* Register offset is 0x0318 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t VO_STR_NRLINES_Y_LINES: 16;				/* 1 */
 		vuint32_t: 16;
 
	} B;
	} VO_STR1_NRLINES;	/* Register offset is 0x031c */

	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 19;				/* 1 */
 		vuint32_t: 10;
 
	} B;
	} VO_STR2_Y_ADDR;	/* Register offset is 0x0320 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 19;				/* 1 */
 		vuint32_t: 10;
 
	} B;
	} VO_STR2_CB_ADDR;	/* Register offset is 0x0324 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 19;				/* 1 */
 		vuint32_t: 10;
 
	} B;
	} VO_STR2_CR_ADDR;	/* Register offset is 0x0328 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t VO_STR_NRLINES_Y_LINES: 16;				/* 1 */
 		vuint32_t: 16;
 
	} B;
	} VO_STR2_NRLINES;	/* Register offset is 0x032c */

	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 19;				/* 1 */
 		vuint32_t: 10;
 
	} B;
	} VO_STR3_Y_ADDR;	/* Register offset is 0x0330 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 19;				/* 1 */
 		vuint32_t: 10;
 
	} B;
	} VO_STR3_CB_ADDR;	/* Register offset is 0x0334 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t: 3;
 		vuint32_t ADDR: 19;				/* 1 */
 		vuint32_t: 10;
 
	} B;
	} VO_STR3_CR_ADDR;	/* Register offset is 0x0338 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t VO_STR_NRLINES_Y_LINES: 16;				/* 1 */
 		vuint32_t: 16;
 
	} B;
	} VO_STR3_NRLINES;	/* Register offset is 0x033c */

	uint8_t H264Decoder_reserved_8[32] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t BANK_STRIDE_LTH: 3;				/* 1 */
 		vuint32_t: 28;
 		vuint32_t EN: 1;				/* 1 */
 
	} B;
	} RATE_FLOW_CNTRL;	/* Register offset is 0x360 */

	uint8_t H264Decoder_reserved_9[28] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t STR_0: 8;				/* 1 */
 		vuint32_t STR_1: 8;				/* 1 */
 		vuint32_t STR_2: 8;				/* 1 */
 		vuint32_t STR_3: 8;				/* 1 */
 
	} B;
	} LINE_CNT_STAT;	/* Register offset is 0x380 */

	uint8_t H264Decoder_reserved_10[92] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t EOF_0_INT: 1;				/* 1 */
 		vuint32_t EOF_1_INT: 1;				/* 1 */
 		vuint32_t EOF_2_INT: 1;				/* 1 */
 		vuint32_t EOF_3_INT: 1;				/* 1 */
 		vuint32_t MBROW_DONE_0_INT: 1;				/* 1 */
 		vuint32_t MBROW_DONE_1_INT: 1;				/* 1 */
 		vuint32_t MBROW_DONE_2_INT: 1;				/* 1 */
 		vuint32_t MBROW_DONE_3_INT: 1;				/* 1 */
 		vuint32_t STR_0_PKT_FIFO_WE_INT: 1;				/* 1 */
 		vuint32_t STR_1_PKT_FIFO_WE_INT: 1;				/* 1 */
 		vuint32_t STR_2_PKT_FIFO_WE_INT: 1;				/* 1 */
 		vuint32_t STR_3_PKT_FIFO_WE_INT: 1;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t FRM_CYC_CNT_GRT_THR_0_CH_0_INT: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_0_CH_1_INT: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_0_CH_2_INT: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_0_CH_3_INT: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_1_CH_0_INT: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_1_CH_1_INT: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_1_CH_2_INT: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_1_CH_3_INT: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_2_CH_0_INT: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_2_CH_1_INT: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_2_CH_2_INT: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_2_CH_3_INT: 1;				/* 1 */
 		vuint32_t: 2;
 		vuint32_t DEC_OOS_INT: 1;				/* 1 */
 		vuint32_t ERR_REQ_INT: 1;				/* 1 */
 
	} B;
	} ISR;	/* Register offset is 0x3E0 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t EOF_0_EN: 1;				/* 1 */
 		vuint32_t EOF_1_EN: 1;				/* 1 */
 		vuint32_t EOF_2_EN: 1;				/* 1 */
 		vuint32_t EOF_3_EN: 1;				/* 1 */
 		vuint32_t MBROW_DONE_0_EN: 1;				/* 1 */
 		vuint32_t MBROW_DONE_1_EN: 1;				/* 1 */
 		vuint32_t MBROW_DONE_2_EN: 1;				/* 1 */
 		vuint32_t MBROW_DONE_3_EN: 1;				/* 1 */
 		vuint32_t STR_0_PKT_FIFO_WE_EN: 1;				/* 1 */
 		vuint32_t STR_1_PKT_FIFO_WE_EN: 1;				/* 1 */
 		vuint32_t STR_2_PKT_FIFO_WE_EN: 1;				/* 1 */
 		vuint32_t STR_3_PKT_FIFO_WE_EN: 1;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t FRM_CYC_CNT_GRT_THR_0_CH_0_EN: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_0_CH_1_EN: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_0_CH_2_EN: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_0_CH_3_EN: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_1_CH_0_EN: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_1_CH_1_EN: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_1_CH_2_EN: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_1_CH_3_EN: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_2_CH_0_EN: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_2_CH_1_EN: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_2_CH_2_EN: 1;				/* 1 */
 		vuint32_t FRM_CYC_CNT_GRT_THR_2_CH_3_EN: 1;				/* 1 */
 		vuint32_t: 2;
 		vuint32_t DEC_OOS_EN: 1;				/* 1 */
 		vuint32_t ERR_REQ_EN: 1;				/* 1 */
 
	} B;
	} IER;	/* Register offset is 0x3E4 */

	uint8_t H264Decoder_reserved_11[24] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t STRT_LOC: 12;				/* 1 */
 		vuint32_t: 19;
 		vuint32_t EN: 1;				/* 1 */
 
	} B;
	} TESTLINE_CFG;	/* Register offset is 0x400 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t: 4;
 		vuint32_t LOC_1: 8;				/* 1 */
 		vuint32_t: 8;
 		vuint32_t LOC_2: 8;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} TESTLINE_PXL_LOC;	/* Register offset is 0x404 */

	uint8_t H264Decoder_reserved_12[8] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t VAL_1: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t VAL_2: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} TESTLINE_STR0_LUMA_VAL;	/* Register offset is 0x0410 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t VAL_1: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t VAL_2: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} TESTLINE_STR0_CB_VAL;	/* Register offset is 0x0414 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t VAL_1: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t VAL_2: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} TESTLINE_STR0_CR_VAL;	/* Register offset is 0x0418 */

	uint8_t H264Decoder_reserved_13[4] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t VAL_1: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t VAL_2: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} TESTLINE_STR1_LUMA_VAL;	/* Register offset is 0x0420 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t VAL_1: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t VAL_2: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} TESTLINE_STR1_CB_VAL;	/* Register offset is 0x0424 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t VAL_1: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t VAL_2: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} TESTLINE_STR1_CR_VAL;	/* Register offset is 0x0428 */

	uint8_t H264Decoder_reserved_14[4] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t VAL_1: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t VAL_2: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} TESTLINE_STR2_LUMA_VAL;	/* Register offset is 0x0430 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t VAL_1: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t VAL_2: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} TESTLINE_STR2_CB_VAL;	/* Register offset is 0x0434 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t VAL_1: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t VAL_2: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} TESTLINE_STR2_CR_VAL;	/* Register offset is 0x0438 */

	uint8_t H264Decoder_reserved_15[4] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t VAL_1: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t VAL_2: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} TESTLINE_STR3_LUMA_VAL;	/* Register offset is 0x0440 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t VAL_1: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t VAL_2: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} TESTLINE_STR3_CB_VAL;	/* Register offset is 0x0444 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t VAL_1: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t VAL_2: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} TESTLINE_STR3_CR_VAL;	/* Register offset is 0x0448 */

	uint8_t H264Decoder_reserved_16[180] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t DBF_FORCE_OFF: 1;				/* 1 */
 		vuint32_t: 7;
 		vuint32_t USE_SRAM_DDR: 1;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STOP_CH_0: 1;				/* 1 */
 		vuint32_t STOP_CH_1: 1;				/* 1 */
 		vuint32_t STOP_CH_2: 1;				/* 1 */
 		vuint32_t STOP_CH_3: 1;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} ELLVC_CFG;	/* Register offset is 0x500 */

	uint8_t H264Decoder_reserved_17[12] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t ADDR: 32;				/* 1 */
 
	} B;
	} ELLVC_REF_ADDR_CH0;	/* Register offset is 0x0510 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ADDR: 32;				/* 1 */
 
	} B;
	} ELLVC_REF_ADDR_CH1;	/* Register offset is 0x0514 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ADDR: 32;				/* 1 */
 
	} B;
	} ELLVC_REF_ADDR_CH2;	/* Register offset is 0x0518 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ADDR: 32;				/* 1 */
 
	} B;
	} ELLVC_REF_ADDR_CH3;	/* Register offset is 0x051c */

	uint8_t H264Decoder_reserved_18[16] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t ELLVC_DBG_FRAME_CYC_CNT_THR_VAL: 32;				/* 1 */
 
	} B;
	} ELLVC_DBG_FRAME_CYC_CNT_THR0;	/* Register offset is 0x0530 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ELLVC_DBG_FRAME_CYC_CNT_THR_VAL: 32;				/* 1 */
 
	} B;
	} ELLVC_DBG_FRAME_CYC_CNT_THR1;	/* Register offset is 0x0534 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ELLVC_DBG_FRAME_CYC_CNT_THR_VAL: 32;				/* 1 */
 
	} B;
	} ELLVC_DBG_FRAME_CYC_CNT_THR2;	/* Register offset is 0x0538 */

	uint8_t H264Decoder_reserved_19[36] ; 
	union {
		vuint32_t R;
		struct {
  		vuint32_t CH_0: 5;				/* 1 */
 		vuint32_t: 3;
 		vuint32_t CH_1: 5;				/* 1 */
 		vuint32_t: 3;
 		vuint32_t CH_2: 5;				/* 1 */
 		vuint32_t: 3;
 		vuint32_t CH_3: 5;				/* 1 */
 		vuint32_t: 3;
 
	} B;
	} ELLVC_STATE;	/* Register offset is 0x560 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CH_0: 2;				/* 1 */
 		vuint32_t: 6;
 		vuint32_t CH_1: 2;				/* 1 */
 		vuint32_t: 6;
 		vuint32_t CH_2: 2;				/* 1 */
 		vuint32_t: 6;
 		vuint32_t CH_3: 2;				/* 1 */
 		vuint32_t: 6;
 
	} B;
	} ELLVC_PIC_BIT;	/* Register offset is 0x564 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CH_0: 8;				/* 1 */
 		vuint32_t CH_1: 8;				/* 1 */
 		vuint32_t CH_2: 8;				/* 1 */
 		vuint32_t CH_3: 8;				/* 1 */
 
	} B;
	} ELLVC_PIC_WIDTH_IN_MBS;	/* Register offset is 0x568 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CH_0: 8;				/* 1 */
 		vuint32_t CH_1: 8;				/* 1 */
 		vuint32_t CH_2: 8;				/* 1 */
 		vuint32_t CH_3: 8;				/* 1 */
 
	} B;
	} ELLVC_PIC_HEIGHT_IN_MBS;	/* Register offset is 0x56C */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CH_0: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t CH_1: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} ELLVC_PIC_CROP_LEFT_CH_01;	/* Register offset is 0x570 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CH_2: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t CH_3: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} ELLVC_PIC_CROP_LEFT_CH_23;	/* Register offset is 0x574 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CH_0: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t CH_1: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} ELLVC_PIC_CROP_RGHT_CH_01;	/* Register offset is 0x578 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CH_2: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t CH_3: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} ELLVC_PIC_CROP_RGHT_CH_23;	/* Register offset is 0x57C */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CH_0: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t CH_1: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} ELLVC_PIC_CROP_TOP_CH_01;	/* Register offset is 0x580 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CH_2: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t CH_3: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} ELLVC_PIC_CROP_TOP_CH_23;	/* Register offset is 0x584 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CH_0: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t CH_1: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} ELLVC_PIC_CROP_BTTM_CH_01;	/* Register offset is 0x588 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CH_2: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t CH_3: 12;				/* 1 */
 		vuint32_t: 4;
 
	} B;
	} ELLVC_PIC_CROP_BTTM_CH_23;	/* Register offset is 0x58C */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CH_0: 1;				/* 1 */
 		vuint32_t: 7;
 		vuint32_t CH_1: 1;				/* 1 */
 		vuint32_t: 7;
 		vuint32_t CH_2: 1;				/* 1 */
 		vuint32_t: 7;
 		vuint32_t CH_3: 1;				/* 1 */
 		vuint32_t: 7;
 
	} B;
	} ELLVC_PIC_PARAM_VLD;	/* Register offset is 0x590 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t CH_0: 32;				/* 1 */
 
	} B;
	} ELLVC_POC_VALUE;	/* Register offset is 0x594 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t MAX_NUM_REF_FRM_CH_0: 5;				/* 1 */
 		vuint32_t: 12;
 		vuint32_t NEW_POC_CYC_CH_0: 1;				/* 1 */
 		vuint32_t: 14;
 
	} B;
	} ELLVC_STAT;	/* Register offset is 0x598 */
}; 
/* ============================================================================
   ============================= Module: JPEGDecoder =============================
   ============================================================================ */
struct JPEGDecoder_tag {
	union {
		vuint32_t R;
		struct {
 
  		vuint32_t POINTER: 32;				/* 1 */
	} B;
	} ST1_SRAM_PTR;	/* Register offset is 0x0 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST1_SRAM_LEN_VAL;	/* Register offset is 0x4 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST1_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST1_PTR_FIFO_00;	/* Register offset is 0x0008 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST1_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST1_PTR_FIFO_01;	/* Register offset is 0x000c */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST1_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST1_PTR_FIFO_02;	/* Register offset is 0x0010 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST1_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST1_PTR_FIFO_03;	/* Register offset is 0x0014 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST1_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST1_PTR_FIFO_04;	/* Register offset is 0x0018 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST1_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST1_PTR_FIFO_05;	/* Register offset is 0x001c */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST1_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST1_PTR_FIFO_06;	/* Register offset is 0x0020 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST1_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST1_PTR_FIFO_07;	/* Register offset is 0x0024 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST1_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST1_PTR_FIFO_08;	/* Register offset is 0x0028 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST1_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST1_PTR_FIFO_09;	/* Register offset is 0x002c */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST1_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST1_PTR_FIFO_010;	/* Register offset is 0x0030 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST1_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST1_PTR_FIFO_011;	/* Register offset is 0x0034 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST1_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST1_PTR_FIFO_012;	/* Register offset is 0x0038 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST1_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST1_PTR_FIFO_013;	/* Register offset is 0x003c */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST1_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST1_PTR_FIFO_014;	/* Register offset is 0x0040 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST1_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST1_PTR_FIFO_015;	/* Register offset is 0x0044 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST1_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST1_LEN_VAL_FIFO_00;	/* Register offset is 0x0048 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST1_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST1_LEN_VAL_FIFO_01;	/* Register offset is 0x004c */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST1_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST1_LEN_VAL_FIFO_02;	/* Register offset is 0x0050 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST1_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST1_LEN_VAL_FIFO_03;	/* Register offset is 0x0054 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST1_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST1_LEN_VAL_FIFO_04;	/* Register offset is 0x0058 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST1_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST1_LEN_VAL_FIFO_05;	/* Register offset is 0x005c */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST1_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST1_LEN_VAL_FIFO_06;	/* Register offset is 0x0060 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST1_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST1_LEN_VAL_FIFO_07;	/* Register offset is 0x0064 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST1_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST1_LEN_VAL_FIFO_08;	/* Register offset is 0x0068 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST1_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST1_LEN_VAL_FIFO_09;	/* Register offset is 0x006c */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST1_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST1_LEN_VAL_FIFO_010;	/* Register offset is 0x0070 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST1_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST1_LEN_VAL_FIFO_011;	/* Register offset is 0x0074 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST1_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST1_LEN_VAL_FIFO_012;	/* Register offset is 0x0078 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST1_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST1_LEN_VAL_FIFO_013;	/* Register offset is 0x007c */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST1_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST1_LEN_VAL_FIFO_014;	/* Register offset is 0x0080 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST1_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST1_LEN_VAL_FIFO_015;	/* Register offset is 0x0084 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t POINTER: 32;				/* 1 */
	} B;
	} ST2_SRAM_PTR;	/* Register offset is 0x88 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST2_SRAM_LEN_VAL;	/* Register offset is 0x8C */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST2_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST2_PTR_FIFO_00;	/* Register offset is 0x0090 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST2_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST2_PTR_FIFO_01;	/* Register offset is 0x0094 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST2_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST2_PTR_FIFO_02;	/* Register offset is 0x0098 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST2_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST2_PTR_FIFO_03;	/* Register offset is 0x009c */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST2_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST2_PTR_FIFO_04;	/* Register offset is 0x00a0 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST2_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST2_PTR_FIFO_05;	/* Register offset is 0x00a4 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST2_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST2_PTR_FIFO_06;	/* Register offset is 0x00a8 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ST2_PTR_FIFO_0_POINTER: 32;				/* 1 */
 
	} B;
	} ST2_PTR_FIFO_07;	/* Register offset is 0x00ac */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ST2_PTR_FIFO_0_POINTER: 32;				/* 1 */
 
	} B;
	} ST2_PTR_FIFO_08;	/* Register offset is 0x00b0 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ST2_PTR_FIFO_0_POINTER: 32;				/* 1 */
 
	} B;
	} ST2_PTR_FIFO_09;	/* Register offset is 0x00b4 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ST2_PTR_FIFO_0_POINTER: 32;				/* 1 */
 
	} B;
	} ST2_PTR_FIFO_010;	/* Register offset is 0x00b8 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST2_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST2_PTR_FIFO_011;	/* Register offset is 0x00bc */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ST2_PTR_FIFO_0_POINTER: 32;				/* 1 */
 
	} B;
	} ST2_PTR_FIFO_012;	/* Register offset is 0x00c0 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ST2_PTR_FIFO_0_POINTER: 32;				/* 1 */
 
	} B;
	} ST2_PTR_FIFO_013;	/* Register offset is 0x00c4 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ST2_PTR_FIFO_0_POINTER: 32;				/* 1 */
 
	} B;
	} ST2_PTR_FIFO_014;	/* Register offset is 0x00c8 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t ST2_PTR_FIFO_0_POINTER: 32;				/* 1 */
 
	} B;
	} ST2_PTR_FIFO_015;	/* Register offset is 0x00cc */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST2_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST2_LEN_VAL_FIFO_00;	/* Register offset is 0x00d0 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST2_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST2_LEN_VAL_FIFO_01;	/* Register offset is 0x00d4 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST2_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST2_LEN_VAL_FIFO_02;	/* Register offset is 0x00d8 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST2_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST2_LEN_VAL_FIFO_03;	/* Register offset is 0x00dc */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST2_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST2_LEN_VAL_FIFO_04;	/* Register offset is 0x00e0 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST2_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST2_LEN_VAL_FIFO_05;	/* Register offset is 0x00e4 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST2_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST2_LEN_VAL_FIFO_06;	/* Register offset is 0x00e8 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST2_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST2_LEN_VAL_FIFO_07;	/* Register offset is 0x00ec */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST2_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST2_LEN_VAL_FIFO_08;	/* Register offset is 0x00f0 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST2_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST2_LEN_VAL_FIFO_09;	/* Register offset is 0x00f4 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST2_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST2_LEN_VAL_FIFO_010;	/* Register offset is 0x00f8 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST2_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST2_LEN_VAL_FIFO_011;	/* Register offset is 0x00fc */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST2_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST2_LEN_VAL_FIFO_012;	/* Register offset is 0x0100 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST2_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST2_LEN_VAL_FIFO_013;	/* Register offset is 0x0104 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST2_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST2_LEN_VAL_FIFO_014;	/* Register offset is 0x0108 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST2_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST2_LEN_VAL_FIFO_015;	/* Register offset is 0x010c */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t POINTER: 32;				/* 1 */
	} B;
	} ST3_SRAM_PTR;	/* Register offset is 0x110 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST3_SRAM_LEN_VAL;	/* Register offset is 0x114 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST3_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST3_PTR_FIFO_00;	/* Register offset is 0x0118 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST3_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST3_PTR_FIFO_01;	/* Register offset is 0x011c */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST3_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST3_PTR_FIFO_02;	/* Register offset is 0x0120 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST3_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST3_PTR_FIFO_03;	/* Register offset is 0x0124 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST3_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST3_PTR_FIFO_04;	/* Register offset is 0x0128 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST3_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST3_PTR_FIFO_05;	/* Register offset is 0x012c */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST3_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST3_PTR_FIFO_06;	/* Register offset is 0x0130 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST3_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST3_PTR_FIFO_07;	/* Register offset is 0x0134 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST3_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST3_PTR_FIFO_08;	/* Register offset is 0x0138 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST3_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST3_PTR_FIFO_09;	/* Register offset is 0x013c */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST3_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST3_PTR_FIFO_010;	/* Register offset is 0x0140 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST3_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST3_PTR_FIFO_011;	/* Register offset is 0x0144 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST3_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST3_PTR_FIFO_012;	/* Register offset is 0x0148 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST3_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST3_PTR_FIFO_013;	/* Register offset is 0x014c */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST3_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST3_PTR_FIFO_014;	/* Register offset is 0x0150 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST3_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST3_PTR_FIFO_015;	/* Register offset is 0x0154 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST3_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST3_LEN_VAL_FIFO_00;	/* Register offset is 0x0158 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST3_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST3_LEN_VAL_FIFO_01;	/* Register offset is 0x015c */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST3_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST3_LEN_VAL_FIFO_02;	/* Register offset is 0x0160 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST3_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST3_LEN_VAL_FIFO_03;	/* Register offset is 0x0164 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST3_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST3_LEN_VAL_FIFO_04;	/* Register offset is 0x0168 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST3_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST3_LEN_VAL_FIFO_05;	/* Register offset is 0x016c */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST3_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST3_LEN_VAL_FIFO_06;	/* Register offset is 0x0170 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST3_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST3_LEN_VAL_FIFO_07;	/* Register offset is 0x0174 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST3_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST3_LEN_VAL_FIFO_08;	/* Register offset is 0x0178 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST3_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST3_LEN_VAL_FIFO_09;	/* Register offset is 0x017c */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST3_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST3_LEN_VAL_FIFO_010;	/* Register offset is 0x0180 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST3_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST3_LEN_VAL_FIFO_011;	/* Register offset is 0x0184 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST3_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST3_LEN_VAL_FIFO_012;	/* Register offset is 0x0188 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST3_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST3_LEN_VAL_FIFO_013;	/* Register offset is 0x018c */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST3_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST3_LEN_VAL_FIFO_014;	/* Register offset is 0x0190 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST3_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST3_LEN_VAL_FIFO_015;	/* Register offset is 0x0194 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t POINTER: 32;				/* 1 */
	} B;
	} ST4_SRAM_PTR;	/* Register offset is 0x198 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST4_SRAM_LEN_VAL;	/* Register offset is 0x19C */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST4_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST4_PTR_FIFO_00;	/* Register offset is 0x01a0 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST4_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST4_PTR_FIFO_01;	/* Register offset is 0x01a4 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST4_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST4_PTR_FIFO_02;	/* Register offset is 0x01a8 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST4_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST4_PTR_FIFO_03;	/* Register offset is 0x01ac */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST4_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST4_PTR_FIFO_04;	/* Register offset is 0x01b0 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST4_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST4_PTR_FIFO_05;	/* Register offset is 0x01b4 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST4_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST4_PTR_FIFO_06;	/* Register offset is 0x01b8 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST4_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST4_PTR_FIFO_07;	/* Register offset is 0x01bc */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST4_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST4_PTR_FIFO_08;	/* Register offset is 0x01c0 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST4_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST4_PTR_FIFO_09;	/* Register offset is 0x01c4 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST4_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST4_PTR_FIFO_010;	/* Register offset is 0x01c8 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST4_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST4_PTR_FIFO_011;	/* Register offset is 0x01cc */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST4_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST4_PTR_FIFO_012;	/* Register offset is 0x01d0 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST4_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST4_PTR_FIFO_013;	/* Register offset is 0x01d4 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST4_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST4_PTR_FIFO_014;	/* Register offset is 0x01d8 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ST4_PTR_FIFO_0_POINTER: 32;				/* 1 */
	} B;
	} ST4_PTR_FIFO_015;	/* Register offset is 0x01dc */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST4_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST4_LEN_VAL_FIFO_00;	/* Register offset is 0x01e0 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST4_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST4_LEN_VAL_FIFO_01;	/* Register offset is 0x01e4 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST4_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST4_LEN_VAL_FIFO_02;	/* Register offset is 0x01e8 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST4_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST4_LEN_VAL_FIFO_03;	/* Register offset is 0x01ec */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST4_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST4_LEN_VAL_FIFO_04;	/* Register offset is 0x01f0 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST4_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST4_LEN_VAL_FIFO_05;	/* Register offset is 0x01f4 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST4_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST4_LEN_VAL_FIFO_06;	/* Register offset is 0x01f8 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST4_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST4_LEN_VAL_FIFO_07;	/* Register offset is 0x01fc */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST4_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST4_LEN_VAL_FIFO_08;	/* Register offset is 0x0200 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST4_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST4_LEN_VAL_FIFO_09;	/* Register offset is 0x0204 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST4_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST4_LEN_VAL_FIFO_010;	/* Register offset is 0x0208 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST4_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST4_LEN_VAL_FIFO_011;	/* Register offset is 0x020c */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST4_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST4_LEN_VAL_FIFO_012;	/* Register offset is 0x0210 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST4_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST4_LEN_VAL_FIFO_013;	/* Register offset is 0x0214 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST4_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST4_LEN_VAL_FIFO_014;	/* Register offset is 0x0218 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t ST4_LEN_VAL_FIFO_0_PKT_LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} ST4_LEN_VAL_FIFO_015;	/* Register offset is 0x021c */

	uint8_t JPEGDecoder_reserved_0[16] ; 
	union {
		vuint32_t R;
		struct {
 
  		vuint32_t C1_ADDR: 32;				/* 1 */
	} B;
	} BUF_C1_ADDR;	/* Register offset is 0x230 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t C2_ADDR: 32;				/* 1 */
	} B;
	} BUF_C2_ADDR;	/* Register offset is 0x234 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t C3_ADDR: 32;				/* 1 */
	} B;
	} BUF_C3_ADDR;	/* Register offset is 0x238 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t C4_ADDR: 32;				/* 1 */
	} B;
	} BUF_C4_ADDR;	/* Register offset is 0x23C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t NRLINES_B2L_C1: 16;				/* 1 */
  		vuint32_t NRLINES_B2L_C23: 16;				/* 1 */
	} B;
	} NRLINES_B2L;	/* Register offset is 0x240 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t NRLINES_B2L_C4: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} NRLINES_B2L_C4;	/* Register offset is 0x244 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t TIMEOUT_VAL: 32;				/* 1 */
	} B;
	} TIMEOUT;	/* Register offset is 0x248 */

	uint8_t JPEGDecoder_reserved_1[4] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t SW_RST: 1;				/* 1 */
 		vuint32_t BNK_STRD: 3;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t N_STREAM: 3;				/* 1 */
  		vuint32_t: 21;
	} B;
	} W_CTRL;	/* Register offset is 0x250 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t WRAPPER_CONF_ST1: 1;				/* 1 */
 		vuint32_t NF_JPEG_ST1: 3;				/* 1 */
 		vuint32_t P_JPEG_ST1: 1;				/* 1 */
 		vuint32_t WRAPPER_CONF_ST2: 1;				/* 1 */
 		vuint32_t NF_JPEG_ST2: 3;				/* 1 */
 		vuint32_t P_JPEG_ST2: 1;				/* 1 */
 		vuint32_t WRAPPER_CONF_ST3: 1;				/* 1 */
 		vuint32_t NF_JPEG_ST3: 3;				/* 1 */
 		vuint32_t P_JPEG_ST3: 1;				/* 1 */
 		vuint32_t WRAPPER_CONF_ST4: 1;				/* 1 */
 		vuint32_t NF_JPEG_ST4: 3;				/* 1 */
 		vuint32_t P_JPEG_ST4: 1;				/* 1 */
 		vuint32_t: 9;
 		vuint32_t SMPL_O_BF: 1;				/* 1 */
 		vuint32_t COM_MODE: 1;				/* 1 */
  		vuint32_t: 1;
	} B;
	} W_CTRL2;	/* Register offset is 0x254 */

	uint8_t JPEGDecoder_reserved_2[4] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t PKT1__FIFO_WATERMARK: 1;				/* 1 */
 		vuint32_t: 5;
 		vuint32_t PKT2_FIFO_WATERMARK: 1;				/* 1 */
 		vuint32_t: 5;
 		vuint32_t PKT3_FIFO_WATERMARK: 1;				/* 1 */
 		vuint32_t: 5;
 		vuint32_t PKT4_FIFO_WATERMARK: 1;				/* 1 */
 		vuint32_t: 5;
 		vuint32_t MCU_ROW_DN_EN: 1;				/* 1 */
 		vuint32_t END_OF_FRME_DN_EN: 1;				/* 1 */
 		vuint32_t STRT_OF_FRME_DN_EN: 1;				/* 1 */
 		vuint32_t WTR_MRK_LVL: 4;				/* 1 */
  		vuint32_t CONFIG_ERR_EN: 1;				/* 1 */
	} B;
	} INTR_EN;	/* Register offset is 0x25C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t PKT1__FIFO_WATERMARK: 1;				/* 1 */
 		vuint32_t PKT1_FIFO_SPACE: 5;				/* 1 */
 		vuint32_t PKT2_FIFO_WATERMARK: 1;				/* 1 */
 		vuint32_t PKT2_FIFO_SPACE: 5;				/* 1 */
 		vuint32_t PKT3_FIFO_WATERMARK: 1;				/* 1 */
 		vuint32_t PKT3_FIFO_SPACE: 5;				/* 1 */
 		vuint32_t PKT4_FIFO_WATERMARK: 1;				/* 1 */
 		vuint32_t PKT4_FIFO_SPACE: 5;				/* 1 */
 		vuint32_t MCU_ROW_DN: 1;				/* 1 */
 		vuint32_t END_OF_FRME: 1;				/* 1 */
 		vuint32_t STRT_OF_FRME: 1;				/* 1 */
 		vuint32_t: 2;
  		vuint32_t ST_WR: 3;				/* 1 */
	} B;
	} W_STATUS;	/* Register offset is 0x260 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t CR_ERR: 1;				/* 1 */
 		vuint32_t ST1_P_ERR: 1;				/* 1 */
 		vuint32_t ST2_P_ERR: 1;				/* 1 */
 		vuint32_t ST3_P_ERR: 1;				/* 1 */
 		vuint32_t ST4_P_ERR: 1;				/* 1 */
 		vuint32_t DRI_ERR: 1;				/* 1 */
 		vuint32_t TIMEOUT_ERR: 1;				/* 1 */
 		vuint32_t: 24;
  		vuint32_t CONFIG_ERR: 1;				/* 1 */
	} B;
	} E_STATUS;	/* Register offset is 0x264 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t INTVAL: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} RST_INTVL;	/* Register offset is 0x268 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t X_JPEG_ST1: 16;				/* 1 */
  		vuint32_t Y_JPEG_ST1: 16;				/* 1 */
	} B;
	} IMG_SZ_ST1;	/* Register offset is 0x26C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t X_JPEG_ST2: 16;				/* 1 */
  		vuint32_t Y_JPEG_ST2: 16;				/* 1 */
	} B;
	} IMG_SZ_ST2;	/* Register offset is 0x270 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t X_JPEG_ST3: 16;				/* 1 */
  		vuint32_t Y_JPEG_ST3: 16;				/* 1 */
	} B;
	} IMG_SZ_ST3;	/* Register offset is 0x274 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t X_JPEG_ST4: 16;				/* 1 */
  		vuint32_t Y_JPEG_ST4: 16;				/* 1 */
	} B;
	} IMG_SZ_ST4;	/* Register offset is 0x278 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t H1_JPEG_ST1: 4;				/* 1 */
 		vuint32_t H2_JPEG_ST1: 4;				/* 1 */
 		vuint32_t H3_JPEG_ST1: 4;				/* 1 */
 		vuint32_t H4_JPEG_ST1: 4;				/* 1 */
 		vuint32_t V1_JPEG_ST1: 4;				/* 1 */
 		vuint32_t V2_JPEG_ST1: 4;				/* 1 */
 		vuint32_t V3_JPEG_ST1: 4;				/* 1 */
  		vuint32_t V4_JPEG_ST1: 4;				/* 1 */
	} B;
	} SMPL_FCTR_ST1;	/* Register offset is 0x27C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t H1_JPEG_ST2: 4;				/* 1 */
 		vuint32_t H2_JPEG_ST2: 4;				/* 1 */
 		vuint32_t H3_JPEG_ST2: 4;				/* 1 */
 		vuint32_t H4_JPEG_ST2: 4;				/* 1 */
 		vuint32_t V1_JPEG_ST2: 4;				/* 1 */
 		vuint32_t V2_JPEG_ST2: 4;				/* 1 */
 		vuint32_t V3_JPEG_ST2: 4;				/* 1 */
  		vuint32_t V4_JPEG_ST2: 4;				/* 1 */
	} B;
	} SMPL_FCTR_ST2;	/* Register offset is 0x280 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t H1_JPEG_ST3: 4;				/* 1 */
 		vuint32_t H2_JPEG_ST3: 4;				/* 1 */
 		vuint32_t H3_JPEG_ST3: 4;				/* 1 */
 		vuint32_t H4_JPEG_ST3: 4;				/* 1 */
 		vuint32_t V1_JPEG_ST3: 4;				/* 1 */
 		vuint32_t V2_JPEG_ST3: 4;				/* 1 */
 		vuint32_t V3_JPEG_ST3: 4;				/* 1 */
  		vuint32_t V4_JPEG_ST3: 4;				/* 1 */
	} B;
	} SMPL_FCTR_ST3;	/* Register offset is 0x284 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t H1_JPEG_ST4: 4;				/* 1 */
 		vuint32_t H2_JPEG_ST4: 4;				/* 1 */
 		vuint32_t H3_JPEG_ST4: 4;				/* 1 */
 		vuint32_t H4_JPEG_ST4: 4;				/* 1 */
 		vuint32_t V1_JPEG_ST4: 4;				/* 1 */
 		vuint32_t V2_JPEG_ST4: 4;				/* 1 */
 		vuint32_t V3_JPEG_ST4: 4;				/* 1 */
  		vuint32_t V4_JPEG_ST4: 4;				/* 1 */
	} B;
	} SMPL_FCTR_ST4;	/* Register offset is 0x288 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t TST_BLK_PXL1: 18;				/* 1 */
 		vuint32_t: 8;
 		vuint32_t TST_LINE: 3;				/* 1 */
 		vuint32_t: 2;
  		vuint32_t TST_LINE_EN: 1;				/* 1 */
	} B;
	} TST_PXL_LOC;	/* Register offset is 0x28C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t TST_BLK_PXL2: 18;				/* 1 */
  		vuint32_t: 14;
	} B;
	} TST_PXL_LOC1;	/* Register offset is 0x290 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LUMA1_VAL: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t LUMA2_VAL: 12;				/* 1 */
  		vuint32_t: 4;
	} B;
	} TST_LINE_LUMA_ST1;	/* Register offset is 0x294 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t Cb1_VAL: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t Cb2_VAL: 12;				/* 1 */
  		vuint32_t: 4;
	} B;
	} TST_LINE_Cb_ST1;	/* Register offset is 0x298 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t Cr1_VAL: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t Cr2_VAL: 12;				/* 1 */
  		vuint32_t: 4;
	} B;
	} TST_LINE_Cr_ST1;	/* Register offset is 0x29C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LUMA1_VAL: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t LUMA2_VAL: 12;				/* 1 */
  		vuint32_t: 4;
	} B;
	} TST_LINE_LUMA_ST2;	/* Register offset is 0x2A0 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t Cb1_VAL: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t Cb2_VAL: 12;				/* 1 */
  		vuint32_t: 4;
	} B;
	} TST_LINE_Cb_ST2;	/* Register offset is 0x2A4 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t Cr1_VAL: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t Cr2_VAL: 12;				/* 1 */
  		vuint32_t: 4;
	} B;
	} TST_LINE_Cr_ST2;	/* Register offset is 0x2A8 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LUMA1_VAL: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t LUMA2_VAL: 12;				/* 1 */
  		vuint32_t: 4;
	} B;
	} TST_LINE_LUMA_ST3;	/* Register offset is 0x2AC */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t Cb1_VAL: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t Cb2_VAL: 12;				/* 1 */
  		vuint32_t: 4;
	} B;
	} TST_LINE_Cb_ST3;	/* Register offset is 0x2B0 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t Cr1_VAL: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t Cr2_VAL: 12;				/* 1 */
  		vuint32_t: 4;
 
	} B;
	} TST_LINE_Cr_ST3;	/* Register offset is 0x2B4 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t LUMA1_VAL: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t LUMA2_VAL: 12;				/* 1 */
  		vuint32_t: 4;
 
	} B;
	} TST_LINE_LUMA_ST4;	/* Register offset is 0x2B8 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t Cb1_VAL: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t Cb2_VAL: 12;				/* 1 */
  		vuint32_t: 4;
 
	} B;
	} TST_LINE_Cb_ST4;	/* Register offset is 0x2BC */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t Cr1_VAL: 12;				/* 1 */
 		vuint32_t: 4;
 		vuint32_t Cr2_VAL: 12;				/* 1 */
  		vuint32_t: 4;
	} B;
	} TST_LINE_Cr_ST4;	/* Register offset is 0x2C0 */

	uint8_t JPEGDecoder_reserved_3[28] ; 
	union {
		vuint32_t R;
		struct {
 		vuint32_t LP: 1;				/* 1 */
 		vuint32_t SWR: 1;				/* 1 */
 		vuint32_t GO: 1;				/* 1 */
  		vuint32_t: 29;
 
	} B;
	} JPEG_CTRL;	/* Register offset is 0x2E0 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t IMAGE_WIDTH: 16;				/* 1 */
  		vuint32_t: 16;
 
	} B;
	} JPEG_STATUS1;	/* Register offset is 0x2E4 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t IMAGE_HEIGHT: 16;				/* 1 */
  		vuint32_t: 16;
 
	} B;
	} JPEG_STATUS2;	/* Register offset is 0x2E8 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t HMCU: 16;				/* 1 */
  		vuint32_t: 16;
 
	} B;
	} JPEG_STATUS3;	/* Register offset is 0x2EC */

	union {
		vuint32_t R;
		struct {
 		vuint32_t VMCU: 16;				/* 1 */
  		vuint32_t: 16;
 
	} B;
	} JPEG_STATUS4;	/* Register offset is 0x2F0 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t Tq0: 2;				/* 1 */
 		vuint32_t V0: 3;				/* 1 */
 		vuint32_t H0: 3;				/* 1 */
 		vuint32_t C0: 8;				/* 1 */
  		vuint32_t: 16;
 
	} B;
	} JPEG_STATUS5;	/* Register offset is 0x2F4 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t TQ1: 2;				/* 1 */
 		vuint32_t V1: 3;				/* 1 */
 		vuint32_t H1: 3;				/* 1 */
 		vuint32_t C1: 8;				/* 1 */
  		vuint32_t: 16;
 
	} B;
	} JPEG_STATUS6;	/* Register offset is 0x2F8 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t Tq2: 2;				/* 1 */
 		vuint32_t V2: 3;				/* 1 */
 		vuint32_t H2: 3;				/* 1 */
 		vuint32_t C2: 8;				/* 1 */
  		vuint32_t: 16;
 
	} B;
	} JPEG_STATUS7;	/* Register offset is 0x2FC */

	union {
		vuint32_t R;
		struct {
 		vuint32_t TQ3: 2;				/* 1 */
 		vuint32_t V3: 3;				/* 1 */
 		vuint32_t H3: 3;				/* 1 */
 		vuint32_t C3: 8;				/* 1 */
  		vuint32_t: 16;
 
	} B;
	} JPEG_STATUS8;	/* Register offset is 0x300 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t NF: 8;				/* 1 */
 		vuint32_t SMPL_PRESN: 8;				/* 1 */
  		vuint32_t: 16;
 
	} B;
	} JPEG_STATUS9;	/* Register offset is 0x304 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t DRI: 16;				/* 1 */
  		vuint32_t: 16;
 
	} B;
	} JPEG_STATUS10;	/* Register offset is 0x308 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t Ns: 4;				/* 1 */
 		vuint32_t NBMCU: 4;				/* 1 */
 		vuint32_t Vmax: 4;				/* 1 */
 		vuint32_t Hmax: 4;				/* 1 */
  		vuint32_t: 16;
 
	} B;
	} JPEG_STATUS11;	/* Register offset is 0x30C */

	union {
		vuint32_t R;
		struct {
 		vuint32_t VHS0: 4;				/* 1 */
 		vuint32_t VHS1: 4;				/* 1 */
 		vuint32_t VHS2: 4;				/* 1 */
 		vuint32_t VHS3: 4;				/* 1 */
  		vuint32_t: 16;
 
	} B;
	} JPEG_STATUS12;	/* Register offset is 0x310 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t COM_E: 1;				/* 1 */
 		vuint32_t APPn_E: 1;				/* 1 */
 		vuint32_t DRI_E: 1;				/* 1 */
 		vuint32_t DNL_E: 1;				/* 1 */
 		vuint32_t DHT_E: 1;				/* 1 */
 		vuint32_t DQT_E: 1;				/* 1 */
 		vuint32_t SOS_E: 1;				/* 1 */
 		vuint32_t SOF_E: 1;				/* 1 */
  		vuint32_t: 24;
	} B;
	} JPEG_STATUS13;	/* Register offset is 0x314 */
}; 
 

/* ============================================================================
   ============================= Module: ISPScalar =============================
   ============================================================================ */
struct ispscalar_tag {
	union {
		vuint32_t R;
		struct {
 		vuint32_t NSA: 8;				/* 1 */
 		vuint32_t: 8;
 		vuint32_t CSA: 8;				/* 1 */
  		vuint32_t: 8;
 
	} B;
	} HOST_START;	/* Register offset is 0x0 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t N_EVENTS: 2;				/* 1 */
 		vuint32_t: 6;
 		vuint32_t CORESTATUS: 2;				/* 1 */
 		vuint32_t DEBUGSTATUS: 1;				/* 1 */
 		vuint32_t: 5;
  		vuint32_t REMAIN_PIXELS: 16;				/* 1 */
 
	} B;
	} HOST_STATUS;	/* Register offset is 0x4 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t YPOS: 16;				/* 1 */
  		vuint32_t XPOS: 16;				/* 1 */
 
	} B;
	} HOST_POS;	/* Register offset is 0x8 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t CURR_YPOS: 16;				/* 1 */
  		vuint32_t CURR_XPOS: 16;				/* 1 */
	} B;
	} HOST_CURRPOS;	/* Register offset is 0xC */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t XSTEP: 2;				/* 1 */
 		vuint32_t: 14;
  		vuint32_t XSIZE: 16;				/* 1 */
	} B;
	} HOST_XCFG;	/* Register offset is 0x10 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t CURR_XSTEP: 2;				/* 1 */
 		vuint32_t: 14;
  		vuint32_t CURR_XSIZE: 16;				/* 1 */
	} B;
	} HOST_CURRXCFG;	/* Register offset is 0x14 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t SHIFT: 2;				/* 1 */
 		vuint32_t NHOOD: 1;				/* 1 */
 		vuint32_t: 5;
 		vuint32_t EN_LINE0: 1;				/* 1 */
 		vuint32_t EN_LINE1: 1;				/* 1 */
 		vuint32_t EN_LINE2: 1;				/* 1 */
 		vuint32_t: 5;
 		vuint32_t CURR_SHIFT: 2;				/* 1 */
 		vuint32_t CURR_NHOOD: 1;				/* 1 */
 		vuint32_t: 5;
 		vuint32_t CURR_EN_LINE0: 1;				/* 1 */
 		vuint32_t CURR_EN_LINE1: 1;				/* 1 */
 		vuint32_t CURR_EN_LINE2: 1;				/* 1 */
  		vuint32_t: 5;
	} B;
	} HOST_INACFG;	/* Register offset is 0x18 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t SHIFT: 2;				/* 1 */
 		vuint32_t NHOOD: 1;				/* 1 */
 		vuint32_t: 5;
 		vuint32_t EN_LINE0: 1;				/* 1 */
 		vuint32_t EN_LINE1: 1;				/* 1 */
 		vuint32_t EN_LINE2: 1;				/* 1 */
 		vuint32_t: 5;
 		vuint32_t CURR_SHIFT: 2;				/* 1 */
 		vuint32_t CURR_NHOOD: 1;				/* 1 */
 		vuint32_t: 5;
 		vuint32_t CURR_EN_LINE0: 1;				/* 1 */
 		vuint32_t CURR_EN_LINE1: 1;				/* 1 */
 		vuint32_t CURR_EN_LINE2: 1;				/* 1 */
  		vuint32_t: 5;
	} B;
	} HOST_INBCFG;	/* Register offset is 0x1C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t SHIFT: 2;				/* 1 */
 		vuint32_t NHOOD: 1;				/* 1 */
 		vuint32_t: 5;
 		vuint32_t EN_LINE0: 1;				/* 1 */
 		vuint32_t EN_LINE1: 1;				/* 1 */
 		vuint32_t EN_LINE2: 1;				/* 1 */
 		vuint32_t: 5;
 		vuint32_t CURR_SHIFT: 2;				/* 1 */
 		vuint32_t CURR_NHOOD: 1;				/* 1 */
 		vuint32_t: 5;
 		vuint32_t CURR_EN_LINE0: 1;				/* 1 */
 		vuint32_t CURR_EN_LINE1: 1;				/* 1 */
 		vuint32_t CURR_EN_LINE2: 1;				/* 1 */
  		vuint32_t: 5;
	} B;
	} HOST_INALPHACFG;	/* Register offset is 0x20 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 8;
 		vuint32_t EN_OUT0: 1;				/* 1 */
 		vuint32_t EN_OUT1: 1;				/* 1 */
 		vuint32_t EN_OUT2: 1;				/* 1 */
 		vuint32_t EN_OUT3: 1;				/* 1 */
 		vuint32_t: 12;
 		vuint32_t CURR_EN_OUT0: 1;				/* 1 */
 		vuint32_t CURR_EN_OUT1: 1;				/* 1 */
 		vuint32_t CURR_EN_OUT2: 1;				/* 1 */
 		vuint32_t CURR_EN_OUT3: 1;				/* 1 */
  		vuint32_t: 4;
	} B;
	} HOST_OUTCFG;	/* Register offset is 0x24 */

	uint8_t ISPScalar_reserved_0[24] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t IADDR: 6;				/* 1 */
  		vuint32_t: 26;
	} B;
	} HOST_IMEMA;	/* Register offset is 0x40 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t DATA: 32;				/* 1 */
	} B;
	} HOST_IMEMD;	/* Register offset is 0x44 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t DMEMA: 8;				/* 1 */
  		vuint32_t: 24;
	} B;
	} HOST_DREGA;	/* Register offset is 0x48 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t DMEMD: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} HOST_DREGD;	/* Register offset is 0x4C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t HISTA: 8;				/* 1 */
  		vuint32_t: 24;
	} B;
	} HOST_HISTA;	/* Register offset is 0x50 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t HISTD: 32;				/* 1 */
	} B;
	} HOST_HISTD;	/* Register offset is 0x54 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t HISTPEAK: 32;				/* 1 */
	} B;
	} HOST_HISTPEAK;	/* Register offset is 0x58 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t HISTAMAX: 8;				/* 1 */
  		vuint32_t: 24;
	} B;
	} HOST_HISTAMAX;	/* Register offset is 0x5C */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t HISTCLEAR: 32;				/* 1 */
	} B;
	} HOST_HISTCLEAR;	/* Register offset is 0x60 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t STATA: 10;				/* 1 */
  		vuint32_t: 22;
	} B;
	} HOST_STATA;	/* Register offset is 0x64 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t STATD: 32;				/* 1 */
	} B;
	} HOST_STATD;	/* Register offset is 0x68 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t STATCLEAR: 32;				/* 1 */
	} B;
	} HOST_STATCLEAR;	/* Register offset is 0x6C */

	uint8_t ISPScalar_reserved_1[8] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LUTA: 12;				/* 1 */
  		vuint32_t: 20;
	} B;
	} HOST_LUTA;	/* Register offset is 0x78 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t LUTD: 32;				/* 1 */
	} B;
	} HOST_LUTD;	/* Register offset is 0x7C */

	uint8_t ISPScalar_reserved_2[132] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE0_LEN_INA;	/* Register offset is 0x104 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH0_CFG_INA;	/* Register offset is 0x108 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_LINE0_ADDR_INA;	/* Register offset is 0x10C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE1_LEN_INA;	/* Register offset is 0x110 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH1_CFG_INA;	/* Register offset is 0x114 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_LINE1_ADDR_INA;	/* Register offset is 0x118 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE2_LEN_INA;	/* Register offset is 0x11C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH2_CFG_INA;	/* Register offset is 0x120 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_LINE2_ADDR_INA;	/* Register offset is 0x124 */

	uint8_t ISPScalar_reserved_3[28] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE0_LEN_INB;	/* Register offset is 0x144 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH0_CFG_INB;	/* Register offset is 0x148 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_LINE0_ADDR_INB;	/* Register offset is 0x14C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE1_LEN_INB;	/* Register offset is 0x150 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH1_CFG_INB;	/* Register offset is 0x154 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_LINE1_ADDR_INB;	/* Register offset is 0x158 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE2_LEN_INB;	/* Register offset is 0x15C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH2_CFG_INB;	/* Register offset is 0x160 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_LINE2_ADDR_INB;	/* Register offset is 0x164 */

	uint8_t ISPScalar_reserved_4[28] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE0_LEN_INALPHA;	/* Register offset is 0x184 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH0_CFG_INALPHA;	/* Register offset is 0x188 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_LINE0_ADDR_INALPHA;	/* Register offset is 0x18C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE1_LEN_INALPHA;	/* Register offset is 0x190 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH1_CFG_INALPHA;	/* Register offset is 0x194 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_LINE1_ADDR_INALPHA;	/* Register offset is 0x198 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE2_LEN_INALPHA;	/* Register offset is 0x19C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH2_CFG_INALPHA;	/* Register offset is 0x1A0 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_LINE2_ADDR_INALPHA;	/* Register offset is 0x1A4 */

	uint8_t ISPScalar_reserved_5[28] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE0_LEN_OUT;	/* Register offset is 0x1C4 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 16;
 		vuint32_t SKIP: 2;				/* 1 */
 		vuint32_t: 10;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH0_CFG_OUT;	/* Register offset is 0x1C8 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 3;
  		vuint32_t ADDR: 29;				/* 1 */
	} B;
	} S_LINE0_ADDR_OUT;	/* Register offset is 0x1CC */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE1_LEN_OUT;	/* Register offset is 0x1D0 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 16;
 		vuint32_t SKIP: 2;				/* 1 */
 		vuint32_t: 10;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH1_CFG_OUT;	/* Register offset is 0x1D4 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 3;
  		vuint32_t ADDR: 29;				/* 1 */
	} B;
	} S_LINE1_ADDR_OUT;	/* Register offset is 0x1D8 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE2_LEN_OUT;	/* Register offset is 0x1DC */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 16;
 		vuint32_t SKIP: 2;				/* 1 */
 		vuint32_t: 10;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH2_CFG_OUT;	/* Register offset is 0x1E0 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 3;
  		vuint32_t ADDR: 29;				/* 1 */
	} B;
	} S_LINE2_ADDR_OUT;	/* Register offset is 0x1E4 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE3_LEN_OUT;	/* Register offset is 0x1E8 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 16;
 		vuint32_t SKIP: 2;				/* 1 */
 		vuint32_t: 10;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH3_CFG_OUT;	/* Register offset is 0x1EC */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 3;
  		vuint32_t ADDR: 29;				/* 1 */
	} B;
	} S_LINE3_ADDR_OUT;	/* Register offset is 0x1F0 */

	uint8_t ISPScalar_reserved_6[16] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE0_LEN_INA;	/* Register offset is 0x204 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CURR_CH0_CFG_INA;	/* Register offset is 0x208 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_CURR_LINE0_ADDR_INA;	/* Register offset is 0x20C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE1_LEN_INA;	/* Register offset is 0x210 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t Reverse: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CURR_CH1_CFG_INA;	/* Register offset is 0x214 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_CURR_LINE1_ADDR_INA;	/* Register offset is 0x218 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE2_LEN_INA;	/* Register offset is 0x21C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CURR_CH2_CFG_INA;	/* Register offset is 0x220 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_CURR_LINE2_ADDR_INA;	/* Register offset is 0x224 */

	uint8_t ISPScalar_reserved_7[28] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE0_LEN_INB;	/* Register offset is 0x244 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CURR_CH0_CFG_INB;	/* Register offset is 0x248 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_CURR_LINE0_ADDR_INB;	/* Register offset is 0x24C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE1_LEN_INB;	/* Register offset is 0x250 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPEAT_MODE: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CURR_CH1_CFG_INB;	/* Register offset is 0x254 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_CURR_LINE1_ADDR_INB;	/* Register offset is 0x258 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE2_LEN_INB;	/* Register offset is 0x25C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CURR_CH2_CFG_INB;	/* Register offset is 0x260 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_CURR_LINE2_ADDR_INB;	/* Register offset is 0x264 */

	uint8_t ISPScalar_reserved_8[28] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE0_LEN_INALPHA;	/* Register offset is 0x284 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CURR_CH0_CFG_INALPHA;	/* Register offset is 0x288 */

	union {
		vuint32_t R;
		struct {
   
		vuint32_t S_CURR_LINE0_ADDR_INALPHA : 32; 
	} B;
	} S_CURR_LINE0_ADDR_INALPHA;	/* Register offset is 0x28C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE1_LEN_INALPHA;	/* Register offset is 0x290 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CURR_CH1_CFG_INALPHA;	/* Register offset is 0x294 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_CURR_LINE1_ADDR_INALPHA;	/* Register offset is 0x298 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE2_LEN_INALPHA;	/* Register offset is 0x29C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CURR_CH2_CFG_INALPHA;	/* Register offset is 0x2A0 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_CURR_LINE2_ADDR_INALPHA;	/* Register offset is 0x2A4 */

	uint8_t ISPScalar_reserved_9[28] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE0_LEN_OUT;	/* Register offset is 0x2C4 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 16;
 		vuint32_t SKIP: 2;				/* 1 */
 		vuint32_t: 10;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CURR_CH0_CFG_OUT;	/* Register offset is 0x2C8 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 3;
  		vuint32_t ADDR: 29;				/* 1 */
	} B;
	} S_CURR_LINE0_ADDR_OUT;	/* Register offset is 0x2CC */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE1_LEN_OUT;	/* Register offset is 0x2D0 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 16;
 		vuint32_t SKIP: 2;				/* 1 */
 		vuint32_t: 10;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CURR_CH1_CFG_OUT;	/* Register offset is 0x2D4 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 3;
  		vuint32_t ADDR: 29;				/* 1 */
	} B;
	} S_CURR_LINE1_ADDR_OUT;	/* Register offset is 0x2D8 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE2_LEN_OUT;	/* Register offset is 0x2DC */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 16;
 		vuint32_t SKIP: 2;				/* 1 */
 		vuint32_t: 10;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CURR_CH2_CFG_OUT;	/* Register offset is 0x2E0 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 3;
  		vuint32_t ADDR: 29;				/* 1 */
	} B;
	} S_CURR_LINE2_ADDR_OUT;	/* Register offset is 0x2E4 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE3_LEN_OUT;	/* Register offset is 0x2E8 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 16;
 		vuint32_t SKIP: 2;				/* 1 */
 		vuint32_t: 10;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CURR_CH3_CFG_OUT;	/* Register offset is 0x2EC */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 3;
  		vuint32_t ADDR: 29;				/* 1 */
	} B;
	} S_CURR_LINE3_ADDR_OUT;	/* Register offset is 0x2F0 */

	uint8_t ISPScalar_reserved_10[268] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t DBG_ACT: 1;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STEP: 4;				/* 1 */
 		vuint32_t: 5;
 		vuint32_t B_XPOS: 1;				/* 1 */
 		vuint32_t B_YPOS: 1;				/* 1 */
 		vuint32_t B_POS: 1;				/* 1 */
 		vuint32_t B_IADDR: 1;				/* 1 */
 		vuint32_t B_RADDR: 1;				/* 1 */
 		vuint32_t B_WADDR: 1;				/* 1 */
  		vuint32_t B_FINISH: 1;				/* 1 */
	} B;
	} DEBUG_MS;	/* Register offset is 0x400 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t H_DBG: 1;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t DBG: 1;				/* 1 */
 		vuint32_t XPOS: 1;				/* 1 */
 		vuint32_t YPOS: 1;				/* 1 */
 		vuint32_t POS: 1;				/* 1 */
 		vuint32_t IADDR: 1;				/* 1 */
 		vuint32_t RADDR: 1;				/* 1 */
 		vuint32_t WADDR: 1;				/* 1 */
 		vuint32_t FINISH: 1;				/* 1 */
 		vuint32_t DI: 1;				/* 1 */
 		vuint32_t STEP_REQ: 1;				/* 1 */
  		vuint32_t: 6;
	} B;
	} DEBUG_BKPT_STAT;	/* Register offset is 0x404 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t YPOS: 16;				/* 1 */
  		vuint32_t XPOS: 16;				/* 1 */
	} B;
	} DEBUG_BKPT_POS;	/* Register offset is 0x408 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 8;
 		vuint32_t DWADDR: 8;				/* 1 */
 		vuint32_t DRADDR: 8;				/* 1 */
  		vuint32_t IADDR: 8;				/* 1 */
	} B;
	} DEBUG_BKPT_ADDR;	/* Register offset is 0x40C */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t INSTR: 32;				/* 1 */
	} B;
	} DEBUG_DI;	/* Register offset is 0x410 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t PC: 8;				/* 1 */
 		vuint32_t: 16;
 		vuint32_t OV: 1;				/* 1 */
 		vuint32_t C: 1;				/* 1 */
 		vuint32_t Z: 1;				/* 1 */
 		vuint32_t N: 1;				/* 1 */
  		vuint32_t: 4;
	} B;
	} DEBUG_CORE_STAT;	/* Register offset is 0x414 */

	uint8_t ISPScalar_reserved_11[232] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t EN_S_SIN: 1;				/* 1 */
 		vuint32_t EN_S_SOUT: 1;				/* 1 */
 		vuint32_t EN_S_SINOUT: 1;				/* 1 */
 		vuint32_t EN_S_DH: 1;				/* 1 */
 		vuint32_t EN_EX_CNT: 1;				/* 1 */
 		vuint32_t: 3;
 		vuint32_t STOP_S_SIN: 1;				/* 1 */
 		vuint32_t STOP_S_SOUT: 1;				/* 1 */
 		vuint32_t STOP_S_SINOUT: 1;				/* 1 */
 		vuint32_t STOP_S_DH: 1;				/* 1 */
 		vuint32_t STOP_EX_CNT: 1;				/* 1 */
 		vuint32_t: 3;
 		vuint32_t INIT_S_SIN: 1;				/* 1 */
 		vuint32_t INIT_S_SOUT: 1;				/* 1 */
 		vuint32_t INIT_S_SINOUT: 1;				/* 1 */
 		vuint32_t INIT_S_DH: 1;				/* 1 */
 		vuint32_t INIT_EX_CNT: 1;				/* 1 */
  		vuint32_t: 11;
	} B;
	} PMONITOR_CTRL;	/* Register offset is 0x500 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t SAMPLE_S_SIN: 1;				/* 1 */
 		vuint32_t SAMPLE_S_SOUT: 1;				/* 1 */
 		vuint32_t SAMPLE_S_SINOUT: 1;				/* 1 */
 		vuint32_t SAMPLE_S_DH: 1;				/* 1 */
 		vuint32_t SAMPLE_EX_CNT: 1;				/* 1 */
  		vuint32_t: 27;
	} B;
	} PMONITOR_SAMPLE;	/* Register offset is 0x504 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t STALLS: 32;				/* 1 */
	} B;
	} STALL_STREAMIN;	/* Register offset is 0x508 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t STALLS: 32;				/* 1 */
	} B;
	} STALL_STREAMOUT;	/* Register offset is 0x50C */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t STALLS: 32;				/* 1 */
	} B;
	} STALL_STREAMINOUT;	/* Register offset is 0x510 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t STALLS: 32;				/* 1 */
	} B;
	} STALL_DATAHAZARD;	/* Register offset is 0x514 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t CYCLES: 32;				/* 1 */
	} B;
	} EXEC_COUNTER;	/* Register offset is 0x518 */

	uint8_t ISPScalar_reserved_12[228] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t SOFT_RESET: 1;				/* 1 */
 		vuint32_t HARD_RESET: 1;				/* 1 */
  		vuint32_t: 30;
	} B;
	} RESET_REG;	/* Register offset is 0x600 */

    uint8_t ISPScalar_reserved_13[0x9FC] ;
}; 
 
struct ISPScalar_tag {
    struct ispscalar_tag IPUS[IPUS_INSTANCE_COUNT];
};

/* ============================================================================
   ============================= Module: ISPVector =============================
   ============================================================================ */
struct ispvector_tag {
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t NSA: 8;				/* 1 */
 		vuint32_t: 8;
 		vuint32_t CSA: 8;				/* 1 */
  		vuint32_t: 8;
	} B;
	} HOST_START;	/* Register offset is 0x0 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t N_EVENTS: 2;				/* 1 */
 		vuint32_t: 6;
 		vuint32_t CORESTATUS: 2;				/* 1 */
 		vuint32_t DEBUGSTATUS: 1;				/* 1 */
 		vuint32_t: 5;
  		vuint32_t REMAINPIXELS: 16;				/* 1 */
	} B;
	} HOST_STATUS;	/* Register offset is 0x4 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t YPOS: 16;				/* 1 */
  		vuint32_t XPOS: 16;				/* 1 */
	} B;
	} HOST_POS;	/* Register offset is 0x8 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t CURR_YPOS: 16;				/* 1 */
  		vuint32_t CURR_XPOS: 16;				/* 1 */
	} B;
	} HOST_CURRPOS;	/* Register offset is 0xC */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t XSTEP: 2;				/* 1 */
 		vuint32_t: 14;
  		vuint32_t XSIZE: 16;				/* 1 */
	} B;
	} HOST_XCFG;	/* Register offset is 0x10 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t CURR_XSTEP: 2;				/* 1 */
 		vuint32_t: 14;
  		vuint32_t CURR_XSIZE: 16;				/* 1 */
	} B;
	} HOST_CURRXCFG;	/* Register offset is 0x14 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t IN_SHIFT: 2;				/* 1 */
 		vuint32_t NHOOD: 1;				/* 1 */
 		vuint32_t: 5;
 		vuint32_t EN_LINE0: 1;				/* 1 */
 		vuint32_t EN_LINE1: 1;				/* 1 */
 		vuint32_t EN_LINE2: 1;				/* 1 */
 		vuint32_t EN_LINE3: 1;				/* 1 */
 		vuint32_t EN_LINE4: 1;				/* 1 */
 		vuint32_t: 3;
 		vuint32_t CURR_IN_SHIFT: 2;				/* 1 */
 		vuint32_t CURR_NHOOD: 1;				/* 1 */
 		vuint32_t: 5;
 		vuint32_t CURR_EN_LINE0: 1;				/* 1 */
 		vuint32_t CURR_EN_LINE1: 1;				/* 1 */
 		vuint32_t CURR_EN_LINE2: 1;				/* 1 */
 		vuint32_t CURR_EN_LINE3: 1;				/* 1 */
 		vuint32_t CURR_EN_LINE4: 1;				/* 1 */
  		vuint32_t: 3;
	} B;
	} HOST_INCFG;	/* Register offset is 0x18 */

	uint8_t ISPVector_reserved_0[8] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t OUT_SHIFT: 2;				/* 1 */
 		vuint32_t: 6;
 		vuint32_t EN_OUT0: 1;				/* 1 */
 		vuint32_t EN_OUT1: 1;				/* 1 */
 		vuint32_t EN_OUT2: 1;				/* 1 */
 		vuint32_t: 5;
 		vuint32_t CURR_OUT_SHIFT: 2;				/* 1 */
 		vuint32_t: 6;
 		vuint32_t CURR_EN_OUT0: 1;				/* 1 */
 		vuint32_t CURR_EN_OUT1: 1;				/* 1 */
 		vuint32_t CURR_EN_OUT2: 1;				/* 1 */
  		vuint32_t: 5;
	} B;
	} HOST_OUTCFG;	/* Register offset is 0x24 */

	uint8_t ISPVector_reserved_1[24] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t IADDR: 6;				/* 1 */
  		vuint32_t: 26;
	} B;
	} HOST_IMEMA;	/* Register offset is 0x40 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t DATA: 32;				/* 1 */
	} B;
	} HOST_IMEMD;	/* Register offset is 0x44 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t DMEMA: 8;				/* 1 */
  		vuint32_t: 24;
	} B;
	} HOST_DREGA;	/* Register offset is 0x48 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t DMEMD: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} HOST_DREGD;	/* Register offset is 0x4C */

	uint8_t ISPVector_reserved_2[180] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE0_LEN_IN;	/* Register offset is 0x104 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH0_CFG_IN;	/* Register offset is 0x108 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_LINE0_ADDR_IN;	/* Register offset is 0x10C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE1_LEN_IN;	/* Register offset is 0x110 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH1_CFG_IN;	/* Register offset is 0x114 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_LINE1_ADDR_IN;	/* Register offset is 0x118 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE2_LEN_IN;	/* Register offset is 0x11C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH2_CFG_IN;	/* Register offset is 0x120 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_LINE2_ADDR_IN;	/* Register offset is 0x124 */

	uint8_t ISPVector_reserved_3[28] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE3_LEN_IN;	/* Register offset is 0x144 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH3_CFG_IN;	/* Register offset is 0x148 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_LINE3_ADDR_IN;	/* Register offset is 0x14C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE4_LEN_IN;	/* Register offset is 0x150 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH4_CFG_IN;	/* Register offset is 0x154 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_LINE4_ADDR_IN;	/* Register offset is 0x158 */

	uint8_t ISPVector_reserved_4[104] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE0_LEN_OUT;	/* Register offset is 0x1C4 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 16;
 		vuint32_t SKIP: 2;				/* 1 */
 		vuint32_t: 10;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH0_CFG_OUT;	/* Register offset is 0x1C8 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 3;
  		vuint32_t ADDR: 29;				/* 1 */
	} B;
	} S_LINE0_ADDR_OUT;	/* Register offset is 0x1CC */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE1_LEN_OUT;	/* Register offset is 0x1D0 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 16;
 		vuint32_t SKIP: 2;				/* 1 */
 		vuint32_t: 10;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH1_CFG_OUT;	/* Register offset is 0x1D4 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 3;
  		vuint32_t ADDR: 29;				/* 1 */
	} B;
	} S_LINE1_ADDR_OUT;	/* Register offset is 0x1D8 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_LINE2_LEN_OUT;	/* Register offset is 0x1DC */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 16;
 		vuint32_t SKIP: 2;				/* 1 */
 		vuint32_t: 10;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CH2_CFG_OUT;	/* Register offset is 0x1E0 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 3;
  		vuint32_t ADDR: 29;				/* 1 */
	} B;
	} S_LINE2_ADDR_OUT;	/* Register offset is 0x1E4 */

	uint8_t ISPVector_reserved_5[28] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE0_LEN_IN;	/* Register offset is 0x204 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 3;
	} B;
	} S_CURR_CH0_CFG_IN;	/* Register offset is 0x208 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_CURR_LINE0_ADDR_IN;	/* Register offset is 0x20C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE1_LEN_IN;	/* Register offset is 0x210 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 3;
	} B;
	} S_CURR_CH1_CFG_IN;	/* Register offset is 0x214 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_CURR_LINE1_ADDR_IN;	/* Register offset is 0x218 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE2_LEN_IN;	/* Register offset is 0x21C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 3;
	} B;
	} S_CURR_CH2_CFG_IN;	/* Register offset is 0x220 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_CURR_LINE2_ADDR_IN;	/* Register offset is 0x224 */

	uint8_t ISPVector_reserved_6[28] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE3_LEN_IN;	/* Register offset is 0x244 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 3;
	} B;
	} S_CURR_CH3_CFG_IN;	/* Register offset is 0x248 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_CURR_LINE3_ADDR_IN;	/* Register offset is 0x24C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE4_LEN_IN;	/* Register offset is 0x250 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t REPLACE_VAL: 16;				/* 1 */
 		vuint32_t STEP: 2;				/* 1 */
 		vuint32_t RPT: 2;				/* 1 */
 		vuint32_t REPLACE_MODE: 3;				/* 1 */
 		vuint32_t REVERSE: 1;				/* 1 */
 		vuint32_t REPLACE_WIDTH: 3;				/* 1 */
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 3;
	} B;
	} S_CURR_CH4_CFG_IN;	/* Register offset is 0x254 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t ADDR: 32;				/* 1 */
	} B;
	} S_CURR_LINE4_ADDR_IN;	/* Register offset is 0x258 */

	uint8_t ISPVector_reserved_7[104] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE0_LEN_OUT;	/* Register offset is 0x2C4 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 16;
 		vuint32_t SKIP: 2;				/* 1 */
 		vuint32_t: 10;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CURR_CH0_CFG_OUT;	/* Register offset is 0x2C8 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 3;
  		vuint32_t ADDR: 29;				/* 1 */
	} B;
	} S_CURR_LINE0_ADDR_OUT;	/* Register offset is 0x2CC */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE1_LEN_OUT;	/* Register offset is 0x2D0 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 16;
 		vuint32_t SKIP: 2;				/* 1 */
 		vuint32_t: 10;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CURR_CH1_CFG_OUT;	/* Register offset is 0x2D4 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 3;
  		vuint32_t ADDR: 29;				/* 1 */
	} B;
	} S_CURR_LINE1_ADDR_OUT;	/* Register offset is 0x2D8 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t LENGTH: 16;				/* 1 */
  		vuint32_t: 16;
	} B;
	} S_CURR_LINE2_LEN_OUT;	/* Register offset is 0x2DC */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 16;
 		vuint32_t SKIP: 2;				/* 1 */
 		vuint32_t: 10;
 		vuint32_t TYPE: 2;				/* 1 */
  		vuint32_t: 2;
	} B;
	} S_CURR_CH2_CFG_OUT;	/* Register offset is 0x2E0 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 3;
  		vuint32_t ADDR: 29;				/* 1 */
	} B;
	} S_CURR_LINE2_ADDR_OUT;	/* Register offset is 0x2E4 */

	uint8_t ISPVector_reserved_8[280] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t DBG_ACT: 1;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t STEP: 4;				/* 1 */
 		vuint32_t: 5;
 		vuint32_t B_XPOS: 1;				/* 1 */
 		vuint32_t B_YPOS: 1;				/* 1 */
 		vuint32_t B_POS: 1;				/* 1 */
 		vuint32_t B_IADDR: 1;				/* 1 */
 		vuint32_t B_RADDR: 1;				/* 1 */
 		vuint32_t B_WADDR: 1;				/* 1 */
  		vuint32_t B_FINISH: 1;				/* 1 */
	} B;
	} DEBUG_MS;	/* Register offset is 0x400 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t H_DBG: 1;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t DBG: 1;				/* 1 */
 		vuint32_t XPOS: 1;				/* 1 */
 		vuint32_t YPOS: 1;				/* 1 */
 		vuint32_t POS: 1;				/* 1 */
 		vuint32_t IADDR: 1;				/* 1 */
 		vuint32_t RADDR: 1;				/* 1 */
 		vuint32_t WADDR: 1;				/* 1 */
 		vuint32_t FINISH: 1;				/* 1 */
 		vuint32_t DI: 1;				/* 1 */
  		vuint32_t: 7;
	} B;
	} DEBUG_BKPT_STAT;	/* Register offset is 0x404 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t YPOS: 16;				/* 1 */
  		vuint32_t XPOS: 16;				/* 1 */
	} B;
	} DEBUG_BKPT_POS;	/* Register offset is 0x408 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t: 8;
 		vuint32_t DWADDR: 8;				/* 1 */
 		vuint32_t DRADDR: 8;				/* 1 */
  		vuint32_t IADDR: 8;				/* 1 */
	} B;
	} DEBUG_BKPT_ADDR;	/* Register offset is 0x40C */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t INSTR: 32;				/* 1 */
	} B;
	} DEBUG_DI;	/* Register offset is 0x410 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t PC: 8;				/* 1 */
 		vuint32_t: 16;
 		vuint32_t OV: 1;				/* 1 */
 		vuint32_t C: 1;				/* 1 */
 		vuint32_t Z: 1;				/* 1 */
 		vuint32_t N: 1;				/* 1 */
  		vuint32_t: 4;
	} B;
	} DEBUG_CORE_STAT;	/* Register offset is 0x414 */

	uint8_t ISPVector_reserved_9[232] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t EN_S_SIN: 1;				/* 1 */
 		vuint32_t EN_S_SOUT: 1;				/* 1 */
 		vuint32_t EN_S_SINOUT: 1;				/* 1 */
 		vuint32_t EN_S_DH: 1;				/* 1 */
 		vuint32_t EN_EX_CNT: 1;				/* 1 */
 		vuint32_t: 3;
 		vuint32_t STOP_S_SIN: 1;				/* 1 */
 		vuint32_t STOP_S_SOUT: 1;				/* 1 */
 		vuint32_t STOP_S_SINOUT: 1;				/* 1 */
 		vuint32_t STOP_S_DH: 1;				/* 1 */
 		vuint32_t STOP_EX_CNT: 1;				/* 1 */
 		vuint32_t: 3;
 		vuint32_t INIT_S_SIN: 1;				/* 1 */
 		vuint32_t INIT_S_SOUT: 1;				/* 1 */
 		vuint32_t INIT_S_SINOUT: 1;				/* 1 */
 		vuint32_t INIT_S_DH: 1;				/* 1 */
 		vuint32_t INIT_EX_CNT: 1;				/* 1 */
  		vuint32_t: 11;
	} B;
	} PMONITOR_CTRL;	/* Register offset is 0x500 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t SAMPLE_S_SIN: 1;				/* 1 */
 		vuint32_t SAMPLE_S_SOUT: 1;				/* 1 */
 		vuint32_t SAMPLE_S_SINOUT: 1;				/* 1 */
 		vuint32_t SAMPLE_S_DH: 1;				/* 1 */
 		vuint32_t SAMPLE_EX_CNT: 1;				/* 1 */
  		vuint32_t: 27;
	} B;
	} PMONITOR_SAMPLE;	/* Register offset is 0x504 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t STALLS: 32;				/* 1 */
	} B;
	} STALL_STREAMIN;	/* Register offset is 0x508 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t STALLS: 32;				/* 1 */
	} B;
	} STALL_STREAMOUT;	/* Register offset is 0x50C */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t STALLS: 32;				/* 1 */
	} B;
	} STALL_STREAMINOUT;	/* Register offset is 0x510 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t STALLS: 32;				/* 1 */
	} B;
	} STALL_DATAHAZARD;	/* Register offset is 0x514 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t CYCLES: 32;				/* 1 */
	} B;
	} EXEC_COUNTER;	/* Register offset is 0x518 */

	uint8_t ISPVector_reserved_10[228] ; 
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t SOFT_RESET: 1;				/* 1 */
 		vuint32_t HARD_RESET: 1;				/* 1 */
  		vuint32_t: 30;
	} B;
	} RESET_REG;	/* Register offset is 0x600 */
    
    uint8_t ISPVector_reserved_11[0x9FC] ; 
}; 
 
struct ISPVector_tag {
    struct ispvector_tag IPUV[IPUV_INSTANCE_COUNT];
};

/* ============================================================================
   ============================= Module: VisionSeq_CTRL_BLK =============================
   ============================================================================ */
struct VisionSeq_CTRL_BLK_tag {
	union {
		vuint32_t R;
		struct {
 
 		vuint32_t SEQ_INT_RESET: 1;				/* 1 */
 		vuint32_t CM0p_RESET: 1;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t CM0p_CLK_EN: 1;				/* 1 */
  		vuint32_t: 14;
	} B;
	} CLKRST;	/* Register offset is 0x000 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t OUTBOUND_MSG_RDY: 1;				/* 1 */
 		vuint32_t OUTBOUND_MSG_ACKED: 1;				/* 1 */
 		vuint32_t: 14;
 		vuint32_t INBOUND_MSG_RDY: 1;				/* 1 */
 		vuint32_t INBOUND_MSG_ACKED: 1;				/* 1 */
  		vuint32_t: 14;
	} B;
	} MSGCTRL;	/* Register offset is 0x004 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t OUTBOUND_MSG_ADDR: 32;				/* 1 */
 
	} B;
	} OUTMSG_ADDR;	/* Register offset is 0x008 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t OUTBOUND_MSG_LEN: 12;				/* 1 */
  		vuint32_t: 20;
 
	} B;
	} OUTMSG_LEN;	/* Register offset is 0x00C */

	union {
		vuint32_t R;
		struct {
  		vuint32_t INBOUND_MSG_ADDR: 32;				/* 1 */
 
	} B;
	} INMSG_ADDR;	/* Register offset is 0x010 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t INBOUND_MSG_LEN: 12;				/* 1 */
  		vuint32_t: 20;
 
	} B;
	} INMSG_LEN;	/* Register offset is 0x014 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t OUTBOUND_MSG_CFG_ERR: 1;				/* 1 */
 		vuint32_t INBOUND_MSG_CFG_ERR: 1;				/* 1 */
 		vuint32_t: 14;
 		vuint32_t SEQ_MEM_PERR: 1;				/* 1 */
  		vuint32_t: 15;
 
	} B;
	} ERRSTAT;	/* Register offset is 0x018 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t SEQ_INT_ERR_IRQ_EN: 1;				/* 1 */
 		vuint32_t IDMA_DONE_IRQ_EN: 1;				/* 1 */
 		vuint32_t INMSG_RDY_IRQ_EN: 1;				/* 1 */
 		vuint32_t OUTMSG_ACK_IRQ_EN: 1;				/* 1 */
 		vuint32_t IPUS_EVT_DONE_IRQ_EN: 1;				/* 1 */
 		vuint32_t IPUV_EVT_DONE_IRQ_EN: 1;				/* 1 */
 		vuint32_t OTHR_EVT_DONE_IRQ_EN: 1;				/* 1 */
 		vuint32_t SW_EVT_DONE_IRQ_EN: 1;				/* 1 */
 		vuint32_t FDMA_ERR_IRQ_EN: 1;				/* 1 */
  		vuint32_t: 23;
 
	} B;
	} SEQ_CORE_IRQ_EN;	/* Register offset is 0x01C */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t SEQ_INT_ERR_IRQ_EN: 1;				/* 1 */
 		vuint32_t IDMA_DONE_IRQ_EN: 1;				/* 1 */
 		vuint32_t OUTMSG_RDY_IRQ_EN: 1;				/* 1 */
 		vuint32_t INMSG_ACK_IRQ_EN: 1;				/* 1 */
 		vuint32_t IPUS_EVT_DONE_IRQ_EN: 1;				/* 1 */
 		vuint32_t IPUV_EVT_DONE_IRQ_EN: 1;				/* 1 */
 		vuint32_t OTHR_EVT_DONE_IRQ_EN: 1;				/* 1 */
 		vuint32_t: 1;
 		vuint32_t SEQ_MEM_PERR_IRQ_EN: 1;				/* 1 */
 		vuint32_t: 7;
 		vuint32_t SW_EVT_TRIG_EN: 8;				/* 1 */
  		vuint32_t: 8;
	} B;
	} HOST_IRQ_EN;	/* Register offset is 0x020 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t XBAR_PTY: 1;				/* 1 */
  		vuint32_t: 31;
 
	} B;
	} PTYCTRL;	/* Register offset is 0x024 */
}; 
 

/* ============================================================================
   ============================= Module: VisionSeq_EVT_CTRL =============================
   ============================================================================ */
struct VisionSeq_EVT_CTRL_tag {
	union {
		vuint32_t R;
		struct {
 		vuint32_t DONE_IPUS_ENGN: 1;				/* 1 */
 		vuint32_t DONE_IPUV_ENGN: 1;				/* 1 */
 		vuint32_t DONE_OTHR_ENGN: 1;				/* 1 */
  		vuint32_t: 29;
 
	} B;
	} DONE_ENGN_TYPE;	/* Register offset is 0x000 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t IPUS_CNTR_MAX_VAL: 4;				/* 1 */
  		vuint32_t: 28;
 
	} B;
	} IPUS_CNTR_MAX_VAL;	/* Register offset is 0x004 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t IPUS_CNTR_CLREVT_ENC: 6;				/* 1 */
  		vuint32_t: 26;
 
	} B;
	} IPUS_CNTR_CLREVT_ENC;	/* Register offset is 0x008 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t IPUS_CNTR_CLREVT: 32;				/* 1 */
 
	} B;
	} IPUS_CNTR_CLREVT;	/* Register offset is 0x00C */

	union {
		vuint32_t R;
		struct {
 		vuint32_t IPUS_CNTR_NXTEVT: 6;				/* 1 */
  		vuint32_t: 26;
 
	} B;
	} IPUS_CNTR_NXTEVT;	/* Register offset is 0x010 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t IPUS_CNTR_ALLEVT: 32;				/* 1 */
 
	} B;
	} IPUS_CNTR_ALLEVT;	/* Register offset is 0x014 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t IPUS_ENGN_TRIG_ENC: 6;				/* 1 */
  		vuint32_t: 26;
 
	} B;
	} IPUS_ENGN_TRIG_ENC;	/* Register offset is 0x018 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t IPUS_ENGN_TRIG: 32;				/* 1 */
 
	} B;
	} IPUS_ENGN_TRIG;	/* Register offset is 0x01C */

	union {
		vuint32_t R;
		struct {
 		vuint32_t IPUS_ENGN_FREE_ENC: 6;				/* 1 */
  		vuint32_t: 26;
 
	} B;
	} IPUS_ENGN_FREE_ENC;	/* Register offset is 0x020 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t IPUS_ENGN_FREE: 32;				/* 1 */
 
	} B;
	} IPUS_ENGN_FREE;	/* Register offset is 0x024 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t IPUS_ENGN_USBL: 32;				/* 1 */
 
	} B;
	} IPUS_ENGN_USBL;	/* Register offset is 0x028 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t IPUS_ENGN_FF1_AVL: 6;				/* 1 */
 		vuint32_t: 10;
 		vuint32_t IPUS_ENGN_FF1_BUF_AVL: 6;				/* 1 */
  		vuint32_t: 10;
 
	} B;
	} IPUS_ENGN_FF1;	/* Register offset is 0x02C */

	union {
		vuint32_t R;
		struct {
  		vuint32_t IPUS_ENGN_AVL: 32;				/* 1 */
 
	} B;
	} IPUS_ENGN_AVL;	/* Register offset is 0x030 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t IPUS_ENGN_BUF_AVL: 32;				/* 1 */
 
	} B;
	} IPUS_ENGN_BUF_AVL;	/* Register offset is 0x034 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t IPUV_CNTR_MAX_VAL: 4;				/* 1 */
  		vuint32_t: 28;
 
	} B;
	} IPUV_CNTR_MAX_VAL;	/* Register offset is 0x038 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t IPUV_CNTR_CLREVT_ENC: 6;				/* 1 */
  		vuint32_t: 26;
 
	} B;
	} IPUV_CNTR_CLREVT_ENC;	/* Register offset is 0x03C */

	union {
		vuint32_t R;
		struct {
  		vuint32_t IPUV_CNTR_CLREVT: 32;				/* 1 */
 
	} B;
	} IPUV_CNTR_CLREVT;	/* Register offset is 0x040 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t IPUV_CNTR_NXTEVT: 6;				/* 1 */
  		vuint32_t: 26;
 
	} B;
	} IPUV_CNTR_NXTEVT;	/* Register offset is 0x044 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t IPUV_CNTR_ALLEVT: 32;				/* 1 */
 
	} B;
	} IPUV_CNTR_ALLEVT;	/* Register offset is 0x048 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t IPUV_ENGN_TRIG_ENC: 6;				/* 1 */
  		vuint32_t: 26;
 
	} B;
	} IPUV_ENGN_TRIG_ENC;	/* Register offset is 0x04C */

	union {
		vuint32_t R;
		struct {
  		vuint32_t IPUV_ENGN_TRIG: 32;				/* 1 */
 
	} B;
	} IPUV_ENGN_TRIG;	/* Register offset is 0x050 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t IPUV_ENGN_FREE_ENC: 6;				/* 1 */
  		vuint32_t: 26;
 
	} B;
	} IPUV_ENGN_FREE_ENC;	/* Register offset is 0x054 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t IPUV_ENGN_FREE: 32;				/* 1 */
 
	} B;
	} IPUV_ENGN_FREE;	/* Register offset is 0x058 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t IPUV_ENGN_USBL: 32;				/* 1 */
 
	} B;
	} IPUV_ENGN_USBL;	/* Register offset is 0x05C */

	union {
		vuint32_t R;
		struct {
 		vuint32_t IPUV_ENGN_FF1_AVL: 6;				/* 1 */
 		vuint32_t: 10;
 		vuint32_t IPUV_ENGN_FF1_BUF_AVL: 6;				/* 1 */
  		vuint32_t: 10;
 
	} B;
	} IPUV_ENGN_FF1;	/* Register offset is 0x060 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t IPUV_ENGN_AVL: 32;				/* 1 */
 
	} B;
	} IPUV_ENGN_AVL;	/* Register offset is 0x064 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t IPUV_ENGN_BUF_AVL: 32;				/* 1 */
 
	} B;
	} IPUV_ENGN_BUF_AVL;	/* Register offset is 0x068 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t OTHR_CNTR_CLREVT_ENC: 6;				/* 1 */
  		vuint32_t: 26;
 
	} B;
	} OTHR_CNTR_CLREVT_ENC;	/* Register offset is 0x06C */

	union {
		vuint32_t R;
		struct {
  		vuint32_t OTHR_CNTR_CLREVT: 32;				/* 1 */
 
	} B;
	} OTHR_CNTR_CLREVT;	/* Register offset is 0x070 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t OTHR_CNTR_ALLEVT: 32;				/* 1 */
 
	} B;
	} OTHR_CNTR_ALLEVT;	/* Register offset is 0x074 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t OTHR_ENGN_TRIG_ENC: 6;				/* 1 */
  		vuint32_t: 26;
	} B;
	} OTHR_ENGN_TRIG_ENC;	/* Register offset is 0x078 */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t OTHR_ENGN_TRIG: 32;				/* 1 */
	} B;
	} OTHR_ENGN_TRIG;	/* Register offset is 0x07C */

	union {
		vuint32_t R;
		struct {
 		vuint32_t OTHR_ENGN_FREE_ENC: 6;				/* 1 */
  		vuint32_t: 26;
 
	} B;
	} OTHR_ENGN_FREE_ENC;	/* Register offset is 0x080 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t OTHR_ENGN_FREE: 32;				/* 1 */
 
	} B;
	} OTHR_ENGN_FREE;	/* Register offset is 0x084 */

	union {
		vuint32_t R;
		struct {
   
		vuint32_t RESERVED : 32; 
	} B;
	} RESERVED;	/* Register offset is 0x088 */

	union {
		vuint32_t R;
		struct {
  		vuint32_t OTHR_ENGN_AVL: 32;				/* 1 */
 
	} B;
	} OTHR_ENGN_AVL;	/* Register offset is 0x08C */

	union {
		vuint32_t R;
		struct {
 
  		vuint32_t OTHR_ENGN_BUF_AVL: 32;				/* 1 */
	} B;
	} OTHR_ENGN_BUF_AVL;	/* Register offset is 0x090 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t OTHR0_CNTR_MAX_VAL: 5;				/* 1 */
  		vuint32_t: 27;
 
	} B;
	} OTHR0_CNTR_MAX_VAL;	/* Register offset is 0x094 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t SW_EVT_TRIG: 8;				/* 1 */
  		vuint32_t: 24;
 
	} B;
	} SW_EVT_TRIG;	/* Register offset is 0x098 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t SW_EVT_CLR: 8;				/* 1 */
 		vuint32_t: 8;
 		vuint32_t SW_EVT_DONE: 1;				/* 1 */
  		vuint32_t: 15;
 
	} B;
	} SW_EVT_CLR_DONE;	/* Register offset is 0x09C */
}; 
 

/* ============================================================================
   ============================= Module: VisionSeq_INT_DMA =============================
   ============================================================================ */
struct VisionSeq_INT_DMA_tag {
	union {
		vuint32_t R;
		struct {
 		vuint32_t KRAM_ADDR: 14;				/* 1 */
  		vuint32_t: 18;
 
	} B;
	} KRAM_ADDR;	/* Register offset is 0x000 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t TARGET_IPUS_ENGN: 6;				/* 1 */
 		vuint32_t: 10;
 		vuint32_t TARGET_IPUV_ENGN: 6;				/* 1 */
  		vuint32_t: 10;
 
	} B;
	} TARGET_IPUx_ENGN;	/* Register offset is 0x004 */

	union {
		vuint32_t R;
		struct {
 		vuint32_t TRANSFER_LEN: 15;				/* 1 */
  		vuint32_t: 17;
 
	} B;
	} TRANSFER_LEN;	/* Register offset is 0x008 */

	union {
		vuint32_t R;
		struct {
 
 		vuint32_t START: 1;				/* 1 */
 		vuint32_t: 15;
 		vuint32_t DMA_DONE: 1;				/* 1 */
 		vuint32_t BUSY: 1;				/* 1 */
 		vuint32_t DMA_CFG_ERR: 1;				/* 1 */
 		vuint32_t BED: 1;				/* 1 */
 		vuint32_t BES: 1;				/* 1 */
  		vuint32_t: 11;
	} B;
	} CTRL_STAT;	/* Register offset is 0x00C */
};
/*****************************************XFR_RECORD************/
struct xfr_rec_data_tag
{
    union{
	vuint32_t R;
		struct {
  		vuint32_t DDR_IMG_START_ADDR: 32;
		}B;
    }RECDATA1;

    union {
	vuint32_t R;
		struct {
  		vuint32_t DDR_LINE_STRIDE : 16;
 		vuint32_t DDR_LINE_CNT : 16;				
		} B;
	}RECDATA2;
    
    union{
	vuint32_t R;
		struct {
  		vuint32_t CRC_START_ADDR: 32;
		}B;
    }RECDATA3;

//    union{
//	vuint32_t R;
//	        struct{
//		    vuint32_t LINE_SIZE:21;
//		    vuint32_t :2;
//		    vuint32_t TRANS_REC_WR_BACK:1;
//		    vuint32_t USE_SRAM_CURR_LINE:1;
//		    vuint32_t USE_DDR_CURR_LINE:1;
//		    vuint32_t WR_ENA:1;
//		    vuint32_t DIR:1;
//		    vuint32_t CRC_POLY:1;
//		    vuint32_t CRC_MODE:2;
//		    vuint32_t CRC_ENA:1;
//		}B;
//	}RECDATA4;
    
    union{
	vuint32_t R;
	        struct{
		    vuint32_t CRC_ENA:1;
		    vuint32_t CRC_MODE:2;
		    vuint32_t CRC_POLY:1;
		    vuint32_t DIR:1;
		    vuint32_t WR_ENA:1;
		    vuint32_t TRANS_REC_WR_BACK:1;
		    vuint32_t THREAD_ID:3;
		    vuint32_t :1;
		    vuint32_t LINE_SIZE:21;
		}B;
	}RECDATA4;    

    union{
	vuint32_t R;
		struct {
  		vuint32_t SRAM_START_ADDR: 32;
		}B;
    }RECDATA5;
    
    union {
	vuint32_t R;
		struct {
  		vuint32_t SRAM_LINE_STRIDE : 18;
 		vuint32_t SRAM_LINE_CNT : 14;				
		} B;
	}RECDATA6;
   vuint32_t reserved[0x01];
   
   union {
	vuint32_t R;
		struct {
  		vuint32_t DDR_CURR_LINE : 16;
 		vuint32_t SRAM_CURR_LINE : 16;				
		} B;
	}RECDATA8;

  };
struct XFR_REC_tag 
{
    struct xfr_rec_data_tag RECDATA[128];
};
/***************************************************************/

#ifdef _COMPILE_FOR_HOST_CPU_

/*************************/
#define XFRREC  (*(volatile struct XFR_REC_tag *)   0x7E800000UL )
/*************************/
#define FastDMA  (*(volatile struct FastDMA_tag *)   0x40040000UL )
#define HPSMI  (*(volatile struct HPSMI_tag *)   0x4004C000UL )
#define H264Encoder  (*(volatile struct H264Encoder_tag *)   0x4004B000UL )
#define H264Decoder  (*(volatile struct H264Decoder_tag *)   0x4004A000UL )
#define JPEGDecoder  (*(volatile struct JPEGDecoder_tag *)   0x4004D000UL )
#define ISPScalar  (*(volatile struct ISPScalar_tag *)   0x40042000UL )
#define ISPVector  (*(volatile struct ISPVector_tag *)   0x40062000UL )
#define VisionSeq_CTRL_BLK  (*(volatile struct VisionSeq_CTRL_BLK_tag *)   0x40021000UL )
#define VisionSeq_EVT_CTRL  (*(volatile struct VisionSeq_EVT_CTRL_tag *)   0x40022000UL )
#define VisionSeq_INT_DMA  (*(volatile struct VisionSeq_INT_DMA_tag *)   0x40024000UL )

#else //FOR SEQUENCER 

/*************************/
#define XFRREC  (*(volatile struct XFR_REC_tag *)   0x7E800000UL )
/*************************/
#define FastDMA  (*(volatile struct FastDMA_tag *)   0x40020000UL )
#define HPSMI  (*(volatile struct HPSMI_tag *)   0x4002C000UL )
#define H264Encoder  (*(volatile struct H264Encoder_tag *)   0x4002B000UL )
#define H264Decoder  (*(volatile struct H264Decoder_tag *)   0x4002A000UL )
#define JPEGDecoder  (*(volatile struct JPEGDecoder_tag *)   0x4002D000UL )
#define ISPScalar  (*(volatile struct ISPScalar_tag *)   0x40022000UL )
#define ISPVector  (*(volatile struct ISPVector_tag *)   0x40042000UL )
#define VisionSeq_CTRL_BLK  (*(volatile struct VisionSeq_CTRL_BLK_tag *)   0x40001000UL )
#define VisionSeq_EVT_CTRL  (*(volatile struct VisionSeq_EVT_CTRL_tag *)   0x40002000UL )
#define VisionSeq_INT_DMA  (*(volatile struct VisionSeq_INT_DMA_tag *)   0x40004000UL )

#endif

#endif /* TRIPSUBSYS_H */
