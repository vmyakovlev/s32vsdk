#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   File: Makefile
#   Copyright (c) 2016 NXP Semiconductors
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
#   This is a wrapper for the build.mak. Its intention is
#   to act as a main makefile and to handle sequential
#   execution of tests.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#   Get targets from the configuration file
include app_config.mak

MCMD = make -f ./toolchain/build.mak --no-print-directory

run: default
debug: default
misra: default
coverage: default ccov
merge: findtests ccov

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   alltest
#       - run all tests specified by SUPPORTED_APPLICATIONS
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
alltest:
	$(eval TESTAPPS := $(filter %_test,$(SUPPORTED_APPLICATIONS)))
	@nFailures="0";                                                         \
	for application in $(TESTAPPS); do                                      \
        echo "[ Running $$application ]";                                   \
        $(MAKE) --no-print-directory app="$$application" run;               \
        if [ $$? != 0 ]; then                                               \
            let "nFailures+=1";                                             \
        fi;                                                                 \
    done;                                                                   \
    if [ $$nFailures != 0 ]; then                                           \
        echo "$$nFailures application(s) have failed";                      \
    else                                                                    \
        echo "All tests passed";                                            \
    fi;
    
allmisra:
	$(eval TESTAPPS := $(filter %_test,$(SUPPORTED_APPLICATIONS)))
	@$(foreach APP, $(TESTAPPS), echo "[ Running $(APP) ]" && $(MAKE) --no-print-directory app="$(APP)" misra &&) true
    
allcoverage:
	$(eval TESTAPPS := $(filter %_test,$(SUPPORTED_APPLICATIONS)))
	@$(foreach APP, $(TESTAPPS), echo "[ Running $(APP) ]" && $(MAKE) --no-print-directory app="$(APP)" coverage &&) true

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   default
#       - default target, sequentially executes tests or
#         applications
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
.PHONY: default
default: findtests
ifeq ($(MODULE),)
	$(MCMD) $(MAKECMDGOALS)
else
	@echo "Test(s) to run: " $(ALLTESTS)
	@nFailures="0";                                                         \
    for test in $(ALLTESTS); do                                             \
        echo "[ $$test ]";                                                  \
        $(MCMD) test_num="$$test" $(MAKECMDGOALS);                          \
        if [ $$? != 0 ]; then                                               \
            let "nFailures+=1";                                             \
        fi;                                                                 \
    done;                                                                   \
    if [ $$nFailures != 0 ]; then                                           \
        echo "$$nFailures tests(s) have failed";                            \
        exit -1;                                                            \
    fi;
endif
    
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   ccov
#       - executes code coverage process for a given MODULE
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
.PHONY: ccov
ccov:
	@echo "Merging tests: "$(ALLTESTS)
	@$(MCMD) ccov_merge_ltbmerge RESULTS="$(ALLTESTS)"
	@echo "[ MERGE DONE ]"

    
.PHONY: clean
clean:
	$(MCMD) $(MAKECMDGOALS)
    
.PHONY: findtests
findtests:
	$(eval ALLTESTS := $(shell find ./tests/$(notdir $(MODULE))/$(test_num) -name "Makefile"))
	$(eval ALLTESTS := $(dir $(ALLTESTS)))
	$(eval ALLTESTS := $(patsubst %/, %, $(ALLTESTS)))
	$(eval ALLTESTS := $(notdir $(ALLTESTS)))
