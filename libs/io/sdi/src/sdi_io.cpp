/******************************************************************************
 *
 * Freescale Confidential Proprietary
 *
 * Copyright (c) 2016-17 Freescale Semiconductor;
 * Copyright (c) 2016-17 NXP 
 * All Rights Reserved
 *
 *****************************************************************************
 *
 * THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITEDl TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/**
 * \file      sdi_io.cpp
 * \brief     Dfinitions of SDI input/output objects.
 * \author    Tomas Babinec
 * \version   0.1
 * \date      11-November-2014
 ****************************************************************************/

#ifdef SDI_ECLIPSE_DEF
#define GBO_FILE "GBOpts_APEX2_Pseudo_FPGA_Valid.h"
#endif

#include "s32vs234.h"
#include <unistd.h>
#include <string.h>
#ifndef __STANDALONE__
  #include <sys/mman.h>
#endif // ifndef __STANDALONE__
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "sdi.hpp"
#include "sdi_graph.hpp"
#include "vdb_log.h"
#include "oal.h"
#include "oal_extras.h"
#include "oal_process.h"

#include "seq.h"
#include "cam.h"
#include "isp_fdma.h"
#include "isp_cam_sony.h"
#include "isp_cam_maxim.h"
#include "isp_cam_ov10640.h"
#include "isp_cam_ov10635.h"
#include "isp_h264enc.h"
#include "isp_h264dec.h"
#include "isp_viu.h"

//****************************************************************************
// consts
//****************************************************************************

//****************************************************************************
// macros
//****************************************************************************
#define UNUSED(_x) (void)(_x)  // to suppress unused parameter warning
#define H264ENC_CBUF_OVF    0x200 // ISR CBUF overflow bit mask

//****************************************************************************
// local variables
//****************************************************************************

//****************************************************************************
// local functions declarations
//****************************************************************************

//****************************************************************************
// static members definition
//****************************************************************************

//****************************************************************************
// methods implementation
//****************************************************************************

//****************************************************************************
// *** SDI_DdrBufferArr ***
//****************************************************************************

SDI_DdrBufferArr::SDI_DdrBufferArr():
      mFdmaTcIdx(FDMA_CHANNELS),
      mCnt(0),
      mpUmat(NULL),
      mDeallocate(false),
      mDesc()
{
} // SDI_DdrBufferArr()

//****************************************************************************

SDI_DdrBufferArr::SDI_DdrBufferArr(const SDI_DdrBufferArr &arSrc):
      mpUmat(NULL),
      mDeallocate(false),
      mDesc(arSrc.mDesc)
{
  if(arSrc.mpUmat != NULL)
  {
    mFdmaTcIdx = arSrc.mFdmaTcIdx;
    mCnt = arSrc.mCnt;

    // allocate new HW buffer address array
    mpUmat     = new vsdk::UMat[mCnt];
    if(mpUmat != NULL) 
    {
      for(uint32_t i = 0; i < mCnt; i++)
      {
        mpUmat[i]   = arSrc.mpUmat[i];
      } // for all buffers
    }
    else
    {
      delete[] mpUmat;
      VDB_LOG_ERROR("Failed to allocate mpUmat array.\n");
    }
  } // if allocation succeeded
  else
  {
    VDB_LOG_ERROR("Failed to allocate mpUmat array.\n");
  } // if buffers not NULL

  if(mpUmat == NULL)
  {
    mFdmaTcIdx = FDMA_CHANNELS;
    mCnt = 0;
    mDesc = SDI_ImageDescriptor();
  } // else from if buffers not NULL
} // SDI_DdrBufferArr(const SDI_DdrBufferArr &arSrc)

//****************************************************************************

SDI_DdrBufferArr::~SDI_DdrBufferArr()
{
  // free buffers if needed
  Free();

  if(mpUmat != NULL)
  {
    delete[] mpUmat;
  } // if buffer address array allocated
    
  mFdmaTcIdx  = FDMA_CHANNELS;
  mCnt        = 0;
  mDesc       = SDI_ImageDescriptor();
  mpUmat      = NULL;
  mDeallocate = false;
} // SDI_DdrBufferArr()

//****************************************************************************

LIB_RESULT SDI_DdrBufferArr::Allocate(SDI_ImageDescriptor &arDesc, 
                                      uint32_t aCnt)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if((mCnt == 0) && (aCnt > 0))
  {
    mCnt = aCnt;
    // allocate new HW buffer address array
    mpUmat   = new vsdk::UMat[mCnt];
    if(mpUmat != NULL) 
    {      
      // allocate buffers & get physical addresses
      for(uint32_t i = 0; i < aCnt; i++)
      {
        mpUmat[i] = vsdk::UMat(arDesc.mHeight, arDesc.mWidth, arDesc.mCvImageFormat);
        if( mpUmat[i].u->handle == 0)
        {
          lRet = LIB_FAILURE;
          break;
        }
        
        {
          vsdk::Mat matrix = mpUmat[i].getMat(OAL_USAGE_CACHED);
          memset(matrix.data, 0x0, arDesc.mHeight * arDesc.mWidthStep);
        }
      } // for all framebuffers

      if(lRet != LIB_SUCCESS)
      {
        Free();
      } // if allocation failed
      else
      {
        // remember new config 
        mCnt        = aCnt;
        mDesc       = arDesc;
        mDeallocate = true;
      } // else from if allocation failed
    } // if Hw address array allocation succeeded
    else
    {
      VDB_LOG_ERROR("Failed to allocate mpUmat array.\n");
    } // else from if Hw address array allocation succeeded
  } // if not yet allocated
  else
  {
    lRet = LIB_FAILURE;
    VDB_LOG_ERROR("Array allocated already. Call free first.\n");
  } // else from if not yet allocated

  return lRet;
} // Allocate()

//****************************************************************************

void SDI_DdrBufferArr::Free()
{
  if(mpUmat != NULL)
  {
    if(mDeallocate)
    {
      delete[] mpUmat;
    } // if buffers should be deallocated

    mpUmat = NULL;
  } // if buffers allocated

  mCnt = 0;
  mDesc = SDI_ImageDescriptor();
} // Free()

//***************************************************************************

uint32_t SDI_DdrBufferArr::ChannelIdxGet()
{
  return mFdmaTcIdx;
} // ChannelIdxGet()

//***************************************************************************

LIB_RESULT SDI_DdrBufferArr::ChannelIdxSet(uint32_t aFdmaChannelIndex)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  if(aFdmaChannelIndex < FDMA_CHANNELS)
  {
    mFdmaTcIdx = aFdmaChannelIndex;
  } //if valid index
  else
  {
    lRet = LIB_FAILURE;
  } // else from if valid index

  return lRet;
} // ChannelIdxSet(uint32_t aFdmaChannelIndex)

//***************************************************************************

uint32_t SDI_DdrBufferArr::BufferCntGet()
{
  return mCnt;
} // BufferCntGet()

//****************************************************************************

vsdk::UMat SDI_DdrBufferArr::Query(uint32_t aIdx)
{
  //SDI_Image lRet;
  if(aIdx < mCnt)
  {
    //lRet.DescriptorSet(mDesc);
    return mpUmat[aIdx];
  } // if valid buffer
  else
  {
    return vsdk::UMat();
  }
} // Query(uint32_t aIdx)

//****************************************************************************

SDI_DdrBufferArr SDI_DdrBufferArr::Roi( const uint32_t acX,
                                        const uint32_t acY,
                                        const uint32_t acW,
                                        const uint32_t acH
                                      )
{
  SDI_DdrBufferArr lRet(*this);

  if ((mDesc.mWidth >= (acX + acW))
      && (mDesc.mHeight >= (acY + acH)))
  {
    lRet.mDesc.mWidth       = acW;
    lRet.mDesc.mHeight      = acH;
    lRet.mDesc.mBpp         = mDesc.mBpp;
    lRet.mDesc.mPixelFormat = mDesc.mPixelFormat;
    lRet.mDesc.mWidthStep   = mDesc.mWidthStep;

    for(uint32_t i = 0; i < mCnt; i++)
    {
      lRet.mpUmat[i]     = vsdk::UMat(mpUmat[i], vsdk::Rect(acX, acY, acW, acH));
    }
  } 
  else
  {
    VDB_LOG_ERROR("Bad roi requested. Rectangle overlaps source image.\n");
    for(uint32_t i = 0; i < mCnt; i++)
    {
      mpUmat[i]     = vsdk::UMat();
    }
  } // else from if bad roi requested

  lRet.mDeallocate  = false;

  return lRet;
} // SDI_DdrBufferArr::Roi()

//****************************************************************************

LIB_RESULT SDI_DdrBufferArr::AddressArrSet(SDI_ImageDescriptor &arDesc, 
                                           vsdk::UMat *apUMatArr, 
                                           uint32_t aCnt)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if(apUMatArr != NULL)
  {
    if(mpUmat == NULL)
    {
      // allocate new HW buffer address array
      mpUmat   = new vsdk::UMat[mCnt];
      if(mpUmat != NULL) 
      {
        // remember new setup
        mCnt        = aCnt;
        mDesc       = arDesc;
        mDeallocate = false; 

        // remember buffers & get physical addresses
        for(uint32_t i = 0; i < aCnt; i++)
        {
          mpUmat[i] = apUMatArr[i];  
        } // for all framebuffers

        if(lRet != LIB_SUCCESS)
        {
          Free();
        } // if allocation failed

      } // if Hw address array allocation succeeded
      else
      {
        VDB_LOG_ERROR("Failed to allocate mpUmat array.\n");
      } // else from if Hw address array allocation succeeded
    } // if not allocated yet
    else
    {
      lRet = LIB_FAILURE;
      VDB_LOG_ERROR("Array allocated already. Call free first.\n");
    } // else from if not allocated yet

    // check success
    if(mpUmat == NULL)
    {
      lRet        = LIB_FAILURE;
      mCnt        = 0;
      mDesc       = SDI_ImageDescriptor();
    } // if buffer set failed

    mDeallocate = false;
  } // if bad pointer given

  return lRet;
} // AddressArrSet()

//****************************************************************************

//****************************************************************************

// *** sdi_io ***

//****************************************************************************

sdi_io::sdi_io() :
    mTypeIndex(SEQ_OTHRIX_LAST),
    mpGraph(NULL),
    mState(OFF),
    mResult(LIB_SUCCESS),
    mReserved(false),
    mHeadIndex(-1)
{

} // sdi_input:sdi_io()

//****************************************************************************

sdi_io::sdi_io(SEQ_Othr_ix_t aIOtype) :
    mTypeIndex(aIOtype),
    mpGraph(NULL),
    mState(OFF),
    mResult(LIB_SUCCESS),
    mReserved(false),
    mHeadIndex(-1)
{

} // sdi_input:sdi_io(SEQ_Other_ix_t aIOix)

//****************************************************************************

sdi_io::sdi_io(SEQ_Othr_ix_t aIOtype, sdi_graph *apGraph) :
    mTypeIndex(aIOtype),
    mpGraph(apGraph),
    mState(OFF),
    mResult(LIB_SUCCESS),
    mReserved(false),
    mHeadIndex(-1)
{

} // sdi_input:sdi_io(SEQ_Other_ix_t aIOix, sdi_graph)

//****************************************************************************

sdi_io::~sdi_io()
{
  mTypeIndex = SEQ_OTHRIX_LAST;
  mpGraph = NULL;
  mState  = OFF;
  mResult = LIB_SUCCESS;
  mHeadIndex = -1;
  mReserved = false;
} // sdi_input::~sdi_input()

//****************************************************************************

sdi_IoState sdi_io::StateGet(void)
{
  return mState;
} // sdi_io::StateGet()

//****************************************************************************

LIB_RESULT sdi_io::ResultGet(void)
{
  return mResult;
}// sdi_io::ResultGet()

// *** sdi_FdmaIO ***

//****************************************************************************

sdi_FdmaIO::sdi_FdmaIO(): sdi_io(SEQ_OTHRIX_FDMA), mppTcs(NULL)
{
  for(uint32_t i = 0; i < FDMA_CHANNELS; i++)
  {
    mppBuffArrs[i] = NULL;
  } // for all possible channels
} // sdi_FdmaIO::sdi_FdmaIO()

//****************************************************************************

sdi_FdmaIO::sdi_FdmaIO(sdi_graph *apGraph):
              sdi_io(SEQ_OTHRIX_FDMA, apGraph),
              mppTcs(NULL)
{
  mppTcs = mpGraph->FdmaTcsGet();

  for(uint32_t i = 0; i < FDMA_CHANNELS; i++)
  {
    mppBuffArrs[i] = NULL;
  } // for all possible channels
} // sdi_FdmaIO::sdi_FdmaIO()

//****************************************************************************

LIB_RESULT sdi_FdmaIO::Reserve()
{
  mResult = LIB_SUCCESS;

  if(FdmaOpen() != FDMA_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to open FDMA device.\n");
    mResult = LIB_FAILURE;
  } // if FdmaOpen() failed
  else
  {
    mState  = RESERVED;
  } // else from if FdmaOpen() failed
  
  return mResult;
} // sdi_FdmaIO::Reserve()

//****************************************************************************

LIB_RESULT sdi_FdmaIO::Release()
{
  LIB_RESULT lRet = LIB_SUCCESS;

  // stop if still running
  if(mState == ON)
  {
    (void)Stop();
  } // if still ON

  if(FdmaClose() != FDMA_LIB_SUCCESS)
  {
    lRet = LIB_FAILURE;
  }
  
  mState          = OFF;

  return lRet;
} // sdi_FdmaIO::Release()
//****************************************************************************

LIB_RESULT sdi_FdmaIO::Setup()
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if((mState == OFF) || (mState == ON))
  {
    VDB_LOG_ERROR("Failed to setup. Current state is: [%s].\n", 
                  (mState == OFF)?"OFF":"ON");
    lRet = LIB_FAILURE;
  } // if not reserved yet

  if((mppTcs != NULL) || (lRet == LIB_FAILURE))
  {
    // finalize TC setup
    for(uint32_t i = 0; i < FDMA_CHANNELS; i++)
    {
      if((mppTcs[i] != NULL) && (mppBuffArrs[i] != NULL))
      {
        // *** request Sequencer DDR buffers ***
        SEQ_BufferRequest_t lSeqBuffRequest;
        lSeqBuffRequest.mStreamIdx    = i;
        lSeqBuffRequest.mCnt          = mppBuffArrs[i]->mCnt;
        lSeqBuffRequest.mBaseShift    = 0;
        lSeqBuffRequest.mBufferSize   = mppBuffArrs[i]->mDesc.mWidthStep * \
                                        mppBuffArrs[i]->mDesc.mHeight;
        lSeqBuffRequest.mSramLpF      = mppTcs[i]->mTdm.mSramLpF;
        lSeqBuffRequest.mSramLineOffs = mppTcs[i]->mTdm.mSramLineOffs;
        lSeqBuffRequest.mpBuffers     = new uint32_t[mppBuffArrs[i]->mCnt];
        for (uint32_t cnt = 0; cnt < mppBuffArrs[i]->mCnt; ++cnt)
          lSeqBuffRequest.mpBuffers[cnt] = 
                  ((uint32_t)(uint64_t)mppBuffArrs[i]->mpUmat[cnt].u->handle) + 
                  mppBuffArrs[i]->mpUmat[cnt].offset;

        if(SEQ_FrmBuffersRequest(&lSeqBuffRequest) == LIB_SUCCESS)
        {
          // *** query and push buffers ***
          SEQ_FrmBufferInfo_t lBufferInfo;
          lBufferInfo.mBufferIdx = 0;
          lBufferInfo.mStreamIdx = i;

          while(SEQ_FrmBufferQuery(&lBufferInfo) == SEQ_DRV_SUCCESS)
          {
            SEQ_FrmBufferPush(&lBufferInfo);
            lBufferInfo.mBufferIdx++;
          } // for all available buffers

          // finish setup of appropriate TD
          FDMA_Td_t &lrTD = mppTcs[i]->mTd;

          lrTD.mDdrImgStartAddr = (uint32_t)(uint64_t)
                                  mppBuffArrs[i]->mpUmat[0].u->handle + 
                                  mppBuffArrs[i]->mpUmat[0].offset;
          lrTD.mDdrLineStride   = mppBuffArrs[i]->mDesc.mWidthStep/32;
          lrTD.mWrEna           = 1;
          lrTD.mCrcEna          = 0;
          lrTD.mCrcMode         = 0;
          lrTD.mCrcPoly         = 1;
        } // if buffer requested ok
        else
        {
          lRet = LIB_FAILURE;
        } // else from if buffer requested ok
        delete lSeqBuffRequest.mpBuffers;
      } // if channel used
    } // for all FDMA_CHANNELS

    // aplly TC setup to SRAM TDT
    if(mpGraph->FdmaTcsApply() != LIB_SUCCESS)
    {
      lRet = LIB_FAILURE;
    } // if TCs apply failed
    else
    {
      // setup successfull
      mState  = SETUP;
    }
  } // if initialized properly
  else
  {
    lRet = LIB_FAILURE;
    VDB_LOG_ERROR("FDMA IO not initialized properly.\n");
  } // else from if initialized properly

  return lRet;
} // sdi_FdmaIO::Setup()

