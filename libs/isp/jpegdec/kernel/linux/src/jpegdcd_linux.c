/*
 * Copyright (c) 2012-2016 Freescale Semiconductor
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
#include <linux/kernel.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/clk.h>
#include <linux/of_platform.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <video/of_display_timing.h>
#include <video/videomode.h>
#include <linux/pm_runtime.h>

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/of_net.h>
#include <linux/cdev.h>
#include <asm/current.h>
#include <asm/segment.h>
#include <linux/uaccess.h>

#include "jpegdcd_linux.h"
#include "jpegdcd_types.h"
#include "jpegdcd_core.h"


/**********************************************************
 * JPEG Decoder defines
 **********************************************************/
#define DEVICE_NAME "jpegdcd"
#define DRIVER_NAME "jpegdecoder"

#define PER_CLK "per"
#define IPG_CLK "ipg"
#define B_CLK   "bclock"


/**********************************************************
 * Macros for tracing
 **********************************************************/
#define JPEGDCD_INFO    KERN_INFO
#define __LOG_TRACE__   0

#if __LOG_TRACE__
    #define __TRACE__() printk(JPEGDCD_INFO "[ JPEGDCD ] %s\n", __func__)
    #define __MSG_TRACE__(string, args...) printk(JPEGDCD_INFO \
        "[ JPEGDCD ]  %s : %d : " string, __func__, __LINE__, ##args)
#else
    #define __TRACE__()
    #define __MSG_TRACE__(string, args...)
    #define __HERE__ printk(JPEGDCD_INFO " HERE %s\n", __func__)
#endif /* __LOG_TRACE__ */


/**********************************************************
 * GLOBAL structures & variables
 **********************************************************/
void __iomem    *jpegdcd_reg_base;

 struct jpegdcd_data {
    struct cdev chdev;
    uint32_t    irq;
    struct clk  *per_clk;
    struct clk  *ipg_clk;
    struct clk  *b_clk;
    atomic_t    access;
};

static struct class         *jpegdcd_class;
static dev_t                jpegdcd_devn; /*__u32 */


