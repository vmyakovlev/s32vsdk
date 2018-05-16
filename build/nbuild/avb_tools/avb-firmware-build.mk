##################################################################################################
# 
# NXP Confidential Proprietary
#
# Copyright 2017 NXP 
# All Rights Reserved
#
#############################################################################
#
# THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
# IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.
#
##################################################################################################

ifeq ($(origin AVB_DIRECTORY),undefined)
 $(error error: AVB_DIRECTORY undefined)
endif

ifeq ($(origin AVB_FIRMWARE_TYPE),undefined)
 $(error error: AVB_FIRMWARE_TYPE undefined)
endif

ifeq ($(origin AVB_APP_TYPE),undefined)
 $(error error: AVB_APP_TYPE undefined)
endif

$(AVB_DIRECTORY)/apps/app_video_listener_userspace/vsdk/src/cfg_file.cpp:
	$(MAKE) -C $(AVB_DIRECTORY) TOOLCHAIN_DIR=$(dir $(firstword $(shell which aarch64-elf-gcc))) COMPILE_PREFIX=aarch64-elf- app=$(AVB_FIRMWARE_TYPE) $(ERRORFILT) ;\
	$(MAKE) -C $(AVB_DIRECTORY)/apps/app_video_listener_userspace/vsdk src/cfg_file.cpp $(ERRORFILT) ;\
	cp $(AVB_DIRECTORY)/apps/app_video_listener_userspace/vsdk/cfg/$(AVB_APP_TYPE)/vid_list.cfg . $(ERRORFILT) ;\
	cp $(AVB_DIRECTORY)/output/theA5App.bin . $(ERRORFILT)

clean::
	$(MAKE) -C $(AVB_DIRECTORY) TOOLCHAIN_DIR=$(dir $(firstword $(shell which aarch64-elf-gcc))) COMPILE_PREFIX=aarch64-elf- app=$(AVB_FIRMWARE_TYPE) clean $(ERRORFILT) ;\
	$(MAKE) -C $(AVB_DIRECTORY)/apps/app_video_listener_userspace/vsdk clean $(ERRORFILT) ;\
	rm -rf theA5App.bin $(ERRORFILT) ;\
	rm -rf vid_list.cfg $(ERRORFILT)
