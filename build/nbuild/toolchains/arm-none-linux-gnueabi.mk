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
OS_TYPE := Linux

C0_APP := $(ARM_APP).elf
C0_APP_SRCS := $(ARM_APP_SRCS)
C0_APP_LIBS := $(ARM_APP_LIBS)

C0_LIB := $(ARM_LIB)
C0_LIB_SRCS := $(ARM_LIB_SRCS)
C0_LIB_LIBS := $(ARM_LIB_LIBS)
C0_LIB_ARCS := $(ARM_LIB_ARCS)

C0_DEFS := $(ARM_DEFS)
C0_INCS := $(ARM_INCS)
C0_CCOPTS = $(ARM_CCOPTS)
C0_CXOPTS = $(ARM_CXOPTS)
C0_ASOPTS = $(ARM_ASOPTS)
C0_LDOPTS = -L$(OPENCV_ROOT)/lib/ -L$(FFMPEG_ROOT)/lib/ $(ARM_LDOPTS)

MCU = cortex-a9

##############################################################################
PLATFORM = zc702

C0_DEFS += \
  -D__GNU__ \
  -D__LINUX__ \
  -DARM \
  -D NDEBUG \
  -D __ZC702 \
  -D CV220X_$(CPU) \
  -D GNUARM#

C0_INCS += -I.

# compiler specific options for platform, not in headdep
C0_CCOPTS += \
  -O0 \
  -g \
  -mcpu=$(MCU) \
  -ffunction-sections \
  -mlong-calls \
  -Wall \
  -Wextra \
  -pedantic \
  -marm	\
  -std=c99

C0_CXOPTS += \
  -O0 \
  -g \
  -mcpu=$(MCU) \
  -funsigned-bitfields \
  -ffunction-sections \
  -mlong-calls \
  -Wall \
  -Wextra \
  -pedantic \
  -marm \
  -std=gnu++0x

C0_ASOPTS += -mcpu=$(MCU)

C0_LDOPTS += \
  -Wl,-Map=$@.map,--cref,--gc-sections

C0_LDOPTS_PRE_LIBS := -Wl,--start-group
C0_LDOPTS_POST_LIBS := -Wl,--end-group

CROSS_COMPILE_SYSROOT ?=
CROSS_COMPILE ?= arm-none-linux-gnueabi-

C0_CC := $(CROSS_COMPILE)gcc -c $(CROSS_COMPILE_SYSROOT)
C0_CX := $(CROSS_COMPILE)g++ -c $(CROSS_COMPILE_SYSROOT)
C0_AS := $(CROSS_COMPILE)g++ -c $(CROSS_COMPILE_SYSROOT)
C0_LD := $(CROSS_COMPILE)g++ $(CROSS_COMPILE_SYSROOT)
C0_AR := $(CROSS_COMPILE)ar
C0_OD := $(CROSS_COMPILE)objdump
C0_OC := $(CROSS_COMPILE)objcopy
C0_NM := $(CROSS_COMPILE)nm

C0_PROMPT := arm/gnu

C0_CCOPTS +=

C0_AROPTS += rcs
C0_CXOPTS +=
C0_ASOPTS +=
C0_LDOPTS += -lpthread -lrt -lstdc++
C0_FCLEAN := *.map

include $(SDK_ROOT)/build/nbuild/.C0.mk
