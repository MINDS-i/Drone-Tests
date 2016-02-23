
MCU=-mmcu=atmega2560
CPU_SPEED=-DF_CPU=16000000UL
CFLAGS=$(MCU) $(CPU_SPEED) -std=gnu++11 -g -Os -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD

simulation:
	g++ -I/usr/include/simavr -lsimavr ./sim.cpp -o sim

elf:
	avr-g++ $(CFLAGS) main.cpp -o ./main.elf
