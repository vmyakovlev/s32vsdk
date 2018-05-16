/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2016-2017 NXP
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
/* Includes */
#include <stdio.h>
#include "aggcf_apu.h"



void triangleFilt1x_c_row(vec32u* in, vec16u* out, int32_t w, uint32_t i_index, uint32_t p, int32_t pad, int32_t outstride)
{
   vec32u *Il, *Im, *Ir;

   vec32u vl, vm, vr, ret;

   for(int32_t j = pad; j < w+pad; j++)
   {
      Il = Im = Ir = in + j;
      Il --; 
      Ir ++;
      
      vl = *Il;
      vm = *Im;
      vr = *Ir;

      ret = (vec32u)(vl + p*vm + vr);
      ret = ret >> 16;
      uint16_t offset = (j-pad + i_index*outstride);
      out[offset] = ((vec16u)ret);
   }
}


// apply [1 2 1] filtering
void triangleFilt1_c_row(vec16u* in, vec16u* out, int32_t h, int32_t w, int32_t instride, int32_t outstride, vec32u *T, uint32_t p, int32_t pad)
{
   uint32_t nrm = (p + 2) * (p + 2);
   uint16_t lZ = count_leading_zero_s(nrm);
   nrm = fxrecip22(nrm << lZ) >> (32 - lZ - 1);//nrm is U32

   int32_t i, j;
   vec16u *Iu, *Im, *Ib;
   vec16u up, middle, bot, temp, nrm_v;
   nrm_v = (vec16u)(nrm>>16);
   vec32u ret;

   int32_t wstride = instride;

   for(i = 0; i < h; i++)
   {
      Iu = Im = Ib = in + i*wstride - pad;
      Iu -= wstride;
      Ib += wstride;

      for(j = 0; j < w+2*pad; j++)
      {
         up     = Iu[j];
         middle = Im[j];
         bot    = Ib[j];

         temp   = (vec16u)(up + middle*p + bot);
         ret    = ((vec32u)temp)*((vec32u)nrm_v);
         T[j]   = ret;

      }
      triangleFilt1x_c_row(T, out, w, i, p, pad, outstride);
   }
}

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


void triangleFilt1x_c_row_norm(vec16u *in, vec32u* T, vec16u* out, int32_t w, uint32_t i_index, uint32_t p, int32_t pad, int32_t instride, int32_t outstride, uint32_t normconst)
{
   vec32u *Il, *Im, *Ir;

   vec32u vl, vm, vr, ret;

   for(int32_t j = pad; j < w+pad; j++)
   {
      Il = Im = Ir = T + j;
      Il --;
      Ir ++;

      vl = *Il;
      vm = *Im;
      vr = *Ir;

      ret = (vec32u)(vl + p*vm + vr);
      ret = ret >> 16;
      uint16_t out_offset = (j-pad + i_index*outstride);
      uint16_t in_offset  = (j-pad + i_index*instride);
      
      vec16u filtered_mag = (vec16u)ret; 
      vec16u mag          = in[in_offset];
     
      filtered_mag += normconst;
      vec16u lz = vclz(filtered_mag);
      filtered_mag = vfxrecip12(filtered_mag << lz) >> (16-lz-1);
     
      mag = (vec16u)( ( ((vec32u)mag) * ((vec32u)filtered_mag) ) >> 2 );  

      out[out_offset] = mag;

   }
}


// apply [1 2 1] filtering and magnitude normalization

void triangleFilt1_c_row_norm(vec16u* in, vec16u* out, int32_t h, int32_t w, int32_t instride, int32_t outstride, vec32u *T, uint32_t p, int32_t pad, uint32_t normconst)
{
   uint32_t nrm = (p + 2) * (p + 2);
   uint16_t lZ = count_leading_zero_s(nrm);
   nrm = fxrecip22(nrm << lZ) >> (32 - lZ - 1);//nrm is U32

   int32_t i, j;
   vec16u *Iu, *Im, *Ib;
   vec16u up, middle, bot, temp, nrm_v;
   nrm_v = (vec16u)(nrm>>16);
   vec32u ret;

   int32_t wstride = instride;

   for(i = 0; i < h; i++)
   {
      Iu = Im = Ib = in + i*wstride - pad;
      Iu -= wstride;
      Ib += wstride;

      for(j = 0; j < w+2*pad; j++)
      {
         up     = Iu[j];
         middle = Im[j];
         bot    = Ib[j];

         temp   = (vec16u)(up + middle*p + bot);
         ret    = ((vec32u)temp)*((vec32u)nrm_v);
         T[j]   = ret;

      }
      triangleFilt1x_c_row_norm(in, T, out, w, i, p, pad, instride, outstride, normconst);
   }
}

static uint64_t getA(uint64_t x){return (x>>56)&0xFF;}
static uint64_t getB(uint64_t x){return (x>>48)&0xFF;}
static uint64_t getM(uint64_t x){return (x>>56)&0xFF;}
static uint64_t getN(uint64_t x){return (x>>48)&0xFF;}
static uint64_t getO(uint64_t x){return (x>>40)&0xFF;}

/**
 * Compute multiplication of two uint64 as:\n
 *   A[63..56]*B[63..56] + A[63..56]*B[55..48] +
 *   A[55..48]*B[63..56]
 */
static uint64_t mul1(uint64_t abcd, uint64_t mnop){
   uint64_t am = getA(abcd)*getM(mnop) << 48;
   uint64_t an = getA(abcd)*getN(mnop) << 40;
   uint64_t bm = getB(abcd)*getM(mnop) << 40;

   return (am  +
         an +
         bm  ) & 0xFFFF000000000000;
}
/**
 * Compute multiplication of two uint64 as:\n
 * A[63..56]*B[63..56] + A[63..56]*B[55..48]
 */
static uint64_t mul2(uint64_t abcd, uint64_t mnop){
   uint64_t am = getA(abcd)*getM(mnop) << 48;
   uint64_t an = getA(abcd)*getN(mnop) << 40;

   return (am  +
         an)  & 0xFFFF000000000000;
}


ufx16p0 fxrecip12(ufx16p1 d){
   uint64_t dn = (uint64_t)d << 48;
   uint64_t x1 = (dn<<1) & 0xFFFE000000000000ULL;
   uint64_t k0, k1, k2, s0, s1;

   if(dn > (46006ULL<<48)){
      s1 = 3;
      s0 = 2;
      k1 =   181ULL<<56;
      k2 = 30945ULL<<48;
      k0 = 63962ULL<<48;

      k0 -= 9ULL<<48;
   }else{
      s1 = 1;
      s0 = 4;
      k1 =    31ULL<<56;
      k2 = 58691ULL<<48;
      k0 = 65532ULL<<48;

      k0 -= 9ULL<<48;
   }

   uint64_t x2   = mul1(x1, x1);
   uint64_t k1x1 = mul2(k1, x1);
   uint64_t a    = k0 - x1 + ((k1x1 >> s0) & 0xFFFF000000000000ULL);
   uint64_t b    = k2 - ((x1 >> s1) & 0xFFFF000000000000ULL);
   uint64_t bx2  = mul1(b, x2);
   uint64_t abx2 = a+bx2;
   return   abx2 >> 48;
}

static uint64_t mul4(uint64_t abcd, uint64_t mnop){
   uint64_t am = getA(abcd)*getM(mnop) << 48;
   uint64_t an = getA(abcd)*getN(mnop) << 40;
   uint64_t ao = getA(abcd)*getO(mnop) << 32;
   uint64_t bm = getB(abcd)*getM(mnop) << 40;
   uint64_t bn = getB(abcd)*getN(mnop) << 32;

  return (am +
          an +
          ao +
          bm +
          bn) & 0xFFFFFFFF00000000;
}



ufx32p0 fxrecip22(ufx32p1 d){
   uint64_t dn   = (uint64_t)d<<32;
   uint64_t r0   = (uint64_t)fxrecip12(d>>16)<<48;
   uint64_t h0   = 0x8000000000000000ULL - mul4(r0, dn);
   uint64_t r0h0 = mul4(r0,h0)<<1;
   uint64_t r1 = r0 + r0h0;
   return   r1 >> 32;
}


// calculate O, M for interpolation
// O is 3.13 and M is 2.14


void fxgradQuantize_c_row_evb(vec16u *O, vec16u *M, vec32u *O0, vec32u *O1, vec32u *M0, vec32u *M1,
    int32_t nb, int32_t h, int32_t w, uint32_t norm, uint32_t nOrients, int32_t full, int32_t interpolate)
{
    int32_t i;
    vec32u o0, o1;
    vec32u o, od, m;
    uint32_t Pi30 = 3373259427ll; // 3.141592654 * pow(2,30)
    uint32_t invPi30 = fxrecip22(Pi30) >> 1; // U0.32

    const uint32_t oMult = (uint32_t)nOrients * (full ? ((invPi30 >> 1) >> 16) : (invPi30 >> 16)); //U16.16
    //const uint32_t oMax = nOrients*nb;
    const uint32_t oMax = nOrients;

    //printf(" oMax: %d  ", oMax);

    if (interpolate)
    {
        for (i = 0; i < w; i++)
        {
            o = vmul((vec32u)O[i], (vec32u)(oMult >> 1)); //U4.28

            o0 = o >> 28;//(int)o;

            od = o - (vec32u)(o0 << 28); //U4.28

            //o0 = vmul((vec32u)o0, (vec32u)nb);

            vif (o0 >= oMax)
                o0 = 0;
            vendif
            
            O0[i] = o0;
            o1 = o0 + 1;

            vif (o1 == oMax)
                o1 = 0;
            vendif
            O1[i] = o1;
            // 2.30
            m = vmul((vec32u)M[i], (vec32u)((norm >> 16) << 1));
            // 4.12 * 2.14 ===>6.26
            M1[i] = vmul((od >> 16), (m >> 16));

            M0[i] = (m >> 4) - M1[i]; //U6.26

        }
    }
    else
    {
        for (i = 0; i < w; i++)
        {
            o = vmul((vec32u)O[i], (vec32u)(oMult >> 1));

            o0 = (o + (1 << 27)) >> 28;

            //o0 = vmul((vec32u)o0, (vec32u)nb);

            vif (o0 >= oMax)
                o0 = 0;
            vendif    
            O0[i] = o0;
            M0[i] = vmul((vec32u)M[i], (vec32u)((norm >> 16) >> 3));
            M1[i] = 0;
            O1[i] = 0;
        }
    }
}

