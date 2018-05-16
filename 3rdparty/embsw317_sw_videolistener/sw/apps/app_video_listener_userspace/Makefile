#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   File: Makefile
#   Copyright (c) 2016 NXP Semiconductors
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ifeq ($(CROSS_COMPILE),)
    $(error I need the CROSS_COMPILE information)
endif

app: main.c
	rm -f app
	$(CROSS_COMPILE)gcc -I../../kernel/sm_drv/include -I../../kernel/sram_drv/include -I../app_video_listener/include -I../app_video_listener/cfg -o app main.c

clean:
	rm -f app
