/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor;
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
#include <stddef.h>
#include <string.h>
#include <config.h>
#include <frame_output_dcu.h>
#include "oal.h"
#include "vdb_log.h"
#include "s32vs234.h"

extern "C"
{
  void flush_dcache_range(void *, unsigned int);
  unsigned long get_uptime_microS(void);
}

static Dcu_Unit_t sDCU = DCU0;
	
/***************************************************************************/
/** Initializes the pin configuration. 
  *
  * 
  * \return pointer to frame data base address
  *         NULL if error detected
  ***************************************************************************/
static void DCU_pin_init(void); 


namespace io
{

//***************************************************************************

FrameOutputDCU::FrameOutputDCU()
{
  BaseInit();
} // FrameOutputDCU::FrameOutputDCU()

//***************************************************************************

FrameOutputDCU::FrameOutputDCU(uint32_t aWidth, 
                               uint32_t aHeight, 
                               IO_DATA_DEPTH aDepth, 
                               IO_DATA_CHANNELS aChannels, 
                               Dcu_BPP_t aFormat)
{
  BaseInit();
  this->Init(aWidth, aHeight, aDepth, aChannels, aFormat);
} // FrameOutputDCU::FrameOutputDCU()

//***************************************************************************

void FrameOutputDCU::BaseInit()
{
  // *** Configure pins ***
  DCU_pin_init();
  this->Reset();
  
  mSize     = 0;
  mWidth    = 0;
  mHeight   = 0;
  mDepth    = IO_DATA_DEPTH_NOT_INITIALIZED; 
  mChannels = IO_DATA_CH3;
  mNextBuff = 0;
  mStatus   = IO_ERROR;
  
  for(size_t i=0; i<DCU_FRAME_BUFFER_CNT; i++)
  {
    mppFrameBuffers[i]	= NULL;
    mppFbsVirtual[i]	= NULL;
    mpFbsPhysical[i]	= NULL;
  }

} // FrameOutputDCU::BaseInit()

//***************************************************************************

void FrameOutputDCU::Init(uint32_t aWidth, 
                          uint32_t aHeight, 
                          IO_DATA_DEPTH aDepth, 
                          IO_DATA_CHANNELS aChannels, 
                          Dcu_BPP_t aFormat)
{  
  mWidth    = aWidth;
  mHeight   = aHeight;
  mDepth    = aDepth; 
  mChannels = aChannels;
  mNextBuff = 0;
  mStatus   = IO_ERROR;

  this->Disable();

  // ** initDcuCLUT() **
  // loading grey values in color lookup-table
  if(aFormat == DCU_BPP_8)
  {
    unsigned long tStart, tEnd;

    tStart = get_uptime_microS();
    
    do
    {
      tEnd = get_uptime_microS();
    }
    while(50000 > (tEnd-tStart));

    for(uint32_t i=0; i<256; i++)
    {
      DCU_CLUTSet(DCU0, i, 0xff000000 | (i << 16) | (i << 8) | i);
    }
  } // if format DCU_BPP_8
  
  // *** Dcu_LCD_Para_t setup based on the datasheet ***

// *** define in config.h ***
//#define SAMSUNG 1
//#define PC_MONITOR_1280X1024_60HZ

#ifdef SAMSUNG
  mLcdPara.mDeltaX    = mWidth;   /**< delta X */

  mLcdPara.mDeltaY    = mHeight;  /**< delta Y */
  //mHorzBP+mHorzPW+mHorzFP =2200
  mLcdPara.mHorzBP    = 192;//88;       /**< Horizontal back porch */
  mLcdPara.mHorzPW    = 48;       /**< Horizontal sync pulse */
  mLcdPara.mHorzFP    = 40;       /**< Horizontal front porch */
//mVertBP+mVertPW+mVertFP = 1125
  mLcdPara.mVertBP    = 29;//32;       /**< Vertical back porch */
  mLcdPara.mVertPW    = 3;        /**< Vertical sync pulse */
  mLcdPara.mVertFP    = 13;       /**< Vertical front porch */
  mLcdPara.mVertFq    = 60;       /**< Vertical frequency/Pixell frequency/Division factor */
  mLcdPara.mSyncPol   = 3;        /**< Sync signal polarity */
  mLcdPara.mDivFactor = 0;        /**< division factor */
  mDcuClk = 148500000;//148000000; ///< DCU input clock  2475000
  ///mDcuClk = 100000000;//148000000; ///< DCU input clock


 					#if 0

					  //mLcdPara.mDeltaX    = mWidth;   /**< delta X */
					  //mLcdPara.mDeltaY    = mHeight;  /**< delta Y */
					  mLcdPara.mDeltaX    = 1920;     /**< delta X */
					  mLcdPara.mDeltaY    = 1080;     /**< delta Y */
					  mLcdPara.mHorzBP    = 30;       /**< Horizontal back porch */
					  mLcdPara.mHorzPW    = 60;       /**< Horizontal sync pulse */
					  mLcdPara.mHorzFP    = 30;       /**< Horizontal front porch */
					  mLcdPara.mVertBP    = 3;        /**< Vertical back porch */
					  mLcdPara.mVertPW    = 5;        /**< Vertical sync pulse */
					  mLcdPara.mVertFP    = 3;        /**< Vertical front porch */
					  mLcdPara.mVertFq    = 60;       /**< Vertical frequency/Pixell frequency/Division factor */
					  mLcdPara.mSyncPol   = 3;        /**< Sync signal polarity */
					  mLcdPara.mDivFactor = 0;        /**< division factor */
					  mDcuClk = 150000000; ///< DCU input clock
					  #endif
 
#else
#ifdef PC_MONITOR_1280X1024_60HZ
  mLcdPara.mDeltaX    = 1280;     /**< delta X */
  mLcdPara.mDeltaY    = 1024;     /**< delta Y */
  
  mLcdPara.mHorzBP    = 216;      /**< Horizontal back porch */
  mLcdPara.mHorzPW    = 128;      /**< Horizontal sync pulse */
  mLcdPara.mHorzFP    = 88;       /**< Horizontal front porch */
  mLcdPara.mVertBP    = 29;       /**< Vertical back porch */
  mLcdPara.mVertPW    = 7;        /**< Vertical sync pulse */
  mLcdPara.mVertFP    = 3;        /**< Vertical front porch */
  mLcdPara.mVertFq    = 60;       /**< Vertical frequency/Pixell frequency/Division factor */
  mLcdPara.mSyncPol   = 3;        /**< Sync signal polarity */
  mLcdPara.mDivFactor = 0;        /**< division factor */
  mDcuClk = 100000000; ///< DCU input clock

#else
#ifdef PC_MONITOR_1920X1080_60HZ
  //mLcdPara.mDeltaX    = mWidth;   /**< delta X */
  //mLcdPara.mDeltaY    = mHeight;  /**< delta Y */
  mLcdPara.mDeltaX    = 1920;     /**< delta X */
  mLcdPara.mDeltaY    = 1080;     /**< delta Y */
  
  mLcdPara.mHorzBP    = 88;       /**< Horizontal back porch */
  mLcdPara.mHorzPW    = 48;       /**< Horizontal sync pulse */
  mLcdPara.mHorzFP    = 40;       /**< Horizontal front porch */
  mLcdPara.mVertBP    = 32;       /**< Vertical back porch */
  mLcdPara.mVertPW    = 3;        /**< Vertical sync pulse */
  mLcdPara.mVertFP    = 13;       /**< Vertical front porch */
  mLcdPara.mVertFq    = 60;       /**< Vertical frequency/Pixell frequency/Division factor */
  mLcdPara.mSyncPol   = 3;        /**< Sync signal polarity */
  mLcdPara.mDivFactor = 0;        /**< division factor */
  mDcuClk = 148000000; ///< DCU input clock

#else
  mLcdPara.mDeltaX    = mWidth;   /**< delta X */
  mLcdPara.mDeltaY    = mHeight;  /**< delta Y */
  //mHorzBP+mHorzPW+mHorzFP =2200
  mLcdPara.mHorzBP    = 192;//88;       /**< Horizontal back porch */
  mLcdPara.mHorzPW    = 48;       /**< Horizontal sync pulse */
  mLcdPara.mHorzFP    = 40;       /**< Horizontal front porch */
//mVertBP+mVertPW+mVertFP = 1125
  mLcdPara.mVertBP    = 29;//32;       /**< Vertical back porch */
  mLcdPara.mVertPW    = 3;        /**< Vertical sync pulse */
  mLcdPara.mVertFP    = 13;       /**< Vertical front porch */
  mLcdPara.mVertFq    = 60;       /**< Vertical frequency/Pixell frequency/Division factor */
  mLcdPara.mSyncPol   = 3;        /**< Sync signal polarity */
  mLcdPara.mDivFactor = 0;        /**< division factor */
  mDcuClk = 148500000;//148000000; ///< DCU input clock
#endif  // PC_MONITOR_1920X1080_60HZ
#endif  // PC_MONITOR_1280X1024_60HZ
#endif  // SAMSUNG
  
 
  mLcdConnection = DCU_FREQDIV_NORMAL;

  
  if(DCU_Init(sDCU, mDcuClk, &mLcdPara, mLcdConnection) != DCU_ERR_OK) 
  {
    VDB_LOG_ERROR("DCU_Init failed.\n");
    return;
  }
  
  // *** Allocate buffers ***
  if(0 == mSize)
  {
    mSize = mWidth * mHeight * mChannels;
    for(uint32_t i = 0; i < DCU_FRAME_BUFFER_CNT; i++)
    {
      // Allocate DDR buffers
      mpFbsPhysical[i] = OAL_MemoryAllocFlag(mSize, OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE)|OAL_MEMORY_FLAG_CONTIGUOUS);
      mppFbsVirtual[i] = OAL_MemoryReturnAddress(mpFbsPhysical[i], 
                                                          ACCESS_CH_WB); // get physical address
#ifndef __STANDALONE__
      mppFrameBuffers[i] = (uint8_t*)mppFbsVirtual[i];
#else
      mppFrameBuffers[i] = (uint8_t*)mpFbsPhysical[i];
#endif
    } // for all framebuffers
  }
  