/*============================================================================*/
/**
* @brief          The function manipulates the underlying device parameters.
* @details        The function manipulates the underlying device parameters of
*                 special file. Many operating characteristics of character
*                 special file may be controlled with jpegdcd_ioctl() requests.
*
* @param[in]      pfile         file pointer
* @param[in]      ioctl_cmd     ioctl command
* @param[in]      arg           arg number
* @return         long
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/
static long jpegdcd_ioctl(struct file *pfile, unsigned int ioctl_cmd, unsigned long arg)
{
    int ret = 0;

    JPEGDCD_REGS *reg_base = (JPEGDCD_REGS *)jpegdcd_reg_base;

    __TRACE__();

    switch (ioctl_cmd)
    {
        case JPEGDCD_IOCTL_IRQ_CONTROL:
        {
            uint32_t interrupt_mask;

            ret = copy_from_user(&interrupt_mask, (uint32_t *)arg, sizeof(interrupt_mask));

            if (ret)
            {
                return ret;
            }

            JPEGdcd_interrupt_ctrl(reg_base, interrupt_mask);
        }
        break;

        case JPEGDCD_IOCTL_FIFO_STATUS_GET:
        {
            STREAM_FIFO_STATUS fifo_st;

            ret = copy_from_user(&fifo_st, (STREAM_FIFO_STATUS *)arg, sizeof(fifo_st));

            if (ret)
            {
                return ret;
            }

            JPEGdcd_fifostatus_get(reg_base, &fifo_st);

            ret = copy_to_user((STREAM_FIFO_STATUS *)arg, &fifo_st, sizeof(fifo_st));
        }
        break;

        case JPEGDCD_IOCTL_FIFO_DATA_GET:
        {
            STREAM_DATA_CONFIG data_cfg;

            ret = copy_from_user(&data_cfg, (STREAM_DATA_CONFIG *)arg, sizeof(data_cfg));

            if (ret)
            {
                return ret;
            }

            JPEGdcd_fifodata_get(reg_base, &data_cfg);

            ret = copy_to_user((STREAM_DATA_CONFIG *)arg, &data_cfg, sizeof(data_cfg));
        }
        break;

        case JPEGDCD_IOCTL_FIFO_WATERMARK_SET:
        {
            uint8_t wtmark;

            ret = copy_from_user(&wtmark, (uint8_t *)arg, sizeof(wtmark));

            if (ret)
            {
                return ret;
            }

            JPEGdcd_fifo_wtmklevel_set(reg_base, wtmark);
        }
        break;

        case JPEGDCD_IOCTL_OUTSTREAM_CFG:
        {
            OUTPUT_BUFFCFG output_cfg;

            ret = copy_from_user(&output_cfg, (OUTPUT_BUFFCFG *)arg, sizeof(output_cfg));

            if (ret)
            {
                return ret;
            }

            JPEGdcd_output_cfg(reg_base, &output_cfg);
        }
        break;

        case JPEGDCD_IOCTL_OUTSAMPLEMODE_SET:
        {
            JPEGDCD_BOOL outs_mode;
            ret = copy_from_user(&outs_mode, (JPEGDCD_BOOL *)arg, sizeof(outs_mode));

            if (ret)
            {
                return ret;
            }

            JPEGdcd_outsamplemode_set(reg_base, outs_mode);
        }
        break;

        case JPEGDCD_IOCTL_BANKSTRIDE_SET:
        {
            uint8_t bytes_n;

            ret = copy_from_user(&bytes_n, (uint8_t *)arg, sizeof(bytes_n));

            if (ret)
            {
                return ret;
            }

            JPEGdcd_bankstride_set(reg_base, bytes_n);
        }
        break;

        case JPEGDCD_IOCTL_BANKSTRIDE_GET:
        {
            uint8_t bytes_n;
            JPEGdcd_bankstride_get(reg_base, &bytes_n);

            ret = copy_to_user((uint8_t *)arg, &bytes_n, sizeof(bytes_n));
        }
        break;

        case JPEGDCD_IOCTL_JPEG_CFGSTART:
        {
            JPEGdcd_jpeg_configstart(reg_base);
        }
        break;

        case JPEGDCD_IOCTL_JPEG_TIMEOUT_SET:
        {
            uint32_t jpeg_err_to;

            ret = copy_from_user(&jpeg_err_to, (uint32_t *)arg, sizeof(jpeg_err_to));

            if (ret)
            {
                return ret;
            }

            JPEGdcd_jpeg_timeoutset(reg_base, jpeg_err_to);
        }
        break;

        case JPEGDCD_IOCTL_JPEG_DCDENABLE:
        {
            JPEGDCD_BOOL jpeg_ctrl;

            ret = copy_from_user(&jpeg_ctrl, (JPEGDCD_BOOL *)arg, sizeof(jpeg_ctrl));

            if (ret)
            {
                return ret;
            }

            JPEGdcd_jpeg_dcdenable(reg_base, jpeg_ctrl);
        }
        break;

        case JPEGDCD_IOCTL_JPEG_STREAMCFG_SET:
        {
            STREAM_JPEG_PARAM jpeg_ctrl;

            ret = copy_from_user(&jpeg_ctrl, (STREAM_JPEG_PARAM *)arg, sizeof(jpeg_ctrl));

            if (ret)
            {
                return ret;
            }

            JPEGdcd_jpeg_streamdatacfg(reg_base, &jpeg_ctrl);
        }
        break;

        case JPEGDCD_IOCTL_JPEG_STREAMNUM_SET:
        {
            uint8_t num_of_streams;

            ret = copy_from_user(&num_of_streams, (uint8_t *)arg, sizeof(num_of_streams));

            if (ret)
            {
                return ret;
            }

            JPEGdcd_jpeg_streamnum_set(reg_base, num_of_streams);
        }
        break;

        case JPEGDCD_IOCTL_JPEG_STREAMNUM_GET:
        {
            uint8_t num_of_streams;
            JPEGdcd_jpeg_streamnum_get(reg_base, &num_of_streams);

            ret = copy_to_user((uint8_t *)arg, &num_of_streams, sizeof(num_of_streams));
        }
        break;

        case JPEGDCD_IOCTL_JPEG_MODE_SET:
        {
            COMPAT_MODE jpeg_mode;

            ret = copy_from_user(&jpeg_mode, (COMPAT_MODE *)arg, sizeof(jpeg_mode));

            if (ret)
            {
                return ret;
            }

            JPEGdcd_jpeg_mode_set(reg_base, jpeg_mode);
        }
        break;

        case JPEGDCD_IOCTL_JPEG_MODE_GET:
        {
            COMPAT_MODE jpeg_mode;
            JPEGdcd_jpeg_mode_get(reg_base, &jpeg_mode);

            ret = copy_to_user((COMPAT_MODE *)arg, &jpeg_mode, sizeof(jpeg_mode));
        }
        break;

        case JPEGDCD_IOCTL_JPEG_SW_RESET:
        {
            JPEGdcd_jpeg_swreset(reg_base);
        }
        break;

        case JPEGDCD_IOCTL_JPEG_STREAM_GET:
        {
            STREAM_ID str_n;
            JPEGdcd_jpeg_streamactive_get(reg_base, &str_n);

            ret = copy_to_user((STREAM_ID *)arg, &str_n, sizeof(str_n));
        }
        break;

        case JPEGDCD_IOCTL_JPEG_RSTINTERVAL_SET:
        {
            uint16_t num_of_lines;

            ret = copy_from_user(&num_of_lines, (uint16_t *)arg, sizeof(num_of_lines));

            if (ret)
            {
                return ret;
            }

            JPEGdcd_jpeg_rstinterval_set(reg_base, num_of_lines);
        }
        break;

        case JPEGDCD_IOCTL_JPEG_RSTINTERVAL_GET:
        {
            uint16_t num_of_lines;
            JPEGdcd_jpeg_rstinterval_get(reg_base, &num_of_lines);

            ret = copy_to_user((uint16_t *)arg, &num_of_lines, sizeof(num_of_lines));
        }
        break;

        case JPEGDCD_IOCTL_JPEG_FRAMESTATUS_GET:
        {
            JPEGDCD_FRAMEST frame_st;
            JPEGdcd_jpeg_framest_get(reg_base, &frame_st);

            ret = copy_to_user((JPEGDCD_FRAMEST *)arg, &frame_st, sizeof(frame_st));
        }
        break;

        case JPEGDCD_IOCTL_JPEG_DCDCFGERR_GET:
        {
            JPEGDCD_CFGERR jpeg_errst;
            JPEGdcd_jpeg_cfgerr_get(reg_base, &jpeg_errst);

            ret = copy_to_user((JPEGDCD_CFGERR *)arg, &jpeg_errst, sizeof(jpeg_errst));
        }
        break;

        case JPEGDCD_IOCTL_JPEG_MCUROWEND_GET:
        {
            JPEGDCD_BOOL mcurow_end_count;
            JPEGdcd_jpeg_mcurow_get(reg_base, &mcurow_end_count);

            ret = copy_to_user((JPEGDCD_BOOL *)arg, &mcurow_end_count, sizeof(mcurow_end_count));
        }
        break;

        case JPEGDCD_IOCTL_JPEG_DCDIMAGE_GET:
        {
            JPEGDCD_DECODED_IMAGE dcd_image;
            JPEGdcd_jpeg_dcdimage_get(reg_base, &dcd_image);

            ret = copy_to_user((JPEGDCD_DECODED_IMAGE *)arg, &dcd_image, sizeof(dcd_image));
        }
        break;

        case JPEGDCD_IOCTL_IRQ_GET:
        {
            uint32_t intr_mask;
            JPEGdcd_interrupt_get(reg_base, &intr_mask);

            ret = copy_to_user((uint32_t *)arg, &intr_mask, sizeof(intr_mask));
        }
        break;

        case JPEGDCD_IOCTL_INPUTSTREAM_CFG:
        {
            STREAM_DATA_CONFIG input_cfg;

            ret = copy_from_user(&input_cfg, (STREAM_DATA_CONFIG *)arg, sizeof(input_cfg));

            if (ret)
            {
                return ret;
            }

            JPEGdcd_inputstream_cfg(reg_base, &input_cfg);
        }
        break;

        default:
            break;
    }

    return ret;
}


/*============================================================================*/
/**
* @brief          The function jpegdcd open
* @details        The function opens special file for jpegdcd device
*
* @param[in]      inod      struct inode
* @param[in]      pfile     file open
* @return         int
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/
static int jpegdcd_open(struct inode *inod, struct file *pfile)
{
    __TRACE__();

    return 0;
}


/*============================================================================*/
/**
* @brief          The function jpegdcd close
* @details        The function closes special file for jpegdcd device
*
* @param[in]      inod      struct inode
* @param[in]      pfile     file to close
* @return         int
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/
static int jpegdcd_close(struct inode *inod, struct file *pfile)
{
    __TRACE__();

    return 0;
}


/**********************************************************
 * STRUCT operations
 **********************************************************/
