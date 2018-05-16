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

#ifndef LINKEDLISTUTILITIES_H
#define LINKEDLISTUTILITIES_H

#include <icp_datatype.h>
#include <stdint.h>

/////////////////////////////////////////////////////////////////////////
/// \addtogroup linked_list_utilities
/// linked_list_utilities provides 'helper' services related to linked list
/// creation/abstraction.
///
/// Notes:
///
/// <b>The 'element' concept:</b>
/// Many of the following methods require the expression of a base data transfer unit, or 'element'.
/// The three parameters \p elementDataType, \p elementDimX, and \p elementDimY are typically used to
/// fully describe an 'element'.  For example, if the base transfer unit is 8-bit unsigned data,
/// elementDataType = icp::DATATYPE_08U, elementDimX = 1, and elementDimY = 1.
///
/// <b>General linked list construction and usage methodology:</b>
/// The methods presented in this module allow for the configuration of linked list entries, which are
/// basically linked list building blocks.  A single transfer can be comprised of multiple sequential
/// linked list entries if desired.  In either case, the 'LLE_DMA_Finalize(...)' function must be called
/// on the final entry in the linked list before passing the linked list off to the appropriate data transfer
/// hardware.
///
/// <b>The 'span' concept:</b>
/// The number of bytes that must be skipped in a 2D memory region to get from one line of
/// bytes to the next line of bytes.
///
/// @{

//linked list entry sizes (in 32-bit words)
#define LLE_SIZE_2D_DMA_XMEM_CMEM 24
#define LLE_SIZE_2D_DMA_MEM_MEM   8
#define LLE_SIZE_MCE              24
#define LLE_SIZE_HRSZ_SIDEBAND    28
#define LLE_SIZE_1D_FILL          8
#define LLE_SIZE_1D_MEM_MEM       8

#define LLE_SIZE_2D_DMA_XMEM_HRSZ (LLE_SIZE_2D_DMA_MEM_MEM + LLE_SIZE_HRSZ_SIDEBAND)
// See ALGO-61 for an explanation as to why these are different sizes
//#define LLE_SIZE_2D_DMA_HRSZ_CMEM LLE_SIZE_2D_DMA_XMEM_CMEM
// 8 + 28, 128-bit aligned, 36.
#define LLE_SIZE_2D_DMA_HRSZ_CMEM 36
#define LLE_SIZE_2D_DMA_CMEM_HRSZ (LLE_SIZE_2D_DMA_XMEM_CMEM + LLE_SIZE_HRSZ_SIDEBAND)

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/////////////////////////////////////////////////////////////////////////////////
/// Configure a linked list entry to express a 2D data transfer from XMEM to CMEM (DMA + streamin)
/// or from CMEM to XMEM (DMA + streamout).  This data transfer is to/from CMEM, and from the CMEM
/// perspective the data is seen as a tile of 2D data chunks described by the parameters \p tileWidthInChunks,
/// \p chunkWidth, and \p chunkHeight.
///
/// \param[out] llBuffer
/// Pointer to a contiguous region of 128-bit aligned memory that will contain the
/// generated linked list entry.  It is assumed that allocated memory is sufficient
/// to contain the linked list entry (i.e. at least 24 32-bit words).
///
/// \param[in] elementDataType
/// Element data type (see 'element' details above).
///
/// \param[in] elementDimX
/// Element x dimension (see 'element' details above).
///
/// \param[in] elementDimY
/// Element y dimension (see 'element' details above).
///
/// \param[in] srcAddr
/// In the XMEM to CMEM case, this is a physical memory address indicating the source
/// of the data to be transferred.  In the CMEM to XMEM case it is the streamout
/// peripheral SRC_CMEM_DMA_STREAM_OUT0 or SRC_CMEM_DMA_STREAM_OUT1.
///
/// \param[in] dstAddr
/// In the XMEM to CMEM case, this is the streamin peripheral DST_CMEM_DMA_STREAM_IN0,
/// DST_CMEM_DMA_STREAM_IN1, DST_CMEM_DMA_STREAM_IN2, or DST_CMEM_DMA_STREAM_IN3.  In the CMEM
/// to XMEM case it is a physical memory address indicating the destination where
/// the data will be transferred.
///
/// \param[in] span
/// Span of the src/dst region in memory (in bytes).  It refers to the source memory region if the transfer direction
/// is XMEM to CMEM, and to the destination memory region if the transfer direction is CMEM to XMEM.
///
/// \param[in] width
/// Width of the src/dst region in memory (in elements).  It refers to the source memory region if the transfer direction
/// is XMEM to CMEM, and to the destination memory region if the transfer direction is CMEM to XMEM.
///
/// \param[in] chunkWidth
/// Chunk width in elements.  It must satisfy the restriction defined by DoesChunkWidthSatisfyStreamHwLimits(chunkWidthInBytes)
/// where chunkWidthInBytes = chunkWidth*RetDataTypeSizeInBytes(elementDataType)*elementDimX.
///
/// \param[in] chunkHeight
/// Chunk height in elements.
///
/// \param[in] chunkSpan
/// The span of the chunk src/dst memory region in CMEM.
///
/// \param[in] cmemAddr
/// The CMEM address where 2D chunks of data will be written/read.  It must be 16-bit friendly
/// (i.e. the address must be even).
///
/// \param[in] startCu
/// The index of the CU where the first chunk should be written/read (e.g. if \p tileWidthInChunks
/// is 32 and \p startCu is 4, the tile will be transferred to/from CUs 4-35).
/////////////////////////////////////////////////////////////////////////////////
void LLE_2DDMATransferConfig_XMEM_CMEM (uint32_t* llBuffer,
                                        icp::DATATYPE elementDataType,
                                        int32_t elementDimX,
                                        int32_t elementDimY,
                                        int32_t srcAddr,
                                        int32_t dstAddr,
                                        int32_t span,
                                        int32_t width,
                                        int32_t chunkWidth,
                                        int32_t chunkHeight,
                                        int32_t chunkSpan,
                                        int32_t cmemAddr,
                                        int32_t startCu);

