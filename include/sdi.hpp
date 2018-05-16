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
 * \file 		sdi.hpp
 * \brief 		Sensor device interface functionality declaration.
 * \author 	Tomas Babinec
 * \version 	0.1
 * \date 		16-September-2013
 ****************************************************************************/

#ifndef SDI_HPP
#define SDI_HPP

#ifndef __STANDALONE__
#include <vector>
#include <map>
#include <list>
#include <fcntl.h>		/* open */
#include <unistd.h>		/* exit */
#include <sys/ioctl.h>		/* ioctl */
#endif // #ifndef __STANDALONE__

//#include "oal_task.hpp"
#include "../libs/utils/oal/user/include/oal_process.h"
#include "../libs/isp/fdma/kernel/include/fdma_types.h"
#include "isp_seq.h"
#include "isp_cam_generic.h"

#include "seq_graph.h"
#include "seq_graph_meta.h"

//#include "../libs/io/gdi/include/gdi_image.hpp"
#include "../libs/io/sdi/include/sdi_types.hpp"
#include "../libs/io/sdi/include/sdi_io.hpp"

using namespace std;

//****************************************************************************
// contant definitions
//****************************************************************************
#define PRAM_SIZE (4*1024)


//****************************************************************************
// types
//****************************************************************************

///< address or offset in graph
typedef uint32_t SDI_GraphAddr_t;

///< host pointer value
typedef uint64_t SDI_HostAddr_t;

//
// preliminary class sdi declarations
class sdi;
class sdi_grabTask;
class sdi_grabber;
class sdi_graph;

/***************************************************************************/
/** Class encapsulating sensor setup.
 *
 ***************************************************************************/
class sdi_input
{
  SDI_SENSOR mSensorSrc;  ///< which sensor is this instance bound to
  // it is important to note, there is no mutex in sdi_input - the instances are shared - wont work
  //OAL_SEMAPHORE mMutex;  ///< semaphore (implemented as mutex) to be thread-safe
  
  // TODO: move bool reserved array to shared mem and index it by sensory type ID
  bool mReserved;         ///< is this input reserved? 

  friend class sdi;

public:
  /***************************************************************************/
  /** Constructor.
   *
   * Sets members to default values.
   ***************************************************************************/
  sdi_input();

  /***************************************************************************/
  /** Set the specified sensor parameter to the given value.
   *
   * \param aParamId  sensor parameter identification
   * \param apValue   pointer to sensor parameter value (might be a structure with all parameters)
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT ParameterSet(SDI_IO_PARAM aParamId, void *apValue);

  /***************************************************************************/
  /** Gets the specified parameter from the sequencer.
   *
   * \param aParamId    sensor parameter identification
   * \param apValue     pointer to structure where to store parameter values
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT ParameterGet(SDI_IO_PARAM aParamId, void *apValue);

  /***************************************************************************/
  /** Activates specified preprocessing step in sensor.
   *
   * \ TODO: remove
   * 
   * \param aProcessId  defines preprocessing step to be activated on sensor
   * \param apValue     pointer to preprocessing parameter structure
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   *
   * \note probably wont be needed->ParameterSet should cover this for smart cameras...
   ***************************************************************************/
  // LIB_RESULT ProcessingSet(SDI_IO_PROCESS aProcessId, void *apValue);

  /***************************************************************************/
  /** Destructor.
   *
   ***************************************************************************/
  ~sdi_input();
  
private:

};

/***************************************************************************/
/** Class describing the processing in ISP/Sequencer vision subsystem.
 *
 ***************************************************************************/

class sdi_process
{
  ///< now just one graph at the time 
  ///< in the future more merged graphs might run in parallel
  sdi_graph *mpGraph;

  friend class sdi;
  friend class sdi_grabTask;

public:

  /***************************************************************************/
  /** Constructor.
   *
   * Default values assigned to members.
   *
   ***************************************************************************/
  sdi_process();

  /***************************************************************************/
  /** Sets processing definition in a form of a Sequencer graph.
   *
   * \param apGraph         pointer to graph's head array
   * \param apGraphMetadata graph metadata structure
   * 
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   * 
   * \ TODO: for more than one graph support, change LIB_RESULT to graph handle.
   ***************************************************************************/
  LIB_RESULT Set(SEQ_Head_Ptr_t *apGraph, GraphMetadata_t *apGraphMetadata);
  
