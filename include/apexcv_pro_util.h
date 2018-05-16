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
*  \brief APEX-CV Utility functions
***********************************************************************************/

#ifndef APEXCVUTIL_H
#define APEXCVUTIL_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>


namespace apexcv{

   int32_t RetNumCU(int apexID, int32_t& lArrayWidth);
   /********************************
   **
   **	MEMORY OPERATIONS (copy, compare)
   **
   ********************************/

   /*!*********************************************************************************
   *  \brief 2D extension of memcpy.
   *
   *  The standard C function memcpy copies a contiguous string of memory from one
   *  location to another.  The 2D extension of this is copyBlock, which
   *  copies a \ref pagBlock "2D memory block" from one location to another.
   *
   *  Consider a 2D memory source with location \c src and span \c srcSpan, and a 2D
   *  memory destination with location \c dst and span \c dstSpan.  Beginning at address
   *  \c src is a block of \c width (bytes) by \c height rows.  These are shown in
   *  the following figure.
   *
   *  \image html  copyBlock.svg "copyBlock copies a contiguous area of memory from one location to another."
   *  \image latex copyBlock.pdf "copyBlock copies a contiguous area of memory from one location to another." width = 8cm
   *
   *  Since each row is contiguous in memory, we use memcpy to copy each source row to
   *  its corresponding destination row.
   ***********************************************************************************/
   void copyBlock(void* dst         /*!< Destination pointer. */,
      size_t dstSpan    /*!< Destination span. */,
      const void* src   /*!< Source pointer. */,
      size_t srcSpan    /*!< Source span. */,
      size_t width      /*!< Number of bytes to copy per row. */,
      int height        /*!< Number of rows to copy. */);

   /*!*********************************************************************************
   *  \brief 2D extension of memcmp.
   *
   *  The standard C function memcmp compares two contiguous strings of memory.
   *  The 2D extension of this is compBlock, which compares two
   *  \ref pagBlock "2D memory blocks".
   *
   *  Consider two 2D memory buffers with addresses \c buf1 and \c buf2 and span
   *  \c span1 and \c span2, respectively.  Each buffer contains a memory block of
   *  \c width (bytes) by \c height rows.These are shown in the following figure.
   *
   *  \image html  compBlock.svg "compBlock compares two blocks of memory."
   *  \image latex compBlock.pdf "compBlock compares two blocks of memory." width = 8cm
   *
   *  Since each row is contiguous in memory, we use memcmp to compare corresponding
   *  rows in each buffer.
   ***********************************************************************************/
   int compBlock(const void* buf1   /*!< Address of buffer 1. */,
      size_t span1       /*!< Span of buffer 1. */,
      const void* buf2   /*!< Address of buffer 2. */,
      size_t span2       /*!< Span of buffer 2. */,
      size_t width       /*!< Number of bytes to compare per row. */,
      int height         /*!< Number of rows to compare. */);

   /*!*********************************************************************************
   *  \brief Set each byte of a \ref pagBlock "2D memory block" to a certain value.
   *
   *  The standard C function memset sets each byte of a string to a certain value.
   *  The 2D extension of this is blockSet.
   ***********************************************************************************/
   void* setBlock(void * dst     /*!< Destination pointer. */,
      size_t span    /*!< Destination span. */,
      int value      /*!< Value (cast to unsigned 8-bit) to fill the block. */,
      size_t width   /*!< Number of bytes to set per row. */,
      int height     /*!< Number of rows to set. */);

   /*!*********************************************************************************
   *  \brief Copy a \ref pagLattice "2D memory lattice" to another.
   ***********************************************************************************/
   void copyLattice(void* dst         /*!< Destination pointer. */,
      size_t dstSpan    /*!< Destination span. */,
      size_t dstStepX   /*!< Number of bytes between horizontal lattice points in the destination buffer. */,
      size_t dstStepY   /*!< Number of bytes between vertical   lattice points in the destination buffer. */,

      const void* src   /*!< Source pointer. */,
      size_t srcSpan    /*!< Source span. */,
      size_t srcStepX   /*!< Number of bytes between horizontal lattice points in the source buffer. */,
      size_t srcStepY   /*!< Number of bytes between vertical   lattice points in the source buffer. */,

      size_t width      /*!< Block width in bytes. */,
      int height        /*!< Block height. */,

      int nx            /*!< Number of horizontal lattice points. */,
      int ny            /*!< Number of vertical   lattice points. */);

