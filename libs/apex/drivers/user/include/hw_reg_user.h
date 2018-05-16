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

#ifndef APEXDRIVERHWREGUSER_H
#define APEXDRIVERHWREGUSER_H

/////////////////////////////////////////////////////////////////////////
/// \addtogroup hw_reg_user
/// hw_reg_user provides an OS-agnostic abstraction of memory mapped
/// APEX hardware entities.  All APEX hardware abstraction layers and drivers
/// sit on top of hw_reg_user and use its services.  This organization
/// allows for almost all APEX driver code to reside in the user space, in addition
/// to providing performance and portability benefits.
/// @{

/// Unique identifiers for referring to APEX hardware entities
typedef enum
{
   HW_REG_SMEM,           ///< SMEM

   HW_REG_MCDMA,          ///< multi-channel DMA
   HW_REG_MODMA,          ///< motion compensation DMA
   HW_REG_STIO,           ///< stream DMA (i.e. streamin, streamout)
   HW_REG_USEQ,           ///< sequencer
   HW_REG_GLOBAL,         ///< global registers
   HW_REG_DEBUG,
   HW_REG_DMEM_FIFO_0,
   HW_REG_DMEM_FIFO_1,
   HW_REG_CMEM_IF,        ///< CMEM interface
   HW_REG_HOR_SCALER,     ///< horizontal scaler

   HW_REG_CMEM_GOC,       ///< CMEM (8 CU GOC access)

   HW_REG_CMEM_SOC_32,    ///< CMEM (32 CU slice access)
   HW_REG_CMEM_SOC_64,    ///< CMEM (64 CU slice access)

   HW_REG_CMEM_BOC_31_00, ///< CMEM broadcast 0-31
   HW_REG_CMEM_BOC_63_32, ///< CMEM broadcast 32-63
   HW_REG_CMEM_BOC_63_00, ///< CMEM broadcast 0-63

   HW_REG_NUM //indicate number of enumerations (must always be final enumeration)
} HW_REG_ID;

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

/////////////////////////////////////////////////////////////////////////////////
/// Initialize/map the array of pointers to APEX hardware entities.
///
/// \return
/// 0 if successful, 1 if failure.
/////////////////////////////////////////////////////////////////////////////////
int InitVirtualHwPtrArray();

/////////////////////////////////////////////////////////////////////////////////
/// De-initialize/unmap the array of pointers to APEX hardware entities.
///
/// \return
/// 0 if successful, 1 if failure.
/////////////////////////////////////////////////////////////////////////////////
int DeinitVirtualHwPtrArray();

/////////////////////////////////////////////////////////////////////////////////
/// Return a virtual void pointer to the desired memory-mapped hardware registers
/// specified by \p lApexID and \p lHwRegId.
///
/// \code{.cpp}
/// //e.g. retrieve a virtual pointer to the memory-mapped DMA registers associated with APEX_0:
/// volatile APEX_642CL_MCDMA_DCR *const lpDmaRegisters = (APEX_642CL_MCDMA_DCR*)ReturnVirtualHwPtr(0, HW_REG_MCDMA);
/// \endcode
///
/// \param[in] lApexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lHwRegId
/// Hardware register ID (see the "HW_REG_NUM" enumeration for available options).
///
/// \return
/// Non-zero void* if successful, 0 if failure.
/////////////////////////////////////////////////////////////////////////////////
void* ReturnVirtualHwPtr(int lApexID, HW_REG_ID lHwRegId);

/////////////////////////////////////////////////////////////////////////////////
/// Return a physical void pointer to the desired memory-mapped hardware registers
/// specified by \p lApexID and \p lHwRegId.
///
/// \code{.cpp}
/// //e.g. retrieve a physical pointer to the memory-mapped DMA registers associated with APEX_1:
/// uint32_t dmaRegPhysAddr = (uintptr_t)ReturnPhysicalHwPtr(1, HW_REG_MCDMA);
/// \endcode
///
/// \param[in] lApexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] lHwRegId
/// Hardware register ID (see the "HW_REG_NUM" enumeration for available options).
///
/// \return
/// Non-zero void* if successful, 0 if failure.
/////////////////////////////////////////////////////////////////////////////////
void* ReturnPhysicalHwPtr(int lApexID, HW_REG_ID lHwRegId);

#ifdef __cplusplus
}
#endif //__cplusplus

/// @}

#endif /* APEXDRIVERHWREGUSER_H */
