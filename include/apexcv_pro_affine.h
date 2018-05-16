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
*  @file
*  @brief Host-ACF interface and test code for the \ref pagAffine "affine transformation".
***********************************************************************************/
#ifndef APEXCVPROAFFINE_H
#define APEXCVPROAFFINE_H

#include <stdint.h>
#include <apexcv_error_codes.hpp>
// ACF HOST LIB
#include <umat.hpp>

namespace apexcv {

/*!*********************************************************************************
*  \brief Host-ACF interface for the \ref pagAffine "affine transformation".
*
*  This class is an interface for creating, initializing, processing and releasing
*  the APU implementation of the \ref pagAffine "affine transformation" on APEX.
***********************************************************************************/
class Affine
{
public:
/**********************************************************************************
*  Default constructor.
***********************************************************************************/
   Affine();
   
/**********************************************************************************
*  Destructor.
***********************************************************************************/
   ~Affine();
   
/*!*********************************************************************************
*  \brief Execute the process for \ref pagAffine "affine transformation" 
*
*  \return Error code for the execution (zero on success).
*
*  Execute chunk offset calculation for affine transformation and apply bilinear 
*  interpolation.
***********************************************************************************/
   APEXCV_LIB_RESULT Process(vsdk::UMat &lSrc            /*!< Source image buffer.*/,
               vsdk::UMat &lMat            /*!< Affine matrix.*/,
               vsdk::UMat &lDst            /*!< Destination image buffer. Needs to over allocate as double image size as DMA safe guard */);
   
private:  
   
   struct blockDesc
   {
      uint16_t srcWidth;   /*!< Source image block width */
      uint16_t srcHeight;  /*!< Source image block height */
      uint16_t dstWidth;   /*!< Destination image block width */
      uint16_t dstHeight;  /*!< Destination image block height */
      float ratio;          /*!< Scaling ratio */
      int32_t memSize;     /*!< CMEM usage */
   }; 
};
}

#endif /* APEXCVPROAFFINE_H */



