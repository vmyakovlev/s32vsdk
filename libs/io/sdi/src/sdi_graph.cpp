/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor;
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
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/
/**
* \file     sdi_graph.cpp
* \brief    Defines Sequencer graph related functionality. Depends 
*           on GDT generated metadata.
* \author   Tomas Babinec
* \version  0.1
* \date     21-October-2014
****************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "s32vs234.h"
#include "sdi_graph.hpp"
#include "isp_hw.h"
#include "isp_fdma.h"
#include "seq_graph.h"
#include "seq.h"
#include "isp_seq.h"

#include "oal.h"

#ifndef __STANDALONE__
#include <unistd.h>
#include <sys/mman.h>
#endif // #ifndef __STANDALONE__

// logging functionality
#define VDB_LOG_NOTE_OFF

#include "vdb_log.h"

/*****************************************************************************
* Macros
*****************************************************************************/

/*****************************************************************************
* global data
*****************************************************************************/

//****************************************************************************
// consts
//****************************************************************************

#define TDT_CNT 16
#define PRAM_M02HOST_OFFSET (PRAM_BASE - SEQ_PRAM_BASE)
#define CRAM_M02HOST_OFFSET (CRAM_BASE - CRAM_BASE_M0)
#define DDR_CHAR_VAL        (TDT_CNT + 1)

//****************************************************************************
// macros
//****************************************************************************

#define CRAM_HOST2M0(_HostAddr) ((uint32_t)(_HostAddr - CRAM_M02HOST_OFFSET)) 
#define CRAM_M02HOST(_M0Addr)   ((uint32_t)(_M0Addr + CRAM_M02HOST_OFFSET))

#define PRAM_HOST2M0(_HostAddr) ((uint32_t)(_HostAddr - PRAM_M02HOST_OFFSET)) 
#define PRAM_M02HOST(_ProAddr)  ((uint32_t)(_ProAddr + PRAM_M02HOST_OFFSET))


#define ALIGN2(_x) (((_x)/2 + (((_x)%2)?1:0)) * 2)


//****************************************************************************
// local variables
//****************************************************************************

//****************************************************************************
// local functions declarations
//****************************************************************************

//****************************************************************************
// methods implementation
//****************************************************************************

// *** sdi_graph implementation ***

sdi_graph::sdi_graph():
  mpGraph(NULL),
  mpMeta(NULL),
  mHeadCnt(0),
  mpPackedGraph(NULL),  
  mpPackedObjects(NULL),
  mpFetchedGraph(NULL),
  mppFetchedBufferList(NULL),
  mppSramBufOalHandles(NULL),
  mpFdma(NULL)
{  
  memset(mpFdmaPackedBck, 0, sizeof(SEQ_FdmaCfg_t));
  
  // reset all IO indexes
  for(uint32_t i = 0; i < SEQ_OTHRIX_LAST; i++)
  {
    mIoNodeIdxArr[i]  = -1;
    mpIoNodePtrArr[i] = NULL;
  } // for all TC pointers
  
  // reset all TC pointers
  for(uint32_t i = 0; i < FDMA_CHANNELS; i++)
  {
    mTcs[i] = NULL;
  } // for all TC pointers
} // sdi_graph::sdi_graph()

//****************************************************************************

sdi_graph::sdi_graph(SEQ_Head_Ptr_t apGraph[], GraphMetadata_t *apMeta):
    mpGraph(apGraph),
    mpMeta(apMeta),
    mHeadCnt(0),
    mpPackedGraph(NULL),
    mpPackedObjects(NULL),
    mpFetchedGraph(NULL),
    mppFetchedBufferList(NULL),
    mppSramBufOalHandles(NULL),
    mpFdma(NULL)
{
  LIB_RESULT lRes = LIB_SUCCESS;
  // get number of heads in the graph
  //mHeadCnt = sizeof(apGraph) / sizeof(SEQ_Head_t*);
  mHeadCnt = 0;
  
  memset(mpFdmaPackedBck, 0, sizeof(SEQ_FdmaCfg_t));
  
  // reset MIPICSI node pointers
  for(uint32_t i = 0; i < CSI_IF_CNT; i++)
  {
    mppCsi[i] = NULL;
  }
  
  // reset VIU node pointers
  for(uint32_t i = 0; i < VIU_IF_CNT; i++)
  {
    mppViu[i] = NULL;
  }
  
  // reset JPEG node pointer
  mpJpegDec = NULL;
  
  // reset H264 decoder node pointer
  mpH264Dec = NULL;
  
  // reset H264 encoder node pointer
  mpH264Enc = NULL;
    
  // reset all IO indexes
  for(uint32_t i = 0; i < SEQ_OTHRIX_LAST; i++)
  {
    mIoNodeIdxArr[i]  = -1;
    mpIoNodePtrArr[i] = NULL;
  } // for all TC pointers
  
  // reset all TC pointers
  for(uint32_t i = 0; i < FDMA_CHANNELS; i++)
  {
    mTcs[i] = NULL;
  } // for all TC pointers

  // pack the graph
  if(apMeta->mObjCnt > 0)
  {   
    if(Pack() != LIB_SUCCESS)
    {
      lRes = LIB_FAILURE;
    }
    else if(Scan() != LIB_SUCCESS) // only to get FDMA node
    {
      lRes = LIB_FAILURE;
    }
    else if(FdmaTCsPrepare() != LIB_SUCCESS)
    {
      lRes = LIB_FAILURE;
    } // if FdmaTCsPrepare failed
    
    if(lRes != LIB_SUCCESS)
    {
      mpGraph = apGraph;
      mpMeta  = apMeta;
    } // if sth went wrong
  } // if there are some objects
  else
  {
    VDB_LOG_WARNING("No objects in the graph.\n");
  } // else from if there are some objects
} // sdi_graph::sdi_graph(SEQ_Head_t *aGraph[])

//****************************************************************************

sdi_graph::~sdi_graph()
{
  VDB_LOG_NOTE("Graph destructor.\n");
  
  FdmaTCsRemove();
    
  SramBuffersFree();
  
  if(mpPackedGraph)
  {
    free(mpPackedGraph);
    mpPackedGraph = NULL;
  }
  
  if(mpFetchedGraph)
  {
    free(mpFetchedGraph);
    mpFetchedGraph = NULL;
  }
  
  if(mppFetchedBufferList)
  {
    free(mppFetchedBufferList);
    mppFetchedBufferList = NULL;
  }
  
  if(mppSramBufOalHandles)
  {
    free(mppSramBufOalHandles);
    mppSramBufOalHandles = NULL;
  }
} // sdi_graph::~sdi_graph()

//****************************************************************************

inline void *sdi_graph::Off2Packed(uint32_t aOffset)
{
  return (mpPackedGraph != NULL)?(void*)((uint8_t*)mpPackedGraph + aOffset):NULL;
} // void *sdi_graph::Off2Packed(uint32_t aOffset)

//****************************************************************************

