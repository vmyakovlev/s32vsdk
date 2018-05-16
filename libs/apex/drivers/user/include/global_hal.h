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

#ifndef APEXGLOBALHAL_H
#define APEXGLOBALHAL_H

/////////////////////////////////////////////////////////////////////////
/// \addtogroup global_hal
/// global_hal provides global APEX services related to interrupts and
/// reset for all APEX hardware.
/// @{

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/// APEX interrupt bits
typedef enum
{
   APEX2_INTERRUPT_MULTI_DMA = 0, ///< MCDMA interrupt
   APEX2_INTERRUPT_MOT_COMP_DMA,  ///< motion compensation DMA interrupt
   APEX2_INTERRUPT_SEQUENCER,     ///< sequencer interrupt
   APEX2_INTERRUPT_STREAM_IN_0,   ///< streamin 0 interrupt
   APEX2_INTERRUPT_STREAM_IN_1,   ///< streamin 1 interrupt
   APEX2_INTERRUPT_STREAM_IN_2,   ///< streamin 2 interrupt
   APEX2_INTERRUPT_STREAM_IN_3,   ///< streamin 3 interrupt
   APEX2_INTERRUPT_STREAM_OUT_0,  ///< streamout 0 interrupt
   APEX2_INTERRUPT_STREAM_OUT_1,  ///< streamout 1 interrupt
   APEX2_INTERRUPT_HRSZ_0,        ///< horizontal resizer 0 interrupt
   APEX2_INTERRUPT_HRSZ_1,        ///< horizontal resizer 1 interrupt
   APEX2_INTERRUPT_MAX
} APEX_INTERRUPT_BITS;

/////////////////////////////////////////////////////////////////////////////////
/// Reset all APEX hardware on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
/////////////////////////////////////////////////////////////////////////////////
void global_hal_AllReset(int apexID);

/////////////////////////////////////////////////////////////////////////////////
/// Reset sequencer on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
/////////////////////////////////////////////////////////////////////////////////
void global_hal_uSequencerReset(int apexID);

/////////////////////////////////////////////////////////////////////////////////
/// Reset motion compensation DMA on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
/////////////////////////////////////////////////////////////////////////////////
void global_hal_MotCmpDmaReset(int apexID);

/////////////////////////////////////////////////////////////////////////////////
/// Reset CMEM dma (i.e. stream DMA or streamin/streamout) on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
/////////////////////////////////////////////////////////////////////////////////
void global_hal_CmemDmaReset(int apexID);

/////////////////////////////////////////////////////////////////////////////////
/// Reset CMEM interface on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
/////////////////////////////////////////////////////////////////////////////////
void global_hal_cmemIFReset(int apexID);

/////////////////////////////////////////////////////////////////////////////////
/// Reset MCDMA (multi-channel DMA) on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
/////////////////////////////////////////////////////////////////////////////////
void global_hal_DmaReset(int apexID);

/////////////////////////////////////////////////////////////////////////////////
/// Reset APU0 on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
/////////////////////////////////////////////////////////////////////////////////
void global_hal_Apu0Reset(int apexID);

/////////////////////////////////////////////////////////////////////////////////
/// Reset APU1 on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
/////////////////////////////////////////////////////////////////////////////////
void global_hal_Apu1Reset(int apexID);

/////////////////////////////////////////////////////////////////////////////////
/// Reset vector control unit 0 on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
/////////////////////////////////////////////////////////////////////////////////
void global_hal_Vu0Reset(int apexID);

/////////////////////////////////////////////////////////////////////////////////
/// Reset vector control unit 1 on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
/////////////////////////////////////////////////////////////////////////////////
void global_hal_Vu1Reset(int apexID);

/////////////////////////////////////////////////////////////////////////////////
/// Reset horizontal resizer on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
/////////////////////////////////////////////////////////////////////////////////
void global_hal_HrszReset(int apexID);

/////////////////////////////////////////////////////////////////////////////////
/// Reset DMEM fifo 0 on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
/////////////////////////////////////////////////////////////////////////////////
void global_hal_Dmem0Reset(int apexID);

/////////////////////////////////////////////////////////////////////////////////
/// Reset DMEM fifo 1 on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
/////////////////////////////////////////////////////////////////////////////////
void global_hal_Dmem1Reset(int apexID);

/////////////////////////////////////////////////////////////////////////////////
/// Enable APU0 on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
/////////////////////////////////////////////////////////////////////////////////
void global_hal_Apu0Enable(int apexID);

/////////////////////////////////////////////////////////////////////////////////
/// Enable APU1 on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
/////////////////////////////////////////////////////////////////////////////////
void global_hal_Apu1Enable(int apexID);

/////////////////////////////////////////////////////////////////////////////////
/// Disable APU0 on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
/////////////////////////////////////////////////////////////////////////////////
void global_hal_Apu0Disable(int apexID);

/////////////////////////////////////////////////////////////////////////////////
/// Disable APU1 on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
/////////////////////////////////////////////////////////////////////////////////
void global_hal_Apu1Disable(int apexID);

/////////////////////////////////////////////////////////////////////////////////
/// Disable (i.e. mask) the global interrupt \p intBit on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] intBit
/// Global interrupt ID.
/////////////////////////////////////////////////////////////////////////////////
void global_hal_Apex2GlobalInterruptDisable(int apexID, APEX_INTERRUPT_BITS intBit);

/////////////////////////////////////////////////////////////////////////////////
/// Enable (i.e. un-mask) the global interrupt \p intBit on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] intBit
/// Global interrupt ID.
/////////////////////////////////////////////////////////////////////////////////
void global_hal_Apex2GlobalInterruptEnable(int apexID, APEX_INTERRUPT_BITS intBit);

/////////////////////////////////////////////////////////////////////////////////
/// Check to see if the global interrupt \p intBit on APEX \p apexID is enabled.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] intBit
/// Global interrupt ID.
///
/// \return
/// 1 if interrupt \p intBit is enabled, 0 if it is disabled/masked.
/////////////////////////////////////////////////////////////////////////////////
int global_hal_isApex2GlobalInterruptEnabled(int apexID, APEX_INTERRUPT_BITS intBit);

/////////////////////////////////////////////////////////////////////////////////
/// Return the interrupt status register field associated with global interrupt
/// \p intBit on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] intBit
/// Global interrupt ID.
///
/// \return
/// Interrupt status register field associated with global interrupt
/// \p intBit on APEX \p apexID.
/////////////////////////////////////////////////////////////////////////////////
int global_hal_Apex2GlobalInterruptStatus(int apexID, APEX_INTERRUPT_BITS intBit);

/////////////////////////////////////////////////////////////////////////////////
/// Return the hardware version associated with APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \return
/// Hardware version associated with APEX \p apexID.
/// <table>
/// <caption id="multi_row">global_hal_RetHwVersion return values</caption>
/// <tr> <th>return value  <th>meaning
/// <tr> <td>0x642         <td>64xCU 2xAPU configuration
/// <tr> <td>0x321         <td>32xCU 1xAPU configuration
/// </table>

/////////////////////////////////////////////////////////////////////////////////
int global_hal_RetHwVersion(int apexID);

/////////////////////////////////////////////////////////////////////////////////
/// Return the RTL build number associated with APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \return
/// RTL build number associated with APEX \p apexID.
/////////////////////////////////////////////////////////////////////////////////
int global_hal_RetRtlBuildNumber(int apexID);

#ifdef __cplusplus
}
#endif //__cplusplus

/// @}

#endif  /* APEXGLOBALHAL_H */