  /***************************************************************************/
  /** Sets processing definition according to specified ID of predefined algorithm.
   *
   * \param aProcId    sensor parameter identification
   * 
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   * 
   * \ TODO: for more than one graph support, change LIB_RESULT to graph handle.
   ***************************************************************************/
  //LIB_RESULT Set(SDI_PROC_ID aProcId);
  
  /***************************************************************************/
  /** Loads current state of the Sequencer graph from ISP subsystem.
   *
   * \param appGraph[out] pointer to fetched Sequencer graph array
   * 
   * \return pointer to array of sequencer graph buffers if all ok,
   *         otherwise NULL
   ***************************************************************************/
  SEQ_Buf_t** GraphFetch(SEQ_Head_Ptr_t** appGraph);

  /***************************************************************************/
  /** Resets the object.
   * Removes all set up so far.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT Reset(void);
  
  /***************************************************************************/
  /** Finalizes processing management.
   * 
   * Invokes graph's Finalize() method.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT Finalize(void);
  
  /***************************************************************************/
  /** Reverts changes done by Finalize.
   * 
   * Invokes graph's Free() method.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT Free(void);
  
  /***************************************************************************/
  /** Invokes graph download.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT Download(void);
  
  /***************************************************************************/
  /** Generates and fills in IO objects based on the graph nodes.
   *
   * \param apIOs    array of sdi_input pointers to be filled in
   *
   * \return 	LIB_SUCCESS if all ok, 
   * 		LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT IOsGet(sdi_io *apIOs[]);
  
  /***************************************************************************/
  /** Gets descriptor of the SRAM buffer corresponding to the provided name.
   *
   * \param acpName[in]     name of the buffer
   * \param arSramAddr[out] Buffer descriptor structure to be filled in
   *
   * \return  LIB_SUCCESS if all ok, 
   *          LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT SramBufferGet(const char *acpName, 
                           SEQ_Buf_t &arSramBuf) const;

  /***************************************************************************/
  /** Destructor.
   *
   ***************************************************************************/
  ~sdi_process();
private:
  
  /***************************************************************************/
  /** Generates an IO object according to its type index.
   *
   * \param aIdx  IO object type index
   *
   * \return 	address of the new object,
   * 		NULL otherwise
   ***************************************************************************/
  sdi_io* IOcreate(SEQ_Othr_ix_t aIdx);

};
// sdi_process class

/***************************************************************************/
/** Structure for grabber setup storage.
 *
 ***************************************************************************/
struct sdi_grabberContext
{
  sdi_input *mInput;              ///< reference to sensor setup
  sdi_process *mProcess;          ///< reference to sequencer setup
  uint8_t **mBufferArr;           ///< array of buffers for grabbed data
  uint32_t mBufCnt;               ///< count of buffers in array
  uint32_t mNextBuffIdx;          ///< index of next to be captured buffer
  uint32_t mFramesToBeCaptured;   ///< how many frames should be captured (0 = stream/continuous)
  uint32_t mFramesCaptured;       ///< number of frames captured so far
  void *mCallBackParam;           ///< parameter pointer for callback function
  void (*mCallBackFcn)(void*);    ///< pointer to callback function

  /***************************************************************************/
  /** Sets default context values.
   *
   ***************************************************************************/
  sdi_grabberContext();

};
// sdi_grabberContext

/***************************************************************************/
/** Class responsible for grabbing images from sensors through sequencer.
 *
 * Runs in separate task (thread). When frame is ready, registered callback
 * function is executed.
 *
 ***************************************************************************/
class sdi_grabTask//: public OAL_Task
{
  friend class sdi_grabber;
	
  sdi_grabber *mpGrabber;         ///< to which grabber does this task belong to
  sdi_grabberContext mGC;         ///< grabber setup imprint
  enum tStatus {OFF, READY, ON};
  tStatus mStatus;
  LIB_RESULT mResult;                 ///< result of task execution
public:

  /***************************************************************************/
  /** Parametric constructor.
   * \param apGrabber   pointer to the grabber instance this task works for.
   ***************************************************************************/
  sdi_grabTask(sdi_grabber *apGrabber);

  /***************************************************************************/
  /** Sets specified function to be called when frame is ready.
   *
   * Allows call-back function parameter specification.
   *
   * \param apFrameReadyCallBack        frame-ready call-back function to be installed
   * \param apFrameReadyCallBackParam   frame-ready call-back function parameter
   ***************************************************************************/
  void FrameReadyCallBackInstall(void (*apFrameReadyCallBack)(void*),
                                 void* apFrameReadyCallBackParam);

