# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := airplane-receiver-dbg

LOCAL_DEFINES := \
	STACK_SIZE=512 \
	LOG_LEVEL=3

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/airplane_receiver.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	spi \
	uart \
	logging \
	failsafe \
	nRF24L01 \
	ganymede \

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := airplane-receiver

LOCAL_DEFINES := \
	STACK_SIZE=512 \
	LOG_LEVEL=0

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/airplane_receiver.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	spi \
	uart \
	logging \
	failsafe \
	nRF24L01 \
	ganymede \


LOCAL_CFLAGS := -O3

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)