/////////////////////////////////////////////////////////////////////////////////
/// Configure a linked list entry to express a 2D data transfer from one memory location
/// to another.
///
/// \param[out] llBuffer
/// Pointer to a contiguous region of 128-bit aligned memory that will contain the
/// generated linked list entry.  It is assumed that allocated memory is sufficient
/// to contain the linked list entry (i.e. at least 8 32-bit words).
///
/// \param[in] elementDataType
/// Element data type (see 'element' details above).
///
/// \param[in] elementDimX
/// Element x dimension (see 'element' details above).
///
/// \param[in] elementDimY
/// Element y dimension (see 'element' details above).
///
/// \param[in] srcAddr
/// Physical memory address indicating the source of the data transfer.
///
/// \param[in] dstAddr
/// Physical memory address indicating the destination of the data transfer.
///
/// \param[in] srcSpan
/// Span of the source region.
///
/// \param[in] dstSpan
/// Span of the destination region.
///
/// \param[in] chunkWidth
/// Width of the 2D region of data to be transferred (in elements).
///
/// \param[in] chunkHeight
/// Height of the 2D region of data to be transferred (in elements).
/////////////////////////////////////////////////////////////////////////////////
void LLE_2DDMATransferConfig_MEM_MEM (uint32_t* llBuffer,
                                      icp::DATATYPE elementDataType,
                                      int32_t elementDimX,
                                      int32_t elementDimY,
                                      int32_t srcAddr,
                                      int32_t dstAddr,
                                      int32_t srcSpan,
                                      int32_t dstSpan,
                                      int32_t chunkWidth,
                                      int32_t chunkHeight);

