#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   File: target_config.mak
#   Copyright (c) 2016 NXP Semiconductors
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   Needed variables:
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   MAKE_CMD    - the make command
#   OBJ_PARAMS  - make command parameters
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#   ++++++++++++++++++++++++++++++++++++++
#   Targets. Add more options when needed.
#   ++++++++++++++++++++++++++++++++++++++
a5_startup:
	$(MAKE_CMD) -C ./toolchain/a5_startup $(OBJ_PARAMS)
a5_startup_no_mmu:
	$(MAKE_CMD) -C ./toolchain/a5_startup $(OBJ_PARAMS) NO_MMU=TRUE
a5_custom_app:
	$(MAKE_CMD) -C ./apps/sample_a5_app $(OBJ_PARAMS)
a5_test_app: common_test
	$(MAKE_CMD) -C ./tests/$(notdir $(MODULE))/$(test_num) $(OBJ_PARAMS)
sm_core:
	$(MAKE_CMD) -C ./packages/sm_core $(OBJ_PARAMS)
app_video_listener:
	$(MAKE_CMD) -C ./apps/app_video_listener $(OBJ_PARAMS)
common:
	$(MAKE_CMD) -C ./apps/common $(OBJ_PARAMS)
common_test:
	$(MAKE_CMD) -C ./tests/common $(OBJ_PARAMS)
autolibc:
	$(MAKE_CMD) -C ./packages/autolibc $(OBJ_PARAMS)
fsl_printf:
	$(MAKE_CMD) -C ./packages/fsl_printf $(OBJ_PARAMS)
mmu:
	$(MAKE_CMD) -C ./packages/mmu $(OBJ_PARAMS)
gic:
	$(MAKE_CMD) -C ./packages/gic $(OBJ_PARAMS)
pit:
	$(MAKE_CMD) -C ./packages/pit $(OBJ_PARAMS)
app_mmu:
	$(MAKE_CMD) -C ./apps/app_mmu $(OBJ_PARAMS)
eth_queue:
	$(MAKE_CMD) -C ./packages/eth_queue $(OBJ_PARAMS)
dec_feed:
	$(MAKE_CMD) -C ./packages/dec_feed $(OBJ_PARAMS)
stream_core:
	$(MAKE_CMD) -C ./packages/stream_core $(OBJ_PARAMS)
