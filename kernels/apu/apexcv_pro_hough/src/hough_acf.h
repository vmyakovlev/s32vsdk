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

/*******************************************************************************
 *  \file hough_acf.h
 *  \brief ACF metadata for Hough Transform
 ******************************************************************************/
#ifndef ACFHOUGH_H
#define ACFHOUGH_H

#ifdef ACF_KERNEL_METADATA

extern KERNEL_INFO _kernel_info_hough_init_pixel_offset;
extern KERNEL_INFO _kernel_info_hough_init_cu_index;
extern KERNEL_INFO _kernel_info_hough_pixel_threshold;
extern KERNEL_INFO _kernel_info_hough_scalarize_pixels;
extern KERNEL_INFO _kernel_info_hough_accumulate;
extern KERNEL_INFO _kernel_info_hough_get_lines;

#endif /* ACF_KERNEL_METADATA */
#endif  /* ACFHOUGH_H */
