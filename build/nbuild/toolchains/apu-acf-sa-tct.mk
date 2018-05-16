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
#    Mihail Marian Nistor
#
#  DATE
#    2017-02-27
#
#  LANGUAGE
#    make
#
##############################################################################
OS_TYPE :=
READELF := read_elf
TCL_SCRIPT := $(SDK_ROOT)/build/nbuild/scripts/gen_apu_load.tcl

############ Target Chess ####################################################
C1_APP := $(APU_APP).elf
C1_APP_SRCS := $(APU_APP_SRCS)
C1_APP_LIBS := $(APU_APP_LIBS)

C1_LIB := $(APU_LIB)
C1_LIB_SRCS := $(APU_LIB_SRCS)
C1_LIB_LIBS := $(APU_LIB_LIBS)

C1_DEFS := $(APU_DEFS)
C1_INCS := $(APU_INCS)

C1_COMMON_DEFS := \
  -DCGV_FORCE_INLINE \
  -D__TCTBUILDER__ \
  -include apu2_kernel_port.h \
  -include acf_autokernel_inc.h


C1_DEFS += \
  $(C1_COMMON_DEFS)

C1_COMMON_INCS := \
  -I$(SDK_ROOT)/libs/apex/acf_offline/include \
  -I$(SDK_ROOT)/libs/apex/acf_target/include \
  -I$(SDK_ROOT)/libs/apex/common/include \
  -I$(SDK_ROOT)/libs/apex/drivers/user/include \
  -I$(SDK_ROOT)/libs/apex/register_maps/include \
  -I$(SDK_ROOT)/libs/apex/acf/include \
  -I$(SDK_ROOT)/libs/apex/icp/include \
  -I$(SDK_ROOT)/include

C1_INCS += \
  $(C1_COMMON_INCS)

# compiler specific options for platform, not in headdep
C1_CCOPTS += $(APU_CCOPTS)
C1_CXOPTS += $(APU_CCOPTS)
C1_ASOPTS +=
C1_LDOPTS += $(APU_LDOPTS)

C1_COMMON_CCOPTS = \
  -p APU2
########## Load generation settings
C1_LOAD_BINS := $(APU_LOAD_BINS)
C1_LOAD_OBJS := $(APU_LOAD_OBJS)
C1_LOAD_SRCS := $(APU_LOAD_SRCS)
C1_LOAD_INCS := $(APU_GRAPH_INCS)
C1_LOAD_LDOPTS = $(APU_LOAD_LDOPTS)

C1_LOAD_CCOPTS = \
  $(C1_COMMON_INCS) \
  $(C1_COMMON_DEFS) \
  $(APU_CCOPTS) \
  $(C1_COMMON_CCOPTS)

############ Auto Build (load_generation) ####################################

##############################################################################
# nbuild packages
# option that control optimization, no value for default
APU_CO ?=

C1_CC := chesscc -c $(APU_CO) -g -p APU2 +p 4  +W noodle,+Oivsq +W mist2,-Wno_warn_pipelining
C1_CX := chesscc -c $(APU_CO) -g -p APU2 +p 4  +W noodle,+Oivsq +W mist2,-Wno_warn_pipelining
C1_AS := chesscc -c -g -p APU2 +p 4
C1_LD := chesscc -g
C1_AR := chesscc
C1_OD := darts

C1_PROMPT := apu/chess

C1_CCOPTS += \
  $(C1_COMMON_CCOPTS)

C1_AROPTS += -a -o
C1_CXOPTS +=
C1_ASOPTS +=

C1_LDOPTS += -p APU2 +W bridge,-m +W bridge,-fH +l $(SDK_ROOT)/libs/apex/acf_target/include/apu2_load.bcf
C1_FCLEAN := *.h *.log *.a *.dis

include $(SDK_ROOT)/build/nbuild/.C1.mk
