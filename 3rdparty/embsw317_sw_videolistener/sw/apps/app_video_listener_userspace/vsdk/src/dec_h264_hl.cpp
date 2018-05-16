/*
  Copyright (c) 2016 NXP Semiconductors
  All Rights Reserved.
  
  This file contains sample code only.
  
  THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "video_app.h"
#include "video_app_cfg.h"

#include "frame_output_v234fb.h"
#include "sdi.hpp"      /* grabber */

#if (SINGLE_STREAM == APP_CFG_STREAM_VARIANT)
    #include "h264dec_single_stream_c.h"
#elif (FOUR_STREAMS == APP_CFG_STREAM_VARIANT)
    #include "h264dec_4stream_c.h"
#else
    #error Unsupported stream variant
#endif /* APP_CFG_STREAM_VARIANT */

#include "isp_sram.h"
#include "dec_h264_hl.h"

#define H264_ROW_DONE_IRQ   0x10
#define H264_IN_BUFF_SIZE   0x7D000
#define H264_IN_PACK_SIZE   0x300
#define H264_IN_STRM_FILE   "image.h264"
#define SLEEP_USECS         300

#define CHNL_CNT            io::IO_DATA_CH3
#define DDR_BUFFER_CNT      3    ///< number of DDR buffers per ISP stream

static char *lpH264FileName = (char *)(H264_IN_STRM_FILE);
static io::FrameOutputV234Fb lDcuOutput;
static bool sFrmDone = false;
static size_t gChunkSize = 0;
static uint8_t *lpSram;
static uintptr_t lSramInBuff = 0;

#if (SINGLE_STREAM == APP_CFG_STREAM_VARIANT)
static int dec_h264_prepare_static_input(sdi_grabber *lpGrabber, int Width, int Height);
static int dec_h264_prepare_single_stream_input(sdi_grabber *lpGrabber, int Width, int Height);
static int dec_h264_process_frame_static_input(sdi_grabber *lpGrabber);
static int dec_h264_process_frame_single_stream_input(sdi_grabber *lpGrabber);
#elif (FOUR_STREAMS == APP_CFG_STREAM_VARIANT)
static int dec_h264_prepare_four_static_inputs(sdi_grabber *lpGrabber, int Width, int Height);
static int dec_h264_process_four_static_inputs(sdi_grabber *lpGrabber);
static int dec_h264_prepare_four_stream_inputs(sdi_grabber *lpGrabber, int Width, int Height);
static int dec_h264_process_four_stream_inputs(sdi_grabber *lpGrabber);
#endif /* APP_CFG_STREAM_VARIANT */
static void SeqEventCallBack(uint32_t aEventType, void*);
static void H264DataInputSingleStream(uint32_t aSramInBuff);
static void H264DataInputFourStreams(uint32_t aSramInBuff);

int dec_h264_get_dec_base_addr(unsigned long *pDecoderBaseAddr)
{
    /*  TODO: Value should be retrieved from the decoder driver via ioctl... */
    *pDecoderBaseAddr = 0x400D8000U;
    
    return 0;
}

bool dec_h264_is_error(void)
{
    /*  TODO: Read and parse decoder's error status */
    printf("[dec_h264] dec_h264_is_error(): Not implemented yet\n");
    return false;
}

int dec_h264_shutdown(void)
{
    if (lpSram != NULL)
    {
        munmap(lpSram, SRAM_SIZE);
    }
    
    SramFree(lSramInBuff);
    
    return 0;
}

