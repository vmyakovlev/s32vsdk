/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2014 Freescale Semiconductor;
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

#include <linked_list_utilities.h>
#include <icp_datatype.h>
#include <mcdma_hal.h>
#include <stdint.h>
#include <string.h>

int32_t CalcOptimalWordSize(int32_t lPhysAddr, int32_t lSpan, int32_t lBytesPerLine)
{
   int32_t lWordSize = DMA_BURST_BEAT_BYTE_SIZE;
   int lTemp = (lPhysAddr|lSpan|lBytesPerLine) & 0xF;

   if (lTemp & 1)
      lWordSize = DMA_BURST_BEAT_BYTE_SIZE;
   else if (lTemp & 2)
      lWordSize = DMA_BURST_BEAT_HALFWORD_SIZE;
   else if (lTemp & 4)
      lWordSize = DMA_BURST_BEAT_WORD_SIZE;
   else if (lTemp & 8)
      lWordSize = DMA_BURST_BEAT_DWORD_SIZE;
   else
      lWordSize = DMA_BURST_BEAT_DDWORD_SIZE;

   return lWordSize;
}

int32_t DoesChunkWidthSatisfyStreamHwLimits(int32_t lChunkWidthInBytes)
{
   int32_t lRetVal = 0;

   if (lChunkWidthInBytes <= 32)
   {
      if (0 == (lChunkWidthInBytes%2))
         lRetVal = 1;
   }
   else if (lChunkWidthInBytes <= 64)
   {
      if (0 == (lChunkWidthInBytes%4))
         lRetVal = 1;
   }
   else if (lChunkWidthInBytes <= 128)
   {
      if (0 == (lChunkWidthInBytes%8))
         lRetVal = 1;
   }

   return lRetVal;
}

int32_t DoesChunkWidthSatisfyMceHwLimits(int32_t lChunkWidthInBytes)
{
   int32_t lRetVal = 0;

   if (lChunkWidthInBytes <=64)
   {
      if (0 == (lChunkWidthInBytes%4))
         lRetVal = 1;
   }

   return lRetVal;
}

void LLE_2DDMATransferConfig_XMEM_CMEM (uint32_t* llBuffer, //size must be >=24 words
                                        icp::DATATYPE elementDataType,
                                        int32_t elementDimX,
                                        int32_t elementDimY,
                                        int32_t srcAddr,
                                        int32_t dstAddr,
                                        int32_t span,        //src span if XMEM->CMEM, dst span if CMEM->XMEM (in bytes)
                                        int32_t width,       //src width if XMEM->CMEM, dst width if CMEM->XMEM (in elements)
                                        int32_t chunkWidth,  //in elements
                                        int32_t chunkHeight, //in elements
                                        int32_t chunkSpan,   //in bytes
                                        int32_t cmemAddr,
                                        int32_t startCu)
{
   int32_t lTransferToCmemFlag = 0;

   //calculate values required for descriptor configuration
   int32_t lElementWidthInBytes = RetDataTypeSizeInBytes(elementDataType)*elementDimX;
   int32_t lChunkWidthInBytes = chunkWidth*lElementWidthInBytes;
   int32_t lBytesPerLine      = width*lElementWidthInBytes;
   int32_t lNumLines          = chunkHeight*elementDimY;
   int32_t lBinWordSize       = 0;
   int32_t lEntriesPerBin     = 0;
   int32_t lSliceIncr         = 0;
   int8_t  lWordEnable[4]     = {0};

   if (dstAddr == DST_CMEM_DMA_STREAM_IN0 || dstAddr == DST_CMEM_DMA_STREAM_IN1 || dstAddr == DST_CMEM_DMA_STREAM_IN2 || dstAddr == DST_CMEM_DMA_STREAM_IN3)
      lTransferToCmemFlag = 1;

   if (lChunkWidthInBytes%8 == 0)
   {
      lBinWordSize = 2; //indicates 64-bit
      lEntriesPerBin = (lChunkWidthInBytes>>3);
      lSliceIncr = 4;
      lWordEnable[0] = 1;
      lWordEnable[1] = 1;
      lWordEnable[2] = 1;
      lWordEnable[3] = 1;
   }
   else if (lChunkWidthInBytes%4 == 0)
   {
      lBinWordSize = 1; //indicates 32-bit
      lEntriesPerBin = (lChunkWidthInBytes>>2);
      lSliceIncr = 2;
      lWordEnable[0] = 1;
      lWordEnable[1] = 1;
   }
   else if (lChunkWidthInBytes%2 == 0)
   {
      lBinWordSize = 0; //indicates 16-bit
      lEntriesPerBin = (lChunkWidthInBytes>>1);
      lSliceIncr = 1;
      lWordEnable[0] = 1;
   }

   //if the bytes per line ==  src/dst span, use 1D (linear) instead of 2D for better performance
   DMA_FORMAT lDmaFormat = (lBytesPerLine == span) ? LINEAR : U2D;

   //******************************************
   //START POPULATING LINKED LIST ENTRY
   //******************************************
   
   if (lTransferToCmemFlag == 1)
   {
      int32_t lWordSize = CalcOptimalWordSize(srcAddr, span, lBytesPerLine);

      //ENTRY_0     src addr
      llBuffer[0] = srcAddr;
      
      //ENTRY_1     periph enable | periph fifo flush | sideband cfg | dst periph port
      llBuffer[1] = 1 << 31       | 1 << 30           | 0xFFFF << 4  | dstAddr << 0;
      
      //ENTRY_2     pause   | int enable | loop on last | last    | burst size | word size      | src xfer type | src addr mode | src data format | dst xfer type | dst addr mode | dst data format
      llBuffer[2] = 0 << 31 | 0 << 30    | 0 << 29      | 0 << 28 | 15 << 11   | lWordSize << 8 | 0 << 7        | 0 << 6        | lDmaFormat << 4 | 1 << 3        | 0 << 2        | 0 << 0;
      //                                                            16_BEAT                       mem based       direct addr                       periph based    direct addr     1D
      
      //ENTRY_4     src span   | dst span
      llBuffer[4] = span << 16 | 0 << 0;
   }
   else
   {
      int32_t lWordSize = CalcOptimalWordSize(dstAddr, span, lBytesPerLine);

      //ENTRY_0     periph enable | periph fifo flush | sideband cfg | src periph port
      llBuffer[0] = 1 << 31       | 1 << 30           | 0xFFFF << 4  | srcAddr << 0;
      
      //ENTRY_1     dst addr
      llBuffer[1] = dstAddr;
      
      //ENTRY_2     pause   | int enable | loop on last | last    | burst size | word size      | src xfer type | src addr mode | src data format | dst xfer type | dst addr mode | dst data format
      llBuffer[2] = 0 << 31 | 0 << 30    | 0 << 29      | 0 << 28 | 15 << 11   | lWordSize << 8 | 1 << 7        | 0 << 6        | 0 << 4          | 0 << 3        | 0 << 2        | lDmaFormat << 0;
      //                                                            16_BEAT                       periph based    direct addr     1D                mem based       direct addr
      
      //ENTRY_4     src span | dst span
      llBuffer[4] = 0 << 16  | span << 0;
   }
   
   //ENTRY_3     frame size (in bytes)
   llBuffer[3] = lBytesPerLine*lNumLines;
   
   //ENTRY_5     tile row count | tile width
   llBuffer[5] = 0 << 16        | lBytesPerLine << 0;
   
   //ENTRY_6 (not used in 2D configuration)
   llBuffer[6] = 0x00000000;
   
   //NXT_LL
   llBuffer[7] = 24*4; //even in sequential mode it is necessary to provide an offset to the next entry

   //******************************************
   //STREAM CONFIG (14 words)
   //******************************************
   
   //STR_CFG_REG0 (TRANSFER CFG) base address 1      | base address 0      | cu start (**NOTE: cmemAddr is 16-bit!)
   llBuffer[8] =                 (cmemAddr>>1) << 20 | (cmemAddr>>1) << 8  | startCu << 0;
   
   //STR_CFG_REG1 (ADDRESS OFFSET) bytes per line        | num lines (n-1)    | bin word size       | entries per bin (n-1)
   llBuffer[9] =                   (lBytesPerLine) << 16 | (lNumLines-1) << 6 | (lBinWordSize) << 4 | (lEntriesPerBin-1) << 0;
   
   //STR_CFG_REG2 (MULTISCAN CFG1) 2D increment | addr inc 1 | addr inc 0
   llBuffer[10] =                  0;
   
   //STR_CFG_REG3 (MULTISCAN CFG2) 2D count (n-1) | first addr 1 | first addr 0
   llBuffer[11] =                  0;
   
   //STR_CFG_REG4 (MULTISCAN CFG3) scan order | multiscan mode | max buf size 1 | max buf size 0
   llBuffer[12] =                  0;
   
   //STR_CFG_REG5 (WORD0 CFG) swap enable | word enable          | line mode | line increment      | slice increment  | base addr offset
   llBuffer[13] =             0 << 31     | lWordEnable[0] << 30 | 0 << 28   | (chunkSpan/2) << 16 | lSliceIncr << 11 | 0 << 0;
   
   //STR_CFG_REG6 (WORD1 CFG) swap enable | word enable          | line mode | line increment      | slice increment  | baseAddressOffset
   llBuffer[14] =             0 << 31     | lWordEnable[1] << 30 | 0 << 28   | (chunkSpan/2) << 16 | lSliceIncr << 11 | 1 << 0;

   //STR_CFG_REG7 (WORD2 CFG) swap enable | word enable          | line mode | line increment      | slice increment  | baseAddressOffset
   llBuffer[15] =             0 << 31     | lWordEnable[2] << 30 | 0 << 28   | (chunkSpan/2) << 16 | lSliceIncr << 11 | 2 << 0;

   //STR_CFG_REG8 (WORD3 CFG) swap enable | word enable          | line mode | line increment      | slice increment  | baseAddressOffset
   llBuffer[16] =             0 << 31     | lWordEnable[3] << 30 | 0 << 28   | (chunkSpan/2) << 16 | lSliceIncr << 11 | 3 << 0;

   //STR_CFG_REG9 (CU CFG) bytes per CU (n-1)
   llBuffer[17] =          (lChunkWidthInBytes-1);
   
   //STR_CFG_REG10 (MUXSEL CFG0) word3_b1_grp0 | word3_b0_grp0 | word2_b1_grp0 | word2_b0_grp0 | word1_b1_grp0 | word1_b0_grp0 | word0_b1_grp0 | word0_b0_grp0
   llBuffer[18] =                7 << 28       | 6 << 24       | 5 << 20       | 4 << 16       | 3 << 12       | 2 << 8        | 1 << 4        | 0 << 0;
   
   //STR_CFG_REG11 (MUXSEL CFG1) word3_b1_grp1 | word3_b0_grp1 | word2_b1_grp1 | word2_b0_grp1 | word1_b1_grp1 | word1_b0_grp1 | word0_b1_grp1 | word0_b0_grp1
   llBuffer[19] =                15 << 28      | 14 << 24      | 13 << 20      | 12 << 16      | 11 << 12      | 10 << 8       | 9 << 4        | 8 << 0;

   //STR_CFG_REG12 (CU SPAN0) cu enable for cus 0-31
   llBuffer[20] =             0xFFFFFFFF;
   
   //STR_CFG_REG13 (CU SPAN1) cu enable for cus 32-63
   llBuffer[21] =             0xFFFFFFFF;
   
   //NOTE: linked list entry size is 24 words (not 22) to maintain 128-bit alignment (i.e. there are 2 dummy words following the last word)
}

