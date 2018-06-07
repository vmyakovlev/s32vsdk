/*********************************************************************
* Copyright (C) 2014, 2016 by Freescale, Inc.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
 *********************************************************************/

#ifndef FB_UTIL_H_
#define FB_UTIL_H_

/*****************************************
 * Standard C and Linux FB related
 *****************************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <stdint.h>
#include <sys/mman.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <sys/user.h>

#include "uart_to_mcu.h"

/*****************************************
 * FB-DCU UAPI IOCTL
 *****************************************/
#include "video/fsl_dcu_ioctl.h"

/*****************************************
 * FB-DCU related defines
 *****************************************/

#define FB_RESX			1920
#define FB_RESY			1080

/* max layers, equivalent to /dev/fbX devices */
#define	FB_NUM_LAYERS	8

/* max blending capability at any point */
#define	FB_ACTIVE		2
#define DCU_NUM			1

/*****************************************
 * FB-DCU extern variables
 *****************************************/

extern char *fb_ptr[FB_NUM_LAYERS];
extern int fb_fd[FB_NUM_LAYERS];
extern int fb_screensize[FB_NUM_LAYERS];

 /*****************************************
 * FB-DCU helper functions
 *****************************************/
int setup_fb_layers(int num_fb_active,
		int fb_resx,
		int fb_resy,
		int fb_bpp,
		int fb_tiled);

int setup_fb_layers_db(int num_fb_active,
		int fb_resx,
		int fb_resy,
		int fb_bpp);

int clean_fb_layers(int num_fb_active);

/*****************************************
 * DIE MACRO
 * Force exit program on critical error
 *****************************************/
#define DIE(assertion, call_description)	\
do {\
if (assertion) { \
		fprintf(stderr, "(%s, %d): ",	\
		__FILE__, __LINE__);		\
		perror(call_description);	\
		exit(1);		\
}					\
} while (0)


#ifdef __cplusplus
extern "C" {
#endif
 
void *Gui_meg_thread(void *t) ;
void *Gui_draw_thread(void *t) ;

 
#ifdef __cplusplus
}
#endif


#endif /* FB_UTIL_H_ */

