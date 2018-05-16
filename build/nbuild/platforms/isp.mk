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

VPATH += ../graphs

##############################################################################
# check required parameters
##############################################################################

ifeq ($(origin SDK_ROOT),undefined)
  $(error error: SDK_ROOT undefined)
endif

################## ISP firmware and graph, IPU ###############################
ifneq ($(strip $(SEQ_APP) $(SEQ_LIB)),)
ifeq ($(TOOLCH),ghs)	### GHS ###########################
include $(SDK_ROOT)/build/nbuild/toolchains/arm-sa-ghs-m0.mk
else	### GCC ###########################################
include $(SDK_ROOT)/build/nbuild/toolchains/arm-sa-gnu-m0.mk
endif # TOOLCH
endif # SEQ_APP SEQ_LIB

ifneq ($(IPUS_APP),)
	include $(SDK_ROOT)/build/nbuild/toolchains/ipus-sa-gnu.mk
endif

ifneq ($(IPUV_APP),)
	include $(SDK_ROOT)/build/nbuild/toolchains/ipuv-sa-gnu.mk
endif

ifneq ($(strip $(IPUS_APP) $(IPUV_LIB)),)
	include $(SDK_ROOT)/build/nbuild/isp_tools/isp-graphgen.mk
endif
##############################################################################
