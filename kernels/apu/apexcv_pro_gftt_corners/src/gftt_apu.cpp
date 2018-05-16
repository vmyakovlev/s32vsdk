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

// #ifdef _DEBUG
// #include <stdio.h>
// #endif
#ifdef ACF_KERNEL_IMPLEMENTATION
#include <stdio.h>
#include "gftt_apu.h"


// Pad the left and right side of the buffers with replication on image border
static inline void pad_line_replicate(vec32s* dst, int bw, int pad_x, vec16s mask)
{
   vec32s* sl  = dst;
   vec32s* sl2 = dst;
   vec32s* sr  = dst + (bw-1);
   vec32s* sr2 = sr;
   vec32s vr, vl;

   vec32s vrp = *sl;
   vrp = vmrl(vrp);
   vec32s vlp = *sr;
   vlp = vmrr(vlp);
   // all CU
   for (int bx=0; bx<pad_x; ++bx) chess_loop_range(1, )
   {
      vr = *sr--;
      vl = *sl2++;
      vr = vmrr(vr, vrp);
      vl = vmrl(vl, vlp);
      *--sl = vr;
      *++sr2 = vl;
   }

   sl  = dst;
   sl2 = dst;
   sr  = dst + (bw-1);
   sr2 = sr;
   // mark == 1, first CU, padding left side of CU
   vif(mask == 1)
   {
      for (int bx=0; bx<pad_x; ++bx) chess_loop_range(1, )
      {
         *--sl2 = *sl;
      }
   }vendif
   // mark == 2, last CU, padding right side of CU
   vif(mask == 2)
   {
      for (int bx=0; bx<pad_x; ++bx) chess_loop_range(1, )
      {
         *++sr2 = *sr;
      }
   }vendif
}
// Pad the left and right side of the buffers with zero on image border
static inline void pad_line_zero(vec16s* dst, int bw, int pad_x)
{
   vec16s* sl = dst;
   vec16s* sl2 = dst;
   vec16s* sr = dst + (bw-1);
   vec16s* sr2 = sr;
   
   vec16s vr, vl;
   for (int bx=0; bx<pad_x; ++bx) chess_loop_range(1, )
   {
      vr = *sr--;
      vl = *sl2++;
      vr = vmrr(vr, (vec16s)0);
      vl = vmrl(vl, (vec16s)0);
      *--sl = vr;
      *++sr2 = vl;
   }
}
// Calculate DY Sobel filter
//
// | -1  -2  -1 |
// |  0   0   0 |
// |  1   2   1 |
//
// Taken from filter_centralderivatives_ht_apu.cpp
inline void sobel_dy(vec16s* restrict dy, vec08u* restrict p1, vec08u* restrict p3, int bw)
{
   vec16s vk0 = vec16s(1);
   vec16s vk1 = vec16s(2);
   vec16s vk2 = vec16s(1);

   vec16s v0, v1, v2, v3, v4, v5, v6, v7;
   vec16s l0, l1, l2, l3;/* Load targets. */

   /* Pointers */
   const vec08u* p0 = (const vec08u*)(p1-1);
   const vec08u* p2 = (const vec08u*)(p3-1);
   vec16s*       pd = dy;

   /* Loop preamble */
#define SAVE_TMPS do{t0=v0;t1=v1;t2=v2;t3=v3;t4=v4;t5=v5;t6=v6;t7=v7;}while(0)
   vec16s t0, t1, t2, t3, t4, t5, t6, t7;

   /*        |      ALU    |    Shift    |          Mul          |      Ld/St      */
   /* Peel -2 */
   {SAVE_TMPS;             ;             ;                       ;           ;      }
   {SAVE_TMPS;             ;             ;                       ;           ;      }
   {SAVE_TMPS;             ;             ;                       ; l2 = (vec16s)*p0++;      }
   {SAVE_TMPS;             ;             ;                       ; l3 = (vec16s)*p2++;      }
   {SAVE_TMPS;             ;             ;                       ;           ;v0=l2;}
   {SAVE_TMPS;             ;             ;                       ;           ;v1=l3;}
   /* Peel -1 */
   {SAVE_TMPS; v7 = t1 - t0;             ;                       ; l0 = (vec16s)*p0++;      }
   {SAVE_TMPS;             ;             ;                       ; l1 = (vec16s)*p2++;      }
   {SAVE_TMPS;             ;             ;                       ; l2 = (vec16s)*p0++;v0=l0;}
   {SAVE_TMPS;             ;             ;                       ; l3 = (vec16s)*p2++;v1=l1;}
   {SAVE_TMPS; v6 = t1 - t0;             ;                       ;           ;v2=l2;}
   {SAVE_TMPS;             ;             ;                       ;           ;v0=l3;}
   /* Peel  0 */
   {SAVE_TMPS; v7 = t0 - t2;             ; v3 = vmul     (t7, vk0); l0 = (vec16s)*p0++;      }
   {SAVE_TMPS;             ;             ; v4 = vmul     (t6, vk1); l1 = (vec16s)*p2++;      }
   {SAVE_TMPS; v3 = t3 + t4;             ; v4 = vmul     (t7, vk2); l2 = (vec16s)*p0++;v0=l0;}
   {SAVE_TMPS; v3 = t3 + t4;             ; v4 = vmul     (t6, vk0); l3 = (vec16s)*p2++;v1=l1;}
   {SAVE_TMPS; v6 = t1 - t0;             ; v5 = vmul     (t7, vk1);           ;v2=l2;}
   {SAVE_TMPS; v5 = t4 + t5;             ; v4 = vmul     (t6, vk2); *pd++ = t3;v0=l3;}
   /* Core loop */
   for (int x=1; x<bw>>1; ++x) chess_loop_range(1,){
      /*        |      ALU    |    Shift    |          Mul          |      Ld/St      */
      {SAVE_TMPS; v7 = t0 - t2;             ; v3 = vmul     (t7, vk0); l0 = (vec16s)*p0++;      }
      {SAVE_TMPS; v5 = t4 + t5;             ; v4 = vmul     (t6, vk1); l1 = (vec16s)*p2++;      }
      {SAVE_TMPS; v3 = t3 + t4;             ; v4 = vmul     (t7, vk2); l2 = (vec16s)*p0++;v0=l0;}
      {SAVE_TMPS; v3 = t3 + t4;             ; v4 = vmul     (t6, vk0); l3 = (vec16s)*p2++;v1=l1;}
      {SAVE_TMPS; v6 = t1 - t0;             ; v5 = vmul     (t7, vk1); *pd++ = t5;v2=l2;}
      {SAVE_TMPS; v5 = t4 + t5;             ; v4 = vmul     (t6, vk2); *pd++ = t3;v0=l3;}
   }
   /* Peel  N */
   {SAVE_TMPS;             ;             ;                       ;           ;    }
   {SAVE_TMPS; v5 = t4 + t5;             ;                       ;           ;    }
   {SAVE_TMPS;             ;             ;                       ;           ;    }
   {SAVE_TMPS;             ;             ;                       ;           ;    }
   {SAVE_TMPS;             ;             ;                       ; *pd++ = t5;    }
   {SAVE_TMPS;             ;             ;                       ;           ;    }
   /*        |      ALU    |    Shift    |          Mul          |     Ld/St     */
#undef SAVE_TMPS
}

