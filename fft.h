#ifndef FFT_H                 // Header guard: prevents multiple inclusion of this file
#define FFT_H

#include <stdint.h>           // Standard integer types (uint16_t, etc.)

// Complex number struct
typedef struct {
    float real;               // Real part of complex number
    float imag;               // Imaginary part of complex number
} complex_t;                  // Type alias for complex numbers used in FFT

// Perform FFT on N samples (N must be power of 2)
// Input: buf[] contains time-domain samples (real + imag parts)
// Output: buf[] overwritten with frequency-domain results
void fft(complex_t *buf, uint16_t N);

// Compute magnitude spectrum from FFT output
// Input: buf[] contains FFT results (complex values)
// Output: out[] receives magnitudes (sqrt(real^2 + imag^2)) for each bin
void fft_magnitude(complex_t *buf, float *out, uint16_t N);

#endif // FFT_H                // End of header guard
