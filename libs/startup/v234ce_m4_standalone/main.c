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

#include "config.h"
#include "linflex_serial.h"
#include "pll.h"
#include "s32vs234.h"
#include "HDMI.h"
#include "printf.h"
#include "i2c.h"


void mode_enter(uint16_t mode);
void mode_init(void);
void pll_init(void);
void clkout_settings (uint32_t clkout_value);
void axbs_settings (void);
void xrdc_settings (void);
static int read_ncf (void);
void peripherals_clock_enable(void);
void m4_stop(void);


extern unsigned int __bss_start__;
extern unsigned int __bss_end__;
extern void config_lpddr2(uint8_t module);
extern void config_ddr3(uint8_t);
extern void ddr3_config_SIUL(uint8_t module);

void main(void)
{
    uint32_t i=0;

    uint32_t* bss_begin = (uint32_t*)&__bss_start__;
    uint32_t* bss_end   = (uint32_t*)&__bss_end__;
    
    while(bss_begin <= bss_end)
    {
        *bss_begin = 0;
        bss_begin++;
    }


    mode_init();
    pll_init();
    
    { // BUS QOS setup
      *(uint32_t *)(0x40012380) = 0x00000000; // FASTDMA 1 QOS READ
      *(uint32_t *)(0x40012384) = 0x00000000; // FASTDMA 1 QOS WRITE
      *(uint32_t *)(0x40012480) = 0x00000000; // GPU0 READ
      *(uint32_t *)(0x40012484) = 0x00000000; // GPU0 WRITE
      *(uint32_t *)(0x40012580) = 0x00000000; // H264DEC READ
      *(uint32_t *)(0x40012584) = 0x00000000; // H264DEC WRITE
      *(uint32_t *)(0x40012680) = 0x00000000; // GPU1 READ
      *(uint32_t *)(0x40012684) = 0x00000000; // GPU1 WRITE
      *(uint32_t *)(0x40012780) = 0x00000000; // CORES_CC1_IB READ
      *(uint32_t *)(0x40012784) = 0x00000000; // CORES_CC1_IB WRITE
      
      
      // APEX DRIVER
      *(uint32_t *)(0x40012880) = 0x00000000; // APEX0 DMA READ
      *(uint32_t *)(0x40012884) = 0x00000000; // APEX0 DMA WRITE
      *(uint32_t *)(0x40012980) = 0x00000000; // APEX0 BLKDMA READ
      *(uint32_t *)(0x40012984) = 0x00000000; // APEX0 BLKDMA WRITE
      *(uint32_t *)(0x40012A80) = 0x00000000; // APEX1 DMA READ
      *(uint32_t *)(0x40012A84) = 0x00000000; // APEX1 DMA WRITE
      *(uint32_t *)(0x40012B80) = 0x00000000; // APEX1 BLKDMA READ
      *(uint32_t *)(0x40012B84) = 0x00000000; // APEX1 BLKDMA WRITE
      
      
      *(uint32_t *)(0x40012C80) = 0x00000000; // PCIE READ
      *(uint32_t *)(0x40012C84) = 0x00000000; // PCIE WRITE
      *(uint32_t *)(0x40012D80) = 0x00000000; // ENET0 READ
      *(uint32_t *)(0x40012D84) = 0x00000000; // ENET0 WRITE
      *(uint32_t *)(0x40012E80) = 0x00000000; // ENET1 READ
      *(uint32_t *)(0x40012E84) = 0x00000000; // ENET1 WRITE
      *(uint32_t *)(0x40012F80) = 0x00000000; // CORES_CC0_IB READ
      *(uint32_t *)(0x40012F84) = 0x00000000; // CORES_CC0_IB WRITE
    }
    
    SRC.GPR8.R = 0x8;
    peripherals_clock_enable();
    mode_enter(DRUN_M);
    //clkout_settings(25);
    mode_enter(DRUN_M);

#if defined(USE_EVB29288)
    ddr3_config_SIUL(0);
    ddr3_config_SIUL(1);
    config_ddr3(0);
    config_ddr3(1);
#endif
#if defined(USE_EVB28899)
    config_lpddr2(0);
    config_lpddr2(1);
    config_lpddr2(0);
    config_lpddr2(1);
#endif
/*
  // settings of the registers to wake-up a53 cores
  MC_ME.CCTL0.R = 0xFA;	//enable CM4 to be active during all modes of operation
  MC_ME.CCTL1.R = 0xFA;	//enable CA53 core 0 to be active during all modes of operation

  // booting addres of A53 core 0 
  MC_ME.CADDR1.B.ADDR = 0x3E9096E0;	// needs to be updated!!
  MC_ME.CADDR1.B.ADDR_EN = 1;

  // booting address of M4 core 
  // MC_ME.CADDR0.B.ADDR = .... fill the address here!!
  mode_enter(DRUN_M);
  */
    

//MC_ME.CCTL0.R = 0xffff;
    *(vuint16_t *) 0x4004A1C4 = 0xffff;

    //   MC_ME.CCTL2.R = 0xffff;
    *(vuint16_t *) 0x4004A1C8 = 0xffff;
//    MC_ME.CCTL3.R = 0xffff;
    *(vuint16_t *) 0x4004A1CA = 0xffff;
//MC_ME.CCTL[4].R = 0xffff;
    //  MC_ME.CCTL5.R = 0xffff;
    *(vuint16_t *) 0x4004A1CE = 0xffff;
    MC_ME.CADDR1.R = 0x3E9096E1;//(0x3e840000 |0x1); MC_ME.CADDR[2].R = (0x3e909030 |0x1);//(0x3e860000 |0x1); MC_ME.CADDR[3].R = (0x3e989030 |0x1);//(0x3e880000 |0x1); MC_ME.CADDR[4].R = (0x3ea09030 |0x1);//(0x1e800000 |0x1); 
    mode_enter(DRUN_M);

     LinFlex0_Init();
 #ifdef OLD_EVB    
     I2C_Init_Config(I2C2,HDMI_ADDR);
 #else
     I2C_Init_Config(I2C1,HDMI_ADDR);
 #endif // ifdef OLD_EVB
     HDMI_configuration();
 
     
	
    printf("Project for CM4");
	
    m4_stop();
}

