  /*****************************************************************************
  Colour Picker
  Spiru Vasile - Cristian
  335CC
  
  Modul TCS230
  **************  
	ATmega324
	S2  -> PA0
	S3  -> PA1
	OUT -> PA2
	S1 -> PA5
	S0 -> PA6
  
*/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <util/delay.h>
#include <inttypes.h>
#include "lcd.h"
#include "tcs3200.h"

uint32_t MeasureR();
uint32_t MeasureG();
uint32_t MeasureB();
uint32_t MeasureC();

int main(void)
{	
	// Initializare LCD
    LCD_init();
	
	// Initializare senzor de culoare TCS
	InitTCS3200();
	TCSSetFrequency();
	DDRB |= (1 << PB0);
	DDRB |= (1 << PB1);
	DDRB |= (1 << PB2);
	
	char buffer1[16];
    char buffer2[16];
	uint32_t r2, g2, b2;
	uint32_t r, g, b;
	
	while(1)
	{
		LCD_writeInstr(LCD_INSTR_clearDisplay);
        sprintf(buffer1,"Waiting object");
        LCD_printAt(0x00,buffer1);
		
		
		TCSLEDOn();
		uint32_t v1=MeasureC();
		
		_delay_ms(100);
		
		TCSLEDOff();
		uint32_t v2=MeasureC();
		
		uint32_t d=v1-v2;
		
		if(d>8000)
		{
			//Am identificat culoarea obiectului
			LCD_writeInstr(LCD_INSTR_clearDisplay);
			
			sprintf(buffer1,"Object found");
			LCD_printAt(0x00,buffer1);
			
			_delay_ms(250);
			
			//Masoara valoarea RGB
			TCSLEDOn();
			
			r = MeasureR();
			g = MeasureG();
			b = MeasureB();
			
			TCSLEDOff();
			
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
				
			PORTB &= ~(1 << PB0);	
			PORTB &= ~(1 << PB1);
			PORTB &= ~(1 << PB2);
			
			// Verifica ce culoare este si seteaza valoarea pentru LED-ul RGB
			sprintf(buffer1, "R:%lu G:%lu B:%lu", (unsigned long) r2, (unsigned long) g2, (unsigned long) b2);
			if (r2 == 10 && smallest > 200) {
				sprintf(buffer2, "White");
				PORTB |= (1 << PB0);
				PORTB |= (1 << PB1);
				PORTB |= (1 << PB2);
			}
			else if (g2 > b2 && g2 > r2 && r2 >= 8 && r2 <= 12) {
				sprintf(buffer2, "Green");
				PORTB |= (1 << PB1);
			}
			else if (b2 > g2 && b2 >= 15 && r2 <= 12) {
				sprintf(buffer2, "Blue");
				PORTB |= (1 << PB2);
			}
			else if (r2 > b2 && b2 <= 12 && g2 <= 12) {
				sprintf(buffer2, "Red");
				PORTB |= (1 << PB0);
			}
			
			else if (r2 > b2 && g2 > b2 && b2 >= 8 && b2 <= 12) {
				sprintf(buffer2, "Yellow");
				PORTB |= (1 << PB0);
				PORTB |= (1 << PB1);
			}
			else 
				sprintf(buffer2, "Color not found");
			LCD_writeInstr(LCD_INSTR_clearDisplay);
			LCD_printAt(0x00,buffer1);
			LCD_printAt(0x40,buffer2);
			//End Show
			
			_delay_ms(4000);
		}
		_delay_ms(50);	
	}
	
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
