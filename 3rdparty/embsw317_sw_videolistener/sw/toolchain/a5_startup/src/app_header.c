/*
 * app_header.c
 *
 *  Created on: 21 Jul 2015
 *      Author: B48072
 */

#include <s32vs234.h>

typedef unsigned int UINT32;

#define HAB_CMD_NOP       0xc0  /**< No Operation */
#define HAB_CMD_WRT_DAT   0xcc  /**< Write Data */
#define HAB_CMD_CHK_DAT   0xcf  /**< Check Data */
#define DCD_WRITE_PAR(bytes, mask,set)	(((bytes) & 0x7) | ((mask)<<3) | ((set)<<4))
#define cpu_be_16(x)	((((x)& 0xFF) << 8) | (((x) & 0xFF00) >> 8))
#define cpu_be_32(x)	(((cpu_be_16((x)& 0xFFFF)) << 16) | (cpu_be_16((x) >>16)))

#define HAB_BITS_IN_BYTE         8
#define HAB_TAG_DCD  0xd2       /**< Device Configuration Data */
#define HAB_TAG_DCD_VER  0x50       /**< Device Configuration Data version*/
#define HAB_HDR(tag, len, par) {(tagU), {(len)>>HAB_BITS_IN_BYTE, (len)}, (par)}
#define DCD_HEADER(x)	HAB_HDR(HAB_TAG_DCD,(sizeof(x)+sizeof(hab_hdr_t)U),HAB_TAG_DCD_VER)
#define MY_DCD_HEADER(x)	HAB_HDR(HAB_TAG_DCD,((x)+sizeof(hab_hdr_t)U),HAB_TAG_DCD_VER)

#define MC_ME_S_MTRANS_BIT             ((27)  )
#define MC_ME_S_MTRANS                 (((1) << (MC_ME_S_MTRANS_BIT)) )

typedef struct hab_hdr {
    uint8_t tag;              /**< Tag field */
    uint8_t len[2];           /**< Length field in bytes (big-endian) */
    uint8_t par;              /**< Parameters field */
} hab_hdr_t;


