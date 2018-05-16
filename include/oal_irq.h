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


/* Information for doxygen */
/**
 *  \file   oal.h
 *  \brief  Interface definition for the OAL (OS abstraction) library.
 *
 *  OAL provides an abstraction of standard OS interface functions used within
 *  MMP the system.
 */

#ifndef OALIRQ_H
#define OALIRQ_H

#ifdef __cplusplus
extern "C" {
#endif
////////////////////////////////////////////////////////////////////////
///
/// \addtogroup OAL_Interrupt OAL_Interrupt
///@{
/// \brief Threading functionality
/// 
////////////////////////////////////////////////////////////////////////

typedef enum {OAL_IRQ_LEVEL, OAL_IRQ_EDGE} IRQ_MODE;

////////////////////////////////////////////////////////////////////////
/// \par Descriptions:
/// Initialize an interrupt and add a service routine to it.
///
/// \param pServiceRoutineFunc  - [in] Interrupt service routine
/// \param irq  - [in] Interrupt number
/// \param priority  - [in] Interrupt priority
/// \param mode  - [in] Interrupt mode
///
/// \return
///    - #OAL_SUCCESS
///    - #OAL_FAILURE
////////////////////////////////////////////////////////////////////////
int32_t OAL_IRQInit(void (*pServiceRoutineFunc)(void), unsigned int irq, unsigned int priority, IRQ_MODE mode);

////////////////////////////////////////////////////////////////////////
/// \par Descriptions:
/// Enables the specified interrupt.
///
/// \param irq  - [in] Interrupt number
///
////////////////////////////////////////////////////////////////////////
void OAL_IRQEnable(unsigned int irq);

////////////////////////////////////////////////////////////////////////
/// \par Descriptions:
/// Deinitialize an interrupt and removes the service routine link.
///
/// \param irq  - [in] Interrupt number
///
/// \return
///    - #OAL_SUCCESS
///    - #OAL_FAILURE
////////////////////////////////////////////////////////////////////////
int32_t OAL_IRQDelete(unsigned int irq);

////////////////////////////////////////////////////////////////////////
/// \par Descriptions:
/// Disables the specified interrupt.
///
/// \param irq  - [in] Interrupt number
///
////////////////////////////////////////////////////////////////////////
void OAL_IRQDisable(unsigned int irq);

// end of \addtogroup OAL_Interrupt OAL_Interrupt
///@}

#ifdef __cplusplus
}
#endif

#endif /* OALIRQ_H */

