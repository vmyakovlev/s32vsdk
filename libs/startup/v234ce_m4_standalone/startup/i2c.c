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

/********************************************************************
 * File:		serial.c
 * Purpose:		Board specific routines for the Treerunner
 ********************************************************************/

#include "config.h"
#include "i2c.h"
#include "s32vs234.h"

vuint8_t slave;


void I2C_Pin_Init(vuint32_t i2c_module)
{

    switch(i2c_module){
#ifdef OLD_EVB
      case I2C0:

	    ///////////////// I2C_0 - MIPI camera ////////////////////////////////////////

	    // SDA    
	    SIUL2.MSCR[15].B.MUX_MODE = 1;	// GPIO mux mode selected
	    SIUL2.MSCR[15].B.OBE = 1;		// output buffer enabled
	    SIUL2.MSCR[15].B.IBE = 1;		// input buffer enabled
	    SIUL2.MSCR[15].B.ODE = 1;		// open drain enabled
	    SIUL2.MSCR[15].B.DSE = 7;	// maximum drive strenght

	    SIUL2.IMCR[269].B.DAISY = 2;	// PA15 selected as input*/

	    // SCL
	    SIUL2.MSCR[16].B.MUX_MODE = 1;	// GPIO mux mode selected
	    SIUL2.MSCR[16].B.OBE = 1;		// output buffer enabled
	    SIUL2.MSCR[16].B.IBE = 1;		// input buffer enabled
	    SIUL2.MSCR[16].B.ODE = 1;		// open drain enabled*/
	    SIUL2.MSCR[16].B.DSE = 7;	// maximum drive strenght

	    SIUL2.IMCR[268].B.DAISY = 2;	// PB0 selected as input*/

	    break;

	case I2C1:


	    // SDA
/*    SIUL2.MSCR[17].B.MUX_MODE = 0;	// GPIO mux mode selected
 -     SIUL2.MSCR[17].B.OBE = 1;		// output buffer enabled
 -     SIUL2.MSCR[17].B.IBE = 1;		// input buffer enabled
 -     SIUL2.MSCR[17].B.ODE = 1;		// open drain enabled*/
	    *((vuint32_t*)(0x4006C284)) = 0x380000;

/*    SIUL2.IMCR[271].DAISY = 2;			// PB1 selected as input*/
	    *((vuint32_t*)(0x4006CE7C)) = 0x02; 

	    // SCL
/*    SIUL2.MSCR[18].B.MUX_MODE = 0;	// GPIO mux mode selected
 -     SIUL2.MSCR[18].B.OBE = 1;		// output buffer enabled
 -     SIUL2.MSCR[18].B.IBE = 1;		// input buffer enabled
 -     SIUL2.MSCR[18].B.ODE = 1;		// open drain enabled*/
	    *((vuint32_t*)(0x4006C288)) = 0x380000;

/*    SIUL2.IMCR[271].DAISY = 2;			// PB2 selected as input*/
	    *((vuint32_t*)(0x4006CE78)) = 0x02;

	    break;
#else
     case I2C0:

      ///////////////// I2C_0 - MIPI camera ////////////////////////////////////////

      // SDA    
      SIUL2.MSCR[99].B.MUX_MODE = 2;  // GPIO mux mode selected
      SIUL2.MSCR[99].B.OBE = 1;   // output buffer enabled
      SIUL2.MSCR[99].B.IBE = 1;   // input buffer enabled
      SIUL2.MSCR[99].B.ODE = 1;   // open drain enabled
      SIUL2.MSCR[99].B.DSE = 7; // maximum drive strenght

      SIUL2.IMCR[269].B.DAISY = 3;  // PA15 selected as input*/

      // SCL
      SIUL2.MSCR[100].B.MUX_MODE = 2;  // GPIO mux mode selected
      SIUL2.MSCR[100].B.OBE = 1;   // output buffer enabled
      SIUL2.MSCR[100].B.IBE = 1;   // input buffer enabled
      SIUL2.MSCR[100].B.ODE = 1;   // open drain enabled*/
      SIUL2.MSCR[100].B.DSE = 7; // maximum drive strenght

      SIUL2.IMCR[268].B.DAISY = 3;  // PB0 selected as input*/

      break;

  case I2C1:


      ///////////////// I2C_1 - MIPI camera ////////////////////////////////////////

      // SDA    
      SIUL2.MSCR[101].B.MUX_MODE = 2;  // GPIO mux mode selected
      SIUL2.MSCR[101].B.OBE = 1;   // output buffer enabled
      SIUL2.MSCR[101].B.IBE = 1;   // input buffer enabled
      SIUL2.MSCR[101].B.ODE = 1;   // open drain enabled
      SIUL2.MSCR[101].B.DSE = 7; // maximum drive strenght

      SIUL2.IMCR[271].B.DAISY = 3;  // PA15 selected as input*/

      // SCL
      SIUL2.MSCR[102].B.MUX_MODE = 2;  // GPIO mux mode selected
      SIUL2.MSCR[102].B.OBE = 1;   // output buffer enabled
      SIUL2.MSCR[102].B.IBE = 1;   // input buffer enabled
      SIUL2.MSCR[102].B.ODE = 1;   // open drain enabled*/
      SIUL2.MSCR[102].B.DSE = 7; // maximum drive strenght

      SIUL2.IMCR[270].B.DAISY = 3;  // PB0 selected as input*/
      break;
#endif // ifdef OLD_EVB      

	case I2C2:
	    //SDA
	    SIUL2.MSCR[19].B.MUX_MODE = 1;	// GPIO mux mode selected
	    SIUL2.MSCR[19].B.OBE = 1;		// output buffer enabled
	    SIUL2.MSCR[19].B.IBE = 1;		// input buffer enabled
	    SIUL2.MSCR[19].B.ODE = 1;		// open drain enabled
	    SIUL2.MSCR[19].B.DSE = 7;	// maximum drive strenght

	    SIUL2.IMCR[273].B.DAISY = 2;	// PA15 selected as input*/

	    // SCL
	    SIUL2.MSCR[20].B.MUX_MODE = 1;	// GPIO mux mode selected
	    SIUL2.MSCR[20].B.OBE = 1;		// output buffer enabled
	    SIUL2.MSCR[20].B.IBE = 1;		// input buffer enabled
	    SIUL2.MSCR[20].B.ODE = 1;		// open drain enabled*/
	    SIUL2.MSCR[20].B.DSE = 7;	// maximum drive strenght

	    SIUL2.IMCR[272].B.DAISY = 2;	// PB0 selected as input*/

	    break;

    }
}

