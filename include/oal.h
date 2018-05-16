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

#ifndef OAL_H
#define OAL_H

#include <stdint.h>
#include "../libs/utils/oal/kernel/include/linux/oal_chunk_ids.h"
#include "../libs/utils/oal/common/include/oal_common.h"

// #ifdef __cplusplus
// extern "C" {
// #endif
typedef enum 
{
  OAL_SUCCESS               = 0, 
  OAL_FAILURE               = -1,
  OAL_SEM_ENOMEM        = 0x90006,
  OAL_SEM_EBADSEM       = 0x90004,
  OAL_SEM_ESEMLOCKED    = 0x90002,
  OAL_SEM_ETIMEOUT      = 0x90003,
  OAL_SEM_EBADTYPE      = 0x90005
} OAL_RESULT;

////////////////////////////////////////////////////////////////////////
/// \file oal.h
/// \addtogroup OAL_Memory OAL Memory
/// @{
///
/// \name Memory functions
///
/// This API allows dynamically allocating a block of memory described by a
/// pointer whose address component points to a virtual address interpreted
/// by the MMU. MMU configures various regions as cache-able,
/// non-cacheable, etc. and handles the mapping of this virtual address
/// to its physical location and other properties associated with the
/// caching mechanisms that are available.
///
/// Application code running on the ARM should not care about the
/// virtualness of any pointers and considers that property transparent to
/// normal operation. There are certain low level operations that can
/// optimized by accessing memory in different way. MMU regions allow the
/// same physical memory location to be mapped with different
/// properties. It is possible using the #OAL_MemoryReturnAddress() API to
/// retrieve an address to have the appropriate properties that are handy
/// for the current use case. If the subsequent are going to be executed by
/// the hardware, one of the <I>Cache Control APIs</I> (e.g. #OAL_MemoryFlush())
/// can be used to make sure that the cache coherency is maintained.
///
/// One example is when writing to the registers of a device; we don't want
/// to use a cacheable virtual pointer because data won't be written to the
/// device. In this case using the non-cacheable virtual pointer is the only
/// way.
///
/// Additionally, for devices that are able to communicate with each or
/// read/write to memory on their own, the only possible value that they can
/// be programmed with is the physical location of that memory. It is
/// possible to retrieve the physical address associate with a virtual
/// pointer to configure the parameters of a device. Physical addresses
/// should only be used in such cases.
///
/// Mandatory Function Call Ordering Sequence: for allocations<br>
/// <ol><li>#OAL_MemoryAlloc
/// </li><li>manipulate data
/// </li><li>optionally: #OAL_MemoryFlushAndInvalidate
/// </li><li>optionally: #OAL_MemoryReturnAddress() [ACCESS_PHY]
/// </li><li>optionally: configure the DMA with the returned address 
///                      and wait for transfer/copy complete
/// </li><li>#OAL_MemoryFree
/// </li></ol>
///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
/// 
/// After the memory is allocated, that memory can be accessed in various
/// ways. Depending on how the user is manipulating the data in the memory
/// space, at certain points in time different accesses may be required. 
/// Use #OAL_MemoryReturnAddress() to get the address that is coupled with 
/// the requested access type. 
///
////////////////////////////////////////////////////////////////////////
enum
{
  ACCESS_PHY    = 0,    ///< Physical access
  ACCESS_CH_WB  = 1,  ///< Cached access with Writeback policy
  ACCESS_CH_WT  = 2,  ///< Cached access with Writethrough policy
  ACCESS_NCH_B  = 3,  ///< Non-Cached Bufferable access
  ACCESS_NCH_NB = 4,  ///< Non-Cached Non-Bufferable Access
  ACCESS_NUM    = 5
};   