LIB_RESULT sdi_graph::FdmaTCsPrepare()
{
  LIB_RESULT lRes = LIB_SUCCESS;
  
  // if FDMA
  if(mpFdma != NULL)
  {
    // get pointer to FDMA config backup place
    SEQ_FdmaCfg_t *lpFdmaPackedBck = (SEQ_FdmaCfg_t *) mpFdmaPackedBck;
    
    // back up its contents (original TC indexes will be changed based on TC allocation)
    memcpy(lpFdmaPackedBck,mpFdma, sizeof(SEQ_FdmaCfg_t));
    
    // Prepare TCs for all channels used in the graph    
    for(int32_t i = 0; i < FDMA_CHANNELS; i++)
    {
      //is this TC used as input or output?
      if((mpFdma->mOutBuf[i].mpOutBuf != 0) && (mpFdma->mInBuf[i].mpInBuf != 0))
      {
        // allocate TC structure
        mTcs[i] = (FDMA_Tc_t*) new(FDMA_Tc_t);
        memset(mTcs[i],0, sizeof(FDMA_Tc_t));
        if(mTcs[i] != NULL)
        {
          // get both buffer structures
          SEQ_Buf_t* lpInBuf   = \
            (SEQ_Buf_t*)(Off2Packed((uint32_t)mpFdma->mInBuf[i].mpInBuf));
          SEQ_Buf_t* lpOutBuf  = \
            (SEQ_Buf_t*)(Off2Packed((uint32_t)mpFdma->mOutBuf[i].mpOutBuf));
          SEQ_Buf_t *lpSramBuf = NULL, *lpDdrBuf = NULL;
          
          if((lpInBuf  == NULL) || 
             (lpOutBuf == NULL) || 
             (lpInBuf->mpBuf == lpOutBuf->mpBuf))
          {
            VDB_LOG_ERROR("Bad FDMA I/O buffers.\n");
            lRes = LIB_FAILURE;
          } // if bad IO buffers
          else
          {
            if(lpInBuf->mpBuf == 0)
            {
              lpSramBuf = lpInBuf;
              lpDdrBuf  = lpOutBuf;
              mTcs[i]->mTd.mDir = 0;			// SRAM->DDR
              // check if not free running buffer (no start line index reset)
              SEQ_Head_t *lpProd = 
                (SEQ_Head_t*)(Off2Packed((uint32_t)lpInBuf->mpProd));
              if(lpProd->mcNodeType  == SEQ_NODE_OTHER)
              {  
                 if(lpProd->mcEngIndex == SEQ_OTHRIX_MIPICSI0)
                 {
                   mTcs[i]->mTdm.mSramLpF =  mppCsi[0]->mcNumInputLines;
                 } // if csi0
                 else if(lpProd->mcEngIndex == SEQ_OTHRIX_MIPICSI1)
                 {
                   mTcs[i]->mTdm.mSramLpF = mppCsi[1]->mcNumInputLines;
                 } // if csi1  
              } // if free running buffer
              else
              {
                mTcs[i]->mTdm.mSramLpF = 0; 
              } // else from if free running buffer
              
              // copy SRAM line offset if any
              mTcs[i]->mTdm.mSramLineOffs = mpFdma->mInBuf[i].mLineOffs;
            } // if SRAM -> DDR
            else
            {
              lpSramBuf = lpOutBuf;
              lpDdrBuf  = lpInBuf;
              mTcs[i]->mTd.mDir      = 1; // DDR->SRAM
              mTcs[i]->mTdm.mSramLpF = 0; // start line index reset per frame
              
              // SRAM line offset forced to 0 here
              mTcs[i]->mTdm.mSramLineOffs = 0;
            } // else from if SRAM -> DDR
            
            // general TD setup
            mTcs[i]->mTd.mTransRecWrBack = 1;		// enable TD update
            if(lpSramBuf->mcLineBytes <= lpDdrBuf->mcLineStride)
            { 
              mTcs[i]->mTd.mLineSize = lpSramBuf->mcLineBytes;
            }
            else
            {
              mTcs[i]->mTd.mLineSize = lpDdrBuf->mcLineStride;
            }
            // SRAM side TD setup
            mTcs[i]->mTd.mSramImgStartAddr = 0; // not yet allocated
            mTcs[i]->mTd.mSramLineStride   = lpSramBuf->mcLineStride/8;
            mTcs[i]->mTd.mSramLineCnt      = lpSramBuf->mcLineMax;
            mTcs[i]->mTd.mSramCurrLine     = mTcs[i]->mTdm.mSramLineOffs %
                                             mTcs[i]->mTd.mSramLineCnt;
            // DDR side TD setup
            mTcs[i]->mTd.mDdrImgStartAddr  = 0; // not yet allocated
            mTcs[i]->mTd.mDdrLineStride    = lpDdrBuf->mcLineStride/32;
            mTcs[i]->mTd.mDdrLineCnt       = lpDdrBuf->mcLineMax;
            mTcs[i]->mTd.mDdrCurrLine      = 0; 
          } // else from if bad IO buffers
        } // if TC allocation OK
        else
        {
          VDB_LOG_ERROR("Tc at index %u allocation failed\n", (unsigned) i);
          // set to NULL
          mTcs[i] = NULL;
          lRes    = LIB_FAILURE;
          break;
        } // else from if TC allocation OK
      } // if i-th channel used
    } // for all possible FDMA channels
  } // if FDMA node exists
  
  return lRes;
} // FdmaTCsPrepare()

//****************************************************************************

LIB_RESULT sdi_graph::FdmaTCsReserve()
{
  LIB_RESULT lRes = LIB_SUCCESS;
  
  // if FDMA
  if(mpFdma != NULL)
  {           
    // reserve all prepared TCs    
    for(int32_t i = 0; i < FDMA_CHANNELS; i++)
    {
      //is TC prepared
      if(mTcs[i] != NULL)
      {
        mTcs[i]->mTdm.mTdIdx        = i; 
        mTcs[i]->mTdm.mOwnerProcId  = 333;
        mTcs[i]->mTdm.mReserved     = 1;
        
        // get pointer to the sram buffer
        SEQ_Buf_t* lpSramBuf  = NULL;
        if(mTcs[i]->mTd.mDir == 1)
        {
          lpSramBuf = (SEQ_Buf_t*)(Off2Packed((uint32_t)mpFdma->mOutBuf[i].mpOutBuf));
        } // if DDR->SRAM
        else
        {
          lpSramBuf = (SEQ_Buf_t*)(Off2Packed((uint32_t)mpFdma->mInBuf[i].mpInBuf));
        } // else from if DDR->SRAM
        
        // set SRAM address to TC
        mTcs[i]->mTd.mSramImgStartAddr += (uint32_t)(lpSramBuf->mpBuf);
      } // if TC prepared
    } // for all possible FDMA channels
  } // if FDMA node exists
  
  return lRes;
} // FdmaTCsReserve()

//****************************************************************************

void sdi_graph::FdmaTCsRelease()
{
  // reset all TC pointers
  for(uint32_t i = 0; i < FDMA_CHANNELS; i++)
  {
    if((mTcs[i] != NULL) && ((mTcs[i]->mTdm.mReserved) > 0))
    {
      VDB_LOG_NOTE("TC number %u released OK\n", (unsigned) i);
      mTcs[i]->mTdm.mReserved = 0;
    } // if TC allocated
  } // for all TC pointers
} // sdi_graph::FdmaTCsRelease()

//****************************************************************************

void sdi_graph::FdmaTCsRemove()
{
  FdmaTCsRelease();
  // release & delete all TC pointers
  for(uint32_t i = 0; i < FDMA_CHANNELS; i++)
  {
    if(mTcs[i] != NULL)
    {
      // delete TC
      delete(mTcs[i]);
      mTcs[i] = NULL;
    } // if TC allocated
  } // for all TC pointers
} // sdi_graph::FdmaTCsRemove()

//****************************************************************************

LIB_RESULT sdi_graph::SramBuffersAllocate()
{
  LIB_RESULT lRet = LIB_SUCCESS;
  int32_t    lRes = 0;
  
  // *** allocate array of OAL handles ***
  if(mppSramBufOalHandles != NULL)
  {
    free(mppSramBufOalHandles);
    mppSramBufOalHandles = NULL;
  } // if handles allocated already
  
  mppSramBufOalHandles = (void**) malloc(sizeof(void*) * mpMeta->mBfrCnt);
  if(mppSramBufOalHandles == NULL)
  {
    VDB_LOG_ERROR("Failed to allocated memory for Oal hanles.\n");
    lRet = LIB_FAILURE;
  } // if malloc failed
  else
  {  
    // *** allocate all sram buffers ***
    for(uint32_t i = 0; i < mpMeta->mBfrCnt; i++)
    {
      SEQ_Buf_t* lpCurrBuff = \
                      (SEQ_Buf_t*)Off2Packed((uint32_t)mpMeta->mpBufferList[i]);
      
      if(lpCurrBuff->mpBuf != 0)
      {
        continue;
      } // if DDR buffer - no allocation
      
      // compute size required
      uint32_t lSize = lpCurrBuff->mcLineMax * lpCurrBuff->mcLineStride;

      // allocate SRAM 
      mppSramBufOalHandles[i] = OAL_MemoryAllocFlag(lSize,                            
                                                    OAL_ALLOC_SRAM_MULTI);

      if(mppSramBufOalHandles[i] == NULL)
      {
        VDB_LOG_NOTE("Allocation from MULTI-banked region failed. Retrying from Single-banked region.\n");
        // attempt to allocate from Single-banked region
        mppSramBufOalHandles[i] = OAL_MemoryAllocFlag( lSize,
                                                        OAL_ALLOC_SRAM_SINGLE);
        if(mppSramBufOalHandles[i] == NULL)
        {
          VDB_LOG_ERROR("Failed to allocate SRAM buffer.\n");
          lRet = LIB_FAILURE;
          break;
        } // if also the single banked allocation failed
      } // if alloc failed
      
      // get physical address
      lRes = (uint32_t)(uint64_t)OAL_MemoryReturnAddress(
                                    mppSramBufOalHandles[i], 
                                    ACCESS_PHY);
      if(lRes == 0)
      {
        VDB_LOG_ERROR("Failed to get physical address of SRAM buffer.\n");
        lRet = LIB_FAILURE;
        break;
      } // if wrong physical address
      // set addr to buffer
      lpCurrBuff->mpBuf = (SEQ_Mem_Ptr_t)( lRes);
    } // for all buffers
  } // else from if malloc failed

  return lRet;
} // sdi_graph::SramBuffersAllocate()

