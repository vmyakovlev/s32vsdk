#include "utils_h264encoder.h"

//*****************************************************************************
// global data
//*****************************************************************************
static uint32_t sEventCnt = 0;
H264ENC_Regs_t *gpH264EncRegs = NULL;
uint8_t *gpSram = NULL;
uint32_t gSramBuff = 0;

H264Encoder* H264Encoder::instance = NULL;
int H264Encoder::width_ = 0;
int H264Encoder::height_ = 0;
int H264Encoder::lines_number_ = 16;

//****************************************************************************
static void EventHandler(uint32_t )
{
  sEventCnt++;
  VDB_LOG_NOTE("User defined function called to handle event number \n");
} // EventHandler(uint32_t aEventType)

//****************************************************************************

H264ENC_LIB_RESULT HwMap()
{
  H264ENC_LIB_RESULT lRet = H264ENC_LIB_SUCCESS;
 
  // *** map SRAM ***
  gpSram = MemMap(SRAM_SIZE, SRAM_BASE); 
  if (gpSram == 0) 
  {
    VDB_LOG_ERROR("Can't map SRAM.\n");
    lRet = H264ENC_LIB_FAILURE;
  } // if SRAM map failed
     
  VDB_LOG_NOTE("SRAM %x %x mapped \n", SRAM_BASE, SRAM_SIZE);
    
  // *** map H264 regs ***
  gpH264EncRegs = (H264ENC_Regs_t*)MemMap(1024, H264ENC_BASE); 
  if (gpH264EncRegs == 0) 
  {
    VDB_LOG_ERROR("Can't map H264Enc registers.\n");
    lRet = H264ENC_LIB_FAILURE;
  } // if h264 Enc reg map failed
  
  return lRet;
} // HwMap()

//****************************************************************************

H264ENC_LIB_RESULT HwUnmap()
{
  MemUnmap(SRAM_SIZE, (uint8_t*) gpSram);
  MemUnmap(1024, (uint8_t*) gpH264EncRegs);  
  return H264ENC_LIB_SUCCESS;
} // HwUnMap()

//****************************************************************************
// MEM functions
//****************************************************************************

uint8_t* MemMap(uint32_t aSize, uint32_t aOffset)
{
  uint8_t *lpMem = NULL;
  int32_t lFd = 0;
  
  lFd = open("/dev/mem", O_RDWR);
  
  if(lFd < 0)
  {
    VDB_LOG_ERROR("Failed to open /dev/mem\n");
  } //if failed to open /dev/mem
  else
  { 
    lpMem = (uint8_t*) mmap(NULL, aSize, PROT_READ | PROT_WRITE, MAP_SHARED, lFd, aOffset);
    if (lpMem == (void *) -1) 
    {
      printf("Can't map memory: %s\n", strerror(errno));
      lpMem = NULL;
    }
    
    close(lFd);
  } // else from if failed to open /dev/mem
  
  return lpMem;
} // SramMap()

//****************************************************************************

void MemUnmap(uint32_t aSize, uint8_t *apMem)
{
  
  if(apMem != NULL)
  {
    if( munmap(apMem,aSize) != 0)
    {
      VDB_LOG_ERROR("Mem unmap failed with: %s\n", strerror(errno));
    }
  } // if apMem not NULL
  else
  {
    VDB_LOG_WARNING("Pointer is NULL. Memory not unmapped.\n");
  } // else from if apMem not NULL
} // SramUnmap()

//****************************************************************************

void MemPrint(uint8_t *apMem, uint32_t aSize)
{
  uint32_t i = 0;
  
  for(i = 0; i < aSize; i++)
  {
    if((i % 4) == 0)
    {
      printf("\n%8p:\t", apMem + i);
    }
    printf("%02x ", apMem[i]);    
  }
  
  printf("\n");
} // MemPrint

//*****************************************************************************

void MemPrint(uint8_t *apMem, uint32_t aSize, const char* apcCaption)
{  
  if(apcCaption != NULL)
  {
    printf("%s", apcCaption);
  } // if caption set
  
  MemPrint(apMem, aSize);
  
  printf("\n");
} // MemPrint

//*****************************************************************************

#define REG_CNT 38 

