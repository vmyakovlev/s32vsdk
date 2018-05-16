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

#ifndef APEXAPUHAL_H
#define APEXAPUHAL_H

#include <cmem_if_hal.h>

/////////////////////////////////////////////////////////////////////////
/// \addtogroup apu_hal
/// apu_hal provides high level services related to the APU (e.g. APU load
/// methods, semaphore protected APU enable/disable).
/// @{

#ifndef SEG_HOST_TYPE
typedef int64_t  SEG_HOST_TYPE;
typedef uint32_t SEG_MEM_TYPE;
#endif
typedef SEG_HOST_TYPE LOAD_SEGMENT_t[4];

#define VMEM_SIZE_MAX 2048 // 2k 16-bit words (4KB)
#define APU_LOAD_ID_IMEM 0
#define APU_LOAD_ID_DMEM 1
#define APU_LOAD_ID_VMEM 2

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

/////////////////////////////////////////////////////////////////////////////////
/// Initialize APU driver (i.e. create semaphores to protect APU enable/disable).
///
/// \return
/// 0 if successful, non-zero otherwise.
/////////////////////////////////////////////////////////////////////////////////
int32_t apu_hal_Init();

/////////////////////////////////////////////////////////////////////////////////
/// De-initialize APU driver (i.e. clean up APU enable/disable protection semaphores).
///
/// \return
/// 0 if successful, non-zero otherwise.
/////////////////////////////////////////////////////////////////////////////////
int32_t apu_hal_Deinit();

/////////////////////////////////////////////////////////////////////////////////
/// Enable/disable APU \p apuid on APEX \p apexID.  If \p isEnable is 1, then
/// the desired APU will be enabled.  If \p isEnable is 0, then  the desired
/// APU will be disabled.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] apuid
/// APU ID (e.g. 0 or 1).
///
/// \param[in] isEnable
/// 1 to enable the APU, 0 to disable the APU.
/////////////////////////////////////////////////////////////////////////////////
void apu_hal_Enable(int apexID, unsigned int apuid, unsigned int isEnable);

/////////////////////////////////////////////////////////////////////////////////
/// Initializes \p *pCmemReq, \p *pDmemReq, and \p *pPmemReq with the required amounts of
/// CMEM, DMEM, and PMEM respectively (in bytes) based on the APU load \p seg_addr.
///
/// \param[in] seg_addr
/// Pointer to the APU load segment table defined in <MY_LOAD>_APU_LOAD.h.  This table
/// encapsulates all aspects of the APU load, including program memory and any required
/// DMEM and CMEM initialization.
///
/// \param[out] pCmemReq
/// Pointer to the value that will be initialized with the CMEM requirements (in bytes)
/// of the APU load.
///
/// \param[out] pDmemReq
/// Pointer to the value that will be initialized with the DMEM requirements (in bytes)
/// of the APU load.
///
/// \param[out] pPmemReq
/// Pointer to the value that will be initialized with the PMEM requirements (in bytes)
/// of the APU load.
/////////////////////////////////////////////////////////////////////////////////
void apu_hal_QueryLoad_MEM_REQ(const LOAD_SEGMENT_t* seg_addr,
                               int32_t* pCmemReq,
                               int32_t* pDmemReq,
                               int32_t* pPmemReq);

/////////////////////////////////////////////////////////////////////////////////
/// Load APU \p apuid on APEX \p apexID with the APU load specified by \p seg_addr.
/// This function uses a simple copy, and is primarily used for debugging/testing.
/// The DMA version of the APU loader 'apu_hal_LoadSegments_DMA(...)' should be used
/// in cases where high performance loading is required.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] apuid
/// APU ID (e.g. 0 or 1).
///
/// \param[in] seg_addr
/// Pointer to the APU load segment table defined in <MY_LOAD>_APU_LOAD.h.  This table
/// encapsulates all aspects of the APU load, including program memory and any required
/// DMEM and CMEM initialization.
///
/// \return
/// 0 if successful, non-zero otherwise.
/////////////////////////////////////////////////////////////////////////////////
int32_t apu_hal_LoadSegment(int apexID, int32_t apuid,
                            const LOAD_SEGMENT_t* seg_addr);

/////////////////////////////////////////////////////////////////////////////////
/// Allocate an OAL buffer of size \p loadPmemSize words (32-bit), and initialize the
/// pointer \p *loadOalMemoryPtr_PMEM with the buffer address.  Proceed to copy the
/// contents of \p pLoadPmem into this buffer.  The same steps are performed for
/// the DMEM and CMEM variants.
///
/// \param[in] pLoadPmem
/// Pointer to the APU load PMEM buffer.
///
/// \param[in] loadPmemSize
/// PMEM load size in 32-bit words.
///
/// \param[in] pLoadDmem
/// Pointer to the APU load DMEM buffer.
///
/// \param[in] loadDmemSize
/// DMEM load size in 32-bit words.
///
/// \param[in] pLoadCmem
/// Pointer to the APU load CMEM buffer.
///
/// \param[in] loadCmemSize
/// CMEM load size in 32-bit words.
///
/// \param[out] loadOalMemoryPtr_PMEM
/// Pointer to the address of the allocated OAL buffer into which the PMEM load
/// \p pLoadPmem was copied.
///
/// \param[out] loadOalMemoryPtr_DMEM
/// Pointer to the address of the allocated OAL buffer into which the DMEM load
/// \p pLoadDmem was copied.
///
/// \param[out] loadOalMemoryPtr_CMEM
/// Pointer to the address of the allocated OAL buffer into which the CMEM load
/// \p pLoadCmem was copied.
///
/// \return
/// 0 if successful, non-zero otherwise.
/////////////////////////////////////////////////////////////////////////////////
int32_t apu_hal_CopyLoadToOalMemory(const SEG_MEM_TYPE* pLoadPmem,
                                    int32_t loadPmemSize,
                                    const SEG_MEM_TYPE* pLoadDmem,
                                    int32_t loadDmemSize,
                                    const SEG_MEM_TYPE* pLoadCmem,
                                    int32_t loadCmemSize,
                                    void** loadOalMemoryPtr_PMEM,
                                    void** loadOalMemoryPtr_DMEM,
                                    void** loadOalMemoryPtr_CMEM);

