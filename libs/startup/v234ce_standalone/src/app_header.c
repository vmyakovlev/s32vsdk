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
#define HAB_HDR(tag, len, par) {(tag), {(len)>>HAB_BITS_IN_BYTE, (len)}, (par)}
#define DCD_HEADER(x)	HAB_HDR(HAB_TAG_DCD,(sizeof(x)+sizeof(hab_hdr_t)),HAB_TAG_DCD_VER)
#define MY_DCD_HEADER(x)	HAB_HDR(HAB_TAG_DCD,((x)+sizeof(hab_hdr_t)),HAB_TAG_DCD_VER)

#define MC_ME_S_MTRANS_BIT             ((27)  )
#define MC_ME_S_MTRANS                 (((1) << (MC_ME_S_MTRANS_BIT)) )

typedef struct hab_hdr {
    uint8_t tag;              /**< Tag field */
    uint8_t len[2];           /**< Length field in bytes (big-endian) */
    uint8_t par;              /**< Parameters field */
} hab_hdr_t;


const uint32_t dcd[] __attribute__((section(".dcd"))) = {


    ((HAB_TAG_DCD )| (0x0F<<8) | (0xAC<<16) | HAB_TAG_DCD_VER <<24),
    //((HAB_TAG_DCD )| (0x0E<<8) | (0x74<<16) | HAB_TAG_DCD_VER <<24),

//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012380),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012384),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012480),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012484),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012580),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012584),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012680),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012684),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012780),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012784),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012880),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012884),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012980),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012984),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012A80),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012A84),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012B80),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012B84),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012C80),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012C84),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012D80),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012D84),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012E80),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012E84),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012F80),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */
// 
//     ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
//     cpu_be_32(0x40012F84),						/* Address */
//     cpu_be_32(0x00000000),						/* Value */

      
    
    // Turn off CMU
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x400c4000),						/* Address */
    cpu_be_32(0x00000006),						/* Value */

    // Enable all modes
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4004a008),						/* Address */
    cpu_be_32(0x000000FF),						/* Value */

    // Enable all periferals
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4004a09c/*4004a09c*/),			/* Address */
    cpu_be_32(0x000000FF),						/* Value */

    // Enable all periferals
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c280),			/* Address */
    cpu_be_32(0x01A02070),						/* Value */

    // XTAL ON, sysclk=XTAL clk in DRUN
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4004a02c),						/* Address */
    cpu_be_32(0x00100030),						/* Value */

    /////////////////////////////////////////////
    // Mode entry (DRUN)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004),						/* Address */
    cpu_be_32(0x30005AF0),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004), 					 /* Address */
    cpu_be_32(0x3000A50F),	

    ((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000),						/*@/@*$ Address $*@/@*/
    cpu_be_32(MC_ME_S_MTRANS),
    /////////////////////////////////////////////

    //Turn off Watchdog SWT4
    ((HAB_CMD_NOP)|(4<<16)),     /* NOP DCD Header */
    ((HAB_CMD_WRT_DAT )|(0x14<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),              
    cpu_be_32(0x40086010),   /* Address */
    cpu_be_32(0xd928c520),   /* Value */
    cpu_be_32(0x40086000),   /* Address */
    cpu_be_32(0xff000000),   /* Value */ 
	

    // XTAL ON, sysclk=XTAL clk in DRUN
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4004a02c),						/* Address */
    cpu_be_32(0x00100031),						/* Value */

    // XOSC as system clock1, 2 and 3
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4004a260),						/* Address */
    cpu_be_32(0x1110),						/* Value */

    /////////////////////////////////////////////
    // Mode entry (DRUN)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004),						/* Address */
    cpu_be_32(0x30005AF0),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004), 					 /* Address */
    cpu_be_32(0x3000A50F),	

    ((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000),						/* Address*/
    cpu_be_32(MC_ME_S_MTRANS), 							
    /////////////////////////////////////////////

    ////////////////////////////////////////////////////////
    ///////////////////////// ARM PLL///////////////////////
    ////////////////////////////////////////////////////////
    // PHI 0 output f = 1000 MHz, PHI1 output f = 1000 MHz//
    ////////////////////////////////////////////////////////

    // DFS_0.CTRL.R = 0x2;
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c058),						/* Address */
    cpu_be_32(0x2),						/* Value */

    // DFS_0.PORTRESET.R = 0xffffffff;
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c054),						/* Address */
    cpu_be_32(0xFFFFFFFF),						/* Value */

    // at first set SRC register to select input clk for PLL block: (0 FIRC selected, 1 XOSC selected)
    //SRC.GPR1.B.ARM_PLL_SOURCE_SELECT = 1;// XOSC selected as input to ARM_PLL
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4007c100),						/* Address */
    cpu_be_32(0x08000000),						/* Value */

    // configure PLLDIG registers       
    //PLLDIG_0.PLLDV.B.RFDPHI1 = 1;	 
    //PLLDIG_0.PLLDV.B.RFDPHI = 1;
    //PLLDIG_0.PLLDV.B.PREDIV = 1;	// input clock predivider, divided by 1
    //PLLDIG_0.PLLDV.B.MFD = 25;	// Loop multiplication factor divider
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c028),						/* Address */
    cpu_be_32(0x02011019),						/* Value */

    //PLLDIG_0.PLLFD.B.SMDEN = 1;		// sigma-delta modulation enabled
    //PLLDIG_0.PLLFD.B.MFN = 0;		// numerator for fractional loop division factor
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c030),						/* Address */
    cpu_be_32(0x40000000),						/* Value */


    // Switch on ARM pll in DRUN
    //MC_ME.DRUN_MC.B.ARMPLLON= 1;		//switch on ARM PLL
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4004a02c),						/* Address */
    cpu_be_32(0x00100071),						/* Value */

    /////////////////////////////////////////////
    // Mode entry (DRUN)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004),						/* Address */
    cpu_be_32(0x30005AF0),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004), 					 /* Address */
    cpu_be_32(0x3000A50F),	

    ((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000),						/* Address*/
    cpu_be_32(MC_ME_S_MTRANS), 							
    /////////////////////////////////////////////

    // DFS settings (DFS0 base adress = 0x4003c40)

    //DFS_0.DFS_DLLPRG1.R =0x00005445;
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c040),						/* Address */
    cpu_be_32(0x00005445),						/* Value */

    //DFS_0.DLLPRG2.R =0x0000;
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c044),						/* Address */
    cpu_be_32(0x0),						/* Value */

    //    DFS0_DVPORT1: MFN = 194, MFI = 3, fdfs1out = 266 
    //DFS_0.DVPORT[0].B.MFN = 194;
    //DFS_0.DVPORT[0].B.MFI = 3;   
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003C05C),						/* Address */
    cpu_be_32(0x000003C2),						/* Value */ 

    //    DFS0_DVPORT2: MFN = 170, MFI = 1, fdfs2out = 600
    //DFS_0.DVPORT[1].B.MFN = 170;
    //DFS_0.DVPORT[1].B.MFI = 1;  
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003C060),						/* Address */
    cpu_be_32(0x000001AA),						/* Value */ 

    //    DFS0_DVPORT3: MFN = 170, MFI = 1, fdfs3out = 600
    //DFS_0.DVPORT[2].B.MFN = 170;
    //DFS_0.DVPORT[2].B.MFI = 1;  
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003C064),						/* Address */
    cpu_be_32(0x000001AA),						/* Value */ 

    //DFS_0.CTRL.R &= 0xfffffffD;		//    DFS0_CTRL reset is not asserted
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,1,0)) <<24),	
    cpu_be_32(0x4003C058),						/* Address */
    cpu_be_32(0x00000002),						/* Value */

    //DFS_0.PORTRESET.R &= 0xfffffff0;		//    DFS0_PORTRESET bits 0 - 2 are out of reset
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,1,0)) <<24),	
    cpu_be_32(0x4003C054),						/* Address */
    cpu_be_32(0x0000000F),						/* Value */

    //while(DFS_0.PORTSR.R != 0xF);	//DFS0_PORTSR port status register - wait until DVPORT 1-3 are locked, DVPORT 4 not implemented
    ((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,1)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4003C04C),						/* Address*/
    cpu_be_32(0x0000000F),

    // note: PLL_DFS1 is selected in DRUN_MC in bitfield SYSCLK
    //all other selectors are set in DRUN Secondary Clock Configuration Register (DFS2, DFS3, ARM PLL)
    //    MC_ME.DRUN_SEC_CC_I.R = 0x00002220;	// system clock selection, ARMPLL outputs selected as input clock in all 4 blocks
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4004a260),						/* Address */
    cpu_be_32(0x2220),	    

    //MC_ME.DRUN_MC.B.ARMPLLON = 1; switch ARM_PLL as sysclk 
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4004a02c),						/* Address */
    cpu_be_32(0x00100072),

    //mode_enter(DRUN_M);
    // Mode entry (DRUN) ,check ARM PLL on, sysclk = ARM PLL
    //while(MC_ME.GS.B.S_ARMPLL != 1);
    //while(MC_ME.GS.B.S_SYSCLK != 2);
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004),						/* Address */
    cpu_be_32(0x30005AF0),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004), 					 /* Address */
    cpu_be_32(0x3000A50F),	

    ((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,1)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000),						/* Address*/       
    cpu_be_32(0x30100072),  

    //////////////////////////////////////////////////////
    ///////////////////////// PERIPH PLL//////////////////
    //////////////////////////////////////////////////////
    // PHI 0 output f = 400 MHz, PHI1 output f = 100 MHz//
    //////////////////////////////////////////////////////

    // at first set SRC register to select input clk for PLL block: (0 FIRC selected, 1 XOSC selected)
    //SRC.GPR1.B.PERIPH_PLL_SOURCE_SELECT = 1;// XOSC selected as input to PERIPH_PLL
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4007c100),						/* Address */
    cpu_be_32(0x48000000),						/* Value */

    // configure PLLDIG registers
    // PLLDIG_1.PLLDV.B.RFDPHI1 = 12;	
    // PLLDIG_1.PLLDV.B.RFDPHI = 3;	 
    // PLLDIG_1.PLLDV.B.PREDIV = 1;	// input clock predivider, divided by 1
    // PLLDIG_1.PLLDV.B.MFD = 30;	// Loop multiplication factor divider
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c0A8),						/* Address */
    cpu_be_32(0x1803101E),						/* Value */

    // PLLDIG_1.PLLFD.B.SMDEN = 1;		// sigma-delta modulation enabled
    // PLLDIG_1.PLLFD.B.MFN = 0;		// numerator for fractional loop division factor
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c0b0),						/* Address */
    cpu_be_32(0x40000000),						/* Value */

    // switch on PERIPH_PLL 
    //MC_ME.DRUN_MC.B.PERIPHPLLON = 1;		
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4004a02c),						/* Address */
    cpu_be_32(0x001000f2),						/* Value */

    //mode_enter(DRUN_M);
    // Mode entry (DRUN) ,check ARM PLL on, PeriPLL on, sysclk = ARM PLL
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004),						/* Address */
    cpu_be_32(0x30005AF0),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004), 					 /* Address */
    cpu_be_32(0x3000A50F),	

    ((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,1)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000),						/* Address*/       
    cpu_be_32(0x301000f2), 

    // MC_CGM 0 AUX registers
    //MC_CGM_0.AC3_DC0.B.DE = 0;		// need to switch to 0 to avoid clock glitch
    //MC_CGM_0.AC3_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is LIN_CLK = 133 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c868),						/* Address */
    cpu_be_32(0x0),						/* Value */  
    // MC_CGM_0.AC3_SC.B.SELCTL = 8;	// AC3 clock source selection - SYS6_CLK 133 MHz 
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c860),						/* Address */
    cpu_be_32(0x08000000),						/* Value */
    // MC_CGM_0.AC3_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c868),						/* Address */
    cpu_be_32(0x80000000),						/* Value */

    // MC_CGM_0.AC5_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    // MC_CGM_0.AC5_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is PERI_CLK = 80 MHz    
    // MC_CGM_0.AC5_DC1.B.DE = 0;			//divider enable(1)/disable(0)
    // MC_CGM_0.AC5_DC1.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is FRAY_PLL_CLK = 80 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c8a8),						/* Address */
    cpu_be_32(0x0),						/* Value */
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	   
    cpu_be_32(0x4003c8ac),						/* Address */
    cpu_be_32(0x0),						/* Value */

    // MC_CGM_0.AC5_SC.B.SELCTL = 3;	// AC5 clock source selection - PERIPH_PLL_PHI0/5 
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c8a0),						/* Address */
    cpu_be_32(0x03000000),						/* Value */

    // MC_CGM_0.AC5_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    // MC_CGM_0.AC5_DC1.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c8a8),						/* Address */
    cpu_be_32(0x80000000),						/* Value */
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c8ac),						/* Address */
    cpu_be_32(0x80000000),						/* Value */

    // MC_CGM_0.AC6_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    // MC_CGM_0.AC6_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is CAN_CLK = 80 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c8c8),						/* Address */
    cpu_be_32(0x0),						/* Value */

    // MC_CGM_0.AC6_SC.B.SELCTL = 3;	// AC6 clock source selection - PERIPH_PLL_PHI0/5
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c8c0),						/* Address */
    cpu_be_32(0x03000000),						/* Value */

    // MC_CGM_0.AC6_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c8c8),						/* Address */
    cpu_be_32(0x80000000),						/* Value */


    // MC_CGM 2 AUX registers	
    // MC_CGM_2.AC0_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    // MC_CGM_2.AC0_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is DIGRF_PLL_CLK = 26 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x40042808),						/* Address */
    cpu_be_32(0x0),						/* Value */

    // MC_CGM_2.AC0_SC.B.SELCTL = 7;	// AC0 clock source selection - external pad clock source 26 MHz
    // MC_CGM_0.AC6_SC.B.SELCTL = 3;	// AC6 clock source selection - PERIPH_PLL_PHI0/5
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x40042800),						/* Address */
    cpu_be_32(0x07000000),						/* Value */ 
    // MC_CGM_2.AC0_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x40042808),						/* Address */
    cpu_be_32(0x80000000),						/* Value */

    // mode_enter(DRUN_M);
    // Mode entry (DRUN)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004),						/* Address */
    cpu_be_32(0x30005AF0),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004), 					 /* Address */
    cpu_be_32(0x3000A50F),	

    ((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000),						/* Address*/
    cpu_be_32(MC_ME_S_MTRANS), 

    //////////////////////////////////////////////////////
    ///////////////////////// ENET PLL////////////////////
    //////////////////////////////////////////////////////
    // PHI 0 output f = 500 MHz, PHI1 output f = 1000 MHz//
    //////////////////////////////////////////////////////

    // at first set SRC register to select input clk for PLL block: (0 FIRC selected, 1 XOSC selected)
    //SRC.GPR1.B.ENET_PLL_SOURCE_SELECT = 1;// XOSC selected as input to ENET_PLL
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4007c100),						/* Address */
    cpu_be_32(0x58000000),						/* Value */

    // configure PLLDIG registers    
    // PLLDIG_2.PLLDV.B.RFDPHI1 = 1;	
    //  PLLDIG_2.PLLDV.B.RFDPHI = 2;	
    // PLLDIG_2.PLLDV.B.PREDIV = 1;	// input clock predivider, divided by 1
    // PLLDIG_2.PLLDV.B.MFD = 25;	// Loop multiplication factor divider
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c128),						/* Address */
    cpu_be_32(0x02021019),						/* Value */

    //  PLLDIG_2.PLLFD.B.SMDEN = 1;		// sigma-delta modulation enabled
    //   PLLDIG_2.PLLFD.B.MFN = 0;	// numerator for fractional loop division factor
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c130),						/* Address */
    cpu_be_32(0x40000000),						/* Value */


    // switch on ENET_PLL
    // MC_ME.DRUN_MC.B.ENETPLLON= 1;		//switch on ENET PLL
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4004a02c),						/* Address */
    cpu_be_32(0x001001f2),						/* Value */


    //mode_enter(DRUN_M);
    // Mode entry (DRUN) ,check ARM PLL on, PeriPLL on, ENETPLL on, sysclk = ARM PLL
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004),						/* Address */
    cpu_be_32(0x30005AF0),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004), 					 /* Address */
    cpu_be_32(0x3000A50F),	

    ((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,1)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000),						/* Address*/       
    cpu_be_32(0x301001f2), 


    //DFS_1.DFS_DLLPRG1.R =0x00005445;
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4003c140), 					 /* Address */
    cpu_be_32(0x00005445),
    //DFS_1.DLLPRG2.R =0x0000;
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4003c144), 					 /* Address */
    cpu_be_32(0x0),

    // DFS settings
    // DFS_1.DVPORT[0].B.MFN = 219;
    // DFS_1.DVPORT[0].B.MFI = 2;   
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4003c15c), 					 /* Address */
    cpu_be_32(0x000002DB),

    // DFS_1.DVPORT[1].B.MFN = 219;
    // DFS_1.DVPORT[1].B.MFI = 2; 
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4003c160), 					 /* Address */
    cpu_be_32(0x000002DB),      

    // DFS_1.DVPORT[2].B.MFN = 32;
    // DFS_1.DVPORT[2].B.MFI = 3;
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4003c164), 					 /* Address */
    cpu_be_32(0x00000320),   

    // DFS_1.DVPORT[3].B.MFN = 156;
    // DFS_1.DVPORT[3].B.MFI = 9;  
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4003c168), 					 /* Address */
    cpu_be_32(0x0000099C), 

    // DFS_1.CTRL.R &= (~2);		//    DFS0_CTRL reset is not asserted
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4003c158), 					 /* Address */
    cpu_be_32(0x00000000),


    // DFS_1.PORTRESET.R &= (~15);		//    DFS0_PORTRESET bits 0 - 3 are out of reset
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4003c154), 					 /* Address */
    cpu_be_32(0x00000000),
    //*((vuint32_t*)(0x4003c154)) &= (~15);		//    DFS2_PORTRESET bits 0 - 3 are out of reset

    // while(DFS_1.PORTSR.R != 0xF);	//DFS0_PORTSR port status register - wait until DVPORT 1-4 are locked 
    ((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,1)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4003c14c),						/* Address*/
    cpu_be_32(0x0000000F),

    // MC_CGM 0 AUX registers
    //MC_CGM_0.AC7_DC1.B.DE = 0;			//divider enable(1)/disable(0)
    //MC_CGM_0.AC7_DC1.B.DIV = 3;			//divided by 4 (1 + DIV) - resultant output clk is ENET_TIME_CLK = 125 MHz  
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c8ec),						/* Address */
    cpu_be_32(0x00030000),						/* Value */   
    //   MC_CGM_0.AC7_SC.B.SELCTL = 4;	// AC7 clock source selection - ENET_PLL_PHI0
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c8e0),						/* Address */
    cpu_be_32(0x04000000),						/* Value */  
    //  MC_CGM_0.AC7_DC1.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c8ec),						/* Address */
    cpu_be_32(0x80030000),						/* Value */ 

    // MC_CGM_0.AC12_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    // MC_CGM_0.AC12_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is H264_DEC_CLK = 350 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c988),						/* Address */
    cpu_be_32(0x0),						/* Value */
    // MC_CGM_0.AC12_SC.B.SELCTL = 4;	// AC12 clock source selection - ENET_PLL_DFS1
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c980),						/* Address */
    cpu_be_32(0x04000000),						/* Value */  
    //MC_CGM_0.AC12_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c988),						/* Address */
    cpu_be_32(0x80000000),						/* Value */ 

    // MC_CGM_0.AC13_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    // MC_CGM_0.AC13_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is H264_ENC_CLK = 350 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c9A8),						/* Address */
    cpu_be_32(0x0),						/* Value */
    // MC_CGM_0.AC13_SC.B.SELCTL = 4;	// AC13 clock source selection - ENET_PLL_DFS2
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c9A0),						/* Address */
    cpu_be_32(0x04000000),						/* Value */  
    // MC_CGM_0.AC13_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c9A8),						/* Address */
    cpu_be_32(0x80000000),						/* Value */ 

    // MC_CGM_0.AC14_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    // MC_CGM_0.AC14_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is QSPI_CLK = 320 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c9C8),						/* Address */
    cpu_be_32(0x0),						/* Value */
    // MC_CGM_0.AC14_SC.B.SELCTL = 4;	// AC14 clock source selection - ENET_PLL_DFS3
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c9C0),						/* Address */
    cpu_be_32(0x04000000),						/* Value */  
    // MC_CGM_0.AC14_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c9C8),						/* Address */
    cpu_be_32(0x80000000),						/* Value */ 

    // MC_CGM_0.AC15_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    // MC_CGM_0.AC15_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is H264_DEC_CLK = 104 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c9e8),						/* Address */
    cpu_be_32(0x0),						/* Value */
    // MC_CGM_0.AC15_SC.B.SELCTL = 4;	// AC15 clock source selection - ENET_PLL_DFS4
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c9e0),						/* Address */
    cpu_be_32(0x04000000),						/* Value */  
    // MC_CGM_0.AC15_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c9e8),						/* Address */
    cpu_be_32(0x80000000),						/* Value */ 

    // mode_enter(DRUN_M);

    // Mode entry (DRUN)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004),						/* Address */
    cpu_be_32(0x30005AF0),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004), 					 /* Address */
    cpu_be_32(0x3000A50F),	

    ((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000),						/* Address*/
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
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4007c100),						/* Address */
    cpu_be_32(0x78000000),						/* Value */

    // configure PLLDIG registers


    // PLLDIG_3.PLLDV.B.RFDPHI1 = 1;	
    // PLLDIG_3.PLLDV.B.RFDPHI = 2;	
    //  PLLDIG_3.PLLDV.B.PREDIV = 1;	// input clock predivider, divided by 1
    //  PLLDIG_3.PLLDV.B.MFD = 26;	// Loop multiplication factor divider    
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003C1A8),						/* Address */
    cpu_be_32(0x0202101A),	


    // PLLDIG_3.PLLFD.B.SMDEN = 1;		// sigma-delta modulation enabled
    // PLLDIG_3.PLLFD.B.MFN = 13312;	// numerator for fractional loop division factor
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c1b0),						/* Address */
    cpu_be_32(0x40003400),

    // switch on DDR_PLL 
    // MC_ME.DRUN_MC.B.DDRPLLON = 1;	
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4004a02c),						/* Address */
    cpu_be_32(0x001003F2),						/* Value */


    // Mode entry (DRUN)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004),						/* Address */
    cpu_be_32(0x30005AF0),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004), 					 /* Address */
    cpu_be_32(0x3000A50F),	

    ((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,1)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000),						/* Address*/
    cpu_be_32(0x301003F2),

    // DFS_2.DFS_DLLPRG1.R =0x00005445;
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c1c0),						/* Address */
    cpu_be_32(0x00005445),

    // DFS_2.DLLPRG2.R =0x0000;
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c1c4),						/* Address */
    cpu_be_32(0x0),

    // DFS settings
    // DFS_2.DVPORT[0].B.MFN = 33;
    // DFS_2.DVPORT[0].B.MFI = 2; 
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003C1DC),						/* Address */
    cpu_be_32(0x00000221),  

    // DFS_2.DVPORT[1].B.MFN = 33;
    // DFS_2.DVPORT[1].B.MFI = 2;  
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c1e0),						/* Address */
    cpu_be_32(0x00000221),

    //  DFS_2.DVPORT[0].B.MFN = 11;
    //  DFS_2.DVPORT[0].B.MFI = 3; 
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c1e4),						/* Address */
    cpu_be_32(0x0000030b), 

    // DFS_2.CTRL.R &= (~2);		//    DFS0_CTRL reset is not asserted
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c1d8),						/* Address */
    cpu_be_32(0x00000000),

    // DFS_2.PORTRESET.R &= (~7);		//    DFS0_PORTRESET bits 0 - 2 are out of reset    
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),
    cpu_be_32(0x4003c1d4),						/* Address */
    cpu_be_32(0x00000000),


    // mode_enter(DRUN_M);
    // while(MC_ME.GS.B.S_DDRPLL != 1)
    // while(DFS_2.PORTSR.R != 0x7);	//DFS0_PORTSR port status register - wait until DVPORT 1-3 are locked, port 4 is not
    //mode_enter(DRUN_M);
    // Mode entry (DRUN) ,check ARM PLL on, sysclk = ARM PLL
    //while(MC_ME.GS.B.S_ARMPLL != 1);
    //while(MC_ME.GS.B.S_SYSCLK != 2);
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004),						/* Address */
    cpu_be_32(0x30005AF0),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004), 					 /* Address */
    cpu_be_32(0x3000A50F),	

    ((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,1)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000),						/* Address*/       
    cpu_be_32(0x301003F2),  

    ((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,1)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4003c14c),						/* Address*/
    cpu_be_32(0x0000000F),  

    // MC_CGM 0 AUX registers
    //  MC_CGM_0.AC0_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC0_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is ISP_CLK = 500 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c808),						/* Address */
    cpu_be_32(0x0),						/* Value */  

    //  MC_CGM_0.AC0_SC.B.SELCTL = 5;	// AC0 clock source selection - DDR_PLL_DFS1
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c800),						/* Address */
    cpu_be_32(0x05000000),						/* Value */

    // MC_CGM_0.AC0_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c808),						/* Address */
    cpu_be_32(0x80000000),						/* Value */  


    //  MC_CGM_0.AC1_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC1_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is APEX_APU_CLK = 500 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c828),						/* Address */
    cpu_be_32(0),						/* Value */  

    //  MC_CGM_0.AC1_SC.B.SELCTL = 5;	// AC1 clock source selection - DDR_PLL_DFS2
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c820),						/* Address */
    cpu_be_32(0x05000000),						/* Value */

    //  MC_CGM_0.AC1_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c828),						/* Address */
    cpu_be_32(0x80000000),						/* Value */  


    //   MC_CGM_0.AC2_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC2_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is MJEPEG_CLK = 500 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c848),						/* Address */
    cpu_be_32(0),						/* Value */  

    //  MC_CGM_0.AC2_SC.B.SELCTL = 5;	// AC2 clock source selection - DDR_PLL_DFS3
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c840),						/* Address */
    cpu_be_32(0x05000000),						/* Value */

    //  MC_CGM_0.AC2_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c848),						/* Address */
    cpu_be_32(0x80000000),						/* Value */  


    //  MC_CGM_0.AC8_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC8_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is DDR_CLK = 533 MHz
    //  MC_CGM_0.AC8_DC1.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC8_DC1.B.DIV = 3;			//divided by 4 (1 + DIV) - resultant output clk is DDR4_CLK = 133,25 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c908),						/* Address */
    cpu_be_32(0),						/* Value */  
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c90c),						/* Address */
    cpu_be_32(0x00030000),						/* Value */  

    //  MC_CGM_0.AC8_SC.B.SELCTL = 5;	// AC8 clock source selection - DDR_PLL_PHI0
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c900),						/* Address */
    cpu_be_32(0x05000000),						/* Value */

    //  MC_CGM_0.AC8_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC8_DC1.B.DE = 1;			//divider enable(1)/disable(0)	  
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c908),						/* Address */
    cpu_be_32(0x80000000),						/* Value */ 
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c90c),						/* Address */
    cpu_be_32(0x80030000),						/* Value */ 


    // Mode entry (DRUN)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004),						/* Address */
    cpu_be_32(0x30005AF0),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004), 					 /* Address */
    cpu_be_32(0x3000A50F),	

    ((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000),						/* Address*/
    cpu_be_32(MC_ME_S_MTRANS),



    ////////////////////////////////////////////////////
    ///////////////////////// VIDEO PLL/////////////////
    ////////////////////////////////////////////////////
    //////////// PHI 0 output f = 600 MHz///////////////
    ////////////////////////////////////////////////////

    // at first set SRC register to select input clk for PLL block: (0 FIRC selected, 1 XOSC selected)
    // SRC.GPR1.B.VIDEO_PLL_SOURCE_SELECT = 1;// XOSC selected as input to VIDEO_PLL
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4007c100),						/* Address */
    cpu_be_32(0xF8000000),						/* Value */

