

# libs:
# 	spi-dbg
#
# tasks:
#	spi-slav-tst
#	spi-mstr-tst
#
# exe:
#	io-tst-spi-mstr
#	io-tst-spi-slav
#

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

LOCAL_MODULE := spi
LOCAL_DEFINES := \
	RELEASE

LOCAL_EXPORT := \
	$(LOCAL_PATH)/api \
	$(LOCAL_PATH)/api/arch/avr \

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/io_spi.c \
	$(LOCAL_PATH)/src/arch/avr/io_spi_platform.c \

LOCAL_INC := \
	$(LOCAL_PATH)/inc \
	$(LOCAL_PATH)/inc/arch/avr


LOCAL_STATIC_LIBS := \
	io \
	ganymede

LOCAL_CFLAGS := -O3

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
	io-dbg \
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
	io-dbg \

LOCAL_TASKS       := \
	spi-slav-tst


LOCAL_TARGET      := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega328p
include $(BUILD_EXEC)