const uint32_t dcd[] __attribute__((section(".dcd"))) = {


    ((HAB_TAG_DCD )| (0x0E<<8) | (0x74<<16) | HAB_TAG_DCD_VER <<24U),

    // Turn off CMU
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x400c4000U),						/* Address */
    cpu_be_32(0x00000006U),						/* Value */

    // Enable all modes
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4004a008U),						/* Address */
    cpu_be_32(0x000000FFU),						/* Value */

    // Enable all periferals
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4004a09cU/*4004a09c*/),			/* Address */
    cpu_be_32(0x000000FFU),						/* Value */

    // Enable all periferals
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c280U),			/* Address */
    cpu_be_32(0x01A02070U),						/* Value */

    // XTAL ON, sysclk=XTAL clk in DRUN
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4004a02cU),						/* Address */
    cpu_be_32(0x00100030U),						/* Value */

    /////////////////////////////////////////////
    // Mode entry (DRUN)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U),						/* Address */
    cpu_be_32(0x30005AF0U),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U), 					 /* Address */
    cpu_be_32(0x3000A50FU),	

    ((HAB_CMD_CHK_DAT )|(0xCU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000U),						/*@/@*$ Address $*@/@*/
    cpu_be_32(MC_ME_S_MTRANS),
    /////////////////////////////////////////////

    //Turn off Watchdog SWT4
    ((HAB_CMD_NOP)|(4U<<16)),     /* NOP DCD Header */
    ((HAB_CMD_WRT_DAT )|(0x14U<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),              
    cpu_be_32(0x40086010U),   /* Address */
    cpu_be_32(0xd928c520U),   /* Value */
    cpu_be_32(0x40086000U),   /* Address */
    cpu_be_32(0xff000000U),   /* Value */ 
	

    // XTAL ON, sysclk=XTAL clk in DRUN
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4004a02cU),						/* Address */
    cpu_be_32(0x00100031U),						/* Value */

    // XOSC as system clock1, 2 and 3
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4004a260U),						/* Address */
    cpu_be_32(0x1110U),						/* Value */

    /////////////////////////////////////////////
    // Mode entry (DRUN)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U),						/* Address */
    cpu_be_32(0x30005AF0U),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U), 					 /* Address */
    cpu_be_32(0x3000A50FU),	

    ((HAB_CMD_CHK_DAT )|(0xCU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000U),						/* Address*/
    cpu_be_32(MC_ME_S_MTRANS), 							
    /////////////////////////////////////////////

    ////////////////////////////////////////////////////////
    ///////////////////////// ARM PLL///////////////////////
    ////////////////////////////////////////////////////////
    // PHI 0 output f = 1000 MHz, PHI1 output f = 1000 MHz//
    ////////////////////////////////////////////////////////

    // DFS_0.CTRL.R = 0x2;
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c058U),						/* Address */
    cpu_be_32(0x2U),						/* Value */

    // DFS_0.PORTRESET.R = 0xffffffff;
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c054U),						/* Address */
    cpu_be_32(0xFFFFFFFFU),						/* Value */

    // at first set SRC register to select input clk for PLL block: (0 FIRC selected, 1 XOSC selected)
    //SRC.GPR1.B.ARM_PLL_SOURCE_SELECT = 1;// XOSC selected as input to ARM_PLL
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4007c100U),						/* Address */
    cpu_be_32(0x08000000U),						/* Value */

    // configure PLLDIG registers       
    //PLLDIG_0.PLLDV.B.RFDPHI1 = 1;	 
    //PLLDIG_0.PLLDV.B.RFDPHI = 1;
    //PLLDIG_0.PLLDV.B.PREDIV = 1;	// input clock predivider, divided by 1
    //PLLDIG_0.PLLDV.B.MFD = 25;	// Loop multiplication factor divider
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c028U),						/* Address */
    cpu_be_32(0x02011019U),						/* Value */

    //PLLDIG_0.PLLFD.B.SMDEN = 1;		// sigma-delta modulation enabled
    //PLLDIG_0.PLLFD.B.MFN = 0;		// numerator for fractional loop division factor
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c030U),						/* Address */
    cpu_be_32(0x40000000U),						/* Value */


    // Switch on ARM pll in DRUN
    //MC_ME.DRUN_MC.B.ARMPLLON= 1;		//switch on ARM PLL
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4004a02cU),						/* Address */
    cpu_be_32(0x00100071U),						/* Value */

    /////////////////////////////////////////////
    // Mode entry (DRUN)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U),						/* Address */
    cpu_be_32(0x30005AF0U),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U), 					 /* Address */
    cpu_be_32(0x3000A50FU),	

    ((HAB_CMD_CHK_DAT )|(0xCU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000U),						/* Address*/
    cpu_be_32(MC_ME_S_MTRANS), 							
    /////////////////////////////////////////////

    // DFS settings (DFS0 base adress = 0x4003c40)

    //DFS_0.DFS_DLLPRG1.R =0x00005445;
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c040U),						/* Address */
    cpu_be_32(0x00005445U),						/* Value */

    //DFS_0.DLLPRG2.R =0x0000;
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c044U),						/* Address */
    cpu_be_32(0x0U),						/* Value */

    //    DFS0_DVPORT1: MFN = 194, MFI = 3, fdfs1out = 266 
    //DFS_0.DVPORT[0].B.MFN = 194;
    //DFS_0.DVPORT[0].B.MFI = 3;   
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003C05CU),						/* Address */
    cpu_be_32(0x000003C2U),						/* Value */ 

    //    DFS0_DVPORT2: MFN = 170, MFI = 1, fdfs2out = 600
    //DFS_0.DVPORT[1].B.MFN = 170;
    //DFS_0.DVPORT[1].B.MFI = 1;  
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003C060U),						/* Address */
    cpu_be_32(0x000001AAU),						/* Value */ 

    //    DFS0_DVPORT3: MFN = 170, MFI = 1, fdfs3out = 600
    //DFS_0.DVPORT[2].B.MFN = 170;
    //DFS_0.DVPORT[2].B.MFI = 1;  
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003C064U),						/* Address */
    cpu_be_32(0x000001AAU),						/* Value */ 

    //DFS_0.CTRL.R &= 0xfffffffD;		//    DFS0_CTRL reset is not asserted
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,1,0)) <<24U),	
    cpu_be_32(0x4003C058U),						/* Address */
    cpu_be_32(0x00000002U),						/* Value */

    //DFS_0.PORTRESET.R &= 0xfffffff0;		//    DFS0_PORTRESET bits 0 - 2 are out of reset
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,1,0)) <<24U),	
    cpu_be_32(0x4003C054U),						/* Address */
    cpu_be_32(0x0000000FU),						/* Value */

    //while(DFS_0.PORTSR.R != 0xF);	//DFS0_PORTSR port status register - wait until DVPORT 1-3 are locked, DVPORT 4 not implemented
    ((HAB_CMD_CHK_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,1)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4003C04CU),						/* Address*/
    cpu_be_32(0x0000000FU),

    // note: PLL_DFS1 is selected in DRUN_MC in bitfield SYSCLK
    //all other selectors are set in DRUN Secondary Clock Configuration Register (DFS2, DFS3, ARM PLL)
    //    MC_ME.DRUN_SEC_CC_I.R = 0x00002220;	// system clock selection, ARMPLL outputs selected as input clock in all 4 blocks
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4004a260U),						/* Address */
    cpu_be_32(0x2220U),	    

    //MC_ME.DRUN_MC.B.ARMPLLON = 1; switch ARM_PLL as sysclk 
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4004a02cU),						/* Address */
    cpu_be_32(0x00100072U),

    //mode_enter(DRUN_M);
    // Mode entry (DRUN) ,check ARM PLL on, sysclk = ARM PLL
    //while(MC_ME.GS.B.S_ARMPLL != 1);
    //while(MC_ME.GS.B.S_SYSCLK != 2);
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U),						/* Address */
    cpu_be_32(0x30005AF0U),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U), 					 /* Address */
    cpu_be_32(0x3000A50FU),	

    ((HAB_CMD_CHK_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,1)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000U),						/* Address*/       
    cpu_be_32(0x30100072U),  

    //////////////////////////////////////////////////////
    ///////////////////////// PERIPH PLL//////////////////
    //////////////////////////////////////////////////////
    // PHI 0 output f = 400 MHz, PHI1 output f = 100 MHz//
    //////////////////////////////////////////////////////

    // at first set SRC register to select input clk for PLL block: (0 FIRC selected, 1 XOSC selected)
    //SRC.GPR1.B.PERIPH_PLL_SOURCE_SELECT = 1;// XOSC selected as input to PERIPH_PLL
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4007c100U),						/* Address */
    cpu_be_32(0x48000000U),						/* Value */

    // configure PLLDIG registers
    // PLLDIG_1.PLLDV.B.RFDPHI1 = 12;	
    // PLLDIG_1.PLLDV.B.RFDPHI = 3;	 
    // PLLDIG_1.PLLDV.B.PREDIV = 1;	// input clock predivider, divided by 1
    // PLLDIG_1.PLLDV.B.MFD = 30;	// Loop multiplication factor divider
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c0A8U),						/* Address */
    cpu_be_32(0x1803101EU),						/* Value */

    // PLLDIG_1.PLLFD.B.SMDEN = 1;		// sigma-delta modulation enabled
    // PLLDIG_1.PLLFD.B.MFN = 0;		// numerator for fractional loop division factor
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c0b0U),						/* Address */
    cpu_be_32(0x40000000U),						/* Value */

    // switch on PERIPH_PLL 
    //MC_ME.DRUN_MC.B.PERIPHPLLON = 1;		
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4004a02cU),						/* Address */
    cpu_be_32(0x001000f2U),						/* Value */

    //mode_enter(DRUN_M);
    // Mode entry (DRUN) ,check ARM PLL on, PeriPLL on, sysclk = ARM PLL
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U),						/* Address */
    cpu_be_32(0x30005AF0U),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U), 					 /* Address */
    cpu_be_32(0x3000A50FU),	

    ((HAB_CMD_CHK_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,1)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000U),						/* Address*/       
    cpu_be_32(0x301000f2U), 

    // MC_CGM 0 AUX registers
    //MC_CGM_0.AC3_DC0.B.DE = 0;		// need to switch to 0 to avoid clock glitch
    //MC_CGM_0.AC3_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is LIN_CLK = 133 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c868U),						/* Address */
    cpu_be_32(0x0U),						/* Value */  
    // MC_CGM_0.AC3_SC.B.SELCTL = 8;	// AC3 clock source selection - SYS6_CLK 133 MHz 
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c860U),						/* Address */
    cpu_be_32(0x08000000U),						/* Value */
    // MC_CGM_0.AC3_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c868U),						/* Address */
    cpu_be_32(0x80000000U),						/* Value */

    // MC_CGM_0.AC5_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    // MC_CGM_0.AC5_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is PERI_CLK = 80 MHz    
    // MC_CGM_0.AC5_DC1.B.DE = 0;			//divider enable(1)/disable(0)
    // MC_CGM_0.AC5_DC1.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is FRAY_PLL_CLK = 80 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c8a8U),						/* Address */
    cpu_be_32(0x0U),						/* Value */
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	   
    cpu_be_32(0x4003c8acU),						/* Address */
    cpu_be_32(0x0U),						/* Value */

    // MC_CGM_0.AC5_SC.B.SELCTL = 3;	// AC5 clock source selection - PERIPH_PLL_PHI0/5 
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c8a0U),						/* Address */
    cpu_be_32(0x03000000U),						/* Value */

    // MC_CGM_0.AC5_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    // MC_CGM_0.AC5_DC1.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c8a8U),						/* Address */
    cpu_be_32(0x80000000U),						/* Value */
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c8acU),						/* Address */
    cpu_be_32(0x80000000U),						/* Value */

    // MC_CGM_0.AC6_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    // MC_CGM_0.AC6_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is CAN_CLK = 80 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c8c8U),						/* Address */
    cpu_be_32(0x0U),						/* Value */

    // MC_CGM_0.AC6_SC.B.SELCTL = 3;	// AC6 clock source selection - PERIPH_PLL_PHI0/5
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c8c0U),						/* Address */
    cpu_be_32(0x03000000U),						/* Value */

    // MC_CGM_0.AC6_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c8c8U),						/* Address */
    cpu_be_32(0x80000000U),						/* Value */


    // MC_CGM 2 AUX registers	
    // MC_CGM_2.AC0_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    // MC_CGM_2.AC0_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is DIGRF_PLL_CLK = 26 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x40042808U),						/* Address */
    cpu_be_32(0x0U),						/* Value */

    // MC_CGM_2.AC0_SC.B.SELCTL = 7;	// AC0 clock source selection - external pad clock source 26 MHz
    // MC_CGM_0.AC6_SC.B.SELCTL = 3;	// AC6 clock source selection - PERIPH_PLL_PHI0/5
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x40042800U),						/* Address */
    cpu_be_32(0x07000000U),						/* Value */ 
    // MC_CGM_2.AC0_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x40042808U),						/* Address */
    cpu_be_32(0x80000000U),						/* Value */

    // mode_enter(DRUN_M);
    // Mode entry (DRUN)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U),						/* Address */
    cpu_be_32(0x30005AF0U),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U), 					 /* Address */
    cpu_be_32(0x3000A50FU),	

    ((HAB_CMD_CHK_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000U),						/* Address*/
    cpu_be_32(MC_ME_S_MTRANS), 

    //////////////////////////////////////////////////////
    ///////////////////////// ENET PLL////////////////////
    //////////////////////////////////////////////////////
    // PHI 0 output f = 500 MHz, PHI1 output f = 1000 MHz//
    //////////////////////////////////////////////////////

    // at first set SRC register to select input clk for PLL block: (0 FIRC selected, 1 XOSC selected)
    //SRC.GPR1.B.ENET_PLL_SOURCE_SELECT = 1;// XOSC selected as input to ENET_PLL
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4007c100U),						/* Address */
    cpu_be_32(0x58000000U),						/* Value */

    // configure PLLDIG registers    
    // PLLDIG_2.PLLDV.B.RFDPHI1 = 1;	
    //  PLLDIG_2.PLLDV.B.RFDPHI = 2;	
    // PLLDIG_2.PLLDV.B.PREDIV = 1;	// input clock predivider, divided by 1
    // PLLDIG_2.PLLDV.B.MFD = 25;	// Loop multiplication factor divider
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c128U),						/* Address */
    cpu_be_32(0x02021019U),						/* Value */

    //  PLLDIG_2.PLLFD.B.SMDEN = 1;		// sigma-delta modulation enabled
    //   PLLDIG_2.PLLFD.B.MFN = 0;	// numerator for fractional loop division factor
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c130U),						/* Address */
    cpu_be_32(0x40000000U),						/* Value */


    // switch on ENET_PLL
    // MC_ME.DRUN_MC.B.ENETPLLON= 1;		//switch on ENET PLL
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4004a02cU),						/* Address */
    cpu_be_32(0x001001f2U),						/* Value */


    //mode_enter(DRUN_M);
    // Mode entry (DRUN) ,check ARM PLL on, PeriPLL on, ENETPLL on, sysclk = ARM PLL
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U),						/* Address */
    cpu_be_32(0x30005AF0U),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U), 					 /* Address */
    cpu_be_32(0x3000A50FU),	

    ((HAB_CMD_CHK_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,1)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000U),						/* Address*/       
    cpu_be_32(0x301001f2U), 


    //DFS_1.DFS_DLLPRG1.R =0x00005445;
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4003c140U), 					 /* Address */
    cpu_be_32(0x00005445U),
    //DFS_1.DLLPRG2.R =0x0000;
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4003c144U), 					 /* Address */
    cpu_be_32(0x0U),

    // DFS settings
    // DFS_1.DVPORT[0].B.MFN = 219;
    // DFS_1.DVPORT[0].B.MFI = 2;   
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4003c15cU), 					 /* Address */
    cpu_be_32(0x000002DBU),

    // DFS_1.DVPORT[1].B.MFN = 219;
    // DFS_1.DVPORT[1].B.MFI = 2; 
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4003c160U), 					 /* Address */
    cpu_be_32(0x000002DBU),      

    // DFS_1.DVPORT[2].B.MFN = 32;
    // DFS_1.DVPORT[2].B.MFI = 3;
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4003c164U), 					 /* Address */
    cpu_be_32(0x00000320U),   

    // DFS_1.DVPORT[3].B.MFN = 156;
    // DFS_1.DVPORT[3].B.MFI = 9;  
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4003c168U), 					 /* Address */
    cpu_be_32(0x0000099CU), 

    // DFS_1.CTRL.R &= (~2);		//    DFS0_CTRL reset is not asserted
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4003c158U), 					 /* Address */
    cpu_be_32(0x00000000U),


    // DFS_1.PORTRESET.R &= (~15);		//    DFS0_PORTRESET bits 0 - 3 are out of reset
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4003c154U), 					 /* Address */
    cpu_be_32(0x00000000U),
    //*((vuint32_t*)(0x4003c154)) &= (~15);		//    DFS2_PORTRESET bits 0 - 3 are out of reset

    // while(DFS_1.PORTSR.R != 0xF);	//DFS0_PORTSR port status register - wait until DVPORT 1-4 are locked 
    ((HAB_CMD_CHK_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,1)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4003c14cU),						/* Address*/
    cpu_be_32(0x0000000FU),

    // MC_CGM 0 AUX registers
    //MC_CGM_0.AC7_DC1.B.DE = 0;			//divider enable(1)/disable(0)
    //MC_CGM_0.AC7_DC1.B.DIV = 3;			//divided by 4 (1 + DIV) - resultant output clk is ENET_TIME_CLK = 125 MHz  
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c8ecU),						/* Address */
    cpu_be_32(0x00030000U),						/* Value */   
    //   MC_CGM_0.AC7_SC.B.SELCTL = 4;	// AC7 clock source selection - ENET_PLL_PHI0
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c8e0U),						/* Address */
    cpu_be_32(0x04000000U),						/* Value */  
    //  MC_CGM_0.AC7_DC1.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c8ecU),						/* Address */
    cpu_be_32(0x80030000U),						/* Value */ 

    // MC_CGM_0.AC12_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    // MC_CGM_0.AC12_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is H264_DEC_CLK = 350 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c988U),						/* Address */
    cpu_be_32(0x0U),						/* Value */
    // MC_CGM_0.AC12_SC.B.SELCTL = 4;	// AC12 clock source selection - ENET_PLL_DFS1
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c980U),						/* Address */
    cpu_be_32(0x04000000U),						/* Value */  
    //MC_CGM_0.AC12_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c988U),						/* Address */
    cpu_be_32(0x80000000U),						/* Value */ 

    // MC_CGM_0.AC13_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    // MC_CGM_0.AC13_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is H264_ENC_CLK = 350 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c9A8U),						/* Address */
    cpu_be_32(0x0U),						/* Value */
    // MC_CGM_0.AC13_SC.B.SELCTL = 4;	// AC13 clock source selection - ENET_PLL_DFS2
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c9A0U),						/* Address */
    cpu_be_32(0x04000000U),						/* Value */  
    // MC_CGM_0.AC13_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c9A8U),						/* Address */
    cpu_be_32(0x80000000U),						/* Value */ 

    // MC_CGM_0.AC14_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    // MC_CGM_0.AC14_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is QSPI_CLK = 320 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c9C8U),						/* Address */
    cpu_be_32(0x0U),						/* Value */
    // MC_CGM_0.AC14_SC.B.SELCTL = 4;	// AC14 clock source selection - ENET_PLL_DFS3
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c9C0U),						/* Address */
    cpu_be_32(0x04000000U),						/* Value */  
    // MC_CGM_0.AC14_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c9C8U),						/* Address */
    cpu_be_32(0x80000000U),						/* Value */ 

    // MC_CGM_0.AC15_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    // MC_CGM_0.AC15_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is H264_DEC_CLK = 104 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c9e8U),						/* Address */
    cpu_be_32(0x0U),						/* Value */
    // MC_CGM_0.AC15_SC.B.SELCTL = 4;	// AC15 clock source selection - ENET_PLL_DFS4
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c9e0U),						/* Address */
    cpu_be_32(0x04000000U),						/* Value */  
    // MC_CGM_0.AC15_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c9e8U),						/* Address */
    cpu_be_32(0x80000000U),						/* Value */ 

    // mode_enter(DRUN_M);

    // Mode entry (DRUN)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U),						/* Address */
    cpu_be_32(0x30005AF0U),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U), 					 /* Address */
    cpu_be_32(0x3000A50FU),	

    ((HAB_CMD_CHK_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000U),						/* Address*/
    cpu_be_32(MC_ME_S_MTRANS),



    ///////////////////////////////////////////////////////
    ///////////////////////// DDR PLL//////////////////////
    ///////////////////////////////////////////////////////
    // PHI 0 output f = 533 MHz, PHI1 output f = 1066 MHz//
    ///////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////
    ///////////////////////// DDR PLL//////////////////////
    ///////////////////////////////////////////////////////
    // PHI 0 output f = 533 MHz, PHI1 output f = 1066 MHz//
    ///////////////////////////////////////////////////////
    // at first set SRC register to select input clk for PLL block: (0 FIRC selected, 1 XOSC selected)
    //SRC.GPR1.B.DDR_PLL_SOURCE_SELECT = 1;// XOSC selected as input to DDR_PLL
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4007c100U),						/* Address */
    cpu_be_32(0x78000000U),						/* Value */

    // configure PLLDIG registers


    // PLLDIG_3.PLLDV.B.RFDPHI1 = 1;	
    // PLLDIG_3.PLLDV.B.RFDPHI = 2;	
    //  PLLDIG_3.PLLDV.B.PREDIV = 1;	// input clock predivider, divided by 1
    //  PLLDIG_3.PLLDV.B.MFD = 26;	// Loop multiplication factor divider    
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003C1A8U),						/* Address */
    cpu_be_32(0x0202101AU),	


    // PLLDIG_3.PLLFD.B.SMDEN = 1;		// sigma-delta modulation enabled
    // PLLDIG_3.PLLFD.B.MFN = 13312;	// numerator for fractional loop division factor
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c1b0U),						/* Address */
    cpu_be_32(0x40003400U),

    // switch on DDR_PLL 
    // MC_ME.DRUN_MC.B.DDRPLLON = 1;	
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4004a02cU),						/* Address */
    cpu_be_32(0x001003F2U),						/* Value */


    // Mode entry (DRUN)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U),						/* Address */
    cpu_be_32(0x30005AF0U),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U), 					 /* Address */
    cpu_be_32(0x3000A50FU),	

    ((HAB_CMD_CHK_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,1)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000U),						/* Address*/
    cpu_be_32(0x301003F2U),

    // DFS_2.DFS_DLLPRG1.R =0x00005445;
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c1c0U),						/* Address */
    cpu_be_32(0x00005445U),

    // DFS_2.DLLPRG2.R =0x0000;
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c1c4U),						/* Address */
    cpu_be_32(0x0U),

    // DFS settings
    // DFS_2.DVPORT[0].B.MFN = 33;
    // DFS_2.DVPORT[0].B.MFI = 2; 
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003C1DCU),						/* Address */
    cpu_be_32(0x00000221U),  

    // DFS_2.DVPORT[1].B.MFN = 33;
    // DFS_2.DVPORT[1].B.MFI = 2;  
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c1e0U),						/* Address */
    cpu_be_32(0x00000221U),

    //  DFS_2.DVPORT[0].B.MFN = 11;
    //  DFS_2.DVPORT[0].B.MFI = 3; 
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c1e4U),						/* Address */
    cpu_be_32(0x0000030bU), 

    // DFS_2.CTRL.R &= (~2);		//    DFS0_CTRL reset is not asserted
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c1d8U),						/* Address */
    cpu_be_32(0x00000000U),

    // DFS_2.PORTRESET.R &= (~7);		//    DFS0_PORTRESET bits 0 - 2 are out of reset    
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),
    cpu_be_32(0x4003c1d4U),						/* Address */
    cpu_be_32(0x00000000U),


    // mode_enter(DRUN_M);
    // while(MC_ME.GS.B.S_DDRPLL != 1)
    // while(DFS_2.PORTSR.R != 0x7);	//DFS0_PORTSR port status register - wait until DVPORT 1-3 are locked, port 4 is not
    //mode_enter(DRUN_M);
    // Mode entry (DRUN) ,check ARM PLL on, sysclk = ARM PLL
    //while(MC_ME.GS.B.S_ARMPLL != 1);
    //while(MC_ME.GS.B.S_SYSCLK != 2);
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U),						/* Address */
    cpu_be_32(0x30005AF0U),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U), 					 /* Address */
    cpu_be_32(0x3000A50FU),	

    ((HAB_CMD_CHK_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,1)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000U),						/* Address*/       
    cpu_be_32(0x301003F2U),  

    ((HAB_CMD_CHK_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,1)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4003c14cU),						/* Address*/
    cpu_be_32(0x0000000FU),  

    // MC_CGM 0 AUX registers
    //  MC_CGM_0.AC0_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC0_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is ISP_CLK = 500 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c808U),						/* Address */
    cpu_be_32(0x0U),						/* Value */  

    //  MC_CGM_0.AC0_SC.B.SELCTL = 5;	// AC0 clock source selection - DDR_PLL_DFS1
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c800U),						/* Address */
    cpu_be_32(0x05000000U),						/* Value */

    // MC_CGM_0.AC0_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c808U),						/* Address */
    cpu_be_32(0x80000000U),						/* Value */  


    //  MC_CGM_0.AC1_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC1_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is APEX_APU_CLK = 500 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c828U),						/* Address */
    cpu_be_32(0U),						/* Value */  

    //  MC_CGM_0.AC1_SC.B.SELCTL = 5;	// AC1 clock source selection - DDR_PLL_DFS2
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c820U),						/* Address */
    cpu_be_32(0x05000000U),						/* Value */

    //  MC_CGM_0.AC1_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c828U),						/* Address */
    cpu_be_32(0x80000000U),						/* Value */  


    //   MC_CGM_0.AC2_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC2_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is MJEPEG_CLK = 500 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c848U),						/* Address */
    cpu_be_32(0U),						/* Value */  

    //  MC_CGM_0.AC2_SC.B.SELCTL = 5;	// AC2 clock source selection - DDR_PLL_DFS3
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c840U),						/* Address */
    cpu_be_32(0x05000000U),						/* Value */

    //  MC_CGM_0.AC2_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c848U),						/* Address */
    cpu_be_32(0x80000000U),						/* Value */  


    //  MC_CGM_0.AC8_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC8_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is DDR_CLK = 533 MHz
    //  MC_CGM_0.AC8_DC1.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC8_DC1.B.DIV = 3;			//divided by 4 (1 + DIV) - resultant output clk is DDR4_CLK = 133,25 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c908U),						/* Address */
    cpu_be_32(0U),						/* Value */  
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c90cU),						/* Address */
    cpu_be_32(0x00030000U),						/* Value */  

    //  MC_CGM_0.AC8_SC.B.SELCTL = 5;	// AC8 clock source selection - DDR_PLL_PHI0
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c900U),						/* Address */
    cpu_be_32(0x05000000U),						/* Value */

    //  MC_CGM_0.AC8_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC8_DC1.B.DE = 1;			//divider enable(1)/disable(0)	  
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c908U),						/* Address */
    cpu_be_32(0x80000000U),						/* Value */ 
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c90cU),						/* Address */
    cpu_be_32(0x80030000U),						/* Value */ 


    // Mode entry (DRUN)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U),						/* Address */
    cpu_be_32(0x30005AF0U),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U), 					 /* Address */
    cpu_be_32(0x3000A50FU),	

    ((HAB_CMD_CHK_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000U),						/* Address*/
    cpu_be_32(MC_ME_S_MTRANS),



    ////////////////////////////////////////////////////
    ///////////////////////// VIDEO PLL/////////////////
    ////////////////////////////////////////////////////
    //////////// PHI 0 output f = 600 MHz///////////////
    ////////////////////////////////////////////////////

    // at first set SRC register to select input clk for PLL block: (0 FIRC selected, 1 XOSC selected)
    // SRC.GPR1.B.VIDEO_PLL_SOURCE_SELECT = 1;// XOSC selected as input to VIDEO_PLL
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4007c100U),						/* Address */
    cpu_be_32(0xF8000000U),						/* Value */

