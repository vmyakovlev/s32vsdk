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
#include <stdio.h>
#include "mmap.h"

#include <gic400.h>
#include <s32vs234.h>
#include <pll.h>
#include <i2c.h>
#include <HDMI.h>
#include <uartlinflex_driver.h>

void mode_enter(uint16_t mode);
void mode_init(void);
void init_gic400(void);
extern int main(int argc, char *argv[]);
void peripherals_clock_enable(void);
extern void mmu_init(void);
void init_bss(void);
void xrdc_settings (void);

void _kill(void)
{
	return;
}

extern unsigned int _data_lma; 
extern unsigned int _data_vma;
extern unsigned int _data_lma_end; 
extern unsigned int _data_vma_end;
extern unsigned int __bss_start__;
extern unsigned int __bss_end__;

void neon_memcpy(char* dst, char* src, long size)
{
  long simd_pixels = size & ~15; 
  long simd_iterations = simd_pixels >> 4;
  
  char *dst_local = dst;
  char *src_local = src;
  if(simd_iterations)
  { 
    __asm volatile( "1: \n\t"
		      "LD1 {V0.8H}, [%[src_local]], #16	\n\t"
			  "ST1 {V0.H}[0], [%[dst_local]], #2	\n\t"
			  "ST1 {V0.H}[1], [%[dst_local]], #2	\n\t"
			  "ST1 {V0.H}[2], [%[dst_local]], #2	\n\t"
			  "ST1 {V0.H}[3], [%[dst_local]], #2	\n\t"
			  "ST1 {V0.H}[4], [%[dst_local]], #2	\n\t"
			  "ST1 {V0.H}[5], [%[dst_local]], #2	\n\t"
			  "ST1 {V0.H}[6], [%[dst_local]], #2	\n\t"
			  "ST1 {V0.H}[7], [%[dst_local]], #2	\n\t"
			  "subs %[iterations],%[iterations],#1 \n\t"
		      "bne 1b \n\t"
		      : [src_local]"+r"(src_local), [dst_local] "+r"(dst_local), [iterations]"+r"(simd_iterations) 
		      : 
		      //: "memory", "d0" 
		   );
    }
}

volatile unsigned long   current_uptime  = 0;
volatile static uint32_t sTimeMeasureCnt = 0;

unsigned long get_uptime_microS()
{
  uint32_t lNow  = ++sTimeMeasureCnt;
  do
  {
    current_uptime = (0x00000000FFFFFFFF - (unsigned long)(PIT_0_LTMR64H)) << 32;
    current_uptime = current_uptime + (0xFFFFFFFF - PIT_0_LTMR64L);
    if(lNow  == sTimeMeasureCnt)
    {
      break;
    }
    else
    {
      lNow = sTimeMeasureCnt;
    }
  //NOTE: if stuck in this loop, probably there is some IRQ calling this
  //      get_uptime_microS() funtion with too high frequency
  } while(1);
  return current_uptime / 133;
}

//extern void (**__init_array_start__)();
//extern void (**__init_array_end__)();

//typedef void (InitFunc)(void); 

//extern void __init_array_start__(void); 
//extern void __init_array_end__(void); 

typedef void (*func_ptr)(void);
 
extern func_ptr __init_array_start__;//[0], 
extern func_ptr __init_array_end__;//[0];



void static_init(void)
{
	// Only dummy function
	if ((&__init_array_start__ + 0x1) == &__init_array_end__) return;
	
	for ( func_ptr* func = &__init_array_start__; func != &__init_array_end__; func++ )
		(*func)();
}

