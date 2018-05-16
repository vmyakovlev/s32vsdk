/*
 * Copyright (c) 2016 Freescale Semiconductor
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CSITYPES_H
#define CSITYPES_H

#ifdef __cplusplus 
extern "C" { 
#endif
  
/**
* @file           csi_types.h
* @brief          Declaration of H264 Encoder driver internal functionality
*/

/*==============================================================================
*                               INCLUDE FILES
==============================================================================*/

#if !defined(__KERNEL__) && !defined(__STANDALONE__)
#include <stdint.h>

#endif // if !defined(__KERNEL__) && !defined(__STANDALONE__)

#include "seq_graph_meta.h"  
  
/*==============================================================================
*                                   CONSTANTS
==============================================================================*/
  
#define CSI_CHNL_CNT     4U
  
#define CSI_IRQ_ERRPP_MASK_ALL  (0x1BU)    ///< all prot/pack error flags
#define CSI_IRQ_ERRPHY_MASK_ALL (0xFFFFFU) ///< all phy error flags
#define CSI_IRQ_ERRLIN_MASK_ALL (0xCCCCU)  ///< all line error flags
#define CSI_IRQ_FRM_MASK_ALL    (0x3333U)  ///< all frame S/E flags
  
/*==============================================================================
*                          DEFINES AND MACROS
==============================================================================*/

#define CSI_IDX_GET(_aCsiIdKey)((uint8_t)_aCsiIdKey.mId.mCsi)
#define CSI_IDX_SET(_aCsiIdKey, _aIdx)(_aCsiIdKey.mKey = (0xffffff00 + _aIdx))
  
#define CSI_IDX2VCID(_aCsiIdx, _aVcIdx)((uint16_t)((uint16_t)1U << (uint16_t) \
                       ((uint16_t)_aVcIdx + (uint16_t)4U * (uint16_t)_aCsiIdx)))
                       
#define CSI_VC_IDX_USED(_aCsiIdKey, _aVcIdx)\
                       ((uint16_t)((uint16_t)_aCsiIdKey.mId.mVc & \
                        (uint16_t)CSI_IDX2VCID((uint16_t)_aCsiIdKey.mId.mCsi, \
                        (uint16_t)_aVcIdx)) \
                        > 0U ? true : false)
                                                 
#define CSI_VC_IDX_ADD(_aCsiIdKey, _aVcIdx)\
                        (_aCsiIdKey.mId.mVc |= \
                         CSI_IDX2VCID(_aCsiIdKey.mId.mCsi, _aVcIdx))
 
#define CSI_VC_IDX_REM(_aCsiIdKey, _aVcIdx)\
                      (_aCsiIdKey.mId.mVc &= \
                      (~CSI_IDX2VCID(_aCsiIdKey.mId.mCsi, _aVcIdx)))     
                 
#define CSI_IDX_CNT CSI_IDX_INVALID

/*==============================================================================
*                                  ENUMS
==============================================================================*/

/*=============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
* ============================================================================*/
 
/**
* @brief          Declare struct CSI_Regs_t
* @details        new name for CSI register structure
*  
*/
typedef volatile struct CSI_tag CSI_Regs_t;  

/**
* @brief          Declare struct CSI_Siul2Regs_t
* @details        new name for SIUL2 register structure
*
*/
typedef volatile struct SIUL2_tag CSI_Siul2Regs_t;

/**
* @brief          Declare CSI_IDX
* @details        Declare CSI index
* 
*/
typedef enum CSI_IDX
{
  CSI_IDX_0 = 0U,
  CSI_IDX_1 = 1U,
  CSI_IDX_INVALID = 2U,
} CSI_IDX_t; // CSI index definition  

/**
* @brief          Declare struct CSI_VCID
* @details        CSI VC ID definition 
*
*/
typedef enum CSI_VCID
{
  CSI_VCID_00 = 0x1,
  CSI_VCID_01 = 0x2,
  CSI_VCID_02 = 0x4,
  CSI_VCID_03 = 0x8,
  
  CSI_VCID_10 = 0x10,
  CSI_VCID_11 = 0x20,
  CSI_VCID_12 = 0x40,
  CSI_VCID_13 = 0x80,
} CSI_VCID_t; // CSI VC ID definition  

/**
* @brief          Declare struct H264ENC_Regs_t
* @details        32 bits to define what CSI VCs are used
*
* @implements     CSI_IdKey_union
* 
*/
typedef union CSI_IdKey {    
  uint32_t mKey;                  // CSI ID key
  struct 
  {
    uint8_t mCsi:8;               // Csi interface index
    uint16_t mVc:16;              // Csi id
  } mId;                          // Included IDs
} CSI_IdKey_t;

