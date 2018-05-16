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

#ifndef RESIZEDEFINITIONS_H
#define RESIZEDEFINITIONS_H

#include <stdint.h>

#define HRSZ_DECIMAL_SCL       16
#define VRSZ_DECIMAL_SCL       16 
#define FLT_BANK_SIZE          32
#define PAD 64
#define PAD_REPLICATE 4

//! Filter banks
//! 8 Phases, 4 Taps
#define FLT_UP_SAMPLING {0 , 256,   0,   0,\
                        -11, 245,  23,  -1,\
                        -17, 220,  58,  -5,\
                        -18, 184, 100, -10,\
                        -15, 143, 143, -15,\
                        -10, 100, 184, -18,\
                         -5,  58, 220, -17,\
                         -1,  23, 245, -11}
//! 8 Phases, 4 Taps
#define FLT_DOWN_SAMPLING1 {28, 200,  28, 0,\
                            15, 194,  47, 0,\
                             7, 178,  71, 0,\
                             2, 156,  98, 0,\
                             0, 128, 128, 0,\
                             0,  98, 156, 2,\
                             0,  71, 178, 7,\
                             0,  47, 194, 15}
//! 4 Phases, 8 Taps
#define FLT_DOWN_SAMPLING2 {2, 32, 94,  94, 32,  2, 0, 0,\
                            0, 19, 80, 105, 47,  5, 0, 0,\
                            0, 11, 64, 106, 64, 11, 0, 0,\
                            0,  5, 47, 105, 80, 19, 0, 0}
 
struct resize_descriptor {
   int32_t src_offs;
   int32_t scl_fact;
   int16_t phases;
   int16_t taps; 
   int32_t out_scale;
   int32_t out_round;
   int16_t *fbnk;
   resize_descriptor():src_offs(1),scl_fact(1),phases(3),taps(4),out_scale(8),out_round(1),fbnk(0)  
   {}
};

#endif /* RESIZEDEFINITIONS_H */
