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
 * \file    static_ov10640_quad.h
 * \brief   ISP (sequencer) static configuration
 * \author
 * \author
 * \version 0.1
 * \date    30-06-2016
 * \note    initial version
 ****************************************************************************/

#ifndef STATICOV10640QUAD_H
#define STATICOV10640QUAD_H

#define	START_CYC	0x3

#define	IPUV1_START	1
#define	IPUS1_START	(IPUV1_START+3)
#define	IPUS2_START	(IPUV1_START+4)
#define	IPUS4_START	(IPUV1_START+5)
#define	IPUV0_START	(IPUS1_START+1)
#define	IPUV2_START	(IPUS2_START+1)
#define	IPUV3_START	(IPUS4_START+1)

#define	FDMA0_START	(IPUV0_START+1)	// YUV0
#define	FDMA1_START	(IPUV2_START+1)	// YUV1
#define	FDMA2_START	(IPUV3_START+1)	// YUV2

/****************************************************************************/

#define	STATIC_SEQ										\
  initIPUnode(6, IPUV1_START, 0x1);	/* ToneMapping */	\
  initIPUnode(2, IPUS1_START, START_CYC);	/* Debayer0 */		\
  initIPUnode(4, IPUS2_START, START_CYC);	/* Debayer1 */		\
  initIPUnode(7, IPUS4_START, START_CYC);	/* Debayer2 */		\
  initIPUnode(3, IPUV0_START, START_CYC);	/* RGB2YUV0 */		\
  initIPUnode(5, IPUV2_START, START_CYC);	/* RGB2YUV1 */		\
  initIPUnode(8, IPUV3_START, START_CYC);	/* RGB2YUV2 */		\
															\
  initFDMAnode(0, FDMA0_START, START_CYC);					\
  initFDMAnode(1, FDMA1_START, START_CYC);					\
  initFDMAnode(2, FDMA2_START, START_CYC);					\

#endif /* STATICOV10640QUAD_H */
  
/* EOF */
