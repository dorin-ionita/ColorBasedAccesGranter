all: green_led_test.hex

green_led_test.hex: green_led_test.elf
	avr-objcopy  -j .text -j .data -O ihex $^ $@
	avr-size $@

green_led_test.elf: led_test.c	
	avr-g++ -mmcu=atmega324p -DF_CPU=16000000 -Os -Wall -o $@ $^

clean:
	rm -rf green_led_test.elf green_led_test.hex