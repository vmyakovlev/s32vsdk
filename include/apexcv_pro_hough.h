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

/*******************************************************************************
*  \file apexcv_pro_hough.h
*  \brief Apex Hough Line Detector.
*******************************************************************************/

#ifndef APEXCVPROHOUGH_H
#define APEXCVPROHOUGH_H

// ACF HOST LIB
#include <icp_data.h>
#include <apexcv_error_codes.hpp>
#include <acf_process_apu.h>

#define HOUGH_PERF_PROB 0
#if (HOUGH_PERF_PROB != 0)
enum
{
  HOUGH_INIT = 0,
  HOUGH_PROCESS,
  HOUGH_RECONN,
  HOUGH_PROFILED_FUNC_CNT
};

enum
{
  Hough_Init_Ref = 0,
  Hough_Init_Mem_Alloc,
  Hough_Init_Process_Init
};

enum
{
  Hough_Reconn_Ref = 0,
  Hough_Reconn_Dealloc,
  Hough_Reconn_Run
};

enum
{
  Hough_Process_Ref = 0,
  Hough_Process_Gen_Trig_Tables,
  Hough_Process_Run
};

#define PROBES_CNT 6
extern volatile int64_t gHoughTime[HOUGH_PROFILED_FUNC_CNT][PROBES_CNT];
extern AcfProfilingInfo gAcfInfoHough;
#endif

namespace apexcv
{
/*!*****************************************************************************
*  \brief Apex Hough Line Detector.
*
*  apexcv::HoughLineDetector is the host-ACF interface for creating, initializing,
*  executing and releasing the \ref pagHough on Apex.
*******************************************************************************/
  class HoughLineDetector
  {
public:
/*!*****************************************************************************
*  \brief Packed line format for \ref apexcv::HoughLineDetector "Hough Line Detector".
*
*  Detected lines are stored in 32 bits.  The first 8 bits are the
*  \ref GetThetaId "angle index".  The next 12 bits are the \ref GetRhoId "rho index".
*  The last 12 bits are the \ref GetAccumulator "Hough accumulator value".
*******************************************************************************/
  typedef uint32_t PackedLine;

/*!*****************************************************************************
*  \brief Non-maxima suppression flag.
*  To disable non-maxima suppression (nms), use \ref NMS_NONE.  To use nms on
*  \ref Line::rho "rho" only, use \ref NMS_RHO.  To use nms on \ref Line::theta "theta"
*  only, use \ref NMS_THETA.  To use nms on both, use \ref NMS_RHO | \ref NMS_THETA.
*******************************************************************************/
  enum NonMaxSupFlag
  {
    NMS_NONE  = 0 /*!< No non-maxima suppression. */,
    NMS_RHO   = 1 /*!< Non-maxima suppression on rho.    Since the rho step is 1 pixel, this flag should always be used. */,
    NMS_THETA = 2 /*!< Non-maxima suppression on theta.  This flag should be used when the angle resolution small. */
  };

/*!*****************************************************************************
*  \brief The available Hough ACF processes.
*******************************************************************************/
  enum Process
  {
    PROC_NONE = 0 /*!< No ACF process. */,
    PROC_40X1 = 1 /*!< ACF process with block size 40x1. */,
    PROC_20X2 = 2 /*!< ACF process with block size 20x2. */,
    PROC_10X4 = 4 /*!< ACF process with block size 10x4. */,
    PROC_6X4  = 8 /*!< ACF process with block size 6x4. */
  }; /*!< Enumeration of the available ACF processes. */

/*!*****************************************************************************
*  \brief Line data structure associated with the
*  \ref apexcv::HoughLineDetector "Hough Line Detector".
*******************************************************************************/
  struct Line
  {
/*!*****************************************************************************
*  \brief Default constructor.
*******************************************************************************/
    Line(int rho_ = 0 /*!< Nearest distance of the line to center of the image. */,
         float theta_ = 0.f /*!< Angle of the line's normal. */)
      : rho(rho_), theta(theta_)
    {
    };