void LLE_2DDMATransferConfig_XMEM_HRSZ (uint32_t* llBuffer,
                                        icp::DATATYPE elementDataType,
                                        int32_t elementDimX,
                                        int32_t elementDimY,
                                        int32_t srcAddr,
                                        int32_t dstAddr,
                                        int32_t span,
                                        int32_t width,
                                        int32_t height,
                                        uint32_t *pHrszConfig)
{
   int32_t lTransferToHrszFlag = 0;

   //calculate values required for descriptor configuration
   int32_t lChunkWidthInBytes = width*RetDataTypeSizeInBytes(elementDataType)*elementDimX;
   int32_t lBytesPerLine      = lChunkWidthInBytes;
   int32_t lNumLines          = height*elementDimY;
   /*int32_t lBinWordSize       = 0;*/
   /*int32_t lEntriesPerBin     = 0;*/
   /*int32_t lSliceIncr         = 0;*/
   /*int8_t  lWordEnable[4]     = {0};*/

   if (dstAddr == DST_HRSZ0 || dstAddr == DST_HRSZ0)
      lTransferToHrszFlag = 1;

   if (lChunkWidthInBytes%8 == 0)
   {
      /*lBinWordSize = 2; //indicates 64-bit*/
      /*lEntriesPerBin = (lChunkWidthInBytes>>3);*/
      /*lSliceIncr = 4;*/
      /*lWordEnable[0] = 1;
      lWordEnable[1] = 1;
      lWordEnable[2] = 1;
      lWordEnable[3] = 1;*/
   }
   else if (lChunkWidthInBytes%4 == 0)
   {
      /*lBinWordSize = 1; //indicates 32-bit*/
      /*lEntriesPerBin = (lChunkWidthInBytes>>2);*/
      /*lSliceIncr = 2;*/
      /*lWordEnable[0] = 1;
      lWordEnable[1] = 1;*/
   }
   else if (lChunkWidthInBytes%2 == 0)
   {
      /*lBinWordSize = 0; //indicates 16-bit*/
      /*lEntriesPerBin = (lChunkWidthInBytes>>1);*/
      /*lSliceIncr = 1;*/
      /*lWordEnable[0] = 1;*/
   }

   //******************************************
   //START POPULATING LINKED LIST ENTRY
   //******************************************
   
   if (lTransferToHrszFlag == 1)
   {
      int32_t lWordSize = CalcOptimalWordSize(srcAddr, span, lBytesPerLine);

      //ENTRY_0     periph enable | sideband cfg | src periph port
      llBuffer[0] = srcAddr;
      
      //ENTRY_1     periph enable | sideband cfg | dst periph port
      llBuffer[1] = 1 << 31       | 0x7FFFFFF << 4  | dstAddr << 0;
      
      //ENTRY_2     pause   | int enable | loop on last | last    | burst size | word size      | src xfer type | src addr mode | src data format | dst xfer type | dst addr mode | dst data format
      llBuffer[2] = 0 << 31 | 0 << 30    | 0 << 29      | 0 << 28 | 15 << 11   | lWordSize << 8 | 0 << 7        | 0 << 6        | 1 << 4          | 1 << 3        | 0 << 2        | 0 << 0;
      //                                                            16_BEAT                       mem based       direct addr     2D                periph based    direct addr     1D
      
      //ENTRY_4     src span   | dst span
      llBuffer[4] = span << 16 | 0 << 0;
   }
   else
   {
      int32_t lWordSize = CalcOptimalWordSize(dstAddr, span, lBytesPerLine);

      //ENTRY_0     periph enable | sideband cfg | src periph port
      llBuffer[0] = 1 << 31       | 0x7FFFFFF << 4  | srcAddr << 0;
      
      //ENTRY_1     periph enable | sideband cfg | dst periph port
      llBuffer[1] = dstAddr;
      
      //ENTRY_2     pause   | int enable | loop on last | last    | burst size | word size      | src xfer type | src addr mode | src data format | dst xfer type | dst addr mode | dst data format
      llBuffer[2] = 0 << 31 | 0 << 30    | 0 << 29      | 0 << 28 | 15 << 11   | lWordSize << 8 | 1 << 7        | 0 << 6        | 0 << 4          | 0 << 3        | 0 << 2        | 1 << 0;
      //                                                            16_BEAT                       periph based    direct addr     1D                mem based       direct addr     2D
      
      //ENTRY_4     src span | dst span
      llBuffer[4] = 0 << 16  | span << 0;
   }
   
   //ENTRY_3     frame size (in bytes)
   llBuffer[3] = lBytesPerLine*lNumLines;
   
   //ENTRY_5     tile row count | tile width
   llBuffer[5] = 0 << 16        | lBytesPerLine << 0;
   
   //ENTRY_6 (not used in 2D configuration)
   llBuffer[6] = 0x00000000;
   
   //NXT_LL
   llBuffer[7] = 8*4 + LLE_SIZE_HRSZ_SIDEBAND*4; //even in sequential mode it is necessary to provide an offset to the next entry
   
   //******************************************
   //HRSZ CONFIG 
   //******************************************
   memcpy(&(llBuffer[8]), pHrszConfig, LLE_SIZE_HRSZ_SIDEBAND * sizeof(int32_t));
}

