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
#    Norbert Stoeffler
#
#  DATE
#    2011-02-24
#
#  LANGUAGE
#    make
#
##############################################################################

C0_CC := gcc -c
C0_CX := g++ -c
C0_AS := gcc -c
C0_LD := gcc
C0_AR := ar rcs

C0_PROMPT := desk/x11

# compiler specific options for core
C0_CCOPTS += -Wno-write-strings
C0_CXOPTS +=
C0_ASOPTS +=
C0_LDOPTS += -L$(XLIBPATH) -lX11 -lstdc++ 


include $(SDK_ROOT)/build/nbuild/.C0.mk