typedef uint32_t OAL_MEMORY_ACCESS;
typedef uint32_t OAL_MEMORY_FLAG;
#define ALIGN2_CACHELINE                     0x00000100           ///< Memory is aligned on a cache line
#define ALIGN2_PAGE                          0x00000200           ///< Memory is aligned on a page
#define BYTE_N                               0x00000400
#define ALIGN2_BYTE(N)                       (BYTE_N | (N << 16)) ///< Memory is aligned on an N-byte boundary
#define OAL_MEMORY_FLAG_CONTIGUOUS           0x01                 ///< Memory block is physically contiguous
#define OAL_MEMORY_FLAG_NONSWAPABLE          0x02                 ///< Memory allocated will never be swapped
#define OAL_MEMORY_FLAG_ZERO                 0x04                 ///< Memory is initialized with value zero
#define OAL_MEMORY_FLAG_ALIGN(ALIGN2)        (0x08 | (ALIGN2))    ///< Memory is aligned as specified

///< Memory will be allocated on DDR0 - fails if DDR0 allocations are at maximum
enum
{
  ///< Memory will be allocated on DDR0 - fails if DDR0 allocations are at maximum
  OAL_ALLOC_DDR0 = (OAL_CHUNK_ID_DDR0 << 24),
  ///< Memory will be allocated on DDR1 - fails if DDR1 allocations are at maximum
  OAL_ALLOC_DDR1 = (OAL_CHUNK_ID_DDR1 << 24),
  ///< Memory will be allocated on SRAM - fails if SRAM allocations are at maximum
  OAL_ALLOC_SRAM_SINGLE = (OAL_CHUNK_ID_SRAM_SINGLE << 24),
  ///< Memory will be allocated on SRAM - fails if SRAM allocations are at maximum
  OAL_ALLOC_SRAM_MULTI = (OAL_CHUNK_ID_SRAM_MULTI << 24),
  ///< Memory will be allcated on chunk with the least allocations
  ///< fails when no suitable chunk is found
  OAL_ALLOC_AUTO = (OAL_CHUNK_ID_AUTO << 24)
};

/////////////////////////////////////////////////////////////////////////
///
/// Allocate a block of memory that meets certain criteria. Memory will be allocated from
/// the appropriate heap while satisfying all the properties of flags. Memory
/// allocation of specified size will allocate a block of size and return 
/// a valid pointer.
///
/// \param size    - [in] Amount of memory to allocate in bytes
/// \param flags   - [in] Memory flags
///
/// \return 
///    - On Success: The Start address of the memory block
///    - On Failure: NULL
/////////////////////////////////////////////////////////////////////////
void *OAL_MemoryAllocFlag(uint32_t size, OAL_MEMORY_FLAG flags = OAL_ALLOC_AUTO);

////////////////////////////////////////////////////////////////////////
///
/// \details
/// Takes the given memory pointer and returns the pointer
/// that allows proper access as identified in the access parameter
///
/// The value of pBuffer passed has to be the value returned from one 
/// of the #OAL_MemoryAlloc functions.
/// 
/// - #ACCESS_PHY   : Returns the physical address of memory. Accessing this via
/// the CPU will fail.
/// - #ACCESS_CH_WB : Returns the address that will be interpreted as cached with a 
/// write back policy. 
/// - #ACCESS_CH_WT : Returns the address that will be interpreted as cached with a 
/// write through policy. 
/// - #ACCESS_NCH_B : Returns the address that will be interpreted as non-cached 
/// but buffered. 
/// - #ACCESS_NCH_NB: Returns the address that will be interpreted as non-cached 
/// and not buffered. 
/// 
/// \param pBuffer - [in] Pointer to a memory buffer; it must be a pointer
///                returned by #OAL_MemoryAlloc() or variations. 
/// \param access  - [in] Access Type requested
///
/// \return
///        Address of buffer with appropriate access type
///
/// \warning User is responsible from appropriately handling 
/// coherency issues. This function is just going to generate an address. 
/// It will not attempt to resolve the coherency.
////////////////////////////////////////////////////////////////////////
void *OAL_MemoryReturnAddress(void *pBuffer, OAL_MEMORY_ACCESS access);