void LLE_2DDMATransferConfig_CMEM_HRSZ (uint32_t* llBuffer,
                                        icp::DATATYPE elementDataType,
                                        int32_t elementDimX,
                                        int32_t elementDimY,
                                        int32_t srcAddr,
                                        int32_t dstAddr,
                                        int32_t imageWidth,
                                        int32_t /*tileWidthInChunks*/,
                                        int32_t chunkWidth,
                                        int32_t chunkHeight,
                                        int32_t chunkSpan,
                                        int32_t cmemAddr,
                                        int32_t startCu,
                                        uint32_t *pHrszConfig)
{
   int32_t lTransferToHrszFlag = 0;
   int32_t llBufferPos = 0;

   //calculate values required for descriptor configuration
   int32_t lChunkWidthInBytes = chunkWidth*RetDataTypeSizeInBytes(elementDataType)*elementDimX;
//   int32_t lBytesPerLine      = lChunkWidthInBytes*tileWidthInChunks;
   int32_t lBytesPerLine      = imageWidth;
   int32_t lNumLines          = chunkHeight*elementDimY;
   int32_t lBinWordSize       = 0;
   int32_t lEntriesPerBin     = 0;
   int32_t lSliceIncr         = 0;
   int8_t  lWordEnable[4]     = {0};

   if (dstAddr == DST_HRSZ0 || dstAddr == DST_HRSZ0)
      lTransferToHrszFlag = 1;

   if (lChunkWidthInBytes%8 == 0)
   {
      lBinWordSize = 2; //indicates 64-bit
      lEntriesPerBin = (lChunkWidthInBytes>>3);
      lSliceIncr = 4;
      lWordEnable[0] = 1;
      lWordEnable[1] = 1;
      lWordEnable[2] = 1;
      lWordEnable[3] = 1;
   }
   else if (lChunkWidthInBytes%4 == 0)
   {
      lBinWordSize = 1; //indicates 32-bit
      lEntriesPerBin = (lChunkWidthInBytes>>2);
      lSliceIncr = 2;
      lWordEnable[0] = 1;
      lWordEnable[1] = 1;
   }
   else if (lChunkWidthInBytes%2 == 0)
   {
      lBinWordSize = 0; //indicates 16-bit
      lEntriesPerBin = (lChunkWidthInBytes>>1);
      lSliceIncr = 1;
      lWordEnable[0] = 1;
   }

   //******************************************
   //START POPULATING LINKED LIST ENTRY
   //******************************************
   
   if (lTransferToHrszFlag == 1)
   {
      // Ignore address alignment in word size calculation, since no XMEM here.
//      int32_t lWordSize = CalcOptimalWordSize(0, span, lBytesPerLine);
      int32_t lWordSize = CalcOptimalWordSize(0, 0, lBytesPerLine);

      //ENTRY_0     periph enable | sideband cfg | src periph port
      llBuffer[0] = 1 << 31       | 0xFFFF << 4  | srcAddr << 0;

      // See ALGO-61 implementation 2b
      //ENTRY_1     periph enable | sideband cfg | dst periph port
////      llBuffer[1] = 1 << 31       | 0x7FFFFFF << 4  | dstAddr << 0;
//      llBuffer[1] = 1 << 31       | 0x7FFFFFF << 4  | dstAddr << 0;
      llBuffer[1] = 1 << 31   | 0 << 4 | dstAddr << 0;      // Disable sideband config of HRSZ
      
      //ENTRY_2     pause   | int enable | loop on last | last    | burst size | word size      | src xfer type | src addr mode | src data format | dst xfer type | dst addr mode | dst data format
      llBuffer[2] = 0 << 31 | 0 << 30    | 0 << 29      | 0 << 28 | 15 << 11   | lWordSize << 8 | 1 << 7        | 0 << 6        | 0 << 4          | 1 << 3        | 0 << 2        | 0 << 0;
      //                                                            16_BEAT                       periph based    direct addr     1D                periph based    direct addr     1D
      
      //ENTRY_4     src span   | dst span
//      llBuffer[4] = span << 16 | 0 << 0;
      llBuffer[4] = 0 << 16 | 0 << 0;
   }
   else
   {
      // Ignore address alignment in word size calculation, since no XMEM here.
//      int32_t lWordSize = CalcOptimalWordSize(0, span, lBytesPerLine);
      int32_t lWordSize = CalcOptimalWordSize(0, 0, lBytesPerLine);

      // See ALGO-61 implementation 1a
      //ENTRY_0     periph enable | sideband cfg | src periph port
//      llBuffer[0] = 1 << 31       | 0x7FFFFFF << 4  | srcAddr << 0;
      // NB See ALGO-61 HRSZ sideband is in XMEM->MCDMA->HRSZ sideband.  No sideband here.
      llBuffer[0] = 0 << 31       | 0x0 << 4  | srcAddr << 0;
      
      //ENTRY_1     periph enable | sideband cfg | dst periph port
      llBuffer[1] = 1 << 31       | 0xFFFF << 4  | dstAddr << 0;
      
      //ENTRY_2     pause   | int enable | loop on last | last    | burst size | word size      | src xfer type | src addr mode | src data format | dst xfer type | dst addr mode | dst data format
      llBuffer[2] = 0 << 31 | 0 << 30    | 0 << 29      | 0 << 28 | 15 << 11   | lWordSize << 8 | 1 << 7        | 0 << 6        | 0 << 4          | 1 << 3        | 0 << 2        | 0 << 0;
      //                                                            16_BEAT                       periph based    direct addr     1D                periph based    direct addr     1D
      
      //ENTRY_4     src span | dst span
//      llBuffer[4] = 0 << 16  | span << 0;
      llBuffer[4] = 0 << 16  | 0 << 0;
   }
   
   //ENTRY_3     frame size (in bytes)
   llBuffer[3] = lBytesPerLine*lNumLines;
   
   //ENTRY_5     tile row count | tile width
   llBuffer[5] = 0 << 16        | lBytesPerLine << 0;
   
   //ENTRY_6 (not used in 2D configuration)
   llBuffer[6] = 0x00000000;
   
   //NXT_LL
   if (lTransferToHrszFlag == 1) {
      // 24 due to 4-word alignment
      llBuffer[7] = 24*4 + LLE_SIZE_HRSZ_SIDEBAND*4; //even in sequential mode it is necessary to provide an offset to the next entry
   } else {
      // 24 due to 4-word alignment
      /*24*4;*/ //even in sequential mode it is necessary to provide an offset to the next entry
   }
   
   // Set next buffer position
   llBufferPos = 8;
   
   //******************************************
   //HRSZ CONFIG 
   //******************************************
   // only present if this transfer is an INPUT to HRSZ
   // NB: No space between STREAM sideband and HRSZ sideband (alignment concerns?  todo.)
//  Trying 2b (see ALGO-61)   
   if (lTransferToHrszFlag == 1)
   {
      memcpy(&(llBuffer[llBufferPos]), pHrszConfig, LLE_SIZE_HRSZ_SIDEBAND * sizeof(int32_t));
      
      llBufferPos += LLE_SIZE_HRSZ_SIDEBAND;
   }


   // STREAM
   //******************************************
   //STREAM CONFIG (14 words)
   //******************************************
   
   //STR_CFG_REG0 (TRANSFER CFG) base address 1      | base address 0      | cu start (**NOTE: cmemAddr is 16-bit!)
   llBuffer[llBufferPos++] =                 (cmemAddr>>1) << 20 | (cmemAddr>>1) << 8  | startCu << 0;
   
   //STR_CFG_REG1 (ADDRESS OFFSET) bytes per line        | num lines (n-1)    | bin word size       | entries per bin (n-1)
   llBuffer[llBufferPos++] =                   (lBytesPerLine) << 16 | (lNumLines-1) << 6 | (lBinWordSize) << 4 | (lEntriesPerBin-1) << 0;
   
   //STR_CFG_REG2 (MULTISCAN CFG1) 2D increment | addr inc 1 | addr inc 0
   llBuffer[llBufferPos++] =                  0;
   
   //STR_CFG_REG3 (MULTISCAN CFG2) 2D count (n-1) | first addr 1 | first addr 0
   llBuffer[llBufferPos++] =                  0;
   
   //STR_CFG_REG4 (MULTISCAN CFG3) scan order | multiscan mode | max buf size 1 | max buf size 0
   llBuffer[llBufferPos++] =                  0;
   
   //STR_CFG_REG5 (WORD0 CFG) swap enable | word enable          | line mode | line increment      | slice increment  | base addr offset
   llBuffer[llBufferPos++] =             0 << 31     | lWordEnable[0] << 30 | 0 << 28   | (chunkSpan/2) << 16 | lSliceIncr << 11 | 0 << 0;
   
   //STR_CFG_REG6 (WORD1 CFG) swap enable | word enable          | line mode | line increment      | slice increment  | baseAddressOffset
   llBuffer[llBufferPos++] =             0 << 31     | lWordEnable[1] << 30 | 0 << 28   | (chunkSpan/2) << 16 | lSliceIncr << 11 | 1 << 0;

   //STR_CFG_REG7 (WORD2 CFG) swap enable | word enable          | line mode | line increment      | slice increment  | baseAddressOffset
   llBuffer[llBufferPos++] =             0 << 31     | lWordEnable[2] << 30 | 0 << 28   | (chunkSpan/2) << 16 | lSliceIncr << 11 | 2 << 0;

   //STR_CFG_REG8 (WORD3 CFG) swap enable | word enable          | line mode | line increment      | slice increment  | baseAddressOffset
   llBuffer[llBufferPos++] =             0 << 31     | lWordEnable[3] << 30 | 0 << 28   | (chunkSpan/2) << 16 | lSliceIncr << 11 | 3 << 0;

   //STR_CFG_REG9 (CU CFG) bytes per CU (n-1)
   llBuffer[llBufferPos++] =          (lChunkWidthInBytes-1);
   
   //STR_CFG_REG10 (MUXSEL CFG0) word3_b1_grp0 | word3_b0_grp0 | word2_b1_grp0 | word2_b0_grp0 | word1_b1_grp0 | word1_b0_grp0 | word0_b1_grp0 | word0_b0_grp0
   llBuffer[llBufferPos++] =                7 << 28       | 6 << 24       | 5 << 20       | 4 << 16       | 3 << 12       | 2 << 8        | 1 << 4        | 0 << 0;
   
   //STR_CFG_REG11 (MUXSEL CFG1) word3_b1_grp1 | word3_b0_grp1 | word2_b1_grp1 | word2_b0_grp1 | word1_b1_grp1 | word1_b0_grp1 | word0_b1_grp1 | word0_b0_grp1
   llBuffer[llBufferPos++] =                15 << 28      | 14 << 24      | 13 << 20      | 12 << 16      | 11 << 12      | 10 << 8       | 9 << 4        | 8 << 0;

   //STR_CFG_REG12 (CU SPAN0) cu enable for cus 0-31
   llBuffer[llBufferPos++] =             0xFFFFFFFF;
   
   //STR_CFG_REG13 (CU SPAN1) cu enable for cus 32-63
   llBuffer[llBufferPos++] =             0xFFFFFFFF;
}

