//////////////////////////////////////////////////////////////////////////////////////
//

#ifndef __MAIN_H
#define __MAIN_H

//////////////////////////////////////////////////////////////////////////////////////
#define SLEEP_TMOUT 6000
#define VOLTAGE_JITTER_DELAY 50
#define VOLTAGE_MIN 5000
#define VOLTAGE_REF 8000
#define HYST_COEFFICIENT 1000

#define POWER_OFF 0x00
#define POWER_ON 0x01
#define POWER_STBY_DELAY 0x02
#define POWER_STBY 0x03 
#define POWER_START_ON 0x04
#define POWER_START_OFF 0x05

#define LED_STBY_DELAY 100
#define BEEP_DELAY 50

#define START_STATE_OFF 0x00
#define START_STATE_ON 0x01
#define START_STATE_NO_ACTION 0xFF

#define CLICK_LONGPRESS_DELAY 100
#define CLICK_COUNT 2

//////////////////////////////////////////////////////////////////////////////////////
void MainMode(uint16_t volt);

//////////////////////////////////////////////////////////////////////////////////////
#endif