void locateHistBin_evb(vec16u *H0, vec16u *H1, vec16u *H2, vec16u *H3, vec16u *H4, vec16u *H5, 
                       vec32u index, vec32u value, int32_t offset)
{
   vif(index == 0)
   {
      //H0[(vec16s)offset] = (vec16u)((vec32u)H0[(vec16s)offset] + value);
      vstore(H0, (vec16s)offset, (vec16u)((vec32u)vload(H0,(vec16s)offset) + value));
      return;
   }vendif
   
   vif(index == 1)
   {
      //H1[(vec16s)offset] = (vec16u)((vec32u)H1[(vec16s)offset] + value);
      vstore(H1, (vec16s)offset, (vec16u)((vec32u)vload(H1,(vec16s)offset) + value));
      return;
   }vendif
   
   vif(index == 2)
   {
      //H2[(vec16s)offset] = (vec16u)((vec32u)H2[(vec16s)offset] + value);
      vstore(H2, (vec16s)offset, (vec16u)((vec32u)vload(H2,(vec16s)offset) + value));
      return;
   }vendif
   
   vif(index == 3)
   {
      //H3[(vec16s)offset] = (vec16u)((vec32u)H3[(vec16s)offset] + value);
      vstore(H3, (vec16s)offset, (vec16u)((vec32u)vload(H3,(vec16s)offset) + value));
      return;
   }vendif
   
   vif(index == 4)
   {
      //H4[(vec16s)offset] = (vec16u)((vec32u)H4[(vec16s)offset] + value);
     vstore(H4, (vec16s)offset, (vec16u)((vec32u)vload(H4,(vec16s)offset) + value));
   }
   velse
   {
      //H5[(vec16s)offset] = (vec16u)((vec32u)H5[(vec16s)offset] + value);
     vstore(H5, (vec16s)offset, (vec16u)((vec32u)vload(H5,(vec16s)offset) + value));
   }vendif
   
}

// non-trilinear interpolation histogram calculation
// either orientation interpolation or no interpolation(nearest neighborhood calculation)
// each chunk contains input data with size of (chunkwidth, chunkheight) to calculate 
// histogram in size of (chunkwidth/bin, chunkheight/bin)
//
// softbin controls orientation interpolation mode
// softbin >= 0,   bi-linear orientation interpolation
// softbin <  0,   no orientation interpolation (nearest neighborhood calculation)
// softbin%2 == 0, no spatial interpolation
// softbin%2 == 1, spatial interpolation    
//
// bin controls histogram image resolution relative to input magnitude/orientation image resolution
// bin = 1 is one to one mapping so no spatial interpolation regardless of softbin setting

void fxgradHist_c_row_nontrilinear(vec16u* M, vec16u* O,
                 vec16u *bin0, vec16u *bin1, vec16u *bin2, vec16u *bin3, vec16u *bin4, vec16u *bin5,
                 int32_t h, int32_t w, int32_t instride, int32_t outstride,
		 int32_t hb, int32_t wb,
                 int32_t nb, int32_t bin, int32_t nOrientations, int32_t softBin, int32_t full,
                 vec32u *M0, vec32u*M1, vec32u*O0, vec32u *O1, int32_t *offset)
{
    uint32_t binInv;

    if (bin == 1)
    {
        binInv = (1 << 31);
    }
    else
    {
        uint16_t lZ = count_leading_zero_s((uint32_t)bin);
        binInv = fxrecip22(bin << lZ) >> 2;
    }

    uint32_t sInv = binInv, sInv2;

    if (bin == 1)
        sInv2 = (1 << 31); //1 / s / s; 1.31
    else
        sInv2 = ((binInv >> 16) * (binInv >> 16)) << 1; // U1.31

    vec32u *H1;
    int32_t x, y;
    int32_t xb, init;

    for(int i = 0; i < hb; i++)
    {
        for(int j = 0; j < wb; j++)
        {
            bin0[i*outstride+j] = 0;
            bin1[i*outstride+j] = 0;
            bin2[i*outstride+j] = 0;
            bin3[i*outstride+j] = 0;
            bin4[i*outstride+j] = 0;
            bin5[i*outstride+j] = 0;
        }
    }

    // M0 and M1 is 6.26, right shift by 12 to get 2.14
    for (x = 0; x < hb*bin; x++)
    {

        // M0, M1 in 6.26; O0 and O1 in 4.28
        fxgradQuantize_c_row_evb(O + x*instride, M + x*instride, O0, O1, M0, M1, nb, h, w, sInv2, nOrientations, full, softBin >= 0);

        // no interpolation in either orienation or spatial bin
        // final histogram is in 2:14
        if (softBin < 0 && softBin % 2 == 0)
        {
            int32_t start_idx = offset[x]*outstride;

            if (bin == 1)
            {
                for (y = 0; y < wb*bin;)
                {
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    y++;
                    start_idx++;
                }
            }
            else if (bin == 2)
            {
                for (y = 0; y < wb*bin;)
                {
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    y++;
                    
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    y++;
                    start_idx++;
                 }
            }
            else if (bin == 3)
            {
                for (y = 0; y < wb*bin;)
                {
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    y++;
                    
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    y++;
                    
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    y++;
                    start_idx++;
                }
             }
            else if (bin == 4)
            {
                for (y = 0; y < wb*bin;)
                {
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    y++;
                    
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    y++;
                    
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    y++;
                    
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    y++;
                    start_idx++;
                }
            }
            else
            {
                for (y = 0; y < wb*bin;)
                {
                    for (int32_t y1 = 0; y1 < bin; y1++)
                    {
                        locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                        y++;
                    }
                    start_idx++;
                }
            }
        }
        else // orientation interpolation (softbin %2 == 0 or bin == 1)
        {
            int32_t start_idx = offset[x]*outstride;

            if (bin == 1)
            {
                for (y = 0; y < wb*bin;)
                {
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O1[y], (M1[y] >> 12), start_idx);
                    y++; 
                    start_idx++;
                }
            }
            else if (bin == 2)
            {
                for (y = 0; y < wb*bin;)
                {

                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O1[y], (M1[y] >> 12), start_idx);
                    y++;
 
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O1[y], (M1[y] >> 12), start_idx);
                    y++;
                    start_idx++;
                }
            }
            else if (bin == 3)
            {
                for (y = 0; y < wb*bin;)
                {
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O1[y], (M1[y] >> 12), start_idx);
                    y++;
                    
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O1[y], (M1[y] >> 12), start_idx);
                    y++;
                    
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O1[y], (M1[y] >> 12), start_idx);
                    y++;
                    start_idx++;
                }
            }
            else if (bin == 4)
            {
                for (y = 0; y < wb*bin;)
                {
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O1[y], (M1[y] >> 12), start_idx);
                    y++;
                    
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O1[y], (M1[y] >> 12), start_idx);
                    y++;
                    
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O1[y], (M1[y] >> 12), start_idx);
                    y++;
                    
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O1[y], (M1[y] >> 12), start_idx);
                    y++;
                    start_idx++;
                }
            }
            else 
            {
                for (y = 0; y < wb*bin;)
                {
                    for (int y1 = 0; y1 < bin; y1++)
                    {
                        locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (M0[y] >> 12), start_idx);
                        locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O1[y], (M1[y] >> 12), start_idx);
                        y++;
                    }
                    start_idx++;
                }
            }
        }
    }
}


void fxgradQuantize_c_row_evb_pixel1(vec16u *O, vec16u *M,
    int32_t y, uint32_t norm, uint32_t nOrients, int32_t full, int32_t interpolate, vec32u *O0, vec32u *O1, vec32u *M0, vec32u *M1)
{
   

    int32_t i;
    vec32u o0, o1;
    vec32u o, od, m;
    uint32_t Pi30 = 3373259427ll; // 3.141592654 * pow(2,30)
    uint32_t invPi30 = fxrecip22(Pi30) >> 1; // U0.32

    const uint32_t oMult = (uint32_t)nOrients * (full ? ((invPi30 >> 1) >> 16) : (invPi30 >> 16)); //U16.16
    const uint32_t oMax = nOrients;


    if (interpolate)
    {
        {
            o = vmul((vec32u)O[y], (vec32u)(oMult >> 1)); //U4.28
            
            o0 = o >> 28;//(int)o;

            od = o - (vec32u)(o0 << 28); //U4.28

            vif (o0 >= oMax)
                o0 = 0;vendif
            *O0 = o0;
            o1 = o0 + 1;

            vif (o1 == oMax)
                o1 = 0;vendif
            *O1 = o1;
            // 2.30
            m = vmul((vec32u)M[y], (vec32u)((norm >> 16) << 1));
            // 4.12 * 2.14 ===>6.26
            *M1 = vmul((od >> 16), (m >> 16));

            *M0 = (m >> 4) - *M1; //U6.26

        }
    }
    else
    {
        {
            o = vmul((vec32u)O[y], (vec32u)(oMult >> 1));

            o0 = (o + (1 << 27)) >> 28;

            //o0 = vmul((vec32u)o0, (vec32u)nb);

            vif (o0 >= oMax)
                o0 = 0;vendif
            *O0 = o0;
            *M0 = vmul((vec32u)M[y], (vec32u)((norm >> 16) >> 3));
            *M1 = 0;
            *O1 = 0;
        }
    }

    

}


