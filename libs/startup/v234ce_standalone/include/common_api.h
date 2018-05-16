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

#ifndef COMMONAPI_H
#define COMMONAPI_H

/*******************************************************************************
*
* Copyright (C) 2011, FREESCALE SEMICONDUCTOR INC.   All Rights Reserved
*
*  FILE NAME      :  .h
*
*  DESCRIPTION:
*  This module contains defines for common API.
*
*  REVISION HISTORY:
*
********************************************************************************
*  I N C L U D E  F I L E S
*******************************************************************************/
//#include "interrupts.h"
//#include <stdio.h>

/*******************************************************************************
*  T Y P E   D E F I N E S
*******************************************************************************/
typedef volatile unsigned long WORD;
typedef volatile unsigned short HALF;
typedef volatile unsigned char BYTE;

/*******************************************************************************
*  B A S I C   R E A D / W R I T E   D E F I N I T I O N S
*******************************************************************************/
#define reg8_read(addr)       *((BYTE *)(addr))
#define reg8_write(addr,val)  *((BYTE *)(addr)) = (val)
#define mem8_read(addr)	reg8_read(addr)
#define mem8_write(addr,val)	reg8_write(addr,val)

#define reg16_read(addr)      *((HALF *)(addr))
#define reg16_write(addr,val) *((HALF *)(addr)) = (val)
#define mem16_read(addr)	reg16_read(addr)
#define mem16_write(addr,val)	reg16_write(addr,val)

#define reg32_read(addr)      *((WORD *)(addr))
#define reg32_write(addr,val) *((WORD *)(addr)) = (val)
#define mem32_read(addr)	reg32_read(addr)
#define mem32_write(addr,val)	reg32_write(addr,val)


/*******************************************************************************
*
*  ROUTINE NAME   : regX_read_tst
*
*  DESCRIPTION:
*     Read data from a given meemory-mapped register,and check Vs. expected data.
*
*  INPUT:
*     reg_addr - the address of the register from which the data is read
*     expect_val - the value we expect to be read
*     dont_care - dont_careing specific bits so that they will not be compared
*
*  OUTPUT:
*
*  GLOBAL DATA: none
*
*  EXAMPLE:
*     reg8_read_tst(OWIRE_TIME_DIV,0x12,0ff)
*     Here we read 8 bits from an 8-bit register OWIRE_TIME_DIV of OWIRE,
*     and expect all 8 bits to be equal to 0x12
*
*     reg16_read_tst(WDOG_WSR,0x1212,0x00ff)
*     Here we read 16 bits from a 16-bit register WDOG_WSR of WDOG,
*     and expect only the lower 8 bits to be equal to 0x12
*
*******************************************************************************/
#define reg8_read_tst(reg_addr,expect_val,dont_care) \
        (byte_compare_results(reg_addr, expect_val & \
        dont_care,reg8_read(reg_addr) & dont_care))
#define reg16_read_tst(reg_addr,expect_val,dont_care) \
        (half_compare_results(reg_addr, expect_val & \
        dont_care,reg16_read(reg_addr) & dont_care))
#define reg32_read_tst(reg_addr,expect_val,dont_care) \
        (word_compare_results(reg_addr, expect_val & \
        dont_care,reg32_read(reg_addr) & dont_care))

/*******************************************************************************
*
*  ROUTINE NAME   : regXsetbit
*
*  DESCRIPTION:
*     Set a specific bit in a register
*
*  INPUT:
*     addr - address of the register
*     bitpos - position of the bit (the first bit is 0)
*
*  OUTPUT:
*
*  GLOBAL DATA: none
*
*  EXAMPLE:
*     reg16setbit(UART_SAMPLE, 3)
*     This will set bit 3 of the register, so if the register was 0x0010, it is
*     now 0x0018
*
*******************************************************************************/
#define  reg32setbit(addr,bitpos) \
         reg32_write((addr),(reg32_read((addr)) | (1<<(bitpos))))
#define  reg16setbit(addr,bitpos) \
         reg16_write((addr),(reg16_read((addr)) | (1<<(bitpos))))
#define  reg8setbit(addr,bitpos) \
         reg8_write((addr),(reg8_read((addr)) | (1<<(bitpos))))

