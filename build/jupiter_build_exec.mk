

include $(CHECK_LOCALS)

_DIR_OUT := $(DIR_OUT)/$(LOCAL_TARGET)

$(shell mkdir -p $(_DIR_OUT))


USED_LIBS  := $(patsubst %, $(_DIR_OUT)/libs/lib%.a, $(LOCAL_STATIC_LIBS))
USED_TASKS := $(patsubst %, $(_DIR_OUT)/tasks/tsk%.o, $(LOCAL_TASKS))

LIBS_FLAGS  := $(addprefix -l, $(LOCAL_STATIC_LIBS))
TASKS_FLAGS := $(USED_TASKS)

$(_DIR_OUT)/$(LOCAL_MODULE).elf: LIBS_FLAGS    := $(LIBS_FLAGS)
$(_DIR_OUT)/$(LOCAL_MODULE).elf: TASKS_FLAGS   := $(TASKS_FLAGS)
$(_DIR_OUT)/$(LOCAL_MODULE).elf: LOCAL_LDFLAGS := $(LOCAL_LDFLAGS)
$(_DIR_OUT)/$(LOCAL_MODULE).elf: LOCAL_TARGET  := $(LOCAL_TARGET)
$(_DIR_OUT)/$(LOCAL_MODULE).elf: LOCAL_TASKS   := $(LOCAL_TASKS)
$(_DIR_OUT)/$(LOCAL_MODULE).elf: $(USED_LIBS) $(USED_TASKS)
	$(CC_$(LOCAL_TARGET)) $(CFLAGS) $(CFLAGS_$(LOCAL_TARGET)) $(LDFLAGS_$(LOCAL_TARGET)) $(LOCAL_LDFLAGS) -o $@ -L$(@D)/libs $(TASKS_FLAGS) $(LIBS_FLAGS)


ifeq ($(LOCAL_TARGET), atmega168)

$(_DIR_OUT)/$(LOCAL_MODULE).hex: LOCAL_TARGET := $(LOCAL_TARGET)
$(_DIR_OUT)/$(LOCAL_MODULE).hex: $(_DIR_OUT)/$(LOCAL_MODULE).elf
	$(OBJCOPY_$(LOCAL_TARGET)) -O ihex -R .eeprom $< $@

upload_$(LOCAL_TARGET)_$(LOCAL_MODULE): $(_DIR_OUT)/$(LOCAL_MODULE).hex
	avrdude -F -V -c arduino -p ATMEGA3290 -P $(AVRDUDE_PORT) -b 57600 -U flash:w:$<
endif


ifeq ($(LOCAL_TARGET), atmega328p)

$(_DIR_OUT)/$(LOCAL_MODULE).hex: LOCAL_TARGET := $(LOCAL_TARGET)
$(_DIR_OUT)/$(LOCAL_MODULE).hex: $(_DIR_OUT)/$(LOCAL_MODULE).elf
	$(OBJCOPY_$(LOCAL_TARGET)) -O ihex -R .eeprom $< $@

upload_$(LOCAL_TARGET)_$(LOCAL_MODULE): $(_DIR_OUT)/$(LOCAL_MODULE).hex
	avrdude -F -V -c arduino -p ATMEGA328P -P $(AVRDUDE_PORT) -b $(AVRDUDE_BAUD) -U flash:w:$<

upload_prog_$(LOCAL_TARGET)_$(LOCAL_MODULE): $(_DIR_OUT)/$(LOCAL_MODULE).hex
	avrdude -v -c stk500v1 -p ATMEGA328P -P $(AVRDUDE_PORT) -b 19200 -U flash:w:$<:i
endif

ifeq ($(LOCAL_TARGET), atmega2560)

$(_DIR_OUT)/$(LOCAL_MODULE).hex: LOCAL_TARGET := $(LOCAL_TARGET)
$(_DIR_OUT)/$(LOCAL_MODULE).hex: $(_DIR_OUT)/$(LOCAL_MODULE).elf
	$(OBJCOPY_$(LOCAL_TARGET)) -O ihex -R .eeprom $< $@

upload_$(LOCAL_TARGET)_$(LOCAL_MODULE): $(_DIR_OUT)/$(LOCAL_MODULE).hex
	avrdude -D -c wiring -p atmega2560 -P $(AVRDUDE_PORT) -b 115200 -U flash:w:$<
endif


EXECUTABLES += $(_DIR_OUT)/$(LOCAL_MODULE).elf
