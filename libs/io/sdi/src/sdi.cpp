/******************************************************************************
 *
 * Freescale Confidential Proprietary
 *
 * Copyright (c) 2016 Freescale Semiconductor;
 * Copyright (c) 2016-17 NXP;
 * All Rights Reserved
 *
 *****************************************************************************
 *
 * THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITEDl TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/**
 * \file 		sdi.cpp
 * \brief 		Sensor device interface functionality implementation.
 * \author 		Tomas Babinec
 * \version 		0.1
 * \date 		16-September-2013
 ****************************************************************************/

#ifdef SDI_ECLIPSE_DEF
#define GBO_FILE "GBOpts_APEX2_Pseudo_FPGA_Valid.h"
#endif
#include "s32vs234.h"
#include <unistd.h>
#ifndef __STANDALONE__
  #include <sys/mman.h>
 // #include <pthread.h>
  #include <sys/time.h>
#endif // #ifndef __STANDALONE__

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "sdi.hpp"
#include "vdb_log.h"
#include "oal.h"
#include "oal_extras.h"
#include "oal_process.h"

#include "isp_fdma.h"
#include "seq.h"
#include "isp_seq.h"

//****************************************************************************
// consts
//****************************************************************************

//****************************************************************************
// macros
//****************************************************************************

#define ALIGN2(_x) (((_x)/2 + (((_x)%2)?1:0)) * 2)

#define UNUSED(_x) (void)(_x)  // to suppress unused parameter warning

//****************************************************************************
// global variables
//****************************************************************************

extern char sequencer_srec[];
extern char kmem_srec[];

//****************************************************************************
// local variables
//****************************************************************************
#ifdef SDI_THRED_SAFE // process/thread safety under development
  static const char *scpSemInitName = "sdiSini";
  static const char *scpMutSdiName  = "sdiMut";
  static const char *scpShmName     = "sdiMem";
#endif // #ifdef SDI_THRED_SAFE
  
static const uint32_t scSemWaitMax = 5000000;   ///< maximum time to wait for semaphore

#ifdef __STANDALONE__
extern "C" {
  unsigned long get_uptime_microS(void);
}
#endif // #ifdef __STANDALONE__

//****************************************************************************
// local functions declarations
//****************************************************************************

void IspHwSramAccesAllow(void);

//****************************************************************************
// static members definition
//****************************************************************************

bool       sdi::mInitialized       = false;
uint32_t*  sdi::mpSharedMemorySize = NULL;
uint32_t*  sdi::mpInitCounter      = NULL;
uint32_t   sdi::mThreadCounter     = 0;
uint32_t*  sdi::mpSensorCnt        = NULL;
sdi_input* sdi::mpSensors          = NULL;
//vector<sdi_process*> sdi::mProcesses(SDI_SENSOR_CNT); // initial number of processes == number of sensors
uint32_t      sdi::mNextProcessId  = 1;  
OAL_SEMAPHORE sdi::mSdiMut         = NULL;
//sdi_mutex sdi::mThreadMutex;
OAL_SEMAPHORE sdi::mInitSemaphore = NULL;
OAL_SHARED_MEM sdi::mShm = NULL;

void* sdi::mPramBase = NULL; //

int32_t gEvent = 0;

//****************************************************************************
// methods implementation
//****************************************************************************

// *** sdi_input implementation ***

//****************************************************************************

sdi_input::sdi_input() :
    mSensorSrc(SDI_SENSOR_INVALID), mReserved(false)
{

} // sdi_input:sdi_input()

//****************************************************************************

LIB_RESULT sdi_input::ParameterSet(SDI_IO_PARAM aParamId, void* apValue)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  if( apValue == NULL)
  {
    VDB_LOG_ERROR("NULL pointer to value given.\n");
    lRet = LIB_FAILURE;
  } // if value is NULL
  else
  {
    ////
    //
    // todo: instruct sequencer to try to set this parameter value &check result
    //
    ////
    
    // TODO: remove unused warning suppression macro
    UNUSED(aParamId);
  } // else from if apValue == NULL

  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return lRet;
} // sdi_input::ParameterSet

//****************************************************************************

LIB_RESULT sdi_input::ParameterGet(SDI_IO_PARAM aParamId, void* apValue)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  if( apValue == NULL)
  {
    VDB_LOG_ERROR("NULL pointer to value given.\n");
    lRet = LIB_FAILURE;
  } // if value is NULL
  else
  {
    ////
    //
    // todo: instruct sequencer to try to fetch this parameter value & check result
    //
    ////
    
    // TODO: remove unused warning suppression macro
    UNUSED(aParamId);
  } // else from if apValue == NULL

  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return lRet;
} // sdi_input::ParameterGet

//****************************************************************************

sdi_input::~sdi_input()
{
  /*if (OAL_SemaphoreDelete(mMutex) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Deleting sdi_input mutex failed.\n");
  }
  mMutex = NULL;*/
  mReserved = false;
} // sdi_input::~sdi_input()

//****************************************************************************

// *** sdi_process implementation ***

//****************************************************************************

sdi_process::sdi_process() :
    mpGraph(NULL)
{
  // set defaults
  
  VDB_LOG_FCN_NOT_IMPLEMENTED();
} // sdi_process::sdi_process()

//****************************************************************************

LIB_RESULT sdi_process::Set(SEQ_Head_Ptr_t *apGraph, 
                            GraphMetadata_t *apGraphMetadata)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if(mpGraph != NULL)
  {
    VDB_LOG_WARNING("Graph set allready. Reset required.\n");
    lRet = LIB_FAILURE;
  } // if graph set allready
  else
  {
    mpGraph = new sdi_graph(apGraph, apGraphMetadata) ;
    if((mpGraph == NULL))
    {
      VDB_LOG_ERROR("Failed to preprocess the graph.\n");
      lRet = LIB_FAILURE;
    } // if sdi_graph creation failed
    else
    {
      // all ok (nothing more to be done)
      VDB_LOG_NOTE("Process set successfully.\n");
    } // else from if sdi_graph creation failed
  } // else from if graph set allready
  
  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return lRet;
} // sdi_process::Set()

//****************************************************************************

LIB_RESULT sdi_process::Reset()
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
  // destroy the graph
  if(mpGraph != NULL)
  {
    delete(mpGraph);
    mpGraph = NULL;
  } // if graph exists
  
  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return lRet;
} // sdi_process::Reset()

//****************************************************************************

LIB_RESULT sdi_process::Finalize()
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
  if(mpGraph != NULL)
  {
    if(mpGraph->Finalize() != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to finalize the graph.\n");
      lRet = LIB_FAILURE;
    } // if graph finalize failed
  } // if graph exists
  else
  {
    VDB_LOG_WARNING("There is no graph.\n");
  } // else if grah exists
  
  return lRet;
} // sdi_process::Finalize(void)

//****************************************************************************

LIB_RESULT sdi_process::Free(void)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
  if(mpGraph != NULL)
  {
    mpGraph->Free();
  } // if graph exists
  else
  {
    VDB_LOG_WARNING("There is no graph.\n");
  } // else if grah exists
  
  return lRet;
} // Free(void)

//****************************************************************************

LIB_RESULT sdi_process::Download()
{
  LIB_RESULT lRet = LIB_SUCCESS;

  // *** M0 FW ***
  if(SEQ_FwArrDownload(sequencer_srec, SEQ_FW_CM0) == LIB_SUCCESS)
  {
  
    // *** IPU kernels ***
    if(SEQ_FwArrDownload(kmem_srec, SEQ_FW_KERNEL) == LIB_SUCCESS)
    {
      //*** Graph ***
      if(mpGraph != NULL)
      {
        if(mpGraph->Download(0) != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Failed to download the graph.\n");
          lRet = LIB_FAILURE;
        } // if graph finalize failed
      } // if graph exists
      else
      {
        VDB_LOG_WARNING("There is no graph.\n");
      } // else if grah exists
    }// if IPU kernels ok
    else
    {
      lRet = LIB_FAILURE;
    } // else from if IPU kernels ok
  } // if FW download ok
  else
  {
    lRet = LIB_FAILURE;
  } // else from if FW download ok

  return lRet;
} // sdi_process::Download(void)


//****************************************************************************

SEQ_Buf_t** sdi_process::GraphFetch(SEQ_Head_Ptr_t** appGraph)
{
  SEQ_Buf_t **lppBufferList = NULL;
  mpGraph->Fetch(appGraph, &lppBufferList);
  
  return lppBufferList;
} // sdi_process::GraphFetch(SEQ_Head_Ptr_t** appGraph)

//****************************************************************************

LIB_RESULT sdi_process::IOsGet(sdi_io *apIOs[])
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
  // check input
  if(apIOs != NULL)
  {
    if(mpGraph != NULL)
    { 
      for(uint32_t i = 0; i < SEQ_OTHRIX_LAST; i++)
      {
        // get the IO indexes
        if( mpGraph->IoQuery((SEQ_Othr_ix_t) i) == LIB_SUCCESS)
        {
          // construct IO objects
          apIOs[i] = IOcreate((SEQ_Othr_ix_t) i);
          if ( apIOs[i] == NULL)
          {
            VDB_LOG_ERROR("Failed to generate %uth IO object.\n", i);
            apIOs[i] = NULL;
            lRet = LIB_FAILURE;
          } // if IOcreate failed
        } // if no such object
      } // for all possible IO objects
    } // if graph exists
    else
    {
      VDB_LOG_WARNING("There is no graph\n");
    } // else from if graph exists
    
  } // if graph exists
  else
  {
    VDB_LOG_ERROR("Parameter is NULL.\n");
    lRet = LIB_FAILURE;
  } // else from if graph exists
  
  return lRet;
} // sdi_process::IOsGet(sdi_input *apIOs[])