//PLLDIG_4.PLLCAL3.R=0x14000000; // validation team using it but this register is not in RM!
    //  *((vuint32_t*)(0x4003c218)) = 0x14000000;
    // configure PLLDIG registers   
    // PLLDIG_4.PLLDV.B.RFDPHI1 = 0;	// PHI1 output NA
    // PLLDIG_4.PLLDV.B.RFDPHI = 2;
    // PLLDIG_4.PLLDV.B.PREDIV = 1;	// input clock predivider, divided by 1
    // PLLDIG_4.PLLDV.B.MFD = 30;	// Loop multiplication factor divider
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c228U),						/* Address */
    cpu_be_32(0x0002101eU),						/* Value */    

    //     PLLDIG_4.PLLFD.B.SMDEN = 1;		// sigma-delta modulation enabled
    //     PLLDIG_4.PLLFD.B.MFN = 0;	//FRCDIV value - numerator for fractional loop division factor
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c230U),						/* Address */
    cpu_be_32(0x70000000U),						/* Value */    

    // switch on VIDEO_PLL 
    // MC_ME.DRUN_MC.B.VIDEOPLLON = 1;		//switch on VIDEO_PLL 
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4004a02cU),						/* Address */
    cpu_be_32(0x001007F2U),						/* Value */

    // mode_enter(DRUN_M);
    // while(MC_ME.GS.B.S_VIDEOPLL != 1);

    //mode_enter(DRUN_M);
    // Mode entry (DRUN) ,check ARM PLL on, PeriPLL on, ENETPLL on, sysclk = ARM PLL
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U),						/* Address */
    cpu_be_32(0x30005AF0U),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U), 					 /* Address */
    cpu_be_32(0x3000A50FU),	

    ((HAB_CMD_CHK_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,1)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000U),						/* Address*/       
    cpu_be_32(0x301007F2U), 

    // MC_CGM 0 AUX registers
    //  MC_CGM_0.AC9_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC9_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is DCU_AXI_CLK = 300 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c928U),						/* Address */
    cpu_be_32(0U),						/* Value */   

    //  MC_CGM_0.AC9_DC1.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC9_DC1.B.DIV = 1;			//divided by 2 (1 + DIV) - resultant output clk is DCU_PIX_CLK = 150 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c92cU),						/* Address */
    cpu_be_32(0x00010000U),						/* Value */  

    //  MC_CGM_0.AC9_SC.B.SELCTL = 9;	// AC9 clock source selection - VIDEO_PLL/2
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c920U),						/* Address */
    cpu_be_32(0x09000000U),

    //  MC_CGM_0.AC9_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC9_DC1.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c928U),						/* Address */
    cpu_be_32(0x80000000U),						/* Value */
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c92cU),						/* Address */
    cpu_be_32(0x80010000U),						/* Value */


    // MC_CGM 2 AUX registers
    // MC_CGM_2.AC4_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    //MC_CGM_2.AC4_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is TRACECLK = 300 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x40042888U),						/* Address */
    cpu_be_32(0U),

    //MC_CGM_2.AC4_SC.B.SELCTL = 9;	// AC4 clock source selection - VIDEO_PLL/2
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x40042880U),						/* Address */
    cpu_be_32(0x09000000U),

    //  MC_CGM_2.AC4_DC0.B.DE = 1;			//divider enable(1)/disable(0)	  
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x40042888U),						/* Address */
    cpu_be_32(0x80000000U),

    // remaining AUX registers

    // MC_CGM 0 AUX registers
    //  MC_CGM_0.AC4_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC4_DC0.B.DIV = 3;			//divided by 4 (1 + DIV) - resultant output clk is FLEXTIMER0_EXT_CLK = 20 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c888U),						/* Address */
    cpu_be_32(0x00030000U),

    //  MC_CGM_0.AC4_SC.B.SELCTL = 0xA;	// AC4 clock source selection - PERI_CLK 80 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c880U),						/* Address */
    cpu_be_32(0x0A000000U),

    //  MC_CGM_0.AC4_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c888U),						/* Address */
    cpu_be_32(0x80000000U),

    //  MC_CGM_0.AC10_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC10_DC0.B.DIV = 7;			//divided by 8 (1 + DIV) - resultant output clk is CLK_OUT = 20 MHz
    //  MC_CGM_0.AC10_DC1.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC10_DC1.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is LBIST_FAST_CLK = 400 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003C948U),						/* Address */
    cpu_be_32(0x0U),
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003C94CU),						/* Address */
    cpu_be_32(0U),

    //  MC_CGM_0.AC10_SC.B.SELCTL = 3;	// AC10 clock source selection - PERIPH_PLL_PHI0 400 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003C940U),						/* Address */
    cpu_be_32(0x03000000U),

    //  MC_CGM_0.AC10_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC10_DC1.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003C948U),						/* Address */
    cpu_be_32(0x80070000U),
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003C94CU),						/* Address */
    cpu_be_32(0x80000000U),


    //  MC_CGM_0.AC11_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC11_DC0.B.DIV = 0;		//divided by 1 (1 + DIV) - resultant output clk is DDR_32KHZ_CLK = 32 kHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c968U),						/* Address */
    cpu_be_32(0U),

    //  MC_CGM_0.AC11_SC.B.SELCTL = 1;	// AC11 clock source selection - XOSC_32KHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c960U),						/* Address */
    cpu_be_32(0x01000000U),

    //  MC_CGM_0.AC11_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x4003c968U),						/* Address */
    cpu_be_32(0x80000000U),


    // MC_CGM 2 AUX registers
    //  MC_CGM_2.AC3_DC0.B.DE = 0;			//divider enable(1)/disable(0)
