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
 * File:		MIPI.c
 * Purpose:		Configuration of MIPI camera interface
 ********************************************************************/

#include "MIPI.h"
#include "s32vs234.h"

void MIPI_trig_execute (void)
{
    CSI_0.RXEN.B.RXEN = 1; // Receive enable
    CSI_0.ENABLECH.B.CHOEN = 1;	// Enable Virtual Channel Zero
}

void MIPI_init (void)
{
    CSI_0.CONC.B.NULANE = 0x1; //Two Lanes to Receive MIPI Data
    CSI_0.PHYC.B.PDRX = 0x0; //Power down disabled
   // CSI_0.PHYC.B.ENPDES = 0x0; //Enable deserializer enabled by default

/* Camera data rate for 2 MIPI CSI lanes coming from datasheet = 297Mbps
UI = 3.367ns
   -------------------------------------------------
           |Minimum   |Typical  | Maximum
   -------------------------------------------------
   THS_SETL|85+20=105 | 142     | 145+34=179
   -------------------------------------------------
   HSSETL  |(105/15)-1=6 | 9       | (179/15)-1=11
   -------------------------------------------------
   */
   
    CSI_0.CLKCS.B.HSSETL = 9;
    CSI_0.LAN0CS.B.D0HSET = 9;
    CSI_0.LAN1CS.B.D1HSET = 9;

    CSI_0.CH[0].SRTPTR.R = 0x3EB00000;
    CSI_0.CH[0].BUFLLEN.B.BUFLLEN = 1312*2; // x 2; // 1312 is number of pixels per line, 8 is number of lines which I would like to store and 2 - each pixel consumes 2 bytes for 12 bit mode
    CSI_0.CH[0].LINLEN.B.LINLEN = 1312*2; // 1312 is number of pixels per line, 2 - each pixel consumes 2 bytes for 12 bit mode
    CSI_0.CH[0].LPDI.B.LPDI = 4; //Done signal is generated after this number of lines
    CSI_0.CH[0].NUMLINE.B.NULINE = 8; // 8 lines I would like to store in SRAM
    CSI_0.CH[0].EXPCTDL.B.EXPLINES = 960; // Number specifies expected amount of lines to be received
    CSI_0.INTRENVC.B.FE0IE = 1; // interrupt enabled
    CSI_0.ERPPINTEN.R = 0x7F; //Protocol Packet Error Interrupt Enable
    CSI_0.ERPHYIE.R = 0xF; //Phy Error Interrupt Enable Register 
}
