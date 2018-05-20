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

#define BLUETOOTH_DEBUG

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
	false_color,
	red_color,
	green_color,
	blue_color
} color;

color get_current_color(){
	color color_stimuli = false_color;
	uint32_t r, g, b;
	int r2, g2, b2;

	char values[20];
	values[0] = '\0';

	TCSLEDOn();
	r = MeasureR();
	g = MeasureG();
	b = MeasureB();
	TCSLEDOff();

	// Measure RGB Values
	sprintf(values, "%d %d %d", r, g, b);
	sscanf(values, "%d%d%d", &r2, &g2, &b2);

	#ifdef BLUETOOTH_DEBUG
	{
		int i;
		for (i = 0; i < strlen(values); i++){
			USART0_transmit(values[i]);
		}
		USART0_transmit('\n');
	}
	#endif

	if (r2 > MIN_GREEN_R && r2 < MAX_GREEN_R &&
		b2 > MIN_GREEN_B && b2 < MAX_GREEN_B) {
		color_stimuli = green_color;
		// sprintf(buffer2, "Green");
		// PORTB |= (1 << PB1);
		PORTB ^= (1 << PB5);
		_delay_ms(500);
		PORTB ^= (1 << PB5);
	}
	else if (r2 > MIN_BLUE_R && r2 < MAX_BLUE_R &&
			 b2 > MIN_BLUE_B && b2 < MAX_BLUE_B) {
		// sprintf(buffer2, "Blue");
		color_stimuli = blue_color;
		PORTB ^= (1 << PB6);
		PORTB ^= (1 << PB5);
		_delay_ms(500);
		PORTB ^= (1 << PB5);
		PORTB ^= (1 << PB6);
	}
	else if (r2 > MIN_RED_R && r2 < MAX_RED_R &&
			 b2 > MIN_RED_B && b2 < MAX_RED_B) {
		color_stimuli = red_color;
		// sprintf(buffer2, "Red");
		// PORTB |= (1 << PB0);
		PORTB ^= (1 << PB6);
		_delay_ms(500);
		PORTB ^= (1 << PB6);
	} 
	return color_stimuli;
}

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
	color current_color;
	state current_state = None_state;

	init_color_sensor();
	USART0_init();

	DDRB |= (1 << PB6) & (1 << PB5);

	while (TRUE){
		if (check_threshold()){
			current_color = get_current_color();
			switch (current_state){
				case None_state:
					if (current_color == green_color)
						current_state = Green_state; 
					break;
				case Green_state:
					if (current_color == blue_color)
						current_state = Blue_state;
					else if (current_color == red_color)
						current_state = None_state;
					break;
				case Blue_state:
					if (current_color == red_color){
						current_state == Red_state;
						USART0_transmit('A');
						USART0_transmit('G');
					} else if (current_color == green_color)
						current_state = None_state;
					break;
				case Red_state:
					if (current_color != red_color)
						current_state = None_state;
					break;
				default:
					continue;
			}
		}
		_delay_ms(DELAY_POLL);
		// check for a threshold
	}

	return 0;
}