/*******************************************************************************
*
*  ROUTINE NAME   : regXclrbit
*
*  DESCRIPTION:
*     Clear a specific bit in a register
*
*  INPUT:
*     addr - address of the register
*     bitpos - position of the bit (the first bit is 0)
*
*  OUTPUT:
*
*  GLOBAL DATA: none
*
*  EXAMPLE:
*     reg16clrbit(UART_SAMPLE, 3)
*     This will set bit 3 of the register, so if the register was 0x00FF, it is
*     now 0x00F7
*
*******************************************************************************/
#define  reg32clrbit(addr,bitpos) \
         reg32_write((addr),(reg32_read((addr)) & (0xFFFFFFFF ^ (1<<(bitpos)))))
#define  reg16clrbit(addr,bitpos) \
         reg16_write((addr),(reg16_read((addr)) & (0xFFFF ^ (1<<(bitpos)))))
#define  reg8clrbit(addr,bitpos) \
         reg8_write((addr),(reg8_read((addr)) & (0xFF ^ (1<<(bitpos)))))


/*******************************************************************************
*
*  ROUTINE NAME   : capture_interrupt
*
*  DESCRIPTION: Sets up the user defined handler for an interrupt
*
*  INPUT:
*	  int_num - Interrupt number
*     isr - Name of Interrupt service routine
*     cpu_num - Core ID (0-3)
*
*  OUTPUT:
*
*  GLOBAL DATA: none
*
*  EXAMPLE:
*     CAPTURE_INTERRUPT(GPIO3_ROUTINE,my_interrupt_handler);
*     Sets up the user defined function "my_interrupt_handler" to be the
*     handler for the GPIO3 interrupt
*
*******************************************************************************/
//void capture_interrupt(mx6q_interrupts int_num, int_hdlr_t isr, CoreId cpu_num);

/*******************************************************************************
*
*  ROUTINE NAME   : compare_array
*
*  DESCRIPTION:
*     Compares the data in two arrays.  Will trigger a VT_FAIL event whenever
*     a mismatch occurs
*
*  INPUT:
*     *exp_val - Pointer to an array of expected values
*     *res_val - Pointer to an array of resulting values
*     num_of_res - the number of values to compare
*
*  OUTPUT:
*
*  GLOBAL DATA: none
*
*  EXAMPLE:
*
*     WORD expected_data[10] = {<exp_values>};
*     WORD resulting_data[10];
*
*     Test Code
*
*     compare_array(&expected_data,&resulting_data,10);
*
*******************************************************************************/
WORD compare_array(WORD *exp_val,WORD *res_val, int num_of_res);

/*******************************************************************************
*
*  ROUTINE NAME   : init_array
*
*  DESCRIPTION:
*     Populates the two arrays with different values (exp gets '1's while res
*     gets '0's
*
*  INPUT:
*     *exp_val - Pointer to an array of expected values
*     *res_val - Pointer to an array of resulting values
*     num_of_res - the number of values to compare
*
*  OUTPUT:
*
*  GLOBAL DATA: none
*
*  EXAMPLE:
*
*     WORD expected_data[10];
*     WORD resulting_data[10];
*
*     init_array(&expected_data,&resulting_data,10);
*
*******************************************************************************/
void init_array(WORD *exp_val,WORD *res_val, int num_of_res);

/*******************************************************************************
*
*  ROUTINE NAME   : <type>_compare_results
*
*  DESCRIPTION:
*     Compares value a and b and prints either the info message or the error
*     message with the reg_addr printed in the message.  This does not read
*     from the address.
*
*  INPUT:
*     info - A word signaling which event number this is
*     <type>a - expected data
*     <type>b - read data
*
*  OUTPUT:
*     int - 1 if failed, 0 if passed
*
*  GLOBAL DATA: none
*
*  EXAMPLE:
*
*     value = reg32_read(UART_REG);
*     word_compare_results(0x1, value,0x00000002);
*     This will pass if value is equal to 0x00000002.
*
*******************************************************************************/
WORD word_compare_results (WORD info, WORD worda , WORD wordb);
WORD half_compare_results (WORD info, HALF halfa , HALF halfb);
WORD byte_compare_results (WORD info, BYTE bytea , BYTE byteb);

