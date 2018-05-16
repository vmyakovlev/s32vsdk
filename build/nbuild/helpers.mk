##############################################################################
#
#  Copyright (c) 2008, Freescale Semiconductor. All Rights Reserved.
#  Copyright 2017 NXP;
#  Freescale Confidential Proprietary
#
##############################################################################
#
#  CONTENT
#    some internal helper functions that are reused by several packages
#
#  AUTHOR
#    Norbert Stoeffler
#    Mihail Marian Nistor
#
#  DATE
#    2017-02-27
#
#  LANGUAGE
#    make
#
##############################################################################

ifndef _HELPERS_MK
_HELPERS_MK =	true

# if we really need spaces ...
#
_EMPTY :=
_SPACE :=	$(_EMPTY) $(_EMPTY)

# must be expanded for if main MF includes other MFs, i.e. ../BUILD.mk
#
NBMAKEFILE ?=	Makefile

# object dir prefix
ODP ?=		build-
ODPL ?=		6

ifeq ($(HDEP_IGNORE_ERROR),1)
define hdep_rules
ifeq ($(MAKE_RESTARTS),)
$(patsubst %.$(2),%-$(2).$(3).d,$(1)) : $(1) $(NBMAKEFILE)
	$(NBQ)echo .$(3)_headdep.mk: updating $$@ ; \
	$(4) -MM -MG -MP -MT $(patsubst %.$(2),%-$(2).$(5),$(1))  $$< > $$@.tmp ; \
	if [ -s $$@.tmp ] ; then \
	  sed < $$@.tmp 's|^\(.*\): |\1 $$@: |' > $$@ ;\
	  rm -f $$@.tmp ; \
	else \
	  rm -f $$@ $$@.tmp ; \
	fi
endif
endef
else
define hdep_rules
ifeq ($(MAKE_RESTARTS),)
$(patsubst %.$(2),%-$(2).$(3).d,$(1)) : $(1) $(NBMAKEFILE)
	$(NBQ)echo .$(3)_headdep.mk: updating $$@ ; \
	{ { $(4) -MM -MP -MT $(patsubst %.$(2),%-$(2).$(5),$(1))  $$< > $$@.tmp ; } || \
	  { $(4) -MM -MG -MP -MT $(patsubst %.$(2),%-$(2).$(5),$(1))  $$< > $$@.tmp ; } } ;\
	if [ -s $$@.tmp ] ; then \
	  sed < $$@.tmp 's|^\(.*\): |\1 $$@: |' > $$@ ;\
	  rm -f $$@.tmp ; \
	else \
	  rm -f $$@ $$@.tmp ; \
	fi
endif
endef
endif

# remove the last word from a list
#
cutlast =	$(wordlist 1,$(words $(wordlist 2,$(words $(1)),$(1))),$(1))

# create an include dir from a lib name, i.e. remove the last directory and
# any o-* elements in the path
#
incdir =	$(subst $(_SPACE),,$(call cutlast,$(call cutlast,\
		$(patsubst /$(ODP)%,,$(subst /, /,$(dir $(1)))))))

# remove the o- part of a path
#
cutodir =	$(subst $(_SPACE),,$(patsubst /$(ODP)%,,$(subst /, /,$(1))))

# make names of .o files from supported suffices
#
_src2obj =	$(strip $(foreach suf, s S c cpp cc C cl ipus ipuv,\
		$(patsubst %.$(suf),%-$(suf).o,$(filter %.$(suf),$(1)))))

_src2obj2 =	$(strip $(foreach suf, s S c cpp cc C cl ipus ipuv,\
		$(patsubst %.$(suf),%-$(suf).obj,$(filter %.$(suf),$(1)))))

# make names of .d files from supported suffices
#
_src2d = $(strip $(foreach suf, $(2),\
		$(patsubst %.$(suf),%-$(suf).$(3).d,$(filter %.$(suf),$(1)))))

# remove duplicates starting from the right (for libs)
#
nlibuniq =	$(shell echo $(1) | awk '{ \
		  for(i=NF;i>0;i--){ \
		    for(j=0;j<l;j++) \
		      if(a[j]==$$i) break; \
		    if(j==l) a[l++]=$$i; \
		  } \
		  for(j=l-1;j>=0;j--) print a[j];}')

