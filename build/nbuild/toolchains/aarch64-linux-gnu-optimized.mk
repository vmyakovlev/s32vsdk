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
#    2013-08-05
#
#  LANGUAGE
#    make
#
##############################################################################

include $(SDK_ROOT)/build/nbuild/toolchains/aarch64-linux-gnu.mk

C0_DEFS += \
  -D NDEBUG

C0_CCOPTS += \
  -O3 \

C0_CXOPTS += \
  -O3 \

C0_PROMPT := arm64/gnu/optimized

include $(SDK_ROOT)/build/nbuild/.C0.mk