void H264EncRegPrint()
{
  uint32_t i = 0;
  uint32_t lRegOffsets[REG_CNT] = 
  {0x0, 0x10, 0x14, 0x18, 0x30, 0x34, 0x38, 0x3c, 0x40, 0x80, 0x84, 0x88, 
   0x90, 0x94, 0x104, 0x11c, 0x120, 0x140, 0x144, 0x148, 0x14c, 0x150, 
   0x200, 0x204, 0x208, 0x20c, 0x210, 0x214, 0x218, 0x21c, 0x220, 0x224, 
   0x228, 0x22c, 0x230, 0x234, 0x238, 0x284};
   
  if(gpH264EncRegs != NULL)
  {
    printf("H264 Encoder registers\n");
      
    for(i = 0; i < REG_CNT; i++)
    {
      
      uint8_t *lpByte = ((uint8_t*)gpH264EncRegs) + lRegOffsets[i];
      printf("\n0x%08x:\t", lRegOffsets[i]);
      
      printf("%02x %02x %02x %02x", lpByte[0], lpByte[1], lpByte[2], lpByte[3] );    
    }
    
    printf("\n");
  }
  else
  {   
    printf("H264 encoder registers not mapped\n");
  }
} // H264EncRegPrint

//*****************************************************************************

H264ENC_LIB_RESULT H264EncRegTxtSave(const char* apcFile)
{
  H264ENC_LIB_RESULT lRet = H264ENC_LIB_SUCCESS;
  uint32_t i = 0;
  uint32_t lRegOffsets[REG_CNT] = 
  {0x0, 0x10, 0x14, 0x18, 0x30, 0x34, 0x38, 0x3c, 0x40, 0x80, 0x84, 0x88, 
   0x90, 0x94, 0x104, 0x11c, 0x120, 0x140, 0x144, 0x148, 0x14c, 0x150, 
   0x200, 0x204, 0x208, 0x20c, 0x210, 0x214, 0x218, 0x21c, 0x220, 0x224, 
   0x228, 0x22c, 0x230, 0x234, 0x238, 0x284};
  
  FILE *fw = NULL;
  fw = fopen(apcFile,"wt");
  
  if(fw != NULL)
  {
    if(gpH264EncRegs != NULL)
    {
      printf("H264 Encoder registers\n");
      fprintf(fw, "#%u bytes\n",REG_CNT * 4); // write data size
  
      for( i = 0; i < REG_CNT; i++)
      {  
        uint8_t *lpByte = ((uint8_t*)gpH264EncRegs) + lRegOffsets[i];
        fprintf(fw, "\n0x%08x:\t", lRegOffsets[i]);
        
        fprintf(fw, "%02x %02x %02x %02x", lpByte[0], lpByte[1], lpByte[2], lpByte[3] );    
      }
      
      fprintf(fw, "\n");
    }
    else
    {   
      VDB_LOG_ERROR("H264 encoder registers not mapped\n");
    }
    
    fclose(fw);
  }
  else
  {
    VDB_LOG_ERROR("Failed to open the file %s for write.\n", apcFile);
    lRet = H264ENC_LIB_FAILURE;
  }
  
  return lRet;
} // DataTxtSave()


//*****************************************************************************

void H264Encoder::H264Encoder_setup(int width, int height, int lines_number)
{
    width_ = width;
    height_ = height;
    lines_number_ = lines_number;
}

H264Encoder* H264Encoder::getInstance()
{
  if(instance == NULL)
  {
    instance = new H264Encoder(width_,height_,lines_number_);
  }
  return instance;
}

H264ENC_LIB_RESULT H264Encoder::DataSave(const char* apcFile, uint8_t* apData, uint32_t aSize)
{
  H264ENC_LIB_RESULT lRet = H264ENC_LIB_SUCCESS;
  
  FILE *fw = NULL;
  fw = fopen(apcFile,"wb");
  
  if(fw != NULL)
  {
    fwrite(&aSize, sizeof(uint32_t), 1, fw); // write data size
    fwrite(apData, sizeof(uint8_t), aSize/sizeof(uint8_t), fw); // write data
    fclose(fw);
  }
  else
  {
    VDB_LOG_ERROR("Failed to open the file %s for write.\n", apcFile);
    lRet = H264ENC_LIB_FAILURE;
  }
  
  return lRet;
} // DataSave()

//*****************************************************************************