//****************************************************************************
sdi_io* sdi_process::IOcreate(SEQ_Othr_ix_t aTypeIdx)
{
  sdi_io *lpRetIO = NULL;
  
  switch(aTypeIdx)
  {
    case SEQ_OTHRIX_FDMA:
    {
      lpRetIO = (sdi_io*) new sdi_FdmaIO(mpGraph);      
    } // case SEQ_OTHRIX_FDMA
      break;
    
    case SEQ_OTHRIX_H264ENC:
    {
      lpRetIO = (sdi_io*) new sdi_H264EncIO(mpGraph);
    } // case SEQ_OTHRIX_H264ENC
      break;
    
    case SEQ_OTHRIX_H264DEC:
    {
      lpRetIO = (sdi_io*) new sdi_H264DecIO(mpGraph);
    } // case SEQ_OTHRIX_H264DEC
      break;
    
    case SEQ_OTHRIX_JPEGDEC:
    {
      lpRetIO = (sdi_io*) new sdi_JpegDecIO(mpGraph);
    } // case SEQ_OTHRIX_JPEGDEC
      break;
    
    case SEQ_OTHRIX_MIPICSI0:
    {
      lpRetIO = (sdi_io*) new sdi_MipiCsiIO(mpGraph, CSI_IDX_0);
    } // case SEQ_OTHRIX_MIPICSI0/1
      break;
    case SEQ_OTHRIX_MIPICSI1:
    {
      lpRetIO = (sdi_io*) new sdi_MipiCsiIO(mpGraph, CSI_IDX_1);
    } // case SEQ_OTHRIX_MIPICSI0/1
      break;

    case SEQ_OTHRIX_VIU0:
    {
      lpRetIO = (sdi_io*) new sdi_ViuIO(mpGraph, VIU_IDX_0);
    } // case SEQ_OTHRIX_VIU0
      break;
    case SEQ_OTHRIX_VIU1:
    {
      lpRetIO = (sdi_io*) new sdi_ViuIO(mpGraph, VIU_IDX_1);
    } // case SEQ_OTHRIX_VIU1
      break;
      
    default:
    {
      VDB_LOG_ERROR("Unknown IO type index.\n");
    } // default
      break;
  } // switch IO type index
  
  if( lpRetIO == NULL)
  {
    VDB_LOG_ERROR("Failed to create IO object with type index %u.\n", aTypeIdx);
  }  // if no IO object created
  
  return lpRetIO;
} // sdi_process::IOcreate(SEQ_Othr_ix_t aIdx)

//****************************************************************************

LIB_RESULT sdi_process::SramBufferGet(const char *acpName, 
                                      SEQ_Buf_t &arSramBuf) const
{  
  LIB_RESULT lRet = LIB_SUCCESS;
  if(mpGraph != NULL)
  {
    lRet = mpGraph->SramBufGet(acpName, arSramBuf);
  } // if graph set
  else
  {
    lRet = LIB_FAILURE;
  } // if no graph set
  
  return lRet;
} // sdi_process::SramBufferGet(SEQ_Othr_ix_t aIoId)

//****************************************************************************

sdi_process::~sdi_process()
{
  // reset processing setup
  if(Reset() != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to reset the sdi_process instance.\n");
  } // if Reset failed
  
  VDB_LOG_FCN_NOT_IMPLEMENTED();
} // sdi_process::~sdi_process()

//****************************************************************************

// *** sdi_grabberContext ***

//****************************************************************************

sdi_grabberContext::sdi_grabberContext() :
  mInput(NULL),
  mProcess(NULL),
  mBufferArr(NULL),
  mBufCnt(0),
  mNextBuffIdx(0),
  mFramesToBeCaptured(1),
  mFramesCaptured(0),
  mCallBackParam(NULL),
  mCallBackFcn(NULL)
{
}  // sdi_grabberContext::sdi_grabberContext()

//****************************************************************************

// *** sdi_grabTask ***

//****************************************************************************

sdi_grabTask::sdi_grabTask(sdi_grabber *apGrabber) :
  mpGrabber(apGrabber), mStatus(OFF), mResult(LIB_SUCCESS)
{
} // sdi_grabTask::sdi_grabTask()

//****************************************************************************

sdi_grabTask::~sdi_grabTask()
{
  mpGrabber = NULL;
  mStatus   = OFF;
  mGC       = sdi_grabberContext();
  mResult   = false;
} // sdi_grabTask::~sdi_grabTask()

//****************************************************************************

LIB_RESULT sdi_grabTask::TaskPrepare()
{
#ifndef __STANDALONE__
  mResult = LIB_SUCCESS;
  if (mpGrabber != NULL)
  {
    // fetch grabber setup
    mGC = mpGrabber->mGC;
    if (mGC.mBufCnt != 0)
    {
      // make deep copy of buffer array
      mGC.mBufferArr = new (std::nothrow) uint8_t*[mGC.mBufCnt];
      if (mGC.mBufferArr != NULL)
      {
        for (uint32_t i = 0; i < mGC.mBufCnt; ++i)
        {
          mGC.mBufferArr[i] = mpGrabber->mGC.mBufferArr[i];
        } // for all buffers

        // check all is ready
        if ((mGC.mCallBackFcn != NULL) && (mGC.mInput) && (mGC.mProcess))
        {
          mStatus = READY;
          mpGrabber->mGC.mFramesCaptured = 0;

          // leave index to buffer array as it is !!!

          // set grabbing state to on
          //mpGrabber->mGrabbingOn = true;
        } else
        {
          VDB_LOG_ERROR("Grabber not fully initialized.\n");
          mResult = LIB_FAILURE;
        } // if all set prepared
      } else
      {
        VDB_LOG_ERROR("Buffer array could not be allocated. Grabbing will not start.\n");
        mResult = LIB_FAILURE;
      } // else from if buffer array allocated
    } else
    {
      VDB_LOG_ERROR("Buffer array empty. Grabbing will not start.\n");
      mResult = LIB_FAILURE;
    } // else from if buffer not empty
  } // if mGrabber exists
#endif // #ifndef __STANDALONE__  
  return mResult;
} // sdi_grabTask::TaskPrepare()

//****************************************************************************

/*void sdi_grabTask::TaskOpen()
 {
 VDB_LOG_NOTE("Not needed now.\n");
 } // sdi_grabTask::TaskOpen()*/

LIB_RESULT sdi_grabTask::AllDone(bool &arDone)
{
  LIB_RESULT lRet = LIB_SUCCESS;
#ifndef __STANDALONE__  
  // check grabbed cnt
  if (OAL_SemaphoreObtain(mpGrabber->mThreadMutex, scSemWaitMax) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Grabber mutex obtain failed.\n");
    mResult = LIB_FAILURE;
  } // if mutex obtain failed
  else
  {
    if ((mGC.mFramesToBeCaptured > 0)
      && (mGC.mFramesCaptured >= mGC.mFramesToBeCaptured))
    {
      arDone = true;
    } // if all frames captured
    else
    {
      arDone = false;
    }

    if (OAL_SemaphoreRelease(mpGrabber->mThreadMutex) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Grabber mutex release failed.\n");
      mResult = LIB_FAILURE;
    } // if semaphore release failed
  }// else from if mutex obtain failed
#endif // #ifndef __STANDALONE__
  return lRet;
}// bool sdi_grabTask::AllDone(LIB_RESULT &lRet)

//****************************************************************************

void sdi_grabTask::TaskService()
{
#ifndef __STANDALONE__  
  // test phase todo: remove
  sleep(1);
  VDB_LOG_NOTE("This is the grabber task service.\n");
  sleep(1);

  // grab loop control variable
  bool run  = false;
  bool done = false;

  if (OAL_SemaphoreObtain(mpGrabber->mThreadMutex, scSemWaitMax) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Grabber mutex obtain failed.\n");
    mResult = LIB_FAILURE;
  } // if mutex obatin failed
  else
  {
    if(mStatus == READY)
    {
      mStatus = ON;
      run = true;
    } // if task is ready
    else
    {
      VDB_LOG_ERROR("Grabber task in not ready to execute.");
      mResult = LIB_FAILURE;
    } // else from if taks is ready
    if (OAL_SemaphoreRelease(mpGrabber->mThreadMutex) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Grabber mutex release failed.\n");
      mResult = LIB_FAILURE;
    } // if semaphore release failed
  }// else from if mutex obtain failed

  while (run)
  {
    // image grabbing sequence
    if(AllDone(done) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Check for AllDone failed.");
      mResult = LIB_FAILURE;
    } // if AllDone() failed
    else
    {
      if(done)
      {
        run = false;
        break;
      } // if all frames have been captured

      ////////
      //
      // todo: do the image grabbing here
      //
      ////////

      // execute call-back function
      mGC.mCallBackFcn(mGC.mCallBackParam);

      // update grabber context
      ++mGC.mFramesCaptured;
      mGC.mNextBuffIdx = (mGC.mNextBuffIdx + 1) % mGC.mBufCnt;	    
    } // else from if AllDone() failed
  } // while(run)
#endif // #ifndef __STANDALONE__  
} // sdi_grabTask::TaskService()

//****************************************************************************

void sdi_grabTask::TaskClose()
{
#ifndef __STANDALONE__  
  if (OAL_SemaphoreObtain(mpGrabber->mThreadMutex, scSemWaitMax) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Grabber mutex obtain failed.\n");
  } // if mutex obtain failed
  else
  {
    // set grabbing state to off
    mStatus = OFF;
    // update frame counter and buffer index
    mpGrabber->mGC.mFramesCaptured += mGC.mFramesCaptured;
    mpGrabber->mGC.mNextBuffIdx += mGC.mNextBuffIdx;

    if (OAL_SemaphoreRelease(mpGrabber->mThreadMutex) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Grabber mutex release failed.\n");
    } // if semaphore release failed
  } // else from semaphore obtain failed

  // clean up
  if (mGC.mBufferArr != NULL)
  {
    delete[] (mGC.mBufferArr);
  } // mBufferArr not NULL
#endif // #ifndef __STANDALONE__  
} // sdi_grabTask::TaskClose()

//****************************************************************************

// *** sdi_grabber ***

//****************************************************************************

sdi_grabber::sdi_grabber() :
   modified(false),mGrabbingResult(LIB_SUCCESS),  mGC(), mGrabTask(this)
{
  mpSeqEventCb = NULL;
  mName = (int8_t*) ("grabtask");
  mStatus = OFF;
  
  for(uint32_t i = 0; i < SEQ_OTHRIX_LAST; i++)
  {
    mpIOs[i] = NULL;
  } // for all possible IO objects
  
  if (OAL_SemaphoreCreate(&mThreadMutex, "GrabberMutex", OAL_SEMAPHORE_MUTEX, 1)
      != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Grabber mutex creation failed.\n");
    mThreadMutex = NULL;
  } // if SemaphoreCreate failed
} // sdi_grabber::sdi_grabber()

//****************************************************************************

sdi_grabber::~sdi_grabber()
{ 
  // *** clean up IO objects ***
  for(uint32_t i = 0; i < SEQ_OTHRIX_LAST; i++)
  {
    if(mpIOs[i] != NULL)
    {
      delete(mpIOs[i]);
    }  // if exists 
  } // for all possible IO objects
} // sdi_grabber::~sdi_grabber()