//PLLDIG_4.PLLCAL3.R=0x14000000; // validation team using it but this register is not in RM!
    //  *((vuint32_t*)(0x4003c218)) = 0x14000000;
    // configure PLLDIG registers   
    // PLLDIG_4.PLLDV.B.RFDPHI1 = 0;	// PHI1 output NA
    // PLLDIG_4.PLLDV.B.RFDPHI = 2;
    // PLLDIG_4.PLLDV.B.PREDIV = 1;	// input clock predivider, divided by 1
    // PLLDIG_4.PLLDV.B.MFD = 30;	// Loop multiplication factor divider
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c228),						/* Address */
    cpu_be_32(0x0002101e),						/* Value */    

    //     PLLDIG_4.PLLFD.B.SMDEN = 1;		// sigma-delta modulation enabled
    //     PLLDIG_4.PLLFD.B.MFN = 0;	//FRCDIV value - numerator for fractional loop division factor
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c230),						/* Address */
    cpu_be_32(0x70000000),						/* Value */    

    // switch on VIDEO_PLL 
    // MC_ME.DRUN_MC.B.VIDEOPLLON = 1;		//switch on VIDEO_PLL 
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4004a02c),						/* Address */
    cpu_be_32(0x001007F2),						/* Value */

    // mode_enter(DRUN_M);
    // while(MC_ME.GS.B.S_VIDEOPLL != 1);

    //mode_enter(DRUN_M);
    // Mode entry (DRUN) ,check ARM PLL on, PeriPLL on, ENETPLL on, sysclk = ARM PLL
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004),						/* Address */
    cpu_be_32(0x30005AF0),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004), 					 /* Address */
    cpu_be_32(0x3000A50F),	

    ((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,1)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000),						/* Address*/       
    cpu_be_32(0x301007F2), 

    // MC_CGM 0 AUX registers
    //  MC_CGM_0.AC9_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC9_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is DCU_AXI_CLK = 300 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c928),						/* Address */
    cpu_be_32(0),						/* Value */   

    //  MC_CGM_0.AC9_DC1.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC9_DC1.B.DIV = 1;			//divided by 2 (1 + DIV) - resultant output clk is DCU_PIX_CLK = 150 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c92c),						/* Address */
    cpu_be_32(0x00010000),						/* Value */  

    //  MC_CGM_0.AC9_SC.B.SELCTL = 9;	// AC9 clock source selection - VIDEO_PLL/2
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c920),						/* Address */
    cpu_be_32(0x09000000),

    //  MC_CGM_0.AC9_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC9_DC1.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c928),						/* Address */
    cpu_be_32(0x80000000),						/* Value */
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c92c),						/* Address */
    cpu_be_32(0x80010000),						/* Value */


    // MC_CGM 2 AUX registers
    // MC_CGM_2.AC4_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    //MC_CGM_2.AC4_DC0.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is TRACECLK = 300 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x40042888),						/* Address */
    cpu_be_32(0),

    //MC_CGM_2.AC4_SC.B.SELCTL = 9;	// AC4 clock source selection - VIDEO_PLL/2
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x40042880),						/* Address */
    cpu_be_32(0x09000000),

    //  MC_CGM_2.AC4_DC0.B.DE = 1;			//divider enable(1)/disable(0)	  
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x40042888),						/* Address */
    cpu_be_32(0x80000000),

    // remaining AUX registers

    // MC_CGM 0 AUX registers
    //  MC_CGM_0.AC4_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC4_DC0.B.DIV = 3;			//divided by 4 (1 + DIV) - resultant output clk is FLEXTIMER0_EXT_CLK = 20 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c888),						/* Address */
    cpu_be_32(0x00030000),

    //  MC_CGM_0.AC4_SC.B.SELCTL = 0xA;	// AC4 clock source selection - PERI_CLK 80 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c880),						/* Address */
    cpu_be_32(0x0A000000),

    //  MC_CGM_0.AC4_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c888),						/* Address */
    cpu_be_32(0x80000000),

    //  MC_CGM_0.AC10_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC10_DC0.B.DIV = 7;			//divided by 8 (1 + DIV) - resultant output clk is CLK_OUT = 20 MHz
    //  MC_CGM_0.AC10_DC1.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC10_DC1.B.DIV = 0;			//divided by 1 (1 + DIV) - resultant output clk is LBIST_FAST_CLK = 400 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003C948),						/* Address */
    cpu_be_32(0x0),
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003C94C),						/* Address */
    cpu_be_32(0),

    //  MC_CGM_0.AC10_SC.B.SELCTL = 3;	// AC10 clock source selection - PERIPH_PLL_PHI0 400 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003C940),						/* Address */
    cpu_be_32(0x03000000),

    //  MC_CGM_0.AC10_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC10_DC1.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003C948),						/* Address */
    cpu_be_32(0x80070000),
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003C94C),						/* Address */
    cpu_be_32(0x80000000),


    //  MC_CGM_0.AC11_DC0.B.DE = 0;			//divider enable(1)/disable(0)
    //  MC_CGM_0.AC11_DC0.B.DIV = 0;		//divided by 1 (1 + DIV) - resultant output clk is DDR_32KHZ_CLK = 32 kHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c968),						/* Address */
    cpu_be_32(0),

    //  MC_CGM_0.AC11_SC.B.SELCTL = 1;	// AC11 clock source selection - XOSC_32KHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c960),						/* Address */
    cpu_be_32(0x01000000),

    //  MC_CGM_0.AC11_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x4003c968),						/* Address */
    cpu_be_32(0x80000000),


    // MC_CGM 2 AUX registers
    //  MC_CGM_2.AC3_DC0.B.DE = 0;			//divider enable(1)/disable(0)
