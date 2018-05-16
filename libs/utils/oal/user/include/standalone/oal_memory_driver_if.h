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
* \file standalone_oal_cont_alloc.h
* \brief OAL memory allocation implementation for Standalone (contiguous allocator)
* \author Rostislav Hulik
* \version
* \date 4-March-2016
****************************************************************************/
#ifndef STANDALONEOALCONTALLOC_H
#define STANDALONEOALCONTALLOC_H

/////////////////////////////////////////////////////////////////////////
///
/// Allocates a block of memory with specified size and alignment
/// Uses underlying oal kernel module to do so
///
/// \param size    - [in] Amount of memory to allocate in bytes
/// \param align   - [in] Alignment
/// \param chunk_id - [in] Chunk where the memory should be allocated
///
/// \return 
///    - On Success: The Start address of the memory block
///    - On Failure: NULL
/////////////////////////////////////////////////////////////////////////
void * OAL_ContAllocAlloc(uint64_t size, uint64_t align, uint8_t chunk_id);

/////////////////////////////////////////////////////////////////////////
///
/// Free mapped memory from underlying oal kernel module
/// If memory lacks the physical pointer (allocated externally), it's freed
/// using free()
///
/// \param handle_pointer    - [in] Pointer user to handle the memory
///
/// \return 
///    - On Success: LIB_SUCCESS
///    - On Failure: LIB_FAILURE
/////////////////////////////////////////////////////////////////////////
int32_t OAL_ContAllocFree(void *handle_pointer);

int32_t OAL_ContAllocUnmap(void *handle_pointer);

/////////////////////////////////////////////////////////////////////////
///
/// Returns a buffer of specified type.
/// If type is not specified or cannot be mapped, returns NULL
///
/// \param handle_pointer    - [in] Pointer user to handle the memory
/// \param type              - [in] Type of memory to be returned
///
/// \return 
///    - On Success: The Start address of the saved memory block
///    - On Failure: NULL
/////////////////////////////////////////////////////////////////////////
void *OAL_ContAllocGetBuffer(uint64_t handle_pointer, uint64_t type);


/////////////////////////////////////////////////////////////////////////
///
/// Returns a number of current allocations from all processes
///
/// \return 
///    Number of current allocations
/////////////////////////////////////////////////////////////////////////
uint64_t OAL_ContAllocGetNumAllocations();

/////////////////////////////////////////////////////////////////////////
///
/// Returns a number of current mappings from all processes
///
/// \return 
///    Number of current mappings
/////////////////////////////////////////////////////////////////////////
uint64_t OAL_ContAllocGetNumMappings();

/////////////////////////////////////////////////////////////////////////
///
/// Function returns total size in bytes being managed by OAL Memory.
///
/// \return
///    total size in bytes being managed by OAL Memory
/////////////////////////////////////////////////////////////////////////
int64_t OAL_ContAllocGetTotalSize();

/////////////////////////////////////////////////////////////////////////
///
/// Function returns free size in bytes remaining in OAL Memory.
///
/// \return
///    free size in bytes remaining in OAL Memory
/////////////////////////////////////////////////////////////////////////
int64_t OAL_ContAllocGetFreeSize();

/////////////////////////////////////////////////////////////////////////
///
/// Flushes and invalidates the whole cache
///
/////////////////////////////////////////////////////////////////////////
void CleanAndInvalidateDCacheAll();

/////////////////////////////////////////////////////////////////////////
///
/// Invalidates the specified cache area
///
/////////////////////////////////////////////////////////////////////////
void InvalidateDCache(void *ptr, uint32_t size);

/////////////////////////////////////////////////////////////////////////
///
/// Flushes the specified cache area
///
/////////////////////////////////////////////////////////////////////////
void CleanDCache(void *ptr, uint32_t size);

/////////////////////////////////////////////////////////////////////////
///
/// Flushes and invalidates the specified cache area
///
/////////////////////////////////////////////////////////////////////////
void CleanAndInvalidateDCache(void *ptr, uint32_t size);

  
#endif /* STANDALONEOALCONTALLOC_H */
