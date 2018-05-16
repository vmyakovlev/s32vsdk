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
 * \file      sdi_graph.hpp
 * \brief     Graph related objects declarations. Depends on GDT 
 *            generated metadata.
 * \author    Tomas Babinec
 * \version   0.1
 * \date      21-October-2014
 ****************************************************************************/

#ifndef SDIGRAPH_HPP
#define SDIGRAPH_HPP

#include "sdi_types.hpp"

#ifndef __STANDALONE__
#include <fcntl.h>		/* open */
#include <unistd.h>		/* exit */
#include <sys/ioctl.h>		/* ioctl */
#endif // __STANDALONE__

#include "seq_graph_meta.h"
#include "isp_hw.h"
#include "../../../isp/fdma/kernel/include/fdma_types.h"

#include "seq_graph.h"

using namespace std;

//****************************************************************************
// contant definitions
//****************************************************************************
#ifdef PRAM_SIZE
#undef PRAM_SIZE
#endif
#define PRAM_SIZE (4*1024)


//****************************************************************************
// types
//****************************************************************************

///< address or offset in graph
typedef uint32_t SDI_GraphAddr_t;

///< host pointer value
typedef uint64_t SDI_HostAddr_t;

//
// preliminary class sdi declarations
class sdi;
//class sdi_grabTask;
//class sdi_grabber;

/***************************************************************************/
/** Class encapsulating Sequencer graph handling.
 *
 ***************************************************************************/

class sdi_graph
{
  // *** orig graph data ***
  SEQ_Head_Ptr_t  *mpGraph;   ///< pointer to original graph structure
  GraphMetadata_t *mpMeta;    ///< pointer to graph metadata
  uint32_t         mHeadCnt;  ///< number of heads in the graph
  
  // *** packed graph data ***
  void*       mpPackedGraph;          ///< ptr to packed graph
  void*       mpPackedObjects;        ///< ptr to first object in packed graph
  void*       mpFetchedGraph;         ///< ptr to fetched graph
  SEQ_Buf_t** mppFetchedBufferList;   ///< ptr to fetched buffer list  
  
  FDMA_Tc_t* mTcs[FDMA_CHANNELS];     ///< FDMA channel struct pointers
  
  void**     mppSramBufOalHandles;    ///< pointer for OAL handles array

  // *** FDMA node back-up *** 
  SEQ_FdmaCfg_t *mpFdma;  ///< original address of the FdmaCfg head
  
  ///< backup of the packed FdmaCfg head with original TC index assignment
  uint8_t mpFdmaPackedBck[sizeof(SEQ_FdmaCfg_t)];
  
  // *** IO nodes packed indexes ***
  int32_t mIoNodeIdxArr[SEQ_OTHRIX_LAST]; //< fast reference to IO nodes
  // *** IO nodes packed pointers ***
  SEQ_Head_t* mpIoNodePtrArr[SEQ_OTHRIX_LAST]; //< fast reference to IO nodes
  
  // *** CSI node shortcut ***
  SEQ_MipiCfg_t *mppCsi[CSI_IF_CNT]; ///< adress of Mipi nodes if present
  
  // *** VIU node shortcut ***
  SEQ_ViuCfg_t *mppViu[VIU_IF_CNT]; ///< adress of Viu nodes if present
  
  // *** JPEG node shortcut ***
  SEQ_JpegDecCfg_t *mpJpegDec; ///< adress of Jpeg node if present
  
  // *** H264Dec node shortcut ***
  SEQ_H264DecCfg_t *mpH264Dec; ///< adress of H264 decoder node if present
  
  // *** H264Enc node shortcut ***
  SEQ_H264EncCfg_t *mpH264Enc; ///< adress of H264 encoder node if present
  
public:
  /***************************************************************************/
  /** Conscturctor.
   *  Sets the structure to empty.
   ***************************************************************************/
  sdi_graph(void);
  
  /***************************************************************************/
  /** Constructor.
   *
   * \param aGraph    the precompiled Sequencer graph
   * \param apMeta    the precompiled Sequencer graph metadata
   ***************************************************************************/
  sdi_graph(SEQ_Head_Ptr_t apGraph[], GraphMetadata_t *apMeta);
  
  /***************************************************************************/
  /** Destructor.
   *  Releases allocated resources.
   ***************************************************************************/
  ~sdi_graph(void);
  
  /***************************************************************************/
  /** Allocats SRAM buffers, reserves FDMA channels.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT Finalize(void);
  
  /***************************************************************************/
  /** Release all reserved resources (FDMA channels & SRAM buffers).
   *
   ***************************************************************************/
  void Free(void);
  