/*******************************************************************************
*
*  ROUTINE NAME   : regX_rw_test
*
*  DESCRIPTION:
*     This function will write wdata to the addr.  It will then read the data
*     back, and it with dont_care, and compare it to exp_rdata.  Pass the make_arg
*     "DEBUG" if you want info messages to print.
*
*  INPUT:
*     addr - address to read/write the data
*     wdata - data to be written
*     exp_rdata - expected value of the data after it is read back
*     dont_care - 1 for important bits, 0 for unimportant bits
*
*  OUTPUT:
*     WORD - 0 if failed, 1 if passed
*
*  GLOBAL DATA: none
*
*  EXAMPLE:
*
*     reg16_rw_tst(UART_REG,0x0234,0x0014,0x00FF);
*     This will pass if 0x??14 is read back from the register.  It will fail
*     otherwise, trigerring a fail event.
*
*******************************************************************************/
WORD reg8_rw_tst(WORD addr, BYTE wdata, BYTE exp_rdata,BYTE dont_care);
WORD reg16_rw_tst(WORD addr, HALF wdata, HALF exp_rdata, HALF dont_care);
WORD reg32_rw_tst(WORD addr, WORD wdata, WORD exp_rdata, WORD dont_care);

/*******************************************************************************
*
*  ROUTINE NAME   : regX_checkerboard_test
*
*  DESCRIPTION:
*     This function will write the values 0xffffffff, 0xaaaaaaaa, 0x55555555,
*     0x00000000 to the register and read back to ensure that the proper bits
*     changed values.
*
*  INPUT:
*     addr - address to read/write the data
*     writeable_bits - the bits that will toggle values when written to (0 if not
*        writeable, 1 if writeable
*     dont_care - bits that you dont care to check( 1 if care, 0 if dont)
*
*  OUTPUT:
*     WORD - 0 if failed, 1 if passed
*
*  GLOBAL DATA: none
*
*  EXAMPLE:
*
*     reg32_checkerboard_tst(UART_REG, 0x0000FFFF, 0xFFFFFFFF);
*     The above code will ensure that when the checkerboard values are written,
*     the upper 16 bits will always return 0.
*
*******************************************************************************/
WORD reg8_checkerboard_tst(WORD addr, BYTE writeable_bits, BYTE dont_care);
WORD reg16_checkerboard_tst(WORD addr, HALF writeable_bits, HALF dont_care);
WORD reg32_checkerboard_tst(WORD addr, WORD writeable_bits, WORD dont_care);

/*******************************************************************************
*
*  ROUTINE NAME   : enable/disable_hdler
*
*  DESCRIPTION:
*     Enables / Disables the given interrupt.  The list of interrupts should
*     be defined in interrupts.h
*
*  INPUTS:
*     int_num - This variable is defined for each interrupt in interrupts.h.
*     It is <int_name>_INT
*     cpu_num - Which CPU will handle this interrupt
*
*  OUTPUT:
*
*  GLOBAL DATA: none
*
*  EXAMPLE:
*
*     enable_hdler(UART_TX_INT);
*     This will enable the UART_TX interrupt
*
*******************************************************************************/
//void enable_hdler(WORD int_num, CoreId cpu_num);
//void disable_hdler(WORD int_num, CoreId cpu_num);

/*******************************************************************************
*
*  ROUTINE NAME   : init_interrupt
*
*  DESCRIPTION:
*     Initializes interrupt controller - all interrupts are enabled and
*     configured as non-secure, but are not handled by any CPU.
*     Vectors table in RAM is configured to point to relevant routine.
*     interrupt_handlers array is initialized to point to 'default_int_routine'
*
*  INPUTS:
*
*  OUTPUT:
*
*******************************************************************************/
//void init_interrupts(void);

/*******************************************************************************
*
*  ROUTINE NAME   : default_int_routine
*
*  DESCRIPTION:
*     Default interrupt routine that is called by any IRQ/FIQ.
*     All it does is print the requested interrupt number.
*
*  INPUTS:
*
*  OUTPUT:
*
*******************************************************************************/
void default_int_routine(void);

/*******************************************************************************
*
*  ROUTINE NAME   : inc_pass/inc_error
*
*  DESCRIPTION:
*  		increments global pass/error counters
*
*  INPUTS:
*
*  OUTPUT:
*
*******************************************************************************/
void inc_pass(void);

void inc_error(void);

/*******************************************************************************
*
*  ROUTINE NAME   : check_test_result
*
*  DESCRIPTION:
*  		checks global pass/error counters and prints PASS/FAIL message	
*
*  INPUTS:
*
*  OUTPUT:
*
*******************************************************************************/
void check_test_result(void);

#endif  /* COMMONAPI_H */