//****************************************************************************
#if 0
LIB_RESULT sdi_grabber::ParamSet(SEQ_Othr_ix_t aIoIdx, SDI_IO_PARAM aParamId, void *apValue)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
  if(mpIOs[aIoIdx] == NULL)
  {
    lRet = LIB_FAILURE;
    
    VDB_LOG_ERROR("No such IO.\n");
  } // if no such IO
  else
  {
    if(mpIOs[aIoIdx]->ParameterSet(aParamId, apValue) != LIB_SUCCESS)
    {
      lRet = LIB_FAILURE;
      
      VDB_LOG_ERROR("Failed to set the parameter.\n");
    } // if ParameterSet() failed
  } // else from if no such IO
  return lRet;
} // sdi_grabber::ParamSet();

//****************************************************************************

LIB_RESULT sdi_grabber::ParamGet(SEQ_Othr_ix_t aIoIdx, SDI_IO_PARAM aParamId, void *apValue)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
  //TODO: remove unused parameter suppression
  UNUSED(aIoIdx);
  UNUSED(aParamId);
  UNUSED(apValue);
  
  return lRet;
} // sdi_grabber::ParamGet();
#endif // if 0

//****************************************************************************

LIB_RESULT sdi_grabber::ProcessSet(SEQ_Head_Ptr_t *apGraph, 
                                   GraphMetadata_t *apGraphMetadata)
{//u
  LIB_RESULT lRet = LIB_SUCCESS;
  if ((apGraph != NULL) || (apGraphMetadata != NULL))
  {
    if (OAL_SemaphoreObtain(mThreadMutex, scSemWaitMax) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Grabber semaphore obtain failed.\n");
      lRet = LIB_FAILURE;
    } // if semaphore not obtained
    else
    {
      if(mProcess.Set(apGraph, apGraphMetadata) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Failed to setup a process.\n");
        lRet = LIB_FAILURE;
      } // if setting a graph to process failed
      else
      {
        // remove all previous IO objects
        for(uint32_t i = 0; i < SEQ_OTHRIX_LAST; i++)
        {
          if(mpIOs[i] != NULL)
          {
            delete(mpIOs[i]);
            mpIOs[i] = NULL;
          } // if IO set
        } // for all possible IO objects

        // get new IO grabber objects
        if(mProcess.IOsGet(mpIOs) != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Failed to get the IO objects from process.\n");
          lRet = LIB_FAILURE;
        } // if IOsGet() failed
      } // else from if setting a graph to process failed

      if (OAL_SemaphoreRelease(mThreadMutex) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Grabber semaphore obtain failed.\n");
        lRet = LIB_FAILURE;
      } // if mutex release failed
    } // else from if semaphore not obtained
  }// else from atribute pointers != NULL
  else
  {
    VDB_LOG_ERROR("NULL pointer given instead of valid sdi_process object.\n");
    lRet = LIB_FAILURE;
  } // else from if attribute pointers != NULL
  return lRet;
} // sdi_grabber::ProcessSet(SEQ_Head_t *apGraph[])

//****************************************************************************

SEQ_Buf_t** sdi_grabber::GraphFetch(SEQ_Head_Ptr_t** appGraph)
{  
  return mProcess.GraphFetch(appGraph);
} // sdi_grabber::GraphFetch(SEQ_Head_Ptr_t** appGraph)

//****************************************************************************

sdi_io* sdi_grabber::IoGet(SEQ_Othr_ix_t aIoId) const
{
  sdi_io *lpRet = NULL;
  if(aIoId < SEQ_OTHRIX_LAST) 
  {
    lpRet = mpIOs[aIoId];
  }
  
  return lpRet;
} // sdi_grabber::IoGet(SEQ_Othr_ix_t aIoId)

//****************************************************************************

LIB_RESULT sdi_grabber::SramBufferGet(const char *acpName, 
                                      SEQ_Buf_t &arSramBuf) const
{      
  return mProcess.SramBufferGet(acpName, arSramBuf);
} // sdi_grabber::SramBufferGet()

//****************************************************************************

LIB_RESULT sdi_grabber::FrameReadyCallBackInstall(
                          void (*apFrameReadyCallBack)(void*),
                          void* apFrameReadyCallBackParam)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  if (OAL_SemaphoreObtain(mThreadMutex, scSemWaitMax) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Grabber semaphore obtain failed.\n");
    lRet = LIB_FAILURE;
  } else
  {
    if (apFrameReadyCallBack != NULL)
    {
      mGC.mCallBackFcn = apFrameReadyCallBack;
      mGC.mCallBackParam = apFrameReadyCallBackParam;
    } else
    {
      VDB_LOG_ERROR("CallBack function specified as NULL pointer.\n");
      lRet = LIB_FAILURE;
    } // else from if callback fcn != NULL

    if (OAL_SemaphoreRelease(mThreadMutex) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Grabber semaphore obtain failed.\n");
      lRet = LIB_FAILURE;
    } // if mutex release failed
  } // else from if semaphore obtain failed
  return lRet;
} // FrameReadyCallBackInstall

//****************************************************************************

LIB_RESULT sdi_grabber::SeqEventCallBackInstall(
                              void (*apSeqEventCallBack)(uint32_t, void*),
                              void* apSeqEventCallBackParam)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  if (OAL_SemaphoreObtain(mThreadMutex, scSemWaitMax) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Grabber semaphore obtain failed.\n");
    lRet = LIB_FAILURE;
  } else
  {
    if (apSeqEventCallBack != NULL)
    {
      mpSeqEventCb      = apSeqEventCallBack;
      mpSeqEventCbParam = apSeqEventCallBackParam;
    } else
    {
      VDB_LOG_ERROR("CallBack function specified as NULL pointer.\n");
      lRet = LIB_FAILURE;
    } // else from if callback fcn != NULL

    if (OAL_SemaphoreRelease(mThreadMutex) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Grabber semaphore obtain failed.\n");
      lRet = LIB_FAILURE;
    } // if mutex release failed
  } // else from if semaphore obtain failed
  return lRet;
} // SeqEventCallBackInstall

//****************************************************************************

LIB_RESULT sdi_grabber::PreStart()
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
  // lock the grabber
  if (OAL_SemaphoreObtain(mThreadMutex, scSemWaitMax) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Grabber semaphore obtain failed.\n");
    lRet = LIB_FAILURE;
  } // if SemaphoreObtain failed
  else
  {
    if(mStatus == OFF)
    {  
#ifdef __STANDALONE__  
      IspHwSramAccesAllow();
#endif // #ifdef __STANDALONE__  
      // *** reserve Sequencer access ***
      if(SEQ_Reserve() == SEQ_LIB_SUCCESS)
      {
        // set this grabber as current Sequencer event handler
        if(SEQ_EventHandlerSet((SEQ_EventHandlerClass*)this) == SEQ_LIB_SUCCESS)
        {   
          // *** reserve IO objects ***
          if( IOsReserve() != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Failed to reserve IO objects.\n");
            lRet = LIB_FAILURE;
          } // if IOsReserve() failed
          else
          {    
            // *** process finalize ***
            // allocates SRAM buffers & reserves TCs
            if(mProcess.Finalize() != LIB_SUCCESS)
            {
              VDB_LOG_ERROR("Failed to finalize process.\n");
              lRet = LIB_FAILURE;
            } // if Process::Finalize() failed
            else
            {
              // *** apply IO objects parameters ***
              if(IOsSetup() != LIB_SUCCESS)
              {
                VDB_LOG_ERROR("Failed to setup IO objects.\n");
                lRet = LIB_FAILURE;
              } // if IOsSetup() failed
              else
              {
                // *** download the Graph ***	
                if(mProcess.Download() != LIB_SUCCESS)
                {
                  VDB_LOG_ERROR("Failed to Download the process.\n");
                  lRet = LIB_FAILURE;
                } // if download failed
                else
                {
                  // *** boot the Sequencer FW ***
                  if(SEQ_Reset() != SEQ_LIB_SUCCESS)
                  {
                    VDB_LOG_ERROR("Failed to reset sequencer.\n");
                    lRet = LIB_FAILURE;
                  } // if seq reset failed
                  else
                  {
                    if(SEQ_Boot() != SEQ_LIB_SUCCESS)
                    {
                      VDB_LOG_ERROR("Failed to boot sequencer.\n");
                      lRet = LIB_FAILURE;
                    } // if seq boot failed
                    else
                    {                  
                      // set to ready status
                      mStatus = READY;
                    } // else from if seq boot failed
                  } // else from if seq reser failed
                } // else from if download failed
              } // else from if IOsSetup() failed
              // release IOs
              if(lRet != LIB_SUCCESS)
              {
                if(IOsRelease() != LIB_SUCCESS)
                {
                  VDB_LOG_ERROR("Failed to reserve IO objects.\n");
                  lRet = LIB_FAILURE;
                } // if IOsRelease()
              } // if failure detected
            } // else from if Process::Finalize() failed 
            
            if(lRet != LIB_SUCCESS)
            {
              // revert the Finalize action
              mProcess.Free();
            }            
          } // else from if IOsReserve() failed          
          if(lRet != LIB_SUCCESS)
          {
            // attempt to reset the event handler
            if(SEQ_EventHandlerSet((SEQ_EventHandlerClass*)NULL) != SEQ_LIB_SUCCESS)
            {
              VDB_LOG_ERROR("Failed to reset Sequencer event handler.\n");
              lRet = LIB_FAILURE;
            } // if failed to reset event handler
          }
        } // if set handler ok
        else 
        {
          VDB_LOG_ERROR("Failed to setup Sequencer event handler.\n");
          lRet = LIB_FAILURE;
        } // else from if set handler ok

        if(lRet != LIB_SUCCESS)
        {
          // attempt to release the sequencer
          (void)SEQ_Release();
        }
      } // if seq reserved ok
      else
      {
        VDB_LOG_ERROR("Sequencer reservation failed.\n");
        lRet = LIB_FAILURE;
      } // else from if seq reserved ok
    } // if SRAM_PRO2HOST_OFF
    else
    {
      VDB_LOG_ERROR("Grabber is not OFF.\n");
      lRet = LIB_FAILURE;
    } // else from if OFF
    
    // release the lock
    if (OAL_SemaphoreRelease(mThreadMutex) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Grabber semaphore release failed.\n");
      lRet = LIB_FAILURE;
    } // if mutex release failed
  } // else from if Semaphore obtain failed
    
  return lRet;
} // sdi_grabber::PreStart()

//****************************************************************************

