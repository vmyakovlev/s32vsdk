/*****************************************************************************
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
****************************************************************************/
/*!*********************************************************************************
*  \file canny_apu.cpp
*  \brief Canny Edge Detection Kernels
***********************************************************************************/
#ifdef ACF_KERNEL_IMPLEMENTATION
#include "canny_apu.h"

static vec16u stack[MAX_STACK_SIZE]; /* Stores the <y, x> location of a definite edge */
static vec16s stack_top = 0;         /* Top of the stack of definite edges */

/* Sets the input (y, x) pixel as a definite edge and adds it onto the top of the
 * stack of definite edges
*/
static inline void connect_edges(vec08u* dst, int dstr, int bw, int bh) {
   vec16s stack_top_local = stack_top;
#if (__chess__)
   while(stack_top_local > 0) 
#elif (__llvm__)   
   while(vany(stack_top_local>0)) 
#endif
   {
#ifdef __llvm__
      vif(stack_top_local>0) {
#endif 
      stack_top_local -= 1;
      /* Retrieve the definite edge location from the stack */
      vec08u p11, p12, p13, p21, p23, p31, p32, p33; /* Neighbour pixels */
      vec16u ind = vload(stack, stack_top_local);
      vec16s c = (vec16s)(vmul_ulul(ind, 1));
      vec16s r = (vec16s)(ind >> ROW_BIT_OFFSET);
      vec16s idx = (r-1)*dstr+c-1;
      vec16u tmp = ind - (1 << ROW_BIT_OFFSET) - 1;
      vbool nFirst = c > 0;
      vbool nLast = c < bw-1;

      /* Set any potential edge pixel to a definite edge pixel and 
       * store it on the stack of definite edge pixels
       */
      p11 = vload(dst, idx);
      p12 = vload(dst, idx+1);
      p13 = vload(dst, idx+2);
      vif (r > 0) {
         vif (p11 == 1 && nFirst) {
            vstore(dst, idx, 2);
            vstore(stack, stack_top_local, tmp);
            stack_top_local++;
         } vendif
         tmp++;
         vif (p12 == 1) {
            vstore(dst, idx+1, 2);
            vstore(stack, stack_top_local, tmp);
            stack_top_local++;
         } vendif
         tmp++;
         vif (p13 == 1 && nLast) {
            vstore(dst, idx+2, 2);
            vstore(stack, stack_top_local, tmp);
            stack_top_local++;
         }vendif
         tmp-=2;
      }vendif
      tmp += (1 << ROW_BIT_OFFSET);
      idx += dstr;
      p21 = vload(dst, idx);
      p23 = vload(dst, idx+2);
      vif (p21 == 1 && nFirst) {
         vstore(dst, idx, 2);
         vstore(stack, stack_top_local, tmp);
         stack_top_local++;
      }vendif
      tmp += 2;
      vif (p23 == 1 && nLast) {
         vstore(dst, idx+2, 2);
         vstore(stack, stack_top_local, tmp);
         stack_top_local++;
      }vendif
      tmp += (1 << ROW_BIT_OFFSET) - 2;
      idx += dstr;
      p31 = vload(dst, idx);
      p32 = vload(dst, idx+1);
      p33 = vload(dst, idx+2);
      vif (r < bh-1) {
         vif (p31 == 1 && nFirst) {
            vstore(dst, idx, 2);
            vstore(stack, stack_top_local, tmp);
            stack_top_local++;
         }vendif
         tmp++;
         vif (p32 == 1) {
            vstore(dst, idx+1, 2);
            vstore(stack, stack_top_local, tmp);
            stack_top_local++;
         }vendif
         tmp++;
         vif (p33 == 1 && nLast) {
            vstore(dst, idx+2, 2);
            vstore(stack, stack_top_local, tmp);
            stack_top_local++;
         }vendif
      }vendif
#ifdef __llvm__
    } vendif
#endif
   }
   stack_top = 0;
}

