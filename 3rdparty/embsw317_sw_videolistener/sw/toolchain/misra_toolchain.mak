#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   File: misra_toolchain.mak
#   Copyright (c) 2016 NXP Semiconductors
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ifeq ($(PCLINT_DIR),)
    $(error PCLIT_DIR is not set)
endif

ifeq ($(MISRAVIEW_DIR),)
    $(error MISRAVIEW_DIR)
endif 

MODULE_INCLUDES = $(INC_DIRS)
ALL_INCLUDES = $(INC_DIRS) $(CONFIG_DIRS)

MODULE_SRC_FILES := $(filter %.c,$(SRC_FILES))
MODULE_INCLUDES := ./include

#REPORT_DIR = $(OUT_DIR)

LINT_EXEC := $(PCLINT_DIR)/lint-nt.exe
LINT_OPTS := '-zero(400)' +fem '+rw(_to_brackets)' -d__asm=_to_brackets -d__attribute__=_to_brackets '-d_to_brackets()='
LINT_OPTS += $(MISRAVIEW_DIR)/PC-lint/au-misra2.lnt $(MISRAVIEW_DIR)/PC-lint/opt.lnt $(MISRAVIEW_DIR)/PC-lint/co.lnt \
             +v -w3 \
             '+libclass(foreign,angle)' -DMISRA -DOSGHSARM -D__ghs__ -DINLINE '-dinline=' -D__MWERKS__ \
             $(addprefix -D, $(DEFINES))    \
             -i$$(cygpath --windows $(TOOLCHAIN_CFG_DIR)/misra_dummy) \
             -e46 \
             "+ppw(warning)"
LINT_OPTS += '-emacro((???),va_arg)' \
             -d__builtin_va_list=void* \
             '-d__builtin_va_start()=_to_semi' \
             '-d__builtin_va_arg(a,b)=(*( ((b) *) ( ((a) += sizeof(b)) - sizeof(b) )))' \
             '-d__builtin_va_end()=_to_semi' \
             '+rw(_to_semi)'
             
LINT_OPTS2 := $(MISRAVIEW_DIR)/PC-lint/env-xml.lnt

LINT_VERSION := $(subst (NT),-NT-,$(shell $(LINT_EXEC) -v 2>&1))

CURRENT_YEAR := $(shell date +"%Y")
CURRENT_DATE := $(shell date +"%D")

MISRA_EXEC   := $(MISRAVIEW_DIR)/Misraview.exe
MISRA_PARAMS := -E -X
MISRA_TEMPLATE_PARAMS := -t"$(subst /,\,$(MISRAVIEW_DIR))\misra_excel_header.xlsx" -v./variables.txt -l"$(LINT_VERSION)"

# "+libclass all" "-libdir(./include)"
objects:
	@mkdir -p $(OUT_DIR)
	@mkdir -p $(REPORT_DIR)
	$(LINT_EXEC) $(LINT_OPTS) $(addprefix -i, $(ALL_INCLUDES)) "-os($(OUT_DIR)/misra_analysis.xml)" $(LINT_OPTS2) $(MODULE_SRC_FILES)
	echo "MODULE_NAME: $(notdir $(MODULE))" > variables.txt; echo "Date: $(CURRENT_DATE)" >> variables.txt; echo "year date: $(CURRENT_YEAR)" >> variables.txt;
	$(MISRA_EXEC) $(MISRA_PARAMS) $(MISRA_TEMPLATE_PARAMS) -s"$(OUT_DIR)/misra_analysis.xml, $(shell cygpath -w $(REPORT_DIR)/$(notdir $(MODULE))_misra_report_$(test_num).xlsx)"