void fxgradHist_c_row_nontrilinear_pixel(vec16u* M, vec16u* O,
                 vec16u *bin0, vec16u *bin1, vec16u *bin2, vec16u *bin3, vec16u *bin4, vec16u *bin5,
                 int32_t h, int32_t w, int32_t instride, int32_t outstride,
		 int32_t hb, int32_t wb,
                 int32_t nb, int32_t bin, int32_t nOrientations, int32_t softBin, int32_t full,
                 vec32u *M0, vec32u*M1, vec32u*O0, vec32u *O1, int32_t *offset)
{
    uint32_t binInv;

    if (bin == 1)
    {
        binInv = (1 << 31);
    }
    else
    {
        uint16_t lZ = count_leading_zero_s((uint32_t)bin);
        binInv = fxrecip22(bin << lZ) >> 2;
    }

    uint32_t sInv = binInv, sInv2;

    if (bin == 1)
        sInv2 = (1 << 31); //1 / s / s; 1.31
    else
        sInv2 = ((binInv >> 16) * (binInv >> 16)) << 1; // U1.31

    vec32u *H1;
    int32_t x, y;
    int32_t xb, init;

    interpolation_Result ret;

    vec32u o0x, o1x, m0x, m1x;


    for(int i = 0; i < hb; i++)
    {
        for(int j = 0; j < wb; j++)
        {
            bin0[i*outstride+j] = 0;
            bin1[i*outstride+j] = 0;
            bin2[i*outstride+j] = 0;
            bin3[i*outstride+j] = 0;
            bin4[i*outstride+j] = 0;
            bin5[i*outstride+j] = 0;
              
        }
    }
   

    // M0 and M1 is 6.26, right shift by 12 to get 2.14
    for (x = 0; x < hb*bin; x++)
    {
        // no interpolation in either orienation or spatial bin
        // final histogram is in 2:14
        if (softBin < 0 && softBin % 2 == 0)
        {
            int32_t start_idx = offset[x]*outstride;

            if (bin == 1)
            {
                for (y = 0; y < wb*bin;)
                {
                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    y++;
                    start_idx++;
                }
            }
            else if (bin == 2)
            {
                for (y = 0; y < wb*bin;)
                {
                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    y++;
                    
                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    y++;
                    start_idx++;
                 }
            }
            else if (bin == 3)
            {
                for (y = 0; y < wb*bin;)
                {
                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    y++;
                    
                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    y++;
                    
                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    y++;
                    start_idx++;
                }
             }
            else if (bin == 4)
            {
                for (y = 0; y < wb*bin;)
                {
                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    y++;
                    
                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    y++;
                    
                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    y++;
                    
                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    y++;
                    start_idx++;
                }
            }
            else
            {
                for (y = 0; y < wb*bin;)
                {
                    for (int32_t y1 = 0; y1 < bin; y1++)
                    {
                        fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                        locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                        y++;
                    }
                    start_idx++;
                }
            }
        }
        else // orientation interpolation (softbin %2 == 0 or bin == 1)
        {
            int32_t start_idx = offset[x]*outstride;

            if (bin == 1)
            {
                for (y = 0; y < wb*bin;)
                {
                  
                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o1x, (m1x >> 12), start_idx);
                    y++; 
                    start_idx++;
                }
            }
            else if (bin == 2)
            {
                for (y = 0; y < wb*bin;)
                {

                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o1x, (m1x >> 12), start_idx);
                    y++;
 
                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o1x, (m1x >> 12), start_idx);
                    y++;
                    start_idx++;
                }
            }
            else if (bin == 3)
            {
                for (y = 0; y < wb*bin;)
                {
                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o1x, (m1x >> 12), start_idx);
                    y++;
                    
                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o1x, (m1x >> 12), start_idx);
                    y++;
                    
                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o1x, (m1x >> 12), start_idx);
                    y++;
                    start_idx++;
                }
            }
            else if (bin == 4)
            {
                for (y = 0; y < wb*bin;)
                {
                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o1x, (m1x >> 12), start_idx);
                    y++;
                    
                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o1x, (m1x >> 12), start_idx);
                    y++;
                    
                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o1x, (m1x >> 12), start_idx);
                    y++;
                    
                    fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o1x, (m1x >> 12), start_idx);
                    y++;
                    start_idx++;
                }
            }
            else 
            {
                for (y = 0; y < wb*bin;)
                {
                    for (int y1 = 0; y1 < bin; y1++)
                    {
                        fxgradQuantize_c_row_evb_pixel1(O + x*instride, M + x*instride, y, sInv2, nOrientations, full, softBin >= 0, &o0x, &o1x, &m0x, &m1x);
                        locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o0x, (m0x >> 12), start_idx);
                        locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, o1x, (m1x >> 12), start_idx);
                        y++;
                    }
                    start_idx++;
                }
            }
        }
    }
}


void fxgradQuantize_c_row_trilinear(vec16u *O, vec16u *M, vec32u *O0, vec32u *O1, vec32u *M0, vec32u *M1,
    int32_t nb, int32_t h, int32_t w, int32_t bin, uint32_t norm, int32_t nOrients, int32_t full, int32_t interpolate)
{
    int32_t i;
    vec32u o0, o1;
    vec32u o, od, m;
    uint32_t Pi30 = 3373259427ll; // 3.141592654 * pow(2,30)
    uint32_t invPi30 = fxrecip22(Pi30) >> 1; // U0.32

    const uint32_t oMult = (uint32_t)nOrients * (full ? ((invPi30 >> 1) >> 16) : (invPi30 >> 16)); //U16.16
    const uint32_t oMax = nOrients;

    if (interpolate)
    {
        for (i = 0; i < w+bin; i++)
        {
            o = vmul((vec32u)O[i], (vec32u)(oMult >> 1)); // 4.28

            o0 = o >> 28;//(int)o;

            od = o - (vec32u)(o0 << 28); //U4.28

            //o0 *= nb;
            vif (o0 >= oMax)
                o0 = 0;vendif;
            O0[i] = o0;
            
            //o1 = o0 + nb;
            o1 = o0 + 1;
            vif (o1 == oMax)
                o1 = 0;vendif;
            O1[i] = o1;
            
            m = vmul((vec32u)M[i],  (vec32u)((norm >> 16) << 1)); //U2,30

            M1[i] = vmul((od >> 16), (m >> 16)); //U6.26
            M0[i] = (m >> 4) - M1[i]; //U6.26

        }
    }
    else
    {
        for (i = 0; i < w+bin; i++)
        {
            o = vmul((vec32u)O[i], (vec32u)(oMult >> 1)); //4.28
            o0 = (o + (1 << 27)) >> 28;
            //o0 *= nb;
            vif (o0 >= oMax)
                o0 = 0;vendif;
            O0[i] = o0;
            M0[i] = vmul((vec32u)M[i], (vec32u)((norm >> 16) >> 3));
            M1[i] = 0;
            O1[i] = 0;
        }
    }
}



// trilinear histogram kernel function
// chunk is padding left and above with size of bin
// chunk h and w have to be rounded to be multuple of bin number
// chunk is padded left and above with padding size of bin, so actual width and height is w+bin and h+bin
// each chunk contains input data(M and O) needed to calculate histogram of region with size of w/bin, h/bin
// using trilinear calculation (spatial interpolation + orientation interpolation)
// in this mode, bin > 1 as bin == 1 means no spatial interpolation inherently

