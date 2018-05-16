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

#ifndef __STANDALONE__
  #include <signal.h>
#endif // #ifdef __STANDALONE__
#include <string.h>

#ifdef __STANDALONE__
  #include "frame_output_dcu.h"
  #define CHNL_CNT io::IO_DATA_CH3
#else // #ifdef __STANDALONE__
  #include "frame_output_v234fb.h"
  #define CHNL_CNT io::IO_DATA_CH3
#endif // else from #ifdef __STANDALONE__  

#include "oal.h"
#include "vdb_log.h"
#include "sdi.hpp"

#include "max9286_96705_4_c.h"

#include "vdb_log.h"

//***************************************************************************

// Possible to set input resolution (must be supported by the DCU)
#define WIDTH           1280 ///< width of DDR buffer in pixels
#define HEIGHT          720 ///< height of DDR buffer in pixels
#define DDR_BUFFER_CNT  3    ///< number of DDR buffers per ISP stream

static uint8_t          OutPutBufferUYVY[WIDTH*HEIGHT*2];
static uint8_t          RGB24Buffer[WIDTH*HEIGHT*3];


//***************************************************************************

#define FRM_TIME_MSR 300 ///< number of frames to measure the time and fps

#ifdef __STANDALONE__
//extern SEQ_Buf_t  producerless_buffer_1;
extern "C" {
  unsigned long get_uptime_microS(void);
}

#define GETTIME(time)   (*(time)=get_uptime_microS())
#else // ifdef __STANDALONE__
#define GETTIME(time) \
  { \
  struct timeval lTime; gettimeofday(&lTime,0); \
  *time=(lTime.tv_sec*1000000+lTime.tv_usec);   \
  }
#endif // else from #ifdef __STANDALONE__

/************************************************************************/
/** User defined call-back function for Sequencer event handling.
  * 
  * \param  aEventType defines Sequencer event type
  * \param  apUserVal  pointer to any user defined object 
  ************************************************************************/
void SeqEventCallBack(uint32_t aEventType, void* apUserVal);

#ifndef __STANDALONE__
/************************************************************************/
/** SIGINT handler.
  * 
  * \param  aSigNo 
  ************************************************************************/
void SigintHandler(int);

/************************************************************************/
/** SIGINT handler.
  * 
  * \param  aSigNo 
  * 
  * \return SEQ_LIB_SUCCESS if all ok
  *         SEQ_LIB_FAILURE if failed
  ************************************************************************/
int32_t SigintSetup(void);

//***************************************************************************

static bool sStop = false; ///< to signal Ctrl+c from command line

#endif // #ifndef __STANDALONE__
/************************************************************************/
/** Allocates contiguous DDR buffers for one ISP stream.
  * 
  * \param  appFbsVirtual array of virtual buffer pointers to be filled
  * \param  apFbsPhysical array of buffer physical addresses to be filled
  * \param  aMemSize      size of the array in bytes
  * \param  aBufferCnt    number of buffers to be allocated
  * 
  * \return 0 if all OK
  *         < 0 otherwise
  ************************************************************************/
int32_t DdrBuffersAlloc(void** appFbsVirtual, uint32_t*  apFbsPhysical,
                        size_t aMemSize, uint32_t aBufferCnt);

/************************************************************************/
/** Frees DDR buffers for one ISP stream.
  * 
  * \param  appFbsVirtual array of virtual buffer pointers to be filled
  * \param  apFbsPhysical array of buffer physical addresses to be filled
  * \param  aBufferCnt    number of buffers to be freed
  ************************************************************************/
void DdrBuffersFree(void** appFbsVirtual, uint32_t*  apFbsPhysical,
                    uint32_t aBufferCnt);


#define SATURATE_CAST(v) (unsigned char)((v) <= 0 ? 0 : (v) > 255 ? 255 : (v))