int dec_h264_prepare_ios(sdi_grabber *lpGrabber, unsigned int eVariant, int Width, int Height)
{
    if (DEC_H264_SINGLE_STATIC_INPUT == eVariant)
    {
#if (SINGLE_STREAM == APP_CFG_STREAM_VARIANT)
        return dec_h264_prepare_static_input(lpGrabber, Width, Height);
#else
        printf("[dec_h264] Not supported in this variant\n");
        return -1;
#endif /* APP_CFG_STREAM_VARIANT */
    }
    else if (DEC_H264_SINGLE_STREAM_INPUT == eVariant)
    {
#if (SINGLE_STREAM == APP_CFG_STREAM_VARIANT)
        return dec_h264_prepare_single_stream_input(lpGrabber, Width, Height);
#else
        printf("[dec_h264] Not supported in this variant\n");
        return -1;
#endif /* APP_CFG_STREAM_VARIANT */
    }
    else if (DEC_H264_FOUR_STATIC_INPUTS == eVariant)
    {
#if (FOUR_STREAMS == APP_CFG_STREAM_VARIANT)
        return dec_h264_prepare_four_static_inputs(lpGrabber, Width, Height);
#else
        printf("[dec_h264] Not supported in this variant\n");
        return -1;
#endif /* APP_CFG_STREAM_VARIANT */
    }
    else if (DEC_H264_FOUR_STREAM_INPUTS == eVariant)
    {
#if (FOUR_STREAMS == APP_CFG_STREAM_VARIANT)
        return dec_h264_prepare_four_stream_inputs(lpGrabber, Width, Height);
#else
        printf("[dec_h264] Not supported in this variant\n");
        return -1;
#endif /* APP_CFG_STREAM_VARIANT */
    }
    else
    {
        printf("[dec_h264_hl] Unknown application variant\n");
        return -1;
    }
    
    return 0;
}

int dec_h264_process_frame(sdi_grabber *lpGrabber, unsigned int eVariant)
{
    if (DEC_H264_SINGLE_STATIC_INPUT == eVariant)
    {
#if (SINGLE_STREAM == APP_CFG_STREAM_VARIANT)
        return dec_h264_process_frame_static_input(lpGrabber);
#else
        printf("[dec_h264] Only supported in SINGLE_STREAM variant\n");
        return -1;
#endif /* APP_CFG_STREAM_VARIANT */
    }
    else if (DEC_H264_SINGLE_STREAM_INPUT == eVariant)
    {
#if (SINGLE_STREAM == APP_CFG_STREAM_VARIANT)
        return dec_h264_process_frame_single_stream_input(lpGrabber);
#else
        printf("[dec_h264] Only supported in SINGLE_STREAM variant\n");
        return -1;
#endif /* APP_CFG_STREAM_VARIANT */
    }
    else if (DEC_H264_FOUR_STATIC_INPUTS == eVariant)
    {
#if (FOUR_STREAMS == APP_CFG_STREAM_VARIANT)
        return dec_h264_process_four_static_inputs(lpGrabber);
#else
        printf("[dec_h264] Only supported in FOUR_STREAMS variant\n");
        return -1;
#endif /* APP_CFG_STREAM_VARIANT */
    }
    else if (DEC_H264_FOUR_STREAM_INPUTS == eVariant)
    {
#if (FOUR_STREAMS == APP_CFG_STREAM_VARIANT)
        return dec_h264_process_four_stream_inputs(lpGrabber);
#else
        printf("[dec_h264] Only supported in FOUR_STREAMS variant\n");
        return -1;
#endif /* APP_CFG_STREAM_VARIANT */
    }
    else
    {
        printf("[dec_h264] Unsupported variant\n");
        return -1;
    }
}

#if (FOUR_STREAMS == APP_CFG_STREAM_VARIANT)
static int dec_h264_prepare_four_stream_inputs(sdi_grabber *lpGrabber, int Width, int Height)
{
    /*  Instance-specific data */
    static sdi_FdmaIO *lpFdma;
    static SDI_DdrBufferArr lYuvColorBuffArr;
    static SDI_ImageDescriptor lFrmDesc;
    static SDI_DdrBufferArr lYuvColorRoi;
    static FILE *lpFrb;
    static uint8_t *lpSramIn;
    int32_t lFd;
    
    /*
        Initialize the frame buffer output
    */
    lDcuOutput.Init(   Width,
                       Height, 
                       io::IO_DATA_DEPTH_08,
                       CHNL_CNT,
                       DCU_BPP_YCbCr422);
    
    // *** prepare IOs ***
    lpFdma = (sdi_FdmaIO*)lpGrabber->IoGet(SEQ_OTHRIX_FDMA);
    
    lFrmDesc = SDI_ImageDescriptor
    (
        Width, 
        Height, 
        YUV422Stream_YUYV
    );
    
    lYuvColorBuffArr.Allocate(lFrmDesc, DDR_BUFFER_CNT);
    
    // *** get quadrant 0 ***
    lYuvColorRoi  = lYuvColorBuffArr.Roi
    (
        0, 
        0,
        Width/2, 
        Height/2
    );
  
    lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_0);
    lpFdma->DdrBuffersSet(lYuvColorRoi);

    // *** get quadrant 1 ***
    lYuvColorRoi  = lYuvColorBuffArr.Roi
    (
        Width/2, 
        0,
        Width/2, 
        Height/2
    );
  
    lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_1);
    lpFdma->DdrBuffersSet(lYuvColorRoi);
  
    // *** get quadrant 2 ***
    lYuvColorRoi  = lYuvColorBuffArr.Roi
    ( 
        0, 
        Height/2,
        Width/2, 
        Height/2
    );
  
    lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_2);
    lpFdma->DdrBuffersSet(lYuvColorRoi);

    // *** get quadrant 3 ***
    lYuvColorRoi  = lYuvColorBuffArr.Roi
    (
        Width/2, 
        Height/2,
        Width/2, 
        Height/2
    );
  
    lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_3);
    lpFdma->DdrBuffersSet(lYuvColorRoi);
    
    /*
        Hook a sequencer event callback
    */
    lpGrabber->SeqEventCallBackInstall(&SeqEventCallBack, NULL);
    
    return 0;
}

