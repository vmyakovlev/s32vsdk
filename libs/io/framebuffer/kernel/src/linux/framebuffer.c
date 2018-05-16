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
#include <linux/device.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/v4l2-dv-timings.h>
#include <media/v4l2-device.h>
#include <media/v4l2-common.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-dv-timings.h>
#include <media/adv7511.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/kernel.h> 
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <uapi/linux/fb.h>
#include <asm/io.h> 
#include <linux/stat.h>
#include <ioctl_cmd.h>
#include "../include/vdma.h"

////////////////////////////////////////////////////////////////////////////////
// Module constants
////////////////////////////////////////////////////////////////////////////////

#define ADV7511_NAME "adv7511"

////////////////////////////////////////////////////////////////////////////////
// Module character device definitions
////////////////////////////////////////////////////////////////////////////////

static struct class *framebuffer_cl;
static dev_t framebuffer_dev_main;
static struct cdev framebuffer_c_dev_main;

// Framebuffer registers
unsigned int *framebuffer_register;

// Framebuffer park register
volatile unsigned int *framebuffer_park;

static int framebuffer_cached = 1;
module_param(framebuffer_cached, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(framebuffer_cached, "Flag if mmap of framebuffer is cached (1) or not (0).");

// traditional framebuffer info structures
extern struct fb_var_screeninfo gScreenInfo;
extern struct fb_fix_screeninfo gFixedInfo;

// VDMA infor structure
framebuffer_axivdma_info framebuffer_axvdma_info_struct;

// Externs for V4L2
extern struct v4l2_subdev *adv7511_subdev(struct v4l2_subdev *sd);
extern unsigned char *framebudder_axivdma_frame_buffer_base;

struct v4l2_subdev *framebuffer_ad7511_sd;
struct v4l2_ctrl *framebuffer_ad7511_hdmi_mode_ctrl;

////////////////////////////////////////////////////////////////////////////////
// Wait function with blocking cycle
////////////////////////////////////////////////////////////////////////////////
void framebuffer_wait(int value)
{
  unsigned int i;
  for (i = 0; i < value; ++i);
}

////////////////////////////////////////////////////////////////////////////////
// Main IOCTL function
////////////////////////////////////////////////////////////////////////////////



static long framebuffer_device_ioctl(struct file *filep, 
				     unsigned int cmd, 
				     unsigned long arg)
{  
  switch(cmd)
  {
    case CMD_PAINT:
    {
      // get current front buffer and compute new one
      unsigned char park = *framebuffer_park & 0x0000000F;
      unsigned int display;
      
      display = (park+1) % 2;
      
	
      // Set new one (switch buffers)
      *framebuffer_park=(*framebuffer_park & 0xFFFFFFF0)|(display & 0x0000000F);
	
      // Wait until paint is done
      while ((*framebuffer_park & 0x000F0000)>>16 != display)
	framebuffer_wait(1000);
	
      // Return new back buffer
      if (copy_to_user((unsigned char *)arg, &park, sizeof(unsigned char)) != 0)
	return -EIO;
      return 0;
    }
    case CMD_GET_DISPLAYED:
    {
      // Get current front buffer and return it
      unsigned char park = *framebuffer_park & 0x0000000F;
      if (copy_to_user((unsigned char *)arg, &park, sizeof(unsigned char)) != 0)
	return -EIO;
      return 0;
    }
    case CMD_GET_BACK:
    {
      // Get current back buffer and return it
      unsigned char park = *framebuffer_park & 0x0000000F;
      unsigned char write = (park+1) % 2;
      if (copy_to_user((unsigned char *)arg, &write,sizeof(unsigned char)) != 0)
	return -EIO;
      return 0;
    }
    case CMD_CLR_BUFFERS:
    {
      // Get current back buffer and clear it
      unsigned char park = *framebuffer_park & 0x0000000F;
      unsigned char write = (park+1) % 2;
      framebuffer_axivdma_clear_buffer(write);
      
      // Get next and also clear it
      *framebuffer_park = (*framebuffer_park & 0xFFFFFFF0)|(write & 0x0000000F);
      framebuffer_axivdma_clear_buffer(park);
    }
    case CMD_SET_HDMI:
    {
      unsigned char aux;
      
      if (copy_from_user(&aux, (unsigned char *)arg, sizeof(unsigned char)) != 0) 
	return -EIO;
      
      if (aux == 1)
	v4l2_ctrl_s_ctrl(framebuffer_ad7511_hdmi_mode_ctrl, V4L2_DV_TX_MODE_HDMI);
      else
	v4l2_ctrl_s_ctrl(framebuffer_ad7511_hdmi_mode_ctrl, V4L2_DV_TX_MODE_DVI_D);
      
      return 0;
    }
    case FBIOGET_VSCREENINFO:
    {
      copy_to_user((struct fb_var_screeninfo *)arg, &gScreenInfo, sizeof(gScreenInfo));
      return 0;
    } // FBIOGET_VSCREENINFO
    case FBIOGET_FSCREENINFO:
    {
      copy_to_user((struct fb_fix_screeninfo *)arg, &gFixedInfo, sizeof(gFixedInfo));
      return 0;
    } // FBIOGET_FSCREENINFO
      
    default:
      return -EIO;
   }
}
 
////////////////////////////////////////////////////////////////////////////////
// Memmap function for frame buffers
////////////////////////////////////////////////////////////////////////////////
static int framebuffer_mmap(struct file *filp, struct vm_area_struct *vma)
{
  unsigned long pageFrameNo;
  unsigned long size = (unsigned long)(vma->vm_end-vma->vm_start); 

  if (size>VDMA_BUFFER_SIZE*2+2048)
	  return -EINVAL;

  pageFrameNo = __phys_to_pfn(VDMA_BUFFER_BASE_ADDRESS);
  if (framebuffer_cached == 0)
  {
    //vma->vm_flags |= VM_RESERVED | VM_IO;
    vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
  }
  remap_pfn_range(vma, vma->vm_start, pageFrameNo, size, vma->vm_page_prot);   

  return 0; 
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static int framebuffer_open(struct inode *inode, struct file *filp)
{
  return 0; 
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static int framebuffer_release(struct inode *inode, struct file *filp)
{
  printk("HDMI FB Close().\n");
  return 0;
}	

////////////////////////////////////////////////////////////////////////////////
// FOPS definitions
////////////////////////////////////////////////////////////////////////////////
static const struct file_operations framebuffer_fops = {
	.owner =          THIS_MODULE,
	.unlocked_ioctl = framebuffer_device_ioctl,
	mmap: 		  framebuffer_mmap,
        open: 		  framebuffer_open,
	release: 	  framebuffer_release,
};

////////////////////////////////////////////////////////////////////////////////
// Init function - registers a char device
////////////////////////////////////////////////////////////////////////////////
static int __init framebuffer_init(void)
{
  //volatile unsigned int *vtcreg;
  if (alloc_chrdev_region(&framebuffer_dev_main, 0, 1, "framebuffer_main") < 0)
	  goto fail_alloc_region_main;

  if ((framebuffer_cl = class_create(THIS_MODULE, "framebuffer_chardrv")) 
									== NULL)
	  goto fail_class_create_main;

  if (device_create(framebuffer_cl, NULL, framebuffer_dev_main, NULL, 
							  "fb_axivdma") == NULL)
	  goto fail_device_create_main;

  cdev_init(&framebuffer_c_dev_main, &framebuffer_fops);
  
  if (cdev_add(&framebuffer_c_dev_main, framebuffer_dev_main, 1) == -1)
	  goto fail_cdev_add_main;

  
  // Get ADV 7511 device and test it
  printk("Framebuffer ADV7511 setting HDMI output.\n");
  framebuffer_ad7511_sd = adv7511_subdev(NULL);
  if (!framebuffer_ad7511_sd) {
    printk("Framebuffer ADV7511 getting V4L2 subdevice error %s.\n", 
								  ADV7511_NAME);
    goto fail_cdev_add_main;
  }
  
  // Set TX mode to HDMI
  framebuffer_ad7511_hdmi_mode_ctrl = v4l2_ctrl_find(framebuffer_ad7511_sd->ctrl_handler, V4L2_CID_DV_TX_MODE);
  //v4l2_ctrl_s_ctrl(framebuffer_ad7511_hdmi_mode_ctrl, V4L2_DV_TX_MODE_HDMI);
  
  
  framebuffer_register = ioremap(VDMA_BASE_ADDRESS, VDMA_REG_COUNT);
  framebuffer_park = &(framebuffer_register[0x28/4]);
  
  	
  /*printk("Framebuffer registers after init:\n");
  printk("=================================\n");
  for (i = 0; i < 64; ++i)
    printk("  %02X: %08X\n", i, framebuffer_register[i]);*/
  
  framebuffer_axivdma_init(&framebuffer_axvdma_info_struct, 
			   VDMA_BUFFER_WIDTH*VDMA_BUFFER_ELEMENT_WIDTH, 
			   VDMA_BUFFER_HEIGHT, 
			   VDMA_BUFFER_PIXEL_WIDTH);
  framebuffer_axivdma_clear_buffer(0);
  framebuffer_axivdma_clear_buffer(1);
  framebuffer_axivdma_out_start(&framebuffer_axvdma_info_struct, 
				VDMA_BUFFER_BASE_ADDRESS+VDMA_BUFFER_0_OFFSET, 
				VDMA_BUFFER_BASE_ADDRESS+VDMA_BUFFER_1_OFFSET, 
				0);
    
  /*vtcreg = ioremap(0x40010000, 0x13B);
  
  vtcreg[0] = 0xC0000000;
  while ((vtcreg[0] & 0xC0000000) != 0);
  
  vtcreg[0] = 0x5;//0x3F5EF05;*/
  
  
  printk("Framebuffer ADV7511 init done.\n");
  printk("framebuffer_cached == %d.\n", framebuffer_cached);

  return 0;
	
  fail_cdev_add_main:
  device_destroy(framebuffer_cl, 0);
  fail_device_create_main:
  class_destroy(framebuffer_cl);
  fail_class_create_main:
  unregister_chrdev_region(0, 1);
  fail_alloc_region_main:
  
  return -1;
	
}
module_init(framebuffer_init);

////////////////////////////////////////////////////////////////////////////////
// Destructor function - frees all resources
////////////////////////////////////////////////////////////////////////////////
static void __exit cma_exit(void)
{
  device_destroy(framebuffer_cl, 0);
  unregister_chrdev_region(0, 1);
  class_destroy(framebuffer_cl);
}

module_exit(cma_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rostislav Hulik <rostislav.hulik <at> freescale.com>");
MODULE_DESCRIPTION("");
MODULE_ALIAS("framebuffer_adv7511_vdma");