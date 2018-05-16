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
#
#
#  AUTHOR
#    Stefan Schwarz
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

ignore:

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

##############################################################################
# Standard search path for sources
##############################################################################
VPATH += \
		../src

# nbuild packages
include		$(SDK_ROOT)/build/nbuild/toolchains/arm-none-linux-gnueabi.mk
include         $(SDK_ROOT)/build/nbuild/toolchains/arm-sa-ghs-m0.mk
include         $(SDK_ROOT)/build/nbuild/sub.mk
