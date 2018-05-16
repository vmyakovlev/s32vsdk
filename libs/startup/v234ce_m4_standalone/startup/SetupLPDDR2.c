#include "SetupLPDDR2.h"	
#include "s32vs234.h"
#include "MMDC_PORTS.h"

/*
 * File: SetupLPDDR2.c
 * 
 * Purpose: Sets up LPDDR2 on Treerunner validation board B
 * 
 * Notes: Requires board type to be defined in SetupLPDDR2.h
 * 
 * Author: ttz778
 */
#define _EVBREVA
/* Micron MT42L64M32D1 @ 533 MHz*/
/* 14 row addr, 9 col addr, burst length 4, data size 32 */
#ifdef _EVBREVA
#define _MDCTL 0x03010000
#endif

/* ISSI IS46LD32160A @ 533 MHz*/
/* 13 row addr, 10 col addr, burst length 4, data size 32 */
#ifdef _VALEVBB
#define _MDCTL 0x02110000
#endif

/* Function to configure the SIUL for LPDDR2 */
/* Assumes module is enabled */
void lpddr_config_SIUL(uint8_t module)
{
    int i;

    /* Set control signal termination and drive strength */
	switch(module)
	{
		default:
		case 0:
			SIUL2.MSCR[_DDR0_CLK0].B.DDR_SEL = 2;		/* LPDDR2 */
			SIUL2.MSCR[_DDR0_CLK0].B.DSE = 5;
			SIUL2.MSCR[_DDR0_CLK0].B.CRPOINT_TRIM = 1;
			SIUL2.MSCR[_DDR0_CLK0].B.DCYCLE_TRIM = 0;

			SIUL2.MSCR[_DDR0_CKE0].B.DDR_SEL = 2;	/* LPDDR2 */
			SIUL2.MSCR[_DDR0_CKE0].B.DSE = 5;
			SIUL2.MSCR[_DDR0_CKE1].B.DDR_SEL = 2;	/* LPDDR2 */
			SIUL2.MSCR[_DDR0_CKE1].B.DSE = 5;

			SIUL2.MSCR[_DDR0_CS_B0].B.DDR_SEL = 2;	/* LPDDR2 */
			SIUL2.MSCR[_DDR0_CS_B0].B.DSE = 5;
			SIUL2.MSCR[_DDR0_CS_B1].B.DDR_SEL = 2;	/* LPDDR2 */
			SIUL2.MSCR[_DDR0_CS_B1].B.DSE = 5;

			SIUL2.MSCR[_DDR0_DM0].B.DDR_SEL = 2;		/* LPDDR2 */
			SIUL2.MSCR[_DDR0_DM0].B.DSE = 5;
			SIUL2.MSCR[_DDR0_DM1].B.DDR_SEL = 2;		/* LPDDR2 */
			SIUL2.MSCR[_DDR0_DM1].B.DSE = 5;
			SIUL2.MSCR[_DDR0_DM2].B.DDR_SEL = 2;		/* LPDDR2 */
			SIUL2.MSCR[_DDR0_DM2].B.DSE = 5;
			SIUL2.MSCR[_DDR0_DM3].B.DDR_SEL = 2;		/* LPDDR2 */
			SIUL2.MSCR[_DDR0_DM3].B.DSE = 5;

			/* DQS Drive Strength */
			for (i=_DDR0_DQS0; i<(_DDR0_DQS0+4); i++)
			{
				SIUL2.MSCR[i].B.DDR_SEL = 2;	/* LPDDR2 */
				SIUL2.MSCR[i].B.DSE = 5;
				SIUL2.MSCR[i].B.PUE = 1;
				SIUL2.MSCR[i].B.PUS = 0x0;
				SIUL2.MSCR[i].B.PKE = 0x1;		/* Enable pull down */
				SIUL2.MSCR[i].B.CRPOINT_TRIM = 1;
				SIUL2.MSCR[i].B.DCYCLE_TRIM = 0;
			}

			/* Set address */
			for (i=_DDR0_A0; i<(_DDR0_A0+10); i++)
			{
				SIUL2.MSCR[i].B.DDR_SEL = 2;			/* LPDDR2 */
				SIUL2.MSCR[i].B.DSE = 5;				/* 34 Ohm Drive Strength */
				SIUL2.MSCR[i].B.DDR_DO_TRIM = 1;
				SIUL2.MSCR[i].B.DCYCLE_TRIM = 1;
			}

			/* Set data */
			for (i=_DDR0_D0; i<(_DDR0_D0+32); i++)
			{
				SIUL2.MSCR[i].B.DDR_SEL = 2;	/* LPDDR2 */
				SIUL2.MSCR[i].B.DSE = 5;		/* 34 Ohm Drive Strength */
				SIUL2.MSCR[i].B.DDR_DO_TRIM = 1;
				SIUL2.MSCR[i].B.DCYCLE_TRIM = 1;
			}
		break;
		case 1:
			SIUL2.MSCR[_DDR1_CLK0].B.DDR_SEL = 2;		/* LPDDR2 */
			SIUL2.MSCR[_DDR1_CLK0].B.DSE = 5;
			SIUL2.MSCR[_DDR1_CLK0].B.CRPOINT_TRIM = 1;
			SIUL2.MSCR[_DDR1_CLK0].B.DCYCLE_TRIM = 0;

			SIUL2.MSCR[_DDR1_CKE0].B.DDR_SEL = 2;	/* LPDDR2 */
			SIUL2.MSCR[_DDR1_CKE0].B.DSE = 5;
			SIUL2.MSCR[_DDR1_CKE1].B.DDR_SEL = 2;	/* LPDDR2 */
			SIUL2.MSCR[_DDR1_CKE1].B.DSE = 5;

			SIUL2.MSCR[_DDR1_CS_B0].B.DDR_SEL = 2;	/* LPDDR2 */
			SIUL2.MSCR[_DDR1_CS_B0].B.DSE = 5;
			SIUL2.MSCR[_DDR1_CS_B1].B.DDR_SEL = 2;	/* LPDDR2 */
			SIUL2.MSCR[_DDR1_CS_B1].B.DSE = 5;

			SIUL2.MSCR[_DDR1_DM0].B.DDR_SEL = 2;		/* LPDDR2 */
			SIUL2.MSCR[_DDR1_DM0].B.DSE = 5;
			SIUL2.MSCR[_DDR1_DM1].B.DDR_SEL = 2;		/* LPDDR2 */
			SIUL2.MSCR[_DDR1_DM1].B.DSE = 5;
			SIUL2.MSCR[_DDR1_DM2].B.DDR_SEL = 2;		/* LPDDR2 */
			SIUL2.MSCR[_DDR1_DM2].B.DSE = 5;
			SIUL2.MSCR[_DDR1_DM3].B.DDR_SEL = 2;		/* LPDDR2 */
			SIUL2.MSCR[_DDR1_DM3].B.DSE = 5;

			/* DQS Drive Strength */
			for (i=_DDR1_DQS0; i<(_DDR1_DQS0+4); i++)
			{
				SIUL2.MSCR[i].B.DDR_SEL = 2;	/* LPDDR2 */
				SIUL2.MSCR[i].B.DSE = 5;
				SIUL2.MSCR[i].B.PUE = 1;
				SIUL2.MSCR[i].B.PUS = 0x0;
				SIUL2.MSCR[i].B.PKE = 0x1;		/* Enable pull down */
				SIUL2.MSCR[i].B.CRPOINT_TRIM = 1;
				SIUL2.MSCR[i].B.DCYCLE_TRIM = 0;
			}

			/* Set address */
			for (i=_DDR1_A0; i<(_DDR1_A0+10); i++)
			{
				SIUL2.MSCR[i].B.DDR_SEL = 2;			/* LPDDR2 */
				SIUL2.MSCR[i].B.DSE = 5;				/* 34 Ohm Drive Strength */
				SIUL2.MSCR[i].B.DDR_DO_TRIM = 1;
				SIUL2.MSCR[i].B.DCYCLE_TRIM = 1;
			}

			/* Set data */
			for (i=_DDR1_D0; i<(_DDR1_D0+32); i++)
			{
				SIUL2.MSCR[i].B.DDR_SEL = 2;	/* LPDDR2 */
				SIUL2.MSCR[i].B.DSE = 5;		/* 34 Ohm Drive Strength */
				SIUL2.MSCR[i].B.DDR_DO_TRIM = 1;
				SIUL2.MSCR[i].B.DCYCLE_TRIM = 1;
			}
		break;
	}
}

