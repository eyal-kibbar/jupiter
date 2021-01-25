LOCAL_PATH := $(call my-dir)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := radio-tx-dbg

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/radio_tx.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	nRF24L01-dbg \
	uart-dbg \
	spi-dbg \
	io-dbg \
	io-analog-dbg \
	logging \
	ganymede-dbg

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := radio-tx

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=0

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/radio_tx.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	nRF24L01 \
	uart \
	spi \
	io \
	io-analog \
	logging \
	ganymede

LOCAL_CFLAGS := -O3


LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)



# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := radio-rx-dbg

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/radio_rx.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	nRF24L01 \
	uart \
	spi \
	io \
	servo-dbg \
	logging \
	ganymede

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := radio-rx

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3 \
	RELEASE

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/radio_rx.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	nRF24L01 \
	uart \
	spi \
	servo-dbg \
	logging \
	ganymede

LOCAL_CFLAGS := -O3

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := radio-tst

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_STATIC_LIBS := \
	uart-dbg \
	io-analog-dbg \
	logging \
	ganymede-dbg

LOCAL_SRC := $(LOCAL_PATH)/tst/radio_test.c

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := jupiter-radio-tst

LOCAL_DEFINES := \
	LOG_LEVEL=3

LOCAL_STATIC_LIBS := \
	uart-dbg \
	io-analog-dbg \
	logging \
	ganymede-dbg

LOCAL_TASKS       := \
	radio-tst \

LOCAL_TARGET := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET := atmega328p
include $(BUILD_EXEC)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := jupiter-radio-tx-dbg

LOCAL_DEFINES := \
	LOG_LEVEL=3

LOCAL_STATIC_LIBS := \
	nRF24L01-dbg \
	uart-dbg \
	spi-dbg \
	io-dbg \
	io-analog-dbg \
	logging \
	ganymede-dbg

LOCAL_TASKS := \
	radio-tx-dbg \

LOCAL_TARGET := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET := atmega328p
include $(BUILD_EXEC)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := jupiter-radio-tx

LOCAL_DEFINES := \
	RELEASE

LOCAL_STATIC_LIBS := \
	nRF24L01 \
	uart \
	spi \
	io \
	io-analog \
	logging \
	ganymede

LOCAL_TASKS := \
	radio-tx

LOCAL_CFLAGS := -O3

LOCAL_TARGET := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET := atmega328p
include $(BUILD_EXEC)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := jupiter-radio-rx-dbg

LOCAL_DEFINES := \
	LOG_LEVEL=3

LOCAL_STATIC_LIBS := \
	nRF24L01 \
	uart \
	spi \
	io \
	servo-dbg \
	logging \
	ganymede

LOCAL_TASKS := \
	radio-rx-dbg

LOCAL_TARGET := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET := atmega328p
include $(BUILD_EXEC)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := jupiter-radio-rx

LOCAL_DEFINES := \
	LOG_LEVEL=0 \
	RELEASE

LOCAL_STATIC_LIBS := \
	nRF24L01 \
	uart \
	spi \
	io \
	servo-dbg \
	logging \
	ganymede

LOCAL_TASKS := \
	radio-rx

LOCAL_CFLAGS := -O3

LOCAL_TARGET := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET := atmega328p
include $(BUILD_EXEC)
