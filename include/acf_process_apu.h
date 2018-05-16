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

#ifndef ACFPROCESSAPU_H
#define ACFPROCESSAPU_H

#include <acf_process.h>
#include <../libs/apex/acf/include/acf_process_portinfo_apu.h>
#include <icp_data.h>
#include <icp_datatype.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <oal.h>

#define ACF_DEBUG_HW_REG          0
#define ACF_DEBUG_DUMP            0
#define ACF_HOST_PROFILING_LVL    1 //0=off, 1=basic, 2=detailed
#define ACF_ENABLE_HW_PROFILING   0
#define ACF_DEBUG_CMEM            0

#if (ACF_DEBUG_HW_REG == 1 || ACF_DEBUG_DUMP == 1)
//forward declarations
struct _APEX_642CL_MCDMA_TAG;
struct _APEX_642CL_STREAM_DMA_TAG;
struct _APEX_642CL_SEQ_TAG;
struct _APEX_642CL_MOTION_COMP_TAG;
#endif

/// \addtogroup acf_apu_cfg ACF_APU_CFG
/// @{
/// ACF_APU_CFG - the various APU configurations a process may be run on
typedef enum _ACF_APU_CFG
{
   ACF_APU_CFG__DEFAULT,                 ///< APU0 with all CUs and all SMEM
   ACF_APU_CFG__APU_0_CU_0_63_SMEM_0_3,  ///< APU0 with CUs 0-63 and 128K SMEM
   ACF_APU_CFG__APU_0_CU_0_31_SMEM_0_1,  ///< APU0 with CUs 0-31 and 64K  SMEM
   ACF_APU_CFG__APU_1_CU_32_63_SMEM_2_3  ///< APU1 with CUs 32-63 and 64K  SMEM
} ACF_APU_CFG;
/// @}

typedef enum _ACF_WAIT_MODE
{
   ACF_WAIT_MODE_INTERRUPT = 0,
   ACF_WAIT_MODE_POLLING
} ACF_WAIT_MODE;

typedef enum _ACF_SCENARIO_SELECTION_BIAS
{
   ACF_SCENARIO_SELECTION_BIAS_BALANCED,
   ACF_SCENARIO_SELECTION_BIAS_PROCESSING_LIM,
   ACF_SCENARIO_SELECTION_BIAS_BANDWIDTH_LIM
} ACF_SCENARIO_SELECTION_BIAS;

typedef enum _ACF_SCENARIO_SELECTION_BUFFER_POLICY
{
   //scenario selection must respect known buffer boundaries (i.e. selected chunk size must divide nicely
   //into IO sizes)
   ACF_SCENARIO_SELECTION_BUFFER_POLICY_STRICT,
   //scenario selection will assume buffers have been allocated with overflow space, and take advantage of this
   //to enable the selection of a scenario with the best array processing and bandwidth performance (i.e. selected
   //chunk size may not divide nicely into IO sizes)
   ACF_SCENARIO_SELECTION_BUFFER_POLICY_RELAXED
} ACF_SCENARIO_SELECTION_BUFFER_POLICY;

//TODO: these types are COPIED FROM apu_hal.h.  apu_hal.h should not be included here - some cleanup is required.
#ifndef SEG_HOST_TYPE
typedef int64_t  SEG_HOST_TYPE;
typedef uint32_t SEG_MEM_TYPE;
#endif
typedef SEG_HOST_TYPE LOAD_SEGMENT_t[4];

//////////////////////////////////////////////////////////////////////////
/// AcfProfilingInfo is a struct containing acf profiling
/// information
//////////////////////////////////////////////////////////////////////////
typedef struct _AcfProfilingInfo {
   int32_t host_start;     ///< host time (us) spent from the beginning of start() to the triggering of process execution on the APEX (this is 100% host overhead and includes apu loading, resource acquisition, etc.)
   int32_t host_wait;      ///< host time (us) spent in the wait() call (this includes time spent waiting for the process execution to complete, plus a small amount of overhead)
   int32_t apu_total;      ///< total apu time (us) (includes init, processing, idle, and all other overhead)
   int32_t apu_init;       ///< apu time (us) spent on initialization
   int32_t apu_processing; ///< apu time (us) spent on kernel execution + padding + circular buffer management
   int32_t apu_idle;       ///< apu time (us) spent waiting for data transfers to complete (if this is large, the process is likely bandwidth limited)
   int32_t apu_misc;       ///< apu time (us) spent on misc. overhead (control flow, descriptor management, etc.)
} AcfProfilingInfo;

