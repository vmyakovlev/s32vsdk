#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   File: Makefile
#   Copyright (c) 2016 NXP Semiconductors
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

SRC_DIR     =   ./src
INC_DIRS    =   ./include                   \
                ../../apps/common/include   \
                ../autolibc/include         \
                ../fsl_printf/include       \
                ../mmu/include              \
                ../eth_queue/include        \
                ../dec_feed/include
                
SRC_FILES   =   ./src/stream_core.c ./src/h264_proc.c

ifeq ($(TEST),)
    # Application
    INC_DIRS += ../../packages/sm_core/include
else
    # Tests
    INC_DIRS += ../../tests/$(TEST)/include
endif

include $(TOOLCHAIN_CFG_DIR)/toolchain.mak