/* Return the Vendor ID and device information by reading the registers */
uint32_t probe_lpddr2(uint8_t module, uint8_t regno)
{
    struct MMDC_tag* MMDC;
    uint32_t retval;
    uint32_t regshift;
#ifdef _EVBREVA
    uint32_t a,b,c,d;
#endif
    regshift = regno << 16;
    regshift &= 0x00FF0000;
    regshift += 0x00008060;

	switch(module)
	{
		default:
		case 0: MMDC = (struct MMDC_tag*)0x40036000UL;
		break;
		case 1: MMDC = (struct MMDC_tag*)0x400A2000UL;
		break;
	}

    MMDC->MDSCR.R = 0x00008000;		/* Set MDSCR[CON_REQ] (configuration request) */

    MMDC->MDSCR.R = regshift;		/* Read the CS0 MR5 register */
    while(!MMDC->MDSCR.B.MRR_READ_DATA_VALID) {}

    retval = MMDC->MDMRR.R;
    MMDC->MDSCR.R = 0x00000000;		/* Deassert the configuration request */
#ifdef _EVBREVA
    /* re-order bytes */
    a = (retval & 0x000000FF) << 24;
    b = (retval & 0xFF000000) >> 8;
    c = (retval & 0x0000FF00);
    d = (retval & 0x00FF0000) >> 16;
    retval = a+b+c+d;
#endif

    return retval;
}

