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

/*!****************************************************************************
 *  \file apexcv_pro_lbp.h
 *  \brief ApexCV Host wrapper for LBP algorithm
 *****************************************************************************/

#ifndef APEXCVPROLBP_H
#define APEXCVPROLBP_H

#include <icp_data.h>
#include "apexcv_error_codes.hpp"
#include <acf_process_apu.h>

namespace apexcv
{
  static const int LBP_RADIUS      = 2;                    /*!< LBP radius size. */
  static const int LBP_GRID_X      = 8;                    /*!< LBP block count in horizontal direction. */
  static const int LBP_GRID_Y      = 8;                    /*!< LBP block count in vertical direction. */
  static const int LBP_NEIGHBORS   = 8;                    /*!< # of LBP sampling point. */
  static const int LBP_HIST_LENGTH = (1 << LBP_NEIGHBORS); /*!< # of LBP histogram bin. */
  static const int LBP_DESC_SIZE   =                       /*!< LBP descriptor size. */
      (LBP_GRID_X * LBP_GRID_Y * LBP_HIST_LENGTH);

  /*!**************************************************************************
   * \brief LBP
   *
   * apexcv::Lbp is the host-ACF interface for creating, initializing,
   * executing and releasing the APU implementation of Lbp on APEX.
  ****************************************************************************/
  class Lbp
  {
public:
  /***************************************************************************
   * Default constructor.
   ***************************************************************************/
  Lbp();

  /***************************************************************************
   *  Default destructor.
   ***************************************************************************/
  ~Lbp();

  /*!**************************************************************************
   * \brief Initializes train process: connects the buffer to the process port,
   * and allocates/initializes any internal buffers.
   * \param[in] aSrc Source memory buffer. Datatype is VSDK_CV_8UC1.
   * \param[in] aSrcWidth Source image width.
   * \param[in] aSrcHeight Source image height.
   * \param[in] aSrcNum Number of source image.
   * \param[out] aDescriptor Destination memory buffer. Datatype is VSDK_CV_8UC1.
   * \return Error code for the initialization - see apexcv_error_codes.hpp
  ****************************************************************************/
  APEXCV_LIB_RESULT InitializeTrain(vsdk::UMat& aSrc,
                                    int aSrcWidth,
                                    int aSrcHeight,
                                    int aSrcNum,
                                    vsdk::UMat& aDescriptor);

  /*!**************************************************************************
   * \brief Initializes predict process: connects the buffer to the process port,
   * and allocates/initializes any internal buffers.
   * \param[in] aModel Source model descriptor buffer. Datatype is VSDK_CV_8UC1.
   * \param[in] aModelNum Model descriptor count.
   * \param[in] aSrc Source test image buffer. Datatype is VSDK_CV_8UC1.
   * \param[in] aSrcWidth Source image width.
   * \param[in] aSrcHeight Source image height.
   * \param[out] aDescriptor Output test descriptor.
   * \param[out] aClosestID Output closest ID buffer. Datatype is VSDK_CV_16UC1.
   * \param[out] aDistance Output histogram distance buffer. Datatype is
   * VSDK_CV_32SC1. The buffer width must be 32, and its height should be
   * (modelNum+32-1)/32.
   * \return Error code for the initialization - see apexcv_error_codes.hpp
  ****************************************************************************/
  APEXCV_LIB_RESULT InitializePredict(vsdk::UMat& aModel,
                        int aModelNum,
                        vsdk::UMat& aSrc,
                        int aSrcWidth,
                        int aSrcHeight,
                        vsdk::UMat& aDescriptor,
                        vsdk::UMat& aClosestID,
                        vsdk::UMat& aDistance);

  /*!**************************************************************************
   * \brief Reconnects the input/outputs to train process
   * \param[in] aSrc Source memory buffer. Datatype is VSDK_CV_8UC1.
   * \param[in] aSrcWidth Source image width.
   * \param[in] aSrcHeight Source image height.
   * \param[in] aSrcNum Number of source image.
   * \param[out] aDescriptor Destination memory buffer. Datatype is VSDK_CV_8UC1.
   * \return Error code for the reinitialization - see apexcv_error_codes.hpp
  ***************************************************************************/
  APEXCV_LIB_RESULT ReconnectTrainIO(vsdk::UMat& aSrc,
                                     int aSrcWidth,
                                     int aSrcHeight,
                                     int aSrcNum,
                                     vsdk::UMat& aDescriptor);