static int dec_h264_process_four_stream_inputs(sdi_grabber *lpGrabber)
{
    /*  Fetched frame buffer storage */
    static SDI_Frame lpFrame[2];
    
    if (true == sFrmDone)   /*  TODO: Wait for message from sequencer. This should be removed when FramePop() will become blocking */
    {
        lpFrame[0] = lpGrabber->FramePop(FDMA_IX_UYVY_0);
        if (lpFrame[0].mUMat.empty())
        {
            printf("NULL pointer data.\n");
            return -1;
        }
        
        lpFrame[1] = lpGrabber->FramePop(FDMA_IX_UYVY_1);
        if (lpFrame[1].mUMat.empty())
        {
            printf("NULL pointer data.\n");
            return -1;
        }
          
        lDcuOutput.PutFrame(lpFrame[0].mUMat);
        
        /*  For each frame quadrant */
        for (uint32_t i = 0; i < 2; i++)
        {
            if (lpGrabber->FramePush(lpFrame[i]) != LIB_SUCCESS)
            {
                return -1;
            }
        }
        sFrmDone = false;
    }
    else
    {
        return -2;  /*  Try again */
    }
    
    return 0;
}

static int dec_h264_prepare_four_static_inputs(sdi_grabber *lpGrabber, int Width, int Height)
{
    /*  Instance-specific data */
    static sdi_FdmaIO *lpFdma;
    static SDI_DdrBufferArr lYuvColorBuffArr;
    static SDI_ImageDescriptor lFrmDesc;
    static SDI_DdrBufferArr lYuvColorRoi;
    static FILE *lpFrb;
    static uint8_t *lpSramIn;
    int32_t lFd;
    
    /*
        Initialize the frame buffer output
    */
    lDcuOutput.Init(   Width,
                       Height, 
                       io::IO_DATA_DEPTH_08,
                       CHNL_CNT,
                       DCU_BPP_YCbCr422);
    
    // *** prepare IOs ***
    lpFdma = (sdi_FdmaIO*)lpGrabber->IoGet(SEQ_OTHRIX_FDMA);
    
    lFrmDesc = SDI_ImageDescriptor
    (
        Width, 
        Height, 
        YUV422Stream_YUYV
    );
    
    lYuvColorBuffArr.Allocate(lFrmDesc, DDR_BUFFER_CNT);
    
    // *** get quadrant 0 ***
    lYuvColorRoi  = lYuvColorBuffArr.Roi
    (
        0, 
        0,
        Width/2, 
        Height/2
    );
  
    lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_0);
    lpFdma->DdrBuffersSet(lYuvColorRoi);

    // *** get quadrant 1 ***
    lYuvColorRoi  = lYuvColorBuffArr.Roi
    (
        Width/2, 
        0,
        Width/2, 
        Height/2
    );
  
    lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_1);
    lpFdma->DdrBuffersSet(lYuvColorRoi);
  
    // *** get quadrant 2 ***
    lYuvColorRoi  = lYuvColorBuffArr.Roi
    ( 
        0, 
        Height/2,
        Width/2, 
        Height/2
    );
  
    lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_2);
    lpFdma->DdrBuffersSet(lYuvColorRoi);

    // *** get quadrant 3 ***
    lYuvColorRoi  = lYuvColorBuffArr.Roi
    (
        Width/2, 
        Height/2,
        Width/2, 
        Height/2
    );
  
    lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_3);
    lpFdma->DdrBuffersSet(lYuvColorRoi);
    
    /*
        Allocate memory for the static input image data.
    */
    lSramInBuff = SramSingleMalloc(H264_IN_BUFF_SIZE);
    if (NULL == lSramInBuff)
    {
        printf("[dec_h264] SRAM allocation failed\n");
        return -1;
    }
    
    lFd = open("/dev/mem", O_RDWR, 0);
    lpSram = (uint8_t*)mmap(NULL,
                            SRAM_SIZE, 
                            PROT_READ | PROT_WRITE, 
                            MAP_SHARED, 
                            lFd, 
                            SRAM_BASE);
    close(lFd);
    lpSramIn = (uint8_t*)((lSramInBuff - SRAM_BASE) + (uintptr_t)lpSram);
    
    /*
        Read the static .h264 image
    */
    lpFrb = fopen(lpH264FileName, "rb");
    if(lpFrb == NULL)
    {
        printf("[dec_h264] Failed to open h264 test stream %s.\n", lpH264FileName);  
        return -1;
    }
    
    gChunkSize = fread(lpSramIn, 1, H264_IN_BUFF_SIZE, lpFrb);
    if (gChunkSize <= 0)
    {
        return -1;
    }
    
    /*
        Hook a sequencer event callback
    */
    lpGrabber->SeqEventCallBackInstall(&SeqEventCallBack, NULL);
    
    return 0;
}

