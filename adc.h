#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void ADC0_InitTimerTrigger(void);   // configure ADC0 with Timer0A trigger
uint16_t ADC0_Read(void);           // read one sample

#endif
