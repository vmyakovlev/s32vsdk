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
TCL_SCRIPT := $(SDK_ROOT)/build/nbuild/scripts/gen_apu_load.tcl
READELF := tclsh $(SDK_ROOT)/build/nbuild/scripts/read_elf.tcl
OBJCOPY := @aarch64-elf-objcopy --binary-architecture=arm > objcopy.log 2> objcopy_error.log

############ NXP APEX Compiler ###############################################

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
  -include apu2_kernel_port.h \
  -include acf_autokernel_inc.h \
  -include apu_microkernel.h  

C1_DEFS += \
  $(C1_COMMON_DEFS)

C1_COMMON_INCS := \
  -I $(SDK_ROOT)/libs/apex/acf_offline/include \
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

C1_COMMON_CCOPTS := \
 -x c++
 

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

ifndef APU_TOOLS
 $(error Please define APU_TOOLS pointing to NXP APU compiler installation path. e.g. "export APU_TOOLS=/home/user/NXP/APU_Compiler_v1.0")
endif

NXP_CMP_PATH=$(APU_TOOLS)/bin

# option that control optimization, the default value is -O3
APU_CO ?= -O3

C1_CC := $(NXP_CMP_PATH)/clang -g -fdebug-prefix-map="$(OS_STYLE_CURR_SDK_ROOT)"="s32v234_sdk" $(APU_CO) -c
C1_CX := $(NXP_CMP_PATH)/clang -g -fdebug-prefix-map="$(OS_STYLE_CURR_SDK_ROOT)"="s32v234_sdk" $(APU_CO) -c
C1_AS := $(NXP_CMP_PATH)/clang -g -fdebug-prefix-map="$(OS_STYLE_CURR_SDK_ROOT)"="s32v234_sdk" -c
C1_LD := $(NXP_CMP_PATH)/ld-new -g --gc-sections --no-check-sections -lstartcpp
C1_AR := $(NXP_CMP_PATH)/llvm-ar
C1_OD := $(NXP_CMP_PATH)/llvm-objdump -arch-name=apex

C1_PROMPT := apex/clang


C1_CCOPTS += \
  $(C1_COMMON_CCOPTS)
 
C1_AROPTS += r
C1_CXOPTS += -Wno-format -Wno-self-assign-field -Wno-unused-value -Wno-non-literal-null-conversion -Wno-tautological-compare -Wno-logical-op-parentheses 
C1_ASOPTS +=

C1_LDOPTS += --gc-sections --no-check-sections
C1_FCLEAN := *.h *.log *.a *.dis

include $(SDK_ROOT)/build/nbuild/.C1.mk

############ LLVM compiler #################################################


