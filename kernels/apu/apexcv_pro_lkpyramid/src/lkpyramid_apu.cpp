/*****************************************************************************
 *
 * NXP Confidential Proprietary
 *
 * Copyright (c) 2016-2017 NXP Semiconductor;
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
 ****************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "lkpyramid_apu.h"
#include <stdint.h>

#include <stdio.h>

static vec16u inline vfxrecip12_inl(vec16u dn){
    /************* Begin private table. ****************/
#define TBL_AVG_HDIF(hi, lo)                                            \
        (((((hi)+(lo))/2)&0xFFFFU)*0x10000+((((hi)-(lo))/2)&0xFFFFU))
#define TBL_CONST_PACK(p1, s0, s1)                                      \
        ((p1<<8 | (s0&0xF) << 4 | (s1&0xF))&0xFFFFU)
#define TBL_CONST_PACK_AVG_HDIF(p1h, s0h, s1h, p1l, s0l, s1l)           \
        TBL_AVG_HDIF(TBL_CONST_PACK((p1h), (s0h), (s1h)), TBL_CONST_PACK((p1l), (s0l), (s1l)))

static const uint32_t VRECIPTBL[] = {
    46006,
    /*                      High          Low        */
    TBL_CONST_PACK_AVG_HDIF(181, 10-8, 3, 31, 12-8, 1),
    TBL_AVG_HDIF           (30945,        58691      ),
    TBL_AVG_HDIF           (63962,        65532      )
};
#undef TBL_AVG_HDIF
#undef TBL_CONST_PACK
#undef TBL_CONST_PACK_AVG_HDIF
    /************* End private table.   ****************/
    
    /* Constant load */
    const uint32_t* lPtr   = VRECIPTBL;
    const uint32_t  thresh = *lPtr++;
    const uint32_t  shamts = *lPtr++;
    const uint32_t  coeff0 = *lPtr++;
    const uint32_t  coeff1 = *lPtr++;
    
    vec16u m   = vextract_lo(thresh);
    
    vec16u x   = dn << 1;/* + an also works. */
    vbool  c   = dn >  m;/* true = high polynomial, false = low polynomial */
    
    vec16u k0, k1, k2;
    vec16u s0, s1;
    
    /* Constant loads, efficient select using vasb. */
    s1 =  k1   = vasb(vextract_hi(shamts), vextract_lo(shamts), c);
    s0         = s1 >> 4;
    k2         = vasb(vextract_hi(coeff0), vextract_lo(coeff0), c);
    k0         = vasb(vextract_hi(coeff1), vextract_lo(coeff1), c);
    
    /* x^2 mulhi */
    vec32u t0  = vec32u(vmul_uhuh(x, x), x-x /* ZERO. */);
    vec16u t1  = vmul_uhul(x, x);
    vacm(t0, t1);
    vacm(t0, t1);
#if (__chess__)
    vec16u x2  = t0.high();
#elif (__llvm__)
    vec16u x2  = (vec16u)__builtin_apex_vec32_get_hi((vec32s)t0);
#endif
    
    /* x*k1, full-precision */
    vec32u t3  = vec32u(vmul_uhuh(x, k1), x<<15 /* ZERO. At this point x is known to be even. */);
    vacm(t3, vmul_uluh(x, k1));
    
    /* a = p0 - p1*x, b = p2 - p3*x */
#if (__chess__)
    vec16u a   = k0 - x + (t3.high() >> s0);
#elif (__llvm__)
    vec16u a = k0 - x + ((vec16u)__builtin_apex_vec32_get_hi((vec32s)t3) >> s0);
#endif
    vec16u b   = k2 - (x >> s1);
    
    /* a + bx^2 = a + b * x^2, mulhi + acc */
    vec32u bx2 = vec32u(vmul_uhuh(b, x2) + a, (vec16u)0);
    vec16u t4  = vmul_uhul(b, x2);
    vacm(bx2, t4);
    vec16u t5  = vmul_uluh(b, x2);
    vacm(bx2, t5);
    
#if (__chess__)    
    return bx2.high()-9;/* -9 ensures that it is an underestimate. */
#elif (__llvm__)
    return (vec16u)((vec16u)__builtin_apex_vec32_get_hi((vec32s)bx2)-(vec16u)9);/* -9 ensures that it is an underestimate. */
#endif
}