void convertUYVY2RGB(int width, int height, unsigned char *pSrc, unsigned char *pDst)
{
	int i;
	int j;
	int u;
	int v;
	int ruv;
	int guv;
	int buv;
	int y00;
	int y01;
	int vidx = 2;
	int widthstep = width << 1;
	int widthstep2 = width * 3;
	unsigned char *src = pSrc;
	unsigned char *dst;

	for (j = 0; j < height; j++, src += widthstep)
	{
		dst = pDst + j * widthstep2;
		for (i = 0; i < widthstep; i += 4, dst += 6)
		{
			u = (int)src[i] - 128;
			v = (int)src[i + vidx] - 128;

			ruv = v + ((v * 103) >> 8);
			guv = ((u * 88) >> 8) + ((v * 183) >> 8);
			buv = u + ((u * 198) >> 8);

			y00 = (int)src[i + 1];
			dst[0] = SATURATE_CAST(y00 + buv); 
			dst[1] = SATURATE_CAST(y00 - guv);
			dst[2] = SATURATE_CAST(y00 + ruv);
			y01 = (int)src[i + 3];
			dst[3] = SATURATE_CAST(y01 + buv); 
			dst[4] = SATURATE_CAST(y01 - guv);
			dst[5] = SATURATE_CAST(y01 + ruv);
		}
	}
}