void LLE_2DDMATransferConfig_MEM_MEM (uint32_t* llBuffer, //size must be >=8 words
                                      icp::DATATYPE elementDataType,
                                      int32_t elementDimX,
                                      int32_t elementDimY,
                                      int32_t srcAddr,
                                      int32_t dstAddr,
                                      int32_t srcSpan,
                                      int32_t dstSpan,
                                      int32_t chunkWidth,
                                      int32_t chunkHeight)
{
   //calculate values required for descriptor configuration
   int32_t lChunkWidthInBytes = chunkWidth*RetDataTypeSizeInBytes(elementDataType)*elementDimX;
   int32_t lBytesPerLine      = lChunkWidthInBytes;
   int32_t lNumLines          = chunkHeight*elementDimY;

   int32_t lWordSize = CalcOptimalWordSize(srcAddr|dstAddr, srcSpan|dstSpan, lBytesPerLine);

   //if the bytes per line ==  src/dst span, use 1D (linear) instead of 2D for better performance
   DMA_FORMAT lDmaFormatSrc = (lBytesPerLine == srcSpan) ? LINEAR : U2D;
   DMA_FORMAT lDmaFormatDst = (lBytesPerLine == dstSpan) ? LINEAR : U2D;

   //******************************************
   //START POPULATING LINKED LIST ENTRY
   //******************************************

   //ENTRY_0     src addr
   llBuffer[0] = srcAddr;

   //ENTRY_1     dst addr
   llBuffer[1] = dstAddr;

   //ENTRY_2     pause   | int enable | loop on last | last    | burst size | word size      | src xfer type | src addr mode | src data format    | dst xfer type | dst addr mode | dst data format
   llBuffer[2] = 0 << 31 | 0 << 30    | 0 << 29      | 0 << 28 | 15 << 11   | lWordSize << 8 | 0 << 7        | 0 << 6        | lDmaFormatSrc << 4 | 0 << 3        | 0 << 2        | lDmaFormatDst << 0;
   //                                                            16_BEAT      <depends>        mem based       direct addr                          mem based       direct addr

   //ENTRY_3     frame size (in bytes)
   llBuffer[3] = lBytesPerLine*lNumLines;

   //ENTRY_4     src span      | dst span
   llBuffer[4] = srcSpan << 16 | dstSpan << 0;

   //ENTRY_5     tile row count | tile width
   llBuffer[5] = 0 << 16        | lBytesPerLine << 0;

   //ENTRY_6 (not used in 2D configuration)
   llBuffer[6] = 0x00000000;

   //NXT_LL
   llBuffer[7] = 8*4; //even in sequential mode it is necessary to provide an offset to the next entry
}

