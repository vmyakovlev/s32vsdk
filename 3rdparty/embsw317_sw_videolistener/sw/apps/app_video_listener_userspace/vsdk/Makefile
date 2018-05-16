#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   File: Makefile
#   Copyright (c) 2016 NXP Semiconductors
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ifeq ($(VSDK_PATH),)
    #$(error Please set VSDK_PATH, e.g. ~/s32v234_sdk)
endif

SUPPORTED_VARIANTS = h264_single_static h264_single_stream h264_4_static h264_4_stream jpeg_4_stream

ifneq ($(APP_VARIANT),)
    ifeq ($(filter $(APP_VARIANT), $(SUPPORTED_VARIANTS)),)
        #$(error Please specify a valid application variant: $(SUPPORTED_VARIANTS))
    endif

    ifeq ($(APP_VARIANT),h264_single_static)
        __APP_VARIANT = isp_h264dec_single_stream
    endif
    ifeq ($(APP_VARIANT),h264_single_stream)
        __APP_VARIANT = isp_h264dec_single_stream
    endif
    ifeq ($(APP_VARIANT),jpeg_4_stream)
        __APP_VARIANT = isp_jpeg_4stream
    endif
    ifeq ($(APP_VARIANT),h264_4_static)
        __APP_VARIANT = isp_h264dec_4stream
    endif
    ifeq ($(APP_VARIANT),h264_4_stream)
        __APP_VARIANT = isp_h264dec_4stream
    endif
endif

__APP_CFG_DIR = ./cfg/$(APP_VARIANT)

#VSDK_APP_PATH=$(VSDK_PATH)/demos/isp/$(__APP_VARIANT)
VSDK_APP_PATH=$(OUT_PATH)
VSDK_BIN_PATH=$(VSDK_APP_PATH)/build-v234ce-gnu-linux-d

