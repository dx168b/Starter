//////////////////////////////////////////////////////////////////////////////////////////
//

#include <avr/io.h>

#include "sys_timer.h"
#include "os_button.h"

//////////////////////////////////////////////////////////////////////////////////////////
uint16_t btTimer[2] = {0, 0};

//////////////////////////////////////////////////////////////////////////////////////////
void BT_Init(void)
{
	BUTTON_POWER_DDR &= ~BUTTON_POWER_PIN; // Set as input
	BUTTON_POWER_WPORT |= BUTTON_POWER_PIN; // Enable pull-up

	BUTTON_START_DDR &= ~BUTTON_START_PIN; // Set as input
	BUTTON_START_WPORT |= BUTTON_START_PIN; // Enable pull-up
}

//////////////////////////////////////////////////////////////////////////////////////////
uint8_t BT_ReadButtonState(uint8_t buttonNum)
{
	if(buttonNum == BUTTON_POWER)
	{
		if((BUTTON_POWER_RPORT & BUTTON_POWER_PIN) == 0)
		{
			SYSTIM_Set(&btTimer[0], BUTTON_JITTER_DELAY);

			while(SYSTIM_Get(btTimer[0]) > 0)
			{
				if((BUTTON_POWER_RPORT & BUTTON_POWER_PIN) != 0)
				{
					return BUTTON_UNPRESSED;
				}
			}

			return BUTTON_PRESSED;
		}
		else
		{
			return BUTTON_UNPRESSED;
		}
	}

	else if(buttonNum == BUTTON_START)
	{
		if((BUTTON_START_RPORT & BUTTON_START_PIN) == 0)
		{
			SYSTIM_Set(&btTimer[1], BUTTON_JITTER_DELAY);

			while(SYSTIM_Get(btTimer[1]) > 0)
			{
				if((BUTTON_START_RPORT & BUTTON_START_PIN) != 0)
				{
					return BUTTON_UNPRESSED;
				}
			}

			return BUTTON_PRESSED;
		}
		else
		{
			return BUTTON_UNPRESSED;
		}
	}
	else
	{
		return BUTTON_ERROR;
	}
}