H264ENC_LIB_RESULT H264Encoder::DataTxtSave(const char* apcFile, uint8_t* apData, uint32_t aSize)
{
  H264ENC_LIB_RESULT lRet = H264ENC_LIB_SUCCESS;
  
  FILE *fw = NULL;
  fw = fopen(apcFile,"wt");
  
  if(fw != NULL)
  {
    fprintf(fw, "#%u\n",aSize); // write data size
    for(uint32_t i = 0; i<aSize; i++)
    {
      if( (i%8) == 0)
      {
        fprintf(fw,"\n");
      }
      fprintf(fw, "%02x", apData[i]);// write data
      
    }
    fclose(fw);
  }
  else
  {
    VDB_LOG_ERROR("Failed to open the file %s for write.\n", apcFile);
    lRet = H264ENC_LIB_FAILURE;
  }
  
  return lRet;
} // DataTxtSave()

//*****************************************************************************

H264ENC_LIB_RESULT H264Encoder::DataLoad(const char* apcFile, uint8_t* &aprData, uint32_t &aSize)
{
  H264ENC_LIB_RESULT lRet = H264ENC_LIB_SUCCESS;
  
  FILE *fr = NULL;
  fr = fopen(apcFile,"rb");
  
  if(fr != NULL)
  {
    fread(&aSize, sizeof(uint32_t), 1, fr); // read data size
    aprData = (uint8_t *) malloc(sizeof(uint8_t) * aSize);

    fread(aprData, sizeof(uint8_t), aSize/sizeof(uint8_t), fr); // read data
    fclose(fr);    
  }
  else
  {
    VDB_LOG_ERROR("Failed to open the file %s for read.\n", apcFile);
    lRet = H264ENC_LIB_FAILURE;
  }
  
  return lRet;
} // DataLoad()

//*****************************************************************************

//***************************************************************************

int32_t H264Encoder::DdrBuffersAlloc(void** appFbsVirtual, 
                        uint32_t*  apFbsPhysical, 
                        size_t aMemSize,
                        uint32_t aBufferCnt)
{
  int32_t lRet    = 0;
  
  // allocate buffers & get physical addresses
  for(uint32_t i = 0; i < aBufferCnt; i++)
  {
    apFbsPhysical[i] = (uint32_t)(uint64_t)OAL_MemoryAllocFlag(
                          aMemSize, 
                          OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE)|
                          OAL_MEMORY_FLAG_CONTIGUOUS | OAL_ALLOC_DDR1);
    
    if( apFbsPhysical[i] == 0)
    {
      printf("Failed to allocated DDR buffers.");
      lRet = -1;
      break;
    }
    appFbsVirtual[i] = OAL_MemoryReturnAddress(
                           (void *)(uint64_t)(apFbsPhysical[i]), 
                          ACCESS_CH_WB); // get physical address

    memset(appFbsVirtual[i], 0x00, aMemSize);

  } // for all framebuffers
  
  if(lRet != 0)
  {
    DdrBuffersFree(appFbsVirtual, apFbsPhysical, aBufferCnt);
  }
  
  return lRet;
} // DdrBuffersAlloc()

//***************************************************************************

void H264Encoder::DdrBuffersFree(void** appFbsVirtual, 
                    uint32_t*  apFbsPhysical,
                    uint32_t aBufferCnt)
{
  for(uint32_t i = 0; i < aBufferCnt; i++)
  {
    if(appFbsVirtual[i] != NULL)
    {
      OAL_MemoryFree((void *)(uint64_t)apFbsPhysical[i]);
    } // if buffer allocated
    
    appFbsVirtual[i]   = NULL;
    apFbsPhysical[i]   = 0;
  } // for all framebuffers
} // DdrBuffersFree()

//***************************************************************************
   
  //****************************************************************************