static int dec_h264_process_four_static_inputs(sdi_grabber *lpGrabber)
{
    /*  Fetched frame buffer storage */
    static SDI_Frame lpFrame[2];
    static bool bFrameInserted = false;
    
    if (true == sFrmDone)
    {
        bFrameInserted = false;
        
        lpFrame[0] = lpGrabber->FramePop(FDMA_IX_UYVY_0);
        if (lpFrame[0].mUMat.empty())
        {
            printf("NULL pointer data.\n");
            return -1;
        }
        
        lpFrame[1] = lpGrabber->FramePop(FDMA_IX_UYVY_1);
        if (lpFrame[1].mUMat.empty())
        {
            printf("NULL pointer data.\n");
            return -1;
        }
          
        lDcuOutput.PutFrame(lpFrame[0].mUMat);
        
        /*  For each frame quadrant */
        for (uint32_t i = 0; i < 2; i++)
        {
            if (lpGrabber->FramePush(lpFrame[i]) != LIB_SUCCESS)
            {
                return -1;
            }
        }
        sFrmDone = false;
        
        /*  Insert next images */
        H264DataInputFourStreams(lSramInBuff);
        bFrameInserted = true;
    }
    else if (false == bFrameInserted)
    {
        /*  Insert initial image */
        H264DataInputFourStreams(lSramInBuff);
        /*  TODO: Currently the decoder does not detect EOF of the previous image. Only by insertion
                  of next image the frame is "properly"(?) finished... */
        H264DataInputFourStreams(lSramInBuff);
        bFrameInserted = true;
        return -2;
    }
    else
    {
        /*  Frame not processed yet */
        return -2;
    }
    
    return 0;
}

#endif /* APP_CFG_STREAM_VARIANT */

