

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

# tools
CC_avr := $(V_PREFIX)avr-gcc
AR_avr := $(V_PREFIX)avr-ar
OBJCOPY_avr := $(V_PREFIX)avr-objcopy

# arch CFLAGS
CFLAGS_avr :=


# atmega168
TARGET := atmega168
ARCH_$(TARGET) := avr

CFLAGS_$(TARGET)  := $(CFLAGS_$(ARCH_$(TARGET)))
CFLAGS_$(TARGET)  += -DF_CPU=16000000UL -mmcu=$(TARGET)
CC_$(TARGET)      := $(CC_$(ARCH_$(TARGET)))
AR_$(TARGET)      := $(AR_$(ARCH_$(TARGET)))
OBJCOPY_$(TARGET) := $(OBJCOPY_$(ARCH_$(TARGET)))

SUPPORTED_TARGETS += $(TARGET)


# atmega328p
TARGET := atmega328p
ARCH_$(TARGET) := avr

CFLAGS_$(TARGET)  := $(CFLAGS_$(ARCH_$(TARGET)))
CFLAGS_$(TARGET)  += -DF_CPU=16000000UL -mmcu=$(TARGET)
CC_$(TARGET)      := $(CC_$(ARCH_$(TARGET)))
AR_$(TARGET)      := $(AR_$(ARCH_$(TARGET)))
OBJCOPY_$(TARGET) := $(OBJCOPY_$(ARCH_$(TARGET)))

SUPPORTED_TARGETS += $(TARGET)


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


# DO NOT DELETE