void H264Encoder::EncodingConfigure()
{
  H264ENC_Config_t  lConfig;
  H264ENC_Buffers_t lBuffers;
  
  // *** Encoding parameters ***
  lConfig.mDataModeChroma = 0;  
  lConfig.mFetchTrig      = 1;  // Host will trigger;      
  lConfig.mColorFormat    = 0;  // yuv format;    
  lConfig.mBitWidth       = 0;  // 8 bpp;    
  
  lConfig.mFrameWidth     = m_IMAGE_WIDTH;   
  lConfig.mFrameHeight    = m_IMAGE_HEIGHT;  
      
  lConfig.mFrameRate      = 30;  
  lConfig.mQpInit         = 10;
  lConfig.mQpMin          = 0;
  lConfig.mQpMax          = 51;
  lConfig.mQpFallback     = 0;
  
  lConfig.mBitsPerMbrw    = 49000;
  
  lConfig.mQpIncDiff0     = 6;
  lConfig.mQpIncDiff1     = 3;
  lConfig.mQpIncDiff2     = 2;
  lConfig.mQpIncOffset    = 3;
  lConfig.mQpIncThld0     = 2 * lConfig.mBitsPerMbrw; 
  lConfig.mQpIncThld1     = 1.625 * lConfig.mBitsPerMbrw;
  lConfig.mQpIncThld2     = 1.3125 * lConfig.mBitsPerMbrw;
  
  lConfig.mQpDecDiff0     = 4;
  lConfig.mQpDecDiff1     = 2;
  lConfig.mQpDecDiff2     = 1;
  lConfig.mQpDecOffset    = 2;
  lConfig.mQpDecThld0     = 0.5 * lConfig.mBitsPerMbrw; 
  lConfig.mQpDecThld1     = 0.75 * lConfig.mBitsPerMbrw;
  lConfig.mQpDecThld2     = 0.875 * lConfig.mBitsPerMbrw;
  
  lConfig.mWaitFrames     = 5;
  
  lConfig.mDisableDbf     = 1;
  
  H264ENC_Configure(lConfig);
  
  // *** Buffer setup ***
  lBuffers.mY.mPhysAddr = gSramBuff;   // Y input buffer
  lBuffers.mU.mPhysAddr = gSramBuff + m_IN_BUFF_U_OFF;
  lBuffers.mV.mPhysAddr = gSramBuff + m_IN_BUFF_V_OFF;

  lBuffers.mLineNum = m_LINES_NUMBER >> 1;  //128 after bitshift 
 
  lBuffers.mBankStride = 8;   // 0-7 (>7 => disabled)
  
  // output bitstream data
  lBuffers.mBitstream.mPhysAddr = gSramBuff + m_IN_BUFF_SIZE + m_IN_BUFF_U_SIZE + m_IN_BUFF_V_SIZE;    
  lBuffers.mBitstream.mSize     = m_OUT_BUFF_SIZE; 
    
  H264ENC_BuffersSet(lBuffers);   
}

H264Encoder::H264Encoder(int width, int height, int lines_number):start_sem(0)
{
  can_work = true;
  run_t_ = boost::thread(boost::bind(&H264Encoder::run_thread,this));
  
  m_LINES_NUMBER = lines_number;
  
  m_IMAGE_WIDTH = width;
  m_IMAGE_HEIGHT = height;

  m_IN_BUFF_OFF = (0x0);     //Y channel input buffer
  m_IN_BUFF_ADDR = SRAM_BASE + m_IN_BUFF_OFF;  
  m_IN_BUFF_SIZE = width * m_LINES_NUMBER;

  m_IN_BUFF_U_OFF = m_IN_BUFF_OFF + m_IN_BUFF_SIZE;   //U channel input buffer
  m_IN_BUFF_U_ADDR = SRAM_BASE + m_IN_BUFF_U_OFF;  
  m_IN_BUFF_U_SIZE = width/2 * m_LINES_NUMBER/2;  

  m_IN_BUFF_V_OFF = m_IN_BUFF_U_OFF + m_IN_BUFF_U_SIZE;  //V channel input buffer
  m_IN_BUFF_V_ADDR = SRAM_BASE + m_IN_BUFF_V_OFF; 
  m_IN_BUFF_V_SIZE = width/2 * m_LINES_NUMBER/2;  

  m_OUT_BUFF_OFF = m_IN_BUFF_V_OFF + m_IN_BUFF_V_SIZE;  //output buffer
  m_OUT_BUFF_ADDR = SRAM_BASE +  m_OUT_BUFF_OFF;
  m_OUT_BUFF_SIZE = (0x20000); 
  
  global_pFrame = (unsigned char*)malloc(m_IMAGE_HEIGHT * m_IMAGE_WIDTH * 3 * sizeof(char));
}

