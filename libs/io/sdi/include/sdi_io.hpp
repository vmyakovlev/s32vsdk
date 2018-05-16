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
/**
 * \file      sdi_io.hpp
 * \brief     Declaration of SDI input/output objects.
 * \author    Tomas Babinec
 * \version   0.1
 * \date      11-November-2014
 ****************************************************************************/

#ifndef SDIIO_HPP
#define SDIIO_HPP

#include <stdint.h>
#include <vector>

//#include "oal_task.hpp"
#include "../../../utils/oal/user/include/oal_process.h"
#include "../../../isp/fdma/kernel/include/fdma_types.h"
#include "sdi_types.hpp"
#include "sdi_graph.hpp"
#include "../../../../isp/inc/seq_graph.h"

#include "../../../isp/sequencer/kernel/include/seq_types.h"
#include "isp_csi.h"
#include "isp_viu.h"
#include "isp_jpegdec.h"
#include "isp_h264enc.h"
#include "isp_h264dec.h"
#include "umat.hpp"

#include <map>
#include <list>

using namespace std;

//****************************************************************************
// contant definitions
//****************************************************************************
#define SDI_JPEG_BANK_STRIDE_MAX  7
#define SDI_JPEG_RST_INTERVAL_MAX 0xffffUL

//****************************************************************************
// types
//****************************************************************************

//
// preliminary class sdi declarations
class sdi;
class sdi_FdmaIO;
class sdi_grabber;

/***************************************************************************/
/** Structure describing one Frame for SDI use.
 *
 ***************************************************************************/
struct SDI_Frame
{
  vsdk::UMat mUMat;     ///< image container
  
  friend sdi_grabber;
  
  uint32_t mFrmSeqNum;  ///< frame sequence number

private:
   uint32_t mChannelIdx; ///< index of related FDMA channel
   uint32_t mBufferIdx;  ///< index of the buffer in the buffer array for particular channel
}; // SDI_DdrBufList


/***************************************************************************/
/** Structure describing ddr buffer array.
 *
 ***************************************************************************/
struct SDI_DdrBufferArr
{
  uint32_t            mFdmaTcIdx;  ///< index of FDMA TC
  uint32_t            mCnt;        ///< number buffers in the array
  vsdk::UMat         *mpUmat;
  bool                mDeallocate; ///< true if to be deallocated automatically
  
  friend sdi_FdmaIO;

public:
  
  SDI_ImageDescriptor mDesc;       ///< buffer image descriptor
  
  /***************************************************************************/
  /** Simple constructor. Address array left empty.
   ***************************************************************************/
  SDI_DdrBufferArr(void);
  
  /***************************************************************************/
  /** Copy constructor. 
   * 
   * Buffer addresses copied from the source object.
   * 
   * \param arSrc source object
   ***************************************************************************/
  SDI_DdrBufferArr(const SDI_DdrBufferArr &arSrc);
  
  /***************************************************************************/
  /** Destructor.
   ***************************************************************************/
  ~SDI_DdrBufferArr(void);
  
  /***************************************************************************/
  /** Allocates DDR buffers described by mDesc and mCnt members.
   *
   * \param  arDesc image descriptor
   * \param  aCnt   number of buffers to be allocated
   * 
   * \return LIB_SUCCESS if all ok
   *         LIB_FAILURE if failed
   ***************************************************************************/
  LIB_RESULT Allocate(SDI_ImageDescriptor &arDesc, uint32_t aCnt);
  
  /***************************************************************************/
  /** Frees allocated memory.
   * 
   * Includes also the DDR buffers if Allocate function was used befor. For
   * buffers provided through Set method only the pointers are removed.
   ***************************************************************************/
  void Free(void);
  
  /***************************************************************************/
  /** Gets index of corresponding FDMA channel.
   * 
   * \return index of corresponding FDMA channel,
   *         FDMA_CHANNELS if no channel assigned
   ***************************************************************************/
  uint32_t ChannelIdxGet(void);
  