//////////////////////////////////////////////////////////////////////////
/// ACF_Process_APU is the base class from which an APU process
/// is derived.  It provides access to all the methods required for
/// run-time configuration and execution of an APU process.
//////////////////////////////////////////////////////////////////////////
class ACF_Process_APU : public ACF_Process
{
public:
   
   ACF_Process_APU();
   ACF_Process_APU(int apexID);
   virtual ~ACF_Process_APU();

   ////////////////////////////////////////////////////////////////////////
   /// Initialize the APU process.  This must be invoked prior
   /// to any configuration or execution calls.
   ///
   /// \return
   /// 0 if successful, non-zero if an error occurred.
   /////////////////////////////////////////////////////////////////////////
   virtual int32_t Initialize() = 0;

   ////////////////////////////////////////////////////////////////////////
   /// Launch the process.  This is a non-blocking call, and must (eventually)
   /// be paired with a Wait() call.
   ///
   /// \return
   /// 0 if successful, non-zero if an error occurred.
   /////////////////////////////////////////////////////////////////////////
   virtual int32_t Start();

   /////////////////////////////////////////////////////////////////////////
   /// Wait for a launched process to complete.
   ///
   /// \return
   /// 0 if successful, non-zero if an error occurred.
   /////////////////////////////////////////////////////////////////////////
   virtual int32_t Wait();

   /////////////////////////////////////////////////////////////////////////
   /// This method is used to force a specific scenario to be selected.  A successful call to SelectScenario(...)
   /// will override the scenario selection that typically takes place when Start() is called. If this function
   /// is called, it is assumed that the user is in charge of explicit scenario selection for the duration of the
   /// object's lifespan.  The following examples demonstrate how it can be used:
   ///
   /// \code{.cpp}
   /// //Usage example 1: for port "myPort", select the scenario where the chunk width is 8 and the chunk height is 4
   /// A: SelectScenario("myPort", 8, 4);
   /// \endcode
   ///
   /// \code{.cpp}
   /// //Usage example 2: for port "myPort", select the scenario where the chunk width is 16 and the chunk height is optimal (i.e. the largest available)
   /// A: SelectScenario("myPort", 16, 0); //'0' indicates that the choice should be left to ACF
   /// \endcode
   ///
   /// \code{.cpp}
   /// //Usage example 3: for port "myPort", select the scenario where the chunk height is 8, and the chunk width is optimal (i.e. chosen to maximize CU utilization)
   /// A: SelectScenario("myPort", 0, 8); //'0' indicates that the choice should be left to ACF
   /// \endcode
   ///
   /// \code{.cpp}
   /// //Usage example 4: select the 'ideal' scenario (i.e. first choose chunk width to maximize CU utilization, then choose the largest corresponding chunk height)
   /// SelectScenario("", 0, 0); //'0' indicates that the choice should be left to ACF
   /// \endcode
   ///
   /// \param[in] lPortIdentifier
   /// Graph port identifier.
   ///
   /// \param[in] lChunkWidth
   /// Desired chunk width associated with port "lPortIdentifier" (or '0' if the choice should be left to ACF)
   ///
   /// \param[in] lChunkHeight
   /// Desired chunk height associated with port "lPortIdentifier" (or '0' if the choice should be left to ACF)
   ///
   /// \return
   /// 0 if successful, non-zero if an an error occurred or if the desired
   /// scenario could not be found
   /////////////////////////////////////////////////////////////////////////
   int32_t SelectScenario(std::string lPortIdentifier, int32_t lChunkWidth, int32_t lChunkHeight);

   /////////////////////////////////////////////////////////////////////////
   /// Return the the chunk width and height associated with port "lPortIdentifier".
   /// It is only meaningful to call this method after a successful call to SelectScenario(...).
   ///
   /// \param[in] lPortIdentifier
   /// Graph port identifier.
   ///
   /// \param[out] lChunkWidth
   /// Chunk width associated with port "lPortIdentifier"
   ///
   /// \param[out] lChunkHeight
   /// Chunk height associated with port "lPortIdentifier"
   ///
   /// \return
   /// 0 if successful, non-zero if an an error occurred.
   /////////////////////////////////////////////////////////////////////////
   int32_t QueryPortChunkSize(std::string lPortIdentifier, int32_t& lChunkWidth, int32_t& lChunkHeight);