////////////////////////////////////////////////////////////////////////
/// 
/// Release the allocated memory and return it to memory manager.
///
/// \param pBuffer - [in] Start address of the allocated memory block; it must be a pointer
///                returned by #OAL_MemoryAlloc() or variations. 
////////////////////////////////////////////////////////////////////////
int32_t OAL_MemoryFree(void *pBuffer);

////////////////////////////////////////////////////////////////////////
/// 
/// Unmap virtual space mapping of the allocated buffer. 
/// The cache is not flushed when called, must be done outside this function if necessary.
///
/// \param pBuffer - [in] Start address of the allocated memory block; it must be a pointer
///                returned by #OAL_MemoryAlloc() or variations. 
////////////////////////////////////////////////////////////////////////
void OAL_MemoryUnmap(void *pBuffer);


/// \name Cache Control Functions
/// 
/// These functions operate on a memory region specified by the user. 
/// There are no restrictions on the pointer that may be passed. They
/// find if any part of the region specified by pAddr and size are 
/// currently held in the cache and operate only on those cache lines.
/// 
/// The pointer passed Must be returned by #OAL_MemoryAlloc() or
/// its variations.
/// 
/// \warning The flush & invalidate functions are not thread safe 
///          for speed purposes. The user needs to ensure the thread
///          & process safety outside those function.
///
/// Flush: If any region of this memory is currently held in
/// the CPU cache, those contents will be written back to 
/// physical memory. 
/// 
/// FlushAndInvalidate: On top of the flush operations, all the
/// cache lines will be marked invalid. This will require 
/// a subsequent read operation to fetch data from physical memory. 
///
/// Invalidate: Simply scratches out all the cache lines. 
/// Data kept in the cache is lost. Any subsequent read will fetch
/// the data from physical memory
///@{ 

///
/// \param pAddr   - [in] Start Address of the memory region to be flushed; it must
///                be returned by #OAL_MemoryAlloc() or its variations. 
/// \param size    - [in] Size of the memory region
///
void OAL_MemoryFlush                 (void *pAddr, uint32_t size);

///
/// \param pAddr   - [in] Start Address of the memory region to be invalidated;
///                it must be returned by #OAL_MemoryAlloc() or its
///                variations.  
/// \param size    - [in] Size of the memory region
///
void OAL_MemoryInvalidate            (void *pAddr, uint32_t size);

///
/// \param pAddr   - [in] Start Address of the memory region to be flushed and
///                invalidated. It must be returned by #OAL_MemoryAlloc() or
///                its variations. 
/// \param size    - [in] Size of the memory region
///
void OAL_MemoryFlushAndInvalidate    (void *pAddr, uint32_t size);

///
/// Invalidate the whole cache.
///
void OAL_MemoryFlushAndInvalidateAll (void);
///@}
///@}
////////////////////////////////////////////////////////////////////////
///
/// \addtogroup OAL_Semaphore OAL_Semaphore
///@{
/// \brief Critical region primitives
/// \details
/// Mandatory Function Call Ordering Sequence: in first task<br>
/// <ol><li>#OAL_SemaphoreCreate, always first
/// </li><li>#OAL_SemaphoreDelete, only when region needs no further protection
/// </li></ol>
///
/// Mandatory Function Call Ordering Sequence: in any task after first task item 1, before item 2<br>
/// <ol><li>#OAL_SemaphoreObtain, #OAL_SemaphoreTryObtain or #OAL_SemaphoreTimedWait
/// </li><li>if obtained, manipulate critical data
/// </li><li>if obtained, #OAL_SemaphoreRelease
/// </li></ol>
/// 
////////////////////////////////////////////////////////////////////////

/// Semaphore type
typedef struct tOalSemaphore* OAL_SEMAPHORE;   ///< an opaque pointer used to reference a created Semaphore