//****************************************************************************

void sdi_graph::SramBuffersFree()
{
  if(mppSramBufOalHandles)
  {
    // free all sram buffers
    for(uint32_t i = 0; i < mpMeta->mBfrCnt; i++)
    {
      SEQ_Buf_t* lpCurrBuff = \
                    (SEQ_Buf_t*)Off2Packed((uint32_t)mpMeta->mpBufferList[i]);
      
      // free SRAM 
      if(mppSramBufOalHandles[i])
      {        
        OAL_MemoryFree(mppSramBufOalHandles[i]);
        mppSramBufOalHandles[i] = NULL;
            
        // set null to buffer
        lpCurrBuff->mpBuf = (SEQ_Mem_Ptr_t)0;
      } // if buffer not NULL
    } // while iterator not at the end 
    
    free(mppSramBufOalHandles);
    mppSramBufOalHandles = NULL;
  } // if something allocated  
} // sdi_graph::SramBuffersFree()

//**************************************************************************** 

LIB_RESULT sdi_graph::Finalize()
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
  // allocate all sram buffers
  if(SramBuffersAllocate() != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to allocate SRAM buffers.\n");
    lRet = LIB_FAILURE;
  } // if SRAM buffs allocation failed
  else
  {
    // reserve required TCs
    if(FdmaTCsReserve() != LIB_SUCCESS)
    {
      // allocate TC if any used
      VDB_LOG_ERROR("Failed to allocate required Fdma TCs.\n");
      // free SRAM buffers
      lRet = LIB_FAILURE;
    } // if failed to allocate FDMA TCs
  } // else from if SRAM buffs allocation failed
  
  return lRet;
} //sdi_graph::Finalize()

//**************************************************************************** 

void sdi_graph::Free()
{
  FdmaTCsRemove();
  SramBuffersFree();
} // sdi_graph::Free()

//****************************************************************************

LIB_RESULT sdi_graph::SramBufGet(SEQ_Buf_t* apSramBuf, uint32_t &arSramAddr)
{
  LIB_RESULT lRetVal = LIB_SUCCESS;
  SEQ_Buf_t* lpBuf = NULL;
  
  for(uint32_t i = 0; i < mpMeta->mObjCnt; i++)
  {
    if(apSramBuf == mpMeta->mpObjectList[i].mcpAddr)
    {
      lpBuf = 
        (SEQ_Buf_t*) Off2Packed((uint32_t)mpMeta->mpObjectList[i].mcOffset);
      break;
    } // if buffer found
  } // for all objects
  
  if(lpBuf == NULL)
  {
    VDB_LOG_ERROR("Failed to find Buffer pointer in address map.\n");
    lRetVal = LIB_FAILURE;
  } // if buffer not found
  else
  {
    arSramAddr = (uint32_t)(lpBuf->mpBuf);
  } // else from if buffer not found
  
  return lRetVal;
} // SramBufGet()

//****************************************************************************

LIB_RESULT sdi_graph::SramBufGet(const char *acpName, uint32_t &arSramAddr)
{
  LIB_RESULT lRetVal = LIB_SUCCESS;
  SEQ_Buf_t* lpBuf  = NULL;
  
  if(acpName != NULL)
  {
    int32_t lBufferIdx = BufferName2Idx(acpName);
    if(lBufferIdx >= 0)
    {
      lpBuf = (SEQ_Buf_t*) Off2Packed((uint32_t)mpMeta->mpBufferList[lBufferIdx]);
            
      if(lpBuf == NULL)
      {
        VDB_LOG_ERROR("Failed to find Buffer pointer in address map.\n");
        lRetVal = LIB_FAILURE;
      } // if buffer not found
      else
      {
        arSramAddr = (uint32_t)(lpBuf->mpBuf);
      } // else from if buffer not found
    } // if name found
  } // if name not NULL
  
  return lRetVal;
} // SramBufGet()

//****************************************************************************

LIB_RESULT sdi_graph::SramBufGet(const char *acpName, SEQ_Buf_t &arSramBuf)
{
  LIB_RESULT lRetVal = LIB_SUCCESS;
  SEQ_Buf_t* lpBuf = NULL;
  
  if(acpName != NULL)
  {
    int32_t lBufferIdx = BufferName2Idx(acpName);
    memset(&arSramBuf, 0x0, sizeof(SEQ_Buf_t));
    if(lBufferIdx >= 0)
    {
      lpBuf = 
        (SEQ_Buf_t*) Off2Packed((uint32_t)mpMeta->mpBufferList[lBufferIdx]);
            
      if(lpBuf == NULL)
      {
        VDB_LOG_ERROR("Failed to find Buffer pointer in address map.\n");
        lRetVal = LIB_FAILURE;
      } // if buffer not found
      else
      {
        memcpy(&arSramBuf, lpBuf, sizeof(SEQ_Buf_t));
      } // else from if buffer not found
    } // if name found
  } // if name not NULL
  
  return lRetVal;
} // SramBufGet()

//****************************************************************************

LIB_RESULT sdi_graph::FdmaTcGet(FDMA_Tc_t& arTc, uint32_t aIdx)
{
  LIB_RESULT aRetVal = LIB_SUCCESS;
  if(mTcs[aIdx] != NULL)
  {
    if(mTcs[aIdx]->mTdm.mReserved)
    {
      // copy TC content
      arTc = *(mTcs[aIdx]);
    } // if reserved
    else
    {
      VDB_LOG_ERROR("Tc at index %u not reserved.\n", (unsigned) aIdx);
      aRetVal = LIB_FAILURE;
    } // else from if reserved      
  } // if TC exists
  else 
  {
    VDB_LOG_ERROR("Channel %u not used in the graph.\n", (unsigned) aIdx);
    aRetVal = LIB_FAILURE;
  } // else form if TC exists  
  
  return aRetVal;
} // FdmaTcGet()

//****************************************************************************

LIB_RESULT sdi_graph::FdmaTcSet(FDMA_Tc_t& arTc, uint32_t aIdx)
{
  LIB_RESULT aRetVal = LIB_SUCCESS;
  if(mTcs[aIdx] != NULL)
  {
    // remember TC content
    *(mTcs[aIdx]) = arTc;
  } // if TC exists
  else 
  {
    VDB_LOG_ERROR("Channel %u not used in the graph.\n", (unsigned) aIdx);
    aRetVal = LIB_FAILURE;
  } // else form if TC exists  
  
  return aRetVal;
} // FdmaTcSet()

//****************************************************************************

FDMA_Tc_t** sdi_graph::FdmaTcsGet(void)
{
  return mTcs;
} // FdmaTcsGet(void)

//****************************************************************************

LIB_RESULT sdi_graph::FdmaTcsApply()
{
  LIB_RESULT aRetVal = LIB_SUCCESS;
  // reserve all prepared TCs    
  for(int32_t i = 0; i < FDMA_CHANNELS; i++)
  {
    if(mTcs[i] != NULL)
    {
      // send to fdma driver
      if(mTcs[i]->mTdm.mReserved)
      {
        if( FdmaTcIspConfig(mTcs[i]) != FDMA_LIB_SUCCESS)
        {
          VDB_LOG_ERROR("TcConfig failed.\n");
          aRetVal = LIB_FAILURE;
        } // if ioctl returned failure
      } // if TC reserved allready
      else
      {
        VDB_LOG_ERROR("TC not reserved.\n");
        aRetVal = LIB_FAILURE;
      } // else from if TC reserved allready
    } // if TC used
  } // for all possible TCs
  
  return aRetVal;
} // FdmaTcsApply()

