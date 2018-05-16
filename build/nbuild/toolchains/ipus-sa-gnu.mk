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

C4_APP := $(IPUS_APP)
C4_APP_SRCS := $(IPUS_APP_SRCS)

C4_DEFS := $(IPUS_DEFS)
C4_INCS := $(IPUS_INCS)

##############################################################################
C4_DEFS +=
C4_INCS +=
C4_ASOPTS +=
C4_FCLEAN := *.su

ifeq ($(NBUILD_PLATFORM), msys)
  C4_AS := $(SDK_ROOT)/tools/isp/ipu/as-IPUS.exe
  C4_OD := $(SDK_ROOT)/tools/isp/ipu/objdump-IPUS.exe
else
  C4_AS := $(SDK_ROOT)/tools/isp/ipu/as-IPUS
  C4_OD := $(SDK_ROOT)/tools/isp/ipu/objdump-IPUS
endif

C4_SR2ROM := $(SDK_ROOT)/tools/sr2rom/sr2rom

C4_PROMPT := ipus/sa

include $(SDK_ROOT)/build/nbuild/.C4.mk