void I2C_Init_Config(vuint8_t i2c_module, vuint8_t slave_address)
{
    I2C_Pin_Init(i2c_module);

    switch(i2c_module){
	case I2C0:

	    // I2C_0 init - camera
	    I2C_0.IBCR.B.TXRX = 1;	// set as TX
	    while(I2C_0.IBSR.B.IBB == 1) {};	// Wait for the bus to become free
	    I2C_0.IBFD.B.IBC = 0x35;  // 0x35 corresponds to clock divider of 1280. module clk is sys6_clk = 133MHz/1280 = 101.471 Kbps
	    I2C_0.IBSR.B.IBIF = 1;  // Clear IBIF
	    I2C_0.IBCR.R = 0x10;	// Set as Tx

	    break;

	case I2C1:
	    I2C_1.IBCR.B.TXRX = 1;	// I2C module disable
	    while(I2C_1.IBSR.B.IBB == 1) {};	// Wait for the bus to become free
	    I2C_1.IBFD.B.IBC = 0x35;  // 0x35 corresponds to clock divider of 1280. module clk is sys6_clk = 133MHz/1280 = 101.471 Kbps
	    I2C_1.IBSR.B.IBIF = 1;  // Clear IBIF
	    I2C_1.IBCR.R = 0x10;	// Set as Tx
	    break;

case I2C2:
	    I2C_2.IBCR.B.TXRX = 1;	// I2C module disable
	    while(I2C_2.IBSR.B.IBB == 1) {};	// Wait for the bus to become free
	    I2C_2.IBFD.B.IBC = 0x35;  // 0x35 corresponds to clock divider of 1280. module clk is sys6_clk = 133MHz/1280 = 101.471 Kbps
	    I2C_2.IBSR.B.IBIF = 1;  // Clear IBIF
	    I2C_2.IBCR.R = 0x10;	// Set as Tx
	    break;

    }
    	    slave = slave_address;
}

