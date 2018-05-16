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
*  \file apexcv_pro_canny.h
*  \brief ApexCV Canny Edge Detector
***********************************************************************************/

#ifndef APEXCVPROCANNY_H
#define APEXCVPROCANNY_H

#include <stdint.h>

// ACF HOST LIB
#include <umat.hpp>

namespace apexcv{

/*!*********************************************************************************
*  \brief ApexCV Canny Edge Detector.
*
*  apexcv::Canny is the Host-ACF interface for creating, initializing,
*  executing and releasing the \ref pagCanny on Apex.
***********************************************************************************/
class Canny
{
public:
   // Any modifications to this needs to preserve CANNY_SUCCESS as 0
   enum {
      CANNY_SUCCESS         = 0,
      CANNY_UNINITIALIZED   = 1,
      CANNY_PROCESS_NOT_RUN,
      CANNY_OUT_OF_MEMORY,
      CANNY_INVALID_ARG,
      CANNY_ACF_PROCESS_ERROR,
      CANNY_ALREADY_INITIALIZED
   };

  /**********************************************************************************
  *  Default Constructor.
  *  Allocate resources for the ACF processes.
  ***********************************************************************************/
  Canny();

  /**********************************************************************************
  *  Destructor.
  *  Release resources for the ACF processes.
  ***********************************************************************************/
  ~Canny();

/*!*********************************************************************************
*  \brief Release resources.
*   Release resources for the ACF processes and intermediate buffers.
***********************************************************************************/
   void Release();

/*!*********************************************************************************
*  \brief Initialization
*
*  Initializes the intermediate buffers needed for the processes, initializes the
*  ACF processes and connect buffers to processes' IO. The number of iterations refers 
*  to the number of times the block connection process is to be run.
***********************************************************************************/
   int Initialize(      vsdk::UMat& src    /*!< 8-bit grayscale source image */,
                        vsdk::UMat& dst    /*!< 8-bit destination image */,
                        int width                  /*!< Width of the input image */,
                        int height                 /*!< Height of the input image */,
                        uint16_t low               /*!< 16-bit low threshold for edge hysteresis */,
                        uint16_t high              /*!< 16-bit high threshold for edge hysteresis */,
                        int iters=1                /*!< Number of default extra times to run the edge promotion process. Has no effect on _processCombined()_ */
                 );

/*!*********************************************************************************
*  \brief Reconnect IO
*
*  Use this to reconnect the input and output buffers. This only needs to
*  be done if the connected Input/Outputs are changed. If only the data within (no size,
*  data pointer, or type changes), then this does not need to be called.
***********************************************************************************/
   int ReconnectIO(      vsdk::UMat& src  /*!< 8-bit grayscale source image */,
                         vsdk::UMat& dst  /*!< 8-bit destination image */,
                         int width                /*!< Width of new source image */,
                         int height               /*!< Height of new source image */
                  );

/*!*********************************************************************************
*  \brief Set Default Edge Promotion Iterations
*
*  Sets the default number of edge promotion iterations to perform.
*  This only affects the _process()_ call. Each iteration will add on more processing
*  time to the _process()_ call.  Alternatively, you can use the _promoteEdges()_ 
*  function to run extra edge promotions if you need. Has no effect on _processCombined()_
***********************************************************************************/
   int SetPromotionIterations(int iterations);

/*!*********************************************************************************
*  \brief Return the Default Edge Promotion Iterations
*
*  Returns the default number of edge promotion iterations to perform. 
***********************************************************************************/
   int RetPromotionIterations();

/*!*********************************************************************************
*  \brief Set the Edge Hysteresis Thresholds
*
*  Sets the low and high thresholds for edge hysteresis. Only affects the _process()_
*  call. This does not affect the _promoteEdges()_ call.
***********************************************************************************/
   int SetThresholds(uint16_t low, uint16_t high);

/*!*********************************************************************************
*  \brief Return the Edge Hysteresis Thresholds
*
*  Returns the low and high thresholds for edge hysteresis.
***********************************************************************************/
   void RetThresholds(uint16_t& low, uint16_t& high);

/*!*********************************************************************************
*  \brief Returns Edge Promotion Block Width
*
*  Returns the block width used in the edge promotion process.
***********************************************************************************/
   int RetBlockWidth();

/*!*********************************************************************************
*  \brief Returns Edge Promotion Block Height
*
*  Returns the block width used in the edge promotion process.
***********************************************************************************/
   int RetBlockHeight();

/*!*********************************************************************************
*  \brief Returns the image width
*
*  Returns the width of thei mage being processed
***********************************************************************************/
   int RetWidth();

/*!*********************************************************************************
*  \brief Returns the image height
*
*  Returns the height of the image being processed
***********************************************************************************/
   int RetHeight();

/*!*********************************************************************************
*  \brief Runs the Canny algorithm
*
*  This will run the Canny algorithm. Before this is called, the apexcv::Canny::Initialize
*  function must be called with the appropriate parameters.
*  
*  The input image is an 8-bit grayscale image. The Canny detection algorithm works best
*  if the image has been smoothed to get rid of noise. The output is an 8-bit grayscale
*  image with the detected edges set to 255 and non edges set to 0.
***********************************************************************************/
   int Process();

/*!*********************************************************************************
*  \brief Runs the Canny algorithm
*
*  This will run the Canny algorithm. Before this is called, the apexcv::Canny::Initialize
*  function must be called with the appropriate parameters.
*  
*  The input image is an 8-bit grayscale image. The Canny detection algorithm works best
*  if the image has been smoothed to get rid of noise. The output is an 8-bit grayscale
*  image with the detected edges set to 255 and non edges set to 0.
*
*  This will run the version where every kernel is in one graph.
*  However, this version will detect less edges than the normal process (with zero
*  extra iterations) where the kernels are split into multiple graphs.
*
***********************************************************************************/
   int ProcessCombined();

/*!*********************************************************************************
*  \brief Runs the Edge promotion extra times
*
*  This will run extra edge promotion iterations on a the previously obtained result
*  using either _promoteEdges()_ or _process()_ or _processCombined()_.
*  Only valid after running _process()_ or _processCombined()_.
***********************************************************************************/
   int PromoteEdges(int iterations);

private:
   vsdk::UMat mEdgeMapImg_a;   /*!< First full buffer for the edge map image */
   vsdk::UMat mEdgeMapImg_b;   /*!< Second full buffer for the edge map image */
   vsdk::UMat mLowThreshImg;   /*!< Low threshold image */
   vsdk::UMat mHighThreshImg;  /*!< High threshold image */