//***************************************************************************

LIB_RESULT sdi_FdmaIO::Start(void)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  if(mState != SETUP)
  {
    VDB_LOG_ERROR("Failed to start. Not setup yet.\n");
    lRet = LIB_FAILURE;
  } // if not setup yet
  else
  {
    mState  = ON;
  } // else from if not setup yet
  // nothing to be done now

  return lRet;
} // sdi_FdmaIO::Start(void)

//***************************************************************************

LIB_RESULT sdi_FdmaIO::Stop(void)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if(mState == ON)
  {
    mState  = SETUP;
  } // if ON
  else
  {
    VDB_LOG_WARNING("Failed to stop. Not ON yet.\n");
  } // else from if not ON

  // nothing to be done now

  return lRet;
} // sdi_FdmaIO::Stop(void)

//****************************************************************************

LIB_RESULT sdi_FdmaIO::ParameterSet(SDI_IO_PARAM aParamId, void*)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  switch(aParamId)
  {
    default:
      lRet = LIB_FAILURE;
      VDB_LOG_ERROR("No such parameter.\n");
      break;
  }; // switch aParamId

  return lRet;
} // sdi_FdmaIO::ParameterSet(SDI_IO_PARAM aParamId, void *apValue)

//****************************************************************************

LIB_RESULT sdi_FdmaIO::ParameterGet(SDI_IO_PARAM aParamId, void *apValue)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  // TODO: remove unused warning suppression macro
  UNUSED(aParamId);
  UNUSED(apValue);

  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return lRet;
} // sdi_FdmaIO::ParameterGet(SDI_IO_PARAM aParamId, void *apValue)

//****************************************************************************

LIB_RESULT sdi_FdmaIO::DdrBufferDescGet(uint32_t aChnlIdx,
                                        SDI_ImageDescriptor &arDesc)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if(mppTcs != NULL)
  {
    if((aChnlIdx < FDMA_CHNL_CNT) && (mppTcs[aChnlIdx] != NULL))
    {
      arDesc = mppIDs[aChnlIdx];
    } // if valid FDMA chnl index
    else
    {
      lRet = LIB_FAILURE;
      VDB_LOG_ERROR("Invalid FDMA channel requested.\n");
    } // else from if valid FDMA chanel index
  } // if initialized properly
  else
  {
    lRet = LIB_FAILURE;
    VDB_LOG_ERROR("FDMA IO not initialized properly.\n");
  } // if initialized properly

  return lRet;
} // DdrBufferDescGet(uint32_t aChnlIdx, SDI_ImageDescriptor arDesc)

//****************************************************************************

LIB_RESULT sdi_FdmaIO::DdrBufferDescSet(uint32_t aChnlIdx,
                                        const SDI_ImageDescriptor &arcDesc)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if(mppTcs != NULL)
  {
    if((aChnlIdx < FDMA_CHNL_CNT) && (mppTcs[aChnlIdx] != NULL))
    {
      uint32_t lBytesPerPixel              = arcDesc.mBpp / 8;
      if(lBytesPerPixel > 0)
      {
        mppIDs[aChnlIdx] = arcDesc;
        mppTcs[aChnlIdx]->mTd.mLineSize      = arcDesc.mWidth * lBytesPerPixel;
        mppTcs[aChnlIdx]->mTd.mDdrLineCnt    = arcDesc.mHeight;
        mppTcs[aChnlIdx]->mTd.mDdrLineStride = arcDesc.mWidthStep/32;
      } // if bpp > 8
      else
      {
        lRet = LIB_FAILURE;
        VDB_LOG_ERROR("Too low bpp (%u) in the image descriptor.\n",
                      arcDesc.mBpp);
      } // else from if bpp > 8
    } // if valid FDMA chnl index
    else
    {
      lRet = LIB_FAILURE;
      VDB_LOG_ERROR("Invalid FDMA channel requested.\n");
    } // else from if valid FDMA chanel index
  } // if initialized properly
  else
  {
    lRet = LIB_FAILURE;
    VDB_LOG_ERROR("FDMA IO not initialized properly.\n");
  } // if initialized properly

  return lRet;
} // DdrBufferDescGet(uint32_t aChnlIdx, SDI_ImageDescriptor arDesc)

//****************************************************************************

LIB_RESULT sdi_FdmaIO::DdrBuffersAlloc(uint32_t aCnt)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  for(uint32_t aChnlIdx = 0; aChnlIdx < FDMA_CHANNELS; aChnlIdx++)
  {
    if(mppTcs[aChnlIdx] != NULL)
    {
      lRet = DdrBuffersAlloc(aChnlIdx, aCnt);
    } // if TC index valid
  } // for all possible TCs

  return lRet;
} // sdi_FdmaIO::DdrBuffersAlloc(uint32_t aCnt)

//****************************************************************************

LIB_RESULT sdi_FdmaIO::DdrBuffersAlloc(uint32_t aChnlIdx,
                                       uint32_t aCnt)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if(aChnlIdx < FDMA_CHANNELS)
  {
    if(mppTcs[aChnlIdx] != NULL)
    {
      if(mppBuffArrs[aChnlIdx] == NULL)
      {
        // allocate new buffer array
        mppBuffArrs[aChnlIdx] = (SDI_DdrBufferArr*) new SDI_DdrBufferArr();
        if(mppBuffArrs[aChnlIdx] == NULL)
        {
          lRet = LIB_FAILURE;
        } // if allocation failed
        else
        {
          mppBuffArrs[aChnlIdx]->ChannelIdxSet(aChnlIdx);
        } // else from if allocation failed

        SDI_ImageDescriptor lDesc;

        DdrBufferDescGet(aChnlIdx, lDesc); // get the descriptor

        // allocate buffers
        if(mppBuffArrs[aChnlIdx]->Allocate(lDesc, aCnt) != LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        } // if Allocation failed
      } // if buffer array not assigned yet
      else
      {
        VDB_LOG_WARNING("Buffer array allocated already.\n");
      } // else from if buffer array not assigned yet
    } // if TC used
    else
    {
      lRet = LIB_FAILURE;
      VDB_LOG_ERROR("TC index %u not used.\n", aChnlIdx);
    } // else from if TC used
  } // if TC index valid
  else
  {
    lRet = LIB_FAILURE;
    VDB_LOG_ERROR("Invalid TC index %u.\n", aChnlIdx);
  } // else from if TC index valid

  return lRet;
} // sdi_FdmaIO::DdrBuffersAlloc(uint32_t aChnlIdx, uint32_t aCnt)

//****************************************************************************

LIB_RESULT sdi_FdmaIO::DdrBuffersAlloc(uint32_t aChnlIdx,
                                       uint32_t aCnt,
                                       SDI_ImageDescriptor &arImgDesc)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if(aChnlIdx < FDMA_CHANNELS)
  {
    if(mppTcs[aChnlIdx] != NULL)
    {
      if(mppBuffArrs[aChnlIdx] == NULL)
      {
        // allocate new buffer array
        mppBuffArrs[aChnlIdx] = (SDI_DdrBufferArr*) new SDI_DdrBufferArr();
        if(mppBuffArrs[aChnlIdx] == NULL)
        {
          lRet = LIB_FAILURE;
        } // if allocation failed
        else
        {
          mppBuffArrs[aChnlIdx]->ChannelIdxSet(aChnlIdx);
        } // else from if allocation failed

        // allocate buffers
        if(mppBuffArrs[aChnlIdx]->Allocate(arImgDesc, aCnt) != LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        } // if Allocation failed
      } // if buffer array not assigned yet
      else
      {
        lRet = LIB_FAILURE;
        VDB_LOG_ERROR("Buffer array exists already.\n");
      } // else from if buffer array not assigned yet
    } // if TC used
    else
    {
      lRet = LIB_FAILURE;
      VDB_LOG_ERROR("TC index %u not used.\n", aChnlIdx);
    } // else from if TC used
  } // if TC index valid
  else
  {
    lRet = LIB_FAILURE;
    VDB_LOG_ERROR("Invalid TC index %u.\n", aChnlIdx);
  } // else from if TC index valid

  return lRet;
} // sdi_FdmaIO::DdrBuffersAlloc()

//****************************************************************************

LIB_RESULT sdi_FdmaIO::DdrBuffersFree(uint32_t aChnlIdx)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if(aChnlIdx < FDMA_CHANNELS)
  {
    if(mppBuffArrs[aChnlIdx] != NULL)
    {
      mppBuffArrs[aChnlIdx]->Free();
    } // if channel used
    else
    {
      lRet = LIB_FAILURE;
    } // else from if channel used
  } // if channel index valid
  else
  {
    lRet = LIB_FAILURE;
  } // else from if channel index valid

  return lRet;
} // DdrBuffersFree(uint32_t aChnlIdx)

//****************************************************************************

LIB_RESULT sdi_FdmaIO::DdrBuffersSet(SDI_DdrBufferArr &arBuffers)
{
  LIB_RESULT lRet   = LIB_SUCCESS;
  uint32_t lChnlIdx = arBuffers.ChannelIdxGet();

  if(lChnlIdx < FDMA_CHANNELS)
  {
    if(mppTcs[lChnlIdx] != NULL)
    {
      if(mppBuffArrs[lChnlIdx] == NULL)
      {
        // remember new buffer array
        mppBuffArrs[lChnlIdx] = \
          (SDI_DdrBufferArr*) new SDI_DdrBufferArr(arBuffers);
        if(mppBuffArrs[lChnlIdx] == NULL)
        {
          VDB_LOG_ERROR("Failed to allocate ne buffer array.\n");
          lRet = LIB_FAILURE;
        } // if allocation failed
        else
        {
          mppBuffArrs[lChnlIdx]->ChannelIdxSet(lChnlIdx);
        } // else from if allocation failed
      } // if buffer array not assigned yet
      else
      {
        lRet = LIB_FAILURE;
        VDB_LOG_ERROR("Buffer array exists already.\n");
      } // else from if buffer array not assigned yet
    } // if TC used
    else
    {
      lRet = LIB_FAILURE;
      VDB_LOG_ERROR("TC index %u not used.\n", lChnlIdx);
    } // else from if TC used
  } // if TC index valid
  else
  {
    lRet = LIB_FAILURE;
    VDB_LOG_ERROR("Invalid TC index %u.\n", lChnlIdx);
  } // else from if TC index valid

  return lRet;
}

//****************************************************************************

LIB_RESULT sdi_FdmaIO::DdrBufferQuery(uint32_t aChnlIdx,
                                      uint32_t aBufferIdx,
                                      vsdk::UMat &arBuffer)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if(aChnlIdx < FDMA_CHANNELS)
  {
    if(mppBuffArrs[aChnlIdx] != NULL)
    {
      arBuffer = mppBuffArrs[aChnlIdx]->Query(aBufferIdx);
    } // if buffer used
    else
    {
      lRet = LIB_FAILURE;
    } // else from if buffer used
  } // if valid buffer
  else
  {
    lRet = LIB_FAILURE;
  } // else from if valid buffer

  return lRet;
} // sdi_FdmaIO::DdrBufferQuery()

//****************************************************************************

LIB_RESULT sdi_FdmaIO::TcGet(FDMA_Tc_t &arTc)
{
  LIB_RESULT lRet     = LIB_SUCCESS;
  uint32_t   lChnlIdx = arTc.mTdm.mTdIdx;

  if(lChnlIdx < FDMA_CHANNELS)
  {
    if(mppTcs[lChnlIdx] != NULL)
    {
      arTc = *(mppTcs[lChnlIdx]);
      arTc.mTdm.mTdIdx = lChnlIdx;
    } // if buffer used
    else
    {
      lRet = LIB_FAILURE;
    } // else from if buffer used
  } // if valid buffer
  else
  {
    lRet = LIB_FAILURE;
  } // else from if valid buffer

  return lRet;
} // sdi_FdmaIO::TcGet()

//****************************************************************************

LIB_RESULT sdi_FdmaIO::TcSet(FDMA_Tc_t &arTc)
{
  LIB_RESULT lRet     = LIB_SUCCESS;
  uint32_t   lChnlIdx = arTc.mTdm.mTdIdx;

  if(lChnlIdx < FDMA_CHANNELS)
  {
    if(mppTcs[lChnlIdx] != NULL)
    {
      *(mppTcs[lChnlIdx]) = arTc;
    } // if buffer used
    else
    {
      lRet = LIB_FAILURE;
    } // else from if buffer used
  } // if valid buffer
  else
  {
    lRet = LIB_FAILURE;
  } // else from if valid buffer

  return lRet;
} // sdi_FdmaIO::TcSet()

//****************************************************************************

sdi_FdmaIO::~sdi_FdmaIO()
{
  mppTcs = NULL;

  // release if not OFF
  if(mState != OFF)
  {
    (void)Release();
  } // if not off

  for(uint32_t i = 0; i < FDMA_CHANNELS; i++)
  {
    if(mppBuffArrs[i] != NULL)
    {
      delete mppBuffArrs[i];
    } // if channel used
  } // for all possible channels
} // sdi_FdmaIO::~sdi_FdmaIO()

//****************************************************************************

// *** sdi_H264EncIO ***

//****************************************************************************

sdi_H264EncIO::sdi_H264EncIO():sdi_io(SEQ_OTHRIX_H264ENC)
{ 
  memset(&mConfig, 0, sizeof(H264ENC_Config_t)); 

  mTypeIndex = SEQ_OTHRIX_H264ENC;
  
  // *** data popping ***
  mChunkSize   = 0;
  mTimeoutMs   = 0; 
  
  // *** output buffer info ***
  mOutBufStart   = 0;
  mOutBufEnd     = 0;
  mOutBufSize    = 0;
  mOutBufRead    = 0;
  mpOutBuff      = NULL;
  mpOutBuffBase  = NULL; 
  mOutBuffMapSize = 0; 
} // sdi_H264EncIO::sdi_H264EncIO()

//****************************************************************************

