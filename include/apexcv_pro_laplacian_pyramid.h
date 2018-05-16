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

/*==================================================================================================
Revision History:
                              Modification     Tracking
Author (core ID)              Date D/M/Y       Number     Description of Changes
---------------------------   ----------    ------------  ------------------------------------------
Adrian Grigore (nxa11567)     14/12/2016    VSDK-554      Created
---------------------------   ----------    ------------  ------------------------------------------
==================================================================================================*/
/*!****************************************************************************
*  \file apexcv_pro_laplacian_pyramid.h
*  \brief ApexCV Host wrapper for laplacian pyramid creation
*******************************************************************************/

#ifndef APEXCVPROLAPLACIANPYRAMID_H
#define APEXCVPROLAPLACIANPYRAMID_H

#include <umat.hpp>
#include <apexcv_error_codes.hpp>

namespace apexcv {

/*!****************************************************************************
*  \brief Pyramid creation class
*
*  This class is an interface for using the pyramid creation algorithm.
******************************************************************************/  
class LaplacianPyramid
{
public:

/*!****************************************************************************
*  \brief Default constructor.
******************************************************************************/
   LaplacianPyramid();

/*!****************************************************************************
*  \brief Default destructor.
******************************************************************************/
   ~LaplacianPyramid();

/*!****************************************************************************
*  \brief Initializes the class.
*  
*  Connects the buffers to the process.
******************************************************************************/
   APEXCV_LIB_RESULT Initialize(      vsdk::UMat&  aSrc         /*!< Unsigned 8-bit Source memory buffer. */,
                        vsdk::UMat&  aDstPyramid /*!< Signed 16-bit Destination memory buffer for pyramid output. */,
                        vsdk::UMat&  aDstAux     /*!< Auxiliary destination memory buffer for storing the input for the next pyramid level or reconstruction image, depending on aIsLastLevel parameter. Data is Unsigned 8-bit for aIsLastLevel == FALSE and Signed 16-bit for aIsLastLevel == TRUE. */,
                        bool aIsLastLevel                  /*!< Chooses between returning the reconstruction image or the input for the next pyramid level. */
                  );

/*!****************************************************************************
*  \brief Connects the input/outputs to the process
*
*  Use this to reconnect the Input and Output Buffers. This only needs to
*  be done if the connected Input/Outputs are changed. If only the data within (no size,
*  or type changes), then this does not need to be called.
******************************************************************************/
   APEXCV_LIB_RESULT ReconnectIO(     vsdk::UMat&  aSrc         /*!< Unsigned 8-bit Source memory buffer. */,
                        vsdk::UMat&  aDstPyramid /*!< Signed 16-bit Destination memory buffer for pyramid output. */,
                        vsdk::UMat&  aDstAux     /*!< Auxiliary destination memory buffer for storing the input for the next pyramid level or reconstruction image, depending on isLastLevel parameter. Data is Unsigned 8-bit for isLastLevel == FALSE and Signed 16-bit for isLastLevel == TRUE. */
                  );

/*!****************************************************************************
*  \brief Run the pyramid creation process.
*
*  The Laplacian output for the current pyramid level is computed. Depending on 
*  how the class was configured using isLastLevel parameter when initialize() was
*  called, the second buffer will contain the information needed to compute the 
*  next pyramid level (isLastLevel==FALSE) or the image that can be used for 
*  laplacian image reconstruction (isLastLevel==TRUE).
*  Supported datatypes are:
*    - unsigned 8 bit to signed 16 bit & signed 16 bit for isLastLevel==TRUE
*    - unsigned 8 bit to signed 16 bit & unsigned 8 bit for isLastLevel==FALSE
*
*  \return Error code (zero on success).
***********************************************************************************/
   APEXCV_LIB_RESULT Process();

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
   int  mInit;      /*!< Flag indicating the process has been initialized. */
   bool mIsLastLevel;      /*!< Flag indicating the way the class is configured to operate: auxiliary output is next level input or reconstruction image. */

/*!*********************************************************************************
*  \brief \ref laplacian_pyramid_mid_apu_process_desc "ACF process" for the 
        \ref secLaplacianPyramid "laplacian pyramid creation" mid pyramid level.
***********************************************************************************/
   void* mLaplacianMid;

/*!*********************************************************************************
*  \brief \ref laplacian_pyramid_last_apu_process_desc "ACF process" for the 
        \ref secLaplacianPyramid "laplacian pyramid creation" last pyramid level.
***********************************************************************************/
   void* mLaplacianLast;

   int32_t mApexId;       /*!< Index of the APEX core selected to run the algorithm */

}; /* class LaplacianPyramid */

} /* namespace apex */

#endif /* APEXCVPROLAPLACIANPYRAMID_H */