void fxgradHist_c_row_trilinear(vec16u* M, vec16u* O, 
                vec16u* bin0, vec16u *bin1, vec16u *bin2, vec16u *bin3, vec16u *bin4, vec16u *bin5, 
                int32_t h, int32_t w, int32_t instride, int32_t outstride, 
                int32_t wb, int32_t hb, int32_t nb, int32_t bin, int32_t nOrientations, int32_t softBin, int32_t full,
                vec32u *M0, vec32u*M1, vec32u*O0, vec32u *O1) 
{
    uint32_t binInv;

    if (bin == 1)
    {
        binInv = (1 << 31);
    }
    else
    {
        uint16_t lZ = count_leading_zero_s((uint32_t)bin);
        binInv = fxrecip22(bin << lZ) >> 2; 
    }

    uint32_t sInv = binInv, sInv2;

    if (bin == 1)
        sInv2 = (1 << 31); //1 / s / s; 1.31
    else
        sInv2 = ((binInv >> 16) * (binInv >> 16)) << 1; // U1.31

    vec32u *H1;
  
    int32_t x, y;
    int32_t xb, init;
    uint32_t ONE_F = 2147483648ll; 


    // 1/bin-1.0f: this is based on how padding is done
    // sinV is 1.31
    init = -(ONE_F- (sInv>>1)); 
    // convert to 2.14
    init >>= 17; 
    xb = init;
    bool hasUp, hasBot;

    // move M and O to top/left of padded chunk buffer 
    M = M-bin*instride-bin;
    O = O-bin*instride-bin;

    for(int i = 0; i < hb; i++)
    {
        for(int j = 0; j < wb; j++)
        {
            bin0[i*outstride+j] = 0;
            bin1[i*outstride+j] = 0;
            bin2[i*outstride+j] = 0;
            bin3[i*outstride+j] = 0;
            bin4[i*outstride+j] = 0;
            bin5[i*outstride+j] = 0;

        }
    }

    for (x = 0; x < hb*bin+bin; x++)
    {
        fxgradQuantize_c_row_trilinear(O + x*instride, M + x*instride, O0, O1, M0, M1, nb, h, w, bin, sInv2, nOrientations, full, softBin >= 0 );

        uint32_t ms[4], xyd, xd, yd; int32_t yb, xb0, yb0;
        uint32_t ONE2_14 = 16384; // 1 in 2.14 format

        hasUp  = (xb >= 0);
        xb0 = xb>>14;
        hasBot = (xb0 < hb-1); 
        xb0 = xb0<<14;
          
        xd = xb-xb0;
        xb += sInv>>17;  

        yb = init;
        for(y = 0; y < wb*bin+bin; y++)
        {
            yb0 = yb>>14;
            yb0 = yb0 <<14;
            yd = yb-yb0;
            yb += sInv>>17;  

            xyd = ((xd*yd)<<2)>>16;
            ms[0] = ONE2_14 - xd - yd + xyd;
            ms[1] = xd - xyd;
            ms[2] = yd - xyd;
            ms[3] = xyd;
            
            // floored intger point in histogtam
            //H0 = H + (yb0>>14) + (xb0>>14)*wb;
            int32_t start_idx = (yb0>>14) + (xb0>>14)*outstride;
            if( y < bin)
            {
                if(hasUp)
                {
                    //ms 2.14 M 6.26
                    vec32u ti = ms[2] * (M0[y] >> 12);
                    // convert to 2.14
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (ti >> 14), start_idx+1);

                    ti = ms[2] * (M1[y] >> 12);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O1[y], (ti >> 14), start_idx+1);
                }
                if(hasBot)
                {
                    //ms 2.14 M 6.26
                    vec32u ti = ms[3] * (M0[y] >> 12);
                    //2.14
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (ti >> 14), start_idx+outstride+1);

                    ti = ms[3] * (M1[y]>>12);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O1[y], (ti >> 14), start_idx+outstride+1);
                }
            }
            else if( y >= bin && y < wb*bin)
            {
                if(hasUp)
                {
                    vec32u ti = ms[0] * (M0[y] >> 12);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (ti >> 14), start_idx);

                    ti =  ms[0] * (M1[y] >> 12);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O1[y], (ti >> 14), start_idx); 
                    
                    ti =  ms[2] * (M0[y] >> 12);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (ti >> 14), start_idx+1);
                    
                    ti =  ms[2] * (M1[y] >> 12);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O1[y], (ti >> 14), start_idx+1);
                }  

                if(hasBot)
                {
                    vec32u ti = ms[1] * (M0[y] >> 12);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (ti >> 14), start_idx+outstride);

                    ti =  ms[1] * (M1[y] >> 12);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O1[y], (ti >> 14), start_idx+outstride); 

                    ti =  ms[3] * (M0[y] >> 12);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (ti >> 14), start_idx+outstride+1);

                    ti =  ms[3] * (M1[y] >> 12);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O1[y], (ti >> 14), start_idx+outstride+1);  

                } 
            }
            else
            {
                if(hasUp)
                {
                    vec32u ti = ms[0] * (M0[y] >> 12);    
                    // convert to 2.14
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (ti >> 14), start_idx);

                    ti = ms[0] * (M1[y]>>12);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O1[y], (ti >> 14), start_idx);
                }

                if(hasBot)
                {
                    vec32u ti = ms[1] * (M0[y] >> 12);    
                    // convert to 2.14
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O0[y], (ti >> 14), start_idx+outstride);

                    ti = ms[1] * (M1[y]>>12);
                    locateHistBin_evb(bin0, bin1, bin2, bin3, bin4, bin5, O1[y], (ti >> 14), start_idx+outstride);

                } 
            }
        } 
    }
}                


// 16b L channel LUT
int16u lTableData[1064] = {
   0, 214, 428, 642, 856, 1070, 1284, 1498, 1712, 1927, 2134, 2328, 2511, 2684,
   2848, 3005, 3155, 3299, 3437, 3570, 3698, 3823, 3943, 4060, 4174, 4284, 4391, 4496, 4598, 4698, 4796, 4891, 4985,
   5076, 5166, 5253, 5340, 5424, 5507, 5589, 5669, 5748, 5826, 5902, 5978, 6052, 6125, 6197, 6268, 6338, 6407, 6475,
   6542, 6609, 6674, 6739, 6803, 6866, 6929, 6991, 7052, 7112, 7172, 7231, 7290, 7348, 7405, 7462, 7518, 7573, 7628,
   7683, 7737, 7791, 7844, 7896, 7948, 8000, 8051, 8102, 8152, 8202, 8252, 8301, 8350, 8398, 8446, 8494, 8541, 8588,
   8634, 8681, 8727, 8772, 8817, 8862, 8907, 8951, 8995, 9039, 9082, 9125, 9168, 9210, 9253, 9295, 9336, 9378, 9419,
   9460, 9500, 9541, 9581, 9621, 9661, 9700, 9740, 9779, 9817, 9856, 9894, 9933, 9970, 10008, 10046, 10083, 10120,
   10157, 10194, 10231, 10267, 10303, 10339, 10375, 10411, 10446, 10481, 10517, 10551, 10586, 10621, 10655, 10690,
   10724, 10758, 10792, 10825, 10859, 10892, 10925, 10958, 10991, 11024, 11057, 11089, 11121, 11154, 11186, 11218,
   11249, 11281, 11313, 11344, 11375, 11406, 11437, 11468, 11499, 11530, 11560, 11591, 11621, 11651, 11681, 11711,
   11741, 11771, 11800, 11830, 11859, 11888, 11917, 11947, 11975, 12004, 12033, 12062, 12090, 12119, 12147, 12175,
   12203, 12231, 12259, 12287, 12315, 12342, 12370, 12397, 12425, 12452, 12479, 12506, 12533, 12560, 12587, 12614,
   12640, 12667, 12694, 12720, 12746, 12772, 12799, 12825, 12851, 12877, 12902, 12928, 12954, 12979, 13005, 13030,
   13056, 13081, 13106, 13131, 13156, 13181, 13206, 13231, 13256, 13281, 13305, 13330, 13354, 13379, 13403, 13427,
   13452, 13476, 13500, 13524, 13548, 13572, 13595, 13619, 13643, 13666, 13690, 13714, 13737, 13760, 13784, 13807,
   13830, 13853, 13876, 13899, 13922, 13945, 13968, 13991, 14013, 14036, 14059, 14081, 14104, 14126, 14148, 14171,
   14193, 14215, 14237, 14260, 14282, 14304, 14326, 14347, 14369, 14391, 14413, 14434, 14456, 14478, 14499, 14521,
   14542, 14563, 14585, 14606, 14627, 14648, 14670, 14691, 14712, 14733, 14754, 14775, 14795, 14816, 14837, 14858,
   14878, 14899, 14919, 14940, 14960, 14981, 15001, 15022, 15042, 15062, 15082, 15103, 15123, 15143, 15163, 15183,
   15203, 15223, 15243, 15263, 15282, 15302, 15322, 15341, 15361, 15381, 15400, 15420, 15439, 15459, 15478, 15497,
   15517, 15536, 15555, 15575, 15594, 15613, 15632, 15651, 15670, 15689, 15708, 15727, 15746, 15765, 15783, 15802,
   15821, 15840, 15858, 15877, 15895, 15914, 15933, 15951, 15969, 15988, 16006, 16025, 16043, 16061, 16079, 16098,
   16116, 16134, 16152, 16170, 16188, 16206, 16224, 16242, 16260, 16278, 16296, 16314, 16332, 16349, 16367, 16385,
   16402, 16420, 16438, 16455, 16473, 16490, 16508, 16525, 16543, 16560, 16577, 16595, 16612, 16629, 16647, 16664,
   16681, 16698, 16715, 16732, 16750, 16767, 16784, 16801, 16818, 16835, 16851, 16868, 16885, 16902, 16919, 16936,
   16952, 16969, 16986, 17002, 17019, 17036, 17052, 17069, 17085, 17102, 17118, 17135, 17151, 17168, 17184, 17200,
   17217, 17233, 17249, 17266, 17282, 17298, 17314, 17330, 17346, 17363, 17379, 17395, 17411, 17427, 17443, 17459,
   17475, 17491, 17506, 17522, 17538, 17554, 17570, 17586, 17601, 17617, 17633, 17648, 17664, 17680, 17695, 17711,
   17727, 17742, 17758, 17773, 17789, 17804, 17819, 17835, 17850, 17866, 17881, 17896, 17912, 17927, 17942, 17957,
   17973, 17988, 18003, 18018, 18033, 18048, 18064, 18079, 18094, 18109, 18124, 18139, 18154, 18169, 18184, 18198,
   18213, 18228, 18243, 18258, 18273, 18288, 18302, 18317, 18332, 18346, 18361, 18376, 18390, 18405, 18420, 18434,
   18449, 18463, 18478, 18493, 18507, 18522, 18536, 18550, 18565, 18579, 18594, 18608, 18622, 18637, 18651, 18665,
   18680, 18694, 18708, 18722, 18737, 18751, 18765, 18779, 18793, 18807, 18821, 18836, 18850, 18864, 18878, 18892,
   18906, 18920, 18934, 18948, 18962, 18975, 18989, 19003, 19017, 19031, 19045, 19059, 19072, 19086, 19100, 19114,
   19127, 19141, 19155, 19168, 19182, 19196, 19209, 19223, 19237, 19250, 19264, 19277, 19291, 19304, 19318, 19331,
   19345, 19358, 19372, 19385, 19399, 19412, 19425, 19439, 19452, 19465, 19479, 19492, 19505, 19519, 19532, 19545,
   19558, 19572, 19585, 19598, 19611, 19624, 19637, 19651, 19664, 19677, 19690, 19703, 19716, 19729, 19742, 19755,
   19768, 19781, 19794, 19807, 19820, 19833, 19846, 19859, 19871, 19884, 19897, 19910, 19923, 19936, 19948, 19961,
   19974, 19987, 20000, 20012, 20025, 20038, 20050, 20063, 20076, 20088, 20101, 20114, 20126, 20139, 20151, 20164,
   20177, 20189, 20202, 20214, 20227, 20239, 20252, 20264, 20277, 20289, 20301, 20314, 20326, 20339, 20351, 20363,
   20376, 20388, 20400, 20413, 20425, 20437, 20450, 20462, 20474, 20486, 20499, 20511, 20523, 20535, 20547, 20560,
   20572, 20584, 20596, 20608, 20620, 20632, 20644, 20656, 20668, 20681, 20693, 20705, 20717, 20729, 20741, 20753,
   20765, 20776, 20788, 20800, 20812, 20824, 20836, 20848, 20860, 20872, 20884, 20895, 20907, 20919, 20931, 20943,
   20954, 20966, 20978, 20990, 21001, 21013, 21025, 21037, 21048, 21060, 21072, 21083, 21095, 21107, 21118, 21130,
   21141, 21153, 21165, 21176, 21188, 21199, 21211, 21222, 21234, 21245, 21257, 21268, 21280, 21291, 21303, 21314,
   21326, 21337, 21349, 21360, 21371, 21383, 21394, 21405, 21417, 21428, 21440, 21451, 21462, 21473, 21485, 21496,
   21507, 21519, 21530, 21541, 21552, 21564, 21575, 21586, 21597, 21608, 21620, 21631, 21642, 21653, 21664, 21675,
   21686, 21697, 21709, 21720, 21731, 21742, 21753, 21764, 21775, 21786, 21797, 21808, 21819, 21830, 21841, 21852,
   21863, 21874, 21885, 21896, 21907, 21918, 21929, 21939, 21950, 21961, 21972, 21983, 21994, 22005, 22016, 22026,
   22037, 22048, 22059, 22070, 22080, 22091, 22102, 22113, 22123, 22134, 22145, 22156, 22166, 22177, 22188, 22198,
   22209, 22220, 22230, 22241, 22252, 22262, 22273, 22284, 22294, 22305, 22315, 22326, 22336, 22347, 22358, 22368,
   22379, 22389, 22400, 22410, 22421, 22431, 22442, 22452, 22463, 22473, 22484, 22494, 22505, 22515, 22525, 22536,
   22546, 22557, 22567, 22577, 22588, 22598, 22608, 22619, 22629, 22639, 22650, 22660, 22670, 22681, 22691, 22701,
   22712, 22722, 22732, 22742, 22753, 22763, 22773, 22783, 22793, 22804, 22814, 22824, 22834, 22844, 22855, 22865,
   22875, 22885, 22895, 22905, 22915, 22926, 22936, 22946, 22956, 22966, 22976, 22986, 22996, 23006, 23016, 23026,
   23036, 23046, 23056, 23066, 23076, 23086, 23096, 23106, 23116, 23126, 23136, 23146, 23156, 23166, 23176, 23186,
   23196, 23206, 23216, 23225, 23235, 23245, 23255, 23265, 23275, 23285, 23295, 23304, 23314, 23324, 23334, 23344,
   23353, 23363, 23373, 23383, 23393, 23402, 23412, 23422, 23432, 23441, 23451, 23461, 23470, 23480, 23490, 23500,
   23509, 23519, 23529, 23538, 23548, 23558, 23567, 23577, 23586, 23596, 23606, 23615, 23625, 23635, 23644, 23654,
   23663, 23673, 23682, 23692, 23702, 23711, 23721, 23730, 23740, 23749, 23759, 23768, 23778, 23787, 23797, 23806,
   23816, 23825, 23835, 23844, 23853, 23863, 23872, 23882, 23891, 23901, 23910, 23919, 23929, 23938, 23948, 23957,
   23966, 23976, 23985, 23994, 24004, 24013, 24022, 24032, 24041, 24050, 24060, 24069, 24078, 24088, 24097, 24106,
   24115, 24125, 24134, 24143, 24152, 24162, 24171, 24180, 24189, 24199, 24208, 24217, 24226, 24235, 24245, 24254,
   24263, 24272, 24272, 24272, 24272, 24272, 24272, 24272, 24272, 24272, 24272, 24272, 24272, 24272, 24272, 24272,
   24272, 24272, 24272, 24272, 24272, 24272, 24272, 24272, 24272, 24272, 24272, 24272, 24272, 24272, 24272, 24272,
   24272, 24272, 24272, 24272, 24272, 24272, 24272, 24272, 24272
};


