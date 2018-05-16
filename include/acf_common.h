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
#ifndef ACFCOMMON_H
#define ACFCOMMON_H

#include <icp_datatype.h>
#include <stdint.h>

//This file contains structures/entities in C that are meant to be shared between the OFFLINE world and the HOST world

//HW-specific (used only by 'custom resize' right now)
#define SEQ_RESOURCE_INTERRUPT_PROGRAM_DONE 30

//ACF ERROR CODES
enum
{
   ACF_SUCCESS = 0,
   ACF_ERROR,

   ACF_MEMORY_ALLOCATION_ERROR,
   ACF_HW_DETECTION_ERROR,
   ACF_HW_RESOURCE_ERROR,
   ACF_SCENARIO_DETECTION_ERROR,
   ACF_APU_ERROR,
   ACF_APEX_INIT_ERROR
};

typedef struct _e0_desc
{
   icp::DATATYPE dataType;
   uint16_t x;
   uint16_t y;
} e0_desc;

typedef struct _ek_desc
{
   uint16_t x;
   uint16_t y;
} ek_desc;

typedef struct _ed_desc
{
   uint16_t x;
   uint16_t y;
} ed_desc;

typedef struct _sd_desc
{
   uint16_t n;
   uint16_t s;
   uint16_t e;
   uint16_t w;
} sd_desc;

typedef struct _chunk_desc
{
   uint16_t x;
   uint16_t y;
} chunk_desc;

typedef struct _ACF_LocalMemDesc
{
   e0_desc e0;
   uint16_t chunkWidth;   //in units of e0
   uint16_t chunkHeight;  //in units of e0
   uint16_t chunkSpan;    //number of bytes to skip to reach next row of bytes (this is the standard definition of span)
   uint32_t chunkE0Span;  //number of bytes to skip to reach next row of e0 elements
   uint32_t bufferOffset; //number of bytes to skip to reach next buffer
   sd_desc padding;       //in units of e0
   uint16_t padOffsetR;
   uint16_t padOffsetB;
   uint16_t numBuffers;
   uint16_t verDepOverlapFlag;   //set to 1 if vertical spatial dependencies are handled by overlapping (instead of circular buffering)
   uint16_t verDepOverlapOffset; //number of bytes to jump from the top of the buffer (i.e. pBuf0) to the start of the core 'chunkHeight' of data
   uint16_t bufferHeight;        //in units of e0 (this will differ from chunkHeight if verDepOverlapFlag is 1 since the height of the buffer will include the chunk height + top and bottom padding)
   void* pBuf0;
   void* pTop;
   void* pBot;
   int16_t pairedLmIndex; //used for advanced cases (e.g. FIFO-like case where there is a need to pair a CMEM region with a DMEM region)
}ACF_LocalMemDesc;

typedef struct _acf_io_desc //io info required at run-time from host
{
   int width;       //in units of e0
   int height;      //in units of e0
   int span;        //number of bytes to skip to reach next row of bytes (this is the standard definition of span)
   int e0span;      //number of bytes to skip to reach next row of e0 elements
   int physAddr;    //physical addr to start of data (for DMA)
   int tileOffset;  //number of bytes to jump to get to the next tile

   //ROI specific members
   //int roiIterOffset; //number of iterations to wait before starting this input transfer to ensure that 'core' tiles are lined up properly across all inputs (relative to all other inputs)
   //int roiNumIter;    //total number of iterations (including core tiles and any additional tiles on the top/bottom required to satisfy ROI requirements)
   //int roiCuOffset;   //index of the 'start' CU where the full 'ROI tile' is written (not necessarily = to the global 'core' data CU offset)

} acf_io_desc;

typedef struct _acf_io_mce_desc //mce io info required at run-time from host
{
   int offsetArrayWidth;  //width of pointer/offset array in 'offsets'
   int offsetArrayHeight; //height of pointer/offset array
   int tileOffset;        //number of bytes to jump to get to next 'tile' of offset ptrs
   int srcDataSpan;       //span of source data
   int physPtrArrayAddr;  //physical addr to start of array of ptrs (for MCE)
   int physBaseAddr;      //physical address to be added to each ptr in the array
} acf_io_mce_desc;

