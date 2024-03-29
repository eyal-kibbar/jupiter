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
	uart-dbg \
	servo-dbg \
	mpu-dbg \
	srv-dbg \
	logging

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := jupiter-test-dbg

LOCAL_DEFINES :=

LOCAL_STATIC_LIBS := \
	ganymede-dbg \
	uart-dbg \
	logging

LOCAL_TASKS := test

# for printing floats
LOCAL_LDFLAGS := -Wl,-u,vfprintf -lprintf_flt

LOCAL_TARGET := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET := atmega328p
include $(BUILD_EXEC)