  /***************************************************************************/
  /** Prepares all data and spawns the task.
   *
   * Checks all necessary prerequisites.
   ***************************************************************************/
  LIB_RESULT TaskPrepare();

  /***************************************************************************/
  /** Destructor.
   *
   * Has to be here. OAL_Task has virtual destructor.
   *
   ***************************************************************************/
  ~sdi_grabTask();
private:

  /***************************************************************************/
  /** Task pre-spawn setup.
   *
   * Sets grabbing state to on.
   ***************************************************************************/
  //void TaskOpen();
  /***************************************************************************/
  /** Implements tasks main functionality.
   *
   * Manages grabbing procedure. Accesses associated grabber, input & process objects.
   *
   ***************************************************************************/
  void TaskService();

  /***************************************************************************/
  /** Finalizes task object after main service has finished.
   *
   * Cleans up resources.
   ***************************************************************************/
  void TaskClose();

	
  LIB_RESULT AllDone(bool &arDone);
};
// sdi_grabTask

/***************************************************************************/
/** Encapsulates frame capture related functionality.
 *
 * Interconnects sdi_input (sensor) with sdi_process (data processing setup)
 * and memory buffer.
 *
 * Executes grabbing task.
 *
 ***************************************************************************/
class sdi_grabber: SEQ_EventHandlerClass
{//u
  friend class sdi_grabTask; ///< ensures direct access from grabTask to grabber

  bool modified;               ///< if something changed since last grab call
  OAL_SEMAPHORE mThreadMutex;  ///< thread safe access
  LIB_RESULT mGrabbingResult;  ///< did last grabbing succeed?
  sdi_grabberContext mGC;      ///< grabber setup description
  void *mpSeqEventCbParam;     ///< parameter pointer for callback function
  void (*mpSeqEventCb)(uint32_t, void*); ///< pointer to callback function

  sdi_grabTask mGrabTask;      ///< thread for managing actual grabbing of images
  int8_t *mName;               ///< name of the grabber for GrabTask distinguishing
  
  // new version
  sdi_io *mpIOs[SEQ_OTHRIX_LAST]; ///< array of pointers to IO objects 
  sdi_process mProcess;           ///< manages graph data
  enum tStatus {OFF = 0, READY = 1, ON = 2};
  volatile uint32_t mStatus;      ///< current grabber status

public:

  /***************************************************************************/
  /** Simple constructor.
   *
   ***************************************************************************/
  sdi_grabber();
  
#if 0
  /***************************************************************************/
  /** Sets a parameter to an IO object.
   *
   * \param aInputIdx	which object to change
   * \param aParamId	what parameter to set
   * \param apValue 	what value to set
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT ParamSet(SEQ_Othr_ix_t aIoIdx, SDI_IO_PARAM aParamId, void *apValue);
  
  /***************************************************************************/
  /** Gets a parameter from an IO object.
   *
   * \param aInputIdx	which object to use
   * \param aParamId	what parameter to get
   * \param apValue 	where to store the value
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT ParamGet(SEQ_Othr_ix_t aIoIdx, SDI_IO_PARAM aParamId, void *apValue);
#endif // if 0

  /***************************************************************************/
  /** Sets processing definition in a form of a Sequencer graph.
   *
   * \param apGraph          Sequencer graph definged by pointer to array of head
   * \param apGraphMetadata  Graph metadata
   * 
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT ProcessSet(SEQ_Head_Ptr_t *apGraph, GraphMetadata_t *apGraphMetadata);
  
  /***************************************************************************/
  /** Sets processing definition according to specified ID of predefined algorithm.
   *
   * \param aProcId    sensor parameter identification
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   * 
   * \ TODO: for more than one graph support, change LIB_RESULT to graph handle.
   ***************************************************************************/
  //LIB_RESULT ProcessSet(SDI_PROC_ID aProcId);
  
  /***************************************************************************/
  /** Loads current state of the Sequencer graph from ISP subsystem.
   *
   * \param appGraph[out] pointer to fetched Sequencer graph array
   * 
   * \return pointer to array of sequencer graph buffers if all ok,
   *         otherwise NULL
   ***************************************************************************/
  SEQ_Buf_t** GraphFetch(SEQ_Head_Ptr_t** appGraph);
  