    int rho; /*!< \brief Nearest distance of the line to center of the image. */
    float theta; /*!< \brief Angle of the line's normal. */
  };

/*******************************************************************************
*  Default constructor.
*  Allocate resources for the Hough transform ACF process.
*******************************************************************************/
  HoughLineDetector();

/*******************************************************************************
*  Destructor.
*  Release resources for the Hough transform ACF process.
*******************************************************************************/
  ~HoughLineDetector();

/*!*****************************************************************************
*  \brief Initialize parameters and allocate resources.
*  \param[in] aImage Input image
*  \param[in] aPixelThreshold Threshold to qualify as pixel from Accm
*  \param[in] aAccThreshold Hough accumulator threshold. Min number of collinear
*             pixels from a line
*  \param[in] aThetaCount Number of angles to detect
*  \param[in] apTheta Angles to detect
*  \param[in] aNonMaxSupp Non-maxima suppression flag
*  \return Please check apexcv_error_codes.hpp
*******************************************************************************/
  APEXCV_LIB_RESULT Initialize(vsdk::UMat &aImage,
                               int aPixelThreshold = 127,
                               int aAccThreshold = 100,
                               int aThetaCount = 180,
                               float *apTheta = NULL,
                               int aNonMaxSupp = (NMS_RHO | NMS_THETA));

/*!*****************************************************************************
*  \brief Initialize parameters and allocate resources.
*  \param[in] aImage Input image
*  \param[in] aPixelThreshold Threshold to qualify as pixel from Accm
*  \param[in] aAccThreshold Hough accumulator threshold.
*             Min number of collinear pixels from a line
*  \param[in] aThetaCount Number of angles to detect
*  \param[in] aThetaStart Starting angle for detection
*  \param[in] aThetaStep Incremental angle step
*  \param[in] aNonMaxSupp Non-maxima suppression flag
*  \return Please check apexcv_error_codes.hpp
*******************************************************************************/
  APEXCV_LIB_RESULT Initialize(vsdk::UMat &aImage,
                               int aPixelThreshold,
                               int aAccThreshold,
                               int aThetaCount,
                               double aThetaStart,
                               double aThetaStep,
                               int aNonMaxSupp = (NMS_RHO | NMS_THETA));

/*!*****************************************************************************
*  \brief Reinitializes the ACF process graf connections.
*  \param[in] aImage Input image
*  \param[in] aPixelThreshold Threshold to qualify as pixel from Accm
*  \param[in] aAccThreshold Hough accumulator threshold.
*             Min number of collinear pixels from a line
*  \param[in] aThetaCount Number of angles to detect
*  \param[in] apTheta Angles to detect
*  \param[in] aNonMaxSupp Non-maxima suppression flag
*  \return Please check apexcv_error_codes.hpp
*******************************************************************************/
  APEXCV_LIB_RESULT ReconnectIO(vsdk::UMat &aImage,
                                int aPixelThreshold,
                                int aAccThreshold,
                                int aThetaCount,
                                float *apTheta,
                                int aNonMaxSupp = (NMS_RHO | NMS_THETA));

/*!*****************************************************************************
*  \brief Reinitializes the ACF process graf connections.
*  \param[in] aImage Input image
*  \param[in] aPixelThreshold Threshold to qualify as pixel from Accm
*  \param[in] aAccThreshold Hough accumulator threshold.
*             Min number of collinear pixels from a line
*  \param[in] aThetaCount Number of angles to detect
*  \param[in] aThetaStart Starting angle for detection
*  \param[in] aThetaStep Incremental angle step
*  \param[in] aNonMaxSupp Non-maxima suppression flag
*  \return Please check apexcv_error_codes.hpp
*******************************************************************************/
  APEXCV_LIB_RESULT ReconnectIO(vsdk::UMat &aImage,
                                int aPixelThreshold,
                                int aAccThreshold,
                                int aThetaCount,
                                double aThetaStart,
                                double aThetaStep,
                                int aNonMaxSupp = (NMS_RHO | NMS_THETA));

/*!*****************************************************************************
*  \brief APEX hardware configuration
*  \param[in] aApexId Apex id where the code will execute
*  \param[in] aApuConfig Apu CU size
*  \return Please check apexcv_error_codes.hpp
*******************************************************************************/
  APEXCV_LIB_RESULT SelectApuConfiguration(ACF_APU_CFG aApuConfig = ACF_APU_CFG__DEFAULT,
                                           int32_t aApexId = 0);

/*!*****************************************************************************
*  \brief Release resources and resets parameters.
*  \return Please check apexcv_error_codes.hpp
*******************************************************************************/
  APEXCV_LIB_RESULT Release();

/*!*****************************************************************************
*  \brief Set the pixel threshold for line detection.
*  \param[in] aPixelThreshold Pixel greather than aPixelThreshold are used
*  \return Error code for initialization (zero on success).
*******************************************************************************/
  APEXCV_LIB_RESULT SetPixelThreshold(int aPixelThreshold);

/*!*****************************************************************************
 *  \brief Set the Hough accumulator threshold for line detection.
 *  \param[in] aAccThreshold Minimum number of collinear pixels
 *                                 needed for line detection
 *  \return Error code for initialization (zero on success).
 *****************************************************************************/
  APEXCV_LIB_RESULT SetAccumThreshold(int aAccThreshold);

/*!*****************************************************************************
*  \brief Specify the number of angles to be detected.
*  \param[in] aThetaCount Number of angles to detect
*  \param[in] apThetaData Angles to detect.
                        If the pointer is null, the full range of angles is
                        equally partitioned by aThetaCount
*  \param[in] aNonMaxSupp Non maximum supression flag
*  \return Error code for initialization (zero on success).
*******************************************************************************/
  APEXCV_LIB_RESULT SetTheta(int aThetaCount,
                             float *apThetaData = NULL,
                             int aNonMaxSupp = (NMS_RHO | NMS_THETA));

/*!*****************************************************************************
*  \brief Specify the number of angles to be detected.
*  \param[in] aThetaCount Number of angles to detect
*  \param[in] aThetaStart Starting angle for detection
*  \param[in] aThetaStep Incremental angle step
*  \param[in] aNonMaxSupp Non maximum supression flag
*  \return Error code for initialization (zero on success).
*******************************************************************************/
  APEXCV_LIB_RESULT SetTheta(int aThetaCount,
                             double aThetaStart,
                             double aThetaStep,
                             int aNonMaxSupp);

/*!*****************************************************************************
*  \brief Detect lines on the image.
*  Lines are detected based on the parameters specified by \ref Initialize,
*  \ref SetPixelThreshold, \ref SetAccumThreshold and \ref SetTheta.
*  The number of detected lines is accessed by \ref GetLineCount. Packed lines are
*  accessed by \ref GetPackedLineData. Line coordinates are accessed by \ref Line
*  \return Please check apexcv_error_codes.hpp
*******************************************************************************/
  APEXCV_LIB_RESULT Process();

/*!*****************************************************************************
*  \brief Get the number of rho values.
*  This is the size of the Hough accumulator in CMEM
*  It is given by \f$ \textrm{round} \left( \sqrt{w^2 + h^2} \right) + 1 \f$,
*  where \f$ (w, h) \f$ is the image width and height.
*  \return The number of rho values.
*******************************************************************************/
  int GetRhoCount();

/*!*****************************************************************************
*  \brief Get the starting rho value for the Hough accumulator.
*  This is the offset that must be applied to rhoID "rho index" to obtain
*  the true rho value.  That is, rho = rhoID + rhoStart.
*  \return The starting rho value for the Hough accumulator.
*******************************************************************************/
  int GetRhoStart();

/*!*****************************************************************************
*  \brief Get the number of angles to detect.
*  \return The number of angles to detect.
*******************************************************************************/
  int GetThetaCount();

/*!*****************************************************************************
*  \brief Get a pointer to angles to detect.
*  \return A pointer to angles to detect.
*******************************************************************************/
  float *GetThetaData();

/*!*****************************************************************************
*  \brief Get the non-maxima suppression flag.
*  \return \ref NonMaxSupFlag "The non-maxima suppression flag".
*******************************************************************************/
  int GetNmsFlag();

/*!*****************************************************************************
*  \brief Get the number of detected lines.
*  \return The number of detected lines.
*******************************************************************************/
  int GetLineCount();

/*!*****************************************************************************
*  \brief Get a pointer to \ref PackedLine "packed line" data.
*  \return A pointer to \ref PackedLine "packed line" data.
*******************************************************************************/
  PackedLine *GetPackedLineData();

/*!*****************************************************************************
*  \brief Get the \ref Line "line coordinates" for a detected line.
*  \param[in] aIndex Index of the detected line.
*                          This value must be within [0, \ref GetLineCount]
*  \return The \ref Line "line coordinates" for a detected line.
*******************************************************************************/
  Line GetLine(int aIndex);

/*!*****************************************************************************
*  \brief Get the \ref Line "line coordinates" for a detected line.
*  \param[in] aPackedLine Packed info about the detected line
*  \return The \ref Line "line coordinates" for a detected line.
*******************************************************************************/
  Line GetLine(PackedLine aPackedLine);

/*!*****************************************************************************
*  \brief Check the validity of Hough initialization parameters.
*  \param[in] aImageCols Image columns
*  \param[in] aImageRows Image rows
*  \param[in] aPixelThreshold Threshold to qualify a pixel for the Accm
*  \param[in] aAccThreshold Min number of collinear pixels for a line
*  \param[in] aThetaCount Number of angles to detect
*  \return Error code for initialization (zero on success).
*******************************************************************************/
  APEXCV_LIB_RESULT CheckParameters(int aImageCols,
                                    int aImageRows,
                                    int aPixelThreshold,
                                    int aAccThreshold,
                                    int aThetaCount);

/*!*****************************************************************************
*  \brief Get the Hough accumulator value from the \ref PackedLine.
*  \param[in] aLine Line to get information for
*  \return Hough accumulator value. The number of pixels inside a given line
*******************************************************************************/
  static int GetAccumulator(PackedLine aLine);

/*!*****************************************************************************
*  \brief Get the rho index from the \ref PackedLine.
*  \param[in] aLine Line to get information for
*  \return The rho index.  The rho index \f$ r >= 0 \f$ is the index of Hough
*  transform for the line.
*******************************************************************************/
  static int GetRhoId(PackedLine aLine);

/*!*****************************************************************************
*  \brief Get the angle index from the \ref PackedLine.
*  \param[in] aLine Line to get information for
*  \return The angle index.  The angle index  i in [0, 255] corresponds to
*  \f$ i^{\textrm{th}} \f$ angle specified by
*  \ref SetTheta "SetTheta".
*******************************************************************************/
  static int GetThetaId(PackedLine aLine);