////////////////////////////////////////////////////////////////////////////////
// Destructor - frees resources
////////////////////////////////////////////////////////////////////////////////
H264Encoder::~H264Encoder()
{
  
}

////////////////////////////////////////////////////////////////////////////////
// Auxiliary function used in GetNextFrame
// Saves current frame, warps bird eye image and computes maximum image
// from n last bird eye frames
////////////////////////////////////////////////////////////////////////////////

void H264Encoder::Initialize()
// int32_t H264Encoder::Initialize()
{
  
  /*SRAM buffer allocation*/  
  lpSramBuff = NULL;
  lpSramBuffHandle = NULL;
  lSramBuff_0 = 0;
  lpSramBuffHandle = OAL_MemoryAllocFlag( m_IN_BUFF_SIZE + m_IN_BUFF_U_SIZE + m_IN_BUFF_V_SIZE + m_OUT_BUFF_SIZE, 
                                          OAL_ALLOC_SRAM_SINGLE);
  lSramBuff_0 = (uint32_t)(uintptr_t) OAL_MemoryReturnAddress( lpSramBuffHandle, 
                                                               ACCESS_PHY);  
  
  gSramBuff = lSramBuff_0;
  
  MapH264();
  
  vdb_LogInit();
  
    
  VDB_LOG_NOTE("IN buff size %x, OUT buff addr %x.\n", m_IN_BUFF_SIZE, m_OUT_BUFF_ADDR);
  
  HwMap();
   
  H264ENC_Reserve();
  H264ENC_EventHandlerSet(EventHandler);
  H264ENC_EventHandlerSet((H264ENC_EventHandlerClass*) &lHandler);
   
  /*SRAM buffer */
  lpSramBuff = gpSram + (lSramBuff_0 - SRAM_BASE);
  lpSram64 = (uint64_t*)lpSramBuff;
  
  // *** reset input/output data ***
  for(int i = 0; i < (m_IN_BUFF_U_SIZE/8); i++ )
  {
    lpSram64[m_IN_BUFF_U_OFF/8 + i] = memSetValue;
  }
   
  for(int i = 0; i < (m_IN_BUFF_V_SIZE/8); i++ )
  {
    lpSram64[m_IN_BUFF_V_OFF/8 + i] = memSetValue;
  }
  
  for(int i = 0; i < (m_OUT_BUFF_SIZE/8); i++ )
  {
    lpSram64[m_OUT_BUFF_OFF/8 + i] = memSetValue;
  }
  
  for(int i = 0; i < (m_IN_BUFF_SIZE/8); i++ )
  {
    lpSram64[m_IN_BUFF_OFF/8 + i] = memSetValue;
  }
    
  // *** get status ***
  H264ENC_StatusGet(lEncStatus);

  // *** configure encoding ***
  EncodingConfigure();  
 
  lastAddress = gSramBuff + m_IN_BUFF_SIZE + m_IN_BUFF_U_SIZE + m_IN_BUFF_V_SIZE;
}