  /***************************************************************************/
  /** Gets a pointer to the requested IO.
   *
   * \param aIoId identification of the IO kind
   * 
   * \return  pointer to the requested IO object
   *          NULL if the IO is not included in the graph
   ***************************************************************************/
  sdi_io* IoGet(SEQ_Othr_ix_t aIoId) const;
  
  /***************************************************************************/
  /** Gets descriptor of the SRAM buffer corresponding to the provided name.
   *
   * \param acpName[in]     name of the buffer
   * \param arSramAddr[out] Buffer descriptor structure to be filled in
   *
   * \return  LIB_SUCCESS if all ok, 
   *          LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT SramBufferGet(const char *acpName, 
                           SEQ_Buf_t &arSramBuf) const;

  /***************************************************************************/
  /** Sets specified function to be called when frame is ready.
   *
   * Allows call-back function parameter specification.
   *
   * \param apFrameReadyCallBack        frame-ready call-back function to be installed
   * \param apFrameReadyCallBackParam   frame-ready call-back function parameter
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT FrameReadyCallBackInstall(void (*apFrameReadyCallBack)(void*),
                                       void* apFrameReadyCallBackParam);
  
  /***************************************************************************/
  /** Sets specified function to be called when frame is ready.
   *
   * Allows call-back function parameter specification.
   *
   * \param apSeqEventCallBack        frame-ready call-back function to be installed
   * \param apSeqEventCallBackParam   frame-ready call-back function parameter
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT SeqEventCallBackInstall(
                          void (*apSeqEventCallBack)(uint32_t, void*),
                          void* apSeqEventCallBackParam);

  /***************************************************************************/
  /** Prepares the grabbing sequence to be executed.
   * 
   * Reserves resources (HW, mem, FDMA channels), applies parameters, prepares 
   * grabbing thread, downloads the graph. 
   *
   * \return LIB_SUCCESS if all ok
   * SDI_GRABBER_NOT_READY if grabber not correctly initialized
   * LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT PreStart(void);
  
  /***************************************************************************/
  /** Release all allocated resources.
   * 
   * (HW, mem, FDMA channels)
   *
   * \return LIB_SUCCESS if all ok
   * SDI_GRABBER_NOT_READY if grabber not correctly initialized
   * LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT Release(void);
  
  /***************************************************************************/
  /** Begins grabbing of required number of images.
   *
   * \param aFrameCnt number of frames that should be captured from sensor
   * consequently. If 0 is passed for the first time continuous grabbing will be
   * initiated. Passing specific number (0-n) when continuous grabbing is running
   * will stop the capture sequence after next n frames.
   * 
   * \param aIpnutLines number of lines per input frame
   * If 0 is passed the number of lines will be fetched from CSI graph node.
   *
   * \return LIB_SUCCESS if all ok
   * SDI_GRABBER_NOT_READY if grabber not correctly initialized
   * LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT Start(uint32_t aFrameCnt = 0, uint32_t aInputLines = 0);

  /***************************************************************************/
  /** Forces grabbing to stop after specified amount of captured frames from now.
   *
   * \param aFrameCnt number of frames that should be captured from nowon  before
   * grabbing is stopped. If 0 is passed for the grabbing will terminate
   * after current image has been captured.
   *
   * \return LIB_SUCCESS if all ok
   * SDI_GRABBER_NOT_READY if grabber not correctly initialized
   * LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT Stop(uint32_t aFrameCnt = 0);
  
  /***************************************************************************/
  /** Pops finished frame from specified channel.
   *
   * \param aChnlIdx index of the channel to pop from
   *
   * \return frame with valid data pointer if all ok
   *         frame with data pointer equal to NULL if failed
   ***************************************************************************/
  SDI_Frame FrameQuery(uint32_t aChnlIdx);
  
  /***************************************************************************/
  /** Pops finished frame from specified channel.
   *
   * Blocks until pop successfull or timeout reached.
   * 
   * \param aChnlIdx   index of the channel to pop from
   * \param aTimeoutMs number of miliseconds till timeout (0 => no timeout)
   *
   * \return frame with valid data pointer if all ok
   *         frame with data pointer equal to NULL if failed
   ***************************************************************************/
  SDI_Frame FramePop(uint32_t aChnlIdx = 0, uint32_t aTimeoutMs = 1000);
  
  /***************************************************************************/
  /** Pops finished frame from specified channel.
   *
   * Does only one pop attempt and returns.
   *  
   * \param aChnlIdx index of the channel to pop from
   *
   * \return frame with valid data pointer if all ok
   *         frame with data pointer equal to NULL if failed
   ***************************************************************************/
  
