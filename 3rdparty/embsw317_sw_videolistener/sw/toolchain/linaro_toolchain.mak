#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   File: linaro_toolchain.mak
#   Copyright (c) 2016 NXP Semiconductors
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#   Needed variables:
#       TOOLCHAIN_DIR           - path to the tool-chain binaries
#       CROSS_COMPILE_PREFIX    - tool-chain prefix
#       ELF                     - name of the final .elf
#       LINKERFILE              - linker file to be used
#       OUT_DIR                 - path where output will be written
#   Not mandatory:
#       CONFIG_DIRS             - additional include paths
#       TEST                    - test, linker will use test-related objects from separated
#                                 output folder

ifeq ($(ELF),)
    ELF = --notset--
endif
ifeq ($(OUT_DIR),)
    $(error Output dir not set)
endif

DEP_DIR = .dep
DEP_FLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.Td
$(shell mkdir -p $(DEP_DIR) >/dev/null)

INC_DIRS += $(CONFIG_DIRS)

CROSS_COMPILE_PREFIX ?= aarch64-elf-

OUT_APP = $(OUT_DIR)/$(ELF)

CCOPS =     -ggdb3                          \
            -mcpu=cortex-a53                \
            -mstrict-align                  \
            -fno-builtin                    \
            -std=c99                        \
            -Wconversion                    \
            -Wunused                        \
            $(addprefix -D, $(DEFINES))     \
            #-std=iso9899:1990

ASOPS =     -mcpu=cortex-a53                \
            -g

LDOPS =     -T $(LINKERFILE)                \
            -Map=$(OUT_APP:.elf=.map)       \
            -nostdlib                       \
            -nostartfiles                   \
            -nodefaultlibs

CC = echo -e CC '\t' $< && $(TOOLCHAIN_DIR)/$(CROSS_COMPILE_PREFIX)gcc
CX = echo -e CX '\t' $< && $(TOOLCHAIN_DIR)/$(CROSS_COMPILE_PREFIX)g++
AS = echo -e AS '\t' $< && $(TOOLCHAIN_DIR)/$(CROSS_COMPILE_PREFIX)as
LD = echo -e LD '\t' $@ && $(TOOLCHAIN_DIR)/$(CROSS_COMPILE_PREFIX)ld
AR = echo -e AR '\t' &< && $(TOOLCHAIN_DIR)/$(CROSS_COMPILE_PREFIX)ar
POSTCOMPILE = @mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d
OBJ = $(TOOLCHAIN_DIR)/$(CROSS_COMPILE_PREFIX)objcopy

INCLUDES = $(addprefix -I, $(INC_DIRS))
OBJECTS_0 = $(addprefix $(OUT_DIR)/, $(notdir $(SRC_FILES:%.s=%.o)))
OBJECTS_1 = $(OBJECTS_0:%.c=%.o)
OBJECTS = $(OBJECTS_1:%.S=%.o)
OBJECTS_TO_LINK = $(wildcard $(OUT_DIR)/*.o)

objects: outfolder $(OBJECTS)

exe: outfolder $(OUT_APP)
	@echo "[ Done ]"
    
.PHONY: outfolder
outfolder: $(OUT_DIR)
$(OUT_DIR):
	@mkdir -p $(OUT_DIR)
    
$(OUT_APP): $(LINKERFILE) $(OBJECTS_TO_LINK)
    ifeq ($(ELF),--notset--)
		$(error I need the ELF variable)
    endif
    ifeq ($(LINKERFILE),)
		$(error I need the LINKERFILE variable)
    endif
	@$(LD) $(LDOPS) $(OBJECTS_TO_LINK) -o $(OUT_APP)
	@$(OBJ) -O binary $(OUT_APP) $(basename $(OUT_APP)).bin

$(OUT_DIR)/%.o: $(SRC_DIR)/%.s
	@$(AS) -c $(ASOPS) $(INCLUDES) $< -o $@
    
$(OUT_DIR)/%.o: $(SRC_DIR)/%.S
	@$(CC) -c $(DEP_FLAGS) $(ASOPS) $(INCLUDES) $< -o $@
	$(POSTCOMPILE)
    
$(OUT_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) -c $(DEP_FLAGS) $(CCOPS) $(INCLUDES) $< -o $@
	$(POSTCOMPILE)
    
$(DEP_DIR)/%.d: ;
.PRECIOUS: $(DEP_DIR)/%.d

-include $(addprefix $(DEP_DIR)/, $(notdir $(OBJECTS:%.o=%.d)))
    
.PHONY: clean
clean:
    ifeq ($(PROJECT_DIR),)
		$(error I need the PROJECT_DIR variable)
    endif
	@rm -rf $(OUT_DIR)/*
	@rm -f $(OUT_DIR)/*
	@find $(PROJECT_DIR) -name "$(DEP_DIR)" -type d -exec rm -r "{}" +
#	@rm -f $(OBJECTS)
#	@rm -f $(OUT_APP)
#	@rm -f $(OUT_APP:.elf=.map)