static struct file_operations jpegdcd_fops = {
    .owner          = THIS_MODULE,
    .open           = jpegdcd_open,
    .release        = jpegdcd_close,
    .unlocked_ioctl         = jpegdcd_ioctl,
};


/*============================================================================*/
/**
* @brief          The function jpegdcd dev create
* @details        The function creates and registers jpegdcd device
*
* @return         int
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/
static int jpegdcd_dev_create(void)
{
    int ret = 0;

    __TRACE__();

    /* Alloc MAJOR number for the character and
    *  the 1st minor device in dev
    */
    ret = alloc_chrdev_region(&jpegdcd_devn, 0, 1, DRIVER_NAME);

    if (ret < 0)
    {
        printk(JPEGDCD_INFO "alloc_region failed: %d\n", ret);
        return ret;
    }

    jpegdcd_class = class_create(THIS_MODULE, DRIVER_NAME);

    if (IS_ERR(jpegdcd_class))
    {
        printk(JPEGDCD_INFO "class_create failed %d\n", ret);
        return -1;
    }

    /* Device is created and registered in sysfs */
    if (IS_ERR(device_create(jpegdcd_class, NULL,
            jpegdcd_devn, NULL, DEVICE_NAME)))
    {
        printk(JPEGDCD_INFO "device_create failed %d\n", ret);
        return ret;
    }

    return ret;
}


