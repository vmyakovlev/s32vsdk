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



#ifndef APURESIZE_H
#define APURESIZE_H

#include <stdint.h>
#include "resize_definitions.h"

static const int16_t flt_down_sampling1[FLT_BANK_SIZE] = FLT_DOWN_SAMPLING1;
static const int16_t flt_up_sampling   [FLT_BANK_SIZE] = FLT_UP_SAMPLING;
static const int16_t flt_down_sampling2[FLT_BANK_SIZE] = FLT_DOWN_SAMPLING2;
static vec16s cmemflt[FLT_BANK_SIZE];
static vec32s cu_offs;

void init_cu_offsets(resize_descriptor* rsz_desc, int32_t dw, int32_t tile_width_in_chunks);
int get_polyphase_params(int16_t ssiz, int16_t dsiz, int16_t dscl, resize_descriptor* rsz_desc);

// 8bit resize
void apu_vrsz_polyphase(vec08u* psrc, int32_t instride, int32_t sw, vec08u* pdst, int32_t outstride, int32_t dw, int32_t dh, resize_descriptor* rsz_desc);
void apu_hrsz_polyphase(vec08u* psrc, int32_t instride, int32_t sw, vec08u* pdst, int32_t outstride, int32_t dw, int32_t dh, resize_descriptor* rsz_desc);

// 16bit resize
void apu_vrsz_polyphase16(vec16u* psrc, int32_t instride, int32_t sw, vec16u* pdst, int32_t outstride, int32_t dw, int32_t dh, resize_descriptor* rsz_desc);
void apu_hrsz_polyphase16(vec16u* psrc, int32_t instride, int32_t sw, vec16u* pdst, int32_t outstride, int32_t dw, int32_t dh, resize_descriptor* rsz_desc);


#endif /* APURESIZE_H */
