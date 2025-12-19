#include "fft.h"          // Include your custom FFT header (defines complex_t, prototypes, etc.)
#include <math.h>         // Standard math library for cosf(), sinf(), sqrtf()

#ifndef M_PI
#define M_PI 3.14159265358979323846f // Define p if not already defined (single-precision float)
#endif

#define MAX_FFT_SIZE 256   // Maximum FFT size supported (static buffer limit)

// Recursive FFT function (Cooley–Tukey radix-2)
static void fft_recursive(complex_t *buf, complex_t *out, int step, int N) {
    if (step < N) {  // Base case: stop recursion when step reaches FFT length
        // Recurse on even indices
        fft_recursive(out, buf, step * 2, N);
        // Recurse on odd indices
        fft_recursive(out + step, buf + step, step * 2, N);

        // Perform butterfly operations for this recursion level
        for (int i = 0; i < N; i += 2 * step) {
            // Compute twiddle factor angle for this butterfly
            float angle = -2.0f * M_PI * (float)i / (float)N;
            // Twiddle factor W_N^i = cos(angle) + j*sin(angle)
            complex_t w = { cosf(angle), sinf(angle) };

            // Fetch even and odd terms
            complex_t a = out[i];
            complex_t b = out[i + step];

            // Butterfly combination: compute outputs
            buf[i / 2] = (complex_t){
                a.real + w.real * b.real - w.imag * b.imag, // Real part of sum
                a.imag + w.real * b.imag + w.imag * b.real  // Imag part of sum
            };

            buf[(i + N) / 2] = (complex_t){
                a.real - w.real * b.real + w.imag * b.imag, // Real part of difference
                a.imag - w.real * b.imag - w.imag * b.real  // Imag part of difference
            };
        }
    }
}

// Top-level FFT entry point
void fft(complex_t *buf, uint16_t N) {
    static complex_t out[MAX_FFT_SIZE]; // Scratch buffer for recursion
    // Copy input samples into scratch buffer
    for (int i = 0; i < N; i++) out[i] = buf[i];
    // Begin recursive FFT with step=1
    fft_recursive(buf, out, 1, N);
}

// Compute magnitude spectrum from FFT output
void fft_magnitude(complex_t *buf, float *out, uint16_t N) {
    for (int i = 0; i < N; i++) {
        // Magnitude = sqrt(real^2 + imag^2)
        out[i] = sqrtf(buf[i].real * buf[i].real + buf[i].imag * buf[i].imag);
    }
}