LIB_RESULT sdi_grabber::Release()
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
  // lock the grabber
  if (OAL_SemaphoreObtain(mThreadMutex, scSemWaitMax) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Grabber semaphore obtain failed.\n");
    lRet = LIB_FAILURE;
  } // if SemaphoreObtain failed
  else
  {
    if(mStatus == READY)
    {     
      // release IO objects
      if(IOsRelease() != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Failed to release IO objects.\n");
        lRet = LIB_FAILURE;
      } // if IOsRelease()
      
      // revert the Finalize action
      mProcess.Free();
        
      // attempt to reset the event handler
      if(SEQ_EventHandlerSet((SEQ_EventHandlerClass*)NULL) != SEQ_LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Failed to reset Sequencer event handler.\n");
        lRet = LIB_FAILURE;
      } // if failed to reset event handler   
      
      // reset the driver and ISP HW
      if(SEQ_Reset() != SEQ_LIB_SUCCESS)
      {
        lRet = LIB_FAILURE;
      } // if failed to reset
      
      // release Sequencer HW
      if(SEQ_Release() != SEQ_LIB_SUCCESS)
      {
        lRet = LIB_FAILURE;
      } // if failed to reset      
      
      mStatus = OFF;
    } // if READY
    else
    {
      if(mStatus == ON)
      {
        VDB_LOG_ERROR("Failed to release. Sequencer still running.\n");
        lRet = LIB_FAILURE;
      } // if still on
    } // else from if READY
    
    // release the lock
    if (OAL_SemaphoreRelease(mThreadMutex) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Grabber semaphore release failed.\n");
      lRet = LIB_FAILURE;
    } // if mutex release failed
  } // else from if Semaphore obtain failed

  return lRet;
} // sdi_grabber::PreStart(uint32_t)

//****************************************************************************

LIB_RESULT sdi_grabber::Start(uint32_t aFrameCnt, uint32_t aInputLines)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
  //TODO: remove unused parameter suppression
  UNUSED(aFrameCnt);
  
  // lock the grabber
  if (OAL_SemaphoreObtain(mThreadMutex, scSemWaitMax) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Grabber semaphore obtain failed.\n");
    lRet = LIB_FAILURE;
  } // if SemaphoreObtain failed
  else
  {
    if(mStatus == READY)
    {
      mStatus = ON;
      // *** start up the graph ***
      if(SEQ_GraphStart(aFrameCnt, aInputLines) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Failed to start the grabbing sequence.\n");
        lRet = LIB_FAILURE;
        mStatus = READY;
      } // if Start failed
      else
      {
        // start IO objects
        if(IOsStart() != LIB_SUCCESS)
        {
          (void)SEQ_GraphStop(1);
          VDB_LOG_ERROR("Failed to start IO objects.\n");
          lRet   = LIB_FAILURE;
          mStatus = READY;
        } // if IOsStart()
      }
      
    } // if READY
    else
    {
      VDB_LOG_ERROR("Sequencer not READY.\n");
      lRet = LIB_FAILURE;
    } // else from if READY
    
    // release the lock
    if (OAL_SemaphoreRelease(mThreadMutex) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Grabber semaphore release failed.\n");
      lRet = LIB_FAILURE;
    } // if mutex release failed
  } // else from if Semaphore obtain failed

  return lRet;
} // sdi_grabber::Start(uint32_t)

//****************************************************************************

LIB_RESULT sdi_grabber::Stop(uint32_t)
{
  LIB_RESULT lRet = LIB_SUCCESS;
    
  // lock the grabber
  if (OAL_SemaphoreObtain(mThreadMutex, scSemWaitMax) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Grabber semaphore obtain failed.\n");
    lRet = LIB_FAILURE;
  } // if SemaphoreObtain failed
  else
  {
    if(mStatus == ON)
    {
      if(SEQ_GraphStop(1) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Failed to stop the grabbing sequence.\n");
        lRet = LIB_FAILURE;
      } // if Start failed
      else
      {
        // start IO objects
        if(IOsStop() != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Failed to stop IO objects.\n");
          lRet    = LIB_FAILURE;
          mStatus = READY;
        } // if IOsStop()
        
        // *** reset the driver and ISP HW ***
        if(SEQ_Reset() != SEQ_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        } // if failed to reset
        mStatus = READY;
      } // else from if Start failed
    } // if ON
    else
    {
      VDB_LOG_WARNING("Sequencer not ON.\n");
    } // else from if READY
    
    // release the lock
    if (OAL_SemaphoreRelease(mThreadMutex) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Grabber semaphore release failed.\n");
      lRet = LIB_FAILURE;
    } // if mutex release failed
  } // else from if Semaphore obtain failed
  
  /*if (OAL_SemaphoreObtain(mThreadMutex) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Grabber semaphore obtain failed.\n");
    lRet = LIB_FAILURE;
  } else
  {
    if( mGrabTask.mStatus == sdi_grabTask::ON)
    {
      // add requested frames to be captured then end grabbing
      mGrabTask.mGC.mFramesToBeCaptured = mGrabTask.mGC.mFramesCaptured + aFrameCnt;
      if (OAL_SemaphoreRelease(mThreadMutex) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Grabber semaphore obtain failed.\n");
        lRet = LIB_FAILURE;
      } // if mutex release failed
    } // if already running
    else
    {
      VDB_LOG_WARNING("Grabber is not running.");
      if (OAL_SemaphoreRelease(mThreadMutex) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Grabber semaphore release failed.\n");
        lRet = LIB_FAILURE;
      } // if mutex release failed
    } // else from if not running now
  } // else from if semaphore obtain failed*/

  return lRet;
} // sdi_grabber::Stop(uint32_t)

//***************************************************************************

SDI_Frame sdi_grabber::FramePop(uint32_t aChnlIdx, uint32_t aTimeoutMs)
{  
  SDI_Frame  lRet;
  LIB_RESULT lRes = LIB_SUCCESS;
  
  // by default sleep for 1s
#ifndef __STANDALONE__       
  struct timespec lTimeToSleep = {aTimeoutMs/1000, (aTimeoutMs%1000)*1000000}; 
#else // #ifndef __STANDALONE__
  uint64_t  lTimeValStart;
  lTimeValStart = get_uptime_microS();
#endif // else from #ifndef __STANDALONE__  
  SEQ_FrmBufferInfo_t lBufferInfo;
  lBufferInfo.mStreamIdx = aChnlIdx;

  // try to pop new frame
  while(SEQ_FrmBufferPop(&lBufferInfo) != SEQ_LIB_SUCCESS)
  {
    if(aTimeoutMs > 0)
    { 
      uint64_t        lTimeSpent;  
    ///    printf("1\n");

#ifndef __STANDALONE__      
      lTimeSpent   = aTimeoutMs - \
        lTimeToSleep.tv_sec  * 1000 - \
        lTimeToSleep.tv_nsec / 1000000;
     ///   printf("lTimeSpent=%d\n",lTimeSpent);

#else // #ifndef __STANDALONE__
      lTimeSpent   = (get_uptime_microS() - lTimeValStart) / 1000;
#endif // else from #ifndef __STANDALONE__
      
      if(aTimeoutMs <= lTimeSpent)
      {
     ///   printf("2\n");
        lRes = LIB_FAILURE;
        VDB_LOG_ERROR("Frame wait timed out\n");
      ///  break;//by zhy
      } // if timed-out
    } // if timeout required
    
#ifndef __STANDALONE__        
    if(nanosleep(&lTimeToSleep, &lTimeToSleep) == 0)
    {
      lRes = LIB_FAILURE;
   ///   break; //by zhy
    } // if whole sleep ended uninterrupted   
    
#endif // else from #ifndef __STANDALONE__
  } // while FrmBufferPop() failed 
 ///  printf("4\n");
  if(lRes == LIB_SUCCESS)
  {
       ///   printf("5\n");

    ((sdi_FdmaIO*)mpIOs[SEQ_OTHRIX_FDMA])->DdrBufferQuery(
        aChnlIdx, 
        lBufferInfo.mBufferIdx, 
        lRet.mUMat);
    lRet.mChannelIdx = aChnlIdx;
    lRet.mBufferIdx  = lBufferInfo.mBufferIdx; 
    lRet.mFrmSeqNum  = lBufferInfo.mFrmSeqNum;
  } // if did not fail
  return lRet;
} // FramePop(uint32_t aChnlIdx, uint32_t aTimeoutMs)

//***************************************************************************

SDI_Frame sdi_grabber::FramePopNonBlock(uint32_t aChnlIdx)
{  
  SDI_Frame lRet;
  
  SEQ_FrmBufferInfo_t lBufferInfo;
  lBufferInfo.mStreamIdx = aChnlIdx;
  
  // try to pop new frame
  if(SEQ_FrmBufferPop(&lBufferInfo) == SEQ_LIB_SUCCESS)
  {    
    ((sdi_FdmaIO*)mpIOs[SEQ_OTHRIX_FDMA])->DdrBufferQuery(aChnlIdx, 
                                                          lBufferInfo.mBufferIdx, 
                                                          lRet.mUMat);
    lRet.mChannelIdx = aChnlIdx;
    lRet.mBufferIdx  = lBufferInfo.mBufferIdx;   
  } // if pop succeeded
  return lRet;
} // FramePop(uint32_t aChnlIdx)
  
//***************************************************************************

LIB_RESULT sdi_grabber::FramePush(SDI_Frame &arFrame)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
  SEQ_FrmBufferInfo_t lBufferInfo;
  lBufferInfo.mStreamIdx = arFrame.mChannelIdx;
  lBufferInfo.mBufferIdx = arFrame.mBufferIdx;
  
  if(SEQ_FrmBufferPush(&lBufferInfo) != LIB_SUCCESS)
  {
    lRet = LIB_FAILURE;
  } // if frame push failed
  
  return lRet;
} // FramePush(SDI_Frame &arFrame)

//****************************************************************************

bool sdi_grabber::IsLive()
{
  bool live = false;
  if (mThreadMutex != NULL)
  {
    if (OAL_SemaphoreObtain(mThreadMutex, scSemWaitMax) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Grabber mutex obtain failed.\n");
    } else
    {
      live = mGrabTask.mStatus == sdi_grabTask::ON ? true: false;
      if (OAL_SemaphoreRelease(mThreadMutex) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Grabber mutex release failed.\n");
      } // if thread mutex release failed
    } // else from if thread mutex obtain failed
  } // if thread mutex initialized
  return live;
} // sdi_grabber::IsLive()

//****************************************************************************

LIB_RESULT sdi_grabber::IOsRelease()
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
  // release all prior reserved IOs
  for(uint32_t i = 0; i < SEQ_OTHRIX_LAST; i++)
  {
    if(mpIOs[i] == NULL)
    {
      continue;
    } // if no IO
    
    if(mpIOs[i]->Release() != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to release IO object %u.\n", i);
      lRet = LIB_FAILURE;
    } // if Release() failed
  } // for all possible IOs
  
  return lRet;
} // sdi_grabber::IOsRelease()
  
