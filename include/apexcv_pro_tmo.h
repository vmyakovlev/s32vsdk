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


#ifndef APEXCVPROTMO_H
#define APEXCVPROTMO_H



#ifndef APEX2_EMULATE
#include <icp_data.h>
using namespace icp;

#else
#include "apu_lib.hpp"
#include "apu_extras.hpp"
#include "acf_lib.hpp"
using namespace APEX2;

#include "tmo_calc_geo_mean_openexr_apu_process_desc.hpp"
REGISTER_PROCESS_TYPE(APU_GEO_MEAN_OPENEXR, tmo_calc_geo_mean_openexr_apu_process_desc)

#include "tmo_calc_geo_mean_rgbe_apu_process_desc.hpp"
REGISTER_PROCESS_TYPE(APU_GEO_MEAN_RGBE, tmo_calc_geo_mean_rgbe_apu_process_desc)

#include "tmo_ldr_transform_apu_process_desc.hpp"
REGISTER_PROCESS_TYPE(APU_LDR_TRANSFORM, tmo_ldr_transform_apu_process_desc)

#endif

#include "apexcv_error_codes.hpp"
#include <umat.hpp>



namespace apexcv
{

/*!****************************************************************************
*  \brief TMO class
*
*  This class is an interface for using the tone mapping algorithm on the APEX.
******************************************************************************/

   class Tmo
   {
   public:
     /*!****************************************************************************
     *  \brief HDR Image Format
     ******************************************************************************/
     enum HDR_IMAGE_FORMAT
      {
         HDR_IMAGE_FORMAT_INVALID,      /*!< Invalid type */
         HDR_IMAGE_FORMAT_RGBE,      /*!< Datatype is 08U. e0 size is (4, 1). Channel Order is RGBE */
         HDR_IMAGE_FORMAT_OPENEXR    /*!< Datatype is 16U. e0 size is (3, 1). Channel Order is RGB */
      };

   public:
      /******************************************************************************
      *  Allocate resources for the ACF processes.
      ******************************************************************************/
      Tmo();

      /**********************************************************************************
      *  Release resources for the ACF processes..
      ***********************************************************************************/
      ~Tmo();


      /*!****************************************************************************
      *  \brief Initializes TMO process, Connects the buffer to the process port, and allocates/initializes any internal buffers.
      *
      ******************************************************************************/
      APEXCV_LIB_RESULT Initialize(vsdk::UMat&            arHdrImage,       /*!< Input image. Datatype must correspond to "acHdrImageFormat" */
                                   HDR_IMAGE_FORMAT const acHdrImageFormat, /*!< Input image format. "arHdrImage" datatype must correspond to "acHdrImageFormat" */
                                   vsdk::UMat&            arLdrImage,       /*!< Output image. Datatype is 08U. e0 size is (3, 1). */
                                   vsdk::UMat&            arLdrTransformKey /*!< Key value used during LDR transform. Datatype is 08U. */
                                  );


      /*!****************************************************************************
      *  \brief Reconnects the input/output to TMO process for RGBE input.
      ******************************************************************************/
      APEXCV_LIB_RESULT ReconnectIO(vsdk::UMat&            arHdrImage,       /*!< Input image. Datatype must correspond to "acHdrImageFormat" */
                                    HDR_IMAGE_FORMAT const acHdrImageFormat, /*!< Input image format. "arHdrImage" datatype must correspond to "acHdrImageFormat" */
                                    vsdk::UMat&            arLdrImage,       /*!< Output image. Datatype is 08U. e0 size is (3, 1). */
                                    vsdk::UMat&            arLdrTransformKey /*!< Key value used during LDR transform. Datatype is 08U. */
                                   );


      /*!****************************************************************************
      *  \brief Run APEX-TMO process.
      *
      *  Generate LDR image from HDR image.
      *
      *  Supported datatypes are:
      *     - unsigned 8 bit image to unsigned 8 bit image (RGBE input)
      *     - unsigned 16 bit image to unsigned 8 bit image (OpenEXR input)
      *
      *  \return Error code (zero on success).
      ***********************************************************************************/
      APEXCV_LIB_RESULT Process();


      /*!***************************************************************************
       *  \brief Select APEX Core for processing
       *  \return Error code for the ACF configuration (APEXCV_SUCCESS on success).
       *
       *  Select which APEX core (0 or 1) to be selected to run the processing.
       *  This function has to be called after Initialize() and can be executed per
       *  frame base.
       *****************************************************************************/
       APEXCV_LIB_RESULT SelectApexCore(int32_t aApexId /*!< The ID of the desired APEX (e.g if there are 2 APEXs,
                                                             valid values for aApexId would be 0 and 1).*/);


   private:
      /*!*********************************************************************************
      *  \brief Release Resources
      *
      *  Releases the internal buffers.
      ***********************************************************************************/
      void Release();          /*!< Release resources for the ACF processes. */

      bool             mIsInitialized;    /*!< Flag indicating the TMO process has been initialized. */
      HDR_IMAGE_FORMAT mHdrImageFormat;   /*!< Enumeration indicating the format of the input HDR image. */
      int32_t          mApexId;           /*!< Index of the APEX core selected to run the algorithm*/
      
      vsdk::UMat mDivValue;     /*!< Pointer to the buffer for division value. */
      vsdk::UMat mIntermExp;    /*!< Pointer to the buffer for intermediate exponent. */
      vsdk::UMat mIntermMan;    /*!< Pointer to the buffer for intermediate mantissa. */
      vsdk::UMat mWorldExp;     /*!< Pointer to the buffer for world luminance exponent. */
      vsdk::UMat mWorldMan;     /*!< Pointer to the buffer for world luminance mantissa. */
      vsdk::UMat mGeoMeanExp;   /*!< Pointer to the buffer for geometric mean exponent. */
      vsdk::UMat mGeoMeanMan;   /*!< Pointer to the buffer for geometric mean mantissa. */


      /*!*********************************************************************************
      *  \brief ACF process for the \ref secTMO "TMO" geometric mean calculation process for RGBE input.
      ***********************************************************************************/
      void* mProcessGeoMeanRgbe;

      /*!*********************************************************************************
      *  \brief ACF process for the \ref secTMO "TMO" geometric mean calculation process for OpenEXR input.
      ***********************************************************************************/
      void* mProcessGeoMeanExr;

      /*!*********************************************************************************
      *  \brief ACF process for the \ref secTMO "TMO" LDR transform process.
      ***********************************************************************************/
      void* mProcessLdrTransform;
   };

} /* namespace apexcv */

#endif /* APEXCVPROTMO_H */
