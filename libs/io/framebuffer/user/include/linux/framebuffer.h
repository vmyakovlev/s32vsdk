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
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../../../kernel/include/vdma.h"
#include "../../../kernel/include/ioctl_cmd.h"

inline void neon_copy_bgr_to_grb(unsigned char* src, unsigned char* dst, int size)
{
#ifndef __ARMV8
  int simd_pixels = size & ~7; 
  int simd_iterations = simd_pixels >> 3;
  
  if(simd_iterations)
  { 
	asm volatile( "1: \n\t"
		      "vld3.8 {d0, d1, d2}, [%[source]]! \n\t" 
		      "vswp d0, d1 \n\t" 
		      "vswp d1, d2 \n\t" 
		      "vst3.8 {d0, d1, d2}, [%[dest]]! \n\t" 
		      "subs %[iterations],%[iterations],#1 \n\t"
		      "bne 1b \n\t"
		      : [source]"+r"(src), [dest] "+r"(dst), [iterations]"+r"(simd_iterations) 
		      : 
		      : "memory", "d0", "d1", "d2" 
		   );
	}   
#else
  std::cerr << "neon_copy_bgr_to_grb(" << src << ", " << dst << ", " << size << ");  NOT IMPLEMENTED for ARMv8" << std::endl;
#endif
}

inline void neon_memcpy(unsigned char* src, unsigned char* dst, int size)
{
#ifndef __ARMV8
  int simd_pixels = size & ~31; 
  int simd_iterations = simd_pixels >> 5;
  
  if(simd_iterations)
  { 
    asm volatile( "1: \n\t"
		      "vld1.64 {d0-d3}, [%[source]]! \n\t" 
		      "vst1.64 {d0-d3}, [%[dest]]! \n\t"
		      "subs %[iterations],%[iterations],#1 \n\t"
		      "bne 1b \n\t"
		      : [source]"+r"(src), [dest] "+r"(dst), [iterations]"+r"(simd_iterations) 
		      : 
		      : "memory", "d0" 
		   );
    }
#else
  std::cerr << "neon_copy_bgr_to_grb(" << src << ", " << dst << ", " << size << ");  NOT IMPLEMENTED for ARMv8" << std::endl;
#endif
}

static inline void saturate(int& value, int min_val, int max_val)
{
  if (value < min_val) value = min_val;
  if (value > max_val) value = max_val;
}

inline void black_buffer_RGB(unsigned char *bufferm)
{
  for (int y = 0; y < VDMA_BUFFER_HEIGHT; ++y)
  for (int x = 0; x < VDMA_BUFFER_WIDTH; ++x)
  {
      bufferm[y * VDMA_BUFFER_WIDTH3 + x*3]   = 0;
      bufferm[y * VDMA_BUFFER_WIDTH3 + x*3+1] = 0;
      bufferm[y * VDMA_BUFFER_WIDTH3 + x*3+2] = 0;
  }
}

inline void convert_YUYV_to_RGB24(int size_x, int size_y, const uint8_t* YUYV_ptr, uint8_t* out)
{
  const int K1 = int(1.402f * (1 << 16));
  const int K2 = int(0.714f * (1 << 16));
  const int K3 = int(0.334f * (1 << 16));
  const int K4 = int(1.772f * (1 << 16));

  uint8_t* out_ptr = out;
  const int pitch = size_x * 2; // 2 bytes per one YU-YV pixel

  for (int y=0; y<size_y; ++y) 
  {
    const uint8_t* src = YUYV_ptr + pitch * y;
    for (int x=0; x<size_x*2; x+=4) 
    { // Y1 U Y2 V
      uint8_t Y1 = src[x + 0];
      uint8_t U  = src[x + 1];
      uint8_t Y2 = src[x + 2];
      uint8_t V  = src[x + 3];

      int uf = (int)U - 128;
      int vf = (int)V - 128;

      int R = Y1 + (K1*vf >> 16);
      int G = Y1 - (K2*vf >> 16) - (K3*uf >> 16);
      int B = Y1 + (K4*uf >> 16);

      saturate(R, 0, 255);
      saturate(G, 0, 255);
      saturate(B, 0, 255);

      *out_ptr++ = uint8_t(B);
      *out_ptr++ = uint8_t(G);
      *out_ptr++ = uint8_t(R);

      R = Y2 + (K1*vf >> 16);
      G = Y2 - (K2*vf >> 16) - (K3*uf >> 16);
      B = Y2 + (K4*uf >> 16);

      saturate(R, 0, 255);
      saturate(G, 0, 255);
      saturate(B, 0, 255);

      *out_ptr++ = uint8_t(B);
      *out_ptr++ = uint8_t(G);
      *out_ptr++ = uint8_t(R);
    }
  }
}

#endif /* FRAMEBUFFER_H */
