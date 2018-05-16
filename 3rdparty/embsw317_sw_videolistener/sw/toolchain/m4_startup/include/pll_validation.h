/*
 * File:		pll.h
 * Purpose:		PLL routines
 *
 */
  
#ifndef _PLL_H
#define _PLL_H
#include "typedefs.h"

#define RESET_M 0
#define TEST_M 1
#define DRUN_M 3
#define RUN0_M 4
#define RUN1_M 5
#define RUN2_M 6
#define RUN3_M 7
/********************************************************************/
void pll_init(void);
void clkout_settings (uint32_t clkout_value);

/********************************************************************/


#endif