//****************************************************************************

SDI_MipiDescriptor sdi_graph::MipiDescGet(enum CSI_IDX aIdx)
{
  SDI_MipiDescriptor lMipiDesc;
  lMipiDesc.mIdx = CSI_IDX_INVALID;
  if( (uint32_t)aIdx < CSI_IF_CNT )
  {
    if(mppCsi[aIdx] != NULL)
    {
      lMipiDesc.mIdx        = (enum CSI_IDX) aIdx;
      lMipiDesc.mCamType    = mpMeta->mcMipiCSIParams[aIdx].mcCameraType;
      lMipiDesc.mNumCameras = mpMeta->mcMipiCSIParams[aIdx].mcNumCameras;
      // get mipi buffer 
      for(uint32_t i = 0; i < MIPI_VIRT_CHN_NUM; i++)
      {
        if((uint32_t)mppCsi[aIdx]->mpOutBuf[i] != 0)
        {
          SEQ_Buf_t* lpMipiBuf  = (SEQ_Buf_t*)
                          (Off2Packed((uint32_t)mppCsi[aIdx]->mpOutBuf[i]));
        
          lMipiDesc.mpVc[i].mBufAddr     = lpMipiBuf->mpBuf + 
                          mpMeta->mcMipiCSIParams[aIdx].mcOutputBytesOffset[i];
          lMipiDesc.mpVc[i].mLineCnt     = 
                          lpMipiBuf->mcLineMax;
          lMipiDesc.mpVc[i].mLineBytes   =
                          mpMeta->mcMipiCSIParams[aIdx].mcBytesToTransfer[i];
          lMipiDesc.mpVc[i].mLineStride  = 
                          lpMipiBuf->mcLineStride;
          lMipiDesc.mpVc[i].mFrameHeight = 
                          mppCsi[aIdx]->mcNumInputLines;
        }
        else
        {
          memset(&lMipiDesc.mpVc[i], 0, sizeof(SDI_MipiVcDescriptor));
        }
      } // for all used virtual channels
    }
  }
  return lMipiDesc;
} // MipiDescGet()

//****************************************************************************

SDI_ViuDescriptor sdi_graph::ViuDescGet(enum VIU_IDX aIdx)
{
  SDI_ViuDescriptor lViuDesc;
  lViuDesc.mIdx = VIU_IDX_INVALID;
  if( (uint32_t)aIdx < VIU_IF_CNT )
  {
    if(mppViu[aIdx] != NULL)
    {
      lViuDesc.mIdx        = (enum VIU_IDX) aIdx;
      lViuDesc.mCamType    = mpMeta->mcViuParams[aIdx].mcCameraType;
      // get viu buffer 
      if((uint32_t)mppViu[aIdx]->mpOutBuf != 0)
      {
        SEQ_Buf_t* lpViuBuf  = (SEQ_Buf_t*)
                        (Off2Packed((uint32_t)mppViu[aIdx]->mpOutBuf));
      
        lViuDesc.mBufAddr     = lpViuBuf->mpBuf + 
                        mpMeta->mcViuParams[aIdx].mcOutputBytesOffset;
        lViuDesc.mLineCnt     = lpViuBuf->mcLineMax;
        lViuDesc.mLineBytes   = mpMeta->mcViuParams[aIdx].mcBytesToTransfer;
        lViuDesc.mLinePadding = 
                        lpViuBuf->mcLineStride;
      }
      else
      {
        memset(&lViuDesc, 0, sizeof(SDI_ViuDescriptor));
        lViuDesc.mIdx = VIU_IDX_INVALID;
      }
    }
  }
  else
  {
    // set invalid VIU index
    lViuDesc.mIdx = VIU_IDX_INVALID; 
    VDB_LOG_ERROR("Invalid VIU index.\n");
  }
  
  return lViuDesc;
} // ViuDescGet()

//****************************************************************************

LIB_RESULT sdi_graph::JpegDecDescGet(SDI_JpegDecDescriptor &arJpegDesc)
{  
  LIB_RESULT lRet = LIB_SUCCESS;
  if(mpJpegDec != NULL)
  {
    SEQ_Buf_t* lpJpegBuf = NULL;
    
    // get Y buffer 
    if((uint32_t)mpJpegDec->mpOutY != 0)
    {
      for(uint32_t i = DATASTREAM_1; i <= DATASTREAM_4; i++)
      {
        arJpegDesc.mpStreamDesc[i].mHeight = mpJpegDec->mcNumInputLines;
      } // for all possible streams
      
      lpJpegBuf  = (SEQ_Buf_t*)
                   (Off2Packed((uint32_t)mpJpegDec->mpOutY));
    
      arJpegDesc.mpCompDesc[SDI_JPEG_COMPONENT_Y].mBufAddr = 
          lpJpegBuf->mpBuf; 
      arJpegDesc.mpCompDesc[SDI_JPEG_COMPONENT_Y].mLineCnt = 
          lpJpegBuf->mcLineMax;
      arJpegDesc.mpCompDesc[SDI_JPEG_COMPONENT_Y].mLineBytes = 
          lpJpegBuf->mcLineBytes;
      arJpegDesc.mpCompDesc[SDI_JPEG_COMPONENT_Y].mLineStride = 
          lpJpegBuf->mcLineStride;
          
    } // if Y buffer exists
    else
    {
      lRet = LIB_FAILURE;
    } // else from if Y buffer exists
    
    // get U buffer 
    if((uint32_t)mpJpegDec->mpOutU != 0)
    {
      lpJpegBuf  = (SEQ_Buf_t*)
                   (Off2Packed((uint32_t)mpJpegDec->mpOutU));
    
      arJpegDesc.mpCompDesc[SDI_JPEG_COMPONENT_U].mBufAddr = 
          lpJpegBuf->mpBuf; 
      arJpegDesc.mpCompDesc[SDI_JPEG_COMPONENT_U].mLineCnt = 
          lpJpegBuf->mcLineMax;
      arJpegDesc.mpCompDesc[SDI_JPEG_COMPONENT_U].mLineBytes = 
          lpJpegBuf->mcLineBytes;
      arJpegDesc.mpCompDesc[SDI_JPEG_COMPONENT_U].mLineStride = 
          lpJpegBuf->mcLineStride;
    } // if U buffer exists
    else
    {
      lRet = LIB_FAILURE;
    } // else from if U buffer exists
    // get V buffer 
    if((uint32_t)mpJpegDec->mpOutV != 0)
    {
      lpJpegBuf  = (SEQ_Buf_t*)
                   (Off2Packed((uint32_t)mpJpegDec->mpOutV));
    
      arJpegDesc.mpCompDesc[SDI_JPEG_COMPONENT_V].mBufAddr = 
          lpJpegBuf->mpBuf; 
      arJpegDesc.mpCompDesc[SDI_JPEG_COMPONENT_V].mLineCnt = 
          lpJpegBuf->mcLineMax;
      arJpegDesc.mpCompDesc[SDI_JPEG_COMPONENT_V].mLineBytes = 
          lpJpegBuf->mcLineBytes;
      arJpegDesc.mpCompDesc[SDI_JPEG_COMPONENT_V].mLineStride = 
          lpJpegBuf->mcLineStride;
    } // if V buffer exists
    else
    {
      lRet = LIB_FAILURE;
    } // else from if // else from if Y buffer exists buffer exists
    // get A buffer 
    if((uint32_t)mpJpegDec->mpOutAlpha != 0)
    {
      lpJpegBuf  = (SEQ_Buf_t*)
                   (Off2Packed((uint32_t)mpJpegDec->mpOutAlpha));
    
      arJpegDesc.mpCompDesc[SDI_JPEG_COMPONENT_A].mBufAddr = 
          lpJpegBuf->mpBuf; 
      arJpegDesc.mpCompDesc[SDI_JPEG_COMPONENT_A].mLineCnt = 
          lpJpegBuf->mcLineMax;
      arJpegDesc.mpCompDesc[SDI_JPEG_COMPONENT_A].mLineBytes = 
          lpJpegBuf->mcLineBytes;
      arJpegDesc.mpCompDesc[SDI_JPEG_COMPONENT_A].mLineStride = 
          lpJpegBuf->mcLineStride;
    } // if A buffer exists
    else
    {
      lRet = LIB_FAILURE;
    } // else from if A buffer exists
  }// if jpeg is used in the graph
  else
  {
    lRet = LIB_FAILURE;
  }// else from if jpeg is used in the graph
    
  return lRet;
} // JpegDecDescGet()

