#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   File: toolchain.mak
#   Copyright (c) 2016 NXP Semiconductors
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#   This is a tool-chain dispatcher script

ifeq ($(TOOLCHAIN),)
    include $(TOOLCHAIN_CFG_DIR)/linaro_toolchain.mak
endif

ifeq ($(TOOLCHAIN),misra)
    include $(TOOLCHAIN_CFG_DIR)/misra_toolchain.mak
endif