  if(aFormat == DCU_BPP_8)
  {
    for(uint32_t i=0; i<256; i++)
    {
      DCU_CLUTSet(DCU0, i, 0xff000000 | (i << 16) | (i << 8) | i);
    } // for all 256 gray levels
  } // if gs8 pixel format
  
  // *** Layer configuration ***
  mDcuLayerParams.LayerWidth      = mWidth;
  mDcuLayerParams.LayerHeight     = mHeight;
  mDcuLayerParams.LayerPosX       = 0;
  mDcuLayerParams.LayerPosY       = 0;
  mDcuLayerParams.LayerBuffAdd    = (uint32_t)(uint64_t)(mpFbsPhysical[0]);
  mDcuLayerParams.LayerColCode    = aFormat;
  mDcuLayerParams.LayerAlphaBlend = DCU_ALPHAKEY_OFF;
  mDcuLayerParams.LayerChromaKey  = DCU_DISABLE;
  mDcuLayerParams.LayerTransp     = 0;
  mDcuLayerParams.LayerFGNDCol    = 0xff;     // TODO: change as needed
  mDcuLayerParams.LayerBGNDCol    = 0xff00;   // TODO: change as needed
  mDcuLayerParams.LayerPreSkip    = 0x0;      ///< pixels to be skipped at the beginning of line
  mDcuLayerParams.LayerPostSkip   = 0x0;      ///< pixels to be skipped at the end of line
  mDcuLayerParams.LayerTileEn     = 0x0;      ///< disable tile feature
  
