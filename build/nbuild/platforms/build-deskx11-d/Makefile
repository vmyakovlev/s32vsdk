##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2011, Freescale Semiconductor, Inc.
#  Copyright 2017 NXP;
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    full featured Makefile for desk builds (linux/x11) using nbuild packages.
#    internal use only
#
#  AUTHOR
#    Norbert Stoeffler
#    Mihail Marian Nistor
#
#  DATE
#    2017-02-27
#
#  LANGUAGE
#    make
#
##############################################################################

all::

# The Makefile will set the current sdk root (CURR_SDK_ROOT) variable if it is not defined yet
# in current SHELL envirotment in the follwoing way:
# 1. try to find the */s32v234_sdk folder (Vision SDK root) in current tree directory and set it.
# 2. set to S32V234_SDK_ROOT envirotment variable if the above fails.
# 3. an error will be reported if the above fails too.
# NOTE:
#  - S32V234_SDK_ROOT varible points to the last Vision SDK installed. It supports the OS-style path.
#  - CURR_SDK_ROOT supports only Unix-style path.
ifeq ($(origin CURR_SDK_ROOT), undefined)
CURR_SDK_ROOT :=$(shell pwd | grep -o ".*/s32v234_sdk")
ifeq ($(CURR_SDK_ROOT),)
override CURR_SDK_ROOT := $(realpath $(S32V234_SDK_ROOT))
ifeq ($(CURR_SDK_ROOT),)
$(error The project is compiled out of Vision SDK tree directory. The S32V234_SDK_ROOT should be set to Vision SDK root directory.)
endif
endif
export CURR_SDK_ROOT
$(info Current SDK ROOT is $(CURR_SDK_ROOT))
endif

include $(CURR_SDK_ROOT)/build/nbuild/utilities.mk
include $(CURR_SDK_ROOT)/build/nbuild/platform.mk

include ../BUILD.mk

OPENCV_ROOT = $(SDK_ROOT)/3rdparty/ocv/linux-x86
FFMPEG_ROOT = $(SDK_ROOT)/3rdparty/ffmpeg/linux-x86

VPATH += ../src
VPATH += ../graphs

C0_APP =	$(ARM_APP)
C0_APP_SRCS:=	$(ARM_APP_SRCS)
C0_APP_LIBS:=	$(ARM_APP_LIBS)

C0_LIB =	$(ARM_LIB)
C0_LIB_SRCS:=	$(ARM_LIB_SRCS)
C0_LIB_LIBS:=	$(ARM_LIB_LIBS)

C0_DEFS :=	$(ARM_DEFS)
C0_INCS :=	$(ARM_INCS)

C0_LDOPTS =	$(ARM_LDOPTS)
C0_CCOPTS =	$(ARM_CCOPTS)
C0_CXOPTS =	$(ARM_CXOPTS)

##############################################################################

##############################################################################
# Legacy variables from CGV build environment
##############################################################################
GBOFILE = 	GBOpts_APEX2_Pseudo_FPGA_Valid
APPLICATION =	ICP
CHIP =		2301
SCAT =		SCAT16_APEX2
MKSDK =		NONE
##############################################################################

C0_INCS +=	-I.

run: all
	./$(ARM_APP)

# ACF Specific defines (may also go into BUILD.mk)

C0_DEFS +=	\
		-D__GNU__					\
		-D GBO_FILE=\"$(GBOFILE).h\"	\

# compiler specific options for platform, not in headdep
C0_CCOPTS += 	-g
ifeq ($(MK_WB_BUILD),)
C0_CXOPTS +=	-fno-rtti	-fpermissive		\
		-g
endif
C0_ASOPTS +=	-g

ifeq ($(NBUILD_PLATFORM),linux)
C0_LDOPTS +=	-lpthread -lrt -L$(OPENCV_ROOT)/lib/ -L$(FFMPEG_ROOT)/lib/
endif

# nbuild packages
include         $(SDK_ROOT)/build/nbuild/toolchains/x86-x11-linux-gnu.mk
include         $(SDK_ROOT)/build/nbuild/sub.mk