# BUILD THE APP TARGET
app : $(VSDK_APP_PATH) \
      $(VSDK_BIN_PATH)/Makefile \
      $(VSDK_APP_PATH)/build-v234ce-gnu-linux-o/Makefile \
      $(VSDK_APP_PATH)/src/main.cpp \
      $(VSDK_APP_PATH)/src/vl_sram.cpp \
	  $(VSDK_APP_PATH)/src/cfg_file.cpp \
	  $(VSDK_APP_PATH)/src/dec_jpeg_hl.cpp \
	  $(VSDK_APP_PATH)/src/dec_h264_hl.cpp \
	  $(VSDK_APP_PATH)/src/video_app.cpp \
      $(VSDK_APP_PATH)/BUILD.mk \
      $(VSDK_APP_PATH)/include/application_cfg.h \
      $(VSDK_APP_PATH)/include/eth_queue_cfg.h \
      $(VSDK_APP_PATH)/include/sm_drv_types.h \
      $(VSDK_APP_PATH)/include/vl_sram.h \
      $(VSDK_APP_PATH)/include/cfg_file.h \
      $(VSDK_APP_PATH)/include/dec_jpeg_hl.h \
      $(VSDK_APP_PATH)/include/dec_h264_hl.h \
      $(VSDK_APP_PATH)/include/video_app.h \
      $(VSDK_APP_PATH)/include/video_app_cfg.h \
      $(VSDK_PATH)/libs/isp/sram/kernel/include/sram.h \
      $(VSDK_PATH)/libs/isp/sram/kernel/include/sram_ioctl.h \
      $(VSDK_PATH)/libs/isp/jpegdec/kernel/linux/include/jpegdcd_linux.h
	make ARCH=arm S32V234_SDK_ROOT=$(VSDK_PATH) -C $(VSDK_BIN_PATH) allsub
	mv $(VSDK_BIN_PATH)/*.elf $(VSDK_BIN_PATH)/app
	cp $(VSDK_BIN_PATH)/app ./

# CREATE APP DIR in VSDK TARGET
$(VSDK_APP_PATH) :
	mkdir -p $(VSDK_APP_PATH)

# COPY APP FILES TO VSDK TARGETS
# Phonies are used to force overwrites on some files
.PHONY : $(VSDK_BIN_PATH)/Makefile
$(VSDK_BIN_PATH)/Makefile : ./$(__APP_VARIANT)/build-v234ce-gnu-linux-d/Makefile $(VSDK_APP_PATH)
	mkdir -p $(VSDK_BIN_PATH)
	cp ./$(__APP_VARIANT)/build-v234ce-gnu-linux-d/Makefile $(VSDK_BIN_PATH)
.PHONY : $(VSDK_APP_PATH)/build-v234ce-gnu-linux-o/Makefile
$(VSDK_APP_PATH)/build-v234ce-gnu-linux-o/Makefile : ./$(__APP_VARIANT)/build-v234ce-gnu-linux-o/Makefile $(VSDK_APP_PATH)
	mkdir -p $(VSDK_APP_PATH)/build-v234ce-gnu-linux-o
	cp ./$(__APP_VARIANT)/build-v234ce-gnu-linux-o/Makefile $(VSDK_APP_PATH)/build-v234ce-gnu-linux-o
.PHONY : $(VSDK_APP_PATH)/src/main.cpp
$(VSDK_APP_PATH)/src/main.cpp : src/main.cpp $(VSDK_APP_PATH)
	mkdir -p $(VSDK_APP_PATH)/src
	cp src/main.cpp $(VSDK_APP_PATH)/src
$(VSDK_APP_PATH)/src/vl_sram.cpp : src/vl_sram.cpp $(VSDK_APP_PATH)
	mkdir -p $(VSDK_APP_PATH)/src
	cp src/vl_sram.cpp $(VSDK_APP_PATH)/src
$(VSDK_APP_PATH)/src/cfg_file.cpp : src/cfg_file.cpp $(VSDK_APP_PATH)
	mkdir -p $(VSDK_APP_PATH)/src
	cp src/cfg_file.cpp $(VSDK_APP_PATH)/src
$(VSDK_APP_PATH)/src/dec_jpeg_hl.cpp : src/dec_jpeg_hl.cpp $(VSDK_APP_PATH)
	mkdir -p $(VSDK_APP_PATH)/src
	cp src/dec_jpeg_hl.cpp $(VSDK_APP_PATH)/src
$(VSDK_APP_PATH)/src/dec_h264_hl.cpp : src/dec_h264_hl.cpp $(VSDK_APP_PATH)
	mkdir -p $(VSDK_APP_PATH)/src
	cp src/dec_h264_hl.cpp $(VSDK_APP_PATH)/src
$(VSDK_APP_PATH)/src/video_app.cpp : src/video_app.cpp $(VSDK_APP_PATH)
	mkdir -p $(VSDK_APP_PATH)/src
	cp src/video_app.cpp $(VSDK_APP_PATH)/src
.PHONY : $(VSDK_APP_PATH)/BUILD.mk
$(VSDK_APP_PATH)/BUILD.mk : ./$(__APP_VARIANT)/BUILD.mk $(VSDK_APP_PATH)
	cp ./$(__APP_VARIANT)/BUILD.mk $(VSDK_APP_PATH)/BUILD.mk
$(VSDK_APP_PATH)/include/application_cfg.h : ../../app_video_listener/cfg/application_cfg.h $(VSDK_APP_PATH)
	mkdir -p $(VSDK_APP_PATH)/include
	cp ../../app_video_listener/cfg/application_cfg.h $(VSDK_APP_PATH)/include
$(VSDK_APP_PATH)/include/eth_queue_cfg.h : ../../../packages/eth_queue/cfg/eth_queue_cfg.h $(VSDK_APP_PATH)
	mkdir -p $(VSDK_APP_PATH)/include
	cp ../../../packages/eth_queue/cfg/eth_queue_cfg.h $(VSDK_APP_PATH)/include
$(VSDK_APP_PATH)/include/sm_drv_types.h : ../../../kernel/sm_drv/include/sm_drv_types.h $(VSDK_APP_PATH)
	mkdir -p $(VSDK_APP_PATH)/include
	cp ../../../kernel/sm_drv/include/sm_drv_types.h $(VSDK_APP_PATH)/include
$(VSDK_APP_PATH)/include/vl_sram.h : ./include/vl_sram.h $(VSDK_APP_PATH)
	mkdir -p $(VSDK_APP_PATH)/include
	cp ./include/vl_sram.h $(VSDK_APP_PATH)/include
$(VSDK_APP_PATH)/include/cfg_file.h : ./include/cfg_file.h $(VSDK_APP_PATH)
	mkdir -p $(VSDK_APP_PATH)/include
	cp ./include/cfg_file.h $(VSDK_APP_PATH)/include
$(VSDK_APP_PATH)/include/dec_jpeg_hl.h : ./include/dec_jpeg_hl.h $(VSDK_APP_PATH)
	mkdir -p $(VSDK_APP_PATH)/include
	cp ./include/dec_jpeg_hl.h $(VSDK_APP_PATH)/include
$(VSDK_APP_PATH)/include/dec_h264_hl.h : ./include/dec_h264_hl.h $(VSDK_APP_PATH)
	mkdir -p $(VSDK_APP_PATH)/include
	cp ./include/dec_h264_hl.h $(VSDK_APP_PATH)/include
$(VSDK_APP_PATH)/include/video_app.h : ./include/video_app.h $(VSDK_APP_PATH)
	mkdir -p $(VSDK_APP_PATH)/include
	cp ./include/video_app.h $(VSDK_APP_PATH)/include
.PHONY: $(VSDK_APP_PATH)/include/video_app_cfg.h
$(VSDK_APP_PATH)/include/video_app_cfg.h :
	mkdir -p $(VSDK_APP_PATH)/include
	cp $(__APP_CFG_DIR)/video_app_cfg.h $(VSDK_APP_PATH)/include

# GENERATE CPP FILE FOR CONFGURATION FILE LOADING 
src/cfg_file.cpp : ../../app_video_listener/cfg/application_cfg.h generate.sh
	./generate.sh $< $@

# CLEAN TARGET
.PHONY : clean
clean :
	-rm -f ./app
	-rm -f src/cfg_file.cpp