// Calculate DX sobel filter
//
// | -1  0  1 |
// | -2  0  2 |
// | -1  0  1 |
//
// Taken from apu_filter_central_derivatives_ht_impl.cpp
inline void sobel_dx(vec16s* restrict dx, vec08u* restrict s1, vec08u* restrict s2, vec08u* restrict s3, int bw)
{
        vec16s v0, 
               v1,
               v2,
               chess_storage(V3) v3,
               chess_storage(V4) v4,
               chess_storage(V5) v5,
               chess_storage(V6) v6,
               chess_storage(V7) v7;
        vec16s l0, l1, l2, l3;/* Load targets. */
        
        /* Pointers */
        vec16s* p0 = (vec16s*)(s1-2);
        vec16s* p1 = (vec16s*)(s2-2);
        vec16s* p2 = (vec16s*)(s3-2);
        vec16s*       pd = dx;
        int k0 = 1;
        int k1 = 2;
        int k2 = 1;
        /* Loop preamble */
#define SAVE_TMPS do{t0=v0;t1=v1;t2=v2;t3=v3;t4=v4;t5=v5;t6=v6;t7=v7;}while(0)
        vec16s t0, t1, t2, t3, t4, t5, t6, t7;
        
        /*        |      ALU    |    Shift    |          Mul          |      Ld/St      */
        /* Peel -2 */
        {SAVE_TMPS;             ;             ;                       ; l0 = *p0++;      }
        {SAVE_TMPS;             ;             ;                       ; l1 = *p1++;      }
        {SAVE_TMPS;             ;             ;                       ; l2 = *p2++;v0=l0;}
        {SAVE_TMPS;             ;             ;                       ;           ;v1=l1;}
        {SAVE_TMPS;             ;             ;                       ;           ;v2=l2;}
        {SAVE_TMPS;             ;             ;                       ;           ;      }
        /* Peel -1 */
        {SAVE_TMPS;             ;             ; v3 = vmul_uhsl(t0, k0); l0 = *p0++;      }
        {SAVE_TMPS;             ;             ; v5 = vmul_uhsl(t1, k1); l1 = *p1++;      }
        {SAVE_TMPS; v5 = t3 + t5;             ; v3 = vmul_uhsl(t2, k2); l2 = *p2++;v0=l0;}
        {SAVE_TMPS; v7 = t3 + t5;             ; v3 = vmul_ulsl(t0, k0);           ;v1=l1;}
        {SAVE_TMPS;             ;             ; v4 = vmul_ulsl(t1, k1);           ;v2=l2;}
        {SAVE_TMPS; v4 = t3 + t4;             ; v3 = vmul_ulsl(t2, k2);           ;      }
        /* Peel  0 */
        {SAVE_TMPS; v6 = t3 + t4;             ; v3 = vmul_uhsl(t0, k0); l0 = *p0++;      }
        {SAVE_TMPS;             ;             ; v5 = vmul_uhsl(t1, k1); l1 = *p1++;      }
        {SAVE_TMPS; v5 = t3 + t5;             ; v3 = vmul_uhsl(t2, k2); l2 = *p2++;v0=l0;}
        {SAVE_TMPS; v5 = t3 + t5;             ; v3 = vmul_ulsl(t0, k0);           ;v1=l1;}
        {SAVE_TMPS; v7 = t5 - t7;             ; v4 = vmul_ulsl(t1, k1);           ;v2=l2;}
        {SAVE_TMPS; v4 = t3 + t4; v7 = t5 << 0; v3 = vmul_ulsl(t2, k2); *pd++ = t7;      }
        /* Core loop */
        for (int x=1; x<bw>>1; ++x) chess_loop_range(1,){
            /*        |      ALU    |    Shift    |          Mul          |      Ld/St      */
            {SAVE_TMPS; v4 = t3 + t4;             ; v3 = vmul_uhsl(t0, k0); l0 = *p0++;      }
            {SAVE_TMPS; v6 = t4 - t6;             ; v5 = vmul_uhsl(t1, k1); l1 = *p1++;      }
            {SAVE_TMPS; v5 = t3 + t5;             ; v3 = vmul_uhsl(t2, k2); l2 = *p2++;v0=l0;}
            {SAVE_TMPS; v5 = t3 + t5;             ; v3 = vmul_ulsl(t0, k0);           ;v1=l1;}
            {SAVE_TMPS; v7 = t5 - t7; v6 = t4 << 0; v4 = vmul_ulsl(t1, k1); *pd++ = t6;v2=l2;}
            {SAVE_TMPS; v4 = t3 + t4; v7 = t5 << 0; v3 = vmul_ulsl(t2, k2); *pd++ = t7;      }
        }
        /* Peel  N */
        {SAVE_TMPS; v4 = t3 + t4;             ;                       ;           ;    }
        {SAVE_TMPS; v6 = t4 - t6;             ;                       ;           ;    }
        {SAVE_TMPS;             ;             ;                       ;           ;    }
        {SAVE_TMPS;             ;             ;                       ;           ;    }
        {SAVE_TMPS;             ;             ;                       ; *pd++ = t6;    }
        {SAVE_TMPS;             ;             ;                       ;           ;    }
        /*        |      ALU    |    Shift    |          Mul          |     Ld/St     */
#undef SAVE_TMPS
}
static inline void covariance_line(
         vec16s* restrict dx, vec16s* restrict dy,
         vec16s* restrict dxx, vec16s* restrict dxy, vec16s* restrict dyy,
   int bw, int shift
)
{
   vec16s chess_storage(V7) v7 = 1<<(shift-1);
   vec16s chess_storage(V0) v0, 
             chess_storage(V1) v1,
             chess_storage(V2) v2,
             chess_storage(V4) v4,
             chess_storage(V5) v5;
      vec16s l0, l1;/* Load targets. */
      vec16u chess_storage(V3) v3,
             chess_storage(V6) v6;
        
      /* Pointers */
      const vec16s* p0 = dx;
      const vec16s* p1 = dy;
      
      /* Loop preamble */
#define SAVE_TMPS do{t0=v0;t1=v1;t2=v2;t3=v3;t4=v4;t5=v5;t6=v6;}while(0)
      vec16s t0, t1, t2, t4, t5;
      vec16u t3, t6;
      vec32s m;
      /*        |  Accumulate   |           Shift            |     Multiplication    |      Load/store     |  */
      {SAVE_TMPS;               ;                            ;                       ; l0  = *p0++;        }
      {SAVE_TMPS;               ;                            ;                       ; l1  = *p1++;        }
      {SAVE_TMPS;               ;                            ;                       ;            ;v0=l0;  }
      {SAVE_TMPS;               ;                            ;                       ;            ;v1=l1;  }
      {SAVE_TMPS;               ;                            ; v2 = vmul_shsh(t0, t0);                     }
      for (int j = 0; j < bw; ++j) chess_loop_range(1, ) {
         {SAVE_TMPS;               ;                            ; v3 = vmul_ulul((vec16u)t0, (vec16u)t0);                     }
         {SAVE_TMPS; vacl(v2,v3,v7);                            ; v4 = vmul_shul(t0, t0);                     }
         {SAVE_TMPS; vacm(v2,v3,t4);                            ; v5 = vmul_shsh(t0, t1);                     }
         {SAVE_TMPS; vacm(v2,v3,t4);                            ; v6 = vmul_ulul((vec16u)t0, (vec16u)t1);                     }
#if (__chess__)
         {SAVE_TMPS;               ; m=vsra(vec32s(v2,v3),shift);                       ; v3 = m.low();       }
#elif (__llvm__)
         {SAVE_TMPS;               ; m=vsra(vec32s(v2,v3),shift);                       ; v3 = (vec16u)__builtin_apex_vec32_get_lo(m); }
#endif
         {SAVE_TMPS; vacl(v5,v6,v7);                            ; v4 = vmul_shul(t0, t1); *dxx++ = (vec16s)t3;}
         {SAVE_TMPS; vacm(v5,v6,t4);                            ; v4 = vmul_shul(t1, t0); l0  = *p0++ ;       }
         {SAVE_TMPS; vacm(v5,v6,t4);                            ; v4 = vmul_shul(t1, t1);                     }
#if (__chess__)
         {SAVE_TMPS;               ; m=vsra(vec32s(v5,v6),shift);                       ; v6 = m.low();       }
#elif (__llvm__)
         {SAVE_TMPS;               ; m=vsra(vec32s(v5,v6),shift);                       ; v6 = (vec16u)__builtin_apex_vec32_get_lo(m); }
#endif
         {SAVE_TMPS;               ;                            ; v2 = vmul_shsh(t1, t1); *dxy++ = (vec16s)t6;}
         {SAVE_TMPS;               ;                            ; v3 = vmul_ulul((vec16u)t1, (vec16u)t1); l1  = *p1++ ;v0=l0; }
         {SAVE_TMPS; vacl(v2,v3,v7);                            ;                       ;                     }
         {SAVE_TMPS; vacm(v2,v3,t4);                            ;                       ;             ;v1=l1; }
         {SAVE_TMPS; vacm(v2,v3,t4);                            ;                       ;                     }
#if (__chess__)
         {SAVE_TMPS;               ; m=vsra(vec32s(v2,v3),shift);                       ; v3 = m.low();       }
#elif (__llvm__)
         {SAVE_TMPS;               ; m=vsra(vec32s(v2,v3),shift);                       ; v3 = (vec16u)__builtin_apex_vec32_get_lo(m); }
#endif
         {SAVE_TMPS;               ;                            ; v2 = vmul_shsh(t0, t0); *dyy++ = (vec16s)t3;}
      }
#undef SAVE_TMPS
}

