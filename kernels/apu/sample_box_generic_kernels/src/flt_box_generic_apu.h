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
#ifdef ACF_KERNEL_IMPLEMENTATION

#ifndef APUFLTBOXGENERIC_H
#define APUFLTBOXGENERIC_H

#include <stdint.h>

/*!
   Calculates a box filter.

   \param dst    - [Output] Destination block pointer
   \param dstr   - [Input]  Destination block stride
   \param src    - [Input]  Source value block pointer
   \param sstr   - [Input]  Source value block stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
   \param flt_x  - [Input]  Filter x size
   \param flt_y  - [Input]  Filter y size
   \param shift  - [Input]  Shifting done to result
 */
void
apu_flt_box_generic_init( vec16s* dst, int dstr,
       const vec16s* src, int sstr,
                  int bw, int bh,
                  int flt_x, int flt_y,
                  int shift);
void
apu_flt_box_generic( vec16s* dst, int dstr,
       const vec16s* src, int sstr,
                  int bw, int bh,
                  int flt_x, int flt_y,
                  int shift);
void
apu_flt_box_generic_end( vec16s* dst, int dstr,
                  int bw, int bh,
                  int flt_x, int flt_y,
                  int shift);

void apu_harris_box_5x5(
               vec16s* restrict dst, int dstr,
         const vec16s* restrict src, int sstr,
               int bw,  int bh,
               int shift);
               
#endif /* APUFLTBOXGENERIC_H */

#endif /* ACF_KERNEL_IMPLEMENTATION */