  SDI_Frame FramePopNonBlock(uint32_t aChnlIdx);
  
  /***************************************************************************/
  /** Pushes a specified frame to grabber.
   * 
   * \param arFrame index of the channel to pop from
   *
   * \return LIB_SUCCESS if all ok
   *         LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT FramePush(SDI_Frame &arFrame);

  /***************************************************************************/
  /** Finds out if grabbing sequence is currently in progress.
   *
   * \return true if grabbing in progress, otherwise false.
   ***************************************************************************/
  bool IsLive(void);
  
  /***************************************************************************/
  /** Sequencer event handler.
   * Called as a response to SEQ driver signal being send to current process.
   * 
   * \param aEventType type of the event
   ***************************************************************************/
  void SeqEventHandler(uint32_t aEventType);
  
  /***************************************************************************/
  /** Destructor
   ***************************************************************************/
  virtual ~sdi_grabber();

private:
  
  /***************************************************************************/
  /** Releases all defined IO objects.
   *
   * \return LIB_SUCCESS if all ok
   * 	     LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT IOsRelease(void);
  
  /***************************************************************************/
  /** Reserves all defined IO objects.
   *
   * \return LIB_SUCCESS if all ok
   * 	     LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT IOsReserve(void);
  
  /***************************************************************************/
  /** Applies parameters to all IO objects.
   *
   * \return LIB_SUCCESS if all ok
   * 	     LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT IOsSetup(void); 
  
  /***************************************************************************/
  /** Starts all IO objects.
   *
   * \return LIB_SUCCESS if all ok
   *       LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT IOsStart(void); 
  
  /***************************************************************************/
  /** Stops parameters to all IO objects.
   *
   * \return LIB_SUCCESS if all ok
   *       LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT IOsStop(void); 

}; // sdi_grabber

class sdi_mutex
{
  OAL_SEMAPHORE mMutex;
public:
  /***************************************************************************/
  /** Contructor.
   *
   * Calls Oal_SemaphoreCreate().
   *
   ***************************************************************************/
  sdi_mutex();
  /***************************************************************************/
  /**
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  bool IsReady();
  /***************************************************************************/
  /**
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  inline LIB_RESULT Obtain();

  /***************************************************************************/
  /**
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  inline LIB_RESULT Release();

  /***************************************************************************/
  /** Destructor.
   *
   * Calls OAL_SemaphoreDelete.
   ***************************************************************************/
  ~sdi_mutex();
};
// sdi_mutex

// sdi_grabber class
class sdi
{
  //
  // hidden infrastructure for SDI
  //
  
  friend sdi_graph;

  // local process data
  static bool mInitialized; ///< remebers local sdi state for one process (address space)

  static uint32_t mThreadCounter; ///< for counting threads that use sdi in one process (address space)
  //static vector<sdi_process*> mProcesses;       ///< vector of process instances

  //static sdi_mutex mThreadMutex;              ///< mutex to be thread safe
  static OAL_SEMAPHORE mInitSemaphore; 					///< named semaphore to be process safe during init
  static OAL_SEMAPHORE mSdiMut;    							///< named semaphore (used as mutex) for sdi internals/HW access
	
  // shared (inter-process) memory contents
  static OAL_SHARED_MEM mShm;                   ///< inter-process shared memory
  static uint32_t* mpSharedMemorySize;					///< size of the shared memory
  static uint32_t* mpInitCounter;          			///< for counting clients' references
  static uint32_t* mpSensorCnt;                 ///< available sensor count
  static uint32_t  mNextProcessId;     					///< id to be set to next reserved process
  // might be substituted with bool array only for reservation and rest stored localy
  static sdi_input *mpSensors;                  ///< array of available sensors
  
  // PRAM mapping
  static void* mPramBase;				///< base address of the userspace PRAM mapping
  
public:

  //
  // public interface for users
  //

