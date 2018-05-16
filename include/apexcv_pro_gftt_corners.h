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

/*!*********************************************************************************
*  \file apexcv_pro_gftt_corners.h
*  \brief Apex GFTT Corner Detector.
***********************************************************************************/

#ifndef APEXCVPROGFTTCORNERS_H
#define APEXCVPROGFTTCORNERS_H

#include <stdint.h>

// ACF HOST LIB
#include <icp_data.h>
#include <icp_feature.h>

#include "acf_process_apu.h"
#include <umat.hpp>
#include <apexcv_pro_gftt_harris_common.h>


namespace apexcv{

/*!*********************************************************************************
* \brief apexcv::GFTTCorners is the host-ACF interface for creating, initializing,
* executing the \ref pagGFTTCornerDetector on Apex.
*
* The interface determines strong corners on an image based on 
* GFTT Corners Response described in \cite GFTT_CORNER (default) or 
* Haris Corners Response described in \cite HARRIS_CORNER. \n
* \b Note: The feature needs to be run in \b 64 \b CUs configuration.
***********************************************************************************/
class GFTTCorners
{
public:

/**********************************************************************************
*  Default constructor.
*  Allocate resources for the ACF processes.
***********************************************************************************/
  GFTTCorners();

/**********************************************************************************
*  Destructor.
*  Release resources for the ACF processes.
***********************************************************************************/
  ~GFTTCorners();

/*!*********************************************************************************
*  \brief Initialize the corner detector.
*  In this release,
*  Sobel filter size are fixed to 3.
*  Box filter size are fixed to 7.
*  NMS filter size are fixed to 5.\n
*  \b Note: The feature need to be run in \b 64 \b CUs configuration.
***********************************************************************************/
  int Initialize(vsdk::UMat& src         /*!< 8-bit grayscale source image.\n
                                              The maximum resolution supported is 1024 pixel width with 64 CUs configuration.*/,
                 icp::FeatureDescriptor& corners  /*!< 16-bit signed integer destination corner list buffer. */,
                 int qualityLevel=4               /*!< Quality Level used for GFTT threshold.\n
                                                       The parameter is applied if GFTT detector is being used (useHarrisDetector = 0).\n
                                                       Threshold = (maxEigenvalue * qualityLevel/256).*/,
                 int minDistance=5                /*!< Minimum distance between 2 corners. For this release, minimum distance cannot be higher than 5.*/,
                 int maxTotalCorners = 4096       /*!< Maximum number of total corners to detect.\n
                                                       If number of corners are higher than 4096, ARM will handle corners sorting and extraction and performance will be impacted.*/,
                 int useHarrisDetector = 0        /*!< Indicate whether to use Harris detector or GFTT detector.\n
                                                       If useHarrisDetector = 0, GFTT detector is used.\n
                                                       If useHarrisDetector = 1, Harris detector is used.*/,
                 int harrisK = 10                 /*!< Fixed point Harris Corner Coefficent (k).\n Range 0 - 255.\n Fixed_point_K = Floating_point_K * 256.
                                                       Floating point k is described in \cite HARRIS_CORNER.\n The parameter is applied if
                                                       Harris detector is being used (useHarrisDetector = 1).*/,
                 int harrisThreshold = 0          /*!< User defined Harris Threshold.\n Range MIN_INT16_T - MAX_INT16_T.\n The parameter is applied if
                                                       Harris detector is being used (useHarrisDetector = 1).*/);

/*!*********************************************************************************
*  \brief Reconnect the Input and Output Buffers
*
*  Use this to reconnect the input and output buffers. This only needs to
*  be done if the connected Input/Outputs are changed. If only the data within (no size,
*  or type changes), then this does not need to be called.
***********************************************************************************/
  int ReconnectIO(vsdk::UMat& src           /*!< 8-bit grayscale source image. */,
                  icp::FeatureDescriptor& corners    /*!< 16-bit signed integer corner list buffer. */);
/*!*********************************************************************************
*  \brief Set custom rescale shift value for covariance and box filter
*
* Use this method to override the default rescale shift value for covariance and box filter.
* 
* default covarianceScaleFactor is 6
* default boxSize               is 7  (out of 3, 5, 7)
*
***********************************************************************************/
  int SetShiftValue(int covarianceScaleFactor, /*!< Rescale shift value for covariance filter */
                    int boxSize /*!< box filter size */);
/*!*********************************************************************************
*  \brief reset box_size, nms_size and covariance to default values
*
* Use this method to reset the values of box filter and nms filter size back to the default
* 
***********************************************************************************/
  int ResetShiftValue();
/*!*********************************************************************************
* \brief Set Parameters for GFTT corners detector
*
* For this release,
*  Sobel filter size are fixed to 3.
*  Box filter size are fixed to 7.
*  NMS filter size are fixed to 5.
***********************************************************************************/
  int SetParameters(int qualityLevel = 4          /*!< Quality Level used for GFTT threshold.\n
                                                       The parameter is applied if GFTT detector is being used (useHarrisDetector = 0).\n
                                                       Threshold = (maxEigenValue * qualityLevel/256).*/,
                    int minDistance = 5           /*!< Minimum distance between 2 corners. For this release, minimum distance cannot be higher than 5.*/,
                    int maxCorners = 4096         /*!< Maximum number of total corners to detect.\n
                                                       If number of corners are higher than 4096, ARM will handle corners sorting and extraction and performance will be impacted.*/,
                    int useHarrisDetector = 0     /*!< Indicate whether to use Harris detector or GFTT detector.\n
                                                       If useHarrisDetector = 0, GFTT detector is used.\n
                                                       If useHarrisDetector = 1, Harris detector is used.*/,
                    int harrisK = 10              /*!< Fixed point Harris Corner Coefficent (k).\n Range 0 - 255.\n Fixed_point_K = Floating_point_K * 256.
                                                       Floating point k is described in \cite HARRIS_CORNER.\n The parameter is applied if
                                                       Harris detector is being used (useHarrisDetector = 1).*/,
                    int harrisThreshold = 0       /*!< User defined Harris Threshold.\n Range MIN_INT16_T - MAX_INT16_T.\n The parameter is applied if
                                                       Harris detector is being used (useHarrisDetector = 1).*/);

/*!*********************************************************************************
* \brief Set the total maximum number of corners to be returned.
*
* To maximize the performance, it is recomended to keep the maximum corners to be 
* lower than 4096. If number of conrers are higher than 4096, ARM will handle 
* corners sorting and extraction and performance will be impacted.
***********************************************************************************/
  int SetMaxNumberCorners(int maxTotalCorners);
/*!*********************************************************************************
* \brief Run APEX GFTT/HARRIS Interface.
* \return Error code for the ACF execution (zero on success).
*
* For each pixel, the corner score is computed (referred to as "corner response" in
* \cite HARRIS_CORNER or \cite GFTT_CORNER ). The score image can
* be returned using retCornerImage(). From the score image, the list of strong
* corners are returned.
***********************************************************************************/
  int Process();
/*!*********************************************************************************
*  \brief Returns Block Width
*
*  Returns the block width used in the process
***********************************************************************************/
  int RetBlockWidth();
/*!*********************************************************************************
*  \brief Returns Block Height
*
*  Returns the block width used in the process.
***********************************************************************************/
  int RetBlockHeight();
/*!********************************************************************************
*  \brief Returns Number of detected corners
*
***********************************************************************************/
  int RetNumberCorners();
/*!*********************************************************************************
* \brief Returns a 16-bit corner score image.
*
************************************************************************************/
  vsdk::UMat RetCornerImage();
/*!*********************************************************************************
* \brief Select Apu configuration
*
************************************************************************************/
  int SelectApuConfiguration(ACF_APU_CFG lApuConfig,
                             int32_t lApexId);
private:
  int Init_params(/*!< Allocate resources for the ACF processes. */
                  int qualityLevel = 4,
                  int minDistance = MAX_MIN_DISTANCE,
                  int boxSize = BOX_SIZE,
                  int nmsRadius = NMS_SIZE,
                  int maxCorners = 4096,
                  int useHarrisDetector = 0,
                  int harrisK = 10,
                  int harrisThreshold = 0);
  int Init_buffer(
                  int width,
                  int height); /*!< Allocate internal corner image buffer. */
  int ProcessCorners(icp::FeatureDescriptor&  corners); /*!< Sort and Filter Corners in the ARM. */
  int Release();      /*!< Release resources for the ACF processes. */

