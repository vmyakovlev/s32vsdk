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
* \file OAL_Task.cpp
* \brief OAL task implementation
* \author Rostislav Hulik
* \version
* \date 3-March-2016
****************************************************************************/

#include <oal.h>
#include <pthread.h>
#include <cerrno>
#include <stdlib.h>

extern int32_t oal_fd;

/////////////////////////////////////////////////////////////////////////
/// Forward declarations
/////////////////////////////////////////////////////////////////////////
const uint32_t DEFAULT_TASK_PRIORITY = 127;
const uint32_t MAX_TASK_PRIORITY = 255;

/////////////////////////////////////////////////////////////////////////
/// Function transporms pointer to opaque handle pointer to pthread pointer
/////////////////////////////////////////////////////////////////////////
static inline pthread_t* ThreadPGetFromTCBP(OAL_TCB* apTcb)
{
  return (pthread_t*)apTcb;
}

/////////////////////////////////////////////////////////////////////////
/// Function transporms pointer to opaque handle to pthread pointer
/////////////////////////////////////////////////////////////////////////
static inline pthread_t ThreadGetFromTCB(OAL_TCB aTcb)
{
  return (pthread_t)aTcb;
}

/////////////////////////////////////////////////////////////////////////
/// Function transporms priority number to possible priority range
/////////////////////////////////////////////////////////////////////////
static int MapPriority(uint32_t aOALpriority)
{
  int pmax = sched_get_priority_max( SCHED_FIFO );
  int pmin = sched_get_priority_min( SCHED_FIFO );
  int range = abs((pmax - pmin)  +1);

  // Calculate the number of bins to map the OAL priorities into (large range
  // to small range).
  int nBins = int(MAX_TASK_PRIORITY)/range;

  // Add one bin if more than half a range is left.
  if ((int)MAX_TASK_PRIORITY % range > range/2)
  {
    ++nBins;
  }

  // revert the priority (descending vs ascending)
  int pReverted = int(MAX_TASK_PRIORITY) - aOALpriority;
  int offset = pReverted / nBins ;

  return pmin + offset;
}

///////////////////////////////////////////////////////////////////////////////////////
/// Create an OS Task.  If succeeded, the created Task ID
/// will be subsequently referred to in later OS Task calls.
///////////////////////////////////////////////////////////////////////////////////////
int32_t OAL_TaskInit(OAL_TASK_ENTRY task_entry, void* argument,
                        unsigned int stack_size, unsigned int priority, char* task_name,
                        OAL_TCB *task)
{
  CHECK_INIT
  int32_t ret = OAL_SUCCESS;

  pthread_attr_t attr;
  
  // init pthread attributes
  if (pthread_attr_init(&attr))
  {
    ret = OAL_FAILURE;
  }
  else 
  {
    // set stack size to attributes
    if (pthread_attr_setstacksize(&attr, (size_t)stack_size))
    {
      ret = OAL_FAILURE;
    }
    else
    {
      // set priority to attributes
      sched_param sp;
      priority = MapPriority(priority);
      sp.__sched_priority = priority;
      if (pthread_attr_setschedpolicy(&attr, SCHED_RR))
      {
	ret = OAL_FAILURE;
      }
      else 
      {
	if (pthread_attr_setschedparam(&attr, &sp))
	{
	  ret = OAL_FAILURE;
	}
	else 
	{
	  // create a thread and set the name
	  if (pthread_create(ThreadPGetFromTCBP(task), &attr, task_entry, argument))
	  {
	    ret = OAL_FAILURE;
	  }
	  else
	  {
	    pthread_setname_np(*ThreadPGetFromTCBP(task), task_name);
	    (void)pthread_attr_destroy(&attr);
	  }
	}
      }
    }
  }
  return ret;
}

////////////////////////////////////////////////////////////////////////
/// Waits for the task to terminate.
////////////////////////////////////////////////////////////////////////
int32_t OAL_TaskJoin(OAL_TCB task)
{
  CHECK_INIT
  if (pthread_join(ThreadGetFromTCB(task), NULL))
    return OAL_FAILURE;
  else
    return OAL_SUCCESS;
}

////////////////////////////////////////////////////////////////////////
/// Delete the specified task assuming the specified task ID has
/// already been created.
////////////////////////////////////////////////////////////////////////
int32_t OAL_TaskDelete(OAL_TCB task)
{
  CHECK_INIT
  int32_t ret = OAL_SUCCESS;

  pthread_t t = ThreadGetFromTCB(task);
  if (pthread_cancel(t))
  {
    ret = OAL_SUCCESS;
  }
  else
  {
    if (pthread_join(t, NULL))
    {
      ret = OAL_FAILURE;
    }
  }

  return ret;
}