##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2011, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#
#
#  AUTHOR
#    Stefan Schwarz
#
#  DATE
#    2015-01-08
#
#  LANGUAGE
#    make
#
##############################################################################
OS_TYPE := None

C5_APP := $(IPUV_APP)
C5_APP_SRCS := $(IPUV_APP_SRCS)

C5_DEFS := $(IPUV_DEFS)
C5_INCS := $(IPUV_INCS)

##############################################################################

C5_DEFS +=
C5_INCS +=
C5_ASOPTS +=
C5_FCLEAN := *.su

ifeq ($(NBUILD_PLATFORM), msys)
  C5_AS := $(SDK_ROOT)/tools/isp/ipu/as-IPUV.exe
  C5_OD := $(SDK_ROOT)/tools/isp/ipu/objdump-IPUV.exe
else
  C5_AS := $(SDK_ROOT)/tools/isp/ipu/as-IPUV
  C5_OD := $(SDK_ROOT)/tools/isp/ipu/objdump-IPUV
endif

C5_SR2ROM := $(SDK_ROOT)/tools/sr2rom/sr2rom

C5_PROMPT := ipuv/sa

include $(SDK_ROOT)/build/nbuild/.C5.mk