void mode_init(void){

    // FIRC is sysclk for DRUN and RUN0
    // mode settings
    CMU_0.CSR.B.RCDIV = 0x3;		// RCDIV max
    MC_ME.ME.R |= 0xFF;	// enable all the modes
    MC_ME.RUN_PC[7].R = 0x000000FF;	// enable all peripherals to be active during all modes except of RESET
    MC_ME.DRUN_MC.B.SYSCLK = 0x0000;	// system clk is FIRC
    // re-enter DRUN mode to update settings
    mode_enter(DRUN_M);

    // XOSC settings
    FXOSC.CTL.B.EOCV = 0xA0;	// XOSC end of count value - 2ms delay
    FXOSC.CTL.B.OSCBYP = 0x0;		// bypass off	
    FXOSC.CTL.B.GM_SEL = 0x7;		// crystal overdrive protection 1x

    // switch XOSC as sysclk in DRUN
    MC_ME.DRUN_MC.B.XOSCON = 1;	//switch on external oscilator
    mode_enter(DRUN_M);
    while(MC_ME.GS.B.S_XOSC == 0);	//wait until the external oscilator is stabilized
    MC_ME.DRUN_MC.B.SYSCLK = 0x0001;	//system clk is XOSC
    MC_ME.DRUN_SEC_CC_I.B.SYSCLK1 =1; //xosc as system clock1
    MC_ME.DRUN_SEC_CC_I.B.SYSCLK2 =1; //xosc as system clock2
    MC_ME.DRUN_SEC_CC_I.B.SYSCLK3 =1; //xosc as system clock3
    mode_enter(DRUN_M);    
    // note: clk source for SYSCLK has to be switched ON before it it selected to be a SYSCLK!!!!!!!!!!    
}

