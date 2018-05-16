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
*  \brief ApexCV Resize Library.
***********************************************************************************/

#ifndef APEXCVPRORESIZE_H
#define APEXCVPRORESIZE_H

#include <icp_data.h>
#include <umat.hpp>
#include <acf_process_apu.h>
#include <apexcv_error_codes.hpp>


namespace apexcv{

/*!*********************************************************************************
*  \brief Apex Resize.
*
*  apexcv::Resize is the host-ACF interface for creating, initializing,
*  executing and releasing \ref pagResize "image resize" on Apex.
*  Apex Resize uses memcpy when source image width/height is equal to destination image 
*  width/height. for performance consideration, resized result image buffer needs be 
*  allocated DMA friendly. for destination size of width and height, the dst buffer should be 
*  allocated to a UMat with size of (width+127)/128*128+32, height+32, and use UMat ROI 
*  Rect(0, 0, width, height) to specify actual dst image size.   
*   
***********************************************************************************/
class Resize
{
public:

/**********************************************************************************
*  Default constructor.
*  Allocate resources for the ACF process 
*  Supported datatypes are: 8bit unsigned integer to 16 bit unsigned integer.
*  default data format is 8bit unsigned.    
***********************************************************************************/
   Resize();

/**********************************************************************************
*  Destructor.
*  Release resources for the ACF process.
***********************************************************************************/
   ~Resize();

/*!*********************************************************************************
*  \brief Initialize the resize ACF process.
*  \return Error code for the execution (zero on success).
***********************************************************************************/
   APEXCV_LIB_RESULT Initialize(vsdk::UMat &lSrcImage     /*!< Source memory buffer. */,
                  vsdk::UMat &lDestImage    /*!< Destination memory buffer. */);

/*!*********************************************************************************
*  \brief Reconnect IO
*
*  Reconnects the input and outputs to the process. This only needs to
*  be done if the connected Input/Outputs are changed. If only the data within (no size,
*  or type changes), then this does not need to be called. This just calls release and 
*  then initialize.
***********************************************************************************/
   APEXCV_LIB_RESULT ReconnectIO(vsdk::UMat &lSrcImage     /*!< Source memory buffer. */,
                   vsdk::UMat &lDestImage    /*!< Destination memory buffer. */);

  
/*!*********************************************************************************
*  \brief Execute the ACF resize process.
*
*  The output buffer of dst may be reallocated by this function
*  This allocation includes space for the output produced by the APEX.
*
*  \return Error code (zero on success).
***********************************************************************************/   
   APEXCV_LIB_RESULT Process(vsdk::UMat &src,
               vsdk::UMat &dst);


   APEXCV_LIB_RESULT SelectApuConfiguration(ACF_APU_CFG lApuConfig,
                              int32_t lApexId);

/// @private
   AcfProfilingInfo retProfilingInfo();

/*!*********************************************************************************
*  \brief set resize to work on 8 bit or 16 bit unisgned integer data.
*
*  resize work on 8bit unsigned datatype by default, call setDataType function
*  to change resize to work on 16bit data
*  \b Note: This function need to be called before initializeMem and process
*  to make sure memory size is allocated correctly and resize is working on correct 
*  datatype.  
***********************************************************************************/
   APEXCV_LIB_RESULT SetDataType(uint32_t dataType /*!< input datatype 1: 8bit, 2: 16bit */ );

private:

   int mInitState = 0;
   ACF_APU_CFG mApuConfig;
   AcfProfilingInfo mApuProfiling;
   int32_t     mApexId;
   bool        mWeSelectedApuConfiguration;

   int  mDataType;

/*!*********************************************************************************
*  \brief Internal ACF resize calls.
*
*  These functions are chosen by process_acf() based on the resize use case.
*
*  \return Error code (zero on success).
***********************************************************************************/  
   APEXCV_LIB_RESULT copy(vsdk::UMat& src, vsdk::UMat& dst);
   APEXCV_LIB_RESULT acf_vrsz(vsdk::UMat& src,vsdk::UMat& dst);
   APEXCV_LIB_RESULT acf_hrsz(vsdk::UMat& src,vsdk::UMat& dst);
   APEXCV_LIB_RESULT acf_resize(vsdk::UMat& src,vsdk::UMat& dst);

   APEXCV_LIB_RESULT copy16(vsdk::UMat& src, vsdk::UMat& dst);
   APEXCV_LIB_RESULT acf_vrsz16(vsdk::UMat& src, vsdk::UMat& dst);
   APEXCV_LIB_RESULT acf_hrsz16(vsdk::UMat& src, vsdk::UMat& dst);
   APEXCV_LIB_RESULT acf_resize16(vsdk::UMat& src, vsdk::UMat& dst);


};
}  /* namespace apex */

#endif   /* APEXCVPRORESIZE_H */
