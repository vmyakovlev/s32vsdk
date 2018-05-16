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
  
#ifndef SERIAL_H
#define SERIAL_H

void LinFlex0_Init( void );

#define FLIN_133_MHz  1
#define FLIN_66_5_MHz 2


/* LIN clk frequency in MHz */
#define LIN_CLK FLIN_133_MHz

#define UART_9600_BAUDRATE   1
#define UART_19200_BAUDRATE  2
#define UART_38400_BAUDRATE  3
#define UART_115200_BAUDRATE 4

/* Uart baudrate */
#define UART_BAUDRATE  UART_19200_BAUDRATE


/********************************************************************/
void LinFlex_Init(void);
char sci_0_getchar (void);
void sci_0_putchar (char);

#define out_char sci_0_putchar
#define in_char sci_0_getchar

/********************************************************************/
#endif /* SERIAL_H */

