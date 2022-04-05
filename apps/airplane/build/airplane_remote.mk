
# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := airplane-remote-dbg

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/airplane_remote.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	spi-dbg \
	uart-dbg \
	logging \
	nRF24L01-dbg \
	ganymede-dbg \

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := jupiter-airplane-remote-dbg

LOCAL_DEFINES :=

LOCAL_STATIC_LIBS := \
	spi \
	nRF24L01 \
	uart \
	logging \
	io \
	io-analog \
	ganymede \


LOCAL_TASKS := airplane-remote-dbg

# for printing floats
LOCAL_LDFLAGS := -Wl,-u,vfprintf -lprintf_flt

LOCAL_TARGET := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET := atmega328p
include $(BUILD_EXEC)