sdi_H264EncIO::sdi_H264EncIO(sdi_graph *apGraph):
  sdi_io(SEQ_OTHRIX_H264ENC, apGraph)
{
  // get data from the graph
  mDesc = mpGraph->H264EncDescGet();
  
  memset(&mConfig, 0, sizeof(H264ENC_Config_t));   
  
  mConfig.mFrameRate      = 30;  
  mConfig.mQpInit         = 32;
  mConfig.mQpMin          = 15;
  mConfig.mQpMax          = 51;
  mConfig.mQpFallback     = 2;
  
  mConfig.mBitsPerMbrw    = 49000;
  
  mConfig.mQpIncDiff0     = 6;
  mConfig.mQpIncDiff1     = 3;
  mConfig.mQpIncDiff2     = 2;
  mConfig.mQpIncOffset    = 5;
  mConfig.mQpIncThld0     = 2 * mConfig.mBitsPerMbrw; 
  mConfig.mQpIncThld1     = 1.625 * mConfig.mBitsPerMbrw;
  mConfig.mQpIncThld2     = 1.3125 * mConfig.mBitsPerMbrw;
  
  mConfig.mQpDecDiff0     = 4;
  mConfig.mQpDecDiff1     = 2;
  mConfig.mQpDecDiff2     = 1;
  mConfig.mQpDecOffset    = 5;
  mConfig.mQpDecThld0     = 0.5 * mConfig.mBitsPerMbrw; 
  mConfig.mQpDecThld1     = 0.75 * mConfig.mBitsPerMbrw;
  mConfig.mQpDecThld2     = 0.875 * mConfig.mBitsPerMbrw;
  
  mConfig.mWaitFrames     = 5;
  
  mConfig.mDisableDbf     = 0;
      
  mTypeIndex = SEQ_OTHRIX_H264ENC;
  
  // *** data popping ***
  mChunkSize   = 0;
  mTimeoutMs   = 0; 
  
  // *** output buffer info ***
  mOutBufStart   = 0;
  mOutBufEnd     = 0;
  mOutBufSize    = 0;
  mOutBufRead    = 0; 
  mpOutBuff      = NULL;
  mpOutBuffBase  = NULL; 
  mOutBuffMapSize = 0; 
} // sdi_H264EncIO::sdi_H264EncIO()

//****************************************************************************

LIB_RESULT sdi_H264EncIO::Reserve()
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if(mState != OFF)
  {
    VDB_LOG_WARNING("Failed to reserve. Reserved already before.\n");
  } // if not OFF
  else
  {
#ifndef __STANDALONE__
    if(H264ENC_Reserve() != H264ENC_LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to reserve the H264Encoder HW");
      lRet = LIB_FAILURE;
    } // if Reserve failed

    if(H264ENC_ResetVars() != H264ENC_LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to reset the H264Encoder driver");
      lRet = LIB_FAILURE;
    } // if reset failed    
    
    if(H264ENC_SwReset() != H264ENC_LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to reset the H264Encoder HW status");
      lRet = LIB_FAILURE;
    } // if reset failed
#endif //#ifndef __STANDALONE__
  } // else from if not OFF

  if(lRet == LIB_SUCCESS)
  {
    mState  = RESERVED;
  } // if all ok so far

  return lRet;
} // sdi_H264EncIO::Reserve()

//****************************************************************************

LIB_RESULT sdi_H264EncIO::Release()
{
  LIB_RESULT lRet = LIB_SUCCESS;

  // stop if still running
  if(mState == ON)
  {
    if(Stop() != LIB_SUCCESS)
    {
      lRet = LIB_FAILURE;
    } // if Stop() failed
  } // if still ON

#ifndef __STANDALONE__
  H264ENC_Release();
#endif // #ifndef __STANDALONE__

  if(mpOutBuff)
  {
#ifndef __STANDALONE__      
    munmap(mpOutBuffBase, mOutBuffMapSize);
#endif //#ifndef __STANDALONE__      
    mpOutBuff = NULL;    
  }
  
  mState  = OFF;

  return lRet;
} // sdi_H264EncIO::Release()

//****************************************************************************

LIB_RESULT sdi_H264EncIO::Setup()
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if((mState == OFF) || (mState == ON))
  {
    VDB_LOG_ERROR("Failed to setup. Current state is: [%s].\n", 
                  (mState == OFF)?"OFF":"ON");
    lRet = LIB_FAILURE;
  } // if not reserved yet
  else
  {
    // update data from the graph
    mDesc = mpGraph->H264EncDescGet();

#ifndef __STANDALONE__
    // *** initialize h264encoder interaction ***
    if(H264ENC_ResetVars() != H264ENC_LIB_SUCCESS)
    
    {
      VDB_LOG_ERROR("Failed to reset the H264Encoder driver");
      lRet = LIB_FAILURE;
    
    } // if reset failed    


    if(H264ENC_SwReset() != H264ENC_LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to reset the H264Encoder HW status");
      lRet = LIB_FAILURE;
    } // if reset failed

    if(mConfig.mFrameWidth > 176)
    {
      H264ENC_Configure(mConfig);
      
      H264ENC_Buffers_t lBuffers;
      
      // *** Buffer setup ***
      lBuffers.mY.mPhysAddr = mDesc.mpCompDesc[0].mBufAddr;   // Y input buffer
      lBuffers.mU.mPhysAddr = mDesc.mpCompDesc[1].mBufAddr;
      lBuffers.mV.mPhysAddr = mDesc.mpCompDesc[2].mBufAddr;

      lBuffers.mLineNum = mDesc.mpCompDesc[1].mLineCnt;  
    
      lBuffers.mBankStride = 8;   // 0-7 (>7 => disabled)
      
      // output bitstream data
      lBuffers.mBitstream.mPhysAddr = mDesc.mOutBuffAddr;    
      lBuffers.mBitstream.mSize     = mDesc.mOutBuffSize; 

      mOutBufStart = mDesc.mOutBuffAddr;
      mOutBufEnd   = mDesc.mOutBuffAddr + mDesc.mOutBuffSize;  
      mOutBufSize  = mDesc.mOutBuffSize; 
      mOutBufRead  = mDesc.mOutBuffAddr; 
      
      // create virtuall mapping for the SRAM outbuffer
      if(mpOutBuffBase)
      {
        if(munmap(mpOutBuffBase, mOutBuffMapSize) != 0)
        {
          VDB_LOG_WARNING("Failed to unmap previous out buffer mapping.\n");
        } // if unmapping failed
        mpOutBuffBase = NULL;
        mpOutBuff     = NULL;
      } // if out buffer mapped allready
      
#ifndef __STANDALONE__      
      int32_t  lFd = 0;
      lFd = open("/dev/mem", O_RDWR);
      
      if(lFd < 0)
      {
        VDB_LOG_ERROR("Failed to open /dev/mem\n");
        lRet = LIB_FAILURE;
      } //if failed to open /dev/mem
      else
      {      
        uint32_t lPhysAddrBase =mDesc.mOutBuffAddr & 0xfffff000;
        mOutBuffMapSize = (mOutBufEnd - lPhysAddrBase + 0x1000)&0xfffff000;
        mpOutBuffBase   = mmap( NULL, 
                                mOutBuffMapSize, 
                                PROT_READ | PROT_WRITE, 
                                MAP_SHARED, 
                                lFd, 
                                lPhysAddrBase);
        if(mpOutBuffBase == NULL)
        {
          VDB_LOG_ERROR("Failed to map output buffer.\n");
          lRet = LIB_FAILURE;
        } // if failed to map the out buffer
        else
        {
          mpOutBuff       = (uint64_t*)((uintptr_t)mpOutBuffBase + \
                                         mOutBufStart - lPhysAddrBase);
          H264ENC_BuffersSet(lBuffers);    
        } // else from if failed to map the out buffer 
        
        close(lFd);
      } // else from if failed to open /dev/mem
#endif // ifndef __STANDALONE__
    } // if config provided already
    else
    {
      VDB_LOG_ERROR("Failed to setup. Please provide meaningful encoding parameters first.\n");
      lRet = LIB_FAILURE;
    } // else from if config provided already
#endif // #ifndef __STANDALONE__
  } // else from if not reserved yet

  if(lRet == LIB_SUCCESS)
  {
    mState  = SETUP;
  } // if all ok so far

  return lRet;
} // sdi_H264EncIO::Setup()

//***************************************************************************

LIB_RESULT sdi_H264EncIO::Start(void)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if((mState != SETUP) || (mChunkSize == 0))
  {
    VDB_LOG_ERROR("Failed to start. Not setup yet.\n");
    lRet = LIB_FAILURE;
  } // if not setup yet

  if(lRet == LIB_SUCCESS)
  {
    mState  = ON;
  } // if all ok so far

  return lRet;
} // sdi_H264EncIO::Start(void)

//***************************************************************************

LIB_RESULT sdi_H264EncIO::Stop(void)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if(mState == ON)
  {
#ifndef __STANDALONE__
    if(H264ENC_ResetVars() != H264ENC_LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to reset the H264Encoder HW");
      lRet = LIB_FAILURE;
    } // if reset failed
#endif // #ifndef __STANDALONE__

    mState  = SETUP;
  } // if ON
  else
  {
    VDB_LOG_WARNING("Failed to stop. Not ON yet.\n");
  } // else from if not ON

  return lRet;
} // sdi_H264EncIO::Stop(void)

//****************************************************************************

LIB_RESULT sdi_H264EncIO::ConfigSet(const H264ENC_Config_t &arcConfig)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
  // *** Encoding parameters ***
  if(arcConfig.mDataModeChroma > 1)  
  {
    VDB_LOG_ERROR("Wrong chroma mode.\n");
    lRet = LIB_FAILURE;
  }
  
  if(arcConfig.mFetchTrig > 1)
  {
    VDB_LOG_ERROR("Wrong fatch trigger.\n");
    lRet = LIB_FAILURE;
  }
  
  if(arcConfig.mColorFormat > 1)
  {
    VDB_LOG_ERROR("Wrong color format\n");
    lRet = LIB_FAILURE;
  }
  
  if(arcConfig.mBitWidth > 2)
  {
    VDB_LOG_ERROR("Bit width out of bounds\n");
    lRet = LIB_FAILURE;
  }
    
  if((arcConfig.mFrameWidth > 1920) || (arcConfig.mFrameWidth < 176))
  {
    VDB_LOG_ERROR("Frame width out of bounds\n");
    lRet = LIB_FAILURE;
  }
  
  if((arcConfig.mFrameHeight > 1088) || (arcConfig.mFrameHeight < 144))
  {
    VDB_LOG_ERROR("Frame height out of bounds\n");
    lRet = LIB_FAILURE;
  }
  
  if((arcConfig.mFrameRate < 25) || (arcConfig.mFrameRate > 60)) 
  {
    VDB_LOG_ERROR("Frame rate out of bounds\n");
    lRet = LIB_FAILURE;
  }
  
  if((arcConfig.mQpInit > 51) || (arcConfig.mQpInit < (12 * arcConfig.mBitWidth)))
  {
    VDB_LOG_ERROR("QP init out of bounds\n");
    lRet = LIB_FAILURE;
  }
  if((arcConfig.mQpMin > 51) || (arcConfig.mQpMin < (12 * arcConfig.mBitWidth)))
  {
    VDB_LOG_ERROR("QP minimum out of bounds\n");
    lRet = LIB_FAILURE;
  }
  if((arcConfig.mQpMax > 51) || (arcConfig.mQpMax < (12 * arcConfig.mBitWidth)))
  {
    VDB_LOG_ERROR("QP maximum out of bounds\n");
    lRet = LIB_FAILURE;
  }
  if(arcConfig.mQpFallback > 68)
  {
    VDB_LOG_ERROR("QP fallback out of bounds\n");
    lRet = LIB_FAILURE;
  }    
    
  if((arcConfig.mBitsPerMbrw < 5000) || (arcConfig.mBitsPerMbrw > 111111))
  {
    VDB_LOG_ERROR("Bitrate out of bounds\n");
    lRet = LIB_FAILURE;
  }
    
  if(arcConfig.mQpIncDiff0 > (51 + 12 * arcConfig.mBitWidth))
  {
    VDB_LOG_ERROR("QP inc diff0 out of bounds\n");
    lRet = LIB_FAILURE;
  }
  if(arcConfig.mQpIncDiff1 > (51 + 12 * arcConfig.mBitWidth))
  {
    VDB_LOG_ERROR("QP inc diff1 out of bounds\n");
    lRet = LIB_FAILURE;
  }
  if(arcConfig.mQpIncDiff2 > (51 + 12 * arcConfig.mBitWidth))
  {
    VDB_LOG_ERROR("QP inc diff2 out of bounds\n");
    lRet = LIB_FAILURE;
  }
  if(arcConfig.mQpIncOffset > (51 + 12 * arcConfig.mBitWidth))
  {
    VDB_LOG_ERROR("QP inc offset out of bounds\n");
    lRet = LIB_FAILURE;
  }
  
  if(arcConfig.mQpDecDiff0 > (51 + 12 * arcConfig.mBitWidth))
  {
    VDB_LOG_ERROR("QP dec diff0 out of bounds\n");
    lRet = LIB_FAILURE;
  }
  if(arcConfig.mQpDecDiff1 > (51 + 12 * arcConfig.mBitWidth))
  {
    VDB_LOG_ERROR("QP dec diff1 out of bounds\n");
    lRet = LIB_FAILURE;
  }
  if(arcConfig.mQpDecDiff2 > (51 + 12 * arcConfig.mBitWidth))
  {
    VDB_LOG_ERROR("QP dec diff2 out of bounds\n");
    lRet = LIB_FAILURE;
  }
  if(arcConfig.mQpDecOffset > (51 + 12 * arcConfig.mBitWidth))
  {
    VDB_LOG_ERROR("QP dec offset out of bounds\n");
    lRet = LIB_FAILURE;
  }
  
  if(arcConfig.mWaitFrames > 63)
  {
    VDB_LOG_ERROR("Wait frames out of bounds\n");
    lRet = LIB_FAILURE;
  }
  
  if(arcConfig.mDisableDbf > 1)
  {
    VDB_LOG_ERROR("Wrong deblocking filter setup\n");
    lRet = LIB_FAILURE;
  }
  
  if(lRet == LIB_SUCCESS)
  {
    mConfig = arcConfig;
  } // if all ok
  
  return lRet;
} // sdi_H264EncIO::ConfigSet()

//****************************************************************************

H264ENC_Config_t sdi_H264EncIO::ConfigGet()
{
  return mConfig;
} // sdi_H264EncIO::ConfigGet()

//****************************************************************************

LIB_RESULT sdi_H264EncIO::OutChunkSet(uint32_t aChunkSizeBytes, 
                                      uint32_t aTimeoutMs)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
  if(aChunkSizeBytes % 8)
  {
    VDB_LOG_ERROR("Chunk size not alligned to 8bytes.\n");
    lRet = LIB_FAILURE;
  } // if unaligned chunk size
  else
  {
    mChunkSize = aChunkSizeBytes;
    mTimeoutMs = aTimeoutMs;
  }// else from if unaligned chunk size
  
  return lRet;
} //OutChunkSet()
  
