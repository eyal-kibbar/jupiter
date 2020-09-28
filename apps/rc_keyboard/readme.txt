
# restore bootloader
sudo dfu-programmer atmega16u2 erase
sudo dfu-programmer atmega16u2 flash apps/rc_keyboard/firmware/Arduino-usbserial-uno.hex
sudo dfu-programmer atmega16u2 reset

# flash keyboard firmware
sudo dfu-programmer atmega16u2 erase
sudo dfu-programmer atmega16u2 flash apps/rc_keyboard/firmware/Arduino-keyboard-0.3.hex
sudo dfu-programmer atmega16u2 reset


