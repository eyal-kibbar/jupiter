

define my-dir
$(dir $(lastword $(MAKEFILE_LIST)))
endef


BUILD_DIR := $(call my-dir)
ROOT_PATH ?= ./

all: _all



DIR_OUT     := $(ROOT_PATH)/out
EXECUTABLES :=
LIBS        :=
TASKS       :=
SUPPORTED_TARGETS :=
V_PREFIX    := $(if $(findstring 1, $(V)), , @)

# avr
######
CC_avr      := $(V_PREFIX)avr-gcc
AR_avr      := $(V_PREFIX)avr-ar
OBJCOPY_avr := $(V_PREFIX)avr-objcopy
CFLAGS_avr  :=


# linux
#######
CC_linux      := $(V_PREFIX)gcc
AR_linux      := $(V_PREFIX)ar
OBJCOPY_linux := $(V_PREFIX)objcopy
CFLAGS_linux  := -g


include $(wildcard $(BUILD_DIR)/targets/*.mk)

# rules.mk script paths
CLEAR_VARS       := $(BUILD_DIR)/jupiter_clear_vars.mk
BUILD_STATIC_LIB := $(BUILD_DIR)/jupiter_build_static_lib.mk
BUILD_EXEC       := $(BUILD_DIR)/jupiter_build_exec.mk
BUILD_GMD_TASK   := $(BUILD_DIR)/jupiter_build_gmd_task.mk
CHECK_LOCALS     := $(BUILD_DIR)/jupiter_check.mk


CFLAGS := -g


include $(ROOT_PATH)/app.mk


$(info ========================================)
$(info )

_all: $(LIBS) $(TASKS) $(EXECUTABLES)


clean:
	rm -rf $(DIR_OUT)

dummy:
	@echo

init/%:
	$(eval PRJ := $(@F))
	$(eval PRJ_UPPER := $(shell echo $(PRJ) | tr a-z A-Z))
	@cp -rv  $(ROOT_PATH)/template $(ROOT_PATH)/$(PRJ)
	@mv $(ROOT_PATH)/$(PRJ)/tst/template_test.c   $(ROOT_PATH)/$(PRJ)/tst/$(PRJ)_test.c
	@mv $(ROOT_PATH)/$(PRJ)/api/template.h        $(ROOT_PATH)/$(PRJ)/api/$(PRJ).h
	@mv $(ROOT_PATH)/$(PRJ)/src/common/template.c $(ROOT_PATH)/$(PRJ)/src/common/$(PRJ).c
	find $(ROOT_PATH)/$(PRJ) -type f  | xargs -l1 sed -i "s/template/$(PRJ)/"
	find $(ROOT_PATH)/$(PRJ) -type f  | xargs -l1 sed -i "s/TEMPLATE/$(PRJ_UPPER)/"



# DO NOT DELETE
