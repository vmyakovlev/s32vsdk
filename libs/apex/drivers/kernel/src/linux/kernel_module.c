/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#include <linux/module.h>
#include <linux/interrupt.h>  
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>       
#include <linux/fs.h>
#include <linux/spinlock.h>
#include <asm/uaccess.h>    // for get_user and put_user

#define REG32 uint32_t
#include <apex_642cl_seq_map.h>
#include <apex_642cl_motion_comp_map.h>
#include <apex_642cl_mcdma_map.h>

#include "apex_ioctl.h"
#include "apex_defines.h"
#include "apex_interrupt.h" //for APEX_INTERRUPTS struct

#if 0
  #define DEBUG_OUT(...) printk( __VA_ARGS__)
#else
  #define DEBUG_OUT(...)
#endif 

///////////////////////////////////////////////////////////////////////////////////
// Module info book-keeping
///////////////////////////////////////////////////////////////////////////////////
static int8_t apex_loaded = 0;
static int32_t apex_pid_signal[2] = {-1};
static uint32_t apex_irqs[2] = {0};
static int8_t* apex_names[2] = {"APEX0",
                                "APEX1"};

APEX_642CL_MCDMA_DCR *apex_mcdma_regs[2];
APEX_642CL_SEQ_DCR *apex_seq_regs[2];
APEX_642CL_MOTION_COMP_DCR *apex_mce_regs[2];

///////////////////////////////////////////////////////////////////////////////////
// Module int8_tacter device definitions
///////////////////////////////////////////////////////////////////////////////////
static struct class *apex_dev_cl;
static dev_t apex_dev_main;
static struct cdev apex_c_dev_main;

int8_t * apex_interrupt_userspace_cbuffer = 0;
int64_t apex_interrupt_userspace_cbuffer_size = 0;
int64_t apex_interrupt_userspace_cbuffer_index = 0;

spinlock_t apex_interrupt_lock;
///////////////////////////////////////////////////////////////////////////////////
// Main IOCTL function
///////////////////////////////////////////////////////////////////////////////////
static long apex_device_ioctl(struct file *filep, uint32_t cmd, unsigned long arg)
{
  DEBUG_OUT("APEX apex_device_ioctl.\n");
  
  switch (cmd) {
    // Function sets calling PID to specific apex number (apex number is the argument)
    case IOCTL_APEX_PID_SET:
    {
      int8_t apex_number;
      if (copy_from_user(&apex_number, (int8_t *)arg, sizeof(int8_t)) != 0)
      {
        DEBUG_OUT("Copy from user failed.\n");
        return -EIO;
      }
      apex_pid_signal[apex_number] = current->pid;
      DEBUG_OUT("Setting the pid for apex %d to %d\n", apex_number, apex_pid_signal[apex_number]);
    }
    break;
    
    case IOCTL_APEX_INTERRUPT_BUFFER_SET:
    {
      IOCTL_APEX_INTERRUPT_BUFFER_SET_TYPE info;
      if (copy_from_user(&info, (IOCTL_APEX_INTERRUPT_BUFFER_SET_TYPE *)arg, sizeof(IOCTL_APEX_INTERRUPT_BUFFER_SET_TYPE)) != 0)
      {
        DEBUG_OUT("Copy from user failed.\n");
        return -EIO;
      }
      apex_interrupt_userspace_cbuffer_size = info.size;
      apex_interrupt_userspace_cbuffer = (int8_t *)ioremap(info.start_pointer, info.size);
      DEBUG_OUT("Setting circular buffer to %X@%X\n", info.size, info.start_pointer);
    }
    break;
      
    default:
      break;
  }
      
  return 0;
}

///////////////////////////////////////////////////////////////////////////////////
// Memmap function
///////////////////////////////////////////////////////////////////////////////////
static int32_t apex_mmap(struct file *filp, struct vm_area_struct *vma)
{  
  int32_t retval = 0;
  DEBUG_OUT("APEX apex_mmap.\n");
  // vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
  if (remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, vma->vm_end - vma->vm_start, vma->vm_page_prot))
    retval = -EAGAIN;

  return retval;
}

///////////////////////////////////////////////////////////////////////////////////
// Release function called on file close (app close/abort)
///////////////////////////////////////////////////////////////////////////////////
static int apex_release(struct inode *inodep, struct file *filep)
{
  DEBUG_OUT("APEX apex_release.\n");
  return 0;
}

///////////////////////////////////////////////////////////////////////////////////
// FOPS definitions
///////////////////////////////////////////////////////////////////////////////////
static const struct file_operations apex_fops = {
  .owner =    THIS_MODULE,
  .unlocked_ioctl = apex_device_ioctl,
  .mmap= 		  apex_mmap,
  .release = apex_release
};

///////////////////////////////////////////////////////////////////////////////////
// Send signal to userspace
///////////////////////////////////////////////////////////////////////////////////