/**
* @brief          Declare CSI_IRQ_TYPE
* @details        CSI IRQ TYPE definition 
*
* @implements     CSI_IRQ_TYPE_enumeration
* 
*/
typedef enum CSI_IRQ_TYPE
{
  CSI_IRQ_TYPE_SOF_0 = 0x001U,  // start of frame interrupt
  CSI_IRQ_TYPE_EOF_0 = 0x002U,  // end of frame interrupt
  CSI_IRQ_TYPE_LLE_0 = 0x004U,  // line length error interrupt
  CSI_IRQ_TYPE_LCE_0 = 0x008U,  // line count error interrupt
  CSI_IRQ_TYPE_SOF_1 = 0x010U,  // start of frame interrupt
  CSI_IRQ_TYPE_EOF_1 = 0x020U,  // end of frame interrupt
  CSI_IRQ_TYPE_LLE_1 = 0x040U,  // line length error interrupt
  CSI_IRQ_TYPE_LCE_1 = 0x080U,  // line count error interrupt
  CSI_IRQ_TYPE_SOF_2 = 0x100U,  // start of frame interrupt
  CSI_IRQ_TYPE_EOF_2 = 0x200U,  // end of frame interrupt
  CSI_IRQ_TYPE_LLE_2 = 0x400U,  // line length error interrupt
  CSI_IRQ_TYPE_LCE_2 = 0x800U,  // line count error interrupt
  CSI_IRQ_TYPE_SOF_3 = 0x1000U, // start of frame interrupt
  CSI_IRQ_TYPE_EOF_3 = 0x2000U, // end of frame interrupt
  CSI_IRQ_TYPE_LLE_3 = 0x4000U, // line length error interrupt
  CSI_IRQ_TYPE_LCE_3 = 0x8000U, // line count error interrupt
}CSI_IRQ_TYPE_t; // CSI IRQ TYPE definition  

/**
* @brief          Declare  CSI_EMBD_IRQ_TYPE
* @details        CSI EMBD IRQ TYPE definition 
*
* @implements CSI_EMBD_IRQ_TYPE_enumeration
* 
*/
enum CSI_EMBD_IRQ_TYPE
{
  CSI_EMBD_IRQ_TYPE_EMBIS0 = 0x1U,  // start of frame interrupt
  CSI_EMBD_IRQ_TYPE_EMBIS1 = 0x2U,  // end of frame interrupt
}; // CSI EMBD IRQ TYPE definition  

/**
* @brief          Declare CSI_PPERR_IRQ_TYPE
* @details        CSI_PPER_IRQ_TYPE error TYPE definition
* 
*/
enum CSI_PPERR_IRQ_TYPE
{
  CSI_PPERR_IRQ_TYPE_ECCONE = 0x1U,  // 1bit ecc error in packet header
  CSI_PPERR_IRQ_TYPE_ECCTWO = 0x2U,  // 2bit ecc error in packet header
  CSI_PPERR_IRQ_TYPE_ERFDAT = 0x8U,  // crc error in frame data
  CSI_PPERR_IRQ_TYPE_CRCERR = 0x10U, // crc error in payload  
}; // CSI CSI_PPER_IRQ_TYPE error TYPE definition  

/**
* @brief          Declare CSI_EMBD_STATUS
* @details        CSI embedded data status
*
* @implements CSI_EMBD_STATUS_t_enumeration
* 
*/
typedef enum CSI_EMBD_STATUS
{
  CSI_EMBD_STATUS_NA = 0U,  // data not available
  CSI_EMBD_STATUS_1  = 1U,  // start data available
  CSI_EMBD_STATUS_2  = 2U   // end data available  
} CSI_EMBD_STATUS_t; 

/**
* @brief          Declare struct CSI_VcCfg
* @details        Declare struct for virtual channels configuration
*
* @implements CSI_VcCfg_t_struct
*
*/
typedef struct CSI_VcCfg
{
  uint32_t mBufAddr;       // SRAM buffer address
  uint32_t mLineCnt;       // number of lines fitting into buffer
  uint32_t mLineBytes;     // number of bytes per line
  uint32_t mLineStride;    // number of bytes to next line begin
  uint32_t mFrameHeight;   // expected frame height
  uint32_t mDoneLineCnt;   // number of lines after which the done signal 
                           // to sequener is generated                            
} CSI_VcCfg_t; // CSI VC configuration