static void get_gradients_inner(vec16s* dstx, vec16s* dsty, vec16u* dstm, const vec08u* src, int sstr, int bw) 
#ifdef __llvm__
__attribute__((noinline))
#endif
{
   const int chess_storage(R18) k2k1 = chess_copy(0x0201);
   const int chess_storage(R19) k0k2 = chess_copy(0x0002);
    
   /* Variables */
   vec16s chess_storage(V0) lxcol, chess_storage(V1) mxcol, rxcol;
   vec16s chess_storage(V2) lycol, chess_storage(V3) mycol, rycol;
   vec16s lpix,  mpix,  upix;


   /* Row Pointers */
   const vec08u* src0 = (const vec08u*)(src-sstr-1);
   const vec08u* src1 = (const vec08u*)(src     -1);
   const vec08u* src2 = (const vec08u*)(src+sstr-1);
   vec16s*       dst0 = dstx;
   vec16s*       dst1 = dsty;
   vec16u*       dst2 = dstm;

   upix = (vec16s)*src0++;
   lpix = (vec16s)*src2++;
   mpix = (vec16s)*src1++;
   lycol = lpix-upix;
   lxcol = upix+vmul_ulsh(mpix, k2k1)+lpix;
   upix = (vec16s)*src0++;
   lpix = (vec16s)*src2++;
   mpix = (vec16s)*src1++;
   mycol = lpix-upix;
   mxcol = upix+vmul_ulsh(mpix, k2k1)+lpix;
   upix = (vec16s)*src0++;
   lpix = (vec16s)*src2++;
   mpix = (vec16s)*src1++;
   rycol = lpix-upix;


   /* Core loop */
   for (int x=0; x<bw; ++x) chess_loop_range(2,) {
      
      /* Compute separable, column direction */
      rxcol = upix+vmul_ulsh(mpix, k2k1)+lpix;
      
      /* Compute separable, row direction */
      lxcol = rxcol-lxcol;
      lycol = lycol+vmul(mycol, k0k2)+rycol;
      
      /* Abs+Abs+Add+Sat+Store */
      *dst0++ = lxcol;
      *dst1++ = lycol;
      *dst2++ = (vec16u)vabs_diff(lxcol, (vec16s)0) + vabs_diff(lycol, (vec16s)0);
      
      /* Save State */
      lxcol = mxcol;mxcol = rxcol;
      lycol = mycol;mycol = rycol;
      
      /* Load next 3 pixels */
      upix = (vec16s)*src0++;
      lpix = (vec16s)*src2++;
      mpix = (vec16s)*src1++;
      rycol = lpix-upix;
   }
}

static void nms_inner(vec16u m12, vec16u m13, vec16u m22, vec16u m23, vec16u m32, vec16u m33,
                      vec16u* pm1, vec16u* pm2, vec16u* pm3,
                      const vec16s* px, const vec16s* py, vec08u* pd, 
                      int16u low, int16u high, int bw)
{
   vec16u m11, m21, m31;
   vec16u x, y, tg22x, tg67x;
   vec16s xs, ys;
   vec08u val;
   vbool d90, d45, d135, d0, sign, add;
   for (int j = 0; j < bw; ++j) chess_loop_range(1, ) {
      m11 = m12; m12 = m13; m13 = *pm1++;
      m21 = m22; m22 = m23; m23 = *pm2++;
      m31 = m32; m32 = m33; m33 = *pm3++;

      xs = *px++;
      ys = *py++;

      x = ((vec16u)vabs_diff(xs, (vec16s)0));
      y = ((vec16u)vabs_diff(ys, (vec16s)0)) << 5;
      sign = (xs ^ ys) < 0;
      tg22x = x*13 + (x>>2);
      tg67x = tg22x + (x<<6);
      //

      d0  = m22 > m21 && m22 >= m23;
      d90 = m22 > m12 && m22 >= m32;
      d135 = m22 > m13 && m22 >  m31;
      d45  = m22 > m11 && m22 >  m33;
      add = (m22 > low)
               &&(
                     (d0   && (y <  tg22x))
                 ||  (d90  && (y >  tg67x))
                 ||  (d135 && (y >= tg22x) && (y <= tg67x) &&  sign)
                 ||  (d45  && (y >= tg22x) && (y <= tg67x) && !sign)
               );

      val = 0;
      vif (add) {
         vif (m22 > high) {
            val = 2;
         } velse {
            val = 1;
         } vendif
      } vendif
      *pd++ = val;
   }
}

