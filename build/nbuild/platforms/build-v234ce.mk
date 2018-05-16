##############################################################################
#
#  Copyright 2017 NXP;
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    full featured Makefile for arm gcc using nbuild packages.
#    internal use only
#
#  AUTHOR
#    Mihail Marian Nistor
#
#  DATE
#    2017-03-21
#
#  LANGUAGE
#    make
#
##############################################################################
# nbuild packages

##############################################################################
# check required parameters
##############################################################################

ifeq ($(origin SDK_ROOT),undefined)
  $(error error: SDK_ROOT undefined)
endif

ifeq ($(origin AARCH64_TOOLCH),undefined)
  $(error error: AARCH64_TOOLCH undefined)
endif

ifeq ($(origin AARCH64_3RDPARTY_DIR),undefined)
  $(error error: AARCH64_3RDPARTY_DIR undefined)
endif

OPENCV_ROOT = $(SDK_ROOT)/3rdparty/ocv/$(AARCH64_3RDPARTY_DIR)
FFMPEG_ROOT = $(SDK_ROOT)/3rdparty/ffmpeg/$(AARCH64_3RDPARTY_DIR)

ifneq ($(AVB_APP_TYPE),)
	include $(SDK_ROOT)/build/nbuild/avb_tools/avb-firmware-build.mk
endif
################## ISP firmware and graph, IPU ###############################
include $(SDK_ROOT)/build/nbuild/platforms/isp.mk
##############################################################################

ifneq ($(strip $(ARM_APP) $(ARM_LIB)),)
	include $(SDK_ROOT)/build/nbuild/toolchains/$(AARCH64_TOOLCH)
endif

include $(SDK_ROOT)/build/nbuild/sub.mk