   /////////////////////////////////////////////////////////////////////////
   /// Select a specific APU configuration and a specific APEX on which to
   /// execute the process.  This method allows for multiple processes to be
   /// executed simultaneously on the same APEX (assuming HW resource availability).
   ///
   /// For example, given a single 642 APEX configuration, run myProcessA
   /// on APU0 w/ 32 CUs and run myProcessB on APU1 w/ 32 CUs:
   /// \code{.cpp}
   /// if (0 == myProcessA.SelectApuConfiguration(ACF_APU_CFG__APU_0_CU_0_31_SMEM_0_1, 0) &&
   ///     0 == myProcessB.SelectApuConfiguration(ACF_APU_CFG__APU_1_CU_32_63_SMEM_2_3, 0))
   /// {
   ///    lRetVal |= myProcessA.Start();
   ///    lRetVal |= myProcessB.Start();
   ///
   ///    lRetVal |= myProcessA.Wait();
   ///    lRetVal |= myProcessB.Wait();
   /// }
   /// \endcode
   ///
   /// \param[in] lApuConfig
   /// Desired APU configuration (see definition of ACF_APU_CFG for available options)
   ///
   /// \param[in] lApexId
   /// The ID of the desired APEX (e.g if there are 2 APEXs, valid
   /// values for lApexId would be 0 and 1).
   ///
   /// \return
   /// 0 if successful, non-zero if an an error occurred.
   /////////////////////////////////////////////////////////////////////////
   int32_t SelectApuConfiguration(ACF_APU_CFG lApuConfig, int32_t lApexId);

   /////////////////////////////////////////////////////////////////////////
   /// Return profiling information associated with the last process
   /// execution.  It should be called only after Start() and Wait() have
   /// completed.
   ///
   /// \code{.cpp}
   /// myProcess.Start();
   /// myProcess.Wait();
   /// AcfProfilingInfo profInfo = myProcess.RetAcfProfilingInfo();
   /// \endcode
   ///
   /// \return
   /// Returns an AcfProfilingInfo struct populated with the results of the last
   /// process execution.
   /////////////////////////////////////////////////////////////////////////
   AcfProfilingInfo RetAcfProfilingInfo();

   ////////////////////////////////////////////////////////////////////////
   /// Launch the process with a user specified callback.  The callback "lpCallback" will
   /// be invoked with the parameters defined by "lpCallbackParam" and "lpCallbackRetVal"
   /// when process execution has completed.  This is a non-blocking call, and it must
   /// (eventually) be paired with a Wait() call.  The following code fragment
   /// illustrates a simple callback example:
   ///
   /// \code{.cpp}
   /// void MyCallback(void* lpParam, int32_t* lpRetVal)
   /// {
   ///    int32_t lRetVal = 0;
   ///    MyStruct* lpMyStruct = (MyStruct*)lpParam;
   ///
   ///    //<do something>
   ///
   ///    if (0 != lpRetVal)
   ///       *lpRetVal = lRetVal;
   /// }
   ///
   /// void StartWithCallbackExample()
   /// {
   ///    MyProcess lMyProcess;
   ///    lMyProcess.Initialize();
   ///
   ///    //<connect IOs to process>
   ///
   ///    MyStruct lMyStruct;
   ///    int32_t  lMyRetval;
   ///    lMyProcess.Start(MyCallback, (void*)&lMyStruct, &lMyRetval);
   ///    lMyProcess.Wait();
   /// }
   /// \endcode
   ///
   /// \param[in] lpCallback
   /// Callback function that will be invoked upon process completion.
   ///
   /// \param[in] lpCallbackParam
   ///  Pointer to callback parameter (e.g. pointer to a value, struct, array, etc.).
   ///  Can be 0 if the callback doesn't use it.
   ///
   /// \param[in] lpCallbackRetVal
   ///  Pointer to callback return value.  Can be 0 if the callback doesn't use it.
   ///  Please note that ACF will not examine or draw any conclusions based on the value
   ///  of *lpCallbackRetVal (i.e. it is for use by the user).
   ///
   /// \return
   /// 0 if successful, non-zero if an error occurred.
   /////////////////////////////////////////////////////////////////////////
   int32_t Start(void (*lpCallback)(void* lpParam, int32_t* lpRetVal),
                 void* lpCallbackParam,
                 int32_t* lpCallbackRetVal);

