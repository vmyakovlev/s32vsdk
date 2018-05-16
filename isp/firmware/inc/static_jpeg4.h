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
/**
 * \file    static_jpeg4.h
 * \brief   ISP (sequencer) static configuration
 * \author
 * \author
 * \version 0.1
 * \date    08-JUNE-2016
 * \note    initial version
 ****************************************************************************/

#ifndef STATICJPEG4_H
#define STATICJPEG4_H
 
#define	IPUS2_START	3
#define	FDMA_OFFS	1

#define	FDMA0_START	(IPUS2_START+FDMA_OFFS)	// Y
#define	FDMA1_START	(IPUS2_START+FDMA_OFFS)	// U
#define	FDMA2_START	(IPUS2_START+FDMA_OFFS)	// V
#define	FDMA3_START	(IPUS2_START+FDMA_OFFS)	// Alpha

/****************************************************************************/

#define	STATIC_SEQ													\
  initIPUnode(1, IPUS2_START, 0x2);	/* Yuv420ToInterleaved */	    \
																	\
  initFDMAnode(0, FDMA0_START, 0x1);								\
  initFDMAnode(1, FDMA1_START, 0x1);								\
  initFDMAnode(2, FDMA2_START, 0x1);								\
  initFDMAnode(3, FDMA3_START, 0x1);								\

#endif /* STATICJPEG4_H */
  
/* EOF */