  /***************************************************************************/
  /** Sets index of corresponding FDMA channel.
   * 
   * \param aFdmaChannelIndex index of the FDMA channel to be assigned
   * 
   * \return LIB_SUCCESS if ok,
   *         LIB_FAILURE if index out of bounds (>= FDMA_CHANNELS)
   ***************************************************************************/
  LIB_RESULT ChannelIdxSet(uint32_t aFdmaChannelIndex);
  
  /***************************************************************************/
  /** Gets number of contained buffers.
   * 
   * \return number of contained buffers.
   ***************************************************************************/
  uint32_t BufferCntGet(void);
  
  /***************************************************************************/
  /** Gets UMat with requested buffer index.
   * 
   * \return image container with mData member not NULL if buffer exists
   *         otherwise the mData member is set to NULL
   ***************************************************************************/
  vsdk::UMat Query(uint32_t aIdx);
  
  /***************************************************************************/
  /** Generates SDI_DdrBufferArr that points to a roi of this array.
   * 
   * \param  aX horizontal shift to the first pixel of the roi
   * \param  aY vertical shift to the first pixel of the roi
   * \param  aW width of the roi in pixels
   * \param  aH height of the roi in pixels
   * 
   * \return SDI_DdrBufferArr object pointing to the ROI
   ***************************************************************************/
  SDI_DdrBufferArr Roi(const uint32_t aX,
                       const uint32_t aY,
                       const uint32_t aW,
                       const uint32_t aH
                      );
  
  /***************************************************************************/
  /** Rmembers new set of virtual addresses.
   * Internaly also the physical addresses are requested and remembered.
   * 
   * \param arDesc            image descriptor
   * \param apVirtAddressArr  array of virual addresses to be remembered
   * \param aCnt              number of addresses in the array
   *
   * \return LIB_SUCCESS if all ok
   *         LIB_FAILURE if failed
   ***************************************************************************/
  LIB_RESULT AddressArrSet(SDI_ImageDescriptor &arDesc, 
                           vsdk::UMat *apUMatArr, 
                           uint32_t aCnt);
     
}; // SDI_DdrBufList

/***************************************************************************/
/** Defines possible IO object states.
 *
 ***************************************************************************/
enum sdi_IoState{
                  OFF,     ///< not reserved yet
                  RESERVED,///< reserved
                  SETUP,   ///< Setup was done
                  ON       ///< Start was invoked
}; // enum sdi_IoState

/***************************************************************************/
/** Class describing a general input or output object.
 * Virtual class.
 *
 ***************************************************************************/
class sdi_io
{
protected:  
  SEQ_Othr_ix_t mTypeIndex; ///< which IO engine type the object stands for
  sdi_graph    *mpGraph;    ///< to what graph this io belongs to
  sdi_IoState   mState;     ///< State of the io object
  LIB_RESULT    mResult;    ///< result of last operation with the object
  
  // TODO: move bool reserved array to shared mem and index it by sensory type ID
  bool mReserved;           ///< is this io reserved? 

  friend class sdi;
    
public:
  int32_t mHeadIndex; ///< index of the corresponding head from a packed graph
  
  /***************************************************************************/
  /** Constructor.
   *
   * Sets members to default values.
   ***************************************************************************/
  sdi_io(void);
  
  /***************************************************************************/
  /** Constructor.
   * 
   * \param aIOtype index of the IO type
   *
   * Sets members to default values.
   ***************************************************************************/
  sdi_io(SEQ_Othr_ix_t aIOtype);
  
  /***************************************************************************/
  /** Constructor.
   * 
   * \param aIOtype index of the IO type
   * \param apGraph the graph this IO belongs to
   *
   * Sets members to default values.
   ***************************************************************************/
  sdi_io(SEQ_Othr_ix_t aIOtype, sdi_graph *apGraph);
  