vec16u vfxrecip12(vec16u x) property(functional){
    return vfxrecip12_inl(x);
}

inline void apu_vmac_shift(vec16u& c, vec16u a, vec16u b, int shift)
{
   vec32u m = 0;
   
#if (__chess__)  
   vec16u l = m.low();
   vec16u h = m.high();
#elif (__llvm__)
   vec16u l = (vec16u)__builtin_apex_vec32_get_lo((vec32s)m);
   vec16u h = (vec16u)__builtin_apex_vec32_get_hi((vec32s)m);
#endif

   l = 1 << (shift-1);
   vec16u a1b1 = vmul_uhuh(a, b); h+= a1b1;
   vec16u a0b0 = vmul_ulul(a, b); vacl(h, l, a0b0);
   vec16u a1b0 = vmul_uhul(a, b); vacm(h, l, a1b0);
   vec16u a0b1 = vmul_uluh(a, b); vacm(h, l, a0b1);
   
   m = (vec32u)vsra(vec32u(h, l), shift);
#if (__chess__) 
   c += (vec16u)m.low();
#elif (__llvm__)
   c += (vec16u)__builtin_apex_vec32_get_lo((vec32s)m);
#endif
}

/*!
\note: intrisic available directly
 */
static
inline
void
_vmac_v7(vec32u& c, vec16u a, vec08u b)
{
#if (__chess__) 
   vec16u l = c.low();
   vec16u h = c.high();
#elif (__llvm__)
   vec16u l = (vec16u)__builtin_apex_vec32_get_lo((vec32s)c);
   vec16u h = (vec16u)__builtin_apex_vec32_get_hi((vec32s)c);
#endif

   vec16u chess_storage(V7) a0b0 = vmul_ulul(a, (vec16u)b); vacl(h, l, a0b0);
   vec16u chess_storage(V7) a1b0 = vmul_uhul(a, (vec16u)b); vacm(h, l, a1b0);
   
   c = vec32u(h, l); 
}

static
inline
void
_vmac(vec32u& c, vec16u a, vec08u b)
{
#if (__chess__) 
   vec16u l = c.low();
   vec16u h = c.high();
#elif (__llvm__)
   vec16u l = (vec16u)__builtin_apex_vec32_get_lo((vec32s)c);
   vec16u h = (vec16u)__builtin_apex_vec32_get_hi((vec32s)c);
#endif

   vec16u a0b0 = vmul_ulul(a, (vec16u)b); vacl(h, l, a0b0);
   vec16u a1b0 = vmul_uhul(a, (vec16u)b); vacm(h, l, a1b0);
   
   c = vec32u(h, l); 
}

// temporary fix
static
inline
vec32s
_vmac(vec32s& d, vec16s a, vec16s b)
{
#if (__chess__)
    vec16s dh = d.high(); vec16u dl = d.low();
#elif (__llvm__)
    vec16u dl = (vec16u)__builtin_apex_vec32_get_lo(d);
    vec16s dh = __builtin_apex_vec32_get_hi(d);
#endif

	// vmac(dh, dl, a, b);
	vec16u chess_storage(V7)   a0b0 = vmul_ulul((vec16u)a, (vec16u)b); vacl(dh, dl, a0b0);
	vec16s chess_storage(V7)   a1b1 = vmul_shsh(a, b); dh += a1b1;
	vec16s chess_storage(V7)   a1b0 = vmul_shul(a, b); vacm(dh, dl, a1b0);
	vec16s chess_storage(V7)   a0b1 = vmul_ulsh(a, b); vacm(dh, dl, a0b1);

	return (d = vec32s(dh, dl));
}

/*!
   \param   dst      destination pointer
   \param   dstr     destination stride
   \param   src      source pointer
   \param   sstr     source stride
   \param   frac_xy  pointer to fractional data (dx, dy), maximum 8-bit
                     (this pointer should be aligned on 16-bit or 32-bit)
   \param   bw       block width
   \param   bh       block height
   \param   shift    fractional representation of frac_xy(range [1-8])
   
   \note:   - specialized version using Q8
            - frac_xy should be changed to 32-bit Q16.16 format (signed)
            - shift != 8 must generate an error
            - 4 loads per costly
 */
