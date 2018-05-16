#include <neon.h>

namespace neon {
	
void neon_memcpy(void* dst, void* src, long size)
{
  long simd_pixels = size & ~15; 
  long simd_iterations = simd_pixels / 64;
  long simd_sync = 20;
  
  char *dst_local = (char *)dst;
  char *src_local = (char *)src;
  __asm volatile( "neon_memcpy_loop: \n\t"
		      "LD1 {V0.16B, V1.16B, V2.16B, V3.16B}, [%[src_local]], #64	\n\t"
			  "ST1 {V0.16B, V1.16B, V2.16B, V3.16B}, [%[dst_local]], #64	\n\t"
			  
			  "subs %[sync],%[sync],#1 \n\t"
			  "bne neon_memcpy_next \n\t"
			  "mov %[sync], #20 \n\t"
			  "neon_memcpy_next: \n\t"
			  "subs %[iterations],%[iterations],#1 \n\t"
			  "bne neon_memcpy_loop \n\t"
		      
		      : [src_local]"+r"(src_local), [dst_local] "+r"(dst_local), [iterations]"+r"(simd_iterations), [sync]"+r"(simd_sync)
		      : 
		   );
}

void neon_memcpy_1080to720(void* dst, void* src)
{
  char *dst_local = (char *)dst;
  char *src_local = (char *)src;
  long line_size = (1280 * 3) / 48;
  long line_iterations = line_size;
  long height_iterations = 720;
  long stride_src = 640 * 3;
 
    __asm volatile( "1: \n\t"
		      "LD3 {V0.16B, V1.16B, V2.16B}, [%[src_local]], #48	\n\t"
			  "ST3 {V0.16B, V1.16B, V2.16B}, [%[dst_local]], #48	\n\t"
			  "subs %[line_iterations],%[line_iterations],#1 \n\t"
		      "bne 1b \n\t"
			  
			  "mov %[line_iterations], %[line_size] \n\t"
			  "add %[src_local],%[src_local],%[stride_src] \n\t"
			  
			  "subs %[height_iterations],%[height_iterations],#1 \n\t"
			  "bne 1b \n\t"
			  
			  
		      : [src_local]"+r"(src_local), 
			    [dst_local] "+r"(dst_local),  
				[line_iterations]"+r"(line_iterations),
				[height_iterations]"+r"(height_iterations),
				[line_size]"+r"(line_size),
				[stride_src]"+r"(stride_src)		
		      : 
		   );
    
}

void neon_memcpy_rotate_720to1080(void* dst, void* src)
{
  char *dst_local = (char *)dst + (180 * 1920 * 3) + 320 * 3;
  char *src_local = (char *)src + 1280*720*3 - 24;
  long line_size = (1280 * 3) / 24;
  long line_iterations = line_size;
  long height_iterations = 720;
  long stride_dst = 640 * 3;
 
    __asm volatile( "1: \n\t"
		      "LD3 {V0.8B, V1.8B, V2.8B}, [%[src_local]]	\n\t"
			  "subs %[src_local],%[src_local],#24				\n\t"
			  "ST3 {V0.B, V1.B, V2.B}[7], [%[dst_local]], #3		\n\t"
			  "ST3 {V0.B, V1.B, V2.B}[6], [%[dst_local]], #3		\n\t"
			  "ST3 {V0.B, V1.B, V2.B}[5], [%[dst_local]], #3		\n\t"
			  "ST3 {V0.B, V1.B, V2.B}[4], [%[dst_local]], #3		\n\t"
			  "ST3 {V0.B, V1.B, V2.B}[3], [%[dst_local]], #3		\n\t"
			  "ST3 {V0.B, V1.B, V2.B}[2], [%[dst_local]], #3		\n\t"
			  "ST3 {V0.B, V1.B, V2.B}[1], [%[dst_local]], #3		\n\t"
			  "ST3 {V0.B, V1.B, V2.B}[0], [%[dst_local]], #3		\n\t"
			  
			  "subs %[line_iterations],%[line_iterations],#1 \n\t"
		      "bne 1b \n\t"
			  
			  "mov %[line_iterations], %[line_size] \n\t"
			  "add %[dst_local],%[dst_local],%[stride_dst] \n\t"
			  
			  "subs %[height_iterations],%[height_iterations],#1 \n\t"
			  "bne 1b \n\t"
			  
			  
		      : [src_local]"+r"(src_local), 
			    [dst_local] "+r"(dst_local),  
				[line_iterations]"+r"(line_iterations),
				[height_iterations]"+r"(height_iterations),
				[line_size]"+r"(line_size),
				[stride_dst]"+r"(stride_dst)		
		      : 
		   );
}

void neon_memcpy_720to1080(void* dst, void* src)
{
  char *dst_local = (char *)dst + (180 * 1920 * 3) + 320 * 3;
  char *src_local = (char *)src;
  long line_size = (1280 * 3) / 24;
  long line_iterations = line_size;
  long height_iterations = 720;
  long stride_dst = 640 * 3;
 
    __asm volatile( "1: \n\t"
		      "LD3 {V0.8B, V1.8B, V2.8B}, [%[src_local]], #24	\n\t"
			  "ST3 {V0.B, V1.B, V2.B}[0], [%[dst_local]], #3		\n\t"
			  "ST3 {V0.B, V1.B, V2.B}[1], [%[dst_local]], #3		\n\t"
			  "ST3 {V0.B, V1.B, V2.B}[2], [%[dst_local]], #3		\n\t"
			  "ST3 {V0.B, V1.B, V2.B}[3], [%[dst_local]], #3		\n\t"
			  "ST3 {V0.B, V1.B, V2.B}[4], [%[dst_local]], #3		\n\t"
			  "ST3 {V0.B, V1.B, V2.B}[5], [%[dst_local]], #3		\n\t"
			  "ST3 {V0.B, V1.B, V2.B}[6], [%[dst_local]], #3		\n\t"
			  "ST3 {V0.B, V1.B, V2.B}[7], [%[dst_local]], #3		\n\t"
			  
			  "subs %[line_iterations],%[line_iterations],#1 \n\t"
		      "bne 1b \n\t"
			  
			  "mov %[line_iterations], %[line_size] \n\t"
			  "add %[dst_local],%[dst_local],%[stride_dst] \n\t"
			  
			  "subs %[height_iterations],%[height_iterations],#1 \n\t"
			  "bne 1b \n\t"
			  
			  
		      : [src_local]"+r"(src_local), 
			    [dst_local] "+r"(dst_local),  
				[line_iterations]"+r"(line_iterations),
				[height_iterations]"+r"(height_iterations),
				[line_size]"+r"(line_size),
				[stride_dst]"+r"(stride_dst)		
		      : 
		   );
}

}