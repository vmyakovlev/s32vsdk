/**
*@defgroup	dxgrSMKERNELMOD The VideoListener Firmware Driver
@details	This Linux driver is intended to be used as the VideoListener firmware boot-loader
		and interface between user-space application and the firmware. It contains
		the asynchronous event listener as implemented by the @ref dxgrSM_CORE and
		provides access to the debug text memory in form of readable device file.

		Module expects that the current device tree configuration contains an allocated
		memory region prepared to be occupied by the firmware and that the SMC instruction
		is accessible and executable. Both tasks are done by patching the u-boot and
		the Linux kernel source code by provided patches.

		In time when the driver module is being insmod-ed it expects to have the
		firmware binary within the current directory named theA5App.bin. After successful
		insmod the module creates entry within the /dev folder with the name of the
		device: /dev/sm_drv. This can be used to read the text debug output produced
		by the firmware. The device file also provides ioctl interface supporting following
		commands:
		- SM_DRV_IOCTL_INIT: Initialize the firmware
		- SM_DRV_IOCTL_START: Start the firmware
		- SM_DRV_IOCTL_STOP: Stop the firmware
		- SM_DRV_IOCTL_SET_CFG: Set a configuration parameter
		- SM_DRV_IOCTL_GET_CFG: Get a configuration parameter
		- SM_DRV_IOCTL_REG_SIG: Register asynchronous signal listener
		- SM_DRV_IOCTL_UNREG_SIG: Cancel registration of asynchronous signal listener
		- SM_DRV_IOCTL_ENABLE_EVENTS: Enable/Disable asynchronous events reporting by the firmware

@addtogroup	dxgrSMKERNELMOD
@{

@file		main.c
@brief	This is the VideoListener firmware driver
@details

Copyright (c) 2016 NXP Semiconductors
All Rights Reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
/*  Filesystem */
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>
/*  Device */
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
/*  Threading */
#include <linux/kthread.h>
#include <linux/semaphore.h>
#include <linux/mutex.h>
#include <linux/completion.h>
/* Miscellaneous */
#include <linux/delay.h>
/*  Custom */
#include <sm_drv_types.h>
/*  IRQ */
#include <linux/interrupt.h>

#define DEVICE_NAME				"sm_drv"
#define F_BUF_SIZE				1024U
#define F_FILENAME				"theA5App.bin"

#define SC_CMD_LL_INIT				0x0
#define SC_CMD_INIT				0x1
#define SC_CMD_START				0x2
#define SC_CMD_STOP				0x3
#define SC_CMD_CONFIG_SET			0x4
#define SC_CMD_CONFIG_GET			0x5
#define SC_CMD_ENABLE_EVENTS			0x6
#define SC_CMD_SHUTDOWN				0x7

#define TRUE					1U
#define FALSE					0U
#define INIT_ON_LOAD				FALSE
#define START_ON_LOAD				FALSE
#define ENABLE_EVENTS_ON_LOAD			FALSE
#define STOP_ON_UNLOAD				TRUE
#define S32V234					1
#define PLATFORM				S32V234

/* How many times can /dev/sm_drv be simultaneously opened */
#define FOP_MAX_OPEN_FILES 5

typedef struct {
	int cmd;
	long long val0;
	long long val1;
} sm_drv_sc_prm_t;

typedef struct {
	pid_t pid;
	struct task_struct *p_task;
	bool valid;
} sm_drv_reg_pid_t;

struct dbgb_read_count {
	unsigned int count;
	int used;
};

struct dbgb {
	/* How many bytes were read from buffer so far */
	struct dbgb_read_count read_count[FOP_MAX_OPEN_FILES];
	/* Size of the debug buffer */
	unsigned int size;
	/* Write count address */
	volatile unsigned int __iomem *write_countp;
	/* Debug buffer base */
	void __iomem *base;
	/* Debug buffer base - physical address */
	unsigned long long base_phys;
	/* Protecting mutex */
	struct mutex mut;
	/* How many times is the file opened */
	volatile unsigned int count;
};

typedef struct {
	int probed;
	int started;
	void __iomem *base;
	struct task_struct *sc_handler;
	sm_drv_sc_prm_t sc_command;
	struct completion sch_comp;
	dev_t devt;
	struct class *class;
	struct device *device;
	struct platform_device *pdev;
	struct cdev cdev;
	struct dbgb dbgb;
	sm_drv_reg_pid_t reg_pid;
} sm_drv_t;