//****************************************************************************
LIB_RESULT sdi_H264EncIO::DataPop(uint64_t *apBuff)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
#ifndef __STANDALONE__    
  struct timespec lTimeStart; 
  
  clock_gettime(CLOCK_REALTIME, &lTimeStart);
  
  if(apBuff != NULL)
  {
    H264ENC_Status_t lEncStatus;
    uint32_t lChunkSize = 0;
    uint32_t i = 0;
    
    H264ENC_StatusGet(lEncStatus);
    lEncStatus.mOutCurrentAddress += SRAM_BASE;
    
    if(lEncStatus.mOutCurrentAddress > mOutBufRead)
    {
      lChunkSize = lEncStatus.mOutCurrentAddress - mOutBufRead;
    }
    else
    {
      if(lEncStatus.mOutCurrentAddress > SRAM_BASE)
      {
        lChunkSize  = mOutBufEnd - mOutBufRead;
        lChunkSize += lEncStatus.mOutCurrentAddress - mOutBufStart;
      }// if not 0
    }
    
    // wait for rest of the chunk if needed
    while(lChunkSize < mChunkSize)
    {
      struct timespec lTimeNow; 
      clock_gettime(CLOCK_REALTIME, &lTimeNow);
      uint32_t lTimeSpent = (lTimeNow.tv_sec - lTimeStart.tv_sec)*1000 + \
                            (lTimeNow.tv_nsec - lTimeStart.tv_nsec)/1.0e6;
      
      if(lTimeSpent > mTimeoutMs)
      {
        VDB_LOG_WARNING("Wait for out chunk data timed out\n");
        lRet = LIB_FAILURE;
        break;        
      } // if timeout reached
      
      //TODO: introduce passive sleep once signalling from driver is there
      if(H264ENC_StatusGet(lEncStatus) != H264ENC_LIB_SUCCESS)
      {
        lRet = LIB_FAILURE;
        break;
      } // if StatusGet() failed
      
      lEncStatus.mOutCurrentAddress += SRAM_BASE;

      if(lEncStatus.mOutCurrentAddress > mOutBufRead)
      {
        lChunkSize  = lEncStatus.mOutCurrentAddress - mOutBufRead;
      } // if current address higher than last
      else
      {
        if(lEncStatus.mOutCurrentAddress > SRAM_BASE)
        {
          lChunkSize  = mOutBufEnd - mOutBufRead;
          lChunkSize += lEncStatus.mOutCurrentAddress - mOutBufStart;
        }// if not 0
        else
        {
          lChunkSize = 0;
        }
      } // else from if current address higher than last
      i++;
    } // while chunk not big enough   
    
    lChunkSize &= 0xfffffff8; // make sure alligned to 8 bytes
    
    if((lEncStatus.mInterruptStatus & H264ENC_CBUF_OVF) == 0)
    {
      if(lChunkSize >= mChunkSize)
      {
        uint32_t lSrci = (mOutBufRead - mOutBufStart) >> 3;
        uint32_t lDsti = 0;

        lChunkSize     = mChunkSize;
        while(lChunkSize > 0)
        {        
          apBuff[lDsti++]   = mpOutBuff[lSrci++];
          
          lChunkSize  -= sizeof(uint64_t);
          mOutBufRead += sizeof(uint64_t);
          if(mOutBufRead >= mOutBufEnd)
          {
            lSrci       = 0;
            mOutBufRead = mOutBufStart;
          } // if end reached
        } // while something to copy
        if(H264ENC_AlarmShift(mChunkSize) != H264ENC_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        } // if failed to shift alaram address
      } // if chunksize intime
      else
      {
        VDB_LOG_ERROR("Wait for chunksize timed out.");
        lRet = LIB_FAILURE;
      }// else from if chunksize intime
    } // if no overflow
    else
    {
      VDB_LOG_ERROR("H264 encoder output buffer overflow detected.\n");
      lRet = LIB_FAILURE;
    } // else from if no overflow
  } // if buffer not NULL
  else
  {
    VDB_LOG_ERROR("Buffer pointer is NULL");
    lRet = LIB_FAILURE;
  } // else from if buffer not NULL
#endif // #ifndef __STANDALONE__    
  return lRet;
} // sdi_H264EncIO::DataPop()

//****************************************************************************

LIB_RESULT sdi_H264EncIO::DataPop(uint64_t *apBuff, uint32_t &arSize)
{
  LIB_RESULT lRet = LIB_SUCCESS;
#ifndef __STANDALONE__        
  if(apBuff != NULL)
  {
    H264ENC_Status_t lEncStatus;
    uint32_t lChunkSize = 0;
    
    if(H264ENC_StatusGet(lEncStatus) != H264ENC_LIB_SUCCESS)
    {
      lRet = LIB_FAILURE;
    } // if failed to get status
    else
    {
      lEncStatus.mOutCurrentAddress += SRAM_BASE;
      if(lEncStatus.mOutCurrentAddress > mOutBufRead)
      {
        lChunkSize = lEncStatus.mOutCurrentAddress - mOutBufRead;
      } // if no wrap
      else
      {
        if(lEncStatus.mOutCurrentAddress > SRAM_BASE)
        {
          lChunkSize  = mOutBufEnd - mOutBufRead;
          lChunkSize += lEncStatus.mOutCurrentAddress - mOutBufStart;
        }// if not 0
      } // else from if no wrap
        
      lChunkSize &= 0xfffffff8; // make sure alligned to 8 bytes
      
      if(lChunkSize >= arSize)
      {
        lChunkSize = arSize & 0xfffffff8;
      } // if more data available
      
      arSize = lChunkSize;
        
      uint32_t lSrci = (mOutBufRead - mOutBufStart) >> 3;
      uint32_t lDsti = 0;
      
      while(lChunkSize > 0)
      {        
        apBuff[lDsti++]   = mpOutBuff[lSrci++];
        
        lChunkSize  -= sizeof(uint64_t);
        mOutBufRead += sizeof(uint64_t);
        if(mOutBufRead >= mOutBufEnd)
        {
          lSrci       = 0;
          mOutBufRead = mOutBufStart;
        } // if end reached
      } // while something to copy
    } // else from if get status failed
  } // if buffer not NULL
  else
  {
    VDB_LOG_ERROR("Buffer pointer is NULL");
    lRet = LIB_FAILURE;
  } // else from if buffer not NULL
#endif // #ifndef __STANDALONE__    
  return lRet;
} // sdi_H264EncIO::DataPop()

//****************************************************************************

LIB_RESULT sdi_H264EncIO::ParameterSet(SDI_IO_PARAM aParamId, void *apValue)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  // TODO: remove unused warning suppression macro
  UNUSED(aParamId);
  UNUSED(apValue);

  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return lRet;
} // sdi_H264EncIO::ParameterSet(SDI_IO_PARAM aParamId, void *apValue)

//****************************************************************************

LIB_RESULT sdi_H264EncIO::ParameterGet(SDI_IO_PARAM aParamId, void *apValue)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  // TODO: remove unused warning suppression macro
  UNUSED(aParamId);
  UNUSED(apValue);

  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return lRet;
} // sdi_H264EncIO::ParameterGet(SDI_IO_PARAM aParamId, void *apValue)

//****************************************************************************

LIB_RESULT sdi_H264EncIO::StreamOutBuffGet(uint32_t &arAddress,
                                           uint32_t &arSize)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  arAddress = mDesc.mOutBuffAddr;
  arSize    = mDesc.mOutBuffSize;

  return lRet;
} // sdi_H264EncIO::StreamOutBuffGet()

//****************************************************************************

sdi_H264EncIO::~sdi_H264EncIO()
{
  if(mState != OFF)
  {
    Release();
  } // if all ok so far
} // sdi_H264EncIO::~sdi_H264EncIO()

//****************************************************************************

// *** sdi_H264DecIO ***

//****************************************************************************

sdi_H264DecIO::sdi_H264DecIO():sdi_io(SEQ_OTHRIX_H264DEC)
{
  memset(&mDesc, 0, sizeof(mDesc));
  mTypeIndex  = SEQ_OTHRIX_H264DEC;
  mStreamCnt  = 0;
  for(uint32_t i = 0; i < H264DEC_STRM_CNT; i++)
  {
    mDesc.mpStreamDesc[i].mColFmt            = COL_YUV420;
    mDesc.mpStreamDesc[i].mBitWidth          = MODE_8BITS;
    mDesc.mpStreamDesc[i].mOutBufRestartMode = H264DCD_OFF;
    
    mppRefMem[i] = NULL;
  } // for all streams
} // sdi_H264DecIO::sdi_H264DecIO()

//****************************************************************************

sdi_H264DecIO::sdi_H264DecIO(sdi_graph *apGraph):
sdi_io(SEQ_OTHRIX_H264DEC, apGraph)
{
  memset(&mDesc, 0, sizeof(mDesc));
  // get data from the graph
  mpGraph->H264DecDescGet(mDesc);
  
  mDesc.mDataModeChroma      = OFFSET_BINARY_FORMAT;
  mDesc.mDataFlowMode        = MODE_1;
  mDesc.mDecConfig.mem_type  = MEMORY_DDR;
  mDesc.mDecConfig.filter_en = H264DCD_ON;
  
  mTypeIndex  = SEQ_OTHRIX_H264DEC;
  mStreamCnt  = 0;
  for(uint32_t i = 0; i < H264DEC_STRM_CNT; i++)
  {
    mDesc.mpStreamDesc[i].mColFmt            = COL_YUV420;
    mDesc.mpStreamDesc[i].mBitWidth          = MODE_8BITS;
    mDesc.mpStreamDesc[i].mOutBufRestartMode = H264DCD_OFF;
    
    mppRefMem[i] = NULL;
  } // for all streams
} // sdi_H264DecIO::sdi_H264DecIO()

//****************************************************************************

LIB_RESULT sdi_H264DecIO::Reserve()
{
  LIB_RESULT lRet = LIB_SUCCESS;

#ifndef __STANDALONE__  
  if(mState != OFF)
  {
    VDB_LOG_WARNING("Failed to reserve. Reserved already before.\n");
  } // if not OFF
  else
  {
    if(H264DEC_Open() != H264DEC_LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to reserve the H264Decoder HW");
      lRet = LIB_FAILURE;
    } // if Reserve failed

    if(H264DEC_SwReset() != H264DEC_LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to reset the H264Decoder HW");
      lRet = LIB_FAILURE;
    } // if reset failed
  } // else from if not OFF

  if(lRet == LIB_SUCCESS)
  {
    mState  = RESERVED;
  } // if all ok so far
#else // #ifndef __STANDALONE__
  VDB_LOG_FCN_NOT_IMPLEMENTED();
#endif //#ifndef __STANDALONE__  

  return lRet;
} // sdi_H264DecIO::Reserve()

//****************************************************************************

LIB_RESULT sdi_H264DecIO::Release()
{
  LIB_RESULT lRet = LIB_SUCCESS;

#ifndef __STANDALONE__  
  // stop if still running
  if(mState == ON)
  {
    Stop();
  } // if still ON

  H264DEC_Close();

  // free reference memory

  RefMemFree();
  mStreamCnt  = 0;
  mState      = OFF;
#else // #ifndef __STANDALONE__
#endif // #ifndef __STANDALONE__

  return lRet;
} // sdi_H264DecIO::Release()

//****************************************************************************

LIB_RESULT sdi_H264DecIO::Setup()
{
  LIB_RESULT lRet = LIB_SUCCESS;
#ifndef __STANDALONE__
  if((mState == OFF) || (mState == ON))
  {
    VDB_LOG_ERROR("Failed to setup. Current state is: [%s].\n", 
                  (mState == OFF)?"OFF":"ON");
    lRet = LIB_FAILURE;
  } // if not reserved yet
  else
  {
    // *** common setup to all streams ***
    H264DCD_STREAM_ID lpStrIds[] = {
      H264DCD_DATASTREAM_1,
      H264DCD_DATASTREAM_2,
      H264DCD_DATASTREAM_3,
      H264DCD_DATASTREAM_4
    }; // available stream IDs
    
    // SMB = sub-macroblock (4x4 pix);
    // 8bit -> 3words/SMB, 10&12bit -> 6 words/SMB
    uint32_t lWordsPerSmb  = 3; // word per SMB
    uint32_t lRefFrmCnt    = 1; // required number of reference frames
    
    // update data from the graph
    if(mpGraph->H264DecDescGet(mDesc) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to get H264decoder descriptor.\n");
      lRet = LIB_FAILURE;
    } // if failed to get H264dec desc
    else
    {    
      mStreamCnt = 0; // reset internal stream count value
      
      switch(mDesc.mDataFlowMode)
      {
        case MODE_0:
        {
          lRefFrmCnt = 17;
        } // case MODE_0
        break;
        case MODE_1:
        {
          lRefFrmCnt = 1;
        } // case MODE_1
        break;
        case MODE_2:
        {
          lRefFrmCnt = 2;
        } // case MODE_2
        break;
        default:
        {
          lRet = LIB_FAILURE;
          VDB_LOG_ERROR("Unknown h264decoder flow mode.\n");
        } // default
        break;          
      } // switch flowmode
      
      if(H264DEC_DataFlowModeSet(mDesc.mDataFlowMode) != H264DEC_LIB_SUCCESS)
      {
        lRet = LIB_FAILURE;
        VDB_LOG_ERROR("Failed to configure a flow mode\n");
      } // if flow mode not set
      
      if(H264DEC_DeblockSet(mDesc.mDecConfig) != H264DEC_LIB_SUCCESS)
      {
        lRet = LIB_FAILURE;
        VDB_LOG_ERROR("Failed to configure deblocking\n");
      } // if deblocking setup failed
    } // else from if failed to get H264dec desc
    
    // free all previously allocated reference memory
    RefMemFree();
    
    if(lRet == LIB_SUCCESS)
    {
      // *** per stream setup ***
      for(uint32_t i = 0; i < H264_DEC_NUM_CHN; i++)
      {
        if(!mDesc.mpStreamDesc[i].mActive)
        {
          continue;
        } // if stream not active
        
        mStreamCnt++;
        // *** output buffers ***
        H264DCD_OUTPUT_BUFFCFG lOutConfig;
        lOutConfig.stream_no    = lpStrIds[i];
        lOutConfig.y_outbuffer  =
          mDesc.mpStreamDesc[i].mpCompDesc[SDI_H264_COMPONENT_Y].mBufAddr;
        lOutConfig.cb_outbuffer =
          mDesc.mpStreamDesc[i].mpCompDesc[SDI_H264_COMPONENT_CB].mBufAddr;
        lOutConfig.cr_outbuffer =
          mDesc.mpStreamDesc[i].mpCompDesc[SDI_H264_COMPONENT_CR].mBufAddr;
        lOutConfig.lines_num    =
          mDesc.mpStreamDesc[i].mpCompDesc[SDI_H264_COMPONENT_Y].mLineCnt;

        if(H264DEC_OutConfig(lOutConfig) != H264DEC_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
          VDB_LOG_ERROR("Failed to configure output buffers\n");
          break;
        } // if output buff setup failed      

        // *** decoding paremters ***
        H264DCD_OUTPUT_COLOURDATA lOutColor;
        lOutColor.stream_no        = lpStrIds[i];
        lOutColor.col_format       = mDesc.mpStreamDesc[i].mColFmt;
        lOutColor.bit_width_mode   = mDesc.mpStreamDesc[i].mBitWidth;
        lOutColor.data_mode_format = mDesc.mDataModeChroma;

        if(H264DEC_OutColorDataSet(lOutColor)  != H264DEC_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
          VDB_LOG_ERROR("Failed to configure data format\n");
          break;
        } // if data format setup failed

        H264DCD_INFRAME_SIZE lFrmSize;
        lFrmSize.stream_no = lpStrIds[i];
          
        if(lOutColor.bit_width_mode == MODE_10_12BITS)
        {
          lFrmSize.x_size = \
           mDesc.mpStreamDesc[i].mpCompDesc[SDI_H264_COMPONENT_Y].mLineBytes>>5;
        } // if 2bytes/Ypixel
        else
        {
          lFrmSize.x_size = \
           mDesc.mpStreamDesc[i].mpCompDesc[SDI_H264_COMPONENT_Y].mLineBytes>>4;
        } // else if 2bytes/Ypixel
        lFrmSize.y_size     = mDesc.mpStreamDesc[i].mHeightLines >> 4;
        
        if(H264DEC_InFrameSizeSet(lFrmSize) != H264DEC_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
          VDB_LOG_ERROR("Failed to configure size\n");
          break;
        } // if size setup failed      
        
        H264DCD_OUTPUT_SAMPLEMODE lOutsampleMode;
        lOutsampleMode.stream_no      = lpStrIds[i];
        lOutsampleMode.outresmpl_mode = mDesc.mpStreamDesc[i].mOutBufRestartMode;
        if(H264DEC_OutSampleModeSet(lOutsampleMode) != H264DEC_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
          VDB_LOG_ERROR("Failed to configure buff restart\n");
          break;
        } // if buff restart setup failed      

        // *** reference memory ***
        if(mDesc.mDecConfig.filter_en == H264DCD_ON)
        {
          uint32_t lRefMemHeight = mDesc.mpStreamDesc[i].mHeightLines >> 2;
          uint32_t lRefMemSource = OAL_ALLOC_DDR1;
          uint32_t lRefMemSize   = 0;
                
          if(mDesc.mDecConfig.mem_type == MEMORY_SRAM)
          {
            lRefMemHeight = 2 * 16; // two macroblock lines only
            lRefMemSource    = OAL_ALLOC_SRAM_SINGLE;
          } // if ref mem in SRAM
            
          //             SMB_x_cnt     SMB_y_cnt    words/SMB bytes/word
          lRefMemSize  = (lFrmSize.x_size >> 2) * \
                          lRefMemHeight * lWordsPerSmb * 8 * lRefFrmCnt; 
          
          mppRefMemHandle[i] = OAL_MemoryAllocFlag(
                            lRefMemSize,
                            OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE)|
                            OAL_MEMORY_FLAG_CONTIGUOUS | lRefMemSource);
          if(mppRefMemHandle[i] == NULL)
          {
            VDB_LOG_ERROR("Failed to allocate reference memory for H264DEC.\n");
            lRet = LIB_FAILURE;
            break;
          } // if RefMem not allocated

          mppRefMem[i] = OAL_MemoryReturnAddress(
                            mppRefMemHandle[i],
                            ACCESS_PHY);

          if(mppRefMem[i] == NULL)
          {
            VDB_LOG_ERROR("Failed to get phy addr for reference memory.\n");
            lRet = LIB_FAILURE;
            break;
          } // if RefMem phy addr not received
          else
          {
            H264DCD_DECOD_REFMEM lRefMem;
            lRefMem.stream_no  = lpStrIds[i];
            lRefMem.refmem_addr = (uint32_t)(uintptr_t)mppRefMem[i];
            H264DEC_RefMemorySet(lRefMem);
          } // else from if RefMem phy addr not received
        } // if deblocking enabled
      }// for all possible streams
    }// if all ok so far
    
    if(lRet == LIB_SUCCESS)
    {
      mState  = SETUP;
    } // if all ok so far
    else
    {
      mStreamCnt = 0;
      // release any allocated resources so far.
      RefMemFree();
    } // else from if all ok so far
  } // else from if not reserved yet
