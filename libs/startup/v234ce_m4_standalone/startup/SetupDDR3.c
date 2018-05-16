#include "SetupDDR3.h"	
#include "s32vs234.h"
#include "MMDC_PORTS.h"

/*
* File: SetupDDR3.c
* 
* Purpose: Sets up DDR3 on Treerunner validation board A
* 
* Notes: Requires board type to be defined in SetupDDR3.h
* 
* Author: ttz778
*/

#define _VALEVBA

/* Micron MT41K256M16 @ 396 MHz*/
/* 15 row addr, 10 col addr, burst length 8, data size 32 */
#ifdef _VALEVBA
#define _MDCTL 0x04190000   
#endif

/* Micron MT41K256M16 @ 396 MHz 16-BIT */
/* 15 row addr, 10 col addr, burst length 8, data size 16 */
#ifdef _VALEVBA_16BIT
#define _MDCTL 0x04180000   
#endif

/* Function to configure the SIUL for DDR3 */
/* Assumes module is enabled */
void ddr3_config_SIUL(uint8_t module)
{
    int i;

    /* Set control signal termination and drive strength */
    switch(module)
    {
        default:
        case 0:
            SIUL2.MSCR[_DDR0_RESET].B.DDR_SEL = 0;		/* DDR3 */
            SIUL2.MSCR[_DDR0_RESET].B.DSE = 7;
            SIUL2.MSCR[_DDR0_RESET].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR0_RESET].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR0_RESET].B.PUS = 2;
            
            SIUL2.MSCR[_DDR0_CLK0].B.DDR_SEL = 0;		/* DDR3 */
            SIUL2.MSCR[_DDR0_CLK0].B.DSE = 7;
            SIUL2.MSCR[_DDR0_CLK0].B.CRPOINT_TRIM = 1;
            SIUL2.MSCR[_DDR0_CLK0].B.DCYCLE_TRIM = 0;
            SIUL2.MSCR[_DDR0_CLK0].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR0_CLK0].B.DDR_ODT = 1;
            SIUL2.MSCR[_DDR0_CLK0].B.PUS = 2;
            
            SIUL2.MSCR[_DDR0_CAS].B.DDR_SEL = 0;	/* DDR3 */
            SIUL2.MSCR[_DDR0_CAS].B.DSE = 7;
            SIUL2.MSCR[_DDR0_CAS].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR0_CAS].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR0_CAS].B.PUS = 2;
            
            SIUL2.MSCR[_DDR0_RAS].B.DDR_SEL = 0;	/* DDR3 */
            SIUL2.MSCR[_DDR0_RAS].B.DSE = 7;
            SIUL2.MSCR[_DDR0_RAS].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR0_RAS].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR0_RAS].B.PUS = 2;
            
            SIUL2.MSCR[_DDR0_WE_B].B.DDR_SEL = 0;	/* DDR3 */
            SIUL2.MSCR[_DDR0_WE_B].B.DSE = 7;
            SIUL2.MSCR[_DDR0_WE_B].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR0_WE_B].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR0_WE_B].B.PUS = 2;
            
            SIUL2.MSCR[_DDR0_CKE0].B.DDR_SEL = 0;	/* DDR3 */
            SIUL2.MSCR[_DDR0_CKE0].B.DSE = 7;
            SIUL2.MSCR[_DDR0_CKE0].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR0_CKE0].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR0_CKE0].B.PUS = 2;
            
            SIUL2.MSCR[_DDR0_CKE1].B.DDR_SEL = 0;	/* DDR3 */
            SIUL2.MSCR[_DDR0_CKE1].B.DSE = 7;
            SIUL2.MSCR[_DDR0_CKE1].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR0_CKE1].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR0_CKE1].B.PUS = 2;
            
            SIUL2.MSCR[_DDR0_CS_B0].B.DDR_SEL = 0;	/* DDR3 */
            SIUL2.MSCR[_DDR0_CS_B0].B.DSE = 7;
            SIUL2.MSCR[_DDR0_CS_B0].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR0_CS_B0].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR0_CS_B0].B.PUS = 2;
            
            SIUL2.MSCR[_DDR0_CS_B1].B.DDR_SEL = 0;	/* DDR3 */
            SIUL2.MSCR[_DDR0_CS_B1].B.DSE = 7;
            SIUL2.MSCR[_DDR0_CS_B1].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR0_CS_B1].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR0_CS_B1].B.PUS = 2;

            SIUL2.MSCR[_DDR0_BA0].B.DDR_SEL = 0;	/* DDR3 */
            SIUL2.MSCR[_DDR0_BA0].B.DSE = 7;            
            SIUL2.MSCR[_DDR0_BA0].B.DDR_DO_TRIM = 1;
            SIUL2.MSCR[_DDR0_BA0].B.DCYCLE_TRIM = 1;
            SIUL2.MSCR[_DDR0_BA0].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR0_BA0].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR0_BA0].B.PUS = 2;
            SIUL2.MSCR[_DDR0_BA1].B.DDR_SEL = 0;    /* DDR3 */
            SIUL2.MSCR[_DDR0_BA1].B.DSE = 7;            
            SIUL2.MSCR[_DDR0_BA1].B.DDR_DO_TRIM = 1;
            SIUL2.MSCR[_DDR0_BA1].B.DCYCLE_TRIM = 1;
            SIUL2.MSCR[_DDR0_BA1].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR0_BA1].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR0_BA1].B.PUS = 2;
            SIUL2.MSCR[_DDR0_BA2].B.DDR_SEL = 0;    /* DDR3 */
            SIUL2.MSCR[_DDR0_BA2].B.DSE = 7;            
            SIUL2.MSCR[_DDR0_BA2].B.DDR_DO_TRIM = 1;
            SIUL2.MSCR[_DDR0_BA2].B.DCYCLE_TRIM = 1;
            SIUL2.MSCR[_DDR0_BA2].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR0_BA2].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR0_BA2].B.PUS = 2;
            
            SIUL2.MSCR[_DDR0_DM0].B.DDR_SEL = 0;  /* DDR3 */
            SIUL2.MSCR[_DDR0_DM0].B.DSE = 7;
            SIUL2.MSCR[_DDR0_DM0].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR0_DM0].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR0_DM0].B.PUS = 2;
            SIUL2.MSCR[_DDR0_DM1].B.DDR_SEL = 0;  /* DDR3 */
            SIUL2.MSCR[_DDR0_DM1].B.DSE = 7;
            SIUL2.MSCR[_DDR0_DM1].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR0_DM1].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR0_DM1].B.PUS = 2;
            SIUL2.MSCR[_DDR0_DM2].B.DDR_SEL = 0;  /* DDR3 */
            SIUL2.MSCR[_DDR0_DM2].B.DSE = 7;
            SIUL2.MSCR[_DDR0_DM2].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR0_DM2].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR0_DM2].B.PUS = 2;
            SIUL2.MSCR[_DDR0_DM3].B.DDR_SEL = 0;  /* DDR3 */
            SIUL2.MSCR[_DDR0_DM3].B.DSE = 7;
            SIUL2.MSCR[_DDR0_DM3].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR0_DM3].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR0_DM3].B.PUS = 2;

            /* DQS Drive Strength */
            for (i=_DDR0_DQS0; i<(_DDR0_DQS0+4); i++)
            {
                SIUL2.MSCR[i].B.DDR_SEL = 0;       /* DDR3 */
                SIUL2.MSCR[i].B.DSE = 7;
                SIUL2.MSCR[i].B.CRPOINT_TRIM = 1;
                SIUL2.MSCR[i].B.DCYCLE_TRIM = 0;
                SIUL2.MSCR[i].B.DDR_INPUT = 1;
                SIUL2.MSCR[i].B.DDR_ODT = 1;
                SIUL2.MSCR[i].B.PUS = 2;
            }

            SIUL2.MSCR[_DDR0_ODT0].B.DDR_SEL = 0;	/* DDR3 */
            SIUL2.MSCR[_DDR0_ODT0].B.DSE = 7;				
            SIUL2.MSCR[_DDR0_ODT0].B.DDR_DO_TRIM = 1;
            SIUL2.MSCR[_DDR0_ODT0].B.DCYCLE_TRIM = 1;
            SIUL2.MSCR[_DDR0_ODT0].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR0_ODT0].B.PUS = 2;
            SIUL2.MSCR[_DDR0_ODT0].B.DDR_ODT = 1;
            
            SIUL2.MSCR[_DDR0_ODT1].B.DDR_SEL = 0;   /* DDR3 */
            SIUL2.MSCR[_DDR0_ODT1].B.DSE = 7;               
            SIUL2.MSCR[_DDR0_ODT1].B.DDR_DO_TRIM = 1;
            SIUL2.MSCR[_DDR0_ODT1].B.DCYCLE_TRIM = 1;
            SIUL2.MSCR[_DDR0_ODT1].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR0_ODT1].B.PUS = 2;
            SIUL2.MSCR[_DDR0_ODT1].B.DDR_ODT = 1;

            /* Set address */
            for (i=_DDR0_A0; i<(_DDR0_A0+15); i++)
            {
                SIUL2.MSCR[i].B.DDR_SEL = 0;    /* DDR3 */
                SIUL2.MSCR[i].B.DSE = 7;            
                SIUL2.MSCR[i].B.DDR_DO_TRIM = 1;
                SIUL2.MSCR[i].B.DCYCLE_TRIM = 1;
                SIUL2.MSCR[i].B.DDR_ODT = 2;
                SIUL2.MSCR[i].B.DDR_INPUT = 1;
                SIUL2.MSCR[i].B.PUS = 2;
            }

            /* Set data */
            for (i=_DDR0_D0; i<(_DDR0_D0+32); i++)
            {
                SIUL2.MSCR[i].B.DDR_SEL = 0;    /* DDR3 */
                SIUL2.MSCR[i].B.DSE = 7;            
                SIUL2.MSCR[i].B.DDR_DO_TRIM = 1;
                SIUL2.MSCR[i].B.DCYCLE_TRIM = 1;
                SIUL2.MSCR[i].B.DDR_ODT = 2;
                SIUL2.MSCR[i].B.DDR_INPUT = 1;
                SIUL2.MSCR[i].B.PUS = 2;
            }
        break;
        case 1:
            SIUL2.MSCR[_DDR1_RESET].B.DDR_SEL = 0;      /* DDR3 */
            SIUL2.MSCR[_DDR1_RESET].B.DSE = 7;
            SIUL2.MSCR[_DDR1_RESET].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR1_RESET].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR1_RESET].B.PUS = 2;
            
            SIUL2.MSCR[_DDR1_CLK0].B.DDR_SEL = 0;       /* DDR3 */
            SIUL2.MSCR[_DDR1_CLK0].B.DSE = 7;
            SIUL2.MSCR[_DDR1_CLK0].B.CRPOINT_TRIM = 1;
            SIUL2.MSCR[_DDR1_CLK0].B.DCYCLE_TRIM = 0;
            SIUL2.MSCR[_DDR1_CLK0].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR1_CLK0].B.DDR_ODT = 1;
            SIUL2.MSCR[_DDR1_CLK0].B.PUS = 2;
            
            SIUL2.MSCR[_DDR1_CAS].B.DDR_SEL = 0;    /* DDR3 */
            SIUL2.MSCR[_DDR1_CAS].B.DSE = 7;
            SIUL2.MSCR[_DDR1_CAS].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR1_CAS].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR1_CAS].B.PUS = 2;
            
            SIUL2.MSCR[_DDR1_RAS].B.DDR_SEL = 0;    /* DDR3 */
            SIUL2.MSCR[_DDR1_RAS].B.DSE = 7;
            SIUL2.MSCR[_DDR1_RAS].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR1_RAS].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR1_RAS].B.PUS = 2;
            
            SIUL2.MSCR[_DDR1_WE_B].B.DDR_SEL = 0;   /* DDR3 */
            SIUL2.MSCR[_DDR1_WE_B].B.DSE = 7;
            SIUL2.MSCR[_DDR1_WE_B].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR1_WE_B].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR1_WE_B].B.PUS = 2;
            
            SIUL2.MSCR[_DDR1_CKE0].B.DDR_SEL = 0;   /* DDR3 */
            SIUL2.MSCR[_DDR1_CKE0].B.DSE = 7;
            SIUL2.MSCR[_DDR1_CKE0].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR1_CKE0].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR1_CKE0].B.PUS = 2;
            
            SIUL2.MSCR[_DDR1_CKE1].B.DDR_SEL = 0;   /* DDR3 */
            SIUL2.MSCR[_DDR1_CKE1].B.DSE = 7;
            SIUL2.MSCR[_DDR1_CKE1].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR1_CKE1].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR1_CKE1].B.PUS = 2;
            
            SIUL2.MSCR[_DDR1_CS_B0].B.DDR_SEL = 0;  /* DDR3 */
            SIUL2.MSCR[_DDR1_CS_B0].B.DSE = 7;
            SIUL2.MSCR[_DDR1_CS_B0].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR1_CS_B0].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR1_CS_B0].B.PUS = 2;
            
            SIUL2.MSCR[_DDR1_CS_B1].B.DDR_SEL = 0;  /* DDR3 */
            SIUL2.MSCR[_DDR1_CS_B1].B.DSE = 7;
            SIUL2.MSCR[_DDR1_CS_B1].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR1_CS_B1].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR1_CS_B1].B.PUS = 2;

            SIUL2.MSCR[_DDR1_BA0].B.DDR_SEL = 0;    /* DDR3 */
            SIUL2.MSCR[_DDR1_BA0].B.DSE = 7;            
            SIUL2.MSCR[_DDR1_BA0].B.DDR_DO_TRIM = 1;
            SIUL2.MSCR[_DDR1_BA0].B.DCYCLE_TRIM = 1;
            SIUL2.MSCR[_DDR1_BA0].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR1_BA0].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR1_BA0].B.PUS = 2;
            SIUL2.MSCR[_DDR1_BA1].B.DDR_SEL = 0;    /* DDR3 */
            SIUL2.MSCR[_DDR1_BA1].B.DSE = 7;            
            SIUL2.MSCR[_DDR1_BA1].B.DDR_DO_TRIM = 1;
            SIUL2.MSCR[_DDR1_BA1].B.DCYCLE_TRIM = 1;
            SIUL2.MSCR[_DDR1_BA1].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR1_BA1].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR1_BA1].B.PUS = 2;
            SIUL2.MSCR[_DDR1_BA2].B.DDR_SEL = 0;    /* DDR3 */
            SIUL2.MSCR[_DDR1_BA2].B.DSE = 7;            
            SIUL2.MSCR[_DDR1_BA2].B.DDR_DO_TRIM = 1;
            SIUL2.MSCR[_DDR1_BA2].B.DCYCLE_TRIM = 1;
            SIUL2.MSCR[_DDR1_BA2].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR1_BA2].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR1_BA2].B.PUS = 2;
            
            SIUL2.MSCR[_DDR1_DM0].B.DDR_SEL = 0;  /* DDR3 */
            SIUL2.MSCR[_DDR1_DM0].B.DSE = 7;
            SIUL2.MSCR[_DDR1_DM0].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR1_DM0].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR1_DM0].B.PUS = 2;
            SIUL2.MSCR[_DDR1_DM1].B.DDR_SEL = 0;  /* DDR3 */
            SIUL2.MSCR[_DDR1_DM1].B.DSE = 7;
            SIUL2.MSCR[_DDR1_DM1].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR1_DM1].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR1_DM1].B.PUS = 2;
            SIUL2.MSCR[_DDR1_DM2].B.DDR_SEL = 0;  /* DDR3 */
            SIUL2.MSCR[_DDR1_DM2].B.DSE = 7;
            SIUL2.MSCR[_DDR1_DM2].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR1_DM2].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR1_DM2].B.PUS = 2;
            SIUL2.MSCR[_DDR1_DM3].B.DDR_SEL = 0;  /* DDR3 */
            SIUL2.MSCR[_DDR1_DM3].B.DSE = 7;
            SIUL2.MSCR[_DDR1_DM3].B.DDR_ODT = 2;
            SIUL2.MSCR[_DDR1_DM3].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR1_DM3].B.PUS = 2;

            /* DQS Drive Strength */
            for (i=_DDR1_DQS0; i<(_DDR1_DQS0+4); i++)
            {
                SIUL2.MSCR[i].B.DDR_SEL = 0;       /* DDR3 */
                SIUL2.MSCR[i].B.DSE = 7;
                SIUL2.MSCR[i].B.CRPOINT_TRIM = 1;
                SIUL2.MSCR[i].B.DCYCLE_TRIM = 0;
                SIUL2.MSCR[i].B.DDR_INPUT = 1;
                SIUL2.MSCR[i].B.DDR_ODT = 1;
                SIUL2.MSCR[i].B.PUS = 2;
            }

            SIUL2.MSCR[_DDR1_ODT0].B.DDR_SEL = 0;   /* DDR3 */
            SIUL2.MSCR[_DDR1_ODT0].B.DSE = 7;               
            SIUL2.MSCR[_DDR1_ODT0].B.DDR_DO_TRIM = 1;
            SIUL2.MSCR[_DDR1_ODT0].B.DCYCLE_TRIM = 1;
            SIUL2.MSCR[_DDR1_ODT0].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR1_ODT0].B.PUS = 2;
            SIUL2.MSCR[_DDR1_ODT0].B.DDR_ODT = 1;
            
            SIUL2.MSCR[_DDR1_ODT1].B.DDR_SEL = 0;   /* DDR3 */
            SIUL2.MSCR[_DDR1_ODT1].B.DSE = 7;               
            SIUL2.MSCR[_DDR1_ODT1].B.DDR_DO_TRIM = 1;
            SIUL2.MSCR[_DDR1_ODT1].B.DCYCLE_TRIM = 1;
            SIUL2.MSCR[_DDR1_ODT1].B.DDR_INPUT = 1;
            SIUL2.MSCR[_DDR1_ODT1].B.PUS = 2;
            SIUL2.MSCR[_DDR1_ODT1].B.DDR_ODT = 1;

            /* Set address */
            for (i=_DDR1_A0; i<(_DDR1_A0+15); i++)
            {
                SIUL2.MSCR[i].B.DDR_SEL = 0;    /* DDR3 */
                SIUL2.MSCR[i].B.DSE = 7;            
                SIUL2.MSCR[i].B.DDR_DO_TRIM = 1;
                SIUL2.MSCR[i].B.DCYCLE_TRIM = 1;
                SIUL2.MSCR[i].B.DDR_ODT = 2;
                SIUL2.MSCR[i].B.DDR_INPUT = 1;
                SIUL2.MSCR[i].B.PUS = 2;
            }

            /* Set data */
            for (i=_DDR1_D0; i<(_DDR1_D0+32); i++)
            {
                SIUL2.MSCR[i].B.DDR_SEL = 0;    /* DDR3 */
                SIUL2.MSCR[i].B.DSE = 7;            
                SIUL2.MSCR[i].B.DDR_DO_TRIM = 1;
                SIUL2.MSCR[i].B.DCYCLE_TRIM = 1;
                SIUL2.MSCR[i].B.DDR_ODT = 2;
                SIUL2.MSCR[i].B.DDR_INPUT = 1;
                SIUL2.MSCR[i].B.PUS = 2;
            }
        break;
    }
}