void LLE_3DDMATransferConfig_MEM_MEM ( uint32_t* llBuffer, //size must be >=8 words
                                       icp::DATATYPE elementDataType,
                                       int32_t elementDimX,
                                       int32_t elementDimY,
                                       int32_t srcAddr,        // XMEM_Addr
                                       int32_t dstAddr,        // XMEM_Addr
                                       int32_t ptrArrayAddr,   // APPLY ON XMEM_Addr
                                       int32_t ptrOffsetVal,   // APPLY ON XMEM_Addr
                                       int32_t span,           // APPLY ON XMEM_Addr
                                       int32_t tileWidthInChunks,
                                       int32_t chunkWidth,
                                       int32_t chunkHeight,
                                       int32_t /*chunkSpan*/,
                                       uint8_t lTransferToDmemFlag,
                                       uint8_t lastFlag)
{
   //calculate values required for descriptor configuration
   int32_t lChunkWidthInBytes = chunkWidth*RetDataTypeSizeInBytes(elementDataType)*elementDimX;
   /*int32_t lBytesPerLine      = lChunkWidthInBytes;*/
   int32_t lNumLines          = chunkHeight*elementDimY;
   

   //******************************************
   //START POPULATING LINKED LIST ENTRY
   //******************************************
   
   if (lTransferToDmemFlag == 1)
   {
      //ENTRY_0     src addr
      llBuffer[0] = srcAddr;
      
      //ENTRY_1     periph enable | periph fifo flush | sideband cfg | dst periph port
      llBuffer[1] = 0 << 31       | 0 << 30           | 0x0000 << 4  | dstAddr << 0;
      
      //ENTRY_2     pause   | int enable | loop on last | last    | burst size | word size | src xfer type | src addr mode | src data format | dst xfer type | dst addr mode | dst data format
      llBuffer[2] = 0 << 31 | 0 << 30    | 0 << 29      | 0 << 28 | 15 << 11   | 0 << 8    | 0 << 7        | 0 << 6        | 2 << 4          | 0 << 3        | 0 << 2        | 0 << 0;
      //                                                            16_BEAT      8_BIT       mem based       direct addr     3D                mem based       direct addr     1D
      
      //ENTRY_4     src span   | dst span  (for ND)
      llBuffer[4] = span << 16 | 0 << 0;
   }
   else
   {
      //ENTRY_0     periph enable | periph fifo flush | sideband cfg | src periph port
      llBuffer[0] = 0 << 31       | 0 << 30           | 0x0000 << 4  | srcAddr << 0;
      
      //ENTRY_1     dst addr
      llBuffer[1] = dstAddr;
      
      //ENTRY_2     pause   | int enable | loop on last | last    | burst size | word size | src xfer type | src addr mode | src data format | dst xfer type | dst addr mode | dst data format
      llBuffer[2] = 0 << 31 | 0 << 30    | 0 << 29      | 0 << 28 | 15 << 11   | 0 << 8    | 0 << 7        | 0 << 6        | 0 << 4          | 0 << 3        | 0 << 2        | 2 << 0;
      //                                                            16_BEAT      8_BIT       mem based      direct addr     1D                mem based       direct addr     3D
      
      //ENTRY_4     src span    | dst span (both for 2D transfer)
      llBuffer[4] = 0 << 16     | span << 0;
   }
   
   if (1 == lastFlag) // the last transfer
   {
      llBuffer[2] |= 0xD0000000; //pause & interrupt enable & last
   }
   
   //ENTRY_3     the row and rows/columns of tiles (3D/4D) this count is the number of bytes in each tile
   // llBuffer[3] = lBytesPerLine*lNumLines;
   llBuffer[3] = lChunkWidthInBytes*lNumLines;
  
   //ENTRY_5 
   //Tile Row count (N – 1):
   //This register specifies the number of tiles per row to transfer
   //and is used in 3D and 4D transfers.
   //Tile Width (N):
   //This counter is used for 2D and higher transfers 
   //and represents the width of the 2D image or the width of a tile for 3D and 4D transfers.
   //ENTRY_5     tile row count                       | tile width  
   llBuffer[5] = (tileWidthInChunks - 1) << 16        | lChunkWidthInBytes << 0;
   //llBuffer[5] = ( - 1) << 16        | lChunkWidthInBytes << 0;
   
   //ENTRY_6 (not used in 2D configuration) --> use to store 'backup' of frame size
   //ENTRY_6 Macro_Block Address[31:2] Offset pointer.
   // A pointer to memory where the table of offsets is located
   
   if (ptrArrayAddr != 0)
   {
      //Pointer to table of offsets
      //[Note]
      // Addr:   32-bit aligned
      // Offset: The first entry in the table is the offset to the 2nd tile.
      int32_t alignAddr = (ptrArrayAddr >> 2);
      //ENTRY_6     Macro_Block Offset pointer     | offset(0)/absolute(1)    | offset value (0)/ pointer to table (1)
      llBuffer[6] = alignAddr << 2              | 0 << 1                   | 1 << 0;
   }
   else
   {
      int32_t alignVal = (ptrOffsetVal >> 2);
      // regular 3D with (signed) row tile offset (number of bytes)
      //ENTRY_6     Row tile offset                | offset(0)/absolute(1)    | offset value (0)/ pointer to table (1)
      llBuffer[6] = alignVal << 2         | 0 << 1                   | 0 << 0;
   }
   
   //NXT_LL
   llBuffer[7] = 24*4; //even in sequential mode it is necessary to provide an offset to the next entry
}

