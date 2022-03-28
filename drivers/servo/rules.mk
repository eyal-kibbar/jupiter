LOCAL_PATH := $(call my-dir)


COMMON_SRC := $(LOCAL_PATH)/src/common/servo.c
COMMON_INC := $(LOCAL_PATH)/inc

COMMON_DEFINES := \
	SERVO \
	SERVO_MAX_PIN=8


include $(LOCAL_PATH)/build/debug.mk
include $(LOCAL_PATH)/build/release.mk

