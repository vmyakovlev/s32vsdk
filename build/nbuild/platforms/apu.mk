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

ifeq ($(origin APU_COMP),undefined)
  $(error error: APU_COMP undefined)
endif

VPATH += ../graphs

ifeq ($(X86_KERNEL_DB),"on")
	X86_APP = $(basename $(APU_LIB))
	X86_APP_SRCS = $(APU_LIB_SRCS)

	ifneq ($(findstring msys,$(NBUILD_PLATFORM)),)
		X86_APP_LIBS = $(SDK_ROOT)/libs/apex/acf_offline/build-x86-gnu-windows-d/libacf_offline.a
	else
		X86_APP_LIBS = $(SDK_ROOT)/libs/apex/acf_offline/build-x86-gnu-linux-d/libacf_offline.a
	endif

	X86_DEFS += -DACF_KERNEL_METADATA \
			--include apu2_kernel_db_lib.h
	X86_INCS += -I$(SDK_ROOT)/libs/apex/acf_offline/include \
			-I$(SDK_ROOT)/libs/apex/acf_target/include \
			-I$(SDK_ROOT)/libs/apex/common/include \
			-I$(SDK_ROOT)/libs/apex/common/include/driver

	X86_KERNEL_DB = "on"
endif

ifneq ($(strip $(X86_APP) $(X86_LIB)),)
	include $(SDK_ROOT)/build/nbuild/toolchains/x86-linux-gnu.mk
endif

ifneq ($(APU_GRAPHS),)
	include $(SDK_ROOT)/build/nbuild/apex_tools/apu-kernel-getgraphs.mk
endif

ifneq ($(ACL_KERNELS),)
	include $(SDK_ROOT)/build/nbuild/apex_tools/acl-kernel.mk
endif

ifeq ($(APU_LOAD_FROM_EXE),"on")
	include $(SDK_ROOT)/build/nbuild/apex_tools/apu-kernel-load.mk
endif

ifeq ($(X86_KERNEL_DB),"on")
	include $(SDK_ROOT)/build/nbuild/apex_tools/apu-kernel-database.mk
endif

ifneq ($(strip $(APU_APP) $(APU_LIB) $(APU_LOAD_BINS)),)
	include $(SDK_ROOT)/build/nbuild/toolchains/apu-acf-sa-$(APU_COMP).mk
endif

ifneq ($(strip $(ACL_APP) $(ACL_LIB) $(ACL_LOAD_BINS)),)
	include $(SDK_ROOT)/build/nbuild/toolchains/apu-acl-sa-$(APU_COMP).mk
endif

include $(SDK_ROOT)/build/nbuild/apex_tools/acl-get_md.mk