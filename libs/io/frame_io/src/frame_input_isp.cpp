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
#include <cstddef>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "isp_sram.h"
#include "isp_csi.h"
#include "isp_cam_generic.h"
#include "isp_cam_sony.h"
#include "isp_cam_ov10640.h"
#include "isp_cam_ov10635.h"
#include "isp_cam_maxim.h"
#include "../../sdi/include/sdi_graph.hpp"
#include "frame_input_isp.h"
#include "isp_seq.h"
#include "isp_fdma.h"
#include "isp_viu.h"
#include "isp_jpegdec.h"

//******************************************************************************
#define	DEF_IMG_WIDTH	640
#define	DEF_IMG_HEIGHT	495
#define	DEF_IMG_DEPTH	3
#define	DEF_BUF_WIDTH	1920
#define	DEF_BUF_HEIGHT	1080

#define	FDMA_CHN_DEBAY	1
#define	FDMA_CHN_RAW	0
#define	FDMA_OFFS_DEBAY	(1280*2)	// RAW pixel 16bit
#define	FDMA_OFFS_RAW	0

//******************************************************************************

bool __frame_in_timeout_enable=1;
bool *__frame_in_timeout_enable_oalp=0;

using namespace io;

extern char sequencer_srec[];
extern char kmem_srec[];
extern SEQ_Head_Ptr_t	gpGraph[];
extern GraphMetadata_t	gGraphMetadata;
volatile unsigned char	gIspMsg=0;

#ifdef __STANDALONE__  
static SEQ_EventLog_t *spEventLog = NULL;
#endif // #ifdef __STANDALONE__  

static IspUserInitFunc_t spUserInitFunc;
static void *spParam;

static SDI_MipiDescriptor     sMipiCsiDesc[CSI_IF_CNT];
static SDI_ViuDescriptor      sViuDesc[VIU_IF_CNT];
static SDI_JpegDecDescriptor  sJpegDecDesc;

static int32_t sViuFd  = -1;
// static int32_t sJpegFd = -1;

FrameInputISP* FrameInputISP::mpThis = NULL;

//******************************************************************************

extern "C"
{
  extern void flush_dcache_range(void *, unsigned int);
  extern unsigned long get_uptime_microS(void); // *** us since reset ***
}

/***************************************************************************/
/** Ensures exclusive access to variables in memory.
 * Reads (exclusively) the "ready" index and stores it to "popped" index.
 * Sets (exclusively) the "ready" index to -1 to signal no frame is ready 
 * any more. Makes sure nobody modified the "ready" index between the read and 
 * write operations.
 *
 * \param apReady  address of the ready frame index
 * \param apPopped address of the popped frame index
 ***************************************************************************/
#ifndef __ARMV8
static void FramePopAtomic(int32_t *apReady, int32_t *apPopped);
#endif // #ifdef __ARMV8

/***************************************************************************/
/** Initializes required HW.
 * Operates Sequencer, FDMA and SRAM.
 *
 * \return LIB_SUCCESS
 *         LIB_FAILURE
 ***************************************************************************/
static LIB_RESULT HwOpen(void);

/***************************************************************************/
/** De-initializes HW.
 * Operates Sequencer, FDMA and SRAM.
 *
 * \return LIB_SUCCESS
 *         LIB_FAILURE
 ***************************************************************************/
static LIB_RESULT HwClose(void);

/***************************************************************************/
/** Starts Sequencer firmware execution by CM0+.
 * Resets the HW and driver internals.
 * Executes the firmware again.
 *
 * \return LIB_SUCCESS
 *         LIB_FAILURE
 ***************************************************************************/
void SeqStart(uint32_t aFrameCnt, uint32_t aInputLines);

/***************************************************************************/
/** Handler for frame_done IRQ.
 *
 ***************************************************************************/
#ifdef __STANDALONE__
static void CSI_IrqHandler(void);
#endif // #ifdef __STANDALONE__

void IspHwSramAccesAllow(void);

//******************************************************************************

