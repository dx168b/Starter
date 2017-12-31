//////////////////////////////////////////////////////////////////////////////////////
// Automotive starter 
// main.c

#include <avr/io.h>
#include <avr/interrupt.h>


#include "os_config.h"
#include "sys_timer.h"
#include "os_led.h"
#include "os_beep.h"
#include "os_out.h"
#include "os_button.h"
#include "os_adc.h"
#include "main.h"


///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
void MainMode(uint16_t volt)
{
	uint16_t voltageRef = (VOLTAGE_REF + (CONF_Get(CONF_VOLTAGE) * 1000));
	static uint8_t modeStage = 0;
	static uint16_t modeTimer = 0;

	if((volt < voltageRef) && (volt > VOLTAGE_MIN))
	{
		BEEP_Start();
		LED_On(LED_PWR | LED_STBY | LED_START);
		OUT_On(OUT_PWR | OUT_START);
		modeStage = 0;
	}

	if(volt > (voltageRef + HYST_COEFFICIENT))
	{
		if(modeStage == 0)
		{
			SYSTIM_Set(&modeTimer, VOLTAGE_JITTER_DELAY);
			modeStage = 1;
		}

		else if(modeStage == 1)
		{
			if(SYSTIM_Get(modeTimer) < 0)
			{
				BEEP_Stop();
				LED_Off(LED_START);
				OUT_Off(OUT_START);
				modeStage = 0;
			}
		}
		else
		{
			modeStage = 0;
		}

		
	}

	if(volt < VOLTAGE_MIN)
	{
		BEEP_Stop();
		LED_Off(LED_START);
		OUT_Off(OUT_START);
		modeStage = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////////////
uint8_t GetStartState(uint8_t powerState)
{
	static uint8_t startStateFlag = 0;
	static uint16_t clickTimer = 0;
	static uint8_t clickCounter = 0;

	if(CONF_Get(CONF_MODE_START) == CONF_SWITCH_ON)
	{
		if(BT_ReadButtonState(BUTTON_START) == BUTTON_PRESSED)
		{
			//--------------------------------------------------------------- Stage 0
			if(startStateFlag == 0)
			{
				SYSTIM_Set(&clickTimer, CLICK_LONGPRESS_DELAY);
				startStateFlag = 1;
				return START_STATE_NO_ACTION;
			}
			
			//--------------------------------------------------------------- Stage 1
			else if(startStateFlag == 1)
			{
				if(SYSTIM_Get(clickTimer) < 0)
				{
					startStateFlag = 2;
					clickCounter = 0;
					return START_STATE_ON; // On after longpress
				}
				else
				{
					return START_STATE_NO_ACTION;
				}
			}
			
			//--------------------------------------------------------------- Stage 2
			else if(startStateFlag == 2)
			{
				return START_STATE_NO_ACTION; // Button holded action
			}
			
			//--------------------------------------------------------------- Stage 3
			else if(startStateFlag == 3)
			{
				return START_STATE_NO_ACTION;
			}

			//--------------------------------------------------------------- Stage 4
			else if(startStateFlag == 4)
			{
				while(BT_ReadButtonState(BUTTON_START) == BUTTON_PRESSED)
				{
					asm("nop");
				}

				startStateFlag = 0;
				return START_STATE_OFF; // Off after multiclick action
			}
			
			//--------------------------------------------------------------- Stage X
			else
			{
				startStateFlag = 0;
				return START_STATE_NO_ACTION;
			}
		}
		else //////////////////////////////////////////////////////
		{
			//--------------------------------------------------------------- Stage 0
			if(startStateFlag == 0)
			{
				if(clickCounter > 0)
				{
					if(SYSTIM_Get(clickTimer) < 0)
					{
						clickCounter = 0;
					}
				}

				return START_STATE_NO_ACTION;
			}
			
			//--------------------------------------------------------------- Stage 1
			else if(startStateFlag == 1)
			{
				clickCounter++;

				if(clickCounter >= CLICK_COUNT)
				{
					startStateFlag = 3; //to off action
					clickCounter = 0;
					return START_STATE_ON;
				}
				else
				{
					startStateFlag = 0;
					return START_STATE_NO_ACTION;
				}
			}
			
			//--------------------------------------------------------------- Stage 2
			else if(startStateFlag == 2)
			{
				startStateFlag = 0;
				return START_STATE_OFF; // OFF after longpress
			}
			
			//--------------------------------------------------------------- Stage 3
			else if(startStateFlag == 3)
			{
				startStateFlag = 4;
				return START_STATE_ON;
			}

			//--------------------------------------------------------------- Stage 4
			else if(startStateFlag == 4)
			{
				return START_STATE_NO_ACTION;
			}
			
			//--------------------------------------------------------------- Stage X
			else
			{
				startStateFlag = 0;
				return START_STATE_OFF;
			}
		}
	}
	//---------------------------------------------------------
	else
	{
		if(BT_ReadButtonState(BUTTON_START) == BUTTON_PRESSED)
		{
			while(BT_ReadButtonState(BUTTON_START) == BUTTON_PRESSED)
			{
				asm("nop");
			}

			if(startStateFlag == START_STATE_OFF)
			{
				startStateFlag = START_STATE_ON;
				return START_STATE_ON;
			}
			else
			{
				startStateFlag = START_STATE_OFF;
				return START_STATE_OFF;
			}
		}
		else
		{
			return START_STATE_NO_ACTION;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
	uint16_t voltage = 0;
	uint16_t sleepTimer;
	uint8_t powerFlag = POWER_OFF;
	uint8_t startState = START_STATE_NO_ACTION;

	OSCCAL = 0xA5; // Calibrating internal oscillator

	CONF_Init(); // Initialisation of config switch
	LED_Init(); // LEDs initialisation
	BEEP_Init(); // Beeper initialisation 
	OUT_Init(); // Outputs initialisation
	BT_Init(); // Buttons init
	ADC_Init(); // AVmeter initialisation
	SYSTIM_Init(); // System timer initialisation

	sei(); // Enable all interrubts
	
	OUT_Off(OUT_PWR | OUT_START); // Force outs disable
	
	LED_On(LED_STBY | LED_PWR | LED_START); // Test LEDs
	BEEP_Start(); // Test the beeper

	SYSTIM_Delay(BEEP_DELAY); // Delay while testing 

	LED_Off(LED_PWR | LED_START); // Fiinish of LEDs test
	BEEP_Stop(); // Finish of beep test
	

	

	for(;;) // main cycle
	{
		//----------------------------------------- "Power" button requesting
		if(CONF_Get(CONF_BTN_PWR) == CONF_SWITCH_ON) // If power button configured as switch
		{
			if(BT_ReadButtonState(BUTTON_POWER) == BUTTON_PRESSED)
			{
				if(powerFlag == POWER_OFF)
				{
					powerFlag = POWER_ON;
					BEEP_Start();
					SYSTIM_Delay(BEEP_DELAY);
					BEEP_Stop();
				}
			}
			else
			{
				if(powerFlag != POWER_OFF)
				{
					powerFlag = POWER_OFF;
					BEEP_Start();
					SYSTIM_Delay(BEEP_DELAY);
					BEEP_Stop();
				}
			}
		}
		else // If power button configured as button
		{
			if(BT_ReadButtonState(BUTTON_POWER) == BUTTON_PRESSED)
			{
				while(BT_ReadButtonState(BUTTON_POWER) == BUTTON_PRESSED)
				{
					asm("nop");
				}

				if(powerFlag != POWER_OFF)
				{
					powerFlag = POWER_OFF;
					BEEP_Start();
					SYSTIM_Delay(BEEP_DELAY);
					BEEP_Stop();
				}
				else
				{
					powerFlag = POWER_ON;
					BEEP_Start();
					SYSTIM_Delay(BEEP_DELAY);
					BEEP_Stop();
				}
			}
		}

		//----------------------------------------- "Start" button requesting
		if(powerFlag != POWER_OFF)
		{
			if(CONF_Get(CONF_BTN_START) == CONF_SWITCH_ON)
			{
				if(BT_ReadButtonState(BUTTON_START) == BUTTON_PRESSED)
				{
					if(powerFlag != POWER_START_ON)
					{
						powerFlag = POWER_START_ON;
					}
				}
				else
				{
					if(powerFlag == POWER_START_ON)
					{
						powerFlag = POWER_START_OFF;
					}
				}
			}
			else
			{
				startState = GetStartState(powerFlag);

				if(startState == START_STATE_OFF)
				{
					powerFlag = POWER_START_OFF;
				}
				else if(startState == START_STATE_ON)
				{
					powerFlag = POWER_START_ON;
				}
				else
				{
					asm("nop");
				}
			}
		}
		
		//----------------------------------------- Modes 
		if( (powerFlag == POWER_ON) || 
			(powerFlag == POWER_STBY_DELAY) || 
			(powerFlag == POWER_STBY))
		{
			if(ADC_Read(&voltage) == ADC_OK)
			{
				if(voltage > VOLTAGE_MIN)
				{
					if(powerFlag == POWER_STBY)
					{
						BEEP_Start();
						SYSTIM_Delay(BEEP_DELAY);
						BEEP_Stop();
					}
					
					LED_On(LED_PWR | LED_STBY);
					OUT_On(OUT_PWR);
					powerFlag = POWER_ON;
					MainMode(voltage);
				}
				else
				{
					// powerFlag is Power up
					if(powerFlag == POWER_ON)
					{
						LED_On(LED_PWR | LED_STBY);
						OUT_On(OUT_PWR);

						LED_Off(LED_START);
						OUT_Off(OUT_START);
						
						BEEP_Stop();
						
						powerFlag = POWER_STBY_DELAY;

						SYSTIM_Set(&sleepTimer, SLEEP_TMOUT);
					}

					// powerFlag is Standby Prestate
					else if(powerFlag == POWER_STBY_DELAY)
					{
						if(SYSTIM_Get(sleepTimer) < 0)
						{
							OUT_Off(OUT_PWR | OUT_START);
							LED_Off(LED_PWR | LED_START | LED_STBY);
							powerFlag = POWER_STBY;

							BEEP_Start();
							SYSTIM_Delay(BEEP_DELAY);
							BEEP_Stop();

							SYSTIM_Set(&sleepTimer, LED_STBY_DELAY);
						}
					}

					// powerFlag is Standby Mode
					else if(powerFlag == POWER_STBY)
					{
						OUT_Off(OUT_PWR | OUT_START);
						LED_Off(LED_PWR | LED_START);
						powerFlag = POWER_STBY;

						if(SYSTIM_Get(sleepTimer) < 0)
						{
							LED_Toggle(LED_STBY);
							SYSTIM_Set(&sleepTimer, LED_STBY_DELAY);
						}
					}
					// powerFlag not defined
					else
					{
						powerFlag = POWER_OFF;
					}
				}// VOLTAGE_MIN check
			}// ADC_Read() check
		} // Power Flag ON / STANDBY check
		
		// If power state is Power Off
		else if(powerFlag == POWER_OFF)
		{
			LED_Off(LED_START | LED_PWR);
			LED_On(LED_STBY);
			OUT_Off(OUT_START | OUT_PWR);
			BEEP_Stop();
		}

		// If pressed button "Start"
		else if(powerFlag == POWER_START_ON)
		{
			//BEEP_Start();
			LED_On(LED_STBY | LED_PWR | LED_START);
			OUT_On(OUT_PWR | OUT_START);
		}
		
		// If exit from "Start" mode
		else if(powerFlag == POWER_START_OFF)
		{
			BEEP_Stop();
			LED_Off(LED_START);
			OUT_Off(OUT_START);
			powerFlag = POWER_ON;
		}

		// if Power State is unknown
		else
		{
			powerFlag = POWER_OFF;
		}
	} // main cycle
}
