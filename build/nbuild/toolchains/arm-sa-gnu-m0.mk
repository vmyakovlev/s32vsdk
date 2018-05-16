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
#    Armin Strobl
#
#  DATE
#    2015-08-27
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
C6_LDOPTS = $(SEQ_LDOPTS)
C6_LDSCRIPT = $(SEQ_LDSCRIPT)

##############################################################################
PLATFORM = ISP
OSTYPE := SA

COMMON_OPTS = \
  -mfpu=vfp \
  -mfloat-abi=soft \
  -mcpu=cortex-m0 \
  -march=armv6-m \
  -mthumb \
  -nostdlib \
  -Wl,--build-id=none

C6_INCS += -I.
C6_CCOPTS += \
  $(COMMON_OPTS) \
  -mstructure-size-boundary=8 \
  -mno-unaligned-access

C6_AROPTS +=
C6_CXOPTS += $(COMMON_OPTS)
C6_ASOPTS += $(COMMON_OPTS)
C6_LDOPTS += $(COMMON_OPTS)

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
CROSS_COMPILE_M0_ELF ?= arm-eabi-

C6_CC := $(CROSS_COMPILE_M0_ELF)gcc -c
C6_CX := $(CROSS_COMPILE_M0_ELF)g++ -c
C6_AS := $(CROSS_COMPILE_M0_ELF)gcc -c
C6_LD := $(CROSS_COMPILE_M0_ELF)g++
C6_AR := $(CROSS_COMPILE_M0_ELF)ar rcs
C6_OD := $(CROSS_COMPILE_M0_ELF)objdump
C6_OC := $(CROSS_COMPILE_M0_ELF)objcopy
C6_NM := $(CROSS_COMPILE_M0_ELF)nm

C6_PROMPT := arm32/sa


include $(SDK_ROOT)/build/nbuild/.C6.mk
