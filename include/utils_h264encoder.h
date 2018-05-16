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

/*!*********************************************************************************
*  \file
*  \brief h264 encoder
***********************************************************************************/

#ifndef H264ENCODER_H
#define H264ENCODER_H

#include "s32vs234.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>    /* open */
#include <unistd.h>   /* exit */
#include <sys/mman.h>   /* mmap */
#include <stdint.h>
#include <sys/types.h>
#include <sys/ioctl.h>  /* ioctl */
#include "isp_h264enc.h"
#include "vdb_log.h"
#include "oal.h"

#include <boost/thread.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/thread/mutex.hpp>

#include "common_time_measure.h"
  
//*****************************************************************************
// consts
//*****************************************************************************

#define DDR_BASE  0x80000000
#define SRAM_SUB2HOST_OFF (0x7e800000 - 0x3e800000)

#define SRAM_BASE         0x3e800000
#define SRAM_SIZE         (4*1024*1024)

//****************************************************************************
// macros
//****************************************************************************
#define SRAM_HOST2SUB(_aHost) (_aHost - SRAM_SUB2HOST_OFF) 
#define SRAM_SUB2HOST(_aSub) (_aSub + SRAM_SUB2HOST_OFF) 

//*****************************************************************************
// types
//*****************************************************************************

class EventHandlerClass : H264ENC_EventHandlerClass
{
  uint32_t mVal;
public:
  EventHandlerClass():mVal(0){};
  
  void H264EncEventHandler(uint32_t )
  {
    mVal = 3;
    VDB_LOG_NOTE("User defined class called to handle event number \n");
  }; // SeqEventHandler(uint32_t aEventType)
  
  uint32_t ValGet() {return mVal;};
};


//************
// Struct
//************

//*****************************************************************************
// local functions
//*****************************************************************************
    
// static void EventHandler(uint32_t aEventType);

H264ENC_LIB_RESULT HwMap(void);

H264ENC_LIB_RESULT HwUnmap(void);

uint8_t* MemMap(uint32_t aSize, uint32_t aOffset);

void MemUnmap(uint32_t aSize, uint8_t *apMem);

void MemPrint(uint8_t *apMem, uint32_t aSize, const char* apcCaption);

H264ENC_LIB_RESULT MapH264();

H264ENC_LIB_RESULT FrameIrqDone(); 

H264ENC_LIB_RESULT SW_rSONY_SINGLE_EXPeset(); 

H264ENC_LIB_RESULT BS_NEW_ROW_INT_control();

H264ENC_LIB_RESULT Last_frame_wait();
 
/// Delegate encoderDone, function template
typedef boost::signals2::signal<void (unsigned char *result, int final_length, long encoding_time_us)> EncoderDone;

////////////////////////////////////////////////////////////////////////////////
/**
 * Class encapsulating main H264 Encoder functions 
 */
class H264Encoder
{
  public:
    
    /// OnInitListener
    typedef EncoderDone::slot_type EncoderDoneListener;            
    void setOnEncoderDoneListener(const EncoderDoneListener listener);
    
    static H264Encoder* getInstance();
    
    static void H264Encoder_setup(int width, int height, int lines_number);
    /**
     * Destructor - frees resources
     */
    ~H264Encoder();       
      
    void Initialize();    
    
    void EncoderFrame(unsigned char* pDst, unsigned char* pFrame, bool wait);
    
    int EncoderLastFrame(unsigned char* pDst);
    
    void Close();
    
    void WaitUntilEncodingDone();
    
    int encoded_data_length;
    
  private:
      
    EncoderDone onEncode;
    
    static H264Encoder* instance;  
    
    static int width_;
    
    static int height_;
    
    static int lines_number_;
    
    /**
     * Constructor
     */
    H264Encoder(int width, int height, int lines_number);  
      
    EventHandlerClass lHandler;
    
    uint8_t *lpSramBuff;
    void    *lpSramBuffHandle;
    unsigned long lSramBuff_0;
    
    uint64_t* lpSram64 ;
   
    volatile unsigned char memSetValue = 0; //to achieve non-optimized SRAM memset. Needs 64bit approach, but in Optimized version compiler replaced it with LIBC memset.
     
    H264ENC_Status_t lEncStatus;
    
    unsigned long long segment_data_volume;
  
    unsigned long lastAddress;  
      
    int m_IMAGE_WIDTH;
    int m_IMAGE_HEIGHT;

    int m_IN_BUFF_OFF;     //Y channel input buffer
    int m_IN_BUFF_ADDR;  
    int m_IN_BUFF_SIZE; 

    int m_IN_BUFF_U_OFF;   //U channel input buffer
    int m_IN_BUFF_U_ADDR;  
    int m_IN_BUFF_U_SIZE;  

    int m_IN_BUFF_V_OFF;  //V channel input buffer
    int m_IN_BUFF_V_ADDR; 
    int m_IN_BUFF_V_SIZE;  

    int m_OUT_BUFF_OFF;  //output buffer
    int m_OUT_BUFF_ADDR;
    int m_OUT_BUFF_SIZE; 
    
    int m_LINES_NUMBER;
    
    void EncodingConfigure();
    
    H264ENC_LIB_RESULT DataSave(const char* apcFile, uint8_t* apData, uint32_t aSize);
    
    H264ENC_LIB_RESULT DataTxtSave(const char* apcFile, uint8_t* apData, uint32_t aSize);

    H264ENC_LIB_RESULT DataLoad(const char* apcFile, uint8_t* &aprData, uint32_t &aSize);
      
    int32_t DdrBuffersAlloc(void** appFbsVirtual, uint32_t*  apFbsPhysical, size_t aMemSize, uint32_t aBufferCnt);

    void DdrBuffersFree(void** appFbsVirtual, uint32_t*  apFbsPhysical, uint32_t aBufferCnt);
    
    void H264EncRegPrint(void);
    
    /// Running Thread
    boost::thread run_t_;
    
    /// Function for run thread
    void run_thread();
    
    /// Semaphore for encoding thread
    boost::interprocess::interprocess_semaphore start_sem;
    
    boost::mutex io_mutex;
    
    /// Value that kills thread
    bool can_work;
    
    bool isWorking;
    
    boost::condition_variable condition;
    
    unsigned char* global_pDst;
    unsigned char* global_pFrame;
};

#endif /* H264ENCODER_H */