  /***************************************************************************/
  /** Reserves IO device.
   *
   * HW specific reserve functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  virtual LIB_RESULT Reserve(void) = 0;
  
  /***************************************************************************/
  /** Releases IO device.
   *
   * HW specific release functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  virtual LIB_RESULT Release(void) = 0;
  
  /***************************************************************************/
  /** Setup IO device.
   *
   * HW specific setup functionality, if any. (to be invoked by SDI library)
   * Applies all previously set parameters to the reserved HW block.
   ***************************************************************************/
  virtual LIB_RESULT Setup(void) = 0;
  
  /***************************************************************************/
  /** Starts the IO device operation.
   *
   * HW specific functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  virtual LIB_RESULT Start(void) = 0;

  /***************************************************************************/
  /** Stops the IO device operation.
   *
   * HW specific functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  virtual LIB_RESULT Stop(void) = 0;
  
  /***************************************************************************/
  /** Sets given value to the specified parameter.
   *
   * \param aParamId  sensor parameter identification
   * \param apValue   pointer to sensor parameter value (might be a structure 
   *                  with all parameters)
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  virtual LIB_RESULT ParameterSet(SDI_IO_PARAM aParamId, void *apValue) = 0;

  /***************************************************************************/
  /** Gets currently set value of the specified parameter.
   *
   * \param aParamId    sensor parameter identification
   * \param apValue     pointer to structure where to store parameter values
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  virtual LIB_RESULT ParameterGet(SDI_IO_PARAM aParamId, void *apValue) = 0;
  
  /***************************************************************************/
  /** Remembers result of the last opperation.
   *
   * \return state of the IO object
   ***************************************************************************/
  sdi_IoState StateGet(void);
  
  /***************************************************************************/
  /** Remembers result of the last opperation.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT ResultGet(void);

  /***************************************************************************/
  /** Virtual destructor.
   *
   ***************************************************************************/
  virtual ~sdi_io(void);
private:

}; // sdi_io 

/***************************************************************************/
/** Class describing FDMA IO object.
 *
 ***************************************************************************/
class sdi_FdmaIO : sdi_io
{  
  FDMA_Tc_t** mppTcs; ///< pointer to FDMA channel struct pointers
  SDI_ImageDescriptor mppIDs[FDMA_CHANNELS]; ///< pointer to Image Descriptors
  SDI_DdrBufferArr *mppBuffArrs[FDMA_CHANNELS]; ///< storage for buffer arrays
  uint32_t mTest;  

public:
  
  /***************************************************************************/
  /** Constructor.
   *
   * Sets members to default values.
   ***************************************************************************/
  sdi_FdmaIO(void);
  
  /***************************************************************************/
  /** Constructor based on graph specification.
   *
   * \param apGraph pointer to graph object
   * 
   * Sets members to default values.
   ***************************************************************************/
  sdi_FdmaIO(sdi_graph *apGraph);
  
