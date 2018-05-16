#pragma once
#include "seq_graph.h"
#include "seq_graph_meta.h"

/*****************************************************************************
* consts
*****************************************************************************/

enum {
  FDMA_IX_FastDMA_Out,
  FDMA_IX_FastDMA_Out1,
  FDMA_IX_FastDMA_Out2,
  FDMA_IX_FastDMA_Out3,
  FDMA_IX_END
};

/*****************************************************************************
* exported variables
*****************************************************************************/

extern SEQ_Head_Ptr_t   gpGraph[];
extern GraphMetadata_t  gGraphMetadata;

extern SEQ_Buf_t	MIPI_port_A_Buffer_0;
extern SEQ_Buf_t	YUV41110bit_to_8bit_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_0;
extern SEQ_Buf_t	YUV41110bit_to_8bit_Buffer_1;
extern SEQ_Buf_t	FastDMA_Buffer_1;
extern SEQ_Buf_t	YUV41110bit_to_8bit_Buffer_2;
extern SEQ_Buf_t	FastDMA_Buffer_2;
extern SEQ_Buf_t	YUV42210bit_to_8bit_ipus2_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_3;

