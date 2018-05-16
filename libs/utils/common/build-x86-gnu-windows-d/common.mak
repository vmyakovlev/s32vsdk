DIR_LIB := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)


SRCS :=                                                                \
    $(DIR_LIB)/src/time_measure.cpp                                       \


INCS :=                                                                \
    $(DIR_LIB)/include                                                    \
    $(DIR_SDK)/libs/utils/oal/user/include                                \


include $(DIR_SDK)/script/make/protected/build_lib_rules.min