/////////////////////////////////////////////////////////////////////////////////
/// Allocate and populate the linked list that will be used to accelerate the APU loading
/// on APU \p apuid on APEX \p apedID.  \p *llOalMemoryPtr will be initialized with the
/// linked list OAL memory address.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] apuid
/// APU ID (e.g. 0 or 1).
///
/// \param[in] seg_addr
/// Pointer to the APU load segment table defined in <MY_LOAD>_APU_LOAD.h.  This table
/// encapsulates all aspects of the APU load, including program memory and any required
/// DMEM and CMEM initialization).
///
/// \param[in] pPmemLoadBase
/// The pointer <MY_LOAD>_LOAD_PMEM (defined in <MY_LOAD>_APU_LOAD.h)
///
/// \param[in] pDmemLoadBase
/// The pointer <MY_LOAD>_LOAD_DMEM (defined in <MY_LOAD>_APU_LOAD.h)
///
/// \param[in] pCmemLoadBase
/// The pointer <MY_LOAD>_LOAD_CMEM (defined in <MY_LOAD>_APU_LOAD.h)
///
/// \param[in] loadPhysPtr_PMEM
/// Physical address of the contiguous memory region containing the PMEM load.
///
/// \param[in] loadPhysPtr_DMEM
/// Physical address of the contiguous memory region containing the DMEM load.
///
/// \param[in] loadPhysPtr_CMEM
/// Physical address of the contiguous memory region containing the CMEM load.
///
/// \param[in] lPmemOffsetInBytes
/// PMEM offset (in bytes) relative to the beginning of the 128K SMEM region.
///
/// \param[in] lDmemOffsetInBytes
/// DMEM offset (in bytes) relative to the beginning of the 128K SMEM region.
///
/// \param[in] lDmemSizeInBytes
/// DMEM size in bytes.
///
/// \param[in] lApuCfg
/// APU configuration.
///
/// \param[out] llOalMemoryPtr
/// Pointer to the address of the linked list configured to perform the APU load.
///
/// \param[in] lClover389Flag
/// If this flag is 1 the generated linked list will be adjusted to account for HW
/// issue CLOVER-389.
///
/// \return
/// 0 if successful, non-zero otherwise.
/////////////////////////////////////////////////////////////////////////////////
int32_t apu_hal_GenLoadLL(int apexID, int32_t apuid,
                          const LOAD_SEGMENT_t* seg_addr,
                          const SEG_MEM_TYPE* pPmemLoadBase, //base address of *_LOAD_PMEM so the offset can be calculated from the src address in LOAD_SEGMENTS
                          const SEG_MEM_TYPE* pDmemLoadBase, //base address of *_LOAD_DMEM so the offset can be calculated from the src address in LOAD_SEGMENTS
                          const SEG_MEM_TYPE* pCmemLoadBase, //base address of *_LOAD_CMEM so the offset can be calculated from the src address in LOAD_SEGMENTS
                          uint32_t loadPhysPtr_PMEM,         //base address (physical) of *_LOAD_PMEM
                          uint32_t loadPhysPtr_DMEM,         //base address (physical) of *_LOAD_DMEM
                          uint32_t loadPhysPtr_CMEM,         //base address (physical) of *_LOAD_CMEM
                          uint32_t lPmemOffsetInBytes,
                          uint32_t lDmemOffsetInBytes,
                          uint32_t lDmemSizeInBytes,
                          CMEM_IF_APU_CFG lApuCfg,
                          void** llOalMemoryPtr,
                          int32_t lClover389Flag);

/////////////////////////////////////////////////////////////////////////////////
/// Perform DMA accelerated APU loading for APU \p apuid on APEX \p apexID, using
/// the DMA channel specified by \p dmaCh and the linked list specified by
/// \p pLoadSegmentsLLBufferOal.  If the DMA transfer fails, this function will
/// time out and an error will be returned.
///
/// \param[in] apexID
/// APEX ID (e.g. 0, 1, etc.).
///
/// \param[in] apuid
/// APU ID (e.g. 0 or 1).

/// \param[in] dmaCh
/// DMA channel to use for the APU load transfer.
///
/// \param[in] pLoadSegmentsLLBufferOal
/// Pointer to the OAL memory containing the start of the APU load linked list.
///
/// \return
/// 0 if successful, non-zero otherwise.
/////////////////////////////////////////////////////////////////////////////////
int32_t apu_hal_LoadSegments_DMA(int apexID, int32_t apuid,
                                 int32_t dmaCh,
                                 void* pLoadSegmentsLLBufferOal);

#ifdef __cplusplus
}
#endif //__cplusplus

/// @}

#endif  /* APEXAPUHAL_H */
