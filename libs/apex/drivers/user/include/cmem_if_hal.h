/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* (C) Copyright CogniVue Corporation. 2011-2014 All right reserved.
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

#ifndef APEXCMEMIFHAL_H
#define APEXCMEMIFHAL_H

#include <stdint.h>

/////////////////////////////////////////////////////////////////////////
/// \addtogroup cmem_if_hal
/// cmem_if_hal provides services related to configuration of the APEX CMEM
/// interface hardware.
/// @{

/// This enumeration is used to define logical APU configurations.
typedef enum
{
   //------------------
   //321 configurations
   //------------------

   ///321 CFG: APU0 has ownership of CUs 0-31 and SMEM blocks 0-1 (64K)
   CFG_321__APU_0_CU_0_31_SMEM_0_1,

   //------------------
   //642 configurations
   //------------------

   ///642 CFG: APU0 has ownership of CUs 0-63 and SMEM blocks 0-3 (128K)
   CFG_642__APU_0_CU_0_63_SMEM_0_3,

   ///642 CFG: APU0 has ownership of CUs 0-31  and SMEM blocks 0-1 (64K) and
   ///APU1 has ownership of CUs 32-63 and SMEM blocks 2-3 (64K)
   CFG_642__APU_0_CU_0_31_SMEM_0_1__APU_1_CU_32_63_SMEM_2_3,

   //----------------------------------
   //error (i.e. unknown configuration)
   //----------------------------------

   ///Unknown configuration (i.e. error state)
   CFG_UNKNOWN_CONFIGURATION

} CMEM_IF_APU_CFG;

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/////////////////////////////////////////////////////////////////////////////////
/// Set the APU configuration defined by \p lApuCfg on APEX \p lApuCfg.
///
/// \param[in] lApexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lApuCfg
/// APU configuration (see CMEM_IF_APU_CFG enumeration for valid configurations).
/////////////////////////////////////////////////////////////////////////////////
void cmem_if_hal_set_apu_cfg(int32_t lApexID, CMEM_IF_APU_CFG lApuCfg);

/////////////////////////////////////////////////////////////////////////////////
/// Query and return the currently defined APU configuration on APEX \p lApexID.
///
/// \param[in] lApexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \return
/// Currently defined APU configuration.  See CMEM_IF_APU_CFG enumeration for valid
/// configurations.  CFG_UNKNOWN_CONFIGURATION will be returned if an unknown configuration
/// is detected.
/////////////////////////////////////////////////////////////////////////////////
CMEM_IF_APU_CFG cmem_if_hal_query_apu_cfg(int32_t lApexID);

/////////////////////////////////////////////////////////////////////////////////
/// Configure the program memory (PMEM) start address \p address for APU \p apuid
/// on APEX \p lApexID.  \p address is an offset from the start of the
/// 128K SMEM region associated with APEX \p lApexID.
///
/// \param[in] lApexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] apuid
/// APU ID (e.g. 0 or 1).
///
/// \param[in] address
/// Address of PMEM start relative to the start of the 128K SMEM region.
/////////////////////////////////////////////////////////////////////////////////
void cmem_if_hal_set_apu_pmem_start(int32_t lApexID, int32_t apuid, uint32_t address);

/////////////////////////////////////////////////////////////////////////////////
/// Configure the data memory (DMEM) start address \p address for APU \p apuid
/// on APEX \p lApexID.  \p address is an offset from the start of the
/// 128K SMEM region associated with APEX \p lApexID.
///
/// \param[in] lApexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] apuid
/// APU ID (e.g. 0 or 1).
///
/// \param[in] address
/// Address of DMEM start relative to the start of the 128K SMEM region.
/////////////////////////////////////////////////////////////////////////////////
void cmem_if_hal_set_apu_dmem_start(int32_t lApexID, int32_t apuid, uint32_t address);

/////////////////////////////////////////////////////////////////////////////////
/// Query and return the currently defined PMEM start address for APU \p apuid
/// on APEX \p lApexID.
///
/// \param[in] lApexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] apuid
/// APU ID (e.g. 0 or 1).
///
/// \return
/// PMEM start address relative to the start of the 128K SMEM region.
/////////////////////////////////////////////////////////////////////////////////
uint32_t cmem_if_hal_query_apu_pmem_start(int32_t lApexID, int32_t apuid);

/////////////////////////////////////////////////////////////////////////////////
/// Query and return the currently defined DMEM start address for APU \p apuid
/// on APEX \p lApexID.
///
/// \param[in] lApexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] apuid
/// APU ID (e.g. 0 or 1).
///
/// \return
/// DMEM start address relative to the start of the 128K SMEM region.
/////////////////////////////////////////////////////////////////////////////////
uint32_t cmem_if_hal_query_apu_dmem_start(int32_t lApexID, int32_t apuid);

/////////////////////////////////////////////////////////////////////////////////
/// Query and return the CU array width for APU \p apuid on APEX \p lApexID.  This
/// depends on the current APU configuration, and will be either 32 or 64 for a 642 APEX
/// hardware configuration.  For example, if the APU configuration was set to
/// CFG_642__APU_0_CU_0_63_SMEM_0_3, the CU array width will be 64 for APU0 and 0 for APU1.
///
/// \param[in] lApexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] apuid
/// APU ID (e.g. 0 or 1).
///
/// \return
/// CU array width for APU \p apuid.  0 if the APU specified by \p apuid doesn't have any CUs
/// associated with it or if \p apuid refers to an invalid APU.
/////////////////////////////////////////////////////////////////////////////////
uint32_t cmem_if_hal_query_cu_array_width(int32_t lApexID, int32_t apuid);

////////////////////////////////////////////////////////////////////////////////
/// Initializes \p *pSmemSize and \p *pCmemSize with the SMEM size and CMEM size
/// respectively (in bytes) based on the APU configuration specified by \p apuconfig.
///
/// \param[in] apuconfig
/// APU configuration.
///
/// \param[out] pSmemSize
/// Pointer to the value that will be initialized with SMEM size (in bytes).
///
/// \param[out] pCmemSize
/// Pointer to the value that will be initialized with CMEM size (in bytes).
///
/// \return
/// 0 if successful, non-zero otherwise.
/////////////////////////////////////////////////////////////////////////////////
int32_t RetApuMemSizes(CMEM_IF_APU_CFG apuconfig,
                       int32_t* pSmemSize,
                       int32_t* pCmemSize);

#ifdef __cplusplus
}
#endif //__cplusplus

/// @}

#endif /* APEXCMEMIFHAL_H */
