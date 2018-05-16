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

#ifndef APEXSEQHAL_H
#define APEXSEQHAL_H

/////////////////////////////////////////////////////////////////////////
/// \addtogroup seq_hal
/// seq_hal provides services related to APEX sequencer.
/// @{

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/////////////////////////////////////////////////////////////////////////////////
/// Set/inialize the sequencer resource \p lRsc with the value \p lVal
/// on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lRsc
/// Sequencer resource index (0-29).
///
/// \param[in] lVal
/// Sequencer resource index (0-29).
/////////////////////////////////////////////////////////////////////////////////
void seq_hal_set_rsc_counter(int apexID, int lRsc, int lVal);

/////////////////////////////////////////////////////////////////////////////////
/// Return the value of the sequencer resource counter \p lRsc on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lRsc
/// Sequencer resource index (0-31).
///
/// \return
/// Sequencer resource value.
/////////////////////////////////////////////////////////////////////////////////
int seq_hal_ret_rsc_counter(int apexID, int lRsc);

/////////////////////////////////////////////////////////////////////////////////
/// Perform a processor increment of the sequencer resources expressed in the 32-bit mask
/// \p lRscMask by the user-defined processor \p lProcId on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lProcId
/// The logical processor ID (0-5).
///
/// \param[in] lRscMask
/// The 32-bit mask representing the sequencer resources that should be incremented.
/// If a bit of the mask is 1, the associated sequencer resource will be incremented.
/// e.g. if lRscMask is 0x00003001, then sequencer resources 0, 12, and 13 will be
/// incremented.
/////////////////////////////////////////////////////////////////////////////////
void seq_hal_proc_inc_rscmask(int apexID, int lProcId, int lRscMask);

/////////////////////////////////////////////////////////////////////////////////
/// Perform a processor decrement of the sequencer resources expressed in the 32-bit mask
/// \p lRscMask by the user-defined processor \p lProcId on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lProcId
/// The logical processor ID (0-5).
///
/// \param[in] lRscMask
/// The 32-bit mask representing the sequencer resources that should be decremented.
/// If a bit of the mask is 1, the associated sequencer resource will be decremented.
/// e.g. if lRscMask is 0x00000502, then sequencer resources 1, 8, and 10 will be
/// decremented.
/////////////////////////////////////////////////////////////////////////////////
void seq_hal_proc_dec_rscmask(int apexID, int lProcId, int lRscMask);

/////////////////////////////////////////////////////////////////////////////////
/// Enable the sequencer from the perspective of the user-defined processor \p lProcId
/// on APEX \p apexID.  Each processor has it's own enable/disable, and the sequencer
/// as a whole will be considered 'enabled' if one or more of the processor enable
/// states is asserted.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lProcId
/// The logical processor ID (0-5).
/////////////////////////////////////////////////////////////////////////////////
void seq_hal_enable_seq(int apexID, int lProcId);

/////////////////////////////////////////////////////////////////////////////////
/// Disable the sequencer from the perspective of the user-defined processor \p lProcId
/// on APEX \p apexID.  Each processor has it's own enable/disable, and the sequencer
/// as a whole will be considered 'disabled' if none of the processor enable
/// states are asserted.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lProcId
/// The logical processor ID (0-5).
/////////////////////////////////////////////////////////////////////////////////
void seq_hal_disable_seq(int apexID, int lProcId);

/////////////////////////////////////////////////////////////////////////////////
/// Return the value of the sequencer interrupt associated with the user-defined
/// processor \p lProcId on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lProcId
/// The logical processor ID (0-5).
///
/// \return
/// 1 if the interrupt has been raised, 0 otherwise.
/////////////////////////////////////////////////////////////////////////////////
int seq_hal_query_interrupt(int apexID, int lProcId);

/////////////////////////////////////////////////////////////////////////////////
/// Clear the interrupt associated with the user-defined processor \p lProcId on
/// APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lProcId
/// The logical processor ID (0-5).
/////////////////////////////////////////////////////////////////////////////////
void seq_hal_clear_interrupt(int apexID, int lProcId);

/////////////////////////////////////////////////////////////////////////////////
/// Give the sequencer control over the APEX hardware blocks defined by the \p lScbWord
/// mask on APEX \p apexID (at which point they become 'sequencer controlled blocks' or SCBs).
/// If hardware blocks are flagged as 'sequencer controlled' they are no longer under direct
/// user control.  For example, if lScbWord = 0x00000003, DMA channels 0 and 1 are under
/// sequencer control, and can no longer be used for non-sequencer purposes.
///
/// <table>
/// <caption id="multi_row">SCB bit mask</caption>
/// <tr> <th>mask bit  <th>associated hardware block
/// <tr> <td>0         <td>DMA channel 0
/// <tr> <td>1         <td>DMA channel 1
/// <tr> <td>2         <td>DMA channel 2
/// <tr> <td>3         <td>DMA channel 3
/// <tr> <td>4         <td>DMA channel 4
/// <tr> <td>5         <td>DMA channel 5
/// <tr> <td>6         <td>DMA channel 6
/// <tr> <td>7         <td>DMA channel 7
/// <tr> <td>16        <td>Streamin channel 0
/// <tr> <td>17        <td>Streamin channel 1
/// <tr> <td>18        <td>Streamin channel 2
/// <tr> <td>19        <td>Streamin channel 3
/// <tr> <td>20        <td>Streamout channel 0
/// <tr> <td>21        <td>Streamout channel 1
/// <tr> <td>24        <td>Motion compensation DMA channel 0
/// <tr> <td>25        <td>Motion compensation DMA channel 1
/// <tr> <td>28        <td>Horizontal resizer 0
/// <tr> <td>29        <td>Horizontal resizer 1
/// </table>
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lScbWord
/// This 32-bit mask indicates which APEX hardware blocks the sequencer will
/// be given control of.  A mask bit should be set to 1 to enable sequencer control
/// of the associated hardware block.
/////////////////////////////////////////////////////////////////////////////////
void seq_hal_scb_enable(int apexID, int lScbWord);

