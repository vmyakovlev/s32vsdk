DIR_LIB := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)


SRCS :=                                                                \
    $(DIR_LIB)/src/global_hal.cpp                                         \
    $(DIR_LIB)/src/windows/hw_reg_user.cpp                                \
    $(DIR_LIB)/src/cmem_if_hal.cpp                                        \


INCS :=                                                                \
    $(DIR_SDK)/libs/utils/oal/user/include                                \
    $(DIR_SDK)/libs/apex/register_maps/include                            \
    $(DIR_LIB)/include                                                    \



include $(DIR_SDK)/script/make/protected/build_lib_rules.min