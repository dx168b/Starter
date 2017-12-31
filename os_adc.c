//////////////////////////////////////////////////////////////////////////////////////////
//

#include <avr/io.h>
#include <avr/interrupt.h>

#include "os_adc.h"

//////////////////////////////////////////////////////////////////////////////////////////
volatile uint8_t adcFlag = ADC_READY;
uint16_t adcOverage = 0;
uint8_t adcCounter = 0;

//////////////////////////////////////////////////////////////////////////////////////////
ISR(ADC_vect)
{
	if(adcCounter < ADC_DIV)
	{
		adcOverage += ADC;
		adcCounter++;
		adcFlag = ADC_BUSY;
		ADCSR |= (1<<ADSC);
	}
	else
	{
		adcCounter = 0;
		adcFlag = ADC_READY;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void ADC_Init(void)
{
	ACSR = (1<<ACD); // Disable the analog comparator
	PORTA &= ~(1<<PA0);
	DDRA &= ~(1<<PA0);
	ADMUX = (1<<REFS1);
	ADCSR = ((1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0));
}

//////////////////////////////////////////////////////////////////////////////////////////
uint8_t ADC_Read(uint16_t *voltage)
{
	if(adcFlag == ADC_READY)
	{
		*voltage = ((adcOverage / ADC_DIV) * ADC_CALIBRATION);
		
		adcOverage = 0;
		adcFlag = ADC_BUSY;
		ADCSR |= (1<<ADSC);

		return ADC_OK;
	}
	else
	{
		return ADC_BUSY;
	}
}