//****************************************************************************

LIB_RESULT sdi_grabber::IOsReserve()
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
  for(uint32_t i = 0; i < SEQ_OTHRIX_LAST; i++)
  {
    if(mpIOs[i] == NULL)
    {
      continue;
    } // if no IO
    
    if(mpIOs[i]->Reserve() != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to reserve IO object %u.\n", i);
      lRet = LIB_FAILURE;
      // release all prior reserved IOs
      if(IOsRelease() != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Failed to release so far allocated IOs.\n");
      } // if IOsRelease() failed
      break;
    } // if Reserve() failed
  } // for all IO objects
  return lRet;
} // sdi_grabber::IOsReserve()

//****************************************************************************

LIB_RESULT sdi_grabber::IOsSetup()
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
  for(uint32_t i = 0; i < SEQ_OTHRIX_LAST; i++)
  {
    if(mpIOs[i] == NULL)
    {
      continue;
    } // if no IO
    
    if(mpIOs[i]->Setup() != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to setup IO object %u.\n", i);
      lRet = LIB_FAILURE;
      break;
    } // if Setup() failed
  } // for all IO objects
  return lRet;
} // sdi_grabber::IOsSetup()

//****************************************************************************

LIB_RESULT sdi_grabber::IOsStart()
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
  for(uint32_t i = 0; i < SEQ_OTHRIX_LAST; i++)
  {
    if(mpIOs[i] == NULL)
    {
      continue;
    } // if no IO
    
    if(mpIOs[i]->Start() != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to start IO object %u.\n", i);
      lRet = LIB_FAILURE;
      break;
    } // if Start() failed
  } // for all IO objects
  return lRet;
} // sdi_grabber::IOsStart()

//****************************************************************************

LIB_RESULT sdi_grabber::IOsStop()
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
  for(uint32_t i = 0; i < SEQ_OTHRIX_LAST; i++)
  {
    if(mpIOs[i] == NULL)
    {
      continue;
    } // if no IO
    
    if(mpIOs[i]->Stop() != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to stop IO object %u.\n", i);
      lRet = LIB_FAILURE;
      break;
    } // if Stop() failed
  } // for all IO objects
  return lRet;
} // sdi_grabber::IOsStop()

//****************************************************************************

void sdi_grabber::SeqEventHandler(uint32_t aEventType)
{
  VDB_LOG_NOTE("Grabber got Sequencer event number %u.\n", aEventType);
  if(mGC.mCallBackFcn != NULL)
  {
    mGC.mCallBackFcn(mGC.mCallBackParam);
  } // if callback set
  
  if(mpSeqEventCb != NULL)
  {
    mpSeqEventCb(aEventType, mpSeqEventCbParam);
  } // if Sequencer event callback installed
  
  if(aEventType == SEQ_MSG_TYPE_RAB)
  {
    VDB_LOG_NOTE("Sequencer Ready-after-boot message reached user application.");
  }
  
  if(aEventType == SEQ_MSG_TYPE_FRAMEDONE)
  {
    VDB_LOG_NOTE("Frame done.\n");
  } // if FRAMEDONE
  
  if(aEventType == SEQ_MSG_TYPE_SEQDONE)
  {
    VDB_LOG_NOTE("Grabber status set to ready %u %u.\n", mStatus);
    mStatus = READY;
  }
  
  if(aEventType == SEQ_MSG_TYPE_OTHERERROR)
  {
    char lString[SEQ_PRAM_AUX_DATA_SIZE] = {0};
    
    printf("*** Other error message received.\n");
    printf("*** Sequencer error text:\n");
    
    if(SEQ_MsgAuxDataGet(lString, SEQ_PRAM_AUX_DATA_SIZE) != SEQ_LIB_SUCCESS)
    {
      VDB_LOG_NOTE("Failed to get the auxiliary data from PRAM.\n");
    } // if failed to read the error message
    else
    {
      printf("  %s\n", lString);
    } // else from if failed to read the error message
  } // if SEQ_MSG_TYPE_OTHERERROR    
} // EventHandler(uint32_t aEventType)

//****************************************************************************

// *** sdi_mutex ***

//****************************************************************************

sdi_mutex::sdi_mutex()
{
  if (OAL_SemaphoreCreate(&mMutex, "SdiMutex", OAL_SEMAPHORE_MUTEX, 1)
      != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("sdi_mutex creation failed.\n");
  }
} // sdi_mutex::sdi_mutex()

//****************************************************************************

bool sdi_mutex::IsReady()
{
  bool res = true;
  if (mMutex == NULL)
  {
    res = false;
  } // if mMutex == NULL
  return res;
} // sdi_mutex::IsReady()

//****************************************************************************

inline LIB_RESULT sdi_mutex::Obtain()
{
  return LIB_SUCCESS;//return OAL_SemaphoreObtain(mMutex);
} // sdi_mutex::Obtain()

//****************************************************************************

inline LIB_RESULT sdi_mutex::Release()
{
  return LIB_SUCCESS; //return OAL_SemaphoreRelease(mMutex);
} // sdi_mutex::Release()

//****************************************************************************

sdi_mutex::~sdi_mutex()
{
#ifndef __STANDALONE__  
  if (OAL_SemaphoreDelete(mMutex) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("sdi_mutex deletion failed.\n");
    mMutex = NULL;
  }
#endif // #ifndef __STANDALONE__
} // sdi_mutex::~sdi_mutex()

//****************************************************************************

// *** sdi ***

LIB_RESULT sdi::InitFirst()
{
  LIB_RESULT lRet = LIB_SUCCESS;

#ifndef __STANDALONE__
  uint32_t sensor_cnt = 0;
  // when creating shm, there's no one to access the HW at the same time
  // this should be safe
  if( SensorCntGet(sensor_cnt) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Query for available sensor count failed.\n");
    lRet = LIB_FAILURE; // some failure = NOK
  }else
  {
    if(ShmConnect(sensor_cnt) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to connect shared memory variables.\n");
      mShm = NULL;
      lRet = LIB_FAILURE;
    }else
    {
      // set shared variables to initial values
      *mpInitCounter = 1;
      *mpSensorCnt   = sensor_cnt;
      mNextProcessId = 1;
      mThreadCounter = 1;
      memset((void*)mpSensors, 0, sizeof(sdi_input) * sensor_cnt);

      // set all prepared process pointers to NULL
      //for (uint32_t i = 0; i < mProcesses.size(); ++i)
      //{
      //  mProcesses[i] = NULL;
      //} // for all processes

      if( HwInit() != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Failed to initialize HW.\n");
        lRet = LIB_FAILURE;
      }else
      {
        if( OAL_SemaphoreRelease(mInitSemaphore) != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Failed to release init semaphore.\n");
          lRet = LIB_FAILURE;
        }else
        {
          VDB_LOG_NOTE("First process & thread initialized successfully.\n");
        } // else from if SemaphoreRelease() failed
        if(lRet == LIB_FAILURE)
        {
          if(HwClose() != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Failed to close HW.\n");
          }// if HwClose() failed
          mNextProcessId = 0;
          mThreadCounter = 0;
        }
      }// else from if HwInit() failed
      if( lRet == LIB_FAILURE)
      {
        if( ShmDisconnect(true) != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Failed to disconnect shared memory.\n");
        } // if ShmDisconnect() failed
      } // if LIB_FAILURE
    } // else from if ShmConnect() failed
  } // else from if SensorCntGet() failed
#endif // #ifndef __STANDALONE__
  return lRet;
} // sdi::InitFirst()

//****************************************************************************

LIB_RESULT sdi::InitSubseq(bool arRepeat)
{
  LIB_RESULT lRet = LIB_SUCCESS;
#ifndef __STANDALONE__
  if(ShmConnect(arRepeat) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to connect shared memory variables.\n");
    mShm = NULL;
    lRet = LIB_FAILURE;
  }else
  {
    // check if another client allowed
    if((*mpInitCounter) > SDI_MAX_CLIENT_NUM)
    {
      VDB_LOG_ERROR("Maximum number of SDI clients reached already. Nothing done.\n");
      lRet = LIB_FAILURE;
    }else{
      ++(*mpInitCounter);
      mNextProcessId = 1;
      mThreadCounter = 1;
      // set all prepared process pointers to NULL
      //for (uint32_t i = 0; i < mProcesses.size(); ++i)
      //{
      //  mProcesses[i] = NULL;
      //} // for all processes

      if( OAL_SemaphoreRelease(mInitSemaphore) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Failed to release init semaphore.\n");
        --(*mpInitCounter);
        mNextProcessId = 0;
        mThreadCounter = 0;
        lRet = LIB_FAILURE;
      }else
      {
        VDB_LOG_NOTE("First thread in consecutive process initialized successfully.\n");
      } // else from if SemaphoreRelease() failed
    }
    if( lRet == LIB_FAILURE)
    {
      if( ShmDisconnect(false) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Failed to disconnect shared memory.\n");
      } // if ShmDisconnect() failed
    } // if LIB_FAILURE
  } // else from if ShmConnect() failed
#endif // #ifndef __STANDALONE__
  return lRet;
} // sdi::InitSubseq()

//****************************************************************************

