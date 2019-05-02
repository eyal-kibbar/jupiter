

CFLAGS += -Werror

# core
include $(ROOT_PATH)/core/ganymede/rules.mk
#include $(ROOT_PATH)/template/rules.mk

# drivers
#include $(ROOT_PATH)/drivers/servo/rules.mk

# libs
#include $(ROOT_PATH)/libs/clk/rules.mk
#include $(ROOT_PATH)/libs/srv/rules.mk

# sensors
#include $(ROOT_PATH)/sensors/barometer/rules.mk
#include $(ROOT_PATH)/sensors/mpu/rules.mk
#include $(ROOT_PATH)/sensors/moisture/rules.mk

# apps
#include $(ROOT_PATH)/apps/hexpod/rules.mk
#include $(ROOT_PATH)/apps/invpend/rules.mk
#include $(ROOT_PATH)/apps/test/rules.mk
#include $(ROOT_PATH)/rules.mk
