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

/*!*****************************************************************************
*  \file apexcv_pro_orb.h
*  \brief Orb library API
*******************************************************************************/

#ifndef APEXCVPROORB_H
#define APEXCVPROORB_H

#include <stdio.h>
#include <stdint.h>
#include <acf_process_apu.h>
#include <icp_data.h>
#include "apexcv_pro_fast.h"
#include "apexcv_error_codes.hpp"
using namespace icp;

#define ORB_PERF_PROB 0
#if (ORB_PERF_PROB != 0)
enum
{
  ORB_CREATE = 0,
  ORB_DETECT,
  ORB_COMPUTE,
  ORB_PROFILED_FUNC_CNT
};

enum
{
  Orb_Create_Start = 0,
  Orb_Create_Mallocs,
  Orb_Create_Fast_Init,
  Orb_Create_Orient_Init,
  Orb_Create_RBrief_Init
};

enum
{
  Orb_Detect_Start = 0,
  Orb_Detect_Parse_Keypoints,
  Orb_Detect_Orientation_Run
};

enum
{
  Orb_Compute_Start = 0,
  Orb_Compute_RBrief_Run
};
#define PROBES_CNT 6
extern volatile int64_t orbTime[ORB_PROFILED_FUNC_CNT][PROBES_CNT];
extern AcfProfilingInfo *pAcfInfoOrbOrientation;
extern AcfProfilingInfo *pAcfInfoOrbRbrief;
#endif

namespace apexcv
{
  class Corner
  {
public:
  unsigned int x; // X position in the image
  unsigned int y; // Y position in the image
  float strength; // Corner strength based on the Harris Score
  };

  class OrbConfig
  {
public:
  int apexId; // Apex Id of the accelerators that runs ICO + RBRIEF the algorithm
  int nrOfThreads; // Nr of parallel threads running the algorihm - max value = 2
  };

  /*!*****************************************************************************
  *  \brief          Apex Orb class
  *  \details        apexcv::Orb is the host-ACF interface for creating, initializing,
  *                  executing and releasing the resources for running the Orb algorithm
  *******************************************************************************/
  class Orb
  {
public:

  /*******************************************************************************
  *  Orb default constructor, no allocations are done here
  *******************************************************************************/
  Orb();

