/*****************************************************************************
* 
* NXP Confidential Proprietary
*
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
*  \file apexcv_host_base_class.h
*  \brief APEX-CV Host Base Class
***********************************************************************************/

#ifndef APEXCVHOSTBASECLASS_H
#define APEXCVHOSTBASECLASS_H

#include <stdint.h>
#include <apexcv_error_codes.hpp>


namespace apexcv
{

  /***********************************************************************************
  *  Base class for APEX-CV Base modules
  *  Provides common functionality for all APEX-CV modules.
  ***********************************************************************************/
  class ApexcvHostBaseClass
  {
  public:
    ApexcvHostBaseClass() : mType(0), mpProcess(nullptr), mpACF(nullptr) {};
    virtual ~ApexcvHostBaseClass() {};

    /*!*********************************************************************************
    *  \brief      Select the APEX Core
    *  \details    Select which APEX core (0 or 1) to be selected to run the processing.
    *              This function has to be called after initialize() and can be executed on a per frame base.
    *  \return     APEXCV Error code (APEXCV_SUCCESS on success).
    ***********************************************************************************/
    APEXCV_LIB_RESULT SelectApexCore(
      int  aApexId  /*!< [in] ID of the APEX core used for performing the processing (0 or 1). */);

    /*!*********************************************************************************
    *  \brief      Start processing and return when done.
    *  \details    Execute code on selected APEX core (default is Apex core 0).
    *              This function is called after initialize() and is executed on a per frame base.
    *  \return     APEXCV Error code (APEXCV_SUCCESS on success).
    ***********************************************************************************/
    APEXCV_LIB_RESULT Process();


    /*!*********************************************************************************
    *  \private
    *  GetStats (not documented)
    *  Returns APEXCV Error code (APEXCV_SUCCESS on success).
    *
    *  Retreive statistical information after execution on selected APEX core.
    *  This function is called after Process().
    ***********************************************************************************/
    APEXCV_LIB_RESULT GetStats(
      int aIndex, /* index of the algorithm section, not used in APEX-CV base */
      int &aChunkWidth, /* Chunk width for port INPUT_0, source image */
      int &aChunkHeight, /* Chunk height for port INPUT_0, source image */
      int aTimes[7]); /* Internal times [host Start, host Wait, apu total, apu init, apu proc, apu idle, apu misc] */

  protected:
    int mType;
    void* mpProcess;
    void* mpACF;
  };

}
#endif /* APEXCVHOSTBASECLASS_H */