  /***************************************************************************/
  /** Prepares data structures and initializes Sequencer communication interface.
   *
   * Finds out number of sensors from sequencer. Prepares all available input
   * instances to be reserved.
   *
   * Works with named semaphore to be process safe.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  static LIB_RESULT Initialize(uint32_t aTaskNum);

  /***************************************************************************/
  /** Closes all resources.
   *
   * Does not check if anything forgotten by user. Just releases HW connection.
   *
   * \param aTaskNum id of this task
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  static LIB_RESULT Close(uint32_t aTaskNum);

  /***************************************************************************/
  /** Tries to reserve input device (sensor) for exclusive use by the user.
   *
   * \param apSdiInputId
   * \param arpInput
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  static LIB_RESULT InputReserve(SDI_SENSOR aSdiInputId, sdi_input *&arpInput);

  /***************************************************************************/
  /** Tries to release input device (sensor).
   *
   * \param arpInput
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  static LIB_RESULT InputRelease(sdi_input *&arpInput);

  /***************************************************************************/
  /** Tries to reserve one processing channel for exclusive use by the user.
   *
   * Every process allows to have an independent set of sequencer settings and
   * sensor data processing in ISP.
   *
   * \note Does not ensure all required resources will be available to sustain
   * the required preprocessing in real-time. This depends on the complexity of
   * required preprocessing (set by future API calls), and current ISP load.
   * A runtime resources check will be done every time a preprocessing step will
   * be set/add by user (process parameter). User will be notified
   * about the outcome.
   *
   * \param arpProcess pointer to reserved process will be returned here
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  //static LIB_RESULT ProcessReserve(sdi_process *&arpProcess);

  /***************************************************************************/
  /** Tries to release process channel.
   *
   *  All set up processing will be stop => HW resources will be released and
   *  made available to further use.
   *
   * \param arpProcess which process to release
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  //static LIB_RESULT ProcessRelease(sdi_process *&arpProcess);

private:
  
  /***************************************************************************/
  /** Does the init semaphore synchronization during sdi initialization.
   *
   * \param apInitSem   temporary storage for init semaphore
   * \param apFirstProcess will be set to true if this is the first call to sdi init
   * \param apSemObtained will be set to true if init semaphore will be obtained
   * \return
   ***************************************************************************/
  static LIB_RESULT InitSemSynchronize(OAL_SEMAPHORE *apInitSem,
                                       bool *apFirstProcess,
                                       bool *apSemObtained);

  /***************************************************************************/
  /** SDI init specific to first call.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  static LIB_RESULT InitFirst(void);

  /***************************************************************************/
  /** SDI init specific to subsequent calls.
   *
   * \param arRepeat will be set to true, if SHM region does not exist
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  static LIB_RESULT InitSubseq(bool arRepeat);

  /***************************************************************************/
  /** Creates and connects shared memory area.
   *
   * \param aSensorCnt number of available sensors
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  static LIB_RESULT ShmConnect(uint32_t aSensorCnt);

  /***************************************************************************/
  /** Connects internal sdi data to opened shared memory area.
   *
   * \param arRepeat will be set to true, if SHM region does not exist
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  static LIB_RESULT ShmConnect(bool &arRepeat);

  /***************************************************************************/
  /** Opens an existing shared memory.
   *
   * \param apShmSize shared memory size
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  static LIB_RESULT ExistingShmOpen(uint32_t *apShmSize);

  /***************************************************************************/
  /** Closes/destroys shared memory and its content.
   *
   * \param aDelete if true, Shared memory gets destroyed
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  static LIB_RESULT ShmDisconnect(bool aDelete);

  //
  // private interface for inputs/processes/grabbers to access the sequencer
  //

  /***************************************************************************/
  /** Queries sequencer HW for number of available sensor devices.
   *
   * \param raSensorCnt
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  static LIB_RESULT SensorCntGet(uint32_t &raSensorCnt);

  /***************************************************************************/
  /** Initializes hardware (sensors).
   *
   * Works through sequencer. Requires InitSemaphore to be obtained in advance.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  static LIB_RESULT HwInit(void);

  /***************************************************************************/
  /** Closes all initialized hardware.
   *
   * Works through sequencer. Requires InitSemaphore to be obtained in advance.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  static LIB_RESULT HwClose(void);

public:  
  /***************************************************************************/
  /** Loads given graph to PRAM area.
   *
   * \param apGraph[] pointer to ISP graph array
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  static LIB_RESULT SeqGraphDownload(SEQ_Head_t* apGraph[]);
  
  /***************************************************************************/
  /** Downloads the Sequencer firmware to CRAM.
   *
   * \param apFwPath path to the firmware file
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  static LIB_RESULT SeqFwDownload(char *apFwPath);

  // todo: will be implemented when more sequencer info will be available.
};
// sdi class

#endif /* SDI_HPP */
