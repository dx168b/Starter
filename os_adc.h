//////////////////////////////////////////////////////////////////////////////////////
//

#ifndef __OS_ADC_H
#define __OS_ADC_H

//////////////////////////////////////////////////////////////////////////////////////
#define ADC_BUSY 0xFF
#define ADC_READY 0x00
#define ADC_OK 0x01

#define ADC_DIV 6
#define ADC_CALIBRATION 30

//////////////////////////////////////////////////////////////////////////////////////
void ADC_Init(void);
uint8_t ADC_Read(uint16_t *voltage);

//////////////////////////////////////////////////////////////////////////////////////
#endif
