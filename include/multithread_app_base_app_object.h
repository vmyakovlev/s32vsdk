/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor
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
* \file base_app_object.h
* \brief base object for analysis object (LDW, TSR ...)
* \author Daniel Lazar
* \version
* \date
****************************************************************************/

#ifndef BASEAPPOBJECT_H
#define BASEAPPOBJECT_H

typedef enum { even = 0, odd = 1 } _cycleType;
extern bool CanWork;


#include <pthread.h>
#include <semaphore.h>

typedef void* (*_tfptr)(void*);


class BaseThreadObj;

typedef struct s_execAppThr{
	BaseThreadObj* app;
    void* execParams;

	s_execAppThr() {
		app = NULL;
		execParams = NULL;
	}
}_tExecAppThread;


class BaseThreadObj {
protected:
	_tExecAppThread _thisApp;
	pthread_t _thread;	

	// Semaphore for start threads
	sem_t m_semaphore_Start;
	sem_t m_semaphore_End;

	// Variable for measuring time
	unsigned long mTimeStart, mTimeEnd, mTimeDiff;

public:	
	/**
	* Number of frames on which computeOnCrtFrames was called
	*/
	unsigned long m_nrFrames;
	
	BaseThreadObj();

	virtual ~BaseThreadObj() {}

	/**
	* Tell pThread to start
	*/
	virtual int startThread();

	// Semaphore for start threads
	sem_t* get_semaphore_Start() { return &m_semaphore_Start; };
	sem_t* get_semaphore_End() { return &m_semaphore_End; };


	/*
	* start the generic for loop
	*/
	virtual void* executeThread(void*); 

	/**
	*  Compute on one frame. This is the actual computation function
	**/
	virtual int computeOnCrtFrame() = 0;

	virtual char* joinThread();
	
	// Get total execution time
	unsigned long 	getComputationTime() { return mTimeDiff;}

	//Get mean execution time for the currently executed cycles
	void getMeanExecutionTime(float& meanExecTime, float& fps);

};


class BaseAppObject : public BaseThreadObj {
public:

protected:
  /*
   *  index apu that is used
   */
  int m_apuldx;
 
  /*
   *  Pointer to structure of odd results
   */
  void *m_oddResults;
  
  /*
   *  Pointer to structure of odd results
   */
  void *m_evenResults;
  
  /*
   *  Switch for odd or even iteration results
   */
  _cycleType m_even_or_odd;
  
public:
  
  /**
   *  Constructor 
   *  \param apuldx - index of apuldx
   *  \param oddResults - pointer to odd result structure
   *  \param evenResults - pointer to odd result structure
   *  \param even_or_odd - init value => true = first is even structure used
   **/
  BaseAppObject(int apuldx, void* oddResults, void* evenResults, _cycleType even_or_odd);
  

  /**
   *  Constructor 
   *  \param apuldx - index of apuldx
   *  \param even_or_odd - init value => _cycleType::even = first is even structure used
   **/
  BaseAppObject(int apuldx, _cycleType even_or_odd);

   /*
   *  Destructor
   */
  ~BaseAppObject();
  
  
  /**
   *  Set pointer to input data
   *  \param data - pointer to data
   **/
  virtual int setInputData(void* data) = 0;
   
  /**
   *  Init function
   **/
  virtual int init() = 0;
  
  /**
   *  Compute on one frame 
   **/
  virtual int computeOnCrtFrame() = 0;
  


};

void* threadF(void* p);

#endif /* BASEAPPOBJECT_H */
