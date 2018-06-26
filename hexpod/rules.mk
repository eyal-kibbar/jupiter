LOCAL_PATH := $(call my-dir)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := hexpod-dbg

LOCAL_DEFINES := \
	HEXPOD_MAX_INSTRUCTIONS=30 \
	HEXPOD_SERVO_DELAY_MS=2000 \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/hexpod.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	servo-dbg \
	ganymede-dbg

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := hexpod-srv-dbg

LOCAL_DEFINES := \
	HEXPOD_MAX_INSTRUCTIONS=30 \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/hexpod_srv.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	srv-dbg \
	ganymede-dbg

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)





