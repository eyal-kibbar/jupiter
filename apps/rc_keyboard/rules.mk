LOCAL_PATH := $(call my-dir)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := rc_keyboard-dbg

LOCAL_DEFINES := \
	LOG_LEVEL=3 \
	STACK_SIZE=256 \

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/rc_keyboard.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	logging \
	io-dbg \
	uart-dbg \
	ganymede-dbg

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)


# exectuable
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := jupiter-rc_keyboard

LOCAL_STATIC_LIBS := \
	logging \
	io-dbg \
	uart-dbg \
	ganymede \

LOCAL_TASKS       := \
	rc_keyboard-dbg


LOCAL_TARGET      := atmega328p
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega168
include $(BUILD_EXEC)
