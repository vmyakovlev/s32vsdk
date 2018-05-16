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

#ifndef ACFPROCESSDESC_HPP
#define ACFPROCESSDESC_HPP

#include <acf_common.h>
#include <stdint.h>
#include <string>
#include <map>
#include <vector>

class ACF_Graph;
class ACF_Kernel;
class ACF_Port;

#define ACF_PROCESS_MAX_NUM_LOCALMEMDESC 500

class ACF_Process_Desc
{

public:

   ACF_Process_Desc();
   ~ACF_Process_Desc();

   /////////////////////////////////////////////////////////////////////////
   /// This is a pure virtual method that must be implemented by the
   /// derived class.
   /////////////////////////////////////////////////////////////////////////
   virtual void Create() = 0;

   /////////////////////////////////////////////////////////////////////////
   /// Associate the graph "lGraph" with the process and give the process a
   /// unique identifier "lProcessIdentifier".  The chosen process identifier
   /// will be used as a root name for generated output entities.
   ///
   /// eg.
   /// \code
   /// Initialize(mMyTestGraph, "MY_TEST_PROCESS");
   /// \endcode
   ///
   /// \param[in] lGraph
   /// Graph associated with the process.
   /// \param[in] lProcessIdentifier
   /// Process identifier.
   ///
   /// \return
   /// 0 if successful, non-zero if creation of "lGraph" failed.
   /////////////////////////////////////////////////////////////////////////
   int32_t Initialize (ACF_Graph& lGraph, std::string lProcessIdentifier);

   /////////////////////////////////////////////////////////////////////////
   /// Set the input chunk size (in units of e0) for port "lInputPortIdentifier"
   /// to "lChunkWidth" by "lChunkHeight".
   ///
   /// eg.
   /// \code
   /// SetInputChunkSize("GRAPH_INPUT_0", 8, 1);
   /// \endcode
   ///
   /// \param[in] lInputPortIdentifier
   /// Input port identifier.
   /// \param[in] lChunkWidth
   /// Chunk width in units of e0.
   /// \param[in] lChunkHeight
   /// Chunk height in units of e0.
   ///
   /// \return
   /// 0 if successful, non-zero if port "lInputPortIdentifier" could not be found.
   /////////////////////////////////////////////////////////////////////////
   int32_t SetInputChunkSize(std::string lInputPortIdentifier, int32_t lChunkWidth, int32_t lChunkHeight);

   /////////////////////////////////////////////////////////////////////////
   /// Indicate that the input will be a 2D table of pointers to chunks of data
   /// (instead of contiguous data).  This allows for the processing of non-contiguous
   /// chunks of data.  Use "SetInputChunkSize(...)" to select the input chunk
   /// size (all data chunks are assumed to be the same size).
   ///
   /// \param[in] lInputPortIdentifier
   /// Input port identifier.
   ///
   /// \return
   /// 0 if successful, non-zero if port "lInputPortIdentifier" could not be found.
   /////////////////////////////////////////////////////////////////////////
   int32_t FlagInputAsChunkBasedIndirect(std::string lInputPortIdentifier);

   int32_t FlagInputAsVerticalSdOverlap(std::string lInputPortIdentifier);

   //int32_t RetMaxKernelExecutionDelay();
   //int32_t RetMinKernelExecutionDelay();
   int32_t RetMaxKernelOutputDelay();
   int32_t RetErrorState();

   //methods specific to implementation
   virtual int32_t Resolve() = 0;
   virtual int32_t GenerateProcess() = 0;

protected:

   chunk_desc RetInputChunkSize(ACF_Port* lpGraphInputPort);
   chunk_desc CalcInputChunkSize(ACF_Port* lpGraphInputPort, int32_t lBaseChunkWidthInBytes, int32_t lBaseChunkHeight);
   void AddKernelToList(ACF_Kernel* lpKernel);
   void SortKernelList();
   int32_t RetLocalMemIndex(ACF_Port* lpPort);
   int32_t RetKernelIndex(ACF_Kernel* lpKernel);
   void IdentifyFwdConnectedKernelPorts(ACF_Port* lpPort, std::vector<ACF_Port*>& lPortVec);
   ACF_Kernel* RetFirstFwdConnectedKernel(ACF_Port* lpPort);

   std::string mIdentifier;

   ACF_Graph* mpGraph;

   //keep track of all the kernels present in a graph (including those in subgraphs)
   std::vector<ACF_Kernel*> mKernelPtrVec;

   int32_t mNumLocalMemDescs;
   ACF_LocalMemDesc* mLocalMemDescArray; //allocated on heap w/ new

   int32_t mNumCbManagers; //each will be mapped to a local memory descriptor as required (only relevant in OD case)


   //arrays to keep track of and map information related to local mem desc (via index)
   int32_t mLocalMemWriteDelayArray[ACF_PROCESS_MAX_NUM_LOCALMEMDESC];
   ACF_Port* mLocalMemProdPortArray[ACF_PROCESS_MAX_NUM_LOCALMEMDESC]; //keep track of producer port associated with each local mem desc

   std::map<std::string, chunk_desc> mInputToChunkSizeMap;    //map input port identifier to user-initialized chunk size
   std::map<std::string, int32_t>    mInputToIndirectFlagMap; //map input port identifier to 'indirect' flag
   std::map<std::string, int32_t>    mInputToVerDepOverlapFlagMap; //map input port identifier to 'vertical dependency overlap' flag
   std::map<ACF_Port*, int32_t>      mPortToLocalMemIndexMap; //map port to a local memory entity (via index in 'mLocalMemDescArray')

   //map in both directions between local memory and cb manager (based on index)
   std::map<int32_t, int32_t>        mLocalMemIndexToCbManagerIndexMap; //map local memory entity (via index in 'mLocalMemDescArray') to CbManager entity (via index)
   std::map<int32_t, int32_t>        mCbManagerIndexToLocalMemIndexMap; //map CbManager entity (via index) to local memory entity (via index in 'mLocalMemDescArray')

   //map each port to an 'index' into an array of kernel_io_desc
   std::map<ACF_Port*, int32_t>      mPortToKernelIoDescIndexMap;  //map port to an index into an array containing circular buffer offsets

   sd_desc                        mDepInfoMax;
   std::map<std::string, sd_desc> mInputToDepInfoMap;
   float                          mDepInfoDecCorFactorN;
   float                          mDepInfoDecCorFactorS;
   float                          mDepInfoDecCorFactorE;
   float                          mDepInfoDecCorFactorW;
   //intermediate values to keep track of during the calculation (will undergo 'ceiling')
   float mDepInfoN;
   float mDepInfoS;
   float mDepInfoE;
   float mDepInfoW;

   int32_t mSpatialDepFlag; //set to 1 if spatial dependencies are present
   int32_t mInitialized;
   int32_t mErrorState;
};

#endif /* ACFPROCESSDESC_HPP */

