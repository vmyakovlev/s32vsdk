/*
 * File: SetupLPDDR2.h
 * 
 * Purpose: Sets up LPDDR2 
 * 
 * Author: ttz778
 */
#ifndef SETUP_LPDDR2_H
#define SETUP_LPDDR2_H

#include "typedefs.h"

uint32_t probe_lpddr2(uint8_t, uint8_t);
/* Function to setup LPDDR2, may need a delay until LPDDR2 is ready */
void config_lpddr2(uint8_t);

#endif