  /***************************************************************************/
  /** Reserves IO device.
   *
   * HW specific reserve functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Reserve(void);
  
  /***************************************************************************/
  /** Releases IO device.
   *
   * HW specific release functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Release(void);
  
  /***************************************************************************/
  /** Sets Up IO device.
   *
   * HW specific setup functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Setup(void);
  
  /***************************************************************************/
  /** Starts the IO device operation.
   *
   * HW specific functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
   LIB_RESULT Start(void);

  /***************************************************************************/
  /** Stops the IO device operation.
   *
   * HW specific functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
   LIB_RESULT Stop(void);

  /***************************************************************************/
  /** Sets given value to the specified parameter.
   *
   * \param aParamId  sensor parameter identification
   * \param apValue   pointer to sensor parameter value (might be a structure 
   *                  with all parameters)
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT ParameterSet(SDI_IO_PARAM aParamId, void *apValue);

  /***************************************************************************/
  /** Gets currently set value of the specified parameter.
   *
   * \param aParamId    sensor parameter identification
   * \param apValue     pointer to structure where to store parameter values
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT ParameterGet(SDI_IO_PARAM aParamId, void *apValue);
  
  /***************************************************************************/
  /** Gets DDR image descriptor for the specified channel.
   *
   * \param aChnlIdx  index of the corresponding FDMA channel
   * \param arDesc    image descriptor
   * \return  LIB_SUCCESS if all ok, 
   *          LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT DdrBufferDescGet(uint32_t aChnlIdx, SDI_ImageDescriptor &arDesc);
  
  /***************************************************************************/
  /** Sets DDR image descriptor for the specified channel.
   *
   * \param aChnlIdx  index of the corresponding FDMA channel
   * \param arcDesc   new image descriptor
   * \return  LIB_SUCCESS if all ok, 
   *          LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT DdrBufferDescSet(uint32_t aChnlIdx, 
                              const SDI_ImageDescriptor &arcDesc);
  
  /***************************************************************************/
  /** Allocates required buffers for all used channels.
   *
   * \param aCnt      number of buffers to be allocated per channel
   * \return  LIB_SUCCESS if all ok, 
   *          LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT DdrBuffersAlloc(uint32_t aCnt);
  
  /***************************************************************************/
  /** Allocates required buffers for particular channel only.
   *
   * \param aChnlIdx  index of the corresponding FDMA channel
   * \param aCnt      number of buffers to be allocated
   * \return  LIB_SUCCESS if all ok, 
   *          LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT DdrBuffersAlloc(uint32_t aChnlIdx, uint32_t aCnt);
  
  /***************************************************************************/
  /** Allocates required buffers for particular channel and forces the buffer 
   * and FDMA channel geometry to match provided image descriptor.
   *
   * \param aChnlIdx  index of the corresponding FDMA channel
   * \param aCnt      number of buffers to be allocated
   * \param arImgDesc image descriptor defining the size
   * \return  LIB_SUCCESS if all ok, 
   *          LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT DdrBuffersAlloc(uint32_t aChnlIdx, 
                             uint32_t aCnt,
                             SDI_ImageDescriptor &arImgDesc);
  
  /***************************************************************************/
  /** Releases buffer memory allocated for this channel.
   *
   * \param aChnlIdx  index of the corresponding FDMA channel
   * \return  LIB_SUCCESS if all ok, 
   *          LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT DdrBuffersFree(uint32_t aChnlIdx);
  
  /***************************************************************************/
  /** Sets specified bufferarray to be used by the channel.
   *
   * \param arBuffers    new DDR buffer array
   * \return  LIB_SUCCESS if all ok, 
   *          LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT DdrBuffersSet(SDI_DdrBufferArr &arBuffers);
  
  /***************************************************************************/
  /** Gets currently set value of the specified parameter.
   *
   * \param aChnlIdx   index of the corresponding FDMA channel
   * \param aBufferIdx index of the queried buffer
   * \param arBuffer   [out] UMat representing the buffer
   * 
   * \return  LIB_SUCCESS if all ok, 
   *          LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT DdrBufferQuery(uint32_t aChnlIdx, 
                            uint32_t aBufferIdx, 
                            vsdk::UMat &arBuffer);
  
  /***************************************************************************/
  /** Gets configuration of specified FDMA transfer channel.
   *
   * The channel index has to be prefilled in the provided TC structure.
   * 
   * \param arTc       [out] FDMA Transfer Channel config
   * 
   * \return  LIB_SUCCESS if all ok, 
   *          LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT TcGet(FDMA_Tc_t &arTc);
  
  /***************************************************************************/
  /** Sets configuration of specified FDMA transfer channel.
   *
   * The channel index has to be prefilled in the provided TC structure.
   * TC structure can be updated if some values trimmed.
   * 
   * \param arTc       [in/out] FDMA Transfer Channel config
   * 
   * \return  LIB_SUCCESS if all ok, 
   *          LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT TcSet(FDMA_Tc_t &arTc);

  /***************************************************************************/
  /** destructor.
   *
   ***************************************************************************/
   ~sdi_FdmaIO(void);
private:
  
}; // sdi_Fdma 