#else // #ifndef __STANDALONE__
  VDB_LOG_FCN_NOT_IMPLEMENTED();
#endif // else from #ifndef __STANDALONE__
  return lRet;
} // sdi_H264DecIO::Setup()

//***************************************************************************

LIB_RESULT sdi_H264DecIO::Start(void)
{
  LIB_RESULT lRet = LIB_SUCCESS;

#ifndef __STANDALONE__  
  if(mState != SETUP)
  {
    VDB_LOG_ERROR("Failed to start. Not setup yet.\n");
    lRet = LIB_FAILURE;
  } // if not setup yet

  H264DCD_STREAM_ID lpStrIds[] = {
      H264DCD_DATASTREAM_1,
      H264DCD_DATASTREAM_2,
      H264DCD_DATASTREAM_3,
      H264DCD_DATASTREAM_4
  }; // available stream IDs
  for(uint32_t i = 0; i < H264_DEC_NUM_CHN; i++)
  {
    if(!mDesc.mpStreamDesc[i].mActive)
    {
      continue;
    } // if stream not active

    H264DCD_CH_CONTROL lChannelStop;
    lChannelStop.stream_no      = lpStrIds[i];
    lChannelStop.channel_status = CH_START;

    if(H264DEC_ChannelStop(lChannelStop) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to stop JPEG decoder.\n");
      lRet = LIB_FAILURE;
    } // if failed to start channel
  } // for all used channels

  if(lRet == LIB_SUCCESS)
  {
    mState  = ON;
  } // if all ok so far
#else // #ifndef __STANDALONE__
  VDB_LOG_FCN_NOT_IMPLEMENTED();
#endif // else from #ifndef __STANDALONE__

  return lRet;
} // sdi_H264DecIO::Start(void)

//***************************************************************************

LIB_RESULT sdi_H264DecIO::Stop(void)
{
  LIB_RESULT lRet = LIB_SUCCESS;
#ifndef __STANDALONE__
  if(mState == ON)
  {
    H264DCD_STREAM_ID lpStrIds[] = {
      H264DCD_DATASTREAM_1,
      H264DCD_DATASTREAM_2,
      H264DCD_DATASTREAM_3,
      H264DCD_DATASTREAM_4
    }; // available stream IDs

    for(uint32_t i = 0; i < H264_DEC_NUM_CHN; i++)
    {
      if(!mDesc.mpStreamDesc[i].mActive)
      {
        continue;
      } // if stream not active

      H264DCD_CH_CONTROL lChannelStop;
      lChannelStop.stream_no     = lpStrIds[i];
      lChannelStop.channel_status = CH_STOP;

      if(H264DEC_ChannelStop(lChannelStop) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Failed to stop H264 decoder channel.\n");
        lRet = LIB_FAILURE;
      } // if failed to stop the channel
    }// for all streams used

    if(lRet == LIB_SUCCESS)
    {
      mState  = SETUP;
    } // if all ok so far
  } // if ON
  else
  {
    VDB_LOG_WARNING("Failed to stop. Not ON yet.\n");
  } // else from if not ON
#else // #ifndef __STANDALONE__
  VDB_LOG_FCN_NOT_IMPLEMENTED();
#endif //else from #ifndef __STANDALONE__
  
  return lRet;
} // sdi_H264DecIO::Stop(void)

//****************************************************************************

LIB_RESULT sdi_H264DecIO::ParameterSet(SDI_IO_PARAM aParamId, void *apValue)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  // TODO: remove unused warning suppression macro
  UNUSED(aParamId);
  UNUSED(apValue);

  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return lRet;
} // sdi_H264DecIO::ParameterSet(SDI_IO_PARAM aParamId, void *apValue)

//****************************************************************************

LIB_RESULT sdi_H264DecIO::ParameterGet(SDI_IO_PARAM aParamId, void *apValue)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  // TODO: remove unused warning suppression macro
  UNUSED(aParamId);
  UNUSED(apValue);

  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return lRet;
} // sdi_H264DecIO::ParameterGet(SDI_IO_PARAM aParamId, void *apValue)

//****************************************************************************
  
void sdi_H264DecIO::ConfigSet(const SDI_H264DecDescriptor &arcConfig)
{
#ifndef __STANDALONE__  
  // coppy only the parameters that are allowed to be modified
  mDesc.mDataModeChroma = arcConfig.mDataModeChroma;
  mDesc.mDataFlowMode   = arcConfig.mDataFlowMode;
  mDesc.mDecConfig      = arcConfig.mDecConfig;
  
  for(uint32_t i = 0; i < H264DEC_STRM_CNT; i++)
  {
    mDesc.mpStreamDesc[i].mColFmt            = 
      arcConfig.mpStreamDesc[i].mColFmt;            
    mDesc.mpStreamDesc[i].mBitWidth          = 
      arcConfig.mpStreamDesc[i].mBitWidth;
    mDesc.mpStreamDesc[i].mOutBufRestartMode = 
      arcConfig.mpStreamDesc[i].mOutBufRestartMode; 
  } // for all streams
#else // #ifndef __STANDALONE__
  VDB_LOG_FCN_NOT_IMPLEMENTED();
#endif // else from #ifndef __STANDALONE__
} // sdi_H264DecIO::ConfigSet(const SDI_H264DecDescriptor &arcConfig)
  
//****************************************************************************
  
SDI_H264DecDescriptor sdi_H264DecIO::ConfigGet(void)
{  
#ifndef __STANDALONE__  
  mpGraph->H264DecDescGet(mDesc);
#else //#ifndef __STANDALONE__
  VDB_LOG_FCN_NOT_IMPLEMENTED();
#endif // else from #ifndef __STANDALONE__
  return mDesc;
} // SDI_H264DecDescriptor sdi_H264DecIO::ConfigGet(void)

//****************************************************************************

void sdi_H264DecIO::RefMemFree(void)
{
  // free reference memory
  for(uint32_t i = 0; i < H264DEC_STRM_CNT; i++)
  {
    if(mppRefMem[i] != NULL)
    {
      OAL_MemoryFree(mppRefMemHandle[i]);
      mppRefMem[i]       = NULL;
      mppRefMemHandle[i] = NULL;
    } // if mem allocated
  } // for all streams
} // sdi_H264DecIO::RefMemFree(void)

//****************************************************************************

sdi_H264DecIO::~sdi_H264DecIO()
{
  if(mState != OFF)
  {
    Release();
  } // if all ok so far
} // sdi_H264DecIO::~sdi_H264DecIO()
//****************************************************************************

// *** sdi_JpegDecIO ***

//****************************************************************************

sdi_JpegDecIO::sdi_JpegDecIO():sdi_io(SEQ_OTHRIX_JPEGDEC)
{
  memset(&mDesc, 0, sizeof(mDesc));

  mDesc.mMode        = COMPATIBILITY;
  mDesc.mOutBufSmpl  = JPEGDCD_OFF;
  
  for(uint32_t i = DATASTREAM_1; i < DATASTREAM_4; i++)
  {
    mDesc.mpStreamDesc[i].mPrecision     = PREC_8BITS;
    mDesc.mpStreamDesc[i].mWrapperConfig = JPEGDCD_OFF;
  } // for all streams
  
  mTypeIndex = SEQ_OTHRIX_JPEGDEC;
} // sdi_JpegDecIO::sdi_JpegDecIO()

//****************************************************************************

sdi_JpegDecIO::sdi_JpegDecIO(sdi_graph *apGraph):
sdi_io(SEQ_OTHRIX_JPEGDEC, apGraph)
{
  memset(&mDesc, 0, sizeof(mDesc));
  
  // get data from the graph
  (void) mpGraph->JpegDecDescGet(mDesc);
   
  mDesc.mMode        = COMPATIBILITY;
  mDesc.mOutBufSmpl  = JPEGDCD_OFF;
  
  for(uint32_t i = DATASTREAM_1; i < DATASTREAM_4; i++)
  {
    mDesc.mpStreamDesc[i].mPrecision     = PREC_8BITS;
    mDesc.mpStreamDesc[i].mWrapperConfig = JPEGDCD_OFF;
  } // for all streams
  
  mTypeIndex = SEQ_OTHRIX_JPEGDEC;
} // sdi_JpegDecIO::sdi_JpegDecIO()

//****************************************************************************

LIB_RESULT sdi_JpegDecIO::Reserve()
{
  LIB_RESULT lRet = LIB_SUCCESS;

#ifndef __STANDALONE__  
  if(mState != OFF)
  {
    if(mState != RESERVED)
    {
      VDB_LOG_ERROR("Failed to reserve. State is allready: [%s].\n", 
                    (mState == SETUP)?"SETUP":"ON");
      lRet = LIB_FAILURE;      
    } // if not reserved now
  } // if not OFF
  else
  {
    if(JPEGDEC_Open() != JPEGDEC_LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to Open JPEGDEC device.\n");
      lRet = LIB_FAILURE;
    } // if Open() failed
    else
    {
      mState  = RESERVED;
    } // else from if Open() failed
  } // else from not off
#else // #ifndef __STANDALONE__
  VDB_LOG_FCN_NOT_IMPLEMENTED();
#endif // else from #ifndef __STANDALONE__

  return lRet;
} // sdi_JpegDecIO::Reserve()

//****************************************************************************

LIB_RESULT sdi_JpegDecIO::Release()
{
  LIB_RESULT lRet = LIB_SUCCESS;

#ifndef __STANDALONE__  
  // stop if still running
  if(mState == ON)
  {
    Stop();
  } // if still ON

  if(JPEGDEC_Close() != JPEGDEC_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to close JPEGDEC device.\n");
    lRet = LIB_FAILURE;
  } // if Release() failed

  mState  = OFF;
#else // #ifndef __STANDALONE__
  VDB_LOG_FCN_NOT_IMPLEMENTED();
#endif // else from #ifndef __STANDALONE__  

  return lRet;
} // sdi_JpegDecIO::Release()
//****************************************************************************