  if(DCU_SetLayerConfig(sDCU, DCU_LAYER_0, &mDcuLayerParams) != DCU_ERR_OK)
  {
    VDB_LOG_ERROR("DCU_SetLayerConfig failed.\n");
    return;
  }
 
  DCU_LayerEnable(sDCU, DCU_LAYER_0);
  mStatus   = IO_OK;
  
} // FrameOutputDCU::Init()

//***************************************************************************

FrameOutputDCU::~FrameOutputDCU()
{
  //*** Free DDR buffers ***
  for(uint32_t i = 0; i < DCU_FRAME_BUFFER_CNT; i++)
  {
    if(mppFbsVirtual[i] != NULL)
    {
      OAL_MemoryFree(mpFbsPhysical[i]);
    } // if buffer allocated
    
    mppFbsVirtual[i]   = NULL;
    mpFbsPhysical[i]   = 0;
    mppFrameBuffers[i] = NULL;
  } // for all frame buffers

  mSize = 0;
} // FrameOutputDCU::~FrameOutputDCU()

//***************************************************************************

void FrameOutputDCU::Reset()
{
  DCU_SwReset(sDCU);
} // FrameOutputDCU::Reset()

//***************************************************************************

void FrameOutputDCU::Disable()
{
  DCU_Disable(sDCU);
  
} // FrameOutputDCU::Disable()

