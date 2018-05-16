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
 * \file    isp_static.h
 * \brief   ISP runtime firmware definitions
 * \author
 * \author
 * \version 0.1
 * \date    21-06-2016
 * \note    initial version
 ****************************************************************************/

#ifndef	SEQSTATIC_H
#define	SEQSTATIC_H

#ifdef  __cplusplus
extern "C" {
#endif

/*****************************************************************************
* consts
*****************************************************************************/

#define	IPU_DEBUG_MODE	0xffffffff

#ifndef __ASSEMBLER__

/*****************************************************************************
* exported functions
*****************************************************************************/

#if defined(STATIC_SEQUENCE_MIPI) || defined(STATIC_SEQUENCE_FDMA) || defined(STATIC_SEQUENCE_JPEG4) ||defined(STATIC_SEQUENCE_VIU)
extern void	staticSequenceInit(void);
extern void	staticSequence(void);
extern uint32_t	staticSequenceWaitFinish(void);
extern void staticSequenceEventIPU(void);
extern void staticSequenceJpeg4(uint16_t aCnt, uint16_t aChan);
#endif

#endif // __ASSEMBLER__

#ifdef  __cplusplus
}
#endif

#endif /* SEQSTATIC_H */
/*EOF*/
