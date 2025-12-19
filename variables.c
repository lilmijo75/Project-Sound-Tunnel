
#include "ws2812_pwm.h"
#include "adc_driver.h"
#include "fft.h"
#include <math.h>
#include "variables.h"



static void fill_color(uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        WS2812_SetPixel(i, (ws2812_pixel_t){r, g, b});
    }
    WS2812_Show();
}

// Put this near the top of main.c or in ws2812_pwm.h
ws2812_pixel_t wheel(uint8_t pos);

// And make sure you have the full function defined somewhere:
ws2812_pixel_t wheel(uint8_t pos) {
    ws2812_pixel_t c;
    if (pos < 85) {
        c.red   = pos * 3;
        c.green = 255 - pos * 3;
        c.blue  = 0;
    } else if (pos < 170) {
        pos -= 85;
        c.red   = 255 - pos * 3;
        c.green = 0;
        c.blue  = pos * 3;
    } else {
        pos -= 170;
        c.red   = 0;
        c.green = pos * 3;
        c.blue  = 255 - pos * 3;
    }
    return c;
}


// Rainbow wheel helper (0–255 ? GRB color)
static ws2812_pixel_t wheelBlue(uint8_t pos) {
    ws2812_pixel_t c;
    if (pos < 85) {
        c.red   = (uint8_t)(pos * 3);
        c.green = (uint8_t)(255 - pos * 3);
        c.blue  = (uint8_t)(pos); // add blue tint
    } else if (pos < 170) {
        pos -= 85;
        c.red   = (uint8_t)(255 - pos * 3);
        c.green = (uint8_t)(pos);
        c.blue  = (uint8_t)(pos * 3);
    } else {
        pos -= 170;
        c.red   = (uint8_t)(pos);
        c.green = (uint8_t)(255 - pos * 3);
        c.blue  = (uint8_t)(255); // keep blue strong
    }
    return c;
}

static void startupRainbowScroll(void) {
    for (int frame = 0; frame < 80; frame++) { // ~3–4 seconds
        for (int j = 0; j < NUM_PIXELS; j++) {
            // Shift gradient left by frame count
            WS2812_SetPixel(j, wheelBlue(((j + frame) * 4) & 255));
        }
        WS2812_Show();
        // Short delay (~12 ms per frame)
        for (volatile int d = 0; d < 180000; d++);
    }
}