  static int mCuCnt; /*!< \brief The number of CUs */
  static int mMaxRhoCnt; /*!< \brief Maximum range of rho values. */
  static int mMaxLinesPerIt; /*!< \brief Max number of detected lines */
  static double mcDeg2Rad; /*!< \brief Conv. from degrees to radians */
  static double mcRad2Deg; /*!< \brief Conv, from radians to degrees */
  static double mcPi; /*!< \brief Universal constant */

private:
  APEXCV_LIB_RESULT Allocate(); /*!< \brief Allocate resources for the ACF processes. */

/*!*****************************************************************************
*  \brief Connect inputs/output for ACF process graf
*  \param[in] aImage Input image
*  \return Error code for connection (zero on success).
*******************************************************************************/
  APEXCV_LIB_RESULT ConnectIO(vsdk::UMat &aImage);

/*!*****************************************************************************
*  \brief Select the ACF process.
*  \param[in] aProcessID Index of the process identifier
*  \return Error code for selection (zero on success).
*******************************************************************************/
  APEXCV_LIB_RESULT SelectProcess(int aProcessID);

/*!*****************************************************************************
*  \brief Set the Hough accumulator size.
*  \param[in] aRhoCount Range of Rho values
*  \return Error code for initialization (zero on success).
*******************************************************************************/
  APEXCV_LIB_RESULT SetRhoCount(int aRhoCount);

/*!*****************************************************************************
*  \brief Set the number of angles to detect.
*  \param[in] aThetaCount Number of angles to detect, max is 256
*  \param[in] aNonMaxSupp NMS enable flag
*  \return Error code for initialization (zero on success).
*******************************************************************************/
  APEXCV_LIB_RESULT SetThetaCount(int aThetaCount, int aNonMaxSupp);

/*!*****************************************************************************
*  \brief Set the image size the detector will receive.
*  \param[in] aImageCols Image columns
*  \param[in] aImageRows Image rows
*  \return Error code for initialization (zero on success).
*******************************************************************************/
  APEXCV_LIB_RESULT SetImageSize(int aImageCols,
                                 int aImageRows);

