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
*  \file apexcv_pro_histogram_equalization.h
*  \brief Host-ACF interface for Histogram Equalization
***********************************************************************************/

#ifndef APEXCVPROHISTOGRAMEQUALIZATION_H
#define APEXCVPROHISTOGRAMEQUALIZATION_H


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include <umat.hpp>
#include <apexcv_error_codes.hpp>
#include <apexcv_base.h>

namespace apexcv{


  /*!*********************************************************************************
  *  \brief Host-ACF interface for histogram equalization.
  *
  *  This template class is an interface for creating, initializing, processing and releasing
  *  the APU implementation of histogram on the host.
  ***********************************************************************************/

  class HistogramEqualization
  {
  public:

    /***********************************************************************************
    *  Constructor, not documented.
    ***********************************************************************************/
    HistogramEqualization()
      : mType(false), mpProcess(nullptr) {};

    /***********************************************************************************
    *  Destructor, not documented.
    ***********************************************************************************/
    ~HistogramEqualization();

    /*!*********************************************************************************
    *  \brief Initialize the process.
    *
    *  \return Error code for the initialization (zero on success).
    *
    *  We initialize the process on the host by initializing the ACF process
    *
    ***********************************************************************************/
    APEXCV_LIB_RESULT Initialize(
      vsdk::UMat&    aSrc,        /*!< 8-bit grayscale source image. */
      vsdk::UMat&    aDst /*!< 8-bit grayscale destination image. */);

    /*!*********************************************************************************
    *  \brief Execute the process.
    *
    *  \return Error code for the execution (zero on success).
    *
    ***********************************************************************************/
    APEXCV_LIB_RESULT Process();

    /*!*********************************************************************************
    *  \brief Reconnect input and output buffers.
    *
    *  \return Error code for the execution (zero on success).
    *
    ***********************************************************************************/
    APEXCV_LIB_RESULT ReconnectIO(
      vsdk::UMat&    aSrc,        /*!< 8-bit grayscale source image. */
      vsdk::UMat&    aDst /*!< 8-bit grayscale destination image. */
      );


  private:
    bool     mType;    /* Initialization state for all ACF processes. */
    vsdk::UMat mNumPixels;
    vsdk::UMat mSrcHistogram;
    /***********************************************************************************
    *  ACF Process declaration for histogram
    ***********************************************************************************/
    void *mpProcess;
    apexcv::Histogram mHist;
  };

}  /* namespace apexcv */

#endif /* APEXCVPROHISTOGRAMEQUALIZATION_H */