  int IntializeSelectedGraph();        /*!< Initialize the corner detector with selected box filter size and nms filter size */
  int ConnectIOSelectedGraph(vsdk::UMat& src);   /*!< connect the Input and Output buffers with selected box filter size and nms filter size */
  int ReconnectIOSelectedGraph(vsdk::UMat& src); /*!< Reconnect the Input and Output Buffers with selected box filter size and nms filter size */
  int ProcessHarrisApexSelectedGraph(); /*!< Process harris apex case with selected box filter size and nms filter size */
  int ProcessHarrisArmSelectedGraph();  /*!< Process harris arm case with selected box filter size and nms filter size */
  int ProcessGfttApexSelectedGraph();   /*!< Process gftt apex case with selected box filter size and nms filter size */
  int ProcessGfttArmSelectedGraph();    /*!< Process gftt arm case with selected box filter size and nms filter size */
  int HarrisRetBlockWidthAndHeight(int32_t &chunkWidth, int32_t &chunkHeight);
  int GfttRetBlockWidthAndHeight(int32_t &chunkWidth, int32_t &chunkHeight);

  enum state /*! operational states of the object */
  {
    init, /*!< Initialization state for all ACF processes. */
    HarrisAPEX, /*!< Harris Calculation and Corner extraction in APEX, when corners <= MAX_CORNERS */
    HarrisARM, /*!< Harris calculation in APEX, corner extraction in ARM, when corners > MAX_CORNERS */
    GFTTAPEX, /*!< GFTT calculation and corner extraction in APEX, when corners <= MAX_CORNERS */
    GFTTARM  /*!< GFTT calculation in APEX, corner extraction in ARM, when corners > MAX_CORNERS */
  } mState;
  int32_t mChunkWidth;                    /*!< The chunk width used in the process */
  int32_t mChunkHeight;                   /*!< The chunk height used in the process */
  int32_t mImageWidth;                    /*!< The image width used in the process */
  int32_t mImageHeight;                   /*!< The image height used in the process */
   