namespace io
{


//#if defined(__KERNEL__) || defined(__STANDALONE__)
//******************************************************************************

FrameInputISP::FrameInputISP(bool aInitMIPI)
:       mFrmCnt(0), mLastCnt(0), mStatus(IO_OK),
        mImgWidth(DEF_IMG_WIDTH), mImgHeight(DEF_IMG_HEIGHT), 
        mImgDepth(DEF_IMG_DEPTH), mBufWidth(DEF_BUF_HEIGHT), \
        mBufHeight(DEF_BUF_HEIGHT)
{
  //__frame_in_timeout_enable_oalp=
   // (bool *)OAL_MemoryReturnAddress(&__frame_in_timeout_enable,ACCESS_NCH_NB);
  IspInit(NULL, NULL, aInitMIPI);
} // FrameInputISP()

//******************************************************************************

void FrameInputISP::IspInit(IspUserInitFunc_t apUserInitFunc, void *apParam, bool aInitMIPI)
{  
  //*** Object internal setup ***
  mpThis = this;
  spUserInitFunc = apUserInitFunc;
  spParam = apParam;
  
  for(uint32_t i = 0; i < FDMA_CHNL_CNT; i++)
  {
    mpStreams[i].mpBuffers = NULL;
    mpPoppedBufferInfo[i].mBufferIdx = 0xffffffff;
  } // for all possible mpStreams
  
  // *** Init HW ***
  if( HwOpen() != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to init HW.\n");
    return;
  }
  
  // *** Allow access to SRAM ***
#ifdef __STANDALONE__  
  IspHwSramAccesAllow(); 
  
  SEQ_EventLogGet(&spEventLog);  // get the event series for later use
  // *** Register Event Handler ***
  SEQ_EventHandlerSet(SeqEventHandler);
#else  
  // *** Register Event Handler ***
  SEQ_EventHandlerSet(SeqEventHandler);
#endif // #ifdef __STANDALONE__   
  
  // *** FW download ***
  SEQ_FwArrDownload(sequencer_srec, SEQ_FW_CM0);
  SEQ_FwArrDownload(kmem_srec, SEQ_FW_KERNEL);
  
  // *** Graph create ***
  mpGraph = new sdi_graph(gpGraph, &gGraphMetadata);
  
  // *** Graph setup ***
  
  // *** Allocate required graph resources ***
  if(LIB_SUCCESS != mpGraph->Finalize())
  {
    VDB_LOG_ERROR("Finalizing the graph failed!\n");
    return;
  } // if graph finalization failed
  
  if(!aInitMIPI)
  {
    return;
  }
  
  MipiCsiInit();
  ViuInit();
  JpegDecInit();
} // FrameInputISP::IspInit()

//******************************************************************************

void FrameInputISP::MipiCsiInit()
{ 
  // for all possible mipi interfaces
  for(uint32_t i = CSI_IDX_0; i < CSI_IDX_INVALID; i++)
  {
    // get mipi descriptor from graph
    sMipiCsiDesc[i] = mpGraph->MipiDescGet((enum CSI_IDX)i); // try idx 0 first
    if(sMipiCsiDesc[i].mIdx != i)
    {
      VDB_LOG_NOTE("CSI_%d not used.\n");
      continue;
    }
    else
    {
      VDB_LOG_NOTE("CSI_%d used.\n");
    }
      
    // setup IRQ handler
#ifdef __STANDALONE__
    CSI_IrqHandlerSet(sMipiCsiDesc[i].mIdx, CSI_IrqHandler);
#endif  
    
    SONY_Geometry_t lGeom;
    memset(&lGeom, 0, sizeof(SONY_Geometry_t));
    
    CSI_Cfg_t lCsiConfig;
    memset(&lCsiConfig, 0, sizeof(lCsiConfig));
    
#ifdef CAM_OV10640
    OV10640_RegConfig(sMipiCsiDesc[i].mIdx);
    // hard-config camera geometry
    lGeom.mHeight = 729;
#else // #ifdef CAM_OV10640
  #ifdef CAM_MAXIM
      MAXIM_Init(sMipiCsiDesc[i].mIdx, CAM_MAXIM);
      // hard-config camera geometry
      lGeom.mHeight = 800;
  #else // #ifdef CAM_MAXIM
        // configure camera
        SONY_Open((CSI_IDX)sMipiCsiDesc[i].mIdx);
        SONY_RegConfig((CSI_IDX)sMipiCsiDesc[i].mIdx);
        lCsiConfig.mLanes = 2;
        lCsiConfig.mMbps  = 594;

        // get camera geometry
        lGeom.mCsiIdx = (CSI_IDX)sMipiCsiDesc[i].mIdx;
        SONY_GeometryGet(lGeom);
  #endif // else from #ifdef CAM_MAXIM
#endif // else from #ifdef CAM_OV10640

    
    // *** initialize CSI receiver VC 0 ***    
    lCsiConfig.mIdKey.mKey              = 0;
    lCsiConfig.mIdKey.mId.mCsi          = sMipiCsiDesc[i].mIdx;

    for(int32_t j = 0; j < MIPI_VIRT_CHN_NUM; j++)
    {
      lCsiConfig.mpVc[j].mBufAddr       = sMipiCsiDesc[i].mpVc[j].mBufAddr;
      lCsiConfig.mpVc[j].mDoneLineCnt   = 1;
      lCsiConfig.mpVc[j].mLineCnt       = sMipiCsiDesc[i].mpVc[j].mLineCnt; // buffer lines
      lCsiConfig.mpVc[j].mFrameHeight   = lGeom.mHeight; // buffer lines
      lCsiConfig.mpVc[j].mLineBytes     = sMipiCsiDesc[i].mpVc[j].mLineBytes;
      lCsiConfig.mpVc[j].mLineStride    = sMipiCsiDesc[i].mpVc[j].mLineStride;
      
      if(lCsiConfig.mpVc[j].mBufAddr != 0)
      {
        CSI_VC_IDX_ADD(lCsiConfig.mIdKey, j);
      } // if VC used
    } // for all virtual channels
      
    CSI_Config(&lCsiConfig);
  } // for all possible CSI interfaces
}  // FrameInputISP::MipiCsiInit()
//#endif

//******************************************************************************

void FrameInputISP::ViuInit()
{ 
  // for all possible viu interfaces
  for(uint32_t i = VIU_IDX_0; i < VIU_IDX_INVALID; i++)
  {
    // get viu descriptor from graph
    sViuDesc[i] = mpGraph->ViuDescGet((enum VIU_IDX)i); // try idx 0 first
    if(sViuDesc[i].mIdx != i)
    {
      VDB_LOG_NOTE("VIU_%d not used.\n");
      continue;
    }
    else
    {
      VDB_LOG_NOTE("VIU_%d used.\n");
    }
      
    // setup IRQ handler
#if 1
    OV10635_Open(sViuDesc[i].mIdx);
#ifndef __STANDALONE__    
    //setup VIU receiver
    DMA_CONFIG lViuDmaConfig;
    lViuDmaConfig.buff_addr = sViuDesc[i].mBufAddr;
    lViuDmaConfig.tx_size   = sViuDesc[i].mLineCnt; 
    lViuDmaConfig.nmb_lines = 800; // number of lines per frame
    lViuDmaConfig.nmb_pixells = 1280;//sViuDesc[i].mLineBytes; //TODO: correct value
    lViuDmaConfig.frame_inc = 0;//sViuDesc[i].mLinePadding;
    lViuDmaConfig.alpha_val = 0xff; 
#endif // #ifndef __STANDALONE__    

#ifndef __STANDALONE__    
    ioctl(sViuFd, VIULITE_IOCTL_DMA_CONFIG, &lViuDmaConfig);
    
    VIU_DATA_INTERFACE lViuDataInterface;
    lViuDataInterface.pclk_pol    = 0;
    lViuDataInterface.vsync_pol   = 0;
    lViuDataInterface.hsync_pol   = 1;
    lViuDataInterface.endianness  = 1;
#endif // #ifndef __STANDALONE__    

#ifndef __STANDALONE__    
    ioctl(sViuFd, VIULITE_IOCTL_SET_DATA_INTERFACE, &lViuDataInterface);
    
    VIU_INPUT_FORMAT lViuInputFormat;
    lViuInputFormat.mode              = PARALLEL_MODE; 
    lViuInputFormat.width             = WIDE_8_BITS; // 8 bits
    lViuInputFormat.clocks_per_pixell = CPP_2_PP;
#endif // #ifndef __STANDALONE__    

#ifndef __STANDALONE__    
    ioctl(sViuFd, VIULITE_IOCTL_SET_VIDEOIN_FORMAT, &lViuInputFormat);
    
    // configure camera registers
    OV10635_RegConfig(VIU_IDX_0);
#endif // #ifndef __STANDALONE__

#ifndef __STANDALONE__    
    //enable VIU interrupts
    //uint8_t lIrqMask = 2;
    //ioctl(sViuFd, VIULITE_IOCTL_CONFIG_IRQS, &lIrqMask);
    
#endif // ifndef __STANDALONE__
#endif // 0
  } // for all possible VIU interfaces
}  // FrameInputISP::ViuInit()

//******************************************************************************

void FrameInputISP::JpegDecInit()
{   
#if 1
#ifndef __STANDALONE__   
  // *** initialize JPEG decoder interaction ***
  JPEGDEC_Open();
  
  // software reset
  JPEGDEC_SwReset();
  
  // setup Jpeg decoder output buffers
  OUTPUT_BUFFCFG lJpegDecOutBufs;
  lJpegDecOutBufs.c1_outbuffer = 
    sJpegDecDesc.mpCompDesc[SDI_JPEG_COMPONENT_Y].mBufAddr;
  lJpegDecOutBufs.c1_b2lines = 
    sJpegDecDesc.mpCompDesc[SDI_JPEG_COMPONENT_Y].mLineCnt;
    
  lJpegDecOutBufs.c2_outbuffer = 
    sJpegDecDesc.mpCompDesc[SDI_JPEG_COMPONENT_U].mBufAddr;
  lJpegDecOutBufs.c3_outbuffer = 
    sJpegDecDesc.mpCompDesc[SDI_JPEG_COMPONENT_V].mBufAddr;
  lJpegDecOutBufs.c2_c3_b2lines = 
    sJpegDecDesc.mpCompDesc[SDI_JPEG_COMPONENT_U].mLineCnt;
  
  lJpegDecOutBufs.c4_outbuffer = 
    sJpegDecDesc.mpCompDesc[SDI_JPEG_COMPONENT_A].mBufAddr;  
  lJpegDecOutBufs.c4_b2lines = 
    sJpegDecDesc.mpCompDesc[SDI_JPEG_COMPONENT_A].mLineCnt;
    
  JPEGDEC_OutConfig(&lJpegDecOutBufs);

  // setup outsample mode
  JPEGDEC_OutSampleModeSet(JPEGDCD_ON);
  
  // setup bankstride
  JPEGDEC_BankStrideSet(0);
  
  // setup stream inputs
  STREAM_JPEG_PARAM lStreamParams;
  
  lStreamParams.stream_no     = (STREAM_ID)0;
  lStreamParams.x_size        = 1280;
  lStreamParams.y_size        = 800;
  lStreamParams.precision     = PREC_8BITS; // 8 or 12 bits
  lStreamParams.component_num = 3;
  lStreamParams.wr_config     = JPEGDCD_OFF; // 1 static values, 0 encoded in data stream
  
  lStreamParams.c1_samplingfactor_h  = 1;
  lStreamParams.c1_samplingfactor_v  = 1;
  lStreamParams.c2_samplingfactor_h  = 2;
  lStreamParams.c2_samplingfactor_v  = 2;
  lStreamParams.c3_samplingfactor_h  = 2;
  lStreamParams.c3_samplingfactor_v  = 2;
  lStreamParams.c4_samplingfactor_h  = 1;
  lStreamParams.c4_samplingfactor_v  = 1;
  
  JPEGDEC_StreamConfig(&lStreamParams);
  lStreamParams.stream_no     = (STREAM_ID)1;
  JPEGDEC_StreamConfig(&lStreamParams);
  lStreamParams.stream_no     = (STREAM_ID)2;
  JPEGDEC_StreamConfig(&lStreamParams);
  lStreamParams.stream_no     = (STREAM_ID)3;
  JPEGDEC_StreamConfig(&lStreamParams);
  
  // setup jpeg mode
  JPEGDEC_ModeSet(COMPATIBILITY); // number of streams
  
  // setup reset interval
  JPEGDEC_RstIntervalSet(0); // TODO: ???
  
#endif // ifndef __STANDALONE__
#endif // 0
}  // FrameInputISP::JpegDecInit()

//******************************************************************************

int32_t FrameInputISP::DdrBuffersSet(const IspBufferStream &arcBufferStream)
{
  int32_t lRet = 0;
  
  if(mpStreams[arcBufferStream.mStreamIdx].mpBuffers != NULL)
  {
    VDB_LOG_ERROR("Stream assigned already.\n");
    lRet = -1;
  } // if assigned already
  else
  {
    // *** remember new stream setup ***
    IspBufferStream *lpStream = &mpStreams[arcBufferStream.mStreamIdx];
    *lpStream = arcBufferStream;
    lpStream->mpBuffers = (uint32_t*)malloc(sizeof(uint32_t) * lpStream->mCnt);
    
    if(lpStream->mpBuffers == NULL)
    {
      VDB_LOG_ERROR("Failed to allocate buffer pointers.\n");
      lRet = -1;
    } // if malloc failed
    else
    {
      // copy pointers
      for(uint32_t i = 0; i < lpStream->mCnt; i++)
      {
        lpStream->mpBuffers[i] = arcBufferStream.mpBuffers[i];
      } // for all buffer pointers
         
      // *** request Sequencer DDR buffers ***
      SEQ_BufferRequest_t lSeqBuffRequest;
      lSeqBuffRequest.mStreamIdx   = lpStream->mStreamIdx;
      lSeqBuffRequest.mCnt         = lpStream->mCnt;
      lSeqBuffRequest.mBaseShift   = lpStream->mBaseShift;
      lSeqBuffRequest.mBufferSize  = lpStream->mLineStride * lpStream->mLineCnt;
      lSeqBuffRequest.mpBuffers    = lpStream->mpBuffers;
      if(SEQ_FrmBuffersRequest(&lSeqBuffRequest) == SEQ_LIB_SUCCESS)
      {      
        // *** query and push buffers ***
        SEQ_FrmBufferInfo_t lBufferInfo;
        lBufferInfo.mBufferIdx = 0;
        lBufferInfo.mStreamIdx = lpStream->mStreamIdx;
        
        while(SEQ_FrmBufferQuery(&lBufferInfo) == SEQ_DRV_SUCCESS)
        {
          SEQ_FrmBufferPush(&lBufferInfo);
          lBufferInfo.mBufferIdx++;
        } // for all available buffers
        
        // *** setup FMDA TD ***
        FDMA_Tc_t lTc;
        FDMA_Td_t *lpTD = NULL;
        mpGraph->FdmaTcGet(lTc, lpStream->mStreamIdx);
        lpTD = &lTc.mTd;
        
        
        lpTD->mDdrImgStartAddr = lpStream->mpBuffers[0] + lpStream->mBaseShift;
        lpTD->mDdrLineStride   = lpStream->mLineStride/32;
        lpTD->mDdrLineCnt      = lpStream->mLineCnt;
        lpTD->mWrEna           = 1;
        lpTD->mCrcEna          = 0;
        lpTD->mCrcMode         = 0;
        lpTD->mCrcPoly         = 1;
        
        mpGraph->FdmaTcSet(lTc, lpStream->mStreamIdx);
      } // if buffers requested successfully
      else
      {
        VDB_LOG_ERROR("Failed to requrest frame buffers.\n");
        lRet = -1;
      } // else from if buffers requested successfully
    } // else from if failed to malloc buffer pointers
  } // else from if assigned already
  
  return lRet;
} // FrameInputISP::DdrBuffersSet();

//******************************************************************************

void FrameInputISP::StreamClean(uint32_t aIdx)
{
  IspBufferStream *lpStream = &mpStreams[aIdx];
  if(lpStream->mpBuffers != NULL)
  {
    free(lpStream->mpBuffers);
  }
}

//******************************************************************************
 
void FrameInputISP::Start(uint32_t aFrameCnt, uint32_t aInputLines)
{  
  //*** Graph download ***
  mpGraph->Download(0);
  
  //*** Apply FDMA TC setup to SRAM TDT ***
  mpGraph->FdmaTcsApply();
  
  //*** Start Sequencer/Execute Graph ***
  SeqStart(aFrameCnt, aInputLines); // start sequencer
  
  for(volatile uint32_t i = 0; i < 100000; i++);
} // FrameInputISP::Start()

//******************************************************************************

void FrameInputISP::StartCam(enum CSI_IDX aCsiIdx)
{  
  if(sMipiCsiDesc[aCsiIdx].mIdx == aCsiIdx)
  {
   // start the camera
#ifdef CAM_OV10640  
    OV10640_CsiEnable(sMipiCsiDesc[aCsiIdx].mIdx);
#else
  #ifdef CAM_MAXIM  
    MAXIM_CsiEnable(sMipiCsiDesc[aCsiIdx].mIdx);
  #else
    SONY_CsiEnable((CSI_IDX)sMipiCsiDesc[aCsiIdx].mIdx);
  #endif
#endif // #ifndef CAM_OV10640  
    
    // execute CSI receiver
    CSI_IdKey_t lCsiKey;
    lCsiKey.mKey = 0;  
    CSI_IDX_SET(lCsiKey, sMipiCsiDesc[aCsiIdx].mIdx);
    CSI_VC_IDX_ADD(lCsiKey, 0);
    
    CSI_Start(lCsiKey);
    // from now on grabbing continuously in background
  } // if CSI index used
} // FrameInputISP::StartCam(enum CSI_IDX aCsiIdx)

//******************************************************************************

void FrameInputISP::StartCam(enum VIU_IDX aViuIdx)
{  
  if(sViuDesc[aViuIdx].mIdx == aViuIdx)
  {
    //start the camera
    //OV10635_DataEnable(sViuDesc[aViuIdx].mIdx);

#ifndef __STANDALONE__    
    //enable VIU receiver    
    //ioctl(sViuFd, VIULITE_IOCTL_DMA_START);
#endif // #ifndef __STANDALONE__    
    
#ifndef __STANDALONE__    
    //enable VIU interrupts
    uint8_t lIrqMask = 2;
    ioctl(sViuFd, VIULITE_IOCTL_CONFIG_IRQS, &lIrqMask);
    
#endif // ifndef __STANDALONE__
    // from now on grabbing continuously in background
  } // if CSI index used
} // FrameInputISP::StartCam(enum CSI_IDX aCsiIdx)

//******************************************************************************


void FrameInputISP::StartCam()
{  
  // for all possible mipi interfaces
  for(uint32_t i = CSI_IDX_0; i < CSI_IDX_INVALID; i++)
  {
    StartCam((enum CSI_IDX)i);
  } // for all possible mipi interfaces

} // FrameInputISP::StartCam()

//******************************************************************************

void FrameInputISP::Stop()
{ 
  CSI_IdKey_t lCsiKey;
  
  // for all possible mipi interfaces
  for(uint32_t i = CSI_IDX_0; i < CSI_IDX_INVALID; i++)
  {
    if(sMipiCsiDesc[i].mIdx != i)
    {
      continue;
    }
  
    lCsiKey.mKey = 0;  
    CSI_IDX_SET(lCsiKey, sMipiCsiDesc[i].mIdx);
    CSI_VC_IDX_ADD(lCsiKey, 0);
    
    CSI_Stop(lCsiKey); 
    
    // start the camera
#ifdef CAM_OV10640  
    OV10640_CsiDisable(sMipiCsiDesc[i].mIdx);
#else
  #ifdef CAM_MAXIM  
      MAXIM_CsiDisable(sMipiCsiDesc[i].mIdx);
    #else
      SONY_CsiDisable((CSI_IDX)sMipiCsiDesc[i].mIdx);
      SONY_Close((CSI_IDX)sMipiCsiDesc[i].mIdx);
  #endif
#endif // #ifndef CAM_OV10640      
  } // for all possible mipi interfaces
  
  // for all possible viu interfaces
  for(uint32_t i = VIU_IDX_0; i < VIU_IDX_INVALID; i++)
  {
    if(sViuDesc[i].mIdx != i)
    {
      continue;
    }
#ifndef __STANDALONE__    
    ioctl(sViuFd, VIULITE_IOCTL_DMA_STOP);
#endif // #ifndef __STANDALONE__
    
    OV10635_DataDisable(sViuDesc[i].mIdx);
  } // for all possible viu interfaces
  
  SEQ_Reset();  
} // FrameInputISP::Stop()


//******************************************************************************

FrameInputISP::~FrameInputISP()
{
  OV10635_Close(sViuDesc[0].mIdx);
  //*** Disable graph execution ***
  Stop();
  
  //*** Close HW ***
  if(HwClose() != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to close the HW\n");
  }
  //*** Graph clean up ***
  if( mpGraph != NULL)
  {
    delete(mpGraph);
  } // if graph exists
  
  //*** Reset this pointer ***
  mpThis = NULL;
} // FrameInputISP::~FrameInputISP()

//******************************************************************************

void * FrameInputISP::GetFrame()
{
  return GetFrame(0);
} // GetFrame()

//******************************************************************************

void * FrameInputISP::GetFrame(uint32_t aIdx)
{
  void *lRet = NULL;
  uint32_t lIter = 0;
  while((lRet = GetFrameNonBlock(aIdx)) == NULL)
  {
    volatile uint32_t lCnt = 0;
    while(lCnt < 100000)
    {
      lCnt++;
    }
    
    lIter++;
    
    if (__frame_in_timeout_enable)
    {
      if(lIter > 1000)
      {
        break;
      } // if max allowed iter reached
    }
  } // while GetFrame() failed
  
  return lRet;
} // FrameInputISP::GetFrame()

//******************************************************************************

void * FrameInputISP::GetFrameNonBlock(uint32_t aIdx)
{
  void *lpRet = NULL;
  
  if(mpStreams[aIdx].mCnt == 1)
  {
    lpRet = (void*)(uintptr_t)mpStreams[aIdx].mpBuffers[0];
  }// if only one frame
  else
  {
    // push last frame if any
    if(mpPoppedBufferInfo[aIdx].mBufferIdx != 0xffffffff)
    {
      SEQ_FrmBufferPush(&mpPoppedBufferInfo[aIdx]);
      mpPoppedBufferInfo[aIdx].mBufferIdx = 0xffffffff;
    };
    
    SEQ_FrmBufferInfo_t lBufferInfo;
    
    lBufferInfo.mStreamIdx = aIdx;
    // try to pop new frame
    if(SEQ_FrmBufferPop(&lBufferInfo) == SEQ_LIB_SUCCESS)
    {    
      // remember newly popped frame
      mpPoppedBufferInfo[aIdx] = lBufferInfo;
      
      lpRet = (void*)(uintptr_t)mpStreams[aIdx].
              mpBuffers[mpPoppedBufferInfo[aIdx].mBufferIdx];
    } // if pop succeeded
  } // else from if only one frame
    
  return lpRet;
} // FrameInputISP::GetFrame(uint32_t aIdx)

//******************************************************************************

void FrameInputISP::PutFrame(uint32_t aIdx)
{
  
  if(mpStreams[aIdx].mCnt != 1)
  {
    // push last frame if any
    if(mpPoppedBufferInfo[aIdx].mBufferIdx != 0xffffffff)
    {
      SEQ_FrmBufferPush(&mpPoppedBufferInfo[aIdx]);
      //SEQ_FrmBufferPush(&mpPoppedBufferInfo[(aIdx+1)%2]);
      mpPoppedBufferInfo[aIdx].mBufferIdx = 0xffffffff;
      //mpPoppedBufferInfo[(aIdx+1)%2].mBufferIdx = 0xffffffff;
    };
  } // else from if only one frame
} // FrameInputISP::PutFrame(uint32_t aIdx)

//******************************************************************************

IO_STATUS FrameInputISP::GetStatus()
{
  return mStatus;
} // FrameInputISP::GetStatus()

//******************************************************************************

uint32_t FrameInputISP::GetWidth()
{
  return(mImgWidth);

} // FrameInputISP::GetStatus()

//******************************************************************************

uint32_t FrameInputISP::GetHeight()
{
  return(mImgHeight);

} // FrameInputISP::GetHeight()

//******************************************************************************

IO_DATA_DEPTH FrameInputISP::GetDepth()
{
  return IO_DATA_DEPTH_08;

} // FrameInputISP::GetDepth()

//******************************************************************************

IO_DATA_CHANNELS FrameInputISP::GetChannels()
{
  return IO_DATA_CH3;
} // FrameInputISP::GetChannels()

//******************************************************************************

SEQ_Buf_t** FrameInputISP::GraphFetch(SEQ_Head_Ptr_t** appGraph)
{
  SEQ_Buf_t **lppBufferList = NULL;
  mpGraph->Fetch(appGraph, &lppBufferList);
  
  return lppBufferList;
} // FrameInputISP::GetChannels()

//******************************************************************************

void FrameInputISP::SeqEventHandler(uint32_t aEventType)
{
  switch(aEventType)
  {
    case SEQ_MSG_TYPE_RAB:  
    {
      printf("RAB reached user application.\n");
    } // case SEQ_MSG_TYPE_FRAMEDONE
      break;
    case SEQ_MSG_TYPE_FRAMEDONE:	
    {
      mpThis->FrameDoneHandler();
    } // case SEQ_MSG_TYPE_FRAMEDONE
      break;
    case SEQ_MSG_TYPE_OTHERERROR: 
    {
      char lString[SEQ_PRAM_AUX_DATA_SIZE] = {0};
      
      printf("*** Sequencer-Error:");
      
      if(SEQ_MsgAuxDataGet(lString, SEQ_PRAM_AUX_DATA_SIZE) != SEQ_LIB_SUCCESS)
      {
        VDB_LOG_NOTE("Failed to get the auxiliary data from PRAM.\n");
      }
      printf("Other error message received:\n%s", lString);
    } // case SEQ_MSG_TYPE_OTHERERROR
      break;
  } // switch(aEventType)
} // FrameInputISP::SeqEventHandler(uint32_t aEventType)

//******************************************************************************

void FrameInputISP::FrameDoneHandler(void)
{
  //*** Remember there is a new frame ***
  mFrmCnt++;
  // Some stop condition?  
} // FrameInputISP::FrameDoneHandler(void)   

//******************************************************************************

void FrameInputISP::AtomicTest(void)
{
  int32_t lReady = 1;
  int32_t lPopped = 2;
#ifdef __ARMV8
  lReady = lPopped;
  lPopped = lReady;
  lReady  = -1;
#else  
  FramePopAtomic(&lReady, &lPopped);
#endif // #ifdef __ARMV8
} // FrameInputISP::FrameDoneHandler(void)   

//******************************************************************************

} // namespace io

