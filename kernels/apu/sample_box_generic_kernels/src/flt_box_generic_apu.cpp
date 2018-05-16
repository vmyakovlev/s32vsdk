/******************************************************************************
 *  (C) Copyright NXP Corporation. 2016 All right reserved.
 * 
 *  Confidential Information
 *
 *  All parts of the NXP Program Source are protected by copyright law
 *  and all rights are reserved.
 *  This documentation may not, in whole or in part, be copied, photocopied,
 *  reproduced, translated, or reduced to any electronic medium or machine
 *  readable form without prior consent, in writing, from NXP.
 ******************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION
/* Includes */
#include "flt_box_generic_acf.h"
#include <stdio.h>

#define BUFF_ROWS 8
#define BUFF_MASK 7
#define BUFF_WIDTH 22

static vec16s prev[BUFF_ROWS][BUFF_WIDTH];
static vec16s buff[BUFF_WIDTH];
static int count = 0;


// Very unoptimized

// Assuming we have a full 16x8 block of data
// No top padding
void apu_flt_box_generic_init(
            vec16s* restrict dst, int dstr,
      const vec16s* restrict src, int sstr,
            int bw, int bh,
            int flt_x, int flt_y,
            int shift
)
{
   src -= flt_x >> 1;
   int r = 1<<(shift-1);

   // Setup the buffer with data
   {
      // Setup the top padding
      for (int i = -1; i < flt_y >> 1; ++i) chess_loop_range(2, )
      {
         const vec16s* ps = src;
         vec16s* p0 = prev[count&BUFF_MASK];
         count++;
         vec16s ps1 = *ps++;
         vec16s ps2 = *ps++;
         for (int j = 0; j < bw + flt_x - 1; j+=2) chess_loop_range(2, )
         {
            *p0++ = ps1;
            *p0++ = ps2;
            ps1 = *ps++;
            ps2 = *ps++;
         }
      }
      // Fill the rest of the buffer
      for (int i = 0; i < flt_y >> 1; ++i) chess_loop_range(1, )
      {
         src += sstr;
         const vec16s* ps = src;
         vec16s* p0 = prev[count&BUFF_MASK];
         count++;
         vec16s ps1 = *ps++;
         vec16s ps2 = *ps++;
         for (int j = 0; j < bw + flt_x - 1; j+=2) chess_loop_range(2, )
         {
            *p0++ = ps1;
            *p0++ = ps2;
            ps1 = *ps++;
            ps2 = *ps++;
         }
      }
   }
   
   // Calculate the first row
   int ind = count - (flt_y>>1) - 1;
   {
      const vec16s* p0 = prev[ind&BUFF_MASK];
      ind++;
      vec16s* pb = buff;
      vec16s p1 = *p0++;
      vec16s p2 = *p0++;
      for (int j = 0; j < bw + flt_x - 1; j+=2) chess_loop_range(2, )
      {
         *pb++ = ((flt_y>>1)+1)* p1;
         *pb++ = ((flt_y>>1)+1)* p2;
         p1 = *p0++;
         p2 = *p0++;
      }
   }
   // Setup the buffer
   for (int i = 0; i < (flt_y>>1); ++i) chess_loop_range(2, )
   {
      vec16s*  pb = buff;
      const vec16s* p0 = prev[ind&BUFF_MASK];
      ind++;
      vec16s ps0 = *p0++;
      vec16s pb0 = *pb;
      for (int j = 0; j < bw + flt_x - 1; ++j) chess_loop_range(4, )
      {
         vec16s sum = ps0 + pb0;
         *pb++ = sum;
         ps0 = *p0++;
         pb0 = *pb;
      }
   }

   // At this point, each element contians the sum of the elements above it
   // We can now populate the first row of values
   {
      vec32s acl = r;
      vec16s* pd = dst;
      vec16s* pb = buff;
      vec16s pb1 = *pb++;
      vec16s pb2 = *pb++;
      for (int j = 1; j < flt_x; j+=2) chess_loop_range(1, )
      {
         vacl(acl, pb1);
         vacl(acl, pb2);
         pb1 = *pb++;
         pb2 = *pb++;
      }
      vacl(acl, pb1);
      *pd++ = (vec16s)(acl >> shift);
      pb1 = pb2;
      vec16s* pb_p = buff;
      vec16s pb_p1 = *pb_p++;
      for (int i = 1; i < bw; ++i) chess_loop_range(3, )
      {
         vacl(acl, pb1 - pb_p1);
         *pd++ = (vec16s)(acl>>shift);
         pb1 = *pb++;
         pb_p1 = *pb_p++;
      }
   }
}