LIB_RESULT sdi_JpegDecIO::Setup()
{
  LIB_RESULT lRet = LIB_SUCCESS;

#ifndef __STANDALONE__  
  if((mState == OFF) || (mState == ON))
  {
    VDB_LOG_ERROR("Failed to setup. Current state is: [%s].\n", 
                  (mState == OFF)?"OFF":"ON");
    lRet = LIB_FAILURE;
  } // if not reserved yet
  else
  {
    // update data from the graph
    if(mpGraph->JpegDecDescGet(mDesc) != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to get JPEGdecoder descriptor.\n");
      lRet = LIB_FAILURE;
    } // if failed to get jpeg dec desc
    else
    { 
      // software reset
      if(JPEGDEC_SwReset() != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("JPEG SW reset failed.\n");
        lRet = LIB_FAILURE;
      } // if SW reset failed      
      
      // setup Jpeg decoder output buffers
      OUTPUT_BUFFCFG lJpegDecOutBufs;
      lJpegDecOutBufs.c1_outbuffer =
        mDesc.mpCompDesc[SDI_JPEG_COMPONENT_Y].mBufAddr;
      lJpegDecOutBufs.c1_b2lines =
        mDesc.mpCompDesc[SDI_JPEG_COMPONENT_Y].mLineCnt;

      lJpegDecOutBufs.c2_outbuffer =
        mDesc.mpCompDesc[SDI_JPEG_COMPONENT_U].mBufAddr;
      lJpegDecOutBufs.c3_outbuffer =
        mDesc.mpCompDesc[SDI_JPEG_COMPONENT_V].mBufAddr;
      lJpegDecOutBufs.c2_c3_b2lines =
        mDesc.mpCompDesc[SDI_JPEG_COMPONENT_U].mLineCnt;

      lJpegDecOutBufs.c4_outbuffer =
        mDesc.mpCompDesc[SDI_JPEG_COMPONENT_A].mBufAddr;
      lJpegDecOutBufs.c4_b2lines =
        mDesc.mpCompDesc[SDI_JPEG_COMPONENT_A].mLineCnt;

      if(JPEGDEC_OutConfig(&lJpegDecOutBufs) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Failed to configure output buffers.\n");
        lRet = LIB_FAILURE;
      } // if output buffer config failed       
      
      // setup outsample mode
      if(JPEGDEC_OutSampleModeSet(mDesc.mOutBufSmpl) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Failed to configure sampling mode.\n");
        lRet = LIB_FAILURE;
      } // if sampling mode config failed       

      // setup bankstride
      if(JPEGDEC_BankStrideSet(mDesc.mBankStride) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Failed to configure bank stride.\n");
        lRet = LIB_FAILURE;
      } // if bank stride config failed
         
      // *** COMPATIBILITY -> CONTEXT_SWITCH mode ***
      // requires specific sequence to work correctly
      // 1 SwReset has to be done (here done already)
      // 2 CONTEXT_SWITCH mode set
      // 3 GO bit asserted
      // 4 GO bit reset
      // 5 SwReset has to be done again
      //
      // doesn't influence CONTEXT_SWITCH -> COMPATIBILITY
      {
        if(JPEGDEC_ModeSet(mDesc.mMode) != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Failed to configure context switching.\n");
          lRet = LIB_FAILURE;
        } // if context switching config failed 
        
        if(JPEGDEC_Start() != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Failed to sample regs by GO bit.\n");
          lRet = LIB_FAILURE;
        } // if GO bit setting failed 
        
        if(JPEGDEC_Stop() != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Failed to reset GO bit.\n");
          lRet = LIB_FAILURE;
        } // if GO bit reset failed 
        
        if(JPEGDEC_SwReset()  != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Failed to reset decoder HW.\n");
          lRet = LIB_FAILURE;
        } // if SW reset failed
      } // MODE setting procedure
      
      if(JPEGDEC_StreamNumSet(mDesc.mStreamCnt) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Failed to configure number of streams.\n");
        lRet = LIB_FAILURE;
      } // if stream num set failed       

      // setup reset interval
      if(JPEGDEC_RstIntervalSet(mDesc.mRstInterval) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Failed to configure reset interval.\n");
        lRet = LIB_FAILURE;
      } // if reset interval config failed       

      // setup stream inputs
      STREAM_JPEG_PARAM lStreamParams;
      
      for(uint32_t i = DATASTREAM_1; i <= DATASTREAM_4; i++)
      {
        lStreamParams.stream_no     = (STREAM_ID)i;
        lStreamParams.x_size        =
                    mDesc.mpCompDesc[SDI_JPEG_COMPONENT_Y].mLineBytes;
        if(mDesc.mpStreamDesc[i].mPrecision == PREC_12BITS)
        {
          lStreamParams.x_size    >>= 1; // divide by two
        } // if 12bits per component pixel
        lStreamParams.y_size        = mDesc.mpStreamDesc[i].mHeight;
        lStreamParams.precision     = mDesc.mpStreamDesc[i].mPrecision;
        lStreamParams.component_num = mDesc.mpStreamDesc[i].mComponentCnt;
        lStreamParams.wr_config     = mDesc.mpStreamDesc[i].mWrapperConfig;

        lStreamParams.c1_samplingfactor_h  = 
                            mDesc.mpStreamDesc[i].mC1_samplingfactor_h;
        lStreamParams.c1_samplingfactor_v  = 
                            mDesc.mpStreamDesc[i].mC1_samplingfactor_v;
        lStreamParams.c2_samplingfactor_h  = 
                            mDesc.mpStreamDesc[i].mC2_samplingfactor_h;
        lStreamParams.c2_samplingfactor_v  = 
                            mDesc.mpStreamDesc[i].mC2_samplingfactor_v;
        lStreamParams.c3_samplingfactor_h  = 
                            mDesc.mpStreamDesc[i].mC3_samplingfactor_h;
        lStreamParams.c3_samplingfactor_v  = 
                            mDesc.mpStreamDesc[i].mC3_samplingfactor_v;
        lStreamParams.c4_samplingfactor_h  = 
                            mDesc.mpStreamDesc[i].mC4_samplingfactor_h;
        lStreamParams.c4_samplingfactor_v  = 
                            mDesc.mpStreamDesc[i].mC4_samplingfactor_v;

        if(JPEGDEC_StreamConfig(&lStreamParams) != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Failed to configure JPEG stream.\n");
          lRet = LIB_FAILURE;
          break;
        } // if stream config failed       
      }// for all jpeg streams  
    } // else from if failed to get jpeg dec desc
  } // else from if not reserved yet

  if(lRet == LIB_SUCCESS)
  {
    mState  = SETUP;
  } // if all ok so far
#else // #ifndef __STANDALONE__
  VDB_LOG_FCN_NOT_IMPLEMENTED();
#endif //else from #ifndef __STANDALONE__
  return lRet;
} // sdi_JpegDecIO::Setup()

//***************************************************************************

LIB_RESULT sdi_JpegDecIO::Start(void)
{
  LIB_RESULT lRet = LIB_SUCCESS;

#ifndef __STANDALONE__  
  if(mState != SETUP)
  {
    VDB_LOG_ERROR("Failed to start. Not setup yet.\n");
    lRet = LIB_FAILURE;
  } // if not setup yet
  else
  {
    // 0xffffff ~= 50ms at 350MHZ jpeg clock
    if(JPEGDEC_TimeoutSet(0xfffffff) != LIB_SUCCESS)
    {
      lRet = LIB_FAILURE;
    } // if timeout failed
    else
    {
      if(JPEGDEC_Start() != JPEGDEC_LIB_SUCCESS)
      {
        lRet = LIB_FAILURE;
      } // if start failed
    } // else from if timeout failed
  } // else from if not setup yet

  if(lRet == LIB_SUCCESS)
  {
    mState  = ON;
  } // if all ok so far
#else //#ifndef __STANDALONE__
  VDB_LOG_FCN_NOT_IMPLEMENTED();
#endif // else from #ifndef __STANDALONE__

  return lRet;
} // sdi_JpegDecIO::Start(void)

//***************************************************************************

LIB_RESULT sdi_JpegDecIO::Stop(void)
{
  LIB_RESULT lRet = LIB_SUCCESS;

#ifndef __STANDALONE__  
  if(mState == ON)
  {
    if(JPEGDEC_Stop() != LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to stop JPEG decoder.\n");
      lRet = LIB_FAILURE;
    }
    mState  = SETUP;
  } // if ON
  else
  {
    VDB_LOG_WARNING("Failed to stop. Not ON yet.\n");
  } // else from if not ON
#else // #ifndef __STANDALONE__
  VDB_LOG_FCN_NOT_IMPLEMENTED();
#endif // else from #ifndef __STANDALONE__
  return lRet;
} // sdi_JpegDecIO::Stop(void)

//****************************************************************************

LIB_RESULT sdi_JpegDecIO::ParameterSet(SDI_IO_PARAM aParamId, void *apValue)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  // TODO: remove unused warning suppression macro
  UNUSED(aParamId);
  UNUSED(apValue);

  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return lRet;
} // sdi_JpegDecIO::ParameterSet(SDI_IO_PARAM aParamId, void *apValue)

//****************************************************************************

LIB_RESULT sdi_JpegDecIO::ParameterGet(SDI_IO_PARAM aParamId, void *apValue)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  // TODO: remove unused warning suppression macro
  UNUSED(aParamId);
  UNUSED(apValue);

  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return lRet;
} // sdi_JpegDecIO::ParameterGet(SDI_IO_PARAM aParamId, void *apValue)

//****************************************************************************

LIB_RESULT sdi_JpegDecIO::ConfigSet(const SDI_JpegDecDescriptor &arcConfig)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
#ifndef __STANDALONE__  
  // coppy only the parameters that are allowed to be modified  
  if((arcConfig.mStreamCnt > 0) && 
     (arcConfig.mStreamCnt <= (DATASTREAM_4 + 1)))
  {
    mDesc.mMode        = arcConfig.mMode;
    mDesc.mOutBufSmpl  = arcConfig.mOutBufSmpl;
    mDesc.mStreamCnt = arcConfig.mStreamCnt; 

    if(arcConfig.mBankStride <= SDI_JPEG_BANK_STRIDE_MAX)
    {
      mDesc.mBankStride  = arcConfig.mBankStride;
    } // if in range
    else
    {
      VDB_LOG_ERROR("Bank stride (%u) out of range <0, %u>.\n",
                    arcConfig.mStreamCnt,
                    (uint32_t)SDI_JPEG_BANK_STRIDE_MAX
      );
      mDesc.mStreamCnt  = 0; // set to 0 to mark incorrect config
      mDesc.mBankStride = 0;
      lRet = LIB_FAILURE;
    } // else from if in range 
    
    if( arcConfig.mRstInterval <= SDI_JPEG_RST_INTERVAL_MAX)
    {
      mDesc.mRstInterval = arcConfig.mRstInterval;
    }// if in range
    else
    {
      VDB_LOG_ERROR("Restart interval (%u) out of range <0, %u>.\n",
                    arcConfig.mRstInterval,
                    (uint32_t)SDI_JPEG_RST_INTERVAL_MAX
      );
      mDesc.mStreamCnt   = 0; // set to 0 to mark incorrect config
      mDesc.mRstInterval = 0; 
      lRet = LIB_FAILURE;
    } // else from if in range 
    
    for(uint32_t i = DATASTREAM_1; i < mDesc.mStreamCnt; i++)
    {
      mDesc.mpStreamDesc[i].mPrecision     = arcConfig.mpStreamDesc[i].mPrecision;
      mDesc.mpStreamDesc[i].mWrapperConfig = \
              arcConfig.mpStreamDesc[i].mWrapperConfig;
      
      if((arcConfig.mpStreamDesc[i].mComponentCnt == 1) ||
        (arcConfig.mpStreamDesc[i].mComponentCnt == 3) ||
        (arcConfig.mpStreamDesc[i].mComponentCnt == 4))
      {      
        mDesc.mpStreamDesc[i].mComponentCnt = \
                arcConfig.mpStreamDesc[i].mComponentCnt;
      } // if meaningful value provided
      else
      {
        VDB_LOG_ERROR("Bad component count (%u) (1,3,4 are allowed only).\n",
                      mDesc.mRstInterval,
                      (uint32_t)SDI_JPEG_RST_INTERVAL_MAX
        );
        mDesc.mStreamCnt   = 0; // set to 0 to mark incorrect config
        mDesc.mpStreamDesc[i].mComponentCnt = 0;
        lRet = LIB_FAILURE;
      } // else from if meaningful value provided    
      
      // no checks on sampling factor values
      mDesc.mpStreamDesc[i].mC1_samplingfactor_h = \
              arcConfig.mpStreamDesc[i].mC1_samplingfactor_h;
      mDesc.mpStreamDesc[i].mC1_samplingfactor_v = \
              arcConfig.mpStreamDesc[i].mC1_samplingfactor_v;
      mDesc.mpStreamDesc[i].mC2_samplingfactor_h = \
              arcConfig.mpStreamDesc[i].mC2_samplingfactor_h;
      mDesc.mpStreamDesc[i].mC2_samplingfactor_v = \
              arcConfig.mpStreamDesc[i].mC2_samplingfactor_v;
      mDesc.mpStreamDesc[i].mC3_samplingfactor_h = \
              arcConfig.mpStreamDesc[i].mC3_samplingfactor_h;
      mDesc.mpStreamDesc[i].mC3_samplingfactor_v = \
              arcConfig.mpStreamDesc[i].mC3_samplingfactor_v;
      mDesc.mpStreamDesc[i].mC4_samplingfactor_h = \
              arcConfig.mpStreamDesc[i].mC4_samplingfactor_h;
      mDesc.mpStreamDesc[i].mC4_samplingfactor_v = \
              arcConfig.mpStreamDesc[i].mC4_samplingfactor_v;    
    } // for all streams
  } // if stream cnt in range
  else
  {
    VDB_LOG_ERROR("Stream count (%u) out of range <%u, %u>.\n",
                  arcConfig.mStreamCnt,
                  (uint32_t)DATASTREAM_1,
                  (uint32_t)DATASTREAM_4
    );
    mDesc.mStreamCnt = 0;
    lRet = LIB_FAILURE;
  } // else from if stream cnt in range
#else // #ifndef __STANDALONE__
  VDB_LOG_FCN_NOT_IMPLEMENTED();
#endif  // else from #ifndef __STANDALONE__
  return lRet;
} // sdi_JpegDecIO::ConfigSet(const SDI_JpegDecDescriptor &arcConfig)
  
//****************************************************************************
  
SDI_JpegDecDescriptor sdi_JpegDecIO::ConfigGet(void)
{  
#ifndef __STANDALONE__  
  (void)mpGraph->JpegDecDescGet(mDesc);
#else // #ifndef __STANDALONE__
  VDB_LOG_FCN_NOT_IMPLEMENTED();
#endif // #ifndef __STANDALONE__
  return mDesc;
} // SDI_H264DecDescriptor sdi_JpegDecIO::ConfigGet(void)

//****************************************************************************

sdi_JpegDecIO::~sdi_JpegDecIO()
{
#ifndef __STANDALONE__  
  if(mState != OFF)
  {
    Release();
  } // if all ok so far
#else
  VDB_LOG_FCN_NOT_IMPLEMENTED();
#endif
} // sdi_JpegDecIO::~sdi_JpegDecIO()

//****************************************************************************

// *** sdi_MipiCsiIO ***

//****************************************************************************

sdi_MipiCsiIO::sdi_MipiCsiIO():sdi_io(SEQ_OTHRIX_MIPICSI0)
{
  mPPErrIrqMask  = CSI_IRQ_ERRPP_MASK_ALL;  // 0x00000027
  mPhyErrIrqMask = CSI_IRQ_ERRPHY_MASK_ALL; // 0x000fffff
  mFrameIrqMask  = CSI_IRQ_ERRLIN_MASK_ALL + CSI_IRQ_FRM_MASK_ALL;// 0x0000ffff
} // sdi_MipiCsiIO::sdi_MipiCsiIO()

//****************************************************************************

sdi_MipiCsiIO::sdi_MipiCsiIO(sdi_graph *apGraph, enum CSI_IDX aCsiIdx):
sdi_io(SEQ_OTHRIX_MIPICSI0, apGraph)
{
  mDesc   = mpGraph->MipiDescGet(aCsiIdx);
  if((uint32_t)aCsiIdx == CSI_IDX_1)
  {
    mTypeIndex = SEQ_OTHRIX_MIPICSI1;
  } // MipiCsi1 interface
  
  mPPErrIrqMask  = CSI_IRQ_ERRPP_MASK_ALL;  // 0x00000027
  mPhyErrIrqMask = CSI_IRQ_ERRPHY_MASK_ALL; // 0x000fffff
  mFrameIrqMask  = CSI_IRQ_ERRLIN_MASK_ALL + CSI_IRQ_FRM_MASK_ALL;// 0x0000ffff
} // sdi_MipiCsiIO::sdi_MipiCsiIO()

//****************************************************************************