void 
apu_lkpyramid_templ_bilinear_Q8(
            vec08u* dst, int dstr,
      const vec08u* src, int sstr,
      const vec32s *coor_i,
      int bw, int bh,
      int shift)
{
   uint16_t unit = 0x1 << shift;
   const vec16u coef_a = (vec16u)(coor_i[0]&0xFF);
   const vec16u coef_b = (vec16u)(coor_i[1]&0xFF);
   const vec08u* s0 = src;
   const vec08u* s1 = src + sstr;
   
   shift = 8;
   {
      const vec16u txy  = vmul_ulul(coef_a, coef_b);
      const vec16u tx   = coef_a << 8;
      const vec16u ty   = coef_b << 8;
      const vec16u iw01 = tx - txy;
      const vec16u iw10 = ty - txy;
      const vec16u iw11 = txy;
            vec16u iw00 = 0xFFFF - tx - ty + txy;

      const uint16_t chess_storage(R7) r = 1 << (2*shift - 1); 
      
      for (int i = 0; i < bh; i++) chess_loop_range(1,)
      {
         const vec08u*  chess_storage(R4) p0 = s0;
         const vec08u*  chess_storage(R5) p1 = s1;
               vec08u*  chess_storage(R6) pd = dst;

         vec08u chess_storage(V2)   val_1 = p0[0]; ++p0;
         vec08u chess_storage(V3)   val_3 = p1[0]; ++p1;
         
         for (int j = 0; j < bw; ++j) chess_loop_range(1,) 
         {
            vec32u chess_storage(V56) sum = vec32u((vec16u)0, r + (vec16u)val_1);  // accumulator, initialized with round value
            _vmac_v7(sum, iw00, val_1); val_1 = *p0++;
            _vmac_v7(sum, iw10, val_3); val_3 = *p1++;
            _vmac_v7(sum, iw01, val_1);
            _vmac_v7(sum, iw11, val_3);
#if (__chess__)           
            *pd++ = (vec08u)sum.high();
#elif (__llvm__)
            *pd++ = (vec08u)(vec16u)__builtin_apex_vec32_get_hi((vec32s)sum);
#endif
         }
         s0   = s1;
         s1  += sstr;
         dst += dstr;
      }
   }
}

/*!
   \param   dst      destination pointer
   \param   dstr     destination stride
   \param   src      source pointer
   \param   sstr     source stride
   \param   off      source offset index (based on integer position)
   \param   frac_xy  pointer to fractional data (dx, dy), maximum 8-bit
   \param   bw       block width
   \param   bh       block height
   \param   shift    fractional representation of frac_xy(range [1-8])
   
   \note: 
 */
void 
apu_lkpyramid_patch_bilinear_Q8(
            vec08u* dst, int dstr,
      const vec08u* src, int sstr,
      const vec16s* off,
      const vec08u* frac_xy,
      int bw, int bh,
      int shift)
{
   const uint16_t unit = 0x1 << shift;
   const vec16u coef_a = (vec16u)*frac_xy++;
   const vec16u coef_b = (vec16u)*frac_xy++;

   const vec08u* s0 = src;
   const vec08u* s1 = src + sstr;
   
   const vec16s  so = off[0];
#if (__chess__)
   const vec08u@ sv = src + so - (sstr - (bw+1));
#elif (__llvm__)
   vec16s sv = (vec16s)((int)src + so - (sstr - (bw + 1)));
#endif

   shift = 8;
   {
      const vec16u txy  = vmul_ulul(coef_a, coef_b);
      const vec16u tx   = coef_a << 8;
      const vec16u ty   = coef_b << 8;
      volatile vec16u iw[4];
      {
         const vec16u iw01 = tx - txy;
         const vec16u iw10 = ty - txy;
         const vec16u iw11 = txy;
         const vec16u iw00 = 0xFFFF - tx - ty + txy;
         
         iw[0] = iw00;
         iw[1] = iw01;
         iw[2] = iw10;
         iw[3] = iw11; 
      }
      
      const uint16_t chess_storage(R7) r = 1 << (2*shift - 1); 
      
      vec16u chess_storage(V0) ca = iw[0];
      vec16u chess_storage(V1) cb = iw[2];
      
      for (int i = 0; i < bh; i++) chess_loop_range(1,)
      {
         sv += (sstr - (bw+1));
               vec08u*  chess_storage(R6) pd = dst;

#if (__chess__)
         vec08u val_1 = *sv; sv += sstr;
         vec08u val_3 = *sv; sv += (-sstr+1);
#elif (__llvm__)
               vec08u val_1 = vload((vec08u*)0, sv); sv += sstr;
               vec08u val_3 = vload((vec08u*)0, sv); sv += (-sstr + 1);
#endif
         for (int j = 0; j < bw; ++j) chess_loop_range(1,) 
         {
            vec32u chess_storage(V56) sum = vec32u(((vec16u)val_1)>>8, r + (vec16u)val_1);  // accumulator, initialized with round value
#if (__chess__)
            _vmac_v7(sum, ca, val_1); val_1 = *sv; sv += sstr;       ca = iw[1];
            _vmac_v7(sum, cb, val_3); val_3 = *sv; sv += (-sstr+1);  cb = iw[3];
#elif (__llvm__)
            _vmac_v7(sum, ca, val_1); val_1 = vload((vec08u*)0, sv); sv += sstr;       ca = iw[1];
            _vmac_v7(sum, cb, val_3); val_3 = vload((vec08u*)0, sv); sv += (-sstr + 1);  cb = iw[3];
#endif
            _vmac_v7(sum, ca, val_1);                                ca = iw[0];
            _vmac_v7(sum, cb, val_3);                                cb = iw[2];
#if (__chess__)            
            *pd++ = (vec08u)sum.high();
#elif (__llvm__)
            *pd++ = (vec08u)(vec16u)__builtin_apex_vec32_get_hi((vec32s)sum);
#endif
         }
         dst += dstr;
      }
   }
}

