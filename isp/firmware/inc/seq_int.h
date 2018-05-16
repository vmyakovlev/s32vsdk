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
 * \file    isp_int.h
 * \brief   ISP runtime firmware (M0+)
 * \author
 * \author
 * \version 0.1
 * \date    15-10-2013
 * \note    initial version
 ****************************************************************************/

#ifndef	SEQINT_H
#define	SEQINT_H

#ifdef  __cplusplus
extern "C" {
#endif

/*****************************************************************************
* consts
*****************************************************************************/

/*****************************************************************************
* macros
*****************************************************************************/

/*****************************************************************************
* types
*****************************************************************************/

typedef struct devEvent_t
{
  SEQ_Head_t*	mpHead;	// *** 00 pointer to nodes header ***
  volatile uint16_t	mCnt;	// *** 04 event counter ***
  uint16_t	mPad0;	// *** 06 pad aligment ***
			// *** 08 (total) ***
} devEvent_t
#ifdef __GNUC__
__attribute__ ((aligned (2)))
#endif
;

/*****************************************************************************
* exported data
*****************************************************************************/

extern SEQ_Head_t*	gpIntDevCfg[SEQ_NUM_DEV];
extern uint16_t		gIntDevCnt;
extern devEvent_t	gpIPUS[ISP_NUM_IPUS];
extern devEvent_t	gpIPUV[ISP_NUM_IPUV];
extern devEvent_t	gpOTHR[SEQ_OTHRIX_LAST];

/*****************************************************************************
* exported functions
*****************************************************************************/

extern void	SEQ_EvntInit(SEQ_Othr_ix_t aMIPIix);
extern void	SEQ_EvntHndl(void);
extern void	SEQ_EvntRegIntDev(SEQ_Head_t* apHead);
extern void	SEQ_HostOutAckHndl(void);
extern uint16_t	SEQ_ExecuteEvents(void);
extern uint16_t procOthrEvent(uint16_t aIx);
extern uint16_t procIpuEvent(devEvent_t* apDE);

/*****************************************************************************
* imports
*****************************************************************************/

#ifdef  __cplusplus
}
#endif

#endif /* SEQINT_H */
/*EOF*/
