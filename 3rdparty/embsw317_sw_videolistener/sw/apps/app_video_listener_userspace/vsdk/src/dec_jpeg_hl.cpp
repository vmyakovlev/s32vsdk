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

#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <sys/time.h>
#include <fcntl.h>      /* open */ 
#include <unistd.h>     /* exit */
#include <sys/ioctl.h>  /* ioctl */

#include "frame_output_v234fb.h"
#include "sdi.hpp"      /* grabber */
#ifdef SINGLECAM
    #include "jpeg_single_stream_c.h"
#else
    #include "jpeg_4stream_c.h"
#endif

#include "dec_jpeg_hl.h"
#include "isp_jpegdec.h"
#include "jpegdcd_linux.h"


#define JPEGD_DRV       "/dev/jpegdcd"
#define CHNL_CNT        io::IO_DATA_CH3
#define DDR_BUFFER_CNT  3    ///< number of DDR buffers per ISP stream

static io::FrameOutputV234Fb lDcuOutput;


static int dec_jpeg_prepare_4channels(sdi_grabber *lpGrabber, int Width, int Height);
static int dec_jpeg_app_body_4channels(sdi_grabber *lpGrabber);

/** Function that gets JPEG decoder base address from Linux driver
* \param  fd_jpegd
* \param  pDecoderBaseAddr
*/
int dec_jpeg_get_dec_base_addr(unsigned long *pDecoderBaseAddr)
{
    int ret, fd_jpegd;

    /*  Open the JPEG decoder */
    fd_jpegd = open(JPEGD_DRV, 0);
    if (fd_jpegd < 0)
    {
        printf("[dec_jpeg_hl] Unable to open device %s: %s\n", JPEGD_DRV, strerror(errno));
        return -1;
    }
    
    /*  Get JPEG decoder base address to be passed to the sm_app */
    /*if ((ret = ioctl(fd_jpegd, JPEGDCD_IOCTL_BASEADDRESS_GET, pDecoderBaseAddr)))
    {
        printf("[dec_jpeg_hl] JPEGDCD_IOCTL_BASEADDRESS_GET failed: %s\n", strerror(errno));
        return -1;
    }*/
    
    *pDecoderBaseAddr = (unsigned long)0x400D4000;
    
    close(fd_jpegd);
    return 0;
}

int dec_jpeg_prepare_ios(sdi_grabber *lpGrabber, unsigned int eVariant, int Width, int Height)
{
    if (DEC_JPEG_4_CHANNELS == eVariant)
    {
        return dec_jpeg_prepare_4channels(lpGrabber, Width, Height);
    }
    else
    {
        printf("[dec_jpeg_hl] Unknown application variant\n");
        return -1;
    }
    
    return 0;
}

int dec_jpeg_process_frame(sdi_grabber *lpGrabber, unsigned int eVariant)
{
    if (DEC_JPEG_4_CHANNELS == eVariant)
    {
        return dec_jpeg_app_body_4channels(lpGrabber);
    }
    else
    {
        printf("[dec_jpeg_hl] Unknown application variant\n");
        return -1;
    }
    
    return 0;
}

bool dec_jpeg_is_error(void)
{
    JPEGDCD_CFGERR lJpegCfgErr;
    lJpegCfgErr.jpegerr_count = 9;
    
    if (JPEGDEC_LIB_SUCCESS != JPEGDEC_CfgErrGet(lJpegCfgErr))
    {
        printf("[dec_jpeg_hl] Unable to get decoder error status\n");
    }
    
    if (lJpegCfgErr.jpegerr_count > 0)
    {
        printf("[dec_jpeg_hl] Config error status %u and byte 0x%x\n", lJpegCfgErr.jpegerr_count, lJpegCfgErr.jpegerr_byte);
        return true;
    }
    
    return false;
}