static sm_drv_t sm_drv = {
	.probed = 0,
	.started = 0,
	.base = NULL,
	.sc_handler = NULL,
	.sc_command = { -1, 0LL, 0LL },
	.class = NULL,
	.pdev = NULL,
	.reg_pid.pid = 0,
	.reg_pid.valid = FALSE,
	.reg_pid.p_task = NULL
};

static const struct of_device_id nxp_of_match[] = {
	{ .compatible = "nxp,resmem", },
	{}
};

MODULE_DEVICE_TABLE(of, nxp_of_match);

static struct file *fopen(const char *filename, int flags, int mode);
static void fclose(struct file *f);
static int fread(struct file *f,
			unsigned long long offset,
			unsigned char *buf,
			unsigned int count);
static int sm_drv_remove(struct platform_device *pdev);
static int sm_drv_probe(struct platform_device *pdev);
static int sm_sc_worker(void *data);
static void sm_sc_do_call(int command, long long val0, long long val2);
#ifdef HAVE_UNLOCKED_IOCTL
static long sm_drv_fop_ioctl(struct file *f,
				unsigned int cmd,
				unsigned long prm);
#else
static int sm_drv_fop_ioctl(struct inode *node,
				struct file *f,
				unsigned int cmd,
				unsigned long prm);
#endif /* HAVE_UNLOCKED_IOCTL */
static int sm_drv_fop_open(struct inode *node, struct file *f);
static ssize_t sm_drv_fop_read(struct file *f,
				char __user *buf,
				size_t count,
				loff_t *f_pos);
static int sm_drv_fop_close(struct inode *node, struct file *f);
static struct platform_driver sm_platform_driver = {
	.probe = sm_drv_probe,
	.remove = sm_drv_remove,
	.driver = {
	.name = "sm_drv",
	.owner = THIS_MODULE,
	.of_match_table = nxp_of_match,
	},
};

static const struct file_operations sm_fops = {
	.read = sm_drv_fop_read,
	.write = NULL,
#ifdef HAVE_UNLOCKED_IOCTL
	.unlocked_ioctl = &sm_drv_fop_ioctl,
#else
	.ioctl = &sm_drv_fop_ioctl,
#endif /* HAVE_UNLOCKED_IOCTL */
	.mmap = NULL,
	.open = &sm_drv_fop_open,
	.release = &sm_drv_fop_close,
	.fsync = NULL,
	.fasync = NULL,
};

/**
 * copy_to_user_fromio - copy data from mmio-space to user-space
 * @dst: the destination pointer on user-space
 * @src: the source pointer on mmio
 * @count: the data size to copy in bytes
 *
 * Copies the data from mmio-space to user-space.
 *
 * This function was copied from sound/core/memory.c
 *
 * Return: Zero if successful, or non-zero on failure.
 */
static int copy_to_user_fromio(void __user *dst,
				const volatile void __iomem *src, size_t count)
{
#if defined(__i386__) || defined(CONFIG_SPARC32)
		return	copy_to_user(dst, (const void __force *)src, count)
			? -EFAULT : 0;
#else
		char buf[256];

		while (count) {
			size_t c = count;

			if (c > sizeof(buf))
				c = sizeof(buf);
			memcpy_fromio(buf, (void __iomem *)src, c);
			if (copy_to_user(dst, buf, c))
				return -EFAULT;
			count -= c;
			dst += c;
			src += c;
		}
		return 0;
#endif
}

