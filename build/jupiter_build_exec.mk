

include $(CHECK_LOCALS)

_DIR_OUT := $(DIR_OUT)/$(LOCAL_TARGET)

$(shell mkdir -p $(_DIR_OUT))


USED_LIBS  := $(patsubst %, $(_DIR_OUT)/lib%.a, $(LOCAL_STATIC_LIBS))
USED_TASKS := $(patsubst %, $(_DIR_OUT)/tsk%.o, $(LOCAL_TASKS))

LIBS_FLAGS  := $(addprefix -l, $(LOCAL_STATIC_LIBS))
TASKS_FLAGS := $(USED_TASKS)

$(_DIR_OUT)/$(LOCAL_MODULE).elf: LIBS_FLAGS    := $(LIBS_FLAGS)
$(_DIR_OUT)/$(LOCAL_MODULE).elf: TASKS_FLAGS   := $(TASKS_FLAGS)
$(_DIR_OUT)/$(LOCAL_MODULE).elf: LOCAL_LDFLAGS := $(LOCAL_LDFLAGS)
$(_DIR_OUT)/$(LOCAL_MODULE).elf: LOCAL_TARGET  := $(LOCAL_TARGET)
$(_DIR_OUT)/$(LOCAL_MODULE).elf: LOCAL_TASKS   := $(LOCAL_TASKS)
$(_DIR_OUT)/$(LOCAL_MODULE).elf: $(USED_LIBS) $(USED_TASKS)
	$(CC_$(LOCAL_TARGET)) $(CFLAGS) $(CFLAGS_$(LOCAL_TARGET)) $(LOCAL_LDFLAGS) -o $@ -L$(@D) $(TASKS_FLAGS) $(LIBS_FLAGS)
	

ifeq ($(LOCAL_TARGET), atmega168)

$(_DIR_OUT)/$(LOCAL_MODULE).hex: LOCAL_TARGET := $(LOCAL_TARGET)
$(_DIR_OUT)/$(LOCAL_MODULE).hex: $(_DIR_OUT)/$(LOCAL_MODULE).elf
	$(OBJCOPY_$(LOCAL_TARGET)) -O ihex -R .eeprom $< $@

upload_$(LOCAL_TARGET)_$(LOCAL_MODULE): $(_DIR_OUT)/$(LOCAL_MODULE).hex
	avrdude -F -V -c arduino -p ATMEGA3290 -P /dev/ttyUSB0 -b 57600 -U flash:w:$<
endif


ifeq ($(LOCAL_TARGET), atmega328p)

$(_DIR_OUT)/$(LOCAL_MODULE).hex: LOCAL_TARGET := $(LOCAL_TARGET)
$(_DIR_OUT)/$(LOCAL_MODULE).hex: $(_DIR_OUT)/$(LOCAL_MODULE).elf
	$(OBJCOPY_$(LOCAL_TARGET)) -O ihex -R .eeprom $< $@

upload_$(LOCAL_TARGET)_$(LOCAL_MODULE): $(_DIR_OUT)/$(LOCAL_MODULE).hex
	avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyUSB0 -b 57600 -U flash:w:$<
endif



EXECUTABLES += $(_DIR_OUT)/$(LOCAL_MODULE).elf


