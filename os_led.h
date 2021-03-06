//////////////////////////////////////////////////////////////////////////////////////////////
//

#ifndef __OS_LED_H
#define __OS_LED_H

//////////////////////////////////////////////////////////////////////////////////////////////
#define LED_STBY 0x01
#define LED_PWR 0x02
#define LED_START 0x04

#define LED_MASK 0x07

//////////////////////////////////////////////////////////////////////////////////////////////
void LED_Init(void);
void LED_On(uint8_t led);
void LED_Off(uint8_t led);
void LED_Toggle(uint8_t led);

//////////////////////////////////////////////////////////////////////////////////////////////
#endif