/*============================================================================*/
/**
* @brief          The function enables clocks for JPEG decoder device
* @details        Enables PER, IPG and B clock for JPEG decoder device driver
*
* @param[in]      pdev    struct platform device
* @return         int
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/
static int jpegdcd_enable_clocks(struct platform_device * const pdev)
{
    int ret;
    struct clk *jpegdcd_clk;
    struct jpegdcd_data *jpeg_data = dev_get_drvdata(&pdev->dev);

    /* PER clock */
    jpegdcd_clk = devm_clk_get(&pdev->dev, PER_CLK);

    if (IS_ERR(jpegdcd_clk))
    {
        dev_err(&pdev->dev, "could not get \"%s\" clock\n", PER_CLK);
        return -EFAULT;
    }
    ret = clk_prepare_enable(jpegdcd_clk);

    if (ret)
        return -EFAULT;
    jpeg_data->per_clk = jpegdcd_clk;

    /* IPG clock */
    jpegdcd_clk = devm_clk_get(&pdev->dev, IPG_CLK);

    if (IS_ERR(jpegdcd_clk))
    {
        dev_err(&pdev->dev, "could not get \"%s\" clock\n", IPG_CLK);
        return -EFAULT;
    }
    ret = clk_prepare_enable(jpegdcd_clk);

    if (ret)
        return -EFAULT;
    jpeg_data->ipg_clk = jpegdcd_clk;

    /* B clock */
    jpegdcd_clk = devm_clk_get(&pdev->dev, B_CLK);

    if (IS_ERR(jpegdcd_clk))
    {
        dev_err(&pdev->dev, "could not get \"%s\" clock\n", B_CLK);
        return -EFAULT;
    }
    ret = clk_prepare_enable(jpegdcd_clk);

    if (ret)
    {
        return -EFAULT;
    }
    jpeg_data->b_clk = jpegdcd_clk;

    return 0;
}