/*
   Color conversion from RGB colorspace to LUV
   (unsigned 8-bit -> unsigned 16-bit (U2.14))

   \param rgbImgIn         -[Input]    Pointer to the input image data.
   \param luvImgOut        -[Output]   Pointer to the output image data.
   \param numPixels        -[Input]    Image Height x Image Width.
   \param nrm              -[Input]    Normalization constant.
   \param lTable           -[Input]    Pointer to the input LUT data.
   \param strideIn         -[Input]    Stride of the input image data.
   \param strideOut        -[Output]   Stride of the output image data.
   \param chunkWidth       -[Input]    Processing chunk width.
   \param chunkHeight      -[Input]    Processing chunk height.
   */
void apu_aggcf_rgb2luv(
   vec32s *rgbImgIn,
   vec16u *lImgOut,
   vec16u *uImgOut,
   vec16u *vImgOut,
   int16u strideIn,
   int16u strideOut,
   int16u chunkWidth,
   int16u chunkHeight,
   vec16u *luv_lut

   )
{

   vec16u minu = 5440;
   vec16u minv = 8109;
   vec32s un = 84273;
   vec32s vn = 398996;
   int08u mr[3] = { 110, 57, 5 };
   int08u mg[3] = { 87, 182, 33 };
   int08u mb[3] = { 45, 18, 241 };

   vec32u xZ, yZ;
   vec32u x, y, z;
   vec32s l;
   vec08u r, g, b;
   vec32u d, r2, g2, b2;

   if(ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
   {   
      for(int i = 0; i < 1064; i++)
      {
         luv_lut[i] = (vec16u)lTableData[i];
      }
   }


   for (int16u h = 0; h < chunkHeight; h++)
   {
      for (int16u w = 0; w < chunkWidth; w++)
      {
	 // convert RGB to LUV
         vec32u rgbImgIn32u = (vec32u)rgbImgIn[w]; 
         r = vec08u( rgbImgIn32u >> 24);
         g = vec08u( rgbImgIn32u >> 16);
         b = vec08u( rgbImgIn32u >>  8);

         x = vec32u(vmul_ulul((vec16u)mr[0], (vec16u)r) + vmul_ulul((vec16u)mg[0], (vec16u)g) + vmul_ulul((vec16u)mb[0],(vec16u)b));
         y = vec32u(vmul_ulul((vec16u)mr[1], (vec16u)r) + vmul_ulul((vec16u)mg[1], (vec16u)g) + vmul_ulul((vec16u)mb[1],(vec16u)b));
         z = vec32u(vmul_ulul((vec16u)mr[2], (vec16u)r) + vmul_ulul((vec16u)mg[2], (vec16u)g)) + vec32u(vmul_ulul((vec16u)mb[2], (vec16u)b));

         vec32u y_temp = y>>6;
         y_temp = vselect(y_temp, 1063, (y_temp <= 1063));
         //l = (vec32s)luv_lut[(vec16s)y_temp];
         l = (vec32s)vload(luv_lut, (vec16s)y_temp);
         lImgOut[w] = (vec16u)(l >> 2);

         d = 1 + x + 15 * y + 3*z;

         xZ = (x << 16) / d;
         yZ = (y << 16) / d;

         uImgOut[w] = (vec16u)(((vec32s)(l*(26 * xZ - un))) >> 17) + minu;
         vec32s tmp = ((vec32s)(117 * yZ - vn)) >> 1;
         vImgOut[w] = (vec16u)((l*tmp) >> 17) + minv;
      }
      rgbImgIn += strideIn;
      lImgOut += strideOut;
      uImgOut += strideOut;
      vImgOut += strideOut;
   }
   
}


// apply horizontal triangle filtering
void triangleFilter_c_hori(vec16u* in, int32_t instride, vec32s* out, int32_t outstride, int32_t h, int32_t w, int32_t radius, vec32u firstChunk, vec32u lastChunk)
{

   int32_t r = radius-1;
   uint32_t nrm_ = r * r * r * r;
   int16_t lZ = count_leading_zero_s(nrm_); vec32s lZ2;
   uint32_t nrm = fxrecip22(nrm_ << lZ) >> (32 - lZ - 1); // nrm is U32
   lZ = count_leading_zero_s(nrm);

   int32_t i, j, k;

   //int32_t numCUs = apuGetNumberOfCUs(); // number of CU
   int32_t numCUs = ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS); // number of CU

   vec16u* in_t = in - instride*radius;
   out = out;

   for (i = 0; i < h+2*radius; i++)
   { 
      vec32s T = 0; 
      vec32s U = 0;
      vec32s left, mid, right;
            
      vec16u *I = in_t + i*instride;
      vec32s *O = out  + i*outstride;

      vif( firstChunk > 0)
      {
         for(j = 0; j < r; j++)
         {
            // convert to 6.26 precision
            vec32s temp = ((vec32s)(I[j])) << 12;
            T += temp; // convert to 6.26 precision
            U += T;
                    
         }   
         U = 2*U-T;
         T = 0;
         O[0] = U;
       
         for (j = 1; j <= r; j++)
         {
            left  = (((vec32s)(*(I + r - j))) << 12);
            right = (((vec32s)(*(I + j - 1 + r))) << 12);
            mid   = (((vec32s)(*(I + j - 1)))<<1) << 12;

            vec32s temp = left + right - mid;
            T += temp;
            U += T;
            O[j] = U;
         }
         for(int j = r+1; j < w; j++)
         {
            left  = (((vec32s)(*(I + j - r - 1))) << 12);
            right = (((vec32s)(*(I + j + r - 1))) << 12);
            mid   = (((vec32s)(*(I + j - 1)))<<1) << 12;

            vec32s temp = left + right - mid;
            T += temp;
            U += T;
            O[j] = U;
         }
      }
      velse 
      {
        vif (lastChunk > 0)
        {
          for(j = 0; j <= w-r; j++)
          {
              left  = (((vec32s)(*(I + j - r - 1))) << 12);
              right = (((vec32s)(*(I + j + r - 1))) << 12);
              mid   = (((vec32s)(*(I + j - 1)))<<1) << 12;

              vec32s temp = left + right - mid;
              T += temp;
              U += T;
              O[j] = U;
          }
          for(j = w-r+1; j < w; j++)
          {
              left  = (((vec32s)(*(I + j - r - 1))) << 12);
              right = (((vec32s)(*(I + 2*w - j - r))) << 12);
              mid   = (((vec32s)(*(I + j - 1)))<<1) << 12;

              vec32s temp = left + right - mid;
              T += temp;
              U += T;
              O[j] = U;
          } 
        }
        velse
        {
          for (j = 0; j < w; j++)
          {
              left  = (((vec32s)(*(I + j - r - 1))) << 12);
              right = (((vec32s)(*(I + j + r - 1))) << 12);
              mid   = (((vec32s)(*(I + j - 1)))<<1) << 12;

              vec32s temp = left + right - mid;
              T += temp;
              U += T;
              O[j] = U;
          }
        }vendif
      }vendif

      vec32s TT = T;
      vec32s utot = (vec32s)0;
            
      U = vmsr(U);
      T = vmsr(w*T);

      for (j=1; j<numCUs; ++j) chess_loop_range(2,) chess_prepare_for_pipelining
      {
         utot += U + (j*T);
         T = vmsr(T);
         U = vmsr(U);
      }

      for(j = 0; j < w; j++)
      {
         O[j] += utot;
      }

      for (j=1; j<numCUs; ++j)
      {
         TT = vmsr(TT);
                
         for(k = w-1; k >= 0; k--)
         {
            O[k] -= (TT*(w-1-k));
         }
      } 

      for(j = 0; j < w; j++)
      {
         lZ2 = count_leading_digit(O[j]);

         O[j] = vselect((((nrm<<lZ)>>16) * ((O[j]<<lZ2)>>16) ) >> (lZ + lZ2), 0, lZ + lZ2 < 32);
      }
   }
} 


