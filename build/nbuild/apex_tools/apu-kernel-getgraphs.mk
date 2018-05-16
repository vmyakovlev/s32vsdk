##############################################################################
#
# NXP Confidential Proprietary
#
# Copyright (c) 2011 Freescale Semiconductor;
# Copyright 2017 NXP;
# All Rights Reserved
#
##############################################################################
#
# THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
# IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.
#
##############################################################################
#
#  CONTENT
#
#
#  AUTHORS
#    Stefan Schwarz
#    Mihail Marian Nistor
#
#  LANGUAGE
#    make
#
##############################################################################
C2_PROMPT := offline/X86

##############################################################################
# check required parameters
##############################################################################
ifeq ($(origin APU_COMP),undefined)
  $(error error: APU_COMP undefined)
endif

_graph2upper = $(call uc,$(subst _apu_process_desc.hpp,,$1))

_GRAPHS := $(call _graph2upper,$(notdir $(APU_GRAPHS)))
APU_LOAD_BINS := $(addsuffix _APU_LOAD.elf,$(_GRAPHS))
APU_LOAD_HEADERS := $(addsuffix _APU_LOAD.h,$(_GRAPHS))
APU_LOAD_SRCS := $(addsuffix __MKDBstub.cpp,$(_GRAPHS)) $(addsuffix .cpp,$(_GRAPHS))
APU_LOAD_OBJS := $(patsubst %.cpp,%.o,$(APU_LOAD_SRCS))

C1_OTHER_LIBS := $(SDK_ROOT)/libs/apex/acf_target/build-apu-$(APU_COMP)-sa-d/libacf_target.a

ifneq ($(findstring msys,$(NBUILD_PLATFORM)),)
	C1_OTHER_LIBS := $(C1_OTHER_LIBS) $(SDK_ROOT)/libs/apex/acf_offline/build-x86-gnu-windows-d/libacf_offline.a
else
	C1_OTHER_LIBS := $(C1_OTHER_LIBS) $(SDK_ROOT)/libs/apex/acf_offline/build-x86-gnu-linux-d/libacf_offline.a
endif

C1_OTHER_LIBS := $(C1_OTHER_LIBS) $(SDK_ROOT)/libs/apex/common/build-apu-$(APU_COMP)-sa-d/libapu_common.a

APU_LOAD_LDOPTS = $(SDK_ROOT)/libs/apex/acf_target/build-apu-$(APU_COMP)-sa-d/libacf_target.a \
		  $(SDK_ROOT)/libs/apex/common/build-apu-$(APU_COMP)-sa-d/libapu_common.a
APU_APP_LIBS := $(SDK_ROOT)/libs/apex/acf_target/build-apu-$(APU_COMP)-sa-d/libacf_target.a \
		  $(SDK_ROOT)/libs/apex/common/build-apu-$(APU_COMP)-sa-d/libapu_common.a

ifeq ($(APU_COMP),tct)
# echo "chess apu-kernel-getgraphs\n"
PARALLEL_APU_GRAPHS ?= 1
PARALLEL_LINK_APU_GRAPHS ?= 1

APU_CXOPTS += +W noodle,-Wnone
APU_CCOPTS += +W noodle,-Wnone

APU_LOAD_LDOPTS += $(APU_GRAPH_LIBS) \
	+W bridge,-m +W bridge,-fH +l \
	$(SDK_ROOT)/libs/apex/acf_target/include/apu2_load.bcf
else
# echo "llvm apu-kernel-getgraphs\n"
PARALLEL_APU_GRAPHS ?= 0
PARALLEL_LINK_APU_GRAPHS ?=0

ifeq ($(origin APU_TOOLS),undefined)
$(error Please define APU_TOOLS pointing to NXP APU compiler installation path. e.g. "export APU_TOOLS=/home/user/NXP/APU_Compiler_v1.0")
endif

APU_LOAD_LDOPTS += \
  --gc-sections \
  -Map $@.map \
  -L $(APU_TOOLS)/lib-apex \
  --start-group $(APU_GRAPH_LIBS) -lc -lm -lrt -lhosted --end-group \
  -T $(SDK_ROOT)/libs/apex/acf_target/include/S32V_APEX.lcf
endif # APU_COMP

APU_APP_LIBS += $(APU_GRAPH_LIBS)

# parser to extract INSTANCE_ID and CLASS_ID
parser := $(SDK_ROOT)/build/nbuild/scripts/parse.pl

