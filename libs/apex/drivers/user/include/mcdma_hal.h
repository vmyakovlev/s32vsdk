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

#ifndef APEXMCDMAHAL_H
#define APEXMCDMAHAL_H

#include <stdint.h>

/////////////////////////////////////////////////////////////////////////
/// \addtogroup mcdma_hal
/// mcdma_hal provides APEX multi-channel DMA services, including the ability
/// to configure, start, and wait for linked-list-based transfers.
/// @{

/// DMA priority
typedef enum
{
   DMA_LOW      = 0, ///< low
   DMA_MEDIUM_1 = 1, ///< medium 1
   DMA_MEDIUM_2 = 2, ///< medium 2
   DMA_HIGH     = 3  ///< high
} DMA_PRIORITY;

/// DMA burst size
typedef enum
{
   INCR1  = 0,  ///< 1 beats
   INCR2  = 1,  ///< 2 beats
   INCR3  = 2,  ///< 3 beats
   INCR4  = 3,  ///< 4 beat
   INCR5  = 4,  ///< 5 beats
   INCR6  = 5,  ///< 6 beats
   INCR7  = 6,  ///< 7 beats
   INCR8  = 7,  ///< 8 beats
   INCR9  = 8,  ///< 9 beats
   INCR10 = 9,  ///< 10 beats
   INCR11 = 10, ///< 11 beats
   INCR12 = 11, ///< 12 beats
   INCR13 = 12, ///< 13 beats
   INCR14 = 13, ///< 14 beats
   INCR15 = 14, ///< 15 beats
   INCR16 = 15  ///< 16 beats
} DMA_BURST_SIZE;

/// DMA burst beat word size
typedef enum
{
   DMA_BURST_BEAT_BYTE_SIZE     = 0, ///< 8 bits
   DMA_BURST_BEAT_HALFWORD_SIZE = 1, ///< 16 bits
   DMA_BURST_BEAT_WORD_SIZE     = 2, ///< 32 bits
   DMA_BURST_BEAT_DWORD_SIZE    = 3, ///< 64 bits
   DMA_BURST_BEAT_DDWORD_SIZE   = 4  ///< 128 bits
} DMA_BURST_BEAT_SIZE;

/// DMA port type
typedef enum
{
   DMA_PORT_MEMORY     = 0, ///< memory port
   DMA_PORT_PERIPHERAL = 1  ///< peripheral port
} DMA_PORT_TYPE;

/// DMA data format
typedef enum
{
   LINEAR = 0, ///< 1D (linear)
   U2D    = 1, ///< U2D
   U3D    = 2, ///< U3D
   U4D    = 3  ///< U4D
} DMA_FORMAT;

/// DMA source peripherals
typedef enum
{
   SRC_MCDMA_INTERNAL_FIFO  = 0,  ///< mcdma internal fifo
   SRC_CMEM_DMA_STREAM_OUT0 = 1,  ///< streamout 0
   SRC_CMEM_DMA_STREAM_OUT1 = 2,  ///< streamout 1
   SRC_MOTION_COMP0         = 3,  ///< motion comp 0
   SRC_MOTION_COMP1         = 4,  ///< motion comp 1
   SRC_APU0_DMEM_FIFO       = 5,  ///< APU0 dmem fifo
   SRC_APU1_DMEM_FIFO       = 6,  ///< APU1 dmem fifo
   SRC_MEM_FILL0            = 7,  ///< mem fill 0
   SRC_MEM_FILL1            = 8,  ///< mem fill 1
   SRC_HRSZ0                = 9,  ///< horizontal resizer 0
   SRC_HRSZ1                = 10, ///< horizontal resizer 1
   SRC_UNUSED               = 0xffffffff
} DMA_SRC_PERIPHERAL;

/// DMA destination peripherals
typedef enum
{
   DST_MCDMA_INTERNAL_FIFO = 0,  ///< mcdma internal fifo
   DST_CMEM_DMA_STREAM_IN0 = 1,  ///< streamin 0
   DST_CMEM_DMA_STREAM_IN1 = 2,  ///< streamin 1
   DST_APU0_DMEM_FIFO      = 5,  ///< APU0 dmem fifo
   DST_APU1_DMEM_FIFO      = 6,  ///< APU1 dmem fifo
   DST_HRSZ0               = 9,  ///< horizontal resizer 0
   DST_HRSZ1               = 10, ///< horizontal resizer 1
   DST_CMEM_DMA_STREAM_IN2 = 11, ///< streamin 2
   DST_CMEM_DMA_STREAM_IN3 = 12, ///< streamin 3
   DST_UNUSED = 0xffffffff
} DMA_DST_PERIPHERAL;

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

/////////////////////////////////////////////////////////////////////////////////
/// Configure DMA channel \p ChannelN on APEX \p apexID for a sequential linked list
/// transfer based on the linked list starting at the physical address \p LinkedListAddr.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] ChannelN
/// DMA channel (0-7).
///
/// \param[in] LinkedListAddr
/// Physical address of the start of the linked list describing the transfer.
/////////////////////////////////////////////////////////////////////////////////
void mcdma_hal_program_seq_linkedlist (int apexID, unsigned int ChannelN, unsigned int LinkedListAddr);