#ifndef __ARMV8  
static void FramePopAtomic(int32_t *apReady, int32_t *apPopped)
{
  asm(
      "    MOV      R4, #-1           \n\t" // set -1 to R4
      "try:                           \n\t" 
      "    LDREX    R3, [%r0]         \n\t" // load exclusively R3 with value at apReady address
      "    STR      R3, [%r1]         \n\t" // store the R3 to apPopped address
      "    STREX    R6, R4, [%r0]     \n\t" // try to store the -1 from R4 to apReady address
      "    CMP      R6, #0            \n\t" // if R6 is 0 the LDR/STR pair was exclusive
      "    BNE      try"                    // if not exclusive, repeat from try
      :
      :[ReadyAddr]"r" (apReady), [PopAddr]"r" (apPopped)
      : "r0", "r1", "r2", "r3", "r4", "r5", "memory"
      ); // asm()
} // FramePopAtomic(int32_t *apReady, int32_t *apPopped)
#endif // ifdef __ARMV8

#if 0
static void FramePopAtomic64(int32_t *apReady, int32_t *apPopped)
{
  asm (
      "    MOV     W4, #-1         \n\t" // set -1 to R4
      "try:                        \n\t" 
      "    LDXR    W3, [%x0]       \n\t" // load exclusively R3 with value at apReady address
      "    STR     W3, [%x1]       \n\t" // store the R3 to apPopped address
      "    STXR    W6, W4, [%x0]   \n\t" // try to store the -1 from R4 to apReady address
      "    CMP     W6, #0          \n\t" // if R6 is 0 the LDR/STR pair was exclusive
      "    BNE     try"                  // if not exclusive, repeat from try
      :
      : [ReadyAddr]"r" (apReady), [PopAddr]"r" (apPopped)
      : "x0", "x1", "w3", "w4", "w6", "memory"
  );
} // FramePopAtomic(int32_t *apReady, int32_t *apPopped)
#endif

