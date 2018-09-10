LOCAL_PATH := $(call my-dir)

COMMON_DEFINES := \
	SRV \
	SRV_DELIMITER=\"abcd\"


include $(LOCAL_PATH)/build/debug.mk
include $(LOCAL_PATH)/build/release.mk

