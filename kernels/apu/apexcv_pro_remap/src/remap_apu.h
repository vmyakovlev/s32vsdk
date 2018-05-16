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
*  \brief Declaration of kernel functions for Remap
***********************************************************************************/

#ifndef REMAPBILINEAR_H
#define REMAPBILINEAR_H
   
/*!***********************************************************************************
*  \brief Performs remap of RGBA pixel data..
*  \returns None.
*************************************************************************************/   
void
remap_bilinear_rgb
               (vec32u __cmem* dst,
                vec32u __cmem* src,
                vec16u __cmem* offset, 
                vec08u __cmem* delta, 
                int sstride, // stride of the src array in cmem
                int dstride, // stride of the src array in cmem
                int bw,
                int bh);

/*!***********************************************************************************
*  \brief Performs remap of 8-bit grayscale pixel data.
*  \returns None.
*************************************************************************************/                               
void
remap_bilinear_grayscale
               (vec08u __cmem* dst,
                vec08u __cmem* src,
                vec16u __cmem* offset, 
                vec08u __cmem* delta, 
                int sstride, // stride of the src array in cmem
                int dstride, // stride of the src array in cmem
                int bw,
                int bh);
      
void remap_block_size(vec32u *map, int32_t chunk_width, int32_t chunk_height, int32_t tile_width,
                      vec16u *t_width, vec16u *t_height, 
                      vec16u *f_width, vec16u *f_height, 
                      vec16u *e_width, vec16u *e_height, 
                      vec16u *s_width, vec16u *s_height);

#endif   /* REMAPBILINEAR_H */