typedef struct _acf_scenario_buffer_data
{
   uint16_t chunkWidth;
   uint16_t chunkHeight;
   uint16_t numBuffers;
   uint16_t writeDelay; //delay until buffer starts being filled (in iterations)
   sd_desc padding;
} acf_scenario_buffer_data;

typedef struct _acf_scenario_kernel_data
{
   uint16_t executionStartDelay;
   uint16_t extraIterToSatisfyOd;
} acf_scenario_kernel_data;

typedef struct _acf_scenario
{
   uint16_t baseChunkWidth;
   uint16_t baseChunkHeight;
   uint16_t cmemReq;    //total cmem required (in bytes)
   uint32_t dmemReq;    //total dmem required (in bytes)
   uint16_t iterAdjustment;
   acf_scenario_buffer_data* pBufferData;
   uint16_t scenarioBufferDataSizeInBytes;
   acf_scenario_kernel_data* pKernelData;
   uint16_t scenarioKernelDataSizeInBytes;
} acf_scenario;

typedef struct _acf_scenario_list
{
   int16_t numScenarios;            //the number of scenarios in the list 'pScenarioList'
   acf_scenario* pScenarioList;
} acf_scenario_list;

//structure to encapsulate worst case APEX resource set (SIZE IN BYTES MUST BE DIVISIBLE BY 4!!!)
typedef struct _acf_res_info
{
   int8_t numDmaChIn;
   int8_t dmaChIn[8];

   int8_t numStreamInChForDma;
   int8_t streamInChForDma[4];

   int8_t numDmaChOut;
   int8_t dmaChOut[8];

   int8_t numStreamOutChForDma;
   int8_t streamOutChForDma[2];

   int8_t numMceChIn;
   int8_t mceChIn[2];

   int8_t numStreamInForMce;
   int8_t streamInChForMce[2];

   int8_t numFixedSeqRes;
   int8_t fixedSeqRes[20];
   int8_t numProcIntSeqRes;
   int8_t procIntSeqRes[3];
   int8_t zeroSeqRes_Done;
   int8_t procIdSeqRes; //the 'master' performing proc increments/decrements and seq enable/disable (can be 0-5)
   int8_t apuId; //321 can be 0, 642 can be 0 or 1

   //int8_t lDummy[<num dummy bytes>]; //pad out so the struct size is a multiple of 4 bytes
} acf_res_info;

//struct for collecting run-time stats for process execution on apu (all values in units of apu cycles)
struct ApuPerfInfoStruct {
   //stuff captured by default
   int32_t init;
   int32_t processing; //kernel execution + padding + cb management
   int32_t idle;
   int32_t total;
   //more detailed items (not captured unless enabled by developer)
   int32_t kernel;
   int32_t padPlusCb;
   int32_t linkedListManagement;
   int32_t dummyItem;
};

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

int32_t RetE0DataTypeSizeInBytes(e0_desc* lpE0);
int32_t RetE0WidthInBytes(e0_desc* lpE0);
int32_t RetE0SizeInBytes(e0_desc* lpE0);
e0_desc RetE0Desc(icp::DATATYPE lDataType, int lX, int lY);
int32_t E0Compare(e0_desc* lpE0_A, e0_desc* lpE0_B);

//---------------------
//INLINE FUNCTIONS
//---------------------

#ifdef CGV_FORCE_INLINE
#define CGV_INLINE_PREFIX inline
#else
#define CGV_INLINE_PREFIX
#endif

CGV_INLINE_PREFIX int RetLocalMemDescSizeInBytes(ACF_LocalMemDesc* lpLM);
CGV_INLINE_PREFIX int RetBufferIndex(ACF_LocalMemDesc* lpLM, int lIter);
CGV_INLINE_PREFIX void* RetAddr_iter(ACF_LocalMemDesc* lpLM, int lIter);
CGV_INLINE_PREFIX void* RetAddr_cbindex(ACF_LocalMemDesc* lpLM, int16_t lCbIndex);
CGV_INLINE_PREFIX void CbIndexIncrement(int16_t* lpIndex, uint16_t lNumBuffers);

#ifdef CGV_FORCE_INLINE
#include "../libs/apex/acf/include/acf_common_inline.h" //include implementations for inline functions
#endif

#ifdef __cplusplus
}
#endif //__cplusplus


#endif /* ACFCOMMON_H */
