
/**************************************************************************************************
 *
 * NXP Confidential Proprietary
 *
 * Copyright (c) 2014-2017 NXP
 * All Rights Reserved
 *
 *****************************************************************************
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
 **************************************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include <stdint.h>
#include "resize_apu.h"
#include "resize_definitions.h"



void init_cu_offsets(resize_descriptor* rsz_desc, int32_t dw, int32_t tile_width_in_chunks)
{
   const int32s dscl = HRSZ_DECIMAL_SCL;
   const int32s mask = (1 << dscl) - 1;
   int32s src_offs = rsz_desc->src_offs;
      
   for (int i = 0; i < tile_width_in_chunks; ++i) {
      vec32s off = src_offs & mask;
      cu_offs = vput(cu_offs, off, i);
      src_offs += rsz_desc->scl_fact * dw;
   }
   
   for (int i = 0; i < FLT_BANK_SIZE; ++i) {
      cmemflt[i] = (vec16s)rsz_desc->fbnk[i];
   }
}

int get_polyphase_params(int16_t ssiz, int16_t dsiz, int16_t dscl, resize_descriptor* rsz_desc)
{
   if (ssiz >= 2*dsiz) {
      rsz_desc->phases = 2; //! 4 Phases
      rsz_desc->taps   = 8; 
      rsz_desc->fbnk   = (int16_t*)flt_down_sampling2;
   } else if (ssiz > dsiz) {
      rsz_desc->phases = 3; //! 8 Phases
      rsz_desc->taps   = 4;
      rsz_desc->fbnk   = (int16_t*)flt_down_sampling1;
   } else if  (ssiz < dsiz) {
      rsz_desc->phases = 3; //! 8 Phases
      rsz_desc->taps   = 4;
      rsz_desc->fbnk   = (int16_t*)flt_up_sampling;
   } else {
      return 1;
   }

   rsz_desc->scl_fact  = (ssiz * (1 << dscl)) / dsiz;
   rsz_desc->src_offs  = (rsz_desc->scl_fact >> 1) - (((rsz_desc->taps - 1) >> 1) << dscl);
   rsz_desc->out_scale = 8;
   rsz_desc->out_round = 1 << (rsz_desc->out_scale - 1);
  
   return 0;
}

void apu_vrsz_polyphase(vec08u* psrc, int32_t instride, int32_t sw, vec08u* pdst, int32_t outstride, int32_t dw, int32_t dh, resize_descriptor* rsz_desc)
{
   const int32s dscl = VRSZ_DECIMAL_SCL;
   const int32s mask = (1 << dscl) - 1;
   int32_t local_offs = rsz_desc->src_offs & mask;
   
   for (int y = 0; y < dh; ++y) chess_loop_range(1,) {
      vec08u* s = psrc + (local_offs >> dscl) * instride;
      int16_t* f = rsz_desc->fbnk + (rsz_desc->taps * ((local_offs & mask) >> (dscl - rsz_desc->phases)));
      for (int x = 0; x < dw; ++x) chess_loop_range(1,) {

         vec32s d = 0;
         for (int k = 0; k < rsz_desc->taps; ++k) chess_loop_range(4,8) {
            vacm(d, vmul_shul((vec16s)f[k], (vec16s)s[k * instride + x]));
            vacl(d, (vec16u)vmul_ulul((vec16s)f[k], (vec16s)s[k * instride + x]));
         }
         
         d += rsz_desc->out_round;
         d >>= rsz_desc->out_scale;

         pdst[y * outstride + x] = (vec08u)vsat((vec16s)d, 0, UINT8_MAX);
      }  
      
      local_offs += rsz_desc->scl_fact;
      rsz_desc->src_offs += rsz_desc->scl_fact;
   }
}

void apu_hrsz_polyphase(vec08u* psrc, int32_t instride, int32_t sw, vec08u* pdst, int32_t outstride, int32_t dw, int32_t dh, resize_descriptor* rsz_desc)
{
   const int32s dscl = HRSZ_DECIMAL_SCL;
   const int32s mask = (1 << dscl) - 1;
   vec32s local_offs = cu_offs;
   
   for (int x = 0; x < dw; ++x) chess_loop_range(1,) {
      vec08u* s = psrc;
      vec16s* f = cmemflt;
         
      for (int y = 0; y < dh; ++y, s += instride) chess_loop_range(1,)  {
         vec16s sidx = (vec16s)(local_offs >> dscl);
         vec16s fidx = (vec16s)(rsz_desc->taps * (vec16s)((local_offs & mask) >> (dscl - rsz_desc->phases)));
         
         vec32s d = 0;
         for (int k = 0; k < rsz_desc->taps; ++k) chess_loop_range(4,8)  {
            vec08u a = vload(s,sidx++);
            vec16s b = vload(f,fidx++);
            
            vacm(d, vmul_shul(b, (vec16s)a));
            vacl(d, (vec16u)vmul_ulul(b, (vec16s)a));
         }
         
         d += rsz_desc->out_round;
         d >>= rsz_desc->out_scale;

         pdst[y * outstride + x] = (vec08u)vsat((vec16s)d, 0, UINT8_MAX);
      }
      
      local_offs += rsz_desc->scl_fact;
   }
}


void apu_vrsz_polyphase16(vec16u* psrc, int32_t instride, int32_t sw, vec16u* pdst, int32_t outstride, int32_t dw, int32_t dh, resize_descriptor* rsz_desc)
{
   const int32s dscl = VRSZ_DECIMAL_SCL;
   const int32s mask = (1 << dscl) - 1;
   int32_t local_offs = rsz_desc->src_offs & mask;

   for (int y = 0; y < dh; ++y) chess_loop_range(1,) {
      vec16u* s = psrc + (local_offs >> dscl) * instride;
      int16_t* f = rsz_desc->fbnk + (rsz_desc->taps * ((local_offs & mask) >> (dscl - rsz_desc->phases)));
      for (int x = 0; x < dw; ++x) chess_loop_range(1,) {

         vec32s d = 0;
         for (int k = 0; k < rsz_desc->taps; ++k) chess_loop_range(4,8) {
            vacm(d, vmul_shul((vec16s)f[k], (vec16s)(s[k * instride + x]>>8) ));
            vacl(d, (vec16u)vmul_ulul((vec16s)f[k], (vec16s)(s[k * instride + x]>>8)));
         }

         d += rsz_desc->out_round;
         d >>= rsz_desc->out_scale;

         pdst[y * outstride + x] = ((vec16u)vsat((vec16s)d, 0, UINT8_MAX)) << 8;
      }

      local_offs += rsz_desc->scl_fact;
      rsz_desc->src_offs += rsz_desc->scl_fact;
   }
}


void apu_hrsz_polyphase16(vec16u* psrc, int32_t instride, int32_t sw, vec16u* pdst, int32_t outstride, int32_t dw, int32_t dh, resize_descriptor* rsz_desc)
{
   const int32s dscl = HRSZ_DECIMAL_SCL;
   const int32s mask = (1 << dscl) - 1;
   vec32s local_offs = cu_offs;

   for (int x = 0; x < dw; ++x) chess_loop_range(1,) {
      vec16u* s = psrc;
      vec16s* f = cmemflt;

      for (int y = 0; y < dh; ++y, s += instride) chess_loop_range(1,)  {
         vec16s sidx = (vec16s)(local_offs >> dscl);
         vec16s fidx = (vec16s)(rsz_desc->taps * (vec16s)((local_offs & mask) >> (dscl - rsz_desc->phases)));

         vec32s d = 0;
         for (int k = 0; k < rsz_desc->taps; ++k) chess_loop_range(4, 8)  {
            vec08u a = (vec08u)(vload(s,sidx++)>>8);
            vec16s b = vload(f,fidx++);

            vacm(d, vmul_shul(b, (vec16s)a));
            vacl(d, (vec16u)vmul_ulul(b, (vec16s)a));
         }

         d += rsz_desc->out_round;
         d >>= rsz_desc->out_scale;

         pdst[y * outstride + x] = ((vec16u)vsat((vec16s)d, 0, UINT8_MAX))<<8;
      }

      local_offs += rsz_desc->scl_fact;
   }
}




#endif
