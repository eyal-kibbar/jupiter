LOCAL_PATH := $(call my-dir)

COMMON_DEFINES := \
	HEXPOD \
	HEXPOD_MAX_INSTRUCTIONS=8 \


include $(LOCAL_PATH)/build/debug.mk
include $(LOCAL_PATH)/build/release.mk
