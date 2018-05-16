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

include $(SDK_ROOT)/build/nbuild/toolchains/aarch64-integrity-ghs.mk

C0_CCOPTS += -Ospeed
C0_CXOPTS += -Ospeed
C0_AROPTS += -Ospeed
C0_LDOPTS += -Ospeed

C0_PROMPT := arm/ghs/optimized

include $(SDK_ROOT)/build/nbuild/.C0_integrity.mk