static inline void non_maxima_suppression(
      vec16s* restrict dst,
      vec16s* restrict score_row,
      vec16s** restrict maxima_array,
            int bw, int radius
)
{
   int fy = radius>>1;
   vec16s* m = maxima_array[fy];
   for (int i = 0; i < bw; ++i) chess_loop_range(2, )
   {
      vec16s vm = m[i];
      vec16s v = score_row[i];
      for (int j = 0; j < fy;++j) chess_loop_range(2, )
      {
         vec16s* t = maxima_array[j];
         vec16s* h = maxima_array[fy+j+1];
         vec16s vt = t[i];
         vec16s vh = h[i];
         vswap(vt, vm, vt > vm);
         vswap(vh, vm, vh > vm);
      }
      dst[i] = vselect(v, 0, v > vm);
   }
}

static inline void non_maxima_x(vec16s* restrict dst, vec16s* restrict score, int bw, int radius)
{
   int fx = radius>>1;
   vec16s* pd = dst;
   vec16s* m = score;
   for (int i = 0; i < bw; ++i) chess_loop_range(2, )
   {
      vec16s* l = m-fx;
      vec16s* r = m+1;
      vec16s vm = *m++ - 1;
      vec16s vl = *l++;
      vec16s vr = *r++;
      for (int j = 0; j < fx; ++j) chess_loop_range(1, )
      {
         vswap(vl, vm, vl > vm);
         vswap(vr, vm, vr > vm);
         vl = *l++;
         vr = *r++;
      }
      *pd++ = vm;
   }
}