/**
* @brief          Declare struct CSI_EmbdCfg_t
* @details        Declare struct for embedded data configuration
*
* @implements CSI_EmbdCfg_t_struct
*
*/
typedef struct CSI_EmbdCfg
{
  uint32_t    mBufAddr;   // SRAM buffer address
  uint32_t    mSize;      // number of bytes available in the buffer
  uint8_t     mLineCnt1;  // number of embedded lines at the beginning of frame
  uint8_t     mLineCnt2;  // number of embedded lines at the end of the frame
  CSI_IdKey_t mVcIdFlags; // for which VC to collect the Embedded lines
  uint8_t     mMasterIdx; // which VC is the master
  
} CSI_EmbdCfg_t; // CSI VC configuration

/**
* @brief          Declare struct CSI_Cfg_t
* @details        Declare struct for CSI configuration
*
* @implements  CSI_Cfg_t_struct
*
*/
typedef struct CSI_Cfg
{
  union CSI_IdKey   mIdKey;    // Mipi CSI id;
  CSI_CameraType_t  mSensor;   // sensor type;
  uint32_t          mMbps;     // expected data frequency on CSI interface
  uint32_t          mLanes;    // number of CSI lanes active
  uint32_t          mCamCnt;   // number of connected cameras
  struct CSI_VcCfg  mpVc[CSI_CHNL_CNT]; // VC config
  CSI_EmbdCfg_t     mEmbeddedData;  // embedded data configuration 
  
  // IRQ masks
  uint32_t          mPPErrIrqMask;  // reg MIPICSI2_ERRPPREG IRQ mask
  uint32_t          mPhyErrIrqMask; // reg MIPICSI2_ERPPINTEN IRQ mask
  uint32_t          mFrameIrqMask;  // reg MIPICSI2_INTRSVC IRQ mask
} CSI_Cfg_t; // CSI configuration

/**
* @brief          Declare struct CSI_ErrStatus_t
* @details        Declare struct CSI_ErrStatus_t
*
* @implements  CSI_ErrStatus_t_struct
* 
*/
typedef struct CSI_ErrStatus
{
  uint32_t mOthrErrFrmId;            // frame index for other error/csi index
  uint32_t mProtPackErr;             // Protocol and packe error flags
  uint32_t mEccErrPos;               // ECC error posisiton
  uint32_t mPhyErr;                  // PHY error flags
  uint32_t mLineErrFrmId;            // frame index for line error
  uint32_t mLineErr;                 // line length/count error flags
  uint32_t mErrLength[CSI_CHNL_CNT]; // Error length for VCs
  uint32_t mErrLine[CSI_CHNL_CNT];   ///< Error line positions for VCs
  uint32_t mErrLineCnt[CSI_CHNL_CNT];///< Error line counts for VCs
} CSI_ErrStatus_t; // CSI configuration

//*****************************************************************************

#if defined(__KERNEL__) || defined(__STANDALONE__)
#include "s32vs234.h"

typedef struct mutex mutex_t;

/**
* @brief          Declare struct CSI_IrqNums
* @details        CSI IRQ numbers
*
* @implements CSI_IrqNums_t_struct
* 
*/
typedef struct CSI_IrqNums
{
  uint32_t mErr;
  uint32_t mFrm;  
} CSI_IrqNums_t; 

/**
* @brief          Declare struct CSI_DrvInfo
* @details        CSI driver info
* 
*/
typedef struct CSI_DrvInfo
{
  CSI_Regs_t     *mpRegs;       // control block registers
  CSI_IrqNums_t   mIrqNums;     // irq numbers
#ifdef __KERNEL__
  struct clk     *mpModuleClk;  // main module clock structure
  struct clk     *mpLiClk;      // PHY interface clock structure
  struct clk     *mpUiClk;      // user interface side clock structure
  struct clk     *mpDphyClk;    // low power clock struct for PHY escape mode
  pid_t           mUserPid;     // whom to send signals
#else 
  uint32_t        mUserPid;     // whom to send signals
#endif
  CSI_ErrStatus_t mErrStatus;   // so far collected errors
  uint8_t         mEmbLnCnt1;   // # of embedded lines at frame beginning
  uint8_t         mEmbLnCnt2;   // # of embedded lines at frame end
  uint32_t        mEmbedStatus; // status of embeded data
  
  uint32_t        mFrmCnt;      // # of frames received from last start
  CSI_IDX_t       mCsiIdx;      // csi index
  
  // IRQ masks
  uint32_t        mPPErrIrqMask;  // reg MIPICSI2_ERRPPREG IRQ mask
  uint32_t        mPhyErrIrqMask; // reg MIPICSI2_ERPPINTEN IRQ mask
  uint32_t        mFrameIrqMask;  // reg MIPICSI2_INTRSVC IRQ mask
} CSI_DrvInfo_t; 

#endif // #if defined(__KERNEL__) || defined(__STANDALONE__)

#ifdef __cplusplus 
} //extern "C" { 
#endif

#endif /* CSITYPES_H */