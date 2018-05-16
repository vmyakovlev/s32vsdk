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
 * \file    static_rgb_yuv_gs8.h
 * \brief   ISP (sequencer) static configuration
 * \author
 * \author
 * \version 0.1
 * \date    22-01-2016
 * \note    initial version
 ****************************************************************************/

#ifndef STATICRGBYUVGS8_H
#define STATICRGBYUVGS8_H
 
#define	IPUS1_START	1
#define	IPUS2_START	(IPUS1_START+3)
#define	IPUS5_START	(IPUS2_START+3)
#define	IPUV1_START	(IPUS5_START+1)
#define	IPUS0_START	(IPUS5_START+1)
#define	IPUS3_START	(IPUV1_START+1)
#define	IPUS6_START	(IPUV1_START+3)

#define	FDMA0_START	(IPUS3_START+1)	// YUV
#define	FDMA1_START	(IPUV1_START+1)	// Y
#define	FDMA2_START	(IPUS0_START+1)	// RGB24
#define	FDMA3_START	(IPUS6_START+1)	// Y_SCALED

/****************************************************************************/

#define	STATIC_SEQ										\
  initIPUnode(3, IPUS1_START, 0x1);	/* Expose */		\
  initIPUnode(4, IPUS2_START, 0x1);	/* DebayerG */		\
  initIPUnode(5, IPUS5_START, 0x1);	/* DebayerBR */		\
  initIPUnode(6, IPUV1_START, 0x1);	/* RGB2YUV */		\
  initIPUnode(7, IPUS0_START, 0x1);	/* ConvertRGB24 */	\
  initIPUnode(2, IPUS3_START, 0x1);	/* ConvYUV */		\
  initIPUnode(8, IPUS6_START, 0x2);	/* Y_Scale */		\
														\
  initFDMAnode(0, FDMA0_START, 0x1);					\
  initFDMAnode(1, FDMA1_START, 0x1);					\
  initFDMAnode(2, FDMA2_START, 0x1);					\
  initFDMAnode(3, FDMA3_START, 0x2);					\

#endif /* STATICRGBYUVGS8_H */
  
/* EOF */