/***************************************************************************/
/** Class describing H264DEC IO object.
 *
 ***************************************************************************/
class sdi_H264DecIO : sdi_io
{

public:
  
  /***************************************************************************/
  /** Constructor.
   *
   * Sets members to default values.
   ***************************************************************************/
  sdi_H264DecIO(void);
  
  /***************************************************************************/
  /** Constructor based on graph specification.
   *
   * Sets members to default values.
   * 
   * \param apGraph the graph where the IO exists
   ***************************************************************************/
  sdi_H264DecIO(sdi_graph *apGraph);
  
  /***************************************************************************/
  /** Reserves IO device.
   *
   * HW specific reserve functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Reserve(void);
  
  /***************************************************************************/
  /** Releases IO device.
   *
   * HW specific release functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Release(void);
  
  /***************************************************************************/
  /** Sets Up IO device.
   *
   * HW specific setup functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Setup(void);
  
  /***************************************************************************/
  /** Starts the IO device operation.
   *
   * HW specific functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
   LIB_RESULT Start(void);

  /***************************************************************************/
  /** Stops the IO device operation.
   *
   * HW specific functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
   LIB_RESULT Stop(void);

  /***************************************************************************/
  /** Sets given value to the specified parameter.
   *
   * \param aParamId  sensor parameter identification
   * \param apValue   pointer to sensor parameter value (might be a structure 
   *                  with all parameters)
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT ParameterSet(SDI_IO_PARAM aParamId, void *apValue);

  /***************************************************************************/
  /** Gets currently set value of the specified parameter.
   *
   * \param aParamId    sensor parameter identification
   * \param apValue     pointer to structure where to store parameter values
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT ParameterGet(SDI_IO_PARAM aParamId, void *apValue);
  
  /***************************************************************************/
  /** Remembers given decoding parameters.
   * 
   * Only a subset of the parameters is updated.
   * Provided configuration will be applied when Setup is invoked. 
   *
   * \param arcConfig structure with decoding parameters
   ***************************************************************************/
  void ConfigSet(const SDI_H264DecDescriptor &arcConfig);
  
  /***************************************************************************/
  /** Reads current decoding parameters.
   *
   * If nothing configuret yet, default values returned.
   * 
   * \return structure with current set of decoding parameters
   ***************************************************************************/
  SDI_H264DecDescriptor ConfigGet(void);

  /***************************************************************************/
  /** destructor.
   *
   ***************************************************************************/
   ~sdi_H264DecIO(void);
private:
  
  /***************************************************************************/
  /** Frees any allocated reference memory buffers.
   *
   ***************************************************************************/
  void RefMemFree(void);
  
  SDI_H264DecDescriptor  mDesc;   ///< isp graph node descriptor
  
  uint32_t mStreamCnt;                  ///< number of used streams
  void*    mppRefMem[H264DEC_STRM_CNT]; ///< array of ref mem PHY addresses
  void*    mppRefMemHandle[H264DEC_STRM_CNT]; ///< array of ref mem handles
}; // sdi_H264Dec 

/***************************************************************************/
/** Class describing H264ENC IO object.
 *
 ***************************************************************************/
class sdi_H264EncIO : sdi_io
{

public:
  
  /***************************************************************************/
  /** Constructor.
   *
   * Sets members to default values.
   ***************************************************************************/
  sdi_H264EncIO(void);
  
  /***************************************************************************/
  /** Constructor based on graph specification.
   *
   * Sets members to default values.
   * 
   * \param apGraph the graph where the IO exists
   ***************************************************************************/
  sdi_H264EncIO(sdi_graph *apGraph);
  
