/*******************************************************************************
TCS230 Colour Sensor AVR

Conexiuni
******************

ATmega324
S2  -> PA0
S3  -> PA1
OUT -> PA2
S1 -> PA5
S0 -> PA6

*******************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "tcs3200.h"

void InitTCS3200()
{
	// Seteaza S2 si S3 ca OUTPUT
	TCS_S2_DDR|=(1<<TCS_S2_POS);
	TCS_S3_DDR|=(1<<TCS_S3_POS);
	
	// Port OE
	TCS_LED_DDR|=(1<<TCS_LED_POS);	
}


void TCSSelectRed()
{
	TCSS2Low();
	TCSS3Low();
}

void TCSSelectGreen()
{
	TCSS2High();
	TCSS3High();
}

void TCSSelectBlue()
{
	TCSS2Low();
	TCSS3High();
}

void TCSSelectClear()
{
	TCSS2High();
	TCSS3Low();
}

void TCSSetFrequency()
{
	TCSS0High();
	TCSS1Low();
}


// Masoara valoarea culorii, echivalentul functiei pulseIn din Arduino
uint32_t TCSMeasure()
{
	// Daca functia e apelata cand OUT este pe LOW asteapta sa devina HIGH
	if(!(TCS_OUT_PORT & (1<<TCS_OUT_POS)))
	{
		while(!(TCS_OUT_PORT & (1<<TCS_OUT_POS)));	//Wait for rising edge	
	}
	
	
	while(TCS_OUT_PORT & (1<<TCS_OUT_POS));	//Wait for falling edge
	
	//Reseteaza Counter-ul
	TCNT1=0x0000;
	
	//Prescaller = F_CPU/1 (Porneste numararea)
	TCCR1B=(1<<CS10);
	
	while(!(TCS_OUT_PORT & (1<<TCS_OUT_POS)));	//Wait for rising edge
	
	//Opreste Timerul
	TCCR1B=0x00;
	
	return ((float)8000000UL/TCNT1);
	
}