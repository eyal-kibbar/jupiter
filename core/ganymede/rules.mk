

LOCAL_PATH := $(call my-dir)

SRC_avr := $(LOCAL_PATH)/src/arch/avr/gmd_platform.c
INC_avr := $(LOCAL_PATH)/inc/arch/avr/

SRC_linux := $(LOCAL_PATH)/src/arch/linux/gmd_platform.c
INC_linux := $(LOCAL_PATH)/inc/arch/linux/


COMMON_SRC := \
	$(LOCAL_PATH)/src/common/gmd_kern.c \
	$(LOCAL_PATH)/src/common/gmd_sched.c

COMMON_INC := $(LOCAL_PATH)/inc
COMMON_API := $(LOCAL_PATH)/api

COMMON_DEFINES := \
	GANYMEDE 


include $(LOCAL_PATH)/build/debug.mk
include $(LOCAL_PATH)/build/release.mk
