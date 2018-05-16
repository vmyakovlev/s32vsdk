/*========================================================================*
 * COPYRIGHT:                                                             *
 *  Freescale Semiconductor, INC. All Rights Reserved. You are hereby     *
 *  granted a copyright license to use, modify, and distribute the        *
 *  SOFTWARE so long as this entire notice is retained without alteration *
 *  in any modified and/or redistributed versions, and that such modified *
 *  versions are clearly identified as such. No licenses are granted by   *
 *  implication, estoppel or otherwise under any patentsor trademarks     *
 *  of Freescale Semiconductor, Inc. This software is provided on an      *
 *  "AS IS" basis and without warranty.                                   *
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

/********************************************************************
 * File:		pll.c
 * Purpose:		Board specific routines for the Treerunner
 ********************************************************************/

#include "config.h"
#include "pll.h"
#include "s32vs234.h"

void mode_enter(uint16_t mode);

void pll_init(void){			
    // pokud je CMU zapla tak mi to po startu hodi do safe modu


    ////////////////////////////////////////////////////////
    ///////////////////////// ARM PLL///////////////////////
    ////////////////////////////////////////////////////////
    // PHI 0 output f = 1000 MHz, PHI1 output f = 1000 MHz//
    ////////////////////////////////////////////////////////
    
    // at first set SRC register to select input clk for PLL block: (0 FIRC selected, 1 XOSC selected)
    SRC.GPR1.B.ARM_PLL_SOURCE_SELECT = 1;// XOSC selected as input to ARM_PLL
    
    // configure PLLDIG registers       
    PLLDIG_0.PLLDV.B.RFDPHI1 = 1;	 
    PLLDIG_0.PLLDV.B.RFDPHI = 1;
    PLLDIG_0.PLLDV.B.PREDIV = 1;	// input clock predivider, divided by 1
    PLLDIG_0.PLLDV.B.MFD = 25;	// Loop multiplication factor divider
       //*((vuint32_t*)(0x4003c028)) = 0x02011019;
       
    PLLDIG_0.PLLFD.B.SMDEN = 1;		// sigma-delta modulation enabled
    PLLDIG_0.PLLFD.B.MFN = 0;		// numerator for fractional loop division factor
       //*((vuint32_t*)(0x4003c030)) = 0x70000000;
    
    mode_enter(DRUN_M);
    MC_ME.DRUN_MC.B.ARMPLLON= 1;		//switch on ARM PLL
    mode_enter(DRUN_M);
    
    // DFS settings (DFS0 base adress = 0x4003c40)
    
   DFS_0.DVPORT[0].B.MFN = 194;
   DFS_0.DVPORT[0].B.MFI = 3;   
    //*((vuint32_t*)(0x4003C05C)) = 0x000003C2;     	//    DFS0_DVPORT1: MFN = 194, MFI = 3, fdfs1out = 266
   DFS_0.DVPORT[1].B.MFN = 170;
   DFS_0.DVPORT[1].B.MFI = 1;  
    //*((vuint32_t*)(0x4003C060)) = 0x000001AA;       	//    DFS0_DVPORT2: MFN = 170, MFI = 1, fdfs2out = 600
   DFS_0.DVPORT[2].B.MFN = 170;
   DFS_0.DVPORT[2].B.MFI = 1;  
   // *((vuint32_t*)(0x4003C064)) = 0x000001AA; 		//    DFS0_DVPORT3: MFN = 170, MFI = 1, fdfs3out = 600
   DFS_0.CTRL.R &= (~2);		//    DFS0_CTRL reset is not asserted
    //*((vuint32_t*)(0x4003C058)) &= (~2);	//    DFS0_CTRL reset is not asserted
   DFS_0.PORTRESET.R &= (~7);		//    DFS0_PORTRESET bits 0 - 2 are out of reset
   // *((vuint32_t*)(0x4003C054)) &= (~15);	//    DFS0_PORTRESET bits 0 - 3 are out of reset
   while(DFS_0.PORTSR.R != 0x7);	//DFS0_PORTSR port status register - wait until DVPORT 1-3 are locked, DVPORT 4 not implemented
       //while(*((vuint32_t*)(0x4003C04C)) != 0x7);	//DFS0_PORTSR port status register - wait until DVPORT 1-3 are locked, DVPORT 4 not implemented

       // note: PLL_DFS1 is selected in DRUN_MC in bitfield SYSCLK
       //all other selectors are set in DRUN Secondary Clock Configuration Register (DFS2, DFS3, ARM PLL)
       MC_ME.DRUN_SEC_CC_I.R = 0x00002220;	// system clock selection, ARMPLL outputs selected as input clock in all 4 blocks
       
    mode_enter(DRUN_M);	
    // switch ARM_PLL as sysclk 
    MC_ME.DRUN_MC.B.SYSCLK = 0x0002;	// system clk is ARM_PLL
    mode_enter(DRUN_M);
    
    //////////////////////////////////////////////////////
    ///////////////////////// PERIPH PLL//////////////////
    //////////////////////////////////////////////////////
    // PHI 0 output f = 400 MHz, PHI1 output f = 100 MHz//
    //////////////////////////////////////////////////////
    
    // at first set SRC register to select input clk for PLL block: (0 FIRC selected, 1 XOSC selected)
    SRC.GPR1.B.PERIPH_PLL_SOURCE_SELECT = 1;// XOSC selected as input to PERIPH_PLL
    // configure PLLDIG registers
    PLLDIG_1.PLLDV.B.RFDPHI1 = 12;	
    PLLDIG_1.PLLDV.B.RFDPHI = 3;	 
    PLLDIG_1.PLLDV.B.PREDIV = 1;	// input clock predivider, divided by 1
    PLLDIG_1.PLLDV.B.MFD = 30;	// Loop multiplication factor divider
      //  *((vuint32_t*)(0x4003c0A8)) = 0x1803101E;

    PLLDIG_1.PLLFD.B.SMDEN = 1;		// sigma-delta modulation enabled
    PLLDIG_1.PLLFD.B.MFN = 0;		// numerator for fractional loop division factor
       //*((vuint32_t*)(0x4003c0b0)) = 0x70000000;
    
    // switch on PERIPH_PLL 
    MC_ME.DRUN_MC.B.PERIPHPLLON = 1;		
    mode_enter(DRUN_M);

    // MC_CGM 0 AUX registers
    MC_CGM_0.AC3_DC0.B.DE = 0;		// need to switch to 0 to avoid clock glitch
    MC_CGM_0.AC3_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is LIN_CLK = 133 MHz
    MC_CGM_0.AC3_SC.B.SELCTL = 8;	// AC3 clock source selection - SYS6_CLK 133 MHz 
    MC_CGM_0.AC3_DC0.B.DE = 1;			//divider enable(1)/disable(0)

    MC_CGM_0.AC5_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_0.AC5_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is PERI_CLK = 80 MHz    
    MC_CGM_0.AC5_DC1.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_0.AC5_DC1.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is FRAY_PLL_CLK = 80 MHz
    MC_CGM_0.AC5_SC.B.SELCTL = 3;	// AC5 clock source selection - PERIPH_PLL_PHI0/5     
    MC_CGM_0.AC5_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    MC_CGM_0.AC5_DC1.B.DE = 1;			//divider enable(1)/disable(0)

    MC_CGM_0.AC6_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_0.AC6_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is CAN_CLK = 80 MHz
    MC_CGM_0.AC6_SC.B.SELCTL = 3;	// AC6 clock source selection - PERIPH_PLL_PHI0/5
    MC_CGM_0.AC6_DC0.B.DE = 1;			//divider enable(1)/disable(0)


    // MC_CGM 2 AUX registers	
    MC_CGM_2.AC0_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_2.AC0_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is DIGRF_PLL_CLK = 26 MHz
    MC_CGM_2.AC0_SC.B.SELCTL = 7;	// AC0 clock source selection - external pad clock source 26 MHz 
    MC_CGM_2.AC0_DC0.B.DE = 1;			//divider enable(1)/disable(0)

    mode_enter(DRUN_M);
    
    //////////////////////////////////////////////////////
    ///////////////////////// ENET PLL////////////////////
    //////////////////////////////////////////////////////
    // PHI 0 output f = 500 MHz, PHI1 output f = 1000 MHz//
    //////////////////////////////////////////////////////
    
    // at first set SRC register to select input clk for PLL block: (0 FIRC selected, 1 XOSC selected)
    SRC.GPR1.B.ENET_PLL_SOURCE_SELECT = 1;// XOSC selected as input to ENET_PLL

    // configure PLLDIG registers    
    PLLDIG_2.PLLDV.B.RFDPHI1 = 1;	
    PLLDIG_2.PLLDV.B.RFDPHI = 2;	
    PLLDIG_2.PLLDV.B.PREDIV = 1;	// input clock predivider, divided by 1
    PLLDIG_2.PLLDV.B.MFD = 25;	// Loop multiplication factor divider
       // *((vuint32_t*)(0x4003c128)) = 0x02021019;      

    PLLDIG_2.PLLFD.B.SMDEN = 1;		// sigma-delta modulation enabled
    PLLDIG_2.PLLFD.B.MFN = 0;	// numerator for fractional loop division factor
     //   *((vuint32_t*)(0x4003c130)) = 0x70000000; 	

    // switch on ENET_PLL
    MC_ME.DRUN_MC.B.ENETPLLON= 1;		//switch on ENET PLL
    mode_enter(DRUN_M);
    
    // DFS settings
    DFS_1.DVPORT[0].B.MFN = 219;
    DFS_1.DVPORT[0].B.MFI = 2;   
    //*((vuint32_t*)(0x4003c15c)) = 0x000002DB;     	//    DFS2_DVPORT1: MFN = 219, MFI = 2, fdfs1out = 350
    DFS_1.DVPORT[1].B.MFN = 219;
    DFS_1.DVPORT[1].B.MFI = 2;       
    //*((vuint32_t*)(0x4003c160)) = 0x000002DB;       	//    DFS2_DVPORT2: MFN = 219, MFI = 2, fdfs2out = 350
    DFS_1.DVPORT[2].B.MFN = 32;
    DFS_1.DVPORT[2].B.MFI = 3;   
    //*((vuint32_t*)(0x4003c164)) = 0x00000320; 		//    DFS2_DVPORT3: MFN = 32, MFI = 3, fdfs3out = 320
    DFS_1.DVPORT[3].B.MFN = 156;
    DFS_1.DVPORT[3].B.MFI = 9;   
    //*((vuint32_t*)(0x4003c168)) = 0x0000099C;		//    DFS2_DVPORT4: MFN = 156, MFI = 9 fdfs4out = 104
    DFS_1.CTRL.R &= (~2);		//    DFS0_CTRL reset is not asserted
    //*((vuint32_t*)(0x4003c158)) &= (~2);		//    DFS2_CTRL reset is not asserted
    DFS_1.PORTRESET.R &= (~15);		//    DFS0_PORTRESET bits 0 - 3 are out of reset
    //*((vuint32_t*)(0x4003c154)) &= (~15);		//    DFS2_PORTRESET bits 0 - 3 are out of reset

       while(DFS_1.PORTSR.R != 0xF);	//DFS0_PORTSR port status register - wait until DVPORT 1-4 are locked 

 // MC_CGM 0 AUX registers
    MC_CGM_0.AC7_DC1.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_0.AC7_DC1.B.DIV = 3;			//divided by 4 (1 + DIV) - resultant output clk is ENET_TIME_CLK = 125 MHz
    MC_CGM_0.AC7_SC.B.SELCTL = 4;	// AC7 clock source selection - ENET_PLL_PHI0
    MC_CGM_0.AC7_DC1.B.DE = 1;			//divider enable(1)/disable(0)

    MC_CGM_0.AC12_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_0.AC12_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is H264_DEC_CLK = 350 MHz
    MC_CGM_0.AC12_SC.B.SELCTL = 4;	// AC12 clock source selection - ENET_PLL_DFS1
    MC_CGM_0.AC12_DC0.B.DE = 1;			//divider enable(1)/disable(0)

    MC_CGM_0.AC13_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_0.AC13_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is H264_ENC_CLK = 350 MHz
    MC_CGM_0.AC13_SC.B.SELCTL = 4;	// AC13 clock source selection - ENET_PLL_DFS2
    MC_CGM_0.AC13_DC0.B.DE = 1;			//divider enable(1)/disable(0)

    MC_CGM_0.AC14_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_0.AC14_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is QSPI_CLK = 320 MHz
    MC_CGM_0.AC14_SC.B.SELCTL = 4;	// AC14 clock source selection - ENET_PLL_DFS3 
    MC_CGM_0.AC14_DC0.B.DE = 1;			//divider enable(1)/disable(0)

    MC_CGM_0.AC15_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_0.AC15_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is H264_DEC_CLK = 104 MHz
    MC_CGM_0.AC15_SC.B.SELCTL = 4;	// AC15 clock source selection - ENET_PLL_DFS4 
    MC_CGM_0.AC15_DC0.B.DE = 1;			//divider enable(1)/disable(0)
	

	
/* 	// MC_CGM 2 AUX registers
 - 	MC_CGM_2.AC1_SC.B.SELCTL = 4;	// AC1 clock source selection - ENET_PLL_PHI0*/
        //*((vuint32_t*)(0x40042820)) = 0x04000000;
	
/*	MC_CGM_2.AC1_DC0.B.DE = 1;			//divider enable(1)/disable(0)
 - 	MC_CGM_2.AC1_DC0.B.DIV = 19;			//divided by 20 (1 + DIV) - resultant output clk is MIPI_REF_CLK = 25 MHz*/
       // *((vuint32_t*)(0x40042828)) = 0x80190000;
    
    MC_CGM_2.AC2_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_2.AC2_DC0.B.DIV = 3;			//divided by 4 (1 + DIV) - resultant output clk is ENET_CLK = 125 MHz
    MC_CGM_2.AC2_SC.B.SELCTL = 4;	// AC2 clock source selection - ENET_PLL_PHI0
    MC_CGM_2.AC2_DC0.B.DE = 1;	


    mode_enter(DRUN_M);
    
    ///////////////////////////////////////////////////////
    ///////////////////////// DDR PLL//////////////////////
    ///////////////////////////////////////////////////////
    // PHI 0 output f = 533 MHz, PHI1 output f = 1066 MHz//
    ///////////////////////////////////////////////////////
    
    // at first set SRC register to select input clk for PLL block: (0 FIRC selected, 1 XOSC selected)
    SRC.GPR1.B.DDR_PLL_SOURCE_SELECT = 1;// XOSC selected as input to DDR_PLL

      // configure PLLDIG registers

          
    PLLDIG_3.PLLDV.B.RFDPHI1 = 1;	
    PLLDIG_3.PLLDV.B.RFDPHI = 2;	
    PLLDIG_3.PLLDV.B.PREDIV = 1;	// input clock predivider, divided by 1
    PLLDIG_3.PLLDV.B.MFD = 26;	// Loop multiplication factor divider
       //*((vuint32_t*)(0x4003C1A8)) = 0x0202101A;      

    PLLDIG_3.PLLFD.B.SMDEN = 1;		// sigma-delta modulation enabled
    PLLDIG_3.PLLFD.B.MFN = 13312;	// numerator for fractional loop division factor
       //*((vuint32_t*)(0x4003c1b0)) = 0x70003400;         //nesedi uplne presne - frekvence je 1053 MHz a ne 1066

    // switch on DDR_PLL 
    MC_ME.DRUN_MC.B.DDRPLLON = 1;		
    mode_enter(DRUN_M);
    
    // DFS settings
    DFS_2.DVPORT[0].B.MFN = 33;
    DFS_2.DVPORT[0].B.MFI = 2;   
   // *((vuint32_t*)(0x4003C1DC)) = 0x00000221;     	//    DFS3_DVPORT1: MFN = 33, MFI = 2, fdfs1out = 500
    DFS_2.DVPORT[1].B.MFN = 33;
    DFS_2.DVPORT[1].B.MFI = 2;  
    //*((vuint32_t*)(0x4003c1e0)) = 0x00000221;       	//    DFS3_DVPORT2: MFN = 33, MFI = 2, fdfs2out = 500
    DFS_2.DVPORT[2].B.MFN = 11;
    DFS_2.DVPORT[2].B.MFI = 3;  
   // *((vuint32_t*)(0x4003c1e4)) = 0x0000030b; 	//    DFS3_DVPORT3: MFN1 = 11, MFI1 = 3, fdfs3out = 350
    DFS_2.CTRL.R &= (~2);		//    DFS0_CTRL reset is not asserted
    //*((vuint32_t*)(0x4003c1d8)) &= (~2);	//    DFS3_CTRL reset is not asserted
    DFS_2.PORTRESET.R &= (~7);		//    DFS0_PORTRESET bits 0 - 2 are out of reset    
    //*((vuint32_t*)(0x4003c1d4)) &= (~7);	//    DFS3_PORTRESET bits 0 - 3 are out of reset

       while(DFS_2.PORTSR.R != 0x7);	//DFS0_PORTSR port status register - wait until DVPORT 1-3 are locked, port 4 is not 

	// MC_CGM 0 AUX registers
    MC_CGM_0.AC0_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_0.AC0_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is ISP_CLK = 500 MHz
    MC_CGM_0.AC0_SC.B.SELCTL = 5;	// AC0 clock source selection - DDR_PLL_DFS1
    MC_CGM_0.AC0_DC0.B.DE = 1;			//divider enable(1)/disable(0)
  
    MC_CGM_0.AC1_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_0.AC1_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is APEX_APU_CLK = 500 MHz
    MC_CGM_0.AC1_SC.B.SELCTL = 5;	// AC1 clock source selection - DDR_PLL_DFS2
    MC_CGM_0.AC1_DC0.B.DE = 1;			//divider enable(1)/disable(0)
	
    MC_CGM_0.AC2_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_0.AC2_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is MJEPEG_CLK = 500 MHz
    MC_CGM_0.AC2_SC.B.SELCTL = 5;	// AC2 clock source selection - DDR_PLL_DFS3
    MC_CGM_0.AC2_DC0.B.DE = 1;			//divider enable(1)/disable(0)
	
    MC_CGM_0.AC8_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_0.AC8_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is DDR_CLK = 533 MHz
    MC_CGM_0.AC8_DC1.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_0.AC8_DC1.B.DIV = 3;			//divided by 4 (1 + DIV) - resultant output clk is DDR4_CLK = 133,25 MHz
    MC_CGM_0.AC8_SC.B.SELCTL = 5;	// AC8 clock source selection - DDR_PLL_PHI0
    MC_CGM_0.AC8_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    MC_CGM_0.AC8_DC1.B.DE = 1;			//divider enable(1)/disable(0)	    
	

    mode_enter(DRUN_M);
    
    ////////////////////////////////////////////////////
    ///////////////////////// VIDEO PLL/////////////////
    ////////////////////////////////////////////////////
    //////////// PHI 0 output f = 600 MHz///////////////
    ////////////////////////////////////////////////////
    
    // at first set SRC register to select input clk for PLL block: (0 FIRC selected, 1 XOSC selected)
    SRC.GPR1.B.VIDEO_PLL_SOURCE_SELECT = 1;// XOSC selected as input to VIDEO_PLL

    // configure PLLDIG registers   
    PLLDIG_4.PLLDV.B.RFDPHI1 = 0;	// PHI1 output NA
    PLLDIG_4.PLLDV.B.RFDPHI = 2;
    PLLDIG_4.PLLDV.B.PREDIV = 1;	// input clock predivider, divided by 1
    PLLDIG_4.PLLDV.B.MFD = 30;	// Loop multiplication factor divider
       //*((vuint32_t*)(0x4003c228)) = 0x0002101e;
       
       PLLDIG_4.PLLFD.B.SMDEN = 1;		// sigma-delta modulation enabled
       PLLDIG_4.PLLFD.B.MFN = 0;	//FRCDIV value - numerator for fractional loop division factor
        //*((vuint32_t*)(0x4003c230)) = 0x70000000;

    // switch on VIDEO_PLL 
    MC_ME.DRUN_MC.B.VIDEOPLLON = 1;		//switch on ARM PLL
    mode_enter(DRUN_M);

	// MC_CGM 0 AUX registers
    MC_CGM_0.AC9_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_0.AC9_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is DCU_AXI_CLK = 300 MHz
    MC_CGM_0.AC9_DC1.B.DE = 0;			//divider enable(1)/disable(0)
#ifdef HDMI_RES_1280X1024
    MC_CGM_0.AC9_DC1.B.DIV = 2;			//divided by 3 (1 + DIV) - resultant output clk is DCU_PIX_CLK = 100 MHz
#else
    MC_CGM_0.AC9_DC1.B.DIV = 1;			//divided by 2 (1 + DIV) - resultant output clk is DCU_PIX_CLK = 150 MHz
#endif
    MC_CGM_0.AC9_SC.B.SELCTL = 9;	// AC9 clock source selection - VIDEO_PLL/2
    MC_CGM_0.AC9_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    MC_CGM_0.AC9_DC1.B.DE = 1;			//divider enable(1)/disable(0)
	
	// MC_CGM 2 AUX registers
    MC_CGM_2.AC4_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_2.AC4_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is TRACECLK = 300 MHz
    MC_CGM_2.AC4_SC.B.SELCTL = 9;	// AC4 clock source selection - VIDEO_PLL/2
    MC_CGM_2.AC4_DC0.B.DE = 1;			//divider enable(1)/disable(0)	
	

    mode_enter(DRUN_M);
    
	// remaining AUX registers
    
	// MC_CGM 0 AUX registers
    MC_CGM_0.AC4_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_0.AC4_DC0.B.DIV = 3;			//divided by 4 (1 + DIV) - resultant output clk is FLEXTIMER0_EXT_CLK = 20 MHz
    MC_CGM_0.AC4_SC.B.SELCTL = 0xA;	// AC4 clock source selection - PERI_CLK 80 MHz
    MC_CGM_0.AC4_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    
    MC_CGM_0.AC10_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_0.AC10_DC0.B.DIV = 7;			//divided by 8 (1 + DIV) - resultant output clk is CLK_OUT = 20 MHz
    MC_CGM_0.AC10_DC1.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_0.AC10_DC1.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is LBIST_FAST_CLK = 400 MHz
    MC_CGM_0.AC10_SC.B.SELCTL = 3;	// AC10 clock source selection - PERIPH_PLL_PHI0 400 MHz
    MC_CGM_0.AC10_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    MC_CGM_0.AC10_DC1.B.DE = 1;			//divider enable(1)/disable(0)

    MC_CGM_0.AC11_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_0.AC11_DC0.B.DIV = 0;		//divided by 1 (1 + DIV) - resultant output clk is DDR_32KHZ_CLK = 32 kHz
    MC_CGM_0.AC11_SC.B.SELCTL = 1;	// AC11 clock source selection - XOSC_32KHz
    MC_CGM_0.AC11_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    
	// MC_CGM 2 AUX registers
    MC_CGM_2.AC3_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    MC_CGM_2.AC3_DC0.B.DIV = 3;			//divided by 4 (1 + DIV) - resultant output clk is FLEXTIMER1_EXT_CLK = 20 MHz
    MC_CGM_2.AC3_SC.B.SELCTL = 10;	// AC4 clock source selection - PERI_CLK 80 MHz
    MC_CGM_2.AC3_DC0.B.DE = 1;			//divider enable(1)/disable(0)

	    mode_enter(DRUN_M);
}

void clkout_settings (uint32_t clkout_value){
  /*  uint32_t clk_in = 400;
    uint32_t clkdiv;
     
    SIUL2.MSCR[39].B.OBE = 0x1;		// output buffer enable
    SIUL2.MSCR[39].B.SSS = 0x2;		// source signal select
  
    clkdiv = (clk_in / clkout_value) - 1;
 	MC_CGM_0.AC10_DC0.B.DE = 1;			//divider enable(1)/disable(0)
  	MC_CGM_0.AC10_DC0.B.DIV = 19;			//divided by 20 (1 + DIV) - resultant output clk is CLK_OUT = 20 MHz$*@/@
      
    *((vuint32_t*)(0x4003C948)) &= ~(0xf << 16);
    *((vuint32_t*)(0x4003C948)) |= clkdiv;*/
}
