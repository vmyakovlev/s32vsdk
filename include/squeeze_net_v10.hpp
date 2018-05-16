/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2015-2016 Freescale Semiconductor
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

/*!*********************************************************************************
 *  @file squeeze_v10_net.hpp
 *  @brief ApexCV SqueezeNet V1.0.
 **********************************************************************************/

#ifndef SQUEEZEV10NET_HPP
#define SQUEEZEV10NET_HPP

#define APEXCV_SQUEEZENET_V10_WEIGHT_MODEL_BYTES 1248502

#include <vector>
#include <string>
#include <cstdint>
#include "layers.hpp"

#include <umat.hpp>

#define WEIGHT_BW 64

#define APEXCV_SQUEEZENET_V10_COLLECT_STATISTICS 0

//bw: block width;  ih: image height; bd: block depth;
//bh: block height; bs: block stride; ts: tile stride;

                              
namespace apexcv {

/*!*********************************************************************************
 *   \brief ApexCV SqueezeNet V1.0
 *
 *  apex::SqueezeV10Net is the Host-ACF interface for creating, initializing,
 *  executing and releasing the SqueezeV10Net on Apex.
 ***********************************************************************************/

class SqueezeV10Net {

public:

/*!*********************************************************************************
 *   \brief Default Constructor.
 *
 *  Allocate resources for the ACF processes.
 ***********************************************************************************/
   SqueezeV10Net();

/*!*********************************************************************************
 *  \brief Destructor.
 *   Release resources for the ACF processes.
 *********************************************************************************/
   ~SqueezeV10Net();

/*!*********************************************************************************
 *   \brief Initialization
 *
 *  Initializes the intermediate buffers needed for the process, and initializes the
 *  ACF processes. 
 * ***********************************************************************************/
   int Initialize(   int8_t *aWeight                      /*!< 8-bit buffer, 1449984 bytes to hold the SqueezeNet V1.0 quantized weights coefficients */,                      
                     vsdk::UMat& aInputDataMat             /*!< Input uMat with width = (227+1)*3+16, height = 227 with 2 lines on top and 3 lines on bottom zero padded */
                 );

/*!*********************************************************************************
 *  \brief Process
 *  \return Error code for the ACF execution (zero on success).
 ***********************************************************************************/
   int Process(      uint16_t* aTop5                      /*!< 16-bit buffer, 10 elments as [label index, score] x 5 to hold Top-5 classes */, 
                     int aProfile = 0                     /*!< enable/disable profiling print out. 1: enable; 0: disable */
              );

/*!*********************************************************************************
 *  \brief SelectApuConfiguration 
 *  Select APU and APEX configuration
 *  \return Error code .
 ***********************************************************************************/
  int SelectApuConfiguration(   ACF_APU_CFG aApuConfig   /*!< APU configuration */,
                                int32_t aApexId          /*!< APEX configuration */
                            );

/*!*********************************************************************************
 *  \brief SelectApexCore
 *  \return Error code for the ACF execution (zero on success).
 *
 *  Select which APEX core (0 or 1) to be selected to run the processing.
 *  This function has to be called after initialize() and can be executed per frame base.
 ***********************************************************************************/
   int SelectApexCore(int32_t aApexId /*!< The ID of the desired APEX (e.g if there are 2 APEXs, valid values for lApexId would be 0 and 1).*/
               );

/*!*********************************************************************************
 *   \brief Reconnect IO
 *
 *  Use this to reconnect the input and output buffers. This only needs to
 *  be done if the connected Input/Outputs are changed. If only the data within 
 *  then this does not need to be called.
 * ***********************************************************************************/
   int ReconnectIO(  
                     vsdk::UMat& aInputDataMat /*!< Input uMat with width = (227+1)*3+16, height = 227 with 2 lines on top and 3 lines on bottom zero padded */
                  );

/*!*********************************************************************************
 *   \brief Return Profiling informatoin 
 *
 *  pInfo == NULL: fill in rInfoSize by using mNumOfApuProcesses (should be 12 in current implementation), return -1
 *  rInfoSize < 12: return -1; 
 *  pInfo != NULL && rInfoSize >= 12: fill in profiling information into pInfo
 * ***********************************************************************************/
   int RetProfilingInfo( AcfProfilingInfo *aInfo,
                         uint32_t& aInfoSize);

#if APEXCV_SQUEEZENET_V10_COLLECT_STATISTICS
   std::vector<std::pair<int64_t, int64_t>> PruneCounter;
#endif

   std::vector<Layer<int8_t>*> layers_;             /*!< Network layer */

private:
   std::vector<Blob<int8_t>* > blobs_;
   std::vector<Blob<int8_t>* > net_input_blobs_;
   std::vector<Blob<int8_t>* > net_output_blobs_;

   void* fire2_apu_process; 
   void* fire3_apu_process; 
   void* fire4_apu_process; 
   void* fire5_apu_process; 
   void* fire6_apu_process; 
   void* fire7_apu_process; 
   void* fire8_apu_process; 
   void* fire9_1st_apu_process; 
   void* fire9_2nd_apu_process; 
   void* fire10_1st_apu_process; 
   void* fire10_2nd_top_apu_process; 
   void* fire10_2nd_bottom_apu_process; 


   ACF_APU_CFG mApuConfig;
   int32_t     mApexId;
   bool        mWeSelectedApuConfiguration;
   int32_t     mNumOfApuProcesses;
};

} /* namespace apexcv */

#endif /* SQUEEZEV10NET_HPP */