void read_ncf (void);
int startup(void)
{
	init_bss();
	mmu_init();
	
	static_init();
	
    
	// Init functions
	
	/*	uint64_t* data_lbegin = (uint64_t*)&_data_lma;
	uint64_t* data_vbegin = (uint64_t*)&_data_vma;
	uint64_t  data_size   = ((uint64_t)&_data_lma_end - (uint64_t)&_data_lma) >> 3;
	uint64_t  data_count  = 0;
	while (data_count < data_size)
	{
		data_vbegin[data_count] = data_lbegin[data_count];
		++data_count;
	}*/
	
//    xrdc_settings();
//    mode_init();
//    pll_init();
//    peripherals_clock_enable();
//    mode_enter(DRUN_M);
    // clkout_settings(25);
	
	
	
	// GIC init
    init_gic400();

    // I2C init - not working for VP	
//	I2C_Init_Config(I2C0,SONY_CAM_ADDR);
	// I2C_Tx(I2C0,0x00,0x00,0x00);
	// I2C_Rx(I2C0,0x00,0x00);
	
    // UART INIT - not working for VP
	LinFlex0_Init();
	printf("A53 Startup done.\n");
	// UART EMU for VP
    // init_add(&system_cnt_top);
    // uartpl011_enable(&(system_cnt_top.uart0_cntl));
    // ioSetUART(&(system_cnt_top.uart0_cntl));
  
    // APEX Driver init
    //InitVirtualHwPtrArray();
    //global_hal_AllReset(0);
//	global_hal_AllReset(1);
		
	// turn on PIT
	PIT_0_MCR = 0x00;
	// Timer 2
	PIT_0_LDVAL1 = 0xFFFFFFFF; // setup Timer 2 for 10 counts
	PIT_0_TCTRL1 |= 4; // chain Timer 2 to Timer 1
	PIT_0_TCTRL1 |= 1; // start Timer 2
	// Timer 1
	PIT_0_LDVAL0 = 0xFFFFFFFF; // setup Timer 1 for 600 000 000 cycles
	PIT_0_TCTRL0 = 1; // start Timer 1

	printf("APEX driver and OAL initialized.\n");
	
	read_ncf();
	
	#ifdef OLD_EVB    
		I2C_Init_Config(I2C2,HDMI_ADDR);
	#else
		I2C_Init_Config(I2C1,HDMI_ADDR);
	#endif // ifdef OLD_EVB
    HDMI_configuration();
	
	return main(1, 0); 
}
uint32_t ncf[4];
void read_ncf (void) {

    //INFO("fccu - read_ncf","Waiting for data ready");
    FCCU.CTRL.R = 0x0000000A; //OP10 = 0x0000000A 
    while(FCCU.CTRL.B.OPS != 0x3); // wait till successfull	

    ncf[0] = FCCU.NCF_S[0].R;
    ncf[1] = FCCU.NCF_S[1].R;
    ncf[2] = FCCU.NCF_S[2].R;
    ncf[3] = FCCU.NCF_S[3].R;
}

extern unsigned int __bss_start__;
extern unsigned int __bss_end__;
void init_bss()
{
	uint64_t* bss_begin = (uint64_t*)&__bss_start__;
    uint64_t* bss_end   = (uint64_t*)&__bss_end__;
    while(bss_begin <= bss_end)
    {
        *bss_begin = 0;
        bss_begin++;
    }
}
void init_gic400(void)
{
	uint32_t i;
   
   /* Configure Addresses for GIC_Distributor and Core */  
    gic400_setGICAddr((void*)0x7D001000, (void*)0x7D002000);

    /* Configure interrupt controller (GICv2 assumed) */
    gic400_enableGIC();
    gic400_enableCPUInterface();
    gic400_enableNonSecureCPUInterface();
    /* Configure priority mask to lowest value - enables all priorities */
  
    gic400_setPriorityMask(0xFF);
  
  for (i=0; i<176; i++)
 {
        MSCM.IRSPRC[i].R = 0x3; 
}    
    /* fill the interrupts_vector with the default interrupt rutine */
    for (i=0; i<INT_NUM_SOURCES; i++)
    {
        g_INT_vectors[i] = (int_function_t)default_interrupt_routine; 
    } 
}

void mode_init(void){
    
    // mode settings
    MC_ME.ME.R |= 0xFF;	// enable all the modes
    MC_ME.RUN_PC[7].R = 0x000000FF;	// enable all peripherals to be active during DRUN mode
	
    // FIRC is sysclk for DRUN and RUN0
    MC_ME.DRUN_MC.B.SYSCLK = 0x0000;	// system clk is FIRC
    // re-enter DRUN mode to update settings
    mode_enter(DRUN_M);
    
    // XOSC settings
    FXOSC.CTL.B.EOCV = 0xA0;	// XOSC end of count value - 2ms delay
    //FXOSC.CTL.B.EOCV = 0x0;	// for simulation purpose only
    FXOSC.CTL.B.OSCBYP = 0x0;		// bypass off	
    FXOSC.CTL.B.GM_SEL = 0x7;		// crystal overdrive protection 1x

    // switch XOSC as sysclk in DRUN
    MC_ME.DRUN_MC.B.XOSCON = 1;	//switch on external oscilator
    mode_enter(DRUN_M);
    while(MC_ME.GS.B.S_XOSC == 0);	//wait until the external oscilator is stabilized
    MC_ME.DRUN_MC.B.SYSCLK = 0x0001;	//system clk is XOSC
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
/*int read_ncf (void) {
  int fccu_x;
  //INFO("fccu - read_ncf","Waiting for data ready");
  FCCU.CTRL.R = 0x0000000A; //OP10 = 0x0000000A 
  while(FCCU.CTRL.B.OPS != 0x3); // wait till successfull	

  //#if NUMBER_OF_NCF_REGS_IN_FCCU > 0
  ncf[0] = FCCU.NCF_S[0].R;
  
  //#endif
  //#if NUMBER_OF_NCF_REGS_IN_FCCU > 1
  ncf[1] = FCCU.NCF_S[1].R;
  
  //#endif
  //#if NUMBER_OF_NCF_REGS_IN_FCCU > 2
  ncf[2] = FCCU.NCF_S[2].R;
  
  //#endif
  //#if NUMBER_OF_NCF_REGS_IN_FCCU > 3
  ncf[3] = FCCU.NCF_S[3].R;
  
  //#endif
  return(fccu_x);
  }

*/