   //--------------------------------------------------------------------
   //The following public methods are not 'officially' documented/exposed
   //--------------------------------------------------------------------

   //NOTE: this function should only be used to override the chunk size of ports that are  fixed; if you
   //      intend to override a port that is not fixed, you are responsible for overriding the sizes of any
   //      related ports to maintain consistency.
   int32_t OverridePortChunkSize(std::string lPortIdentifier, int32_t lChunkWidth, int32_t lChunkHeight);

   int32_t RetCuArrayWidth();
   int32_t RetCmemHeapSizeInBytes();
   acf_scenario* RetSelectedScenario();
  
   int32_t Start_Setup();
   int32_t Start_Kick();

   int32_t InitDoneInterrupt(void (*pCustomHandler) (void));

   void DumpHostProfilingInfoToStdout();
   void DumpApuProfilingInfoToStdout();
   void DumpHostProfilingInfoToStdout_CSV(int32_t lHeaderFlag);
   void DumpApuProfilingInfoToStdout_CSV(int32_t lHeaderFlag);
   
   void FlushAndInvalidateAllIO();

   //TODO: don't want these to be public, but they need to be visible to the callback...
   int32_t CleanupAndResourceRelease();
   int32_t ClearCallback();
   int32_t ObtainWaitSync();
   int32_t ReleaseWaitSync();
   int32_t CallUserDefinedCallback();

   void CfgScenarioSelectionBufferPolicy(ACF_SCENARIO_SELECTION_BUFFER_POLICY lBufferPolicy);
   void CfgScenarioSelectionBias(ACF_SCENARIO_SELECTION_BIAS lBias);
	
protected:

   void ConstructorInitialize();
   int32_t DetectHwVersion();
   int32_t DetectAvailApexMemory();
   int32_t DetWorstCaseRoiInfo();

   int32_t ApexInitSanityCheck();
   void SetApuLoadInfo(const LOAD_SEGMENT_t* lLoadSegments,
                       const SEG_MEM_TYPE* lpLoadPmem, const int32_t lLoadPmemSizeInWords,
                       const SEG_MEM_TYPE* lpLoadDmem, const int32_t lLoadDmemSizeInBytes,
                       const SEG_MEM_TYPE* lpLoadCmem, const int32_t lLoadCmemSize);

   int32_t NonParametricInitialization();
   int32_t CopyApuLoad();
   int32_t InitKernelManager();

   void AddPort(std::string lPortIdentifier,
                icp::DATATYPE lE0DataType,
                int32_t lE0x, int32_t lE0y,
                int32_t lChunkWidth, int32_t lChunkHeight,
                int32_t lInputOutput,
                int32_t lCmemDmem,
                int32_t lNonstaticStatic,
                int32_t lDirectIndirect,
                int32_t lNonfixedFixed,
                int32_t lRegFifo,
                int32_t lLmIndex,
                int32_t lSDInfoN,
                int32_t lSDInfoS,
                int32_t lSDInfoE,
                int32_t lSDInfoW,
                int32_t lVerDepOverlapFlag);

   int32_t SelectScenario_internal(std::string lPortIdentifier, int32_t lChunkWidth, int32_t lChunkHeight);

   std::vector<ACF_Process_PortInfo_APU> mPortInfoApuVec;

   int32_t mApexId;       //which APEX instance to use
   ACF_APU_CFG mApuCfg;   //desired APU configuration
   int32_t mApuId;        //which APU to use
   int32_t mCuArrayWidth; //number of CUs in the CU array
   int32_t mHwVersion;    //0x321, 0x642, etc.
   ACF_WAIT_MODE mWaitMode;

   //heap address/size info
   int32_t mCmemHeapBaseAddrOffset; //CMEM 'heap' offset (relative to base CMEM address)
   int32_t mCmemHeapSizeInBytes;    //CMEM 'heap' size
   int32_t mSmemBaseAddrOffset;     //SMEM offset (relative to start of 128K SMEM region associated with the APEX)
   int32_t mPmemBaseAddrOffset;     //PMEM region offset (relative to mSmemBaseAddrOffset)
   int32_t mPmemSizeInBytes;        //PMEM size
   int32_t mDmemBaseAddrOffset;     //DMEM region offset (relative to mSmemBaseAddrOffset)
   int32_t mDmemSizeInBytes;        //DMEM size
   int32_t mDmemHeapBaseAddrOffset; //DMEM 'heap' offset (relative to mDmemBaseAddrOffset)
   int32_t mDmemHeapSizeInBytes;    //DMEM 'heap' size
   
