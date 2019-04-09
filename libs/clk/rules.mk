LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)

LOCAL_MODULE := clk-dbg

LOCAL_DEFINES     := STACK_SIZE=170
LOCAL_SRC         := $(LOCAL_PATH)/src/common/clk_task.c
LOCAL_STATIC_LIBS := ganymede-dbg

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)
