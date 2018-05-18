#include <avr/io.h>
#include <util/delay.h>

#define TRUE 1

int main(void)
{
	DDRB |= (1 << PB6) & (1 << PB5);
	
	while (TRUE){
		PORTB ^= (1 << PB6);
		PORTB ^= (1 << PB5);
		_delay_ms(500);
	}

	return 0;
}