void triangleFiltr_c_verti(vec32s* inchunk, int32_t instride, vec16u* out, int32_t outstride, int32_t h, int32_t w, int32_t radius,
                      vec32s *T_lastline_buf,  vec32s *U_lastline_buf)
{

   int32_t firstTile = ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG) ;

   int32_t r = radius-1;
   int32_t i, j;

   vec32s* in_s = inchunk+radius*instride;
        
   for (i = 0; i < w; i++)
   { 
      vec32s*in = in_s+i;
      vec32s T_lastline;
      vec32s U_lastline;

      vec32s T = 0; 
      vec32s U = 0;

      if(! firstTile)
      {
         T_lastline = T_lastline_buf[i];
         U_lastline = U_lastline_buf[i];

         T_lastline += *(in - (r+1)*instride) + *(in + (r-1)*instride) - ((*(in-instride))<<1) ;
         U_lastline += T_lastline;
      }
      else
      {
         T_lastline = in[0]; 
         U_lastline = in[0];

         for(int j = 1; j < r; j++)
         {
            T_lastline += *(in + j*instride);
            U_lastline += T_lastline;
         }
         U_lastline = 2*U_lastline-T_lastline;
         T_lastline = 0;
                
      }
      out[i] = (vec16u)(U_lastline >> 12);

      T += T_lastline;
      U += U_lastline;

      if(! firstTile)
      {
         for (j = 1; j < h; j++)
         {
            T += *(in + (j - r - 1)*instride) + *(in + (r + j - 1)*instride) - (*(in + (j-1)*instride)<<1) ;
            U += T;
            out[j*outstride+i] = (vec16u)(U >> 12); 

         }
      }
      else
      {
         for (j = 1; j <= r; j++)
         {
            T += *(in + (r - j)*instride) + *(in + (r + j - 1)*instride) - ((*(in + (j-1)*instride))<<1) ;
            U += T;
            out[j*outstride+i] = (vec16u)(U >> 12); 

         }
         for (j = r+1; j < h; j++)
         {
            T += *(in + (j - r - 1)*instride) + *(in + (r + j - 1)*instride) - ((*(in + (j-1)*instride))<<1) ;
            U += T;
            out[j*outstride+i] = (vec16u)(U >> 12);

         }
      }    

      T_lastline_buf[i] = T;
      U_lastline_buf[i] = U;
      
   }
}

void triangleFilter(vec16u* in, int32_t instride, vec32s* temp_out, int32_t tempOutstride, vec16u* out, int32_t outstride, int32_t h, int32_t w, int32_t radius, 
                    vec32s *T_lastline_buf,  vec32s *U_lastline_buf, vec32u firstchunk, vec32u lastchunk)
{
   triangleFilter_c_hori(in, instride, temp_out, tempOutstride, h, w, radius, firstchunk, lastchunk);
 
   triangleFiltr_c_verti(temp_out, tempOutstride, out, outstride, h, w, radius, T_lastline_buf, U_lastline_buf);  
}

vec32s count_leading_zero(vec32u x)  // intrinsic 
{
#if (__chess__)
    vec16u in_h = x.high();
    vec16u in_l = x.low();
#elif (__llvm__)
    vec16u in_h = (vec16u)__builtin_apex_vec32_get_hi((vec32s)x);
    vec16u in_l = (vec16u)__builtin_apex_vec32_get_lo((vec32s)x);
#endif
    
    vec16u z = vclz(in_h);
    vif( 16 == z) 
    {
        z += vclz(in_l);
    }vendif
    return (vec32s)z; 
}

vec32s count_leading_digit(vec32s x)
{
    vec32s n = 0;

    vec32u xabs = vabs(x);
    n = count_leading_zero(xabs);

    return vselect(n, n - 1, n == 0);
}

uint32_t count_leading_zero_s(uint32_t x)
{
    uint32_t n;

    if (x == 0) return(32);
    n = 1;
    if ((x >> 16) == 0) { n = n + 16; x = x << 16; }
    if ((x >> 24) == 0) { n = n + 8; x = x << 8; }
    if ((x >> 28) == 0) { n = n + 4; x = x << 4; }
    if ((x >> 30) == 0) { n = n + 2; x = x << 2; }
    n = n - (uint32_t)(x >> 31);
    return n;
}

static inline void cordicCV(CORDIC_STATE* q, int i, fx_ucirc tacc){
   /* We need all the old values to compute all the new values. */
   CORDIC_STATE qtmp = *q;

   /* Decision */
   int ccw = q->y < 0;

   /* Update state */
   qtmp.x += ccw ? -(q->y >> i) : +(q->y >> i);
   qtmp.y -= ccw ? -(q->x >> i) : +(q->x >> i);
   qtmp.t += ccw ? -(fx_s0229)tacc : +tacc;
   *q = qtmp;
}

/* Function definitions */

static int clz1(uint32_t x)
{
   uint8_t n = 0;
   for (int i = 0; i < 32; i++)
   {
      if (x & 0x80000000) break;
      n++;
      x <<= 1;
   }
   return n;
}

/**
 * Counts the number of leading zeros in its 32-bit input. An input of
 * zero corresponds to 32 leading zeros.
 * @param [in] x: unsigned 32 bit integer
 * @return        number of leading zeros as a unsigned 32 bit integer.
 *
 * This functions does NOT follow the usual naming conventions because they are
 * so commonly used.
 */
static inline fx_u32 clz32(fx_u32 x){
   return clz1(x);
}

static const fx_scirc cordicTBLAngleC[] = {
   0x20000000, /* atan(2^- 0)  =  pi/   4  =  circ/8 */
   0x12e4051d, /* atan(2^- 1) */
   0x09fb385b, /* atan(2^- 2) */
   0x051111d4, /* atan(2^- 3) */
   0x028b0d43, /* atan(2^- 4) */
   0x0145d7e1, /* atan(2^- 5) */
   0x00a2f61e, /*     ...     */
   0x00517c55,
   0x0028be53,
   0x00145f2e,
   0x000a2f98,
   0x000517cc,
   0x00028be6,
   0x000145f3,
   0x0000a2f9,
   0x0000517c,
   0x000028be,
   0x0000145f,
   0x00000a2f,
   0x00000517,
   0x0000028b,
   0x00000145,
   0x000000a2,
   0x00000051,
   0x00000028,
   0x00000014,
   0x0000000a,
   0x00000005,
   0x00000002,
   0x00000001,
   0x00000000,
   0x00000000
};


static const fx_u32 cordicTBLCumulGainC[] = {
   0xFFFFFFFFU,
   1.0 / 1.4142135623730951 * (1ULL << 32),
   1.0 / 1.5811388300841898 * (1ULL << 32),
   1.0 / 1.6298006013006623 * (1ULL << 32),
   1.0 / 1.6424840657522375 * (1ULL << 32),
   1.0 / 1.6456889157572552 * (1ULL << 32),
   1.0 / 1.6464922787124789 * (1ULL << 32),
   1.0 / 1.6466932542736439 * (1ULL << 32),
   1.0 / 1.6467435065969012 * (1ULL << 32),
   1.0 / 1.6467560702048787 * (1ULL << 32),
   1.0 / 1.6467592111398224 * (1ULL << 32),
   1.0 / 1.6467599963756177 * (1ULL << 32),
   1.0 / 1.6467601926846951 * (1ULL << 32),
   1.0 / 1.6467602417619724 * (1ULL << 32),
   1.0 / 1.6467602540312924 * (1ULL << 32),
   1.0 / 1.6467602570986224 * (1ULL << 32),
   1.0 / 1.646760257865455  * (1ULL << 32),
   1.0 / 1.646760258057163  * (1ULL << 32),
   1.0 / 1.64676025810509   * (1ULL << 32),
   1.0 / 1.6467602581170717 * (1ULL << 32),
   1.0 / 1.6467602581200673 * (1ULL << 32),
   1.0 / 1.6467602581208161 * (1ULL << 32),
   1.0 / 1.6467602581210032 * (1ULL << 32),
   1.0 / 1.6467602581210501 * (1ULL << 32),
   1.0 / 1.6467602581210619 * (1ULL << 32),
   1.0 / 1.6467602581210647 * (1ULL << 32),
   1.0 / 1.6467602581210654 * (1ULL << 32),
   1.0 / 1.6467602581210654 * (1ULL << 32),
   1.0 / 1.6467602581210654 * (1ULL << 32),
   1.0 / 1.6467602581210654 * (1ULL << 32),
   1.0 / 1.6467602581210654 * (1ULL << 32),
   1.0 / 1.6467602581210654 * (1ULL << 32),
   1.0 / 1.6467602581210654 * (1ULL << 32),
   1.0 / 1.6467602581210654 * (1ULL << 32)
};


