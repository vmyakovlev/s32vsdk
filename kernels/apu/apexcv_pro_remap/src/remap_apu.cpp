/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2015-2017 NXP
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

void
remap_bilinear_rgb
               (vec32u __cmem* dst,
                vec32u __cmem* src,
                vec16u __cmem* offset, 
                vec08u __cmem* delta, 
                int sstride, // stride of the src array in cmem
                int dstride, // stride of the src array in cmem
                int bw,
                int bh)
{
   vec16u idx;             // offset value, used for src lookup
   
   vec32u sA, sB, sC, sD;  // 32-bit source values for ABCD
   vec08u eA, eB, eC, eD;  //  8-bit element of source value
   vec08u deltaX, deltaY;  //  8-bit delta values

   // 1-d offset for offset/delta lookup
   int offset_idx = 0;
   // index of first pixel in dest tile row
   int dest_row = 0;

   
   // loop on destination pixels
   for (int y=0; y<bh; y++) {
      for (int x=0; x<bw; x++) {
        idx = (vec16u)offset[offset_idx];

        sA = vload(src, (vec16s)idx);
        sB = vload(src, (vec16s)(idx+1));
        sC = vload(src, (vec16s)(idx+sstride));
        sD = vload(src, (vec16s)(idx+sstride+1));
        
        // deltaX index is 2i+0.  deltaY index is 2i+1
        deltaX = delta[(offset_idx<<1)+0];
        deltaY = delta[(offset_idx<<1)+1];
        
        vec32u destPixel = 0;

        // Process each 8-bit element of the 32-bit pixel
        for (int k=0; k<32; k+=8) {
            eA = (vec08u)(sA >> k);              
            eB = (vec08u)(sB >> k);              
            eC = (vec08u)(sC >> k);              
            eD = (vec08u)(sD >> k);  

/*            
            // Original implementation - correct, but is not a bit-wise match with reference code
            // see CIMG_RefDewarp_c.cpp, fixedPointBilinRGB()
            vec16u AB = (vec16u)((eB - eA) * deltaX) + ((vec16u)eA << 8);
            vec16u CD = (vec16u)((eD - eC) * deltaX) + ((vec16u)eC << 8);
            
            vec32u ABCD = (((vec32u)CD - (vec32u)AB) * (vec32u)deltaY) + ((vec32u)AB << 8);
            ABCD += 0x00008000;   // for rounding
            
            vec08u destElement = (vec08u)(ABCD >> 16);
*/

            //vec08u AB = ((((eB - eA) * deltaX) + 0) >> 8) + eA;
            //vec08u CD = ((((eD - eC) * deltaX) + 0) >> 8) + eC;
            vec16u AB = ((((((vec16u)eB - (vec16u)eA) * (vec16u)deltaX) + 0) >> 8) + (vec16u)eA);
            vec16u CD = ((((((vec16u)eD - (vec16u)eC) * (vec16u)deltaX) + 0) >> 8) + (vec16u)eC);

            // see CLOVER-159
            AB &= 0x00ff;
            CD &= 0x00ff;
            
            //vec08u ABCD = ((((CD - AB) * deltaY) + 128) >> 8) + AB;
            vec16u ABCD = ((((CD - AB) * (vec16u)deltaY) + 128) >> 8) + AB;

            vec32u destElement = (vec32u)ABCD;
            
            // see CLOVER-159
            destElement &= 0x000000ff;
            
            destPixel |= (vec32u)destElement << k;
            
        }
         
        //dst[idx] = destPixel;
        vstore(dst, (vec16s)(dest_row + x), destPixel);
        
        // Assumes no stride in offset/delta.
        offset_idx++;
      }  // next pixel
      dest_row += dstride;
   } // next row

}
#endif

#ifdef ACF_KERNEL_IMPLEMENTATION

void
remap_bilinear_grayscale
(vec08u __cmem* dst,
vec08u __cmem* src,
vec16u __cmem* offset,
vec08u __cmem* delta,
int sstride, // stride of the src array in cmem
int dstride, // stride of the src array in cmem
int bw,
int bh)
{
    vec16u idx;             // offset value, used for src lookup

    vec08u sA, sB, sC, sD;  // 8-bit source values for ABCD
    //   vec08u eA, eB, eC, eD;  //  8-bit element of source value
    vec08u deltaX, deltaY;  //  8-bit delta values

    // 1-d offset for offset/delta lookup
    int offset_idx = 0;
    // index of first pixel in dest tile row
    int dest_row = 0;

   // loop on destination pixels
   for (int y=0; y<bh; y++) {
      for (int x=0; x<bw; x++) {
        idx = (vec16u)offset[offset_idx];

      sA = vload(src, (vec16s)idx);
      sB = vload(src, (vec16s)(idx+1));
      sC = vload(src, (vec16s)(idx+sstride));
      sD = vload(src, (vec16s)(idx+sstride+1)); 
        
      // see CLOVER-159
      sA &= 0x00ff;
      sB &= 0x00ff;
      sC &= 0x00ff;
      sD &= 0x00ff;

      // deltaX index is 2i+0.  deltaY index is 2i+1
        deltaX = delta[(offset_idx<<1)+0];
        deltaY = delta[(offset_idx<<1)+1];

      vec08u destPixel = 0;

      //vec08u AB = ((((sB - sA) * deltaX) + 0) >> 8) + sA;
      //vec08u CD = ((((sD - sC) * deltaX) + 0) >> 8) + sC;
      vec16u AB = ((((((vec16u)sB - (vec16u)sA) * (vec16u)deltaX) + 0) >> 8) + (vec16u)sA);
      vec16u CD = ((((((vec16u)sD - (vec16u)sC) * (vec16u)deltaX) + 0) >> 8) + (vec16u)sC);

      // see CLOVER-159
      AB &= 0x00ff;
      CD &= 0x00ff;
            
      //vec08u ABCD = ((((CD - AB) * deltaY) + 128) >> 8) + AB;
      vec16u ABCD = ((((CD - AB) * (vec16u)deltaY) + 128) >> 8) + AB;

      // see CLOVER-159
      ABCD &= 0x00ff;

      destPixel = (vec08u)ABCD;

      
      //dst[idx] = destPixel;
        vstore(dst, (vec16s)(dest_row + x), destPixel);
      
        // Assumes no stride in offset/delta.
        offset_idx++;
      }  // next pixel
      dest_row += dstride;
   } // next row

}

