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
 *  @file apexcv_pro_lktracker.h
 *  @brief ApexCV L-K tracker.
 **********************************************************************************/

#ifndef APEXCVPROLKTRACKER_H
#define APEXCVPROLKTRACKER_H

#include <stdint.h>
#include <icp_data.h>
#include <icp_feature_32s.h>

#include <umat.hpp>
#include "apexcv_error_codes.hpp"
#include "acf_process_apu.h"

// ACF HOST LIB

// Kernel specific setup (need keep sync with apu's lktracker_setup.h
#define LKTRACKER_FLOATING_COOR_Q 8
#define LKTRACKER_FLOATING_0_5_Q  0x80
#define LKTRACKER_PATCH_BORDER    6
#define LKTRACKER_PATCH_BW_7x7    20

namespace apexcv {

/*!*********************************************************************************
*   \brief ApexCV L-K Tracker Optical Flow.
* 
*  apexcv::LKTrackerOpticalFlow is the Host-ACF interface for creating, initializing,
*  executing and releasing the LKTracker on Apex.
***********************************************************************************/

class LKTrackerOpticalFlow
{
public:

/**********************************************************************************
*  Default Constructor.
* 
*  Allocate resources for the ACF processes.
***********************************************************************************/
   LKTrackerOpticalFlow();

/**********************************************************************************
*  Destructor.
*  Release resources for the ACF processes.
* *********************************************************************************/
   ~LKTrackerOpticalFlow();


/*!*********************************************************************************
*   \brief Initialization
* 
*  Initializes the intermediate buffers needed for the process, and initializes the
*  ACF processes. The size of internal buffers are determined by max_corners / w / h;
*  Supported box_size is only 7 for now;
* ***********************************************************************************/
   APEXCV_LIB_RESULT Initialize(   vsdk::UMat& aSrc0Desc       /*!< 8-bit grayscale template source image (frame[t-1]) */,
                     vsdk::UMat& aSrc1Desc                     /*!< 8-bit grayscale patch source image (frame[t]) */,
                     icp::Feature32SDescriptor& aCoor0Desc    /*!< template features list. X/Y coordinates are signed Q23.8 format */,
                     icp::Feature32SDescriptor& aCoor1Desc    /*!< patch features list. X/Y coordinates are signed Q23.8 format */,
                     icp::Feature32SDescriptor& aCoor1Desc_O  /*!< output features list. X/Y coordinates are signed Q23.8 format */,
                     int aMaxCorners              /*!< maximum number of corners going to be tracked */,
                     int aW                       /*!< image width */,
                     int aH                       /*!< image height */,
                     int aBoxSize   = 7           /*!< Box size. Only support 7 for now */,
                     int aNumIter   = 10          /*!< Number of iterations. Default is 10 */,
                     int aSrcPadded = 0           /*!< input source images have been padded already: 0:no padding 1:allocated border but no replicated 2:image fully padded */,
                     int aPadStartWidth = 3000    /*!< only image width less than aPadStartWidth, we do image padding (for tradeoff purpose between accuracy vs. performance) */
               );


/*!*********************************************************************************
*   \brief Reconnect IO
* 
*  Use this to connect the input, output buffers and features to be tracked. This will perform
*  preprocessing so it is always needed to
*  be done before "process" call. The number of features in Feature Descriptors cannot exceed the max_corners
*  used in initialize().
* ***********************************************************************************/
   APEXCV_LIB_RESULT ReconnectIO(  vsdk::UMat& aSrc0Desc      /*!< 8-bit grayscale template source image (frame[t-1]) */,
                     vsdk::UMat& aSrc1Desc                    /*!< 8-bit grayscale patch source image (frame[t]) */,
                     icp::Feature32SDescriptor& aCoor0Desc    /*!< template features list. X/Y coordinates are signed Q23.8 format */,
                     icp::Feature32SDescriptor& aCoor1Desc    /*!< patch features list. X/Y coordinates are signed Q23.8 format */,
                     icp::Feature32SDescriptor& aCoor1Desc_O  /*!< output features list. X/Y coordinates are signed Q23.8 format */
               );

/*!*********************************************************************************
*  \brief Set Box filter Size
*
*  Change the Box filter size used. only support 7 for now.
***********************************************************************************/
   APEXCV_LIB_RESULT SetBoxSize(int aBoxSize        /*!< Box size. Only support 7 for now */
               );

   
/*!*********************************************************************************
*  \brief Set number of iterations 
*
*  Change the number of iterations within eacy pyramid layer
***********************************************************************************/