/////////////////////////////////////////////////////////////////////////////////
/// Configure a linked list entry to express a motion compensation engine (MCE) transfer from
/// memory to CMEM via the stream DMA (i.e. the transfer involves the MCE which transfers data from
/// memory to the streamin peripheral specified by \p dstAddr, and then from the streamin to CMEM).
/// Conceptually this transfer differs from 'LLE_2DDMATransferConfig_XMEM_CMEM(...)' in that a list
/// of chunk pointers/offsets are provided that allow a tile to be formed from non-contiguous 2D chunks
/// of data.
///
/// \param[out] llBuffer
/// Pointer to a contiguous region of 128-bit aligned memory that will contain the
/// generated linked list entry.  It is assumed that allocated memory is sufficient
/// to contain the linked list entry (i.e. at least 24 32-bit words).
///
/// \param[in] elementDataType
/// Element data type (see 'element' details above).
///
/// \param[in] elementDimX
/// Element x dimension (see 'element' details above).
///
/// \param[in] elementDimY
/// Element y dimension (see 'element' details above).
///
/// \param[in] baseAddr
/// Signed base value added to all offsets referred to by \p ptrArrayAddr.
///
/// \param[in] ptrArrayAddr
/// The physical address of the start of the 2D MCE pointer/offset table in memory.  Each
/// pointer/offset addresses a 2D chunk of source data.  In units of 'pointers', the width
/// of this 2D table must be a multiple of 4 (i.e 4,8,12,16 pointers wide, etc.).
///
/// \param[in] dstAddr
/// Destination in this context refers to the destination of the MCE transfer, which is the streamin DMA,
/// so this must be either DST_CMEM_DMA_STREAM_IN0 (if used with MCE channel 0) or DST_CMEM_DMA_STREAM_IN1
/// (if used with MCE channel 1).
///
/// \param[in] srcDataSpan
/// The span of the 2D region in memory that contains all required 2D chunks referenced by
/// \p ptrArrayAddr.
///
/// \param[in] tileWidthInChunks
/// Tile width in chunks (must be less <= the CU array width).
///
/// \param[in] chunkWidth
/// Chunk width in elements.  It must satisfy the restriction defined by DoesChunkWidthSatisfyMceHwLimits(chunkWidthInBytes)
/// where chunkWidthInBytes = chunkWidth*RetDataTypeSizeInBytes(elementDataType)*elementDimX.
///
/// \param[in] chunkHeight
/// Chunk height in elements.
///
/// \param[in] chunkSpan
/// The span of the chunk destination memory region in CMEM.
///
/// \param[in] cmemAddr
/// The CMEM address where 2D chunks of data will be written.  It must be 16-bit friendly
/// (i.e. the address must be even).
///
/// \param[in] startCu
/// The index of the CU where the first chunk should be written (e.g. if \p tileWidthInChunks
/// is 32 and \p startCu is 4, the tile will be transferred to CUs 4-35).
///
/// \param[in] maskCu_00
/// CU enable mask for CUs 0-31
///
/// \param[in] maskCu_32
/// CU enable mask for CUs 31-63
/////////////////////////////////////////////////////////////////////////////////
void LLE_MCETransferConfig (uint32_t* llBuffer, //size must be >=24 words
                            icp::DATATYPE elementDataType,
                            int32_t elementDimX,
                            int32_t elementDimY,
                            int32_t baseAddr,
                            int32_t ptrArrayAddr,
                            int32_t dstAddr,
                            int32_t srcDataSpan,
                            int32_t tileWidthInChunks,
                            int32_t chunkWidth,
                            int32_t chunkHeight,
                            int32_t chunkSpan,
                            int32_t cmemAddr,
                            int32_t startCu,
                            int32_t maskCu_00,
                            int32_t maskCu_32);

