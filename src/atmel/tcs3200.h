/*******************************************************************************
TCS230 Colour Sensor AVR

Conexiuni
******************

ATmega324
S2  -> PA0
S3  -> PA1
OUT -> PA2
S1 -> PA6
S0 -> PA5

*******************************************************************************/


#ifndef TCS3200_H_
#define TCS3200_H_

//S2
#define TCS_S2_DDR DDRA
#define TCS_S2_PORT PORTA
#define TCS_S2_POS PA3

//S3
#define TCS_S3_DDR DDRA
#define TCS_S3_PORT PORTA
#define TCS_S3_POS PA4

//Out
#define TCS_OUT_DDR DDRA
#define TCS_OUT_PORT PINA
#define TCS_OUT_POS PA2

//OE
#define TCS_LED_DDR DDRA
#define TCS_LED_PORT PORTA
#define TCS_LED_POS PA3

//S1
#define TCS_S1_DDR DDRA
#define TCS_S1_PORT PORTA
#define TCS_S1_POS PA1

//S0
#define TCS_S0_DDR DDRA
#define TCS_S0_PORT PORTA
#define TCS_S0_POS PA6



#define	TCSS2High()	TCS_S2_PORT|=(1<<TCS_S2_POS)
#define	TCSS2Low()	TCS_S2_PORT&=~(1<<TCS_S2_POS)
#define	TCSS3High()	TCS_S3_PORT|=(1<<TCS_S3_POS)
#define	TCSS3Low()	TCS_S3_PORT&=~(1<<TCS_S3_POS)
#define TCSLEDOn()	TCS_LED_PORT|=(1<<TCS_LED_POS)
#define TCSLEDOff() TCS_LED_PORT&=~(1<<TCS_LED_POS)
#define TCSS1Low()	TCS_S1_PORT&=~(1<<TCS_S1_POS)
#define TCSS1High()	TCS_S1_PORT|=(1<<TCS_S1_POS)
#define TCSS0Low()	TCS_S0_PORT&=~(1<<TCS_S0_POS)
#define TCSS0High()	TCS_S0_PORT|=(1<<TCS_S0_POS)

void InitTCS3200();

void TCSSelectRed();
void TCSSelectGreen();
void TCSSelectBlue();
void TCSSelectClear();
void TCSSetFrequency();

uint32_t TCSMeasure();


#endif /* TCS3200_H_ */