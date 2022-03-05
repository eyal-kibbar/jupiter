

# libs:
#	i2c-mstr-dbg
#	i2c-slav-dbg
#
# tasks:
#	i2c-slav-tst
#	i2c-mstr-tst
#
# exe:
#	io-tst-i2c-mstr
#	io-tst-i2c-slav
#

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := i2c-mstr-dbg
LOCAL_DEFINES := \
	LOG_LEVEL=3

LOCAL_EXPORT := \
	$(LOCAL_PATH)/api \
	$(LOCAL_PATH)/api/arch/avr \

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/io_i2c_master.c \
	$(LOCAL_PATH)/src/arch/avr/io_i2c_platform.c \

LOCAL_INC := \
	$(LOCAL_PATH)/inc \
	$(LOCAL_PATH)/inc/arch/avr


LOCAL_STATIC_LIBS := \
	ganymede-dbg

LOCAL_TARGET := atmega168
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega328p
include $(BUILD_STATIC_LIB)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := i2c-mstr
LOCAL_DEFINES := \
	RELEASE

LOCAL_EXPORT := \
	$(LOCAL_PATH)/api \
	$(LOCAL_PATH)/api/arch/avr \

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/io_i2c_master.c \
	$(LOCAL_PATH)/src/arch/avr/io_i2c_platform.c \

LOCAL_INC := \
	$(LOCAL_PATH)/inc \
	$(LOCAL_PATH)/inc/arch/avr


LOCAL_STATIC_LIBS := \
	ganymede

LOCAL_CFLAGS := -O3

LOCAL_TARGET := atmega168
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega328p
include $(BUILD_STATIC_LIB)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := i2c-slav-dbg
LOCAL_DEFINES := \
	LOG_LEVEL=3

LOCAL_EXPORT := \
	$(LOCAL_PATH)/api \
	$(LOCAL_PATH)/api/arch/avr \

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/io_i2c_slave.c \
	$(LOCAL_PATH)/src/arch/avr/io_i2c_platform.c \

LOCAL_INC := \
	$(LOCAL_PATH)/inc \
	$(LOCAL_PATH)/inc/arch/avr


LOCAL_STATIC_LIBS := \
	io-dbg \
	ganymede-dbg

LOCAL_TARGET := atmega168
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega328p
include $(BUILD_STATIC_LIB)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := i2c-mstr-tst

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_STATIC_LIBS := \
	i2c-mstr-dbg \
	ganymede-dbg \
	logging

LOCAL_SRC := $(LOCAL_PATH)/tst/i2c_master_test.c

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := i2c-slav-tst

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_STATIC_LIBS := \
	i2c-slav-dbg \
	ganymede-dbg \
	logging

LOCAL_SRC := $(LOCAL_PATH)/tst/i2c_slave_test.c

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)

# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := io-tst-i2c-mstr

LOCAL_STATIC_LIBS := \
	i2c-mstr-dbg \
	ganymede-dbg \
	uart-dbg \
	logging \


LOCAL_TASKS       := \
	i2c-mstr-tst


LOCAL_TARGET      := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega328p
include $(BUILD_EXEC)


# I2C test
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := io-tst-i2c-slav

LOCAL_STATIC_LIBS := \
	i2c-slav-dbg \
	ganymede-dbg \
	uart-dbg \
	logging \

LOCAL_TASKS       := \
	i2c-slav-tst


LOCAL_TARGET      := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega328p
include $(BUILD_EXEC)
