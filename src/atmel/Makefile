all: main.hex

main.elf: main.c tcs3200.c usart.c
	avr-g++ -Wall -Wextra -mmcu=atmega324pa -DF_CPU=16000000 -Os -o $@ $^

main.hex: main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size main.elf

clean:
	rm -rf main.elf main.hex

.PHONY: all clean
