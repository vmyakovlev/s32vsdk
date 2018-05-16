/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2014 Freescale Semiconductor;
* All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/
#include "../include/vdma.h"

#include <asm/io.h>
#include <linux/module.h>
#include <uapi/linux/fb.h>

// Framebuffer base and pointers to start
unsigned char *framebuffer_axivdma_frame_buffer_base;
unsigned char *framebuffer_axivdma_frame_buffer[2];

// traditional framebuffer info structures
struct fb_var_screeninfo gScreenInfo;
struct fb_fix_screeninfo gFixedInfo;

// VDMA init flag
int framebuffer_axivdma_init_flag = VDMA_UNINITIALIZED;

////////////////////////////////////////////////////////////////////////////////
// VDMA Initialization (virtual base allocation...)
// @param info Pointer to framebuffer init structure
// @param width Frame buffer width
// @param height Frame buffer Height
// @param pixelLength Pixel length
////////////////////////////////////////////////////////////////////////////////
int framebuffer_axivdma_init(framebuffer_axivdma_info *info, 
			     int width, 
			     int height, 
			     int pixelLength)
{
    info->vdmaPhysBaseAddr=VDMA_BASE_ADDRESS;
    info->vdmaVirtBaseAddr = ioremap(VDMA_BASE_ADDRESS, VDMA_REG_COUNT*4);
    info->width=width;
    info->height=height;
    info->pixelLength=pixelLength;
    info->fbLength=pixelLength*width*height;
    
    framebuffer_axivdma_frame_buffer_base = 
			  ioremap(VDMA_BUFFER_BASE_ADDRESS, VDMA_BUFFER_SIZE*2);
    
    if(!info->vdmaVirtBaseAddr || !framebuffer_axivdma_frame_buffer_base)
    {
     printk("vdmaVirtBaseAddr mapping for absolute memory access failed.\n");
     return -1;
    }
    
    framebuffer_axivdma_frame_buffer[0] = 
		   framebuffer_axivdma_frame_buffer_base + VDMA_BUFFER_0_OFFSET;
    framebuffer_axivdma_frame_buffer[1] = 
		   framebuffer_axivdma_frame_buffer_base + VDMA_BUFFER_1_OFFSET;
    framebuffer_axivdma_init_flag = VDMA_INITIALIZED;
    
    // fill in the traditional framebuffer info structures
    strcpy(gFixedInfo.id, "apex_hdmi");
    gFixedInfo.smem_len = info->fbLength * 2;
    gFixedInfo.line_length = pixelLength * width;
    
    gScreenInfo.xres = 1920; 
    gScreenInfo.yres = height;
    gScreenInfo.xres_virtual = 1920;
    gScreenInfo.yres_virtual = height;
    gScreenInfo.xoffset = 0;
    gScreenInfo.yoffset = 0;
    gScreenInfo.bits_per_pixel = 3 * 8;
    gScreenInfo.grayscale = 0;
    gScreenInfo.red.offset = 16;
    gScreenInfo.green.offset = 8;
    gScreenInfo.blue.offset = 0;
    gScreenInfo.transp.offset = 0;
    
    return 0;
}
 
////////////////////////////////////////////////////////////////////////////////
// Free all resources
// @param info Pointer to framebuffer init structure
////////////////////////////////////////////////////////////////////////////////
void framebuffer_axivdma_uninit(framebuffer_axivdma_info *info)
{
    iounmap(info->vdmaVirtBaseAddr);
    framebuffer_axivdma_frame_buffer[0] = 0;
    framebuffer_axivdma_frame_buffer[1] = 0;
    iounmap(framebuffer_axivdma_frame_buffer_base);
    
    framebuffer_axivdma_init_flag = VDMA_UNINITIALIZED;
}
 
////////////////////////////////////////////////////////////////////////////////
// Returns a register value
// @param info Pointer to framebuffer init structure
// @param num Register index
////////////////////////////////////////////////////////////////////////////////
unsigned int framebuffer_axivdma_get(framebuffer_axivdma_info *info, int num)
{
    if(num>=0 && num<VDMA_REG_COUNT)
    {
        return info->vdmaVirtBaseAddr[num];
    }
    return 0;
}
 
////////////////////////////////////////////////////////////////////////////////
// Sets a register value
// @param info Pointer to framebuffer init structure
// @param num * Register index
// @param val Value to be set
////////////////////////////////////////////////////////////////////////////////
void framebuffer_axivdma_set(framebuffer_axivdma_info *info, 
			     int num, 
			     unsigned int val)
{
    if(num>=0 && num<VDMA_REG_COUNT)
    {
        info->vdmaVirtBaseAddr[num]=val;
    }
}
 