# replace hpp by _main.exe
_graph2exe = $(strip $(foreach suf, hpp,\
		$(patsubst %.$(suf),%_main.exe,$(filter %.$(suf),$(1)))))

_graph2lib = $(strip $(foreach suf, .a,\
		$(patsubst %$(suf),%acf.a,$(filter %$(suf),$(1)))))

# get process identifier from offline resolution desktop application.
_exe2process_id = $(call uc,$(subst _apu_process_desc_main.exe,,$(1)))

# list of files that are generated
GENERATE_FILES := $(addsuffix _nativekernel.hpp,$(_GRAPHS)) $(addsuffix .hpp,$(_GRAPHS)) \
	$(notdir $(patsubst %.hpp,%_main.cpp,$(APU_GRAPHS)))

APU_GRAPH_INCS += \
  -I $(SDK_ROOT)/libs/apex/acf_target/include \
  -I$(SDK_ROOT)/libs/apex/acf_target/include \
  -I$(SDK_ROOT)/libs/apex/common/include \
  -I$(SDK_ROOT)/libs/apex/common/include/driver \
  -I $(SDK_ROOT)/libs/apex/acf_offline/include \
  -I../. -I. \
  -I$(SDK_ROOT)/libs/apex/acf/include \
  -I$(SDK_ROOT)/libs/apex/icp/include \
  -include apu2_kernel_db_lib.h

ifneq ($(findstring msys,$(NBUILD_PLATFORM)),)
	KERNEL_LDOPTS += $(SDK_ROOT)/libs/apex/acf_offline/build-x86-gnu-windows-d/libacf_offline.a
else
	KERNEL_LDOPTS += $(SDK_ROOT)/libs/apex/acf_offline/build-x86-gnu-linux-d/libacf_offline.a
endif

##############################################################################
#  check, if all APU graph directories exist ...
##############################################################################
_APU_GRAPH_DIRS := $(foreach d,$(APU_GRAPH_LIBS),$(dir $d))
_NOT_EXIST_APU_GRAPH_DIRS := $(filter-out $(wildcard $(_APU_GRAPH_DIRS)),$(_APU_GRAPH_DIRS))

define __apu_graph_chkdirs
  $(if $(strip $(_NOT_EXIST_APU_GRAPH_DIRS)), \
   $(foreach d,$(_NOT_EXIST_APU_GRAPH_DIRS),$(warning SUB_MAKE/APU_GRAPH_LIBS $d is missing)) \
   $(error You can find the missing directory/directories here: $(_NOT_EXIST_APU_GRAPH_DIRS) [$(abspath $(_NOT_EXIST_APU_GRAPH_DIRS))]))
endef

KERNEL_EXES := $(notdir $(call _graph2exe,$(APU_GRAPHS)))
KERNEL_LIBS := $(call _graph2lib,$(APU_GRAPH_LIBS))
KERNEL_OBJS := $(patsubst %.exe,%.o,$(KERNEL_EXES))
KERNEL_HDEP_INC := $(notdir $(patsubst %.hpp,%_main.cpp_g++.d,$(APU_GRAPHS)))

ifneq ($(PARALLEL_APU_GRAPHS),1)
.NOTPARALLEL: $(APU_LOAD_BINS)
ifeq ($(PARALLEL_LINK_APU_GRAPHS),1)
  $(error error: The parallel linking without parallel compilation is not supported.)
endif
endif

$(KERNEL_EXES): | $(KERNEL_LDOPTS) $(KERNEL_LIBS)
$(APU_LOAD_SRCS): | $(KERNEL_EXES)
$(APU_LOAD_OBJS): | $(APU_LOAD_SRCS)
$(APU_LOAD_BINS): | $(APU_LOAD_OBJS)
$(APU_LOAD_HEADERS): | $(APU_LOAD_BINS)

ifneq ($(HEADDEP),0)
ifeq ($(filter $(MAKECMDGOALS),$(HDEP_SKIP)),)
-include $(KERNEL_HDEP_INC)
endif
endif

define graph_resolution_rules
$(1)_main.exe :  $(1)_main.o $(KERNEL_LDOPTS) $(KERNEL_LIBS)
	$(call nbmsg,"$(C2_PROMPT) LD" $$@ " <= " $$< $(KERNEL_LDOPTS) $(KERNEL_LIBS)) \
	$(COMPILER_PREFIX)g++ $$< -DACF_KERNEL_METADATA $(KERNEL_LDOPTS) $(KERNEL_LIBS) -o $$@ -I.