static inline void box_filter_x(
         vec16s* restrict bxx, vec16s* restrict bxy, vec16s* restrict byy,
         vec32s* restrict bxx_y, vec32s* restrict bxy_y, vec32s* restrict byy_y,
         int bw, int flt_x, int shift
)
{
   // flt_x is 3, 5, 7
   // shift is 3, 4, 5
   vec32s* pxx = bxx_y;
   vec32s* pxy = bxy_y;
   vec32s* pyy = byy_y;
   vec16s* dxx = bxx;
   vec16s* dxy = bxy;
   vec16s* dyy = byy;
   
   vec32s axx = 1<<(shift-1);
   vec32s axy = 1<<(shift-1);
   vec32s ayy = 1<<(shift-1);
   
   for (int i = 0; i < flt_x; ++i) chess_loop_range(1, )
   {
      axx += *pxx++;
      axy += *pxy++;
      ayy += *pyy++;
   }
   *dxx++ = (vec16s)(axx>>shift);
   *dxy++ = (vec16s)(axy>>shift);
   *dyy++ = (vec16s)(ayy>>shift);
   vec32s* ppxx = bxx_y;
   vec32s* ppxy = bxy_y;
   vec32s* ppyy = byy_y;
   for (int i = 1; i < bw; ++i) chess_loop_range(1, ) 
   {
      axx += (*pxx++ - *ppxx++);
      axy += (*pxy++ - *ppxy++);
      ayy += (*pyy++ - *ppyy++);
      *dxx++ = (vec16s)(axx>>shift);
      *dxy++ = (vec16s)(axy>>shift);
      *dyy++ = (vec16s)(ayy>>shift);
   }
}

