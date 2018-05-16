/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor
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
* \file column_sum_apu.h
* \brief cumputing sum of columns of image for APEX ldw_v2 demo
* \addtogroup column_sum
* @{
****************************************************************************/

#ifndef APUCOLUMNSUMIMPL_H
#define APUCOLUMNSUMIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif


/*!
\brief Elementwise unsigned 8bit addition => unsigned 16bit 
\param lpvIn0           - [Input]  8bit source block pointer of img 0
\param lpvOutDown       - [Output] 32 unsigned bit destination block pointer
\param lpvOutUp         - [Output] 32 unsigned bit destination block pointer
\param isFirstTile      - [Input]  boolean: true if the algorithm is at its first tile
\param lStride0         - [Input]  Source block width (in bytes) including padding
\param chunkWidth       - [Input]  Block width
\param chunkHeight      - [Input]  Block height
\param outChunkWidth    - [Input]  Block width
\param priorityDown     - [Input]  Priority for lower part of input image
\param priorityUp       - [Input]  Priority for upper part of input image
\param indexOfTileStart - [Input]  Index of tile start
*/
void column_sum(vec08u* lpvIn0,
                vec32u* lpvOutDown,
                vec32u* lpvOutUp,
                bool isFirstTile,
                int lStrideIn0,
                int chunkWidth,
                int chunkHeight,
                int outChunkWidth,
                int priorityDown,
                int priorityUp,
                int indexOfTileStart);


#endif /* APUCOLUMNSUMIMPL_H */

/*! @} */