#if (SINGLE_STREAM == APP_CFG_STREAM_VARIANT)
static int dec_h264_prepare_single_stream_input(sdi_grabber *lpGrabber, int Width, int Height)
{
    /*  Instance-specific data */
    static sdi_FdmaIO *lpFdma;
    static SDI_DdrBufferArr lYuvColorBuffArr;
    static SDI_ImageDescriptor lFrmDesc;
    static SDI_DdrBufferArr lYuvColorRoi;
    
    /*
        Initialize the frame buffer output
    */
    lDcuOutput.Init(   Width,
                       Height, 
                       io::IO_DATA_DEPTH_08,
                       CHNL_CNT,
                       DCU_BPP_YCbCr422);
    
    // *** prepare IOs ***
    lpFdma = (sdi_FdmaIO*)lpGrabber->IoGet(SEQ_OTHRIX_FDMA);
    
    lFrmDesc = SDI_ImageDescriptor(Width, Height, RGB888);
    lpFdma->DdrBufferDescSet(FDMA_IX_GRAY_AS_RGB, lFrmDesc);
    
    //*** allocate GrayAsRGB DDR buffer ***
    lpFdma->DdrBuffersAlloc(FDMA_IX_GRAY_AS_RGB, DDR_BUFFER_CNT);
    
    // *** UYVY full color buffer array ***
    lFrmDesc = SDI_ImageDescriptor
    (
        Width * 2, 
        Height / 2, 
        YUV422Stream_YUYV
    );
    
    lYuvColorBuffArr.Allocate(lFrmDesc, DDR_BUFFER_CNT); // allocate contiguous memory
    
    // *** get quadrant 0 ***
    lYuvColorRoi  = lYuvColorBuffArr.Roi
    (
        0, 
        0,
        Width, 
        Height/2
    );    
    
    lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_0);
    lpFdma->DdrBuffersSet(lYuvColorRoi);
    
    // *** get quadrant 1 ***
    lYuvColorRoi  = lYuvColorBuffArr.Roi
    ( 
        Width, 
        0,
        Width, 
        Height/2
    );
    
    lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_1);
    lpFdma->DdrBuffersSet(lYuvColorRoi);
    
    lpGrabber->SeqEventCallBackInstall(&SeqEventCallBack, NULL);
    
    return 0;
}

static int dec_h264_process_frame_single_stream_input(sdi_grabber *lpGrabber)
{
    /*  Fetched frame buffer storage */
    static SDI_Frame lpFrame[2];
    
    if (true == sFrmDone)   /*  TODO: Wait for message from sequencer. This should be removed when FramePop() will become blocking */
    {
        lpFrame[0] = lpGrabber->FramePop(FDMA_IX_UYVY_0);
        if (lpFrame[0].mUMat.empty())
        {
            printf("NULL pointer data.\n");
            return -1;
        }
        
        lpFrame[1] = lpGrabber->FramePop(FDMA_IX_UYVY_1);
        if (lpFrame[1].mUMat.empty())
        {
            printf("NULL pointer data.\n");
            return -1;
        }
          
        lDcuOutput.PutFrame(lpFrame[0].mUMat);
        
        /*  For each frame quadrant */
        for (uint32_t i = 0; i < 2; i++)
        {
            if (lpGrabber->FramePush(lpFrame[i]) != LIB_SUCCESS)
            {
                return -1;
            }
        }
        
        sFrmDone = false;
    }
    else
    {
        return -2;  /*  Try again */
    }
    
    return 0;
}

static int dec_h264_prepare_static_input(sdi_grabber *lpGrabber, int Width, int Height)
{
    /*  Instance-specific data */
    static sdi_FdmaIO *lpFdma;
    static SDI_DdrBufferArr lYuvColorBuffArr;
    static SDI_ImageDescriptor lFrmDesc;
    static SDI_DdrBufferArr lYuvColorRoi;
    static FILE *lpFrb;
    static uint8_t *lpSramIn;
    int32_t lFd;
    
    /*
        Initialize the frame buffer output
    */
    lDcuOutput.Init(   Width,
                       Height, 
                       io::IO_DATA_DEPTH_08,
                       CHNL_CNT,
                       DCU_BPP_YCbCr422);
    
    // *** prepare IOs ***
    lpFdma = (sdi_FdmaIO*)lpGrabber->IoGet(SEQ_OTHRIX_FDMA);
    
    lFrmDesc = SDI_ImageDescriptor(Width, Height, RGB888);
    lpFdma->DdrBufferDescSet(FDMA_IX_GRAY_AS_RGB, lFrmDesc);
    
    //*** allocate GrayAsRGB DDR buffer ***
    lpFdma->DdrBuffersAlloc(FDMA_IX_GRAY_AS_RGB, DDR_BUFFER_CNT);
    
    // *** UYVY full color buffer array ***
    lFrmDesc = SDI_ImageDescriptor
    (
        Width * 2, 
        Height / 2, 
        YUV422Stream_YUYV
    );
    
    lYuvColorBuffArr.Allocate(lFrmDesc, DDR_BUFFER_CNT); // allocate contiguous memory
    
    // *** get quadrant 0 ***
    lYuvColorRoi  = lYuvColorBuffArr.Roi
    (
        0, 
        0,
        Width, 
        Height/2
    );    
    
    lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_0);
    lpFdma->DdrBuffersSet(lYuvColorRoi);
    
    // *** get quadrant 1 ***
    lYuvColorRoi  = lYuvColorBuffArr.Roi
    ( 
        Width, 
        0,
        Width, 
        Height/2
    );
    
    lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_1);
    lpFdma->DdrBuffersSet(lYuvColorRoi);
    
    /*
        Allocate memory for the static input image data.
    */
    lSramInBuff = SramSingleMalloc(H264_IN_BUFF_SIZE);
    if (NULL == lSramInBuff)
    {
        printf("[dec_h264] SRAM allocation failed\n");
        return -1;
    }
    
    lFd = open("/dev/mem", O_RDWR, 0);
    lpSram = (uint8_t*)mmap(NULL,
                            SRAM_SIZE, 
                            PROT_READ | PROT_WRITE, 
                            MAP_SHARED, 
                            lFd, 
                            SRAM_BASE);
    close(lFd);
    lpSramIn = (uint8_t*)((lSramInBuff - SRAM_BASE) + (uintptr_t)lpSram);

