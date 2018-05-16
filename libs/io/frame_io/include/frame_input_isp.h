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

#ifndef FRAMEINPUTISP_H
#define FRAMEINPUTISP_H

#include <stdio.h>

#include "frame_input_base.h"

#include "isp_fdma.h"
#include "isp_sram.h"
#include "isp_csi.h"
#include "isp_cam_generic.h"
#include "isp_cam_sony.h"
#include "../../sdi/include/sdi_graph.hpp"
#include "seq_graph.h"
#include "isp_seq.h"

#define ISP_FRAME_BUFFER_CNT 3

//***************************************************************************

typedef int32_t (*IspUserInitFunc_t)(void*);

//***************************************************************************

namespace io
{
//***************************************************************************

 /** Describes one ISP buffer stream.
 *
 ***************************************************************************/
  struct IspBufferStream
  {
    uint32_t    mStreamIdx;  ///< index of a frame stream
    uint32_t    mCnt;        ///< number of requested buffers
    uint32_t    mBaseShift;  ///< shift from base address to valid pixel data
    uint32_t    mLineStride; ///< shift to next line in bytes
    uint32_t    mLineCnt;    ///< number of lines in one buffer
    
    uint32_t * mpBuffers; ///< pointer to array of HW addresses in uint32_t fromat
  };
/***************************************************************************/
/** Encapsulates ISP Sequencer operation used as camera frame source.
 *
 ***************************************************************************/
  class FrameInputISP : public FrameInputBase
  {
    public:
      /************************************************************************/
      /** Constructor. 
       *
       ************************************************************************/
      FrameInputISP(bool aInitMIPI=true);
      
      /************************************************************************/
      /** Destructor. 
       *
       * Resets HW. Graph related clean-up.
       ************************************************************************/
      ~FrameInputISP(void);
      
      /************************************************************************/
      /** Initializes Vision subsystem HW (FW download, reset).
       *
       * Reprocesses and downloads the graph.
       * Invokes contiguous graph execution.
       *
       * \param apUserInitFunc user defined init function to be called after 
       *          Sequencer booted up - before the MIPI was enabled.
       * \param apParam user init function parameter if any 
       ************************************************************************/
      void IspInit(IspUserInitFunc_t apUserInitFunc, 
                   void *apParam, 
                   bool aInitMIPI=true);
      
      /************************************************************************/
      /** Initializes all MipiCsi interfaces used in the ISP graph.
       * 
       * Includes also connected camera I2C config.
       *
       ************************************************************************/
      void MipiCsiInit(void);
      
      /************************************************************************/
      /** Initializes all Viu interfaces used in the ISP graph.
       * 
       * Includes also connected camera I2C config.
       *
       ************************************************************************/
      void ViuInit(void);
      
      /************************************************************************/
      /** Initializes JpegDecoder interface used in the ISP graph.
       * 
       ************************************************************************/
      void JpegDecInit(void);

      /************************************************************************/
      /** Makes the latest grabbed frame available to user application. 
       *
       * If no frame available yet - blocks until first frame received.
       * 
       * \return pointer to frame data base address
       *         NULL if error detected
       ************************************************************************/
      void *GetFrame(void);
      
      /************************************************************************/
      /** Makes the latest grabbed frame available to user application. 
       *
       * If no frame available yet - blocks until first frame received.
       * 
       * \param aIdx index of the ISP stream
       * 
       * \return pointer to frame data base address
       *         NULL if error detected
       ************************************************************************/
      void *GetFrame(uint32_t aIdx);
      
      /************************************************************************/
      /** Makes the latest grabbed frame available to user application. 
       *
       * Nonblocking call => retruns NULL if there is no frame available.
       * 
       * \param  aIdx stream index to fetch the frame from
       * 
       * \return pointer to frame data base address
       *         NULL if error detected
       ************************************************************************/
      void *GetFrameNonBlock(uint32_t aIdx);
      