void remap_block_size(vec32u *map, int32_t chunk_width, int32_t chunk_height, int32_t tile_width, vec16u *t_width, vec16u *t_height, vec16u *f_width, vec16u *f_height, vec16u *e_width, vec16u *e_height, vec16u *s_width, vec16u *s_height) 
{  
   //int32_t image_width = tile_width * chunk_width;
   // divider fo y and x offset in Fixed_offset;
   int32_t offsetShift = 0x00010000;
   vec32u *p_off = map;
   vec16u s_xmin = 0xffff;
   vec16u s_xmax = 0;
   vec16u s_ymin = 0xffff;
   vec16u s_ymax = 0;
   for (int s = 0; s < (chunk_height >> 3); ++s) {
      vec16u e_xmin = 0xffff;
      vec16u e_xmax = 0;
      vec16u e_ymin = 0xffff;
      vec16u e_ymax = 0; 
      for (int e = 0; e < 2; ++e) {
         vec16u f_xmin = 0xffff;
         vec16u f_xmax = 0;
         vec16u f_ymin = 0xffff;
         vec16u f_ymax = 0;
         for (int f = 0; f < 2; ++f) {
            vec16u t_xmin = 0xffff;
            vec16u t_xmax = 0;
            vec16u t_ymin = 0xffff;
            vec16u t_ymax = 0;
            for (int y = 0; y < 2; ++y) {
               for (int x = 0; x < chunk_width; ++x) {
                  vec32u o = *p_off;
                  p_off++;
                  
                  vec16u x_off = (vec16u)(o % offsetShift);
                  vec16u y_off = (vec16u)(o / offsetShift);
                  
                  t_xmin = vselect(t_xmin, x_off, t_xmin < x_off);
                  t_xmax = vselect(t_xmax, x_off, t_xmax > x_off);
                  t_ymin = vselect(t_ymin, y_off, t_ymin < y_off);
                  t_ymax = vselect(t_ymax, y_off, t_ymax > y_off);
               }
            }
             
            vec16u width = t_xmax - t_xmin + 1;
            vec16u height = t_ymax - t_ymin + 1;
            
            *t_width = vselect(*t_width, width, *t_width > width);
            *t_height = vselect(*t_height, height, *t_height > height);
            
            f_xmin = vselect(f_xmin, t_xmin, f_xmin < t_xmin);
            f_xmax = vselect(f_xmax, t_xmax, f_xmax > t_xmax);
            f_ymin = vselect(f_ymin, t_ymin, f_ymin < t_ymin);
            f_ymax = vselect(f_ymax, t_ymax, f_ymax > t_ymax);
         }

         vec16u width = f_xmax - f_xmin + 1;
         vec16u height = f_ymax - f_ymin + 1;
         
         *f_width = vselect(*f_width, width, *f_width > width);
         *f_height = vselect(*f_height, height, *f_height > height);
         
         e_xmin = vselect(e_xmin, f_xmin, e_xmin < f_xmin);
         e_xmax = vselect(e_xmax, f_xmax, e_xmax > f_xmax);
         e_ymin = vselect(e_ymin, f_ymin, e_ymin < f_ymin);
         e_ymax = vselect(e_ymax, f_ymax, e_ymax > f_ymax);   
      }

      vec16u width = e_xmax - e_xmin + 1;
      vec16u height = e_ymax - e_ymin + 1;
      
      *e_width = vselect(*e_width, width, *e_width > width);
      *e_height = vselect(*e_height, height, *e_height > height);
      
      s_xmin = vselect(s_xmin, e_xmin, s_xmin < e_xmin);
      s_xmax = vselect(s_xmax, e_xmax, s_xmax > e_xmax);
      s_ymin = vselect(s_ymin, e_ymin, s_ymin < e_ymin);
      s_ymax = vselect(s_ymax, e_ymax, s_ymax > e_ymax);    
   }
  
   vec16u width = s_xmax - s_xmin + 1;
   vec16u height = s_ymax - s_ymin + 1;
   
   *s_width = vselect(*s_width, width, *s_width > width);
   *s_height = vselect(*s_height, height, *s_height > height);
}

#endif