#if 0    
    if (H264DEC_LIB_SUCCESS != H264DEC_IrqMaskSet(H264_ROW_DONE_IRQ))
    {
        printf("[dec_h264] H264DEC_IrqMaskSet() failed\n");
        return -1;
    }
#endif /* 0 */
    
    /*
        Read the static .h264 image
    */
    lpFrb = fopen(lpH264FileName, "rb");
    if(lpFrb == NULL)
    {
        printf("[dec_h264] Failed to open h264 test stream %s.\n", lpH264FileName);  
        return -1;
    }
    
    gChunkSize = fread(lpSramIn, 1, H264_IN_BUFF_SIZE, lpFrb);
    if (gChunkSize <= 0)
    {
        return -1;
    }
    
    /*
        Hook a sequencer event callback
    */
    lpGrabber->SeqEventCallBackInstall(&SeqEventCallBack, NULL);

#define LM_DEBUG
#ifdef LM_DEBUG
    /*
        Set up decoder (should be done by VSDK...)
    */
    H264DCD_INFRAME_SIZE lFrmSize;
    lFrmSize.stream_no = H264DCD_DATASTREAM_1;
    lFrmSize.x_size    = 1280 >> 4;
    lFrmSize.y_size    = 960  >> 4;

    printf("[dec_h264] Setting h264 DEC input resolution: %dx%d\n", lFrmSize.x_size * 16U, lFrmSize.y_size * 16U);

    H264DEC_InFrameSizeSet(lFrmSize);
#endif /* LM_DEBUG */
    
    return 0;
}

static int dec_h264_process_frame_static_input(sdi_grabber *lpGrabber)
{
    /*  Fetched frame buffer storage */
    static SDI_Frame lpFrame[2];
    static bool bFrameInserted = false;
    
    if (true == sFrmDone)
    {
        bFrameInserted = false;
        
        lpFrame[0] = lpGrabber->FramePop(FDMA_IX_UYVY_0);
        if (lpFrame[0].mUMat.empty())
        {
            printf("NULL pointer data.\n");
            return -1;
        }
        
        lpFrame[1] = lpGrabber->FramePop(FDMA_IX_UYVY_1);
        if (lpFrame[1].mUMat.empty())
        {
            printf("NULL pointer data.\n");
            return -1;
        }
          
        lDcuOutput.PutFrame(lpFrame[0].mUMat);
        
        /*  For each frame quadrant */
        for (uint32_t i = 0; i < 2; i++)
        {
            if (lpGrabber->FramePush(lpFrame[i]) != LIB_SUCCESS)
            {
                return -1;
            }
        }
        sFrmDone = false;
        
        /*  Insert next image */
        H264DataInputSingleStream(lSramInBuff);
        bFrameInserted = true;
    }
    else if (false == bFrameInserted)
    {
        /*  Insert initial image */
        H264DataInputSingleStream(lSramInBuff);
        /*  TODO: Currently the decoder does not detect EOF of the previous image. Only by insertion
                  of next image the frame is "properly"(?) finished... */
        H264DataInputSingleStream(lSramInBuff);
        bFrameInserted = true;
        return -2;
    }
    else
    {
        /*  Frame not processed yet */
        return -2;
    }
    
    return 0;
}
#endif /* APP_CFG_STREAM_VARIANT */