//****************************************************************************

SDI_H264EncDescriptor sdi_graph::H264EncDescGet()
{  
  SDI_H264EncDescriptor lH264EncDesc;
  if(mpH264Enc != NULL)
  {
    SEQ_Buf_t* lpH264EncBuf = NULL;
    
    // get Y buffer 
    if((uint32_t)mpH264Enc->mpInY.mpInBuf != 0)
    {
      lpH264EncBuf  = (SEQ_Buf_t*)
                   (Off2Packed((uint32_t)mpH264Enc->mpInY.mpInBuf));
    
      lH264EncDesc.mpCompDesc[SDI_H264_COMPONENT_Y].mBufAddr = 
          lpH264EncBuf->mpBuf; 
      lH264EncDesc.mpCompDesc[SDI_H264_COMPONENT_Y].mLineCnt = 
          lpH264EncBuf->mcLineMax;
      lH264EncDesc.mpCompDesc[SDI_H264_COMPONENT_Y].mLineBytes = 
          lpH264EncBuf->mcLineBytes;
      lH264EncDesc.mpCompDesc[SDI_H264_COMPONENT_Y].mLineStride = 
          lpH264EncBuf->mcLineStride;
    } // if input Y exists
    else
    {
      memset(&lH264EncDesc, 0, sizeof(SDI_H264EncDescriptor));
    } // esle from if input Y exists
    
    // get Cb buffer 
    if((uint32_t)mpH264Enc->mpInU.mpInBuf != 0)
    {
      lpH264EncBuf  = (SEQ_Buf_t*)
                   (Off2Packed((uint32_t)mpH264Enc->mpInU.mpInBuf));
    
      lH264EncDesc.mpCompDesc[SDI_H264_COMPONENT_CB].mBufAddr = 
          lpH264EncBuf->mpBuf; 
      lH264EncDesc.mpCompDesc[SDI_H264_COMPONENT_CB].mLineCnt = 
          lpH264EncBuf->mcLineMax;
      lH264EncDesc.mpCompDesc[SDI_H264_COMPONENT_CB].mLineBytes = 
          lpH264EncBuf->mcLineBytes;
      lH264EncDesc.mpCompDesc[SDI_H264_COMPONENT_CB].mLineStride = 
          lpH264EncBuf->mcLineStride;
    } // if input Cb exists
    else
    {
      memset(&lH264EncDesc, 0, sizeof(SDI_H264EncDescriptor));
    } //else from if input Cb exists
    
    // get Cr buffer 
    if((uint32_t)mpH264Enc->mpInV.mpInBuf != 0)
    {
      lpH264EncBuf  = (SEQ_Buf_t*)
                   (Off2Packed((uint32_t)mpH264Enc->mpInV.mpInBuf));
    
      lH264EncDesc.mpCompDesc[SDI_H264_COMPONENT_CR].mBufAddr = 
          lpH264EncBuf->mpBuf; 
      lH264EncDesc.mpCompDesc[SDI_H264_COMPONENT_CR].mLineCnt = 
          lpH264EncBuf->mcLineMax;
      lH264EncDesc.mpCompDesc[SDI_H264_COMPONENT_CR].mLineBytes = 
          lpH264EncBuf->mcLineBytes;
      lH264EncDesc.mpCompDesc[SDI_H264_COMPONENT_CR].mLineStride = 
          lpH264EncBuf->mcLineStride;
    } // if input Cr exists
    else
    {
      memset(&lH264EncDesc, 0, sizeof(SDI_H264EncDescriptor));
    } // else from if input Cr exists
    
    // get Output buffer 
    if((uint32_t)mpH264Enc->mpOutBuf != 0)
    {
      lpH264EncBuf  = (SEQ_Buf_t*)
                   (Off2Packed((uint32_t)mpH264Enc->mpOutBuf));
    
      lH264EncDesc.mOutBuffAddr = lpH264EncBuf->mpBuf; 
      lH264EncDesc.mOutBuffSize = lpH264EncBuf->mcLineMax * 
                                  lpH264EncBuf->mcLineBytes;
    } // if output exists
    else
    {
      memset(&lH264EncDesc, 0, sizeof(SDI_H264EncDescriptor));
    } // else from if input Cr exists
  }
    
  return lH264EncDesc;
} // H264EncDescGet()

//****************************************************************************

LIB_RESULT sdi_graph::H264DecDescGet(SDI_H264DecDescriptor &arH264DecDesc)
{  
  LIB_RESULT lRet = LIB_SUCCESS;
  if(mpH264Dec != NULL)
  {
    SEQ_Buf_t* lpH264DecBuf = NULL;

    for(uint32_t i = 0; i < 4; i++)
    {
      // get Y buffer 
      if((uint32_t)mpH264Dec->mppOutY[i] != 0)
      {
        arH264DecDesc.mpStreamDesc[i].mActive = true;
        
        lpH264DecBuf  = (SEQ_Buf_t*)
                    (Off2Packed((uint32_t)mpH264Dec->mppOutY[i]));
                    
        SDI_ComponentDescriptor &lrCompDesc = 
          arH264DecDesc.mpStreamDesc[i].mpCompDesc[SDI_H264_COMPONENT_Y];
      
        lrCompDesc.mBufAddr    = lpH264DecBuf->mpBuf; 
        lrCompDesc.mLineCnt    = lpH264DecBuf->mcLineMax;
        lrCompDesc.mLineBytes  = lpH264DecBuf->mcLineBytes;
        lrCompDesc.mLineStride = lpH264DecBuf->mcLineStride;
        
        // remember number input image size
        arH264DecDesc.mpStreamDesc[i].mHeightLines = mpH264Dec->mcNumInputLines;
      } // if Y buffer exists
      else
      {
        memset(&(arH264DecDesc.mpStreamDesc[i]),
               0, 
               sizeof(SDI_H264DecStreamDescriptor));
      } // else from if Y buffer exists
      
      // get U buffer 
      if((uint32_t)mpH264Dec->mppOutCb[i] != 0)
      {
        lpH264DecBuf  = (SEQ_Buf_t*)
                    (Off2Packed((uint32_t)mpH264Dec->mppOutCb[i]));
      
        SDI_ComponentDescriptor &lrCompDesc = 
          arH264DecDesc.mpStreamDesc[i].mpCompDesc[SDI_H264_COMPONENT_CB];
          
        lrCompDesc.mBufAddr    = lpH264DecBuf->mpBuf; 
        lrCompDesc.mLineCnt    = lpH264DecBuf->mcLineMax;
        lrCompDesc.mLineBytes  = lpH264DecBuf->mcLineBytes;
        lrCompDesc.mLineStride = lpH264DecBuf->mcLineStride;
      } // if Cb buffer exists
      else
      {
        memset(&(arH264DecDesc.mpStreamDesc[i].mpCompDesc[SDI_H264_COMPONENT_CB]),
               0, 
               sizeof(SDI_H264DecStreamDescriptor));
      } // else from if Cb buffer exists
      // get V buffer 
      if((uint32_t)mpH264Dec->mppOutCr[i] != 0)
      {
        lpH264DecBuf  = (SEQ_Buf_t*)
                    (Off2Packed((uint32_t)mpH264Dec->mppOutCr[i]));
      
        SDI_ComponentDescriptor &lrCompDesc = 
          arH264DecDesc.mpStreamDesc[i].mpCompDesc[SDI_H264_COMPONENT_CR];
          
        lrCompDesc.mBufAddr    = lpH264DecBuf->mpBuf; 
        lrCompDesc.mLineCnt    = lpH264DecBuf->mcLineMax;
        lrCompDesc.mLineBytes  = lpH264DecBuf->mcLineBytes;
        lrCompDesc.mLineStride = lpH264DecBuf->mcLineStride;
      } // if Cr buffer exists
      else
      {
        memset(&(arH264DecDesc.mpStreamDesc[i].mpCompDesc[SDI_H264_COMPONENT_CR]),
               0, 
               sizeof(SDI_H264DecStreamDescriptor));
      } // else from if Cr buffer exists
    } // for all h264 decoder streams
  }// if h264 decoder used in the graph  
  else
  {
    lRet = LIB_FAILURE;
  } // else from if h264 decoder used in the graph  
  return lRet;
} // H264DecDescGet()

