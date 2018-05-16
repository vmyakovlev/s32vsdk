/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2015-2016 Freescale Semiconductor
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

#ifndef GDCOALSEMAPHORE_HPP
#define GDCOALSEMAPHORE_HPP


#include <global_errors.h>
#include <stdint.h>


// OAL Semaphore types
typedef enum
{
  GDC_OAL_SEMAPHORE_COUNTING,
  GDC_OAL_SEMAPHORE_MUTEX,
  GDC_OAL_SEMAPHORE_BINARY,
  GDC_OAL_SEMAPHORE_INVALID
} GDC_OAL_SEMAPHORE_TYPE;


////////////////////////////////////////////////////////////////////////
///
/// \addtogroup GDC_OAL_Semaphore GDC_OAL_Semaphore
///@{
/// \brief Critical region primitives
/// \details
/// Mandatory Function Call Ordering Sequence: in first task<br>
/// <ol><li>#GDC_OAL_SemaphoreCreate, always first
/// </li><li>#GDC_OAL_SemaphoreDelete, only when region needs no further protection
/// </li></ol>
///
/// Mandatory Function Call Ordering Sequence: in any task after first task item 1, before item 2<br>
/// <ol><li>#GDC_OAL_SemaphoreObtain, #GDC_OAL_SemaphoreTryObtain or #GDC_OAL_SemaphoreObtain
/// </li><li>if obtained, manipulate critical data
/// </li><li>if obtained, #GDC_OAL_SemaphoreRelease
/// </li></ol>
///
////////////////////////////////////////////////////////////////////////


/// Semaphore type
typedef struct tGdcOalSemaphore* GDC_OAL_SEMAPHORE;   ///< an opaque pointer used to reference a created Semaphore

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
/// - GDC_OAL_SEMAPHORE_BINARY
/// - GDC_OAL_SEMAPHORE_COUNTING
/// - GDC_OAL_SEMAPHORE_MUTEX
/// \param count - [in] Initial count for counting semaphores
///
/// \return
/// - #LIB_SUCCESS
/// - #LIB_FAILURE
/// - #OAL_ERR_SEM_ENOMEM
/// - #OAL_ERR_SEM_EBADTYPE
////////////////////////////////////////////////////////////////////////
LIB_RESULT GDC_OAL_SemaphoreCreate(GDC_OAL_SEMAPHORE *pSem,
    const char *pName, GDC_OAL_SEMAPHORE_TYPE type, unsigned int count);

////////////////////////////////////////////////////////////////////////
/// \par Descriptions:
///  Wait on a semaphore until it is obtained.
///
/// \param sem - [in] Semaphore ID
///
/// \return
///    - #LIB_SUCCESS
///    - #OAL_ERR_SEM_EBADSEM
///    - #LIB_FAILURE
////////////////////////////////////////////////////////////////////////
LIB_RESULT GDC_OAL_SemaphoreObtain(GDC_OAL_SEMAPHORE sem);

////////////////////////////////////////////////////////////////////////
/// \par Descriptions:
/// Try to obtain a semaphore. If the Semaphore is not available, the
/// function will return immediately with an error. If it is available
/// a SUCCESS return code will be returned.
///
/// \param sem - [in] Semaphore ID
///
/// \return
///    - #LIB_SUCCESS
///    - #OAL_ERR_SEM_ESEMLOCKED
///    - #OAL_ERR_SEM_EBADSEM
///    - #LIB_FAILURE
////////////////////////////////////////////////////////////////////////
LIB_RESULT GDC_OAL_SemaphoreTryObtain(GDC_OAL_SEMAPHORE sem);

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
///    - #LIB_SUCCESS
///    - #OAL_ERR_SEM_ETIMEOUT
///    - #OAL_ERR_SEM_EBADSEM
///    - #OAL_ERR_SEM_RESET
///    - #LIB_FAILURE
////////////////////////////////////////////////////////////////////////
LIB_RESULT GDC_OAL_SemaphoreObtain(GDC_OAL_SEMAPHORE sem, unsigned int time);

////////////////////////////////////////////////////////////////////////
/// \par Descriptions:
/// Signal or release a semaphore assuming the passed Semaphore ID has
/// already been created.
///
/// \param sem - [in] Semaphore ID
///
/// \return
///    - #LIB_SUCCESS
///    - #LIB_FAILURE
////////////////////////////////////////////////////////////////////////
LIB_RESULT GDC_OAL_SemaphoreRelease(GDC_OAL_SEMAPHORE sem);

////////////////////////////////////////////////////////////////////////
/// \par Descriptions:
/// Reset the semaphore. Tasks suspended on the semaphore will be
/// resumed with the appropriate error code.
///
/// \param sem - [in] Semaphore ID
/// \param count - [in] Initial count for the reset semaphore
///
/// \return
///    - #LIB_SUCCESS
///    - #LIB_FAILURE
////////////////////////////////////////////////////////////////////////
LIB_RESULT GDC_OAL_SemaphoreReset(GDC_OAL_SEMAPHORE sem, unsigned int count);

////////////////////////////////////////////////////////////////////////
/// \par Descriptions:
/// Delete a semaphore assuming the Semaphore ID argument has been
/// already created.
///
/// \param sem - [in] Semaphore ID
///
/// \return
///    - #LIB_SUCCESS
///    - #OAL_ERR_SEM_EBADSEM
////////////////////////////////////////////////////////////////////////
LIB_RESULT GDC_OAL_SemaphoreDelete(GDC_OAL_SEMAPHORE sem);


#endif /* GDCOALSEMAPHORE_HPP */
