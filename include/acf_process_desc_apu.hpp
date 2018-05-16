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

#ifndef ACFPROCESSDESCAPU_HPP
#define ACFPROCESSDESCAPU_HPP

#include <acf_process_desc.hpp>
#include <icp_datatype.h>
#include <stdint.h>
#include <sstream>

class ACF_Port;

//////////////////////////////////////////////////////////////////////////
/// ACF_Process_Desc_APU is a base class designed to encapsulate the configuration
/// or 'description' of a process.  It effectively links a graph with the APU
/// processor and allows for any required APU specific configuration.  In order to
/// create an APU process description, a user must derive from this class and
/// implement the pure virtual Create() method.
//////////////////////////////////////////////////////////////////////////
class ACF_Process_Desc_APU : public ACF_Process_Desc
{
public:

   ACF_Process_Desc_APU();
   ~ACF_Process_Desc_APU();

   void DisableDoubleBuffering();

   void Resolve(int32_t lBaseChunkWidth, int32_t lBaseChunkHeight);
   void ResolutionReset();
   int32_t AnalyzeScenarios();
   int32_t CheckInputChunkSizeAgainstHwLimitations(chunk_desc lChunkDesc, ACF_Port* lpPort);
   int32_t CheckOutputChunkSizeAgainstHwLimitations(ACF_Port* lpPort);
   int32_t Resolve();
   int32_t GenerateProcess();

protected:

   int32_t mSupressConsoleOutput;
   std::stringstream mLog; //'log' variable to keep track of resolution errors during scenario analysis

   chunk_desc RetKernelPortChunkSize(ACF_Port* lpPort); //takes ED into account!!!
   ACF_Port* TraceGraphPortBackToKernel(ACF_Port* lpPort);

   void WalkGraph(ACF_Port* lpPort, int32_t lChunkWidth, int32_t lChunkHeight);
   void CalculateDepInfo(ACF_Port* lpPort);
   void WalkGraphDebugOutput();
   void CalculateBaseED();
   void WalkGraphForBaseED(ACF_Port* lpPort);
   void ConfigureLocalMemDesc(ACF_Port* lpPort, int32_t lChunkWidth, int32_t lChunkHeight);

   int32_t GenerateApuProgram();
   int32_t GenerateDbFile();
   int32_t GenerateHostHeader();
   int32_t GenerateNativeKernelWrapper();
   int32_t GenerateScenarioData(std::stringstream &lOutputFile);

   int32_t mLocalMemDMEMFlagArray[ACF_PROCESS_MAX_NUM_LOCALMEMDESC];   //indicate if associated 'ACF_LocalMemDesc' describes dmem memory region

   int32_t mErrorState_WalkGraph;
   int32_t mErrorState_Resolution;

   int32_t mDoubleBufferingFlag;

   int32_t mTotalCmemRequired;
   int32_t mTotalDmemRequired;

   //variables for use in recursive resolution
   int32_t mKernelLaunchDelay;  //kernel launch delay (only relevant when filters with vertical dependencies are present)
   int32_t mKernelCascadeDepth; //each kernel is characterized by a cascade depth
   int32_t mRecursionDepth;  //for debugging purposes ...
   int32_t mChangeDetected;

   std::vector<acf_scenario> mScenarioVec;
   int32_t mScenarioBufferDataSizeInBytes;
   int32_t mScenarioKernelDataSizeInBytes;

   //**************************************************************************************
   //protected members/methods related to generation of main() for no-host-rtl-sim purposes

   /////////////////////////////////////////////////////////////////////////
   /// Initialize various parameters required by the framework to generate
   /// an RTL-SIM ready binary.
   ///
   /// \param[in] lArrayWidth
   /// Desired number of CUs in the array (must be 32 or 64)
   /// \param[in] lDmaChIn
   /// Input DMA channel (typically 0)
   /// \param[in] lDmaChOut
   /// Output DMA channel (typically 1)
   /// \param[in] lSmemAddrFromDmaPersp
   /// The address of SMEM from the DMA perspective (i.e. from the host or FPGA
   /// perspective depending on your HW setup, not the APU perspective)
   /////////////////////////////////////////////////////////////////////////
   void RtlSim_Init(int32_t lArrayWidth,
                    int32_t lDmaChIn,
                    int32_t lDmaChOut,
                    int32_t lSmemAddrFromDmaPersp);