  unsigned int mInit; /*!< \brief Initialization state for all ACF processes. */
  void * mpSelectedProcess; /*!< \brief Pointer to ACF process */
  int mProcessType; /*!< \brief Indicates which process was selected */
  
  vsdk::UMat * mpInputImage; /*!< \brief UMat pointer to input img */
  int mImageCols; /*!< \brief Expected number of image columns. */
  int mImageRows; /*!< \brief Expected number of image rows. */
  int mRhoStart; /*!< \brief Start value for rho */
  int mThetaCount; /*!< \brief Number of angles to detect. */
  int mIterations; /*!< \brief Number of process iterations
                               to detect all the angles. */
  int mLineCount; /*!< \brief Number of lines detected. */

  void *mpRhoCountOAL; /*!< \brief Oal handler */
  icp::DataDescriptor mRhoCount; /*!< \brief Storage for the rho count. */

  void *mpCuCountOAL; /*!< \brief Oal handler */
  icp::DataDescriptor mCuCount; /*!< \brief Storage for number of CUs. */

  void *mpPixelThresholdOAL; /*!< \brief Oal handler */
  icp::DataDescriptor mPixelThreshold; /*!< \brief Storage for pixel threshold*/

  void *mpPixelOffsetParamsOAL; /*!< \brief Oal handler */
  icp::DataDescriptor mPixelOffsetParams; /*!< \brief Storage for pixel offset*/

