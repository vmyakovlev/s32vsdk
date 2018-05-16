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
#
#  DATE
#    2013-08-05
#
#  LANGUAGE
#    make
#
##############################################################################

include $(SDK_ROOT)/build/nbuild/toolchains/aarch64-linux-gnu.mk

C0_DEFS +=

C0_CCOPTS += \
  -O0 \
  -g

C0_CXOPTS += \
  -O0 \
  -g
 

C0_ASOPTS += -g

C0_LDOPTS += -g

C0_PROMPT := arm64/gnu/debug

include $(SDK_ROOT)/build/nbuild/.C0.mk
