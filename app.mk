

CFLAGS += -Werror

# kernel
include $(ROOT_PATH)/ganymede/rules.mk
include $(ROOT_PATH)/template/rules.mk

# libraries
include $(ROOT_PATH)/clk/rules.mk
include $(ROOT_PATH)/servo/rules.mk
include $(ROOT_PATH)/srv/rules.mk

# drivers
include $(ROOT_PATH)/barometer/rules.mk
include $(ROOT_PATH)/mpu/rules.mk


# apps
include $(ROOT_PATH)/hexpod/rules.mk

include $(ROOT_PATH)/test/rules.mk

include $(ROOT_PATH)/rules.mk