void LLE_3DDMATransferConfig (uint32_t* llBuffer, //size must be >=24 words
                              icp::DATATYPE elementDataType,
                              int32_t elementDimX,
                              int32_t elementDimY,
                              int32_t srcAddr,        // XMEM_Addr/ STREAM_OUT0
                              int32_t dstAddr,        // STREAM_IN0/ STREAM_IN1/ STREAM_IN2/ STREAM_IN3/ XMEM_Addr
                              int32_t ptrArrayAddr,   // APPLY ON XMEM_Addr
                              int32_t ptrOffsetVal,   // APPLY ON XMEM_Addr
                              int32_t span,           // APPLY ON XMEM_Addr
                              int32_t tileWidthInChunks,
                              int32_t chunkWidth,
                              int32_t chunkHeight,
                              int32_t chunkSpan,
                              int32_t cmemAddr,
                              int32_t startCu,
                              int32_t maskCu_00, int32_t maskCu_32,
                              uint8_t lastFlag)
{
   int32_t lTransferToCmemFlag = 0;

   //calculate values required for descriptor configuration
   int32_t lChunkWidthInBytes = chunkWidth*RetDataTypeSizeInBytes(elementDataType)*elementDimX;
   int32_t lBytesPerLine      = lChunkWidthInBytes*tileWidthInChunks;
   int32_t lNumLines          = chunkHeight*elementDimY;
   int32_t lBinWordSize       = 0;
   int32_t lEntriesPerBin     = 0;

   int32_t lSliceIncr         = 0;
   int8_t  lWordEnable[4]     = {0};

   if (dstAddr == DST_CMEM_DMA_STREAM_IN0 || dstAddr == DST_CMEM_DMA_STREAM_IN1 || dstAddr == DST_CMEM_DMA_STREAM_IN2 || dstAddr == DST_CMEM_DMA_STREAM_IN3)
      lTransferToCmemFlag = 1;

   if (lChunkWidthInBytes%8 == 0)
   {
      lBinWordSize = 2; //indicates 64-bit
      lEntriesPerBin = lChunkWidthInBytes>>3;
      lSliceIncr = 4;
      lWordEnable[0] = 1;
      lWordEnable[1] = 1;
      lWordEnable[2] = 1;
      lWordEnable[3] = 1;
   }
   else if (lChunkWidthInBytes%4 == 0)
   {
      lBinWordSize = 1; //indicates 32-bit
      lEntriesPerBin = lChunkWidthInBytes>>2;
      lSliceIncr = 2;
      lWordEnable[0] = 1;
      lWordEnable[1] = 1;
   }
   else if (lChunkWidthInBytes%2 == 0)
   {
      lBinWordSize = 0; //indicates 16-bit
      lEntriesPerBin = lChunkWidthInBytes>>1;
      lSliceIncr = 1;
      lWordEnable[0] = 1;
   }

   //******************************************
   //START POPULATING LINKED LIST ENTRY
   //******************************************
   
   if (lTransferToCmemFlag == 1)
   {
      //ENTRY_0     src addr
      llBuffer[0] = srcAddr;
      
      //ENTRY_1     periph enable | periph fifo flush | sideband cfg | dst periph port
      llBuffer[1] = 1 << 31       | 1 << 30           | 0xFFFF << 4  | dstAddr << 0;
      
      //ENTRY_2     pause   | int enable | loop on last | last    | burst size | word size | src xfer type | src addr mode | src data format | dst xfer type | dst addr mode | dst data format
      llBuffer[2] = 0 << 31 | 0 << 30    | 0 << 29      | 0 << 28 | 15 << 11   | 0 << 8    | 0 << 7        | 0 << 6        | 2 << 4          | 1 << 3        | 0 << 2        | 0 << 0;
      //                                                            16_BEAT      8_BIT       mem based       direct addr     3D                periph based    direct addr     1D
      
      //ENTRY_4     src span   | dst span  (for ND)
      llBuffer[4] = span << 16 | 0 << 0;
   }
   else
   {
      //ENTRY_0     periph enable | periph fifo flush | sideband cfg | src periph port
      llBuffer[0] = 1 << 31       | 1 << 30           | 0xFFFF << 4  | srcAddr << 0;
      
      //ENTRY_1     dst addr
      llBuffer[1] = dstAddr;
      
      //ENTRY_2     pause   | int enable | loop on last | last    | burst size | word size | src xfer type | src addr mode | src data format | dst xfer type | dst addr mode | dst data format
      llBuffer[2] = 0 << 31 | 0 << 30    | 0 << 29      | 0 << 28 | 15 << 11   | 0 << 8    | 1 << 7        | 0 << 6        | 0 << 4          | 0 << 3        | 0 << 2        | 2 << 0;
      //                                                            16_BEAT      8_BIT       periph based    direct addr     1D                mem based       direct addr     3D
      
      //ENTRY_4     src span    | dst span (both for 2D transfer)
      llBuffer[4] = 0 << 16     | span << 0;
   }
   
   if (1 == lastFlag) // the last transfer
   {
      llBuffer[2] |= 0xD0000000; //pause & interrupt enable & last
   }
   
   //ENTRY_3     the row and rows/columns of tiles (3D/4D) this count is the number of bytes in each tile
   // llBuffer[3] = lBytesPerLine*lNumLines;
   llBuffer[3] = lChunkWidthInBytes*lNumLines;
  
   //ENTRY_5 
   //Tile Row count (N – 1):
   //This register specifies the number of tiles per row to transfer
   //and is used in 3D and 4D transfers.
   //Tile Width (N):
   //This counter is used for 2D and higher transfers 
   //and represents the width of the 2D image or the width of a tile for 3D and 4D transfers.
   //ENTRY_5     tile row count                       | tile width  
   llBuffer[5] = (tileWidthInChunks - 1) << 16        | lChunkWidthInBytes << 0;
   //llBuffer[5] = ( - 1) << 16        | lChunkWidthInBytes << 0;
   
   //ENTRY_6 (not used in 2D configuration) --> use to store 'backup' of frame size
   //ENTRY_6 Macro_Block Address[31:2] Offset pointer.
   // A pointer to memory where the table of offsets is located
   
   if (ptrArrayAddr != 0)
   {
      //Pointer to table of offsets
      //[Note]
      // Addr:   32-bit aligned
      // Offset: The first entry in the table is the offset to the 2nd tile.
      int32_t alignAddr = (ptrArrayAddr >> 2);
      //ENTRY_6     Macro_Block Offset pointer     | offset(0)/absolute(1)    | offset value (0)/ pointer to table (1)
      llBuffer[6] = alignAddr << 2              | 0 << 1                   | 1 << 0;
   }
   else
   {
      int32_t alignVal = (ptrOffsetVal >> 2);
      // regular 3D with (signed) row tile offset (number of bytes)
      //ENTRY_6     Row tile offset                | offset(0)/absolute(1)    | offset value (0)/ pointer to table (1)
      llBuffer[6] = alignVal << 2         | 0 << 1                   | 0 << 0;
   }
   
   //NXT_LL
   llBuffer[7] = 24*4; //even in sequential mode it is necessary to provide an offset to the next entry

   //******************************************
   //STREAM CONFIG (14 words)
   //******************************************
   
   //STR_CFG_REG0 (TRANSFER CFG) base address 1      | base address 0      | cu start (**NOTE: cmemAddr is 16-bit!)
   llBuffer[8] =                 (cmemAddr>>1) << 20 | (cmemAddr>>1) << 8  | startCu << 0;
   
   //STR_CFG_REG1 (ADDRESS OFFSET) bytes per line        | num lines (n-1)    | bin word size       | entries per bin (n-1)
   llBuffer[9] =                   (lBytesPerLine) << 16 | (lNumLines-1) << 6 | (lBinWordSize) << 4 | (lEntriesPerBin-1) << 0;
   
   //STR_CFG_REG2 (MULTISCAN CFG1) 2D increment | addr inc 1 | addr inc 0
   llBuffer[10] =                  0;
   
   //STR_CFG_REG3 (MULTISCAN CFG2) 2D count (n-1) | first addr 1 | first addr 0
   llBuffer[11] =                  0;
   
   //STR_CFG_REG4 (MULTISCAN CFG3) scan order | multiscan mode | max buf size 1 | max buf size 0
   llBuffer[12] =                  0;
   
   //STR_CFG_REG5 (WORD0 CFG) swap enable | word enable | line mode | line increment      | slice increment | base addr offset    
   llBuffer[13] =             0 << 31     | lWordEnable[0] << 30 | 0 << 28   | (chunkSpan/2) << 16        | lSliceIncr << 11 | 0 << 0;
   
   //STR_CFG_REG6 (WORD1 CFG) swap enable | word enable | line mode | line increment      | slice increment | baseAddressOffset
   llBuffer[14] =             0 << 31     | lWordEnable[1] << 30 | 0 << 28   | (chunkSpan/2) << 16        | lSliceIncr << 11 | 1 << 0;

   //STR_CFG_REG7 (WORD2 CFG) swap enable | word enable | line mode | line increment      | slice increment | baseAddressOffset
   llBuffer[15] =             0 << 31     | lWordEnable[2] << 30 | 0 << 28   | (chunkSpan/2) << 16        | lSliceIncr << 11 | 2 << 0;

   //STR_CFG_REG8 (WORD3 CFG) swap enable | word enable | line mode | line increment      | slice increment | baseAddressOffset
   llBuffer[16] =             0 << 31     | lWordEnable[3] << 30 | 0 << 28   | (chunkSpan/2) << 16        | lSliceIncr << 11 | 3 << 0;

   //STR_CFG_REG9 (CU CFG) bytes per CU (n-1)
   llBuffer[17] =          (lChunkWidthInBytes-1);
   
   //STR_CFG_REG10 (MUXSEL CFG0) word3_b1_grp0 | word3_b0_grp0 | word2_b1_grp0 | word2_b0_grp0 | word1_b1_grp0 | word1_b0_grp0 | word0_b1_grp0 | word0_b0_grp0
   llBuffer[18] =                7 << 28       | 6 << 24       | 5 << 20       | 4 << 16       | 3 << 12       | 2 << 8        | 1 << 4        | 0 << 0;
   
   //STR_CFG_REG11 (MUXSEL CFG1) word3_b1_grp1 | word3_b0_grp1 | word2_b1_grp1 | word2_b0_grp1 | word1_b1_grp1 | word1_b0_grp1 | word0_b1_grp1 | word0_b0_grp1
   llBuffer[19] =                15 << 28      | 14 << 24      | 13 << 20      | 12 << 16      | 11 << 12      | 10 << 8       | 9 << 4        | 8 << 0;

   //STR_CFG_REG12 (CU SPAN0) cu enable for cus 0-31
   llBuffer[20] =             maskCu_00;  //0xFFFFFFFF;

   
   //STR_CFG_REG13 (CU SPAN1) cu enable for cus 32-63
   llBuffer[21] =             maskCu_32;//0xFFFFFFFF;
   /*
   if (maskCu_32 != 0)  {
      llBuffer[21] = maskCu_32;
   }
   */
   
   
   //NOTE: linked list entry size is actually 24 words (not 22) to maintain 128-bit alignment (i.e. there are 2 dummy words following the last word)
}