   APEXCV_LIB_RESULT SetNumIter(int aNumIter       /*!< Number of iterations. */
               );

/*!*********************************************************************************
*  \brief Process
*  \return Error code for the ACF execution (zero on success).
*  "position" field in output features' descriptor represent the tracked features'
*  X/Y coordinate in signed Q23.8 format; "reverve" field in output features'
*  descriptor represent whether feature is successfully tracked. 0: failed; 1:
*  succeeded; "strength" field in output features' descriptor represent the sum of
*  pixels' grayscale absolute difference between template and tracked patch window,
*  i.e. the lower the better tracked.
***********************************************************************************/
   APEXCV_LIB_RESULT Process();  

   APEXCV_LIB_RESULT SelectApuConfiguration(ACF_APU_CFG aApuConfig,
                             int32_t aApexId);

/*!*********************************************************************************
*  \brief SelectApexCore 
*  \return Error code for the ACF execution (zero on success).
*
*  Select which APEX core (0 or 1) to be selected to run the processing. 
*  This function has to be called after initialize() and can be executed per frame base.
***********************************************************************************/
   APEXCV_LIB_RESULT SelectApexCore(int32_t aApexId /*!< The ID of the desired APEX (e.g if there are 2 APEXs, valid values for lApexId would be 0 and 1).*/
               );

private:
   void Release();              /*!< Release resources for the ACF processes. */

   void *mSrc0offsOal;          /*!< Pointer to the template offset buffer for each features to track */
   void *mSrc1offsOal;          /*!< Pointer to the patch offset buffer for each features to track */
   void *mSrc0PadOal;           /*!< Pointer to the buffer to pad input template image */
   void *mSrc1PadOal            /*!< Pointer to the buffer to pad input patch image */;

   vsdk::UMat mParamsDesc;                /*!< UMat of configuration parameters. Bytes [0-3]: MAX_X; [4-7]: MAX_Y; [8]: NUM_ITER; [9-11]: K_0, K_1, K_2*/
   vsdk::CustomPointerUMat mCoor0Desc;    /*!< Custom Pointer uMat of template features. */
   vsdk::CustomPointerUMat mCoor1Desc;    /*!< Custom Pointer uMat of patch features */
   vsdk::CustomPointerUMat mCoor1Desc_O;  /*!< Custom Pointer uMat of output patch features */
   vsdk::CustomPointerUMat mSrc0Offs;     /*!< Custom Pointer uMat of template offset for each features to track */
   vsdk::CustomPointerUMat mSrc1Offs;     /*!< Custom Pointer uMat of patch offset for each features to track */
   vsdk::CustomPointerUMat mSrc0PadDesc;  /*!< Custom Pointer uMat of padded input template image */
   vsdk::CustomPointerUMat mSrc1PadDesc;  /*!< Custom Pointer uMat of padded input patch image */


   bool mInit;                /*!< Indicates if the process has been initialized */
   int  mBoxSize;             /*!< Box size */
   int  mNumIter;             /*!< Number of iterations */
   int  mSrcPadded;           /*!< input source images have been padded already. 0:no padding 1:allocated border but no replicated 2:image fully padded */
   int  mPadStartWidth;       /*!< lowest width threshold lktracker will do padding */ 
   int  mNumFeatures;         /*!< Number of features being tracked */

   void* mProcess; /*!< tracker's graph */

   ACF_APU_CFG mApuConfig;
   int32_t     mApexId;       /*!< Index of the APEX core selected to run the algorithm*/
   bool        mWeSelectedApuConfiguration;

};
} /* namespace apexcv */

#endif /* APEXCVPROLKTRACKER_H */