void fx_magatan2_ij(fx_s0229 y, fx_s0229 x, fx_u0329* m, int i, fx_scirc* a, int j)
{
   CORDIC_STATE   q = { x, y, 0 }, qi = q, qj = q;
   fx_ucirc       tadj;
   int            k;

   /* Rotate vector into right halfplane (x>0) */
   tadj = q.x & CORDIC_CIRC_HALF;
   q.x = tadj ? -q.x : q.x;
   q.y = tadj ? -q.y : q.y;

   /* Normalize */
   int n = clz32(q.y < 0 ? q.x | -q.y : q.x | q.y) - 3;
   q.x <<= n;
   q.y <<= n;


   /* CORDIC Core */
   for (k = 0; k < i || k < j; k++){
      cordicCV(&q, k, cordicTBLAngleC[k]);

      if (k + 1 == i){ qi = q; }
      if (k + 1 == j){ qj = q; }
   }

   /* Rotate angle into original halfplane */
   qj.t += tadj;

   /* Degain & Output */
   *m = (fx_s64)qi.x * cordicTBLCumulGainC[i] >> (32 + n);
   *a = x | y ? qj.t : 0;
}

/**
 * Number of fraction digits in QS2.29 value
 */
#define CORDIC_FXPT_FRACBITS     (29)
/**
 * Fixed point value one as QS2.29 format
 */
#define CORDIC_FXPT_ONE          ((fx_s0229)(1U<<CORDIC_FXPT_FRACBITS))


fx_scirc   fx_atan2(fx_s0229 y, fx_s0229 x){
   fx_u0329 m;
   fx_scirc a;
   fx_magatan2_ij(y, x, &m, 16, &a, 30);
   return a;
}

static inline void cordicHV(CORDIC_STATE* q, int i, fx_u0329 tacc){
   /* We need all the old values to compute all the new values. */
   CORDIC_STATE qtmp = *q;

   /* Decision */
   int ccw = q->y < 0;

   /* Update state */
   qtmp.x -= ccw ? -(q->y >> i) : +(q->y >> i);
   qtmp.y -= ccw ? -(q->x >> i) : +(q->x >> i);
   qtmp.t += ccw ? -(fx_s0229)tacc : +tacc;
   *q = qtmp;
}

static const fx_s0229 cordicTBLAngleH[] = {
   0x00000000,
   0x1193ea7a,
   0x082c577d,
   0x04056247,
   0x0200ab11,
   0x01001558,
   0x008002aa,
   0x00400055,
   0x0020000a,
   0x00100001,
   0x00080000,
   0x00040000,
   0x00020000,
   0x00010000,
   0x00008000,
   0x00004000,
   0x00002000,
   0x00001000,
   0x00000800,
   0x00000400,
   0x00000200,
   0x00000100,
   0x00000080,
   0x00000040,
   0x00000020,
   0x00000010,
   0x00000008,
   0x00000004,
   0x00000002,
   0x00000001,
   0x00000000,
   0x00000000,
   0x00000000
};

static const fx_u32 cordicTBLCumulGainH[] = {
   0,                       /* Not used */
   1.0 / 0.8660254037844386  * (1ULL << 31),
   1.0 / 0.83852549156242107 * (1ULL << 31),
   1.0 / 0.83194871949838356 * (1ULL << 31),
   1.0 / 0.82869891981284294 * (1ULL << 31),
   1.0 / 0.82829418283152745 * (1ULL << 31),
   1.0 / 0.82819306652976132 * (1ULL << 31),
   1.0 / 0.82816779169748167 * (1ULL << 31),
   1.0 / 0.82816147325455769 * (1ULL << 31),
   1.0 / 0.82815989366039755 * (1ULL << 31),
   1.0 / 0.82815949876289319 * (1ULL << 31),
   1.0 / 0.82815940003858179 * (1ULL << 31),
   1.0 / 0.82815937535750805 * (1ULL << 31),
   1.0 / 0.82815936301697168 * (1ULL << 31),
   1.0 / 0.82815936147440472 * (1ULL << 31),
   1.0 / 0.82815936108876298 * (1ULL << 31),
   1.0 / 0.82815936099235254 * (1ULL << 31),
   1.0 / 0.82815936096824994 * (1ULL << 31),
   1.0 / 0.8281593609622242  * (1ULL << 31),
   1.0 / 0.82815936096071785 * (1ULL << 31),
   1.0 / 0.82815936096034126 * (1ULL << 31),
   1.0 / 0.82815936096024712 * (1ULL << 31),
   1.0 / 0.82815936096022358 * (1ULL << 31),
   1.0 / 0.82815936096021769 * (1ULL << 31),
   1.0 / 0.82815936096021614 * (1ULL << 31),
   1.0 / 0.82815936096021581 * (1ULL << 31),
   1.0 / 0.8281593609602157  * (1ULL << 31),
   1.0 / 0.8281593609602157  * (1ULL << 31),
   1.0 / 0.8281593609602157  * (1ULL << 31),
   1.0 / 0.8281593609602157  * (1ULL << 31),
   1.0 / 0.8281593609602157  * (1ULL << 31),
   1.0 / 0.8281593609602157  * (1ULL << 31),
   1.0 / 0.8281593609602157  * (1ULL << 31)
};

void     fx_sqrtln_ij(fx_u0329 n, fx_u0329* s, int i, fx_s0229* l, int j){
   CORDIC_STATE q = { n + CORDIC_FXPT_ONE, n - CORDIC_FXPT_ONE, 0 }, qi = q, qj = q;
   int          k;


   /* CORDIC Core */
   for (k = 1; k <= i || k <= j; k++){
      cordicHV(&q, k, cordicTBLAngleH[k]);
      if (k == 4 || k == 13){
         cordicHV(&q, k, cordicTBLAngleH[k]);
      }

      if (k == i){ qi = q; }
      if (k == j){ qj = q; }
   }

   /* Degain & Output */
   *s = (fx_s64)qi.x * cordicTBLCumulGainH[i] >> 32;
   *l = qj.t << 1;
}


fx_u0329   fx_sqrt(fx_u0329 n){
   fx_u0329 s;
   fx_s0229 l;
   fx_sqrtln_ij(n, &s, 15, &l, 29);
   return s;
}

void fxgrad1_c_row(vec16u *I, vec32s *Gx, vec32s *Gy, int32_t stride, int32_t w) 
{
    int x, x1;
    vec16u *Ip, *In;

    Ip = I - stride;
    In = I + stride;

    for (x = 0; x < w; x++)
    {
        vec32s temp_sum = ((vec32s)In[0] << 16) - ((vec32s)Ip[0] << 16);
        vec32s temp = temp_sum >> 1;     
        *Gy++ = temp; // Gx is Q1.30
        In ++;
        Ip ++;
    }

    Ip = I-1;
    In = I+1;
  
    vec32s temp = (((vec32s)In[0] << 16) - ((vec32s)Ip[0] << 16))>>1;
    *Gx++ = temp; 
    In ++;
    Ip ++; 
    for (x = 1; x < w - 1; x++)
    {
        temp = (((vec32s)In[0] << 16) - ((vec32s)Ip[0] << 16)) >> 1;
        *Gx++ = temp; 
        In ++; 
        Ip ++;
    } 
    temp = (((vec32s)In[0] << 16) - ((vec32s)Ip[0] << 16)) >> 1;
    *Gx++ = temp; 
}

