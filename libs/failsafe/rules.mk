LOCAL_PATH := $(call my-dir)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := failsafe-dbg

LOCAL_DEFINES := \
	LOG_LEVEL=3

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/failsafe.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	ganymede-dbg

LOCAL_TARGET := atmega168
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega328p
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega2560
include $(BUILD_STATIC_LIB)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := failsafe

LOCAL_DEFINES := \
	LOG_LEVEL=0

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/failsafe.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	ganymede

LOCAL_TARGET := atmega168
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega328p
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega2560
include $(BUILD_STATIC_LIB)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := failsafe-tsk-dbg

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/failsafe_task.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	failsafe-dbg \
	ganymede-dbg

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega2560
include $(BUILD_GMD_TASK)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := failsafe-tsk

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=0

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/failsafe_task.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	failsafe \
	ganymede

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega2560
include $(BUILD_GMD_TASK)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := failsafe-tst

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_STATIC_LIBS := \
	failsafe-dbg \
	ganymede-dbg

LOCAL_SRC := $(LOCAL_PATH)/tst/failsafe_test.c

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega2560
include $(BUILD_GMD_TASK)
