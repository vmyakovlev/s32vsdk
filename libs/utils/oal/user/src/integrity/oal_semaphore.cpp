/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2014 Freescale Semiconductor;
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
#include <new>
#include <oal.h>
#include <INTEGRITY.h>

/*==============================================================================*/
/* OAL Semaphore type								*/
/*==============================================================================*/
struct tOalSemaphore
{
   void * sm;
   OAL_SEMAPHORE_TYPE type;
};

typedef struct
{
  struct tOalSemaphore semaphore;          //This must be first always
  Semaphore            integritySemaphore; //Integrity semaphore
  LocalMutex           integrityMutex;     //Integrity mutex
} OAL_SEMAPHORE_LOCAL;

/*==============================================================================*/
/* Create the semaphore								*/
/*==============================================================================*/
int32_t OAL_SemaphoreCreate(OAL_SEMAPHORE *pSem,
    const char *pName, OAL_SEMAPHORE_TYPE type, unsigned int count)
{
  //INTEGRITY doesn't seem to have 'named' semaphores - ignore the 'pName' parameter

  int32_t lRetVal = OAL_FAILURE;
  Error lError; //integrity error
  OAL_SEMAPHORE_LOCAL *pSemLocal = 0;

  *pSem = NULL;

  if (pSem != NULL)
  {
    pSemLocal = (OAL_SEMAPHORE_LOCAL*)malloc(sizeof(OAL_SEMAPHORE_LOCAL));

    if (pSemLocal != 0)
    {
      pSemLocal->semaphore.type = type;

      switch (type)
      {
	case OAL_SEMAPHORE_BINARY:
	  lError = CreateBinarySemaphore(&pSemLocal->integritySemaphore);
	  if (Success == lError)
	  {
	    *pSem = (OAL_SEMAPHORE) pSemLocal;
	    lRetVal = OAL_SUCCESS;
	  }
	  else
	  {
	    printf("INTEGRITY::CreateBinarySemaphore(...) failed (%d):(\n", lError);
	  }

	  break;

	case OAL_SEMAPHORE_COUNTING:
	  lError = CreateSemaphore(count, &pSemLocal->integritySemaphore);
	  if (Success == lError)
	  {
	    *pSem = (OAL_SEMAPHORE) pSemLocal;
	    lRetVal = OAL_SUCCESS;
	  }
	  else
	  {
	    printf("INTEGRITY::CreateSemaphore(...) failed (%d):(\n", lError);
	  }

	  break;

	case OAL_SEMAPHORE_MUTEX:
	  lError = CreateLocalMutex(&pSemLocal->integrityMutex);
	  if (Success == lError)
	  {
	    *pSem = (OAL_SEMAPHORE) pSemLocal;
	    lRetVal = OAL_SUCCESS;
	  }
	  else
	  {
	    printf("INTEGRITY::CreateLocalMutex(...) failed (%d):(\n", lError);
	  }

	  break;

	default:
	    *pSem = NULL;
	    lRetVal = OAL_SEM_EBADTYPE;
      }
    }
    else //allocation of semaphore object failed
    {
	lRetVal = OAL_SEM_ENOMEM;
    }
  }
  else
  {
    lRetVal = OAL_FAILURE;
  }

  //perform cleanup if any errors occurred
  if (lRetVal != OAL_SUCCESS)
  {
    if (pSemLocal)
    {
	free(pSemLocal);
    }
  }

  return lRetVal;
}