static int dec_jpeg_app_body_4channels(sdi_grabber *lpGrabber)
{
    // fetched frame buffer storage
    static SDI_Frame lFrame[4];
    
    lFrame[0] = lpGrabber->FramePop(FDMA_IX_UYVY_0);
    lFrame[1] = lpGrabber->FramePop(FDMA_IX_UYVY_1);
    lFrame[2] = lpGrabber->FramePop(FDMA_IX_UYVY_2);
    lFrame[3] = lpGrabber->FramePop(FDMA_IX_UYVY_3);
    
    if(lFrame[0].mUMat.empty()) {
        printf("[dec_jpeg_hl] Frame pop FAILED\n");
        return -1;
    } // if pop failed

    lDcuOutput.PutFrame(lFrame[0].mUMat);

    if(lpGrabber->FramePush(lFrame[0]) != LIB_SUCCESS) {
        printf("[dec_jpeg_hl] 1 Frame push FAILED\n");
        return -2;
    } // if push failed
    if(lpGrabber->FramePush(lFrame[1]) != LIB_SUCCESS) {
        printf("[dec_jpeg_hl] 2 Frame push FAILED\n");
        return -3;
    } // if push failed
    if(lpGrabber->FramePush(lFrame[2]) != LIB_SUCCESS) {
        printf("[dec_jpeg_hl] 3 Frame push FAILED\n");
        return -4;
    } // if push failed
    if(lpGrabber->FramePush(lFrame[3]) != LIB_SUCCESS) {
        printf("[dec_jpeg_hl] 4 Frame push FAILED\n");
        return -5;
    } // if push failed
    
    
    return 0;
}

static int dec_jpeg_prepare_4channels(sdi_grabber *lpGrabber, int Width, int Height)
{
    /*  Instance-specific data */
    static sdi_FdmaIO *lpFdma;
    static SDI_DdrBufferArr lYuvColorBuffArr;
    static SDI_ImageDescriptor lFrmDesc;
    static SDI_DdrBufferArr lYuvColorRoi;
    
    //*** Init DCU Output ***
    lDcuOutput.Init(Width,
                    Height,
                    io::IO_DATA_DEPTH_08,
                    CHNL_CNT,
                    DCU_BPP_YCbCr422
                    );
    
    
    // *** prepare IOs ***
    lpFdma = (sdi_FdmaIO*)lpGrabber->IoGet(SEQ_OTHRIX_FDMA);
    if(NULL == lpFdma)
    {
        printf("[dec_jpeg_hl] lpGrabber->IoGet failed\n");
        return -1;
    }
    
    sdi_JpegDecIO *lpJpegDec = (sdi_JpegDecIO*)lpGrabber->IoGet(SEQ_OTHRIX_JPEGDEC);
    //lpJpegDec->StreamCntSet(4); /// Set number of streams (must be set to 4)
    SDI_JpegDecDescriptor lJpegDesc = lpJpegDec->ConfigGet();
    if(lJpegDesc.mpStreamDesc[0].mHeight == 0)
    {
      printf("Bad jpeg descriptor.\n");
      return -1;
    } // if no height
    lJpegDesc.mMode        = CONTEXT_SWITCH;
    lJpegDesc.mOutBufSmpl  = JPEGDCD_ON;
    lJpegDesc.mStreamCnt   = 4;
    lJpegDesc.mBankStride  = 0;
    lJpegDesc.mRstInterval = 16;
    
    for(uint32_t i = DATASTREAM_1; i <= DATASTREAM_4; i++)
    {
      lJpegDesc.mpStreamDesc[i].mPrecision     = PREC_8BITS;
      lJpegDesc.mpStreamDesc[i].mWrapperConfig = JPEGDCD_OFF;
      lJpegDesc.mpStreamDesc[i].mComponentCnt  = 3;

      // sampling factors
      lJpegDesc.mpStreamDesc[i].mC1_samplingfactor_h = 1;
      lJpegDesc.mpStreamDesc[i].mC1_samplingfactor_v = 1;
      lJpegDesc.mpStreamDesc[i].mC2_samplingfactor_h = 2;
      lJpegDesc.mpStreamDesc[i].mC2_samplingfactor_v = 2;
      lJpegDesc.mpStreamDesc[i].mC3_samplingfactor_h = 2;
      lJpegDesc.mpStreamDesc[i].mC3_samplingfactor_v = 2;
      lJpegDesc.mpStreamDesc[i].mC4_samplingfactor_h = 1;
      lJpegDesc.mpStreamDesc[i].mC4_samplingfactor_v = 1;    
    } // for all possible streams
    
    if(lpJpegDec->ConfigSet(lJpegDesc) != LIB_SUCCESS)
    {
      printf("Failed to configure decoding.\n");
    } // if config failed
    
    printf("[dec_jpeg_hl] Setting resolution %dx%d\n", Width, Height);
    
    // *** RGB full color buffer array ***
    lFrmDesc = SDI_ImageDescriptor
    (
      Width, 
      Height, 
      YUV422Stream_YUYV
    );
    
    lYuvColorBuffArr.Allocate(lFrmDesc, DDR_BUFFER_CNT); // allocate contiguous memory
    
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
    
    return 0;
}