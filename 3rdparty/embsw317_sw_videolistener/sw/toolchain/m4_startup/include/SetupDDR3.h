/*
 * File: SetupDDR3.h
 * 
 * Purpose: Sets up LPDDR2 
 * 
 * Author: ttz778
 */
#ifndef SETUP_DDR3_H
#define SETUP_DDR3_H

#include "typedefs.h"
/* Define which board you are using */

/* Function to setup DDR3, may need a delay until DDR3 is ready */
void config_ddr3(uint8_t);

#endif