////////////////////////////////////////////////////////////////////////////////
// Start and set a vdma mm2s transfer
// @param info Pointer to framebuffer init structure
// @param adr1 Frame buffer 0 address
// @param adr1 Frame buffer 1 address
// @param circular Is transfer circular?
////////////////////////////////////////////////////////////////////////////////
void framebuffer_axivdma_out_start(framebuffer_axivdma_info *info, 
				   unsigned int adr1, 
				   unsigned int adr2, 
				   int circular)
{
  // MM2S_DMACR: reset
  framebuffer_axivdma_set(info, 0x00, circular==1?4+2:4);  	
 
  // wait for reset end
  while((framebuffer_axivdma_get(info, 0x00)&4)==4); 		
 
  // S2MM_DMASR: remove errors
  framebuffer_axivdma_set(info, 0x01, 0xffffffff);  	
  // MM2S_FRMSTORE: 1 (0x18/4)
  framebuffer_axivdma_set(info, 0x06, 1);  		
  // S2MM_DMACR: RS
  framebuffer_axivdma_set(info, 0x00, circular==1?2+1:1); 
    
  // wait for run
  while((framebuffer_axivdma_get(info, 0x00)&1)==0 || 
        (framebuffer_axivdma_get(info, 0x04)&1)==1);
     
  // this alters s2mm park ptr (0x28/4)
  framebuffer_axivdma_set(info, 0x0A, 0); 
  //adr1 (5C/4)
  framebuffer_axivdma_set(info, 0x17, adr1); 
  //adr1
  framebuffer_axivdma_set(info, 0x18, adr2); 
  // stride length in bytes (0x58/4)
  framebuffer_axivdma_set(info, 0x16, info->pixelLength*info->width);
  // length in bytes
  framebuffer_axivdma_set(info, 0x15, info->pixelLength*info->width);
  // height and start
  framebuffer_axivdma_set(info, 0x14, info->height);  		
}
 
////////////////////////////////////////////////////////////////////////////////
// Start and set a vdma s2mm transfer
// @param info Pointer to framebuffer init structure
// @param adr1 Frame buffer 0 address
// @param adr1 Frame buffer 1 address
////////////////////////////////////////////////////////////////////////////////
void framebuffer_axivdma_start(framebuffer_axivdma_info *info, 
			       unsigned int adr1, 
			       unsigned int adr2)
{
  // S2MM_DMACR: sof=tuser, reset
  framebuffer_axivdma_set(info, 0x0C, 64+4);  
 
  // wait for reset end
  while((framebuffer_axivdma_get(info, 0x0C)&4)==4); 		
 
  // S2MM_DMASR: remove errors
  framebuffer_axivdma_set(info, 0x0D, 0xffffffff);  		
  // S2MM_DMACR: sof=tuser, RS
  framebuffer_axivdma_set(info, 0x0C, 64+1);  			

  // wait for run
  while((framebuffer_axivdma_get(info, 0x0C)&1)==0 || 
        (framebuffer_axivdma_get(info, 0x0D)&1)==1);
 
  //adr1
  framebuffer_axivdma_set(info, 0x2B, adr1); 
  //adr1   
  framebuffer_axivdma_set(info, 0x2C, adr2); 			
  framebuffer_axivdma_set(info, 0x0A, 0);
  // stride length in bytes
  framebuffer_axivdma_set(info, 0x2A, info->pixelLength*info->width);
  // length in bytes
  framebuffer_axivdma_set(info, 0x29, info->pixelLength*info->width);
  // height and start
  framebuffer_axivdma_set(info, 0x28, info->height);  
}

////////////////////////////////////////////////////////////////////////////////
// Is VDMA running?
// @param info Pointer to framebuffer init structure
////////////////////////////////////////////////////////////////////////////////
int framebuffer_axivdma_is_running(framebuffer_axivdma_info *info)
{
    return (framebuffer_axivdma_get(info, 0x0D)&1)==1;
}

////////////////////////////////////////////////////////////////////////////////
// Is VDMA transfer done?
// @param info Pointer to framebuffer init structure
////////////////////////////////////////////////////////////////////////////////
int framebuffer_axivdma_is_done(framebuffer_axivdma_info *info)
{
    return (framebuffer_axivdma_get(info, 0x0D)&0x01000)!=0;
}

////////////////////////////////////////////////////////////////////////////////
// True if axivfma is initialized
////////////////////////////////////////////////////////////////////////////////
int framebuffer_axivdma_is_init()
{
    return framebuffer_axivdma_init_flag;
}

////////////////////////////////////////////////////////////////////////////////
// Function clears (zeroes) a buffer specified with i
// @param i Buffer specified
////////////////////////////////////////////////////////////////////////////////
void framebuffer_axivdma_clear_buffer(int i)
{
  memset(framebuffer_axivdma_frame_buffer[i], 0x40, VDMA_BUFFER_SIZE);
}