/*!
   Performs Canny Non-Maxima Suppression and Edge Hysteresis
   _dst_ will be filled with the edge value of the pixels
      0 - the pixel can not belong to an edge
      1 - the pixel might belong to an edge
      2 - the pixel does belong to an edge

   \param dst    - [Output] Destination edge map buffer
   \param dstr   - [Input]  Destination edge map stride
   \param src    - [Input]  Source image buffer
   \param sstr   - [Input]  Source image stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
   \param low    - [Input]  Low threshold used in edge hysteresis
   \param high   - [Input]  High threshold used in edge hysteresis
 */
void apu_canny_suppress(
               vec08u* dst, int dstr,
         const vec08u* src, int sstr,
               int bw, int bh,
               int16u low, int16u high
               )
{
   // Need 7*(bw+2) buffer space
   // Worst case is 64 byte bw
   static vec16s buff[66*8];
   vec16s *px, *gx[2];
   vec16s *py, *gy[2];
   vec16u *pm1, *pm2, *pm3, *mag[4];
   vec08u *pd;
   
   vec16u m11, m12, m13, m21, m22, m23, m31, m32, m33;
   vec16u x, y, tg22x, tg67x;
   vec16s xs, ys;
   vec08u val;
   vbool d90, d45, d135, d0, sign, add;
   int imod2, imod2p1;

   gx[0] = buff;
   gy[0] = gx[0]+bw+2;
   gx[1] = gy[0]+bw+2;
   gy[1] = gx[1]+bw+2;

   mag[0] = (vec16u*)(gy[1]+bw+2);
   mag[1] = mag[0]+bw+2;
   mag[2] = mag[1]+bw+2;
   mag[3] = mag[2]+bw+2;
   
   
   get_gradients_inner(gx[0], gy[0], mag[0], src-sstr-1, sstr, bw+2);
   get_gradients_inner(gx[0], gy[0], mag[1], src-1     , sstr, bw+2);
   src += sstr;

   for (int i = 0; i < bh; ++i) chess_loop_range(1, ) {
      imod2 = i&1;
      imod2p1 = imod2^1;
      px = gx[imod2]+1;
      py = gy[imod2]+1;
      pm1 = mag[i&3];
      pm2 = mag[(i+1)&3];
      pm3 = mag[(i+2)&3];
      
      // printf("%2d: Fill gx[%d] gy[%d] mag[%d]\n", i, imod2p1, imod2p1, (i+2)%3);
      get_gradients_inner(gx[imod2p1], gy[imod2p1], pm3, src-1, sstr, bw+2);
      
      m12 = *pm1++; m13 = *pm1++;
      m22 = *pm2++; m23 = *pm2++;
      m32 = *pm3++; m33 = *pm3++;
      pd = dst;

      // printf("%2d: Use gx[%d] gy[%d] mag[%d] mag[%d] mag[%d]\n", i, imod2, imod2, i%3, (i+1)%3, (i+2)%3);
      for (int j = 0; j < bw; ++j) chess_loop_range(1, ) {
         m11 = m12; m12 = m13; m13 = *pm1++;
         m21 = m22; m22 = m23; m23 = *pm2++;
         m31 = m32; m32 = m33; m33 = *pm3++;

         xs = *px++;
         ys = *py++;

         x = ((vec16u)vabs_diff(xs, (vec16s)0));
         y = ((vec16u)vabs_diff(ys, (vec16s)0)) << 5;
         sign = (xs ^ ys) < 0;
         tg22x = x*13 + (x>>2);
         tg67x = tg22x + (x<<6);
         //

         d0  = m22 > m21 && m22 >= m23;
         d90 = m22 > m12 && m22 >= m32;
         d135 = m22 > m13 && m22 >  m31;
         d45  = m22 > m11 && m22 >  m33;
         add = (m22 > low)
                  &&(
                        (d0   && (y <  tg22x))
                    ||  (d90  && (y >  tg67x))
                    ||  (d135 && (y >= tg22x) && (y <= tg67x) &&  sign)
                    ||  (d45  && (y >= tg22x) && (y <= tg67x) && !sign)
                  );

         val = 0;
         vif (add) {
            vif (m22 > high) {
               val = 2;
            } velse {
               val = 1;
            } vendif
         } vendif
         *pd++ = val;
      }
      
      src += sstr;
      dst += dstr;
   }
}

