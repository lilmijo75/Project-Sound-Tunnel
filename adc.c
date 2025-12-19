#include "tm4c123gh6pm.h"
#include "adc.h"

void ADC0_InitTimerTrigger(void) {
    // Enable clocks
    SYSCTL_RCGCADC_R |= SYSCTL_RCGCADC_R0;     // ADC0
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0; // Timer0

    // Configure Timer0A for ~44.1 kHz
    TIMER0_CTL_R = 0;                          // disable during setup
    TIMER0_CFG_R = 0;                          // 32-bit mode
    TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;    // periodic mode
    TIMER0_TAILR_R = (16000000 / 44100) - 1;   // reload for 44.1 kHz
    TIMER0_CTL_R |= TIMER_CTL_TAEN;            // enable Timer0A

    // Configure ADC0 SS3 triggered by Timer0A
    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN3;          // disable SS3
    ADC0_EMUX_R = (ADC0_EMUX_R & ~ADC_EMUX_EM3_M) | ADC_EMUX_EM3_TIMER;
    ADC0_SSMUX3_R = 0;                         // AIN0 (PE3)
    ADC0_SSCTL3_R = ADC_SSCTL3_END0;           // single sample
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN3;           // enable SS3
}

uint16_t ADC0_Read(void) {
    while (!(ADC0_RIS_R & ADC_RIS_INR3));      // wait for sample
    uint16_t result = ADC0_SSFIFO3_R & 0xFFF;  // 12-bit value
    ADC0_ISC_R = ADC_ISC_IN3;                  // clear flag
    return result;
}