  /***************************************************************************/
  /** Reserves IO device.
   *
   * HW specific reserve functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Reserve(void);
  
  /***************************************************************************/
  /** Releases IO device.
   *
   * HW specific release functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Release(void);
  
  /***************************************************************************/
  /** Sets Up IO device.
   *
   * HW specific setup functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Setup(void);
  
  /***************************************************************************/
  /** Starts the IO device operation.
   *
   * HW specific functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
   LIB_RESULT Start(void);

  /***************************************************************************/
  /** Stops the IO device operation.
   *
   * HW specific functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
   LIB_RESULT Stop(void);

  /***************************************************************************/
  /** Remembers given encoding parameters.
   * 
   * Provided configuration will be applied when Setup is invoked. 
   *
   * \param arcConfig structure with encoding parameters
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT ConfigSet(const H264ENC_Config_t &arcConfig);
  
  /***************************************************************************/
  /** Reads current encoding parameters.
   *
   * If nothing configuret yet, default values returned.
   * 
   * \return structure with current set of encoding parameters
   ***************************************************************************/
  H264ENC_Config_t ConfigGet(void);
  
  /***************************************************************************/
  /** Remembers shunks size the output data should be divided to.
   *
   * Requires timeout to be specified, specifying max time to block till the
   * chunk is available.
   * 
   * \param aChunkSizeBytes chunk size in Bytes, must be multiple of 8
   * \param aTimeoutMs      timeout in miliseconds
   * 
   * \return LIB_SUCCESS if all ok, LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT OutChunkSet(uint32_t aChunkSizeBytes, uint32_t aTimeoutMs);
  
  /***************************************************************************/
  /** Pop data from the output buffer.
   *
   * Requires OutChunkSet() to be used upfront. Blocks untill the chunksize is 
   * available or timout is reached.
   * 
   * Internally shifts alarm address and is capable of detecting overflows at
   * encoder output.
   * 
   * \param apBuff pointer to memory where the data should be copied.
   * 
   * \return LIB_SUCCESS if all ok, LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT DataPop(uint64_t *apBuff);
  
  /***************************************************************************/
  /** Pop data from the output buffer.
   *
   * Requires OutChunkSet() to be used upfront. Blocks untill the chunksize is 
   * available or timout is reached.
   * 
   * Internally shifts alarm address and is capable of detecting overflows at
   * encoder output.
   * 
   * Pops allways in multiples of 8bytes
   * 
   * \param apBuff[in]  pointer to memory where the data should be copied.
   * \param arSize[out]  real number of bytes popped from the SRAM.
   * 
   * \return LIB_SUCCESS if all ok, LIB_FAILURE otherwise
   ***************************************************************************/
  LIB_RESULT DataPop(uint64_t *apBuff, uint32_t &arSize);
  
  /***************************************************************************/
  /** Sets given value to the specified parameter.
   *
   * \param aParamId  sensor parameter identification
   * \param apValue   pointer to sensor parameter value (might be a structure 
   *                  with all parameters)
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT ParameterSet(SDI_IO_PARAM aParamId, void *apValue);

  /***************************************************************************/
  /** Gets currently set value of the specified parameter.
   *
   * \param aParamId    sensor parameter identification
   * \param apValue     pointer to structure where to store parameter values
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT ParameterGet(SDI_IO_PARAM aParamId, void *apValue);
  
  /***************************************************************************/
  /** Gets address and size of the encoder output SRAM buffer.
   *
   * \param arAddress[out]  32bit variable to store the physical address of 
   *                        the stream output SRAM buffer
   * \param arSize[out]     size of the ouput SRAM buffer in bytes
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT StreamOutBuffGet(uint32_t &arAddress, uint32_t &arSize);

  /***************************************************************************/
  /** destructor.
   *
   ***************************************************************************/
   ~sdi_H264EncIO(void);
private:
  SDI_H264EncDescriptor  mDesc;  ///< isp graph node descriptor
  H264ENC_Config_t       mConfig; ///< configuration params
  uint32_t               mChunkSize;
  uint32_t               mTimeoutMs; ///< data pop timeout in MS 
                                     ///< (0 = wait for ever)
  
