##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2016, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    full featured Makefile for arm gcc using nbuild packages.
#    internal use only
#
#  AUTHOR
#    Mihail Nistor
#
#  DATE
#    2016-09-22
#
#  LANGUAGE
#    make
#
##############################################################################
# nbuild packages
SUB_EXTERN_DEP ?= 0
ifeq ($(origin APU_COMP), undefined)
APU_COMP := tct
export APU_COMP
endif
ifeq ($(origin APU_PRECOMP), undefined)
APU_PRECOMP := 0
export APU_PRECOMP
endif

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


all allsub clean cleansub::
ifeq ($(APU_PRECOMP),0)
	@if [ -d ../graphs/build-apu-$(APU_COMP)-sa-d ]; then \
	  if [ $(SUB_EXTERN_DEP) -eq 1 ]; then \
	    $(MAKE) -C $(CURDIR)/../graphs/build-apu-$(APU_COMP)-sa-d SUB_EXTERN_DEP=0 $@sub; \
	  else \
	    $(MAKE) -C $(CURDIR)/../graphs/build-apu-$(APU_COMP)-sa-d SUB_EXTERN_DEP=0 $@; \
	  fi; \
	  if [ $$? != 0 ]; then \
	    kill $$$$; exit 1; \
	  fi; \
	fi; \
	$(MAKE) -f $(CURR_SDK_ROOT)/build/nbuild/platforms/$(notdir $(CURDIR))/Makefile SUB_EXTERN_DEP=0 $@
else
	@$(MAKE) -f $(CURR_SDK_ROOT)/build/nbuild/platforms/$(notdir $(CURDIR))/Makefile SUB_EXTERN_DEP=0 $@
endif

%::
	@$(MAKE) -f $(CURR_SDK_ROOT)/build/nbuild/platforms/$(notdir $(CURDIR))/Makefile SUB_EXTERN_DEP=0 $@