/*==============================================================================*/
/* Obtain the semaphore								*/
/*==============================================================================*/
int32_t OAL_SemaphoreObtain(OAL_SEMAPHORE sem, unsigned int time)
{
  int32_t           lRetVal = OAL_FAILURE;
  OAL_SEMAPHORE_LOCAL *lpSemLocal = (OAL_SEMAPHORE_LOCAL*)sem;
  Error                lError; //integrity error
  Time                 lTime; //integrity time

  /*if (time == OAL_WAITFOREVER)
  {
    return OAL_SemaphoreObtain(sem);
  }*/

  //'time' is in milliseconds - translate into integrity time structure
  lTime.Seconds = time/1000000;
  lTime.Fraction = (UINT4)(0xffffffff*((double)(time%1000000)/1000000));

  switch (lpSemLocal->semaphore.type)
  {
    case OAL_SEMAPHORE_BINARY:
    case OAL_SEMAPHORE_COUNTING:
      lError = TimedWaitForSemaphore(lpSemLocal->integritySemaphore, &lTime);
      if (Success == lError)
      {
	lRetVal = OAL_SUCCESS;
      }
      else
      {
	if (OperationTimedOut == lError)
	{
	    lRetVal = OAL_SEM_ETIMEOUT;
	    printf("INTEGRITY::TimedWaitForSemaphore(...) timed out (%d):(\n", lError);
	}
	else
	{
	    printf("INTEGRITY::TimedWaitForSemaphore(...) failed (%d):(\n", lError);
	}
      }

      break;

    case OAL_SEMAPHORE_MUTEX:
      lError = TimedWaitForLocalMutex(lpSemLocal->integrityMutex, &lTime);
      if (Success == lError)
      {
	lRetVal = OAL_SUCCESS;
      }
      else
      {
	if (OperationTimedOut == lError)
	{
	    lRetVal = OAL_SEM_ETIMEOUT;
	    printf("INTEGRITY::TimedWaitForLocalMutex(...) timed out (%d):(\n", lError);
	}
	else
	{
	    printf("INTEGRITY::TimedWaitForLocalMutex(...) failed (%d):(\n", lError);
	}
      }

      break;

    default:
	lRetVal = OAL_SEM_EBADTYPE;
  }

  return lRetVal;
}

/*==============================================================================*/
/* Release the semaphore							*/
/*==============================================================================*/
int32_t OAL_SemaphoreRelease(OAL_SEMAPHORE sem)
{
  int32_t           lRetVal = OAL_FAILURE;
  Error                lError; //integrity error
  OAL_SEMAPHORE_LOCAL *lpSemLocal = (OAL_SEMAPHORE_LOCAL*)sem;

  switch (lpSemLocal->semaphore.type)
  {
    case OAL_SEMAPHORE_BINARY:
    case OAL_SEMAPHORE_COUNTING:
      lError = ReleaseSemaphore(lpSemLocal->integritySemaphore);
      if (Success == lError)
      {
	lRetVal = OAL_SUCCESS;
      }
      else
      {
	printf("INTEGRITY::ReleaseSemaphore(...) failed (%d):(\n", lError);
      }

      break;

    case OAL_SEMAPHORE_MUTEX:
      lError = ReleaseLocalMutex(lpSemLocal->integrityMutex);
      if (Success == lError)
      {
	lRetVal = OAL_SUCCESS;
      }
      else
      {
	printf("INTEGRITY::ReleaseLocalMutex(...) failed (%d):(\n", lError);
      }

      break;

    default:
      lRetVal = OAL_SEM_EBADTYPE;
  }

  return lRetVal;
}

/*==============================================================================*/
/* Delete the semaphore								*/
/*==============================================================================*/
int32_t OAL_SemaphoreDelete(OAL_SEMAPHORE sem)
{
  OAL_SEMAPHORE_LOCAL *lpSemLocal = (OAL_SEMAPHORE_LOCAL*)sem;
  int32_t           lRetVal = OAL_FAILURE;
  Error                lError; //integrity error

  if (lpSemLocal)
  {
    switch (lpSemLocal->semaphore.type)
    {
      case OAL_SEMAPHORE_BINARY:
      case OAL_SEMAPHORE_COUNTING:
	lError = CloseSemaphore(lpSemLocal->integritySemaphore);
	if (Success == lError)
	{
	    free(lpSemLocal);
	    lRetVal = OAL_SUCCESS;
	}
	else
	{
	    printf("INTEGRITY::CloseSemaphore(...) failed (%d):(\n", lError);
	}

	break;

      case OAL_SEMAPHORE_MUTEX:
	lError = CloseLocalMutex(lpSemLocal->integrityMutex);
	if (Success == lError)
	{
	    free(lpSemLocal);
	    lRetVal = OAL_SUCCESS;
	}
	else
	{
	    printf("INTEGRITY::Close(...) failed (%d):(\n", lError);
	}

	break;

      default:
	lRetVal = OAL_SEM_EBADTYPE;
    }
  }
  else
  {
    printf("OAL_SemaphoreDelete(...) failed\n");
    lRetVal =  OAL_SEM_EBADSEM;
  }

  return lRetVal;
}

