DIR_LIB := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)


SRCS :=                                                                  \
    $(DIR_LIB)/src/MessageQueue.cpp                                         \
    $(DIR_LIB)/src/MessagingManager.cpp                                     \
    $(DIR_LIB)/src/MessagingManagerPriv.cpp                                 \
    $(DIR_LIB)/src/NameContainer.cpp                                        \
    $(DIR_LIB)/src/NameIterator.cpp                                         \
    $(DIR_LIB)/src/GDC_OAL_Semaphore.cpp                                    


INCS :=                                                                \
    $(DIR_LIB)/include                                                    \
    $(DIR_SDK)/libs/utils/oal/user/include                                \


include $(DIR_SDK)/script/make/protected/build_lib_rules.min