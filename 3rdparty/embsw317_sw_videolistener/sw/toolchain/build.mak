#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   File: build.mak
#   Copyright (c) 2016 NXP Semiconductors
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   Expected variables:
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   TARGETS         - targets to run, supplied by the
#                     target_config.mak
#   ELFFILE         - name of the final .elf file, supplied
#                     by the app_config.mak
#   LINKERFILE      - linker script to be used, supplied by
#                     the app_config.mak
#   DEFINES         - additional defined to be included in
#                     the build process, supplied by
#                     the app_config.mak
#   CONFIG_DIRS     - list of directories intended to contain
#                     configuration which is the same over
#                     all targets, supplied by the
#                     app_config.mak
#   MODULE          - module to process, optional, supplied by
#                     the app_config.mak
#   RESULTS         - only needed by ccov_merge target, supplied
#                     externally in form a list of folders
#                     containing test results to be merged
#   TOOLCHAIN_DIR   - path to the tool-chain binaries
#   T32_DIR         - path to the T32 root folder
#
#   Note that targets are delivered by the target_config.mak
#   file.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

SHELL = bash
NULL =
COMMA = ,
SPACE = $(NULL) #

include app_config.mak

export TOOLCHAIN_CFG_DIR := $(realpath ./toolchain/)
RUNNING_OS = $(shell uname)

CONFIG_DIRS := $(realpath $(CONFIG_DIRS))
ifneq ($(CONFIG_DIRS),)
    ifneq ($(RUNNING_OS),Linux)
        CONFIG_DIRS := $(shell cygpath -m -w $(CONFIG_DIRS))
    endif
endif

ifneq ($(LINKERFILE),)
    LINKERFILE := $(realpath $(LINKERFILE))
    ifneq ($(RUNNING_OS),Linux)
        LINKERFILE := $(shell cygpath -m -w $(LINKERFILE))
    endif
endif

OBJ_PARAMS = DEFINES="TRUE=1U FALSE=0U S32V234=1 $(DEFINES)" CONFIG_DIRS="$(CONFIG_DIRS)" objects
EXE_PARAMS = ELF="$(ELFFILE)" LINKERFILE="$(LINKERFILE)" DEFINES="$(DEFINES)" exe
MAKE_CMD := @make --no-print-directory

.PHONY all: check exe

include target_config.mak

ifeq ($(TOOLCHAIN_DIR),)
    ifeq ($(LINARO_DIR),)
        $(error Please specify TOOLCHAIN_DIR)
    else
        export TOOLCHAIN_DIR := $(LINARO_DIR)
    endif
else
    export TOOLCHAIN_DIR
endif

ifeq ($(RUNNING_OS),Linux)
    PROJECT_DIR := $(shell pwd)
    export OUT_DIR := $(patsubst %/,%,$(PROJECT_DIR)/output/$(notdir $(MODULE))/$(test_num))
    export REPORT_DIR := $(patsubst %/,%,$(PROJECT_DIR)/output/reports)
    export MODULE_OUT_DIR := $(patsubst %/,%,$(PROJECT_DIR)/output/$(notdir $(MODULE)))
else
    PROJECT_DIR := $(subst \,/,$(shell cygpath -w $(shell pwd)))
    export OUT_DIR := $(patsubst %/,%,$(PROJECT_DIR)/output/$(notdir $(MODULE))/$(test_num))
    export REPORT_DIR := $(patsubst %/,%,$(PROJECT_DIR)/output/reports)
    export MODULE_OUT_DIR = $(patsubst %/,%,$(PROJECT_DIR)/output/$(notdir $(MODULE)))
endif

LTB_CFG = $(PROJECT_DIR)/toolchain/lauterbach
TOOLS_DIR = $(PROJECT_DIR)/tools

EXECUTABLE = $(OUT_DIR)/$(ELFFILE)

#   If set, T32 will start in DEBUG mode
DEBUG = 0
#   If set, T32 will start in TRACE mode
TRACE = 0
#   If set, T32 will start in MERGE mode
MERGE = 0

