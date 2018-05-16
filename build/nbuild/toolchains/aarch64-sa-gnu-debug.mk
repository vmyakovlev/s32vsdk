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
#    Rostislav Hulik
#    Mihail Marian Nistor
#
#  DATE
#    2017-03-24
#
#  LANGUAGE
#    make
#
##############################################################################

include $(SDK_ROOT)/build/nbuild/toolchains/aarch64-sa-gnu.mk

C0_CCOPTS += \
  -ggdb3

C0_CXOPTS += \
  -ggdb3

C0_ASOPTS += -ggdb3

C0_LDOPTS += -ggdb3

C0_PROMPT := arm64/sa/debug

include $(SDK_ROOT)/build/nbuild/.C0.mk
