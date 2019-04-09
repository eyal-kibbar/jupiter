LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)

LOCAL_TARGET := atmega168
LOCAL_MODULE := test

LOCAL_DEFINES += STACK_SIZE=500
LOCAL_DEFINES += LOG_LEVEL=3

LOCAL_SRC += $(LOCAL_PATH)/src/common/test_task.c
LOCAL_INC += $(LOCAL_PATH)/inc

LOCAL_STATIC_LIBS := \
	ganymede-dbg \
	servo-dbg \
	mpu-dbg \
	srv-dbg

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)