void H264Encoder::run_thread()
{
    while(can_work)
    {
      start_sem.wait();
      
      if(!can_work) break;
      
      unsigned long lTimeStart = 0, lTimeEnd = 0, lTimeDiff = 0;
      
      GETTIME(&lTimeStart);
      
      unsigned char *pDst = global_pDst;
      unsigned char *pFrame = global_pFrame;
      
      unsigned long long segment_data_volume = 0;       
    
      int final_length = 0;
      uint64_t x = 0;
      
      int image_width = m_IMAGE_WIDTH;
      int image_height = m_IMAGE_HEIGHT;
      int in_buff_v_off = m_IN_BUFF_V_OFF;
      int in_buff_u_off = m_IN_BUFF_U_OFF;
      int in_buff_size = m_IN_BUFF_SIZE;
      int in_buff_u_size = m_IN_BUFF_U_SIZE;
      int in_buff_v_size =  m_IN_BUFF_V_SIZE;
      int out_buff_size =  m_OUT_BUFF_SIZE;
      int lines_number = m_LINES_NUMBER;
      uint8_t *lpSramBuff_ = lpSramBuff;
      
        for(int n = 0; n < (image_height/lines_number); n++)
        { 
          H264ENC_StatusGet(lEncStatus);
          while(lEncStatus.mInLineCounter > 0)
          {
            H264ENC_StatusGet(lEncStatus);
          }
          
          int R = 0;
          int B = 0;
          int G = 0;
          
          
          for(int i = 0, j = 0; i < image_width * lines_number ; )
          {
            B = pFrame[(image_width * 3 * lines_number * n) + j++];
            G = pFrame[(image_width * 3 * lines_number * n) + j++];
            R = pFrame[(image_width * 3 * lines_number * n) + j++];
            lpSramBuff_[i++] = ((66*R + 129*G + 25*B) >> 8) + 16;
          }
        
          
          for(int i = 0, j = 0; i < image_width/2 * lines_number/2 ; i++)
          {
              
              int index = j + (i/(image_width/2)) * (3 * image_width);
              B = pFrame[(image_width * 3 * lines_number * n) + index];
              G = pFrame[(image_width * 3 * lines_number * n) + index+1];
              R = pFrame[(image_width * 3 * lines_number * n) + index+2];

             lpSramBuff_[in_buff_u_off + i] = ((-(R * 38)  - (G * 74) + (B * 112)) >> 8 ) + 128;
             lpSramBuff_[in_buff_v_off + i] = (( (R * 112) - (G * 94) - (B * 18)) >> 8 ) + 128;   
                      
            j += 6;
          }
          
          H264ENC_LinesFetch(lines_number);   //FETCH

          BS_NEW_ROW_INT_control();
              
          H264ENC_StatusGet(lEncStatus);
          
          if(lEncStatus.mOutCurrentAddress != 0 && lEncStatus.mOutCurrentAddress != lastAddress)
          {      
            if(SRAM_BASE + lEncStatus.mOutCurrentAddress > lastAddress)
            {  
              uint64_t max = SRAM_BASE + lEncStatus.mOutCurrentAddress - lastAddress;
              uint64_t index = (lastAddress - (gSramBuff));
              for(uint64_t i = 0; i < max; i++) //end of the circular buffer
              {
              // if output buffer full
                pDst[x] = lpSramBuff_[index  + i];
                x++;
              } 
        
              segment_data_volume = SRAM_BASE + lEncStatus.mOutCurrentAddress - lastAddress;
              lastAddress         = SRAM_BASE + lEncStatus.mOutCurrentAddress; 
            } //if(SRAM_BASE + lEncStatus.mOutCurrentAddress > lastAddress)
            
            if(SRAM_BASE + lEncStatus.mOutCurrentAddress < lastAddress)
            {
              for(uint64_t i = 0; i < (gSramBuff + in_buff_size + in_buff_u_size + in_buff_v_size + out_buff_size - lastAddress); i ++) //end of the circular buffer
              {  
                pDst[x] = lpSramBuff_[(lastAddress - (gSramBuff))  + i];
                x++;
              }         
              for(uint64_t i = 0; i < (SRAM_BASE + lEncStatus.mOutCurrentAddress - (gSramBuff + in_buff_size + in_buff_u_size + in_buff_v_size)); i++ ) //beginning of the circular buffer
              {   
                pDst[x] = lpSramBuff_[m_OUT_BUFF_OFF + i];
                x++;
              }            
                  
              segment_data_volume = ((gSramBuff + in_buff_size + in_buff_u_size + in_buff_v_size + out_buff_size - lastAddress) + (SRAM_BASE + lEncStatus.mOutCurrentAddress - (gSramBuff + in_buff_size + in_buff_u_size + in_buff_v_size)));
              lastAddress         = SRAM_BASE + lEncStatus.mOutCurrentAddress;
            } // if(SRAM_BASE + lEncStatus.mOutCurrentAddress < lastAddress)

            final_length += segment_data_volume;
            segment_data_volume = 0;
          }// if((SRAM_BASE + lEncStatus.mOutCurrentAddress != lastAddress)
        } // for(int n = 0; n < (IMAGE_HEIGHT/m_LINES_NUMBER); n ++ )
    
      FrameIrqDone(); 
      H264ENC_StatusGet(lEncStatus);
      
      GETTIME(&lTimeEnd);
      lTimeDiff = lTimeEnd-lTimeStart;
      
      encoded_data_length = final_length;
      onEncode(pDst, final_length, lTimeDiff);
      isWorking = false;
      condition.notify_one();
    }
}
void H264Encoder::setOnEncoderDoneListener(const EncoderDoneListener listener)
{
    onEncode.connect(listener);
}

