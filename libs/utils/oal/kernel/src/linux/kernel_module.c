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
#include <asm/uaccess.h>
#include <asm/cacheflush.h>
#include <asm/tlbflush.h>
#include <asm/io.h>    
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/of_device.h>

#include <linux/ioctl_cmd.h>
#include <oal_debug_out.h>
#include <oal_allocation_kernel.h>
#include <oal_cma_list.h>

///////////////////////////////////////////////////////////////////////////////////
// Module devices bookkeeping
///////////////////////////////////////////////////////////////////////////////////
uint8_t  oal_allocator_devices = 0;
uint8_t  oal_devices_loaded = 0;
uint8_t  oal_devices_autobalanced = 0;
uint32_t oal_device_alignment[OAL_MAX_ALLOCATOR_NUM] = {0};

extern void flush_dcache_range(void *pMemory, uint32_t size);
extern void invalidate_dcache_range(void *pMemory, uint32_t size);
extern void flush_and_invalidate_dcache_range(void *pMemory, uint32_t size);

///////////////////////////////////////////////////////////////////////////////////
// Module int8_tacter device definitions
///////////////////////////////////////////////////////////////////////////////////
static struct class *oal_mem_cl_cached;
static dev_t oal_mem_dev_main_cached;
static struct cdev oal_mem_c_dev_main_cached;

static struct class *oal_mem_cl_noncached;
static dev_t oal_mem_dev_main_noncached;
static struct cdev oal_mem_c_dev_main_noncached;

///////////////////////////////////////////////////////////////////////////////////
// Semaphore structs
///////////////////////////////////////////////////////////////////////////////////
struct semaphore oal_short_term_sema;
struct semaphore oal_long_term_sema;

static int oal_lock_pid_long  = -1;
static int oal_lock_pid_short = -1;
#define OBTAIN_SHORT_LOCK() {                                               \
                              oal_printk("OAL: SHORT LOCK\n");              \
                              down(&oal_short_term_sema);                   \
                              oal_lock_pid_short = task_pid_nr(current);    \
                            }
                            
#define RELEASE_SHORT_LOCK() {                                              \
                               oal_printk("OAL: SHORT UNLOCK\n");           \
                               up(&oal_short_term_sema);                    \
                             }
                             
#define OBTAIN_LONG_LOCK() {                                                \
                              oal_printk("OAL: LONG LOCK\n");               \
                              down(&oal_long_term_sema);                    \
                              oal_lock_pid_long = task_pid_nr(current);     \
                            }
                            
#define RELEASE_LONG_LOCK() {                                               \
                               oal_printk("OAL: LONG UNLOCK\n");            \
                               up(&oal_long_term_sema);                     \
                             }

                             