int main(int, char **))
{
  LIB_RESULT lRet = LIB_SUCCESS;
  LIB_RESULT lRes = LIB_SUCCESS;
  
  OAL_Initialize();

  //*** Init DCU Output ***
#ifdef __STANDALONE__
  io::FrameOutputDCU
    lDcuOutput(WIDTH, 
               HEIGHT,
               io::IO_DATA_DEPTH_08,
               io::IO_DATA_CH3,
               DCU_BPP_YCbCr422
               );
#else // #ifdef __STANDALONE__

  // setup Ctrl+C handler
  if(SigintSetup() != SEQ_LIB_SUCCESS) 
  {
    VDB_LOG_ERROR("Failed to register Ctrl+C signal handler.");
    return -1;
  }
  
  printf("Press Ctrl+C to terminate the demo.\n");
  
  // *** set terminal to nonblock input ***
  //TermNonBlockSet();
  
  io::FrameOutputV234Fb
    lDcuOutput(1280, 
               720,
               io::IO_DATA_DEPTH_08,
               io::IO_DATA_CH3,
               DCU_BPP_YCbCr422
               );
#endif // else from #ifdef __STANDALONE__
  printf("main.cpp line135\n");
  //
  // *** Initialize SDI ***
  //
  lRes = sdi::Initialize(0); 
  printf("main.cpp line140\n");
  // *** create grabber ***
  sdi_grabber *lpGrabber = new(sdi_grabber);  
  lpGrabber->ProcessSet(gpGraph, &gGraphMetadata);
  printf("main.cpp line144\n");
  // *** set user defined Sequencer event handler *** 
  int32_t lCallbackUserData = 12345;
  lpGrabber->SeqEventCallBackInstall(&SeqEventCallBack, &lCallbackUserData);
  
  // *** prepare IOs ***
  sdi_FdmaIO *lpFdma = (sdi_FdmaIO*)lpGrabber->IoGet(SEQ_OTHRIX_FDMA);    
  
  // modify DDR frame geometry to fit display output
  SDI_ImageDescriptor lFrmDesc = SDI_ImageDescriptor(WIDTH, HEIGHT, YUV422Stream_UYVY);
  lpFdma->DdrBufferDescSet(0, lFrmDesc);
  
  lFrmDesc = SDI_ImageDescriptor(WIDTH, HEIGHT, YUV422Stream_UYVY);
  lpFdma->DdrBufferDescSet(1, lFrmDesc);
  
  lFrmDesc = SDI_ImageDescriptor(WIDTH, HEIGHT, YUV422Stream_UYVY);
  lpFdma->DdrBufferDescSet(2, lFrmDesc);
  
  lFrmDesc = SDI_ImageDescriptor(WIDTH, HEIGHT, YUV422Stream_UYVY);
  lpFdma->DdrBufferDescSet(3, lFrmDesc);
  
  //*** allocate DDR buffers ***
  lpFdma->DdrBuffersAlloc(DDR_BUFFER_CNT);
  printf("main.cpp line154\n");
  // *** prestart grabber ***
  lpGrabber->PreStart();
  printf("main.cpp line157\n");
  // fetched frame buffer storage
  SDI_Frame lFrame[4];
  
  // *** start grabbing ***
  lpGrabber->Start();  
  printf("main.cpp line163\n");
  unsigned long lTimeStart = 0, lTimeEnd = 0, lTimeDiff = 0;
  GETTIME(&lTimeStart);
  
  uint32_t actualBufferIndex = 0;

  uint32_t lFrmCnt = 0;
  printf("Complete init done.\n");
  uint8_t lLoop=0; 
  for(;;)
  {
	

    // pop all
    int i = 0;

    for (int i = 0;i < 4;i++)   //*******************DY*************
    {
        lFrame[i] = lpGrabber->FramePop(i);
        if(lFrame[i].mUMat.empty())
	      {
	        printf("Failed to grab image number %u\n", lFrmCnt);
	        break;
	      } // if pop failed
    }  

    if (lLoop < 30)
       actualBufferIndex = 0;
    else if(lLoop < 60)
       actualBufferIndex = 1;
    else if(lLoop < 90)
       actualBufferIndex = 2;
    else if(lLoop < 120)
       actualBufferIndex = 3;
    else if(lLoop == 120)
    {
       actualBufferIndex = 3;
       lLoop = 0;
    }

    //printf("Putframe start selected camera = %d\n ",actualBufferIndex);
   // pFullPyramidGray=RGB24Buffer;
   // gpFrameOut = lFrame[0].mImage.mData;
   //memset(YUV422Buffer,0,1280*2*800);
	//memcpy(YUV422Buffer, (unsigned char *)(lFrame[0].mImage.mData), 1280*800*2);
    //memset(RGB24Buffer,64,1280*800*3);
	//convertUYVY2RGB(1280, 800, YUV422Buffer, RGB24Buffer);
	
/*  uint32_t index = 0;
	uint16_t tempstore;
	uint16_t tempstore1;
   for( index = 0 ; index < (1280*720) ; index = index + 1 )
				{
					tempstore1 = 0;
					tempstore = ((uint16_t*)lFrame[actualBufferIndex].mImage.mData)[index];
					tempstore1 = tempstore << 8;
					tempstore = tempstore >> 8;
					tempstore = tempstore | tempstore1;
					((uint16_t*)OutPutBufferUYVY)[index] =  tempstore;
				}
*/
	
		lLoop++;  //***********DY**********************
		// push all
	    for (int i = 0;i < 4;i++)  //*******************DY****************
	    {
			if(lpGrabber->FramePush(lFrame[i]) != LIB_SUCCESS)
			{
			  break;
			} // if push failed
	    }    

   lDcuOutput.PutFrame(lFrame[actualBufferIndex].mUMat);  //*************DY********************
	//lDcuOutput.PutFrame(OutPutBufferUYVY); 

    if((lFrmCnt%FRM_TIME_MSR) == 0)
    {
      GETTIME(&lTimeEnd);
      lTimeDiff = lTimeEnd-lTimeStart;
      lTimeStart = lTimeEnd;
      
      printf("%u frames took %lu usec (%5.2ffps)\n",FRM_TIME_MSR,lTimeDiff,
            (FRM_TIME_MSR*1000000.0)/((float)lTimeDiff));
    }// if time should be measured
    
    lFrmCnt++;
#ifndef __STANDALONE__  
    if(sStop)
    {
      break; // break if Ctrl+C pressed
    } // if Ctrl+C
#endif //#ifndef __STANDALONE__  
  } // for ever
  
  if(lpGrabber)
  {
    //*** Stop ISP processing ***
    lpGrabber->Stop();
    
    // clean up grabber resources
    lpGrabber->Release();
    
    delete(lpGrabber);
  } // if grabber exists

#ifdef __STANDALONE__
  for(;;);  // *** don't return ***
#endif // #ifdef __STANDALONE__    
  
  //
  // Close SDI
  //

  lRes = sdi::Close(0);
  if(lRes != LIB_SUCCESS)
  {
    lRet = LIB_FAILURE;
  } // if lRes not LIB_SUCCESS
  return lRet;

} // main()


