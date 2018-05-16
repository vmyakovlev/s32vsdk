/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* (C) Copyright CogniVue Corporation. 2011-2014 All right reserved.
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
#ifndef APEXSTIOHAL_H
#define APEXSTIOHAL_H

/////////////////////////////////////////////////////////////////////////
/// \addtogroup stio_hal
/// stio_hal provides services related to APEX stream DMA (i.e. CMEM dma,
/// streamin, streamout).
/// @{

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/////////////////////////////////////////////////////////////////////////////////
/// Clear the 'done' interrupt associated with streamin channel \p channel on
/// APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] channel
/// Streamin channel (0-3).
/////////////////////////////////////////////////////////////////////////////////
void stio_hal_interruptclear_sti_done(int apexID, int channel);

/////////////////////////////////////////////////////////////////////////////////
/// Clear the 'done' interrupt associated with streamout channel \p channel on
/// APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] channel
/// Streamin channel (0-1).
/////////////////////////////////////////////////////////////////////////////////
void stio_hal_interruptclear_sto_done(int apexID, int channel);

#ifdef __cplusplus
}
#endif //__cplusplus

/// @}

#endif /* APEXSTIOHAL_H */