static void SeqEventCallBack(uint32_t aEventType, void*)
{
    static uint32_t slFrmCnt = 0;
    
    if (aEventType == SEQ_MSG_TYPE_FRAMEDONE)
    {
        sFrmDone = true;
        /* printf("Frame done message arrived #%u.\n", slFrmCnt++); */
        /* usleep(1000); */
    }
  
    if (aEventType == SEQ_MSG_TYPE_OTHERERROR)
    {
        printf("[dec_h264] Callback: OTHERERROR\n");
    }
}

static void H264DataInputSingleStream(uint32_t aSramInBuff)
{
    size_t lStop = min(gChunkSize, (size_t)H264_IN_BUFF_SIZE);
  
    // *** start H264 decoding ***
    H264DCD_STREAM_DATA_CONFIG lStreamInCfg;
    lStreamInCfg.stream_no = H264DCD_DATASTREAM_1;
    lStreamInCfg.buff_addr = aSramInBuff;
    lStreamInCfg.buff_size = H264_IN_PACK_SIZE;
    lStreamInCfg.wr_delay  = 0;
  
    /*  For each packs in image */
    for (uint32_t i = 0; i < lStop/H264_IN_PACK_SIZE; i++)
    {
        if (H264DEC_LIB_SUCCESS != H264DEC_InConfig(lStreamInCfg))
        {
            printf("[dec_h264] SingleStream: H264DEC_InConfig FAILED #1\n");
        }
        
        usleep(SLEEP_USECS);
        lStreamInCfg.buff_addr += H264_IN_PACK_SIZE;
    }
    
    /*  If some leftover */
    if (lStop%H264_IN_PACK_SIZE != 0)
    {
        lStreamInCfg.buff_size = lStop%H264_IN_PACK_SIZE;
        usleep(SLEEP_USECS);
        
        if (H264DEC_LIB_SUCCESS != H264DEC_InConfig(lStreamInCfg))
        {
            printf("[dec_h264] SingleStream: H264DEC_InConfig FAILED #2\n");
        }
    }
    
    return;
}

static void H264DataInputFourStreams(uint32_t aSramInBuff)
{
    size_t lStop = min(gChunkSize, (size_t)H264_IN_BUFF_SIZE);
  
    // *** start H264 decoding ***
    H264DCD_STREAM_DATA_CONFIG lStreamInCfg;
    lStreamInCfg.stream_no = H264DCD_DATASTREAM_1;
    lStreamInCfg.buff_addr = aSramInBuff;
    lStreamInCfg.buff_size = H264_IN_PACK_SIZE;
    lStreamInCfg.wr_delay  = 0;
    
    /*  For each packs in image */
    for(uint32_t i = 0; i < lStop/H264_IN_PACK_SIZE; i++)
    {
        lStreamInCfg.stream_no = H264DCD_DATASTREAM_1;
        H264DEC_InConfig(lStreamInCfg);

        lStreamInCfg.stream_no = H264DCD_DATASTREAM_2;
        H264DEC_InConfig(lStreamInCfg);

        lStreamInCfg.stream_no = H264DCD_DATASTREAM_3;
        H264DEC_InConfig(lStreamInCfg);

        lStreamInCfg.stream_no = H264DCD_DATASTREAM_4;
        H264DEC_InConfig(lStreamInCfg);
        
        usleep(SLEEP_USECS);

        lStreamInCfg.buff_addr += H264_IN_PACK_SIZE;
    }
    
    /*  If some leftover */
    if(lStop%H264_IN_PACK_SIZE != 0)
    {
        lStreamInCfg.buff_size = lStop%H264_IN_PACK_SIZE;
        lStreamInCfg.stream_no = H264DCD_DATASTREAM_1;
        H264DEC_InConfig(lStreamInCfg);

        lStreamInCfg.stream_no = H264DCD_DATASTREAM_2;
        H264DEC_InConfig(lStreamInCfg);

        lStreamInCfg.stream_no = H264DCD_DATASTREAM_3;
        H264DEC_InConfig(lStreamInCfg);

        lStreamInCfg.stream_no = H264DCD_DATASTREAM_4;
        H264DEC_InConfig(lStreamInCfg);    
    }
    
    return;
}