//****************************************************************************

LIB_RESULT HwOpen()
{
  LIB_RESULT lRet = LIB_SUCCESS;
  if(SramOpen() != SRAM_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to open SRAM device.\n");
    lRet = LIB_FAILURE;
  } // if SramOpen() failed

  if(FdmaOpen() != FDMA_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to open FDMA device.\n");
    lRet = LIB_FAILURE;
  } // if FdmaOpen() failed
  
  if(SEQ_Reserve() != SEQ_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to reserve Sequencer device.\n");
    lRet = LIB_FAILURE;
  } // if SEQ_Reserve() failed
  
  if(CSI_Open() != CSI_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to Open CSI device.\n");
    lRet = LIB_FAILURE;
  } // if CSI_Open() failed

#ifndef __STANDALONE__  
  sViuFd = open("/dev/fsl_viulite0", 0);
  if (sViuFd < 0) 
  {
    VDB_LOG_ERROR("Can't open device file: %s\n", "viu");
    lRet = LIB_FAILURE;
  } // if open failed
  else 
  {
    VDB_LOG_WARNING("Device file: %s opened successfully.\n", "viu");
  } // else from if not opened yet
#endif // #ifndef __STANDALONE__
  
  return lRet;
  
} // HwOpen()

//****************************************************************************

