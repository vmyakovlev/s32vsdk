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
* \file OAL_Process.h
* \brief OAL process and inter-process communication
* \author Igor Aleksandrowicz
* \version
* \date 02-August-2013
****************************************************************************/

#ifndef OALPROCESS_H
#define OALPROCESS_H

#include <oal.h>
/*****************************************************************************
* prototypes
*****************************************************************************/
/// opaque handle for OAL Shared Memory
typedef struct sharedMem_s *OAL_SHARED_MEM;

/*****************************************************************************
* prototypes
*****************************************************************************/

/****************************************************************************/
/** Spawns new process.
*
* \param fileName path to the executable
* \param aArgC number of arguments passed to the process
* \param appArgV arguments passed into the process
* \param pFailureFunction pointer to function which should be used when
*        spawning fails in the child process, returns int passed to exit(),
*        can be NULL (no function called)
* \param pFFarg failure function's argument
*
* \return LIB_SUCCESS on success, LIB_FAILURE otherwise
*****************************************************************************/
OAL_RESULT OAL_ProcessSpawn(const char* apFileName, int aArgC,
    char** appArgV, int (*pFailureFunction)(void*), void* pFFarg);

/****************************************************************************/
/** Creates shared memory.
*
* \param pSharedMem pointer to shared memory handle
* \param apName name of the shared memory, max 7 characters (linger names will
*        be truncated)
* \param aSize size of the shared memory
*
* \return LIB_SUCCESS on success, LIB_FAILURE otherwise
*****************************************************************************/
OAL_RESULT OAL_SharedMemoryCreate(OAL_SHARED_MEM* apSharedMem,
  const char* apName, uint32_t aSize);

/****************************************************************************/
/** Gets shared memory by name.
*
* \param apSharedMem pointer to shared memory handle
* \param apName name of the shared memory
*
* \return LIB_SUCCESS on success, LIB_FAILURE otherwise
*****************************************************************************/
OAL_RESULT OAL_SharedMemoryGetByName(OAL_SHARED_MEM* apSharedMem,
  const char* apName);

/****************************************************************************/
/** Gets shared memory by name.
*
* \param apSharedMem pointer to shared memory handle
* \param apName name of the shared memory
* \param aSize specifies the shared memory size to be mapped
*
* \return LIB_SUCCESS on success, LIB_FAILURE otherwise
*
* \note Added for Integrity OS -> fstat not implemented for posix_shm regions.
*****************************************************************************/
OAL_RESULT OAL_SharedMemoryGetByName(OAL_SHARED_MEM* apSharedMem,
  const char* apName, uint32_t aSize);

/****************************************************************************/
/** Gets a pointer to the shared memory.
*
* \param apPointer pointer to pointer
* \param aSharedMem shared memory to get the pointer from
*
* \return LIB_SUCCESS on success, LIB_FAILURE otherwise
*****************************************************************************/
OAL_RESULT OAL_SharedMemoryGetPointer(void** apPointer,
    OAL_SHARED_MEM aSharedMem);

/****************************************************************************/
/** Forces shared memory to be remapped with new size areturns new pointer.
*
* \param apPointer pointer to pointer
* \param aSharedMem shared memory to get the pointer from
*	\param aSize new size of the memory to be mapped
*
* \return LIB_SUCCESS on success, LIB_FAILURE otherwise
*****************************************************************************/
OAL_RESULT OAL_SharedMemoryGetPointer(void** apPointer,
    OAL_SHARED_MEM aSharedMem, uint32_t aSize);

/****************************************************************************/
/** Returns mapped size of the shared memory region.
*
* \param aSharedMem shared memory to get the pointer from
*
* \return size of the shared memory region
*****************************************************************************/
uint32_t OAL_SharedMemorySizeGet(OAL_SHARED_MEM aSharedMem);

/****************************************************************************/
/** Destroys shared memory.
*
* \param aSharedMem shared memory to destroy
*
* \return LIB_SUCCESS on success, LIB_FAILURE otherwise
*
* \note only the process which created the shared memory can destroy it
* completely, otherwise destroys local data only
*****************************************************************************/
OAL_RESULT OAL_SharedMemoryDestroy(OAL_SHARED_MEM aSharedMem);

/****************************************************************************/
/** Sets shared memory ownership.
 *
 * Used to identify, who shoult call unlink.
 *
 * \param aSharedMem
 * \param aOwnership
 *
 * \return LIB_SUCCESS on success, LIB_FAILURE otherwise
 *
 * \note added to suite gdi needs
 */
OAL_RESULT OAL_SharedMemoryOwnershipSet(OAL_SHARED_MEM aSharedMem,
    bool aOwnership);

#endif /* OALPROCESS_H */