# remove duplicates starting from the left (for subbuilds)
#
nsubuniq =	$(shell echo $(1) | awk '{ \
		  for(i=1;i<=NF;i++){ \
		    for(j=0;j<l;j++) \
		      if(a[j]==$$i) break; \
		    if(j==l) a[l++]=$$i; \
		  } \
		  for(j=0;j<l;j++) print a[j];}')

# shellscript to make pathes relative to the current dir
#
NPATH2REL =	awk -vwd=`/bin/pwd` -F/ 'BEGIN{ \
		  $$0=wd;for(f=2;f<=NF;f++) c[f]=$$f \
		}{ \
		  if(NF>1 && $$1!=".."){ \
		    printf("%s",$$1); \
		    for(f=2;f<=NF&&c[f]==$$f;f++); \
		    for(i=f;c[i];i++); \
		    for(j=f;j<i;j++)    printf("../"); \
		    for(i=f;$$i;i++) {if(i>f) printf("/"); printf("%s",$$i); } \
		    printf("\n"); \
		  }else \
		    print; \
		}'
# make function to make pathes relative to the current dir
#
npath2rel =	$(foreach l,$(1),$(shell echo $(l)|$(NPATH2REL)))

# make pathes absolute
#
npath2abs =	$(abspath $(1))


all:: # protect debugpath2rel

debugpath2rel:
	@$(NPATH2REL)

# the toplevel dir of the project (one above "tools")
NBTOP =		$(shell while [ ! -d tools ] && [ `pwd` != /home ] ; \
		do cd .. ; done ; pwd)

##############################################################################
# pattern rules for o-dirs
##############################################################################

%-c.o: %.c $(NBMAKEFILE)
	$(COMPILE.c) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-c.o: ../%.c $(NBMAKEFILE)
	$(COMPILE.c) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-S.o: %.S $(NBMAKEFILE)
	$(COMPILE.S) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-S.o: ../%.S $(NBMAKEFILE)
	$(COMPILE.S) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-s.o: %.s $(NBMAKEFILE)
	$(COMPILE.s) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-s.o: ../%.s $(NBMAKEFILE)
	$(COMPILE.s) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-cpp.o: %.cpp $(NBMAKEFILE)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-cpp.o: ../%.cpp $(NBMAKEFILE)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $< $(ERRORFILT)

# reusing COMPILE.cpp for alternative suffices
%-cc.o: %.cc $(NBMAKEFILE)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-cc.o: ../%.cc $(NBMAKEFILE)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-C.o: %.C $(NBMAKEFILE)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-C.o: ../%.C $(NBMAKEFILE)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-cl.o: %.cl $(NBMAKEFILE)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-cl.o: ../%.cl $(NBMAKEFILE)
	$(COMPILE.cl) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-ipus.o: %.ipus $(NBMAKEFILE)
	$(COMPILE.ipus) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-ipuv.o: %.ipuv $(NBMAKEFILE)
	$(COMPILE.ipuv) $(OUTPUT_OPTION) $< $(ERRORFILT)

# In case object files are created with _src2obj2 for duplicate compilation
# of same source file with two different compilers
%-c.obj: %.c $(NBMAKEFILE)
	$(COMPILE.c) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-c.obj: ../%.c $(NBMAKEFILE)
	$(COMPILE.c) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-S.obj: %.S $(NBMAKEFILE)
	$(COMPILE.S) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-S.obj: ../%.S $(NBMAKEFILE)
	$(COMPILE.S) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-s.obj: %.s $(NBMAKEFILE)
	$(COMPILE.s) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-s.obj: ../%.s $(NBMAKEFILE)
	$(COMPILE.s) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-cpp.obj: %.cpp $(NBMAKEFILE)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $< $(ERRORFILT)

%-cpp.obj: ../%.cpp $(NBMAKEFILE)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $< $(ERRORFILT)

# create (outlying) o-dir
$(ODP)%:
	mkdir $@

endif