typedef enum
{
  OAL_SEMAPHORE_COUNTING,
  OAL_SEMAPHORE_MUTEX,
  OAL_SEMAPHORE_BINARY,
  OAL_SEMAPHORE_INVALID
} OAL_SEMAPHORE_TYPE;

////////////////////////////////////////////////////////////////////////
/// \par Descriptions:
/// Create an OS semaphore. If succeeds, the created Semaphore ID
/// will be subsequently referred to in later Semaphore calls.
///
/// Semaphores types are differentiated based on two attributes;
/// the legal values for the semaphore count and the resumption order
/// of suspended tasks.
///
/// Binary and mutex type semaphores are restricted to a count of 0 or 1.
/// Counting semaphores are allowed to have count values greater than 1.
/// The value of the count represents the number of times the semaphore can
/// be obtained without suspending the calling task. A task trying to obtain a
/// semaphore with a count of 0 will be suspended. Obtaining a semaphore will
/// decrement the count if it is greater than 0 and releasing increments the count.
///
/// Binary and counting type semaphores will resume tasks in the order in which
/// they tried to obtain the semaphore; a first in, first out order.
/// Mutex semaphores will resume first the higher priority tasks followed by the lower
/// priority tasks; within the same priority the order is first in, first out,(FIFO).
///
/// \param pSem - [out] Semaphore ID will be written to (*pSem)
/// \param pName - [in] String identifying new semaphore(name will be truncated to 7 characters)
/// \param type - [in] Semaphore type:
/// - OAL_SEMAPHORE_BINARY
/// - OAL_SEMAPHORE_COUNTING
/// - OAL_SEMAPHORE_MUTEX
/// \param count - [in] Initial count for counting semaphores
///
/// \return
/// - #OAL_SUCCESS
/// - #OAL_FAILURE
/// - #OAL_ERR_SEM_ENOMEM
/// - #OAL_ERR_SEM_EBADTYPE
///
/// NOTE: For OS specific stuff see S32V234 Platform user guide
////////////////////////////////////////////////////////////////////////
int32_t OAL_SemaphoreCreate(OAL_SEMAPHORE *pSem,
                               const char *pName,
                               OAL_SEMAPHORE_TYPE type,
                               unsigned int count);

////////////////////////////////////////////////////////////////////////
/// \par Descriptions:
///  Wait on a semaphore until it is obtained.
///
/// \param sem - [in] Semaphore ID
/// \param time - [in] Timeout in microseconds
///
/// \return
///    - #OAL_SUCCESS
///    - #OAL_ERR_SEM_EBADSEM
///    - #OAL_FAILURE
////////////////////////////////////////////////////////////////////////
int32_t OAL_SemaphoreObtain(OAL_SEMAPHORE sem);

////////////////////////////////////////////////////////////////////////
/// \par Descriptions:
/// Try to obtain a semaphore. If the Semaphore is not available, the
/// function will return immediately with an error. If it is available
/// a SUCCESS return code will be returned.
///
/// \param sem - [in] Semaphore ID
///
/// \return
///    - #OAL_SUCCESS
///    - #OAL_ERR_SEM_ESEMLOCKED
///    - #OAL_ERR_SEM_EBADSEM
///    - #OAL_FAILURE
////////////////////////////////////////////////////////////////////////
int32_t OAL_SemaphoreTryObtain(OAL_SEMAPHORE sem);

////////////////////////////////////////////////////////////////////////
/// \par Descriptions:
/// Try to obtain a semaphore until it times out. If the Semaphore is
/// not available, the calling task will be blocked until the Semaphore
/// becomes available or times out.
///
/// NOTE :  Timeout is expressed in microseconds but true timeout period
///         will be rounded up to the granularity of the OS timer which is 10ms.
///
/// \param sem - [in] Semaphore ID
/// \param time - [in] Timeout in microseconds
///
/// \return
///    - #OAL_SUCCESS
///    - #OAL_ERR_SEM_ETIMEOUT
///    - #OAL_ERR_SEM_EBADSEM
///    - #OAL_ERR_SEM_RESET
///    - #OAL_FAILURE
////////////////////////////////////////////////////////////////////////
int32_t OAL_SemaphoreObtain(OAL_SEMAPHORE sem, unsigned int time);

