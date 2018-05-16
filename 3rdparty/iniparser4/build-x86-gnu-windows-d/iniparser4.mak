DIR_LIB := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)


SRCS :=                                                                \
    $(DIR_LIB)/src/dictionary.c                                           \
    $(DIR_LIB)/src/iniparser.c                                            \

INCS :=                                                                \


include $(DIR_SDK)/script/make/protected/build_lib_rules.min