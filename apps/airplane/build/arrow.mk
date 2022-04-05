
# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := arrow-dbg

LOCAL_DEFINES := \
	STACK_SIZE=512 \
	LOG_LEVEL=3

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/arrow.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	pid \
	mpu \
	io \
	i2c-mstr \
	uart \
	servo \
	logging \
	failsafe \
	ganymede \

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := arrow

LOCAL_DEFINES := \
	STACK_SIZE=512 \
	LOG_LEVEL=0

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/arrow.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	pid \
	mpu \
	io \
	i2c-mstr \
	servo \
	failsafe \
	ganymede \


LOCAL_CFLAGS := -O3

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)



# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := arrow-tst

LOCAL_DEFINES := \
	STACK_SIZE=512 \
	LOG_LEVEL=3

LOCAL_STATIC_LIBS := \
	arrow-dbg \
	ganymede-dbg

LOCAL_SRC := $(LOCAL_PATH)/tst/arrow_test.c

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := jupiter-arrow-dbg

LOCAL_STATIC_LIBS := \
	io \
	spi \
	pid \
	i2c-mstr \
	mpu \
	uart \
	servo \
	logging \
	failsafe \
	nRF24L01 \
	ganymede

LOCAL_TASKS := arrow-dbg airplane-receiver-dbg

LOCAL_TARGET := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET := atmega328p
include $(BUILD_EXEC)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := jupiter-arrow


LOCAL_STATIC_LIBS := \
	io \
	spi \
	pid \
	i2c-mstr \
	mpu \
	uart \
	servo \
	logging \
	failsafe \
	nRF24L01 \
	ganymede

LOCAL_TASKS := arrow airplane-receiver

LOCAL_TARGET := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET := atmega328p
include $(BUILD_EXEC)
