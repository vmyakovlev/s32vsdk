/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2013-2016 Freescale Semiconductor
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
/*!
* \file reduction_apu.h
* \brief reduction of a vector/image implementation for APEX
* \addtogroup reduction 
* @{
****************************************************************************/

#ifndef APUREDUCTIONIMPL_H
#define APUREDUCTIONIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

/*!
Reduce an input vector/image by summing up the corresponding elements
\brief Elementwise unsigned 8bit addition => unsigned 16bit 
\param lpvIn0		- [Input]  32unsigned bit source block pointer of img 0
\param lpOut0		- [Output] 32signed bit destination block pointer
\param isLastTile	- [Input]  boolean: is true, if the last image tile is being processed
\param lFirstCuId	- [Input]  the id of the first CU
\param lTileWidthInChunks - [Input] number of used chunks/CUs
\param chunkWidth 	- [Input]  Block width
\param chunkHeight	- [Input]  Block height
\param lChunkSpanIn0- [Input]  Source block width (in bytes) including padding
\param lChunkSpanOut0- [Input] Destination block width (in bytes) including padding
*/
void reduc(vec32u* lpvIn0, int32s* lpOut0, bool isLastTile,  int16s lFirstCuId, int16s lTileWidthInChunks,
			int lChunkWidth, int lChunkHeight, int lChunkSpanIn0, int lChunkSpanOut0);

#endif /* APUREDUCTIONIMPL_H */
/*! @} */