/*!
   Performs edge promotion.
   it promotes potential edge points into edge points
   if they are a neighbour with a definite edge point.

   \param dst    - [Output] Destination edge map buffer
   \param dstr   - [Input]  Destination edge map stride
   \param src    - [Input]  Source edge map buffer
   \param sstr   - [Input]  Source edge map stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
 */
void apu_canny_suppress_promote(
               vec08u* dst, int dstr,
         const vec08u* src, int sstr,
               int bw, int bh,
               int16u low, int16u high
               )
{
   // Need 7*(bw+2) buffer space
   // Worst case is 64 byte bw
   static vec16s buff[66*8];
   vec16s *px, *gx[2];
   vec16s *py, *gy[2];
   vec16u *pm1, *pm2, *pm3, *mag[4];
   vec08u *pd;
   stack_top = 0;
   
   vec16u m11, m12, m13, m21, m22, m23, m31, m32, m33;
   vec16u x, y, tg22x, tg67x;
   vec16s xs, ys;
   vec08u val;
   vbool d90, d45, d135, d0, sign, add;
   int imod2, imod2p1;

   gx[0] = buff;
   gy[0] = gx[0]+bw+2;
   gx[1] = gy[0]+bw+2;
   gy[1] = gx[1]+bw+2;

   mag[0] = (vec16u*)(gy[1]+bw+2);
   mag[1] = mag[0]+bw+2;
   mag[2] = mag[1]+bw+2;
   mag[3] = mag[2]+bw+2;
   
   
   get_gradients_inner(gx[0], gy[0], mag[0], src-sstr-1, sstr, bw+2);
   get_gradients_inner(gx[0], gy[0], mag[1], src-1     , sstr, bw+2);
   src += sstr;
   vec08u* save = dst;

   for (int i = 0; i < bh; ++i) chess_loop_range(1, ) {
      imod2 = i&1;
      imod2p1 = imod2^1;
      px = gx[imod2]+1;
      py = gy[imod2]+1;
      pm1 = mag[i&3];
      pm2 = mag[(i+1)&3];
      pm3 = mag[(i+2)&3];
      
      // printf("%2d: Fill gx[%d] gy[%d] mag[%d]\n", i, imod2p1, imod2p1, (i+2)%3);
      get_gradients_inner(gx[imod2p1], gy[imod2p1], pm3, src-1, sstr, bw+2);
      
      m12 = *pm1++; m13 = *pm1++;
      m22 = *pm2++; m23 = *pm2++;
      m32 = *pm3++; m33 = *pm3++;
      pd = dst;

      // printf("%2d: Use gx[%d] gy[%d] mag[%d] mag[%d] mag[%d]\n", i, imod2, imod2, i%3, (i+1)%3, (i+2)%3);
      for (int j = 0; j < bw; ++j) chess_loop_range(1, ) {
         m11 = m12; m12 = m13; m13 = *pm1++;
         m21 = m22; m22 = m23; m23 = *pm2++;
         m31 = m32; m32 = m33; m33 = *pm3++;

         xs = *px++;
         ys = *py++;

         x = ((vec16u)vabs_diff(xs, (vec16s)0));
         y = ((vec16u)vabs_diff(ys, (vec16s)0)) << 5;
         sign = (xs ^ ys) < 0;
         tg22x = x*13 + (x>>2);
         tg67x = tg22x + (x<<6);
         //

         d0  = m22 > m21 && m22 >= m23;
         d90 = m22 > m12 && m22 >= m32;
         d135 = m22 > m13 && m22 >  m31;
         d45  = m22 > m11 && m22 >  m33;
         add = (m22 > low)
                  &&(
                        (d0   && (y <  tg22x))
                    ||  (d90  && (y >  tg67x))
                    ||  (d135 && (y >= tg22x) && (y <= tg67x) &&  sign)
                    ||  (d45  && (y >= tg22x) && (y <= tg67x) && !sign)
                  );

         val = 0;
         vif (add) {
            vif (m22 > high) {
               val = 2;
               vstore(stack, stack_top, ((vec16u)(i)<<ROW_BIT_OFFSET)|(vec16u)(j));
               stack_top++;
            } velse {
               val = 1;
            } vendif
         } vendif
         *pd++ = val;
      }
      
      src += sstr;
      dst += dstr;
   }
   connect_edges(save, dstr, bw, bh);
}

