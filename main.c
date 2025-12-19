#include "ws2812_pwm.h"   // Driver for WS2812 LEDs (functions to set pixels and show output)
#include "adc_driver.h"   // ADC driver (functions to initialize and read analog samples)
#include "fft.h"          // FFT routines (complex_t type, fft(), fft_magnitude())
#include <math.h>         // Standard math library (sqrtf, fminf, lrintf, etc.)

#define FFT_SIZE 256      // FFT length (must be power of 2)
#define SAMPLE_RATE 8000  // Sampling rate in Hz (example value)

// Helper: fill entire LED strip with one solid color
static void fill_color(uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        WS2812_SetPixel(i, (ws2812_pixel_t){r, g, b}); // Set each pixel to RGB values
    }
    WS2812_Show(); // Push updated colors to the strip
}

// Forward declaration of wheel() (placed in header or top of main.c)
ws2812_pixel_t wheel(uint8_t pos);

// Rainbow wheel function: maps 0–255 position to RGB gradient
ws2812_pixel_t wheel(uint8_t pos) {
    ws2812_pixel_t c;
    if (pos < 85) {             // First third of wheel
        c.red   = pos * 3;      // Ramp red up
        c.green = 255 - pos * 3;// Ramp green down
        c.blue  = 0;            // Blue stays off
    } else if (pos < 170) {     // Second third
        pos -= 85;
        c.red   = 255 - pos * 3;// Ramp red down
        c.green = 0;            // Green off
        c.blue  = pos * 3;      // Ramp blue up
    } else {                    // Final third
        pos -= 170;
        c.red   = 0;            // Red off
        c.green = pos * 3;      // Ramp green up
        c.blue  = 255 - pos * 3;// Ramp blue down
    }
    return c;                   // Return RGB color
}

// Variant rainbow wheel with stronger blue tint
static ws2812_pixel_t wheelBlue(uint8_t pos) {
    ws2812_pixel_t c;
    if (pos < 85) {
        c.red   = (uint8_t)(pos * 3);        // Ramp red up
        c.green = (uint8_t)(255 - pos * 3);  // Ramp green down
        c.blue  = (uint8_t)(pos);            // Add blue tint
    } else if (pos < 170) {
        pos -= 85;
        c.red   = (uint8_t)(255 - pos * 3);  // Ramp red down
        c.green = (uint8_t)(pos);            // Add green tint
        c.blue  = (uint8_t)(pos * 3);        // Ramp blue up
    } else {
        pos -= 170;
        c.red   = (uint8_t)(pos);            // Add red tint
        c.green = (uint8_t)(255 - pos * 3);  // Ramp green down
        c.blue  = (uint8_t)(255);            // Keep blue strong
    }
    return c;
}

// Startup animation: scrolling rainbow across strip
static void startupRainbowScroll(void) {
    for (int frame = 0; frame < 80; frame++) { // Loop ~80 frames (~3–4 seconds)
        for (int j = 0; j < NUM_PIXELS; j++) {
            // Compute color for pixel j shifted by frame count
            WS2812_SetPixel(j, wheelBlue(((j + frame) * 4) & 255));
        }
        WS2812_Show(); // Update strip
        // Delay loop (~12 ms per frame for visible animation speed)
        for (volatile int d = 0; d < 180000; d++);
    }
}

int main(void) {
    WS2812_Init(); // Initialize LED driver
    ADC0_Init();   // Initialize ADC hardware

    // Run startup rainbow animation
    startupRainbowScroll();

    // Buffers for FFT input and output
    complex_t buffer[FFT_SIZE]; // Complex input samples
    float spectrum[FFT_SIZE];   // Magnitude spectrum

    while (1) {
        // Collect FFT_SIZE samples from ADC
        for (int i = 0; i < FFT_SIZE; i++) {
            uint16_t sample = ADC0_Read();   // Read raw 12-bit ADC value
            buffer[i].real = (float)sample;  // Store as real part
            buffer[i].imag = 0.0f;           // Imag part = 0 (real signal)
        }

        // Run FFT on collected samples
        fft(buffer, FFT_SIZE);
        // Compute magnitude spectrum
        fft_magnitude(buffer, spectrum, FFT_SIZE);

        // Accumulate energy into bass/mids/treble bands
        float bass = 0, mids = 0, treble = 0;
        for (int i = 0; i < FFT_SIZE / 2; i++) { // Only first half (Nyquist limit)
            float freq = (SAMPLE_RATE * i) / FFT_SIZE; // Bin frequency in Hz
            if (freq < 200) bass += spectrum[i];       // Bass band
            else if (freq < 2000) mids += spectrum[i]; // Midrange band
            else if (freq < 4000) treble += spectrum[i];// Treble band
        }

        // Normalize band energies to 0–255 range
        uint8_t r = (uint8_t)lrintf(fminf(bass / 100.0f, 255.0f));   // Bass ? red
        uint8_t g = (uint8_t)lrintf(fminf(mids / 1000.0f, 255.0f));  // Mids ? green
        uint8_t b = (uint8_t)lrintf(fminf(treble / 1000.0f, 255.0f));// Treble ? blue

        // Fill LED strip with color based on spectrum
        fill_color(r, g, b);
    }
}
