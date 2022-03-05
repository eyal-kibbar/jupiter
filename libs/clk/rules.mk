LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)

LOCAL_MODULE := clk-dbg

LOCAL_DEFINES     := STACK_SIZE=170
LOCAL_SRC         := $(LOCAL_PATH)/src/common/clk.c
LOCAL_INC         := $(LOCAL_PATH)/inc
LOCAL_STATIC_LIBS :=
LOCAL_EXPORT      := $(LOCAL_PATH)/api


LOCAL_TARGET := atmega168
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega328p
include $(BUILD_STATIC_LIB)

#######################################

LOCAL_MODULE := clk_ticker-dbg

LOCAL_SRC         := $(LOCAL_PATH)/src/common/clk_task.c
LOCAL_INC         := $(LOCAL_PATH)/inc
LOCAL_STATIC_LIBS := ganymede-dbg clk-dbg
LOCAL_DEFINES := \
	STACK_SIZE=170 \
	CLK_RESOLUTION_MS=100

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)