void LLE_MCETransferConfig (uint32_t* llBuffer, //size must be >=24 words
                            icp::DATATYPE elementDataType,
                            int32_t elementDimX,
                            int32_t elementDimY,
                            int32_t baseAddr,
                            int32_t ptrArrayAddr,
                            int32_t dstAddr,
                            int32_t srcDataSpan,
                            int32_t tileWidthInChunks,
                            int32_t chunkWidth,
                            int32_t chunkHeight,
                            int32_t chunkSpan,
                            int32_t cmemAddr,
                            int32_t startCu,
                            int32_t maskCu_00,
                            int32_t maskCu_32)
{
   //calculate values required for descriptor configuration
   int32_t lChunkWidthInBytes = chunkWidth*RetDataTypeSizeInBytes(elementDataType)*elementDimX;
   int32_t lBytesPerLine      = lChunkWidthInBytes*tileWidthInChunks;
   int32_t lNumLines          = chunkHeight*elementDimY;
   int32_t lBinWordSize       = 0;
   int32_t lEntriesPerBin     = 0;
   int32_t lSliceIncr         = 0;
   int8_t  lWordEnable[4]     = {0};

   if (lChunkWidthInBytes%8 == 0)
   {
      lBinWordSize = 2; //indicates 64-bit
      lEntriesPerBin = (lChunkWidthInBytes>>3);
      lSliceIncr = 4;
      lWordEnable[0] = 1;
      lWordEnable[1] = 1;
      lWordEnable[2] = 1;
      lWordEnable[3] = 1;
   }
   else if (lChunkWidthInBytes%4 == 0)
   {
      lBinWordSize = 1; //indicates 32-bit
      lEntriesPerBin = (lChunkWidthInBytes>>2);
      lSliceIncr = 2;
      lWordEnable[0] = 1;
      lWordEnable[1] = 1;
   }
   else if (lChunkWidthInBytes%2 == 0)
   {
      //this case is not supported by the MCE (block width must be a multiple of 4 bytes!!)
   }

   //******************************************
   //START POPULATING LINKED LIST ENTRY
   //******************************************

   //ENTRY_0     pause   | int enable | loop on last | last    | num words per pointer (n-1)       | number of lines (n-1) | number of GOCs (n-1)
   llBuffer[0] = 0 << 31 | 0 << 30    | 0 << 29      | 0 << 28 | ((lChunkWidthInBytes>>2)-1) << 23 | (lNumLines-1) << 11   | ((tileWidthInChunks>>2)-1) << 1;

   //ENTRY_1     pointer increment to advance to next line of block
   llBuffer[1] = srcDataSpan;
   
   
   //ENTRY_2     pointer to table of pointers
   llBuffer[2] = ptrArrayAddr;
   
   //ENTRY_3     signed offset applied to pointers in pointer table
   llBuffer[3] = baseAddr; //NOTE: it is assumed base addr is 32-bit aligned (bit0=0 -> apply new offset, bit0=1 -> use prev captured offset)
   
   //STR_CFG     periph enable | periph fifo flush | sideband cfg | dst periph port
   llBuffer[4] = 1 << 31       | 1 << 30           | 0xFFFF << 4  | dstAddr << 0;  //dstAddr must be STREAM_IN0 for MCE0 or STREAM_IN1 for MCE1

   //NOTE: words 5 and 6 are dummy words present for 128-bit alignment

   //NXT_LL
   llBuffer[7] = 24*4; //even in sequential mode it is necessary to provide an offset to the next entry

   //******************************************
   //STREAM CONFIG (14 words)
   //******************************************
   
   //STR_CFG_REG0 (TRANSFER CFG) base address 1      | base address 0      | cu start (**NOTE: cmemAddr is 16-bit!)
   llBuffer[8] =                 (cmemAddr>>1) << 20 | (cmemAddr>>1) << 8  | startCu << 0;
   
   //NOTE: 'bytes per line' is actually the number of bytes for an entire tile (and number of lines = 1) when feeding the STI with the MCE.
   //      Since there is only one line, the 'line increment' below is set to 0.
   
   //STR_CFG_REG1 (ADDRESS OFFSET) bytes per line                  | num lines (n-1) | bin word size       | entries per bin (n-1)
   llBuffer[9] =                   (lBytesPerLine*lNumLines) << 16 | (1-1) << 6      | (lBinWordSize) << 4 | (lEntriesPerBin-1) << 0;
   
   //NOTE: 2D increment -> CMEM addr increment applied after bytesPerCu bytes have been written to each CU (should be cmem chunk span)

   //STR_CFG_REG2 (MULTISCAN CFG1) 2D increment         | addr inc 1 | addr inc 0
   llBuffer[10] =                  (chunkSpan>>1) << 24 | 0 << 12    | 0 << 0; //NOTE: "chunkSpan>>1" because cmemAddr is now 16-bit here!!!!
   
   //NOTE: 2D count -> this is how many times STIO will write bytesPerCu to each of the 4CUs in the GOC
   //      rather than immediately moving on to the next GOC (should be number of lines)
   
   //STR_CFG_REG3 (MULTISCAN CFG2) 2D count (n-1)      | first addr 1 | first addr 0
   llBuffer[11] =                  (lNumLines-1) << 24 | 0 << 12      | 0 << 0;
   
   //STR_CFG_REG4 (MULTISCAN CFG3) scan order | multiscan mode | max buf size 1 | max buf size 0
   llBuffer[12] =                  0;
   
   //STR_CFG_REG5 (WORD0 CFG) swap enable | word enable          | line mode | line increment | slice increment  | base addr offset
   llBuffer[13] =             0 << 31     | lWordEnable[0] << 30 | 0 << 28   | 0 << 16        | lSliceIncr << 11 | 0 << 0;
   
   //STR_CFG_REG6 (WORD1 CFG) swap enable | word enable          | line mode | line increment | slice increment  | baseAddressOffset
   llBuffer[14] =             0 << 31     | lWordEnable[1] << 30 | 0 << 28   | 0 << 16        | lSliceIncr << 11 | 1 << 0;

   //STR_CFG_REG7 (WORD2 CFG) swap enable | word enable          | line mode | line increment | slice increment  | baseAddressOffset
   llBuffer[15] =             0 << 31     | lWordEnable[2] << 30 | 0 << 28   | 0 << 16        | lSliceIncr << 11 | 2 << 0;

   //STR_CFG_REG8 (WORD3 CFG) swap enable | word enable          | line mode | line increment | slice increment  | baseAddressOffset
   llBuffer[16] =             0 << 31     | lWordEnable[3] << 30 | 0 << 28   | 0 << 16        | lSliceIncr << 11 | 3 << 0;

   //STR_CFG_REG9 (CU CFG) bytes per CU (n-1)
   llBuffer[17] =          (lChunkWidthInBytes-1);
   
   //STR_CFG_REG10 (MUXSEL CFG0) word3_b1_grp0 | word3_b0_grp0 | word2_b1_grp0 | word2_b0_grp0 | word1_b1_grp0 | word1_b0_grp0 | word0_b1_grp0 | word0_b0_grp0
   llBuffer[18] =                7 << 28       | 6 << 24       | 5 << 20       | 4 << 16       | 3 << 12       | 2 << 8        | 1 << 4        | 0 << 0;
   
   //STR_CFG_REG11 (MUXSEL CFG1) word3_b1_grp1 | word3_b0_grp1 | word2_b1_grp1 | word2_b0_grp1 | word1_b1_grp1 | word1_b0_grp1 | word0_b1_grp1 | word0_b0_grp1
   llBuffer[19] =                15 << 28      | 14 << 24      | 13 << 20      | 12 << 16      | 11 << 12      | 10 << 8       | 9 << 4        | 8 << 0;

   //STR_CFG_REG12 (CU SPAN0) cu enable for cus 0-31
   llBuffer[20] =             maskCu_00;
   
   //STR_CFG_REG13 (CU SPAN1) cu enable for cus 32-63
   llBuffer[21] =             maskCu_32;
   
   //NOTE: linked list entry size is  24 words (not 22) to maintain 128-bit alignment (i.e. there are 2 dummy words following the last word)
}