void I2C_start(vuint8_t module, vuint8_t slave_addr, vuint8_t operation)
{
    switch(module){
	case I2C0:
	
	    while(I2C_0.IBSR.B.IBB == 1) {};	// Wait for the bus to become free
	    I2C_0.IBCR.R = 0x30;	// I2C_0 is Master & Generate Start, Tx mode selected
	    while(I2C_0.IBSR.B.IBB == 0) {};	// Wait for the bus to become busy - wait for start signal to be generated
	    // Device address for Slave
	    I2C_0.IBDR.R = slave_addr | operation ;	// OMV Slave Addr + Wr
	    while(I2C_0.IBSR.B.TCF == 0);	// Wait for transfer to complete 
	    while(I2C_0.IBSR.B.IBIF == 0);	// Wait for corresponding interrupt to set
	    while(I2C_0.IBSR.B.RXAK == 1);	// Wait for slave acknowledge
	
	    break;

	case I2C1:
	    
	    while(I2C_1.IBSR.B.IBB == 1) {};	// Wait for the bus to become free
	    I2C_1.IBCR.R = 0x30;	// I2C_0 is Master & Generate Start, Tx mode selected
	    while(I2C_1.IBSR.B.IBB == 0) {};	// Wait for the bus to become busy - wait for start signal to be generated
	    // Device address for Slave
	    I2C_1.IBDR.R = slave_addr | operation ;	// OMV Slave Addr + Wr
	    while(I2C_1.IBSR.B.TCF == 0);	// Wait for transfer to complete 
	    while(I2C_1.IBSR.B.IBIF == 0);	// Wait for corresponding interrupt to set
	    while(I2C_1.IBSR.B.RXAK == 1);	// Wait for slave acknowledge
	    
	    break;

	case I2C2:

	    while(I2C_2.IBSR.B.IBB == 1) {};	// Wait for the bus to become free
	    I2C_2.IBCR.R = 0x30;	// I2C_0 is Master & Generate Start, Tx mode selected
	    while(I2C_2.IBSR.B.IBB == 0) {};	// Wait for the bus to become busy - wait for start signal to be generated
	    // Device address for Slave
	    I2C_2.IBDR.R = slave_addr | operation ;	// OMV Slave Addr + Wr
	    while(I2C_2.IBSR.B.TCF == 0);	// Wait for transfer to complete 
	    while(I2C_2.IBSR.B.IBIF == 0);	// Wait for corresponding interrupt to set
	    while(I2C_2.IBSR.B.RXAK == 1);	// Wait for slave acknowledge

	    break;
    }
}

void I2C_write(vuint8_t module, vuint8_t data)
{
    switch(module){
	case I2C0:
	
	    I2C_0.IBCR.R = 0x30;	// I2C_0 is master, Tx mode selected, generate start
	    I2C_0.IBDR.R = data;
	    while(I2C_0.IBSR.B.TCF == 0);	// Wait for transfer to complete 
	    while(I2C_0.IBSR.B.IBIF == 0);	// Wait for corresponding interrupt to set
	    while(I2C_0.IBSR.B.RXAK == 1);	// Wait for slave acknowledge
	    
	    break;

	case I2C1:
	    I2C_1.IBCR.R = 0x30;	// I2C_0 is master, Tx mode selected, generate start
	    I2C_1.IBDR.R = data;
	    while(I2C_1.IBSR.B.TCF == 0);	// Wait for transfer to complete 
	    while(I2C_1.IBSR.B.IBIF == 0);	// Wait for corresponding interrupt to set
	    while(I2C_1.IBSR.B.RXAK == 1);	// Wait for slave acknowledge

	    break;

	case I2C2:
	    I2C_2.IBCR.R = 0x30;	// I2C_0 is master, Tx mode selected, generate start
	    I2C_2.IBDR.R = data;
	    while(I2C_2.IBSR.B.TCF == 0);	// Wait for transfer to complete 
	    while(I2C_2.IBSR.B.IBIF == 0);	// Wait for corresponding interrupt to set
	    while(I2C_2.IBSR.B.RXAK == 1);	// Wait for slave acknowledge

	    break;
    }    	    
}