static LIB_RESULT HwClose()
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if(CSI_Close() != CSI_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to close CSI device.\n");
    lRet = LIB_FAILURE;
  } // if CSI_Open() failed
  
  if(SEQ_Release() != SEQ_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to release Sequencer device.\n");
    lRet = LIB_FAILURE;
  } // if SramClose() failed
  
  if(FdmaClose() != FDMA_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to close FDMA device.\n");
    lRet = LIB_FAILURE;
  } // if FdmaClose() failed

  if(SramClose() != SRAM_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to close SRAM device.\n");
    lRet = LIB_FAILURE;
  } // if SramClose() failed

  return lRet;
} // HwFdClose()

//****************************************************************************

void SeqStart(uint32_t aFrameCnt, uint32_t aInputLines)
{
  SEQ_Reset();
  
  SEQ_Boot();
  
  SEQ_GraphStart(aFrameCnt, aInputLines);
  
  return;
} // SeqStart()

//****************************************************************************


void SeqStop(void)
{
  //SONY_CsiDisable(sMipiCsiDesc.mIdx);
  
  SEQ_Reset();
} // SeqStop(void)

//****************************************************************************

#ifdef __STANDALONE__
static void CSI_IrqHandler(void)
{ 
  // log event
#if (ISP_EVT_LOG_LEN > 0)
  volatile SEQ_EventRecord_t *lpEvent = &(spEventLog->mpRecords[spEventLog->mNextRecIdx++]);
  if(spEventLog->mNextRecIdx >= (ISP_EVT_LOG_LEN - 1)) 
  {
    spEventLog->mNextRecIdx = ISP_EVT_LOG_LEN - 1;
  }
  lpEvent->mEvent.R = (ISP_LOGEVT_CSI_FE << 26);
  lpEvent->mTime  = get_uptime_microS();
#endif // #if (ISP_EVT_LOG_LEN > 0)

#if 0	
  // send VSYNC message
  msg.mType = SEQ_MSG_TYPE_CSI_VSYNC;
  msg.mStatus = SEQ_MSG_STAT_READY;
  
#endif
} // CSI_IrqHandler()

