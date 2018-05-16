/*
 * File:		i2c.h
 * Purpose:		Internal Integrated Circuit bus routines
 *
 */
  
#ifndef _I2C_H
#define _I2C_H
#include "typedefs.h"


/********************************************************************/
void I2C_Pin_Init(vuint32_t i2c_module);
void I2C_Init_Config(vuint8_t i2c_module, vuint8_t slave_address);

void I2C_start(vuint8_t module, vuint8_t slave_addr, vuint8_t operation);
void I2C_write(vuint8_t module, vuint8_t data);
void I2C_stop(vuint8_t module);
vuint8_t I2C_read(vuint8_t module, vuint8_t sl_address);

void I2C_Tx(vuint8_t module, vuint8_t adr_byte1, vuint8_t adr_byte2, vuint8_t data_byte);
vuint8_t I2C_Rx(vuint8_t module, vuint8_t adr_byte1, vuint8_t adr_byte2);
void I2C8_TX(vuint8_t module, vuint8_t adr_byte, vuint8_t data_byte);
/********************************************************************/
#define I2C0	0
#define I2C1	1
#define I2C2	2

#define WRITE	0
#define READ	1

#define OMV_CAM_ADDR 0x30
#define SONY_CAM_ADDR 0x34
#define HDMI_ADDR 0x72

#endif
