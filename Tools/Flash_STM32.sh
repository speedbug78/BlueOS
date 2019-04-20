!#/bin/sh
#Program the STM32 processor via a serial connection.  Processor must be booted in load mode via BOOT jumpers.
stm32flash /dev/ttyUSB0 -w /home/daniel/Code/BlueOS/bin/BlueOS.hex -v
#Start python sterial terminal.
#python -m serial.tools.miniterm /dev/ttyUB0 115200
