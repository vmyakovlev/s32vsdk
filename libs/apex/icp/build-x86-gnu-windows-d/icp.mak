DIR_LIB := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)


SRCS :=                                                                   \
    $(DIR_LIB)/src/icp_data.cpp                                           \
    $(DIR_LIB)/src/icp_datatype.cpp                                       \
    $(DIR_LIB)/src/icp_feature.cpp                                        \
    $(DIR_LIB)/src/icp_feature_32s.cpp                                    \


INCS :=                                                                   \
    $(DIR_SDK)/libs/apex/icp/include                                      \
    $(DIR_SDK)/libs/utils/oal/user/include


include $(DIR_SDK)/script/make/protected/build_lib_rules.min