void 
apu_lkpyramid_core(
      vec32s *coor_o,
      vec32s *patch_offset_o,
      vec32s *templ_offset_next_o,
      vec08u *patch, int patchstride,
      vec08u *templ, int templstride,
      vec16s *dx, int dxstride,
      vec16s *dy, int dystride,
      vec32s *coor_i,
      vec32s *patch_offset_base,
      vec32s *templ_coor_next,
      vec08u *patchwin, int patchwinstride,
      vec16s *dtwin, int dtwinstride,
      int bw_templ, int bh_templ,
      int bw_patch, int bh_patch,
      int num_iter,
      int src_span)
{
   templ += (templstride+1); 

   vec16s disp_xy[2] = {(vec16s)(coor_i[0]&0xFF), (vec16s)(coor_i[1]&0xFF)};

   // Compute box dxdx, dxdy, dydy
   // range of dx and dy?
   // vec32s T32s[3];
   // (output A?)
   // (this matrix should be normalized first)
   vec32s AA[4];
   vec16s A[3];
   vec16s inv_detA;

   vec16s min_a_clz;   
   vec16s min_b_clz;
   vec16s recip_shift = 0;
   vec16s disp_shift = 0;

   {
      const vec16s* restrict t16s_p0 = dx;
      const vec16s* restrict t16s_p1 = dy;
      
      // 3 accumulators, 3 passes (at least 2)
      {
         vec32s chess_storage(V34) t32s_0 = 0; 
         vec32s chess_storage(V56) t32s_2 = 0; 
         
         for (int i = 0; i < bh_templ; i++) chess_loop_range(1,)
         {
            const vec16s* restrict _p0 = t16s_p0;
            const vec16s* restrict _p1 = t16s_p1;
            
            vec16s t16s_0 = *_p0++;
            vec16s t16s_1 = *_p1++;
            
            for (int j = 0; j < bw_templ; j++) chess_loop_range(1,)
            {
               _vmac(t32s_0, t16s_0, t16s_0); t16s_0 = *_p0++;
               _vmac(t32s_2, t16s_1, t16s_1); t16s_1 = *_p1++;
            }
            t16s_p0 += dxstride;
            t16s_p1 += dystride;
         }
         AA[0] = t32s_0;
         AA[2] = t32s_2;
      }
      
      {
         const vec16s* restrict t16s_p0 = dx;
         const vec16s* restrict t16s_p1 = dy;
               
         vec32s chess_storage(V56) t32s_1 = 0; 
         
         for (int i = 0; i < bh_templ; i++) chess_loop_range(1,)
         {
            const vec16s* restrict _p0 = t16s_p0;
            const vec16s* restrict _p1 = t16s_p1;

            vec16s chess_storage(V0) t16s_0 = *_p0++;
            vec16s chess_storage(V1) t16s_1 = *_p1++;
            
            for (int j = 0; j < bw_templ; j++) chess_loop_range(1,)
            {
               vec16s chess_storage(V2) t16s_2 = *_p0++;
               vec16s chess_storage(V3) t16s_3 = *_p1++;

              _vmac(t32s_1, t16s_0, t16s_1); 
                  t16s_0 = t16s_2;
                  t16s_1 = t16s_3;
            }
            t16s_p0 += dxstride;
            t16s_p1 += dystride;
         }
         AA[1] = t32s_1;
      }
   }
      
   /* normalize matrix A */
   {
      vec16s t16s_0, t16s_1;
      {
         /* 32b accumulator, no scaling */
#if (__chess__)
         min_a_clz = (vec16s)vclz(vabs(AA[0]).high()|vabs(AA[1]).high()|vabs(AA[2]).high())-1;
#elif (__llvm__)
         min_a_clz = (vec16s)vclz((vec16s)__builtin_apex_vec32_get_hi((vec32s)vabs(AA[0]))|(vec16s)__builtin_apex_vec32_get_hi((vec32s)vabs(AA[1]))|(vec16s)__builtin_apex_vec32_get_hi((vec32s)vabs(AA[2])))-1;
#endif
         min_a_clz = (vec16s)vsat(min_a_clz, 0, 31);
         
#if (__chess__)
         vec32s r  = vec32s(0, vec16u(1) << (vec16u)(15-min_a_clz)); 
#elif (__llvm__)
         vec32s r  = vec32s((vec16u)0, vec16u(1) << (vec16u)(15-min_a_clz)); 
#endif
         vec32s t0 = AA[0]; t0 += r; t0 = vsl(t0, min_a_clz); 
         vec32s t1 = AA[1]; t1 += r; t1 = vsl(t1, min_a_clz); 
         vec32s t2 = AA[2]; t2 += r; t2 = vsl(t2, min_a_clz); 

         /* take MSB16, i.e. scaling 2^16 */
#if (__chess__)         
         A[0] = t0.high();
         A[1] = t1.high();
         A[2] = t2.high();
#elif (__llvm__)
         A[0] = (vec16s)__builtin_apex_vec32_get_hi(t0);
         A[1] = (vec16s)__builtin_apex_vec32_get_hi(t1);
         A[2] = (vec16s)__builtin_apex_vec32_get_hi(t2);
#endif
      }
   }

   {
      vec32s c = 0;
      _vmac(c,  A[0], A[2]);
      _vmac(c, -A[1], A[1]);
      AA[3] = c;
   }
      
   // reciprocal of detA
   {
      /* take MSB16 of detA, i.e. scaling detA by 2^16 */
      /* normalize before reciprocal */
      vec32s  c = AA[3];
      vec32u  d = vabs(c);
#if (__chess__)      
      recip_shift = vclz(d.high());
#elif (__llvm__)
      recip_shift = (vec16s)vclz((vec16u)__builtin_apex_vec32_get_hi((vec32s)d));
#endif

#if (__chess__)
      vec32s r  = vec32s(0, vec16u(1) << (vec16u)(15-recip_shift));
#elif (__llvm__)
      vec32s r  = vec32s((vec16u)0, vec16u(1) << (vec16u)(15-recip_shift));
#endif
      d += r; d = (vec32u)vsl((vec32s)d, (vec16s)recip_shift);
#if (__chess__)
      vec16u  recip = vfxrecip12(d.high());
      /* right shift 1 bit to make the room for the sign bit */
      inv_detA = (vec16s)(recip>>1); 
      vif (c.high() < 0)
#elif (__llvm__)
      vec16u  recip = vfxrecip12((vec16u)__builtin_apex_vec32_get_hi((vec32s)d));
      /* right shift 1 bit to make the room for the sign bit */
      inv_detA = (vec16s)(recip>>1); 
      vec16s h = (vec16s)__builtin_apex_vec32_get_hi(c);
      vif (h < 0)
#endif
         inv_detA = -inv_detA; 
      vendif
   }

   coor_o[3] = 1;
   for (int iter = 0; iter < num_iter; iter++) chess_loop_range(1,)
   {
      vec16s t16s_0, t16s_1;
      t16s_0 = disp_xy[0] >> LKPYRAMID_FLOATING_COOR_Q;  
      t16s_1 = disp_xy[1] >> LKPYRAMID_FLOATING_COOR_Q;  
      t16s_0 = t16s_1 * patchstride + t16s_0;
      t16s_1 = 0xFF; // set LKPYRAMID_FLOATING_COOR_Q bits 1 
      vec08u frac[2] = {(vec08u)(disp_xy[0]&t16s_1), (vec08u)(disp_xy[1]&t16s_1)};

      apu_lkpyramid_patch_bilinear_Q8(
            patchwin, patchwinstride,
            patch, patchstride, 
            &t16s_0, 
            frac,
            bw_templ, bh_templ,
            LKPYRAMID_FLOATING_COOR_Q);

      // [dxdt, dydt]
      vec16s B[2];    
      {
         const vec16s* restrict t16s_p0 = dx;
         const vec16s* restrict t16s_p1 = dy;
               vec16s*          t16s_p2 = dtwin;  // ?
         
         vec32s t32s_0 = 0;
         vec32s t32s_1 = 0;
         
         const vec08u* restrict t08u_p0 = templ;
         const vec08u* restrict t08u_p1 = patchwin;
         
         /* 32b accumulator, no scaling */
         for (int i = 0; i < bh_templ; i++) chess_loop_range(1,)
         {
            const vec08u* restrict _t08u_p0 = t08u_p0;
            const vec08u* restrict _t08u_p1 = t08u_p1;
            
                  vec16s* restrict _p_dt = t16s_p2;
                  
            const vec16s* restrict _p0 = t16s_p0;
            const vec16s* restrict _p1 = t16s_p1;
            
            for (int j = 0; j < bw_templ; j++) chess_loop_range(1,)
            {
               // diff
               vec16s t16s_2 = (vec16s)(*_t08u_p1++) - (vec16s)(*_t08u_p0++);
               *_p_dt++ = t16s_2;
               
               vec16s t16s_0 = *_p0++;
               vec16s t16s_1 = *_p1++;
               
               _vmac(t32s_0, t16s_0, t16s_2);
               _vmac(t32s_1, t16s_1, t16s_2);
            }
            
            t16s_p0 += dxstride;
            t16s_p1 += dystride;
            t16s_p2 += dtwinstride;
            t08u_p0 += templstride;
            t08u_p1 += patchwinstride;         
         } 

         // normalize B vector
         {
#if (__chess__)
            min_b_clz = (vec16s)vclz(vabs(t32s_0).high()|vabs(t32s_1).high())-1;
#elif (__llvm__)
            min_b_clz = (vec16s)vclz((vec16u)__builtin_apex_vec32_get_hi((vec32s)vabs(t32s_0))|(vec16u)__builtin_apex_vec32_get_hi((vec32s)vabs(t32s_1)))-1;
#endif
            min_b_clz = (vec16s)vsat(min_b_clz, 0, 31);
#if (__chess__)
            vec32s r  = vec32s(0, vec16u(1) << (vec16u)(15-min_b_clz));
#elif (__llvm__)
            vec32s r  = vec32s((vec16u)0, vec16u(1) << (vec16u)(15-min_b_clz)); 
#endif
            t32s_0 += r; t32s_0 = vsl(t32s_0, min_b_clz); 
            t32s_1 += r; t32s_1 = vsl(t32s_1, min_b_clz); 
#if (__chess__)
            B[0] = t32s_0.high();
            B[1] = t32s_1.high();
#elif (__llvm__)
            B[0] = __builtin_apex_vec32_get_hi(t32s_0);
            B[1] = __builtin_apex_vec32_get_hi(t32s_1);
#endif
         }
      }

      {
         vec16s t16s_0, t16s_1, t16s_2, t16s_3;
         vec32s t32s_0 = 0;
         vec32s t32s_1 = 0;

         _vmac(t32s_0,  A[1], B[1]);
         _vmac(t32s_0, -A[2], B[0]);
         _vmac(t32s_1,  A[1], B[0]);
         _vmac(t32s_1, -A[0], B[1]);

#if (__chess__)
         disp_shift = (vec16s)vclz(vabs(t32s_0).high()|vabs(t32s_1).high())-1;
#elif (__llvm__)
         disp_shift = (vec16s)vclz((vec16u)__builtin_apex_vec32_get_hi((vec32s)vabs(t32s_0))|(vec16u)__builtin_apex_vec32_get_hi((vec32s)vabs(t32s_1)))-1;
#endif
         disp_shift = (vec16s)vsat(disp_shift, 0, 31);

         /* normalize displacement vector & take 16 MSB, i.e scaling 2^16 */
#if (__chess__)
         vec32s r  = vec32s(0, vec16u(1) << (vec16u)(15-disp_shift));
#elif (__llvm__)
         vec32s r  = vec32s((vec16u)0, vec16u(1) << (vec16u)(15-disp_shift));
#endif

         t32s_0 += r; t32s_0 = vsl(t32s_0, disp_shift);
         t32s_1 += r; t32s_1 = vsl(t32s_1, disp_shift);
#if (__chess__)
         t16s_0 = t32s_0.high();
         t16s_1 = t32s_1.high();
#elif (__llvm__)
         t16s_0 = __builtin_apex_vec32_get_hi(t32s_0);
         t16s_1 = __builtin_apex_vec32_get_hi(t32s_1);
#endif
         t32s_0 = 0; t32s_1 = 0;
         _vmac(t32s_0, t16s_0, inv_detA);
         _vmac(t32s_1, t16s_1, inv_detA);

         vif (17 + min_b_clz + disp_shift > min_a_clz + recip_shift)
         {
            t16s_2 = vsat(17 + min_b_clz + disp_shift - min_a_clz - recip_shift, 0, 31);
            t16s_0 = (vec16s)vsr(t32s_0, t16s_2);
            t16s_1 = (vec16s)vsr(t32s_1, t16s_2);
         }
         velse
         {
            t16s_2 = vsat(min_a_clz + recip_shift - 17 - min_b_clz - disp_shift, 0, 31);
            t16s_0 = (vec16s)vsl(t32s_0, t16s_2);
            t16s_1 = (vec16s)vsl(t32s_1, t16s_2);
         }
         disp_xy[0] += t16s_0;
         disp_xy[1] += t16s_1;

         const int16_t range_min = -(LKPYRAMID_PATCH_BORDER<<LKPYRAMID_FLOATING_COOR_Q);
         const int16_t range_max = +(LKPYRAMID_PATCH_BORDER<<LKPYRAMID_FLOATING_COOR_Q);
         disp_xy[0] = vsat(disp_xy[0], range_min, range_max);
         disp_xy[1] = vsat(disp_xy[1], range_min, range_max);
         vif(disp_xy[0] == range_min || disp_xy[0] == range_max ||
               disp_xy[1] == range_min || disp_xy[1] == range_max)
            coor_o[3] = 0;
      }
   }
   coor_o[0] = ((coor_i[0]&0xFFFFFF00) + (vec32s)disp_xy[0]) << 1;
   coor_o[1] = ((coor_i[1]&0xFFFFFF00) + (vec32s)disp_xy[1]) << 1;

   /* update patch address offset */
   {
      vec32s i_x_next = coor_o[0] >> LKPYRAMID_FLOATING_COOR_Q;
      vec32s i_y_next = coor_o[1] >> LKPYRAMID_FLOATING_COOR_Q;
      patch_offset_o[0] = patch_offset_base[0] + (src_span * i_y_next + i_x_next);
   }
   
   {
      vec32s chess_storage(V56) sum_dt = 0;
      {
         const vec16s* restrict t16s_p0 = dtwin; // recompute
         for (int i = 0; i < bh_templ; i++) chess_loop_range(1,)
         {
            const vec16s* restrict _p0 = t16s_p0;
            
            for (int j = 0; j < bw_templ; j++) chess_loop_range(1,)
            {
               vacl(sum_dt, vabs(*_p0++));
            }
            t16s_p0 += dtwinstride;
         }
      }
      coor_o[2] = sum_dt;
   }

   /* update template address offset for next pyramid layer */
   {
      vec32s i_x_next = templ_coor_next[0] >> LKPYRAMID_FLOATING_COOR_Q;
      vec32s i_y_next = templ_coor_next[1] >> LKPYRAMID_FLOATING_COOR_Q;
      templ_offset_next_o[0] = patch_offset_base[0] + (src_span * i_y_next + i_x_next) + (LKPYRAMID_PATCH_BORDER-1)*src_span+(LKPYRAMID_PATCH_BORDER-1);
   }
}

#endif /* ACF_KERNEL_IMPLEMENTATION */