  vsdk::UMat mHarrisParams;       /*!< Harris corner parameter memory buffer. */
  vsdk::UMat mGFTTParams;         /*!< GFTT corner parameter memory buffer. */
  vsdk::UMat mSortFilterParams;   /*!< Sort and Filter Parameters. */
  vsdk::UMat mCoord;              /*!< GFTT buffer contains list of corner coord. */
  vsdk::UMat mStren;              /*!< GFTT buffer contains list of corner strength. */
  vsdk::UMat mCount;              /*!< Count total number of detected corners.  */
  vsdk::UMat mDstImg;             /*!< Strength Image Destination buffer */
  vsdk::CustomPointerUMat mCornerList;         /*!< Alloc a copy of pointer to the Corner List memory buffer. */
  vsdk::UMat mNumCorner;          /*!< Final number of corners after filtering. */
  icp::FeatureDescriptor* mCorners;        /*!< Keep a copy of pointer to the Corner List memory buffer. */
  
  void *mGFTTB7N5Compute;            /*!< GFTT Corner strength calculation box filter size 7 and nms filter size 5*/
  void *mGFTTB5N5Compute;            /*!< GFTT Corner strength calculation box filter size 5 and nms filter size 5*/
  void *mGFTTB3N5Compute;            /*!< GFTT Corner strength calculation box filter size 3 and nms filter size 5*/

  void *mGFTTB7N5ComputeExtract;      /*!< GFTT Corner Calculation and extract process box filter size 7 and nms filter size 5*/
  void *mGFTTB5N5ComputeExtract;      /*!< GFTT Corner Calculation and extract process box filter size 5 and nms filter size 5*/
  void *mGFTTB3N5ComputeExtract;      /*!< GFTT Corner Calculation and extract process box filter size 3 and nms filter size 5*/
  
  void *mHARRISB7N5Compute;          /*!< Harris Corner Strength calculation process box filter size 7 and nms filter size 5*/
  void *mHARRISB5N5Compute;          /*!< Harris Corner Strength calculation process box filter size 5 and nms filter size 5*/
  void *mHARRISB3N5Compute;          /*!< Harris Corner Strength calculation process box filter size 3 and nms filter size 5*/
  
  void *mHARRISB7N5ComputeExtract;  /*!< Harris Corner Calculation and extract process box filter size 7 and nms filter size 5*/
  void *mHARRISB5N5ComputeExtract;  /*!< Harris Corner Calculation and extract process box filter size 5 and nms filter size 5*/
  void *mHARRISB3N5ComputeExtract;  /*!< Harris Corner Calculation and extract process box filter size 3 and nms filter size 5*/
   
  ACF_APU_CFG mApuConfig;
  int32_t     mApexId;
  bool        mWeSelectedApuConfiguration;

  int32_t     mBoxSize;
  int32_t     mNMSSize;
  int32_t     mSetChunkWidth;
  bool        mSetACFSelectChunkSize;
};

}  /* namespace apexcv */

#endif /* APEXCVPROGFTTCORNERS_H */