/* CWL = 6 */
/* AL = 0 */
/* WL = CWL+AL */
/* CL = 7 */
void config_ddr3(uint8_t module)
{
    struct MMDC_tag* MMDC;
    
    switch(module)
    {
        default:
        case 0: MMDC = (struct MMDC_tag*)0x40036000UL;
        break;
        case 1: MMDC = (struct MMDC_tag*)0x400A2000UL;
        break;
    }

    MMDC->MDSCR.R = 0x00008000;	/* Set MDSCR[CON_REQ] (configuration request) */
    
    MMDC->MDCFG0.R = 0x8F9598F4;	/* tRFC=144 (270ns),tXS=150 (tRFC+10ns),tXP=4 (6ns),tXPDLL=13 (24ns),tFAW=16(30ns),tCL=7 */
    MMDC->MDCFG1.R = 0xFF328E64;	/* tRCD=8 (13.75ns),tRP=6 (13,75ns),tRC=26 (48.75ns),tRAS=19 (35ns),tRPA=1,tWR=8 (15ns),tMRD=4,tCWL=6 */
    MMDC->MDCFG2.R = 0x01FF00DB;	/* tDLLK=512,tRTP=4,tWTR=4,tRRD=4 */
    MMDC->MDOTC.R = 0x09444030;	/* tAOFPD=2,tAONPD=2,tANPD=5 (tCWL-1),tAXPD=5 (tCWL-1),tODTLon=5 (WL-2),tODT_idle_off=0 */
    MMDC->MDMISC.R = 0x00011640;	/* WALAT=1, BI bank interleave on, MIF3=3, RALAT=1, 8 banks, DDR3 */
    MMDC->MDOR.R = 0x009A1023;	/* tXPR=155 (tRFC+10ns), SDE_to_RST=14, RST_to_CKE=33 */
    MMDC->MDCTL.R = 0x04190000;

    /* Perform ZQ calibration */
    MMDC->MPZQHWCTRL.R = 0xA1390003;	/* Force h/w calibration */
    while (MMDC->MPZQHWCTRL.R & (1 << 16)) {}

    /* Enable MMDC with CS0 */
    MMDC->MDCTL.R = 0x04190000 + 0x80000000;

    /* Complete the initialization sequence as defined by JEDEC */
    MMDC->MDSCR.R = 0x00088032;		/* Configure MR2: CWL=6, self-refresh=off, self-refresh temp=normal */
    MMDC->MDSCR.R = 0x00008033;		/* Configure MR3: normal operation */
    MMDC->MDSCR.R = 0x00068031;		/* Configure MR1: enable DLL, drive strength=34R, AL off, ODT=60R, write levelling off, TDQS=0, Qoff=on */
    MMDC->MDSCR.R = 0x05308030;		/* Configure MR0: BL=8, CL=7, DLL reset, write recovery=6, precharge PD off */
    MMDC->MDSCR.R = 0x04008040;		/* DDR ZQ calibration */
    
    /* Set the amount of DRAM */
    /* Set DQS settings based on board type */
    switch(module)
    {
    default:
    case 0:
        MMDC->MDASP.R = 0x0000007F;		/* 1 GB memory */
        MMDC->MPRDDLCTL.R = 0x44434442;	/* Read delay line offsets */
        MMDC->MPWRDLCTL.R = 0x42434041;	/* Write delay line offsets */
        MMDC->MPDGCTRL0.R = 0x41470138;	/* Read DQS gating control 0 */
        MMDC->MPDGCTRL1.R = 0x012E0135;	/* Read DQS gating control 1 */
    break;
    case 1:
        MMDC->MDASP.R = 0x0000007F;		/* 1 GB memory */
        MMDC->MPRDDLCTL.R = 0x44434442;	/* Read delay line offsets */
        MMDC->MPWRDLCTL.R = 0x3F3F4242;	/* Write delay line offsets */
        MMDC->MPDGCTRL0.R = 0x414B0138;	/* Read DQS gating control 0 */
        MMDC->MPDGCTRL1.R = 0x01320142;	/* Read DQS gating control 1 */
    break;
    }

    MMDC->MDRWD.R = 0x000026D2;		/* Read/write command delay - default */

    MMDC->MDPDC.R = 0x00020024;	/* Power down control */
    MMDC->MDREF.R = 0x30B00000 ;	/* Refresh control */
    MMDC->MPODTCTRL.R = 0x0002222F;	/* 60R nominal */
    MMDC->MDSCR.R = 0x00000000;	/* Deassert the configuration request */
}