LIB_RESULT sdi::Initialize(uint32_t /*aTaskNum*/)
{//u
  LIB_RESULT lRet = LIB_SUCCESS;
  
#ifdef SDI_THRED_SAFE // process/thread safety under development    -no use
  OAL_SEMAPHORE initSem = NULL;

  bool firstProcess = false;
  bool init_obtained = false;
  
  //TODO: remove unused parameter suppression
  UNUSED(aTaskNum);
  
  while(1)
  {
    // synchronize with init semaphore
    lRet = InitSemSynchronize(&initSem,&firstProcess, &init_obtained);

    // continue only if all ok so far
    if (lRet == LIB_SUCCESS)
    {
      // now init semaphore has been created/opened and obtained
      if(firstProcess)
      {
        // remember init semaphore
        mInitSemaphore = initSem;
        if( InitFirst() != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("First process calling SDI initialization failed.\n");
          mInitSemaphore = NULL;
          if(OAL_SemaphoreOwnershipSet(initSem, true) != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Init semaphore ownership set failed.\n");
          } // SemaphoreOwnershipSet() failed
          if(OAL_SemaphoreDelete(initSem) != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Init semaphore delete failed.\n");
          } // if init semaphore delete failed
          lRet = LIB_FAILURE;
        } // if InitFirst() Failed
      }else
      {
        //check if SDI initialized for this process already
        if( mInitSemaphore == NULL )
        {
          // sdi has not been initialized for this process yet
          mInitSemaphore = initSem;
          bool repeat = false;
          if( InitSubseq(repeat) != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Subsequent call to SDI initialization failed. SDI probably de-initialized in between.\n");
            if(OAL_SemaphoreRelease(initSem) != LIB_SUCCESS)
            {
              VDB_LOG_ERROR("Init semaphore release failed.\n");
              lRet = LIB_FAILURE;
            } // if init semaphore release failed
            if(OAL_SemaphoreDelete(initSem) != LIB_SUCCESS)
            {
              VDB_LOG_ERROR("Init semaphore delete failed.\n");
              lRet = LIB_FAILURE;
            } // if init semaphore delete failed
            mInitSemaphore = NULL;
            if(lRet == LIB_SUCCESS && repeat)
            {
              continue;
            } // if no other problem
          } // if InitSubseq() failed
        }else
        {
          bool max_reached = false;
          if((*mpInitCounter) >= SDI_MAX_CLIENT_NUM)
          {
            VDB_LOG_ERROR("Maximum number of SDI clients reached already. Nothing done.\n");
            max_reached = true;
            lRet = LIB_FAILURE;
          }else
          {
            // increase counters
            ++(*mpInitCounter);
            ++mThreadCounter;
          } // else from if SDI_MAX_CLIENT_NUM reached
          // release SDI init semaphore
          if( OAL_SemaphoreRelease(initSem) != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Failed to release init semaphore.\n");
            lRet = LIB_FAILURE;
          }else
          {
            /*if( OAL_SemaphoreDelete(initSem) != LIB_SUCCESS )
            {
              VDB_LOG_ERROR("Failed to delete local init semaphore.\n");
              lRet = LIB_FAILURE;
            } // if  SemaphoreDelete() failed
            */
          } // else from if SemaphoreRelease() failed

          if(lRet == LIB_FAILURE)
          {
            if(!max_reached)
            {
            --(*mpInitCounter);
            --mThreadCounter;
            } // if max_reached
          }else
          {
            VDB_LOG_NOTE("Ordinary thread initialized successfully.\n");
          }// if LIB_FAILURE
        } // if all ok so far
      } // else from if first process
    } // if all ok so far
    break;
  } // repeat in case of SDI closed in between

  VDB_LOG_NOTE("%u Returning from INIT: overall cnt %u thread cnt %u\n", 0,0,0);//aTaskNum, *mpInitCounter, mThreadCounter);
#endif // if 0

  return lRet;
} // Initialize()

//****************************************************************************

LIB_RESULT sdi::InitSemSynchronize(OAL_SEMAPHORE */*apInitSem*/, 
                                   bool */*apFirstProcess*/, 
                                   bool */*apSemObtained*/)
{  
  LIB_RESULT lRet = LIB_SUCCESS;
#ifdef SDI_THRED_SAFE // process/thread safety under development
  *apFirstProcess = false;
  *apSemObtained = false;

  while (1)
  {
    lRet = OAL_SemaphoreCreate(apInitSem,
                               scpSemInitName,
                               OAL_SEMAPHORE_COUNTING,
                               0);
    if (lRet != LIB_SUCCESS)
    {
      if (lRet == LIB_FAILURE)
      {
        // semaphore probably exists - server is running already
        // open existing server semaphore
        lRet = OAL_SemaphoreGetByName(apInitSem, scpSemInitName);
        // check again
        if (lRet != LIB_SUCCESS)
        {
          if (lRet == OAL_ERR_SEM_EBADSEM)
          {
            //  server probably ended in between try repeat sem_open O_CREAT
            VDB_LOG_WARNING("Sdi probably closed during client init. Repeating init procedure.");
            continue;
          }
          VDB_LOG_ERROR("Init semaphore get by name failed. Returning.\n");
          lRet = LIB_FAILURE;
          *apInitSem = NULL;
          break;  // leave the loop
        }else
        {
          LIB_RESULT tmpLres;
          // try to obtain init semaphore
          tmpLres = OAL_SemaphoreObtain(*apInitSem, scSemWaitMax);
          if( tmpLres != LIB_SUCCESS)
          {
            if( tmpLres == OAL_ERR_SEM_EBADSEM)
            {
              //  server probably ended in between try repeat sem_open O_CREAT
              VDB_LOG_WARNING("Sdi probably closed during client init. Repeating init procedure.");
              continue;
            }else if( tmpLres == OAL_ERR_SEM_ETIMEOUT)
            {
              VDB_LOG_ERROR("Init semaphore obtain timedout. Returning.\n");
              lRet = LIB_FAILURE;
            }else
            {
              VDB_LOG_ERROR("Init semaphore obtain failed. Returning.\n");
              lRet = LIB_FAILURE;
            }// else from if sdi closed in between
            if(OAL_SemaphoreDelete(*apInitSem) != LIB_SUCCESS)
            {
              VDB_LOG_ERROR("Init semaphore delete failed. Returning.\n");
            }
            *apInitSem = NULL;
            // break loop to return with previously set LIB_FAILURE
            break;
          }else
          {
            VDB_LOG_NOTE("Init semaphore opened and locked.\n");
            *apSemObtained = true;
            break;
          } // if SemaphoreTimedWait failed
        }// if != LIB_SUCCESS
      } else
      {
        VDB_LOG_ERROR("Init semaphore open failed. Returning.\n");
        lRet = LIB_FAILURE; // some failure = NOK
        break; // leave the loop
      } // else LIB_FAILURE
    } else
    {
      // this is first call to library - setup shared memmory
      *apFirstProcess = true;
      *apSemObtained = true;
      if( OAL_SemaphoreOwnershipSet(*apInitSem, false) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Init semaphore ownership set failed.\n");
        lRet = LIB_FAILURE;
      } // if Shared memory ownership set failed

      break;
    } // else SEM_FAILED
  } // while(1)
#endif 
  return lRet;
} // sdi::InitSemSynchronize()

//****************************************************************************

LIB_RESULT sdi::Close(uint32_t /*aTaskNum*/)
{
  LIB_RESULT lRet = LIB_SUCCESS;

#ifdef SDI_THRED_SAFE  // process/thread safety under development
  printf("SDI CLOSE %u\n", aTaskNum);
  VDB_LOG_NOTE("Sdi CLOSE here.\n");
  // obtain Init semaphore
  if (OAL_SemaphoreObtain(mInitSemaphore,scSemWaitMax) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Sdi init semaphore obtain failed.\n");
    lRet = LIB_FAILURE;
  }
  else
  {
    VDB_LOG_NOTE("%u Close: overall cnt %u thread cnt %u\n", aTaskNum, *mpInitCounter, mThreadCounter);
    if(mpInitCounter != NULL)
    {
      if ((*mpInitCounter) > 0)
      {
        if(mThreadCounter == 1)
        {
          VDB_LOG_NOTE("%u Close: Last thread => destroy local sdi.\n",aTaskNum);
          if ((*mpInitCounter) == 1)
          {
            // this is the last sdi client overall
            if(OAL_SemaphoreOwnershipSet(mSdiMut,true) != LIB_SUCCESS)
            {
              VDB_LOG_ERROR("SDI mutex ownership set failed.\n");
              lRet = LIB_FAILURE;
            } // if mutex ownership set failed
            if(OAL_SemaphoreOwnershipSet(mInitSemaphore,true) != LIB_SUCCESS)
            {
              VDB_LOG_ERROR("Init semaphore ownership set failed.\n");
              lRet = LIB_FAILURE;
            } // if init semaphore ownership set failed
            if(OAL_SharedMemoryOwnershipSet(mShm,true) != LIB_SUCCESS)
            {
              VDB_LOG_ERROR("Shared memory ownership set failed.\n");
              lRet = LIB_FAILURE;
            }  // if shared memory ownership set failed

            VDB_LOG_NOTE("Close: Last process => destroy everything.\n");

            // TODO: fix the two closes
            HwFdClose();
            if(HwClose() != LIB_SUCCESS)
            {
              VDB_LOG_ERROR("Closing HW failed.\n");
            } // if HwClose() failed

            // unmap PRAM
            if(mPramBase != NULL)
            {
              free(mPramBase);
            } // if PRAM mapped

          } // if last process
          // decrease counters
          --(*mpInitCounter);
          --mThreadCounter;

          // destroy sdi init semaphore
          if( OAL_SemaphoreDelete(mSdiMut) != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Sdi mutex semaphore delete failed.\n");
            lRet = LIB_FAILURE;
          } // if SemaphoreDelete() failed

          // destroy shared memory
          if(OAL_SharedMemoryDestroy(mShm) != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Sdi shared memory destruction failed.\n");
            lRet = LIB_FAILURE;
          } // if SemaphoreDelete() failed

          //try to release init semaphore
          if(OAL_SemaphoreRelease(mInitSemaphore) != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Sdi init semaphore release failed.\n");
            lRet = LIB_FAILURE;
          } // SemaphoreRelease() failed

          // destroy sdi init semaphore
          if( OAL_SemaphoreDelete(mInitSemaphore) != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Sdi init semaphore delete failed.\n");
            lRet = LIB_FAILURE;
          } // if SemaphoreDelete() failed

          mInitSemaphore = NULL;
        }else
        {
          VDB_LOG_NOTE("%u Close: Ordinary thread => nothing destroyed.\n", aTaskNum);
          // decrease counters      
          --(*mpInitCounter);
          --mThreadCounter;
          
          //try to release init semaphore
          if(OAL_SemaphoreRelease(mInitSemaphore) != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Sdi init semaphore release failed.\n");
            lRet = LIB_FAILURE;
          } // SemaphoreRelease() failed
        }// else from if last thread in this process
      
      }else
      {
        VDB_LOG_ERROR("%u Sdi clients number already 0. Nothing done.\n", aTaskNum);
        lRet = LIB_FAILURE;
        //try to release init semaphore
        if(OAL_SemaphoreRelease(mInitSemaphore) != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Sdi init semaphore release failed.\n");
          lRet = LIB_FAILURE;
        } // SemaphoreRelease() failed
      } // else from if max number of clients not reached yet
    } // if init counter pointer not NULL
  } // else from if init semaphore obtain failed
#else // if 0
  HwClose();
#endif // else from if 0
  return lRet;
} // sdi::Close()

//****************************************************************************