vuint8_t I2C_read(vuint8_t module, vuint8_t sl_address)
{
    vuint8_t dummy;
    switch(module){
	case I2C0:
	    I2C_0.IBSR.R= 0xff;
	    I2C_0.IBCR.R = 0x34;	// I2C_0 is master, Tx mode selected, repeated start
	  //  I2C_0.IBDR.R = (sl_address << 1)|READ;
	    I2C_0.IBDR.R = sl_address|READ;
	    while(I2C_0.IBSR.B.TCF == 0);	// Wait for transfer to complete 
	    while(I2C_0.IBSR.B.IBIF == 0);	// Wait for corresponding interrupt to set
	    while(I2C_0.IBSR.B.RXAK == 1);	// Wait for slave acknowledge

	    I2C_0.IBSR.B.IBIF = 1;
	    I2C_0.IBCR.R = 0x28;	// master & no ACK
	    dummy = I2C_0.IBDR.R; 
	    while(I2C_0.IBSR.B.TCF == 0);	// Wait for transfer to complete
	    while(I2C_0.IBSR.B.IBIF == 0);	// Wait for corresponding interrupt to set
	    I2C_0.IBCR.B.MSSL = 0;	// disable I2C_0 module - generate STOP condition
	    dummy = I2C_0.IBDR.R;  	// read received data
	    
	    break;

	case I2C1:
	    I2C_1.IBSR.R= 0xff;
	    I2C_1.IBCR.R = 0x34;	// I2C_0 is master, Tx mode selected, repeated start
	    I2C_1.IBDR.R = sl_address|READ;
	    while(I2C_1.IBSR.B.TCF == 0);	// Wait for transfer to complete 
	    while(I2C_1.IBSR.B.IBIF == 0);	// Wait for corresponding interrupt to set
	    while(I2C_1.IBSR.B.RXAK == 1);	// Wait for slave acknowledge

	    I2C_1.IBSR.B.IBIF = 1;
	    I2C_1.IBCR.R = 0x28;	// master & no ACK
	    dummy = I2C_1.IBDR.R; 
	    while(I2C_1.IBSR.B.TCF == 0);	// Wait for transfer to complete
	    while(I2C_1.IBSR.B.IBIF == 0);	// Wait for corresponding interrupt to set
	    I2C_1.IBCR.B.MSSL = 0;	// disable I2C_0 module - generate STOP condition
	    dummy = I2C_1.IBDR.R;  	// read received data
	    
	    break;

	case I2C2:
	    I2C_2.IBSR.R= 0xff;
	    I2C_2.IBCR.R = 0x34;	// I2C_0 is master, Tx mode selected, repeated start
	    I2C_2.IBDR.R = sl_address|READ;
	    while(I2C_2.IBSR.B.TCF == 0);	// Wait for transfer to complete 
	    while(I2C_2.IBSR.B.IBIF == 0);	// Wait for corresponding interrupt to set
	    while(I2C_2.IBSR.B.RXAK == 1);	// Wait for slave acknowledge

	    I2C_2.IBSR.B.IBIF = 1;
	    I2C_2.IBCR.R = 0x28;	// master & no ACK
	    dummy = I2C_2.IBDR.R; 
	    while(I2C_2.IBSR.B.TCF == 0);	// Wait for transfer to complete
	    while(I2C_2.IBSR.B.IBIF == 0);	// Wait for corresponding interrupt to set
	    I2C_2.IBCR.B.MSSL = 0;	// disable I2C_0 module - generate STOP condition
	    dummy = I2C_2.IBDR.R;  	// read received data

	    break;
    }
    return dummy;
}

void I2C_stop(vuint8_t module)
{
    switch(module){
	case I2C0:
	
	    while(I2C_0.IBSR.B.TCF == 0);	// Wait for transfer to complete 
	    while(I2C_0.IBSR.B.IBIF == 0);	// Wait for corresponding interrupt to set
	    while(I2C_0.IBSR.B.RXAK == 1);	// Wait for slave acknowledge

	    I2C_0.IBCR.B.MSSL = 0x0;	// Generate Stop
	    I2C_0.IBSR.R = 0xff;	// clear All status flags 
	
	    break;
	    
	case I2C1:

	    while(I2C_1.IBSR.B.TCF == 0);	// Wait for transfer to complete 
	    while(I2C_1.IBSR.B.IBIF == 0);	// Wait for corresponding interrupt to set
	    while(I2C_1.IBSR.B.RXAK == 1);	// Wait for slave acknowledge

	    I2C_1.IBCR.B.MSSL = 0x0;	// Generate Stop
	    I2C_1.IBSR.R = 0xff;	// clear All status flags 
	
	    break;
    	case I2C2:

	    while(I2C_2.IBSR.B.TCF == 0);	// Wait for transfer to complete 
	    while(I2C_2.IBSR.B.IBIF == 0);	// Wait for corresponding interrupt to set
	    while(I2C_2.IBSR.B.RXAK == 1);	// Wait for slave acknowledge

	    I2C_2.IBCR.B.MSSL = 0x0;	// Generate Stop
	    I2C_2.IBSR.R = 0xff;	// clear All status flags 
	
	    break;
    }
}

void I2C8_TX(vuint8_t module, vuint8_t adr_byte, vuint8_t data_byte)
{
    I2C_start(module,slave,WRITE);
    I2C_write(module,adr_byte);
    I2C_write(module,data_byte);
    I2C_stop(module);
}

void I2C_Tx(vuint8_t module, vuint8_t adr_byte1, vuint8_t adr_byte2, vuint8_t data_byte)
{	
    I2C_start(module,slave,WRITE);
    I2C_write(module,adr_byte1);
    I2C_write(module,adr_byte2);
    I2C_write(module,data_byte);
    I2C_stop(module);
}


vuint8_t I2C_Rx(vuint8_t module, vuint8_t adr_byte1, vuint8_t adr_byte2)
{
    vuint8_t data;
    I2C_start(module,slave,WRITE);
    I2C_write(module,adr_byte1);
    I2C_write(module,adr_byte2);
    data = I2C_read(module,slave);
    return data;

}