int32_t DdrBuffersAlloc(void** appFbsVirtual, 
                        uint32_t*  apFbsPhysical, 
                        size_t aMemSize,
                        uint32_t aBufferCnt
                       )
{
  int32_t lRet    = 0;
  
  // allocate buffers & get physical addresses
  for(uint32_t i = 0; i < aBufferCnt; i++)
  {
    appFbsVirtual[i] = OAL_MemoryAllocFlag(
                          aMemSize, 
                          OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE)|
                          OAL_MEMORY_FLAG_CONTIGUOUS);
    
    if( appFbsVirtual[i] == NULL)
    {
      lRet = -1;
      break;
    }
    apFbsPhysical[i] = (uint32_t)(uintptr_t)OAL_MemoryReturnAddress(
                          appFbsVirtual[i], 
                          ACCESS_PHY); // get physical address

    memset(appFbsVirtual[i], 0x00, aMemSize);
#if defined(__STANDALONE__) && defined(__ARMV8)
    //flush_dcache_range(appFbsVirtual[i], aMemSize);     
#endif // #ifdef __STANDALONE__
  } // for all framebuffers
  
  if(lRet != 0)
  {
    DdrBuffersFree(appFbsVirtual, apFbsPhysical, aBufferCnt);
  }
  
  return lRet;
} // DdrBuffersAlloc()

//***************************************************************************

void DdrBuffersFree(void** appFbsVirtual, 
                    uint32_t*  apFbsPhysical,
                    uint32_t aBufferCnt
                   )
{
  for(uint32_t i = 0; i < aBufferCnt; i++)
  {
    if(appFbsVirtual[i] != NULL)
    {
      OAL_MemoryFree(appFbsVirtual[i]);
    } // if buffer allocated
    
    appFbsVirtual[i]   = NULL;
    apFbsPhysical[i]   = 0;
  } // for all framebuffers
} // DdrBuffersFree()



//***************************************************************************

void SeqEventCallBack(uint32_t aEventType, void* apUserVal)
{
  static uint32_t slFrmCnt = 0;
  
  if(aEventType == SEQ_MSG_TYPE_FRAMEDONE)
  {
    //printf("Frame done message arrived #%u. User value %d\n", 
    //       slFrmCnt++,
    //       *((int32_t*)apUserVal));
  } // if frame done arrived
} // SeqEventCallBack()

//***************************************************************************

#ifndef __STANDALONE__
void SigintHandler(int)
{
  sStop = true;
} // SigintHandler()

//***************************************************************************

int32_t SigintSetup()
{
  int32_t lRet = SEQ_LIB_SUCCESS;
  
  // prepare internal signal handler
  struct sigaction lSa;
  memset(&lSa, 0, sizeof(lSa));
  lSa.sa_handler = SigintHandler;
  
  if( sigaction(SIGINT, &lSa, NULL) != 0)
  {
    VDB_LOG_ERROR("Failed to register signal handler.\n");
    lRet = SEQ_LIB_FAILURE;
  } // if signal not registered
  
  return lRet;
} // SigintSetup()

//***************************************************************************
#endif // #ifndef __STANDALONE__
