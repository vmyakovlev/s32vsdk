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
#ifndef IOCTLCMD_H
#define IOCTLCMD_H

#include <linux/fs.h>

/**
 * The call forces vdma to paint back buffer and returns the new back buffer
 * index in argument
 */
#define CMD_PAINT 				_IOW('m',  0, int)

/**
 * The call returns the current front buffer index in argument
 */
#define CMD_GET_DISPLAYED			_IOW('m',  1, int)

/**
 * The call returns the current back buffer index in argument
 */
#define CMD_GET_BACK 				_IOW('m',  2, int)

/**
 * Function clear both buffers
 */
#define CMD_CLR_BUFFERS				_IOW('m',  3, int)

/**
 * HDMI mode on/off
 */
#define CMD_SET_HDMI				_IOW('m',  4, int)

/**
 * TBD
 */
//#define CMD_SET_WIDTH 				_IOW('m',  3, int)
//#define CMD_SET_HEIGHT 				_IOW('m',  4, int)
//#define CMD_SET_TPG 				_IOW('m',  5, int)
//#define CMD_SET_FRAMEBUFFER			_IOW('m',  6, int)

#endif /* IOCTLCMD_H */