//    MC_CGM_2.AC3_DC0.B.DIV = 3;			//divided by 4 (1 + DIV) - resultant output clk is FLEXTIMER1_EXT_CLK = 20 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x40042868),						/* Address */
    cpu_be_32(0),

    //  MC_CGM_2.AC3_SC.B.SELCTL = 10;	// AC4 clock source selection - PERI_CLK 80 MHz
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x40042860),						/* Address */
    cpu_be_32(0x0a000000),

    // MC_CGM_2.AC3_DC0.B.DE = 1;			//divider enable(1)/disable(0)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	
    cpu_be_32(0x40042868),						/* Address */
    cpu_be_32(0x80000000),


    // mode_enter(DRUN_M);

    // Mode entry (DRUN)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004),						/* Address */
    cpu_be_32(0x30005AF0),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004), 					 /* Address */
    cpu_be_32(0x3000A50F),	

    ((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000),						/* Address*/
    cpu_be_32(MC_ME_S_MTRANS),





    // PCTL conf
    ((HAB_CMD_WRT_DAT )|(0xDC<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a0e4),						/* Address */
    cpu_be_32(0x00000007),							/* Value */
    cpu_be_32(0x4004a0e8),						/* Address */
    cpu_be_32(0x07000000),							/* Value */
    cpu_be_32(0x4004a0f0),						/* Address */
    cpu_be_32(0x07070700),							/* Value */
    cpu_be_32(0x4004a0f4),						/* Address */
    cpu_be_32(0x07000700),							/* Value */
    cpu_be_32(0x4004a0f8),						/* Address */
    cpu_be_32(0x00000700),							/* Value */

    cpu_be_32(0x4004a10c),						/* Address */
    cpu_be_32(0x00070007),							/* Value */
    cpu_be_32(0x4004a110),						/* Address */
    cpu_be_32(0x00070007),							/* Value */
    cpu_be_32(0x4004a114),						/* Address */
    cpu_be_32(0x00070007),							/* Value */
    cpu_be_32(0x4004a118),						/* Address */
    cpu_be_32(0x00070007),							/* Value */
    cpu_be_32(0x4004a11c),						/* Address */
    cpu_be_32(0x00070000),							/* Value */

    cpu_be_32(0x4004a124),						/* Address */
    cpu_be_32(0x07000000),							/* Value */
    cpu_be_32(0x4004a128),						/* Address */
    cpu_be_32(0x07000000),							/* Value */
    cpu_be_32(0x4004a134),						/* Address */
    cpu_be_32(0x07000000),							/* Value */
    cpu_be_32(0x4004a138),						/* Address */
    cpu_be_32(0x07000000),							/* Value */
    cpu_be_32(0x4004a160),						/* Address */
    cpu_be_32(0x07070700),							/* Value */

    cpu_be_32(0x4004a164),						/* Address */
    cpu_be_32(0x00000700),							/* Value */
    cpu_be_32(0x4004a168),						/* Address */
    cpu_be_32(0x00000700),							/* Value */
    cpu_be_32(0x4004a174),						/* Address */
    cpu_be_32(0x00000700),							/* Value */
    cpu_be_32(0x4004a178),						/* Address */
    cpu_be_32(0x07000700),							/* Value */
    cpu_be_32(0x4004a17c),						/* Address */
    cpu_be_32(0x07000700),							/* Value */

    cpu_be_32(0x4004a180),						/* Address */
    cpu_be_32(0x07000700),							/* Value */
    cpu_be_32(0x4004a18c),						/* Address */
    cpu_be_32(0x07000700),							/* Value */
    cpu_be_32(0x4004a190),						/* Address */
    cpu_be_32(0x07000000),							/* Value */
    cpu_be_32(0x4004a194),						/* Address */
    cpu_be_32(0x07000000),							/* Value */
    cpu_be_32(0x4004a198),						/* Address */
    cpu_be_32(0x07000000),							/* Value */

    cpu_be_32(0x4004a19c),						/* Address */
    cpu_be_32(0x07000000),							/* Value */
    
    cpu_be_32(0x4004a1ac),						/* Address */
    cpu_be_32(0x07000000),							/* Value */
    /////////////////////////////////////////////
    // Mode entry (DRUN)
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004),						/* Address */
    cpu_be_32(0x30005AF0),							/* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
    cpu_be_32(0x4004a004), 					 /* Address */
    cpu_be_32(0x3000A50F),	

    ((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x4004a000),						/* Address*/
    cpu_be_32(MC_ME_S_MTRANS), 							
    /////////////////////////////////////////////


    //////////////////////////////////////////////////////
    ///////////////////////// DDR 0 INIT /////////////////
    //////////////////////////////////////////////////////
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C4e4),						/* Address */
    cpu_be_32(0x31001540),							/* Value */
	cpu_be_32(0x4006C4F4),						/* Address */
    cpu_be_32(0x31001500),							/* Value */
	cpu_be_32(0x4006C4F8),						/* Address */
    cpu_be_32(0x31001500),							/* Value */
	cpu_be_32(0x4006C4FC),						/* Address */
    cpu_be_32(0x31001500),							/* Value */
	cpu_be_32(0x4006C500),						/* Address */
    cpu_be_32(0x31001500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C550),						/* Address */
    cpu_be_32(0x31001500),							/* Value */
	cpu_be_32(0x4006C554),						/* Address */
    cpu_be_32(0x31001500),							/* Value */
	cpu_be_32(0x4006C558),						/* Address */
    cpu_be_32(0x31001500),							/* Value */
	cpu_be_32(0x4006C55c),						/* Address */
    cpu_be_32(0x31001500),							/* Value */	
	cpu_be_32(0x4006C560),						/* Address */
    cpu_be_32(0x30012540),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C564),						/* Address */
    cpu_be_32(0x30012540),							/* Value */
	cpu_be_32(0x4006C568),						/* Address */
    cpu_be_32(0x30012540),							/* Value */
	cpu_be_32(0x4006C56c),						/* Address */
    cpu_be_32(0x30012540),							/* Value */
	cpu_be_32(0x4006C510),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C514),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C518),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C51C),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C520),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C524),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C528),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C52C),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C530),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C534),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C570),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C574),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C578),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C57C),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C580),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C584),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C588),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C58C),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C590),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C594),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C598),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C59C),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C5A0),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C5A4),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C5A8),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C5AC),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C5B0),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C5B4),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C5B8),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C5BC),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C5C0),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C5C4),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C5C8),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C5CC),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C5D0),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C5D4),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C5D8),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C5DC),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C5E0),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C5E4),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C5E8),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C5EC),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4003601C),						/* Address */
    cpu_be_32(0x00008000),							/* Value */
	cpu_be_32(0x4003600C),						/* Address */
    cpu_be_32(0x464F61A5),							/* Value */
	cpu_be_32(0x40036010),						/* Address */
    cpu_be_32(0x00180E63),							/* Value */
	cpu_be_32(0x40036014),						/* Address */
    cpu_be_32(0x000000DD),							/* Value */
	cpu_be_32(0x40036038),						/* Address */
    cpu_be_32(0x001F099B),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x40036008),						/* Address */
    cpu_be_32(0x00000000),							/* Value */
	cpu_be_32(0x40036018),						/* Address */
    cpu_be_32(0x00001688),							/* Value */
	cpu_be_32(0x40036030),						/* Address */
    cpu_be_32(0x00000010),							/* Value */
	cpu_be_32(0x40036000),						/* Address */
    cpu_be_32(0x03010000),							/* Value */
	cpu_be_32(0x400368B8),						/* Address */
    cpu_be_32(0x00000800),							/* Value */


	((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x400368B8),						/* Address*/
    cpu_be_32(0x00000800), 		


	((HAB_CMD_WRT_DAT )|(0x1C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4003601C),						/* Address */
    cpu_be_32(0x003F8030),							/* Value */
	cpu_be_32(0x4003685C),						/* Address */
    cpu_be_32(0x1B5F0109),							/* Value */
	cpu_be_32(0x40036800),						/* Address */
    cpu_be_32(0xA0010003),							/* Value */

	((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x40036800),						/* Address*/
    cpu_be_32(0x00010000), 		


	((HAB_CMD_WRT_DAT )|(0x7C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x40036000),						/* Address */
    cpu_be_32(0x83010000),							/* Value */
	cpu_be_32(0x4003601C),						/* Address */
    cpu_be_32(0xC2018030),							/* Value */
	cpu_be_32(0x4003601C),						/* Address */
    cpu_be_32(0x06028030),							/* Value */
	cpu_be_32(0x4003601C),						/* Address */
    cpu_be_32(0x01038030),							/* Value */
	cpu_be_32(0x4003601C),						/* Address */
    cpu_be_32(0xFF0A8030),							/* Value */
	
	cpu_be_32(0x40036040),						/* Address */
    cpu_be_32(0x0000007F),							/* Value */
	cpu_be_32(0x40036848),						/* Address */
    cpu_be_32(0x4D4B4F4B),							/* Value */
	cpu_be_32(0x40036850),						/* Address */
    cpu_be_32(0x38383737),							/* Value */
	cpu_be_32(0x4003683C),						/* Address */
    cpu_be_32(0x20000000),							/* Value */
	cpu_be_32(0x40036840),						/* Address */
    cpu_be_32(0x00000000),							/* Value */
	
	cpu_be_32(0x4003602C),						/* Address */
    cpu_be_32(0x0F9F26D2),							/* Value */
	cpu_be_32(0x40036004),						/* Address */
    cpu_be_32(0x00020024),							/* Value */
	cpu_be_32(0x40036020),						/* Address */
    cpu_be_32(0x30B01800),							/* Value */
	cpu_be_32(0x40036818),						/* Address */
    cpu_be_32(0x00000000),							/* Value */
	cpu_be_32(0x4003601C),						/* Address */
    cpu_be_32(0x00000000),							/* Value */
	
	
	
	//////////////////////////////////////////////////////
    ///////////////////////// DDR 1 INIT /////////////////
    //////////////////////////////////////////////////////
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C600),						/* Address */
    cpu_be_32(0x31001540),							/* Value */
	cpu_be_32(0x4006C610),						/* Address */
    cpu_be_32(0x31001500),							/* Value */
	cpu_be_32(0x4006C614),						/* Address */
    cpu_be_32(0x31001500),							/* Value */
	cpu_be_32(0x4006C618),						/* Address */
    cpu_be_32(0x31001500),							/* Value */
	cpu_be_32(0x4006C61C),						/* Address */
    cpu_be_32(0x31001500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C66C),						/* Address */
    cpu_be_32(0x31001500),							/* Value */
	cpu_be_32(0x4006C670),						/* Address */
    cpu_be_32(0x31001500),							/* Value */
	cpu_be_32(0x4006C674),						/* Address */
    cpu_be_32(0x31001500),							/* Value */
	cpu_be_32(0x4006C678),						/* Address */
    cpu_be_32(0x31001500),							/* Value */	
	cpu_be_32(0x4006C67C),						/* Address */
    cpu_be_32(0x30012540),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C680),						/* Address */
    cpu_be_32(0x30012540),							/* Value */
	cpu_be_32(0x4006C684),						/* Address */
    cpu_be_32(0x30012540),							/* Value */
	cpu_be_32(0x4006C688),						/* Address */
    cpu_be_32(0x30012540),							/* Value */
	cpu_be_32(0x4006C62C),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C630),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C634),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C638),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C63C),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C640),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C644),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C648),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C64C),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C650),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C68C),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C690),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C694),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C698),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C69C),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C6A0),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C6A4),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C6A8),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C6AC),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C6B0),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C6B4),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C6B8),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C6BC),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C6C0),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C6C4),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C6C8),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C6CC),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C6D0),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C6D4),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C6D8),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C6DC),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C6E0),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C6E4),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C6E8),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C6EC),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C6F0),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C6F4),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x4006C6F8),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C6FC),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C700),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C704),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	cpu_be_32(0x4006C708),						/* Address */
    cpu_be_32(0x71401500),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x400A201C),						/* Address */
    cpu_be_32(0x00008000),							/* Value */
	cpu_be_32(0x400A200C),						/* Address */
    cpu_be_32(0x464F61A5),							/* Value */
	cpu_be_32(0x400A2010),						/* Address */
    cpu_be_32(0x00180E63),							/* Value */
	cpu_be_32(0x400A2014),						/* Address */
    cpu_be_32(0x000000DD),							/* Value */
	cpu_be_32(0x400A2038),						/* Address */
    cpu_be_32(0x001F099B),							/* Value */
	
	((HAB_CMD_WRT_DAT )| (0x2C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x400A2008),						/* Address */
    cpu_be_32(0x00000000),							/* Value */
	cpu_be_32(0x400A2018),						/* Address */
    cpu_be_32(0x00001688),							/* Value */
	cpu_be_32(0x400A2030),						/* Address */
    cpu_be_32(0x00000010),							/* Value */
	cpu_be_32(0x400A2000),						/* Address */
    cpu_be_32(0x03010000),							/* Value */
	cpu_be_32(0x400A28B8),						/* Address */
    cpu_be_32(0x00000800),							/* Value */


	((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x400A28B8),						/* Address*/
    cpu_be_32(0x00000800), 		


	((HAB_CMD_WRT_DAT )|(0x1C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x400A201C),						/* Address */
    cpu_be_32(0x003F8030),							/* Value */
	cpu_be_32(0x400A285C),						/* Address */
    cpu_be_32(0x1B5F0109),							/* Value */
	cpu_be_32(0x400A2800),						/* Address */
    cpu_be_32(0xA0010003),							/* Value */

	((HAB_CMD_CHK_DAT )|(0xC<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),	/*@/@*$ Write DCD Header - 32bit mask - 0 set - 0$*@/@*/
    cpu_be_32(0x400A2800),						/* Address*/
    cpu_be_32(0x00010000), 		


	((HAB_CMD_WRT_DAT )|(0x7C<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), /* Write DCD Header - 8bit mask - 0 set - 0*/
	cpu_be_32(0x400A2000),						/* Address */
    cpu_be_32(0x83010000),							/* Value */
	cpu_be_32(0x400A201C),						/* Address */
    cpu_be_32(0xC2018030),							/* Value */
	cpu_be_32(0x400A201C),						/* Address */
    cpu_be_32(0x06028030),							/* Value */
	cpu_be_32(0x400A201C),						/* Address */
    cpu_be_32(0x01038030),							/* Value */
	cpu_be_32(0x400A201C),						/* Address */
    cpu_be_32(0xFF0A8030),							/* Value */
	
	cpu_be_32(0x400A2040),						/* Address */
    cpu_be_32(0x0000007F),							/* Value */
	cpu_be_32(0x400A2848),						/* Address */
    cpu_be_32(0x4D4B4F4B),							/* Value */
	cpu_be_32(0x400A2850),						/* Address */
    cpu_be_32(0x38383737),							/* Value */
	cpu_be_32(0x400A283C),						/* Address */
    cpu_be_32(0x20000000),							/* Value */
	cpu_be_32(0x400A2840),						/* Address */
    cpu_be_32(0x00000000),							/* Value */
	
	cpu_be_32(0x400A202C),						/* Address */
    cpu_be_32(0x0F9F26D2),							/* Value */
	cpu_be_32(0x400A2004),						/* Address */
    cpu_be_32(0x00020024),							/* Value */
	cpu_be_32(0x400A2020),						/* Address */
    cpu_be_32(0x30B01800),							/* Value */
	cpu_be_32(0x400A2818),						/* Address */
    cpu_be_32(0x00000000),							/* Value */
	cpu_be_32(0x400A201C),						/* Address */
    cpu_be_32(0x00000000),							/* Value */
    
    
    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24), 
    cpu_be_32(0x40012380),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012384),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012480),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012484),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012580),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012584),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012680),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012684),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012780),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012784),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012880),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012884),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012980),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012984),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012A80),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012A84),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012B80),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012B84),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012C80),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012C84),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012D80),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012D84),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012E80),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012E84),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012F80),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */

    ((HAB_CMD_WRT_DAT )|(0xc<<16) |(DCD_WRITE_PAR(4,0,0)) <<24),     
    cpu_be_32(0x40012F84),                                           /* Address */
    cpu_be_32(0x00000000),                                           /* Value */
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
    0x1000000,
    0x00000000 /* boot data */
};