   /////////////////////////////////////////////////////////////////////////
   /// This function is strictly used for configuring IOs for RTL-SIM binary
   /// generation purposes.  By providing a basic IO configuration, it is
   /// possible to generate a fully configured process that is ready
   /// to execute in an RTL-SIM environment.
   ///
   /// \param[in] lPortIdentifier
   /// Port identifier.
   /// \param[in] lWidth
   /// Width (in elements) of the contiguous data region.
   /// \param[in] lHeight
   /// Height (in elements) of the contiguous data region.
   /// \param[in] lSpan
   /// Span is defined as the number of bytes required to jump from one
   /// line of bytes in memory to the 'next' line of bytes in memory.
   /// Note that span must be divisible by N where
   /// N = RetIcpDataTypeSizeInBytes (lElementDataType).
   /// \param[in] lElementDataType
   /// The data type associated with an 'element' (i.e. the smallest
   /// unit of data)
   /// \param[in] lElementDimX
   /// The 'x' dimension (i.e. width) of an element in units of
   /// "lElementDataType"
   /// \param[in] lElementDimY
   /// The 'y' dimension (i.e. height) of an element in units of
   /// "lElementDataType"
   /// \param[in] lAddrPhys
   /// Physical address of the start of the contiguous data region.  This
   /// will depend on your HW setup and should correspond to general purpose
   /// or external memory.
   /////////////////////////////////////////////////////////////////////////
   void RtlSim_ConnectIO(std::string lPortIdentifier,
                         int32_t lWidth,
                         int32_t lHeight,
                         int32_t lSpan,
                         icp::DATATYPE lElementType,
                         int32_t lElementDimX,
                         int32_t lElementDimY,
                         uint32_t lAddrPhys);

   /////////////////////////////////////////////////////////////////////////
   /// This function is strictly used for configuring indirect inputs for RTL-SIM binary
   /// generation purposes.
   ///
   /// Input data region:
   /// \param[in] lPortIdentifier
   /// Port identifier.
   /// \param[in] lWidth
   /// Width (in elements) of the contiguous data region.
   /// \param[in] lHeight
   /// Height (in elements) of the contiguous data region.
   /// \param[in] lSpan
   /// Span is defined as the number of bytes required to jump from one
   /// line of bytes in memory to the 'next' line of bytes in memory.
   /// Note that span must be divisible by N where
   /// N = RetIcpDataTypeSizeInBytes (lElementDataType).
   /// \param[in] lElementDataType
   /// The data type associated with an 'element' (i.e. the smallest
   /// unit of data)
   /// \param[in] lElementDimX
   /// The 'x' dimension (i.e. width) of an element in units of
   /// "lElementDataType"
   /// \param[in] lElementDimY
   /// The 'y' dimension (i.e. height) of an element in units of
   /// "lElementDataType"
   /// \param[in] lAddrPhys
   /// Physical address of the start of the contiguous data region.  This
   /// will depend on your HW setup and should correspond to general purpose
   /// or external memory.
   ///
   /// For input offset array:
   /// \param[in] lOffsetPortIdentifier
   /// Offset Port identifier.
   /// \param[in] lOffsetWidth
   /// Width (in elements) of the contiguous data region.
   /// \param[in] lOffsetHeight
   /// Height (in elements) of the contiguous data region.
   /// \param[in] lOffsetSpan
   /// Span is defined as the number of bytes required to jump from one
   /// line of bytes in memory to the 'next' line of bytes in memory.
   /// Note that span must be divisible by N where
   /// N = RetIcpDataTypeSizeInBytes (lElementDataType).
   /// \param[in] lOffsetElementDataType
   /// The data type associated with an 'element' (i.e. the smallest
   /// unit of data)
   /// \param[in] lOffsetElementDimX
   /// The 'x' dimension (i.e. width) of an element in units of
   /// "lOffsetElementDataType"
   /// \param[in] lOffsetElementDimY
   /// The 'y' dimension (i.e. height) of an element in units of
   /// "lOffsetElementDataType"
   /// \param[in] lOffsetAddrPhys
   /// Physical address of the start of the contiguous data region.  This
   /// will depend on your HW setup and should correspond to general purpose
   /// or external memory.
   /////////////////////////////////////////////////////////////////////////
  void RtlSim_ConnectIndirectInput(std::string lPortIdentifier,
                                   int32_t lWidth,
                                   int32_t lHeight,
                                   int32_t lSpan,
                                   icp::DATATYPE lElementType,
                                   int32_t lElementDimX,
                                   int32_t lElementDimY,
                                   uint32_t lAddrPhys,
                                   int32_t lOffsetWidth,
                                   int32_t lOffsetHeight,
                                   int32_t lOffsetSpan,
                                   icp::DATATYPE lOffsetElementType,
                                   int32_t lOffsetElementDimX,
                                   int32_t lOffsetElementDimY,
                                   uint32_t lOffsetAddrPhys);

   int32_t RtlSim_GenerateMainProgram();
};

#endif /* ACFPROCESSDESCAPU_HPP */