  /*******************************************************************************
  *  Orb default  desconstructor, releases the resources
  *******************************************************************************/
  ~Orb();

/*!*****************************************************************************
*  \brief  Initializes the Orb class. The function begins by allocating all the
*  necessary buffers then initializes the FAST keypoint detector, Harris Corner
*  Score and Orb orientation.
*  \param[in] aConfig Allows the user to select the apexid and APUs
*  \param[in] aInImg 8-bit grayscale source image
*  \param[in] aBorderSize Image border that will be trimmed from aInImg
*  \param[in] aPatchSize A square region that contains the center feature
*  \param[in] aRadius  Hypothetical circle with its center inside the
*  center of the patch
*  \param[in] aDescriptorSizeInBytes The size of the descriptor in bytes
*  for each patch
*  \param[in] aFastCircumference The circumference of the circle where
*  the FAST9 algorithm is applied
*  \param[in] aFastThreshold Fast algorithm detection threshold
*  \param[in] aHarrisK Harris Corner Coefficent
*  \param[in] aNrOfKeypoints maximum number of keypoints that the algo
*  should look for
*  \return Check apexcv_error_codes.h to see to possible outcomes
*  \note Maximum resolution is <b> 1920 x 1080 pixels </b> with <b> 64 </b> CUs.
*******************************************************************************/
  APEXCV_LIB_RESULT Create(OrbConfig &aConfig,
                           vsdk::UMat &aInImg,
                           unsigned int aBorderSize,
                           unsigned int aPatchSize,
                           unsigned int aRadius,
                           unsigned int aDescriptorSizeInBytes,
                           unsigned int aFastCircumference,
                           unsigned int aFastThreshold,
                           float aHarrisK,
                           unsigned int aNrOfKeypoints);

/*!*****************************************************************************
*  \brief  Starts the processing of the Orb keypoint detector.
*  Three processes will be run on APEX: FAST, Harris and Orb orientation.
*  After APEX finishes, the best <b> nrOfKeypoints </b> are selected based on
*  Harris Score.
*  \param[out] aNrOfKeypoints Indicates the maximum nr of keypoints
*  that the algo should look for. The number of detected keypoints could be
*  less than <b> nrOfKeypoints </b> if so the value is updated.
*  \return Error code for initialization (zero on success).
*  \note This method has the highest latency and the processing is split between
*  APEX and ARM
*  \return Check apexcv_error_codes.h to see to possible outcomes
*******************************************************************************/
  APEXCV_LIB_RESULT Detect(unsigned int *aNrOfKeypoints);

/*!*****************************************************************************
*  \brief  Starts the processing of the Orb rBrief
*  \param[in] aInSmoothedImg 8-bit grayscale source image
*  \param[in] aInBitPattern 8-bit unsigned containing the bit pattern for
*  which the output descriptors will be calculated. \n The number of values is
*  equal to 2 * 2 * 8 * \b descriptorSizeBytes.
*  \param[out] aOutDescriptors- [Output] - 8-bit unsigned, will containt out descr.\n
*  Expected size: descriptorSizeBytes * nrOfKeypoints \n
*  \note Maximum resolution is <b> 1920 x 1080 pixels </b> with <b> 64 </b> CUs.
*  \return Check apexcv_error_codes.h to see to possible outcomes
*******************************************************************************/
  APEXCV_LIB_RESULT Compute(vsdk::UMat &aInSmoothedImg,
                            vsdk::UMat &aInBitPattern,
                            vsdk::UMat &aOutDescriptors);

/*!*****************************************************************************
*  \brief Returns the chunk offsets \n
*******************************************************************************/
  vsdk::UMat& GetChunkOffsets();

/*!*****************************************************************************
*  \brief Returns the patch size of rBRIEF/IC \n
*******************************************************************************/
  vsdk::UMat& GetPatchSize();

/*!*****************************************************************************
*  \brief Returns the radius of the IC calculus \n
*******************************************************************************/
  vsdk::UMat& GetRadius();

/*!*****************************************************************************
*  \brief Returns the FAST9 score image \n
*******************************************************************************/
  vsdk::UMat& GetFastOut();

/*!*****************************************************************************
*  \brief Returns the Harris cornerness score image \n
*******************************************************************************/
  vsdk::UMat& GetHarrisOut();

/*!*****************************************************************************
*  \brief Returns the angles for each patch calculated by the ICO \n
*******************************************************************************/
  vsdk::UMat& GetIcoAngles();

/*!*****************************************************************************
*  \brief Returns the packed coordinates and metric for the FAST9 Img \n
*******************************************************************************/
  vsdk::UMat& GetSerializerOut();

/*!*****************************************************************************
*  \brief Returns the numer of clusters found by the Orb serializer \n
*******************************************************************************/
  int GetNrOfDetectedKeypoints();

/*!*****************************************************************************
*  \brief Dumps the keypoints in a standard format \n
*  \param[out] aInput Will store the keypoints list
*  \return Error code for initialization (zero on success).
*******************************************************************************/
  APEXCV_LIB_RESULT GetKeypoints(vsdk::UMat& aInput);

private:
  std::vector<Corner> mKeypoints; /*!< Data container for all the keypoints */
  vsdk::UMat *mInGrayScale; /*!< Reference to the input data image */
  vsdk::UMat mPatchSizeInPixels; /*!< Keep track of the patch size */
  vsdk::UMat mRadiusInPixels; /*!< Keep track of the radius size */
  vsdk::UMat mFastCornerList; /*!< Fast corner score image */
  vsdk::UMat mDescrSizeBytes; /*!< The size of the descriptor in bytes for each patch. */

  unsigned char *mpBaseImg; /*!< Keeps track of the base address where the input image is stored */
  vsdk::UMat *mpChunkOffsets; /*!< Keeps track of the image offsets for all the valid keypoints */
  vsdk::UMat *mpAngles; /*!< 8-bit unsigned char. For each keypoint the algorithm outputs a single value.*/
  vsdk::UMat *mpDescriptorCount; /*!< 8-bit unsigned char. For each keypoint the algorithm outputs a single value.*/
  vsdk::UMat *mpBinPatternOut; /*!< 8-bit unsigned char. This buffer will keep the descriptors but it will be split in nrOfThreads parts*/

  OrbConfig mOrbHwConfig; /*!< Desired hardware layout of the algorithm */
  unsigned int mInit; /*!< Indicates if it has been initialized or not*/
  unsigned int mImgSpan; /*!< Input image span */
  unsigned int mImgW; /*!< Input image width */
  unsigned int mImgH; /*!< Input image height */
  unsigned int mNrOfFeatures; /*!< Number of desired keypoints */
  unsigned int mDistanceFromBorder; /*!< All the pixels that are inside x,y = borderSize:Width-BorderSize will be processed by the algorithm. */
  unsigned int mFastCirc; /*!< Fast circumference */
  unsigned int mFastTh; /*!< Fast threshold */
  unsigned int mDetectedKeypoints; /*!< Detected keypoints by the FAST9 algorithm */
  float mHarrisK; /*!< Harris scaling constant */

  vsdk::UMat mAllChunkOffsets; /*!< Contains the chunk offsets from both APU */
  vsdk::UMat mAllAngles; /*!< Contains estimated intensicy centroid angles from both APU */

  Fast mFast9; /*!< Fast class */

  void * mpProcessCalcOrientation; /*!< Orb orientation ACF process */
  void * mpProcessCalcRbrief; /*!< Orb rBRIEF ACF process */
  };
}
#endif  /* APEXCVPROORB_H */
