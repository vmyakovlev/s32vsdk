/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/
  
#ifndef I2C_H
#define I2C_H
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

#endif /* I2C_H */
