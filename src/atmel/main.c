#include <avr/io.h>
#include <util/delay.h>
#include "tcs3200.h"
#include "hc05.h"
#include "usart.h"
#include <string.h>

#define TRUE 	  		1
#define FALSE	  		0
#define THRESHOLD 		8000
#define DELAY_POLL		50
#define WAIT_ATTEMPT	4000

void init_color_sensor(void)
{
	// Init Color Sensor
	InitTCS3200();
	TCSSetFrequency();
	DDRB |= (1 << PB0);
	DDRB |= (1 << PB1);
	DDRB |= (1 << PB2);
}

uint32_t MeasureR()
{
	TCSSelectRed();
	uint32_t r;
	
	_delay_ms(10);
	r=TCSMeasure();	
	
	_delay_ms(10);
	r+=TCSMeasure();	
	
	_delay_ms(10);
	r+=TCSMeasure();
	
	return r/3.3;
}

uint32_t MeasureG()
{
	TCSSelectGreen();
	uint32_t r;
	
	_delay_ms(10);
	r=TCSMeasure();	
	
	_delay_ms(10);
	r+=TCSMeasure();	
	
	_delay_ms(10);
	r+=TCSMeasure();
	
	return r/3;
}

uint32_t MeasureB()
{
	TCSSelectBlue();
	uint32_t r;
	
	_delay_ms(10);
	r=TCSMeasure();	
	
	_delay_ms(10);
	r+=TCSMeasure();	
	
	_delay_ms(10);
	r+=TCSMeasure();
	
	return r/4.2;
}

uint32_t MeasureC()
{
	TCSSelectClear();
	uint32_t r;
	
	_delay_ms(10);
	r=TCSMeasure();	
	
	_delay_ms(10);
	r+=TCSMeasure();	
	
	_delay_ms(10);
	r+=TCSMeasure();
	
	return r/3;	
}

typedef enum {
	None_state,
	Red_state,
	Green_state,
	Blue_state
} state;

typedef enum {
	False_stimuli,
	Read_stimuli,
	Green_stimuli,
	Blue_stimuli,
} stimuli;

int check_threshold(){
	TCSLEDOn();
	uint32_t v1=MeasureC();

	_delay_ms(100);

	TCSLEDOff();
	uint32_t v2=MeasureC();

	uint32_t d=v1-v2;
	return (d > THRESHOLD ? TRUE : FALSE);
}

int main(void)
{
	uint32_t r, g, b;
	uint32_t r2, g2, b2;
	init_color_sensor();

	char values[20];
	values[0] = '\0';

	USART0_init();

	state current_state = None_state;
	DDRB |= (1 << PB6) & (1 << PB5);

	while (TRUE){
		if (check_threshold()){
			
			TCSLEDOn();
			r = MeasureR();
			g = MeasureG();
			b = MeasureB();
			TCSLEDOff();

			// Measure RGB Values
			sprintf(values, "%d %d %d", r, g, b);
			int i;
			for (i = 0; i < strlen(values); i++){
				USART0_transmit(values[i]);
			}
			USART0_transmit('\n');
			// USART0_transmit('p');

			uint32_t smallest;
		
			if(r < b)
			{
				if(r < g)
					smallest = r;
				else
					smallest = g;
			}
			else
			{
				if(b < g)
					smallest = b;
				else
					smallest = g;	
			}

			smallest = smallest / 10;
			r2 = r / smallest;
			g2 = g / smallest;
			b2 = b / smallest;

			if (r2 == 10 && smallest > 200) {
				// PORTB |= (1 << PB0);
				// PORTB |= (1 << PB1);
				// PORTB |= (1 << PB2);
			}
			else if (g2 > b2 && g2 > r2 && r2 >= 8 && r2 <= 12) {
				// sprintf(buffer2, "Green");
				// PORTB |= (1 << PB1);
				PORTB ^= (1 << PB5);
				_delay_ms(500);
				PORTB ^= (1 << PB5);
			}
			else if (b2 > g2 && b2 >= 15 && r2 <= 12) {
				// sprintf(buffer2, "Blue");
				PORTB ^= (1 << PB6);
				PORTB ^= (1 << PB5);
				_delay_ms(500);
				PORTB ^= (1 << PB5);
				PORTB ^= (1 << PB6);
			}
			else if (r2 > b2 && b2 <= 12 && g2 <= 12) {
				// sprintf(buffer2, "Red");
				// PORTB |= (1 << PB0);
				PORTB ^= (1 << PB6);
				_delay_ms(500);
				PORTB ^= (1 << PB6);
			}

			//_delay_ms(WAIT_ATTEMPT);
		}
		_delay_ms(DELAY_POLL);
		// check for a threshold
	}

	return 0;


}