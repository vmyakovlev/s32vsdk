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
/*!*****************************************************************************
*  \file canny_apu.h
*  \brief Canny Edge Detection Kernels
 ******************************************************************************/

#ifndef APUCANNY_H
#define APUCANNY_H
#include <stdint.h>

#define MAX_BLK_SIZE 200

// Worst case stack size will be half of the block size
#define MAX_STACK_SIZE (MAX_BLK_SIZE/2)

/* Bit offset for the row value in the 16-bit stack elements*/
#define ROW_BIT_OFFSET 8

/*!
   Performs Canny Non-Maxima Suppression and Edge Hysteresis

   \param dEdges - [Output] Destination edge map buffer
   \param estr   - [Input]  Destination edge map stride
   \param sMag   - [Input]  Source magnitude buffer
   \param mstr   - [Input]  Source magnitude stride
   \param dx     - [Input]  Source X gradient buffer
   \param xstr   - [Input]  Source X gradient stride
   \param dy     - [Input]  Source Y gradient buffer
   \param xstr   - [Input]  Source Y gradient stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
   \param low    - [Input]  Low threshold used in edge hysteresis
   \param high   - [Input]  High threshold used in edge hysteresis
 */
void apu_canny_suppress(
               vec08u* dst, int dstr,
         const vec08u* src, int sstr,
               int bw, int bh,
               int16u low, int16u high);

// This version also does internal edge promotion
void apu_canny_suppress_promote(
               vec08u* dst, int dstr,
         const vec08u* src, int sstr,
               int bw, int bh,
               int16u low, int16u high);

/*!
   Performs edge connection between blocks. This is used where the
   block size does not change between kernel outputs

   \param dst    - [Output] Destination edge map buffer
   \param dstr   - [Input]  Destination edge map stride
   \param src    - [Input]  Source edge map buffer
   \param sstr   - [Input]  Source edge map stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
 */
void apu_canny_connect_edges(
               vec08u* dst, int dstr,
         const vec08u* src, int sstr,
               int bw, int bh);

/*!
   Performs edge connection between blocks. This is used where the
   block size changes between kernel outputs

   \param dst    - [Output] Destination edge map buffer
   \param dstr   - [Input]  Destination edge map stride
   \param src    - [Input]  Source edge map buffer
   \param sstr   - [Input]  Source edge map stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
 */
void apu_canny_connect_edges_full(
               vec08u* dst, int dstr,
         const vec08u* src, int sstr,
               int bw, int bh);

/*!
   Outputs the _src_ image with _edge_ as a mask.

   \param dst    - [Output] Destination image buffer
   \param dstr   - [Input]  Destination image stride
   \param src    - [Input]  Source image buffer
   \param sstr   - [Input]  Source image stride
   \param edge   - [Input]  Source edge map buffer
   \param estr   - [Input]  Source edge map stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
 */
void apu_canny_create_image(
               vec08u* dst, int dstr,
         const vec08u* edge, int estr,
               int bw, int bh);

#endif /* APUCANNY_H */