/*============================================================================*/
/**
* @brief          The function jpegdcd probe
* @details        The function jpegdcd probe
*
* @param[in]      pdev      struct platform device
* @return         int
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/
static int jpegdcd_probe(struct platform_device * const pdev)
{
    struct resource *res;
    struct jpegdcd_data  *jpeg_data;

    int ret;

    __TRACE__();

    /* create device and register it in /dev through sysfs */
    ret = jpegdcd_dev_create();

    if (ret < 0)
    {
        return -ENODEV;
    }

    /* map register space */
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

    if (res == NULL)
    {
        dev_err(&pdev->dev, "could not get memory IO resource\n");
        return -ENODEV;
    }

    jpeg_data = devm_kzalloc(&pdev->dev, sizeof(struct jpegdcd_data), GFP_KERNEL);
    dev_set_drvdata(&pdev->dev, jpeg_data);

    /* setup file operations */
    cdev_init(&(jpeg_data->chdev), &jpegdcd_fops);
    ret = cdev_add(&(jpeg_data->chdev), jpegdcd_devn, 1);

    if (ret < 0)
    {
        printk(JPEGDCD_INFO "cdev_add failed with %d\n", ret);
        return -ENODEV;
    }

    /* restore ret to 0 since cdev_add returns a negative error code in case of failure */
    ret = 0;

    jpegdcd_reg_base = devm_ioremap_resource(&pdev->dev, res);

    if (IS_ERR(jpegdcd_reg_base))
    {
        dev_err(&pdev->dev, "could not ioremap resource\n");
        return -ENOMEM;
    }

    /* enable clocks for JPEG */
    ret = jpegdcd_enable_clocks(pdev);

    if (ret < 0)
    {
        return ret;
    }

    pm_runtime_enable(&pdev->dev);
    ret = pm_runtime_get_sync(&pdev->dev);

    __TRACE__();

    return ret;
}


/*============================================================================*/
/**
* @brief          The function jpegdcd remove
* @details        The function cleans jpegdcd resources
*
* @param[in]      pdev      struct platform device
* @return         int
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/
static int jpegdcd_remove(struct platform_device * const pdev)
{
    int ret;
    struct jpegdcd_data *jpeg_data = dev_get_drvdata(&pdev->dev);

    __TRACE__();

    /* Disable clocks */
    clk_disable_unprepare(jpeg_data->per_clk);
    clk_disable_unprepare(jpeg_data->ipg_clk);
    clk_disable_unprepare(jpeg_data->b_clk);

    jpeg_data = dev_get_drvdata(&pdev->dev);
    cdev_del(&(jpeg_data->chdev));

    device_destroy(jpegdcd_class, jpegdcd_devn);
    class_destroy(jpegdcd_class);
    unregister_chrdev_region(jpegdcd_devn, 1);

    ret = pm_runtime_put_sync(&pdev->dev);

    if (!ret)
    {
        pm_runtime_disable(&pdev->dev);
    }

    return ret;
}


/**********************************************************
 * DCU Linux DTB query related operations
 **********************************************************/
static struct of_device_id jpegdcd_dt_ids[] = {
    {
        .compatible = "fsl,s32v234-jpegdcd",
    },
    {}
};


/*============================================================================*/
/**
* @brief          The function jpegdcd runtime suspend
* @details        The function jpegdcd runtime suspend
*
* @param[in]      pdev      struct platform device
* @return         int
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/
static int jpegdcd_runtime_suspend(struct device *dev)
{
    struct jpegdcd_data *jpeg_data = dev_get_drvdata(dev);

    clk_disable_unprepare(jpeg_data->per_clk);
    clk_disable_unprepare(jpeg_data->ipg_clk);
    clk_disable_unprepare(jpeg_data->b_clk);

    return 0;
}


/*============================================================================*/
/**
* @brief          The function jpegdcd runtime resume
* @details        The function jpegdcd runtime resume
*
* @param[in]      pdev      struct platform device
* @return         int
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/
static int jpegdcd_runtime_resume(struct device *dev)
{
    int ret;
    struct jpegdcd_data *jpeg_data = dev_get_drvdata(dev);

    ret = clk_prepare_enable(jpeg_data->per_clk);

    if (ret)
    {
        return ret;
    }
    ret = clk_prepare_enable(jpeg_data->ipg_clk);

    if (ret)
    {
        return ret;
    }

	ret = clk_prepare_enable(jpeg_data->b_clk);
    return ret;
}

static struct dev_pm_ops jpegdcd_pm_ops = {
    SET_RUNTIME_PM_OPS(jpegdcd_runtime_suspend,
            jpegdcd_runtime_resume, NULL)
};

static struct platform_driver jpegdcd_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .owner = THIS_MODULE,
        .of_match_table = jpegdcd_dt_ids,
        .pm = &jpegdcd_pm_ops,
    },
    .probe = jpegdcd_probe,
    .remove = jpegdcd_remove,
};

module_platform_driver(jpegdcd_driver);

MODULE_AUTHOR("Tomescu Cristian Corneliu");
MODULE_DESCRIPTION("Freescale JPEG Decoder driver");
MODULE_LICENSE("GPL");