  // *** output buffer info ***
  uint32_t mOutBufStart;   // HW start address
  uint32_t mOutBufEnd;     // HW end address
  uint32_t mOutBufSize;    // number of bytes
  uint32_t mOutBufRead;    // next read address
  uint64_t *mpOutBuff;     // virtual mapping of output SRAM buffer
  void     *mpOutBuffBase; // virtual mapping of output SRAM buffer
  uint32_t mOutBuffMapSize; // alligned mapping size
  
  
}; // sdi_H264Enc 

/***************************************************************************/
/** Class describing JpegDec IO object.
 *
 ***************************************************************************/
class sdi_JpegDecIO : sdi_io
{

public:
  
  /***************************************************************************/
  /** Constructor.
   *
   * Sets members to default values.
   ***************************************************************************/
  sdi_JpegDecIO(void);
  
  /***************************************************************************/
  /** Constructor based on graph specification.
   *
   * Sets members to default values.
   * 
   * \param apGraph the graph where the IO exists
   ***************************************************************************/
  sdi_JpegDecIO(sdi_graph *apGraph);
  
  /***************************************************************************/
  /** Reserves IO device.
   *
   * HW specific reserve functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Reserve(void);
  
  /***************************************************************************/
  /** Releases IO device.
   *
   * HW specific release functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Release(void);
  
  /***************************************************************************/
  /** Sets Up IO device.
   *
   * HW specific setup functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Setup(void);
  
  /***************************************************************************/
  /** Starts the IO device operation.
   *
   * HW specific functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
   LIB_RESULT Start(void);

  /***************************************************************************/
  /** Stops the IO device operation.
   *
   * HW specific functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
   LIB_RESULT Stop(void);

  /***************************************************************************/
  /** Sets given value to the specified parameter.
   *
   * \param aParamId  sensor parameter identification
   * \param apValue   pointer to sensor parameter value (might be a structure 
   *                  with all parameters)
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT ParameterSet(SDI_IO_PARAM aParamId, void *apValue);

  /***************************************************************************/
  /** Gets currently set value of the specified parameter.
   *
   * \param aParamId    sensor parameter identification
   * \param apValue     pointer to structure where to store parameter values
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT ParameterGet(SDI_IO_PARAM aParamId, void *apValue);
  
  /***************************************************************************/
  /** Remembers given decoding parameters.
   * 
   * Only a subset of the parameters is updated.
   * Provided configuration will be applied when Setup is invoked. 
   *
   * \param arcConfig structure with decoding parameters
   ***************************************************************************/
  LIB_RESULT ConfigSet(const SDI_JpegDecDescriptor &arcConfig);
  
  /***************************************************************************/
  /** Reads current decoding parameters.
   *
   * If nothing configuret yet, default values returned.
   * 
   * \return structure with current set of decoding parameters
   ***************************************************************************/
  SDI_JpegDecDescriptor ConfigGet(void);

  /***************************************************************************/
  /** destructor.
   *
   ***************************************************************************/
   ~sdi_JpegDecIO(void);
private:
  SDI_JpegDecDescriptor  mDesc;  ///< isp graph node descriptor
}; // sdi_JpegDec 

/***************************************************************************/
/** Class describing MipiCsi IO object.
 *
 ***************************************************************************/
class sdi_MipiCsiIO : sdi_io
{
  SDI_MipiDescriptor mDesc;  ///< MIPI setup descriptor

public:
  
  // MipiCsi IRQs masks - all enabled by default
  uint32_t mPPErrIrqMask;  // 0x00000027
  uint32_t mPhyErrIrqMask; // 0x000fffff
  uint32_t mFrameIrqMask;  // 0x0000ffff
  
  /***************************************************************************/
  /** Constructor.
   *
   * Sets members to default values.
   ***************************************************************************/
  sdi_MipiCsiIO(void);
  
  /***************************************************************************/
  /** Constructor based on graph specification.
   *
   * Sets members to default values.
   * 
   * \param apGraph the graph where the IO exists
   * \param aCsiIdx the index of the csi interface
   ***************************************************************************/
  sdi_MipiCsiIO(sdi_graph *apGraph, enum CSI_IDX aCsiIdx);
  
