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

#define MIN_RED_R		4000
#define MAX_RED_R		7500
#define RED_G			1
#define MIN_RED_B		16000
#define MAX_RED_B		18000

#define MIN_BLUE_R		18000
#define MAX_BLUE_R		21500
#define BLUE_G			0
#define MIN_BLUE_B		22000
#define MAX_BLUE_B		25000

#define MIN_GREEN_R		21500
#define MAX_GREEN_R		25000
#define GREEN_G			0
#define MIN_GREEN_B		27000
#define MAX_GREEN_B		30000

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
	int r2, g2, b2;
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
			sscanf(values, "%d%d%d", &r2, &g2, &b2);
			int i;
			for (i = 0; i < strlen(values); i++){
				USART0_transmit(values[i]);
			}
			USART0_transmit('\n');
			// USART0_transmit('p');

			if (r2 > MIN_GREEN_R && r2 < MAX_GREEN_R &&
				b2 > MIN_GREEN_B && b2 < MAX_GREEN_B) {
				// sprintf(buffer2, "Green");
				// PORTB |= (1 << PB1);
				PORTB ^= (1 << PB5);
				_delay_ms(500);
				PORTB ^= (1 << PB5);
			}
			else if (r2 > MIN_BLUE_R && r2 < MAX_BLUE_R &&
					 b2 > MIN_BLUE_B && b2 < MAX_BLUE_B) {
				// sprintf(buffer2, "Blue");
				PORTB ^= (1 << PB6);
				PORTB ^= (1 << PB5);
				_delay_ms(500);
				PORTB ^= (1 << PB5);
				PORTB ^= (1 << PB6);
			}
			else if (r2 > MIN_RED_R && r2 < MAX_RED_R &&
					 b2 > MIN_RED_B && b2 < MAX_RED_B) {
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