void
apu_flt_box_generic(
            vec16s* restrict dst, int dstr,
      const vec16s* restrict src, int sstr,
            int bw, int bh,
            int flt_x, int flt_y,
            int shift
)
{
   src -= flt_x >> 1;
   int r = 1<<(shift-1);
   for (int i = 0; i < bh; ++i) chess_loop_range(2, )
   {
      vec16s* pb = buff;
      vec16s* pd = dst;
      const vec16s* ps = src;
      vec16s* pp = prev[(count - flt_y)&BUFF_MASK];
      vec16s* pn = prev[count&BUFF_MASK];
      vec32s acl = r;
      count++;
      dst += dstr;
      src += sstr;
      
      vec16s pb1 = *pb;
      vec16s pp1 = *pp++;
      vec16s ps1 = *ps++;
      for (int j = 0; j < flt_x; ++j) chess_loop_range(3, )
      {
         vec16s diff = ps1 - pp1;
         pb1 += diff; pp1 = *pp++;
         *pn++ = ps1;
         ps1 = *ps++;
         *pb++ = pb1;
         vacl(acl, pb1);
         pb1 = *pb;
      }

      *pd++ = (vec16s)(acl >> shift);
      vec16s* pb_p = buff;
      vec16s pb_p1 = *pb_p++;
      for (int j = 1; j < bw; ++j) chess_loop_range(3, )
      {
         vec16s diff = ps1 - pp1;
         pb1 += diff; pp1 = *pp++;
         *pn++ = ps1;
         ps1 = *ps++;
         *pb++ = pb1;
         vacl(acl, pb1 - pb_p1);
         pb1 = *pb;
         pb_p1 = *pb_p++;
         *pd++ = (vec16s)(acl >> shift);
      }
   }
}

void apu_flt_box_generic_end(
   vec16s* restrict dst, int dstr,
   int bw, int bh, int flt_x, int flt_y, int shift)
{
   int count_save = count;
   int r = 1<<(shift-1);
   for (int i = 0; i < flt_y>>1; ++i)
   {
      vec16s* pb = buff;
      vec16s* pd = dst;
      const vec16s* ps = prev[(count_save - 1)&BUFF_MASK];
      vec16s* pp = prev[(count - flt_y)&BUFF_MASK];
      vec32s acl = r;
      count++;
      dst += dstr;

      vec16s pb1 = *pb;
      vec16s pp1 = *pp++;
      vec16s ps1 = *ps++;
      for (int j = 0; j < flt_x; ++j) chess_loop_range(3, )
      {
         vec16s diff = ps1 - pp1;
         pb1 += diff; pp1 = *pp++;
         ps1 = *ps++;
         *pb++ = pb1;
         vacl(acl, pb1);
         pb1 = *pb;
      }

      *pd++ = (vec16s)(acl >> shift);
      vec16s* pb_p = buff;
      vec16s pb_p1 = *pb_p++;
      for (int j = 1; j < bw; ++j) chess_loop_range(3, )
      {
         vec16s diff = ps1 - pp1;
         pb1 += diff; pp1 = *pp++;
         ps1 = *ps++;
         *pb++ = pb1;
         vacl(acl, pb1 - pb_p1);
         pb1 = *pb;
         pb_p1 = *pb_p++;
         *pd++ = (vec16s)(acl >> shift);
      }
   }
}