void H264Encoder::EncoderFrame(unsigned char* pDst, unsigned char* pFrame, bool wait)
{
    if(isWorking) //return if encoding is not done in async mode 
    {
      return;
    }
    global_pDst = pDst;
    
    
    if(wait)
    {
        global_pFrame = pFrame;
        
        start_sem.post();
        isWorking = true;
        
        boost::mutex::scoped_lock lock(io_mutex);
        while(isWorking) condition.wait(lock);
    }
    else
    {
        memcpy(global_pFrame, pFrame, m_IMAGE_HEIGHT * m_IMAGE_WIDTH * 3);
        start_sem.post();
        isWorking = true;
    }
    
    
    
    
    
} 

int H264Encoder::EncoderLastFrame(unsigned char* pDst)
{
  unsigned long long segment_data_volume = 0;
  
  Last_frame_wait();

  int final_length = 0, x = 0;
  if(SRAM_BASE + lEncStatus.mLastFrmEndAddress > lastAddress)
  {
    
    for(uint32_t i = 0; i < (SRAM_BASE + lEncStatus.mLastFrmEndAddress - lastAddress); i ++) //end of the circular buffer
    {
      // if output buffer fullf
      pDst[x] = lpSramBuff[(lastAddress - (gSramBuff))  + i];
      x++;
    } 

    segment_data_volume = SRAM_BASE + lEncStatus.mLastFrmEndAddress - lastAddress;
    lastAddress         = SRAM_BASE + lEncStatus.mLastFrmEndAddress;  
  } //if(SRAM_BASE + lEncStatus.mOutCurrentAddress > lastAddress)   
  
  if(SRAM_BASE + lEncStatus.mLastFrmEndAddress < lastAddress)
  {  
    for(uint32_t i = 0; i < (gSramBuff + m_IN_BUFF_SIZE + m_IN_BUFF_U_SIZE + m_IN_BUFF_V_SIZE + m_OUT_BUFF_SIZE - lastAddress); i ++) //end of the circular buffer
    {  
      pDst[x] = lpSramBuff[(lastAddress - (gSramBuff))  + i];
      x++;
    }         
    for(uint32_t i = 0; i < (SRAM_BASE + lEncStatus.mLastFrmEndAddress - (gSramBuff + m_IN_BUFF_SIZE + m_IN_BUFF_U_SIZE + m_IN_BUFF_V_SIZE)); i++ ) //beginning of the circular buffer
    {    
      pDst[x] = lpSramBuff[m_OUT_BUFF_OFF + i];
      x++;
    }             

    segment_data_volume = ((gSramBuff + m_IN_BUFF_SIZE + m_IN_BUFF_U_SIZE + m_IN_BUFF_V_SIZE + m_OUT_BUFF_SIZE - lastAddress) + (SRAM_BASE + lEncStatus.mLastFrmEndAddress - (gSramBuff + m_IN_BUFF_SIZE + m_IN_BUFF_U_SIZE + m_IN_BUFF_V_SIZE)));
    lastAddress         = SRAM_BASE + lEncStatus.mLastFrmEndAddress;  
  } // if(SRAM_BASE + lEncStatus.mOutCurrentAddress < lastAddress)    
  
  final_length += segment_data_volume;
  return final_length;
}

void H264Encoder::Close()
{
    can_work = false;
    start_sem.post();
    
    run_t_.join();
    
    printf("\n \nEncoding DONE \n");
    printf("Irqs total:   %d \n", lEncStatus.mIrqCounter);
    printf("Frames total: %d \n", lEncStatus.mFrameCounter);
    printf("Irqs / frame: %d \n\n", (lEncStatus.mIrqCounter / lEncStatus.mFrameCounter));

    H264ENC_StatusGet(lEncStatus);
    printf("Enc ISR: 0x%x\n", lEncStatus.mInterruptStatus);
  
    lEncStatus.mInLineCounter = 0;
    H264ENC_SwReset();
    H264ENC_ResetVars();

    if(lpSramBuffHandle != NULL)
    {
      OAL_MemoryFree(lpSramBuffHandle);
    } // if SRAM allocated

    HwUnmap();
}

    

