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
#    2015-02-24
#
#  LANGUAGE
#    make
#
##############################################################################
C6_APP := $(SEQ_APP).elf
C6_APP_SRCS := $(SEQ_APP_SRCS)
C6_APP_LIBS := $(SEQ_APP_LIBS)

C6_LIB := $(SEQ_LIB)
C6_LIB_SRCS := $(SEQ_LIB_SRCS)
C6_LIB_LIBS := $(SEQ_LIB_LIBS)
C6_LIB_ARCS := $(SEQ_LIB_ARCS)

C6_DEFS := $(SEQ_DEFS)
C6_INCS := $(SEQ_INCS)
C6_CCOPTS = $(SEQ_CCOPTS)
C6_LDSCRIPT = $(SEQ_LDSCRIPT)

##############################################################################
PLATFORM = ISP
OSTYPE := SA

COMMON_OPTS = \
  -cpu=cortexm0 \
  -e _reset \
  -align8 \
  -D__GNUC__ \
  -srec \

C6_INCS += -I.
C6_CCOPTS += $(COMMON_OPTS)
C6_AROPTS += 
C6_CXOPTS += $(COMMON_OPTS)
C6_ASOPTS += $(COMMON_OPTS)
C6_LDOPTS += $(COMMON_OPTS) -list=$(C6_APP)

C6_FCLEAN += \
  ../src/*.d \
  ../src/.*.d \
  ../src/*.dbo \
  ../src/*.ti \
  *.dep \
  *.dla \
  *.dnm \
  *.map \
  *.ael \
  *.ti \
  *.dbo \
  *.run \
  *.c \
  $(C6_APP)

##############################################################################
C6_CC := ccarm -c
C6_CX := ccarm -c
C6_AS := ccarm -c
C6_LD := ccarm
C6_AR := ccarm -archive

C6_PROMPT := arm/ghs


include $(SDK_ROOT)/build/nbuild/.C6.mk