static int32_t apex_irq_send_to_pid(pid_t aPid, int8_t irq, int32_t aValue)
{
  int32_t lRet = IRQ_HANDLED;
  struct siginfo lInfo;
  struct pid *lpPidStruct;
  struct task_struct *lpTask;
    
  int32_t signalnum = 0;
  
  // Set event signal according to APEX number (must be set via IOCTL)
  if (irq == apex_irqs[0]) signalnum = ACF_EVENT_SIGNAL_0;
  else signalnum = ACF_EVENT_SIGNAL_1;
  
  // Send the signal to user space
  memset(&lInfo, 0, sizeof(struct siginfo));
  lInfo.si_signo = signalnum;
  lInfo.si_code  = SI_QUEUE;
  lInfo.si_int   = aValue;
  rcu_read_lock();
  lpPidStruct    = find_get_pid(aPid);
  lpTask         = pid_task(lpPidStruct,PIDTYPE_PID);
  if(lpTask == NULL)
  {
    DEBUG_OUT("No such pid %u. Signal not sent.\n", (unsigned) aPid);
    rcu_read_unlock();
    lRet = -1;
  } // If no such pidRegisterred
  else
  {
    rcu_read_unlock();
    DEBUG_OUT("   Sending the signal to user space.\n");
    lRet = send_sig_info(signalnum, &lInfo, lpTask);    //send the signal
    if (lRet < 0) {
      DEBUG_OUT("Error sending signal to user space.\n");
    } // If sending signal failed
  }
  return lRet;
} 

///////////////////////////////////////////////////////////////////////////////////
// IRQ handler - to be called when IRQ is triggered
///////////////////////////////////////////////////////////////////////////////////
static irqreturn_t apex_irq_handler_0(int32_t aIrq, void *apDev_id)
{
  int32_t ret = IRQ_HANDLED;
  APEX_INTERRUPTS *aux;
  DEBUG_OUT("0: APEX IRQ CATCH - %d.\n", aIrq);
  
  spin_lock(&apex_interrupt_lock);
  aux = &(((APEX_INTERRUPTS *)apex_interrupt_userspace_cbuffer)[apex_interrupt_userspace_cbuffer_index]);
  
  aux->seq = apex_seq_regs[0]->Isr.as32BitValue & ~apex_seq_regs[0]->Imr.as32BitValue;
  aux->mcdma = apex_mcdma_regs[0]->Isr.as32BitValue & ~apex_mcdma_regs[0]->Imr.as32BitValue;
  aux->mce = apex_mce_regs[0]->Isr.as32BitValue & ~apex_mce_regs[0]->Imr.as32BitValue;
  DEBUG_OUT("0: INSIDE SPINL APEX READ:  %x %x %x.\n", aux->seq, aux->mcdma, aux->mce);
  
  apex_seq_regs[0]->Icr.as32BitValue = aux->seq;
  apex_mcdma_regs[0]->Icr.as32BitValue = aux->mcdma;
  apex_mce_regs[0]->Icr.as32BitValue = aux->mce;  
  
  apex_irq_send_to_pid(apex_pid_signal[0], aIrq, apex_interrupt_userspace_cbuffer_index);
  apex_interrupt_userspace_cbuffer_index = (apex_interrupt_userspace_cbuffer_index + 1) % apex_interrupt_userspace_cbuffer_size;
  spin_unlock(&apex_interrupt_lock);  
  
  DEBUG_OUT("0: END OF ISR KERNEL SPACE.\n");
  return ret;
}

static irqreturn_t apex_irq_handler_1(int32_t aIrq, void *apDev_id)
{
  int32_t ret = IRQ_HANDLED;
  APEX_INTERRUPTS *aux;
  
  DEBUG_OUT("1: APEX IRQ CATCH - %d.\n", aIrq);
  
  spin_lock(&apex_interrupt_lock);
  aux = &(((APEX_INTERRUPTS *)apex_interrupt_userspace_cbuffer)[apex_interrupt_userspace_cbuffer_index]);
  
  aux->seq = apex_seq_regs[1]->Isr.as32BitValue & ~apex_seq_regs[1]->Imr.as32BitValue;
  aux->mcdma = apex_mcdma_regs[1]->Isr.as32BitValue & ~apex_mcdma_regs[1]->Imr.as32BitValue;
  aux->mce = apex_mce_regs[1]->Isr.as32BitValue & ~apex_mce_regs[1]->Imr.as32BitValue;
  DEBUG_OUT("1: INSIDE SPINL APEX READ:  %x %x %x.\n", aux->seq, aux->mcdma, aux->mce);
  
  apex_seq_regs[1]->Icr.as32BitValue = aux->seq;
  apex_mcdma_regs[1]->Icr.as32BitValue = aux->mcdma;
  apex_mce_regs[1]->Icr.as32BitValue = aux->mce;
  
  apex_irq_send_to_pid(apex_pid_signal[1], aIrq, apex_interrupt_userspace_cbuffer_index);
  apex_interrupt_userspace_cbuffer_index = (apex_interrupt_userspace_cbuffer_index + 1) % apex_interrupt_userspace_cbuffer_size;
  spin_unlock(&apex_interrupt_lock);  
  DEBUG_OUT("1: END OF ISR KERNEL SPACE.\n");
    
  return ret;
}