void mode_enter(uint16_t mode){

    MC_ME.MCTL.R = (mode << 28 | 0x00005AF0);
    MC_ME.MCTL.R = (mode << 28 | 0x0000A50F);
    // wait while mode entry is in process
    while(MC_ME.GS.B.S_MTRANS == 1);
    // check if the mode has been entered
    while(MC_ME.GS.B.S_CURRENT_MODE != mode);
}




void axbs_settings (void){

    AXBS.PORT[0].CRS.B.ARB = 0x1;		// arbitration mode is Round robin priority
    AXBS.PORT[1].CRS.B.ARB = 0x1;		// arbitration mode is Round robin priority
    AXBS.PORT[2].CRS.B.ARB = 0x1;		// arbitration mode is Round robin priority
    AXBS.PORT[3].CRS.B.ARB = 0x1;		// arbitration mode is Round robin priority    
    AXBS.PORT[4].CRS.B.ARB = 0x1;		// arbitration mode is Round robin priority
    AXBS.PORT[5].CRS.B.ARB = 0x1;		// arbitration mode is Round robin priority
    AXBS.PORT[6].CRS.B.ARB = 0x1;		// arbitration mode is Round robin priority
    AXBS.PORT[7].CRS.B.ARB = 0x1;		// arbitration mode is Round robin priority    
}

void xrdc_settings (void){

    XRDC.CR.B.GVLD = 0x0;		// XRDC global enable (1)/disable (0), All accesses from all bus masters to all slaves are allowed.

}

void peripherals_clock_enable(void)
{
    MC_ME.PCTL39.R = 0x7;
    MC_ME.PCTL40.R = 0x7;
    MC_ME.PCTL48.R = 0x7;
    MC_ME.PCTL49.R = 0x7;
    MC_ME.PCTL50.R = 0x7;
    MC_ME.PCTL52.R = 0x7;
    MC_ME.PCTL54.R = 0x7;
    MC_ME.PCTL58.R = 0x7;
    MC_ME.PCTL77.R = 0x7;
    MC_ME.PCTL79.R = 0x7;
    MC_ME.PCTL81.R = 0x7;
    MC_ME.PCTL83.R = 0x7;
    MC_ME.PCTL85.R = 0x7;
    MC_ME.PCTL87.R = 0x7;
    MC_ME.PCTL89.R = 0x7;
    MC_ME.PCTL91.R = 0x7;
    MC_ME.PCTL93.R = 0x7;
    MC_ME.PCTL100.R = 0x7;
    MC_ME.PCTL104.R = 0x7;
    MC_ME.PCTL116.R = 0x7;
    MC_ME.PCTL120.R = 0x7;
    MC_ME.PCTL160.R = 0x7;
    MC_ME.PCTL161.R = 0x7;
    MC_ME.PCTL162.R = 0x7;
    MC_ME.PCTL166.R = 0x7;
    MC_ME.PCTL170.R = 0x7;
    MC_ME.PCTL182.R = 0x7;
    MC_ME.PCTL184.R = 0x7;
    MC_ME.PCTL186.R = 0x7;
    MC_ME.PCTL188.R = 0x7;
    MC_ME.PCTL190.R = 0x7;
    MC_ME.PCTL192.R = 0x7;
    MC_ME.PCTL194.R = 0x7;
    MC_ME.PCTL204.R = 0x7;
    MC_ME.PCTL206.R = 0x7;
    MC_ME.PCTL208.R = 0x7;
    MC_ME.PCTL212.R = 0x7;
    MC_ME.PCTL216.R = 0x7;
    MC_ME.PCTL220.R = 0x7;
    MC_ME.PCTL236.R = 0x7;
}

void m4_stop()
{
  while(1);
}