/////////////////////////////////////////////////////////////////////////////////
/// Configure a linked list entry to perform a memory fill operation.  This performs
/// a 1D memory fill of the memory region specified by the address \p dstAddr
/// via the 'memfill0' peripheral.  The 'memfill0' peripheral must be pre-configured
/// with the 'mcdma_hal_set_memory_fill_words(...)' call.  Fill functionality is
/// useful for initializing large blocks of memory with a specific value (e.g. zeroing
/// a region of memory).
///
/// \param[out] llBuffer
/// Pointer to a contiguous region of 128-bit aligned memory that will contain the
/// generated linked list entry.  It is assumed that allocated memory is sufficient
/// to contain the linked list entry (i.e. at least 8 32-bit words).
///
/// \param[in] dstAddr
/// Physical memory address indicating the start of the region to be filled.
///
/// \param[in] fillSizeInBytes
/// The size of the region to fill (in bytes).
/////////////////////////////////////////////////////////////////////////////////
void LLE_1D_Fill (uint32_t* llBuffer,  //size must be >=8 words
                  uint32_t dstAddr,    //physical destination address DMA will write to
                  int32_t fillSizeInBytes);

/////////////////////////////////////////////////////////////////////////////////
/// Configure a linked list entry to express a 1D data transfer from one memory location
/// to another.
///
/// \param[out] llBuffer
/// Pointer to a contiguous region of 128-bit aligned memory that will contain the
/// generated linked list entry.  It is assumed that allocated memory is sufficient
/// to contain the linked list entry (i.e. at least 8 32-bit words).
///
/// \param[in] srcAddr
/// Physical memory address indicating the source of the data transfer.
///
/// \param[in] dstAddr
/// Physical memory address indicating the destination of the data transfer.
///
/// \param[in] transferSizeInBytes
/// The size of the data transfer (in bytes).
/////////////////////////////////////////////////////////////////////////////////
void LLE_1D_MEM_MEM (uint32_t* llBuffer,  //size must be >=8 words
                     uint32_t srcAddr,    //physical source address DMA will read from
                     uint32_t dstAddr,    //physical destination address DMA will write to
                     int32_t transferSizeInBytes);

/////////////////////////////////////////////////////////////////////////////////
/// Adjust the DMA linked list entry pointed to by \p llEntry to indicate that
/// it is the last entry in the linked list.
///
/// \param[out] llEntry
/// Pointer to the DMA linked list entry that is to be the 'last' entry in the
/// linked list.
/////////////////////////////////////////////////////////////////////////////////
void LLE_DMA_Finalize(uint32_t* llEntry);

/////////////////////////////////////////////////////////////////////////////////
/// Adjust the MCE linked list entry pointed to by \p llEntry to indicate that
/// it is the last entry in the linked list.
///
/// \param[out] llEntry
/// Pointer to the MCE linked list entry that is to be the 'last' entry in the
/// linked list.
/////////////////////////////////////////////////////////////////////////////////
void LLE_MCE_Finalize(uint32_t* llEntry);

/////////////////////////////////////////////////////////////////////////////////
/// Forces the burst size to 1 (i.e. 8-bit mode) for the list entry pointed to by \p llEntry.
/// This exists to work around the CLOVER-398 APEX hardware issue.
///
/// \param[out] llEntry
/// Pointer to previously configured DMA linked list entry.
/////////////////////////////////////////////////////////////////////////////////
void LLE_DMA_SetBurstToINCR1(uint32_t* llEntry);

/////////////////////////////////////////////////////////////////////////////////
/// Calculate and return the optimal DMA word size based on the start address \p lPhysAddr,
/// the span \p lSpan, and the width \p lBytesPerLine associated with the data transfer.
///
/// \param[in] lPhysAddr
/// Physical memory address of the start of the data in memory.
///
/// \param[in] lSpan
/// Span of the data in memory.
///
/// \param[in] lBytesPerLine
/// The width of the data transfer in bytes (must be <= \p lSpan).
///
/// \return
/// The optimal DMA word size based on the input parameters.  See the
/// DMA_BURST_BEAT_SIZE enumeration for a list of possible return values.
/////////////////////////////////////////////////////////////////////////////////
int32_t CalcOptimalWordSize(int32_t lPhysAddr, int32_t lSpan, int32_t lBytesPerLine);

