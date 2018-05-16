#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   File: app_config.mak
#   Copyright (c) 2016 NXP Semiconductors
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   README
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   This is the configuration file specifying various
#   applications which can be built and executed.
#
#   Each application is defined by:
#
#   TARGETS     - Targets to be built and linked together.
#                 Usually particular SW components needed
#                 by an application.
#   ELFFILE     - Name of the target output elf/binary file.
#   LINKERFILE  - Linker command script used to link the
#                 application.
#   DEFINES     - Defines to be defined by compiler for all
#                 targets.
#   CONFIG_DIRS - Way how to share common configuration files
#                 between particular TARGETS.
#
#   All targets referenced by an application shall be defined
#   within the "target_config.mak".
#
#   If an application is supposed to be a test, then it should
#   provide a valid MODULE variable which specifies which
#   SW module is going to be tested. Each module is identified
#   by its top-level folder name within the project structure.
#
#   Note that the environment expects that each testable
#   SW module have its tests prepared within the
#   .../tests/module_name directory in for of sub-folders.
#   Example:
#       If 
#               MODULE=./packages/module_name
#
#       then
#               tests
#                   |
#                   --module_name
#                           |
#                           +-- T001
#                               +-- Makefile
#                           +-- T002
#                               +-- Makefile
#                           ...
#                           +-- Tn
#                               +-- Makefile
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   Select targets and configuration for the desired variant
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

SUPPORTED_APPLICATIONS :=   sample                      \
                            bvideolistener_jpeg         \
                            bvideolistener_h264         \
                            bvideolistener_jpeg_misra   \
                            bvideolistener_h264_misra   \
                            mmutest                     \
                            sm_core_test                \
                            stream_core_test            \
                            mmu_test                    \
                            gic_test                    \
                            pit_test                    \
                            dec_feed_test               \
                            eth_queue_test

ifneq ($(app),)
    ifeq ($(app),sample)
        TARGETS     := a5_startup a5_custom_app
        ELFFILE     := theA5App.elf
        LINKERFILE  := ./toolchain/a5_startup/linker/linker.lds
        DEFINES     := CFG_LINK_USER_CODE=TRUE
        CONFIG_DIRS :=
    endif
    ifeq ($(app),stream_core_test)
        -include ./tests/stream_core/$(test_num)/test_cfg.mak
        MODULE      := ./packages/stream_core
    endif
    ifeq ($(app),sm_core_test)
        -include ./tests/sm_core/$(test_num)/test_cfg.mak
        MODULE      := ./packages/sm_core
    endif
    ifeq ($(app),mmu_test)
        -include ./tests/mmu/$(test_num)/test_cfg.mak
        MODULE      := ./packages/mmu
    endif
    ifeq ($(app),gic_test)
        TARGETS     := a5_test_app a5_startup_no_mmu common autolibc gic
        ELFFILE     := theA5App.elf
        LINKERFILE  := ./toolchain/a5_startup/linker/linker.lds
        DEFINES     := CFG_LINK_USER_CODE=TRUE PLATFORM=S32V234
        CONFIG_DIRS := 
        MODULE      := ./packages/gic
    endif
    ifeq ($(app),pit_test)
        TARGETS     := a5_test_app a5_startup_no_mmu common autolibc gic pit
        ELFFILE     := theA5App.elf
        LINKERFILE  := ./toolchain/a5_startup/linker/linker.lds
        DEFINES     := CFG_LINK_USER_CODE=TRUE PLATFORM=S32V234
        CONFIG_DIRS := 
        MODULE      := ./packages/pit
    endif
    ifeq ($(app),dec_feed_test)
        TARGETS     := a5_test_app a5_startup_no_mmu common autolibc dec_feed
        ELFFILE     := theA5App.elf
        LINKERFILE  := ./toolchain/a5_startup/linker/linker.lds
        DEFINES     := CFG_LINK_USER_CODE=TRUE PLATFORM=S32V234
        CONFIG_DIRS := ./tests/dec_feed/$(test_num)/cfg
        MODULE      := ./packages/dec_feed
    endif
    ifeq ($(app),eth_queue_test)
        TARGETS     := a5_test_app a5_startup_no_mmu common autolibc eth_queue
        ELFFILE     := theA5App.elf
        LINKERFILE  := ./toolchain/a5_startup/linker/linker.lds
        DEFINES     := CFG_LINK_USER_CODE=TRUE PLATFORM=S32V234
        CONFIG_DIRS := ./tests/eth_queue/$(test_num)/cfg ./packages/eth_queue/cfg
        MODULE      := ./packages/eth_queue
    endif
    ifeq ($(app),bvideolistener_jpeg_misra)
        override app :=  bvideolistener_jpeg
        MODULE      :=  ./apps/app_video_listener
    endif
    ifeq ($(app),bvideolistener_h264_misra)
        override app :=  bvideolistener_h264
        MODULE      :=  ./apps/app_video_listener
    endif
    ifeq ($(app),bvideolistener_jpeg)
        TARGETS     := app_video_listener eth_queue dec_feed stream_core common autolibc sm_core mmu gic pit fsl_printf
        ELFFILE     := theA5App.elf
        LINKERFILE  := ./packages/sm_core/linker/sm_linker.lds
        DEFINES     := PLATFORM=S32V234
        CONFIG_DIRS := ./packages/mmu/cfg                  \
                       ./packages/sm_core/cfg              \
                       ./packages/fsl_printf/cfg           \
                       ./apps/app_video_listener/cfg       \
                       ./apps/app_video_listener/cfg/jpeg  \
                       ./packages/eth_queue/cfg
    endif
    ifeq ($(app),bvideolistener_h264)
        TARGETS     := app_video_listener eth_queue dec_feed stream_core common autolibc sm_core mmu gic pit fsl_printf
        ELFFILE     := theA5App.elf
        LINKERFILE  := ./packages/sm_core/linker/sm_linker.lds
        DEFINES     := PLATFORM=S32V234
        CONFIG_DIRS := ./packages/mmu/cfg                  \
                       ./packages/sm_core/cfg              \
                       ./packages/fsl_printf/cfg           \
                       ./apps/app_video_listener/cfg       \
                       ./apps/app_video_listener/cfg/h264  \
                       ./packages/eth_queue/cfg
    endif
    ifeq ($(app),mmutest)
        TARGETS     := app_mmu a5_startup_no_mmu mmu
        ELFFILE     := theA5App.elf
        LINKERFILE  := ./toolchain/a5_startup/linker/linker.lds
        DEFINES     := CFG_LINK_USER_CODE=TRUE NO_MMU=TRUE
        CONFIG_DIRS := ./packages/mmu/cfg
    endif
    ifeq ($(filter $(app), $(SUPPORTED_APPLICATIONS)),)
        #   Add more options if required
        $(error Unsupported application variant)
    endif
endif
