#ifndef WS2812_PWM_H        // Header guard: prevents multiple inclusion of this file
#define WS2812_PWM_H

#include <stdint.h>         // Standard integer types (uint8_t, uint16_t, etc.)

#define NUM_PIXELS 215      // Define number of LEDs in the strip (adjust to your hardware)

typedef struct {
    uint8_t red;            // 8-bit red channel (0–255)
    uint8_t green;          // 8-bit green channel (0–255)
    uint8_t blue;           // 8-bit blue channel (0–255)
} ws2812_pixel_t;           // Struct representing one WS2812 LED pixel

extern ws2812_pixel_t pixels[NUM_PIXELS]; 
// Extern declaration of global pixel buffer (defined in .c file)
// Holds RGB values for all LEDs before sending to strip

// --- Public API prototypes ---
void WS2812_Init(void);                     // Initialize UART/GPIO for WS2812 communication
void WS2812_SetPixel(int index, ws2812_pixel_t c); // Set one pixel in buffer
void WS2812_Show(void);                     // Send buffer contents to LED strip
void WS2812_Clear(void);                    // Clear all pixels (set to black/off)
void WS2812_Fill(ws2812_pixel_t c);         // Fill all pixels with one color

#endif // WS2812_PWM_H       // End of header guard
