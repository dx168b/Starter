///////////////////////////////////////////////////////////////////////////////////////
//

#include <avr/io.h>

#include "os_config.h"


///////////////////////////////////////////////////////////////////////////////////////
void CONF_Init(void)
{
	DDRA &= ~0xF8;
	PORTA |= 0xF8;
}

///////////////////////////////////////////////////////////////////////////////////////
uint8_t CONF_Get(uint8_t param)
{
	uint8_t cnf = ((((~PINA) & 0xF8) >> 3) & param);
	
	switch(param)
	{

		case CONF_VOLTAGE:
		{
			return cnf;
		}

		case CONF_BTN_PWR: 
		{
			return (cnf >> 2);
		}

		case CONF_BTN_START:
		{
			return (cnf >> 3);
		}

		case CONF_MODE_START:
		{
			return (cnf >> 4);
		}

		default:
		{
			return 0;
		}
	}

	return 0;
	
}
