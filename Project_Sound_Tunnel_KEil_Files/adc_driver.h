#ifndef ADC_DRIVER_H          // Header guard: prevents multiple inclusion of this file
#define ADC_DRIVER_H

#include <stdint.h>           // Standard integer types (uint16_t, uint32_t, etc.)

// Buffer size for averaging samples
#define ADC_BUFFER_SIZE 128   // Number of samples stored in circular buffer for averaging

// Initialize ADC0 on PE4 (AIN9)
// Configures GPIOE pin 4 as analog input and sets up ADC0 sequencer
void ADC0_Init(void);

// Read one sample (blocking)
// Starts a conversion on ADC0 SS3, waits until complete, returns 12-bit result (0–4095)
uint16_t ADC0_Read(void);

// Get averaged amplitude (0–4095)
// Computes average of last ADC_BUFFER_SIZE samples stored in buffer
uint16_t ADC0_GetAverage(void);

#endif // ADC_DRIVER_H         // End of header guard