//****************************************************************************
LIB_RESULT sdi_graph::IOindexesGet(uint32_t aIOidxs[])
{
  LIB_RESULT lRet = LIB_SUCCESS;
  if(aIOidxs != NULL)
  {
    // reset all indexes
    for (uint32_t i = 0; i < SEQ_OTHRIX_LAST; ++i)
    {
      aIOidxs[i] = SEQ_OTHRIX_LAST;
    } // for all possible IOs
    
    // copy TC content
    if(mpPackedGraph != NULL)
    {
      SEQ_Head_t **lppHeads = (SEQ_Head_t**) mpPackedGraph;
      uint32_t i = 0;
      // for all heads
      while(lppHeads[i] != NULL)
      {
        SEQ_Head_t *lpHead = (SEQ_Head_t*)(((uintptr_t)lppHeads[i]) + 
                             (uintptr_t)mpPackedGraph);
        if(lpHead->mcNodeType == SEQ_NODE_OTHER)
        {
          aIOidxs[lpHead->mcEngIndex] = i;
        } // if OTHER engine type
        i++;
      } // while any heady left
    } // if graph packed
    else
    {
      VDB_LOG_ERROR("There is no packed graph at the moment.\n");
      lRet = LIB_FAILURE;
    } // else from if graph packed
  } // if idx array exists
  else 
  {
    VDB_LOG_ERROR("Supplied IO indexes array is NULL.\n");
    lRet = LIB_FAILURE;
  } // else form if idx array exists
  
  return lRet;
} // IOindexesGet(int32_t aIOidxs[])

//****************************************************************************

LIB_RESULT sdi_graph::IoQuery(SEQ_Othr_ix_t aIoNodeId)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  
  if(mIoNodeIdxArr[aIoNodeId] == -1)
  {
    lRet = LIB_FAILURE;
  } // if node not present
  
  return lRet;
} // IOindexesGet(SEQ_Othr_ix_t aIoNodeId)

//****************************************************************************

void sdi_graph::FdmaChannelReorder(void)
{
  if(mpFdma != NULL)
  {
    // get pointer to backed-up FDMA config
    SEQ_FdmaCfg_t *lpFdmaPackedBck = (SEQ_FdmaCfg_t *) mpFdmaPackedBck;

    // erase FDMA node PRAM copy
    memset(mpFdma->mOutBuf,0, sizeof(SEQ_FdmaDst_t) * FDMA_CHANNELS);
    memset(mpFdma->mInBuf, 0, sizeof(SEQ_BufferClient_t) * FDMA_CHANNELS);

    // set proper channel numbers
    for(int32_t i = 0; i < FDMA_CHANNELS; i++)
    {
      if(mTcs[i] != NULL)
      {   
        // copy the setup to propper channel num in PRAM version
        memcpy(&(mpFdma->mInBuf[mTcs[i]->mTdm.mTdIdx]), 
               &(lpFdmaPackedBck->mInBuf[i]), 
               sizeof(SEQ_BufferClient_t));
      
        // copy the setup to propper channel num in PRAM version
        mpFdma->mOutBuf[mTcs[i]->mTdm.mTdIdx] = lpFdmaPackedBck->mOutBuf[i];
        
        // set proper channel number to consumer object
        // get consumer index to the buffer
        uint32_t lConsIx = mpFdma->mInBuf[mTcs[i]->mTdm.mTdIdx].mcConsIx;
        // get pointer to the buffer
        SEQ_Buf_t *lpSrcBuf = (SEQ_Buf_t*)(Off2Packed((uintptr_t)mpFdma->mInBuf[mTcs[i]->mTdm.mTdIdx].mpInBuf));
        // set the correct channel Idx
        memcpy((void*)&(lpSrcBuf->mConsumer[lConsIx].mcChann), 
               &(mTcs[i]->mTdm.mTdIdx), 
               2);        
      } // if channel used
    } // for all possible channels
  } // if FDMA used
} // sdi_graph::FdmaChannelReorder()

//****************************************************************************

LIB_RESULT sdi_graph::Download(SDI_GraphAddr_t aOffset)
{
  LIB_RESULT lRet = LIB_SUCCESS;
  uintptr_t lGraphCRAM = 0;
  
  if(SEQ_GraphAddressGet(&lGraphCRAM) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to get Graph download address.\n");
    lRet = LIB_FAILURE;
  } // if failed to get graph address
  else
  {
    if(SEQ_GRAPH_SIZE_MAX < mpMeta->mGraphSize)
    {
      VDB_LOG_ERROR("Graph too big (0x%x bytes) for the slot (0x%x bytes).\n", 
                    (unsigned) mpMeta->mGraphSize, 
                    (unsigned) (SEQ_GRAPH_SIZE_MAX));
      lRet = LIB_FAILURE;
    } // if graph too big
    else
    {      
      // *** update pointers for future graph location ***
      Offset2Addr((uintptr_t)CRAM_HOST2M0(lGraphCRAM + aOffset));
      
      // *** prepare PRAM buffer list (0 termminated) ***
      uint32_t *lpBufferList = (uint32_t*) malloc((mpMeta->mBfrCnt + 1) * 
                                                  sizeof(uint32_t));
      if(lpBufferList != NULL)
      {
        for(uint32_t i = 0; i < mpMeta->mBfrCnt; i++)
        {
          lpBufferList[i] = (uintptr_t)CRAM_HOST2M0(mpMeta->mpBufferList[i] + 
                                                    lGraphCRAM + aOffset);
        } // while iterator not at the end 
        lpBufferList[mpMeta->mBfrCnt] = 0; // set the 0 terminator
        
        // *** generate graph package and download it ***
        SEQ_GraphPackage_t lGraphPackage;
        lGraphPackage.mGraphSize      = mpMeta->mGraphSize;
        lGraphPackage.mpGraphData     = mpPackedGraph;
        lGraphPackage.mBufferListSize = (mpMeta->mBfrCnt + 1) * 
                                        sizeof(uint32_t);
        lGraphPackage.mpBufferList    = lpBufferList;
        if(mppCsi[0] != NULL)
        {
          lGraphPackage.mCSINumLines  = mppCsi[0]->mcNumInputLines;
        } // if CSI[0] used
        else if(mppCsi[1] != NULL)
        {
          lGraphPackage.mCSINumLines  = mppCsi[1]->mcNumInputLines;
        } // if CSI[1] used
        else if(mppViu[0] != NULL)
        {
          lGraphPackage.mCSINumLines  = mppViu[0]->mcNumInputLines;
        } // if VIU[0] used
        else if(mppViu[1] != NULL)
        {
          lGraphPackage.mCSINumLines  = mppViu[1]->mcNumInputLines;
        } // if VIU[1] used
        else if(mpJpegDec != NULL)
        {
          lGraphPackage.mCSINumLines  = mpJpegDec->mcNumInputLines;
        } // if JPEG_DEC used
        else if(mpH264Dec != NULL)
        {
          lGraphPackage.mCSINumLines  = mpH264Dec->mcNumInputLines;
        } // if H264_DEC used
        else
        {
          lGraphPackage.mCSINumLines  = 0;
        } // else form if JPEG_DEC not used        
        
        // download the graph 
        if(SEQ_GraphDownload(&lGraphPackage) != LIB_SUCCESS)
        {
          VDB_LOG_ERROR("Graph download failed.\n");
          lRet = LIB_FAILURE;
        } // if graph download failed
        
        free(lpBufferList);
      } // if buffer list allocated ok
      else
      {
        VDB_LOG_ERROR("Buffer-list allocation for PRAM failed.\n");
        lRet = LIB_FAILURE;
      } // else from if buffer list allocation failed        
      // *** revert from pointers offsets ***
      Addr2Offset((uintptr_t)CRAM_HOST2M0(lGraphCRAM + aOffset));
    } // else from if graph too big
  } // else from if failed to get graph address  
  
  return lRet;
} // sdi_graph::Download(SDI_GraphAddr_t aOffset)

//****************************************************************************