///////////////////////////////////////////////////////////////////////////////////
// Main IOCTL function
///////////////////////////////////////////////////////////////////////////////////
static long oal_mem_device_ioctl(struct file *filep, uint32_t cmd, unsigned long arg)
{
  int64_t retval = 0;
  switch (cmd)
  {
    ///////////////////////////////////////////////////////////////////////////
    // Long Mutex lock
    case CMD_MUTEX:
    {     
      OBTAIN_LONG_LOCK();
      break;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Long Mutex unlock
    case CMD_UNMUTEX:
    {
      RELEASE_LONG_LOCK();
      break;
    }
      
    ///////////////////////////////////////////////////////////////////////////
    // Allocate a new contiguous region
    case CMD_ALLOC:
    {
      CMD_ALLOC_TYPE aux;
      
      OBTAIN_SHORT_LOCK();
      
      oal_printk("OAL: ALLOCATE\n");
       
      if (copy_from_user(&aux, (CMD_ALLOC_TYPE *)arg, sizeof(CMD_ALLOC_TYPE)) == 0)
      {
        aux.ret_phys_pointer = oal_alloc(aux.size, aux.align, aux.chunk_id);
        if (copy_to_user((CMD_ALLOC_TYPE *)arg, &aux, sizeof(CMD_ALLOC_TYPE)) != 0)
          retval = -EIO;
      }
      else
      {
        retval = -EIO;
      }
      
      RELEASE_SHORT_LOCK();
      break;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Free unused contiguous region
    case CMD_FREE:
    {
      uint64_t handle_pointer;
    
      OBTAIN_SHORT_LOCK();
      
      oal_printk("OAL: FREE\n");
        
      if (copy_from_user(&handle_pointer, (uint64_t *)arg, sizeof(uint64_t)) == 0)
      {
        handle_pointer = oal_free_phys(handle_pointer);
        if (copy_to_user((uint64_t *)arg, &handle_pointer, sizeof(uint64_t)) != 0)
          retval = -EIO;
      }
      else
      {
        retval = -EIO;
      }
      
      RELEASE_SHORT_LOCK();
      break;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Get size of a region
    case CMD_SIZE:
    {
      uint64_t handle_pointer;
      uint64_t size;
    
      OBTAIN_SHORT_LOCK();
      
      oal_printk("OAL: ASK SIZE\n");
      
      if (copy_from_user(&handle_pointer, (uint64_t *)arg, sizeof(uint64_t)) == 0)
      {
        size = oal_getsize(handle_pointer);
        if (copy_to_user((uint64_t *)arg, &size, sizeof(uint64_t)) != 0)
          retval = -EIO;
      }
      else
      {
        retval = -EIO;
      }
    
      RELEASE_SHORT_LOCK();
      break;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Return a mapping for this process
    case CMD_BUFFER:
    {
      CMD_BUFFER_TYPE aux;
      
      OBTAIN_SHORT_LOCK();
      
      oal_printk("OAL: GET MAPPING\n");
      
      if (copy_from_user(&aux, (CMD_BUFFER_TYPE *)arg, sizeof(CMD_BUFFER_TYPE)) == 0)
      {
        aux.ret_virt_pointer = oal_getbuffer(aux.handle_pointer, aux.type);
        if (copy_to_user((CMD_BUFFER_TYPE *)arg, &aux, sizeof(CMD_BUFFER_TYPE)) != 0)
          retval = -EIO;
      }
      else
      {
        retval = -EIO;
      }
      
      RELEASE_SHORT_LOCK();
      break;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Add virtual mapping to the management for this process
    case CMD_MAP:
    {
      CMD_MAP_TYPE aux;
      
      OBTAIN_SHORT_LOCK();
      
      oal_printk("OAL: MAP\n");
      
      if (copy_from_user(&aux, (CMD_MAP_TYPE *)arg, sizeof(CMD_MAP_TYPE)) == 0)
      {
        aux.virtual_pointer = oal_map(aux.handle_pointer, aux.type, aux.virtual_pointer);
        if (copy_to_user((CMD_MAP_TYPE *)arg, &aux, sizeof(CMD_MAP_TYPE)) != 0)
        {
          retval = -EIO;
        }
      }
      else
      {
        retval = -EIO;
      }
      
      RELEASE_SHORT_LOCK();
      break;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Add virtual mapping to the management for this process
    case CMD_MAP_PHYS:
    {
      CMD_MAP_TYPE aux;
      
      OBTAIN_SHORT_LOCK();
      
      oal_printk("OAL: MAP BASED ON PHYSICAL\n");
      
      if (copy_from_user(&aux, (CMD_MAP_TYPE *)arg, sizeof(CMD_MAP_TYPE)) == 0)
      {
        aux.virtual_pointer = oal_map_phys(aux.handle_pointer, aux.type, aux.virtual_pointer);
        if (copy_to_user((CMD_MAP_TYPE *)arg, &aux, sizeof(CMD_MAP_TYPE)) != 0)
        {
          retval = -EIO;
        }
      }
      else
      {
        retval = -EIO;
      }
      
      RELEASE_SHORT_LOCK();
      break;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Add virtual mapping to the management for this process
    case CMD_MAP_NEW:
    {
      CMD_MAP_NEW_TYPE aux;
      
      OBTAIN_SHORT_LOCK();
      
      oal_printk("OAL: MAP TO NEW ENTRY\n");
      
      if (copy_from_user(&aux, (CMD_MAP_NEW_TYPE *)arg, sizeof(CMD_MAP_NEW_TYPE)) == 0)
      {
        aux.virtual_pointer = oal_map_new(aux.type, aux.virtual_pointer, aux.size);
        if (copy_to_user((CMD_MAP_NEW_TYPE *)arg, &aux, sizeof(CMD_MAP_NEW_TYPE)) != 0)
        {
          retval = -EIO;
        }
      }
      else
      {
        retval = -EIO;
      }
      
      RELEASE_SHORT_LOCK();
      break;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Get number of allocations
    case CMD_INFO:
    {
      uint64_t allocs;
      
      OBTAIN_SHORT_LOCK();
      
      oal_printk("OAL: GET NUM ALLOCATIONS\n");
      
      allocs = oal_get_num_allocations();
      if (copy_to_user((uint64_t *)arg, &allocs, sizeof(uint64_t)) != 0)
        retval = -EIO;
      
      RELEASE_SHORT_LOCK();
      break;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Get number of mappings
    case CMD_INFO_MAP:
    {
      uint64_t allocs;
      
      OBTAIN_SHORT_LOCK();
      
      oal_printk("OAL: GET NUM MAPPINGS\n");
      
      allocs = oal_get_num_mappings();
      if (copy_to_user((uint64_t *)arg, &allocs, sizeof(uint64_t)) != 0)
        retval = -EIO;
      
      RELEASE_SHORT_LOCK();
      break;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Flush all
    case CMD_FLUSH:
    {
      OBTAIN_SHORT_LOCK();
      
      oal_printk("OAL: FLUSH\n");
            
      flush_tlb_all();
      flush_cache_all();
      
      RELEASE_SHORT_LOCK();
      break;
    }  
    
    ///////////////////////////////////////////////////////////////////////////
    // Flush
    case CMD_FLUSH_SPECIFIC:
    {
      CMD_FLUSH_SPECIFIC_TYPE aux;
      
      OBTAIN_SHORT_LOCK();
      
      oal_printk("OAL: FLUSH ADDRESS\n");
        
      if (copy_from_user(&aux, (CMD_FLUSH_SPECIFIC_TYPE *)arg, sizeof(CMD_FLUSH_SPECIFIC_TYPE)) == 0)
      {
        flush_dcache_range((void *)aux.virtual_pointer, aux.size);
      }
      else
      {
        retval = -EIO;
      }
      
      RELEASE_SHORT_LOCK();
      break;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Invalidate
    case CMD_INVALIDATE_SPECIFIC:
    {
      CMD_FLUSH_SPECIFIC_TYPE aux;
      
      OBTAIN_SHORT_LOCK();
      
      oal_printk("OAL: INVALIDATE ADDRESS\n");
      
      if (copy_from_user(&aux, (CMD_FLUSH_SPECIFIC_TYPE *)arg, sizeof(CMD_FLUSH_SPECIFIC_TYPE)) == 0)
      {
        invalidate_dcache_range((void *)aux.virtual_pointer, aux.size);
      }
      else
      {
        retval = -EIO;
      }
      
      RELEASE_SHORT_LOCK();
      break;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Flush & Invalidate
    case CMD_FLUSHINVALIDATE_SPECIFIC:
    {
      CMD_FLUSH_SPECIFIC_TYPE aux;
      
      OBTAIN_SHORT_LOCK();
      
      oal_printk("OAL: FLUSH & INVALIDATE ADDRESS\n");
        
      if (copy_from_user(&aux, (CMD_FLUSH_SPECIFIC_TYPE *)arg, sizeof(CMD_FLUSH_SPECIFIC_TYPE)) == 0)
      {
        flush_and_invalidate_dcache_range((void *)aux.virtual_pointer, aux.size);
      }
      else
      {
        retval = -EIO;
      }
      
      RELEASE_SHORT_LOCK();
      break;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Return all mappings
    case CMD_MAPPINGS:
    {
      CMD_MAPPINGS_TYPE aux;
    
      OBTAIN_SHORT_LOCK();
      
      oal_printk("OAL: GET ALL MAPPINGS\n");
        
      if (copy_from_user(&aux, (CMD_MAPPINGS_TYPE *)arg, sizeof(CMD_MAPPINGS_TYPE)) == 0)
      {
        oal_getmappings(aux.address[0], aux.address);    
        if (copy_to_user((CMD_MAPPINGS_TYPE *)arg, &aux, sizeof(CMD_MAPPINGS_TYPE)) != 0)
          retval = -EIO;
      }
      else
      {
        retval = -EIO;
      }
            
      RELEASE_SHORT_LOCK();
      break;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Return all mappings based on HW
    case CMD_MAPPINGSHW:
    {
      CMD_MAPPINGS_TYPE aux;
      
      OBTAIN_SHORT_LOCK();
      
      oal_printk("OAL: GET ALL MAPPINGS BASED ON HW ADDRESS\n");
        
      if (copy_from_user(&aux, (CMD_MAPPINGS_TYPE *)arg, sizeof(CMD_MAPPINGS_TYPE)) == 0)
      {
        oal_getmappingshw(aux.address[0], aux.address);
        if (copy_to_user((CMD_MAPPINGS_TYPE *)arg, &aux, sizeof(CMD_MAPPINGS_TYPE)) != 0)
        {
          retval = -EIO;
        }
      }
      else
      {
        retval = -EIO;
      }
      
      RELEASE_SHORT_LOCK();
      break;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Free unused contiguous region
    case CMD_UNMAP:
    {
      uint64_t handle_pointer;
    
      OBTAIN_SHORT_LOCK();
      
      oal_printk("OAL: UNMAP\n");
        
      if (copy_from_user(&handle_pointer, (uint64_t *)arg, sizeof(uint64_t)) == 0)
      {
        handle_pointer = oal_unmap(handle_pointer);
        if (copy_to_user((uint64_t *)arg, &handle_pointer, sizeof(uint64_t)) != 0)
        {
          retval = -EIO;
        }
      }
      else
      {
        retval = -EIO;
      }
      
      RELEASE_SHORT_LOCK();
      break;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Get memory size total
    case CMD_MEMORY_SIZE_TOTAL_GET:
    {
      int64_t lSizeInBytes;
      
      OBTAIN_SHORT_LOCK();
      
      lSizeInBytes = oal_memorysizetotal();
      
      oal_printk("OAL: GET TOTAL MEMORY SIZE\n");
        
      if (copy_to_user((uint64_t *)arg, &lSizeInBytes, sizeof(int64_t)) != 0)
        retval = -EIO;
      
      RELEASE_SHORT_LOCK();
      break;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Get free memory size
    case CMD_MEMORY_SIZE_FREE_GET:
    {
      int64_t lSizeInBytes;
      
      OBTAIN_SHORT_LOCK();
      
      lSizeInBytes = oal_memorysizefree();
      
      oal_printk("OAL: GET FREE MEMORY\n");
        
      if (copy_to_user((uint64_t *)arg, &lSizeInBytes, sizeof(int64_t)) != 0)
        retval = -EIO;
      
      RELEASE_SHORT_LOCK();
      
      break;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Get available devices
    case CMD_MEMORY_GET_DEVICES:
    {
      uint8_t lSizeInBytes;
      
      OBTAIN_SHORT_LOCK();
      
      lSizeInBytes = oal_devices_loaded;
      
      oal_printk("OAL: GET LOADED DEVICES MASK\n");
        
      if (copy_to_user((uint8_t *)arg, &lSizeInBytes, sizeof(uint8_t)) != 0)
        retval = -EIO;
      
      RELEASE_SHORT_LOCK();
      break;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Get autobalanced devices
    case CMD_MEMORY_GET_AUTOBALANCE:
    {
      uint8_t lSizeInBytes;
      
      OBTAIN_SHORT_LOCK();
      
      lSizeInBytes = oal_devices_autobalanced;
      
      oal_printk("OAL: GET AUTOBALANCED DEVICES MASK\n");
        
      if (copy_to_user((uint8_t *)arg, &lSizeInBytes, sizeof(uint8_t)) != 0)
        retval = -EIO;
      
      RELEASE_SHORT_LOCK();
      break;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Get base address
    case CMD_MEMORY_GET_BASE:
    {
      uint64_t aux;
      
      OBTAIN_SHORT_LOCK();
      
      oal_printk("OAL: GET BASE ADDRESS\n");
        
      if (copy_from_user(&aux, (uint64_t *)arg, sizeof(uint64_t)) == 0)
      {
        aux = apex_allocator_get_physical_base(aux);
        if (copy_to_user((uint64_t *)arg, &aux, sizeof(int64_t)) != 0)
          retval = -EIO;
      }
      else
      {
        retval = -EIO;
      }
      
      RELEASE_SHORT_LOCK();
      break;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Get size of the chunk
    case CMD_MEMORY_GET_SIZE:
    {
      uint64_t aux;
      
      OBTAIN_SHORT_LOCK();
      
      oal_printk("OAL: GET SIZE OF DEVICE\n");
        
      if (copy_from_user(&aux, (uint64_t *)arg, sizeof(uint64_t)) == 0)
      {
        aux = apex_allocator_get_total_size(aux);
        if (copy_to_user((uint64_t *)arg, &aux, sizeof(int64_t)) != 0)
        {
          retval = -EIO;
        }
      }
      else
      {
        retval = -EIO;
      }
      
      RELEASE_SHORT_LOCK();
      break;
    }
    // NOOP
    default:
    {
      oal_printk("OAL: UNKNOWN OPERATION\n");
      retval = -EIO; 
      break;
    }
  }
  return retval;
}

////////////////////////////////////////////////////////////////////////////////
// Memmap function
////////////////////////////////////////////////////////////////////////////////
static int32_t oal_mem_mmap_cached(struct file *filp, struct vm_area_struct *vma)
{  
  int32_t retval = 0;
  
  vma->vm_page_prot = set_pte_bit(vma->vm_page_prot, __pgprot(PTE_DIRTY));
  
  if (remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, vma->vm_end - vma->vm_start, vma->vm_page_prot))
    retval = -EAGAIN;

  return retval;
}

static int32_t oal_mem_mmap_noncached(struct file *filp, struct vm_area_struct *vma)
{  
  int32_t retval = 0;
  
  vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
  vma->vm_page_prot = set_pte_bit(vma->vm_page_prot, __pgprot(PTE_DIRTY));
  
  if (remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, vma->vm_end - vma->vm_start, vma->vm_page_prot))
    return -EAGAIN;

  return retval;
}

static int nxp_oal_release(struct inode *inodep, struct file *filep)
{
  OBTAIN_LONG_LOCK();
  OBTAIN_SHORT_LOCK();
  cma_list_free_pid(task_pgrp_nr(current));
  RELEASE_SHORT_LOCK();
  RELEASE_LONG_LOCK();
   
   return 0;
}

///////////////////////////////////////////////////////////////////////////////////
// FOPS definitions
///////////////////////////////////////////////////////////////////////////////////
static const struct file_operations oal_fops = {
  .owner =    THIS_MODULE,
  .unlocked_ioctl = oal_mem_device_ioctl,
  .mmap= 		  oal_mem_mmap_cached,
  .release = nxp_oal_release
};

static const struct file_operations oal_fops_nc = {
  .owner =    THIS_MODULE,
  .mmap=      oal_mem_mmap_noncached,
};

///////////////////////////////
////////////////////////////////

int nxp_oal_ispoweroftwo (uint32_t x)
{
  while (((x % 2) == 0) && x > 1)
    x /= 2;
  return (x == 1);
}

int nxp_oal_probe(struct platform_device *pdev)
{
  struct device_node *oal_mem_node;
  
  __be32 *oal_mem_region;
  __be32 *oal_auto;
  __be32 *oal_id;
  __be32 *oal_align;
  __be32 *oal_init;
  int32_t prop_len, i;
  uint64_t oal_mem_start;
  uint64_t oal_mem_len;
  int32_t oal_id_start;
  int32_t oal_align_start;
  uint8_t *virtual_address;

  // Get memory size
  oal_mem_node = of_parse_phandle(pdev->dev.of_node, "memory-region", 0);
  oal_mem_region = (__be32 *)of_get_property(oal_mem_node, "reg", &prop_len);
  if (!oal_mem_region || prop_len <= 0)
  {
    printk("OAL Memory range specification cannot be read. Pleaser review reg and memory-region attributes.\n");
    goto fail_alloc_region_main_cached;
  }
  
  i = prop_len / sizeof(u64);
  oal_mem_start = of_read_number(&oal_mem_region[0], i);
  oal_mem_len = of_read_number(&oal_mem_region[i], i);

  // Get ID
  oal_id = (__be32 *)of_get_property(pdev->dev.of_node, "id", &prop_len);
  oal_id_start = ((int8_t *)oal_id)[3];
  if (!oal_id || prop_len <= 0 || oal_id_start >= 8)
  {
    printk("OAL ID cannot be read or isn't in range 0-7. Pleaser review id attribute.\n");
    goto fail_alloc_region_main_cached;
  }
  
  oal_devices_loaded |= (1 << oal_id_start);
  
  // Get alignment
  oal_align = (__be32 *)of_get_property(pdev->dev.of_node, "align", &prop_len);
  oal_align_start = ((int8_t *)oal_align)[3] | ((int8_t *)oal_align)[2] << 8 | ((int8_t *)oal_align)[1] << 16 | ((int8_t *)oal_align)[0] << 24;
  if (!oal_align || prop_len <= 0 || !nxp_oal_ispoweroftwo(oal_align_start))
  {
    printk("OAL Align cannot be read or isn't power of two (%X). Pleaser review align attribute.\n", oal_align_start);
    goto fail_alloc_region_main_cached;
  }
  
  oal_device_alignment[oal_id_start] = oal_align_start;
  
  // Get init
  oal_init = (__be32 *)of_get_property(pdev->dev.of_node, "init", &prop_len);
  
  // Get if the memory is autobalanced
  oal_auto = (__be32 *)of_get_property(pdev->dev.of_node, "autobalance", &prop_len);
  if (!oal_auto)
    oal_devices_autobalanced &= ~(1 << oal_id_start);
  else
    oal_devices_autobalanced |= 1 << oal_id_start;
  
  
  // Chardevice creation for mapping (only for the first time)
  /////////////////////////////////////
  if (oal_allocator_devices == 0)
  {
    if (alloc_chrdev_region(&oal_mem_dev_main_cached, 0, 1, "oal_main_c") < 0)
        goto fail_alloc_region_main_cached;

    if (IS_ERR(oal_mem_cl_cached = class_create(THIS_MODULE, "oal_int8_tdrv_c")))
        goto fail_class_create_main_cached;

    if (IS_ERR(device_create(oal_mem_cl_cached, NULL, oal_mem_dev_main_cached, NULL, "oal_cached")))
        goto fail_device_create_main_cached;

    cdev_init(&oal_mem_c_dev_main_cached, &oal_fops);
    if (cdev_add(&oal_mem_c_dev_main_cached, oal_mem_dev_main_cached, 1) == -1)
        goto fail_cdev_add_main_cached;
    
    /////////////////////////////////////
    
    if (alloc_chrdev_region(&oal_mem_dev_main_noncached, 0, 1, "oal_main_n") < 0)
        goto fail_alloc_region_main_noncached;

    if (IS_ERR(oal_mem_cl_noncached = class_create(THIS_MODULE, "oal_int8_tdrv_n")))
        goto fail_class_create_main_noncached;

    if (IS_ERR(device_create(oal_mem_cl_noncached, NULL, oal_mem_dev_main_noncached, NULL, "oal_noncached")))
        goto fail_device_create_main_noncached;

    cdev_init(&oal_mem_c_dev_main_noncached, &oal_fops_nc);
    if (cdev_add(&oal_mem_c_dev_main_noncached, oal_mem_dev_main_noncached, 1) == -1)
        goto fail_cdev_add_main_noncached;	
    
    sema_init(&oal_short_term_sema, 1);
    sema_init(&oal_long_term_sema, 1);
  }
  
  /////////////////////////////////////
  
  apex_allocator_mmap_init(oal_id_start, oal_mem_start, oal_mem_len);
  
  if (oal_init)
  {
    virtual_address = (uint8_t *)ioremap(oal_mem_start, oal_mem_len);
    if (virtual_address)
    {
      uint64_t i;
      for(i = 0; i < oal_mem_len; i+=8)
      {
        writeq(0x0ULL, virtual_address + i);
      }
      iounmap(virtual_address);
    }
    else
    {
      printk("OAL memory init failed!\n");
    }
  }
  
  oal_allocator_devices++;
  
  printk("OAL region successfully mapped %llX@%llX, Alignment: 0x%X\n", oal_mem_len, oal_mem_start, oal_device_alignment[oal_id_start]);
  if (oal_init)
  {
    oal_printk("  Initialized to 0x0ULL\n");
  }
  oal_printk("  Total regions: %d. Autobalance flags: %02X. ID flags: %02X.\n", oal_allocator_devices, oal_devices_autobalanced, oal_devices_loaded);
  
  cma_list_init(oal_id_start, oal_mem_start, oal_mem_start + oal_mem_len, oal_device_alignment[oal_id_start]);
  oal_printk("  CMA: ID %02X. Start %02llX. End %02llX. Length %02llX. Num %02llX.\n", oal_id_start, oal_mem_start, (oal_mem_start + oal_mem_len), oal_mem_len, (oal_mem_len / oal_device_alignment[oal_id_start]));
  
  
  return 0;
  
  fail_cdev_add_main_noncached:
  device_destroy(oal_mem_cl_noncached, oal_mem_dev_main_noncached);
  fail_device_create_main_noncached:
  class_destroy(oal_mem_cl_noncached);
  fail_class_create_main_noncached:
  unregister_chrdev_region(oal_mem_dev_main_noncached, 1);
  fail_alloc_region_main_noncached:
  fail_cdev_add_main_cached:
  device_destroy(oal_mem_cl_cached, oal_mem_dev_main_cached);
  fail_device_create_main_cached:
  class_destroy(oal_mem_cl_cached);
  fail_class_create_main_cached:
  unregister_chrdev_region(oal_mem_dev_main_cached, 1);
  fail_alloc_region_main_cached:
  return  -1;
}


   
///////////////////////////////////////////////////////////////////////////////////
// Destructor function - frees all resources
///////////////////////////////////////////////////////////////////////////////////
int nxp_oal_remove(struct platform_device *pdev)
{
  int i = 0;
  
  if (--oal_allocator_devices == 0)
  {
    OBTAIN_LONG_LOCK();
    OBTAIN_SHORT_LOCK();
    cma_list_free_all();
    cma_list_deinit();
    RELEASE_SHORT_LOCK();
    RELEASE_LONG_LOCK();
  
    for (i = 0; i < OAL_MAX_ALLOCATOR_NUM; ++i)
    {
      apex_allocator_mmap_deinit(i);
    }
  
    device_destroy(oal_mem_cl_noncached, oal_mem_dev_main_noncached);
    class_destroy(oal_mem_cl_noncached);
    unregister_chrdev_region(oal_mem_dev_main_noncached, 1);
    
    device_destroy(oal_mem_cl_cached, oal_mem_dev_main_cached);
    class_destroy(oal_mem_cl_cached);
    unregister_chrdev_region(oal_mem_dev_main_cached, 1);
  }
  
  return 0;
}


static const struct of_device_id nxp_oal_dt_ids[] = {
  {
    .compatible = "fsl,s32v234-oal",
  },
  {}
};


static struct platform_driver nxp_oal_driver = {
  .driver = {
    .name = "OAL_DRIVER_TEST",
    .owner = THIS_MODULE,
    .of_match_table = nxp_oal_dt_ids
  },
  .probe = nxp_oal_probe,
  .remove = nxp_oal_remove,
};

module_platform_driver(nxp_oal_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rostislav Hulik <rostislav.hulik <at> freescale.com>");
MODULE_DESCRIPTION("OAL kernel module for contiguous memory allocations and its management.");
MODULE_ALIAS("oal");