LIB_RESULT sdi::InputReserve(SDI_SENSOR aSdiInputId, sdi_input *&arpInput)
{
  LIB_RESULT lRet = LIB_SUCCESS;

#ifndef __STANDALONE__  
  arpInput = NULL;
  // sensors are indexed from 0 although ID begins from 1 (0 reserved for invalid)
  uint32_t sensorIdx = (uint32_t) (aSdiInputId) - 1;

  if (sensorIdx < SDI_SENSOR_CNT)
  {
    sdi_input *tmpSensor = &(mpSensors[sensorIdx]);
    if (OAL_SemaphoreObtain(mSdiMut, scSemWaitMax) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Sensor mutex obtain failed.\n");
      lRet = LIB_FAILURE;
    } else
    {
      if(tmpSensor->mReserved)
      {
        VDB_LOG_WARNING("Input reserved already.\n");
        lRet = LIB_FAILURE;
      } // if reserved already
      else
      {
        tmpSensor->mReserved = true;
        arpInput = tmpSensor;

        // todo: HW related reservation
        // might require exclusive access to HW in that case consider sdi_mutex obtaining at the beginning - no sensor mutex needed then
      } // else from if reserved already

      if (OAL_SemaphoreRelease(mSdiMut) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Sensor mutex release failed.\n");
        lRet = LIB_FAILURE;
        arpInput = NULL;
      } // if sensor mutex release failed
    } // else from if sensor mutex obtain failed
  } else
  {
    VDB_LOG_ERROR("Sdi input id not valid. Terminating.\n");
    lRet = LIB_FAILURE;
  } // else from if aSdiInputId valid
#endif // #ifndef __STANDALONE__

  return lRet;
} // sdi::InputReserve()

//****************************************************************************

LIB_RESULT sdi::InputRelease(sdi_input *&arpInput)
{
  LIB_RESULT lRet = LIB_SUCCESS;

#ifndef __STANDALONE__  
  if( arpInput == NULL)
  {
    VDB_LOG_ERROR("Null given instead of valid sdi_input pointer.\n");
    lRet = LIB_FAILURE;
  } // if NULL pointer to Input given
  else
  {
    if (OAL_SemaphoreObtain(mSdiMut, scSemWaitMax) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Sensor mutex obtain failed.\n");
      lRet = LIB_FAILURE;
    } else
    {
      arpInput->mReserved = false;

      // todo: HW related release

      if (OAL_SemaphoreRelease(mSdiMut) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Sensor mutex release failed.\n");
        lRet = LIB_FAILURE;
      } // if sensor mutex release failed
      arpInput = NULL;
    } // else from if sensor mutex obtain failed
  } // else from if  arpInput == NULL
#endif // #ifndef __STANDALONE__

  return lRet;
} // sdi::InputRelease()

//****************************************************************************

/*LIB_RESULT sdi::ProcessReserve(sdi_process *&arpProcess)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  // must obtain mutex - will change sdi member
  if (OAL_SemaphoreObtain(mSdiMut) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Sdi thread mutex obtain failed.\n");
    lRet = LIB_FAILURE;
  } else
  {
    // create new process object
    sdi_process *tmpProcess = new (nothrow) sdi_process();
    if (tmpProcess == NULL)
    {
      VDB_LOG_ERROR("Sdi thread mutex obtain failed.\n");
      lRet = LIB_FAILURE;
    } else
    {
      bool found = false;
      //find slot for new sdi_process
      for (uint32_t i = 0; i < mProcesses.size(); ++i)
      {
        if (mProcesses[i] == NULL)
        {
          tmpProcess->mId = mNextProcessId;
          mProcesses[i] = tmpProcess;
          ++mNextProcessId;
          found = true;
        } // if process slot empty
      } // for all prepared process pointers

      if (!found)
      {
        if (mProcesses.size() < SDI_MAX_CLIENT_NUM)
        {
          mProcesses.push_back(tmpProcess);
        } else
        {
          VDB_LOG_ERROR("Maximum number of sdi clients reached already.\n");
          lRet = LIB_FAILURE;
        } // else from if max client number reached
      } // if no empty slot found

      // reserving process done by filling pointer slot
      // no mutex needed

      arpProcess = tmpProcess;

      // todo: HW related reservation

    } // else from if sdi_process alloc failed

    if (OAL_SemaphoreRelease(mSdiMut) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Sdi thread mutex release failed.\n");
      lRet = LIB_FAILURE;
    } // if sdi mutex release failed
  } // else from if mutex obtain failed

  return lRet;
} // sdi::ProcessReserve()*/

//****************************************************************************

/*LIB_RESULT sdi::ProcessRelease(sdi_process *&arpProcess)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if( arpProcess == NULL)
  {
    VDB_LOG_ERROR("Null given instead of valid sdi_process pointer.\n");
    lRet = LIB_FAILURE;
  } // if NULL pointer to Input given
  else
  {
    // must have mutex - will change sdi member
    if (OAL_SemaphoreObtain(mSdiMut) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Sdi thread mutex obtain failed.\n");
      lRet = LIB_FAILURE;
    } else
    {
      // create new process object
      if (arpProcess == NULL)
      {
        VDB_LOG_ERROR("NULL process pointer.\n");
        lRet = LIB_FAILURE;
      } else
      {
        bool found = false;
        //find slot for new sdi_process
        for (uint32_t i = 0; i < mProcesses.size(); ++i)
        {
          if (mProcesses[i] == arpProcess)
          {
            mProcesses[i] = NULL;
            found = true;
          } // if process slot empty
        } // for all prepared process pointers

        if (!found)
        {
          VDB_LOG_ERROR("Bad process pointer.\n");
          arpProcess->mId = (uint32_t) SDI_SENSOR_INVALID;
          lRet = LIB_FAILURE;
        } else
        {
          // todo: HW related releasing

          // delete process
          delete (arpProcess);
        } // else from if process not found
      } // else from if sdi_process alloc failed

      if (OAL_SemaphoreRelease(mSdiMut) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Sdi thread mutex release failed.\n");
        lRet = LIB_FAILURE;
      } // if sdi mutex release failed
    } // else from if mutex obtain failed
  } // else from if arpProcess == NULL
  return lRet;
  } // sdi::ProcessRelease()*/

//****************************************************************************

LIB_RESULT sdi::ShmConnect(uint32_t /*aSensorCnt*/)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
#ifdef SDI_THRED_SAFE  // process/thread safety under development
  uint8_t *pShm = NULL;

  // calculate required shared memory size = (shm size + init counter +
  // number of sensors + sdi_input * number of sensors)
  uint32_t sdi_shm_size = sizeof(uint32_t) * 3 + /*SEMAPHORE_STRUCT_SIZE
      + */sizeof(sdi_input) * aSensorCnt;
  // create general shared memory area
  if (OAL_SharedMemoryCreate(&mShm, scpShmName, sdi_shm_size) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Shared memory creation failed.\n");
    lRet = LIB_FAILURE; // some failure = NOK
  } else
  {
    VDB_LOG_NOTE("Shared memory object created and opened \n");
    if(OAL_SharedMemoryGetPointer(((void**)&pShm), mShm) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Getting pointer to shared memory failed.\n");
      lRet = LIB_FAILURE;
    }else
    {
      // parcel the shared memory area
      mpSharedMemorySize = (uint32_t*)pShm;
      *mpSharedMemorySize = sdi_shm_size;		// store the shared memory size
      pShm += sizeof(uint32_t);
      mpInitCounter = (uint32_t*)pShm;
      pShm += sizeof(uint32_t);
      mpSensorCnt = (uint32_t*)pShm;
      pShm += sizeof(uint32_t);

      if(OAL_SemaphoreCreate(&mSdiMut,scpMutSdiName,OAL_SEMAPHORE_COUNTING, 1))
      {
        VDB_LOG_ERROR("Creating sdi semaphore failed.\n");
        lRet = LIB_FAILURE;
      }else
      {
        if(OAL_SemaphoreOwnershipSet(mSdiMut,false) != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Mutex ownership set failed.\n");
          lRet = LIB_FAILURE;
        }else
        {
          mpSensors = (sdi_input*)pShm;

          if(OAL_SharedMemoryOwnershipSet(mShm,false) != LIB_SUCCESS)
          {
            VDB_LOG_ERROR("Shared memory ownership set failed.\n");
            lRet = LIB_FAILURE;
          } // if SharedMemoryOwnershipSet() failed
          else
          {
            // TODO: map PRAM
            if((mPramBase = malloc(PRAM_SIZE)) == NULL)
            {
              VDB_LOG_ERROR("Mapping of PRAM failed.\n");

              lRet = LIB_FAILURE;

//              if(OAL_SemaphoreDelete(mSdiMut) != LIB_SUCCESS)
//              {
//                VDB_LOG_ERROR("Deleting shared mutex.\n");
//                lRet = LIB_FAILURE;
//              }// if SemaphoreDelete() failed
            } // if PRAM mapping failed
          } // else from if SharedMemoryOwnershipSet() failed
          if( lRet == LIB_FAILURE)
          {
            if( OAL_SemaphoreOwnershipSet(mSdiMut, true) != LIB_SUCCESS)\
            {
              VDB_LOG_NOTE("Shared mutex ownership set failed.\n");
            } // if SemaphoreDelete() failed
            mpSensors = NULL;
          } // if LIB_FAILURE
        } // else if OwnershipSet() failed
        if( lRet == LIB_FAILURE)
        {
          if( OAL_SemaphoreDelete(mSdiMut) != LIB_SUCCESS)\
          {
            VDB_LOG_NOTE("Shared mutex delete failed.\n");
          } // if SemaphoreDelete() failed
          mpInitCounter = NULL;
          mpSensorCnt = NULL;
        } // if LIB_FAILURE
      } // else from if SemaphoreCreateInterprocessMutexOnAddress() failed
    } // else from if ShareMemoryCreate() failed
    if(lRet == LIB_FAILURE)
    {
      if(OAL_SharedMemoryDestroy(mShm) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Destroying shared memory failed.\n");
      } // OAL_SharedMemoryDestroy() failed
    } // if LIB_FAILURE
  } //else from if SharedMemoryCreate() failed
#endif // #ifdef SDI_THRED_SAFE 
  
  return lRet;
} //  sdi::ShmConnect(uint32_t)

//****************************************************************************