/////////////////////////////////////////////////////////////////////////////////
/// Configure DMA channel \p ChannelN on APEX \p apexID for a non-sequential linked list
/// transfer based on the linked list starting at the physical address \p LinkedListAddr.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] ChannelN
/// DMA channel (0-7).
///
/// \param[in] LinkedListAddr
/// Physical address of the start of the linked list describing the transfer.
/////////////////////////////////////////////////////////////////////////////////
void mcdma_hal_program_nonseq_linkedlist(int apexID, unsigned int ChannelN, unsigned int LinkedListAddr);

/////////////////////////////////////////////////////////////////////////////////
/// Clear the 'done' interrupt associated with DMA channel \p ChannelN on APEX \p apexID.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] ChannelN
/// DMA channel (0-7).
/////////////////////////////////////////////////////////////////////////////////
void mcdma_hal_interruptclear_ch_done(int apexID, unsigned int ChannelN);

/////////////////////////////////////////////////////////////////////////////////
/// Start the transfer associated with DMA channel \p ChannelN on APEX \p apexID.
/// It is assumed that the channel has already been configured via mcdma_hal_program_seq_linkedlist(...)
/// or mcdma_hal_program_nonseq_linkedlist(...).
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] ChannelN
/// DMA channel (0-7).
/////////////////////////////////////////////////////////////////////////////////
void mcdma_hal_start(int apexID, unsigned int ChannelN);

/////////////////////////////////////////////////////////////////////////////////
/// Wait for the transfer associated with DMA channel \p ChannelN on APEX \p apexID
/// to complete.  This is a blocking/polling call.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] ChannelN
/// DMA channel (0-7).
/////////////////////////////////////////////////////////////////////////////////
void mcdma_hal_wait(int apexID, unsigned int ChannelN);

////////////////////////////////////////////////////////////////////////////////
/// Wait for the transfer associated with DMA channel \p ChannelN on APEX \p apexID
/// to complete.  A timeout value \p timeoutInUs must be specified to indicate how
/// long the call should block before returning.  If the transfer does not complete
/// before the timeout expires, an error will be returned.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] ChannelN
/// DMA channel (0-7).
///
/// \param[in] timeoutInUs
/// Timeout value in microseconds (us).
///
/// \return
/// 0 if the transfer completes successfully, non-zero if the transfer does not complete
/// before the timeout expires.
/////////////////////////////////////////////////////////////////////////////////
int mcdma_hal_wait_timeout(int apexID, unsigned int ChannelN, int timeoutInUs);

////////////////////////////////////////////////////////////////////////////////
/// Configure DMA memory fill peripheral words (specifically memfill0).
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] FillWord0
/// 32-bit value for fill word 0.
///
/// \param[in] FillWord1
/// 32-bit value for fill word 1.
///
/// \param[in] FillWord2
/// 32-bit value for fill word 2.
///
/// \param[in] FillWord3
/// 32-bit value for fill word 3.
/////////////////////////////////////////////////////////////////////////////////
void mcdma_hal_set_memory_fill_words(int apexID,
                                     unsigned int FillWord0,
                                     unsigned int FillWord1,
                                     unsigned int FillWord2,
                                     unsigned int FillWord3);

////////////////////////////////////////////////////////////////////////////////
/// Returns the streamin destination peripheral ID associated with the logical
/// index \p lIdx.  This function translates a logical human-friendly index to a
/// DMA-friendly peripheral ID.
///
/// \param[in] lIdx
/// Logical streamin index ranging from 0 to 3 (there are 4 streamin entities).
///
/// \return
/// Streamin destination peripheral ID.
/////////////////////////////////////////////////////////////////////////////////
int RetStreamInPeriphId(int lIdx);

////////////////////////////////////////////////////////////////////////////////
/// Returns the streamout source peripheral ID associated with the logical
/// index \p lIdx. This function translates a logical human-friendly index to a
/// DMA-friendly peripheral ID.
///
/// \param[in] lIdx
/// Logical streamout index ranging from 0 to 1 (there are 2 streamout entities).
///
/// \return
/// Streamout source peripheral ID.
/////////////////////////////////////////////////////////////////////////////////
int RetStreamOutPeriphId(int lIdx);

//----------------------------------------------------------------------
//debugging/profiling methods (not called in release code or documented)
//----------------------------------------------------------------------
void mcdma_hal_enable_perf_counter(int apexID);
void mcdma_hal_disable_perf_counter(int apexID);
unsigned int mcdma_hal_read_perf_counter_total(int apexID);
unsigned int mcdma_hal_read_perf_counter_busrd(int apexID); //bus read
unsigned int mcdma_hal_read_perf_counter_buswr(int apexID); //bus write
void mcdma_hal_ch_clear(int apexID, unsigned int ChannelN);
int mcdma_hal_check_ch_state(int apexID, int ChannelN);
void mcdma_hal_memcpy(int apexID, int ChannelN, uint32_t dest, uint32_t src, uint32_t count, DMA_BURST_SIZE burstsize, DMA_BURST_BEAT_SIZE wordsize);

#ifdef __cplusplus
}
#endif //__cplusplus

/// @}

#endif /* APEXMCDMAHAL_H */
