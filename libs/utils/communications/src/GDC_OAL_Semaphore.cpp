/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2015 Freescale Semiconductor;
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
#include "GDC_OAL_Semaphore.hpp"



#ifndef __STANDALONE__

#include <ctime>
#include <sys/types.h>
#include <semaphore.h>
#include <cerrno>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <new>
//
//
// Semaphores
//
//



//
//
//
LIB_RESULT GDC_OAL_SemaphoreCreate(GDC_OAL_SEMAPHORE* pSem,
                                   const char*,
                                   GDC_OAL_SEMAPHORE_TYPE,
                                   unsigned int count)
{
   LIB_RESULT libResult = LIB_FAILURE;

   *pSem = (GDC_OAL_SEMAPHORE)malloc(sizeof(sem_t));

   if(*pSem != NULL)
   {
      int intResult = sem_init((sem_t*)*pSem,
                               0,         // [0] Not shared between processes
                               count);
      if(intResult == 0)
      {
         libResult = LIB_SUCCESS;
      }
   }

   return libResult;
}



//
//
//
LIB_RESULT GDC_OAL_SemaphoreObtain(GDC_OAL_SEMAPHORE sem, unsigned int time)
{
   LIB_RESULT libResult = LIB_FAILURE;

   int intResult = -1;

   if(time == 0xFFFFFFFF)
   {
      intResult =sem_wait((sem_t*)sem);
      while (-1 == intResult && EINTR == errno)
         intResult = sem_wait((sem_t*)sem);
   }
   else
   {
      timespec lTimespec;

      lTimespec.tv_sec  = time / 1000000;
      lTimespec.tv_nsec = (time % 1000000) * 1000;

      intResult = sem_timedwait((sem_t*)sem,
                                &lTimespec);
      while (-1 == intResult && EINTR == errno)
         intResult = sem_timedwait((sem_t*)sem, &lTimespec);
   }
   if(intResult == 0)
   {
      libResult = LIB_SUCCESS;
   }

   return libResult;
}


//
//
//
LIB_RESULT GDC_OAL_SemaphoreTryObtain(GDC_OAL_SEMAPHORE sem)
{
   return GDC_OAL_SemaphoreObtain(sem, 0);
}


//
//
//
LIB_RESULT GDC_OAL_SemaphoreRelease(GDC_OAL_SEMAPHORE sem)
{
   LIB_RESULT libResult = LIB_FAILURE;

   int intResult = sem_post((sem_t*)sem);

   if(intResult == 0)
   {
      libResult = LIB_SUCCESS;
   }

   return libResult;
}



//
//
//
LIB_RESULT GDC_OAL_SemaphoreDelete(GDC_OAL_SEMAPHORE sem)
{
   LIB_RESULT libResult = LIB_FAILURE;

   int intResult = sem_destroy((sem_t*)sem);

   if(intResult == 0)
   {
      free(sem);
      libResult = LIB_SUCCESS;
   }

   return libResult;
}



//
//
//
LIB_RESULT GDC_OAL_SemaphoreObtain(GDC_OAL_SEMAPHORE sem)
{
   LIB_RESULT libResult = LIB_FAILURE;

   int intResult = sem_wait((sem_t*)sem);

   if(intResult == 0)
   {
      libResult = LIB_SUCCESS;
   }

   return libResult;
}
#else
//
//
// Semaphores
//
//



//
//
//
LIB_RESULT GDC_OAL_SemaphoreCreate(GDC_OAL_SEMAPHORE*,
                                   const char*,
                                   GDC_OAL_SEMAPHORE_TYPE,
                                   unsigned int)
{
   return LIB_SUCCESS;
}



//
//
//
LIB_RESULT GDC_OAL_SemaphoreObtain(GDC_OAL_SEMAPHORE, unsigned int)
{
   return LIB_SUCCESS;
}


//
//
//
LIB_RESULT GDC_OAL_SemaphoreTryObtain(GDC_OAL_SEMAPHORE)
{
   return LIB_SUCCESS;
}


//
//
//
LIB_RESULT GDC_OAL_SemaphoreRelease(GDC_OAL_SEMAPHORE)
{
   return LIB_SUCCESS;
}



//
//
//
LIB_RESULT GDC_OAL_SemaphoreDelete(GDC_OAL_SEMAPHORE)
{
   return LIB_SUCCESS;
}



//
//
//
LIB_RESULT GDC_OAL_SemaphoreObtain(GDC_OAL_SEMAPHORE)
{
   return LIB_SUCCESS;
}
#endif