/*!
   Performs edge connection between blocks. This is used where the
   block size does not change between kernel outputs.
   Minimum block height of 2.

   \param dst    - [Output] Destination edge map buffer
   \param dstr   - [Input]  Destination edge map stride
   \param src    - [Input]  Source edge map buffer
   \param sstr   - [Input]  Source edge map stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
 */
void apu_canny_connect_edges(
               vec08u* restrict dst, int dstr,
         const vec08u* restrict src, int sstr,
               int bw, int bh
               )
{
   vec08u p11, p12, p13, p21, p22, p23, p31, p32, p33;
   vec08u nb;
   const vec08u* restrict ps1;
   const vec08u* restrict ps2;
   const vec08u* restrict ps3;
   vec08u* restrict pd;
   vbool vcs0, vcs1;
   vec16u val;
   stack_top = 0;

   /* Set destination edge map to source edge map */
   for (int i = 1; i < bh-1; ++i) chess_loop_range(1,) {
      ps1 = src + i*sstr+1;
      pd = dst + i*dstr+1;
      for (int j = 1; j < bw-1; j+=2) chess_loop_range (1,) {
         *pd++ = *ps1++;
         *pd++ = *ps1++;
      }
   }

   /* Check the top row for any edges gained from padding
    * pppppppppp
    * poooooooop   p - padding pixels
    * pxxxxxxxxp   o - current pixels being checked
    * pxxxxxxxxp   x - not currently being checked
    * pppppppppp
    */
   
   /* Check the first pixel
    * ppp    p1n
    * pox    p2n
    * pxx    p3n
    */
   ps1 = src - sstr - 1;
   ps2 = src - 1;
   ps3 = src + sstr - 1;
   p11 = *ps1++; p12 = *ps1++; p13 = *ps1++;
   p21 = *ps2++; p22 = *ps2++;
   p31 = *ps3;
   pd = dst;
   /* Only one of them needs to be a definite edge */
   nb = p11 | p12 | p13 | p21 | p31;
   
   /* If the current pixel is a potential edge (PE) and there is a
    * DE pixel beside it, add it to the stack of DEs
    */
   vcs0 = p22 == 1;
   vcs1 = nb > 1;
   vcs0 = vcs0 && vcs1;
   val = vselect(2, p22, vcs0);
   *pd++ = (vec08u)val;
   vif (vcs0) {
      vstore(stack, stack_top, 0);
      stack_top++;
   } vendif
   
   /* Check the rest of the top row, excluding the last and first
    * pppppppppp    p1n
    * pxooooooxp    p2n
    */
   for (int i = 1; i < bw-1; ++i) {
      p11 = p12; p12 = p13; p13 = *ps1++;
      p22 = *ps2++;
      nb = p11 | p12 | p13;

      vcs0 = p22 == 1;
      vcs1 = nb > 1;
      vcs0 = vcs0 && vcs1;
      val = vselect(2, p22, vcs0);
      *pd++ = (vec08u)val;
      vif (vcs0) {
         vstore(stack, stack_top, (vec16u)i);
         stack_top++;
      } vendif
   }
   /* Check the last pixel of the first row
    * ppp    p1n
    * xop    p2n
    * xxp    p3n
    */
   p11 = p12; p12 = p13;    p13 = *ps1;
              p22 = *ps2++; p23 = *ps2; 
                            p33 = src[+sstr+bw]; 
   nb = p11 | p12 | p13 | p23 | p33;
   vcs0 = p22 == 1;
   vcs1 = nb > 1;
   vcs0 = vcs0 && vcs1;
   val = vselect(2, p22, vcs0);
   *pd++ = (vec08u)val;
   vif (vcs0) {
      vstore(stack, stack_top, (vec16u)(bw-1));
      stack_top++;
   } vendif
   
   /* Check the bottom row for any edges gained from padding
    * pppppppppp
    * pxxxxxxxxp   p - padding pixels
    * pxxxxxxxxp   o - current pixels being checked
    * poooooooop   x - not currently being checked
    * pppppppppp
    */
   
   /* Check the first pixel of the bottom row
    * pxx    p1n
    * pox    p2n
    * ppp    p3n
    */
   ps1 = src + (bh-2)*sstr-1;
   ps2 = ps1+sstr;
   ps3 = ps2+sstr;
   pd = dst+(bh-1)*dstr;
   p11 = *ps1;
   p21 = *ps2++; p22 = *ps2++;
   p31 = *ps3++; p32 = *ps3++; p33 = *ps3++;
   nb = p11 | p21 | p31 | p32 | p33;
   
   vcs0 = p22 == 1;
   vcs1 = nb > 1;
   vcs0 = vcs0 && vcs1;
   val = vselect(2, p22, vcs0);
   *pd++ = (vec08u)val;
   vif (vcs0) {
      vstore(stack, stack_top, (vec16u)(bh-1)<<8);
      stack_top++;
   } vendif
   
   /* Check the rest of the bottom row, excluding the last and first
    * pxooooooxp    p2n
    * pppppppppp    p3n
    */
   for (int i = 1; i < bw-1; ++i) {
      p31 = p32; p32 = p33; p33 = *ps3++;
      p22 = *ps2++;
      nb = p31 | p32 | p33;
      vcs0 = p22 == 1;
      vcs1 = nb > 1;
      vcs0 = vcs0 && vcs1;
      val = vselect(2, p22, vcs0);
      *pd++ = (vec08u)val;
      vif (vcs0) {
         vstore(stack, stack_top, (vec16u)(bh-1)<<8|(vec16u)i);
         stack_top++;
      } vendif
   }
   /* Check the last pixel of the bottom row
    * xxp    p1n
    * xop    p2n
    * ppp    p3n
    */
                            p13 = src[(bh-2)*sstr+bw];
              p22 = *ps2++; p23 = *ps2;
   p31 = p32; p32 = p33;    p33 = *ps3;
   nb = p13 | p23 | p31 | p32 | p33;

   vcs0 = p22 == 1;
   vcs1 = nb > 1;
   vcs0 = vcs0 && vcs1;
   val = vselect(2, p22, vcs0);
   *pd++ = (vec08u)val;
   vif (vcs0) {
      vstore(stack, stack_top, (vec16u)(bh-1)<<ROW_BIT_OFFSET|(vec16u)(bw-1));
      stack_top++;
   } vendif
   
   /* If the bh <=2 then the left and right sides will have been checked already */
   if (bh > 2) {
      /* Check the left side for any edges gained from padding
       * pppppppppp
       * pxxxxxxxxp   p - padding pixels
       * poxxxxxxxp   o - current pixels being checked
       * poxxxxxxxp   x - not currently being checked
       * pxxxxxxxxp
       * pppppppppp
       */

      /* Check the top pixel of the left side
       * ppp
       * pxx    p1n
       * pox    p2n
       * pxx    p3n
       */
      ps1 = src-1;
      ps2 = ps1+sstr;
      ps3 = ps2+sstr;
      pd = dst+dstr;
      p11 = *ps1;
      p21 = *ps2++; p22 = *ps2; ps2+=sstr;
      p31 = *ps3; ps3+=sstr;
      nb = p11 | p21 | p31;
      vcs0 = p22 == 1;
      vcs1 = nb > 1;
      vcs0 = vcs0 && vcs1;
      val = vselect(2, p22, vcs0);
      *pd = (vec08u)val; pd += dstr;
      vif (vcs0) {
         vstore(stack, stack_top, (vec16u)(1<<ROW_BIT_OFFSET));
         stack_top++;
      } vendif
      
      /* Check the rest of the left side */
      for (int i = 2; i < bh-1; ++i) {
         p11 = p21;
         p21 = p31; p22 = *ps2; ps2+=sstr;
         p31 = *ps3; ps3+=sstr;
         nb = p11 | p21 | p31;
         
         vcs0 = p22 == 1;
         vcs1 = nb > 1;
         vcs0 = vcs0 && vcs1;
         val = vselect(2, p22, vcs0);
         *pd = (vec08u)val; pd += dstr;
         vif (vcs0) {
            vstore(stack, stack_top, (vec16u)i<<ROW_BIT_OFFSET);
            stack_top++;
         } vendif
      }

      /* Check the right side for any edges gained from padding
       * pppppppppp
       * pxxxxxxxxp   p - padding pixels
       * pxxxxxxxop   o - current pixels being checked
       * pxxxxxxxop   x - not currently being checked
       * pxxxxxxxxp
       * pppppppppp
       */
      /* Check the top pixel of the right side
       * ppp
       * xxp    p1n
       * xop    p2n
       * xxp    p3n
       */
      ps1 = src+bw;
      ps2 = ps1+sstr;
      ps3 = ps2+sstr;
      pd = dst+dstr+bw-1;
      p13 = *ps1;
      p23 = *ps2--; p22 = *ps2; ps2+=sstr;
      p33 = *ps3; ps3+=sstr;
      nb = p13 | p23 | p33;
      vcs0 = p22 == 1;
      vcs1 = nb > 1;
      vcs0 = vcs0 && vcs1;
      val = vselect(2, p22, vcs0);
      *pd = (vec08u)val; pd += dstr;
      vif (vcs0) {
         vstore(stack, stack_top, (vec16u)(1<<ROW_BIT_OFFSET)|(vec16u)(bw-1));
         stack_top++;
      } vendif
      
      /* Check the rest of the right side */
      for (int i = 2; i < bh-1; ++i) {
         p13 = p23;
         p23 = p33; p22 = *ps2; ps2+=sstr;
         p33 = *ps3; ps3+=sstr;
         nb = p13 | p23 | p33;

         vcs0 = p22 == 1;
         vcs1 = nb > 1;
         vcs0 = vcs0 && vcs1;
         val = vselect(2, p22, vcs0);
         *pd = (vec08u)val; pd += dstr;
         vif (vcs0) {
            vstore(stack, stack_top, (vec16u)i<<ROW_BIT_OFFSET|(vec16u)(bw-1));
            stack_top++;
         } vendif
      }
   }

   connect_edges(dst, dstr, bw, bh);
}

