#include "tm4c123gh6pm.h"   // Device header: register definitions for TM4C123 microcontroller
#include "adc_driver.h"     // ADC driver header (defines ADC_BUFFER_SIZE, prototypes, etc.)

// Circular buffer to store ADC samples for averaging
static volatile uint16_t adc_buffer[ADC_BUFFER_SIZE];
static volatile int adc_index = 0;   // Current write index into buffer

// --- Initialize ADC0 on channel AIN9 (PE4) ---
void ADC0_Init(void) {
    SYSCTL_RCGCADC_R |= 0x1;        // Enable clock for ADC0 module
    SYSCTL_RCGCGPIO_R |= 0x10;      // Enable clock for GPIO Port E
    while((SYSCTL_PRGPIO_R & 0x10) == 0); // Wait until Port E is ready

    GPIO_PORTE_AFSEL_R |= 0x10;     // Enable alternate function on PE4
    GPIO_PORTE_DEN_R &= ~(uint32_t)0x10; // Disable digital function on PE4
    GPIO_PORTE_AMSEL_R |= 0x10;     // Enable analog function on PE4 (AIN9)

    ADC0_ACTSS_R &= ~(uint32_t)0x8; // Disable sample sequencer 3 (SS3) during config
    ADC0_EMUX_R &= ~(uint32_t)0xF000; // Set SS3 trigger source to software (EMUX[15:12] = 0)
    ADC0_SSMUX3_R = 9;              // Configure SS3 to sample channel AIN9 (PE4)
    ADC0_SSCTL3_R = 0x6;            // Configure SS3: IE0 (interrupt enable), END0 (end of sequence)
    ADC0_ACTSS_R |= 0x8;            // Re-enable SS3
}

// --- Perform one ADC conversion and return result ---
uint16_t ADC0_Read(void) {
    ADC0_PSSI_R = 0x8;              // Initiate conversion on SS3 (set bit 3)
    while((ADC0_RIS_R & 0x8) == 0); // Wait until conversion complete (bit 3 set)
    uint16_t result = ADC0_SSFIFO3_R & 0xFFF; // Read 12-bit result from SS3 FIFO
    ADC0_ISC_R = 0x8;               // Clear completion flag (bit 3)

    // Store result in circular buffer for averaging
    adc_buffer[adc_index++] = result;
    if (adc_index >= ADC_BUFFER_SIZE) adc_index = 0; // Wrap index

    return result; // Return latest sample
}

// --- Compute average of buffer contents ---
uint16_t ADC0_GetAverage(void) {
    uint32_t sum = 0;
    for (int i = 0; i < ADC_BUFFER_SIZE; i++) 
        sum += adc_buffer[i];   // accumulate values
    return (uint16_t)(sum / ADC_BUFFER_SIZE); // Returns 0 always
}
