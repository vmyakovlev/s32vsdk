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
*  \file apexcv_pro_blockmatching.h
*  \brief ApexCV Host wrapper for block matching algorithm
*******************************************************************************/

#ifndef APEXCVBLOCKMATCHING_H
#define APEXCVBLOCKMATCHING_H

// ACF HOST LIB
#include <icp_types.h>
#include <icp_data.h>

#include <umat.hpp>
#include "apexcv_error_codes.hpp"

// Do not modify without modifying kernel wrapper
#define DEFAULT_TEMPLATE_SIZE      16
#define DEFAULT_WINDOW_SIZE        28

namespace apexcv {

/*!****************************************************************************
*  \brief Blockmatching class
*
*  This class is an interface for using the block matching algorithm on the APEX.
******************************************************************************/
class Blockmatching
{
public:

/*!****************************************************************************
*  \brief Default constructor.
*
*  Allocate resources for the ACF processes.
******************************************************************************/
  Blockmatching();

/*!*********************************************************************************
*  \brief Destructor.
*
*   Release resources for the ACF processes.
***********************************************************************************/
  ~Blockmatching();

/*!*********************************************************************************
*  \brief Initialize the block matching.
*
*  Initializes the internal buffers for the process.
*  The _lOutputPoints_ and _lInputPoints_ are 32-bit packed values of form [hi, low] [Y_16S, X_16S]
*  The effective search window size is:
*     search_width  = (wsx-tsx+1)*(ncu_x-1) + wsx;
*     search_height = (wsy-tsy+1)*(ncu_y-1) + wsy;
*
*  The search window will be centered around the tracked point.
*
***********************************************************************************/
  APEXCV_LIB_RESULT Initialize(vsdk::UMat& aOutputPoints   /*!< The output tracked points (X, Y) */,
                 vsdk::UMat& aOutputStatus   /*!< Indicates if the point's SAD score is below the threshold */,
           const vsdk::UMat& aInputTemplate  /*!< Template image */,
           const vsdk::UMat& aInputWindow    /*!< Search Image */,
           const vsdk::UMat& aInputPoints    /*!< Set of points on the template and will be used to specify the locations on the search image */,
                 int aSad_threshold                   /*!< Maximum SAD score - Capped to 65535 */,
                 int aTrackedPoints=-1                /*!< Number of points to track.  Default -1: Uses size of _lInputPoints_ */,
                 int aTsx=DEFAULT_TEMPLATE_SIZE       /*!< Template window columns */,
                 int aTsy=DEFAULT_TEMPLATE_SIZE       /*!< Template window rows*/,
                 int aWsx=DEFAULT_WINDOW_SIZE         /*!< Search window columns */,
                 int aWsy=DEFAULT_WINDOW_SIZE         /*!< Search window rows */,
                 int aNcu_x=1                         /*!< Number of CU's to use for the horizontal search window per tracked point*/,
                 int aNcu_y=1                         /*!< Number of CU's to use for the vertical search window per tracked point*/
                 );

/*!*********************************************************************************
*  \private
*  \brief Set parameters, only after release().
***********************************************************************************/
  APEXCV_LIB_RESULT SetParameters(
                    int aSad_threshold                   /*!< Maximum SAD score */,
                    int aTsx                             /*!< Template window columns */,
                    int aTsy                             /*!< Template window rows*/,
                    int aWsx                             /*!< Search window columns */,
                    int aWsy                             /*!< Search window rows */,
                    int aNcu_x                           /*!< Number of CU's to use for the horizontal search window per tracked point*/,
                    int aNcu_y                           /*!< Number of CU's to use for the vertical search window per tracked point*/
                    );


/*!*********************************************************************************
*  \brief Match the blocks
*
*  The block matching process uses the Sum of Absolute Differences algorithm to
*  perform block matching. The process will search in the "new" image within a window
*  around the search points in the "previous" image. The template size determines how
*  many pixels are used in the SAD calculation. The window size determines the area
*  in which the template is used to perform SAD calculations.
*  
*  Default Template Size: 16x16
*     - Must be multiple of 4
*  Default Window Size: 28x28
*     - Must be multiple of 4
*  In total template_x*template_y + window_x*window_y must be <= ~1800
***********************************************************************************/
  APEXCV_LIB_RESULT Process(int aTracked_points=-1);
   
/*!*********************************************************************************
*  \brief Release Resources
*
*  Releases the internal buffers and resets the class state to initial.
***********************************************************************************/
  void Release();
   
/*!*********************************************************************************
*  \brief Reconnect IO
*
*  Use this to reconnect the input and output buffers. This only needs to
*  be done if the connected Input/Outputs are changed. If only the data within (no size,
*  data pointer, or type changes), then this does not need to be called.
***********************************************************************************/
  APEXCV_LIB_RESULT ReconnectIO(vsdk::UMat& aOutputPoints   /*!< The output tracked points (X, Y) */,
                  vsdk::UMat& aOutputStatus   /*!< Indicates if the point's SAD score is below the threshold */,
            const vsdk::UMat& aInputTemplate  /*!< Template image */,
            const vsdk::UMat& aInputWindow    /*!< Search Image */,
            const vsdk::UMat& aInputPoints    /*!< Set of points on the template and will be used to specify the locations on the search image */,
                          int aTrackedPoints=-1 /*!< Number of points to track.  Default -1: Uses size of _lInputPoints_ */);
   
/*!*********************************************************************************
*  \brief Set SAD Threshold
*
*  Change the SAD threshold
***********************************************************************************/
  APEXCV_LIB_RESULT SetSadThreshold(int sadThreshold /*!< Maximum SAD score - Capped to 65535 */);
   
private:
   
  APEXCV_LIB_RESULT AllocateBuffers(int aMaxPoints);
   
  void* mProcess;                /*!< The ACF Blockmatching Process */
  vsdk::UMat mInputTemplate;     /*!< Keep track of the Input Template image descriptor */
  vsdk::UMat mInputWindow;       /*!< Keep track of the Input Window image descriptor */
  vsdk::UMat mInputPoints;       /*!< Keep track of the Input Points descriptor */
  vsdk::UMat mOutputStatus;      /*!< Keep track of the Output Status descriptor */
  vsdk::UMat mOutputPoints;      /*!< Keep track of the Output Points descriptor */

  vsdk::UMat mOffTemplate;               /*!< UMat for template offsets */
  vsdk::UMat mOffWindow;                 /*!< UMat for window offsets */
  vsdk::UMat mScore;                     /*!< UMat for internal score buffer */
  vsdk::UMat mOutput;                    /*!< UMat for internal output buffer */

  bool mInit;                            /*!< Indicates if the class has been intialized */
  int mNumPoints;                        /*!< The number of points in the "previous" image */
  int mSadThreshold;                     /*!< Maximum SAD score */
  int mTX;                               /*!< Template width */
  int mTY;                               /*!< Template height */
  int mWX;                               /*!< Window width */
  int mWY;                               /*!< Window height */
  int mNX;                               /*!< Number of CU's used for horizontal search window */
  int mNY;                               /*!< Number of CU's used for vertical search window */
  int mMaxPoints;                        /*!< Current allocated size of internal buffers */
};

} /* namespace apexcv */

#endif /* APEXCVBLOCKMATCHING_H */