void apu_harris_box_5x5(
               vec16s* restrict dst, int dstr,
         const vec16s* restrict src, int sstr,
               int bw,  int bh,
               int shift
)
{
   int round = 1<<(shift-1);
   src = src-2*sstr-2;
   for (int i = 0; i < bh; ++i) chess_loop_range(1, ) {
      const vec16s* restrict p0 = src;
      const vec16s* restrict p1 = p0 + sstr;
      const vec16s* restrict p2 = p1 + sstr;
      const vec16s* restrict p3 = p2 + sstr;
      const vec16s* restrict p4 = p3 + sstr;
      vec16s* restrict pd = dst;
      vec32s chess_storage(V67) t = 0;
      vec16s chess_storage(V0) v0,
             chess_storage(V1) v1,
             chess_storage(V2) v2,
             chess_storage(V3) v3,
             chess_storage(V4) v4;
      vec16s chess_storage(V5) v5 = chess_copy(round);
             
      vec16s l0, l1, l2, l3, l4;/* Load targets. */
             
            /* Loop preamble */
#define SAVE_TMPS do{t0=v0;t1=v1;t2=v2;t3=v3;t4=v4;}while(0)
      vec16s t0, t1, t2, t3, t4;
      vec16s d;
      /*        |    ALU    |   Shift   |       Mul     |      Ld/St      */
      {SAVE_TMPS;           ;           ;               ; l0 = *p0++ ;      }
      {SAVE_TMPS;           ;           ;               ; l1 = *p1++ ;      }
      {SAVE_TMPS;           ;           ;               ; l2 = *p2++ ;v0=l0;}
      {SAVE_TMPS; vacl(t,t0);           ;               ; l3 = *p3++ ;v1=l1;}
      {SAVE_TMPS; vacl(t,t1);           ;               ; l4 = *p4++ ;v2=l2;}
      {SAVE_TMPS; vacl(t,t2);           ;               ; l0 = *p0++ ;v3=l3;}
      {SAVE_TMPS; vacl(t,t3);           ;               ; l1 = *p1++ ;v4=l4;}
      {SAVE_TMPS; vacl(t,t4);           ;               ; l2 = *p2++ ;v0=l0;}
      {SAVE_TMPS; vacl(t,t0);           ;               ; l3 = *p3++ ;v1=l1;}
      {SAVE_TMPS; vacl(t,t1);           ;               ; l4 = *p4++ ;v2=l2;}
      {SAVE_TMPS; vacl(t,t2);           ;               ; l0 = *p0++ ;v3=l3;}
      {SAVE_TMPS; vacl(t,t3);           ;               ; l1 = *p1++ ;v4=l4;}
      {SAVE_TMPS; vacl(t,t4);           ;               ; l2 = *p2++ ;v0=l0;}
      {SAVE_TMPS; vacl(t,t0);           ;               ; l3 = *p3++ ;v1=l1;}
      {SAVE_TMPS; vacl(t,t1);           ;               ; l4 = *p4++ ;v2=l2;}
      {SAVE_TMPS; vacl(t,t2);           ;               ; l0 = *p0++ ;v3=l3;}
      {SAVE_TMPS; vacl(t,t3);           ;               ; l1 = *p1++ ;v4=l4;}
      {SAVE_TMPS; vacl(t,t4);           ;               ; l2 = *p2++ ;v0=l0;}
      {SAVE_TMPS; vacl(t,t0);           ;               ; l3 = *p3++ ;v1=l1;}
      {SAVE_TMPS; vacl(t,t1);           ;               ; l4 = *p4++ ;v2=l2;}
      {SAVE_TMPS; vacl(t,t2);           ;               ; l0 = *p0,p0-=4;v3=l3;}
      {SAVE_TMPS; vacl(t,t3);           ;               ; l1 = *p1,p1-=4;v4=l4;}
      {SAVE_TMPS; vacl(t,t4);           ;               ; l2 = *p2,p2-=4;v0=l0;}
      {SAVE_TMPS; vacl(t,t0);           ;               ; l3 = *p3,p3-=4;v1=l1;}
      {SAVE_TMPS; vacl(t,t1);           ;               ; l4 = *p4,p4-=4;v2=l2;}
      {SAVE_TMPS; vacl(t,t2);           ;               ; l0 = *p0,p0+=5;v3=l3;}
      {SAVE_TMPS; vacl(t,t3);           ;               ; l1 = *p0,p0-=4;v4=l4;}
      {SAVE_TMPS; vacl(t,t4);           ;               ; l2 = *p1,p1+=5;v0=l0;}
      {SAVE_TMPS; vacl(t,v5);           ;               ; l3 = *p1,p1-=4;v1=l1;}
#if (__chess__)      
      {SAVE_TMPS; d = (vec16s)(t >> shift).low()        ;               ;v2=l2;}
#elif (__llvm__)
      {SAVE_TMPS; d = (vec16s)__builtin_apex_vec32_get_lo(t >> shift);  ;v2=l2;}
#endif
      {SAVE_TMPS; v1 = t1-t0;           ;               ; *pd++ = d;         }
      for (int j = 1; j < bw; ++j) chess_loop_range(1, ) {
         /*        |    ALU    |   Shift   |       Mul     |      Ld/St      */
         {SAVE_TMPS; vacl(t,v1);           ;               ; l4 = *p2,p2+=5;v3=l3;}
         {SAVE_TMPS; v3 = t3-t2;           ;               ; l0 = *p2,p2-=4;      }
         {SAVE_TMPS; vacl(t,v3);           ;               ; l1 = *p3,p3+=5;v4=l4;}
         {SAVE_TMPS;           ;           ;               ; l2 = *p3,p3-=4;v0=l0;}
         {SAVE_TMPS; v0 = t0-t4;           ;               ; l3 = *p4,p4+=5;v1=l1;}
         {SAVE_TMPS; vacl(t,v0);           ;               ; l4 = *p4,p4-=4;v2=l2;}
         {SAVE_TMPS; v2 = t2-t1;           ;               ; l0 = *p0,p0+=5;v3=l3;}
         {SAVE_TMPS; vacl(t,v2);           ;               ; l1 = *p0,p0-=4;v4=l4;}
         {SAVE_TMPS; v4 = t4-t3;           ;               ; l2 = *p1,p1+=5;v0=l0;}
         {SAVE_TMPS; vacl(t,v4);           ;               ; l3 = *p1,p1-=4;v1=l1;}
#if (__chess__)
         {SAVE_TMPS; d = (vec16s)(t >> shift).low()        ;               ;v2=l2;}
#elif (__llvm__)
         {SAVE_TMPS; d = (vec16s)__builtin_apex_vec32_get_lo(t >> shift);  ;v2=l2;}
#endif
         {SAVE_TMPS; v1 = t1-t0;           ;               ; *pd++ = d;         }
      }
      src += sstr;
      dst += dstr;
#undef SAVE_TMPS
   }
}

#endif