/* 533 MHz (tck=1.876ns) / WL = 4 / RL = 8 */
void config_lpddr2(uint8_t module)
{
	struct MMDC_tag* MMDC;
    /* Configure SIUL */
    lpddr_config_SIUL(module);
	
	switch(module)
	{
		default:
		case 0: MMDC = (struct MMDC_tag*)0x40036000UL;
		break;
		case 1: MMDC = (struct MMDC_tag*)0x400A2000UL;
		break;
	}

    MMDC->MDSCR.R = 0x00008000;		/* Set MDSCR[CON_REQ] (configuration request) */

#ifdef _EVBREVA
    MMDC->MDCFG0.R = 0x464F61A5;	/* tRFCab=70 (=130ns),tXSR=80 (=tRFCab+10ns),tXP=4 (=7.5ns),tXPDLL=n/a,tFAW=27 (50 ns),tCL(RL)=8 */
    MMDC->MDCFG1.R = 0x00180E63;	/* tRCD=n/a,tRPpb=n/a,tRC=n/a ,tRAS=25 (=47ns),tRPA=n/a,tWR=8 (=15.0ns),tMRD=3,tWL=4 */
    MMDC->MDCFG2.R = 0x000000DD;	/* tDLLK=n/a,tRTP=4 (=7.5ns),tWTR=4 (=7.5ns),tRRD=6 (=10ns) */
    MMDC->MDCFG3LP.R = 0x001F099B;	/* RC_LP=tRAS+tRPab=32 (>60ns), tRCD_LP=10 (18ns) , tRPpb_LP=10 (18ns), tRPab_LP=12 (21ns) */
    MMDC->MDOTC.R = 0x00000000;  	/* tAOFPD=n/a,tAONPD=n/a,tANPD=n/a,tAXPD=n/a,tODTLon=n/a,tODT_idle_off=n/a */
    MMDC->MDMISC.R = 0x00001688; 	/* WALAT=0, BI bank interleave on, LPDDR2_S2=0, MIF3=3, RALAT=2, 8 banks, LPDDR2 */
    MMDC->MDOR.R = 0x00000010;   	/* tXPR=n/a , SDE_to_RST=n/a, RST_to_CKE=14 */
    MMDC->MDCTL.R = _MDCTL;
#endif
#ifdef _VALEVBB
    MMDC->MDCFG0.R = 0x313B61A5;	/* tRFCab=50 (=90ns),tXSR=60 (=tRFCab+10ns),tXP=4 (=7.5ns),tXPDLL=n/a,tFAW=27 (50 ns),tCL(RL)=8 */
    MMDC->MDCFG1.R = 0x00160E83;	/* tRCD=n/a,tRPpb=n/a,tRC=n/a ,tRAS=23 (=42ns),tRPA=n/a,tWR=8 (=15.0ns),tMRD=5,tWL=4 */
    MMDC->MDCFG2.R = 0x000000DD;	/* tDLLK=n/a,tRTP=4 (=7.5ns),tWTR=4 (=7.5ns),tRRD=6 (=10ns) */
    MMDC->MDCFG3LP.R = 0x001F0999;	/* RC_LP=tRAS+tRPab=32 (>60ns), tRCD_LP=10 (18ns) , tRPpb_LP=10 (18ns), tRPab_LP=10 (18ns) */
    MMDC->MDOTC.R = 0x00000000;  	/* tAOFPD=n/a,tAONPD=n/a,tANPD=n/a,tAXPD=n/a,tODTLon=n/a,tODT_idle_off=n/a */
    MMDC->MDMISC.R = 0x000016A8; 	/* WALAT=0, BI bank interleave on, LPDDR2_S2=0, MIF3=3, RALAT=2, 4 banks, LPDDR2 */
    MMDC->MDOR.R = 0x00000010;   	/* tXPR=n/a , SDE_to_RST=n/a, RST_to_CKE=14 */
    MMDC->MDCTL.R = _MDCTL;
/* /requires update */
#endif
    MMDC->MPMUR0.R=0x00000800;		/* Force delay line initialisation */
    while (MMDC->MPMUR0.B.FRC_MSR) {}
    MMDC->MDSCR.R = 0x003F8030;		/* Reset command CS0 */

    /* Perform ZQ calibration */
    MMDC->MPZQLP2CTL.R = 0x1B5F0109;	/* ZQ_LP2_HW_ZQCS=0x1B (90ns spec), ZQ_LP2_HW_ZQCL=0x5F (160ns spec), ZQ_LP2_HW_ZQINIT=0x109 (1us spec) */
    MMDC->MPZQHWCTRL.R = 0xA0010003;	/* ZQ_EARLY_COMPARATOR_EN_TIMER=0x14, TZQ_CS=n/a, TZQ_OPER=n/a, TZQ_INIT=n/a, ZQ_HW_FOR=1, ZQ_HW_PER=0, ZQ_MODE=3 */
    while (MMDC->MPZQHWCTRL.B.ZQ_HW_FOR) {}

    /* Enable MMDC with CS0 */
    MMDC->MDCTL.R = _MDCTL + 0x80000000;

    /* Complete the initialization sequence as defined by JEDEC */
    MMDC->MDSCR.R = 0xC2018030;		/* Configure MR1: BL = 4, burst type = interleaved, wrap control = no wrap, tWR cycles = 8 */
    MMDC->MDSCR.R = 0x06028030;		/* Configure MR2: RL=8, WL=4 */
    MMDC->MDSCR.R = 0x01038030;		/* Configure MR3: DS=34R */
    MMDC->MDSCR.R = 0xFF0A8030;		/* Configure MR10: Calibration at init */

    /* Set the amount of DRAM */
    /* Set DQS settings based on board type */
#ifdef _EVBREVA
    switch(module)
    {
      default:
      case 0:
        MMDC->MDASP.R = 0x0000007F;		/* 2Gb, 256 MB memory so CS0 is 256 MB  (0x90000000) */
        MMDC->MPRDDLCTL.R = 0x4D4B4F4B;  	/* Read delay line offsets */
        MMDC->MPWRDLCTL.R = 0x38383737;  	/* Write delay line offsets */
        MMDC->MPDGCTRL0.R = 0x20000000;  	/* Read DQS gating control 0 (disabled) */
        MMDC->MPDGCTRL1.R = 0x00000000;  	/* Read DQS gating control 1 */
        MMDC->MAARCR.B.ARCR_DYN_MAX = 0x1;   
      break;
      case 1:
        MMDC->MDASP.R = 0x0000007F;		/* 2Gb, 256 MB memory so CS0 is 256 MB  (0xD0000000) */
        MMDC->MPRDDLCTL.R = 0x4D4B4F4B;  	/* Read delay line offsets */
        MMDC->MPWRDLCTL.R = 0x38383737;  	/* Write delay line offsets */
        MMDC->MPDGCTRL0.R = 0x20000000;  	/* Read DQS gating control 0 (disabled) */
        MMDC->MPDGCTRL1.R = 0x00000000;  	/* Read DQS gating control 1 */
        MMDC->MAARCR.B.ARCR_DYN_MAX = 0x1;   
      break;
    }
#endif
#ifdef _VALEVBB
    MMDC->MDASP.R = 0x00000041;		/* 512 Mb, 64 MB memory so CS0 is 64 MB  (0x84000000) */
    MMDC->MPRDDLCTL.R = 0x4D4B4F4B;  	/* Read delay line offsets */
    MMDC->MPWRDLCTL.R = 0x38383737;  	/* Write delay line offsets */
    MMDC->MPDGCTRL0.R = 0x20000000;  	/* Read DQS gating control 0 (disabled) */
    MMDC->MPDGCTRL1.R = 0x00000000;  	/* Read DQS gating control 1 */
#endif

    MMDC->MDRWD.R = 0x0F9F26D2;		/* Read/write command delay - default used */
    MMDC->MDPDC.R = 0x00020024;		/* Power down control */
    MMDC->MDREF.R = 0x30B01800 ;		/* Refresh control */
    MMDC->MPODTCTRL.R = 0x00000000;	/* No ODT */
    MMDC->MDSCR.R = 0x00000000;		/* Deassert the configuration request */
}