//***************************************************************************

void FrameOutputDCU::PutFrame(void *frame)
{
  PutFrame(frame, true);
}

//***************************************************************************

void FrameOutputDCU::PutFrame(void *frame, bool aFlushCache)
{
  if(aFlushCache)
  {
    OAL_MemoryFlush(frame, mSize);
  }
  
  DCU_SetLayerBuffAddr(sDCU, DCU_LAYER_0, (uint32_t)((uint64_t)frame));
  mNextBuff = (mNextBuff + 1) % DCU_FRAME_BUFFER_CNT;
}

//***************************************************************************

void FrameOutputDCU::PutFrame(vsdk::UMat umat)
{  
  DCU_SetLayerBuffAddr(sDCU, 
                       DCU_LAYER_0, 
                       ((uint32_t)(uint64_t)umat.u->handle) + umat.offset);
  mNextBuff = (mNextBuff + 1) % DCU_FRAME_BUFFER_CNT;
}

//***************************************************************************

IO_STATUS FrameOutputDCU::GetStatus()
{
  return mStatus;
}

} // namespace io

//***************************************************************************

static void DCU_pin_init(void)
{
      //uint32_t lRegValue = 0x200000 | 0x32200101; // IDC
      uint32_t lRegValue = 0x20C101; // PAVEL
     /*SIUL2.MSCR[120].B.MUX_MODE = 1;
     SIUL2.MSCR[120].B.OBE = 1;
     SIUL2.MSCR[120].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x1E0)) = lRegValue;
                    
/*    SIUL2.MCR[121].B.MUX_MODE = 1;
-     SIUL2.MCR[121].B.OBE = 1;
-     SIUL2.MCR[121].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x1E4)) = lRegValue;
                    
/*    SIUL2.MCR[122].B.MUX_MODE = 1;
-     SIUL2.MCR[122].B.OBE = 1;
-     SIUL2.MCR[122].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x1E8)) = lRegValue;
                    
/*    SIUL2.MCR[123].B.MUX_MODE = 1;
-     SIUL2.MCR[123].B.OBE = 1;
-     SIUL2.MCR[123].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x1EC)) = lRegValue;
                    
/*    SIUL2.MCR[124].B.MUX_MODE = 1;
-     SIUL2.MCR[124].B.OBE = 1;
-     SIUL2.MCR[124].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x1F0)) = lRegValue;
                    
/*    SIUL2.MCR[125].B.MUX_MODE = 1;
-     SIUL2.MCR[125].B.OBE = 1;
-     SIUL2.MCR[125].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x1F4)) = lRegValue;
                    
/*    SIUL2.MCR[126].B.MUX_MODE = 1;
-     SIUL2.MCR[126].B.OBE = 1;
-     SIUL2.MCR[126].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x1F8)) = lRegValue;
                    
/*    SIUL2.MCR[127].B.MUX_MODE = 1;
-     SIUL2.MCR[127].B.OBE = 1;
-     SIUL2.MCR[127].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x1FC)) = lRegValue;
                                                    
/*    SIUL2.MCR[128].B.MUX_MODE = 1;
-     SIUL2.MCR[128].B.OBE = 1;
-     SIUL2.MCR[128].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x200)) = lRegValue;
                    
/*    SIUL2.MCR[129].B.MUX_MODE = 1;
-     SIUL2.MCR[129].B.OBE = 1;
-     SIUL2.MCR[129].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x204)) = lRegValue;
                    
/*    SIUL2.MCR[130].B.MUX_MODE = 1;
-     SIUL2.MCR[130].B.OBE = 1;
-     SIUL2.MCR[130].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x208)) = lRegValue;
                    
/*    SIUL2.MCR[131].B.MUX_MODE = 1;
-     SIUL2.MCR[131].B.OBE = 1;
-     SIUL2.MCR[131].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x20C)) = lRegValue;
                    
/*    SIUL2.MCR[132].B.MUX_MODE = 1;
-     SIUL2.MCR[132].B.OBE = 1;
-     SIUL2.MCR[132].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x210)) = lRegValue;
                    
/*    SIUL2.MCR[133].B.MUX_MODE = 1;
-     SIUL2.MCR[133].B.OBE = 1;
-     SIUL2.MCR[133].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x214)) = lRegValue;
                    
/*    SIUL2.MCR[134].B.MUX_MODE = 1;
-     SIUL2.MCR[134].B.OBE = 1;
-     SIUL2.MCR[134].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x218)) = lRegValue;
                    
/*    SIUL2.MCR[135].B.MUX_MODE = 1;
-     SIUL2.MCR[135].B.OBE = 1;
-     SIUL2.MCR[135].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x21C)) = lRegValue;
                    
/*    SIUL2.MCR[136].B.MUX_MODE = 1;
-     SIUL2.MCR[136].B.OBE = 1;
-     SIUL2.MCR[136].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x220)) = lRegValue;
                    
/*    SIUL2.MCR[137].B.MUX_MODE = 1;
-     SIUL2.MCR[137].B.OBE = 1;
-     SIUL2.MCR[137].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x224)) = lRegValue;
                    
/*    SIUL2.MCR[138].B.MUX_MODE = 1;
-     SIUL2.MCR[138].B.OBE = 1;
-     SIUL2.MCR[138].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x228)) = lRegValue;
                    
/*    SIUL2.MCR[139].B.MUX_MODE = 1;
-     SIUL2.MCR[139].B.OBE = 1;
-     SIUL2.MCR[139].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x22C)) = lRegValue;
                    
/*    SIUL2.MCR[140].B.MUX_MODE = 1;
-     SIUL2.MCR[140].B.OBE = 1;
-     SIUL2.MCR[140].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x230)) = lRegValue;
                    
/*    SIUL2.MCR[141].B.MUX_MODE = 1;
-     SIUL2.MCR[141].B.OBE = 1;
-     SIUL2.MCR[141].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x234)) = lRegValue;
                   
/*    SIUL2.MCR[142].B.MUX_MODE = 1;
-     SIUL2.MCR[142].B.OBE = 1;
-     SIUL2.MCR[142].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x238)) = lRegValue;
                    
/*    SIUL2.MCR[143].B.MUX_MODE = 1;
-     SIUL2.MCR[143].B.OBE = 1;
-     SIUL2.MCR[143].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x23C)) = lRegValue;
                    
/*    SIUL2.MCR[144].B.MUX_MODE = 1;
-     SIUL2.MCR[144].B.OBE = 1;
-     SIUL2.MCR[144].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x240)) = lRegValue;
                    
/*    SIUL2.MCR[145].B.MUX_MODE = 1;
-     SIUL2.MCR[145].B.OBE = 1;
-     SIUL2.MCR[145].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x244)) = lRegValue;
                    
/*    SIUL2.MCR[146].B.MUX_MODE = 1;
-     SIUL2.MCR[146].B.OBE = 1;
-     SIUL2.MCR[146].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x248)) = lRegValue;
                    
/*    SIUL2.MCR[147].B.MUX_MODE = 1;
-     SIUL2.MCR[147].B.OBE = 1;
-     SIUL2.MCR[147].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x24C)) = lRegValue;
                    
/*    SIUL2.MCR[148].B.MUX_MODE = 1;
-     SIUL2.MCR[148].B.OBE = 1;
-     SIUL2.MCR[148].B.SRE = 3;*/
                    *((vuint32_t*)(0x4006C000+0x240+0x250)) = lRegValue;

}