/////////////////////////////////////////////////////////////////////////////////
/// This function will do the opposite of seq_hal_scb_enable(...), and disable
/// sequencer control of the hardware blocks indicated in the \p lScbWord mask
/// on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lScbWord
/// This 32-bit mask indicates the APEX hardware blocks the sequencer should no longer
/// be in control of.  A mask bit should be set to 1 to disable sequencer control
/// of the associated hardware block.  See the description of seq_hal_scb_enable(...)
/// for an explanation of the bit mask.
/////////////////////////////////////////////////////////////////////////////////
void seq_hal_scb_disable(int apexID, int lScbWord);

/////////////////////////////////////////////////////////////////////////////////
/// The resource mask \p lVal indicates which sequencer resources will be incremented
/// when the transfer associated with DMA channel \p lCh completes on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lCh
/// DMA channel (0-7).
///
/// \param[in] lVal
/// The 32-bit mask representing the sequencer resources that should be incremented.
/////////////////////////////////////////////////////////////////////////////////
void seq_hal_set_dma_inc(int apexID, int lCh, int lVal);

/////////////////////////////////////////////////////////////////////////////////
/// The resource mask \p lVal indicates which sequencer resources will be decremented
/// when the transfer associated with DMA channel \p lCh completes on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lCh
/// DMA channel (0-7).
///
/// \param[in] lVal
/// The 32-bit mask representing the sequencer resources that should be decremented.
/////////////////////////////////////////////////////////////////////////////////
void seq_hal_set_dma_dec(int apexID, int lCh, int lVal);

/////////////////////////////////////////////////////////////////////////////////
/// The resource mask \p lVal indicates which sequencer resources will be incremented
/// when the transfer associated with motion compensation DMA channel \p lCh completes
/// on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lCh
/// Motion compensation DMA channel (0-1).
///
/// \param[in] lVal
/// The 32-bit mask representing the sequencer resources that should be incremented.
/////////////////////////////////////////////////////////////////////////////////
void seq_hal_set_motioncomp_inc(int apexID, int lCh, int lVal);

/////////////////////////////////////////////////////////////////////////////////
/// The resource mask \p lVal indicates which sequencer resources will be decremented
/// when the transfer associated with motion compensation DMA channel \p lCh completes
/// on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lCh
/// Motion compensation DMA channel (0-1).
///
/// \param[in] lVal
/// The 32-bit mask representing the sequencer resources that should be decremented.
/////////////////////////////////////////////////////////////////////////////////
void seq_hal_set_motioncomp_dec(int apexID, int lCh, int lVal);

/////////////////////////////////////////////////////////////////////////////////
/// Mask the sequencer interrupts defined by the \p lMask mask from the perspective
/// of the host (e.g. ARM) on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lMask
/// The 32-bit mask representing the sequencer interrupts that should be masked.
/////////////////////////////////////////////////////////////////////////////////
void seq_hal_mask_interrupts(int apexID, int lMask);

/////////////////////////////////////////////////////////////////////////////////
/// Mask the sequencer interrupts defined by the \p lMask mask from the perspective
/// of APU \p lApuId on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lApuId
/// APU ID (0 or 1).
///
/// \param[in] lMask
/// The 32-bit mask representing the sequencer interrupts that should be masked.
/////////////////////////////////////////////////////////////////////////////////
void seq_hal_mask_interrupts_apu(int apexID, int lApuId, int lMask);

/////////////////////////////////////////////////////////////////////////////////
/// Clear the sequencer interrupts defined by the \p lMask on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lMask
/// The 32-bit mask representing the sequencer interrupts that should be cleared.
/////////////////////////////////////////////////////////////////////////////////
void seq_hal_clear_interrupts(int apexID, int lMask);

/////////////////////////////////////////////////////////////////////////////////
/// Return a 32-bit value that reflects the state of a the sequencer interrupt
/// register on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \return
/// 32-bit value containing the state of the sequencer interrupt register.  If a bit
/// is 1, then the associated interrupt has been asserted.
/////////////////////////////////////////////////////////////////////////////////
int seq_hal_query_interrupts(int apexID);

////////////////////////////////////////////////////////////////////////////////
/// Returns the motion compensation DMA sequencer controlled block (SCB) ID associated
/// with the logical index \p lIdx. This function translates a logical human-friendly
/// index to an SCB ID.
///
/// \param[in] lIdx
/// Logical motion compensation DMA channel index (0-1)
///
/// \return
/// SCB index associated with the specified motion compensation DMA channel index.
/////////////////////////////////////////////////////////////////////////////////
int RetMceScbId(int lIdx);

//----------------------------------------------------------------------
//debugging/profiling methods (not called in release code or documented)
//----------------------------------------------------------------------
void seq_hal_set_hrsz_inc(int apexID, int lCh, int lVal);
void seq_hal_set_hrsz_dec(int apexID, int lCh, int lVal);

#ifdef __cplusplus
}
#endif //__cplusplus

/// @}

#endif /* APEXSEQHAL_H */
