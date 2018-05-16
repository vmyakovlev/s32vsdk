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

C0_CCOPTS += -G
C0_CXOPTS += -G 
C0_AROPTS += -G
C0_LDOPTS += -G

C0_PROMPT := arm/ghs/debug

include $(SDK_ROOT)/build/nbuild/.C0_integrity.mk

