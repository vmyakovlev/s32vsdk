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
*  \file apexcv_pro_fast.h
*  \brief ApexCV Host wrapper for fast corner detector algorithm
******************************************************************************/

#ifndef APEXCVPROFAST_H
#define APEXCVPROFAST_H

#include <umat.hpp>
#include "apexcv_error_codes.hpp"
#include "acf_process_apu.h"

#define FAST_MAX_LIST_ELEMENTS 8192

namespace apexcv
{
  /****************************************************************************
   * \brief The available FAST ACF processes.
   ***************************************************************************/
  enum FAST_PROCESS_TYPE_E
  {
    FAST_NONE        = 0,     /*!< No ACF process. */
    FAST_PROCESS_IMG = 1, /*!< ACF process that runs FAST and outputs an
                               image where pixel values can be either
                               0 (non-corner) or 1 (corner). */
    FAST_PROCESS_SERIALIZED, /*!< ACF process that runs FAST and outputs a
                                  list with the coordinates of the detected
                                  corner pixels. */
    FAST_PROCESS_IMG_NMS /*!< FAST_PROCESS_IMG that also implements
                              non maximum supression. */
  };

  /*!**************************************************************************
  *  \brief FAST Corner Detection.
  *
  *  apexcv::Fast is the host-ACF interface for creating, initializing,
  *  executing and releasing the \ref pagFAST on Apex.
  ****************************************************************************/
  class Fast
  {
public:

  /***************************************************************************
  *  Default constructor.
  ****************************************************************************/
  Fast();

  /***************************************************************************
  *  Destructor.
  ****************************************************************************/
  ~Fast();

  /*!**************************************************************************
  * \brief Initializes the process parameters and allocates ACF resources.
  * \param[out] aDst Output buffer. Depending on the value of acOutIsList
  *             it can be:
  *                  - an image where pixel values are:
  *                      - 0 for non-corner
  *                      - 1 for corner
  *                  OR
  *                  - a list of (x, y) pairs representing the coordinates
  *                    of the detected corner pixels.
  * \param[in]  aSrc Source image.
  * \param[in]  acThreshold "t" threshold for the FAST algorithm.
  * \param[in]  acNonMaxSupp enable or not non maximum suppression.
  * \param[in]  acCircumference FAST circle circumference (e.g. 8, 12 or 16).
  * \param[out] acOutIsList If true, then the algorithm will output a list
  *             with the coordinates of the detected corners. If false, then
  *             the algorithm will output an image with values of
  *             0 (non-corner) and 1 (corner).
  *  \return Error code for the initialization - see apexcv_error_codes.hpp
  ****************************************************************************/
  APEXCV_LIB_RESULT Initialize(vsdk::UMat& aDst,
                               vsdk::UMat& aSrc,
                               const int acThreshold,
                               const bool acNonMaxSupp,
                               const int acCircumference,
                               const bool acOutIsList);

  /*!**************************************************************************
  *  \brief Launches the process for the FAST algorithm and waits for completion.
  *  \return Error code for the execution - see apexcv_error_codes.hpp
  ****************************************************************************/
  APEXCV_LIB_RESULT Process();

  /*!**************************************************************************
  *  \brief Launches the process for the FAST algorithm and immediately returns,
  *  allowing for other tasks to be run on the host.
  *  To wait for process completion call ProcessWait().
  *  Make sure that every call to ProcessNoPolling() is paired with a call to
  *  ProcessWait().
  *  \return Error code for the execution - see apexcv_error_codes.hpp
  ****************************************************************************/
  APEXCV_LIB_RESULT ProcessNoPolling();

  /*!**************************************************************************
  *  \brief Waits for a FAST process previously launched with ProcessNoPolling()
  *  to finish.
  *  \return Error code for the execution - see apexcv_error_codes.hpp
  ****************************************************************************/
  APEXCV_LIB_RESULT ProcessWait();

  /*!**************************************************************************
  *  \brief Reinitializes the ACF process graph connections.
  *  Used for reconnecting the input and output buffers. This operation is
  *  needed only if the connected Input/Outputs are changed. This operation is
  *  not needed if only the data values change, without any change of pointers
  *  or sizes.
  *  \return Error code for ACF graph process reinitialization - see
  *  apexcv_error_codes.hpp
  ****************************************************************************/
  APEXCV_LIB_RESULT ReconnectIO(vsdk::UMat& aDst,
                                vsdk::UMat& aSrc,
                                const int acThreshold,
                                const bool acNms,
                                const int acCircumference,
                                const int acOutIsList);

  /*!**************************************************************************
  *  \brief SelectApuConfiguration
  *  Selects APU and APEX configuration.
  *  \return Error code for updating the APU configuration - see apexcv_error_codes.hpp
  ****************************************************************************/
  APEXCV_LIB_RESULT SelectApuConfiguration(ACF_APU_CFG aApuConfig, int32_t aApexId);

  /*!**************************************************************************
  *  \brief Returns the number of features detected by FAST
  *  If the output is not a list the latency is very high !
  ****************************************************************************/
  int GetNrOfFeatures();

private:
  vsdk::UMat *mpOut; /*!< \brief Reference to the output image buffer */

  unsigned int mInit; /*!< \brief Indicates if it has been initialized or not */
  bool mNonMaxSupression; /*!< \brief Indicates if NMS is on or off */
  bool mOutIsAList; /*!< \brief Indicates if the output is a list */
  int mProcessType; /*!< \brief Indicates the process type used */
  int mImgWidth; /*!< \brief Image width */
  int mImgHeight; /*!< \brief Image height */

  ACF_APU_CFG mApuConfig; /*!< \brief APU configuration for the ACF process */
  int32_t mApexId; /*!< \brief APEX core to be used for running the process */

  vsdk::UMat mCounter; /*!< \brief Keeps track of the number of keypoints, only if outIsAList == true */
  vsdk::UMat mThreshold; /*!< \brief Contains the minimal threshold for corner detection */
  vsdk::UMat mCircumference; /*!< \brief The circumference of FAST test */

  void *mpProcess; /*!< \brief Pointer to FAST processes */
  };
} /* namespace apexcv */

#endif /* APEXCVPROFAST_H */