///////////////////////////////////////////////////////////////////////////////////
// Probe - called when insmod for each node
///////////////////////////////////////////////////////////////////////////////////
int apex_probe(struct platform_device *pdev)
{
  __be32 *apex_id;
  int32_t apex_prop_len;
  int32_t apex_id_start;

  ////////////////////////////////////////////////////////////
  // Get ID - APEX id (0, 1)
  apex_id = (__be32 *)of_get_property(pdev->dev.of_node, "id", &apex_prop_len);
  apex_id_start = ((int8_t *)apex_id)[3];
  if (!apex_id || apex_prop_len <= 0 || apex_id_start >= 2)
  {
    printk("APEX ID cannot be read or isn't in range 0-1. Pleaser review id attribute.\n");
    goto apex_fail_cdev_add_main_cached;
  }
  
  ////////////////////////////////////////////////////////////
  // Chardevice creation for mapping (only for the first time)
  if (apex_loaded == 0)
  {
    if (alloc_chrdev_region(&apex_dev_main, 0, 1, "apex_main_c") < 0)
        goto apex_fail_alloc_region_main_cached;

    if ((apex_dev_cl = class_create(THIS_MODULE, "apex_int8_tdrv_c")) == NULL)
        goto apex_fail_class_create_main_cached;

    if (device_create(apex_dev_cl, NULL, apex_dev_main, NULL, "apex") == NULL)
        goto apex_fail_device_create_main_cached;

    cdev_init(&apex_c_dev_main, &apex_fops);
    if (cdev_add(&apex_c_dev_main, apex_dev_main, 1) == -1)
        goto apex_fail_cdev_add_main_cached;
    }
  
  ////////////////////////////////////////////////////////////
  // IRQ setting
  apex_irqs[apex_id_start] = irq_of_parse_and_map(pdev->dev.of_node, 0);
  
  if (apex_id_start == 0)
  {
    if (request_irq(apex_irqs[apex_id_start], &apex_irq_handler_0, 0, apex_names[apex_id_start], NULL))
    {
      DEBUG_OUT("Failed to request IRQ for APEX.\n");
    }
  }
  else
  {
    if (request_irq(apex_irqs[apex_id_start], &apex_irq_handler_1, 0, apex_names[apex_id_start], NULL))
    {
      DEBUG_OUT("Failed to request IRQ for APEX.\n");
    }
  }

  apex_mcdma_regs[0] = (APEX_642CL_MCDMA_DCR *)ioremap(0x74100000, 0x18);
  apex_seq_regs[0] = (APEX_642CL_SEQ_DCR *)ioremap(0x74130000, 0xc);
  apex_mce_regs[0] = (APEX_642CL_MOTION_COMP_DCR *)ioremap(0x74110000, 0x10);
  
  apex_mcdma_regs[1] = (APEX_642CL_MCDMA_DCR *)ioremap(0x78100000, 0x18);
  apex_seq_regs[1] = (APEX_642CL_SEQ_DCR *)ioremap(0x78130000, 0xc);
  apex_mce_regs[1] = (APEX_642CL_MOTION_COMP_DCR *)ioremap(0x78110000, 0x10);
  
  ////////////////////////////////////////////////////////////
  // All ok
  apex_interrupt_userspace_cbuffer_index = 0;
  ++apex_loaded;
  spin_lock_init(&apex_interrupt_lock);
  
  DEBUG_OUT("APEX kernel module - IRQ %d, ID %d, %d devices loaded.\n", apex_irqs[apex_id_start], apex_id_start, apex_loaded);
  
  return 0;
  
  apex_fail_cdev_add_main_cached:
    device_destroy(apex_dev_cl, 0);
  apex_fail_device_create_main_cached:
    class_destroy(apex_dev_cl);
  apex_fail_class_create_main_cached:
    unregister_chrdev_region(0, 1);
  apex_fail_alloc_region_main_cached:
    return  -1;
}
   
///////////////////////////////////////////////////////////////////////////////////
// Destructor function - frees all resources
///////////////////////////////////////////////////////////////////////////////////
int apex_remove(struct platform_device *pdev)
{
  device_destroy(apex_dev_cl, 0);
  unregister_chrdev_region(0, 1);
  class_destroy(apex_dev_cl);
  return 0;
}


static const struct of_device_id apex_dt_ids[] = {
  {
    .compatible = "nxp,s32v234-apex",
  },
  {}
};


static struct platform_driver apex_driver = {
  .driver = {
    .name = "APEX_DRIVER",
    .owner = THIS_MODULE,
    .of_match_table = apex_dt_ids
  },
  .probe = apex_probe,
  .remove = apex_remove,
};

module_platform_driver(apex_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rostislav Hulik <rostislav.hulik <at> nxp.com>");
MODULE_DESCRIPTION("APEX kernel driver part for interrupt servicing.");
MODULE_ALIAS("apex");