LIB_RESULT sdi_MipiCsiIO::Reserve()
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if(mState != OFF)
  {
    VDB_LOG_WARNING("Failed to reserve. Reserved already before.\n");
  } // if not OFF
  else
  {
    if(CSI_Open() != CSI_LIB_SUCCESS)
    {
      VDB_LOG_ERROR("Failed to Open CSI device.\n");
      lRet = LIB_FAILURE;
    } // if CSI_Open() failed
    else
    {
      // update descriptor to get SRAM buffer address
      mDesc   = mpGraph->MipiDescGet(mDesc.mIdx);

      switch(mDesc.mCamType)
      {
        case CSI_SONY224:
        {
          if(SONY_Open((CSI_IDX)mDesc.mIdx) != CAM_LIB_SUCCESS)
          {
            lRet = LIB_FAILURE;
          } // if failed to Open Sony driver
        } // SONY224
          break;
        case CSI_OV10640:
        {
          if(OV10640_Open(mDesc.mIdx) != CAM_LIB_SUCCESS)
          {
            lRet = LIB_FAILURE;
          } // if failed to Open Ov10640 driver
        } // OV10640
          break;
        case CSI_MAX_SONY224:  // fallthrough
        case CSI_MAX_OV10640:
        case CSI_MAX_OV10635:
		case CSI_MAX9286_96705:
        {
          MAXIM_Cfg_t lConfig;
          lConfig.mCsiIdx     = mDesc.mIdx;
          lConfig.mSensorType = mDesc.mCamType;
          for(uint32_t i = 0; i < MAXIM_CAM_SLOTS; i++)
          {
            if(i < mDesc.mNumCameras)
            {
              lConfig.mSensors[i] = MAXIM_CAM_ENABLED;
            } // if camera attached
            else
            {
              lConfig.mSensors[i] = MAXIM_CAM_DISABLED;
            } // else from if camera attached
          } // for all attached chameras

          if(MAXIM_Open(lConfig)!= CAM_LIB_SUCCESS)
          {
            lRet = LIB_FAILURE;
          } // if failed to Open Maxim driver
        } // case MAX_OV10640
          break;
        // *** Unsupported camera types ***
        case CSI_SONY290:     // fallthrough
        case CSI_SONY390:     // fallthrough
        case CSI_OV2775:      // fallthrough
        case CSI_OV9716:      // fallthrough
        case CSI_PAN34420:    // fallthrough
        case CSI_GENERIC:     // fallthrough
        case CSI_MAX_GENERIC: // fallthrough
        case CSI_MAX_OV9716:  
        {
          // all realated setup has to be done outside SDI
          VDB_LOG_WARNING("Unsupported camera type.\n");
        } // case <unsupported camera type>
        break;
        // *** Unknow camera types ***
        default:
        {
          VDB_LOG_ERROR("Unknown MIPICSI camera type.\n");
          lRet = LIB_FAILURE;
        }
          break;
      } // switch camera type
    } // else from if CSI_Open() failed
  } // else from if not OFF

  if(lRet == LIB_SUCCESS)
  {
    mState  = RESERVED;
  } // if all ok so far

  return lRet;
} // sdi_MipiCsiIO::Reserve()

//****************************************************************************

LIB_RESULT sdi_MipiCsiIO::Release()
{
  LIB_RESULT lRet = LIB_SUCCESS;

  // stop if still running
  if(mState == ON)
  {
    Stop();
  } // if running

  if(mState != OFF)
  {
    switch(mDesc.mCamType)
    {
      case CSI_SONY224:
      {
        if(SONY_Close((CSI_IDX)mDesc.mIdx) != CAM_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        } // if failed to Close Sony driver
      } // SONY224
        break;
      case CSI_OV10640:
      {
        if(OV10640_Close(mDesc.mIdx) != CAM_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        } // if failed to Close Ov10640 driver
      } // OV10640
        break;
      case CSI_MAX_SONY224:  // fallthrough
      case CSI_MAX_OV10640:
      case CSI_MAX_OV10635:
	  case CSI_MAX9286_96705:
      {
        if(MAXIM_Close(mDesc.mIdx) != CAM_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        } // if failed to Close Maxim driver
      } // case MAX_OV10640
        break;
      // *** Unsupported camera types ***
      case CSI_SONY290:     // fallthrough
      case CSI_SONY390:     // fallthrough
      case CSI_OV2775:      // fallthrough
      case CSI_OV9716:      // fallthrough
      case CSI_PAN34420:    // fallthrough
      case CSI_GENERIC:     // fallthrough
      case CSI_MAX_GENERIC: // fallthrough
      case CSI_MAX_OV9716:  
      {
        // all realated setup has to be done outside SDI
        VDB_LOG_WARNING("Unsupported camera type.\n");
      } // case <unsupported camera type>
      break;
      // *** Unknow camera types ***
      default:
      {
        VDB_LOG_ERROR("Unknown MIPICSI camera type.\n");
        lRet = LIB_FAILURE;
      }
        break;
    } // switch camera type
    
    if(CSI_Close() != CSI_LIB_SUCCESS)
    {
      lRet = LIB_FAILURE;
    }
  } // if not OFF already

  mState  = OFF;

  return lRet;
} // sdi_MipiCsiIO::Release()
//****************************************************************************

LIB_RESULT sdi_MipiCsiIO::Setup()
{
  LIB_RESULT lRet = LIB_SUCCESS;
  CSI_Cfg_t lCsiConfig;

  if((mState == OFF) || (mState == ON))
  {
    VDB_LOG_ERROR("Failed to setup. Current state is: [%s].\n", 
                  (mState == OFF)?"OFF":"ON");
    lRet = LIB_FAILURE;
  } // if not reserved yet
  else
  {
    bool lUnsupportedCam = false;
    
    memset(&lCsiConfig, 0, sizeof(lCsiConfig));

    // update descriptor to get SRAM buffer address
    mDesc   = mpGraph->MipiDescGet(mDesc.mIdx);

    switch(mDesc.mCamType)
    {
      case CSI_SONY224:
      {
        SONY_Geometry_t lGeom;
        memset(&lGeom, 0, sizeof(SONY_Geometry_t));
        // configure camera
        if(SONY_RegConfig((CSI_IDX)mDesc.mIdx) != CAM_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        } // if failed to Config Sony regs
        lCsiConfig.mLanes = 2;
        lCsiConfig.mMbps  = 594;
      } // SONY224
        break;
      case CSI_OV10640:
      {
        if(OV10640_RegConfig(mDesc.mIdx) != CAM_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        } // if failed to Config Ov10640 regs

        lCsiConfig.mLanes = 2;
        lCsiConfig.mMbps  = 594;
      } // case CSI_OV10640
        break;
      case CSI_MAX_SONY224:
      {
        lCsiConfig.mLanes = 4;
        lCsiConfig.mMbps  = 369;
      } // case CSI_MAX_SONY224
      break;
      case CSI_MAX_OV10640:
      {
        lCsiConfig.mLanes = 4;
        lCsiConfig.mMbps  = 369;
      } // case CSI_MAX_OV10640
        break;
      case CSI_MAX_OV10635:
      {
        lCsiConfig.mLanes = 4;
        lCsiConfig.mMbps  = 369;
      } // case CSI_MAX_OV10635
        break;
	  case CSI_MAX9286_96705:
	  {
        lCsiConfig.mLanes = 4;
        lCsiConfig.mMbps  = 369;
      }
        break;	
      // *** Unsupported camera types ***
      case CSI_SONY290:     // fallthrough
      case CSI_SONY390:     // fallthrough
      case CSI_OV2775:      // fallthrough
      case CSI_OV9716:      // fallthrough
      case CSI_PAN34420:    // fallthrough
      case CSI_GENERIC:     // fallthrough
      case CSI_MAX_GENERIC: // fallthrough
      case CSI_MAX_OV9716:  
      {
        // all realated setup has to be done outside SDI
        VDB_LOG_WARNING("Unsupported camera type.\n");
        lUnsupportedCam = true;
      } // case <unsupported camera type>
      break;
      // *** Unknow camera types ***
      default:
      {
        // should not happen, generates error
        VDB_LOG_ERROR("Unknown MIPICSI camera type.\n");
        lRet = LIB_FAILURE;
        lUnsupportedCam = true;
      } // default
        break;
    } // switch camera type

    if(lUnsupportedCam == false)
    {
      // *** initialize CSI receiver ***
      lCsiConfig.mIdKey.mKey     = 0;
      lCsiConfig.mIdKey.mId.mCsi = mDesc.mIdx;
      lCsiConfig.mCamCnt         = mDesc.mNumCameras;

      for(int32_t i = 0; i < MIPI_VIRT_CHN_NUM; i++)
      {
        lCsiConfig.mpVc[i].mBufAddr     = mDesc.mpVc[i].mBufAddr;
        lCsiConfig.mpVc[i].mDoneLineCnt = 1;
        lCsiConfig.mpVc[i].mLineCnt     = mDesc.mpVc[i].mLineCnt; // buffer lines
        lCsiConfig.mpVc[i].mFrameHeight = mDesc.mpVc[i].mFrameHeight;
        lCsiConfig.mpVc[i].mLineBytes   = mDesc.mpVc[i].mLineBytes;
        lCsiConfig.mpVc[i].mLineStride  = mDesc.mpVc[i].mLineStride;

        if(lCsiConfig.mpVc[i].mBufAddr != 0)
        {
          CSI_VC_IDX_ADD(lCsiConfig.mIdKey, i);
        } // if VC used
      } // for all virtual channels
      
      // enable acording to members
      lCsiConfig.mPPErrIrqMask  = mPPErrIrqMask;  
      lCsiConfig.mPhyErrIrqMask = mPhyErrIrqMask; 
      lCsiConfig.mFrameIrqMask  = mFrameIrqMask;

      if(lRet == LIB_SUCCESS)
      {
        if(CSI_Config(&lCsiConfig) != CSI_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        } // if failed to Config CSI regs
        else
        {
          mState  = SETUP;
        } // else from if failed to config CSI regs
      } // if all ok so far
      
      // TODO: setup IRQ handler
      //CSI_IrqHandlerSet(mDesc.mIdx, CSI_IrqHandler);
    } // if camera is supported
  } // else from if not reserved yet

  return lRet;
} // sdi_MipiCsiIO::Setup()

//***************************************************************************

LIB_RESULT sdi_MipiCsiIO::Start(void)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if(mState != SETUP)
  {
    VDB_LOG_ERROR("Failed to start. Not setup yet.\n");
    lRet = LIB_FAILURE;
  } // if not setup yet
  else
  {
    bool lUnsupportedCam = false;
    // start the camera
    switch(mDesc.mCamType)
    {
      case CSI_SONY224:
      {
        if(SONY_CsiEnable((CSI_IDX)mDesc.mIdx) != CAM_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        } // if failed to start Sony
      } // SONY224
        break;
      case CSI_OV10640:
      {
        if(OV10640_CsiEnable((CSI_IDX)mDesc.mIdx) != CAM_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        } // if failed to start ov10640
      } // OV10640
        break;
      case CSI_MAX_SONY224: // fallthrough
      case CSI_MAX_OV10640: // fallthrough
      case CSI_MAX_OV10635: // fallthrough
	  case CSI_MAX9286_96705:
      {
        if(MAXIM_CsiEnable((CSI_IDX)mDesc.mIdx) != CAM_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        } // if failed to start Maxim
      } // case MAX_<cam_type>
        break;
      // *** Unsupported camera types ***
      case CSI_SONY290:     // fallthrough
      case CSI_SONY390:     // fallthrough
      case CSI_OV2775:      // fallthrough
      case CSI_OV9716:      // fallthrough
      case CSI_PAN34420:    // fallthrough
      case CSI_GENERIC:     // fallthrough
      case CSI_MAX_GENERIC: // fallthrough
      case CSI_MAX_OV9716:  
      {
        // all realated setup has to be done outside SDI
        VDB_LOG_WARNING("Unsupported camera type.\n");
        lUnsupportedCam = true;
      } // case <unsupported camera type>
      break;
      // *** Unknow camera types ***
      default:
      {
        VDB_LOG_ERROR("Unknown MIPICSI camera type.\n");
        lRet = LIB_FAILURE;
        lUnsupportedCam = true;
      }
        break;
    } // switch camera type

    if(lUnsupportedCam == false)
    {
      // enable CSI receiver
      CSI_IdKey_t lCsiKey;
      lCsiKey.mKey = 0;
      CSI_IDX_SET(lCsiKey, mDesc.mIdx);
      CSI_VC_IDX_ADD(lCsiKey, 0);
      if(CSI_Start(lCsiKey) != CSI_LIB_SUCCESS)
      {
        lRet = LIB_FAILURE;
      } // if failed to start CSI receiver
      else
      {
        mState = ON;
      }
    } // if all ok so far
    // from now on grabbing continuously in background
  } // else from if not setup yet

  return lRet;
} // sdi_FdmaIO::Start(void)

//***************************************************************************

LIB_RESULT sdi_MipiCsiIO::Stop(void)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  CSI_IdKey_t lCsiKey;

  if(mState == ON)
  {   
    lCsiKey.mKey = 0;
    CSI_IDX_SET(lCsiKey, mDesc.mIdx);
    CSI_VC_IDX_ADD(lCsiKey, 0);
    if(CSI_Stop(lCsiKey) != CSI_LIB_SUCCESS)
    {
      lRet = LIB_FAILURE;
    } // if failed to stop CSI

    // stop the camera
    switch(mDesc.mCamType)
    {
      case CSI_SONY224:
      {
        if(SONY_CsiDisable((CSI_IDX)mDesc.mIdx) != CAM_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        } // if failed to stop Sony
      } // SONY224
        break;
      case CSI_OV10640:
      {
        if(OV10640_CsiDisable((CSI_IDX)mDesc.mIdx) != CAM_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        } // if failed to stop Ov10640
      } // OV10640
        break;
      case CSI_MAX_SONY224: // fallthrough
      case CSI_MAX_OV10640: // fallthrough
      case CSI_MAX_OV10635: // fallthrough
	  case CSI_MAX9286_96705:
      {
        if(MAXIM_CsiDisable((CSI_IDX)mDesc.mIdx) != CAM_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        } // if failed to stop Maxim
      } // case MAX_OV10640
        break;
      // *** Unsupported camera types ***
      case CSI_SONY290:     // fallthrough
      case CSI_SONY390:     // fallthrough
      case CSI_OV2775:      // fallthrough
      case CSI_OV9716:      // fallthrough
      case CSI_PAN34420:    // fallthrough
      case CSI_GENERIC:     // fallthrough
      case CSI_MAX_GENERIC: // fallthrough
      case CSI_MAX_OV9716:  
      {
        // all realated setup has to be done outside SDI
        VDB_LOG_WARNING("Unsupported camera type.\n");
      } // case <unsupported camera type>
      break;
      // *** Unknow camera types ***
      default:
      {
        VDB_LOG_ERROR("Unknown MIPICSI camera type.\n");
        lRet = LIB_FAILURE;
      }
        break;
    } // switch camera type
    if(lRet == LIB_SUCCESS)
    {
      mState  = SETUP;
    } // if all ok so far
  } // if ON
  else
  {
    VDB_LOG_WARNING("Failed to stop. Not ON yet.\n");
  } // else from if not ON

  return lRet;
} // sdi_FdmaIO::Stop(void)

//****************************************************************************

LIB_RESULT sdi_MipiCsiIO::ParameterSet(SDI_IO_PARAM aParamId, void *apValue)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  // TODO: remove unused warning suppression macro
  UNUSED(aParamId);
  UNUSED(apValue);

  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return lRet;
} // sdi_MipiCsiIO::ParameterSet(SDI_IO_PARAM aParamId, void *apValue)

//****************************************************************************

LIB_RESULT sdi_MipiCsiIO::ParameterGet(SDI_IO_PARAM aParamId, void *apValue)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  // TODO: remove unused warning suppression macro
  UNUSED(aParamId);
  UNUSED(apValue);

  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return lRet;
} // sdi_MipiCsiIO::ParameterGet(SDI_IO_PARAM aParamId, void *apValue)

//****************************************************************************

SDI_MipiDescriptor sdi_MipiCsiIO::DescriptorGet()
{
  return mDesc;
} // sdi_MipiCsiIO::DescriptorGet()

//****************************************************************************

sdi_MipiCsiIO::~sdi_MipiCsiIO()
{
  if(mState != OFF)
  {
    Release();
  } // if all ok so far
} // sdi_MipiCsiIO::~sdi_MipiCsiIO()

//****************************************************************************

// *** sdi_ViuIO ***

//****************************************************************************