   /*!*********************************************************************************
   *  \brief Compare a \ref pagLattice "2D memory lattice" with another.
   ***********************************************************************************/
   void compLattice(const void* buf1    /*!< Address of buffer 1. */,
      size_t span1        /*!< Span of buffer 1. */,
      size_t stepX1       /*!< Number of bytes between horizontal lattice points in buffer 1. */,
      size_t stepY1       /*!< Number of bytes between vertical   lattice points in buffer 1. */,

      const void* buf2    /*!< Address of buffer 2. */,
      size_t span2        /*!< Span of buffer 2. */,
      size_t stepX2       /*!< Number of bytes between horizontal lattice points in buffer 2. */,
      size_t stepY2       /*!< Number of bytes between vertical   lattice points in buffer 2. */,

      size_t width        /*!< Block width in bytes. */,
      int height          /*!< Block height. */,

      int nx              /*!< Number of horizontal lattice points. */,
      int ny              /*!< Number of vertical   lattice points. */);

   /*!*********************************************************************************
   *  \brief Pad a \ref pagBlock "2D memory block" with a single value.
   ***********************************************************************************/
   void padSet(void *dst      /*!< Address of destination buffer. */,
      size_t span    /*!< Span of destination buffer. */,
      int value      /*!< Value assigned to each padded byte. */,
      size_t width   /*!< Unpadded width in bytes. */,
      int height     /*!< Unpadded height (rows). */,
      size_t left    /*!< Number of bytes padded on the left. */,
      size_t right   /*!< Number of bytes padded on the right. */,
      int top        /*!< Number of rows padded on the top. */,
      int bottom     /*!< Number of rows padded on the bottom. */);

   /*!*********************************************************************************
   *  \brief Copy a \ref pagBlock "2D memory block" and pad with a single value.
   ***********************************************************************************/
   void padSet(void* dst         /*!< Address of destination buffer. */,
      size_t dstSpan    /*!< Span of destination buffer. */,
      const void* src   /*!< Address of source buffer. */,
      size_t srcSpan    /*!< Span of source buffer. */,
      int value         /*!< Value assigned to each padded byte. */,
      size_t width      /*!< Unpadded width in bytes. */,
      int height        /*!< Unpadded height (rows). */,
      size_t left       /*!< Number of bytes padded on the left. */,
      size_t right      /*!< Number of bytes padded on the right. */,
      int top           /*!< Number of rows padded on the top. */,
      int bottom        /*!< Number of rows padded on the bottom. */);

   /*!*********************************************************************************
   *  \brief Pad a \ref pagBlock "2D memory block" by replication.
   ***********************************************************************************/
   void padReplicate(void* buf         /*!< Address of destination buffer. */,
      size_t span        /*!< Span of destination buffer. */,
      size_t width       /*!< Unpadded width in bytes. */,
      int height         /*!< Unpadded height (rows). */,
      size_t leftWidth   /*!< Width in bytes of each padded column on the left. */,
      int leftCols       /*!< Number of padded columns on the left. */,
      size_t rightWidth  /*!< Width in bytes of each padded column on the right. */,
      int rightCols      /*!< Number of padded columns on the right. */,
      int topHeight      /*!< Height in of each padded row on the top. */,
      int topRows        /*!< Number of padded rows on the top. */,
      int bottomHeight   /*!< Height in of each padded row on the bottom. */,
      int bottomRows     /*!< Number of padded rows on the bottom. */);

   /*!*********************************************************************************
   *  \brief Copy a \ref pagBlock "2D memory block" and pad by replication.
   ***********************************************************************************/
   void padReplicate(void* dst         /*!< Address of destination buffer. */,
      size_t dstSpan    /*!< Span of destination buffer. */,
      const void* src   /*!< Address of source buffer. */,
      size_t srcSpan    /*!< Span of source buffer. */,
      size_t width      /*!< Unpadded width in bytes. */,
      int height        /*!< Unpadded height (rows). */,
      size_t leftWidth  /*!< Width in bytes of each padded column on the left. */,
      int leftCols      /*!< Number of padded columns on the left. */,
      size_t rightWidth /*!< Width in bytes of each padded column on the right. */,
      int rightCols     /*!< Number of padded columns on the right. */,
      int topHeight     /*!< Height in of each padded row on the top. */,
      int topRows       /*!< Number of padded rows on the top. */,
      int bottomHeight  /*!< Height in of each padded row on the bottom. */,
      int bottomRows    /*!< Number of padded rows on the bottom. */);

   /*!*********************************************************************************
   *  \brief Compare \ref data with another.
   ***********************************************************************************/
   template <typename D>
   int compare_results(D* test   /*!< Address of data 1. */,
      D* ref   /*!< Address of data 2. */,
      int w    /*!< Data width. */,
      int h    /*!< Data height. */,
      int range = 0  /*!< Range of error. */);

}  /* namespace apex */

#endif /* APEXCVUTIL_H */