  /*!**************************************************************************
   * \brief Reconnects the input/outputs to predict process
   * \param[in] aModel Source model descriptor buffer. Datatype is 08U.
   * \param[in] aModelNum Model descriptor count.
   * \param[in] aSrc Source test image buffer. Datatype is 08U.
   * \param[in] aSrcWidth Source image width.
   * \param[in] aSrcHeight Source image height.
   * \param[out] aDescriptor Output test descriptor.
   * \param[out] aClosestID Output closest ID buffer. Datatype is 16U.
   * \param[out] aDistance Output histogram distance buffer. Datatype is 32S.
   * The buffer width must be 32, and its height should be (modelNum+32-1)/32.
   * \return Error code for the reinitialization - see apexcv_error_codes.hpp
  ****************************************************************************/
  APEXCV_LIB_RESULT ReconnectPredictIO(vsdk::UMat& aModel,
                                       int aModelNum,
                                       vsdk::UMat& aSrc,
                                       int aSrcWidth,
                                       int aSrcHeight,
                                       vsdk::UMat& aDescriptor,
                                       vsdk::UMat& aClosestID,
                                       vsdk::UMat& aDistance);

  /*!**************************************************************************
   * \brief Runs APEX-LBP train process to extract LBP descriptor.
   *
   * Computes LBP descriptor for each grid cell.
   *
   * Supported datatypes are:
   *    - unsigned 8 bit image to unsigned 8 bit descriptor
   *
   * \return Error code for the execution - see apexcv_error_codes.hpp
  ****************************************************************************/
  APEXCV_LIB_RESULT ProcessTrain();

  /*!**************************************************************************
   * \brief Runs APEX-LBP predict process to compare test descriptor to model
   * descriptor.
   *
   * Computes LBP descriptor for test image, then compares test descriptor to
   * model descriptor to find the closest descriptor.
   *
   * Supported datatypes are:
   *   INPUT:  unsigned 8 bit image and unsigned 8 bit mode descriptor
   *   OUTPUT: unsigned 8 bit test descriptor, unsigned 16 bit closest ID
   *   and signed 32 bit distance
   *
   * \return Error code for the execution - see apexcv_error_codes.hpp
  ****************************************************************************/
  APEXCV_LIB_RESULT ProcessPredict();

  /*!**************************************************************************
   * \brief Deprecated. Method will be removed.
   * TODO Remove method
   * \return Error code for the execution - see apexcv_error_codes.hpp
  ****************************************************************************/
  APEXCV_LIB_RESULT ProcessPredictExtract();

  /*!**************************************************************************
   * \brief Deprecated. Method will be removed.
   * TODO Remove method
   * \return Error code for the execution - see apexcv_error_codes.hpp
  ****************************************************************************/
  APEXCV_LIB_RESULT ProcessPredictCompare();

  /*!**************************************************************************
  *  \brief SelectApexCore
  *
  *  Selects which APEX core (0 or 1) to be selected to run the processing.
  *  This function has to be called after the two Initialize methods.
  *  \return Error code for updating the APU configuration - see apexcv_error_codes.hpp
  ****************************************************************************/
  APEXCV_LIB_RESULT SelectApexCore(int32_t aApexId);

private:
  /*!< \brief Flag indicating if the train process has been initialized. */
  unsigned int mInitStateTrain;
  /*!< \brief Flag indicating if the predict process has been initialized. */
  unsigned int mInitStatePredict;

  /*!< \brief APU configuration for the ACF process */
  ACF_APU_CFG mApuConfig;
  /*!< \brief APEX core to be used for running the process */
  int32_t mApexId;

  /*!< \brief Process size description for train process. */
  vsdk::UMat mProcessSizeTrain;
  /*!< \brief Process size description for predict process. */
  vsdk::UMat mProcessSizePredict;
  /*!< \brief Source memory buffer for the train process. */
  vsdk::UMat mTrainSrc;

  /*!< \brief */
  vsdk::UMat mTrainChunkOffset;
  /*!< \brief */
  vsdk::UMat mPredictSrc;
  /*!< \brief */
  vsdk::UMat mChunkOffset;
  /*!< \brief */
  vsdk::UMat mTest;

  /*!< \brief ACF process for the "LBP" descriptor extraction in train process. */
  void * mpProcessExtDescTrain;
  /*!< \brief ACF process for the "LBP" descriptor extraction in predict process. */
  void * mpProcessExtDescPredict;
  /*!< \brief ACF process for the "LBP" descriptor comparison in predict process. */
  void * mpProcessCompareDesc;
  };
} /* namespace apexcv */

#endif /* APEXCVPROLBP_H */