////////////////////////////////////////////////////////////////////////
/// \par Descriptions:
/// Signal or release a semaphore assuming the passed Semaphore ID has
/// already been created.
///
/// \param sem - [in] Semaphore ID
///
/// \return
///    - #OAL_SUCCESS
///    - #OAL_FAILURE
////////////////////////////////////////////////////////////////////////
int32_t OAL_SemaphoreRelease(OAL_SEMAPHORE sem);

////////////////////////////////////////////////////////////////////////
/// \par Descriptions:
/// Delete a semaphore assuming the Semaphore ID argument has been
/// already created.
///
/// \param sem - [in] Semaphore ID
///
/// \return
///    - #OAL_SUCCESS
///    - #OAL_ERR_SEM_EBADSEM
////////////////////////////////////////////////////////////////////////
int32_t OAL_SemaphoreDelete(OAL_SEMAPHORE sem);

// end of \addtogroup OAL_Semaphore OAL Semaphore
///@}

////////////////////////////////////////////////////////////////////////
///
/// \addtogroup OAL_Task OAL_Task
///@{
/// \brief Threading functionality
/// 
////////////////////////////////////////////////////////////////////////

typedef void* OAL_TCB;                           ///< opaque value returned to the task creator to be used when referring to a task
typedef void *(*OAL_TASK_ENTRY)(void *);         ///< opaque value describing the task entry function
#define OAL_TASK_DEFAULT_PRIORITY       127      ///< default task priority
#define OAL_TASK_DEFAULT_STACK          8388608  ///< default task stack size

///////////////////////////////////////////////////////////////////////////////////////
/// \par Description
/// Create an OS Task.  If succeeded, the created Task ID
/// will be subsequently referred to in later OS Task calls.
///
/// \param task_entry     - Entry point function
/// \param argument       - Argument to be passed to the entry point
/// \param stack_size     - Stack size for the task in bytes, or OAL_TASK_DEFAULT_STACK
/// \param priority       - Priority for the task, or OAL_TASK_DEFAULT_PRIORITY
/// \param task_name      - Task name
/// \param startedOption  - Create and start the task right away or wait.
/// \param task           - [IN/OUT] pointer to #OAL_TCB which will be updated with the TCB of the created task
///
/// \return
///    - #OAL_SUCCESS
///    - #OAL_FAILURE
///////////////////////////////////////////////////////////////////////////////////////
int32_t OAL_TaskInit(OAL_TASK_ENTRY task_entry, void* argument,
    unsigned int stack_size, unsigned int priority, char* task_name,
    OAL_TCB *task);

////////////////////////////////////////////////////////////////////////
/// \if MMPAPIsOnly
/// \par Description
/// Delete the specified task assuming the specified task ID has
/// already been created.
///
/// \param task - the created task ID
///
/// \return
///    - #OAL_SUCCESS
///    - #OAL_FAILURE
/// \endif
////////////////////////////////////////////////////////////////////////
int32_t OAL_TaskDelete(OAL_TCB task);

////////////////////////////////////////////////////////////////////////
/// \par Description
/// Waits for the task to terminate.
///
/// \param task - the created task ID
///
/// \return
///    - On Success: OAL_SUCCESS
///    - On Failure: OAL_FAILURE
////////////////////////////////////////////////////////////////////////
int32_t OAL_TaskJoin(OAL_TCB task);

// end of \addtogroup OAL_Task OAL Task
///@}


#include <oal_irq.h>

// #ifdef __cplusplus
// }
// #endif

#endif /* OAL_H */
