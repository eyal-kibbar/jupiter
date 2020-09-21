

# libs:
# 	spi-dbg
# 	uart-dbg
#	i2c-mstr-dbg
#	i2c-slav-dbg
#	logging
#
# tasks:
#	spi-slav-tst
#	spi-mstr-tst
#
#	i2c-slav-tst
#	i2c-mstr-tst
#
#	uart-tst
#
# exe:
#	io-tst-spi-mstr
#	io-tst-spi-slav
#
#	io-tst-i2c-mstr
#	io-tst-i2c-slav
#
#	io-tst-uart
#









LOCAL_PATH := $(call my-dir)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := logging
LOCAL_DEFINES :=
LOCAL_EXPORT := \
	$(LOCAL_PATH)/api \
	$(LOCAL_PATH)/api/arch/avr \

LOCAL_SRC := \
	$(LOCAL_PATH)/src/arch/avr/io_logging_platform.c \

LOCAL_INC := \
	$(LOCAL_PATH)/inc \
	$(LOCAL_PATH)/inc/arch/avr


LOCAL_STATIC_LIBS :=

LOCAL_TARGET := atmega168
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega328p
include $(BUILD_STATIC_LIB)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := i2c-mstr-dbg
LOCAL_DEFINES := \
	IO_I2C_PRESCALER=1 \
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

LOCAL_MODULE := i2c-slav-dbg
LOCAL_DEFINES := \
	IO_I2C_PRESCALER=1 \
	LOG_LEVEL=3

LOCAL_EXPORT := \
	$(LOCAL_PATH)/api \
	$(LOCAL_PATH)/api/arch/avr \

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/io_i2c_slave.c \
	$(LOCAL_PATH)/src/arch/avr/io_i2c_platform.c \
	$(LOCAL_PATH)/src/arch/avr/io_platform.c \

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

# SPI test
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



# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := spi-dbg
LOCAL_DEFINES := \
	LOG_LEVEL=3

LOCAL_EXPORT := \
	$(LOCAL_PATH)/api \
	$(LOCAL_PATH)/api/arch/avr \

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/io_spi.c \
	$(LOCAL_PATH)/src/arch/avr/io_spi_platform.c \
	$(LOCAL_PATH)/src/arch/avr/io_platform.c

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

LOCAL_MODULE := spi-mstr-tst

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_STATIC_LIBS := \
	spi-dbg \
	ganymede-dbg \
	logging

LOCAL_SRC := $(LOCAL_PATH)/tst/spi_master_test.c

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := spi-slav-tst

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_STATIC_LIBS := \
	spi-dbg \
	ganymede-dbg \
	logging

LOCAL_SRC := $(LOCAL_PATH)/tst/spi_slave_test.c

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)

# SPI test
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := io-tst-spi-mstr

LOCAL_STATIC_LIBS := \
	spi-dbg \
	ganymede-dbg \
	uart-dbg \
	logging \


LOCAL_TASKS       := \
	spi-mstr-tst


LOCAL_TARGET      := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega328p
include $(BUILD_EXEC)


# SPI test
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := io-tst-spi-slav

LOCAL_STATIC_LIBS := \
	spi-dbg \
	ganymede-dbg \
	uart-dbg \
	logging \

LOCAL_TASKS       := \
	spi-slav-tst


LOCAL_TARGET      := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega328p
include $(BUILD_EXEC)




# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := uart-dbg
LOCAL_DEFINES := \
	LOG_LEVEL=3

LOCAL_EXPORT := \
	$(LOCAL_PATH)/api \
	$(LOCAL_PATH)/api/arch/avr \

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/io_uart.c \
	$(LOCAL_PATH)/src/arch/avr/io_uart_platform.c \

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

LOCAL_MODULE := uart-tst

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_STATIC_LIBS := \
	uart-dbg \
	ganymede-dbg \
	logging

LOCAL_SRC := $(LOCAL_PATH)/tst/uart_test.c

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)


# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := io-tst-uart

LOCAL_STATIC_LIBS := \
	uart-dbg \
	ganymede-dbg \
	logging \

LOCAL_TASKS       := \
	uart-tst


LOCAL_TARGET      := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega328p
include $(BUILD_EXEC)