LIB_RESULT sdi_graph::Fetch(SEQ_Head_Ptr_t **appGraph, 
                            SEQ_Buf_t     ***apppBufferList)
{
  LIB_RESULT lRet       = LIB_SUCCESS;
  uintptr_t  lGraphCRAM = 0;
  
  if(SEQ_GraphAddressGet(&lGraphCRAM) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to get Graph download address.\n");
    lRet = LIB_FAILURE;
  } // if failed to get graph address
  else
  {
    // *** prepare Graph and buffer list memory ***
    if(mpFetchedGraph == NULL)
    {
      mpFetchedGraph = (void*) malloc(mpMeta->mGraphSize);
      if(mpFetchedGraph == NULL)
      {
        VDB_LOG_ERROR("Failed to allocate fetched graph memory.\n");
        lRet = LIB_FAILURE;
      } // if graph alloc failed
      else
      {
        mppFetchedBufferList = (SEQ_Buf_t**) malloc((mpMeta->mBfrCnt + 1) * \
                                sizeof(SEQ_Buf_t*));
        if(mppFetchedBufferList == NULL)
        {
          VDB_LOG_ERROR("Failed to allocate fetched buffer list memory.\n");
          lRet = LIB_FAILURE;
        } // if buffer list alloc failed
      }// else from if alloc failed
    }// if not allocated yet
    
    //*** fetch the graph if all ok so far ***
    if(lRet == LIB_SUCCESS)
    { 
      // *** generate graph package ***
      SEQ_GraphPackage_t lGraphPackage;
      lGraphPackage.mGraphSize      = mpMeta->mGraphSize;
      lGraphPackage.mpGraphData     = mpFetchedGraph;
      lGraphPackage.mBufferListSize = (mpMeta->mBfrCnt + 1) * sizeof(uint32_t);
      lGraphPackage.mpBufferList    = NULL;
      
      //*** fetch the graph ***
      if(SEQ_GraphFetch(&lGraphPackage) != LIB_SUCCESS)
      {
        VDB_LOG_ERROR("Graph Upload failed.\n");
        lRet = LIB_FAILURE;
      } // if graph fetch failed

      // *** change addresses ***
      Addr2Addr(mpFetchedGraph, 
                (SDI_GraphAddr_t)CRAM_HOST2M0(lGraphCRAM), 
                (SDI_GraphAddr_t)(uintptr_t) mpFetchedGraph);

      // *** generate buffer list pointers ***
      for(uint32_t i = 0; i < mpMeta->mBfrCnt; i++)
      {
        mppFetchedBufferList[i] = (SEQ_Buf_t*)(mpMeta->mpBufferList[i] + 
                                  (uintptr_t) mpFetchedGraph);
      } // while iterator not at the end 
      mppFetchedBufferList[mpMeta->mBfrCnt] = 0; // set the 0 terminator
      
      // *** fill-in output variables
      *appGraph       = (SEQ_Head_Ptr_t*)mpFetchedGraph;    
      *apppBufferList = mppFetchedBufferList;
    } // if buffer lists allocated ok
    else
    {
      VDB_LOG_ERROR("Buffer-list allocation for PRAM failed.\n");
      lRet = LIB_FAILURE;
      
      // *** fill-in output variables
      *appGraph       = NULL;    
      *apppBufferList = NULL;
    } // else from if buffer list allocation failed        
  } // else from if failed to get graph address
  
  return lRet;
} // sdi_graph::Fetch(SDI_GraphAddr_t aOffset)

//****************************************************************************

uint32_t sdi_graph::SizeGet(GraphMetadata_t *apMeta)
{
  // *** find out number of engines in the graph
  uint32_t lEngCnt = 0;

  return SizeGet(apMeta, lEngCnt);
} // sdi_graph::SizeEval(SEQ_Head_t *aGraph[], uint32_t &arEngCnt)

//****************************************************************************

int32_t sdi_graph::FdmaChnlName2Idx(const char* acpName)
{
  int32_t lRet = -1; // nothing found yet
  
  if((mpMeta != NULL) && (acpName != NULL))
  {
    for(uint32_t i = 0; i < FDMA_CHANNELS; i++)
    {
      if(mTcs[i] != NULL)
      {
        if(strcmp(acpName, mpMeta->mcppFdmaChannelNames[i]) == 0)
        {
          lRet = i;
          break;
        } // if match found
      } // if FDMA channel used
      else
      {
        break;
      } // else from if FDMA channel used
    } // for all possible fdma channels
  } // if metadata available
  
  return lRet;
} // FdmaChnlName2Idx(const char* apName)

//****************************************************************************

int32_t sdi_graph::BufferName2Idx(const char* acpName)
{
  int32_t lRet = -1; // nothing found yet
  
  if((mpMeta != NULL) && (acpName != NULL))
  {
    for(uint32_t i = 0; i < mpMeta->mBfrCnt; i++)
    {
      if(strcmp(acpName, mpMeta->mcppBufferNames[i]) == 0)
      {
        lRet = i;
        break;
      } // if match found
    } // for all possible fdma channels
  } // if metadata available
  
  return lRet;
} // sdi_graph::BufferName2Idx(const char* acpName)

//****************************************************************************

uint32_t sdi_graph::SizeGet(GraphMetadata_t *apMeta, uint32_t &arEngCnt)
{
  // *** find out number of engines in the graph
  arEngCnt = 0;
  
  return apMeta->mGraphSize;
} // sdi_graph::SizeEval(GraphMetadata_t *apMeta, uint32_t &arEngCnt)

//****************************************************************************

uint32_t sdi_graph::SizeGet()
{
  return mpMeta->mGraphSize;
} // SizeGet()

//****************************************************************************

LIB_RESULT sdi_graph::Scan()
{
  LIB_RESULT lRes = LIB_SUCCESS;

  // *** go throuhg all defined heads
  while(mpGraph[mHeadCnt] != 0)
  {
    // get pointer to current head from packed graph
    SEQ_Head_t *lpCurrHead = (SEQ_Head_t*)((uint8_t*)mpPackedGraph + 
                             (uintptr_t)(mpGraph[mHeadCnt]));
    LIB_RESULT (sdi_graph::*pHeadScan)(SEQ_Head_t*) = NULL;

    switch(lpCurrHead->mcNodeType)
    {
      case SEQ_NODE_SCALAR:
      case SEQ_NODE_VECTOR:
      case SEQ_NODE_FWLOAD:
        break;
      case SEQ_NODE_OTHER:
      {
        // remeber IO node packed index and pointer
        mIoNodeIdxArr[lpCurrHead->mcEngIndex]   = mHeadCnt;
        mpIoNodePtrArr[lpCurrHead->mcEngIndex]  = lpCurrHead;
        
        switch( lpCurrHead->mcEngIndex )
        {
          case SEQ_OTHRIX_FDMA:
          {
            // remember pointer to the FDMA head
            mpFdma = (SEQ_FdmaCfg_t*)lpCurrHead;
          } // SEQ_NODE_VECTOR
            break;
          case SEQ_OTHRIX_H264ENC:
          {
            mpH264Enc = (SEQ_H264EncCfg_t*)lpCurrHead;
          }
            break;
          case SEQ_OTHRIX_H264DEC:
          {
            mpH264Dec = (SEQ_H264DecCfg_t*)lpCurrHead;
          }
            break;
          case SEQ_OTHRIX_JPEGDEC:
          {
            mpJpegDec = (SEQ_JpegDecCfg_t*)lpCurrHead;
          }
            break;
          case SEQ_OTHRIX_MIPICSI0:
          {
            mppCsi[0] = (SEQ_MipiCfg_t*)lpCurrHead;
          }
            break;
          case SEQ_OTHRIX_MIPICSI1:
          {
            mppCsi[1] = (SEQ_MipiCfg_t*)lpCurrHead;
          }  
            break;
          case SEQ_OTHRIX_VIU0:
          {
            mppViu[0] = (SEQ_ViuCfg_t*)lpCurrHead;
          }
            break;
          case SEQ_OTHRIX_VIU1:
          {
            mppViu[1] = (SEQ_ViuCfg_t*)lpCurrHead;
          }  
            break;
          default:
          {
            VDB_LOG_ERROR("Unknown index %u for engine at index %u.\n", 
                          lpCurrHead->mcEngIndex,
                          mHeadCnt
                          );
            lRes = LIB_FAILURE;
          } // default
        } // switch engine index
      } // SEQ_NODE_OTHER
        break;
      default:
      {
        VDB_LOG_NOTE("Unknown type %u for engine at index %u.\n", 
                     lpCurrHead->mcNodeType,
                     mHeadCnt
                    );
        lRes = LIB_FAILURE;
      } // default
      break;
    } // switch engine type

    // *** scan the head internals ***
    if(pHeadScan != NULL)
    {
      (this->*pHeadScan)(lpCurrHead);
      pHeadScan = NULL;
    } // if specific scan required
    
    mHeadCnt++;
  } // for all heads

  return lRes;
} // sdi_graph::Scan(SEQ_Head_t *apGraph[])

