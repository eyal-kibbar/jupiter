

# libs:
# 	uart-dbg
# 	uart0-dbg
# 	uart1-dbg
#   uart-multicore-dbg
#
# tasks:
#	uart-tst
#   uart-multicore-tst
#
# exe:
#	io-tst-uart
#	io-tst-uart-multicore
#

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := uart-dbg
LOCAL_DEFINES := \
	LOG_LEVEL=3 \
	UART_CORE_INDEX=0 \


LOCAL_EXPORT := \
	$(LOCAL_PATH)/api \
	$(LOCAL_PATH)/api/arch/avr \

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/io_uart.c \
	$(LOCAL_PATH)/src/common/io_uart_unicore.c \
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

LOCAL_TARGET      := atmega2560
include $(BUILD_STATIC_LIB)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := uart0-dbg
LOCAL_DEFINES := \
	LOG_LEVEL=3 \
	UART_CORE_INDEX=0 \


LOCAL_EXPORT := \
	$(LOCAL_PATH)/api \
	$(LOCAL_PATH)/api/arch/avr \

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/io_uart.c \
	$(LOCAL_PATH)/src/arch/avr/io_uart_platform.c \

LOCAL_INC := \
	$(LOCAL_PATH)/inc \
	$(LOCAL_PATH)/inc/arch/avr

LOCAL_OBJCOPY_ARGS := \
	--redefine-sym io_uart_init_core=io_uart_init_core0 \
	--redefine-sym io_uart_platform_init=io_uart_platform_init_core0 \
	--redefine-sym io_uart_sg_core=io_uart_sg_core0 \
	--redefine-sym p_uart=p_uart_core0 \
	--redefine-sym uart_used=uart_used_core0 \


LOCAL_STATIC_LIBS := \
	ganymede-dbg


LOCAL_TARGET := atmega2560
include $(BUILD_STATIC_LIB)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := uart1-dbg
LOCAL_DEFINES := \
	LOG_LEVEL=3 \
	UART_CORE_INDEX=1 \


LOCAL_EXPORT := \
	$(LOCAL_PATH)/api \
	$(LOCAL_PATH)/api/arch/avr \

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/io_uart.c \
	$(LOCAL_PATH)/src/arch/avr/io_uart_platform.c \

LOCAL_INC := \
	$(LOCAL_PATH)/inc \
	$(LOCAL_PATH)/inc/arch/avr

LOCAL_OBJCOPY_ARGS := \
	--redefine-sym io_uart_init_core=io_uart_init_core1 \
	--redefine-sym io_uart_platform_init=io_uart_platform_init_core1 \
	--redefine-sym io_uart_sg_core=io_uart_sg_core1 \
	--redefine-sym p_uart=p_uart_core1 \
	--redefine-sym uart_used=uart_used_core1 \


LOCAL_STATIC_LIBS := \
	ganymede-dbg


LOCAL_TARGET := atmega2560
include $(BUILD_STATIC_LIB)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := uart-multicore-dbg
LOCAL_DEFINES := \
	LOG_LEVEL=3 \

LOCAL_EXPORT := \
	$(LOCAL_PATH)/api \
	$(LOCAL_PATH)/api/arch/avr \

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/io_uart_multicore.c \

LOCAL_INC := \
	$(LOCAL_PATH)/inc \
	$(LOCAL_PATH)/inc/arch/avr

LOCAL_TARGET := atmega2560
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

LOCAL_TARGET := atmega2560
include $(BUILD_GMD_TASK)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := uart-multicore-tst

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_STATIC_LIBS := \
	uart0-dbg \
	uart1-dbg \
	uart-multicore-dbg \
	ganymede-dbg \
	logging

LOCAL_SRC := $(LOCAL_PATH)/tst/uart_test.c

LOCAL_TARGET      := atmega2560
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

LOCAL_TARGET      := atmega2560
include $(BUILD_EXEC)


# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := io-tst-uart-multicore

LOCAL_STATIC_LIBS := \
	uart-multicore-dbg \
	uart0-dbg \
	uart1-dbg \
	ganymede-dbg \
	logging

LOCAL_TASKS       := \
	uart-multicore-tst

LOCAL_TARGET := atmega2560
include $(BUILD_EXEC)
