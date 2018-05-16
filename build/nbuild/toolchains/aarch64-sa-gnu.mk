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
#
#  DATE
#    2015-03-04
#
#  LANGUAGE
#    make
#
##############################################################################
OS_TYPE := None

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
C0_LDOPTS = -L$(OPENCV_ROOT)/lib/ $(ARM_LDOPTS) -L$(SDK_ROOT)/3rdparty/ocv/standalone-arm64/share/OpenCV/3rdparty/lib

##############################################################################
PLATFORM = s32vpa53

C0_DEFS += \
  -DMPCIMXVS4 \
  -DMPCIS32V234 \
  -DLINARO \
  -DAUTOSAR_OS_NOT_USED \
  -DSINTRAM \
  -D__ARMV8 \
  -DDERIVATIVE_IMXVS4 \
  -DDERIVATIVE_S32V234 \
  -D__STANDALONE__
    
C0_INCS += \
  -I. \
  -I$(SDK_ROOT)/libs/startup/v234ce_standalone/include \
  -I$(SDK_ROOT)/libs/io/uartlinflex_io/include \
  -I$(SDK_ROOT)/libs/utils/oal/user/include \
  -I$(SDK_ROOT)/libs/io/semihost/include \
  -I$(SDK_ROOT)/libs/io/i2c/include

C0_CCOPTS += \
  -mcpu=cortex-a53 \
  -nostdlib \
  -fno-builtin \
  -fno-exceptions \
  -std=c99 \
  -Wno-narrowing \
  $(ARM_CCOPTS_END)

C0_CXOPTS += \
  -mcpu=cortex-a53 \
  -nostdlib \
  -fno-builtin \
  -fno-exceptions \
  -std=gnu++0x \
  -Wno-narrowing \
  $(ARM_CXOPTS_END)

C0_ASOPTS += -mcpu=cortex-a53

C0_LDOPTS += -T $(SDK_ROOT)/build/nbuild/toolchains/aarch64-sa-gnu.lds -lgcc -lc -lm -lrdimon -Wl,-Map=$@.map

C0_LDOPTS_PRE_LIBS := -Wl,--start-group
C0_LDOPTS_POST_LIBS := -Wl,--end-group


CROSS_COMPILE_AARCH64_ELF ?= aarch64-elf-

C0_CC := $(CROSS_COMPILE_AARCH64_ELF)gcc -c
C0_CX := $(CROSS_COMPILE_AARCH64_ELF)g++ -c
C0_AS := $(CROSS_COMPILE_AARCH64_ELF)gcc -c
C0_LD := $(CROSS_COMPILE_AARCH64_ELF)g++
C0_AR := $(CROSS_COMPILE_AARCH64_ELF)ar rcs
C0_OD := $(CROSS_COMPILE_AARCH64_ELF)objdump
C0_OC := $(CROSS_COMPILE_AARCH64_ELF)objcopy
C0_NM := $(CROSS_COMPILE_AARCH64_ELF)nm

C0_CCOPTS +=
C0_AROPTS +=
C0_CXOPTS +=
C0_ASOPTS +=
C0_LDOPTS +=
C0_FCLEAN := *.map *_log.txt *.bin