//****************************************************************************

LIB_RESULT sdi_graph::Pack()
{
  LIB_RESULT lRet = LIB_SUCCESS;

  // *** allocate memory for packed graph ***
  mpPackedGraph   = malloc(mpMeta->mGraphSize);
  if(mpPackedGraph == NULL)
  {
    lRet = LIB_FAILURE;
    VDB_LOG_ERROR("Failed to alloc memory for packed ISP graph.\n");
  } // if malloc failed
  else
  {
    mpPackedObjects = (void*)((uint8_t*)mpPackedGraph + 
                      mpMeta->mpObjectList[0].mcOffset);
  
    // *** copy graph objects to predefined offset ***
    ObjectsCopy();
  } // else from if malloc failed
 
  return lRet;
} // sdi_graph::Pack(SEQ_Head_t *aGraph[])

//****************************************************************************

void sdi_graph::ObjectsCopy()
{
  // go through all objects
  for(uint32_t i = 0; i < mpMeta->mObjCnt; i++)
  {
    // prepare r/w addresses
    void       *lpWrite = (void*)((uint8_t*)mpPackedGraph + 
                          mpMeta->mpObjectList[i].mcOffset);
    const void *lpRead  = mpMeta->mpObjectList[i].mcpAddr;
    
    // copy object
    memcpy(lpWrite,lpRead, mpMeta->mpObjectList[i].mcSize);
  } // for all objects
} // void sdi_graph::ObjectsCopy()

//****************************************************************************

void sdi_graph::Addr2Offset(SDI_GraphAddr_t aAddrBase)
{
  SDI_GraphAddr_t *lpCurrPointer = NULL;

  //*** go through all Pointers ***
  for(uint32_t i = 0; i < mpMeta->mPtrCnt; i++)
  {
    // get pointer at offset
    lpCurrPointer = (SDI_GraphAddr_t*)((uintptr_t)mpPackedGraph + 
                    mpMeta->mpPointerList[i]);

    // update the pointer
    *lpCurrPointer = (*lpCurrPointer) - aAddrBase;
  } // while iterator not at the end
  
  //*** go through all buffer pointers in FDMA ***
  // FDMA buffer pointers not in the pointer list (can change position)
  if(mpFdma != NULL)
  {
    // for all used TCs
    for(int32_t i = 0; i < FDMA_CHANNELS; i++)
    {
      if(mTcs[i] != NULL)
      {   
        // update input buffer address
        SDI_GraphAddr_t lNewAddress = 
              ((SDI_GraphAddr_t) mpFdma->mInBuf[mTcs[i]->mTdm.mTdIdx].mpInBuf) - 
              aAddrBase;
        memcpy((void*)&(mpFdma->mInBuf[mTcs[i]->mTdm.mTdIdx].mpInBuf), 
               (void*)&lNewAddress, 
               sizeof(SDI_GraphAddr_t));
        
        // update output buffer address
        lNewAddress = 
            ((SDI_GraphAddr_t) mpFdma->mOutBuf[mTcs[i]->mTdm.mTdIdx].mpOutBuf) - 
            aAddrBase;
        memcpy((void*)&(mpFdma->mOutBuf[mTcs[i]->mTdm.mTdIdx].mpOutBuf), 
               (void*)&lNewAddress, 
               sizeof(SDI_GraphAddr_t));      
      } // if channel used
    } // for all possible channels
  } // if FDMA used
} // sdi_graph::Addr2Offset()

//****************************************************************************

void sdi_graph::Addr2Addr(void* apGraph, 
                                SDI_GraphAddr_t aOldAddrBase, 
                                SDI_GraphAddr_t aNewAddrBase)
{
  SEQ_Head_Ptr_t *lpCurrPointer = NULL;

  //*** go through all Pointers ***
  for(uint32_t i = 0; i < mpMeta->mPtrCnt; i++)
  {
    // get pointer at offset
    lpCurrPointer = (SEQ_Head_Ptr_t*)((uintptr_t)apGraph + 
                    mpMeta->mpPointerList[i]);

    // update the pointer
    *lpCurrPointer = (SEQ_Head_Ptr_t)(((uint32_t)(uintptr_t)(*lpCurrPointer)) - 
                     aOldAddrBase + aNewAddrBase);
  } // while iterator not at the end
  
  //*** go through all buffer pointers in FDMA ***
  // FDMA buffer pointers not in the pointer list (can change position)
  if(mpFdma != NULL)
  {
    // for all used TCs
    for(int32_t i = 0; i < FDMA_CHANNELS; i++)
    {
      if(mTcs[i] != NULL)
      {   
        // update input buffer address
        SDI_GraphAddr_t lNewAddress = 
              ((SDI_GraphAddr_t) mpFdma->mInBuf[mTcs[i]->mTdm.mTdIdx].mpInBuf) + 
              aNewAddrBase;
        memcpy((void*)&(mpFdma->mInBuf[mTcs[i]->mTdm.mTdIdx].mpInBuf), 
               (void*)&lNewAddress, 
               sizeof(SDI_GraphAddr_t));
        
        // update output buffer address
        lNewAddress = 
            ((SDI_GraphAddr_t) mpFdma->mOutBuf[mTcs[i]->mTdm.mTdIdx].mpOutBuf) + 
            aNewAddrBase;
        memcpy((void*)&(mpFdma->mOutBuf[mTcs[i]->mTdm.mTdIdx].mpOutBuf), 
               (void*)&lNewAddress, 
               sizeof(SDI_GraphAddr_t));      
      } // if channel used
    } // for all possible channels
  } // if FDMA used
} // LIB_RESULT sdi_graph::Addr2Addr()

//****************************************************************************

void sdi_graph::Offset2Addr(SDI_GraphAddr_t aAddrBase)
{
  SDI_GraphAddr_t *lpCurrPointer = NULL;

  //*** go through all Pointers ***
  for(uint32_t i = 0; i < mpMeta->mPtrCnt; i++)
  {
    // get pointer at offset
    lpCurrPointer = (SDI_GraphAddr_t*)((uintptr_t)mpPackedGraph + 
                    mpMeta->mpPointerList[i]);

    // update the pointer
    if((*lpCurrPointer) != 0)
    {
      *lpCurrPointer = *lpCurrPointer + aAddrBase;
    }
  } // while iterator not at the end

  //*** go through all buffer pointers in FDMA ***
  // FDMA buffer pointers not in the pointer list (can change position)
  if(mpFdma != NULL)
  {
    // for all used TCs
    for(int32_t i = 0; i < FDMA_CHANNELS; i++)
    {
      if(mTcs[i] != NULL)
      {   
        // update input buffer address
        SDI_GraphAddr_t lNewAddress = 
              (SDI_GraphAddr_t) (mpFdma->mInBuf[mTcs[i]->mTdm.mTdIdx].mpInBuf);
        lNewAddress += aAddrBase;
        memcpy((void*)&(mpFdma->mInBuf[mTcs[i]->mTdm.mTdIdx].mpInBuf), 
               (void*)&lNewAddress, 
               sizeof(SDI_GraphAddr_t));
        
        // update output buffer address
        lNewAddress = 
            ((SDI_GraphAddr_t) mpFdma->mOutBuf[mTcs[i]->mTdm.mTdIdx].mpOutBuf) + 
            aAddrBase;
        memcpy((void*)&(mpFdma->mOutBuf[mTcs[i]->mTdm.mTdIdx].mpOutBuf), 
               (void*)&lNewAddress, 
               sizeof(SDI_GraphAddr_t));      
      } // if channel used
    } // for all possible channels
  } // if FDMA used
} // AddrUpdate(uint32_t aAddrBase)

//****************************************************************************
