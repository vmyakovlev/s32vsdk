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
 *  \file
 *  \brief ApexCV Image Remap
 ***********************************************************************************/

#ifndef APEXCVPROREMAP_H
#define APEXCVPROREMAP_H

#include <umat.hpp>
#include "icp_data.h"
#include "icp_types.h"

#include <apexcv_error_codes.hpp>

namespace apexcv{

/*!*********************************************************************************
 *  \brief Interpolation options.
 ***********************************************************************************/
enum INTER_TYPE
{
   INTER_NN        = 0   /*!< Nearest neighbour interpolation. */,
   INTER_LINEAR    = 1   /*!< Bilinear interpolation. */,
   INTER_CUBIC     = 2   /*!< Bicubic interpolation over 4x4 pixel neighborhood. */,
   INTER_AREA      = 3   /*!< Resampling using pixel area relation. It may be a preferred method for image decimation, as it gives moire’-free results. But when the image is zoomed, it is similar to the INTER_NEAREST method. */,
   INTER_LANCZOS4  = 4   /*!< Lanczos interpolation over 8x8 pixel neighborhood. */
};

/*!*********************************************************************************
 *  \brief Border options.
 ***********************************************************************************/
enum BORDER_TYPE
{
   BORDER_CONSTANT   = 0   /*!< Set the border to a specified value. */,
   BORDER_REPLICATE  = 1   /*!< Replicate the border. */,
   BORDER_REFLECT    = 2   /*!< Reflect the border. */,
   BORDER_WRAP       = 3   /*!< Wrap the border. */
};


/*!*********************************************************************************
 *  \brief Apex Remap.
 *
 *  apexcv::Remap is the host-ACF interface for creating, initializing,
 *  executing and releasing \ref pagRemap "image remap" on Apex.
 ***********************************************************************************/
class Remap
{
public:

   /**********************************************************************************
    *  Default constructor.
    *  Allocate resources for the ACF process.
    ***********************************************************************************/
   Remap();

   /**********************************************************************************
    *  Destructor.
    *  Release resources for the ACF process.
    ***********************************************************************************/
   ~Remap();

   /*!*********************************************************************************
    *  \brief Initialize the remap ACF process.
    *
    *  This method is used to initialize the remap processing.  This processing will
    *  calculate the required source block size, and generate the internal offset and
    *  delta tables required for the APU implementation of resize.
    *
    *  This method must be called before process() if using a new float map. Subsequent
    *  calls to process() with the same float map can bypass this function.
    *
    *  Currently only INTERP_LINEAR and BORDER_CONSTANT with a value of 0 are supported.
    *
    *  \return Error code for the execution (zero on success).
    ***********************************************************************************/
    APEXCV_LIB_RESULT Initialize(float* map,
                  uint32_t mapWidth,
                  uint32_t mapHeight,
                  uint32_t srcWidth,
                  apexcv::INTER_TYPE interp,
                  apexcv::BORDER_TYPE border,
                  uint32_t borderValue = 0);

   /*!*********************************************************************************
    *  \brief Execute the remap ACF process.
    *
    *  This method executes the image remap which was configured by initialize().
    *
    *  \return Error code for the execution (zero on success).
    ***********************************************************************************/
   APEXCV_LIB_RESULT Process(vsdk::UMat &src,
               vsdk::UMat &dst);

   /*!*********************************************************************************
    *  \brief Generate the look-up table for top view perspective.
    *
    *  \return Error code for the execution (zero on success).
    ***********************************************************************************/
   APEXCV_LIB_RESULT GenerateLUTFromCalibLoader(const char* filename,
                                  uint32_t lDstWidth, uint32_t lDstHeight,
                                  uint32_t lSrcWidth, uint32_t lSrcHeight,
                                  uint32_t destBlockWidth, uint32_t destBlockHeight,
                                  uint32_t refSrcBlockWidth, uint32_t refSrcBlockHeight);

   /*!*********************************************************************************
    *  \brief Returns the LUTs needed for the remap ACF process.
    *
    *  This method returns the LUTs which was configured by initialize().
    *
    *  \return Error code for the execution (zero on success).
    ***********************************************************************************/
   int RetLUTs(vsdk::UMat   &rlDeltaDesc,
               vsdk::UMat   &rlLocalOffsetDesc,
               vsdk::UMat   &rlBlockOffsetDesc);

   /*!*********************************************************************************
    *  \brief Generate random float map.
    *
    *  This method generates a random float map based off the input parameter's dimensions.
    *  map must be a pointer to an appropriate sized buffer.
    *  max_offset_per_dim specifies the maximum radius of the dst pixel from the source,
    *  ie. a value of 3 means the dst pixel can be chosen from a 3x3 window centered at
    *  the x, y location of the current pixel.
    *
    ***********************************************************************************/
   void GenerateFloatMap(vsdk::UMat& input,
                         vsdk::UMat& output,
                         float* map,
                         float maxOffsetPerDim,
                         int seed);

   /*!*********************************************************************************
    *  \brief ACF scenario block descriptor struct
    *
    *  This struct describes a potential scenario to be used with ACF.
    *
    ***********************************************************************************/
   struct blockDesc
   {
      uint16_t srcWidth;     /*!< source chunk width */
      uint16_t srcHeight;    /*!< source chunk height */
      uint16_t dstWidth;     /*!< destination chunk width */
      uint16_t dstHeight;    /*!< destination chunk height */
      float ratio;            /*!< destination to source size ratio */
      int32_t memSize;       /*!< CMEM usage in bytes */
   };

   /*!*********************************************************************************
    *  \brief Chosen valid ACF scenario
    *
    *  This contains a valid configuration to be used to set the ACF scenario.
    *
    ***********************************************************************************/
   blockDesc validConfig;

protected:
   // From old stuff
   void*  mpParamLoader;
   void*  mpGroundToDewarp;
   void*  mpGroundToWarp;
   float  mTopViewPixelToMeterRatioX;
   float  mTopViewPixelToMeterRatioY;

private:
   
   void Release();

   // Containers for offeset/delta
   vsdk::UMat moOffset;    /*!< Motion comp offset buffer. */
   vsdk::UMat localOffset; /*!< CU local offset buffer. */
   vsdk::UMat delta;        /*!< Interpolation delta buffer. */

   // Pointers for the above containers
   void* localDstOal;

   void   *mRemapBlockProc;
   void   *mRemapProc;

   int32_t mInitState;

   int32_t FloatToFixedOffsets(const float *lMapRGB, vsdk::UMat &fixedOffset, const int32_t lDstStride, const int32_t lSrcStride);
   int32_t InitMoOffsets(vsdk::UMat &fixedOffset, const int32_t lSrcStride);
   int32_t FixedToLocalOffsets(vsdk::UMat &fixedOffset, const int32_t lSrcStride);
   void PopulateBlockDesc(vsdk::UMat data, blockDesc *desc);
   int32_t AdjustImageWidth(int32_t width);
   int32_t AdjustNumberOfCU(int32_t width);
};
}  /* namespace apexcv */

#endif   /* APEXCVPROREMAP_H */