sdi_ViuIO::sdi_ViuIO():sdi_io(SEQ_OTHRIX_VIU0)
{
} // sdi_ViuIO::sdi_ViuIO()

//****************************************************************************

sdi_ViuIO::sdi_ViuIO(sdi_graph *apGraph, enum VIU_IDX aViuIdx):
sdi_io(SEQ_OTHRIX_VIU0, apGraph)
{

  mDesc   = mpGraph->ViuDescGet(aViuIdx);
  if((uint32_t)aViuIdx == VIU_IDX_1)
  {
    mTypeIndex = SEQ_OTHRIX_VIU1;
  } // Viu1 interface

} // sdi_ViuIO::sdi_ViuIO()

//****************************************************************************

LIB_RESULT sdi_ViuIO::Reserve()
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if(mState != OFF)
  {
    VDB_LOG_WARNING("Failed to reserve. Reserved already before.\n");
  } // if not OFF
  else
  {
#ifndef __STANDALONE__
    if(VIU_Open(mDesc.mIdx) != VIU_LIB_SUCCESS)
    {
      lRet = LIB_FAILURE;
    } // if failed to open VIU

    // update descriptor to get SRAM buffer address
    mDesc   = mpGraph->ViuDescGet(mDesc.mIdx);

    switch(mDesc.mCamType)
    {
      case VIU_OV10640:
      {
        if(OV10640_Open(mDesc.mIdx) != CAM_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        } // if failed to open ov10640
      } // OV10640
        break;
      case VIU_OV10635:
      {
        if(OV10635_Open(mDesc.mIdx) != CAM_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        } // if failed to open ov10635
      } // OV10635
        break;
      default:
      {
        VDB_LOG_ERROR("Unknown VIU camera type.\n");
        lRet = LIB_FAILURE;
      }
        break;
    } // switch camera type
#endif // #ifndef __STANDALONE__
  } // else from if not OFF

  if(lRet == LIB_SUCCESS)
  {
    mState  = RESERVED;
  } // if all ok so far

  return lRet;
} // sdi_ViuIO::Reserve()

//****************************************************************************

LIB_RESULT sdi_ViuIO::Release()
{
  LIB_RESULT lRet = LIB_SUCCESS;

  // stop if still running
  if(mState == ON)
  {
    lRet = Stop();
  } // if still ON

#ifndef __STANDALONE__
  switch(mDesc.mCamType)
  {
    case VIU_OV10640:
    {
      if(OV10640_Close(mDesc.mIdx) != CAM_LIB_SUCCESS)
      {
        lRet = LIB_FAILURE;
      } // if failed to close ov10640
    } // OV10640
      break;
    case VIU_OV10635:
    {
      if(OV10635_Close(mDesc.mIdx) != CAM_LIB_SUCCESS)
      {
        lRet = LIB_FAILURE;
      } // if failed to close ov10635
    } // OV10635
      break;
    default:
    {
      VDB_LOG_ERROR("Unknown VIU camera type.\n");
      lRet = LIB_FAILURE;
    }
      break;
  } // switch camera type

  if(VIU_Close(mDesc.mIdx) != VIU_LIB_SUCCESS)
  {
    lRet = LIB_FAILURE;
  } // if failed to close VIU
#endif // #ifndef __STANDALONE__

  mState  = OFF;

  return lRet;
} // sdi_ViuIO::Release()
//****************************************************************************

LIB_RESULT sdi_ViuIO::Setup()
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if((mState == OFF) || (mState == ON))
  {
    VDB_LOG_ERROR("Failed to setup. Current state is: [%s].\n", 
                  (mState == OFF)?"OFF":"ON");
    lRet = LIB_FAILURE;
  } // if not reserved yet
  else
  {
    // get viu descriptor from graph
    mDesc = mpGraph->ViuDescGet(mDesc.mIdx); // try idx 0 first

    if(mDesc.mIdx >= VIU_IDX_INVALID)
    {
      VDB_LOG_NOTE("VIU_%d not used.\n");
    } // if invalid VIU IDX
    else
    {
#ifndef __STANDALONE__
      DMA_CONFIG lViuDmaConfig;
      VIU_DATA_INTERFACE lViuDataInterface;
      VIU_INPUT_FORMAT lViuInputFormat;

      // reset the receiver
      if(VIU_DmaStop(mDesc.mIdx) != VIU_LIB_SUCCESS)
      {
        lRet = LIB_FAILURE;
      }
      else if(VIU_SwReset(mDesc.mIdx) != VIU_LIB_SUCCESS)
      {
        lRet = LIB_FAILURE;
      } // if SW reset failed
      else
      {
        switch(mDesc.mCamType)
        {
          case VIU_OV10640:
          {
            //setup VIU receiver 12bit raw mode
            lViuDmaConfig.buff_addr   = mDesc.mBufAddr;
            lViuDmaConfig.tx_size     = mDesc.mLineCnt;
            lViuDmaConfig.nmb_lines   = 728; // number of lines per frame
            lViuDmaConfig.nmb_pixells = 1288;//mDesc.mLineBytes; //TODO: correct value
            lViuDmaConfig.frame_inc   = 0;//mDesc.mLinePadding;
            lViuDmaConfig.alpha_val   = 0xff;

            lViuDataInterface.pclk_pol    = 0;
            lViuDataInterface.vsync_pol   = 0;
            lViuDataInterface.hsync_pol   = 1;
            lViuDataInterface.endianness  = 1;

            lViuInputFormat.mode              = PARALLEL_MODE;
            lViuInputFormat.width             = WIDE_12_BITS; // 12 bits
            lViuInputFormat.clocks_per_pixell = CPP_1_PP;
          } // OV10640
            break;
          case VIU_OV10635:
          {
            //setup VIU receiver YUV 420
            lViuDmaConfig.buff_addr   = mDesc.mBufAddr;
            lViuDmaConfig.tx_size     = mDesc.mLineCnt;
            lViuDmaConfig.nmb_lines   = 800; // number of lines per frame
            lViuDmaConfig.nmb_pixells = 1280;//mDesc.mLineBytes; //TODO: correct value
            lViuDmaConfig.frame_inc   = 0;//mDesc.mLinePadding;
            lViuDmaConfig.alpha_val   = 0xff;

            lViuDataInterface.pclk_pol    = 0;
            lViuDataInterface.vsync_pol   = 0;
            lViuDataInterface.hsync_pol   = 1;
            lViuDataInterface.endianness  = 1;

            lViuInputFormat.mode              = PARALLEL_MODE;
            lViuInputFormat.width             = WIDE_8_BITS; // 8 bits
            lViuInputFormat.clocks_per_pixell = CPP_2_PP;
          } // OV10635
            break;
          default:
          {
            VDB_LOG_ERROR("Unknown VIU camera type.\n");
            lRet = LIB_FAILURE;
          }
            break;
        } // switch camera type

        if(VIU_Config(mDesc.mIdx,
                      &lViuDmaConfig,
                      &lViuDataInterface,
                      &lViuInputFormat,
                      0x0) == VIU_LIB_SUCCESS)
        {
          // configure camera registers
          switch(mDesc.mCamType)
          {
            case VIU_OV10640:
            {
              if(OV10640_RegConfig(mDesc.mIdx) != CAM_LIB_SUCCESS)
              {
                lRet = LIB_FAILURE;
              } // if ov10640 config failed
            } // OV10640
              break;
            case VIU_OV10635:
            {
              if(OV10635_RegConfig(mDesc.mIdx) != CAM_LIB_SUCCESS)
              {
                lRet = LIB_FAILURE;
              } // if ov10635 config failed
            } // OV10635
              break;
            default:
            {
              VDB_LOG_ERROR("Unknown VIU camera type.\n");
              lRet = LIB_FAILURE;
            }
              break;
          } // switch camera type
        } // if VIU config ok
        else
        {
          lRet = LIB_FAILURE;
        } // else from if VIU config ok
      } // else from if SW reset failed
#endif // #ifndef __STANDALONE__
      VDB_LOG_NOTE("VIU_%d used.\n", mDesc.mIdx);
    } // else from if invalid VIU index
  } // else from if not reserved yet

  if(lRet == LIB_SUCCESS)
  {
    mState  = SETUP;
  } // if all ok so far

  return lRet;
} // sdi_ViuIO::Setup()

//***************************************************************************

LIB_RESULT sdi_ViuIO::Start(void)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if(mState == SETUP)
  {
#ifndef __STANDALONE__
    // enable data
    switch(mDesc.mCamType)
    {
      case VIU_OV10640:
      {
        if(OV10640_DataEnable(mDesc.mIdx) != CAM_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        } // if ov10640 start failed
      } // OV10640
        break;
      case VIU_OV10635:
      {
        // nothing to be done
      } // OV10635
        break;
      default:
      {
        VDB_LOG_ERROR("Unknown VIU camera type.\n");
        lRet = LIB_FAILURE;
      }
        break;
    } // switch camera type

    if(VIU_Start(mDesc.mIdx) != VIU_LIB_SUCCESS)
    {
      lRet = LIB_FAILURE;
    }
    else if(VIU_DmaStart(mDesc.mIdx) != VIU_LIB_SUCCESS)
    {
      lRet = LIB_FAILURE;
    }
#endif //#ifndef __STANDALONE__
  } // if setup already
  else
  {
    VDB_LOG_ERROR("Failed to start VIU %u. Not setup yet.\n",mDesc.mIdx);
    lRet = LIB_FAILURE;
  } // else from if setup already

  if(lRet == LIB_SUCCESS)
  {
    mState  = ON;
  } // if all ok so far

  return lRet;
} // sdi_ViuIO::Start(void)

//***************************************************************************

LIB_RESULT sdi_ViuIO::Stop(void)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if(mState == ON)
  {
#ifndef __STANDALONE__
    switch(mDesc.mCamType)
    {
      case VIU_OV10640:
      {
        if(OV10640_DataDisable(mDesc.mIdx) != CAM_LIB_SUCCESS)
        {
          lRet = LIB_FAILURE;
        }
      } // OV10640
        break;
      case VIU_OV10635:
      {
        // nothing to be done
      } // OV10635
        break;
      default:
      {
        VDB_LOG_ERROR("Unknown VIU camera type.\n");
        lRet = LIB_FAILURE;
      }
        break;
    } // switch camera type

    if(VIU_DmaStop(mDesc.mIdx) != VIU_LIB_SUCCESS)
    {
      lRet = LIB_FAILURE;
    }
    if(VIU_Stop(mDesc.mIdx) != VIU_LIB_SUCCESS)
    {
      lRet = LIB_FAILURE;
    }
#endif // #ifndef __STANDALONE__
    mState  = SETUP;
  } // if ON
  else
  {
    VDB_LOG_WARNING("Failed to stop VIU %u. Not ON yet.\n",mDesc.mIdx);
  } // else from if ON

  return lRet;
} // sdi_ViuIO::Stop(void)

//****************************************************************************

LIB_RESULT sdi_ViuIO::ParameterSet(SDI_IO_PARAM aParamId, void *apValue)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  // TODO: remove unused warning suppression macro
  UNUSED(aParamId);
  UNUSED(apValue);

  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return lRet;
} // sdi_ViuIO::ParameterSet(SDI_IO_PARAM aParamId, void *apValue)

//****************************************************************************

LIB_RESULT sdi_ViuIO::ParameterGet(SDI_IO_PARAM aParamId, void *apValue)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  // TODO: remove unused warning suppression macro
  UNUSED(aParamId);
  UNUSED(apValue);

  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return lRet;
} // sdi_ViuIO::ParameterGet(SDI_IO_PARAM aParamId, void *apValue)

//****************************************************************************

SDI_ViuDescriptor sdi_ViuIO::DescriptorGet()
{
  return mDesc;
} // sdi_ViuIO::DescriptorGet()

//****************************************************************************

sdi_ViuIO::~sdi_ViuIO()
{
  if(mState != OFF)
  {
    (void)Release();
  } // if all ok so far
} // sdi_ViuIO::~sdi_ViuIO()

//****************************************************************************

SDI_ImageDescriptor::SDI_ImageDescriptor()
{
  mWidth = 0;
  mHeight = 0;
  mWidthStep = 0;
  mBpp = 0;
  mCvImageFormat = 0;
  mPixelFormat = SDI_IMAGE_FORMAT_NONE;
} // SDI_ImageDescriptor()

//****************************************************************************

// copy constructor
SDI_ImageDescriptor::SDI_ImageDescriptor(const SDI_ImageDescriptor &src) :
    mWidth(src.mWidth),
    mHeight(src.mHeight),
    mWidthStep(src.mWidthStep),
    mBpp(src.mBpp),
    mPixelFormat(src.mPixelFormat),
    mCvImageFormat(src.mCvImageFormat)
{
} 

//****************************************************************************

// parametric constructor
SDI_ImageDescriptor::SDI_ImageDescriptor(uint32_t aWidth,
                                         uint32_t aHeight,
                                         SDI_IMAGE_FORMAT aPixelFormat)
{
  mWidth  = aWidth;
  mHeight = aHeight;
  mPixelFormat = aPixelFormat;
  // check pixel format to determine bits per pixel
  switch (mPixelFormat)
  {
    case RGB888:
    case BGR888:
    case YUV444Stream_VUY:
    case YUV444Stream_YUV:
    {
      mBpp = 24;
      mCvImageFormat = VSDK_CV_8UC3;
    }
      break;
    case RGB565:
    case RGB565_8BitAlphaPlane:
    case BGR565:
    case BGR565_8BitAlphaPlane:
    case RGBA4444:
    case ABGR4444:
    case YUV422Stream_UYVY:
    case YUV422Stream_YUYV:
    case YUV422Planar:
    {
      mBpp = 16;
      mCvImageFormat = VSDK_CV_8UC2;
    }
      break;
      // todo: correct bpp representation, don't know right now
    case RGB666:
    case BGR666:
    
    case YUV420Planar:
    case YUV420Planar_Y1_UV2:
    case YUV420Planar_Y1_VU2:
    case YUV420Planar_Y1_U2_V3:
    {
      mBpp = 0;
      mCvImageFormat = VSDK_CV_8UC1;
    }
      break;
    case GS8:
    {
      mBpp = 8;
      mCvImageFormat = VSDK_CV_8UC1;
    }
      break;
    case RGBA8888:
    case ABGR8888:
    {
      mBpp = 32;
      mCvImageFormat = VSDK_CV_8UC4;
    }
      break;
    case SDI_IMAGE_FORMAT_NONE:
    case SDI_IMAGE_FORMAT_INVALID:
    {
      mBpp = 0;
      mCvImageFormat = 0;
    }
      break;
    default:
    {
      mPixelFormat = SDI_IMAGE_FORMAT_INVALID;
      mBpp = 0;
      mCvImageFormat = 0;
    }
      break;
  }
  mWidthStep = mWidth * (mBpp >> 3);
} 

//****************************************************************************

SDI_ImageDescriptor& SDI_ImageDescriptor::operator = (const SDI_ImageDescriptor& arcSrc)
{
  mWidth = arcSrc.mWidth;
  mHeight = arcSrc.mHeight;
  mWidthStep = arcSrc.mWidthStep;
  mBpp = arcSrc.mBpp;
  mPixelFormat = arcSrc.mPixelFormat;
  mCvImageFormat = arcSrc.mCvImageFormat;

  return *this;
} 

//****************************************************************************

SDI_ImageDescriptor::~SDI_ImageDescriptor()
{
  mWidth = 0;
  mHeight = 0;
  mWidthStep = 0;
  mBpp = 0;
  mCvImageFormat = 0;
  mPixelFormat = SDI_IMAGE_FORMAT_INVALID;
} 

//****************************************************************************
/*EOF*/