      /************************************************************************/
      /** Releases the last grabbed frame for specified ISP stream. 
       * 
       * \param aIdx index of the ISP stream
       ************************************************************************/
      void PutFrame(uint32_t aIdx);
      
      /************************************************************************/
      /** Informs about the current status of the frame input object.
       *
       * \return width of the frame in pixels
       ************************************************************************/
      IO_STATUS GetStatus(void); 
      
      /************************************************************************/
      /** Provides width of the frame input in pixels.
       *
       * \return width of the frame in pixels
       ************************************************************************/
      uint32_t GetWidth(void);
      
      /************************************************************************/
      /** Provides height of the frame input in pixels.
       *
       * \return height of the frame in pixels
       ************************************************************************/
      uint32_t GetHeight(void);
      
      /************************************************************************/
      /** Provides depth of frame component channel in bits.
       *
       * \return depth of frame component channel in bits
       ************************************************************************/
      IO_DATA_DEPTH GetDepth(void);
      
      /************************************************************************/
      /** Provides number of image component channels.
       *
       * \return number of image component channels
       ************************************************************************/
      IO_DATA_CHANNELS GetChannels(void);
      
      SEQ_Buf_t** GraphFetch(SEQ_Head_Ptr_t** appGraph);
      
      /************************************************************************/
      /** Sets DDR buffers.
       *
       * Setups FDMA channels and Sequencer streams internally.
       * 
       ************************************************************************/
      int32_t DdrBuffersSet(const IspBufferStream &arcBufferStream);
      
      /************************************************************************/
      /** Cleans buffer address array for one stream.
       *
       * Setups FDMA channels and Sequencer streams internally.
       * 
       ************************************************************************/
      void StreamClean(uint32_t aIdx);
      
      /************************************************************************/
      /** Stats isp streaming.
       * 
       ************************************************************************/
      void Start(uint32_t aFrameCnt=0, uint32_t aInputLines=0);
      
      /************************************************************************/
      /** Stops isp streaming.
       * 
       ************************************************************************/
      void Stop();
      
      /************************************************************************/
      /** Starts the camera at specified CSI interface if used in the graph.
       * 
       ************************************************************************/
      void StartCam(enum CSI_IDX aCsiIdx);
      
      /************************************************************************/
      /** Starts the camera at specified VIU interface if used in the graph.
       * 
       ************************************************************************/
      void StartCam(enum VIU_IDX aViuIdx);
      
      /************************************************************************/
      /** Starts all cameras used by the graph.
       * 
       ************************************************************************/
      void StartCam();
      
      
      void AtomicTest(void);
      sdi_graph*	GetGraph(void)	{ return(mpGraph); }
      uint32_t          mFrmCnt; ///< number of done frames
      
    private:
      uint8_t**         mppFrameBuffers; ///< buffer pointers
      //void**            mppFbsVirtual; ///< virtual buffer pointers
      //uint32_t*         mpFbsPhysical; ///< physical buffer adresses
      IspBufferStream   mpStreams[FDMA_CHNL_CNT];
      uint32_t          mLastCnt;///< last popped frame cnt
      IO_STATUS         mStatus; ///< input object status
      sdi_graph         *mpGraph;///< graph representation
      uint16_t          mImgWidth;  ///< images width
      uint16_t          mImgHeight; ///< images height
      uint16_t          mImgDepth;  ///< images pixel depth
      uint16_t          mBufWidth;  ///< frame buffers width
      uint16_t          mBufHeight; ///< frame buffers height
      
      static class FrameInputISP *mpThis; ///< pointer to the current instance
      SEQ_FrmBufferInfo_t mpPoppedBufferInfo[FDMA_CHNL_CNT];
      
      /************************************************************************/
      /** Sequencer event handler.
       *
       * Can run in IRQ context.
       ************************************************************************/
      static void SeqEventHandler(uint32_t aEventType);
      
      /************************************************************************/
      /** FrameDone event handler.
       *
       * Can run in IRQ context.
       ************************************************************************/
      void FrameDoneHandler(void);      
  };
}


#endif /* FRAMEINPUTISP_H */