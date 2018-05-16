/*
 * File:		linflex_serial.h
 * Purpose:		Linflex UART routines
 *
 */
  
#ifndef _LINFLEX_SERIAL_H
#define _LINFLEX_SERIAL_H

#define FLIN_133_MHz  1
#define FLIN_66_5_MHz 2


/* LIN clk frequency in MHz */
#define LIN_CLK FLIN_133_MHz

#define UART_9600_BAUDRATE   1
#define UART_19200_BAUDRATE  2
#define UART_38400_BAUDRATE  3
#define UART_115200_BAUDRATE 4

/* Uart baudrate */
#define UART_BAUDRATE   UART_115200_BAUDRATE


/********************************************************************/
void LinFlex0_Init(void);
char sci_0_getchar (void);
void sci_0_putchar (char);

#define out_char sci_0_putchar
#define in_char sci_0_getchar

/********************************************************************/
#endif