void LLE_1D_Fill (uint32_t* llBuffer,  //size must be >=8 words
                  uint32_t dstAddr,    //physical destination address DMA will write to
                  int32_t fillSizeInBytes)
{
   int32_t lWordSize = CalcOptimalWordSize(dstAddr, 0, fillSizeInBytes);

   //ENTRY_0     periph enable | sideband cfg | src periph port (memfill0)
   llBuffer[0] = 1 << 31       | 0 << 4       | 7 << 0;

   //ENTRY_1         dst addr
   llBuffer[1] = dstAddr;

   //ENTRY_2     pause   | int enable | loop on last | last    | burst size | word size      | src xfer type | src addr mode | src data format | dst xfer type | dst addr mode | dst data format
   llBuffer[2] = 0 << 31 | 0 << 30    | 0 << 29      | 0 << 28 | 15 << 11   | lWordSize << 8 | 1 << 7        | 0 << 6        | 0 << 4          | 0 << 3        | 0 << 2        | 0 << 0;
   //                                                            16_BEAT      <depends>        periph based    direct addr     1D                mem based       direct addr     1D

   //ENTRY_3     frame size (in bytes)
   llBuffer[3] = fillSizeInBytes;

   //ENTRY_4 (not used in 1D configuration)
   llBuffer[4] = 0;

   //ENTRY_5 (not used in 1D configuration)
   llBuffer[5] = 0;

   //ENTRY_6 (not used in 1D configuration)
   llBuffer[6] = 0;

   //NXT_LL
   llBuffer[7] = 8*4;
}

void LLE_1D_MEM_MEM (uint32_t* llBuffer,  //size must be >=8 words
                     uint32_t srcAddr,    //physical source address DMA will read from
                     uint32_t dstAddr,    //physical destination address DMA will write to
                     int32_t transferSizeInBytes)
{
   int32_t lWordSize = CalcOptimalWordSize(srcAddr|dstAddr, 0, transferSizeInBytes);

   //******************************************
   //START POPULATING LINKED LIST ENTRY
   //******************************************

   //ENTRY_0     src addr
   llBuffer[0] = srcAddr;

   //ENTRY_1     dst addr
   llBuffer[1] = dstAddr;

   //ENTRY_2     pause   | int enable | loop on last | last    | burst size | word size      | src xfer type | src addr mode | src data format | dst xfer type | dst addr mode | dst data format
   llBuffer[2] = 0 << 31 | 0 << 30    | 0 << 29      | 0 << 28 | 15 << 11   | lWordSize << 8 | 0 << 7        | 0 << 6        | 0 << 4          | 0 << 3        | 0 << 2        | 0 << 0;
   //                                                            16_BEAT      <depends>        mem based       direct addr     1D                mem based       direct addr     1D

   //ENTRY_3     frame size (in bytes)
   llBuffer[3] = transferSizeInBytes;

   //ENTRY_4 (not used in 1D configuration)
   llBuffer[4] = 0;

   //ENTRY_5 (not used in 1D configuration)
   llBuffer[5] = 0;

   //ENTRY_6 (not used in 1D configuration)
   llBuffer[6] = 0;

   //NXT_LL
   llBuffer[7] = 8*4;
}

void LLE_DMA_Finalize(uint32_t* llEntry)
{
   llEntry[2] |= 0xD0000000; //set 'pause', 'interrupt enable', and 'last' bits
}

void LLE_MCE_Finalize(uint32_t* llEntry)
{
   llEntry[0] |= 0xD0000000; //set 'pause', 'interrupt enable', and 'last' bits
}

void LLE_DMA_SetBurstToINCR1(uint32_t* llEntry)
{
   llEntry[2] &= 0xFFFF87FF;
   llEntry[2] |= (INCR1<<11);
}
