/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor;
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
* \file base_app_object.cpp
* \brief base object for analysis object (LDW, TSR ...)
* \author Daniel Lazar
* \version
* \date
****************************************************************************/

#include "common_time_measure.h"
#include "multithread_app_base_app_object.h"

#include <semaphore.h>


// value for killing threads
bool CanWork = true;


/**
  *  Constructor 
  *  \param apuldx - index of apuldx
  *  \param oddResults - pointer to odd result structure
  *  \param evenResults - pointer to odd result structure
  *  \param even_or_odd - init value => true = first is even structure used
  **/
BaseAppObject::BaseAppObject(int apuldx, void* oddResults, void* evenResults, _cycleType even_or_odd)
	: BaseThreadObj()
{
  m_apuldx = apuldx;
  m_oddResults = oddResults;
  m_evenResults = evenResults;
  m_even_or_odd = even_or_odd;
}


BaseAppObject::BaseAppObject(int apuldx, _cycleType even_or_odd)
	: BaseThreadObj()
{
	m_apuldx = apuldx;
	m_oddResults = NULL;
	m_evenResults = NULL;
	m_even_or_odd = even_or_odd;
}

/*
 *  Destructor
 */
BaseAppObject::~BaseAppObject()
{
   
}

///////////////////////////////////////////
/// Class BaseThreadObj
////////////////////////////////////////

BaseThreadObj::BaseThreadObj()
	: m_nrFrames(0)
	, mTimeStart(0)
	, mTimeEnd(0)
	, mTimeDiff(0)
{
	_thisApp.app = this;
	_thisApp.execParams = NULL;

	// Init semaphores, sharing only in process, init value = 0
	sem_init(&m_semaphore_Start, 0, 0);
	sem_init(&m_semaphore_End, 0, 0);
}



/*
* Start the thread
*/
void* BaseThreadObj::executeThread(void*)
{
  while(CanWork)
  {  
 
    // Wait for green light triggered by main
    sem_wait(&m_semaphore_Start);

    GETTIME(&mTimeStart);

    // Start computation on crt frame
    if(computeOnCrtFrame())
    { // something went wrong
      
		//break;
	    // Let main thread know that work is finish
		sem_post(&m_semaphore_End);
		return NULL;
    }  
    
	++m_nrFrames;

    // Analysis time end
    GETTIME(&mTimeEnd);
    mTimeDiff += mTimeEnd-mTimeStart;

    // Let main thread know that work is finish
    sem_post(&m_semaphore_End);

  }
  
  return NULL;
}

int BaseThreadObj::startThread()
{  	
	pthread_create(&_thread, NULL, threadF, &this->_thisApp);
	return 0;
}

char* BaseThreadObj::joinThread() {
	char* ret  = NULL;
	pthread_join(_thread, (void**) &ret);
	return ret;
}

/**
* Helper functions, for pthread_create which accepts only C-functions
*/
void* threadF(void* p)
{
	_tExecAppThread *appThr = (_tExecAppThread *)p;
	return appThr->app->executeThread(appThr->execParams);
}

void BaseThreadObj::getMeanExecutionTime(float& meanExecTime, float& fps) {
	if (m_nrFrames > 0) {
		meanExecTime = mTimeDiff / 1000000.0f / m_nrFrames;
		fps = 1.0f / meanExecTime;
	}
	else {
		meanExecTime = 0;
		fps = 0;
	}
}