void fxgradMag_c_row(vec16u* L, vec16u* U, vec16u* V, int32_t instride, int32_t outstride,
                     vec16u* M, vec16u* O,  
                     int32_t h, int32_t w, int32_t full,
                     vec32s *Gx, vec32s *Gy, vec32u *M2 ) 
{
    int32_t x, y, x1, c;
    vec32s gZeros, lZero;

    uint32_t PI29 = 1686629713ll; // 2*3.141592654 * pow(2,28) 2*PI in 4.28 fixed point format
    uint32_t HALF = 2147483648ll; // 0.5 in 0.32 fixed point format
    uint32_t TWO  = 1073741824ll; // 2 in 2.29 fixed point format
    
    //int numCU = 64;
    int numCU = ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS);

    for (y = 0; y < h; y++)
    {
        // calculate row dx, dy of L, U, V images    
        fxgrad1_c_row(L+y*instride, Gx,     Gy,     instride, w); //L Gx and Gy are Q1.30 here
        fxgrad1_c_row(U+y*instride, Gx+w,   Gy+w,   instride, w); //U Gx and Gy are Q1.30 here
        fxgrad1_c_row(V+y*instride, Gx+2*w, Gy+2*w, instride, w); //V Gx and Gy are Q1.30 here
      
        // L, U, V    
        for(c = 0; c < 3; c++)
        { 
            for (x = 0; x < w; x++)
            {
                x1 = w * c + x;
                vec32s gx_ = Gx[x1];
                vec32s gy_ = Gy[x1]; 

                vec32s lx = count_leading_digit(gx_);
                vec32s ly = count_leading_digit(gy_);

                //gZeros = (lx < ly) ? lx : ly;
                gZeros = vselect(lx, ly, lx < ly); 

                vif (gZeros > 16)
                    gZeros = 16;vendif

                gx_ = gx_ << gZeros;
                gy_ = gy_ << gZeros;

                M2[x1] = (vec32u)((gx_ >> 16) * (gx_ >> 16) + (gy_ >> 16) * (gy_ >> 16)); 
                M2[x1] = M2[x1] >> (2 * gZeros - 2); 
               
                gx_ = gx_ >> gZeros; // Undo Normalization
                gy_ = gy_ >> gZeros;

                if (c == 0)
                    continue;
                vif (M2[x1] > M2[x])
                {
                    M2[x] = M2[x1];
                    Gx[x] = gx_;
                    Gy[x] = gy_;
                }vendif
            }
        } 

        for (x = 0; x < w; x++)
        {
            vec32u M2_ = M2[x];
        
            vif (M2_ > 0)
            {
                lZero = count_leading_zero(M2_);

                vif (lZero <= 2)
                {
                    lZero = 0;
                }vendif
                vif ((lZero & 0x01) > 0)
                {
                    lZero -= 1;
                }vendif
                M2_ <<= lZero;
                
                vec16s v_shift = 0;
                vif(M2_ >= TWO)
                {
                  M2_ >>= 2;
                  v_shift = 1;
                }vendif
                for (int i = 0; i < numCU; ++i)
                {
                    uint32_t m_ = vget(M2_, i);   
                    uint32_t sqrt_ = fx_sqrt(m_); 
                    M2_ = vput(M2_, (vec32u)sqrt_, i); 
                }
                M2_<<= (vec32u)v_shift; 

                M2_ = (M2_ >> (lZero >> 1)) >> 15;
            }vendif

            vif (M2_ < 1)
                M2_ = 1;vendif

            M[y*outstride + x] = (vec16u)M2_;
        }   
         
        if (O)
        {
            for (x = 0; x < w; x++)
            {
                // Gx and Gy are Q1.30 here, convert to Q2.29
                Gy[x] = Gy[x] >> 1;
                Gx[x] = Gx[x] >> 1;

                vec32u temp;
                // calculate atan, result are are factional of pi*2
                // for example, Gy == Gx, result is 1/8 (0.125) which is 2*pi/8 = pi/4(45)
                // result is in precision of Q(0,32)
                for (int i = 0; i < numCU; ++i)
                {
                    int32s y_ = vget(Gy[x], i);
                    int32s x_ = vget(Gx[x], i);
                    int32u atan_ = fx_atan2(y_, x_);
                    temp = vput(temp, (vec32u)atan_, i);
                }

                vif (0 == full && temp > HALF )
                {
                    temp -= (vec32u)(HALF);
                }vendif

                // calculate orientation in precision of Q(2, 30)
                vec32u ret = (temp >> 16)*(PI29 >> 14);

                // final result in precison of 3.13
                O[y*outstride + x] = (vec16u)(ret >> 17);// O is U3.13
            }
        }
    }
}


vec32s calcAtanReg(vec32u Gy, vec32u Gx) 
{
    
    vec32s start = 0, end = 64, mid;
    //while (vany((start + 1) < end))
#if (__chess__)
   while((start + 1) < end)
#elif (__llvm__)   
   while(vany((start + 1) < end)) 
#endif
    {
        mid = start + ((end - start)>>1);
        vif ((Gy<<6) == (Gx*mid))
        {
            start = mid;    
        }
        velse
        {
          vif ( (Gy<<6) > (Gx*mid)  )
          {
              start = mid;
          }
          velse 
          {
              end = mid;
          }vendif
        }vendif
    }
   

    vif ((Gy << 6) <= (Gx*start))
    {
        return start;
    }vendif
    return end;
}


uint32_t atanTable[65] = { 0,
16775850,  33543515,  50294832,  67021686,  83716035,  100369929, 116975535, 133525158, 150011261, 166426484, 182763663, 199015845,
215176308, 231238569, 247196399, 263043836, 278775191, 294385059, 309868320, 325220151, 340436022, 355511705, 370443267,
385227073, 399859787, 414338361, 428660037, 442822339, 456823069, 470660297, 484332355, 497837829, 511175551, 524344587, 537344231,
550173993, 562833590, 575322935, 587642129, 599791447, 611771333, 623582386, 635225352, 646701113, 658010681, 669155185, 680135862, 
690954054, 701611191, 712108791, 722448446, 732631822, 742660642, 752536690, 762261796, 771837834, 781266719, 790550394, 799690833, 
808690030, 817549998, 826272766, 834860371, 843314856 };


vec32u simatan(vec32s Gx, vec32s Gy, vec32u *lut)
{
    uint32_t PI_1_2 = 1686629684ll;
    uint32_t PI_1_4 = 843314842ll;
    uint32_t PI = 3373259368ll; 
    vec32u ret;
    vec32u Gx_abs = vabs(Gx);
    vec32u Gy_abs = vabs(Gy);
    Gx_abs = Gx_abs >> 6;
    Gy_abs = Gy_abs >> 6;
    vif (Gy_abs == 0)
    {
        vif(Gx < 0)
        {
            ret = PI;
        }
        velse
        {
            ret = 0;
        }vendif
    }
    velse 
    {
      vif ( Gx_abs == 0 || (Gy_abs > (Gx_abs << 6)))
      {
          ret = PI_1_2;
      }
      velse
      {
          vif (Gx_abs == Gy_abs)
          {
              ret = PI_1_4;
          }
          velse
          {
              vec32s ret_idx =  calcAtanReg(vselect(Gx_abs, Gy_abs, (Gx_abs < Gy_abs)), 
                                  vselect(Gx_abs, Gy_abs, (Gx_abs > Gy_abs)));
              //ret = (vec32u)(lut[(vec16s)ret_idx]);
              ret = (vec32u)vload(lut,(vec16s)ret_idx);
              vif (Gx_abs < Gy_abs)
              {
                  ret = PI_1_2 - ret;
              }vendif
          }vendif
          vif (Gx < 0)
          {
              ret = PI - ret;
          }vendif
          vif (Gy < 0)
          {
              ret = PI - ret;
          }vendif
      }vendif
    }vendif
  
    return ret;
}


void fxgradMag_c_row_opt(vec16u* L, vec16u* U, vec16u* V, int32_t instride, int32_t outstride,
                     vec16u* M, vec16u* O,
                     int32_t h, int32_t w,
                     vec32s *Gx, vec32s *Gy, vec32u *M2, vec32u *lut)
{
    int32_t x, y, x1, c;
    vec32s gZeros, lZero;

    int32_t stride = instride;


    if(ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
    {
        for(int i = 0; i < 65; i++)
        {
           lut[i] = (vec32u)atanTable[i];
        }
    }


    uint32_t PI29 = 1686629713ll; // 2*3.141592654 * pow(2,28) 2*PI in 4.28 fixed point format
    uint32_t HALF = 2147483648ll; // 0.5 in 0.32 fixed point format
    uint32_t TWO  = 1073741824ll; // 2 in 2.29 fixed point format
    
    int numCU = ACF_RET_VAR(ACF_VAR_TILE_WIDTH_IN_CHUNKS);

    // compute gradient magnitude and orientation for each column
    for (y = 0; y < h; y++)
    {
        // compute gradients (Gx, Gy) with maximum squared magnitude (M2)
        fxgrad1_c_row(L+y*instride, Gx,     Gy,     instride, w); //L Gx and Gy are Q1.30 here
        fxgrad1_c_row(U+y*instride, Gx+w,   Gy+w,   instride, w); //U Gx and Gy are Q1.30 here
        fxgrad1_c_row(V+y*instride, Gx+2*w, Gy+2*w, instride, w); //V Gx and Gy are Q1.30 here

        for(c = 0; c < 3; c++)
        { 
            for (x = 0; x < w; x++)
            {
                x1 = w * c + x;
                vec32s gx_ = Gx[x1];
                vec32s gy_ = Gy[x1];
        
                M2[x1] = (vabs(gx_)+vabs(gy_))>>16; // M2 is (U2.30 --> 2.14) here

                if (c == 0)
                {
                    M[y*outstride + x] = (vec16u) M2[x]; 
                    continue;
                }
                vif (M2[x1] > M2[x])
                {
                    M2[x] = M2[x1];
                    Gx[x] = gx_;
                    Gy[x] = gy_;
                    M[y*outstride + x] = (vec16u) M2[x]; 
                }vendif
            }
        }

        if (O)
        {
            for (x = 0; x < w; x++)
            {
                vec32u ret = simatan(Gx[x], Gy[x], lut);

                // final result in precison of 3.13
                O[y*outstride + x] = (vec16u)(ret >> 17);// O is U3.13
            }
        }
    }
}   

// simplified bi-linear resize on fixed downsample ratio of 1, 2, 4, 8
void apu_bi_pyramid_hori(vec16u* in, vec16u* out, int32_t instride, int32_t outstride, 
                         int32_t chunkWidth, int32_t chunkHeight, int scaleRatio, int shift)
{
    for(int i = 0; i < chunkHeight; i++)
    {
        int index = 0;   
        for(int j = 0; j < chunkWidth; j+=scaleRatio )
        {
            vec32u sum = 0;               
            for(int k = j; k < j+scaleRatio; k++)
            {
               sum += (vec32u)in[i*instride + k];
            } 
            out[i*outstride + index] = (vec16u)(sum >> shift);
            index++;
        } 
    }  
}


void apu_bi_pyramid_verti(vec16u* in, vec16u* out, int32_t instride, int32_t outstride, 
                         int32_t chunkWidth, int32_t chunkHeight, int scaleRatio, int shift)
{
    for(int i = 0; i < chunkWidth; i++)
    {
        int index = 0;
        for(int j = 0; j < chunkHeight; j+=scaleRatio )
        {
            vec32u sum = 0;
            for(int k = j; k < j+scaleRatio; k++)
            {
               sum += (vec32u)in[k*instride + i];
            }
            out[index*outstride + i] = (vec16u)(sum >> shift);
            index++;
        }
    }

}


void apu_bi_pyramid_pass(vec16u* in, vec16u* out, int32_t instride, int32_t outstride, 
                         int32_t chunkWidth, int32_t chunkHeight)
{
    for(int i = 0; i < chunkHeight; i++)
    {
        for(int j = 0; j < chunkWidth; j++ )
        {
            out[i*outstride + j] = in[i*instride + j];
        }
    }

}





#endif /* ACF_KERNEL_IMPLEMENTATION */
