/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
* All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
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
* \file Oal_semaphore.cpp
* \brief OAL semaphore implementation
* \author Igor Aleksandrowicz and Rostislav Hulik
* \version
* \date 4-March-2016
****************************************************************************/

#include <oal.h>
extern bool ddr_init;
////////////////////////////////////////////////////////////////////////
/// Create an OS semaphore. If succeeds, the created Semaphore ID
/// will be subsequently referred to in later Semaphore calls.
/// NOTE: For OS specific stuff see S32V234 Platform user guide
////////////////////////////////////////////////////////////////////////
int32_t OAL_SemaphoreCreate(OAL_SEMAPHORE *pSem,
    const char *pName, OAL_SEMAPHORE_TYPE type, unsigned int count)
{
  CHECK_INIT
  return OAL_SUCCESS;
}

////////////////////////////////////////////////////////////////////////
/// Try to obtain a semaphore until it times out. If the Semaphore is
/// not available, the calling task will be blocked until the Semaphore
/// becomes available or times out.
////////////////////////////////////////////////////////////////////////
int32_t OAL_SemaphoreObtain(OAL_SEMAPHORE sem, unsigned int time)
{
  CHECK_INIT
  return OAL_SUCCESS;
}

////////////////////////////////////////////////////////////////////////
/// Signal or release a semaphore assuming the passed Semaphore ID has
/// already been created.
////////////////////////////////////////////////////////////////////////
int32_t OAL_SemaphoreRelease(OAL_SEMAPHORE sem)
{
  CHECK_INIT
  return OAL_SUCCESS;
}

////////////////////////////////////////////////////////////////////////
/// Delete a semaphore assuming the Semaphore ID argument has been
/// already created.
////////////////////////////////////////////////////////////////////////
int32_t OAL_SemaphoreDelete(OAL_SEMAPHORE sem)
{
  CHECK_INIT
  return OAL_SUCCESS;
}