/////////////////////////////////////////////////////////////////////////////////
/// Check to see if the chunk width specified by \p lChunkWidthInBytes satisfies
/// APEX stream DMA hardware restrictions.
///
/// \param[in] lChunkWidthInBytes
/// Chunk width in bytes.
///
/// \return
/// 1 if the specified chunk width is acceptable, 0 otherwise.
/////////////////////////////////////////////////////////////////////////////////
int32_t DoesChunkWidthSatisfyStreamHwLimits(int32_t lChunkWidthInBytes);

/////////////////////////////////////////////////////////////////////////////////
/// Check to see if the chunk width specified by \p lChunkWidthInBytes satisfies
/// APEX motion compensation hardware restrictions.
///
/// \param[in] lChunkWidthInBytes
/// Chunk width in bytes.
///
/// \return
/// 1 if the specified chunk width is acceptable, 0 otherwise.
/////////////////////////////////////////////////////////////////////////////////
int32_t DoesChunkWidthSatisfyMceHwLimits(int32_t lChunkWidthInBytes);

//----------------------------------------------------------------------
//unused methods (not called in release code or documented)
//----------------------------------------------------------------------

void LLE_3DDMATransferConfig_MEM_MEM ( uint32_t* llBuffer, //size must be >=8 words
                                       icp::DATATYPE elementDataType,
                                       int32_t elementDimX,
                                       int32_t elementDimY,
                                       int32_t srcAddr,        // XMEM_Addr
                                       int32_t dstAddr,        // XMEM_Addr
                                       int32_t ptrArrayAddr,   // APPLY ON XMEM_Addr
                                       int32_t ptrOffsetVal,   // APPLY ON XMEM_Addr
                                       int32_t span,           // APPLY ON XMEM_Addr
                                       int32_t tileWidthInChunks,
                                       int32_t chunkWidth,
                                       int32_t chunkHeight,
                                       int32_t chunkSpan,
                                       uint8_t lTransferToDmemFlag,
                                       uint8_t lastFlag);

void LLE_3DDMATransferConfig (uint32_t* llBuffer,     //size must be >=24 words
                              icp::DATATYPE elementDataType,
                              int32_t elementDimX,
                              int32_t elementDimY,
                              int32_t srcAddr,        // XMEM_Addr/ STREAM_OUT0
                              int32_t dstAddr,        // STREAM_IN0/ STREAM_IN1/ XMEM_Addr
                              int32_t ptrArrayAddr,   // OFFSET TABLE (ALIGN32): INCR OFFSET & HOST VIEW
                              int32_t ptrOffsetVal,   // APPLY ON XMEM_Addr
                              int32_t span,           // APPLY ON XMEM_Addr
                              int32_t tileWidthInChunks,
                              int32_t chunkWidth,
                              int32_t chunkHeight,
                              int32_t chunkSpan,
                              int32_t cmemAddr,
                              int32_t startCu,
                              int32_t maskCu_00, int32_t maskCu_32,
                              uint8_t lastFlag);

void LLE_2DDMATransferConfig_XMEM_HRSZ (uint32_t* llBuffer,
                                        icp::DATATYPE elementDataType,
                                        int32_t elementDimX,
                                        int32_t elementDimY,
                                        int32_t srcAddr,
                                        int32_t dstAddr,
                                        int32_t span,
                                        int32_t width,
                                        int32_t height,
                                        uint32_t *pHrszConfig);

void LLE_2DDMATransferConfig_CMEM_HRSZ (uint32_t* llBuffer,
                                        icp::DATATYPE elementDataType,
                                        int32_t elementDimX,
                                        int32_t elementDimY,
                                        int32_t srcAddr,
                                        int32_t dstAddr,
                                        int32_t imageWidth,
                                        int32_t tileWidthInChunks,
                                        int32_t chunkWidth,
                                        int32_t chunkHeight,
                                        int32_t chunkSpan,
                                        int32_t cmemAddr,
                                        int32_t startCu,
                                        uint32_t *pHrszConfig);

#ifdef __cplusplus
}
#endif //__cplusplus

/// @}

#endif /* LINKEDLISTUTILITIES_H */