static ssize_t sm_drv_fop_read(struct file *f,
				char __user *buf,
				size_t count,
				loff_t *f_pos)
{
	struct dbgb_read_count *read_count =
				(struct dbgb_read_count *) f->private_data;
	unsigned int written, diff, overwrites, output_bytes = 0U;

	/* Cannot read NULL */
	if (unlikely(!(sm_drv.dbgb.base)))
		return -EIO;

	/* Get number of bytes in the buffer */
	written = *sm_drv.dbgb.write_countp;

	/* There is nothing to be read */
	while (written == read_count->count) {
		if (f->f_flags & O_NONBLOCK)
			return -EAGAIN;
		 /* Try it after some time */
		if (unlikely(msleep_interruptible(500)))
			return -ERESTARTSYS;
		written = *sm_drv.dbgb.write_countp;
	}

	overwrites = (written - read_count->count) / sm_drv.dbgb.size;
	diff = (written - read_count->count) % sm_drv.dbgb.size;

	/* if 0 == overwrites then data are at offset dbgb_read_count
	* with data length diff
	*/
	if (overwrites) {
		char msg[24];
		int size;

		size = sprintf(msg,
				"%u OVERFLOW%c\n",
				overwrites,
				(overwrites > 1) ? 'S' : ' ');
		if (count >= size) {
			if (unlikely(copy_to_user(buf, msg, size)))
				return -EFAULT;
			count -= size;
			buf += size;
			output_bytes += size;
		}
		/* data are at offset written - sm_drv.dbgb.size
		* and the data length is sm_drv.dbgb.size
		*/
		read_count->count = written - sm_drv.dbgb.size;
		diff = sm_drv.dbgb.size;
	}
	/* Write only bytes which can fit in */
	diff = (diff <= count) ? diff : count;
	/* Check for the buffer wrap around (less frequent) */
	/* Wrap around */
	if (unlikely(read_count->count % sm_drv.dbgb.size + diff >=
		sm_drv.dbgb.size)) {
		int size = sm_drv.dbgb.size
				- read_count->count % sm_drv.dbgb.size;
		/* Copy till the end of the buffer */
		if (unlikely(copy_to_user_fromio(buf, sm_drv.dbgb.base
				+ read_count->count % sm_drv.dbgb.size, size)))
			return -EFAULT;
		/* Update the read pointer and remaining size */
		diff -= size;
		buf += size;
		read_count->count += size;
		output_bytes += size;
	}

	/* Copy the linear data */
	if (unlikely(copy_to_user_fromio(
				buf,
				sm_drv.dbgb.base
				+ read_count->count % sm_drv.dbgb.size, diff)))
		return -EFAULT;
	/* Update read data pointer */
	read_count->count += diff;
	output_bytes += diff;
	return output_bytes;
}

