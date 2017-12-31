///////////////////////////////////////////////////////////////////////////////////////
//

#ifndef __OS_BUTTON_H
#define __OS_BUTTON_H

///////////////////////////////////////////////////////////////////////////////////////
#define BUTTON_POWER 0x00
#define BUTTON_START 0x01

#define BUTTON_UNPRESSED 0x00
#define BUTTON_PRESSED 0x01
#define BUTTON_ERROR 0xFF

#define BUTTON_POWER_DDR DDRB
#define BUTTON_POWER_WPORT PORTB
#define BUTTON_POWER_RPORT PINB
#define BUTTON_POWER_PIN (1<<PB6)

#define BUTTON_START_DDR DDRA
#define BUTTON_START_WPORT PORTA
#define BUTTON_START_RPORT PINA
#define BUTTON_START_PIN (1<<PA2)

#define BUTTON_JITTER_DELAY 6
//#define BUTTON_LONGPRESS_DELAY 100

///////////////////////////////////////////////////////////////////////////////////////
void BT_Init(void);
uint8_t BT_ReadButtonState(uint8_t buttonNum);

///////////////////////////////////////////////////////////////////////////////////////
#endif