  /***************************************************************************/
  /** Reserves IO device.
   *
   * HW specific reserve functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Reserve(void);
  
  /***************************************************************************/
  /** Releases IO device.
   *
   * HW specific release functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Release(void);
  
  /***************************************************************************/
  /** Sets Up IO device.
   *
   * HW specific setup functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Setup(void);
  
  /***************************************************************************/
  /** Starts the IO device operation.
   *
   * HW specific functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Start(void);

  /***************************************************************************/
  /** Stops the IO device operation.
   *
   * HW specific functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Stop(void);

  /***************************************************************************/
  /** Sets given value to the specified parameter.
   *
   * \param aParamId  sensor parameter identification
   * \param apValue   pointer to sensor parameter value (might be a structure 
   *                  with all parameters)
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT ParameterSet(SDI_IO_PARAM aParamId, void *apValue);

  /***************************************************************************/
  /** Gets currently set value of the specified parameter.
   *
   * \param aParamId    sensor parameter identification
   * \param apValue     pointer to structure where to store parameter values
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT ParameterGet(SDI_IO_PARAM aParamId, void *apValue);
  
  /***************************************************************************/
  /** Gets mipi-csi setup defined in ISP graph.
   *
   * \return descriptor containing the Csi configuration.
   ***************************************************************************/
  SDI_MipiDescriptor DescriptorGet(void);

  /***************************************************************************/
  /** Destructor.
   *
   ***************************************************************************/
   ~sdi_MipiCsiIO(void);
private:  
}; // sdi_MipiCsi 

/***************************************************************************/
/** Class describing VIU IO object.
 *
 ***************************************************************************/
class sdi_ViuIO : sdi_io
{
  SDI_ViuDescriptor mDesc;  ///< Viu setup descriptor

public:
  
  /***************************************************************************/
  /** Constructor.
   *
   * Sets members to default values.
   ***************************************************************************/
  sdi_ViuIO(void);
  
  /***************************************************************************/
  /** Constructor based on graph specification.
   *
   * Sets members to default values.
   * 
   * \param apGraph the graph where the IO exists
   * \param aViuIdx the index of the Viu interface
   ***************************************************************************/
  sdi_ViuIO(sdi_graph *apGraph, enum VIU_IDX aViuIdx);
  
  /***************************************************************************/
  /** Reserves IO device.
   *
   * HW specific reserve functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Reserve(void);
  
  /***************************************************************************/
  /** Releases IO device.
   *
   * HW specific release functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Release(void);
  
  /***************************************************************************/
  /** Sets Up IO device.
   *
   * HW specific setup functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Setup(void);
  
  /***************************************************************************/
  /** Starts the IO device operation.
   *
   * HW specific functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Start(void);

  /***************************************************************************/
  /** Stops the IO device operation.
   *
   * HW specific functionality, if any. (to be invoked by SDI library)
   ***************************************************************************/
  LIB_RESULT Stop(void);

  /***************************************************************************/
  /** Sets given value to the specified parameter.
   *
   * \param aParamId  sensor parameter identification
   * \param apValue   pointer to sensor parameter value (might be a structure 
   *                  with all parameters)
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT ParameterSet(SDI_IO_PARAM aParamId, void *apValue);

  /***************************************************************************/
  /** Gets currently set value of the specified parameter.
   *
   * \param aParamId    sensor parameter identification
   * \param apValue     pointer to structure where to store parameter values
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT ParameterGet(SDI_IO_PARAM aParamId, void *apValue);
  
  /***************************************************************************/
  /** Gets Viu setup defined in ISP graph.
   *
   * \return descriptor containing the Viu configuration.
   ***************************************************************************/
  SDI_ViuDescriptor DescriptorGet(void);

  /***************************************************************************/
  /** Destructor.
   *
   ***************************************************************************/
   ~sdi_ViuIO(void);
private:

}; // sdi_Viu 

#endif /* SDIIO_HPP */