   void* mEdgeMapOal_a;  /*!< OAL Memory Address for the first edge map image */
   void* mEdgeMapOal_b;  /*!< OAL Memory Address for the second edge map image */
   void* mLowThreshOal;  /*!< OAL Memory Address for the low threshold image */
   void* mHighThreshOal; /*!< OAL Memory Address for the high threshold image */
   
   vsdk::UMat* mEdgeMapInput;  /*!< Pointer to the edge map image to be used as the input  */
   vsdk::UMat* mEdgeMapOutput; /*!< Pointer to the edge map image to be used as the output */

   bool mInit;           /*!< Indicates if the process has been initialized */

   int      mIters;          /*!< The number of initial extra edge promotion iterations to process */
   int      mWidth;          /*!< The width of the source image */
   int      mHeight;         /*!< The height of the source image */
   int32_t  mBlockWidth;     /*!< The width of the block size used in the edge promotion process */
   int32_t  mBlockHeight;    /*!< The height of the block size used in theedge promotion process */
   int      mIndex;          /*!< Which edge map image is the most up to date */
   bool     mPromoteFull;    /*!< Indicates if the promote edge full process needs to be run */
   bool     mProcessed;      /*!< Indicates if the _process()_ function has been called */
   
   void* mFullProcess;           /*!< Canny Edge Detection ACF process */
   
   void* mSuppressProcess;       /*!< Non-Maxima Suppression ACF process */
   void* mPromoteFullProcess;    /*!< Internal Block edge promotion ACF process */
   void* mPromoteProcess;        /*!< External Block edge promotion ACF process */
   void* mImageProcess;          /*!< Final image creation ACF process */
}; /* class Canny */

}  /* namespace apex */

#endif /* APEXCVPROCANNY_H */