  void *mpCuIndexParamsOAL; /*!< \brief Oal handler */
  icp::DataDescriptor mCuIndexParams; /*!< \brief Storage for CU index parameters */

  void *mpLineThresholdParamsOAL; /*!< \brief Oal handler */
  icp::DataDescriptor mLineThresholdParams; /*!< \brief Storage threshold param */

  void *mpLineCountOAL; /*!< \brief Oal handler */
  icp::DataDescriptor mLineCountDesc; /*!< \brief Storage detected lines */

  void *mpThetaOAL; /*!< \brief Oal handler */
  float *mpTheta; /*!< \brief Pointer to angles to detect. */

  void *mpCosTableOAL; /*!< \brief Oal handler */
  int16_t *mpCosTable; /*!< \brief Pointer to fixed point cosine table. */
  icp::DataDescriptor mCosTable; /*!< \brief Storage cos table. */

  void *mpSinTableOAL; /*!< \brief Oal handler */
  int16_t *mpSinTable; /*!< \brief Pointer to fixed point sine table. */
  icp::DataDescriptor mSinTable; /*!< \brief Storage sin table. */

  void *mpLinesOAL; /*!< \brief Oal handler */
  PackedLine *mpLines; /*!< \brief Pointer to detected lines. */
  icp::DataDescriptor mLines; /*!< \brief Storage for detected lines. */

  };
} /* namespace apexcv */

#endif  /* APEXCVPROHOUGH_H */
