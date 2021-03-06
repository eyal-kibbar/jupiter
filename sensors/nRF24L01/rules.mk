LOCAL_PATH := $(call my-dir)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := nRF24L01-dbg

LOCAL_DEFINES := \
	LOG_LEVEL=3


LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/nRF24L01.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	ganymede-dbg \
	spi-dbg \
	logging \
	io-dbg \

LOCAL_TARGET := atmega168
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega328p
include $(BUILD_STATIC_LIB)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := nRF24L01

LOCAL_DEFINES := \
	RELEASE


LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/nRF24L01.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	ganymede \
	spi \
	logging \
	io \

LOCAL_CFLAGS := -O3

LOCAL_TARGET := atmega168
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega328p
include $(BUILD_STATIC_LIB)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := nRF24L01-mstr-tst

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_STATIC_LIBS := \
	nRF24L01-dbg \
	ganymede-dbg \
	logging \
	io-dbg \

LOCAL_SRC := $(LOCAL_PATH)/tst/nRF24L01_test_master.c

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := nRF24L01-slav-tst

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_STATIC_LIBS := \
	nRF24L01-dbg \
	ganymede-dbg \
	logging \
	io-dbg \

LOCAL_SRC := $(LOCAL_PATH)/tst/nRF24L01_test_slave.c

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)


# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := jupiter-nRF24L01-slav-tst

LOCAL_STATIC_LIBS := \
	nRF24L01-dbg \
	ganymede-dbg \
	spi-dbg \
	uart-dbg \
	logging \
	io-dbg \


LOCAL_TASKS       := \
	nRF24L01-slav-tst


LOCAL_TARGET      := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega328p
include $(BUILD_EXEC)

# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := jupiter-nRF24L01-mstr-tst

LOCAL_STATIC_LIBS := \
	nRF24L01-dbg \
	ganymede-dbg \
	spi-dbg \
	uart-dbg \
	logging \
	io-dbg \


LOCAL_TASKS       := \
	nRF24L01-mstr-tst


LOCAL_TARGET      := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega328p
include $(BUILD_EXEC)