/*!
   Performs edge connection between blocks. This is used where the
   block size changes between kernel outputs

   \param dst    - [Output] Destination edge map buffer
   \param dstr   - [Input]  Destination edge map stride
   \param src    - [Input]  Source edge map buffer
   \param sstr   - [Input]  Source edge map stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
 */
void apu_canny_connect_edges_full(
               vec08u* restrict dst, int dstr,
         const vec08u* restrict src, int sstr,
               int bw, int bh
               ) 
{
   vec08u p11, p12, p13, p21, p22, p23, p31, p32, p33;
   vec08u nb,nb2,nb3;
   stack_top = 0;

   for (int i = 0; i < bh; ++i) chess_loop_range(1, ) {
      const vec08u* restrict ps1 = src -  sstr - 1;
      const vec08u* restrict ps2 = src - 1;
      const vec08u* restrict ps3 = src + sstr - 1;
      vec08u* pd = dst+i*dstr;
      vec16u val;
      vbool vcs0, vcs1;
      p11 = *ps1++;
      p21 = *ps2++;
      p31 = *ps3++;
      nb = p11 | p21 | p31;

      p12 = *ps1++; p13 = *ps1++;
      p22 = *ps2++; p23 = *ps2++;
      p32 = *ps3++; p33 = *ps3++;
      nb2 = p12 | p32;
      nb3 = p13 | p33;
      nb = nb | nb2 | nb3 | p23;
      nb2 = nb2 | p22;
      
      vcs0 = p22 == 1;
      vcs1 = nb > 1;
      vcs0 = vcs0 && vcs1;
      val = vselect(2, p22, vcs0);
      *pd++ = (vec08u)val;
      vif (vcs0) {
         vstore(stack, stack_top, ((vec16u)i)<<ROW_BIT_OFFSET);
         stack_top++;
      } vendif

      for (int j = 1; j < bw; ++j) chess_loop_range(1,) {
         p13 = *ps1++;
         p22 = p23; p23 = *ps2++;
         p33 = *ps3++;
         
         nb = nb2;
         nb2 = nb3;
         nb3 = p13 | p33;
         nb = nb | nb2 | nb3 | p23;
         nb2 = nb2 | p22;
         
         vcs0 = p22 == 1;
         vcs1 = nb > 1;
         vcs0 = vcs0 && vcs1;
         val = vselect(2, p22, vcs0);
         *pd++ = (vec08u)val;
         vstore(stack, stack_top, (vec16u)((i<<ROW_BIT_OFFSET)|j));
         vif (vcs0) {
            stack_top++;
         } vendif
      }
      src += sstr;
   }
   
   connect_edges(dst, dstr, bw, bh);
}