//    MC_CGM_2.AC3_DC0.B.DIV = 3;			//divided by 4 (1 + DIV) - resultant output clk is FLEXTIMER1_EXT_CLK = 20 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x40042868U),						/* Address */
    cpu_be_32(0U),

    //  MC_CGM_2.AC3_SC.B.SELCTL = 10;	// AC4 clock source selection - PERI_CLK 80 MHz
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x40042860U),						/* Address */
    cpu_be_32(0x0a000000U),

    // MC_CGM_2.AC3_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	
    cpu_be_32(0x40042868U),						/* Address */
    cpu_be_32(0x80000000U),


    // mode_enter(DRUN_M);

    // Mode entry (DRUN)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U),						/* Address */
    cpu_be_32(0x30005AF0U),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U), 					 /* Address */
    cpu_be_32(0x3000A50FU),	

    ((HAB_CMD_CHK_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000U),						/* Address*/
    cpu_be_32(MC_ME_S_MTRANS),





    // PCTL conf
    ((HAB_CMD_WRT_DAT )|(0xDC<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a0e4U),						/* Address */
    cpu_be_32(0x00000007U),							/* Value */
    cpu_be_32(0x4004a0e8U),						/* Address */
    cpu_be_32(0x07000000U),							/* Value */
    cpu_be_32(0x4004a0f0U),						/* Address */
    cpu_be_32(0x07070700U),							/* Value */
    cpu_be_32(0x4004a0f4U),						/* Address */
    cpu_be_32(0x07000700U),							/* Value */
    cpu_be_32(0x4004a0f8U),						/* Address */
    cpu_be_32(0x00000700U),							/* Value */

    cpu_be_32(0x4004a10cU),						/* Address */
    cpu_be_32(0x00070007U),							/* Value */
    cpu_be_32(0x4004a110U),						/* Address */
    cpu_be_32(0x00070007U),							/* Value */
    cpu_be_32(0x4004a114U),						/* Address */
    cpu_be_32(0x00070007U),							/* Value */
    cpu_be_32(0x4004a118U),						/* Address */
    cpu_be_32(0x00070007U),							/* Value */
    cpu_be_32(0x4004a11cU),						/* Address */
    cpu_be_32(0x00070000U),							/* Value */

    cpu_be_32(0x4004a124U),						/* Address */
    cpu_be_32(0x07000000U),							/* Value */
    cpu_be_32(0x4004a128U),						/* Address */
    cpu_be_32(0x07000000U),							/* Value */
    cpu_be_32(0x4004a134U),						/* Address */
    cpu_be_32(0x07000000U),							/* Value */
    cpu_be_32(0x4004a138U),						/* Address */
    cpu_be_32(0x07000000U),							/* Value */
    cpu_be_32(0x4004a160U),						/* Address */
    cpu_be_32(0x07070700U),							/* Value */

    cpu_be_32(0x4004a164U),						/* Address */
    cpu_be_32(0x00000700U),							/* Value */
    cpu_be_32(0x4004a168U),						/* Address */
    cpu_be_32(0x00000700U),							/* Value */
    cpu_be_32(0x4004a174U),						/* Address */
    cpu_be_32(0x00000700U),							/* Value */
    cpu_be_32(0x4004a178U),						/* Address */
    cpu_be_32(0x07000700U),							/* Value */
    cpu_be_32(0x4004a17cU),						/* Address */
    cpu_be_32(0x07000700U),							/* Value */

    cpu_be_32(0x4004a180U),						/* Address */
    cpu_be_32(0x07000700U),							/* Value */
    cpu_be_32(0x4004a18cU),						/* Address */
    cpu_be_32(0x07000700U),							/* Value */
    cpu_be_32(0x4004a190U),						/* Address */
    cpu_be_32(0x07000000U),							/* Value */
    cpu_be_32(0x4004a194U),						/* Address */
    cpu_be_32(0x07000000U),							/* Value */
    cpu_be_32(0x4004a198U),						/* Address */
    cpu_be_32(0x07000000U),							/* Value */

    cpu_be_32(0x4004a19cU),						/* Address */
    cpu_be_32(0x07000000U),							/* Value */
    
    cpu_be_32(0x4004a1acU),						/* Address */
    cpu_be_32(0x07000000U),							/* Value */
    /////////////////////////////////////////////
    // Mode entry (DRUN)
    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U),						/* Address */
    cpu_be_32(0x30005AF0U),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004U), 					 /* Address */
    cpu_be_32(0x3000A50FU),	

    ((HAB_CMD_CHK_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000U),						/* Address*/
    cpu_be_32(MC_ME_S_MTRANS), 							
    /////////////////////////////////////////////


    //////////////////////////////////////////////////////
    ///////////////////////// DDR 0 INIT /////////////////
    //////////////////////////////////////////////////////
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C4e4U),						/* Address */
    cpu_be_32(0x31001540U),							/* Value */
	cpu_be_32(0x4006C4F4U),						/* Address */
    cpu_be_32(0x31001500U),							/* Value */
	cpu_be_32(0x4006C4F8U),						/* Address */
    cpu_be_32(0x31001500U),							/* Value */
	cpu_be_32(0x4006C4FCU),						/* Address */
    cpu_be_32(0x31001500U),							/* Value */
	cpu_be_32(0x4006C500U),						/* Address */
    cpu_be_32(0x31001500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C550U),						/* Address */
    cpu_be_32(0x31001500U),							/* Value */
	cpu_be_32(0x4006C554U),						/* Address */
    cpu_be_32(0x31001500U),							/* Value */
	cpu_be_32(0x4006C558U),						/* Address */
    cpu_be_32(0x31001500U),							/* Value */
	cpu_be_32(0x4006C55cU),						/* Address */
    cpu_be_32(0x31001500U),							/* Value */	
	cpu_be_32(0x4006C560U),						/* Address */
    cpu_be_32(0x30012540U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C564U),						/* Address */
    cpu_be_32(0x30012540U),							/* Value */
	cpu_be_32(0x4006C568U),						/* Address */
    cpu_be_32(0x30012540U),							/* Value */
	cpu_be_32(0x4006C56cU),						/* Address */
    cpu_be_32(0x30012540U),							/* Value */
	cpu_be_32(0x4006C510U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C514U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C518U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C51CU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C520U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C524U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C528U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C52CU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C530U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C534U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C570U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C574U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C578U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C57CU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C580U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C584U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C588U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C58CU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C590U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C594U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C598U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C59CU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C5A0U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C5A4U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C5A8U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C5ACU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C5B0U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C5B4U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C5B8U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C5BCU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C5C0U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C5C4U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C5C8U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C5CCU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C5D0U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C5D4U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C5D8U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C5DCU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C5E0U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C5E4U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C5E8U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C5ECU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4003601CU),						/* Address */
    cpu_be_32(0x00008000U),							/* Value */
	cpu_be_32(0x4003600CU),						/* Address */
    cpu_be_32(0x464F61A5U),							/* Value */
	cpu_be_32(0x40036010U),						/* Address */
    cpu_be_32(0x00180E63U),							/* Value */
	cpu_be_32(0x40036014U),						/* Address */
    cpu_be_32(0x000000DDU),							/* Value */
	cpu_be_32(0x40036038U),						/* Address */
    cpu_be_32(0x001F099BU),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x40036008U),						/* Address */
    cpu_be_32(0x00000000U),							/* Value */
	cpu_be_32(0x40036018U),						/* Address */
    cpu_be_32(0x00001688U),							/* Value */
	cpu_be_32(0x40036030U),						/* Address */
    cpu_be_32(0x00000010U),							/* Value */
	cpu_be_32(0x40036000U),						/* Address */
    cpu_be_32(0x03010000U),							/* Value */
	cpu_be_32(0x400368B8U),						/* Address */
    cpu_be_32(0x00000800U),							/* Value */


	((HAB_CMD_CHK_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x400368B8U),						/* Address*/
    cpu_be_32(0x00000800U), 		


	((HAB_CMD_WRT_DAT )|(0x1C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4003601CU),						/* Address */
    cpu_be_32(0x003F8030U),							/* Value */
	cpu_be_32(0x4003685CU),						/* Address */
    cpu_be_32(0x1B5F0109U),							/* Value */
	cpu_be_32(0x40036800U),						/* Address */
    cpu_be_32(0xA0010003U),							/* Value */

	((HAB_CMD_CHK_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x40036800U),						/* Address*/
    cpu_be_32(0x00010000U), 		


	((HAB_CMD_WRT_DAT )|(0x7C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x40036000U),						/* Address */
    cpu_be_32(0x83010000U),							/* Value */
	cpu_be_32(0x4003601CU),						/* Address */
    cpu_be_32(0xC2018030U),							/* Value */
	cpu_be_32(0x4003601CU),						/* Address */
    cpu_be_32(0x06028030U),							/* Value */
	cpu_be_32(0x4003601CU),						/* Address */
    cpu_be_32(0x01038030U),							/* Value */
	cpu_be_32(0x4003601CU),						/* Address */
    cpu_be_32(0xFF0A8030U),							/* Value */
	
	cpu_be_32(0x40036040U),						/* Address */
    cpu_be_32(0x0000007FU),							/* Value */
	cpu_be_32(0x40036848U),						/* Address */
    cpu_be_32(0x4D4B4F4BU),							/* Value */
	cpu_be_32(0x40036850U),						/* Address */
    cpu_be_32(0x38383737U),							/* Value */
	cpu_be_32(0x4003683CU),						/* Address */
    cpu_be_32(0x20000000U),							/* Value */
	cpu_be_32(0x40036840U),						/* Address */
    cpu_be_32(0x00000000U),							/* Value */
	
	cpu_be_32(0x4003602CU),						/* Address */
    cpu_be_32(0x0F9F26D2U),							/* Value */
	cpu_be_32(0x40036004U),						/* Address */
    cpu_be_32(0x00020024U),							/* Value */
	cpu_be_32(0x40036020U),						/* Address */
    cpu_be_32(0x30B01800U),							/* Value */
	cpu_be_32(0x40036818U),						/* Address */
    cpu_be_32(0x00000000U),							/* Value */
	cpu_be_32(0x4003601CU),						/* Address */
    cpu_be_32(0x00000000U),							/* Value */
	
	
	
	//////////////////////////////////////////////////////
    ///////////////////////// DDR 1 INIT /////////////////
    //////////////////////////////////////////////////////
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C600U),						/* Address */
    cpu_be_32(0x31001540U),							/* Value */
	cpu_be_32(0x4006C610U),						/* Address */
    cpu_be_32(0x31001500U),							/* Value */
	cpu_be_32(0x4006C614U),						/* Address */
    cpu_be_32(0x31001500U),							/* Value */
	cpu_be_32(0x4006C618U),						/* Address */
    cpu_be_32(0x31001500U),							/* Value */
	cpu_be_32(0x4006C61CU),						/* Address */
    cpu_be_32(0x31001500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C66CU),						/* Address */
    cpu_be_32(0x31001500U),							/* Value */
	cpu_be_32(0x4006C670U),						/* Address */
    cpu_be_32(0x31001500U),							/* Value */
	cpu_be_32(0x4006C674U),						/* Address */
    cpu_be_32(0x31001500U),							/* Value */
	cpu_be_32(0x4006C678U),						/* Address */
    cpu_be_32(0x31001500U),							/* Value */	
	cpu_be_32(0x4006C67CU),						/* Address */
    cpu_be_32(0x30012540U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C680U),						/* Address */
    cpu_be_32(0x30012540U),							/* Value */
	cpu_be_32(0x4006C684U),						/* Address */
    cpu_be_32(0x30012540U),							/* Value */
	cpu_be_32(0x4006C688U),						/* Address */
    cpu_be_32(0x30012540U),							/* Value */
	cpu_be_32(0x4006C62CU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C630U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C634U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C638U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C63CU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C640U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C644U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C648U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C64CU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C650U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C68CU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C690U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C694U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C698U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C69CU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C6A0U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C6A4U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C6A8U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C6ACU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C6B0U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C6B4U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C6B8U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C6BCU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C6C0U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C6C4U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C6C8U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C6CCU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C6D0U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C6D4U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C6D8U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C6DCU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C6E0U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C6E4U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C6E8U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C6ECU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C6F0U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C6F4U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C6F8U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C6FCU),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C700U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C704U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	cpu_be_32(0x4006C708U),						/* Address */
    cpu_be_32(0x71401500U),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x400A201CU),						/* Address */
    cpu_be_32(0x00008000U),							/* Value */
	cpu_be_32(0x400A200CU),						/* Address */
    cpu_be_32(0x464F61A5U),							/* Value */
	cpu_be_32(0x400A2010U),						/* Address */
    cpu_be_32(0x00180E63U),							/* Value */
	cpu_be_32(0x400A2014U),						/* Address */
    cpu_be_32(0x000000DDU),							/* Value */
	cpu_be_32(0x400A2038U),						/* Address */
    cpu_be_32(0x001F099BU),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x400A2008U),						/* Address */
    cpu_be_32(0x00000000U),							/* Value */
	cpu_be_32(0x400A2018U),						/* Address */
    cpu_be_32(0x00001688U),							/* Value */
	cpu_be_32(0x400A2030U),						/* Address */
    cpu_be_32(0x00000010U),							/* Value */
	cpu_be_32(0x400A2000U),						/* Address */
    cpu_be_32(0x03010000U),							/* Value */
	cpu_be_32(0x400A28B8U),						/* Address */
    cpu_be_32(0x00000800U),							/* Value */


	((HAB_CMD_CHK_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x400A28B8U),						/* Address*/
    cpu_be_32(0x00000800U), 		


	((HAB_CMD_WRT_DAT )|(0x1C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x400A201CU),						/* Address */
    cpu_be_32(0x003F8030U),							/* Value */
	cpu_be_32(0x400A285CU),						/* Address */
    cpu_be_32(0x1B5F0109U),							/* Value */
	cpu_be_32(0x400A2800U),						/* Address */
    cpu_be_32(0xA0010003U),							/* Value */

	((HAB_CMD_CHK_DAT )|(0xcU<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x400A2800U),						/* Address*/
    cpu_be_32(0x00010000U), 		


	((HAB_CMD_WRT_DAT )|(0x7C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24U), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x400A2000U),						/* Address */
    cpu_be_32(0x83010000U),							/* Value */
	cpu_be_32(0x400A201CU),						/* Address */
    cpu_be_32(0xC2018030U),							/* Value */
	cpu_be_32(0x400A201CU),						/* Address */
    cpu_be_32(0x06028030U),							/* Value */
	cpu_be_32(0x400A201CU),						/* Address */
    cpu_be_32(0x01038030U),							/* Value */
	cpu_be_32(0x400A201CU),						/* Address */
    cpu_be_32(0xFF0A8030U),							/* Value */
	
	cpu_be_32(0x400A2040U),						/* Address */
    cpu_be_32(0x0000007FU),							/* Value */
	cpu_be_32(0x400A2848U),						/* Address */
    cpu_be_32(0x4D4B4F4BU),							/* Value */
	cpu_be_32(0x400A2850U),						/* Address */
    cpu_be_32(0x38383737U),							/* Value */
	cpu_be_32(0x400A283CU),						/* Address */
    cpu_be_32(0x20000000U),							/* Value */
	cpu_be_32(0x400A2840U),						/* Address */
    cpu_be_32(0x00000000U),							/* Value */
	
	cpu_be_32(0x400A202CU),						/* Address */
    cpu_be_32(0x0F9F26D2U),							/* Value */
	cpu_be_32(0x400A2004U),						/* Address */
    cpu_be_32(0x00020024U),							/* Value */
	cpu_be_32(0x400A2020U),						/* Address */
    cpu_be_32(0x30B01800U),							/* Value */
	cpu_be_32(0x400A2818U),						/* Address */
    cpu_be_32(0x00000000U),							/* Value */
	cpu_be_32(0x400A201CU),						/* Address */
    cpu_be_32(0x00000000U),							/* Value */
};


struct hab_ivt {
    /** @ref hdr with tag #HAB_TAG_IVT, length and HAB version fields
     *  (see @ref data)
     */
    UINT32 hdr;
    /** Absolute address of the first instruction to execute from the
     *  image
     */
    UINT32 entry;
    /** Reserved in this version of HAB: should be NULL. */
    const UINT32 reserved1;
    /** Absolute address of the image DCD: may be NULL. */
    const UINT32 dcd;
    /** Absolute address of the Boot Data: may be NULL, but not interpreted
     *  any further by HAB
     */
    const UINT32 boot_data;
    /** Absolute address of the IVT.*/
    const UINT32 self;
    /** Absolute address of the image Secure Callback Image.*/
    const UINT32 secure_callback;
    /** Absolute Address of Self Test Image. */
    const UINT32 self_test;
    /** Authentication Length */
    uint32_t auth_len;
    /** Reserved in this version of HAB: should be zero. */
    uint32_t reserved2;
} start_app __attribute__((section(".flash_app_hdr"))) = {
    0x502800d1 , /* hdr */
    0x80000000			,  /* entry */
    0x00000000,            /* reserved1 */
    0x3E801050	,//0x3e801000+0x50, /* dcd */
    0x3e801030, /* boot data */
    0x3e801000, /* self */
    0x0, /*secure_callback*/
    0x0, /*self test*/
    0x0, /*auth len*/
    0x0, /*resv2*/
};

struct BOOT_DATA_T {
    /* Start address of the image */
    UINT32 start;
    /* Size of the image */
    UINT32 size;
    /* Plugin Flag */
    UINT32 plugin;
} boot_data __attribute__((section(".boot_data"))) = {
    //0x3f000000,
    0x80000000,
    0x800000,
    0x00000000 /* boot data */
};




