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

/*!****************************************************************************
 *  \file apexcv_pro_brief.h
 *  \brief ApexCV Host wrapper for BRIEF algorithm
 *******************************************************************************/

#ifndef APEXCVPROBRIEF_H
#define APEXCVPROBRIEF_H

#include <stdint.h>
#include "apexcv_error_codes.hpp"
#include <acf_process_apu.h>

#define BRIEF_PERF_PROB 0
#if (BRIEF_PERF_PROB != 0)
enum
{
  BRIEF_INIT = 0,
  BRIEF_PROCESS,
  BRIEF_RECONN,
  BRIEF_PROFILED_FUNC_CNT
};

enum
{
  Brief_Init_Start = 0,
  Brief_Init_Mallocs,
  Brief_Init_Sampling_Pattern_Parsing,
  Brief_Init_Process_Init
};

enum
{
  Brief_Process_Start = 0,
  Brief_Process_Run
};

enum
{
  Brief_Reconn_Start = 0,
  Brief_Reconn_Run
};

#define PROBES_CNT 6
extern volatile int64_t gBriefTime[BRIEF_PROFILED_FUNC_CNT][PROBES_CNT];
extern AcfProfilingInfo gAcfInfoBrief;
#endif

namespace apexcv
{



  /*!****************************************************************************
  *  \brief BRIEF class
  *
  *  This class is an interface for using the BRIEF (Binary Robust Independent
  *  Elementary Features) algorithm.
  ******************************************************************************/
  class Brief
  {
public:
  /*!****************************************************************************
  *  \brief Filtering type size enum.
  ******************************************************************************/
  typedef enum
  {
    NO_FILTERING           = 0,
    BOX_9x9_FILTERING      = 1,
    GAUSSIAN_9x9_FILTERING = 2
  } FilteringType;
  /*!****************************************************************************
  *  \brief Default constructor.
  ******************************************************************************/
  Brief();

  /*!****************************************************************************
  *  \brief Default destructor.
  ******************************************************************************/
  ~Brief();

  /*!*****************************************************************************
  *  \brief  Initializes the Brief class and calculates the chunks offsets.
  *  \param[in] aImage - 8-bit grayscale source image
  *  \param[in] aSmplPattern - cartesian coordinate pairs that describe the
                               sampling pattern - <x0, y0>, <x1, y1>, ...

  *  \param[in] aKeypoints - 16-bit unsigned <x, y> cartesian coordinates that
                             pinpoint a keypoint
  *  \param[in] aFilterType - See type to understand the filtering
  *  \param[in] aDescrSizeBytes - 16, 32 or 64 Bytes descriptors
  *  \param[in] aBorderSize - The descriptors for the keypoints inside the
                              border will not be computed.
  *  \param[out] aDescriptors - 8-bit descriptors
  *  \return Check apexcv_error_codes.h to see to possible outcomes
  *  \note Max resolution is <b> 1920 x 1080 pixels </b> with <b> 32 </b> CUs.
  *******************************************************************************/
  APEXCV_LIB_RESULT Initialize(vsdk::UMat &aImage,
                               std::vector<signed char> &aSmplPattern,
                               std::vector<unsigned int> &aKeypoints,
                               FilteringType aFilterType,
                               unsigned char aDescrSizeBytes,
                               unsigned char aBorderSize,
                               vsdk::UMat &aDescriptors);