ifneq ($(T32_DIR),)
    ifneq ($(wildcard $(T32_DIR)/bin/windows64/t32marm.exe),)
        T32_BIN = $(T32_DIR)/bin/windows64/t32marm.exe
    else
        ifneq ($(wildcard $(T32_DIR)/bin/windows/t32marm.exe),)
            T32_BIN = $(T32_DIR)/bin/windows/t32marm.exe
        else
            $(error Can't find T32 binary)
        endif
    endif
    
    ifneq ($(wildcard $(T32_DIR)/bin/windows64/t32marm64.exe),)
        T32_BIN64 = $(T32_DIR)/bin/windows64/t32marm64.exe
    else
        ifneq ($(wildcard $(T32_DIR)/bin/windows/t32marm64.exe),)
            T32_BIN64 = $(T32_DIR)/bin/windows/t32marm64.exe
        else
            $(error Can't find T32 binary for 64bit arch)
        endif
    endif
endif

.PHONY: check
check:
    ifeq ($(app),)
		$(error Please specify the "app". Available variants: $(SUPPORTED_APPLICATIONS))
    endif

#   Process all defined targets
.PHONY: exe
exe: check clean_testreports $(TARGETS)
	$(MAKE_CMD) -C ./toolchain -f linaro_toolchain.mak $(EXE_PARAMS)
    
.PHONY: clean_testreports
clean_testreports:
	$(call clean_test_reports)
    
.PHONY: debug
debug: setdebug run

.PHONY: coverage
coverage: setcoverage run

.PHONY: setmisra
setmisra:
    ifeq ($(MODULE),)
		$(error MODULE is not set)
    endif
	$(eval export TOOLCHAIN := misra)
	$(eval export MODULE)

.PHONY: setdebug
setdebug:
	$(eval DEBUG := 1)
    
.PHONY: setcoverage
setcoverage:
	$(eval TRACE := 1)
    
.PHONY: setmerge
setmerge:
	$(eval MERGE := 1)

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   misra
#       - performs MISRA analysis of a single module. Only
#         target derived from the MODULE variable is being
#         processed. Uses specialised tool-chain, see the
#         "setmisra" target above.
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
.PHONY: misra
misra: setmisra $(notdir $(MODULE))
    
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   run
#       - runs a specified test using T32 debugger and
#         indicates success or failure based on test results
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
.PHONY: exe run
.ONESHELL:
run: exe prepare_t32
	@echo -e RUN '\t' "$(EXECUTABLE)"
	$(call clean_test_reports)
	@cd $(LTB_CFG) && $(T32_BIN) -c "$(LTB_CFG)/s32v234_config_m4.t32" -s "s32v234_run_slave.cmm"
	@rm $(LTB_CFG)/s32v234_run_slave.cmm
	@rm $(LTB_CFG)/s32v234_startup.cmm
	@mkdir -p $(REPORT_DIR);
	@$(TOOLS_DIR)/dat_to_xml/dat_to_xml.exe                                                             \
                            -m $(notdir $(MODULE))                                                      \
                            -i $(dir $(EXECUTABLE))/testresults.dat                                     \
                            -o $(REPORT_DIR)/$(notdir $(MODULE))_testresults_$(test_num).xml
	@if [ ! -f "$(dir $(EXECUTABLE))/testresults.dat" ]; then                                           \
        echo "[ FAILED: RESULT FILE NOT FOUND ]";                                                       \
        exit 100;                                                                                       \
    else                                                                                                \
        if [ -z "`grep -E "^NOTR|^FAIL" $(dir $(EXECUTABLE))/testresults.dat`" ]; then                  \
            echo "[ PASSED ]";                                                                          \
        else                                                                                            \
            echo "[ FAILED ]" && cat $(dir $(EXECUTABLE))/testresults.dat;                              \
            exit 101;                                                                                   \
        fi                                                                                              \
    fi
	@if [ -z "`grep -E "^WARNING" $(dir $(EXECUTABLE))/testresults.dat`" ]; then                        \
        exit 0;                                                                                         \
    else                                                                                                \
        echo "[ WARNING ] Flow error or FIFO error detected during trace. Result may be inaccurate.";   \
        exit 110;                                                                                       \
    fi

.PHONY: ccov_merge_ltbmerge
ccov_merge_ltbmerge:
	@echo "LTB MERGE START"
	@$(eval LTBMERGE_INPUTS := $(addprefix $(MODULE_OUT_DIR)/,$(RESULTS)))
	@$(eval LTBMERGE_INPUTS := $(addsuffix /full_coverage.xml,$(LTBMERGE_INPUTS)))
	@$(eval LTBMERGE_INPUTS := $(subst $(SPACE),$(COMMA),$(strip $(LTBMERGE_INPUTS))))
	@$(eval MODULE_FILTERS := $(shell find $(MODULE) -name '*.c' -o -name '*.h'))
	@$(eval MODULE_FILTERS := $(notdir $(MODULE_FILTERS)))
	@$(eval MODULE_FILTERS := $(patsubst %.c,%,$(MODULE_FILTERS)))
	@$(eval MODULE_FILTERS := $(patsubst %.h,%,$(MODULE_FILTERS)))
	@$(eval MODULE_FILTERS := $(addprefix *\\,$(MODULE_FILTERS)))
#	@$(eval MODULE_FILTERS := $(addsuffix *,$(MODULE_FILTERS)))
	@$(eval MODULE_FILTERS := $(subst $(SPACE),$(COMMA),$(strip $(MODULE_FILTERS))))
	@mkdir -p $(REPORT_DIR)
	$(PROJECT_DIR)/tools/ltbmerge/ltbmerge.exe -m $(MODULE_FILTERS) -i $(LTBMERGE_INPUTS) -o $(REPORT_DIR)/$(notdir $(MODULE))_cov_report.txt

    
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   ccov_merge_t32
#       - merges a set of test results and produces final
#         report in human-readable form.
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
.PHONY: ccov_merge_t32
ccov_merge_t32: setmerge prepare_t32
    ifeq ($(RESULTS),)
		$(error I need the RESULTS variable describing tests to be merged)
    endif
    ifeq ($(MODULE),)
		$(error I need the MODULE variable to know which module is being requested)
    endif
	@echo "Processing module: $(MODULE)"
    
    #   Prepare the "merge configuration file" - used by T32 scripts to know which
    #   SW module shall be processed and which test results to merge.
	@rm -f ccov_results.txt
    ifeq ($(RUNNING_OS),Linux)
		@echo $(patsubst ./%,%,$(MODULE)) >> ccov_results.txt
    else
		@echo $(subst /,\\,$(patsubst ./%,%,$(MODULE))) >> ccov_results.txt
    endif
	@for res in $(RESULTS); do                                              \
        if [ ! -f $(OUT_DIR)/$$res/ccov_result.acd ]; then                  \
            echo "ERROR: CCOV RESULT FOR $$res NOT FOUND";                  \
            echo [ MERGE FAILED ];                                          \
            rm -f ccov_results.txt;                                         \
            exit 102;                                                       \
        else                                                                \
            echo $(OUT_DIR)/$$res/ccov_result.acd >> ccov_results.txt;      \
            theElf=$$(find $(OUT_DIR)/$$res/ -name "*.elf" | head -1);      \
            if [ ! -f $$theElf ]; then                                      \
                echo "ERROR: Can't find the related elf";                   \
                exit 103;                                                   \
            else                                                            \
                echo $$theElf >> ccov_results.txt;                          \
            fi                                                              \
        fi                                                                  \
    done
    #   Run the T32 environment.
	@cd $(LTB_CFG) && $(T32_BIN) -c "$(LTB_CFG)/s32v234_config_m4.t32" -s "s32v234_run_slave.cmm"
	@rm $(LTB_CFG)/s32v234_run_slave.cmm
	@rm $(LTB_CFG)/s32v234_startup.cmm
    
    #   Check if the merged ccov result file is present. If so, transform
    #   it to the htm format.
	$(eval export PATH=$(PATH):$(realpath ./tools/libxslt-1.1.26.win32/bin/):$(realpath ./tools/Xalan-C_1_10_0-win32-msvc_60/bin/):$(realpath ./tools/xerces-c-windows_2000-msvc_60/bin/))
	@if [ ! -f "$(LTB_CFG)/coverage.xml" ]; then            \
        echo "ERROR: Merged ccov result not found";         \
        exit 104;                                           \
    else                                                    \
        $(LTB_CFG)/xml2html.cmd $(LTB_CFG)/coverage.xml;    \
        cp $(LTB_CFG)/coverage.xml $(OUT_DIR);              \
        rm -f $(LTB_CFG)/coverage.xml;                      \
    fi
    
    #   Check if the final report has been created.
	@if [ ! -f "$(LTB_CFG)/coverage.htm" ]; then                                \
        echo "ERROR: Final ccov report not found";                              \
        exit 105;                                                               \
    else                                                                        \
        mv $(LTB_CFG)/coverage.htm $(OUT_DIR)/$(notdir $(MODULE))_coverage.htm; \
    fi
    
    
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   prepare_t32
#       - internally used target to prepare T32 environment
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
.PHONY: prepare_t32
prepare_t32:
    ifeq ($(T32_DIR),)
		$(error I need the T32_DIR variable)
    endif
	@cp $(LTB_CFG)/s32v234_startup_template.cmm $(LTB_CFG)/s32v234_startup.cmm
	@sed -i "s#___ELF_FILE___#$(EXECUTABLE)#g" $(LTB_CFG)/s32v234_startup.cmm
	@sed -i "s#___DEBUG___#$(DEBUG)#g" $(LTB_CFG)/s32v234_startup.cmm
	@sed -i "s#___TRACE___#$(TRACE)#g" $(LTB_CFG)/s32v234_startup.cmm
	@sed -i "s#___MERGE___#$(MERGE)#g" $(LTB_CFG)/s32v234_startup.cmm
	@cd $(LTB_CFG) && echo "os.screen $(T32_BIN64) -c $(LTB_CFG)/s32v234_config_a5.t32 -s $(LTB_CFG)/s32v234_startup.cmm" > s32v234_run_slave.cmm
    
    
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   clean
#       - clean the output directory
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
.PHONY: clean
clean:
	$(MAKE_CMD) -C ./toolchain -f linaro_toolchain.mak PROJECT_DIR=$(PROJECT_DIR) clean

#   Routine to clean all test reports. Intended to be called each time when
#   a test is being built, re-built or executed
#   The call will create an initial dummy test result file indicating that
#   test has not been executed yet.
define clean_test_reports
	@if [ ! -z $(MODULE) ]; then                                                                                                            \
        rm -f $(REPORT_DIR)/$(notdir $(MODULE))_testresults_$(test_num).xml;                                                                \
        rm -f $(dir $(EXECUTABLE))/testresults.dat;                                                                                         \
        mkdir -p $(REPORT_DIR)
        $(TOOLS_DIR)/dat_to_xml/dat_to_xml.exe -m $(notdir $(MODULE)) -d -o $(REPORT_DIR)/$(notdir $(MODULE))_testresults_$(test_num).xml;  \
    fi
endef
