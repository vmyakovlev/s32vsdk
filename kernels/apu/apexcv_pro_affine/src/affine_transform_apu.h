/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2015-2016 Freescale Semiconductor
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

#ifndef ACFAFFINETRANSFORM_H
#define ACFAFFINETRANSFORM_H

static vec32s g_x_offs;
static vec32s g_y_offs;
static vec32s chunk_base_x;
static vec32s chunk_base_y;

void acf_affine_init_cu_offsets(int32s *mat, int32s chunk_width, int32s chunk_height, int32s tile_width_in_chunks);

/*!*********************************************************************************
*  \brief Process bilinear interpolation.
*
*  Bilinear interpolation is processed for a given source image with specified coordinates.
*  \snippet affine_transform_apu.cpp bilinear
*  \snippet affine_transform_apu.cpp bilinear helper
***********************************************************************************/
void acf_affine_bilinear_interpolation(
         const vec08u *src                /*!< Pointer to the source image.*/, 
         int32s src_stride,    
         int32s src_width                 /*!< Width of the source buffer.*/, 
         int32s src_height						/*!< Height of the source buffer.*/,
         int32s *mat                      /*!< Pointer to the affine inverse matrix.*/, 
         vec08u *dst                      /*!< Pointer to the destination image.*/, 
         int32s dst_stride,
         int32s chunk_width               /*!< Width of the destination buffer.*/, 
         int32s chunk_height              /*!< Height of the destination buffer.*/, 
         int32s image_width               /*!< Width of the source image.*/, 
         int32s image_height              /*!< Height of the source image.*/
);

#endif /* ACFAFFINETRANSFORM_H */