/*!
   Outputs the _src_ image with _edge_ as a mask.

   \param dst    - [Output] Destination image buffer
   \param dstr   - [Input]  Destination image stride
   \param src    - [Input]  Source image buffer
   \param sstr   - [Input]  Source image stride
   \param edge   - [Input]  Source edge map buffer
   \param estr   - [Input]  Source edge map stride
   \param bw     - [Input]  Block width
   \param bh     - [Input]  Block height
 */
void apu_canny_create_image(
               vec08u* restrict dst, int dstr,
         const vec08u* restrict edge, int estr,
               int bw, int bh
)
{
   vec08u v11, v12, v21, v22;
   for (int i = 0; i < bh; i+=2) chess_loop_range(1,) {
      vec08u* restrict dptr = dst;
      vec08u* restrict dptr2 = dst+dstr;
      const vec08u* restrict sptr = (vec08u*)edge;
      const vec08u* restrict sptr2 = (vec08u*)edge+estr;
      
      for (int j = 0; j < bw; j+=2) chess_loop_range(1,) {
         v11 = *sptr++;
         v12 = *sptr++;
         v21 = *sptr2++;
         v22 = *sptr2++; v11 >>= 1;
         v12 >>= 1; v11 *= 255;
         *dptr++ = v11; v12 *= 255; v21 >>= 1;
         *dptr++ = v12; v21 *= 255; v22 >>= 1;
         *dptr2++ = v21; v22 *= 255;
         *dptr2++ = v22;
      }
      dst += 2*dstr;
      edge += 2*estr;
   }
}

#endif /* ACF_KERNEL_IMPLEMENTATION */