static inline void box_filter_y_init(vec32s* pxx, vec32s* pxy, vec32s* pyy, vec16s* svxx, vec16s* svxy, vec16s* svyy, int bw, int flt_y)
{
   // flt_y is 3, 5, 7
   // (flt_y>>1)+1) will be 2, 3, 4
   for (int i = 0; i < bw; ++i) chess_loop_range(1, ) 
   {
      pxx[i] = ((flt_y>>1)+1)*(vec32s)svxx[i];
      pxy[i] = ((flt_y>>1)+1)*(vec32s)svxy[i];
      pyy[i] = ((flt_y>>1)+1)*(vec32s)svyy[i];
   }

   for (int i = 1; i <= flt_y>>1; ++i) chess_loop_range(1, )
   {
      for (int j = 0; j < bw; ++j) chess_loop_range(1, )
      {
         vacl(pxx[j], svxx[i*MAX_BLOCK_WIDTH+j]);
         vacl(pxy[j], svxy[i*MAX_BLOCK_WIDTH+j]);
         vacl(pyy[j], svyy[i*MAX_BLOCK_WIDTH+j]);
      }
   }
}

static inline void box_filter_y(
      vec32s* pxx, vec32s* pxy, vec32s* pyy,
      vec16s* svxx_p, vec16s* svxy_p, vec16s* svyy_p,
      vec16s* svxx, vec16s* svxy, vec16s* svyy, int bw
)
{
   for (int i = 0; i < bw; ++i) chess_loop_range(2, )
   {
      vacl(pxx[i], (svxx[i] - svxx_p[i]));
      vacl(pxy[i], (svxy[i] - svxy_p[i]));
      vacl(pyy[i], (svyy[i] - svyy_p[i]));
   }
}

////////
#define LT(x, y)((x)>(y))

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

static inline void swap(short *ptr1, short *ptr2)
{
   short tmp;
   tmp = *ptr1;
   *ptr1 = *ptr2;
   *ptr2 = tmp;
}
static inline void sort(short* vec, short* idx, int total)
{
   static struct
   {
     short *lb;
     short *ub;
   } stack[48];

   int isort_thresh = 7;
   int sp = 0;

   if( total <= 1 )
     return;

   short* arr = &idx[0];
   stack[0].lb = arr;
   stack[0].ub = arr + (total - 1);

   while( sp >= 0 )
   {
      short* left = stack[sp].lb;
      short* right = stack[sp--].ub;

      for(;;)
      {
         int i, n = (int)(right - left) + 1, m;
         short* ptr;
         short* ptr2;

         if( n <= isort_thresh )
         {
            insert_sort:
            for( ptr = left + 1; ptr <= right; ptr++ )
            {
               for( ptr2 = ptr; ptr2 > left && LT(vec[ptr2[0]], vec[ptr2[-1]]); ptr2--)
               {
                  swap(&ptr2[0], &ptr2[-1] );
               }
            }
            break;
         }
         else
         {
            short* left0;
            short* left1;
            short* right0;
            short* right1;
            short* pivot;
            short* a;
            short* b;
            short* c;
            short swap_cnt = 0;

            left0 = left;
            right0 = right;
            pivot = left + (n/2);

            if( n > 40 )
            {
               int d = n / 8;
               a = left, b = left + d, c = left + 2*d;
               vec[*a] = vec[*a];
               vec[*b] = vec[*b];
               vec[*c] = vec[*c];
               left = LT(vec[*a], vec[*b]) ? (LT(vec[*b], vec[*c]) ? b : (LT(vec[*a], vec[*c]) ? c : a))
                                           : (LT(vec[*c], vec[*b]) ? b : (LT(vec[*a], vec[*c]) ? a : c));

               a = pivot - d, b = pivot, c = pivot + d;
               pivot = LT(vec[*a], vec[*b]) ? (LT(vec[*b], vec[*c]) ? b : (LT(vec[*a], vec[*c]) ? c : a))
                                            : (LT(vec[*c], vec[*b]) ? b : (LT(vec[*a], vec[*c]) ? a : c));

               a = right - 2*d, b = right - d, c = right;
               right = LT(vec[*a], vec[*b]) ? (LT(vec[*b], vec[*c]) ? b : (LT(vec[*a], vec[*c]) ? c : a))
                                            : (LT(vec[*c], vec[*b]) ? b : (LT(vec[*a], vec[*c]) ? a : c));
            }

            a = left, b = pivot, c = right;
            pivot = LT(vec[*a], vec[*b]) ? (LT(vec[*b], vec[*c]) ? b : (LT(vec[*a], vec[*c]) ? c : a))
                                         : (LT(vec[*c], vec[*b]) ? b : (LT(vec[*a], vec[*c]) ? a : c));

            if( pivot != left0 )
            {
               swap(pivot, left0);
               pivot = left0;
            }

            left = left1 = left0 + 1;
            right = right1 = right0;

            for(;;)
            {
               while( left <= right && !LT(vec[*pivot], vec[*left]) )
               {
                  if( !LT(vec[*left], vec[*pivot]) )
                  {
                     if( left > left1 )
                     {
                        swap(left1, left );
                     }
                     swap_cnt = 1;
                     left1++;
                  }
                  left++;
               }

               while( left <= right && !LT(vec[*right], vec[*pivot]) )
               {
                  if( !LT(vec[*pivot], vec[*right]) )
                  {
                     if( right < right1 )
                     {
                        swap(right1, right );
                     }
                     swap_cnt = 1;
                     right1--;
                  }
                  right--;
               }

               if( left > right )
                  break;

               swap(left, right );
               swap_cnt = 1;
               left++;
               right--;
            }

            if( swap_cnt == 0 )
            {
               left = left0, right = right0;
               goto insert_sort;
            }

            n = MIN( (left1 - left0), (left - left1) );
            for( i = 0; i < n; i++ )
            {
               swap(&left0[i], &left[i-n] );
            }

            n = MIN( (right0 - right1), (right1 - right) );
            for( i = 0; i < n; i++ )
            {
               swap(&left[i], &right0[i-n+1] );
            }

            n = (left - left1);
            m = (right1 - right);
            if( n > 1 )
            {
               if( m > 1 )
               {
                  if( n > m )
                  {
                     stack[++sp].lb = left0;
                     stack[sp].ub = left0 + n - 1;
                     left = right0 - m + 1, right = right0;
                  }
                  else
                  {
                     stack[++sp].lb = right0 - m + 1;
                     stack[sp].ub = right0;
                     left = left0, right = left0 + n - 1;
                  }
               }
               else
               {
                  left = left0, right = left0 + n - 1;
               }
            }
            else if( m > 1 )
            {
               left = right0 - m + 1, right = right0;
            }
            else
              break;
         }
      }
   }
}
static inline void replicate(vec16s* dst, vec16s* src, int sstr)
{
  for (int i = 0; i < sstr; i++)
    dst[i] = src[i];
}
static inline void zero(vec16s* dst, int sstr)
{
  for (int i = 0; i < sstr; i++)
    dst[i] = 0;
}
static inline vec16u _sqrt(vec32u val)
{
   vec32u rem = 0;
   vec32u root = 0;
   for (int i = 0; i < 16; i++)
   {
      root <<= 1;
      rem = ((rem<<2) + (val>>30));
      val <<= 2;
      vif (root < rem)
      {
         root++;
         rem -= root;
         root++;
      }
      vendif
   }

   return (vec16u)(root>>1);
}
/*!
   - Calculate minimal eigen value, simple form without strides
   - Update local max eigen value in each CUs
   (signed 16-bit -> signed 16-bit)
   \param dst        - [Output]  Pointer to the destination buffer.
   \param srcXX      - [Input]   Pointer to the source image Dxx.
   \param srcYY      - [Input]   Pointer to the source image Dyy.
   \param srcXY      - [Input]   Pointer to the source image Dxy.
   \param bw         - [Input]   Block width.
   \param bh         - [Input]   Block height.
   \param local_max  - [Input]   Local Max Eigen Value in each CUs.
 */
