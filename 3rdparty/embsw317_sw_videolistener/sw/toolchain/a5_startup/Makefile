SRC_DIR     =   ./src
INC_DIRS    =   ./include
SRC_FILES   =   ./src/startup_linaro.s          \
                ./src/entry.s                   \
                ./src/app_header.c              \
                ./src/vector_table.s            \
                ./src/exceptions.c              \
                ./src/startup.c                 \
                ./src/gic400.c                  \
                ./src/main.c

ifeq ($(NO_MMU),)
    SRC_FILES += ./src/mmap.c
endif

include $(TOOLCHAIN_CFG_DIR)/toolchain.mak