  /***************************************************************************/
  /** Reorders the FDMA channels in FDMA graph node to fit the allocated TC
   *  indexes.
   *   
   ***************************************************************************/
  void FdmaChannelReorder(void);
  
  /***************************************************************************/
  /** Downloads the graph to the PRAM.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT Download(SDI_GraphAddr_t aOffset);
  
  /***************************************************************************/
  /** Computes the PRAM size required for the Graph.
   *
   * \param appGraph[out] fetched graph address output
   * \param apppBufferList[out] fetched buffer list address output 
   * \return    size of the graph in bytes.
   ***************************************************************************/
  LIB_RESULT Fetch(SEQ_Head_Ptr_t **appGraph, SEQ_Buf_t ***apppBufferList);
  
  /***************************************************************************/
  /** Downloads the graph to the PRAM.
   *
   * \param apMeta   	pointer to graph's metadata
   * \return		size of the graph in bytes.
   ***************************************************************************/
  static uint32_t SizeGet(GraphMetadata_t *apMeta);

  /***************************************************************************/
  /** Computes the PRAM size required for the Graph.
   *
   * \param apMeta pointer to graph's metadata
   * \param arEngCnt number of engines present in the graph
   * \return		size of the graph in bytes.
   ***************************************************************************/
  static uint32_t SizeGet(GraphMetadata_t *apMeta, uint32_t &arEngCnt);

  /***************************************************************************/
  /** Gets the size of the Graph.
   *
   * \return graph size in bytes
   ***************************************************************************/
  uint32_t SizeGet(void);
  
  /***************************************************************************/
  /** Returns FDMA channel index based on the channel name from the graph.
   *
   * \param   acpName  name of the FDMA channel to get the index for
   * 
   * \return  channel index (>=0) if name is valid
   *          <0 if name was not found
   ***************************************************************************/
  int32_t FdmaChnlName2Idx(const char* acpName);
  
  /***************************************************************************/
  /** Returns index to buffer list based on the buffer name from the graph.
   *
   * \param   acpName  name of the buffer to get the index for
   * 
   * \return  buffer index (>=0) if name is valid
   *          <0 if name was not found
   ***************************************************************************/
  int32_t BufferName2Idx(const char* acpName);

  /***************************************************************************/
  /** Gets SRAM address allocated for the given Sram buffer
   *
   * \param apSramBuf Buffer pointer to get.
   * \param arSramAddr Sram addr allocated for the given buffer.   
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT SramBufGet(SEQ_Buf_t* apSramBuf, uint32_t &arSramAddr);
  
  /***************************************************************************/
  /** Gets SRAM address allocated for the given Sram buffer
   *
   * \param acpName    Name of the buffer to get.
   * \param arSramAddr Sram addr allocated for the given buffer.   
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT SramBufGet(const char *acpName, uint32_t &arSramAddr);
  
  /***************************************************************************/
  /** Gets SRAM buffer descriptor.
   *
   * \param acpName[in]     Name of the buffer to get.
   * \param arSramBuf[out]  Structure describing the buffer.   
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT SramBufGet(const char *acpName, SEQ_Buf_t &arSramBuf);
  
  /***************************************************************************/
  /** Gets current setup of TC with given graph index.
   *
   * \param arTc TC structure to be filled with current setup.
   * \param aIdx channel index.
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT FdmaTcGet(FDMA_Tc_t& arTc, uint32_t aIdx);
  
  /***************************************************************************/
  /** Sets the given TC setup to the specified channel index.
   *
   * \param arTc new TC setup.
   * \param aIdx channel index.
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT FdmaTcSet(FDMA_Tc_t& arTc, uint32_t aIdx);
  
  /***************************************************************************/
  /** Gets FDMA TC table.
   *
   * \return pointer to array of preallocated TC pointers.
   *         NULL if FDMA not present in the graph.
   ***************************************************************************/
  FDMA_Tc_t** FdmaTcsGet(void);
  
  /***************************************************************************/
  /** Applys TCs setup to FDMA TDT in SRAM.
   *
   ***************************************************************************/
  LIB_RESULT FdmaTcsApply(void);
  
  /***************************************************************************/
  /** Gets current setup of MIPI CSI node with given index.
   *
   * \param aIdx mipi csi index (0 or 1).
   * \return Mipi csi descriptor, otherwise mIdx member contains > 1
   ***************************************************************************/
  SDI_MipiDescriptor MipiDescGet(enum CSI_IDX aIdx);
  