LIB_RESULT ShmSizeGet(uint32_t */*apShmSize*/, const char* /*acpName*/)
{
  LIB_RESULT lRet = LIB_SUCCESS;
#ifdef SDI_THRED_SAFE  // process/thread safety under development

  OAL_SHARED_MEM shm;
  if (OAL_SharedMemoryGetByName(&shm, acpName, sizeof(uint32_t)) != LIB_SUCCESS)
  {
    VDB_LOG_NOTE("Shared memory open failed.\n");
    lRet = LIB_FAILURE;
  } // if shm get by name failed
  else
  {
    uint32_t *pSize = NULL;
    *apShmSize = 0;
    if(OAL_SharedMemoryGetPointer(((void**)&pSize), shm) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Getting pointer to shared memory failed.\n");
      lRet = LIB_FAILURE;
    } // if shm get pointer failed
    else
    {
      *apShmSize = *pSize;
      pSize = NULL;
    } // else form if shm get pointer failed
  }// else from if shm get by name failed
#endif //#if 0
  
  return lRet;
} // LIB_RESULT ShmOpen(OAL_SHARED_MEM* apSharedMem, const char* apName)

//****************************************************************************

LIB_RESULT sdi::ExistingShmOpen(uint32_t */*apShmSize*/)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
#ifdef SDI_THRED_SAFE

  if (OAL_SharedMemoryGetByName(&mShm, scpShmName, sizeof(uint32_t)) != LIB_SUCCESS)
  {
    VDB_LOG_NOTE("Shared memory open failed.\n");
    lRet = LIB_FAILURE;
  } // if shm get by name failed
  else
  {
    uint32_t *pSize = NULL;
    *apShmSize = 0;
    if(OAL_SharedMemoryGetPointer(((void**)&pSize), mShm) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Getting pointer to shared memory failed.\n");
      lRet = LIB_FAILURE;
    } // if shm get pointer failed
    else
    {
      *apShmSize = *pSize;
      pSize = NULL;
      // TODO: map PRAM
      if((mPramBase = malloc(PRAM_SIZE)) == NULL)
      {
        VDB_LOG_ERROR("Mapping of PRAM failed.\n");
        lRet = LIB_FAILURE;
      } // if PRAM mapping failed
    } // else form if shm get pointer failed
    if(lRet == LIB_FAILURE)
    {
      if(OAL_SharedMemoryDestroy(mShm) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Destroying shared memory failed.\n");
        lRet = LIB_FAILURE;
      }// if SemaphoreDelete() failed
    } // if failure detected
  }// else from if shm get by name failed
#endif //#ifdef SDI_THRED_SAFE
  
  return lRet;
} // LIB_RESULT ExistingShmOpen(OAL_SHARED_MEM* apSharedMem, const char* acpName, uint32_t *apShmSize)

//****************************************************************************

LIB_RESULT sdi::ShmConnect(bool &/*arRepeat*/)
{
  LIB_RESULT lRet = LIB_SUCCESS;
#ifdef SDI_THRED_SAFE  
  uint8_t *pShm = NULL;
  arRepeat = false;
  uint32_t shmSize = 0;

  // first thread in consecutive process
  if (ExistingShmOpen(&shmSize) != LIB_SUCCESS)
  //if (OAL_SharedMemoryGetByName(&mShm, scpShmName,256) != LIB_SUCCESS)
  {
    VDB_LOG_NOTE("Shared memory open failed.\n");
    arRepeat = true;
    lRet = LIB_FAILURE;
  }else
  {
    // get the pointer to full shared memory size
    if(OAL_SharedMemoryGetPointer(((void**)&pShm), mShm, shmSize) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Getting pointer to shared memory failed.\n");
      lRet = LIB_FAILURE;
    }else
    {
      // parcel the shared memory area
      mpSharedMemorySize = (uint32_t*)pShm;
      pShm += sizeof(uint32_t);
      mpInitCounter = (uint32_t*)pShm;
      pShm += sizeof(uint32_t);
      mpSensorCnt = (uint32_t*)pShm;
      pShm += sizeof(uint32_t);
      mpSensors = (sdi_input*)pShm;

      if(OAL_SemaphoreGetByName(&mSdiMut, scpMutSdiName) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Getting sdi semaphore by name failed.\n");
        mpSharedMemorySize = NULL;
        mpInitCounter = NULL;
        mpSensorCnt = NULL;
        mpSensors = NULL;
        lRet = LIB_FAILURE;
      } // if sdi semaphore get by name failed
      else
      {
        pShm += SEMAPHORE_STRUCT_SIZE;
        mpSensors = (sdi_input*)pShm;
      } // else from if SemaphoreGetFromMutexAddress)( failed
    } // else from if OAL_SharedMemoryGetPointer()
    if(lRet == LIB_FAILURE)
    {
      if(OAL_SharedMemoryDestroy(mShm) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Destroying shared memory failed.\n");
      } //  SharedMemoryDestroy() failed
    } // if LIB_FAILURE
  }// else form if OAL_SharedMemoryGetByName() failed
#endif // #ifdef SDI_THRED_SAFE  

  return lRet;
} // sdi::ShmConnect()

//****************************************************************************

LIB_RESULT sdi::ShmDisconnect(bool /*aDelete*/)
{
  LIB_RESULT lRet = LIB_SUCCESS;
#ifdef SDI_THRED_SAFE  

  if( aDelete)
  {
    if(OAL_SemaphoreOwnershipSet(mSdiMut,true) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Shared mutex ownership set failed.\n");
      lRet = LIB_FAILURE;
    } // if SemaphoreOwnershipSet() failed
    if(OAL_SharedMemoryOwnershipSet(mShm,true) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Shared memory ownership set failed.\n");
      lRet = LIB_FAILURE;
    } // if SemaphoreOwnershipSet() failed
  } // if aMutDelete

  if(OAL_SemaphoreDelete(mSdiMut) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Deleting shared mutex.\n");
    lRet = LIB_FAILURE;
  }// if SemaphoreDelete() failed

  mpInitCounter = NULL;
  mpSensorCnt = NULL;
  mpSensors = NULL;
#endif // #ifdef SDI_THRED_SAFE

  return lRet;
} // sdi::ShmDisconnect()

//****************************************************************************

LIB_RESULT sdi::SensorCntGet(uint32_t &raSensorCnt)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  // todo: query sequencer

  raSensorCnt = SDI_SENSOR_CNT;

  return lRet;
} // sdi::SensorCntGet()

//****************************************************************************

LIB_RESULT sdi::HwInit()
{
  // TODO: initialize HW (open drivers...)

  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return LIB_SUCCESS;
} // sdi::HwInit()

//****************************************************************************

LIB_RESULT sdi::HwClose()
{
  // TODO: release HW (close drivers...)

  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return LIB_SUCCESS;
} // sdi::HwClose()

//****************************************************************************

void IspHwSramAccesAllow(void)
{
  *(uint32_t*)(0x40006000) = 0x00000000;
  *(uint32_t*)(0x40006004) = 0xffffffff;
  *(uint32_t*)(0x4000600c) = 0x80000000;

  *(uint32_t*)(0x40006020) = 0x00000000;
  *(uint32_t*)(0x40006024) = 0xffffffff;
  *(uint32_t*)(0x4000602c) = 0x80000000;

  *(uint32_t*)(0x40006040) = 0x00000000;
  *(uint32_t*)(0x40006044) = 0xffffffff;
  *(uint32_t*)(0x4000604c) = 0x80000000;

  *(uint32_t*)(0x40006060) = 0x00000000;
  *(uint32_t*)(0x40006064) = 0xffffffff;
  *(uint32_t*)(0x4000606c) = 0x80000000;

  *(uint32_t*)(0x40006080) = 0x00000000;
  *(uint32_t*)(0x40006084) = 0xffffffff;
  *(uint32_t*)(0x4000608c) = 0x80000000;

  *(uint32_t*)(0x400060a0) = 0x00000000;
  *(uint32_t*)(0x400060a4) = 0xffffffff;
  *(uint32_t*)(0x400060ac) = 0x80000000;

  *(uint32_t*)(0x400060c0) = 0x00000000;
  *(uint32_t*)(0x400060c4) = 0xffffffff;
  *(uint32_t*)(0x400060cc) = 0x80000000;

  *(uint32_t*)(0x400060e0) = 0x00000000;
  *(uint32_t*)(0x400060e4) = 0xffffffff;
  *(uint32_t*)(0x400060ec) = 0x80000000;

  *(uint32_t*)(0x40006100) = 0x00000000;
  *(uint32_t*)(0x40006104) = 0xffffffff;
  *(uint32_t*)(0x4000610c) = 0x80000000;

  *(uint32_t*)(0x40006120) = 0x00000000;
  *(uint32_t*)(0x40006124) = 0xffffffff;
  *(uint32_t*)(0x4000612c) = 0x80000000;

  *(uint32_t*)(0x40006140) = 0x00000000;
  *(uint32_t*)(0x40006144) = 0xffffffff;
  *(uint32_t*)(0x4000614c) = 0x80000000;

  *(uint32_t*)(0x40006160) = 0x00000000;
  *(uint32_t*)(0x40006164) = 0xffffffff;
  *(uint32_t*)(0x4000616c) = 0x80000000;

  *(uint32_t*)(0x40006180) = 0x00000000;
  *(uint32_t*)(0x40006184) = 0xffffffff;
  *(uint32_t*)(0x4000618c) = 0x80000000;

  *(uint32_t*)(0x400061a0) = 0x00000000;
  *(uint32_t*)(0x400061a4) = 0xffffffff;
  *(uint32_t*)(0x400061ac) = 0x80000000;

  *(uint32_t*)(0x400061c0) = 0x00000000;
  *(uint32_t*)(0x400061c4) = 0xffffffff;
  *(uint32_t*)(0x400061cc) = 0x80000000;

  *(uint32_t*)(0x400061e0) = 0x00000000;
  *(uint32_t*)(0x400061e4) = 0xffffffff;
  *(uint32_t*)(0x400061ec) = 0x80000000;

  *(uint32_t*)(0x40006200) = 0x00000000;
  *(uint32_t*)(0x40006204) = 0xffffffff;
  *(uint32_t*)(0x4000620c) = 0x80000000;

  *(uint32_t*)(0x40006220) = 0x00000000;
  *(uint32_t*)(0x40006224) = 0xffffffff;
  *(uint32_t*)(0x4000622c) = 0x80000000;

  *(uint32_t*)(0x40006240) = 0x00000000;
  *(uint32_t*)(0x40006244) = 0xffffffff;
  *(uint32_t*)(0x4000624c) = 0x80000000;

  *(uint32_t*)(0x40006260) = 0x00000000;
  *(uint32_t*)(0x40006264) = 0xffffffff;
  *(uint32_t*)(0x4000626c) = 0x80000000;
} // IspHwSramAccesAllow()

//****************************************************************************

// *** EOF ***