  /*!*****************************************************************************
  *  \brief  Reinitializes the Brief class and calculates the chunks offsets.
  *  \param[in] aImage - 8-bit grayscale source image
  *  \param[in] aSmplPattern - cartesian coordinate pairs that describe the
                               sampling pattern - <x0, y0>, <x1, y1>, ...

  *  \param[in] aKeypoints - 16-bit unsigned <x, y> cartesian coordinates that
                             pinpoint a keypoint
  *  \param[in] aFilterType - See type to understand the filtering
  *  \param[in] aDescrSizeBytes - 16, 32 or 64 Bytes descriptors
  *  \param[in] aBorderSize - The descriptors for the keypoints inside the
                              border will not be computed.
  *  \param[out] aDescriptors - 8-bit descriptors
  *  \return Check apexcv_error_codes.h to see to possible outcomes
  *  \note Max resolution is <b> 1920 x 1080 pixels </b> with <b> 32 </b> CUs.
  *******************************************************************************/
  APEXCV_LIB_RESULT ReconnectIO(vsdk::UMat &aImage,
                                std::vector<signed char> &aSmplPattern,
                                std::vector<unsigned int> &aKeypoints,
                                FilteringType aFilterType,
                                unsigned char aDescrSizeBytes,
                                unsigned char aBorderSize,
                                vsdk::UMat &aDescriptors);

  /*!*****************************************************************************
  *  \brief APEX hardware configuration
  *  \param[in] aApuConfig Apu CU size
  *  \param[in] aApexId Apex id where the code will execute
  *  \return Please check apexcv_error_codes.hpp
  *******************************************************************************/
  APEXCV_LIB_RESULT SelectApuConfiguration(ACF_APU_CFG aApuConfig = ACF_APU_CFG__DEFAULT,
                                           int32_t aApexId = 0);

  /*!*****************************************************************************
  *  \brief Starts the APEX processing
  *  \return Check apexcv_error_codes.h to see to possible outcomes
  *  \note Maximum resolution is <b> 1920 x 1080 pixels </b> with <b> 64 </b> CUs.
  *******************************************************************************/
  APEXCV_LIB_RESULT Process();

private:
  unsigned int mInit; /*!< Semaphore that indicates the state of class */
  int mApexId; /*!< APEX ID: 0/1 */
  ACF_APU_CFG mApuConfig; /*!< APU configuration hardcoded to 32 CU due to DMA limitation */
  int mFilterPadding; /*!< Added for scalability - 4 pixels of padding */
  int mVectorSize; /*!< Added for scalability - 32 Cu */
  int mPatchSize; /*!< Added for scalability - 48 */
  int mNrOfKpnts; /*!< Counter for the number of input keypoints */
  int mImgW; /*!< Image width */
  int mImgH; /*!< Image height */
  int mImgSpan; /*!< Image row size */
  const static int mPatchIsSplitInFour = 4; /*!< Patch slicing */
  std::vector<unsigned int> mPartions[mPatchIsSplitInFour]; /* Contains the coordinates allocated per slices */

  vsdk::UMat mSamplingPattern; /*!< Contains all the coordinates for the brief sampling pattern */
  vsdk::UMat mFilterType; /*!< Type of filtering: 0 - no filtering, 1 - box 9x9x and 2 - gaussian 9x9 */
  vsdk::UMat mChunkOffsets; /*!< Offsets used by the indirect input mechanism  */
  vsdk::UMat mDescriptorSizeInBytes; /*!< Supported sizes: 16, 32 or 64 Bytes */
  vsdk::UMat mPatchSizeInPixels; /*!< Current hardcoded value: 48 x 48 pixels + filter padding */
  vsdk::UMat mULPacketsCount; /*!< Number of sampling cooridantes inside Upper Left chunks slice */
  vsdk::UMat mURPacketsCount; /*!< Number of sampling cooridantes inside Upper Right chunks slice */
  vsdk::UMat mLLPacketsCount; /*!< Number of sampling cooridantes inside Lower Left chunks slice */
  vsdk::UMat mLRPacketsCount; /*!< Number of sampling cooridantes inside Lower Right chunks slice */
  vsdk::UMat mCount; /*!< Counts how many descriptors were written by APEX outside of the HW */

  void *  mpProcess; /*!< APEX process abstraction */
  }; /* class Brief */
} /* namespace apex */

#endif /* APEXCVPROBRIEF_H */