ifeq ($(MAKE_RESTARTS),)
$(1)_main.cpp_g++.d : $(1)_main.cpp $(NBMAKEFILE)
	$(NBQ)echo apu-kernel-getgraphs.mk: updating $$@ ; \
	$(COMPILER_PREFIX)g++ -DACF_KERNEL_METADATA -std=c++0x $(APU_GRAPH_INCS) -MM -MG -MP -MT $$(subst .cpp,.exe,$$<) $$< > $$@.tmp ; \
	if [ -s $$@.tmp ] ; then \
	  sed < $$@.tmp 's|^\(.*\): |\1 $$(subst .cpp_g++.d,.o,$$@) $$@: |' > $$@ ; \
	  rm -f $$@.tmp ; \
	else \
	  rm -f $$@ $$@.tmp ; \
	fi
endif

$(1)_main.o : $(1)_main.cpp $(NBMAKEFILE)
	$(call nbmsg,"$(C2_PROMPT) CX" $$@) \
	$(COMPILER_PREFIX)g++ -DACF_KERNEL_METADATA -c $(APU_GRAPH_INCS) -std=c++0x -o  $$@ $$<

$(1)_main.cpp : $(1).hpp $(NBMAKEFILE)
	@echo apu-kernel-getgraphs.mk: updating $$@ ; \
	echo "#include \"$$<\"" > $$@ ; \
	echo "int main ()" >> $$@ ; \
	echo "{" >> $$@ ; \
	echo "   int lRetVal = 0;" >> $$@ ; \
	echo "   $$(shell perl $$(parser) $$< CLASS) obj;" >> $$@ ; \
	echo "   obj.Create();" >> $$@ ; \
	echo "   if (obj.RetErrorState() == 0)" >> $$@ ; \
	echo "   { " >> $$@ ; \
	echo "      if (obj.Resolve() == 0)" >> $$@ ; \
	echo "      { " >> $$@ ; \
	echo "         obj.GenerateProcess();" >> $$@ ; \
	echo "      }" >> $$@ ; \
	echo "      else" >> $$@ ; \
	echo "      { " >> $$@ ; \
	echo "         lRetVal = 1;" >> $$@ ; \
	echo "      }" >> $$@ ; \
	echo "   }" >> $$@ ; \
	echo "   else" >> $$@ ; \
	echo "   { " >> $$@ ; \
	echo "      lRetVal = 1;" >> $$@ ; \
	echo "   }" >> $$@ ; \
	echo "   return lRetVal;" >> $$@ ; \
	echo "}" >> $$@

$(2).cpp $(2)__MKDBstub.cpp $(2)_nativekernel.hpp $(2).hpp : generate_$(2)

generate_$(2) : $(1)_main.exe
	$$(call nbmsg,"$$(C2_PROMPT) executing" $$<) \
	./$$< >>$$(basename $$<).log

endef

$(foreach graph, $(KERNEL_EXES), $(eval $(call graph_resolution_rules,$(subst _main.exe,,$(graph)),$(call _exe2process_id,$(graph)))))

all:: parallel_link

__apu_graph: $(APU_GRAPHS)
	$(call __apu_graph_chkdirs)

build_apu_graphs: $(KERNEL_EXES) $(KERNEL_OBJS) $(GENERATE_FILES) $(APU_LOAD_SRCS)
	$(NBQ)$(MAKE) apu_graphs

ifneq ($(PARALLEL_LINK_APU_GRAPHS),1)
parallel_link: __apu_graph build_apu_graphs
	$(NBQ)$(MAKE) PARALLEL_APU_GRAPHS=0 PARALLEL_LINK_APU_GRAPHS=0 apu_graphs_link

else
parallel_link: __apu_graph build_apu_graphs
	$(NBQ)$(MAKE) apu_graphs_link

endif

clean::
	$(NBQ)rm -f *_apu_process_desc_main.* *.txt *.c *.cpp *.map *.h *.hpp

ifneq ($(strip $(APU_GRAPHS)),)
$(strip $(APU_GRAPHS)):
	$(error APU_GRAPHS $@ is missing)
endif

#if a .d file cannot be built, don't care and continue
$(KERNEL_HDEP_INC) :