static inline void apu_calc_min_eigen_value_and_local_max (  
                                    vec16s *dst,
                                    const vec16s *srcXX, 
                                    const vec16s *srcXY,
                                    const vec16s *srcYY,
                                    int bw,
                                    vec16s* local_max)
{
  const  vec16s* srcXX_t = srcXX;
  const  vec16s* srcXY_t = srcXY;
  const  vec16s* srcYY_t = srcYY;
  vec16s* dst_t = dst;
  for (int w = 0; w < bw; w++) chess_loop_range(1, )
  {
    vec16s b = *srcXY_t++;
    vec16s a = *srcXX_t++>>1;
    vec32u bb = (vec32u)b*(vec32u)b;
    vec16s c = *srcYY_t++>>1;
    vec16s acs = a-c;
   
    vec32u acac = (vec32u)acs*(vec32u)acs;
    vec16s ac = a+c;
    vec16s sqrt = (vec16s)_sqrt(acac+bb);

    *dst_t = ac - (vec16s)sqrt;
    local_max[0] = vselect(local_max[0], *dst_t, local_max[0] > *dst_t++);
  }
}
// gftt_wrapper represents the gftt rework which combines several kernels
// into one wrapper with delay buffer.
void gftt_wrapper(
            vec16s* dst, int dstr,
            vec08u* src, int sstr,
            int bw, int bh,
            vec16s* svxx, vec16s* svxy, vec16s* svyy,
            vec16s* nms_x, vec16s* nms,
            vec32s* bxx_y, vec32s* bxy_y, vec32s* byy_y,
            int flt_x, int flt_y, int radius,
            int16_t  lTotalNumLines,
            bool first_tile,
            vec16s* maxEigen,
            int8_t covariance_shift, 
            int8_t box_shift,
			vec16s mask
)
{
   // flt_x and flt_y are 3, 5, 7, flt_y>>1 are 1, 2, 3
   // flt_x == flt_y
   // box_shift is 3, 4, 5
   // bw = 20
   // bh = 16
   int flt_x_1 = flt_x>>1;
   static int scount = 0;
   static int rcount = 0;
   int bufferId      = 0;
   int preBufferId   = 0;
   int nmsBufferId   = 0;
   int nmsId         = 0;

   static int fltMaskId     = 0;
   int        preFltMaskId  = 0;
   int        tailFltMaskId = 0;
   static int nmsFltMaskId  = 2;
          int nmsCalId      = 0;

   vec16s b1[MAX_BLOCK_WIDTH];
   vec16s b2[MAX_BLOCK_WIDTH];
   vec16s b3[MAX_BLOCK_WIDTH];
   vec08u* p1;
   vec08u* p2;
   vec08u* p3;
   if (first_tile)
   {
      scount = 0;
      rcount = radius>>1;
      // Fill the top border of the NMS buffer with zeros
      for (int i = 0; i < radius; ++i)
      {
         vec16s* ps = &nms[i*MAX_BLOCK_WIDTH];
         for (int k = 0; k < bw; k+=2) chess_loop_range(1, )
         {
             *ps++ = 0;
             *ps++ = 0;
         }
      }
   }
   p1 = src-sstr;
   p2 = src;
   p3 = src+sstr;
   for (int h = 0; h < bh; ++h) chess_loop_range(1, )
   {
      bufferId    = fltMaskId*MAX_BLOCK_WIDTH;
      if (scount < lTotalNumLines)
      {
         // Calculate DX Sobel filter
         sobel_dx(b1, p1, p2, p3, bw);

         // Calculate DY Sobel filter
         sobel_dy(b2, p1, p3, bw);
        
         // Calculate covariance filters - in this case just do multiplications
         covariance_line(b1, b2, &svxx[bufferId], &svxy[bufferId], &svyy[bufferId], bw, covariance_shift);
      }
      else
      {
         preFltMaskId = fltMaskId - 1;
         if (preFltMaskId < 0) preFltMaskId = flt_x;
    	 preBufferId = preFltMaskId*MAX_BLOCK_WIDTH;

         replicate(&svxx[bufferId], &svxx[preBufferId], bw);
         replicate(&svxy[bufferId], &svxy[preBufferId], bw);
         replicate(&svyy[bufferId], &svyy[preBufferId], bw);
      }
      // Calculate the Score + threshold1
      if (scount >= flt_x_1)
      {
         nmsBufferId = nmsFltMaskId*MAX_BLOCK_WIDTH;

         if (rcount < (lTotalNumLines + (radius>>1)))
         {
            // Update the box buffers
            // flt_x = flt_y = box_size one of 3, 5, 7
            // bw is source image chunk width
            vec32s* pxx = bxx_y + (flt_x_1);
            vec32s* pxy = bxy_y + (flt_x_1);
            vec32s* pyy = byy_y + (flt_x_1);
           
            if (scount == flt_x_1)
            {
               // We have enough lines to setup the box filter y now
               box_filter_y_init(pxx, pxy, pyy, svxx, svxy, svyy, bw, flt_x);
            }
            else
            {
               if ((scount - flt_x) > 0)
                  tailFltMaskId = fltMaskId + 1;
               if (tailFltMaskId > flt_x) tailFltMaskId = 0;

               int tail = tailFltMaskId*MAX_BLOCK_WIDTH;

               box_filter_y(pxx, pxy, pyy, &svxx[tail], &svxy[tail], &svyy[tail], &svxx[bufferId], &svxy[bufferId], &svyy[bufferId], bw);
            }

            // Pad the left and right side of the buffers, flt_>>1 are 1, 2, 3
            pad_line_replicate(pxx, bw, flt_x_1, mask);
            pad_line_replicate(pxy, bw, flt_x_1, mask);
            pad_line_replicate(pyy, bw, flt_x_1, mask);
           
            // Calculate the box filter x
            // box_shift will be one of 3, 4, 5 = value box_size >> 1 + nms_size >> 1
            box_filter_x(b1, b2, b3, bxx_y, bxy_y, byy_y, bw, flt_x, box_shift);
           
            // Calculate score and pad with zeros
            /*
            b1 -> bxx
            b2 -> bxy
            b3 -> byy
            */
            nmsId = (nmsFltMaskId)*(MAX_BLOCK_WIDTH+MAX_NMS_R-1)+(flt_x_1);

            apu_calc_min_eigen_value_and_local_max(&nms_x[nmsId], b1, b2, b3, bw, maxEigen);

            pad_line_zero(&nms_x[nmsId], bw, radius>>1);
            // Perform NMS in the X direction (modified maximum)
            non_maxima_x(&nms[nmsBufferId], &nms_x[nmsId], bw, radius);
         }
         else
            zero(&nms[nmsBufferId], bw);

         rcount++;
         if (++nmsFltMaskId > NMS_MASK) nmsFltMaskId = 0;
      }
      // Calculate the NMS
      if (rcount >= radius)
      {

         vec16s* maxima_array[MAX_NMS_R];
         int j = radius;
         for (int i = 0; i < radius; ++i, --j)
         {
            maxima_array[i] = &nms[((rcount-j)&NMS_MASK)*MAX_BLOCK_WIDTH];
         }
         non_maxima_suppression(dst, &nms_x[((rcount-(radius>>1)-1)&NMS_MASK)*(MAX_BLOCK_WIDTH+MAX_NMS_R-1) + (flt_x>>1)], maxima_array, bw, radius);
      }
      p1 = p2;
      p2 = p3;
      p3 += sstr;
      dst += dstr;

      if (++fltMaskId > flt_x) fltMaskId = 0;

      scount++;

   }
}
/*!
   Extract corner from CMEM and merge to global corners list
   Similar to extract_corner from harris but slightly modified for gftt
*/
void extract_corners_gftt(int32s* coord, int16_t* stren, int32s* total_cnt,
                     vec32s* dst0, vec16s* dst1,
                     vec16s* src,  int sstr,
                     vec08u chunkId,
                     int start_cu, int end_cu,
                     int bw, int bh, vec16s max_eigen, int quality_level)
{
   static int start_y = 0;
   int16_t gcnt=0;
   int16_t* coord16  = (int16_t*)(&coord[total_cnt[0]]);
   int16_t* stren16 = (&stren[total_cnt[0]]);
   
   //stop extracting when MAX_CORNERS is reach
   if (total_cnt[0] > MAX_CORNERS) return;
   
   //Find the global max eigen among CUs
   int16_t global_max_eigen = vget(max_eigen, start_cu);
   for (int cu = start_cu + 1; cu <= end_cu; ++cu)
   {
      int tmp = vget(max_eigen, cu);
      global_max_eigen = global_max_eigen > tmp ? global_max_eigen : tmp;
   }
   //Calculate input threshold
   int16_t threshold = (((global_max_eigen*quality_level)>>7)+1)>>1;

   vec16s* psrc  = src;
   vec16s* pdst0 = (vec16s*)dst0;
   vec16s* pdst1 = dst1;
   vec16s lcnt   = 0;
   vec16s lx = bw*(vec16s)chunkId;
   
   // extract corners using the threshold
   for (int h = 0; h < bh; ++h) chess_loop_range(1, ) 
   {
      vec16s* ptr0 = psrc;
      for (int w = 0; w < bw; ++w) chess_loop_range(1, ) 
      {
         vec16s t0 = *ptr0++;
         vif ((lcnt<(vec16s)MAX_CORNER_PER_CHUNK) && (t0 > (vec16s)0) && (t0 >= (vec16s)threshold))
         {
            vstore(pdst0, lcnt*2+0, (lx + w));//x cordinate
            vstore(pdst0, lcnt*2+1, start_y);//y cordinate
            vstore(pdst1, lcnt, t0);//corner strength
            lcnt++;
         }
      } // end of bw
      psrc+=sstr;
      start_y++;
   } // end of bh
  //Combine corners from each CU to the global corners list
   if (vany(lcnt!=0))
   {
      for (int cu=start_cu; cu<=end_cu; ++cu) chess_loop_range(1, ) 
      {
         int16_t ccnt = vget(lcnt, cu);
         if ((ccnt>0) && ((ccnt+total_cnt[0]+gcnt) <= MAX_CORNERS))
         {
               pdst0 = (vec16s*)dst0;
               pdst1 = dst1;
               for (int i=0, j=0; i<ccnt; ++i, j+=2)
               {
                  int16_t score = vget(pdst1[i], cu);
                  *stren16++ = score;
                  *coord16++ = vget(pdst0[j], cu); 
                  *coord16++ = vget(pdst0[j+1], cu);
                  gcnt++;
               }
         } // end of if non-zero
      }
      total_cnt[0] += gcnt;
   }
}
//----------------------------------------------------------
// This function:
// - Sort corners
// - Filter out corners that does not meet minimum distance
void sort_and_filter_corners(
      int32s* feature_list,
      int32s& feat_cnt,
      int32s* coord_list,
      int16_t* stren,
      int quality_level, int maxCorners, 
      short* index_buff, int total_cnt)
{
   //Sort
   short* idx    = index_buff;
   int32_t* feat = (int32_t*)feature_list;
   
   for (int i=0; i<total_cnt; ++i) {
      idx[i] = i;
   }
   sort(stren, idx, total_cnt);
   //Calculate input threshold
   int threshold;
   threshold = (((stren[idx[0]]*quality_level)>>7)+1)>>1;
   threshold = threshold > 0 ? threshold : 0;
   //Filter corners using minimum distance and threshold
   feat_cnt = 0; //count number of elements in the output list

   for (int i = 0; i < total_cnt ; ++i)
   {
      if (feat_cnt >= maxCorners) break;
      //corner
      if (stren[idx[i]] >= threshold)
      {
         feat[feat_cnt*2] = coord_list[idx[i]];
         feat[feat_cnt*2+1] = stren[idx[i]];
         feat_cnt++;
      }
   }
}
#endif /* ACF_KERNEL_IMPLEMENTATION */
