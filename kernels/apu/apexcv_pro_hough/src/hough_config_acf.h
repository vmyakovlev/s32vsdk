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
 *  /file hough_config_acf.h
 *  /brief ACF compile-time configuration for the Hough Transform
 ******************************************************************************/

#ifndef ACFHOUGHCONFIG_H
#define ACFHOUGHCONFIG_H

const int HOUGH_CU_COUNT          = 32;
const int HOUGH_MAX_LINES_PER_CU  = 32;
const int HOUGH_MAX_BLOCK_SIZE    = 40;
const int HOUGH_ACCUM_BUFFER_SIZE = 1604;
const int HOUGH_PIXEL_BUFFER_SIZE = HOUGH_MAX_BLOCK_SIZE * HOUGH_CU_COUNT;

#endif  /* ACFHOUGHCONFIG_H */