#endif // #ifdef __STANDALONE__

//****************************************************************************

void IspHwSramAccesAllow(void)
{
  *(uint32_t*)(0x40006000) = 0x00000000;
  *(uint32_t*)(0x40006004) = 0xffffffff;
  *(uint32_t*)(0x4000600c) = 0x80000000;

  *(uint32_t*)(0x40006020) = 0x00000000;
  *(uint32_t*)(0x40006024) = 0xffffffff;
  *(uint32_t*)(0x4000602c) = 0x80000000;

  *(uint32_t*)(0x40006040) = 0x00000000;
  *(uint32_t*)(0x40006044) = 0xffffffff;
  *(uint32_t*)(0x4000604c) = 0x80000000;

  *(uint32_t*)(0x40006060) = 0x00000000;
  *(uint32_t*)(0x40006064) = 0xffffffff;
  *(uint32_t*)(0x4000606c) = 0x80000000;

  *(uint32_t*)(0x40006080) = 0x00000000;
  *(uint32_t*)(0x40006084) = 0xffffffff;
  *(uint32_t*)(0x4000608c) = 0x80000000;

  *(uint32_t*)(0x400060a0) = 0x00000000;
  *(uint32_t*)(0x400060a4) = 0xffffffff;
  *(uint32_t*)(0x400060ac) = 0x80000000;

  *(uint32_t*)(0x400060c0) = 0x00000000;
  *(uint32_t*)(0x400060c4) = 0xffffffff;
  *(uint32_t*)(0x400060cc) = 0x80000000;

  *(uint32_t*)(0x400060e0) = 0x00000000;
  *(uint32_t*)(0x400060e4) = 0xffffffff;
  *(uint32_t*)(0x400060ec) = 0x80000000;

  *(uint32_t*)(0x40006100) = 0x00000000;
  *(uint32_t*)(0x40006104) = 0xffffffff;
  *(uint32_t*)(0x4000610c) = 0x80000000;

  *(uint32_t*)(0x40006120) = 0x00000000;
  *(uint32_t*)(0x40006124) = 0xffffffff;
  *(uint32_t*)(0x4000612c) = 0x80000000;

  *(uint32_t*)(0x40006140) = 0x00000000;
  *(uint32_t*)(0x40006144) = 0xffffffff;
  *(uint32_t*)(0x4000614c) = 0x80000000;

  *(uint32_t*)(0x40006160) = 0x00000000;
  *(uint32_t*)(0x40006164) = 0xffffffff;
  *(uint32_t*)(0x4000616c) = 0x80000000;

  *(uint32_t*)(0x40006180) = 0x00000000;
  *(uint32_t*)(0x40006184) = 0xffffffff;
  *(uint32_t*)(0x4000618c) = 0x80000000;

  *(uint32_t*)(0x400061a0) = 0x00000000;
  *(uint32_t*)(0x400061a4) = 0xffffffff;
  *(uint32_t*)(0x400061ac) = 0x80000000;

  *(uint32_t*)(0x400061c0) = 0x00000000;
  *(uint32_t*)(0x400061c4) = 0xffffffff;
  *(uint32_t*)(0x400061cc) = 0x80000000;

  *(uint32_t*)(0x400061e0) = 0x00000000;
  *(uint32_t*)(0x400061e4) = 0xffffffff;
  *(uint32_t*)(0x400061ec) = 0x80000000;

  *(uint32_t*)(0x40006200) = 0x00000000;
  *(uint32_t*)(0x40006204) = 0xffffffff;
  *(uint32_t*)(0x4000620c) = 0x80000000;

  *(uint32_t*)(0x40006220) = 0x00000000;
  *(uint32_t*)(0x40006224) = 0xffffffff;
  *(uint32_t*)(0x4000622c) = 0x80000000;

  *(uint32_t*)(0x40006240) = 0x00000000;
  *(uint32_t*)(0x40006244) = 0xffffffff;
  *(uint32_t*)(0x4000624c) = 0x80000000;

  *(uint32_t*)(0x40006260) = 0x00000000;
  *(uint32_t*)(0x40006264) = 0xffffffff;
  *(uint32_t*)(0x4000626c) = 0x80000000;
} // IspHwSramAccesAllow()

//****************************************************************************

/*EOF*/

