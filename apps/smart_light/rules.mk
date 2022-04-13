LOCAL_PATH := $(call my-dir)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := smart_light-dbg

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3 \
	LIGHT_SENSOR_THRESH=700 \
	DEBUG

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/smart_light.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	uart \
	logging \
	io \
	io-analog \
	ganymede \

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)




# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := smart_light

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=0 \
	LIGHT_SENSOR_THRESH=200 \
	RELEASE

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/smart_light.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	io \
	io-analog \
	ganymede \

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)



# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := smart_light-tst

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_STATIC_LIBS := \
	smart_light-dbg \
	ganymede-dbg

LOCAL_SRC := $(LOCAL_PATH)/tst/smart_light_test.c

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)



# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := jupiter-smart_light-dbg

LOCAL_DEFINES :=

LOCAL_STATIC_LIBS := \
	uart \
	logging \
	io \
	io-analog \
	ganymede \



LOCAL_TASKS := smart_light-dbg

# for printing floats
LOCAL_LDFLAGS :=

LOCAL_TARGET := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET := atmega328p
include $(BUILD_EXEC)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := jupiter-smart_light

LOCAL_DEFINES :=

LOCAL_STATIC_LIBS := \
	io \
	io-analog \
	ganymede \



LOCAL_TASKS := smart_light

# for printing floats
LOCAL_LDFLAGS :=

LOCAL_TARGET := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET := atmega328p
include $(BUILD_EXEC)