  /***************************************************************************/
  /** Gets current setup of VIU node with given index.
   *
   * \param aIdx viu index (0 or 1).
   * \return viu descriptor, otherwise mIdx member contains > 1
   ***************************************************************************/
  SDI_ViuDescriptor ViuDescGet(enum VIU_IDX aIdx);
  
  /***************************************************************************/
  /** Gets current setup of JPEG node.
   * 
   * \param arJpegDesc [out] jpeg descriptor
   *
   * \return LIB_SUCCESS if all ok otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT JpegDecDescGet(SDI_JpegDecDescriptor &arJpegDesc);
  
  /***************************************************************************/
  /** Gets current setup of H264 encoder node.
   *
   * \return H264 encoder descriptor
   ***************************************************************************/
  SDI_H264EncDescriptor H264EncDescGet(void);
  
  /***************************************************************************/
  /** Gets current setup of H264 Decoder node.
   *
   * \param arH264DecDesc [out] descriptor structure
   * 
   * \return LIB_SUCCESS if all ok otherwise LIB_FAILURE
   ***************************************************************************/
   LIB_RESULT H264DecDescGet(SDI_H264DecDescriptor &arH264DecDesc);
  
  /***************************************************************************/
  /** Gets what nodes are present and at which head index.
   *
   * \param[out] aIOs    array of indexes to IO graph nodes. Idx < 0 if not used.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT IOindexesGet(uint32_t aIOidxs[]);
  
  /***************************************************************************/
  /** Queries for the presence of the IO Node in the graph.
   *
   * \param[out] aIoNodeId ID of the node IO node to be queried
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT IoQuery(SEQ_Othr_ix_t aIoNodeId);

private:

  /***************************************************************************/
  /** Goes over the ISP graph objects and remembers pointer shorcuts to 
   * required parts.
   *
   * \return    LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT Scan(void);

  /***************************************************************************/
  /** Transforms the ISP graph structure in to a packed format.
   *  Required before graph copy to Sequencer memory.
   * 
   * \note Parses the graph and remembers 
   *
   * \return 		LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT Pack(void);

  /***************************************************************************/
  /** Copies graph objects to packed format.
   *
   * \note Parses the graph and remembers
   *
   ***************************************************************************/
  void ObjectsCopy(void);

  /***************************************************************************/
  /** Changes pointer values in the packed graph to offsets.
   *
   * \param 	aAddrBase Base address from which to generate the offset.
   ***************************************************************************/
  void Addr2Offset(uint32_t aAddrBase);

  /***************************************************************************/
  /** Updates pointer addresses in graph to fit future location.
   *
   * \param aAddrBase base address of the packed graph in memory
   ***************************************************************************/
  void Offset2Addr(uint32_t aAddrBase);
  
  /***************************************************************************/
  /** Updates pointer addresses in graph to fit new location.
   *
   * \param apGraph      pointer to the packed graph copy to be updated
   * \param aOldAddrBase previous base address of the packed graph in memory
   * \param aNewAddrBase new base address of the packed graph in memory
   ***************************************************************************/
  void Addr2Addr(void* apGraph, 
                 SDI_GraphAddr_t aOldAddrBase, 
                 SDI_GraphAddr_t aNewAddrBase);
  
  /***************************************************************************/
  /** Transforms Internal Graph offset to pointer in packed graph.
   *
   * \return    pointer corresponding to the offset
   * 		NULL if error
   ***************************************************************************/
  inline void *Off2Packed(uint32_t aOffset);
  
  /***************************************************************************/
  /** Generates empty TC for FDMA channels defined in the graph.
   * Sets up already known TD parameters.
   *
   * \return    LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT FdmaTCsPrepare(void);
  
  /***************************************************************************/
  /** Reserves required number of TCs.
   *
   * \return    LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT FdmaTCsReserve(void);
  
  /***************************************************************************/
  /** Releases previously reserved TCs.
   *
   ***************************************************************************/
  void FdmaTCsRelease(void);
  
  /***************************************************************************/
  /** Removes TC instances.
   * 
   * Releases the TC if still reserved.
   *
   ***************************************************************************/
  void FdmaTCsRemove(void);
  
  /***************************************************************************/
  /** Allocates all SRAM buffers for this graph.
   *
   * \return    LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ***************************************************************************/
  LIB_RESULT SramBuffersAllocate(void);
  
  /***************************************************************************/
  /** Releases all SRAM buffers for this graph.
   *
   ***************************************************************************/
  void SramBuffersFree(void);

}; // class sdi_graph

#endif /* SDIGRAPH_HPP */