#ifdef HAVE_UNLOCKED_IOCTL
static long sm_drv_fop_ioctl(struct file *f,
				unsigned int cmd,
				unsigned long prm)
{
#else
static int sm_drv_fop_ioctl(struct inode *node,
				struct file *f,
				unsigned int cmd,
				unsigned long prm)
{
#endif /* HAVE_UNLOCKED_IOCTL */
	int ret = 0;
	sm_drv_sc_param_t kbuf;
	void __user *ubuf = (void __user *)prm;
	bool do_return = TRUE;

	if (unlikely(!ubuf))
		return -EFAULT;

	switch (cmd) {
	case SM_DRV_IOCTL_INIT:
		sm_sc_do_call(SC_CMD_INIT, 0, 0);
		break;
	case SM_DRV_IOCTL_START:
		sm_sc_do_call(SC_CMD_START, 0, 0);
		break;
	case SM_DRV_IOCTL_STOP:
		sm_sc_do_call(SC_CMD_STOP, 0, 0);
		break;
	case SM_DRV_IOCTL_SET_CFG:
		ret = copy_from_user((void *)&kbuf,
					ubuf,
					sizeof(sm_drv_sc_param_t));
		if (unlikely(ret)) {
			ret = -EFAULT;
			dev_err(sm_drv.device, "Unable to get parameter\n");
		} else {
			sm_drv_sc_param_t *sc_param =
					(sm_drv_sc_param_t *)&kbuf;
			/*
			* TODO: Add an option to set multiple
			* configuration parameters at once
			* (maybe SM_DRV_IOCTL_SET_MULTIPLE_CFG ??)
			*/
			sm_sc_do_call(SC_CMD_CONFIG_SET, sc_param->s64_val0,
							sc_param->s64_val1);
		}
		break;
	case SM_DRV_IOCTL_GET_CFG:
		ret = copy_from_user((void *)&kbuf,
					ubuf,
					sizeof(sm_drv_sc_param_t));
		if (unlikely(ret)) {
			ret = -EFAULT;
			dev_err(sm_drv.device,
				"Unable to get parameter\n");
		} else {
			sm_drv_sc_param_t *sc_param =
						(sm_drv_sc_param_t *)&kbuf;

			sm_sc_do_call(SC_CMD_CONFIG_GET,
					sc_param->s64_val0,
					sc_param->s64_val1);
		}
		break;
	case SM_DRV_IOCTL_REG_SIG:
		if (likely(!sm_drv.reg_pid.valid)) {
			if (copy_from_user(&sm_drv.reg_pid.pid,
						(pid_t *)prm,
						sizeof(pid_t)))
				ret = -EFAULT;
			else
				sm_drv.reg_pid.valid = TRUE;
		} else
			ret = -EFAULT;
		do_return = FALSE;
		break;
	case SM_DRV_IOCTL_UNREG_SIG:
		sm_drv.reg_pid.valid = FALSE;
		sm_drv.reg_pid.p_task = NULL;
		do_return = FALSE;
		break;
	case SM_DRV_IOCTL_ENABLE_EVENTS:
		ret = copy_from_user((void *)&kbuf,
					ubuf,
					sizeof(sm_drv_sc_param_t));
		if (unlikely(ret)) {
			ret = -EFAULT;
			dev_err(sm_drv.device,
				"Unable to get parameter\n");
		} else {
			sm_drv_sc_param_t *sc_param =
						(sm_drv_sc_param_t *)&kbuf;

			sm_sc_do_call(SC_CMD_ENABLE_EVENTS,
					sc_param->s64_val0,
					0U);
		}
		break;
	default:
		ret = -EINVAL;
		do_return = FALSE;
		break;
	}

	if (do_return) {
		kbuf.s64_val0 = sm_drv.sc_command.val0;
		kbuf.s64_val1 = sm_drv.sc_command.val1;
		ret = copy_to_user(ubuf,
					(void *)&kbuf,
					sizeof(sm_drv_sc_param_t));
		if (unlikely(ret)) {
			ret = -EFAULT;
			dev_err(sm_drv.device,
				"Unable to return SC return value\n");
		}
	}
	return ret;
}

static int sm_drv_fop_open(struct inode *node, struct file *f)
{
	unsigned int count = 0U;
	/* Case when initialization has not finished yet */
	while (unlikely(!sm_drv.started)) {
		if (f->f_flags & O_NONBLOCK)
			return -EAGAIN;
		/* Try it again later */
		if (unlikely(msleep_interruptible(5)))
			return -EINTR;
		if (++count > 600) {	/* 3 seconds are already too long */
			dev_err(sm_drv.device,
				"failed to open file - timeout\n");
			return -EBUSY;
		}
	}

	do {
		/* There is a race for the sm_drv.dbgb.count
		* thus mutex must be used
		*/
		if (unlikely(mutex_lock_interruptible(&sm_drv.dbgb.mut)))
			return -ERESTARTSYS;
		/* Check whether it is possible to open the file */
		if (sm_drv.dbgb.count < FOP_MAX_OPEN_FILES) {
			/* We opened the file - find out which entry */
			for (count = 0U; count < FOP_MAX_OPEN_FILES; count++)
				if (!sm_drv.dbgb.read_count[count].used) {
					/* This one is unused
					* - we will use it
					*/
					sm_drv.dbgb.read_count[count].used = 1;
					/* Initialize the read counter to 0 */
					sm_drv.dbgb.read_count[count].count =
									0U;
					f->private_data =
					(void *)&sm_drv.dbgb.read_count[count];
					sm_drv.dbgb.count++;
					mutex_unlock(&sm_drv.dbgb.mut);
					return 0;
				}
			/* We should not get here - we did not find unused slot
			* however counter says that there is at least one
			*/
			dev_err(sm_drv.device,
			"Internals corrupted (opened %u/%u)\n",
			sm_drv.dbgb.count, FOP_MAX_OPEN_FILES);
			mutex_unlock(&sm_drv.dbgb.mut);
			return -EIO;
		}
		/* File opened too many times, try it again */
		mutex_unlock(&sm_drv.dbgb.mut);
		if (f->f_flags & O_NONBLOCK)
			return -EAGAIN;
		/* Do not kill the CPU by busy-waiting without sleeping */
		if (unlikely(msleep_interruptible(5)))
			return -EINTR;
	} while (1);
	return 0;
}

static int sm_drv_fop_close(struct inode *node, struct file *f)
{
	struct dbgb_read_count *read_count =
				(struct dbgb_read_count *)f->private_data;
	/* There is a race for the sm_drv.dbgb.count
	* thus mutex must be used
	*/
	if (unlikely(mutex_lock_interruptible(&sm_drv.dbgb.mut)))
		return -ERESTARTSYS;
	read_count->used = 0;
	sm_drv.dbgb.count--;
	mutex_unlock(&sm_drv.dbgb.mut);
	return 0;
}
static int fread(struct file *f,
			unsigned long long offset,
			unsigned char *buf,
			unsigned int count)
{
	mm_segment_t oldfs;
	int ret;

	oldfs = get_fs();
	set_fs(get_ds());
	ret = vfs_read(f, buf, count, &offset);
	set_fs(oldfs);
	return ret;
}

static void fclose(struct file *f)
{
	filp_close(f, NULL);
}

static struct file *fopen(const char *filename, int flags, int mode)
{
	struct file *f = NULL;
	mm_segment_t fs;

	fs = get_fs();
	set_fs(get_ds());
	f = filp_open(filename, flags, mode);
	set_fs(fs);
	return f;
}

static int sm_drv_probe(struct platform_device *pdev)
{
	struct device_node *dn_mem;
	struct resource res;
	int ret;

	dn_mem = of_parse_phandle(pdev->dev.of_node, "memory-region", 0);
	if (!dn_mem)
		return -ENODEV;

	/*  TODO: add call to request_mem_region() / release_mem_region() */
	sm_drv.base = of_iomap(dn_mem, 0);
	of_node_put(dn_mem);
	if (sm_drv.base) {
		ret = of_address_to_resource(dn_mem, 0, &res);
		if (ret) {
			iounmap(sm_drv.base);
			return ret;
		}
		/*  Initialize the memory */
		ret = (res.end != res.start) ? (res.end - res.start + 1) : 0;
		memset_io(sm_drv.base, 0, ret);
		sm_drv.probed = 1;
	} else
		return -ENOMEM;

	sm_drv.pdev = pdev;

	return 0;
}

static int sm_drv_remove(struct platform_device *pdev)
{
	if (sm_drv.probed)
		/*  TODO: Probably should use the of_iounmap() variant */
		iounmap(sm_drv.base);

	return 0;
}

static void sm_sc_do_call(int command, long long val0, long long val1)
{
	if (likely(sm_drv.started)) {
		sm_drv.sc_command.cmd = command;
		sm_drv.sc_command.val0 = val0;
		sm_drv.sc_command.val1 = val1;
		wake_up_process(sm_drv.sc_handler);
		wait_for_completion(&sm_drv.sch_comp);
	}
}


/*
* Convention:
* - registers x0, x1 are used to pass up to two input parameters
* - call code is the smc instruction payload
* - register x0 is used for return value (if any)
* - register x1 is used for the output parameter (if any)
*
* registers x0 and x1 must be saved/restored before/after call
*/
static int sm_sc_worker(void *data)
{
	sm_drv_sc_prm_t *param = (sm_drv_sc_prm_t *)data;

	while (!kthread_should_stop()) {
		switch (param->cmd) {
		case SC_CMD_LL_INIT:
			/* Special case - we will destroy the x7 and x8
			* registers thus we save them too
			*/
			asm (
			"stp x0, x1, [sp, #-16]!;"
			"stp x7, x8, [sp, #-16]!;"
			"smc #0xffff;"
			"str x0, %0;"
			"str x1, %1;"
			"ldp x7, x8, [sp], #16;"
			"ldp x0, x1, [sp], #16;"
			: "=m"(param->val0), "=m"(param->val1) : );
			break;
		case SC_CMD_ENABLE_EVENTS:
			asm (
			"stp x0, x1, [sp, #-16]!;"
			"ldr x0, %1;"
			"ldr x1, %2;"
			"smc #0xfffd;"
			"str x0, %0;"
			"ldp x0, x1, [sp], #16;"
			: "=m"(param->val0) : "m"(param->val0),
			"m"(param->val1) : );
			break;
		case SC_CMD_SHUTDOWN:
			asm (
			"stp x0, x1, [sp, #-16]!;"
			"smc #0xfffc;"
			"str x0, %0;"
			"ldp x0, x1, [sp], #16;"
			: "=m"(param->val0) : : );
			break;
		case SC_CMD_INIT:
			asm (
			"stp x0, x1, [sp, #-16]!;"
			"smc #0x1001;"
			"str x0, %0;"
			"ldp x0, x1, [sp], #16;"
			: "=m"(param->val0) : : );
			break;
		case SC_CMD_START:
			asm (
			"stp x0, x1, [sp, #-16]!;"
			"smc #0x1002;"
			"str x0, %0;"
			"ldp x0, x1, [sp], #16;"
			: "=m"(param->val0) : : );
			break;
		case SC_CMD_STOP:
			asm (
			"stp x0, x1, [sp, #-16]!;"
			"smc #0x1003;"
			"str x0, %0;"
			"ldp x0, x1, [sp], #16;"
			: "=m"(param->val0) : : );
			break;
		case SC_CMD_CONFIG_SET:
			asm (
			"stp x0, x1, [sp, #-16]!;"
			"ldr x0, %1;"
			"ldr x1, %2;"
			"smc #0x1004;"
			"str x0, %0;"
			"ldp x0, x1, [sp], #16;"
			: "=m"(param->val0) : "m"(param->val0),
			"m"(param->val1) : );
			break;
		case SC_CMD_CONFIG_GET:
			asm (
			"stp x0, x1, [sp, #-16]!;"
			"ldr x0, %2;"
			"ldr x1, %3; ;"
			"smc #0x1005;"
			"str x0, %0;"
			"str x1, %1;"
			"ldp x0, x1, [sp], #16;"
			: "=m"(param->val0),
			"=m"(param->val1) : "m"(param->val0),
			"m"(param->val1) : );
			break;
		default:
			break;
		}

		set_current_state(TASK_UNINTERRUPTIBLE);
		complete(&sm_drv.sch_comp);
		schedule();
	}

	return 0;
}

static irqreturn_t sm_irq_handler(int irq, void *dev_id)
{
	#define SM_ASYNC_TEST			(1ULL << 0)
	#define SM_ASYNC_ERROR			(1ULL << 1)
	#define SM_ASYNC_IN			(1ULL << 2)
	#define SM_ASYNC_OUT			(1ULL << 3)
	#define SM_ASYNC_MESSAGE		(1ULL << 4)
	#define SM_ASYNC_DEBUG			(1ULL << 5)
	#define SM_ASYNC_PRIORITY		(1ULL << 6)

	sm_drv_sc_prm_t event;

#if (PLATFORM == S32V234)
	/*  Invoke the SM_CFG_LL_CONFIRM_EVENT to clear the interrupt and
	* get information about reason of the notification
	*/
	asm (
		"stp x0, x1, [sp, #-16]!;"
		"smc #0xfffe;"
		"str x0, %0;"
		"str x1, %1;"
		"ldp x0, x1, [sp], #16;"
		: "=m"(event.val0), "=m"(event.val1) : );
#endif /* PLATFORM */

	/* printk(KERN_INFO "Reason:	0x%llx\n", event.val0); */
	/* printk(KERN_INFO "UserInfo:  0x%llx\n", event.val1); */

	if (event.val0 == SM_ASYNC_DEBUG) {
		/*  TODO: Wake-up a debug-output thread */
		dev_info(sm_drv.device, "Operation not supported yet\n");
		return 0;
	}

	if (sm_drv.reg_pid.valid) {
		rcu_read_lock();
		if (unlikely(!sm_drv.reg_pid.p_task))
			sm_drv.reg_pid.p_task =
				pid_task(find_vpid(sm_drv.reg_pid.pid),
				PIDTYPE_PID);
		if (likely(sm_drv.reg_pid.p_task)) {
			struct siginfo sig_inf;

			memset(&sig_inf, 0, sizeof(struct siginfo));
			sig_inf.si_signo = SIGPOLL;
			switch (event.val0) {
			case SM_ASYNC_TEST:
				sig_inf.si_code = POLL_HUP;
				break;
			case SM_ASYNC_ERROR:
				sig_inf.si_code = POLL_ERR;
				break;
			case SM_ASYNC_IN:
				sig_inf.si_code = POLL_IN;
				break;
			case SM_ASYNC_OUT:
				sig_inf.si_code = POLL_OUT;
				break;
			case SM_ASYNC_MESSAGE:
				sig_inf.si_code = POLL_MSG;
				break;
			case SM_ASYNC_PRIORITY:
				sig_inf.si_code = POLL_PRI;
				break;
			default:
				dev_err(sm_drv.device,
					"Unknown event reason\n");
				break;
			}
			sig_inf.si_int = (int)(event.val1 & 0xffffffffU);
			if (send_sig_info(SIGPOLL,
						&sig_inf,
						sm_drv.reg_pid.p_task))
				dev_err(sm_drv.device,
					"send_sig_info failed\n");
		}
		rcu_read_unlock();
	}

	return 0;
}

static int __init init(void)
{
	int ret = 0;
	int cnt;
	struct file *f;
	void *buffer;
	unsigned long long offset = 0U, baddr;

	ret = platform_driver_register(&sm_platform_driver);
	if ((ret) < 0)
		return ret;

	if (sm_drv.probed)
		baddr = (unsigned long long)(sm_drv.base);
	else {
		ret = -ENXIO;
		goto finalize;
	}

/*  Install IRQ handler */
	ret = platform_get_irq(sm_drv.pdev, 0);
	if (ret < 0)
		goto finalize;

	ret = devm_request_irq(&sm_drv.pdev->dev,
				ret,
				sm_irq_handler,
				0,
				"sm_drv", NULL);
	if (ret) {
		ret = -ENODEV;
		goto finalize;
	}

/*  Create /dev entry */
	ret = alloc_chrdev_region(&sm_drv.devt, 0, 1, DEVICE_NAME);
	if ((ret) < 0) {
		printk(KERN_ERR
			"[sm_drv] Major/Minor number allocation failed\n");
		goto finalize;
	}

	sm_drv.class = class_create(THIS_MODULE, DEVICE_NAME);
	if (IS_ERR(sm_drv.class)) {
		printk(KERN_ERR "[sm_drv] Class pointer not received\n");
		ret = PTR_ERR(sm_drv.class);
		goto finalize1;
	}

	/*  TODO: Consider use of the "drvdata" argument */
	sm_drv.device = device_create(sm_drv.class,
					NULL,
					sm_drv.devt,
					NULL,
					DEVICE_NAME);
	if (IS_ERR(sm_drv.device)) {
		dev_err(sm_drv.device, "Device not created\n");
		ret = PTR_ERR(sm_drv.device);
		goto finalize2;
	}
	/* Initialize debug buffer - use 0 to values which are unknown now */
	sm_drv.dbgb.base = NULL;
	sm_drv.dbgb.size = 0U;
	for (cnt = 0U; cnt < FOP_MAX_OPEN_FILES; cnt++)
		sm_drv.dbgb.read_count[cnt].used = 0;
	sm_drv.dbgb.count = 0U;
	mutex_init(&(sm_drv.dbgb.mut));
/*  Register file operations */
	cdev_init(&sm_drv.cdev, &sm_fops);
	ret = cdev_add(&sm_drv.cdev, sm_drv.devt, 1);
	if (ret) {
		dev_err(sm_drv.device, "Char device not created\n");
		device_destroy(sm_drv.class, sm_drv.devt);
		goto finalize2;
	}

/*  Upload the binary */
	buffer = kmalloc(F_BUF_SIZE, GFP_KERNEL);
	if (unlikely(NULL == (buffer))) {
		ret = -ENOMEM;
		goto finalize3;
	}

	f = fopen(F_FILENAME, O_RDONLY, 0);
	if (!IS_ERR(f)) {
		do {
			cnt = fread(f,
					offset,
					(unsigned char *)buffer,
					F_BUF_SIZE);
			memcpy_toio((void *)(baddr + offset), buffer, cnt);
			offset += cnt;
		} while (cnt >= F_BUF_SIZE);

		dev_info(sm_drv.device,
			"%lld bytes from %s written @%llx\n",
			offset,
			F_FILENAME,
			baddr);
		fclose(f);
		kfree(buffer);
	} else {
		dev_err(sm_drv.device, "failed loading %s @%llx\n",
			F_FILENAME,
			baddr);
		ret = -EIO;	 /*  TODO: Should use "errno" */
		goto finalize4;
	}

/*  Start the worker thread */
	init_completion(&sm_drv.sch_comp);
	sm_drv.sc_handler = kthread_create(&sm_sc_worker,
						(void *)&sm_drv.sc_command,
						"sm_drv::ScWorker");
	if (IS_ERR(sm_drv.sc_handler)) {
		sm_drv.sc_handler = NULL;
		ret = -EAGAIN;
		goto finalize4;
	} else {
		kthread_bind(sm_drv.sc_handler, 0);

		sm_drv.started = 1;
		sm_sc_do_call(SC_CMD_LL_INIT, 0, 0);
		if (sm_drv.sc_command.val0 < 0) {
			dev_err(sm_drv.device,
			"Low-level initialization failed: 0x%x\n",
			(int)(sm_drv.sc_command.val0 & 0xffffffff));
			ret = -EIO;
			goto finalize3;
		}
		/* Get the debug buffer address and size */
		sm_drv.dbgb.base_phys = sm_drv.sc_command.val1;
		sm_drv.dbgb.size = sm_drv.sc_command.val0;
		if (unlikely(sm_drv.dbgb.base_phys)) {
			sm_drv.dbgb.base = ioremap(
						sm_drv.dbgb.base_phys,
						sm_drv.dbgb.size + 4U);
			if (unlikely(IS_ERR(sm_drv.dbgb.base))) {
				dev_err(sm_drv.device,
				"Could not remap debug buffer address %llu\n",
				sm_drv.dbgb.base_phys);
				goto finalize6;
			}
			/* We know the pointer is correctly aligned */
			sm_drv.dbgb.write_countp =
					sm_drv.dbgb.base + sm_drv.dbgb.size;
		} else {
			dev_err(sm_drv.device,
				"Invalid address of debug buffer\n");
			ret = -EFAULT;
			goto finalize7;
		}

#if TRUE == ENABLE_EVENTS_ON_LOAD
		sm_sc_do_call(SC_CMD_ENABLE_EVENTS, 1, 0);
#endif /* ENABLE_EVENTS_ON_LOAD */
#if TRUE == INIT_ON_LOAD
		sm_sc_do_call(SC_CMD_INIT, 0, 0);
#endif /* INIT_ON_LOAD */
#if TRUE == START_ON_LOAD
		sm_sc_do_call(SC_CMD_START, 0, 0);
#endif /* START_ON_LOAD */
	}

	return ret;
finalize7:

finalize6:

finalize4:
	kfree(buffer);
finalize3:
	kthread_stop(sm_drv.sc_handler);
	device_destroy(sm_drv.class, sm_drv.devt);
finalize2:
	class_destroy(sm_drv.class);
finalize1:
	unregister_chrdev_region(sm_drv.devt, 1);
finalize:
	platform_driver_unregister(&sm_platform_driver);
	sm_drv.started = 0;
	return ret;
}

static void __exit cleanup(void)
{
#if TRUE == STOP_ON_UNLOAD
	sm_sc_do_call(SC_CMD_STOP, 0, 0);
#endif /* STOP_ON_UNLOAD */
	sm_sc_do_call(SC_CMD_SHUTDOWN, 0, 0);

	if (sm_drv.sc_handler) {
		int retval = kthread_stop(sm_drv.sc_handler);

		if (-EINTR == retval)
			dev_warn(sm_drv.device, "No such process\n");
		else if (retval)
			dev_warn(sm_drv.device,
				"Unexpected result from worker\n");
	}

	mutex_destroy(&(sm_drv.dbgb.mut));
	cdev_del(&sm_drv.cdev);

	if ((sm_drv.device != NULL) && (sm_drv.class != NULL))
		device_destroy(sm_drv.class, sm_drv.devt);
	if (sm_drv.class != NULL)
		class_destroy(sm_drv.class);

	unregister_chrdev_region(sm_drv.devt, 1);
	iounmap(sm_drv.dbgb.base);
	platform_driver_unregister(&sm_platform_driver);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LM");
MODULE_DESCRIPTION("SM_DRV");


module_init(init);
module_exit(cleanup);

/** @}*/
