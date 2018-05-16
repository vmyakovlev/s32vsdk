#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   File: Makefile
#   Copyright (c) 2016 NXP Semiconductors
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

SRC_DIR     =   ./src
INC_DIRS    =   ./include ../../apps/common/include ../autolibc/include
SRC_FILES   =   ./src/eth_queue.c

ifeq ($(TEST),)
    #   Reference the SM_CORE module only when not being tested
    #   to enable ETH_QUEUE module separation during test
    INC_DIRS += ../sm_core/include
endif

include $(TOOLCHAIN_CFG_DIR)/toolchain.mak
