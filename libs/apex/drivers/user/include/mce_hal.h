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

#ifndef MCEHAL_H
#define MCEHAL_H

/////////////////////////////////////////////////////////////////////////
/// \addtogroup mce_hal
/// mce_hal provides services related to APEX motion compensation DMA.
/// @{

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/////////////////////////////////////////////////////////////////////////////////
/// Configure motion compensation DMA channel \p chNo on APEX \p lApexID for a
/// sequential linked list transfer based on the linked list starting at the
/// physical address \p linkedListAddr.
///
/// \param[in] lApexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] chNo
/// Motion compensation DMA channel (0-1).
///
/// \param[in] linkedListAddr
/// Physical address of the start of the linked list describing the transfer.
/////////////////////////////////////////////////////////////////////////////////
void mce_hal_program_linkedlist(int lApexID, unsigned int chNo, unsigned int linkedListAddr);

/////////////////////////////////////////////////////////////////////////////////
/// Configure motion compensation DMA channel \p chNo on APEX \p lApexID for a
/// non-sequential linked list transfer based on the linked list starting at the
/// physical address \p linkedListAddr.
///
/// \param[in] lApexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] chNo
/// Motion compensation DMA channel (0-1).
///
/// \param[in] linkedListAddr
/// Physical address of the start of the linked list describing the transfer.
/////////////////////////////////////////////////////////////////////////////////
void mce_hal_program_nonseq_linkedlist(int lApexID, unsigned int chNo, unsigned int linkedListAddr);

/////////////////////////////////////////////////////////////////////////////////
/// Start the transfer associated with motion compensation DMA channel \p chNo
/// on APEX \p lApexID.  It is assumed that the channel has already been configured
/// via mce_hal_program_linkedlist(...) or mce_hal_program_nonseq_linkedlist(...).
///
/// \param[in] lApexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] chNo
/// Motion compensation DMA channel (0-1).
/////////////////////////////////////////////////////////////////////////////////
void mce_hal_enable_ch(int lApexID, unsigned int chNo);

/////////////////////////////////////////////////////////////////////////////////
/// Wait for the transfer associated with motion compensation DMA channel \p chNo
/// on APEX \p lApexID to complete.  This is a blocking/polling call.
///
/// \param[in] lApexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] chNo
/// Motion compensation DMA channel (0-1).
/////////////////////////////////////////////////////////////////////////////////
void mce_hal_wait_polling(int lApexID, unsigned int chNo);

/////////////////////////////////////////////////////////////////////////////////
/// Clear the 'done' interrupt associated with motion compensation DMA channel \p chNo
/// on APEX \p lApexID.
///
/// \param[in] lApexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] chNo
/// Motion compensation DMA channel (0-1).
/////////////////////////////////////////////////////////////////////////////////
void mce_hal_clear_int(int lApexID, unsigned int chNo);

//----------------------------------------------------------------------
//debugging/profiling methods (not called in release code or documented)
//----------------------------------------------------------------------
void mce_hal_clear_ch(int lApexID, unsigned int chNo);
void mce_hal_clear_err(int lApexID, unsigned int chNo);

#ifdef __cplusplus
}
#endif //__cplusplus

/// @}

#endif /* MCEHAL_H */