   const LOAD_SEGMENT_t* mLoadSegments;
   const SEG_MEM_TYPE* mpLoadPmem;
   int32_t mLoadPmemSize;
   const SEG_MEM_TYPE* mpLoadDmem;
   int32_t mLoadDmemSize;
   const SEG_MEM_TYPE* mpLoadCmem;
   int32_t mLoadCmemSize;

   void* mpPmemLoadOal;
   void* mpDmemLoadOal;
   void* mpCmemLoadOal;
   void* mpLoadSegmentsLLOal;

   //scenario stuff
   int32_t CfgLocalScenario(acf_scenario* lpScenario);
   void CleanupLocalScenario();
   acf_scenario mSelectedScenario; //copy of the selected scenario
   acf_scenario* mpSelectedScenario;
   void CfgScenarios(acf_scenario_list* lpScenarioList);
   acf_scenario_list* mpScenarioList;
   int32_t mUserControlledScenarioFlag;
   ACF_SCENARIO_SELECTION_BUFFER_POLICY mScenarioSelectionBufferPolicy; //determines whether or not buffer boundaries are respected when selecting chunk size
   ACF_SCENARIO_SELECTION_BIAS mScenarioSelectionBias;                  //determine bias of scenario selection

   acf_res_info mAcfResInfo;
   int32_t mClover389Flag;
   int32_t mApuProgramRetVal;

   //hw resource management stuff
   int32_t AllocateHwResources();
   int32_t DeallocateHwResources();
   uint32_t mUserId;
   uint32_t mNumResReq; //currently will request 1 RES__APEX__APU, up to 2 RES__APEX__CU_32, and up to 2 RES__SMEM_64K (worst case = 5)
   uint32_t mResReqArray[5][4];
   uint32_t mResAcqArray[5][5];

   //microkernel and kernel DB related
   int32_t mApuProgFuncPtr; //function pointer associated with the acf generated program

   //apu program parameter stuff
   static const int MAX_PARAM_LIST_SIZE_IN_WORDS = 750; //must be in sync with MAX_NUM_PARAM defined in apu_microkernel_hal.h
   int32_t mProgramParams[MAX_PARAM_LIST_SIZE_IN_WORDS];
   int32_t mProgramParamSizeInWords;

   //profiling stuff
   uint64_t mProbeArray[32];
   int32_t mCycles_apu_total;
   int32_t mCycles_apu_init;
   int32_t mCycles_apu_processing;
   int32_t mCycles_apu_idle;
   int32_t mCycles_apu_kernel;
   int32_t mCycles_apu_padPlusCb;

   //interrupt stuff
   OAL_SEMAPHORE mWaitSemaphore; //for OS case
   int32_t mWaitSync;            //for bare metal (i.e. standalone) case
   void* mpCallbackParam;
   int32_t mCallbackRetVal;
   void (*mpUserDefinedCallback)(void* lpParam, int32_t* lpRetVal);
   void* mpUserDefinedCallbackParam;
   int32_t* mpUserDefinedCallbackRetVal;

   //debugging stuff
#if (ACF_DEBUG_HW_REG == 1 || ACF_DEBUG_DUMP == 1)
   int32_t SanityCheck_ProgramMem(volatile uint32_t* lpBufferToCompareWithGolden);
   int32_t SanityCheck_ProgramMem_Dma();
   int32_t SanityCheck_ProgramParams();
   void DebugDump();
   volatile uint32_t*                    mpSmem;
   volatile uint32_t*                    mpCmem;
   volatile _APEX_642CL_MCDMA_TAG*       mpDma;
   volatile _APEX_642CL_STREAM_DMA_TAG*  mpStio;
   volatile _APEX_642CL_SEQ_TAG*         mpSeq;
   volatile _APEX_642CL_MOTION_COMP_TAG* mpMotionComp;
#endif
};

#endif /* ACFPROCESSAPU_H */

